#pragma once
#include "Singleton.h"
#include "Core/Logger.h"
#include <string>
#include <memory>
#include <map>

#define CREATE_CLASS(classname) kiko::Factory::Instance().Create<kiko::classname>(#classname);
#define CREATE_CLASSBASE(classbase, classname) kiko::Factory::Instance().Create<classbase>(classname);
namespace kiko
{
	class Object;

	class CreatorBase
    {
    public:
        virtual ~CreatorBase() = default; //if we dont do this it shits the bed

        virtual std::unique_ptr<kiko::Object> Create() = 0; //create object of that type
    };

    template <typename T>
    class Creator : public CreatorBase
    {
    public:
        std::unique_ptr<kiko::Object> Create() override
        {
            return std::make_unique<T>();
        }
    };

    class Factory : public Singleton<Factory>
    {
    public:
        template <typename T>
        void Register(const std::string& key); 

        template <typename T>
        std::unique_ptr<T> Create(const std::string& key);

        friend class Singleton;

    protected:
        Factory()=default;
    private: //holds creator objects
        std::map<std::string, std::unique_ptr<CreatorBase>> m_registry; //give me a string, ill give you a creator base object
    };

    template<typename T>
    inline void Factory::Register(const std::string& key) //registers key to the registry
    {
        INFO_LOG("Class Registered " << key);
        m_registry[key] = std::make_unique<Creator<T>>(); //makes an object with a creator type of T
    }

    template<typename T>
    inline std::unique_ptr<T> Factory::Create(const std::string& key)
    {
        auto iter = m_registry.find(key); //returns iterator
        if (iter != m_registry.end()) //if it equals end, we didn't find anything
        {
            return std::unique_ptr<T>(dynamic_cast<T*>(iter->second->Create().release())); //1st get key, 2nd gets value // Create makes raw ptr. release gives us the pointer
        }
        return std::unique_ptr<T>();
    }
}