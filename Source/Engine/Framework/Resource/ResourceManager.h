#pragma once
#include "Resource.h"
#include <map>
#include <memory>
#include <string>

namespace kiko
{
    // ResourceManager is a class responsible for managing resources and providing access to them.
    class ResourceManager
    {
    public:
        // Get a resource of type T from the ResourceManager.
        // If the resource with the given filename already exists, it returns the existing resource.
        // If not, it creates a new resource of type T, adds it to the resource map, and returns it.
        // The additional arguments (TArgs) are forwarded to the Create function of the resource type T.
        template<typename T, typename ... TArgs>
        std::shared_ptr<T> Get(const std::string& filename, TArgs ... args);

    private:
        // A map to store resources with their corresponding filenames.
        // 'std::string' (the filename) is the key, and 'res_t<Resource>' is the value, which is a shared pointer to the base Resource class.
        std::map<std::string, std::shared_ptr<Resource>> m_resources;
    };

    // Definition of the template function Get.
    // The definition is placed outside the class declaration because it's a template function.
    template<typename T, typename ... TArgs>
    std::shared_ptr<T> ResourceManager::Get(const std::string& filename, TArgs ... args)
    {
        // Check if the resource with the given filename already exists in the resource map.
        auto it = m_resources.find(filename);
        if (it != m_resources.end())
        {
            // If it exists, return the existing resource after casting it to type T.
            // std::dynamic_pointer_cast is used to safely downcast the base Resource pointer to a derived T pointer.
            return std::dynamic_pointer_cast<T>(it->second);
        }

        // If the resource doesn't exist, create a new instance of it.
        // 'res_t<T>' is a shared pointer to the type T.
        res_t<T> resource = std::make_shared<T>();

        // Call the Create function of the resource with the given filename and additional arguments (args...).
        // The 'args...' is a variadic argument, meaning it can take any number of additional arguments.
        resource->Create(filename, args...);

        // Add the new resource to the resource map with its filename as the key.
        m_resources[filename] = resource;

        // Return the shared pointer to the newly created resource.
        return resource;
    }

    // Declare the global instance of ResourceManager named 'g_resources'.
    // This is the singleton instance of the ResourceManager that can be accessed from anywhere in the code.
    extern ResourceManager g_resources;
}
