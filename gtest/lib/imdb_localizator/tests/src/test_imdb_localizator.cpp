#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <list>

#include "errors.hpp"
#include "imdb_localizator.hpp"

using namespace std::string_literals;

class ListStubDataSource : public IDataSource {
public:
    void addLine(std::string str) {
        lines_.emplace_back(std::move(str));
    }

    std::optional<std::string> getNextLine() override {
        if (lines_.empty()) {
            return std::nullopt;
        }
        std::string result = std::move(lines_.front());
        lines_.pop_front();
        return result;
    }

private:
    std::list<std::string> lines_;
};

class ImdbLocalizatorTest : public ::testing::Test {
protected:
    void SetUp() override {
        listDataSource_ = std::make_unique<ListStubDataSource>();
    }

    void TearDown() override {
    }

protected:
    std::unique_ptr<ListStubDataSource> listDataSource_;
};


TEST_F(ImdbLocalizatorTest, EmptyDataSourceFails) {
    EXPECT_THROW(ImdbLocalizator{std::move(listDataSource_)}, Error);
}

TEST_F(ImdbLocalizatorTest, EmptyLineDataSourceFails) {
    listDataSource_->addLine("");
    EXPECT_THROW(ImdbLocalizator{std::move(listDataSource_)}, Error);
}

TEST_F(ImdbLocalizatorTest, NoTitleIdInHeaderFails) {
    listDataSource_->addLine("titleI\tordering\ttitle\tregion\tlanguage\ttypes\tattributes\tisOriginalTitle");
    EXPECT_THROW(ImdbLocalizator{std::move(listDataSource_)}, Error);
}

TEST_F(ImdbLocalizatorTest, SimpleCase) {
    listDataSource_->addLine("titleId\tordering\ttitle\tregion\tlanguage\ttypes\tattributes\tisOriginalTitle");
    listDataSource_->addLine("tt0022151\t1\tМиллионер\tRU\t\\N\timdbDisplay\t\\N\t0");
    ImdbLocalizator loc{std::move(listDataSource_)};

    EXPECT_EQ(loc.findTitleForId("tt0022151"), "Миллионер");
}

TEST_F(ImdbLocalizatorTest, ShuffledHeaderColumnsSuccess) {
    listDataSource_->addLine("titleId\tlanguage\tisOriginalTitle\tregion\ttitle");
    listDataSource_->addLine("tt0022151\tRU\t0\tRU\tМиллионер");
    ImdbLocalizator loc{std::move(listDataSource_)};

    EXPECT_EQ(loc.findTitleForId("tt0022151"), "Миллионер");
}

struct NoGTestMockStatisticsConsumer : public IStatisticsConsumer {
    void acceptStat(const std::string& metric_name) override {
        passedMetrics.push_back(metric_name);
    }
    std::vector<std::string> passedMetrics;
};

TEST_F(ImdbLocalizatorTest, MockCase) {
    listDataSource_->addLine("titleId\tlanguage\tisOriginalTitle\tregion\ttitle");
    listDataSource_->addLine("tt0022151\tRU\t0\tRU\tМиллионер");
    ImdbLocalizator loc{std::move(listDataSource_)};

    auto statMock = std::make_unique<NoGTestMockStatisticsConsumer>();
    NoGTestMockStatisticsConsumer* mockAddr = statMock.get();
    loc.SetStatisticsConsumer(std::move(statMock));
    loc.findTitleForId("tt0022151");

    std::vector<std::string> expected{"SearchCount", "LocalizedTitleFound"};
    EXPECT_EQ(mockAddr->passedMetrics, expected);
}

struct GMockStatisticsConsumer : public IStatisticsConsumer {
    MOCK_METHOD(void, acceptStat, (const std::string& name), (override));
};

TEST_F(ImdbLocalizatorTest, GMockCase) {
    listDataSource_->addLine("titleId\tlanguage\tisOriginalTitle\tregion\ttitle");
    listDataSource_->addLine("tt0022151\tRU\t0\tRU\tМиллионер");
    ImdbLocalizator loc{std::move(listDataSource_)};

    auto statMock = std::make_unique<GMockStatisticsConsumer>();
    GMockStatisticsConsumer* mockAddr = statMock.get();
    loc.SetStatisticsConsumer(std::move(statMock));

    testing::InSequence s;
    EXPECT_CALL(*mockAddr, acceptStat("SearchCount"));
    EXPECT_CALL(*mockAddr, acceptStat("LocalizedTitleFound"));
    loc.findTitleForId("tt0022151");
}
