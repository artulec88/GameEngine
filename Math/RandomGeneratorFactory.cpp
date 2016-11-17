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
	DELOCUST_LOG_MATH("Random generator factory created");
}


Math::Random::RandomGeneratorFactory::~RandomGeneratorFactory()
{
	m_randomGenerators.clear();
	DELOCUST_LOG_MATH("Random generator factory destroyed");
}

const Math::Random::RandomGenerator& Math::Random::RandomGeneratorFactory::GetRandomGenerator(GeneratorIDs::GeneratorID randomGenerator, unsigned seed /* = ((unsigned int)time(NULL)) */)
{
	/// See: http://en.cppreference.com/w/cpp/container/map/emplace
	RandomGeneratorsMap::const_iterator randomGeneratorItr = m_randomGenerators.find(randomGenerator);
	if (randomGeneratorItr == m_randomGenerators.end()) // random generator not found in the map
	{
		switch (randomGenerator)
		{
		case GeneratorIDs::SIMPLE:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, std::make_unique<SimpleRandomGenerator>(seed))).first->second);
		case GeneratorIDs::DEFAULT_RANDOM_ENGINE:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, std::make_unique<SimpleRandomGenerator>(seed))).first->second);
		case GeneratorIDs::DISCARD_BLOCK_ENGINE:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, std::make_unique<SimpleRandomGenerator>(seed))).first->second);
		case GeneratorIDs::INDEPENDENT_BITS_ENGINE:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, std::make_unique<SimpleRandomGenerator>(seed))).first->second);
		case GeneratorIDs::KNUTH_B:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, std::make_unique<SimpleRandomGenerator>(seed))).first->second);
		case GeneratorIDs::LINEAR_CONGRUENTIAL_ENGINE:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, std::make_unique<SimpleRandomGenerator>(seed))).first->second);
		case GeneratorIDs::MERSENNE_TWISTER_ENGINE:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, std::make_unique<SimpleRandomGenerator>(seed))).first->second);
		case GeneratorIDs::MINSTD_RAND:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, std::make_unique<SimpleRandomGenerator>(seed))).first->second);
		case GeneratorIDs::MINSTD_RAND0:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, std::make_unique<SimpleRandomGenerator>(seed))).first->second);
		case GeneratorIDs::MT19937:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, std::make_unique<SimpleRandomGenerator>(seed))).first->second);
		case GeneratorIDs::MT19937_64:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, std::make_unique<SimpleRandomGenerator>(seed))).first->second);
		case GeneratorIDs::RANDOM_DEVICE:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, std::make_unique<SimpleRandomGenerator>(seed))).first->second);
		case GeneratorIDs::RANLUX24:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, std::make_unique<SimpleRandomGenerator>(seed))).first->second);
		case GeneratorIDs::RANLUX24_BASE:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, std::make_unique<SimpleRandomGenerator>(seed))).first->second);
		case GeneratorIDs::RANLUX48:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, std::make_unique<SimpleRandomGenerator>(seed))).first->second);
		case GeneratorIDs::RANLUX48_BASE:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, std::make_unique<SimpleRandomGenerator>(seed))).first->second);
		case GeneratorIDs::SHUFFLE_ORDER_ENGINE:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, std::make_unique<SimpleRandomGenerator>(seed))).first->second);
		case GeneratorIDs::SUBTRACT_WITH_CARRY_ENGINE:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, std::make_unique<SimpleRandomGenerator>(seed))).first->second);
		}
	}
	return *randomGeneratorItr->second;
}