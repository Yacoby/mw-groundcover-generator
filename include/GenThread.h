#include "GUI.h"
#pragma once

namespace fs = boost::filesystem;

class GenThread : public wxThread{
	std::string mOut, mIdBase, mIniLoc;
	int mOffset;
	std::vector<std::string> mFiles;
	GUI* mGUI;

	std::string getMesh(const std::list<GrassIni2::GrassMesh>& meshList, const std::string& cat){
		std::string grassID = "UNKNOWN_GRASS";
		float meshRand = getRand(0, 100);
		float meshChance = 1;
		for ( std::list<GrassIni2::GrassMesh>::const_iterator iter = meshList.begin(); iter != meshList.end(); ++iter ){

			meshChance += iter->chance;
			if ( meshChance > meshRand ){
				if ( iter->objectID.length() == 0 )
					grassID = mIdBase + cat + toString(iter->id);
				else
					grassID = iter->objectID;
				break;
			}
		}
		return grassID;
	}

	int getCell(float xy){
		int cell = 0;
		for ( xy; xy > 8192; cell++ )
			xy -=8192;
		for ( xy; xy < 0; cell-- )
			xy +=8192;
		return cell;
	}

    void sendStatusUpdate(int progressPercent, std::string message){
        wxCommandEvent evt( wxEVT_COMMAND_MENU_SELECTED, WORKER_UPDATE );
        evt.SetInt(progressPercent);
        evt.SetString(message);
        wxPostEvent( mGUI, evt );
    }

    void sendSuccess(){
        wxCommandEvent evt( wxEVT_COMMAND_MENU_SELECTED, WORKER_SUCCESS );
        wxPostEvent( mGUI, evt );
    }

    void sendFailure(std::string message){
        wxCommandEvent evt( wxEVT_COMMAND_MENU_SELECTED, WORKER_FAILURE );
        evt.SetString(message);
        wxPostEvent( mGUI, evt );
    }

protected:
	ExitCode Entry() {
        try {
            return Generate();
        } catch (std::exception& e) {
            sendFailure(e.what());
        }
        return 0;
    }

    ExitCode Generate() {
		if ( mFiles.size() == 0 ){
			sendFailure("No files to process");
			return 0;
		}

		gNumRecords = 0;
		gNumRecordPos = -1;


		sendStatusUpdate(0, "Loading configuration (ini) file");

		//load the ini
		GrassIni2 ini;
		if ( !ini.load(mIniLoc) ){
			sendFailure("Failed to load ini file");
			return 0;
		}

		ES3::ESFileContainerRef fc = ES3::ESFileContainerRef(new ES3::ESFileContainer());
		for ( std::vector<std::string>::iterator iter = mFiles.begin(); iter != mFiles.end(); ++iter){
            std::string fileName = fs::path(*iter).filename().string();

			sendStatusUpdate(0, "Loading: " + fileName);

			if (!fc->loadDataFile(*iter)){ 
				sendFailure("Failed to load data file: " + fileName);
				return 0;
			}
		}
        sendStatusUpdate(0, "Loaded all files");

        //setStatusText("Writing STAT records");

		std::ofstream ofs(mOut.c_str(), std::ios::out | std::ios::binary );
		if ( !ofs.is_open() ) {
            sendFailure("Failed to open output file: " + mOut);
            return 0;
        }

        sendStatusUpdate(0, "Writing grass STAT objects to output file");
        fileWriteEspHdr(ofs);
		{
			std::list<std::string> texLst = ini.getTextureList();
			for ( std::list<std::string>::iterator iter1 = texLst.begin(); iter1 != texLst.end(); ++iter1 ){
				std::list<GrassIni2::GrassMesh> meshList = ini.getMeshList(*iter1);
				for ( std::list<GrassIni2::GrassMesh>::iterator iter2 = meshList.begin(); iter2 != meshList.end(); ++iter2 ){

					if ( iter2->objectID.length() == 0 )
						fileWriteStatData(ofs, ini.getValue(*iter1, "sRecType"),
							mIdBase + *iter1 + toString(iter2->id), iter2->mesh, 
							ini.getValue(*iter1, "sName"), ini.getValue(*iter1, "sScript"));

				}
			}
		}


		Buff buff(0);
        const auto cells = fc->getExteriorCellCoordinates();
        uint cellsProcessed = 0;
        for (const auto &cellCoord: cells) {
            auto cx = cellCoord.first;
            auto cy = cellCoord.second;
            cellsProcessed++;

            if ( TestDestroy() ){
                sendFailure( "Generation Failed. Forced Exit");
                return 0;
            }

            ES3::ESLandRef land = fc->getLand(cx,cy);
            assert(land);

            sendStatusUpdate( cellsProcessed/float(cells.size())*100, "Cell: " + toString(cx) + ", " + toString(cy) );

            ES3::ESCellRef cell = fc->getFirstCell(cx,cy);

            ES3::ESFileRef file = fc->getLandFile(cx, cy);
            std::vector< std::vector< uint16_t > > landTex = land->getLandTextures();
            int frmr = 0;

            if ( cell )
                buffWriteCellStart(&buff, cell->getCellName(), 0, cx, cy, cell->getRegn(), cell->getNAM0());
            else
                buffWriteCellStart(&buff, "", 0, cx, cy, "", 0);

            bool hasGrassAdded = false;

            //for each tex
            for ( int tx = 0; tx < 16; tx++){
                for ( int ty = 0; ty < 16; ty++){

                    if ( landTex[tx][ty] == 0 ) continue; //no tex I assume?

                    if ( !file->getLTexExists(landTex[tx][ty]-1) ) continue; //bad bad bad


                    std::string iniCat("");
                    for (int j = 0; j < 2; ++j){
                        std::string tex;
                        if ( j == 0 )tex = file->getLTexPath(landTex[tx][ty]-1);
                        else		 tex = file->getLTex(landTex[tx][ty]-1)->getID();


                        if ( cell ){
                            if ( cell->getCellName().length() > 0 && ini.catExists(tex + ":" + cell->getCellName()) ){
                                iniCat = tex + ":" + cell->getCellName();	break;
                            }else if ( cell->getCellName().length() > 0 && ini.catExists(tex + ":ANY_NAMED_CELL") ){
                                iniCat = tex + ":ANY_NAMED_CELL";			break;
                            }else if ( cell->getRegn().length() > 0 && ini.catExists(iniCat + ":" + cell->getRegn()) ){
                                iniCat = tex + ":" + cell->getRegn();		break;
                            }
                        }
                        if ( ini.catExists(tex) )
                            iniCat = tex;

                    }
                    if ( iniCat.length() == 0 ) continue; //didn;t manage to find a match


                    //check if we should place hrass
                    if ( ini.valueExists(iniCat, "bPlaceGrass") && fromString<bool>(ini.getValue(iniCat, "bPlaceGrass")) == 0 )
                        continue;//skip

                    //cache val.
                    bool bRandClump = fromString<bool>(ini.getValue(iniCat,"bRandClump"));

                    //find the grass mesh
                    std::string grassID;
                    if ( bRandClump )
                        grassID = getMesh(ini.getMeshList(iniCat),iniCat);

                    int gap = fromString<float>(ini.getValue(iniCat, "iGap"));

                    //allow a max gap of more than 512
                    if ( gap > 512){
                        float zz = 512 / (float)gap;
                        float xx = getRand(0, 100)/100;
                        if ( zz > xx ) continue;
                    }

                    //put multiple grass objects down
                    for ( int gx = 0; gx < 512; gx += gap ){
                        for ( int gy = 0; gy < 512; gy += gap ){

                            if ( !bRandClump )
                                grassID = getMesh(ini.getMeshList(iniCat),iniCat);

                            //calc the morrowind pos of the
                            float posx = tx * 512 + cx * 8192 + gx;
                            float posy = ty * 512 + cy * 8192 + gy;

                            //ad a random element
                            if ( fromString<bool>(ini.getValue(iniCat, "bPosRand")) ){
                                int min = fromString<float>(ini.getValue(iniCat, "fPosMin"));
                                int max = fromString<float>(ini.getValue(iniCat, "fPosMax"));
                                posx+=getRand(min,max);
                                posy+=getRand(min,max);
                            }

                            //get the correcrt cell, sometimes with the rand function, it goes over a cell boarder
                            int cellx = (int)floor(posx/(float)8192);//getCell(posx);
                            int celly = (int)floor(posy/(float)8192);

                            ES3::ESLandRef land2;
                            if ( cellx !=cx ||
                                celly != cy ){
                                    land2 = fc->getLand(cellx, celly);
                                    //assert (land2);
                                    if ( !land2 )continue;
                            }else{
                                land2 = land;
                            }

                            //check if we really should place it, or if it is on a no place texture
                            {
                                bool doContinue = false;

                                //for every banned texture
                                for ( int c = 0;ini.valueExists(iniCat, "sBan"+toString(c));++c ){
                                    std::string t = ini.getValue(iniCat, "sBan"+toString(c));
                                    float r = fromString<int>(ini.getValue(iniCat, "iBanOff"+toString(c)));

                                    for ( int p = 0; p < 4; p++){

                                        float tposx = posx;
                                        float tposy = posy;

                                        switch (p){
                                            case 0: tposx += r; break;
                                            case 1: tposx -= r; break;
                                            case 2: tposy += r; break;
                                            case 3: tposy -= r; break;
                                        }

                                        //check the cell again
                                        if ( (int)floor(tposx/(float)8192) !=cellx ||
                                            (int)floor(tposy/(float)8192) != celly ){
                                                continue; //ignore it.
                                        }


                                        float offx = (int)(tposx) % (int)8192;
                                        int squx = floor(offx/(float)512);
                                        if ( squx < 0 )
                                            squx = 16 + squx;


                                        float offy = (int)(tposy) % (int)8192;
                                        int squy = floor(offy/(float)512);
                                        if ( squy < 0 )
                                            squy = 16 + squy;

                                        std::vector< std::vector< uint16_t > > landTex2 = land2->getLandTextures();
                                        ES3::ESFileRef file2 = fc->getLandFile(cellx,celly);

                                        if ( landTex2[squx][squy] == 0 )
                                            continue;

                                        if ( !file2->getLTexExists(landTex2[squx][squy]-1) )
                                            continue;

                                        if (t==file2->getLTexPath(landTex2[squx][squy]-1)||
                                            t==file2->getLTex(landTex2[squx][squy]-1)->getID() ){
                                                //ak, this is not good.
                                                doContinue = true;
                                                break;
                                            } //if (t==file2->getLTexPath(land...
                                            if( doContinue == true ) break;
                                        }//for ( int p = 0; p < 4; p++){
                                        if( doContinue == true ) break;
                                    }//for ( int c = 0;ini.valueExists(iniCat, "sBan"+toString(c));++c ){
                                    if( doContinue == true ) continue;
                                }//{


                            float posZ = land2->getHeightAt(posx, posy) + mOffset;

                            ES3::Vector3 rot;
                            if ( fromString<bool>(ini.getValue(iniCat, "bAlignObjectNormalToGround")) ){
                                rot = land2->getAngleAt(posx, posy);
                            }
                            rot.z = getRand(0, 2*PI);

                            if ( ini.valueExists(iniCat, "fMinHeight"))
                                if ( posZ <= fromString<float>(ini.getValue(iniCat, "fMinHeight")))
                                    continue;

                            if ( ini.valueExists(iniCat, "fMaxHeight"))
                                if ( posZ >= fromString<float>(ini.getValue(iniCat, "fMaxHeight")))
                                    continue;

                            //get the scale of tthe object
                            float scale = 1;//config.scale;
                            if ( fromString<bool>(ini.getValue(iniCat, "bSclRand")) ) //option rot
                                scale = getRand(fromString<float>(ini.getValue(iniCat, "fSclMin")), fromString<float>(ini.getValue(iniCat, "fSclMax")));


                            buffWriteObjData(&buff, ++frmr, grassID, scale, posx,posy,posZ,rot.x,rot.y,rot.z);
                            hasGrassAdded = true;

                        }

                    }

                }
            }

            if ( hasGrassAdded ){//write the cell to the disk
                fileWriteCellHdr(&buff, ofs);
                fileWriteBuff(&buff, ofs);
                gNumRecords++;
            }

            buff.clear();


        }//	for

		//fix the number of records
		ofs.seekp(gNumRecordPos); 
		ofs.write((char*)&gNumRecords, 4);

		ofs.close();

		sendSuccess();

		return 0;


	};
public:

	GenThread(GUI* gui,
            const std::string& out,
            const std::string &idBase, 
            const std::string &iniLoc,
            std::vector<std::string> files,
            int offset) : wxThread(wxTHREAD_DETACHED), mGUI(gui), mOut(out), mIdBase(idBase), mIniLoc(iniLoc), mFiles(files), mOffset(offset){
	}
};
