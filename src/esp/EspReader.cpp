#include "EspReader.h"

#include <algorithm>

#include <fmt/core.h>

void EspReader::RecordIterator::next() {
    std::string type;
    type.resize(4);
    ifStream->read(type.data(), 4);

    if (ifStream->tellg() == EOF) {
        nextRecordStart = EOF;
        current = std::nullopt;
        return;
    }

    // Intended as a sanity check - are we actually reading something that looks like an ESP/ESM?
    // Originally this verified against a list of valid records - but OpenMW have added more so this check is relaxed to
    // check that this looks somewhat valid.
    // - isdigit allows for TES3
    // - c == '_' allows for NPC_
    if (!std::all_of(type.begin(), type.end(),[](unsigned char c){ return std::isupper(c) || std::isdigit(c) || c == '_'; })) {
        throw std::runtime_error("Unexpected style of record format in file: '" + type + "'" +
                                 " expected string to contain only case char, digit or underscore");
    }

    uint32_t length, header, flags;
    ifStream->read((char*)&length, sizeof(uint32_t));
    ifStream->read((char*)&header, sizeof(uint32_t));
    ifStream->read((char*)&flags, sizeof(uint32_t));
    nextRecordStart = ifStream->tellg() + std::streampos(length);
    if (nextRecordStart > fileSize) {
        throw std::runtime_error(fmt::format(
                "Start of next record ({}) exceeds file size ({}). Read length was {}",
                (long) nextRecordStart,  (long) fileSize, length
        ));
    }

    current = Record(ifStream, fileSize, nextRecordStart, type, length, header, flags);
}

void EspReader::SubRecordIterator::next() {
    std::string type;
    type.resize(4);
    ifStream->read(type.data(), 4);
    assert(ifStream->good());
    assert(isalnum(type[0]) && isalnum(type[1]) && isalnum(type[2]) && isalnum(type[3]));

    uint32_t length;
    ifStream->read((char*)&length, sizeof(uint32_t));
    assert(ifStream->good());

    nextSubRecordStart = ifStream->tellg() + std::streampos(length);
    if (nextSubRecordStart > fileSize) {
        throw std::runtime_error(fmt::format(
                "Start of next sub record ({}) exceeds file size ({}). Read length was {}",
                (long) nextSubRecordStart,  (long) fileSize, length
        ));
    }

    current = SubRecord(ifStream, type, length);
}