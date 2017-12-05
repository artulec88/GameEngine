#include "PizzaBuilder.h"
#include "Pizza.h"

/* ==================== PizzaBuilder implementation begin ==================== */
utility_test::PizzaBuilder::PizzaBuilder(const std::string& defaultDough, const std::string& defaultSauce, const std::string& defaultTopping) :
	Builder<Pizza>(),
	m_defaultDough(defaultDough),
	m_defaultSauce(defaultSauce),
	m_defaultTopping(defaultTopping),
	m_dough(m_defaultDough),
	m_sauce(m_defaultSauce),
	m_topping(m_defaultTopping)
{
}

utility_test::PizzaBuilder::~PizzaBuilder()
{
}

utility_test::Pizza utility_test::PizzaBuilder::Build()
{
	return Pizza(this);
}
/* ==================== PizzaBuilder implementation end ==================== */

/* ==================== HawaiianPizzaBuilder implementation begin ==================== */
utility_test::HawaiianPizzaBuilder::HawaiianPizzaBuilder() :
	PizzaBuilder("cross", "mild", "ham+pineapple")
{
}

utility_test::HawaiianPizzaBuilder::~HawaiianPizzaBuilder()
{
}
/* ==================== HawaiianPizzaBuilder implementation end ==================== */

/* ==================== SpicyPizzaBuilder implementation begin ==================== */
utility_test::SpicyPizzaBuilder::SpicyPizzaBuilder() :
	PizzaBuilder("pan baked", "hot", "pepperoni+salami")
{
}

utility_test::SpicyPizzaBuilder::~SpicyPizzaBuilder()
{
}
/* ==================== SpicyPizzaBuilder implementation end ==================== */