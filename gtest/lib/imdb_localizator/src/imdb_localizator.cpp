#include "imdb_localizator.hpp"
#include <fstream>
#include <algorithm>
#include <iostream>
#include <unordered_map>

#include "utils.hpp"
#include "errors.hpp"

using namespace std::string_literals;

constexpr std::string_view DEFAULT_LOCALIZATION = "RU";
const std::string NO_VALUE = "\\N";

const std::string TITLE_FIELD = "title";
const std::string TITLE_ID_FIELD = "titleId";
const std::string REGION_FIELD = "region";
const std::string IS_ORIGINAL_TITLE_FIELD = "isOriginalTitle";

using Strings = std::vector<std::string>;

namespace {

std::optional<size_t> FindColumnIndex(const Strings& columns, const std::string& columnName) {
    auto it = std::find(columns.begin(), columns.end(), columnName);
    if (it == columns.end()) {
        return std::nullopt;
    }
    return std::distance(columns.begin(), it);
}

bool parseBool(const std::string& str) {
    if (str == "0" || str == NO_VALUE) {
        return false;
    }
    if (str == "1") {
        return true;
    }
    throw ConversionError{"Bool must be 0 or 1", str};
}

FieldsIndices GetFieldsIndicesFromHeader(const std::string& header) {
    FieldsIndices result;
    Strings allColumns = SplitString(header);
    const Strings columns{ TITLE_ID_FIELD,
                           TITLE_FIELD,
                           REGION_FIELD,
                           IS_ORIGINAL_TITLE_FIELD };

    for (const auto & column : columns) {
        std::optional<size_t> index = FindColumnIndex(allColumns, column);
        if (!index) {
            throw HeaderError{"Column not found in header", column};
        }
        auto [it, is_new] = result.emplace(column, *index);
        if (!is_new) {
            throw HeaderError{"Duplicated column in header", column};
        }
    }
    return result;
}

std::string GetFieldByIndex(const Strings& columns, size_t index) {
    if (index >= columns.size()) {
        throw ColumnError{"Not enough columns", index + 1};
    }
    return columns[index];
}

std::string GetFieldByName(const Strings& columns,
                           const FieldsIndices& fieldsIndices,
                           const std::string& fieldName) {
    try {
        return GetFieldByIndex(columns, fieldsIndices.at(fieldName));
    } catch (ColumnError& e) {
        std::string message ="Failed to parse "s + fieldName + ": " + e.what();
        message += " excpected index: " + std::to_string(e.getColumnNumber());
        throw Error{message};
    }
    throw Error{"Error while parsing field " + fieldName};
}

} // namespace

ImdbLocalizator::ImdbLocalizator(IDataSourceUptr dataSource)
    : dataSource_{std::move(dataSource)},
      localization_{DEFAULT_LOCALIZATION}
{
    if (!dataSource_) {
        throw Error{"Null pointer as dataSource"};
    }

    std::optional<std::string> header = dataSource_->getNextLine();
    if (!header || header->empty()) {
        throw Error{"Empty input"};
    }

    std::unordered_map<std::string, size_t> columnIndices = GetFieldsIndicesFromHeader(*header);
    std::optional<std::string> tmp = dataSource_->getNextLine();
    while (tmp) {
        ParseTitleFromLine(*tmp, columnIndices);
        tmp = dataSource_->getNextLine();
    }
}

void ImdbLocalizator::ParseTitleFromLine(const std::string& str,
                                         const FieldsIndices& fieldsIndices) {
    Strings columns = SplitString(str);

    std::string titleId = GetFieldByName(columns, fieldsIndices, TITLE_ID_FIELD);
    std::string title = GetFieldByName(columns, fieldsIndices, TITLE_FIELD);
    std::string region = GetFieldByName(columns, fieldsIndices, REGION_FIELD);
    std::string isOriginalTitleStr = GetFieldByName(columns, fieldsIndices, IS_ORIGINAL_TITLE_FIELD);
    bool isOriginalTitle = false;
    try {
        isOriginalTitle = parseBool(isOriginalTitleStr);
    } catch (ConversionError& e) {
        std::string error_message = "Error while parsing column " + IS_ORIGINAL_TITLE_FIELD;
        error_message += " " + e.getInputString() + " - " + e.what();
        throw Error{error_message};
    }

    if (!isOriginalTitle && region != localization_) {
        return;
    }
    auto [it, is_new] = titles_.emplace(titleId, Title{});
    Title& mapped_title = it->second;
    if (isOriginalTitle) {
        mapped_title.originalTitle = title;
    }
    if (region == localization_) {
        mapped_title.localizedTitle = title;
    }
}

std::string ImdbLocalizator::findTitleForId(const std::string& name) {
    sendStatistics("SearchCount");
    auto it = titles_.find(name);
    if (it == titles_.end()) {
        throw Error{"Title: "s + name + " not found"};
    }

    Title& result = it->second;
    if (!result.localizedTitle.empty()) {
        sendStatistics("LocalizedTitleFound");
        return result.localizedTitle;
    }
    if (!result.originalTitle.empty()) {
        sendStatistics("OriginalTitleFound");
        return result.originalTitle;
    }
    throw Error{"Title: "s + name + " not found"};
}

void ImdbLocalizator::sendStatistics(const std::string& metric) {
    if (stats_) {
        stats_->acceptStat(metric);
    }
}
