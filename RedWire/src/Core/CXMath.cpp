#include "CXMath.h"

const float CXUtils::CXMath::PI = 3.141592653589793238462643383279502884197169f;
const float CXUtils::CXMath::TAU = 6.2831853071795864769252867665590057683943f;
const float CXUtils::CXMath::PHI = 1.61803398874989484820458683436563811772030f;
const float CXUtils::CXMath::E = 2.7182818284590452353602874713526624977572470f;

const float CXUtils::CXMath::RAD_TO_DEG = 57.2957795130823f;

const float CXUtils::CXMath::DEG_TO_RAD = .0174532925199433f;

float CXUtils::CXMath::clamp( const float& value, const float& min, const float& max )
{
	return value < min ? min : value > max ? max : value;
}

int CXUtils::CXMath::clamp( const int& value, const int& min, const int& max )
{
	return value < min ? min : value > max ? max : value;
}

float CXUtils::CXMath::clamp01( const float& value )
{
	return clamp( value, 0.f, 1.f );
}

int CXUtils::CXMath::clamp01( const int& value )
{
	return clamp( value, 0, 1 );
}

float CXUtils::CXMath::clampNeg11( const float& value )
{
	return clamp( value, -1.f, 1.f );
}

int CXUtils::CXMath::floorToInt( const float& value )
{
	return static_cast<int>( std::floorf( value ) );
}

int CXUtils::CXMath::ceilToInt( const float& value )
{
	return static_cast<int>( std::ceilf( value ) );
}

int CXUtils::CXMath::clampNeg11( const int& value )
{
	return clamp( value, -1, 1 );
}

float CXUtils::CXMath::fract( const float& value )
{
	return value - std::floorf( value );
}

float CXUtils::CXMath::step( const float& value )
{
	return std::floorf( value );
}

float CXUtils::CXMath::stepTo( const float& value, const float& interval )
{
	return interval * std::floorf( value );
}

float CXUtils::CXMath::sign( const float& value )
{
	return value > 0 ? 1 : value < 0 ? -1 : 0;
}

int CXUtils::CXMath::map( const int& value, const int& inMin, const int& inMax, const int& outMin, const int& outMax )
{
	return static_cast<int>( static_cast<float>( value - inMin ) * ( static_cast<float>( outMax - outMin ) / ( inMax - inMin ) ) ) + outMin;
}

float CXUtils::CXMath::map( const float& value, const float& inMin, const float& inMax, const float& outMin, const float& outMax )
{
	return ( value - inMin ) * ( outMax - outMin ) / ( inMax - inMin ) + outMin;
}
