#pragma once
#include <memory>
#include <string>

namespace kiko
{
    // Resource is an abstract base class representing a generic resource.
    class Resource
    {
    public:
        // Virtual destructor makes Resource a polymorphic class.
        virtual ~Resource() = default;


        // Pure virtual function Create.
        // This function is declared as pure virtual (indicated by '= 0'), which makes Resource an abstract class.
        // Any class inheriting from Resource must implement this function.
        // The '...' in the parameter list allows you to have a variadic function, meaning it can take any number of additional arguments.
        virtual bool Create(std::string filename, ...) = 0;

    private:
    };
	template<typename T>
	using res_t = std::shared_ptr<T>;
    ///narrowed down to font and resource issue due to null
}
