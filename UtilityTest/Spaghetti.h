#ifndef __UTILITY_TEST_SPAGHETTI_H__
#define __UTILITY_TEST_SPAGHETTI_H__

#include <string>

namespace UtilityTest {
	class Spaghetti
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Spaghetti constructor. </summary>
		Spaghetti(const std::string& pasta, const std::string& sauce, const std::string& topping) :
			m_pasta(pasta),
			m_sauce(sauce),
			m_topping(topping)
		{
		}
		/// <summary> Spaghetti destructor. </summary>
		virtual ~Spaghetti()
		{
		}
		/// <summary> Spaghetti copy constructor. </summary>
		Spaghetti(Spaghetti& spaghetti) = delete;
		/// <summary> Spaghetti move constructor. </summary>
		Spaghetti(Spaghetti&& spaghetti) = delete;
		/// <summary> Spaghetti copy assignment operator. </summary>
		Spaghetti& operator=(const Spaghetti& spaghetti) = delete;
		/// <summary> Spaghetti move assignment operator. </summary>
		Spaghetti& operator=(Spaghetti&& spaghetti) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		void SetPasta(const std::string& pasta) { m_pasta = pasta; }
		void SetSauce(const std::string& sauce) { m_sauce = sauce; }
		void SetTopping(const std::string& topping) { m_topping = topping; }
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		std::string m_pasta;
		std::string m_sauce;
		std::string m_topping;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Spaghetti */

} /* end namespace UtilityTest */

#endif /* __UTILITY_TEST_SPAGHETTI_H__ */