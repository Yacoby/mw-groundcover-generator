#ifndef MW_MESH_GEN_ESPWRITER_H
#define MW_MESH_GEN_ESPWRITER_H

#include <fstream>
#include <string>
#include <cassert>
#include <filesystem>
#include <optional>

class RecordType {
    uint32_t type;
public:
    RecordType(const std::string& type) : RecordType(type.c_str()) {
    }

    RecordType(const char* cstrType) : RecordType(*reinterpret_cast<const uint32_t*>(cstrType)) {
    }

    RecordType(const uint32_t& type) : type(type) {
    }

    const char* cstring() const {
        return reinterpret_cast<const char*>(&type);
    }

    char* data() {
        return reinterpret_cast<char*>(&type);
    }
};

class SubRecordWriter {
    std::ofstream* ofStream;
    std::variant<std::streampos, size_t> lengthPosOrLength;
    size_t writtenLength = 0;
    bool closed = false;
public:
    SubRecordWriter(std::ofstream* ofStream, std::variant<std::streampos, size_t> lengthPosOrLength) : ofStream(ofStream), lengthPosOrLength(lengthPosOrLength) {
    }
    ~SubRecordWriter() {
        assert(closed);
    }

    template<class T>
    void write(const T& data) {
        writtenLength += SubRecordWriter::getLength(data);
        ofStream->write((char*)&data, SubRecordWriter::getLength(data));
    }

    template<>
    inline void write<std::string>(const std::string& data) {
        auto strLen  = SubRecordWriter::getLength(data);
        assert(strLen == data.length() + 1);
        writtenLength += strLen;
        ofStream->write(data.c_str(), strLen);
        char null = '\0';
        ofStream->write(&null, 0);
    }

    template<class T>
    static size_t getLength(const T& data) {
        return sizeof(T);
    }

    template<>
    static inline size_t getLength<std::string>(const std::string& data) {
        return data.length() + 1;
    }

    void close() {
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
};


class RecordWriter {
    std::ofstream* ofStream;
    std::streampos recordLengthPos;
    int subRecords = 0;
    bool closed = false;
public:
    RecordWriter(std::ofstream* ofStream, std::streampos recordLengthPos) : ofStream(ofStream), recordLengthPos(recordLengthPos) {
    }
    ~RecordWriter() {
        assert(closed);
    }

    template<class T>
    void writeSubRecordAndData(const RecordType& type, const T& data) {
        auto srw = writeSubRecord(type, SubRecordWriter::getLength(data));
        srw.write(data);
        srw.close();
    }

    SubRecordWriter writeSubRecord(const RecordType& type, std::optional<size_t> length = std::nullopt) {
        ofStream->write(type.cstring(), 4);

        // tellp() is really slow so avoid if we can
        std::variant<std::streampos, size_t> lengthPosOrLength = length.has_value() ?
                                                                 std::variant<std::streampos, size_t>(length.value())
                                                                                    : std::variant<std::streampos, size_t>(ofStream->tellp());

        uint32_t l = length.value_or(0);
        ofStream->write(reinterpret_cast<char*>(&l), sizeof(uint32_t));

        return SubRecordWriter(ofStream, lengthPosOrLength);
    }

    void close() {
        auto pos = ofStream->tellp();
        ofStream->seekp(recordLengthPos);
        uint32_t length = pos - (recordLengthPos + 3 * std::streampos(sizeof(uint32_t)));
        ofStream->write((char*)&length, sizeof(uint32_t));
        ofStream->seekp(pos);
        closed = true;
    }
};


class EspWriter {
    std::ofstream ofStream;
public:
    EspWriter(const std::filesystem::path& path) {
        ofStream = std::ofstream(path, std::ios::out | std::ios::binary);
        if (ofStream.fail()) {
            throw std::runtime_error("Cannot open file: " + path.string());
        }
    }

    RecordWriter writeRecord(
            const RecordType& type,
            const uint32_t& data,
            const uint32_t& flags
    ) {
        ofStream.write(type.cstring(), 4);

        std::streampos lengthPosition = ofStream.tellp();
        uint32_t length = 0;
        ofStream.write((char*)&length, sizeof(uint32_t));

        ofStream.write(reinterpret_cast<const char*>(&data), sizeof(uint32_t));
        ofStream.write(reinterpret_cast<const char*>(&flags), sizeof(uint32_t));

        return RecordWriter(&ofStream, lengthPosition);
    }

    void close() {
        ofStream.close();
    }
};

#endif //MW_MESH_GEN_ESPWRITER_H
