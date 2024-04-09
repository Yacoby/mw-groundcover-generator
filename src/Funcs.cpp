#include "Funcs.h"

#include <fstream>
#include "EspWriter.h"


/**
* @param buff the buffer to write the data to
* @param flags, x, y cell data, x,y is the grid cords
*/

void
buffWriteCellStart(RecordWriter& recordWriter, uint32_t flags, int32_t x, int32_t y, const std::string& cellName) {

    recordWriter.writeSubRecordAndData(RecordType("NAME"), cellName);

    /*
    DATA = Cell Data
    long Flags
    0x01 = Interior?
    0x02 = Has Water
    0x04 = Illegal to Sleep here
    0x80 = Behave like exterior (Tribunal)
    long GridX
    long GridY
    */

    auto sub = recordWriter.writeSubRecord(RecordType("DATA"));
    sub.write(0);
    sub.write(x);
    sub.write(y);

    sub.close();
}

/*
Record
4 bytes: char Name[4]
4-byte record name string (not null-terminated)
4 bytes: long Size    
Size of the record not including the 16 bytes of header data.
4 bytes: long Header1
Unknown value, usually 0 (deleted/ignored flag?).
4 bytes: long Flags
Record flags.
0x00002000 = Blocked
0x00000400 = Persistant
? bytes: SubRecords[]
All records are composed of a variable number of sub-records. There
is no sub-record count, just use the record Size value to determine
when to stop reading a record.
*/

void fileWriteStatData(EspWriter &espWriter, const std::string &type, const std::string &id, const std::string &mesh) {
    auto recordWriter = espWriter.writeRecord(RecordType(type), 0, 0);
    recordWriter.writeSubRecordAndData("NAME", id);
    recordWriter.writeSubRecordAndData("MODL", mesh);
    recordWriter.close();
}

/*

*/
void
buffWriteObjData(RecordWriter& recordWriter, uint32_t frmr, const std::string &id, float scale, float px, float py, float pz, float rx,
                 float ry, float rz) {
    /*
    FRMR = Object Index (starts at 1) (4 bytes, long)
    This is used to uniquely identify objects in the cell.  For new files the
    index starts at 1 and is incremented for each new object added.  For modified
    objects the index is kept the same.
    */
    recordWriter.writeSubRecordAndData<uint32_t>(RecordType("FRMR"), frmr);
    recordWriter.writeSubRecordAndData(RecordType("NAME"), id);

    if (scale != 1) {
        recordWriter.writeSubRecordAndData<float>(RecordType("XSCL"), scale);
    }

    /*
    DATA = Ref Position Data (24 bytes)
    float XPos
    float YPos
    float ZPos
    float XRotate
    float YRotate
    float ZRotate
    */
    auto srw = recordWriter.writeSubRecord(RecordType("DATA"), 24);
    srw.write<float>(px);
    srw.write<float>(py);
    srw.write<float>(pz);
    srw.write<float>(rx);
    srw.write<float>(ry);
    srw.write<float>(rz);
    srw.close();
}

/*
0: TES3 = 1 count
Main Header Record, 308 Bytes

HEDR (300 bytes)
4 bytes, float Version (1.2)
4 bytes, long Unknown (1)
32 Bytes, Company Name string
256 Bytes, ESM file description?
4 bytes, long NumRecords (48227)
MAST = string, variable length
Only found in ESP plugins and specifies a master file that the plugin
requires.  Can occur multiple times.  Usually found just after the TES3
record.
DATA = 8 Bytes	long64 MasterSize
Size of the previous master file in bytes (used for version tracking of plugin).
The MAST and DATA records are always found together, the DATA following the MAST record
that it refers to.
*/

void fileWriteEspHdr(EspWriter& writer, uint32_t recordCount) {
    auto recordWriter = writer.writeRecord(RecordType("TES3"), 0, 0);
    auto subRecordWriter = recordWriter.writeSubRecord(RecordType("HEDR"));
    subRecordWriter.write<uint32_t>(1.2);
    subRecordWriter.write<uint32_t>(0);

    for (int i = 0; i < 32 + 256; i++) {
        subRecordWriter.write<char>(0);
    }

    subRecordWriter.write<uint32_t>(recordCount);

    subRecordWriter.close();
    recordWriter.close();
}