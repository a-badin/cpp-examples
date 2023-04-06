#pragma once
#include <string>
#include <unordered_map>
#include <fstream>
#include <memory>
#include <optional>

struct Title {
    std::string localizedTitle;
    std::string originalTitle;
};

using FieldsIndices = std::unordered_map<std::string, size_t>;

class IDataSource {
public:
    virtual ~IDataSource() {}
    virtual std::optional<std::string> getNextLine() = 0;
};

class IStatisticsConsumer {
public:
    virtual ~IStatisticsConsumer() {}
    virtual void acceptStat(const std::string& metric_name) = 0;
};

using IDataSourceUptr = std::unique_ptr<IDataSource>;
using IStatisticsConsumerUPtr = std::unique_ptr<IStatisticsConsumer>;

class ImdbLocalizator {
public:
    ImdbLocalizator(IDataSourceUptr dataSource);

    void setLocalization(std::string localization) {
        localization_ = std::move(localization);
    }

    void SetStatisticsConsumer(IStatisticsConsumerUPtr stat) {
        stats_ = std::move(stat);
    }

    std::string findTitleForId(const std::string& name);

private:
    void ParseTitleFromLine(const std::string& str, const FieldsIndices& fieldsIndices);

    void sendStatistics(const std::string& str);

    IDataSourceUptr dataSource_;
    IStatisticsConsumerUPtr stats_;
    std::string localization_;
    std::unordered_map<std::string, Title> titles_;
};

