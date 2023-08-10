#pragma once
#include "vector2.h"

#include "Math/Maxtrix22.h"

namespace kiko
{
	;
	class Transform
	{
	public:
		vec2 position;
		float rotation = 0;
		float scale = 1;

	public:
		Transform(const vec2& position, float rotation, float scale = 1) :
			position{ position },
			rotation{ rotation },
			scale{ scale }
		{}
		mat2 const GetMatrix() const
		{
			mat2 ms = mat2::CreateScale(scale);
			mat2 mr = mat2::CreateRotation(rotation);

			return ms * mr;
		}
		
	};
}
