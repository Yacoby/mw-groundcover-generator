#ifndef __INI_H_
#define __INI_H_

#include <string>
#include <map>
#include <list>
#include <optional>
#include <filesystem>

class Ini {

protected:
    typedef std::map<std::string, std::map<std::string, std::string> > IniValues;
    typedef std::map<std::string, std::map<std::string, std::string> >::iterator IniValuesItr;
    IniValues mValues;
public:

    bool catExists(const std::string &cat);

    bool valueExists(const std::string &cat, const std::string &key);

    std::optional<const std::reference_wrapper<std::string>> getOptionalValue(const std::string &cat, const std::string &key);

    const std::string& getValue(const std::string &cat, const std::string &key);

    bool load(const std::filesystem::path &f);
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
    std::list<std::string> getTextureList();

    /**
    * @brief gets a list of meshes used by a texture
    */
    std::list<GrassMesh> getMeshList(const std::string &cat);
};

#endif //__INI_H_
