#ifndef MW_MESH_GEN_ESPREADER_H
#define MW_MESH_GEN_ESPREADER_H

#include <fstream>
#include <string>
#include <optional>
#include <cassert>
#include <vector>
#include <set>
#include <filesystem>
#include <sstream>

#include <fmt/core.h>

#include "EspWriter.h"

class EspReader {
public:
    class EndIterator {};

    class SubRecord {
        std::ifstream* ifStream;

    public:
        RecordType type;
        uint32_t length;

        SubRecord(std::ifstream *ifStream, const RecordType &type, uint32_t length) : ifStream(ifStream),
                                                                                       type(type),
                                                                                       length(length) {};
        template<class T>
        T read() {
            T t;
            ifStream->read(reinterpret_cast<char*>(&t), sizeof(T));
            return t;
        }

        template<class T>
        T readEntireSubRecord() {
            if (sizeof(T) != length) {
                throw std::runtime_error(fmt::format(
                        "Unexpected record length. sizeof(T) == {}, but length == {}", sizeof(T), length
                ));
            }
            T t;
            ifStream->read(reinterpret_cast<char*>(&t), sizeof(T));
            return t;
        }

        template<class T>
        std::vector<T> readEntireSubRecordAsArray() {
            const size_t size = length/sizeof(T);
            assert(size > 0);
            std::vector<T> result;
            result.resize(size);
            ifStream->read(reinterpret_cast<char*>(&result[0]), sizeof(T)*size);
            return result;
        }

        template<class T, int SIZE>
        std::array<T, SIZE> readFixedArray() {
            std::array<T, SIZE> result;
            ifStream->read(reinterpret_cast<char*>(result.data()), sizeof(T)*SIZE);
            return result;
        }
    };

    class SubRecordIterator {
        std::ifstream* ifStream;
        uintmax_t fileSize;
        std::streampos recordEnd;
        std::streampos nextSubRecordStart;

        std::optional<SubRecord> current = std::nullopt;
    public:
        SubRecordIterator(std::ifstream* ifStream, uintmax_t fileSize, std::streampos recordEnd) : ifStream(ifStream), fileSize(fileSize), recordEnd(recordEnd) {
            next();
        }

        // TODO this is terrible, and quite possibly resets the buffer
        std::optional<RecordType> peekType() {
            auto pos = ifStream->tellg();

            ifStream->seekg(nextSubRecordStart);

            RecordType type((uint32_t)0);
            ifStream->read(type.data(), 4);
            if (ifStream->tellg() == EOF) {
                ifStream->clear();
                ifStream->seekg(pos);
                return std::nullopt;
            }

            ifStream->seekg(pos);
            return type;
        }

        SubRecord& operator*() {
            return current.value();
        }

        SubRecordIterator& operator++() {
            assert(ifStream->tellg() <= nextSubRecordStart);
            ifStream->seekg(nextSubRecordStart);
            if (nextSubRecordStart < recordEnd) {
                next();
            }
            return *this;
        }

        bool operator==(EndIterator rhs) const {
            return !operator!=(rhs);
        }

        bool operator!=(EndIterator rhs) const {
            std::streampos pos = ifStream->tellg();
            return pos != EOF && pos < recordEnd;
        }
    private:
        void next();
    };

    class Record {
    private:
        std::ifstream* ifStream;
        std::streampos nextRecordStart;
        uintmax_t fileSize;
    public:
        RecordType type;
        uint32_t length;
        uint32_t header;
        uint32_t flags;

        Record(std::ifstream* ifStream, uintmax_t fileSize, const std::streampos& nextRecordStart, const RecordType& type, uint32_t length,
               uint32_t header, uint32_t flags) : ifStream(ifStream), fileSize(fileSize), nextRecordStart(nextRecordStart), type(type),
                                                  length(length), header(header), flags(flags) {}

        SubRecordIterator begin() const {
            return SubRecordIterator{ifStream, fileSize, nextRecordStart};
        }
        EndIterator end() const { return {}; }
    };

    class RecordIterator {
        std::ifstream* ifStream;
        uintmax_t& fileSize;
        std::optional<Record> current;
        std::streampos nextRecordStart;

    public:
        RecordIterator(std::ifstream* ifStream, uintmax_t& fileSize) : ifStream(ifStream), fileSize(fileSize), current(std::nullopt), nextRecordStart(EOF) {
            next();
        }

        const Record& operator*() const {
            return current.value();
        }

        RecordIterator& operator++() {
            assert(ifStream->tellg() <= nextRecordStart);
            ifStream->seekg(nextRecordStart);
            next();
            return *this;
        }

        bool operator!=(EndIterator rhs) {
            return nextRecordStart != EOF;
        }
    private:
        void next();
    };


    std::ifstream ifStream;
    uintmax_t fileSize;
    EspReader(const std::filesystem::path& path) {
        fileSize = std::filesystem::file_size(path);
        ifStream = std::ifstream(path, std::ios::in | std::ios::binary);
        if (ifStream.fail()) {
            throw std::runtime_error("Cannot open file: " + path.string());
        }
    };

    RecordIterator begin() { return RecordIterator(&ifStream, fileSize); }
    EndIterator end()  { return EndIterator{}; }
};

// Template specialization is defined outside of the class for GCC support
template<>
inline std::string EspReader::SubRecord::read<std::string>() {
    std::string result(length, '\0');
    ifStream->read(result.data(), length);
    assert(result[result.length()-1] == '\0');
    return result.substr(0, length - 1);
}

template<>
inline std::string EspReader::SubRecord::readEntireSubRecord<std::string>() {
    return read<std::string>();
}

#endif //MW_MESH_GEN_ESPREADER_H
