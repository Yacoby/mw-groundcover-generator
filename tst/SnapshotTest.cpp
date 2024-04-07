#define BOOST_TEST_DYN_LINK
#include <boost/test/included/unit_test.hpp>
#include <boost/test/parameterized_test.hpp>

#include <fstream>
#include <memory>
#include <vector>
#include <boost/range/combine.hpp>

#include "EspReader.h"
#include "GenThread.h"

using namespace boost::unit_test;
namespace fs = std::filesystem;

// Account for different systems and compilers
const float MAX_FLOAT_ERROR = 0.001f;

class EspSubRecord {
private:
    virtual bool doCompare(const EspSubRecord& rhs) const = 0;
public:
    std::string type;
    uint32_t length;
    bool operator==(const EspSubRecord& rhs) const {
        return type == rhs.type && length == rhs.length && doCompare(rhs);
    }
    bool operator!=(const EspSubRecord& rhs) const {
        return !operator==(rhs);
    }
};

class EspCellSubXscl : public EspSubRecord {
    bool doCompare(const EspSubRecord& rhs) const override {
        const auto* rhsType = dynamic_cast<const EspCellSubXscl*>(&rhs);
        return rhsType && fabs(data - rhsType->data) < MAX_FLOAT_ERROR;
    }
public:
    float data;
};

class EspCellSubData : public EspSubRecord {
    bool doCompare(const EspSubRecord& rhs) const override {
        const auto* rhsType = dynamic_cast<const EspCellSubData*>(&rhs);
        return rhsType &&
                std::equal(data.begin(), data.end(), rhsType->data.begin(), rhsType->data.end(),
                           [](float lhs, float rhs) {
                               return fabs(rhs - lhs) < MAX_FLOAT_ERROR;
                           });;
    }
public:
    std::vector<float> data;
};

class EspGenericSubRecord : public EspSubRecord {
    bool doCompare(const EspSubRecord& rhs) const override {
        const auto* rhsType = dynamic_cast<const EspGenericSubRecord*>(&rhs);
        return rhsType && data == rhsType ->data;
    }
public:
    std::string data;
};

class EspRecord {
public:
    std::string type;
    uint32_t length;
    std::vector<std::shared_ptr<EspSubRecord>> subRecords;

    bool operator==(const EspRecord &rhs) const {
        return type == rhs.type && length == rhs.length &&
               subRecords.size() == rhs.subRecords.size() &&
               std::equal(subRecords.begin(), subRecords.end(), rhs.subRecords.begin(), rhs.subRecords.end(),
                          [](const std::shared_ptr<EspSubRecord> lhs, const std::shared_ptr<EspSubRecord> rhs) {
                              return *lhs == *rhs;
                          });
    }

    bool operator!=(const EspRecord& rhs) const {
        return !operator==(rhs);
    }
};


std::vector<std::shared_ptr<EspRecord>> readEsp(const std::filesystem::path& path) {
    std::vector<std::shared_ptr<EspRecord>> records;
    for (const auto &recordParser: EspReader(path)) {
        std::shared_ptr<EspRecord> record = std::make_shared<EspRecord>();

        record->type = recordParser.type;
        record->length = recordParser.length;

        records.push_back(record);

        for (auto &subRecordParser: recordParser) {
            if (recordParser.type == "CELL" && subRecordParser.type == "XSCL") {
                auto subRecord = std::make_shared<EspCellSubXscl>();
                subRecord->length = subRecordParser.length;
                subRecord->type = subRecordParser.type;
                subRecord->data = subRecordParser.read<float>();
                record->subRecords.push_back(subRecord);
            } else if (record->type == "CELL" && subRecordParser.type == "DATA" && subRecordParser.length == sizeof(float)*6) {
                auto subRecord = std::make_shared<EspCellSubData>();
                subRecord->type = subRecordParser.type;
                subRecord->length = subRecordParser.length;
                subRecord->data = subRecordParser.readArray<float>();
                record->subRecords.push_back(subRecord);

            } else {
                auto subRecord = std::make_shared<EspGenericSubRecord>();
                subRecord->type = subRecordParser.type;
                subRecord->length = subRecordParser.length;
                subRecord->data = subRecordParser.read<std::string>();
                record->subRecords.push_back(subRecord);
            }
        }
    }

    return records;
}

std::vector<std::shared_ptr<EspRecord>> parseEsp(std::vector<std::shared_ptr<EspRecord>> records) {
    for (const auto &record: records) {
        std::vector<std::shared_ptr<EspSubRecord>> newSubRecords;
        for (const auto &subRecord: record->subRecords) {
            newSubRecords.push_back(subRecord);
        }
        record->subRecords = newSubRecords;
    }
    return records;
}

void compareEsp(const fs::path& testPath, const fs::path& expectedPath) {
    auto testRecords = parseEsp(readEsp(testPath));
    auto expectedRecords = parseEsp(readEsp(expectedPath));

    BOOST_TEST(testRecords.size() == expectedRecords.size());
    for (auto recordComparisonTuple : boost::combine(testRecords, expectedRecords)) {
        std::shared_ptr<EspRecord> test, expected;
        boost::tie(test, expected) = recordComparisonTuple;

        BOOST_TEST(test->subRecords.size() == expected->subRecords.size());
        for (auto subRecordcomparisonTuple : boost::combine(test->subRecords, expected->subRecords)) {
            std::shared_ptr<EspSubRecord> testSub, expectedSub;
            boost::tie(testSub, expectedSub) = subRecordcomparisonTuple;
            BOOST_TEST(*testSub == *expectedSub);
        }

        BOOST_TEST(*test == *expected);
    }
}

std::ostream& operator << (std::ostream &os, const EspRecord &s) {
    return (os << "[Record type=" << s.type << "]");
}

std::ostream& operator << (std::ostream &os, const EspSubRecord &s) {
    return (os << "[SubRecord type=" << s.type << "]");
}

void test_snapshot(const std::string& name) {
    Generator::generate(
            [](int, const std::string&) {},
            [](int duration) {},
            [](const std::string& err) { throw std::runtime_error(err); },
            fs::path("snapshots") / fs::path(name) / fs::path("configuration.ini"),
            std::vector<fs::path>({fs::path("snapshots") / fs::path("GrassTestBase.esp")}),
            fs::path("output") / fs::path(name + ".esp"),
            0
    );

    BOOST_TEST_CONTEXT("Snapshot test for: " + name) {
        compareEsp(
                fs::path("output") / fs::path(name + ".esp"),
                fs::path("snapshots") / fs::path(name) / fs::path("expected.esp")
        );
    }
}

void test_snapshot_with_overridden_base(const std::string& name) {
    Generator::generate(
            [](int, const std::string&) {},
            [](int duration) {},
            [](const std::string& err) { throw std::runtime_error(err); },
            fs::path("snapshots") / fs::path(name) / fs::path("configuration.ini"),
            std::vector<fs::path>({
                                          fs::path("snapshots") / fs::path("GrassTestBaseToBeOverridden.esp"),
                                          fs::path("snapshots") / fs::path("GrassTestBase.esp"),
                                  }),
            fs::path("output") / fs::path(name + ".esp"),
            0
    );

    BOOST_TEST_CONTEXT("Snapshot test for: " + name) {
        compareEsp(
                fs::path("output") / fs::path(name + ".esp"),
                fs::path("snapshots") / fs::path(name) / fs::path("expected.esp")
        );
    }
}

void test_configuration_from_mod(const fs::path& configPath) {
    // Tests configuration from existing mods loads as expected. We don't verify the output, just the lack of failure
    Generator::generate(
            [](int, const std::string&) {},
            [](int duration) {},
            [](const std::string& err) { throw std::runtime_error(err); },
            configPath,
            std::vector<fs::path>({fs::path("snapshots") / fs::path("GrassTestBase.esp")}),
            fs::path("output") / fs::path("config_test.esp"),
            0
    );
}

test_suite* init_unit_test_suite(int argc, char* argv[]) {
    std::vector<std::string> snapshotDirectories;
    for (auto const& entry : fs::directory_iterator{fs::path("snapshots")}) {
        if (entry.is_directory()) {
            snapshotDirectories.push_back(entry.path().filename().string());
        }
    }
    framework::master_test_suite().
            add(BOOST_PARAM_TEST_CASE(&test_snapshot, snapshotDirectories.begin(), snapshotDirectories.end()) );
    framework::master_test_suite().
            add(BOOST_PARAM_TEST_CASE(&test_snapshot_with_overridden_base, snapshotDirectories.begin(), snapshotDirectories.end()) );

    std::vector<fs::path> iniFiles;
    for (auto const& entry : fs::recursive_directory_iterator{fs::path("mod_configs")}) {
        if (entry.is_regular_file() && entry.path().extension() == ".ini") {
            iniFiles.push_back(entry.path());
        }
    }
    framework::master_test_suite().
            add(BOOST_PARAM_TEST_CASE(&test_configuration_from_mod, iniFiles.begin(), iniFiles.end() ) );

    return nullptr;
}