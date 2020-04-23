#pragma once

#include <unordered_map>

/// Helper class for storing and managing SFML resources.
template<typename Resource>
class ResourceManager
{
private:
    std::string path_prefix;
    std::string extension;
    std::unordered_map<std::string, Resource> resources;

    std::string get_path(const std::string& name)
    {
        return path_prefix + "/" + name + "." + extension;
    }

public:
    ResourceManager(const std::string& path_prefix, const std::string& extension)
            : path_prefix(path_prefix), extension(extension)
    {}

    Resource& get(const std::string& name)
    {
        if(resources.find(name) == resources.end())
        {
            Resource resource;
            resource.loadFromFile(get_path(name));
            resources.insert({name, resource});
        }

        return resources.at(name);
    }
};
