#include "Int2.h"
#include <functional>

using namespace RedWire;

Int2& Int2::operator+=(const Int2& other)
{
	x += other.x;
	y += other.y;

	return *this;
}

Int2& Int2::operator-=(const Int2& other)
{
	x -= other.x;
	y -= other.y;

	return *this;
}

Int2& Int2::operator*=(const Int2& other)
{
	x *= other.x;
	y *= other.y;

	return *this;
}

Int2& Int2::operator/=(const Int2& other)
{
	x /= other.x;
	y /= other.y;

	return *this;
}

Int2 Int2::operator+(const Int2& other) const
{
	Int2 copy(*this);
	copy += other;

	return copy;
}

Int2 Int2::operator-(const Int2& other) const
{
	Int2 copy(*this);
	copy -= other;

	return copy;
}

Int2 Int2::operator*(const Int2& other) const
{
	Int2 copy(*this);
	copy *= other;

	return copy;
}

Int2 Int2::operator/(const Int2& other) const
{
	Int2 copy(*this);
	copy /= other;

	return copy;
}

bool Int2::operator==(const Int2& other) const
{
	return x == other.x && y == other.y;
}

bool Int2::operator!=(const Int2& other) const
{
	return x != other.x && y != other.y;
}

namespace std
{
	template<> struct hash<Int2>
	{
		size_t operator()(const Int2& value) const
		{
			size_t x = hash<decltype(value.x)>{}(value.x);
			size_t y = hash<decltype(value.y)>{}(value.y);
			return (x * 397) ^ y;
		}
	};
}