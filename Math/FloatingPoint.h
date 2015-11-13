#ifndef __MATH_FLOATING_POINT_H__
#define __MATH_FLOATING_POINT_H__

#include <limits>
#include "Math.h"
//#include "Utility\IConfig.h"
//#include "Utility\ILogger.h" // only used if AlmostEqual logs anything
#ifdef CALCULATE_MATH_STATS
#include "Statistics.h"
#include "IStatisticsStorage.h"
#endif

namespace Math
{

template<size_t size>
class TypeWithSize
{
public:
	// This prevents the user from using TypeWithSize<N> with incorrect values of N
	typedef void UInt;
};

// The specialization for size 4
template<>
class TypeWithSize<4>
{
public:
	// unsigned int has size 4 in both gcc and MSVC
	// As base/basictypes.h doesn't compile on Windows, we cannot use uint32, uint64, and etc here
	typedef int Int;
	typedef unsigned int UInt;
};

// The specialization for size 8
template<>
class TypeWithSize<8>
{
public:
#if _MSC_VER
	typedef __int64 Int;
	typedef unsigned __int64 UInt;
#else
	typedef long long Int;
	typedef unsigned long long UInt;
#endif /* _MSC_VER */
};

template <typename RawType>
class FloatingPoint
{
public:
	// Defines the unsigned integer type that has the same size as the floating point number
	typedef typename TypeWithSize<sizeof(RawType)>::UInt Bits;

public: /* Constants */
	static const size_t s_kBitCount = 8 * sizeof(RawType); // number of bits in a number
	static const size_t s_kFractionBitCount = std::numeric_limits<RawType>::digits - 1; // number of fraction bits in a number
	static const size_t s_kExponentBitCount = s_kBitCount - 1 - s_kFractionBitCount; // number of exponent bits in a number
	static const Bits s_kSignBitMask = static_cast<Bits>(1) << (s_kBitCount - 1); // The mask for the sign bit
	static const Bits s_kFractionBitMask = ~static_cast<Bits>(0) >> (s_kExponentBitCount + 1); // The mask for the fraction bits
	static const Bits s_kExponentBitMask = ~(s_kSignBitMask | s_kFractionBitMask);
	/** 
	 * How many ULP's (Units in the Last Place) we want to tolerate when comparing two numbers. The larger the value the more error we allow.
	 * The value 0 means that two numbers must be exactly the same to be considered equal. The maximum error of a single floating-point operation is 0.5 ULP.
	 * On Intel CPU's all floating-point calculations are done with 80-bit precision, while double has 64 bits. Therefore, 4 should be enough for ordinary use.
	 * See the article http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm for more details on ULP
	 */
	static const size_t s_kMaxUlps = 4;

public: /* non-static methods */
	/**
	 * Constructs a FloatingPoint from a raw floating-point number. On an Intel CPU, passing a non-normalized NAN around may change its bits,
	 * although the new value is guaranteed to be also a NAN. Therefore, don't expect this constructor to preserve the bits in x when x is a NAN.
	 */
	explicit FloatingPoint(const RawType& x)
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("FloatingPoint"))
#endif
	{
		u_.value_ = x;
	}

	//const Bits& Bits() const { return u_.bits_; } // returns the bits that represent this number
	Bits ExponentBits() const { return s_kExponentBitMask & u_.bits_; } // returns the exponent bits of this number
	Bits FractionBits() const { return s_kFractionBitMask & u_.bits_; } // returns the fraction bits of this number
	Bits SignBits() const { return s_kSignBitMask & u_.bits_; } // returns the sign bit of this number

	bool IsNAN() const { return (ExponentBits() == s_kExponentBitMask) && (FractionBits() != 0); } // Returns true iff this is NAN
	/**
	 * Returns true iff this number is at most s_kMaxUlps ULP's away from rhs. In particular, this function:
	 * - returns false if either number is (or both are) NAN
	 * - treats really large numbers as almost equally to infinity
	 * - thinks +0.0 and -0.0 are 0 DLP's apart
	 */
	bool AlmostEqual(const FloatingPoint& rhs) const
	{
		START_PROFILING;
		// The IEEE standard says that any comparison operation involving a NAN must return false
		if (IsNAN() || rhs.IsNAN())
			return false;

		Bits distance = DistanceBetweenSignAndMagnitudeNumbers(u_.bits_, rhs.u_.bits_);
		STOP_PROFILING;
		return distance <= s_kMaxUlps;
	}

public: /* static methods */
	/**
	 * Reinterprets a bit pattern as a floating-point number. This function is needed to test the AlmostEqual() method
	 */
	static RawType ReinterpretBits(const Bits bits)
	{
		FloatingPoint fp(0);
		fp.u_.bits_ = bits;
		return fp.u_.value_;
	}

	/**
	 * Returns the floating-point number that represents positive infinity
	 */
	static RawType Infinity()
	{
		return ReinterpretBits(s_kExponentBitMask);
	}

private:
	// The data type used to store the actual floating-point number
	union FloatingPointUnion
	{
		RawType value_; // The raw floating-point number
		Bits bits_; // the bits that represent the number
	};
	
#ifdef CALCULATE_MATH_STATS
	Statistics::ClassStats& m_classStats;
#endif
	
	/**
	 * Converts an integer from the sign-and-magnitude representation to the biased representation.
	 * More precisely, let N be 2 to the power of (s_kBitCount - 1), an integer x is represented by the
	 * unsigned number x + N. For instance,
	 * -N + 1 (the most negative number representable using sign-and-magnitude) is represented by 1;
	 * 0      is represented by N;
	 * N - 1  (the biggest number representable using sign-and-magnitude) is represented by 2N - 1.
	 * Read http://en.wikipedia.org/wiki/Signed_number_representations for more details on signed number representations.
	 */
	static Bits SignAndMagnitudeToBiased(const Bits& sam)
	{
		if (s_kSignBitMask & sam)
		{
			// sam represents a negative number
			return ~sam + 1;
		}
		else
		{
			return s_kSignBitMask | sam;
		}
	}

	/**
	 * Given two numbers in the sign-and-magnitude representation,
	 * returns the distance between them as an unsigned number
	 */
	static Bits DistanceBetweenSignAndMagnitudeNumbers(const Bits& sam1, const Bits& sam2)
	{
		const Bits biased1 = SignAndMagnitudeToBiased(sam1);
		const Bits biased2 = SignAndMagnitudeToBiased(sam2);
		return (biased1 >= biased2) ? (biased1 - biased2) : (biased2 - biased1);
	}

	FloatingPointUnion u_;
}; /* end class FloatingPoint */

//static int oneTempCounter = 0;
//static int twoTempCounter = 0;
//static int threeTempCounter = 0;
inline bool AlmostEqual(Real value1, Real value2)
{
	//if (((oneTempCounter % 10000 == 0) && (oneTempCounter > 0)) ||
	//	((twoTempCounter % 10000 == 0) && (twoTempCounter > 0)) ||
	//	((threeTempCounter % 10000 == 0) && (threeTempCounter > 0)))
	//{
	//	CRITICAL_LOG("oneTempCounter = %d, twoTempCounter = %d, threeTempCounter = %d", oneTempCounter, twoTempCounter, threeTempCounter);
	//}
	if (abs(value1 - value2) > EPSILON)
	{
		//++oneTempCounter;
		return false;
	}
	return true; // to make comparison much faster we just return true here instead of creating the FloatingPoint objects.

	//FloatingPoint<Real> fpValue1(value1);
	//FloatingPoint<Real> fpValue2(value2);
	//return fpValue1.AlmostEqual(fpValue2);
	
	//bool areEqual = fpValue1.AlmostEqual(fpValue2);
	//if (areEqual)
	//{
	//	++twoTempCounter;
	//}
	//else
	//{
	//	++threeTempCounter;
	//}
	//return areEqual;
}

} /* end namespace Math */

#endif /* __MATH_FLOATING_POINT_H__ */