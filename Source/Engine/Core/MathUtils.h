#pragma once
#include <cmath>
#include <algorithm>
#include <utility>
namespace kiko
{
	constexpr  float Pi = 3.14159265359f;
	constexpr  float TwoPi = Pi * 2;
	constexpr  float HalfPi = Pi / 2;

	constexpr  	float RadiansToDegrees(float radians) { return radians * (180.0f / Pi); };
	constexpr	float DegreesToRadians(float degrees) { return degrees * (Pi / 180); };

	constexpr int Wrap(int value, int max)
	{
		return (value % max) + ((value < 0) ? max : 0);
	}
	inline float Wrap(float value, float max)
	{
		return std::fmod(value, max) + ((value < 0) ? max : 0);
	}
	template<typename T>
	inline T Max(T a, T b)
	{
		return (a > b) ? a : b;
	}
	template<typename T>
	inline T min(T a, T b)
	{
		return (a < b) ? a : b;
	}
	template <typename T>
	inline T Clamp(T value, T min, T max)
	{
		if (min > max) std::swap(min, max);
		return (value < min) ? min : (value > max) ? max : value;
	}

	template<typename T>//linear interpoation ///interpolates between to values moves along 
	constexpr T Lerp(const T& a, const T& b, float t) ///between 2 points and moves there
	{
		return (a * (1.0f - t)) + (b * t);
	}
}
