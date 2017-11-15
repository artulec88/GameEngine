#include "stdafx.h"
#include "RandomGeneratorFactory.h"
#include "Utility\ILogger.h"

/* static */ math::random::RandomGeneratorFactory& math::random::RandomGeneratorFactory::GetRandomGeneratorFactory()
{
	static RandomGeneratorFactory randomGeneratorFactory;
	return randomGeneratorFactory;
}

math::random::RandomGeneratorFactory::RandomGeneratorFactory()
{
	DELOCUST_LOG_MATH("Random generator factory created");
}


math::random::RandomGeneratorFactory::~RandomGeneratorFactory()
{
	m_randomGenerators.clear();
	DELOCUST_LOG_MATH("Random generator factory destroyed");
}

const math::random::RandomGenerator& math::random::RandomGeneratorFactory::GetRandomGenerator(generator_ids::GeneratorID randomGenerator, unsigned seed /* = ((unsigned int)time(NULL)) */)
{
	/// See: http://en.cppreference.com/w/cpp/container/map/emplace
	RandomGeneratorsMap::const_iterator randomGeneratorItr = m_randomGenerators.find(randomGenerator);
	if (randomGeneratorItr == m_randomGenerators.end()) // random generator not found in the map
	{
		switch (randomGenerator)
		{
		case generator_ids::SIMPLE:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, std::make_unique<SimpleRandomGenerator>(seed))).first->second);
		case generator_ids::DEFAULT_RANDOM_ENGINE:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, std::make_unique<SimpleRandomGenerator>(seed))).first->second);
		case generator_ids::DISCARD_BLOCK_ENGINE:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, std::make_unique<SimpleRandomGenerator>(seed))).first->second);
		case generator_ids::INDEPENDENT_BITS_ENGINE:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, std::make_unique<SimpleRandomGenerator>(seed))).first->second);
		case generator_ids::KNUTH_B:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, std::make_unique<SimpleRandomGenerator>(seed))).first->second);
		case generator_ids::LINEAR_CONGRUENTIAL_ENGINE:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, std::make_unique<SimpleRandomGenerator>(seed))).first->second);
		case generator_ids::MERSENNE_TWISTER_ENGINE:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, std::make_unique<SimpleRandomGenerator>(seed))).first->second);
		case generator_ids::MINSTD_RAND:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, std::make_unique<SimpleRandomGenerator>(seed))).first->second);
		case generator_ids::MINSTD_RAND0:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, std::make_unique<SimpleRandomGenerator>(seed))).first->second);
		case generator_ids::MT19937:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, std::make_unique<SimpleRandomGenerator>(seed))).first->second);
		case generator_ids::MT19937_64:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, std::make_unique<SimpleRandomGenerator>(seed))).first->second);
		case generator_ids::RANDOM_DEVICE:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, std::make_unique<SimpleRandomGenerator>(seed))).first->second);
		case generator_ids::RANLUX24:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, std::make_unique<SimpleRandomGenerator>(seed))).first->second);
		case generator_ids::RANLUX24_BASE:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, std::make_unique<SimpleRandomGenerator>(seed))).first->second);
		case generator_ids::RANLUX48:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, std::make_unique<SimpleRandomGenerator>(seed))).first->second);
		case generator_ids::RANLUX48_BASE:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, std::make_unique<SimpleRandomGenerator>(seed))).first->second);
		case generator_ids::SHUFFLE_ORDER_ENGINE:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, std::make_unique<SimpleRandomGenerator>(seed))).first->second);
		case generator_ids::SUBTRACT_WITH_CARRY_ENGINE:
			return *(m_randomGenerators.insert(std::make_pair(randomGenerator, std::make_unique<SimpleRandomGenerator>(seed))).first->second);
		}
	}
	return *randomGeneratorItr->second;
}