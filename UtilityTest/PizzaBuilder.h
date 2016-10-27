#ifndef __UTILITY_TEST_PIZZA_BUILDER_H__
#define __UTILITY_TEST_PIZZA_BUILDER_H__

#include "Pizza.h"

#include "Utility\Builder.h"

#include <array>
#include <vector>

namespace UtilityTest
{

	class PizzaBuilder : public Utility::Builder<Pizza>
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		PizzaBuilder();
		virtual ~PizzaBuilder(void);
		PizzaBuilder(PizzaBuilder& pizzaBuilder) = delete;
		PizzaBuilder(PizzaBuilder&& pizzaBuilder) = delete;
		PizzaBuilder& operator=(const PizzaBuilder& pizzaBuilder) = delete;
		PizzaBuilder& operator=(PizzaBuilder&& pizzaBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual Pizza Get() override
		{
			return Pizza(m_dough, m_sauce, m_topping);
		}

		virtual void BuildPart1() override
		{
			BuildDough();
		}

		virtual void BuildPart2() override
		{
			BuildSauce();
		}

		virtual void BuildPart3() override
		{
			BuildTopping();
		}

		virtual void SetDefault()
		{
			m_dough = M_DEFAULT_PIZZA_DOUGH;
			m_sauce = M_DEFAULT_PIZZA_SAUCE;
			m_topping = M_DEFAULT_PIZZA_TOPPING;
		}

		PizzaBuilder& SetDough(const std::string& dough)
		{
			m_dough = dough;
			return *this;
		}
		PizzaBuilder& SetTopping(const std::string& topping)
		{
			m_topping = topping;
			return *this;
		}
	protected:
		virtual void BuildDough()
		{
			SetDough(M_DEFAULT_PIZZA_DOUGH);
		}
		virtual void BuildSauce()
		{
			SetSauce(M_DEFAULT_PIZZA_SAUCE);
		}
		virtual void BuildTopping()
		{
			SetTopping(M_DEFAULT_PIZZA_TOPPING);
		}

		PizzaBuilder& SetSauce(const std::string& sauce)
		{
			m_sauce = sauce;
			return *this;
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		const std::string M_DEFAULT_PIZZA_DOUGH; // TODO: Make it a static constexpr in the future.
		const std::string M_DEFAULT_PIZZA_SAUCE; // TODO: Make it a static constexpr in the future.
		const std::string M_DEFAULT_PIZZA_TOPPING; // TODO: Make it a static constexpr in the future.

		std::string m_dough;
		std::string m_sauce;
		std::string m_topping;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class PizzaBuilder */

	class HawaiianPizzaBuilder : public PizzaBuilder
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		HawaiianPizzaBuilder();
		virtual ~HawaiianPizzaBuilder(void);
		HawaiianPizzaBuilder(HawaiianPizzaBuilder& hawaiianPizzaBuilder) = delete;
		HawaiianPizzaBuilder(HawaiianPizzaBuilder&& hawaiianPizzaBuilder) = delete;
		HawaiianPizzaBuilder& operator=(const HawaiianPizzaBuilder& hawaiianPizzaBuilder) = delete;
		HawaiianPizzaBuilder& operator=(HawaiianPizzaBuilder&& hawaiianPizzaBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		HawaiianPizzaBuilder& AddSauceIngredient(const std::string& ingredient)
		{
			if (!m_ingredients.empty())
			{
				m_ingredients.push_back("; " + ingredient);
			}
			else
			{
				m_ingredients.push_back(ingredient);
			}
			return *this;
		}
	protected:
		virtual void BuildDough() override;
		virtual void BuildSauce() override;
		virtual void BuildTopping() override;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		std::vector<std::string> m_ingredients;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class HawaiianPizzaBuilder */

	class SpicyPizzaBuilder : public PizzaBuilder
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		SpicyPizzaBuilder();
		virtual ~SpicyPizzaBuilder(void);
		SpicyPizzaBuilder(SpicyPizzaBuilder& spicyPizzaBuilder) = delete;
		SpicyPizzaBuilder(SpicyPizzaBuilder&& spicyPizzaBuilder) = delete;
		SpicyPizzaBuilder& operator=(const SpicyPizzaBuilder& spicyPizzaBuilder) = delete;
		SpicyPizzaBuilder& operator=(SpicyPizzaBuilder&& spicyPizzaBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		SpicyPizzaBuilder& AddSauceIngredient(const std::string& ingredient)
		{
			if (!m_ingredients.empty())
			{
				m_ingredients.push_back("; " + ingredient);
			}
			else
			{
				m_ingredients.push_back(ingredient);
			}
			return *this;
		}
	protected:
		virtual void BuildDough() override;
		virtual void BuildSauce() override;
		virtual void BuildTopping() override;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		std::vector<std::string> m_ingredients;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class SpicyPizzaBuilder */
} /* end namespace UtilityTest */

#endif /* __UTILITY_TEST_PIZZA_BUILDER_H__ */