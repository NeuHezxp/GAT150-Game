#pragma once
#include <cmath>

#include "Vector2.h"   // Include the header for the Vector2 class.
#include "Vector3.h"   // Include the header for the Vector3 class.

namespace kiko
{
	class Matrix33
	{
	public:
		vec3 rows[3];   // Define a 3x3 matrix using an array of Vector3 objects to represent rows.

		// Default constructor for Matrix33.
		Matrix33() = default;

		// Constructor that initializes the matrix rows using provided Vector3 values.
		Matrix33(const vec3& row1, const vec3& row2, const vec3& row3)
		{
			rows[0] = row1;
			rows[1] = row2;
			rows[2] = row3;
		}

		// Overloaded operator to access matrix rows using index.
		vec3  operator [] (size_t index) const { return rows[index]; }
		vec3& operator [] (size_t index) { return rows[index]; }

		// Overloaded operator * for multiplying the matrix by a Vector2.
		vec2 operator * (const vec2& v);

		// Overloaded operator * for matrix multiplication.
		Matrix33 operator * (const Matrix33& mx);

		// Create a translation matrix given a 2D translation vector.
		static Matrix33 CreateTranslation(const vec2& translate);

		// Create a scaling matrix with non-uniform scaling factors.
		static Matrix33 CreateScale(const vec2& scale);

		// Create a uniform scaling matrix given a single scale factor.
		static Matrix33 CreateScale(float scale);

		// Create a rotation matrix given an angle in radians.
		static Matrix33 CreateRotation(float radians);

		// Create an identity matrix.
		static Matrix33 CreateIdentity();

		// Extract translation components from the matrix.
		vec2  GetTranslation() const;

		// Extract rotation angle in radians from the matrix.
		float GetRotation() const;

		// Extract scale components from the matrix.
		vec2  GetScale() const;
	};

	// Implementation of vector-matrix multiplication.
	inline vec2 kiko::Matrix33::operator*(const vec2& v)
	{
		///| a b c |    | x |
		///| d e f |  * | y |
		///| g h i |    | 1 |

		vec2 result;
		result.x = rows[0][0] * v.x + rows[0][1] * v.y + rows[0][2];   // Calculate new x-coordinate.
		result.y = rows[1][0] * v.x + rows[1][1] * v.y + rows[1][2];   // Calculate new y-coordinate.

		return result;
	}

	// Implementation of matrix multiplication.
	inline Matrix33 kiko::Matrix33::operator*(const Matrix33& mx)
	{
		// | a b |   | e f |
		// | c d | * | g h |

		//00 01 02  00 01 02
		//10 11 12* 10 11 12
		//20 21 22  20 21 22

		Matrix33 result;

		// Calculate each element of the resulting matrix.
		result[0][0] = rows[0][0] * mx[0][0] + rows[0][1] * mx[1][0] + rows[0][2] * mx[2][0];
		result[0][1] = rows[0][0] * mx[0][1] + rows[0][1] * mx[1][1] + rows[0][2] * mx[2][1];
		result[0][2] = rows[0][0] * mx[0][0] + rows[0][1] * mx[1][0] + rows[0][2] * mx[2][2];

		// Calculate other elements similarly.

		return result;
	}

	// Function to create an identity matrix.
	inline Matrix33 Matrix33::CreateIdentity()
	{
		return
		{
			{ 1, 0, 0 },
			{ 0, 1, 0 },
			{ 0, 0, 1 }
		};
	}

	// Function to create a scaling matrix with non-uniform scaling factors.
	inline Matrix33 Matrix33::CreateScale(const vec2& scale)
	{
		// | sx 0 0 |
		// | 0 sy 0 |
		// | 0  0 1  |
		Matrix33 mx = CreateIdentity();
		mx[0][0] = scale.x;   // Set scale factor along x-axis.
		mx[1][1] = scale.y;   // Set scale factor along y-axis.

		return mx;
	}

	// Function to create a uniform scaling matrix given a single scale factor.
	inline Matrix33 Matrix33::CreateScale(float scale)
	{
		// | s 0 0 |
		// | 0 s 0 |
		// | 0 0 1  |
		Matrix33 mx = CreateIdentity();
		mx[0][0] = scale;   // Set uniform scale factor.

		return mx;
	}

	// Function to create a rotation matrix given an angle in radians.
	inline Matrix33 Matrix33::CreateRotation(float radians)
	{
		// | c -s 0 |  c=sin
		// | s  c 0 |   -cos
		// | 0  0 1  |

		Matrix33 mx = CreateIdentity();
		float c = cos(radians);   // Calculate cosine of angle.
		float s = sin(radians);   // Calculate sine of angle.

		mx[0][0] = c; mx[0][1] = -s;   // Set elements for first row.
		mx[1][0] = s; mx[1][1] = c;    // Set elements for second row.

		return mx;
	}

	// Function to create a translation matrix given a 2D translation vector.
	inline Matrix33 Matrix33::CreateTranslation(const vec2& translate)
	{
		//Homogeneous coordinates
		//| 1 0 x |
	   // | 0 1 y |
	   // | 0 0 1 |
		Matrix33 mx = CreateIdentity();
		mx[0][2] = translate.x;   // Set translation along x-axis.
		mx[1][2] = translate.y;   // Set translation along y-axis.

		return mx;
	}

	// Function to extract translation components from the matrix.
	inline vec2 Matrix33::GetTranslation() const
	{
		return { rows[0][2], rows[1][2] };
	}

	// Function to extract rotation angle in radians from the matrix.
	inline float Matrix33::GetRotation() const
	{
		// Use the atan2 function to calculate the angle based on matrix elements.
		return std::atan2(rows[1][0], rows[0][0]);
	}

	// Function to extract scale components from the matrix.
	inline vec2 Matrix33::GetScale() const
	{
		vec2 x = { rows[0][0], rows[0][1] };   // Extract scale along x-axis.
		vec2 y = { rows[1][1], rows[1][1] };   // Extract scale along y-axis.
		return (x.length() > y.length());   // Compare scales and return the larger scale component.
	}

	// Alias for Matrix33 using the name mat3.
	using mat3 = Matrix33;
}
