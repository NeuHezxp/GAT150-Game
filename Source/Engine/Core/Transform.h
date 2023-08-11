#pragma once
#include "vector2.h"
#include "Math/Matrix33.h"   // Include the header for the Matrix33 class.
#include "Math/Maxtrix22.h"   // Include the header for the Matrix22 class.

namespace kiko
{
	// Declare an empty class named "Transform."
	;
	class Transform
	{
	public:
		vec2 position;    // The position of the transform.
		float rotation = 0;    // The rotation angle in radians.
		float scale = 1;    // The scale factor.

	public:
		// Constructor that initializes the transform with position, rotation, and scale.
		Transform(const vec2& position, float rotation, float scale = 1) :
			position{ position },
			rotation{ rotation },
			scale{ scale }
		{}

		// Function to calculate and retrieve the transformation matrix.
		mat3 GetMatrix() const
		{
			mat3 ms = mat3::CreateScale(scale);   // Create a scaling matrix.
			mat3 mr = mat3::CreateRotation(rotation);   // Create a rotation matrix.
			mat3 mt = mat3::CreateTranslation(position);   // Create a translation matrix.

			// Calculate the final transformation matrix by combining translation, scaling, and rotation.
			mat3 mx = mt * ms * mr;   // Transform order: translate * scale * rotate.

			return mx;
		}
	};
}
