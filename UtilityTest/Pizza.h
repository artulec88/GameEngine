#ifndef __UTILITY_TEST_PIZZA_H__
#define __UTILITY_TEST_PIZZA_H__

#include <string>
#include "PizzaBuilder.h"

namespace utility_test {

	class Pizza
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Pizza constructor. </summary>
		/// <param name="pizzaBuilder"> The reference to pizza builder object. </param>
		explicit Pizza(const PizzaBuilder* const pizzaBuilder) :
			m_dough(pizzaBuilder->GetDough()),
			m_sauce(pizzaBuilder->GetSauce()),
			m_topping(pizzaBuilder->GetTopping())
		{
		}

		/// <summary> Pizza destructor. </summary>
		~Pizza()
		{
		}

		/// <summary> Pizza copy constructor. </summary>
		Pizza(Pizza& pizza) = delete;

		/// <summary> Pizza move constructor. </summary>
		Pizza(Pizza&& pizza) = default;

		/// <summary> Pizza copy assignment operator. </summary>
		Pizza& operator=(const Pizza& pizza) = delete;

		/// <summary> Pizza move assignment operator. </summary>
		Pizza& operator=(Pizza&& pizza) = default;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		friend std::ostream& operator<<(std::ostream& out, const Pizza& pizza)
		{
			out << "Pizza dough = \"" << pizza.m_dough << "\"; sauce = \"" << pizza.m_sauce << "\"; topping = \"" << pizza.m_topping << "\".";
			return out;
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		std::string m_dough;
		std::string m_sauce;
		std::string m_topping;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Pizza */

} /* end namespace UtilityTest */

#endif /* __UTILITY_TEST_PIZZA_H__ */