#pragma once

#include <functional>

namespace RedWire
{
	struct Int2
	{
		size_t x;
		size_t y;

		Int2& operator+= (const Int2& other);
		Int2& operator-= (const Int2& other);
		Int2& operator*= (const Int2& other);
		Int2& operator/= (const Int2& other);

		Int2 operator+ (const Int2& other) const;
		Int2 operator- (const Int2& other) const;
		Int2 operator* (const Int2& other) const;
		Int2 operator/ (const Int2& other) const;

		bool operator== (const Int2& other) const;
		bool operator!= (const Int2& other) const;
	};
}

namespace std
{
	template<> struct hash<RedWire::Int2>
	{
		size_t operator()(const RedWire::Int2& value) const noexcept;
	};
}