#include "PizzaBuilder.h"

/* ==================== PizzaBuilder implementation begin ==================== */
UtilityTest::PizzaBuilder::PizzaBuilder() :
	Utility::Builder<Pizza>(),
	M_DEFAULT_PIZZA_DOUGH("defaultDough"),
	M_DEFAULT_PIZZA_SAUCE("defaultSauce"),
	M_DEFAULT_PIZZA_TOPPING("defaultTopping"),
	m_dough(M_DEFAULT_PIZZA_DOUGH),
	m_sauce(M_DEFAULT_PIZZA_SAUCE),
	m_topping(M_DEFAULT_PIZZA_TOPPING)
{
}

UtilityTest::PizzaBuilder::~PizzaBuilder()
{
}
/* ==================== PizzaBuilder implementation end ==================== */

/* ==================== HawaiianPizzaBuilder implementation begin ==================== */
UtilityTest::HawaiianPizzaBuilder::HawaiianPizzaBuilder() :
	PizzaBuilder()
{
}

UtilityTest::HawaiianPizzaBuilder::~HawaiianPizzaBuilder(void)
{
}

void UtilityTest::HawaiianPizzaBuilder::BuildDough()
{
}

void UtilityTest::HawaiianPizzaBuilder::BuildSauce()
{
	if (!m_ingredients.empty())
	{
		m_sauce.clear();
		for (auto ingredientItr = m_ingredients.begin(); ingredientItr != m_ingredients.end(); ++ingredientItr)
		{
			m_sauce += *ingredientItr;
		}
	}
	else
	{
		PizzaBuilder::BuildSauce();
	}
}

void UtilityTest::HawaiianPizzaBuilder::BuildTopping()
{
}
/* ==================== HawaiianPizzaBuilder implementation end ==================== */

/* ==================== SpicyPizzaBuilder implementation begin ==================== */
UtilityTest::SpicyPizzaBuilder::SpicyPizzaBuilder() :
	PizzaBuilder()
{
}

UtilityTest::SpicyPizzaBuilder::~SpicyPizzaBuilder(void)
{
}

void UtilityTest::SpicyPizzaBuilder::BuildDough()
{
}

void UtilityTest::SpicyPizzaBuilder::BuildSauce()
{
	if (!m_ingredients.empty())
	{
		m_sauce.clear();
		for (auto ingredientItr = m_ingredients.begin(); ingredientItr != m_ingredients.end(); ++ingredientItr)
		{
			m_sauce += *ingredientItr;
		}
	}
	else
	{
		PizzaBuilder::BuildSauce();
	}
}

void UtilityTest::SpicyPizzaBuilder::BuildTopping()
{
}
/* ==================== SpicyPizzaBuilder implementation end ==================== */