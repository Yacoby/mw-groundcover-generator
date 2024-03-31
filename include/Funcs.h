#ifndef __FUNCS_H_
#define __FUNCS_H_

#include "includes.h"

template<typename T>
std::string toString(T s) {
    std::ostringstream oss;
    oss << s;
    return oss.str();
}

template<typename T>
T fromString(const std::string &s) {
    std::istringstream is(s);
    T t;
    is >> t;
    return t;
}


class Buff {
    std::vector<char> mBuffer;

    void writeToBuffer(const char *data, int bytes) {
#ifdef _DEBUG
        int c = 0;
#endif
        for (int i = 0; i < bytes; i++) {
            mBuffer.push_back(data[i]);
#ifdef _DEBUG
            c++;
#endif
        }
#ifdef _DEBUG
        assert(c == bytes);
#endif
    }

public:
    char getByte(int index) {
#ifdef _DEBUG
        return mBuffer.at(index);
#else
        return mBuffer[index];
#endif
    }

    void clear() { mBuffer.clear(); }

    int getSize() { return (int) mBuffer.size(); }


    void writeType(const char *d) { writeToBuffer(d, 4); }

    void writeData(const std::string &d) {
        long len = (long) d.length() + 1; //+1 for thew nill
        writeToBuffer((char *) &len, 4);
        writeToBuffer(d.c_str(), (int) d.length());

        len = 0;
        writeToBuffer((char *) &len, 1);
    }

    void writeData(const uint32_t d) {
        long len = 4;
        writeToBuffer((char *) &len, 4);
        writeToBuffer((char *) &d, 4);
    }

    void writeData(const float d) {
        long len = 4;
        writeToBuffer((char *) &len, 4);
        writeToBuffer((char *) &d, 4);
    }

    void writeRaw(const std::string &d) { writeToBuffer(d.c_str(), (int) d.length() - 1); }

    void writeRaw(const uint32_t d) { writeToBuffer((char *) &d, 4); }

    void writeRaw(const float d) { writeToBuffer((char *) &d, 4); }

};

void fileWriteBuff(Buff* buff, std::ofstream& ofs);
void buffWriteCellStart(Buff* buff, const std::string& name, uint32_t flags, uint32_t x, uint32_t y, const std::string& rgn, uint32_t col);
void fileWriteStatData(std::ofstream& ofs,const std::string& type, const std::string& id, const std::string& mesh, const std::string& name, const std::string& script);
void buffWriteObjData(Buff* buff, uint32_t frmr, const std::string& id, float scale, float px, float py, float pz, float rx, float ry, float rz  );
void fileWriteEspHdr(std::ofstream& ofs);
void fileWriteCellHdr(Buff* buff, std::ofstream& ofs);

#endif //__FUNCS_H_