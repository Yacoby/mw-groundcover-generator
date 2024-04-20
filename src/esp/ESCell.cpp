#include "ESCell.h"

std::optional<std::string> emptyStringToEmptyOptional(const std::string& s) {
    if (s.empty()) {
        return std::nullopt;
    }
    return s;
}

ESCell ESCell::load(EspReader::Record& record) {
    ESCell cell;

    for (auto &subRecord: record) {
        if (subRecord.type == "FRMR") {
            break;
        } else if (subRecord.type == "DATA") {
            cell.data = subRecord.read<CellData>();
        } else if (subRecord.type == "NAME") {
            cell.cellName = emptyStringToEmptyOptional(subRecord.readEntireSubRecord<std::string>());
        } else if (subRecord.type == "RGNN") {
            cell.region = subRecord.readEntireSubRecord<std::string>();
        }
    }

    return cell;
}
