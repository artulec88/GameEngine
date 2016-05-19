#include "stdafx.h"
#include "RandomGenerator.h"
#include "Utility\ILogger.h"

/* ==================== RandomGenerator class begin ==================== */
Math::Random::RandomGenerator::RandomGenerator(unsigned seed) :
	m_seed(seed)
{
}


Math::Random::RandomGenerator::~RandomGenerator()
{
}

bool Math::Random::RandomGenerator::CheckIfInRange(int value, int lowerBound, int upperBound) const
{
	bool isInRange = value >= lowerBound && value <= upperBound;
	CHECK_CONDITION_ALWAYS_MATH(isInRange, Utility::Error, "Generated random integer (%d) is outside of given range [%d; %d]", value, lowerBound, upperBound);
	return isInRange;
}

bool Math::Random::RandomGenerator::CheckIfInRange(Math::Real value, Math::Real lowerBound, Math::Real upperBound) const
{
	bool isInRange = !(value < lowerBound || value > upperBound);
	CHECK_CONDITION_ALWAYS_MATH(isInRange, Utility::Error, "Generated random floating-point value (%.5f) is outside of given range [%.5f; %.5f]", value, lowerBound, upperBound);
	return isInRange;
}
/* ==================== RandomGenerator class end ==================== */

/* ==================== SimpleRandomGenerator class begin ==================== */
Math::Random::SimpleRandomGenerator::SimpleRandomGenerator(unsigned seed) :
	RandomGenerator(seed)
{
	srand(seed);
}


Math::Random::SimpleRandomGenerator::~SimpleRandomGenerator()
{
}

int Math::Random::SimpleRandomGenerator::NextInt() const
{
	return rand();
}

int Math::Random::SimpleRandomGenerator::NextInt(int lowerBound, int upperBound) const
{
	CHECK_CONDITION_RETURN_MATH(lowerBound <= upperBound, 0, Utility::Error, "Incorrect lower bound specified (%d). It is greater than upper bound (%d).", lowerBound, upperBound);
	int result = (rand() % (upperBound - lowerBound)) + lowerBound;
	return CheckIfInRange(result, lowerBound, upperBound) ? result : lowerBound;
}

Math::Real Math::Random::SimpleRandomGenerator::NextFloat() const
{
	return static_cast<Math::Real>(rand()) / RAND_MAX;
}

Math::Real Math::Random::SimpleRandomGenerator::NextFloat(Real lowerBound, Real upperBound) const
{
	CHECK_CONDITION_RETURN_MATH(lowerBound <= upperBound, REAL_ZERO, Utility::Error, "Incorrect lower bound specified (%d). It is greater than upper bound (%d).", lowerBound, upperBound);
	Math::Real result = (NextFloat() * (upperBound - lowerBound)) + lowerBound;
	return CheckIfInRange(result, lowerBound, upperBound) ? result : lowerBound;
}
/* ==================== SimpleRandomGenerator class end ==================== */

/* ==================== DefaultRandomGenerator class begin ==================== */
Math::Random::DefaultRandomGenerator::DefaultRandomGenerator(unsigned seed) :
	RandomGenerator(seed)//,
	//m_engine(seed)
{
}


Math::Random::DefaultRandomGenerator::~DefaultRandomGenerator()
{
}

int Math::Random::DefaultRandomGenerator::NextInt() const
{
	//return m_engine();
	return 0;
}

int Math::Random::DefaultRandomGenerator::NextInt(int lowerBound, int upperBound) const
{
	CHECK_CONDITION_RETURN_MATH(lowerBound <= upperBound, 0, Utility::Error, "Incorrect lower bound specified (%d). It is greater than upper bound (%d).", lowerBound, upperBound);
	int result = (rand() % (upperBound - lowerBound)) + lowerBound;
	return CheckIfInRange(result, lowerBound, upperBound) ? result : lowerBound;
}

Math::Real Math::Random::DefaultRandomGenerator::NextFloat() const
{
	return static_cast<Math::Real>(rand()) / RAND_MAX;
}

Math::Real Math::Random::DefaultRandomGenerator::NextFloat(Real lowerBound, Real upperBound) const
{
	CHECK_CONDITION_RETURN_MATH(lowerBound <= upperBound, REAL_ZERO, Utility::Error, "Incorrect lower bound specified (%d). It is greater than upper bound (%d).", lowerBound, upperBound);
	Math::Real result = (NextFloat() * (upperBound - lowerBound)) + lowerBound;
	return CheckIfInRange(result, lowerBound, upperBound) ? result : lowerBound;
}
/* ==================== DefaultRandomGenerator class end ==================== */