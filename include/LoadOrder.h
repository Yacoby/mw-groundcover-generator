#ifndef MW_MESH_GEN_LOADORDER_H
#define MW_MESH_GEN_LOADORDER_H

#include <filesystem>
#include <vector>
#include <set>
#include <functional>

namespace fs = std::filesystem;

struct Plugin {
    const std::string fileName;
    const bool isEsp;
    const fs::path path;
    const fs::file_time_type lastModified;
    const std::set<std::string> dependencies;
};

class LoadOrder {
public:
    virtual ~LoadOrder() { };

    virtual void insert(const std::vector<fs::path>& paths) = 0;

    virtual void remove(const std::string& fileName) = 0;

    virtual void clear() = 0;

    virtual const std::vector<std::unique_ptr<Plugin>>& get() const  = 0;
};

class LastModifiedLoadOrder : public LoadOrder {
public:
    ~LastModifiedLoadOrder() {}

    void insert(const std::vector<fs::path>& paths) override;

    void remove(const std::string& fileName) override;

    void clear() override {
        loadOrder.clear();
    }

    const std::vector<std::unique_ptr<Plugin>>& get() const override {
        return loadOrder;
    }
private:
    std::vector<std::unique_ptr<Plugin>> loadOrder;

    void topologicalSort();

    void topologicalSortImpl(
            std::set<std::string>& pluginsAdded,
            std::vector<std::unique_ptr<Plugin>>& inputQueue,
            std::vector<std::unique_ptr<Plugin>>& newLoadOrder,
            std::function<bool(std::unique_ptr<Plugin>&)> predicate
    );

    std::set<std::string> getDependencies(const fs::path& path);
};

class FixedLoadOrder : public LoadOrder {
public:
    ~FixedLoadOrder() {}

    void insert(const std::vector<fs::path>& paths) override;

    void remove(const std::string& fileName) override;

    void clear() override {
        loadOrder.clear();
    }

    const std::vector<std::unique_ptr<Plugin>>& get() const override {
        return loadOrder;
    }
private:
    std::vector<std::unique_ptr<Plugin>> loadOrder;
};

#endif //MW_MESH_GEN_LOADORDER_H
