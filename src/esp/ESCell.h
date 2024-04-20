#ifndef _ESCELL_H_
#define _ESCELL_H_

#include <memory>
#include <fstream>
#include <string>

#include "EspReader.h"

class ESCell;

typedef std::shared_ptr<ESCell> ESCellRef;

struct CellData {
    uint32_t flags = 0;
    int32_t gridX = std::numeric_limits<int32_t>::max();
    int32_t gridY = std::numeric_limits<int32_t>::max();
};

class ESCell {
private:
    std::optional<std::string> cellName;
    std::optional<std::string> region;
    CellData data;
public:
    const std::optional<std::string>& getRegn() const { return region; }
    const std::optional<std::string>& getCellName() const { return cellName; }
    bool isInterior() const { return ((data.flags & 0x01) != 0); }
    int32_t getCellX() const { return data.gridX; }
    int32_t getCellY() const { return data.gridY; }

    static ESCell load(EspReader::Record& record);
};

#endif
