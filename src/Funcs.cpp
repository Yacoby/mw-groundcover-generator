#include "Funcs.h"

#include <fstream>

uint32_t gNumRecords = 0;
long gNumRecordPos = -1;

/**
* @brief writes the cell data, copying it directly from the buffer
*/
void fileWriteBuff(Buff *buff, std::ofstream &ofs) {
    int size = buff->getSize();
    for (int i = 0; i < size; i++) {
        ofs.put(buff->getByte(i));
    }
}


/**
* @param buff the buffer to write the data to
* @param name cell id string
* @param flags, x, y cell data, x,y is the grid cords
* @param rgn the reginon string
* @param col map colour of the cell
*/

void
buffWriteCellStart(Buff *buff, const std::string &name, uint32_t flags, uint32_t x, uint32_t y, const std::string &rgn,
                   uint32_t col) {
    //NAME = Cell ID string.
    buff->writeType("NAME");
    buff->writeData(name);

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
    buff->writeType("DATA");
    buff->writeRaw((uint32_t) 12);
    buff->writeRaw(flags);
    buff->writeRaw(x);
    buff->writeRaw(y);
    //buff->writeData(flags);	buff->writeData(x);	buff->writeData(y);


    if (rgn.length() > 0) {
        ////RGNN = Region name string
        buff->writeType("RGNN");
        buff->writeData(rgn);
    }

    if (col != 0) {
        ////NAM5 = Map Color (4 bytes, long, COLORREF)
        buff->writeType("NAM5");
        buff->writeData(col);
    }
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

void fileWriteStatData(std::ofstream &ofs, const std::string &type, const std::string &id, const std::string &mesh,
                       const std::string &name, const std::string &script) {
    Buff buff;

    buff.writeType("NAME");
    buff.writeData(id);
    buff.writeType("MODL");
    buff.writeData(mesh);

    if (type != "STAT") {
        buff.writeType("FNAM");
        buff.writeData(name);
        buff.writeType("SCPT");
        buff.writeData(script);
    }

    long len = buff.getSize();
    ofs.write(type.c_str(), 4);
    ofs.write((char *) &len, 4);
    len = 0;
    ofs.write((char *) &len, 4);
    ofs.write((char *) &len, 4);

    fileWriteBuff(&buff, ofs);

    gNumRecords++;


}

/*

*/
void
buffWriteObjData(Buff *buff, uint32_t frmr, const std::string &id, float scale, float px, float py, float pz, float rx,
                 float ry, float rz) {
    /*
    FRMR = Object Index (starts at 1) (4 bytes, long)
    This is used to uniquely identify objects in the cell.  For new files the
    index starts at 1 and is incremented for each new object added.  For modified
    objects the index is kept the same.
    */
    buff->writeType("FRMR");
    buff->writeData(frmr);

    //NAME = Object ID string
    buff->writeType("NAME");
    buff->writeData(id);

    if (scale != 1) {
        //XSCL = Scale (4 bytes, float) Static
        buff->writeType("XSCL");
        buff->writeData(scale);
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
    buff->writeType("DATA");
    buff->writeRaw((uint32_t) 24);
    buff->writeRaw(px);
    buff->writeRaw(py);
    buff->writeRaw(pz);
    buff->writeRaw(rx);
    buff->writeRaw(ry);
    buff->writeRaw(rz);
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

void fileWriteEspHdr(std::ofstream &ofs) {
    const float ver = 1.2;
    uint32_t unk = 0;
    char tes3[] = "TES3";

    //buld header
    Buff b;
    b.writeType("HEDR");
    b.writeRaw((uint32_t) 300);
    b.writeRaw((uint32_t) ver);
    b.writeRaw((uint32_t) 0);

    for (int i = 0; i < 32 + 256; i += 4) {
        b.writeRaw((uint32_t) 0);
    }


    gNumRecordPos = b.getSize();
    b.writeRaw(gNumRecords);

    //header finished



    unk = b.getSize();
    ofs.write(tes3, 4);
    ofs.write((char *) &unk, 4);
    unk = 0;
    ofs.write((char *) &unk, 4);
    ofs.write((char *) &unk, 4);

    gNumRecordPos += 4 + 4 + 4 + 4;

    for (int i = 0; i < b.getSize(); i++) {
        char c = b.getByte(i);
        ofs.write((char *) &c, 1);
    }
}

/**
* @brief writes the 16 bytes of header for the cell rec
*/
void fileWriteCellHdr(Buff *buff, std::ofstream &ofs) {
    uint32_t size = buff->getSize();

    ofs.write((char *) "CELL", 4);
    ofs.write((char *) &size, 4);

    uint32_t junk = 0; //set junk
    ofs.write((char *) &junk, 4); //junk
    ofs.write((char *) &junk, 4); //junk

}