#include <iostream>
#include <string_view>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <optional>
#include <variant>
#include <functional>

#include "libconfig.h++"

#include "errors.hpp"
#include "imdb_localizator.hpp"

using libconfig::Config;
using namespace std::string_literals;

class FileDataSource : public IDataSource {
public:
    explicit FileDataSource(std::ifstream&& f)
        : f_{std::move(f)} {}

    std::optional<std::string> getNextLine() override {
        std::string tmp;
        if (std::getline(f_, tmp)) {
            return tmp;
        }
        return std::nullopt;
    }
private:
    std::ifstream f_;
};

namespace {

IDataSourceUptr CreateFileSourceFromConfig(const Config& config) {
    std::string filename = config.lookup("title_akas_path");
    std::ifstream file{filename};
    if (!file) {
        throw Error{"Error opening file" + filename};
    }

    return std::make_unique<FileDataSource>(std::move(file));
}

} // namespace

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << argc << "Wrong usage: imdb CONFIG FILM_ID\n";
        return 1;
    }
    Config cfg;
    cfg.readFile(argv[1]);
    IDataSourceUptr dataSource = CreateFileSourceFromConfig(cfg);

    try {
        ImdbLocalizator loc(std::move(dataSource));
        std::string localization;
        if (cfg.lookupValue("localization", localization)) {
            loc.setLocalization(localization);
        }

        std::cout << loc.findTitleForId(argv[2]) << std::endl;
    } catch (HeaderError& e) {
        std::cerr << e.what() << ": " << e.getColumnName() << std::endl;
        return 2;
    } catch (Error& e) {
        std::cerr << e.what() << std::endl;
        return 2;
    }

    return 0;
}
