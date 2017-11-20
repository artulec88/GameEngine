#include "stdafx.h"
#include "RandomGenerator.h"
#include "Utility/ILogger.h"

/* ==================== RandomGenerator class begin ==================== */
math::random::RandomGenerator::RandomGenerator(unsigned seed) :
	m_seed(seed)
{
}


math::random::RandomGenerator::~RandomGenerator()
{
}

bool math::random::RandomGenerator::CheckIfInRange(int value, int lowerBound, int upperBound) const
{
	const auto isInRange = value >= lowerBound && value <= upperBound;
	CHECK_CONDITION_ALWAYS_MATH(isInRange, utility::logging::ERR, "Generated random integer (", value, ") is outside of given range [", lowerBound, "; ", upperBound, "]");
	return isInRange;
}

bool math::random::RandomGenerator::CheckIfInRange(Real value, Real lowerBound, Real upperBound) const
{
	const auto isInRange = !(value < lowerBound || value > upperBound);
	CHECK_CONDITION_ALWAYS_MATH(isInRange, utility::logging::ERR, "Generated random floating-point value (", value, ") is outside of given range [", lowerBound, "; ", upperBound, "]");
	return isInRange;
}
/* ==================== RandomGenerator class end ==================== */

/* ==================== SimpleRandomGenerator class begin ==================== */
math::random::SimpleRandomGenerator::SimpleRandomGenerator(unsigned seed) :
	RandomGenerator(seed)
{
	srand(seed);
}


math::random::SimpleRandomGenerator::~SimpleRandomGenerator()
{
}

int math::random::SimpleRandomGenerator::NextInt() const
{
	return rand();
}

int math::random::SimpleRandomGenerator::NextInt(int lowerBound, int upperBound) const
{
	CHECK_CONDITION_RETURN_MATH(lowerBound <= upperBound, 0, Utility::Logging::ERR, "Incorrect lower bound specified (", lowerBound, "). It is greater than upper bound (", upperBound, ").");
	const auto result = rand() % (upperBound - lowerBound) + lowerBound;
	return CheckIfInRange(result, lowerBound, upperBound) ? result : lowerBound;
}

math::Real math::random::SimpleRandomGenerator::NextFloat() const
{
	return static_cast<Real>(rand()) / RAND_MAX;
}

math::Real math::random::SimpleRandomGenerator::NextFloat(Real lowerBound, Real upperBound) const
{
	CHECK_CONDITION_RETURN_MATH(lowerBound <= upperBound, REAL_ZERO, Utility::Logging::ERR, "Incorrect lower bound specified (", lowerBound, "). It is greater than upper bound (", upperBound, ").");
	const auto result = NextFloat() * (upperBound - lowerBound) + lowerBound;
	return CheckIfInRange(result, lowerBound, upperBound) ? result : lowerBound;
}
/* ==================== SimpleRandomGenerator class end ==================== */

/* ==================== DefaultRandomGenerator class begin ==================== */
math::random::DefaultRandomGenerator::DefaultRandomGenerator(unsigned seed) :
	RandomGenerator(seed)//,
	//m_engine(seed)
{
}


math::random::DefaultRandomGenerator::~DefaultRandomGenerator()
{
}

int math::random::DefaultRandomGenerator::NextInt() const
{
	//return m_engine();
	return 0;
}

int math::random::DefaultRandomGenerator::NextInt(int lowerBound, int upperBound) const
{
	CHECK_CONDITION_RETURN_MATH(lowerBound <= upperBound, 0, Utility::Logging::ERR, "Incorrect lower bound specified (", lowerBound, "). It is greater than upper bound (", upperBound, ").");
	const auto result = rand() % (upperBound - lowerBound) + lowerBound;
	return CheckIfInRange(result, lowerBound, upperBound) ? result : lowerBound;
}

math::Real math::random::DefaultRandomGenerator::NextFloat() const
{
	return static_cast<Real>(rand()) / RAND_MAX;
}

math::Real math::random::DefaultRandomGenerator::NextFloat(Real lowerBound, Real upperBound) const
{
	CHECK_CONDITION_RETURN_MATH(lowerBound <= upperBound, REAL_ZERO, Utility::Logging::ERR, "Incorrect lower bound specified (", lowerBound, "). It is greater than upper bound (", upperBound, ").");
	const auto result = NextFloat() * (upperBound - lowerBound) + lowerBound;
	return CheckIfInRange(result, lowerBound, upperBound) ? result : lowerBound;
}
/* ==================== DefaultRandomGenerator class end ==================== */