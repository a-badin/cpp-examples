#pragma once

#include <fstream>

#include "imdb_localizator.hpp"
#include "errors.hpp"


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
