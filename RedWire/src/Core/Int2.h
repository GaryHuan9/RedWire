#pragma once

#include <functional>

namespace RedWire
{
	struct Int2
	{
		Int2(size_t x = 0, size_t y = 0);

		size_t x;
		size_t y;

		static const Int2 edges4[4];

		Int2& operator+= (const Int2& other);
		Int2& operator-= (const Int2& other);
		Int2& operator*= (const Int2& other);
		Int2& operator/= (const Int2& other);

		Int2& operator*= (const size_t other);
		Int2& operator/= (const size_t other);

		Int2 operator+ (const Int2& other) const;
		Int2 operator- (const Int2& other) const;
		Int2 operator* (const Int2& other) const;
		Int2 operator/ (const Int2& other) const;

		Int2 operator* (const size_t other) const;
		Int2 operator/ (const size_t other) const;

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