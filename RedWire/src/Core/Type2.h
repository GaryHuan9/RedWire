#pragma once

namespace RedWire
{
	template<typename T>
	struct Type2
	{
	public:
		Type2(const T& _x = (T)0, const T& _y = (T)0) : x(_x), y(_y) {}

		T x, y;

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

		Type2<T> getFloored() const
		{
			Type2<T> copy(*this);

			return copy.floor();
		}

		T getMagnitude() const
		{
			return std::sqrt(getSqrMagnitude());
		}

		T getSqrMagnitude() const
		{
			return x * x + y * y;
		}

		Type2<T>& normalize()
		{
			T magnitude = getMagnitude();

			if (magnitude == 0) return *this;

			(*this) /= magnitude;

			return *this;
		}

		Type2<T> getNormalized() const
		{
			Type2<T> copy(*this);
			return copy.normalize();
		}

	#pragma endregion

	};

	/// <summary>
	/// A 2D float vector
	/// </summary>
	typedef Type2<float> Float2;
	/// <summary>
	/// A 2D unsigned int vector
	/// </summary>
	typedef Type2<unsigned int> UInt2;
}

namespace std
{
	template<typename T>
	struct hash<RedWire::Type2<T>>
	{
		T operator()(const RedWire::Type2<T>& value) const noexcept
		{
			T x = std::hash<T>{}(value.x);
			T y = std::hash<T>{}(value.y);
			return (x * 397) ^ y;
		}
	};
}