#include "ESFileContainer.h"

#include <ranges>
#include <optional>

void ESFileContainer::loadDataFile(const std::filesystem::path &file) {
    auto esFile = ESFileRef(new ESFile());
    esFile->loadFile(file);
    mFile.push_back(esFile);
}

ESFileContainer::CellInformation ESFileContainer::getCellInformation(int x, int y) {
    CellInformation info;
    for (const auto& it : std::ranges::reverse_view(mFile)) {
        const auto cell = it->getCell(x, y);
        if (!cell) {
            continue;
        }

        if (!info.name.has_value() && cell->getCellName().has_value()) {
            info.name = cell->getCellName();
        }
        if (!info.region.has_value() && cell->getRegn().has_value()) {
            info.region = cell->getRegn();
        }
        if (info.name.has_value() && info.region.has_value()) {
            break;
        }
    }

    return info;
}

ESLandRef ESFileContainer::getLand(int squX, int squY) {
    if (landRefCache.has_value() && landRefCache.value().squX == squX && landRefCache.value().squY == squY) {
        return landRefCache.value().value;
    }

    ESLandRef land = nullptr;
    for (const auto& it : std::ranges::reverse_view(mFile)) {
        if (it->landExists(squX, squY)) {
            land = it->getLand(squX, squY);
            break;
        }
    }

    landRefCache = std::optional<SquareCache<ESLandRef>>(SquareCache<ESLandRef> {
            .squX = squX,
            .squY = squY,
            .value = land,
    });
    return land;
}

ESFileRef ESFileContainer::getLandFile(int squX, int squY) {
    if (fileRefCache.has_value() && fileRefCache.value().squX == squX && fileRefCache.value().squY == squY) {
        return fileRefCache.value().value;
    }

    ESFileRef file = nullptr;
    for (const auto& it : std::ranges::reverse_view(mFile)) {
        if (it->landExists(squX, squY)) {
            file = it;
            break;
        }
    }
    fileRefCache = std::optional<SquareCache<ESFileRef>>(SquareCache<ESFileRef> {
            .squX = squX,
            .squY = squY,
            .value = file,
    });
    return file;
}

std::set<std::pair<int32_t, int32_t >> ESFileContainer::getExteriorCellCoordinates() {
    std::set<std::pair<int32_t, int32_t> > result;
    for (unsigned i = 0; i < mFile.size(); i++) {
        const auto& coordinates = mFile[i]->getExteriorCellCoordinates();
        result.insert(coordinates.begin(), coordinates.end());
    }
    return result;
}

ESLTexRef ESFileContainer::getLandTexture(float posX, float posY) {
    int cellX = (int) floor(posX / (float) CELL_SIZE);
    int cellY = (int) floor(posY / (float) CELL_SIZE);

    float offx = (int) (posX) % (int) CELL_SIZE;
    int squx = floor(offx / (float) LTEX_GRID_SIZE);
    if (squx < 0) {
        squx = 16 + squx;
    }

    float offy = (int) (posY) % (int) CELL_SIZE;
    int squy = floor(offy / (float) LTEX_GRID_SIZE);
    if (squy < 0) {
        squy = 16 + squy;
    }

    const auto land = getLand(cellX, cellY);
    if (land == nullptr) {
        return nullptr;
    }

    const auto& ltex = land->getLandTextures();
    const auto ltexIdx = ltex[squx][squy];
    if (ltexIdx == 0) {
        return nullptr;
    }

    const auto file = getLandFile(cellX, cellY);
    assert(file != nullptr);
    return file->getLTex(ltexIdx - 1);
}


int ESFileContainer::getHeightAtVertex(int vertexX, int vertexY) {
    int cellX = floor(vertexX / 64.0);
    int cellY = floor(vertexY / 64.0);
    auto land = getLand(cellX, cellY);
    if (land == nullptr) {
        return DEFAULT_LAND_HEIGHT;
    }

    int vertexOffsetX = vertexX - cellX * 64;
    int vertexOffsetY = vertexY - cellY * 64;
    return land->getHeightData()[vertexOffsetX][vertexOffsetY];
}

float ESFileContainer::getHeightAt(float positionX, float positionY) {
    float vertexPositionX = positionX / 8192 * 64;
    float vertexPositionY = positionY / 8192 * 64;

    int vertexIndexX = floor(vertexPositionX);
    int vertexIndexY = floor(vertexPositionY);

    float diffX = vertexPositionX - vertexIndexX;
    float diffY = vertexPositionY - vertexIndexY;

    float t1 = getHeightAtVertex(vertexIndexX, vertexIndexY) * 8;
    float t2 = getHeightAtVertex(vertexIndexX + 1, vertexIndexY) * 8;
    float b1 = getHeightAtVertex(vertexIndexX, vertexIndexY + 1) * 8;
    float b2 = getHeightAtVertex(vertexIndexX + 1, vertexIndexY + 1) * 8;

    float midpoint = (b1 + t2) / 2.0;

    if (diffX + diffY <= 1) {
        b2 = midpoint + (midpoint - t1);
    } else {
        t1 = midpoint + (midpoint - b2);
    }

    float t = (t1 * (1 - diffX)) + (t2 * (diffX));
    float b = (b1 * (1 - diffX)) + (b2 * (diffX));

    float h = (t * (1 - diffY)) + (b * (diffY));

    return h + 16;
}


Vector3 ESFileContainer::getAngleAt(float x, float y) {
    int vertexIndexX = ceil(x / 8192 * (64));
    int vertexIndexY = ceil(y / 8192 * (64));

    float vx[4] = {
            (float) getHeightAtVertex(vertexIndexX, vertexIndexY) * 8,
            (float) getHeightAtVertex(vertexIndexX, vertexIndexY - 1) * 8,
            (float) getHeightAtVertex(vertexIndexX - 1, vertexIndexY) * 8,
            (float) getHeightAtVertex(vertexIndexX - 1, vertexIndexY - 1) * 8,
    };

    float vy[4] = {
            (float) getHeightAtVertex(vertexIndexX, vertexIndexY) * 8,
            (float) getHeightAtVertex(vertexIndexX - 1, vertexIndexY) * 8,
            (float) getHeightAtVertex(vertexIndexX, vertexIndexY - 1) * 8,
            (float) getHeightAtVertex(vertexIndexX - 1, vertexIndexY - 1) * 8,
    };

    return {
            .x = -(atan((vx[0] - vx[1]) / 128) + atan((vx[2] - vx[3]) / 128)) / 2,
            .y = (atan((vy[0] - vy[1]) / 128) + atan((vy[2] - vy[3]) / 128)) / 2,
            .z = 0,
    };
}