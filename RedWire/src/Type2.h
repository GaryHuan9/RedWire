#pragma once

#include <cmath>
#include <functional>
#include <string>

namespace RedWire
{
	//beware of macros, this is unsafe to code with
#define to_type2(type, from) Type2<type>(static_cast<type>(from.x), static_cast<type>(from.y)) 

	template<typename T> struct Type2
	{
		Type2(const T& value = (T)0) : Type2(value, value) {}

		Type2(const T& x, const T& y) : x(x), y(y) {}

		//I made conversion explicit because I like more this way :D, you can make it implicit by removing the explicit keyword
		template<typename fromType>
		explicit Type2(const Type2<fromType>& from) : x(static_cast<T>(from.x)), y(static_cast<T>(from.y)) {}

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

		static Type2<T> min(const Type2<T>& a, const Type2<T>& b)
		{
			return Type2<T>(std::min(a.x, b.x), std::min(a.y, b.y));
		}

		static Type2<T> max(const Type2<T>& a, const Type2<T>& b)
		{
			return Type2<T>(std::max(a.x, b.x), std::max(a.y, b.y));
		}

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

		Type2<T> min(const Type2<T>& other) const
		{
			return Type2<T>::min(*this, other);
		}

		Type2<T> max(const Type2<T>& other) const
		{
			return Type2<T>::max(*this, other);
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

		template<typename convertToType> Type2<convertToType> toType() const
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