#ifndef __UTILITY_TEST_PIZZA_BUILDER_H__
#define __UTILITY_TEST_PIZZA_BUILDER_H__

#include "Utility/Builder.h"

namespace utility_test
{
	class Pizza;

	class PizzaBuilder : public utility::Builder<Pizza>
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		PizzaBuilder(const std::string& defaultDough, const std::string& defaultSauce, const std::string& defaultTopping);
		virtual ~PizzaBuilder();
		PizzaBuilder(PizzaBuilder& pizzaBuilder) = delete;
		PizzaBuilder(PizzaBuilder&& pizzaBuilder) = delete;
		PizzaBuilder& operator=(const PizzaBuilder& pizzaBuilder) = delete;
		PizzaBuilder& operator=(PizzaBuilder&& pizzaBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		void SetDefault() override
		{
			m_dough = m_defaultDough;
			m_sauce = m_defaultSauce;
			m_topping = m_defaultTopping;
		}

	protected:
		Pizza Build() override;
	public:
		std::string GetDough() const
		{
			return m_dough;
		}

		std::string GetSauce() const
		{
			return m_sauce;
		}

		std::string GetTopping() const
		{
			return m_topping;
		}

		PizzaBuilder& SetDough(const std::string& dough)
		{
			m_dough = dough;
			return *this;
		}
		PizzaBuilder& SetSauce(const std::string& sauce)
		{
			m_sauce = sauce;
			return *this;
		}
		PizzaBuilder& SetTopping(const std::string& topping)
		{
			m_topping = topping;
			return *this;
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		const std::string m_defaultDough; // TODO: Make it a static constexpr in the future.
		const std::string m_defaultSauce; // TODO: Make it a static constexpr in the future.
		const std::string m_defaultTopping; // TODO: Make it a static constexpr in the future.

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
		virtual ~HawaiianPizzaBuilder();
		HawaiianPizzaBuilder(HawaiianPizzaBuilder& hawaiianPizzaBuilder) = delete;
		HawaiianPizzaBuilder(HawaiianPizzaBuilder&& hawaiianPizzaBuilder) = delete;
		HawaiianPizzaBuilder& operator=(const HawaiianPizzaBuilder& hawaiianPizzaBuilder) = delete;
		HawaiianPizzaBuilder& operator=(HawaiianPizzaBuilder&& hawaiianPizzaBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		HawaiianPizzaBuilder& AddSauceIngredient(const std::string& ingredient)
		{
			if (m_sauce.empty())
			{
				m_sauce = ingredient;
			}
			else
			{
				m_sauce += "+" + ingredient;
			}
			return *this;
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class HawaiianPizzaBuilder */

	class SpicyPizzaBuilder : public PizzaBuilder
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		SpicyPizzaBuilder();
		virtual ~SpicyPizzaBuilder();
		SpicyPizzaBuilder(SpicyPizzaBuilder& spicyPizzaBuilder) = delete;
		SpicyPizzaBuilder(SpicyPizzaBuilder&& spicyPizzaBuilder) = delete;
		SpicyPizzaBuilder& operator=(const SpicyPizzaBuilder& spicyPizzaBuilder) = delete;
		SpicyPizzaBuilder& operator=(SpicyPizzaBuilder&& spicyPizzaBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		SpicyPizzaBuilder& AddSauceIngredient(const std::string& ingredient)
		{
			if (m_sauce.empty())
			{
				m_sauce = ingredient;
			}
			else
			{
				m_sauce += "+" + ingredient;
			}
			return *this;
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class SpicyPizzaBuilder */
} /* end namespace UtilityTest */

#endif /* __UTILITY_TEST_PIZZA_BUILDER_H__ */