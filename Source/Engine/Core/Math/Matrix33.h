#pragma once
#include <cmath>

#include "Vector2.h"
#include "Vector3.h"

namespace kiko
{
    class Matrix33
    {
    public:
        vec3 rows[3];
        // [row][column]
        // rows[0] = vec3{ 0, 0 }
        // rows[1] = vec3{ 0, 0 }

        Matrix33() = default;
        Matrix33(const vec3& row1, const vec3& row2, const vec3& row3)
        {
            rows[0] = row1;
            rows[1] = row2;
            rows[2] = row3;
        }

        vec3  operator [] (size_t index) const { return rows[index]; }
        vec3& operator [] (size_t index) { return rows[index]; }

        vec2 operator * (const vec2& v);
        Matrix33 operator * (const Matrix33& mx);

        static Matrix33 CreateTranslation(const vec2& translate);
        static Matrix33 CreateScale(const vec2& scale);
        static Matrix33 CreateScale(float scale);
        static Matrix33 CreateRotation(float radians);

        static Matrix33 CreateIdentity();
    };

    inline vec2 kiko::Matrix33::operator*(const vec2& v)
    {
        ///| a b c |    | x |
        ///| d e f |  * | y |
		///| g h i |    | 1 |

        vec2 result;
        result.x = rows[0][0] * v.x + rows[0][1] * v.y + rows[0][2];
        result.y = rows[1][0] * v.x + rows[1][1] * v.y + rows[1][2];

        return result;
    }

    inline Matrix33 kiko::Matrix33::operator*(const Matrix33& mx)
    {
        // | a b |   | e f |
        // | c d | * | g h |

		//m00 m01 m02   n00 n01 n02
    	//m01 m11 m21 x
        //m02 m12 m22


        Matrix33 result;
        result[0][0] = rows[0][0] * mx[0][0] + rows[0][1] * mx[1][0] + rows[0][2] * mx[2][0];
        result[0][1] = rows[0][0] * mx[0][1] + rows[0][1] * mx[1][1] + rows[0][2] * mx[2][1];
        result[0][2] = rows[0][0] * mx[0][0] + rows[0][1] * mx[1][0] + rows[0][2] * mx[2][2];

        result[1][0] = rows[1][0] * mx[0][0] + rows[1][1] * mx[1][0] + rows[1][2] * mx[2][0];
        result[1][1] = rows[1][0] * mx[0][1] + rows[1][1] * mx[1][1] + rows[1][2] * mx[2][1];
        result[1][2] = rows[1][0] * mx[0][0] + rows[1][1] * mx[1][0] + rows[1][2] * mx[2][2];

        result[2][0] = rows[2][0] * mx[0][0] + rows[2][1] * mx[1][0] + rows[2][2] * mx[2][0];
        result[2][1] = rows[2][0] * mx[0][1] + rows[2][1] * mx[1][1] + rows[2][2] * mx[2][1];
        result[2][2] = rows[2][0] * mx[0][0] + rows[2][1] * mx[1][0] + rows[2][2] * mx[2][2];
       

        return result;
    }

    inline Matrix33 Matrix33::CreateIdentity()
    {
        return
        {
            { 1, 0, 0 },
            { 0, 1, 0 },
            { 0, 0, 1 }
        };
    }

    inline Matrix33 Matrix33::CreateScale(const vec2& scale)
    {
		// | sx 0 0 |
		// | 0 sy 0 |
		// | 0  0 1  |
        Matrix33 mx = CreateIdentity();
        mx[0][0] = scale.x;
        mx[1][1] = scale.y;

        return mx;
    }

    inline Matrix33 Matrix33::CreateScale(float scale)
    {
        // | s 0 0 |
        // | 0 s 0 |
        // | 0 0 1  |
        Matrix33 mx = CreateIdentity();
        mx[0][0] = scale;
        mx[1][1] = scale;

        return mx;
    }

    inline Matrix33 Matrix33::CreateRotation(float radians)
    {
        // | c -s 0 |  c=sin
		// | s  c 0 |   -cos
        // | 0  0 1  |

        Matrix33 mx = CreateIdentity();
        float c = cos(radians);
        float s = sin(radians);

        mx[0][0] = c; mx[0][1] = -s;
        mx[1][0] = s; mx[1][1] = c;

        return mx;
    }
    inline Matrix33 Matrix33::CreateTranslation(const vec2& translate)
    {
		//Homogeneous coordinates
    	//| 1 0 x |
       // | 0 s y |
       // | 0 0 1 |
		Matrix33 mx = CreateIdentity();
		mx[0][2] = translate.x;
		mx[1][2] = translate.y;

		return mx;
    }


    using mat3 = Matrix33;
}