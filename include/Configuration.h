#ifndef MW_MESH_GEN_CONFIGURATION_H
#define MW_MESH_GEN_CONFIGURATION_H

#include <string>
#include <optional>
#include <vector>
#include <map>
#include <filesystem>

class ObjectId {
    const std::string value;
public:
    ObjectId(const std::string& v) : value(v) {}
    const std::string& get() const { return value; }
};


class Mesh {
    const std::string value;
public:
    Mesh(const std::string& v) : value(v) {}
    const std::string& get() const { return value; }
};

struct Bounds {
    const float min;
    const float max;
};

struct PlacementExclusions {
    const std::string texture;
    const std::optional<int> distanceFromTexture;
};

struct ObjectPlacementPossibility {
    const std::variant<ObjectId, Mesh> idOrMesh;
    const int chance;
    const int deprecatedId;
};

class Selector {
public:
    const std::string texture;
    const std::optional<std::string> selector;

    Selector(const std::string& tex) : texture(tex) {};
    Selector(const std::string& tex, const std::string& sel) : texture(tex), selector(sel) {};

    bool operator< (const Selector& rhs) const {
        return std::tie(texture, selector) < std::tie(rhs.texture, rhs.selector);
    }

    std::string toLegacyCategory() const {
        if (selector.has_value()) {
            return texture + ":" + selector.value();
        } else {
            return texture;
        }
    }
};


struct PlaceMeshesBehaviour {
    const bool clump;

    const int gap;
    const bool alignToNormal;

    const Bounds heights;

    const Bounds positionRandomization;
    const Bounds scaleRandomization;

    const std::vector<ObjectPlacementPossibility> placements;
    const std::vector<PlacementExclusions> exclusions;
};


struct Behaviour {
    const std::optional<PlaceMeshesBehaviour> placeMeshesBehaviour;
};

class Configuration {
    const std::map<Selector, Behaviour> behaviours;
public:
    std::optional<std::reference_wrapper<const Behaviour>> get(const Selector& selector) const {
        auto result = behaviours.find(selector);
        if (result == behaviours.end()) {
            return std::nullopt;
        }
        return result->second;
    }

    std::map<Selector, Behaviour>::const_iterator begin() const {
        return behaviours.begin();
    }

    std::map<Selector, Behaviour>::const_iterator end() const {
        return behaviours.end();
    }

    Configuration(const std::map<Selector, Behaviour>& map) : behaviours(map) {}
};


Configuration loadConfigurationFromIni(const std::filesystem::path& path);

#endif //MW_MESH_GEN_CONFIGURATION_H
