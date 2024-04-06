#ifndef MW_MESH_GEN_ESPREADER_H
#define MW_MESH_GEN_ESPREADER_H

#include <fstream>
#include <string>
#include <optional>
#include <cassert>
#include <vector>
#include <set>
#include <filesystem>

class EspReader {
public:
    class EndIterator {};

    class SubRecord {
        std::ifstream* ifStream;

    public:
        std::string type;
        uint32_t length;

        SubRecord(std::ifstream *ifStream, const std::string &type, uint32_t length) : ifStream(ifStream),
                                                                                       type(type),
                                                                                       length(length) {};

        template<class T>
        T read() {
            T t;
            ifStream->read(reinterpret_cast<char*>(&t), sizeof(T));
            return t;
        }

        template<class T>
        std::vector<T> readArray() {
            const size_t size = length/sizeof(T);
            assert(size > 0);
            std::vector<T> result;
            result.resize(size);
            ifStream->read(reinterpret_cast<char*>(&result[0]), sizeof(T)*size);
            return result;
        }

        std::string readNullTerminatedString();
    };

    class SubRecordIterator {
        std::ifstream* ifStream;
        std::streampos recordEnd;
        std::streampos nextSubRecordStart;

        std::optional<SubRecord> current = std::nullopt;
    public:
        SubRecordIterator(std::ifstream* ifStream, std::streampos recordEnd) : ifStream(ifStream), recordEnd(recordEnd) {
            next();
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
    public:
        std::string type;
        uint32_t length;
        uint32_t header;
        uint32_t flags;

        Record(std::ifstream* ifStream, const std::streampos& nextRecordStart, const std::string& type, uint32_t length,
               uint32_t header, uint32_t flags) : ifStream(ifStream), nextRecordStart(nextRecordStart), type(type),
                                                  length(length), header(header), flags(flags) {}

        SubRecordIterator begin() const {
            return SubRecordIterator{ifStream, nextRecordStart};
        }
        EndIterator end() const { return {}; }
    };

    class RecordIterator {
        std::ifstream* ifStream;
        std::optional<Record> current;
        std::streampos nextRecordStart;
    public:
        RecordIterator(std::ifstream* ifStream) : ifStream(ifStream), current(std::nullopt), nextRecordStart(EOF) {
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
    EspReader(const std::filesystem::path& path) {
        ifStream = std::ifstream(path, std::ios::in | std::ios::binary);
        if (ifStream.fail()) {
            throw std::runtime_error("Cannot open file: " + path.string());
        }
    };

    RecordIterator begin() { return RecordIterator(&ifStream); }
    EndIterator end()  { return EndIterator{}; }
};

// Template specialization is defined outside of the class for GCC support
template<>
inline std::string EspReader::SubRecord::read<std::string>() {
    std::string result(length, '\0');
    ifStream->read(result.data(), length);
    return result;
}

#endif //MW_MESH_GEN_ESPREADER_H
