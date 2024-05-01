#include "MutableEsp.h"

std::optional<std::string> emptyStringToEmptyOptional2(const std::string& s) {
    if (s.empty()) {
        return std::nullopt;
    }
    return s;
}

void CellReference::load(const EspReader::Record& record, EspReader::SubRecordIterator& iter) {
    auto frmrRecord = *iter;
    reference = frmrRecord.read<uint32_t>();

    bool exit = false;
    for (; iter != record.end() && !exit; ) {
        ++iter;

        auto nextRecord = iter.peekType();
        if (nextRecord == "FRMR" || nextRecord == "MVRF" || nextRecord == "NAM0") {
            exit = true;
        }
        auto subRecord = *iter;
        switch (subRecord.type.toInt()) {
            case toRecordTypeInt("NAME"):
                name = subRecord.readEntireSubRecord<std::string>();
                break;
            case toRecordTypeInt("XSCL"):
                scale = subRecord.readEntireSubRecord<float>();
                break;
            case toRecordTypeInt("DATA"):
                position = subRecord.readEntireSubRecord<CellReferencePosition>();
                break;
        }
    }
}

void CellReference::save(RecordWriter& writer, int frmr) const {
    writer.writeSubRecordAndData<uint32_t>("FRMR", frmr);
    writer.writeSubRecordAndData("NAME", name);
    if (scale.has_value() && scale.value() != 1) {
        writer.writeSubRecordAndData<float>("XSCL", scale.value());
    }
    writer.writeSubRecordAndData("DATA", position);
}

MutableCell::MutableCell(const EspReader::Record& record) {
    load(record);
}

MutableCell::MutableCell(const GridId& position) {
    data.gridX = position.x;
    data.gridY = position.y;
}

void MutableCell::save(EspWriter& writer) const {
    auto recordWriter = writer.writeRecord("CELL", 0, 0);

    recordWriter.writeSubRecordAndData("NAME", cellName.value_or(""));
    recordWriter.writeSubRecordAndData("DATA", data);
    if (region.has_value()) {
        recordWriter.writeSubRecordAndData("RGNN", region.value());
    }
    if (waterHeight.has_value()) {
        recordWriter.writeSubRecordAndData("WHGT", waterHeight.value());
    }
    if (ambientLight.has_value()) {
        // TODO untested if this works
        recordWriter.writeSubRecordAndData("AMBI", ambientLight.value());
    }

    int frmr = 1;
    for (const auto& ref: references) {
        ref.save(recordWriter, frmr++);
    }

    recordWriter.close();
}

void MutableCell::load(const EspReader::Record& record) {
    for (auto iter = record.begin(); iter != record.end(); ++iter) {
        auto subRecord = *iter;
        if (subRecord.type == "DATA") {
            data = subRecord.read<CellData>();
        } else if (subRecord.type == "NAME") {
            cellName = emptyStringToEmptyOptional2(subRecord.readEntireSubRecord<std::string>());
        } else if (subRecord.type == "RGNN") {
            region = subRecord.readEntireSubRecord<std::string>();
        } else if (subRecord.type == "NAM5") {
            colour = subRecord.readEntireSubRecord<uint32_t>();
        } else if (subRecord.type == "NAM0") {
            // As per this I think we can ignore this
            // https://github.com/OpenMW/openmw/blob/3fcf3aca7725dc4c0d53056a8b254ea08f07e8c1/components/esm3/loadcell.hpp#L136
        } else if (subRecord.type == "WHGT") {
            waterHeight = subRecord.readEntireSubRecord<float>();
        } else if (subRecord.type == "AMBI") {
            ambientLight = subRecord.readFixedArray<uint8_t, 4>();
        } else if (subRecord.type == "FRMR") {
            auto& ref = references.emplace_back();
            ref.load(record, iter);
        } else {
            throw std::runtime_error(fmt::format("Unhandled CELL subrecord type: {}", subRecord.type.string()));
        }
    }
}

Header::Header(const EspReader::Record& record) {
    load(record);
}

Header::Header() {
    companyName.fill('\0');
    description.fill('\0');
}

void Header::load(const EspReader::Record& record) {
    auto iter = record.begin();
    if (iter == record.end()) throw std::runtime_error("Invalid file format. Expected HEDR not empty");
    auto hedr = *iter;
    if (hedr.type != "HEDR") throw std::runtime_error("Invalid file format. Expected HEDR not " + hedr.type.string());
    version = hedr.read<float>();
    flags = hedr.read<uint32_t>();
    companyName = hedr.readFixedArray<char, 32>();
    description = hedr.readFixedArray<char, 256>();
    recordCount = hedr.read<uint32_t>();

    ++iter;

    for (; iter != record.end(); ++iter) {
        auto mast = *iter;
        if (mast.type != "MAST") throw std::runtime_error("Invalid file format. Expected MAST not " + mast.type.string());
        auto mastName = mast.readEntireSubRecord<std::string>();
        if (++iter == record.end()) {
            throw std::runtime_error("");
        }
        auto data = *iter;
        if (data.type != "DATA") throw std::runtime_error("Invalid file format. Expected DATA not " + data.type.string());
        auto mastSize = data.read<uint64_t>();

        dependencies.push_back({
                                       .name = mastName,
                                       .size = mastSize,
                               });
    }
}

void Header::save(EspWriter& writer, std::optional<uint32_t> providedRecordCount) const {
    auto recordWriter = writer.writeRecord("TES3", 0, 0);
    auto hedrWriter = recordWriter.writeSubRecord("HEDR");
    hedrWriter.write<float>(version);
    hedrWriter.write<uint32_t>(flags);
    hedrWriter.writeFixedArray<char, 32>(companyName);
    hedrWriter.writeFixedArray<char, 256>(description);
    hedrWriter.write<uint32_t>(providedRecordCount.value_or(recordCount));
    hedrWriter.close();

    for (const auto& dependency: dependencies) {
        recordWriter.writeSubRecordAndData<std::string>("MAST", dependency.name);
        recordWriter.writeSubRecordAndData<uint64_t>("DATA", dependency.size);
    }

    recordWriter.close();
}

Static::Static(const EspReader::Record& record) {
    load(record);
}

Static::Static(const std::string& name, const std::string& model) : name(name), model(model) {
}

void Static::save(EspWriter& writer) {
    auto subWriter = writer.writeRecord("STAT", 0, 0);
    subWriter.writeSubRecordAndData("NAME", name);
    subWriter.writeSubRecordAndData("MODL", model);
    subWriter.close();
}

void Static::load(const EspReader::Record& record) {
    for (auto& item: record) {
        switch (item.type.toInt()) {
            case toRecordTypeInt("NAME"):
                name = item.readEntireSubRecord<std::string>();
                break;
            case toRecordTypeInt("MODL"):
                model = item.readEntireSubRecord<std::string>();
                break;
            default:
                throw std::runtime_error(fmt::format("Unexpected static subrecord type: {}", item.type.string()));
        }
    }
}

void UnhandledRecord::load(const EspReader::Record& record) {
    for (auto& subReader: record) {
        auto subRecord = subrecords.emplace_back();
        subRecord.type = subReader.type;
        subRecord.data = subReader.readEntireSubRecordAsArray<uint8_t>();
    }
}

void UnhandledRecord::save(EspWriter& writer) const {
    auto subWriter = writer.writeRecord(type.string(), 0, 0);
    for (const auto& item: subrecords) {
        subWriter.writeSubRecordAndData(item.type, item.data);
    }
    subWriter.close();
}

void MutableEsp::addOrReplaceStatic(std::unique_ptr<Static> stat) {
    statics[stat->getName()] = std::move(stat);
}

void MutableEsp::addOrReplaceCell(std::unique_ptr<MutableCell>& cell) {
    exteriorCells[cell->getGridPosition()] = std::move(cell);
}

void MutableEsp::load(const std::filesystem::path& path) {

    EspReader reader(path);
    auto iter = reader.begin();

    header.load(*iter);
    ++iter;

    for (; iter != reader.end(); ++iter) {
        switch ((*iter).type.toInt()) {
            case toRecordTypeInt("CELL"): {
                auto cell = std::make_unique<MutableCell>(MutableCell(*iter));
                if (cell->isInterior()) {
                    // Some groundcover mods have (erroneous) modifications to interior cells
                    // hence the need to support them
                    if (cell->getName()->empty()) {
                        throw std::runtime_error("Got an interior cell with no name?");
                    }
                    interiorCells[cell->getName().value()] = std::move(cell);
                } else {
                    exteriorCells[cell->getGridPosition()] = std::move(cell);
                }
                break;
            } case toRecordTypeInt("STAT"): {
                auto stat = std::make_unique<Static>(Static(*iter));
                statics[stat->getName()] = std::move(stat);
                break;
            } default:
                auto& record = otherRecords.emplace_back();
                record.load(*iter);
        }
    }
}

void MutableEsp::save(const std::filesystem::path& path) {
    EspWriter writer(path);

    uint32_t recordCount = statics.size() + exteriorCells.size() + otherRecords.size();
    header.save(writer, recordCount);

    for (const auto& record: statics) {
        record.second->save(writer);
    }
    for (const auto& cell: exteriorCells) {
        cell.second->save(writer);
    }
    for (const auto& cell: interiorCells) {
        cell.second->save(writer);
    }
    for (const auto& record: otherRecords) {
        record.save(writer);
    }

    writer.close();
}
