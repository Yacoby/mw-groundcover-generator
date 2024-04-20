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
                .chance = chance.value(),
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

Configuration loadConfigurationFromIni(const std::filesystem::path& path) {
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

        configuration.emplace(selector, Behaviour {.placeMeshesBehaviour = std::make_optional(behavior)});
    }

    return Configuration(
            configuration,
            pt.get<int>("global.iZPositionModifier", 10),
            pt.get<std::string>("global.sObjectPrefix", "GRS_")
    );
}

