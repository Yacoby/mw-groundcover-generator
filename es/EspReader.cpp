#include "EspReader.h"

const std::set<std::string> VAlID_RECORD_TYPES = {
        "ACTI", "ALCH", "APPA", "ARMO", "BODY", "BOOK", "BSGN", "CELL", "CLAS", "CLOT", "CONT", "CREA", "DIAL", "DOOR",
        "ENCH", "FACT", "FRMR", "GLOB", "GMST", "INFO", "INGR", "LAND", "LEVC", "LEVI", "LIGH", "LOCK", "LTEX", "MGEF",
        "MISC", "NPC_", "PGRD", "PROB", "RACE", "REGN", "REPA", "SCPT", "SKIL", "SNDG", "SOUN", "SPEL", "SSCR", "STAT",
        "TES3", "WEAP"
};

void EspReader::RecordIterator::next() {
    std::string type;
    type.resize(4);
    ifStream->read(type.data(), 4);

    if (ifStream->tellg() == EOF) {
        nextRecordStart = EOF;
        current = std::nullopt;
        return;
    }

    // Intended as a sanity check - are we actually reading an ESP/ESM?
    if (VAlID_RECORD_TYPES.find(type) == VAlID_RECORD_TYPES.end()) {
        throw std::runtime_error("Unexpected record type in file: " + type);
    }

    uint32_t length, header, flags;
    ifStream->read((char*)&length, sizeof(uint32_t));
    ifStream->read((char*)&header, sizeof(uint32_t));
    ifStream->read((char*)&flags, sizeof(uint32_t));
    nextRecordStart = ifStream->tellg() + std::streampos(length);

    current = Record(ifStream, nextRecordStart, type, length, header, flags);

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
    current = SubRecord(ifStream, type, length);
}

std::string EspReader::SubRecord::readNullTerminatedString() {
    std::string result = read<std::string>();
    assert(result[result.length()-1] == '\0');
    return result.substr(0, result.length()-1);
}