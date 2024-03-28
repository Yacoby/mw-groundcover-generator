#ifndef __INI_H_
#define __INI_H_

#include <boost/algorithm/string.hpp>

class Ini {

    std::string mCurBlok;
protected:
    typedef std::map<std::string, std::map<std::string, std::string> > IniValues;
    typedef std::map<std::string, std::map<std::string, std::string> >::iterator IniValuesItr;
    IniValues mValues;
public:
    Ini() : mCurBlok("unknown") {}

    bool catExists(const std::string &cat) {
        return mValues.find(cat) != mValues.end() ? true : false;
    }

    bool valueExists(const std::string &cat, const std::string &key) {
        if (!catExists(cat)) return false;
        return mValues[cat].find(key) != mValues[cat].end() ? true : false;
    }

    std::string getValue(const std::string &cat, const std::string &key) {
        if (!valueExists(cat, key)) {
            std::string msg = "When getting ini value, value doesn't exist\nExpecting category \"" + cat +
                              "\" to contain key \"" + key + "\"";
            throw std::invalid_argument(msg);
        }
        return mValues[cat][key];
    }

    bool load(const std::string &f) {
        std::ifstream ifs(f.c_str());
        if (!ifs.is_open()) return false;


        std::string line, key, val;
        while (!ifs.eof()) {
            std::getline(ifs, line);
            boost::trim(line);
            boost::trim_right_if(line, boost::is_any_of("\r\n\0"));

            if (line.find(";") != -1) continue; //comment


            if (line.find("[") == 0 && line.rfind("]") != -1) {
                mCurBlok = line.substr(1, line.rfind("]") - 1);
                continue;
            }


            size_t pos = line.find("=");
            if (pos == -1) continue; //not a valid value

            key = line.substr(0, pos);
            val = line.substr(pos + 1, line.length() - pos + 1);

            mValues[mCurBlok][key] = val;
        }
        ifs.close();
        return true;
    }
};

class GrassIni2 : public Ini {
public:
    struct GrassMesh {
        std::string mesh;
        std::string objectID;
        float chance;
        int id;
    };
public:

    /**
    * gets a list of all the texutes used
    */
    std::list<std::string> getTextureList() {
        std::list<std::string> lst;
        for (IniValuesItr iter = mValues.begin(); iter != mValues.end(); ++iter) {
            lst.push_back(iter->first);
        }
        return lst;
    }

    /**
    * @brief gets a list of meshes used by a texture
    */
    std::list<GrassMesh> getMeshList(const std::string &cat) {
        std::list<GrassMesh> lst;
        if (!catExists(cat)) return lst;
        for (int c = 0; valueExists(cat, "sMesh" + toString(c)); ++c) {
            GrassMesh g;
            g.mesh = getValue(cat, "sMesh" + toString(c));
            g.chance = fromString<float>(getValue(cat, "sChance" + toString(c)));
            if (valueExists(cat, "sID" + toString(c))) {
                g.objectID = getValue(cat, "sID" + toString(c));
            }
            g.id = c;
            lst.push_back(g);
        }
        return lst;
    }
};

#endif //__INI_H_
