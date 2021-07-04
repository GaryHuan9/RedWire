#include "Int2.h"
#include <functional>

using namespace RedWire;

Int2::Int2(size_t x, size_t y) : x(x), y(y)
{

}

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

Int2& Int2::operator*=(const size_t other)
{
	x *= other;
	y *= other;

	return *this;
}

Int2& Int2::operator/=(const size_t other)
{
	x /= other;
	y /= other;

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

Int2 Int2::operator*(const size_t other) const
{
	Int2 copy(*this);
	copy *= other;

	return copy;
}

Int2 Int2::operator/(const size_t other) const
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

size_t std::hash<RedWire::Int2>::operator()(const RedWire::Int2& value) const noexcept
{
	size_t x = std::hash<decltype(value.x)>{}(value.x);
	size_t y = std::hash<decltype(value.y)>{}(value.y);
	return (x * 397) ^ y;
}