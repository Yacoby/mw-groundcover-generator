#include "GenThread.h"

#include <cassert>
#include <boost/random/uniform_real.hpp>
#include <boost/random.hpp>

#include "Configuration.h"
#include "Funcs.h"
#include "ESBase.h"

std::optional<Selector> getConfigurationSelector(
        const Configuration& configuration,
        ES3::ESCellRef cell,
        const std::string& texture,
        const std::string& textureId
) {
    std::vector textures({texture, textureId});
    for (const auto &tex: textures) {
        std::vector<Selector> selectors;
        if (cell) {
            if (!cell->getCellName().empty()) {
                selectors.emplace_back(tex, cell->getCellName());
                selectors.emplace_back(tex, "ANY_NAMED_CELL");
            }
            if (!cell->getRegn().empty()) {
                selectors.emplace_back(tex, cell->getRegn());
            }
        }
        selectors.emplace_back(tex);

        for (const auto &selector: selectors) {
            if (configuration.get(selector).has_value()) {
                return selector;
            }
        }
    }
    return std::nullopt;
}

std::string Generator::getMesh(const std::vector<ObjectPlacementPossibility>& placements, const std::string& objectPrefix, const std::string &cat) {
    std::string grassID = "UNKNOWN_GRASS";
    float meshRand = getRandom(0, 100);
    float meshChance = 1;
    for (const auto &item: placements) {
        meshChance += item.chance;
        if (meshChance > meshRand) {
            if (std::holds_alternative<ObjectId>(item.idOrMesh)) {
                grassID = std::get<ObjectId>(item.idOrMesh).get();
            } else {
                grassID = objectPrefix + cat + std::to_string(item.deprecatedId);
            }
            break;
        }
    }
    return grassID;
}

float Generator::getRandom(float min, float max) {
    // Using boost because uniform_real produces consistent output across platforms
    boost::uniform_real<float> dist(min, max);
    return dist(randomNumberSequence);
}

void Generator::generate(
        std::function<void(int, std::string)> sendStatusUpdate,
        std::function<void()> sendSuccess,
        std::function<void(std::string)> sendFailure,
        const fs::path configurationLocation,
        std::vector<fs::path> inputFiles,
        const fs::path outputLocation,
        unsigned randomSeed) {
    try {
        Generator(sendStatusUpdate, sendSuccess, sendFailure, configurationLocation, inputFiles, outputLocation,
                  randomSeed)
                .doGenerate();
    } catch (std::exception &e) {
        sendFailure(e.what());
    }
}

void Generator::doGenerate() {
    if (mFiles.size() == 0) {
        sendFailure("No files to process");
        return;
    }

    gNumRecords = 0;
    gNumRecordPos = -1;


    sendStatusUpdate(0, "Loading configuration (ini) file");

    const auto configuration = loadConfigurationFromIni(mIniLoc);

    ES3::ESFileContainerRef fc = ES3::ESFileContainerRef(new ES3::ESFileContainer());
    for (auto iter = mFiles.begin(); iter != mFiles.end(); ++iter) {
        std::string fileName = (*iter).filename().string();

        sendStatusUpdate(0, "Loading: " + fileName);

        if (!fc->loadDataFile(*iter)) {
            sendFailure("Failed to load data file: " + fileName);
            return;
        }
    }
    sendStatusUpdate(0, "Loaded all files");

    //setStatusText("Writing STAT records");

    std::ofstream ofs(mOut.c_str(), std::ios::out | std::ios::binary);
    if (!ofs.is_open()) {
        sendFailure("Failed to open output file: " + mOut.string());
        return;
    }

    sendStatusUpdate(0, "Writing grass STAT objects to output file");
    fileWriteEspHdr(ofs);
    {
        for (const auto &item: configuration) {
            if (!item.second.placeMeshesBehaviour.has_value()) {
                continue;
            }

            for (const auto &item2: item.second.placeMeshesBehaviour.value().placements) {
                if (!std::holds_alternative<Mesh>(item2.idOrMesh)) {
                    continue;
                }
                auto mesh = std::get<Mesh>(item2.idOrMesh).get();
                fileWriteStatData(ofs, "STAT", configuration.objectPrefix + item.first.toLegacyCategory() + std::to_string(item2.deprecatedId), mesh);
            }
        }
    }


    Buff buff;
    const auto& cells = fc->getExteriorCellCoordinates();
    auto sortedCells = std::vector<std::pair<int32_t, int32_t >>(cells.begin(), cells.end());
    std::sort(sortedCells.begin(), sortedCells.end());

    int cellsProcessed = 0;
    for (const auto &cellCoord: sortedCells) {
        auto cx = cellCoord.first;
        auto cy = cellCoord.second;
        cellsProcessed++;

        ES3::ESLandRef land = fc->getLand(cx, cy);
        assert(land);

        sendStatusUpdate(cellsProcessed / float(cells.size()) * 100, "Cell: " + std::to_string(cx) + ", " + std::to_string(cy));

        ES3::ESCellRef cell = fc->getFirstCell(cx, cy);

        ES3::ESFileRef file = fc->getLandFile(cx, cy);
        const auto& landTex = land->getLandTextures();
        int frmr = 0;

        if (cell) {
            buffWriteCellStart(&buff, cell->getCellName(), 0, cx, cy, cell->getRegn(), cell->getNAM0());
        } else {
            buffWriteCellStart(&buff, "", 0, cx, cy, "", 0);
        }

        bool hasGrassAdded = false;

        //for each tex
        for (int tx = 0; tx < 16; tx++) {
            for (int ty = 0; ty < 16; ty++) {

                if (landTex[tx][ty] == 0) continue; //no tex I assume?

                if (!file->getLTexExists(landTex[tx][ty] - 1)) continue; //bad bad bad

                auto selector = getConfigurationSelector(configuration, cell, file->getLTexPath(landTex[tx][ty] - 1), file->getLTex(landTex[tx][ty] - 1)->getID());
                if (!selector.has_value()) continue; //didn't manage to find a match
                const auto deprecatedIniCat = selector.value().toLegacyCategory();
                const auto& behaviour = configuration.get(selector.value()).value().get();

                if (!behaviour.placeMeshesBehaviour.has_value()) {
                    continue;
                }
                auto placeBehaviour = behaviour.placeMeshesBehaviour.value();


                //find the grass mesh
                std::string grassID;
                auto &meshList = placeBehaviour.placements;
                if (placeBehaviour.clump) {
                    grassID = getMesh(meshList, configuration.objectPrefix, deprecatedIniCat);
                }

                //allow a max gap of more than 512
                if (placeBehaviour.gap > 512) {
                    float zz = 512 / (float) placeBehaviour.gap;
                    float xx = getRandom(0, 100) / 100;
                    if (zz > xx) continue;
                }

                //put multiple grass objects down
                for (int gx = 0; gx < 512; gx += placeBehaviour.gap) {
                    for (int gy = 0; gy < 512; gy += placeBehaviour.gap) {

                        if (!placeBehaviour.clump) {
                            grassID = getMesh(meshList, configuration.objectPrefix, deprecatedIniCat);
                        }

                        //calc the morrowind pos of the
                        float posx = tx * 512 + cx * 8192 + gx;
                        float posy = ty * 512 + cy * 8192 + gy;

                        //add a random element
                        auto posRand = placeBehaviour.positionRandomization;
                        if (!(posRand.max == posRand.min && posRand.max == 0)) {
                            posx += getRandom(posRand.min, posRand.max);
                            posy += getRandom(posRand.min, posRand.max);
                        }

                        //check if we really should place it, or if it is on a no place texture
                        {
                            bool doContinue = false;

                            for (const auto &exclusion: placeBehaviour.exclusions) {
                                std::string t = exclusion.texture;
                                float r = exclusion.distanceFromTexture.value();

                                for (int p = 0; p < 4; p++) {

                                    float tposx = posx;
                                    float tposy = posy;

                                    switch (p) {
                                        case 0:
                                            tposx += r;
                                            break;
                                        case 1:
                                            tposx -= r;
                                            break;
                                        case 2:
                                            tposy += r;
                                            break;
                                        case 3:
                                            tposy -= r;
                                            break;
                                    }

                                    auto ltex = fc->getLandTexture(tposx, tposy);
                                    if (ltex && (t == ltex->getPath() || t == ltex->getID())) {
                                        //ak, this is not good.
                                        doContinue = true;
                                        break;
                                    } //if (t==file2->getLTexPath(land...
                                    if (doContinue == true) break;
                                }//for ( int p = 0; p < 4; p++){
                                if (doContinue == true) break;
                            }//for ( int c = 0;ini.valueExists(iniCat, "sBan"+toString(c));++c ){
                            if (doContinue == true) continue;
                        }//{

                        auto height = fc->getHeightAt(posx, posy);
                        auto rotation = fc->getAngleAt(posx, posy);

                        float posZ = height + configuration.globalOffset;

                        ES3::Vector3 rot;
                        if (placeBehaviour.alignToNormal) {
                            rot = rotation;
                        }
                        rot.z = getRandom(0, 2 * PI);

                        if (posZ <= placeBehaviour.heights.min || posZ >= placeBehaviour.heights.max) {
                            continue;
                        }

                        //get the scale of tthe object
                        float scale = 1;
                        auto scaleRand = placeBehaviour.scaleRandomization;
                        if (!(scaleRand.max == scaleRand.min && scaleRand.max == 1)) {
                            scale = getRandom(scaleRand.min, scaleRand.max);
                        }


                        buffWriteObjData(&buff, ++frmr, grassID, scale, posx, posy, posZ, rot.x, rot.y, rot.z);
                        hasGrassAdded = true;

                    }

                }

            }
        }

        if (hasGrassAdded) {//write the cell to the disk
            fileWriteCellHdr(&buff, ofs);
            fileWriteBuff(&buff, ofs);
            gNumRecords++;
        }

        buff.clear();


    }//	for

    //fix the number of records
    ofs.seekp(gNumRecordPos);
    ofs.write((char *) &gNumRecords, 4);

    ofs.close();

    sendSuccess();
}

Generator::Generator(const std::function<void(int, std::string)> sendStatusUpdateArg,
                     const std::function<void()> sendSuccessArg,
                     const std::function<void(std::string)> sendFailureArg,
                     const fs::path &configurationLocation,
                     const std::vector<fs::path> &files,
                     const fs::path &outputLocation,
                     unsigned randomSeed) : sendStatusUpdate(sendStatusUpdateArg),
                                            sendSuccess(sendSuccessArg),
                                            sendFailure(sendFailureArg),
                                            mOut(outputLocation),
                                            mIniLoc(configurationLocation),
                                            mFiles(files),
                                            randomNumberSequence(randomSeed) {
}
