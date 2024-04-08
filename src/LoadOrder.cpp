#include "LoadOrder.h"

#include <filesystem>
#include <vector>
#include <queue>
#include <algorithm>

#include <boost/algorithm/string/case_conv.hpp>

#include "EspReader.h"

namespace fs = std::filesystem;

bool isEsp(const fs::path& path) {
    std::string extension = path.extension().string();
    boost::algorithm::to_lower(extension);
    return (extension == ".esp");
}

std::set<std::string> LastModifiedLoadOrder::getDependencies(const fs::path& path) {
    std::set<std::string> dependencies;
    auto reader = EspReader(path);
    const auto header = *reader.begin();
    for (auto &subRecord: header) {
        if (subRecord.type == "MAST") {
            dependencies.insert(subRecord.readNullTerminatedString());
        }
    }
    return dependencies;
}

void LastModifiedLoadOrder::insert(const std::vector<fs::path>& paths) {
    for (const auto &path: paths) {
        std::string fileName = path.filename().string();

        remove(fileName);
        loadOrder.push_back(std::make_unique<Plugin>(Plugin{
                .fileName = fileName,
                .isEsp = isEsp(path),
                .path = path,
                .lastModified = last_write_time(path),
                .dependencies = isEsp(path) ? std::set<std::string>() : getDependencies(path),
        }));
    }
    topologicalSort();
}

void LastModifiedLoadOrder::remove(const std::string& fileName) {
    loadOrder.erase(std::remove_if(loadOrder.begin(),
                                   loadOrder.end(),
                                   [fileName](const std::unique_ptr<Plugin>& item) {
                                       return item->fileName == fileName;
                                   }), loadOrder.end());
}

void LastModifiedLoadOrder::topologicalSortImpl(
        std::set<std::string>& pluginsAdded,
        std::vector<std::unique_ptr<Plugin>>& inputQueue,
        std::vector<std::unique_ptr<Plugin>>& newLoadOrder,
        std::function<bool(std::unique_ptr<Plugin>&)> predicate
) {
    auto comp = [](const std::unique_ptr<Plugin>& a, const std::unique_ptr<Plugin>& b) {
        return std::tie(a->isEsp, a->lastModified) > std::tie(b->isEsp, b->lastModified);
    };
    std::priority_queue<std::unique_ptr<Plugin>, std::vector<std::unique_ptr<Plugin>>, decltype(comp)> queue(comp);

    do {
        while (!queue.empty()) {
            pluginsAdded.insert(queue.top().get()->path.filename().string());
            newLoadOrder.push_back(std::move(const_cast<std::unique_ptr<Plugin>&>(queue.top())));
            queue.pop();
        }

        for (auto it = inputQueue.begin(); it != inputQueue.end();) {
            if (!predicate(*it)) {
                ++it;
                continue;
            }

            std::set<std::string> diff;
            std::set_difference((*it)->dependencies.begin(), (*it)->dependencies.end(), pluginsAdded.begin(), pluginsAdded.end(),
                                std::inserter(diff, diff.end()));
            if (diff.empty()) {
                queue.push(std::move(*it));
                inputQueue.erase(it);
            } else {
                ++it;
            }
        }
    } while (!queue.empty());
}

void LastModifiedLoadOrder::topologicalSort() {
    std::set<std::string> allPluginNames;
    std::set<std::string> allDependencies;
    for (const auto &item: loadOrder) {
        allPluginNames.insert(item->path.filename().string());
        allDependencies.insert(item->dependencies.begin(), item->dependencies.end());
    }

    std::set<std::string> pluginsAdded;
    std::set_difference(allDependencies.begin(), allDependencies.end(), allPluginNames.begin(), allPluginNames.end(),
                        std::inserter(pluginsAdded, pluginsAdded.end()));

    std::vector<std::unique_ptr<Plugin>> newLoadOrder;
    topologicalSortImpl(pluginsAdded, loadOrder, newLoadOrder, [](auto& item) { return !item->isEsp; });
    topologicalSortImpl(pluginsAdded, loadOrder, newLoadOrder, [](auto& item) { return item->isEsp; });

    if (!loadOrder.empty()) {
        throw std::runtime_error("Un-sortable list of plugins. A cycle exists in the dependencies");
    }

    std::copy(std::make_move_iterator(newLoadOrder.begin()),
              std::make_move_iterator(newLoadOrder.end()),
              std::back_inserter(loadOrder));
}

void FixedLoadOrder::insert(const std::vector<fs::path> &paths) {
    if (!loadOrder.empty()) {
        throw std::runtime_error("Cannot insert more elements into a fixed load order");
    }

    for (const auto &path: paths) {
        std::string fileName = path.filename().string();

        loadOrder.push_back(std::make_unique<Plugin>(Plugin{
                .fileName = fileName,
                .isEsp = isEsp(path),
                .path = path,
                .lastModified = last_write_time(path),
                .dependencies = std::set<std::string>(),
        }));
    }
}

void FixedLoadOrder::remove(const std::string &fileName) {
    loadOrder.erase(std::remove_if(loadOrder.begin(),
                                   loadOrder.end(),
                                   [fileName](const std::unique_ptr<Plugin>& item) {
                                       return item->fileName == fileName;
                                   }), loadOrder.end());
}