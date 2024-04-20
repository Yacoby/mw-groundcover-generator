#include "EspWriter.h"

RecordType::RecordType() : type(0) {
#ifndef NDEBUG
    typeStr = "UNDEF";
#endif
}

RecordType::RecordType(const std::string& type) : RecordType(type.c_str()) {
#ifndef NDEBUG
    typeStr = type;
#endif
}

RecordType::RecordType(const char* cstrType) : RecordType(*reinterpret_cast<const uint32_t*>(cstrType)) {
#ifndef NDEBUG
    typeStr = std::string(cstrType, 4);
#endif
}

RecordType::RecordType(const uint32_t& type) : type(type) {
#ifndef NDEBUG
    typeStr = string();
#endif
}

uint32_t RecordType::toInt() const {
    return type;
}

const char* RecordType::cstring() const {
    return reinterpret_cast<const char*>(&type);
}

std::string RecordType::string() const {
    return std::string(cstring(), 4);
}

char* RecordType::data() {
    return reinterpret_cast<char*>(&type);
}

bool RecordType::operator==(const RecordType& rhs) const {
    return type == rhs.type;
}

bool RecordType::operator!=(const RecordType& rhs) const {
    return !(rhs == *this);
}

bool RecordType::operator<(const RecordType& rhs) const {
    return type < rhs.type;
}

bool RecordType::operator>(const RecordType& rhs) const {
    return rhs < *this;
}

bool RecordType::operator<=(const RecordType& rhs) const {
    return !(rhs < *this);
}

bool RecordType::operator>=(const RecordType& rhs) const {
    return !(*this < rhs);
}

SubRecordWriter::SubRecordWriter(std::ofstream* ofStream, std::variant<std::streampos, size_t> lengthPosOrLength) : ofStream(ofStream), lengthPosOrLength(lengthPosOrLength) {
}

SubRecordWriter::~SubRecordWriter() {
    assert(closed);
}

void SubRecordWriter::close() {
    if (holds_alternative<std::streampos>(lengthPosOrLength)) {
        auto pos = ofStream->tellp();
        ofStream->seekp(std::get<std::streampos>(lengthPosOrLength));
        ofStream->write((char*) &writtenLength, sizeof(uint32_t));
        ofStream->seekp(pos);
    } else {
        assert(std::get<size_t>(lengthPosOrLength) == writtenLength);
    }
    closed = true;
}

RecordWriter::RecordWriter(std::ofstream* ofStream, std::streampos recordLengthPos) : ofStream(ofStream), recordLengthPos(recordLengthPos) {
}

RecordWriter::~RecordWriter() {
    assert(closed);
}

SubRecordWriter RecordWriter::writeSubRecord(const RecordType& type, std::optional<size_t> length) {
    ofStream->write(type.cstring(), 4);

    // tellp() is really slow so avoid if we can
    std::variant<std::streampos, size_t> lengthPosOrLength = length.has_value() ?
                                                             std::variant<std::streampos, size_t>(length.value())
                                                                                : std::variant<std::streampos, size_t>(ofStream->tellp());

    uint32_t l = length.value_or(0);
    ofStream->write(reinterpret_cast<char*>(&l), sizeof(uint32_t));

    return SubRecordWriter(ofStream, lengthPosOrLength);
}

void RecordWriter::close() {
    auto pos = ofStream->tellp();
    ofStream->seekp(recordLengthPos);
    uint32_t length = pos - (recordLengthPos + 3 * std::streampos(sizeof(uint32_t)));
    ofStream->write((char*)&length, sizeof(uint32_t));
    ofStream->seekp(pos);
    closed = true;
}

EspWriter::EspWriter(const std::filesystem::path& path) {
    ofStream = std::ofstream(path, std::ios::out | std::ios::binary);
    if (ofStream.fail()) {
        throw std::runtime_error("Cannot open file: " + path.string());
    }
}

RecordWriter EspWriter::writeRecord(const RecordType& type, const uint32_t& data, const uint32_t& flags) {
    ofStream.write(type.cstring(), 4);

    std::streampos lengthPosition = ofStream.tellp();
    uint32_t length = 0;
    ofStream.write((char*)&length, sizeof(uint32_t));

    ofStream.write(reinterpret_cast<const char*>(&data), sizeof(uint32_t));
    ofStream.write(reinterpret_cast<const char*>(&flags), sizeof(uint32_t));

    return RecordWriter(&ofStream, lengthPosition);
}

void EspWriter::close() {
    ofStream.close();
}
