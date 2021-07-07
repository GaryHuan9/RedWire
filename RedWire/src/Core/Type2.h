#pragma once

#include <cmath>
#include <functional>
#include <string>

namespace RedWire
{
	template<typename T> struct Type2
	{
		Type2(const T& x = (T)0, const T& y = (T)0) : x(x), y(y) {}

		T x, y;

		static const Type2 edges4[4];

#pragma region Operator overloading

		Type2<T>& operator+= (const Type2<T>& other)
		{
			x += other.x;
			y += other.y;

			return *this;
		}

		Type2<T>& operator-= (const Type2<T>& other)
		{
			x -= other.x;
			y -= other.y;

			return *this;
		}

		Type2<T>& operator*= (const Type2<T>& other)
		{
			x *= other.x;
			y *= other.y;

			return *this;
		}

		Type2<T>& operator/= (const Type2<T>& other)
		{
			x /= other.x;
			y /= other.y;

			return *this;
		}

		Type2<T>& operator*= (const T& other)
		{
			x *= other;
			y *= other;

			return *this;
		}

		Type2<T>& operator/= (const T& other)
		{
			x /= other;
			y /= other;

			return *this;
		}

		Type2<T> operator+ (const Type2<T>& other) const
		{
			Type2<T> copy(*this);
			copy += other;

			return copy;
		}

		Type2<T> operator- (const Type2<T>& other) const
		{
			Type2<T> copy(*this);
			copy -= other;

			return copy;
		}

		Type2<T> operator* (const Type2<T>& other) const
		{
			Type2<T> copy(*this);
			copy *= other;

			return copy;
		}

		Type2<T> operator/ (const Type2<T>& other) const
		{
			Type2<T> copy(*this);
			copy /= other;

			return copy;
		}

		Type2<T> operator* (const T& other) const
		{
			Type2<T> copy(*this);
			copy *= other;

			return copy;
		}

		Type2<T> operator/ (const T& other) const
		{
			Type2<T> copy(*this);
			copy /= other;

			return copy;
		}

		bool operator== (const Type2<T>& other) const
		{
			return x == other.x && y == other.y;
		}

		bool operator!= (const Type2<T>& other) const
		{
			return x != other.x && y != other.y;
		}

#pragma endregion


#pragma region Utility Methods

		Type2<T>& floor()
		{
			x = std::floor(x);
			y = std::floor(y);

			return *this;
		}

		Type2<T>& ceil()
		{
			x = std::ceil(x);
			y = std::ceil(y);

			return *this;
		}

		Type2<T> getFloor() const
		{
			Type2<T> copy(*this);
			return copy.floor();
		}

		Type2<T> getCeil() const
		{
			Type2<T> copy(*this);
			return copy.ceil();
		}

		T getSqrMagnitude() const
		{
			return x * x + y * y;
		}

		T getMagnitude() const
		{
			return std::sqrt(getSqrMagnitude());
		}

		Type2<T>& normalize()
		{
			T sqrMagnitude = getSqrMagnitude();

			if (sqrMagnitude == 0) return *this;
			*this /= std::sqrt(sqrMagnitude);

			return *this;
		}

		Type2<T> getNormalized() const
		{
			Type2<T> copy(*this);
			return copy.normalize();
		}

		template<typename convertToType>
		Type2<convertToType> toType() const
		{
			return Type2<convertToType>(static_cast<convertToType>(x), static_cast<convertToType>(y));
		}

		std::string toString() const
		{
			return std::to_string(x) + std::string(", ") + std::to_string(y);
		}

#pragma endregion

	};

	template<typename T> const Type2<T> Type2<T>::edges4[] = { Type2<T>(1, 0), Type2<T>(0, 1), Type2<T>(-1, 0), Type2<T>(0, -1) };

	typedef Type2<float> Float2;

	typedef Type2<uint32_t> UInt2;

	typedef Type2<int32_t> Int2;
}

namespace std
{
	template<typename T> struct hash<RedWire::Type2<T>>
	{
		size_t operator()(const RedWire::Type2<T>& value) const noexcept
		{
			size_t x = std::hash<T>{}(value.x);
			size_t y = std::hash<T>{}(value.y);
			return (x * 397) ^ y;
		}
	};
}