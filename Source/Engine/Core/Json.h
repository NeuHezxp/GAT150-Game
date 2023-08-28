#pragma once
#include "../../ThirdParty/rapidjson/include/rapidjson/document.h"
#include "Vector2.h"
#include <string>
#include "Math/Rect.h"

#include "Color.h"

// Macro to read data from JSON with a given name
#define READ_DATA(value, data) kiko::Json::Read(value, #data, data)
// Macro to read required data from JSON with a given name
#define READ_DATA_REQUIRED(value, data) kiko::Json::Read(value, #data, data, true)
// Macro to check if a JSON object has a specific data member
#define HAS_DATA(value, data) value.HasMember(#data)
// Macro to get a specific data member from a JSON object
#define GET_DATA(value, data) value[#data]	
// Macro to read named data from JSON
#define READ_NAME_DATA(value, name, data) kiko::Json::Read(value, name, data)
// Macro to read required named data from JSON
#define READ_NAME_DATA_REQUIRED(value, name, data) kiko::Json::Read(value, name, data, true)

namespace kiko
{
	// Utility class for reading data from JSON
	class Json
	{
	public:
		// Load JSON data from a file into a rapidjson::Document
		static bool Load(const std::string& filename, rapidjson::Document& document);
		// Read int data from JSON with a given name
		static bool Read(const rapidjson::Value& value, const std::string& name, int& data, bool required = false);
		// Read float data from JSON with a given name
		static bool Read(const rapidjson::Value& value, const std::string& name, float& data, bool required = false);
		// Read bool data from JSON with a given name
		static bool Read(const rapidjson::Value& value, const std::string& name, bool& data, bool required = false);
		// Read string data from JSON with a given name
		static bool Read(const rapidjson::Value& value, const std::string& name, std::string& data, bool required = false);

		// Read vec2 data from JSON with a given name
		static bool Read(const rapidjson::Value& value, const std::string& name, vec2& data, bool required = false);
		// Read Color data from JSON with a given name
		static bool Read(const rapidjson::Value& value, const std::string& name, Color& data, bool required = false);
		// Read Rect data from JSON with a given name
		static bool Read(const rapidjson::Value& value, const std::string& name, Rect& data, bool required = false);
	};

	// Alias for rapidjson::Value, named json_t
	using json_t = rapidjson::Value;
}
