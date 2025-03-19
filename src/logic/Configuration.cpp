#include "Configuration.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

typedef boost::property_tree::basic_ptree<std::string, std::string> StringPropertyTree;

Selector parseSelector(const std::string& sectionName) {
    auto delim = sectionName.find(':');
    if (delim == std::string::npos) {
        return Selector(sectionName);
    } else {
        return Selector(sectionName.substr(0, delim), sectionName.substr(delim+1));
    }
}

std::vector<ObjectPlacementPossibility> getPlacements(const std::string& sectionName, const StringPropertyTree& sectionProperties) {
    float sumOfChances = 0;
    for(int i = 0;; ++i) {
        auto chance = sectionProperties.get_optional<int>("sChance" + std::to_string(i));
        if (!chance.has_value()) {
            break;
        }
        sumOfChances += static_cast<float>(chance.value());
    }

    std::vector<ObjectPlacementPossibility> placements;
    for(int i = 0;; ++i) {
        auto chance = sectionProperties.get_optional<int>("sChance" + std::to_string(i));
        if (!chance.has_value()) {
            break;
        }

        auto mesh = sectionProperties.get_optional<std::string>("sMesh" + std::to_string(i));
        auto id = sectionProperties.get_optional<std::string>("sId" + std::to_string(i));

        if (!mesh.has_value() && !id.has_value()) {
            throw std::runtime_error("Invalid configuration in section " + sectionName + ". sMesh or sId must be defined");
        } else if (mesh.has_value() && id.has_value()) {
            if (!id->empty()) {
                throw std::runtime_error("Invalid configuration in section " + sectionName + ". sMesh or sId cannot both be defined");
            }
        }

        placements.push_back(ObjectPlacementPossibility{
                .idOrMesh = id.has_value() && !id.value().empty() ? std::variant<ObjectId, Mesh>(ObjectId(id.value())) : std::variant<ObjectId, Mesh>(Mesh(mesh.value())),
                // Technically dividing by the sum of chances is not required, but might make it more obvious what is
                // going on in the logged configuration
                .chance = static_cast<float>(chance.value()) / sumOfChances,
                .deprecatedId = i,
        });
    }
    return placements;
}


std::vector<PlacementExclusions> getExclusions(const std::string& sectionName, const StringPropertyTree& sectionProperties) {
    std::vector<PlacementExclusions> exclusions;
    for(int i = 0;; i++) {
        auto excludedTexture = sectionProperties.get_optional<std::string>("sBan" + std::to_string(i));
        if (!excludedTexture.has_value()) {
            break;
        }
        boost::optional<int> offset = sectionProperties.get_optional<int>("iBanOff" + std::to_string(i));
        if (!offset.has_value()) {
            throw std::runtime_error("Invalid configuration in section " + sectionName + ". iBanOff" + std::to_string(i) + " must be defined" );
        }

        exclusions.push_back({
            .texture = excludedTexture.value(),
            .distanceFromTexture = offset.value(),
        });
    }
    return exclusions;
}

static Bounds getBounds(
        const std::string& minKey,
        const std::string& maxKey,
        const StringPropertyTree& sectionProperties,
        float defaultMin,
        float defaultMax
) {
    return Bounds {
            .min = sectionProperties.get<float>(minKey, defaultMin),
            .max = sectionProperties.get<float>(maxKey, defaultMax),
    };
}


static Bounds getOptBounds(
        const std::string& key,
        const std::string& minKey,
        const std::string& maxKey,
        const StringPropertyTree& sectionProperties,
        float defaultMin,
        float defaultMax
) {
    bool enabled = sectionProperties.get<bool>(key, true);
    if (enabled) {
        return getBounds(minKey, maxKey, sectionProperties, defaultMin, defaultMax);
    } else {
        return Bounds {.min = defaultMin,.max = defaultMax,};
    }
}

Configuration loadConfigurationFromIni(const std::shared_ptr<spdlog::logger> logger, const std::filesystem::path& path) {
    std::map<Selector, Behaviour> configuration;

    boost::property_tree::ptree pt;
    try {
        boost::property_tree::ini_parser::read_ini(path.string(), pt);
    } catch (boost::property_tree::ini_parser_error& e) {
        std::throw_with_nested(std::runtime_error("Failed to load ini file at " + path.string()));
    }

    for (auto& section : pt) {
        if (section.first == "global") {
            continue;
        }
        auto selector = parseSelector(section.first);
        auto sectionProperties = section.second;

        if (!sectionProperties.get<bool>("bPlaceGrass", true)) {
            configuration.emplace(selector, Behaviour { .placeMeshesBehaviour = std::nullopt });
            continue;
        }

        int gap;
        try {
            gap = sectionProperties.get<int>("iGap");
        } catch (boost::property_tree::ptree_bad_path& e) {
            std::throw_with_nested(std::runtime_error("Failed to read property iGap in section " + section.first + " in the file " + path.string()));
        }

        PlaceMeshesBehaviour behavior = {
                .clump = sectionProperties.get<bool>("bRandClump", false),
                .gap = gap,
                .alignToNormal = sectionProperties.get<bool>("bAlignObjectNormalToGround", true),
                .heights = getBounds("fMinHeight", "fMaxHeight", sectionProperties, -std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()),
                .positionRandomization = getOptBounds("bPosRand", "fPosMin", "fPosMax", sectionProperties, 0, 0),
                .scaleRandomization = getOptBounds("bSclRand", "fSclMin", "fSclMax", sectionProperties, 1, 1),
                .placements = getPlacements(section.first, sectionProperties),
                .exclusions = getExclusions(section.first, sectionProperties),
        };

        if (behavior.placements.empty()) {
            logger->warn("No placements found for section {} in {}. This will be ignored", section.first, path.string());
        } else {
            configuration.emplace(selector, Behaviour{.placeMeshesBehaviour = std::make_optional(behavior)});
        }
    }

    return Configuration(
            configuration,
            pt.get<int>("global.iZPositionModifier", 10),
            pt.get<std::string>("global.sObjectPrefix", "GRS_")
    );
}

std::ostream& operator<<(std::ostream& os, const Bounds& bounds) {
    os << "{Bounds min: " << bounds.min << " max: " << bounds.max << "}";
    return os;
}

std::ostream& operator<<(std::ostream& os, const PlacementExclusions& exclusions) {
    if (exclusions.distanceFromTexture.has_value()) {
        os << "{PlacementExclusions texture: " << exclusions.texture << " distanceFromTexture: "
           << exclusions.distanceFromTexture.value() << "}";
    } else {
        os << "{PlacementExclusions texture: " << exclusions.texture << " distanceFromTexture: "
           << "{optional empty}" << "}";
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const ObjectPlacementPossibility& possibility) {
    os << "{ObjectPlacementPossibility idOrMesh: ";
    if (std::holds_alternative<ObjectId>(possibility.idOrMesh)) {
        os << std::get<ObjectId>(possibility.idOrMesh);
    } else {
        os << std::get<Mesh>(possibility.idOrMesh);
    }
    os << " chance: " << possibility.chance << " deprecatedId: " << possibility.deprecatedId << "}";
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::optional<PlaceMeshesBehaviour>& behaviour) {
    if (behaviour.has_value()) {
        os << behaviour.value();
    } else {
        os << "[optional empty]";
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Behaviour& behaviour) {
    os << "{Behaviour placeMeshesBehaviour: " << behaviour.placeMeshesBehaviour << "}";
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<ObjectPlacementPossibility>& possibilities) {
    os << "[";
    for (const auto& item: possibilities) {
        os << item << ", ";
    }
    os << "]";
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<PlacementExclusions>& exclusions) {
    os << "[";
    for (const auto& item: exclusions) {
        os << item << ", ";
    }
    os << "]";
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::map<Selector, Behaviour>& behaviours) {
    os << "{";
    for (const auto& item: behaviours) {
        os << item.first << " = " << item.second << ", ";
    }
    os << "}";
    return os;
}


std::ostream& operator<<(std::ostream& os, const PlaceMeshesBehaviour& behaviour) {
    os << "{PlaceMeshesBehaviour clump: " << behaviour.clump << " gap: " << behaviour.gap << " alignToNormal: " << behaviour.alignToNormal
       << " heights: " << behaviour.heights << " positionRandomization: " << behaviour.positionRandomization
       << " scaleRandomization: " << behaviour.scaleRandomization << " placements: " << behaviour.placements
       << " exclusions: " << behaviour.exclusions << "}";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Configuration& configuration) {
    os << "{Configuration behaviours: " << configuration.behaviours << " globalOffset: " << configuration.globalOffset
       << " objectPrefix: " << configuration.objectPrefix << "}";
    return os;
}
