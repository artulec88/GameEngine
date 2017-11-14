#include "PizzaBuilder.h"

/* ==================== PizzaBuilder implementation begin ==================== */
utility_test::PizzaBuilder::PizzaBuilder() :
	utility::Builder<Pizza>(),
	M_DEFAULT_PIZZA_DOUGH("defaultDough"),
	M_DEFAULT_PIZZA_SAUCE("defaultSauce"),
	M_DEFAULT_PIZZA_TOPPING("defaultTopping"),
	m_dough(M_DEFAULT_PIZZA_DOUGH),
	m_sauce(M_DEFAULT_PIZZA_SAUCE),
	m_topping(M_DEFAULT_PIZZA_TOPPING)
{
}

utility_test::PizzaBuilder::~PizzaBuilder()
{
}
/* ==================== PizzaBuilder implementation end ==================== */

/* ==================== HawaiianPizzaBuilder implementation begin ==================== */
utility_test::HawaiianPizzaBuilder::HawaiianPizzaBuilder() :
	PizzaBuilder()
{
}

utility_test::HawaiianPizzaBuilder::~HawaiianPizzaBuilder()
{
}

void utility_test::HawaiianPizzaBuilder::BuildDough()
{
}

void utility_test::HawaiianPizzaBuilder::BuildSauce()
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

void utility_test::HawaiianPizzaBuilder::BuildTopping()
{
}
/* ==================== HawaiianPizzaBuilder implementation end ==================== */

/* ==================== SpicyPizzaBuilder implementation begin ==================== */
utility_test::SpicyPizzaBuilder::SpicyPizzaBuilder() :
	PizzaBuilder()
{
}

utility_test::SpicyPizzaBuilder::~SpicyPizzaBuilder()
{
}

void utility_test::SpicyPizzaBuilder::BuildDough()
{
}

void utility_test::SpicyPizzaBuilder::BuildSauce()
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

void utility_test::SpicyPizzaBuilder::BuildTopping()
{
}
/* ==================== SpicyPizzaBuilder implementation end ==================== */