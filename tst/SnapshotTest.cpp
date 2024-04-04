#define BOOST_TEST_DYN_LINK
#include <boost/test/included/unit_test.hpp>
#include <boost/test/parameterized_test.hpp>

#include <fstream>
#include <memory>
#include <vector>

#include "GenThread.h"

using namespace boost::unit_test;
namespace fs = std::filesystem;

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
        return rhsType && fabs(data - rhsType->data) < 0.0001f;
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
                               return fabs(rhs - lhs) < 0.0001f;
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
    std::ifstream ifStream(path, std::ifstream::binary);
    if (ifStream.fail()) {
        throw std::runtime_error(path.string() + " could not be opened");
    }

    std::vector<std::shared_ptr<EspRecord>> records;

    while (ifStream.tellg() != -1) {
        std::shared_ptr<EspRecord> record = std::make_shared<EspRecord>();
        records.push_back(record);

        char type[4];
        ifStream.read(type, 4);
        record->type = std::string(type, 4);
        ifStream.read((char*)&record->length, sizeof(uint32_t));
        ifStream.seekg(sizeof(uint32_t)*2, std::ifstream::cur);

        std::streampos end = ifStream.tellg() + std::streampos(record->length);
        while (ifStream.tellg() < end) {
            std::shared_ptr<EspGenericSubRecord> subRecord = std::make_shared<EspGenericSubRecord>();
            record->subRecords.push_back(subRecord);

            char subType[4];
            ifStream.read(subType, 4);
            subRecord->type = std::string(subType, 4);
            ifStream.read((char*)&subRecord->length, sizeof(uint32_t));

            char* data = new char[subRecord->length];
            ifStream.read(data, subRecord->length);
            subRecord->data = std::string(data);
            delete[] data;
        }
    }

    return records;
}

std::optional<std::shared_ptr<EspSubRecord>> replaceSubRecords(
        std::shared_ptr<EspRecord> record,
        std::shared_ptr<EspSubRecord> subRecord) {

    auto genericSubRecord = dynamic_cast<EspGenericSubRecord*>(subRecord.get());
    if (!genericSubRecord) {
        return std::nullopt;
    }

    if (record->type == "CELL" && subRecord->type == "XSCL") {
        auto newRecord = std::make_shared<EspCellSubXscl>();
        newRecord->length = genericSubRecord->length;
        newRecord->type = genericSubRecord->type;
        newRecord->data = atof(genericSubRecord->data.c_str());
        return newRecord;
    }

    // There are multiple DATA sub records in a CELL record, so verify the length to ensure we have the correct one
    const int expectedNumberOfFloats = 6;
    const int expectedLength = sizeof(float) * expectedNumberOfFloats;
    if (record->type == "CELL" && subRecord->type == "DATA" && subRecord->length == expectedLength) {
        auto newRecord = std::make_shared<EspCellSubData>();
        newRecord->type = genericSubRecord->type;
        newRecord->length = genericSubRecord->length;

        const auto* floatArray = reinterpret_cast<const float*>(genericSubRecord->data.c_str());
        newRecord->data = std::vector(floatArray, floatArray+expectedNumberOfFloats);
        return newRecord;
    }

    return std::nullopt;
}

std::vector<std::shared_ptr<EspRecord>> parseEsp(std::vector<std::shared_ptr<EspRecord>> records) {
    for (const auto &record: records) {
        std::vector<std::shared_ptr<EspSubRecord>> newSubRecords;
        for (const auto &subRecord: record->subRecords) {
            auto newSubRecord = replaceSubRecords(record, subRecord);
            newSubRecords.push_back(newSubRecord.has_value() ? newSubRecord.value() : subRecord);
        }
        record->subRecords = newSubRecords;
    }
    return records;
}

bool compareEsp(const fs::path& testPath, const fs::path& expectedPath) {
    auto testRecords = parseEsp(readEsp(testPath));
    auto expectedRecords = parseEsp(readEsp(expectedPath));
    return std::equal(testRecords.begin(), testRecords.end(), expectedRecords.begin(), expectedRecords.end(),
                      [](const std::shared_ptr<EspRecord> lhs, const std::shared_ptr<EspRecord> rhs) {
                          return *lhs == *rhs;
                      });
}

void test_snapshot(const std::string& name) {
    Generator::generate(
            [](int, const std::string&) {},
            []() {},
            [](const std::string& err) { throw std::runtime_error(err); },
            fs::path("snapshots") / fs::path(name) / fs::path("configuration.ini"),
            std::vector<fs::path>({fs::path("snapshots") / fs::path("GrassTestBase.esp")}),
            fs::path("output") / fs::path(name + ".esp"),
            0
    );

    BOOST_TEST_CONTEXT("Snapshot test for: " + name) {
        BOOST_TEST(compareEsp(
                fs::path("output") / fs::path(name + ".esp"),
                fs::path("snapshots") / fs::path(name) / fs::path("expected.esp")
        ));
    }
}

void test_configuration_from_mod(const fs::path& configPath) {
    // Tests configuration from existing mods loads as expected. We don't verify the output, just the lack of failure
    Generator::generate(
            [](int, const std::string&) {},
            []() {},
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