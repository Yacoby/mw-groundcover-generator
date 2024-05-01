#include "TestHelpers.h"

#include <memory>
#include <vector>
#include <fstream>

#include <boost/test/unit_test.hpp>
#include <boost/test/parameterized_test.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/combine.hpp>

#include "esp/EspReader.h"

namespace fs = std::filesystem;

std::shared_ptr<spdlog::logger> makeNullLogger() {
    std::vector<spdlog::sink_ptr> sinks;
    return std::make_shared<spdlog::logger>("null_logger", begin(sinks), end(sinks));
}

// Account for different systems and compilers
const float MAX_FLOAT_ERROR = 0.01f;

class EspSubRecord {
private:
    virtual bool doCompare(const EspSubRecord& rhs) const = 0;
public:
    RecordType type;
    uint32_t length;
    bool operator==(const EspSubRecord& rhs) const {
        return type == rhs.type && length == rhs.length && doCompare(rhs);
    }
    bool operator!=(const EspSubRecord& rhs) const {
        return !operator==(rhs);
    }

    virtual void print(std::ostream& os) const {
        os << "[SubRecord type=" << type.string() << "]";
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

    virtual void print(std::ostream& os) const override {
        std::vector<std::string> output;
        output.reserve(data.size());
        for (auto i: data) output.push_back(std::to_string(i));

        auto dataStr = boost::algorithm::join(output, ", ");
        os << "[SubRecord type=" << type.string() << ", data=(" << dataStr << ")]";
    }
};

class EspGenericSubRecord : public EspSubRecord {
    bool doCompare(const EspSubRecord& rhs) const override {
        const auto* rhsType = dynamic_cast<const EspGenericSubRecord*>(&rhs);
        return rhsType && data == rhsType ->data;
    }
public:
    std::vector<uint8_t> data;
};

class EspRecord {
public:
    RecordType type;
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

std::ostream& operator << (std::ostream& os, const EspRecord& s) {
    return (os << "[Record type=" << s.type.string() << "]");
}

std::ostream& operator << (std::ostream& os, const EspSubRecord& s) {
    s.print(os);
    return os;
}

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
                subRecord->data = subRecordParser.readEntireSubRecordAsArray<float>();
                record->subRecords.push_back(subRecord);
            } else {
                auto subRecord = std::make_shared<EspGenericSubRecord>();
                subRecord->type = subRecordParser.type;
                subRecord->length = subRecordParser.length;
                subRecord->data = subRecordParser.readEntireSubRecordAsArray<uint8_t>();
                record->subRecords.push_back(subRecord);
            }
        }
    }

    return records;
}

void verifyRecordsMatch(std::shared_ptr<EspRecord> test, std::shared_ptr<EspRecord> expected) {
    bool isMatch = *test == *expected;
    if (isMatch) {
        // Avoid writing test assertion output (in CLion) if the records match
        BOOST_TEST(isMatch);
    } else {
        BOOST_REQUIRE(test->subRecords.size() == expected->subRecords.size());
        for (auto subRecordcomparisonTuple: boost::combine(test->subRecords, expected->subRecords)) {
            std::shared_ptr<EspSubRecord> testSub, expectedSub;
            boost::tie(testSub, expectedSub) = subRecordcomparisonTuple;
            BOOST_TEST(*testSub == *expectedSub);
        }
        BOOST_TEST(isMatch);
    }
}

void verifyEspsMatch(const fs::path& testPath, const fs::path& expectedPath) {
    auto testRecords = readEsp(testPath);
    auto expectedRecords = readEsp(expectedPath);

    BOOST_REQUIRE(testRecords.size() == expectedRecords.size());

    for (auto recordComparisonTuple : boost::combine(testRecords, expectedRecords)) {
        std::shared_ptr<EspRecord> test, expected;
        boost::tie(test, expected) = recordComparisonTuple;
        verifyRecordsMatch(test, expected);
    }
}