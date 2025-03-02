#include "ESLand.h"

#include <cassert>

ESLand::ESLand() : mHeightData(std::nullopt),
                   mLandTextures(std::nullopt) {
}

void ESLand::loadVtexRecord(boost::multi_array<uint16_t, 2>& textures, EspReader::SubRecord& record) {
    std::fill(textures.data(), textures.data() + textures.num_elements(), 0);

    for (int y1 = 0; y1 < 4; y1++) {
        for (int x1 = 0; x1 < 4; x1++) {
            for (int y2 = 0; y2 < 4; y2++) {
                for (int x2 = 0; x2 < 4; x2++) {
                    textures[x1 * 4 + x2][y1 * 4 + y2] = record.read<uint16_t>();
                }
            }
        }
    }
}

void ESLand::loadVhgtRecord(boost::multi_array<int, 2>& heights, EspReader::SubRecord& record) {
    std::fill(heights.data(), heights.data() + heights.num_elements(), 0);

    float offset = record.read<float>();
    for (int y = 0; y < 65; y++) {
        offset += record.read<char>();
        heights[0][y] = (int) offset;

        float xOffset = offset;
        for (int x = 1; x < 65; x++) {
            xOffset += record.read<char>();
            heights[x][y] = (int) xOffset;
        }
    }
}

ESLand ESLand::load(EspReader::Record& record) {
    ESLand land;

    for (auto &subRecord: record) {
        if (subRecord.type == "INTV") {
            land.mLandSquare = subRecord.read<LandSquare>();
        } else if (subRecord.type == "VHGT") {
            land.mHeightData = boost::multi_array<int, 2>(boost::extents[65][65]);
            loadVhgtRecord(land.mHeightData.value(), subRecord);
        } else if (subRecord.type == "VTEX") {
            land.mLandTextures = boost::multi_array<uint16_t, 2>(boost::extents[16][16]);
            loadVtexRecord(land.mLandTextures.value(), subRecord);
        }
    }

    return land;
}
