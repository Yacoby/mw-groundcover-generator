#ifndef MW_MESH_GEN_LOADORDER_H
#define MW_MESH_GEN_LOADORDER_H

#include <filesystem>
#include <vector>
#include <set>

namespace fs = std::filesystem;

class LoadOrder {
public:
    struct Plugin {
        const std::string fileName;
        const bool isEsp;
        const fs::path path;
        const fs::file_time_type lastModified;
        const std::set<std::string> dependencies;
    };

    void insert(const std::vector<fs::path>& paths);

    void remove(const std::string& fileName);

    void clear() {
        loadOrder.clear();
    }

    size_t indexOf(const std::string& name) const;

    const std::vector<std::unique_ptr<Plugin>>& get() const {
        return loadOrder;
    }
private:
    std::vector<std::unique_ptr<Plugin>> loadOrder;

    bool isEsp(const fs::path& path) {
        return (path.extension().string() == ".esp");
    }

    void topologicalSort();

    void topologicalSortImpl(
            std::set<std::string>& pluginsAdded,
            std::vector<std::unique_ptr<Plugin>>& inputQueue,
            std::vector<std::unique_ptr<Plugin>>& newLoadOrder,
            std::function<bool(std::unique_ptr<Plugin>&)> predicate
    );

    std::set<std::string> getDependencies(const fs::path& path);

};

#endif //MW_MESH_GEN_LOADORDER_H
