#ifndef __FUNCS_H_
#define __FUNCS_H_

#include <cstdint>
#include <string>
#include <vector>

#include "EspWriter.h"

void buffWriteCellStart(RecordWriter& recordWriter, uint32_t flags, int32_t x, int32_t y, const std::string& cellName);
void fileWriteStatData(EspWriter& espWriter, const std::string& type, const std::string& id, const std::string& mesh);
void buffWriteObjData(RecordWriter& recordWriter, uint32_t frmr, const std::string& id, float scale, float px, float py, float pz, float rx, float ry, float rz  );
void fileWriteEspHdr(EspWriter& writer, uint32_t recordCount) ;

#endif //__FUNCS_H_