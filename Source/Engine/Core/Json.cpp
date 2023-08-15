#include "Json.h"
//#include "C:\Homework\Quarter 4\C++ code\GAT150-Game\Source\ThirdParty\rapidjson\include\rapidjson/istreamwrapper.h"
#include "../../ThirdParty/rapidjson/include/rapidjson/istreamwrapper.h"

#include <sstream>
#include "FileIO.h"
#include "Logger.h"

namespace kiko
{
	bool Json::Load(const std::string& filename, rapidjson::Document& document)
	{
		std::string buffer;
		if (!readFile(filename, buffer)) // Use kiko::readFile to read the file into buffer
		{
			WARNING_LOG("Cannot open file: " << filename);
			return false;
		}
		std::stringstream stream(buffer);
		rapidjson::IStreamWrapper istream(stream);
		// parse stream to json
		document.ParseStream(istream);
		if (!document.IsObject()) // Check if the document is an object
		{
			WARNING_LOG("Json file cannot be parsed/read: " << filename);
			return false;
		}
		return true;
	}
	bool Json::Read(const rapidjson::Value& value, const std::string& name, int& data, bool required)
	{
		if (!value.HasMember(name.c_str()) || !value[name.c_str()].IsInt())
		{
			if (required) ERROR_LOG("Cannot read json info " << name.c_str());
			return false;
		}
		data = value[name.c_str()].GetInt(); // Use GetInt to get int value
		return true;
	}

	bool Json::Read(const rapidjson::Value& value, const std::string& name, float& data, bool required)
	{
		if (!value.HasMember(name.c_str()) || !value[name.c_str()].IsFloat())
		{
			if (required) ERROR_LOG("Cannot read json info " << name.c_str());
			return false;
		}
		data = value[name.c_str()].GetFloat(); // Use GetInt to get int value
		return true;
	}

	bool Json::Read(const rapidjson::Value& value, const std::string& name, bool& data, bool required)
	{
		if (!value.HasMember(name.c_str()) || !value[name.c_str()].IsBool())
		{
			if (required) ERROR_LOG("Cannot read json info " << name.c_str());
			return false;
		}
		data = value[name.c_str()].GetBool();
		return true;
	}

	bool Json::Read(const rapidjson::Value& value, const std::string& name, std::string& data, bool required)
	{
		if (!value.HasMember(name.c_str()) || !value[name.c_str()].IsString())// Ue
		{
			if (required) ERROR_LOG("Cannot read json info " << name.c_str());
			return false;
		}
		data = value[name.c_str()].GetString();
		return true;
	}

	bool Json::Read(const rapidjson::Value& value, const std::string& name, vec2& data, bool required)
	{
		// check if 'name' member exists and is an array with 2 elements
		if (!value.HasMember(name.c_str()) || !value[name.c_str()].IsArray() || value[name.c_str()].Size() != 2)
		{
			if (required) ERROR_LOG("Cannot read json info: " << name.c_str());
			return false;
		}
		// create json array object
		auto& array = value[name.c_str()];
		// get array values
		for (rapidjson::SizeType i = 0; i < array.Size(); i++)
		{
			if (!array[i].IsNumber())
			{
				ERROR_LOG("Invalid json data type: " << name.c_str());
				return false;
			}
			data[i] = array[i].GetFloat();
		}
		return true;
	}
}