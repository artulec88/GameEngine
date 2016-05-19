#include "stdafx.h"
#include "RandomGeneratorFactory.h"
#include "Utility\ILogger.h"

/* static */ Math::Random::RandomGeneratorFactory& Math::Random::RandomGeneratorFactory::GetRandomGeneratorFactory()
{
	static RandomGeneratorFactory randomGeneratorFactory;
	return randomGeneratorFactory;
}

Math::Random::RandomGeneratorFactory::RandomGeneratorFactory()
{
	DEBUG_LOG_MATH("Random generator factory created");
}


Math::Random::RandomGeneratorFactory::~RandomGeneratorFactory()
{
	for (RandomGeneratorsMap::iterator randomGeneratorItr = m_randomGenerators.begin(); randomGeneratorItr != m_randomGenerators.end(); ++randomGeneratorItr)
	{
		SAFE_DELETE(randomGeneratorItr->second);
	}
	m_randomGenerators.clear();
	DEBUG_LOG_MATH("Random generator factory destroyed");
}

const Math::Random::RandomGenerator& Math::Random::RandomGeneratorFactory::GetRandomGenerator(Generators::Generator randomGenerator, unsigned seed /* = ((unsigned int)time(NULL)) */)
{
	/// See: http://en.cppreference.com/w/cpp/container/map/emplace
	RandomGeneratorsMap::const_iterator randomGeneratorItr = m_randomGenerators.find(randomGenerator);
	if (randomGeneratorItr == m_randomGenerators.end()) // random generator not found in the map
	{
		switch (randomGenerator)
		{
		case Generators::SIMPLE:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, new SimpleRandomGenerator(seed))).first->second);
		case Generators::DEFAULT_RANDOM_ENGINE:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, new SimpleRandomGenerator(seed))).first->second);
		case Generators::DISCARD_BLOCK_ENGINE:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, new SimpleRandomGenerator(seed))).first->second);
		case Generators::INDEPENDENT_BITS_ENGINE:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, new SimpleRandomGenerator(seed))).first->second);
		case Generators::KNUTH_B:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, new SimpleRandomGenerator(seed))).first->second);
		case Generators::LINEAR_CONGRUENTIAL_ENGINE:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, new SimpleRandomGenerator(seed))).first->second);
		case Generators::MERSENNE_TWISTER_ENGINE:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, new SimpleRandomGenerator(seed))).first->second);
		case Generators::MINSTD_RAND:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, new SimpleRandomGenerator(seed))).first->second);
		case Generators::MINSTD_RAND0:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, new SimpleRandomGenerator(seed))).first->second);
		case Generators::MT19937:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, new SimpleRandomGenerator(seed))).first->second);
		case Generators::MT19937_64:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, new SimpleRandomGenerator(seed))).first->second);
		case Generators::RANDOM_DEVICE:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, new SimpleRandomGenerator(seed))).first->second);
		case Generators::RANLUX24:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, new SimpleRandomGenerator(seed))).first->second);
		case Generators::RANLUX24_BASE:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, new SimpleRandomGenerator(seed))).first->second);
		case Generators::RANLUX48:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, new SimpleRandomGenerator(seed))).first->second);
		case Generators::RANLUX48_BASE:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, new SimpleRandomGenerator(seed))).first->second);
		case Generators::SHUFFLE_ORDER_ENGINE:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, new SimpleRandomGenerator(seed))).first->second);
		case Generators::SUBTRACT_WITH_CARRY_ENGINE:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, new SimpleRandomGenerator(seed))).first->second);
		}
	}
	return *randomGeneratorItr->second;
}