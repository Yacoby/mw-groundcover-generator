#ifndef MW_MESH_GEN_ESPWRITER_H
#define MW_MESH_GEN_ESPWRITER_H

#include <variant>
#include <fstream>
#include <string>
#include <cassert>
#include <filesystem>
#include <optional>

class RecordType {
    uint32_t type;

#ifndef NDEBUG
    std::string typeStr;
#endif
public:
    RecordType();
    RecordType(const std::string& type);
    RecordType(const char* cstrType);
    RecordType(const uint32_t& type);

    [[nodiscard]] uint32_t toInt() const;
    [[nodiscard]] const char* cstring() const;
    [[nodiscard]] std::string string() const;
    [[nodiscard]] char* data();

    bool operator==(const RecordType& rhs) const;
    bool operator!=(const RecordType& rhs) const;

    bool operator<(const RecordType& rhs) const;
    bool operator>(const RecordType& rhs) const;
    bool operator<=(const RecordType& rhs) const;
    bool operator>=(const RecordType& rhs) const;
};

constexpr uint32_t toRecordTypeInt(const char* f) {
    return static_cast<unsigned char>(f[0])
           | (static_cast<unsigned char>(f[1]) << 8)
           | (static_cast<unsigned char>(f[2]) << 16)
           | (static_cast<unsigned char>(f[3]) << 24);
}

class SubRecordWriter {
    std::ofstream* ofStream;
    std::variant<std::streampos, size_t> lengthPosOrLength;
    size_t writtenLength = 0;
    bool closed = false;
public:
    SubRecordWriter(std::ofstream* ofStream, std::variant<std::streampos, size_t> lengthPosOrLength);

    ~SubRecordWriter();

    template<class T>
    void write(const T& data) {
        writtenLength += SubRecordWriter::getLength(data);
        ofStream->write((char*)&data, SubRecordWriter::getLength(data));
    }

    template<class T, int SIZE>
    void writeFixedArray(const std::array<T, SIZE>& data) {
        writtenLength += sizeof(T)*SIZE;
        ofStream->write(data.data(), sizeof(T)*SIZE);
    }

    template<class T>
    static size_t getLength(const T& data) {
        return sizeof(T);
    }

    void close();
};


class RecordWriter {
    std::ofstream* ofStream;
    std::streampos recordLengthPos;
    int subRecords = 0;
    bool closed = false;
public:
    RecordWriter(std::ofstream* ofStream, std::streampos recordLengthPos);
    ~RecordWriter();

    template<class T>
    void writeSubRecordAndData(const RecordType& type, const T& data) {
        auto srw = writeSubRecord(type, SubRecordWriter::getLength(data));
        srw.write(data);
        srw.close();
    }

    SubRecordWriter writeSubRecord(const RecordType& type, std::optional<size_t> length = std::nullopt);

    void close();
};


class EspWriter {
    std::ofstream ofStream;
public:
    EspWriter(const std::filesystem::path& path);

    RecordWriter writeRecord(
            const RecordType& type,
            const uint32_t& data,
            const uint32_t& flags
    );

    void close();
};

template<>
inline size_t SubRecordWriter::getLength<std::string>(const std::string& data) {
    return data.length() + 1;
}

template<>
inline void SubRecordWriter::write<std::string>(const std::string& data) {
    auto strLen  = SubRecordWriter::getLength(data);
    assert(strLen == data.length() + 1);
    writtenLength += strLen;
    ofStream->write(data.c_str(), strLen);
    char null = '\0';
    ofStream->write(&null, 0);
}

#endif //MW_MESH_GEN_ESPWRITER_H
