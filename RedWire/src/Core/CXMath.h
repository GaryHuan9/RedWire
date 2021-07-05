#pragma once

#define _CXMATHLIB_

#include <cmath>

namespace CXUtils
{
	/// <summary>
	/// A simple helper math class
	/// </summary>
	struct CXMath
	{
	public:
		/// <summary>
		/// The PI
		/// </summary>
		static const float PI;
		/// <summary>
		/// The PI's brother, Double PI
		/// </summary>
		static const float TAU;
		/// <summary>
		/// Formerly known as the golden ratio
		/// </summary>
		static const float PHI;
		/// <summary>
		/// Euler's constant
		/// </summary>
		static const float E;

		/// <summary>
		/// The constant value to convert from radiants to degrees
		/// </summary>
		static const float RAD_TO_DEG;
		/// <summary>
		/// The constant value to convert from degrees to radiants
		/// </summary>
		static const float DEG_TO_RAD;

		static float clamp(const float& value, const float& min, const float& max);
		static int clamp(const int& value, const int& min, const int& max);

		static float clampNeg11(const float& value);
		static int clampNeg11(const int& value);

		static float clamp01(const float& value);
		static int clamp01(const int& value);

		static int floorToInt(const float& value);
		static int ceilToInt(const float& value);

		static float fract(const float& value);

		static float step(const float& value);
		/// <summary>
		/// step to a certain interval
		/// </summary>
		static float stepTo(const float& value, const float& interval);

		static float sign(const float& value);

		static float map(const float& value, const float& inMin, const float& inMax, const float& outMin, const float& outMax);
		static int map(const int& value, const int& inMin, const int& inMax, const int& outMin, const int& outMax);
	};
};
