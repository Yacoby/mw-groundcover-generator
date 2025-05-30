#ifndef MW_MESH_GEN_CONFIGURATION_H
#define MW_MESH_GEN_CONFIGURATION_H

#include <string>
#include <optional>
#include <vector>
#include <map>
#include <filesystem>
#include <variant>
#include <ostream>

#include "spdlog/spdlog.h"

#include "math/Angle.h"

class ObjectId {
    const std::string value;
public:
    ObjectId(const std::string& v) : value(v) {}
    const std::string& get() const { return value; }

    friend std::ostream& operator<<(std::ostream& os, const ObjectId& id) {
        os << "{ObjectId id:" << id.value << "}";
        return os;
    }
};


class Mesh {
    const std::string value;
public:
    Mesh(const std::string& v) : value(v) {}
    const std::string& get() const { return value; }

    friend std::ostream& operator<<(std::ostream& os, const Mesh& mesh) {
        os << "{Mesh value: " << mesh.value << "}";
        return os;
    }
};

struct Bounds {
    friend std::ostream& operator<<(std::ostream& os, const Bounds& bounds);

    const float min;
    const float max;
};

struct PlacementExclusions {
    friend std::ostream& operator<<(std::ostream& os, const PlacementExclusions& exclusions);

    const std::string texture;
    const std::optional<int> distanceFromTexture;
};

struct ObjectPlacementPossibility {
    friend std::ostream& operator<<(std::ostream& os, const ObjectPlacementPossibility& possibility);

    const std::variant<ObjectId, Mesh> idOrMesh;
    const float chance;

    const bool alignToNormal;
    const Bounds heights;
    const Bounds positionRandomization;
    const Bounds scaleRandomization;
    const Angle minimumAngle;
    const Angle maximumAngle;
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

    bool operator==(const Selector& rhs) const {
        return texture == rhs.texture &&
               selector == rhs.selector;
    }

    bool operator!=(const Selector& rhs) const {
        return !(rhs == *this);
    }

    friend std::ostream& operator<<(std::ostream& os, const Selector& aSelector) {
        os << "{Selector " << aSelector.toLegacyCategory() << "}";
        return os;
    }
};


struct PlaceMeshesBehaviour {
    friend std::ostream& operator<<(std::ostream& os, const PlaceMeshesBehaviour& behaviour);

    const bool clump;
    const int gap;

    const std::vector<ObjectPlacementPossibility> placements;
    const std::vector<PlacementExclusions> exclusions;
};


struct Behaviour {
    friend std::ostream& operator<<(std::ostream& os, const Behaviour& behaviour);

    const std::optional<PlaceMeshesBehaviour> placeMeshesBehaviour;
};

class Configuration {
    const std::map<Selector, Behaviour> behaviours;
public:
    const float globalOffset;
    const std::string objectPrefix;

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

    Configuration(const std::map<Selector, Behaviour>& map, int offset, const std::string& prefix) : behaviours(map), globalOffset(offset), objectPrefix(prefix) {}

    friend std::ostream& operator<<(std::ostream& os, const Configuration& configuration);
};


Configuration loadConfigurationFromIni(const std::shared_ptr<spdlog::logger> logger, const std::filesystem::path& path);

#endif //MW_MESH_GEN_CONFIGURATION_H
