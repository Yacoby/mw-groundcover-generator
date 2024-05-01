#ifndef MW_MESH_GEN_MUTABLEESP_H
#define MW_MESH_GEN_MUTABLEESP_H

#include <array>
#include <map>
#include <vector>
#include <iostream>

#include <fmt/core.h>

#include "ESFile.h"
#include "EspWriter.h"

struct CellReferencePosition {
    float x = NAN;
    float y = NAN;
    float z = NAN;
    float xrot = NAN;
    float yrot = NAN;
    float zrot = NAN;
};

struct CellReference {
    uint32_t reference;
    std::string name;
    std::optional<float> scale;
    CellReferencePosition position;

    void load(const EspReader::Record& record, EspReader::SubRecordIterator& iter);

    void save(RecordWriter& writer, int frmr) const;
};

class MutableCell {
    CellData data;
    std::optional<std::string> cellName;
public:
    std::optional<std::string> region;
    std::optional<uint32_t> colour;
    std::optional<float> waterHeight;
    std::optional<std::array<uint8_t, 4>> ambientLight;

    std::vector<CellReference> references;

    MutableCell(const EspReader::Record& record);

    MutableCell(const GridId& position);

    inline bool isInterior() const { return ((data.flags & 0x01) != 0); }

    inline GridId getGridPosition() const {
        assert(!isInterior());
        return GridId(data.gridX, data.gridY);
    }

    inline void setName(const std::string& name) {
        assert(!isInterior());
        cellName = name;
    }

    inline const std::optional<std::string>& getName() const {
        return cellName;
    }

    void save(EspWriter& writer) const;
private:
    void load(const EspReader::Record& record);
};


struct HeaderDependency {
public:
    std::string name;
    uint64_t size;
};

class Header {
public:
    float version = 1.3f;
    uint32_t flags = 0;
    std::array<char, 32> companyName;
    std::array<char, 256> description;
    uint32_t recordCount;

    std::vector<HeaderDependency> dependencies;

    Header(const EspReader::Record& record);

    Header();

    void load(const EspReader::Record& record);

    void save(EspWriter& writer, std::optional<uint32_t> providedRecordCount = std::nullopt) const;
};

class Static {
private:
    std::string name;
public:
    std::string model;

    Static(const EspReader::Record& record);

    Static(const std::string& name, const std::string& model);

    inline const std::string& getName() const {
        return name;
    }

    void save(EspWriter& writer);

private:
    void load(const EspReader::Record& record);
};

class UnhandledSubRecord {
public:
    RecordType type;
    std::vector<std::uint8_t> data;
};

class UnhandledRecord {
public:
    RecordType type;
    std::vector<UnhandledSubRecord> subrecords;

    void load(const EspReader::Record& record);

    void save(EspWriter& writer) const;
};

class MutableEsp {
public:
    Header header;

    std::map<GridId, std::unique_ptr<MutableCell>> exteriorCells;
    std::map<std::string, std::unique_ptr<MutableCell>> interiorCells;
    std::map<std::string, std::unique_ptr<Static>> statics;
    std::vector<UnhandledRecord> otherRecords;

    void addOrReplaceStatic(std::unique_ptr<Static> stat);
    void addOrReplaceCell(std::unique_ptr<MutableCell>& cell);

    void load(const std::filesystem::path& path);
    void save(const std::filesystem::path& path);
};


#endif //MW_MESH_GEN_MUTABLEESP_H
