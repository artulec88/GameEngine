#include "PizzaBuilder.h"
#include "Pizza.h"

#include "Utility/BuilderDirector.h"
#include "Utility/ICommandLineMapper.h"
#include "Utility/ILogger.h"
#include "Utility/Time.h"
#include "Utility/TimeSpan.h"
#include "Utility/Array.h"
#include "Utility/Array_impl.h"
#include "Utility/Vector.h"
#include "Utility/Vector_impl.h"
#include "Utility/StringUtility.h"

#include <ctime>
#include <string>
#include <thread>

using namespace std;
using namespace utility;
using namespace utility_test;

unsigned int testNumber = 0;

bool arrayTestEnabled = true;
bool vectorTestEnabled = true;
bool stringUtilityTestEnabled = true;
bool timeTestEnabled = true;

const std::string MODULE_NAME = "UtilityTest";

void PrintHelp()
{
	cout << "usage: UtilityTest.exe [OPTIONS]" << endl;
	cout << "\t-log LoggingLevel" << endl;
	cout << endl;
}

void ReportError(const std::string& reportStr)
{
	ERROR_LOG(MODULE_NAME, "Test #", testNumber, " FAILED. ", reportStr);
}

void TestReport(bool statusCode /* false if error */, const std::string& reportErrorStr)
{
	testNumber++;
	if (!statusCode)
	{
		ReportError(reportErrorStr);
	}
	else
	{
		INFO_LOG(MODULE_NAME, "Test #", testNumber, " passed");
	}
}

void ArrayTest()
{
	if (!arrayTestEnabled)
	{
		return;
	}

	NOTICE_LOG(MODULE_NAME, "Array test started");
	/* ==================== ARRAY TEST #1- integer array testing- begin ==================== */
	container::Array<int, 8> intArray;
	intArray.SetElement(0, 1);
	intArray.SetElement(2, 5);
	intArray.SetElement(8, 2); // This line should log a Index-Out-Of-Range error message.
	for (unsigned int i = 0; i < intArray.GetSize(); ++i)
	{
		INFO_LOG(MODULE_NAME, "intArray[", i, "] = ", intArray[i]);
	}
	/* ==================== ARRAY TEST #1- integer array testing- end ==================== */

	/* ==================== ARRAY TEST #2- float array testing- begin ==================== */
	container::Array<float, 4> floatArray;
	floatArray.SetElement(0, 1.2f);
	floatArray.SetElement(2, 5.5f);
	floatArray.SetElement(4, 2.2f); // This line should log a Index-Out-Of-Range error message.
	floatArray[1] = 2.4f;
	floatArray[3] = 4.8f;
	floatArray[4] = 1.75f; // This line should log a Index-Out-Of-Range error message.
	for (unsigned int i = 0; i < floatArray.GetSize(); ++i)
	{
		INFO_LOG(MODULE_NAME, "floatArray[", i, "] = ", floatArray[i]);
	}
	/* ==================== ARRAY TEST #2- float array testing- end ==================== */
	NOTICE_LOG(MODULE_NAME, "Array test finished");
}

void VectorTest()
{
	if (!vectorTestEnabled)
	{
		return;
	}

	NOTICE_LOG(MODULE_NAME, "Vector test started");
	/* ==================== VECTOR TEST #1- integer array testing- begin ==================== */
	container::Vector<int> intVector(8, 2);
	intVector.SetElement(0, 1);
	intVector.SetElement(2, 5);
	intVector[3] = 4;
	TestReport(intVector.GetSize() == intVector.GetMaxSize(), "The size of the integer vector and its maximum size should be equal at this point.");
	for (unsigned int i = 0; i < intVector.GetSize(); ++i)
	{
		DEBUG_LOG(MODULE_NAME, "intVector[", i, "] = ", intVector[i]);
	}
	/* ==================== VECTOR TEST #1- integer array testing- end ==================== */

	/* ==================== VECTOR TEST #2- float array testing- begin ==================== */
	container::Vector<float> floatVector(4, 2);
	floatVector.SetElement(0, 1.2f);
	floatVector.SetElement(2, 5.5f);
	floatVector[1] = 2.4f;
	floatVector[3] = 4.8f;
	TestReport(floatVector.GetSize() == floatVector.GetMaxSize(), "The size of the float vector and its maximum size should be equal at this point.");
	for (unsigned int i = 0; i < floatVector.GetSize(); ++i)
	{
		DEBUG_LOG(MODULE_NAME, "floatVector[", i, "] = ", floatVector[i]);
	}
	/* ==================== VECTOR TEST #2- float array testing- end ==================== */
	NOTICE_LOG(MODULE_NAME, "Vector test finished");
}

void StringUtilityTest()
{
	if (!stringUtilityTestEnabled)
	{
		return;
	}

	NOTICE_LOG(MODULE_NAME, "String utility test started");
	/* ==================== STRING UTILITY TEST #1- left trimming operation- begin ==================== */
	std::string str1("	\n\t  str1  \t	");
	string_utility::LeftTrim(str1);
	std::string str2("str1");
	std::string str3("str1  \t	");
	TestReport(str1.compare(str2) != 0, "Left-trimming operation failed. The given strings should not be equal to each other but they are.");
	TestReport(str1.compare(str3) == 0, "Left-trimming operation failed. The given strings should be equal to each other but they are not.");
	/* ==================== STRING UTILITY TEST #1- left trimming operation- end ==================== */

	/* ==================== STRING UTILITY TEST #2- right trimming operation- begin ==================== */
	str1 = "	\n\t  str4		\n	\t ";
	string_utility::RightTrim(str1);
	str2 = "str1";
	str3 = "	\n\t  str4";
	TestReport(str1.compare(str2) != 0, "Right-trimming operation failed. The given strings should not be equal to each other but they are.");
	TestReport(str1.compare(str3) == 0, "Right-trimming operation failed. The given strings should be equal to each other but they are not.");
	/* ==================== STRING UTILITY TEST #2- right trimming operation- end ==================== */

	/* ==================== STRING UTILITY TEST #3- trimming operation- begin ==================== */
	/* ==================== STRING UTILITY TEST #3- trimming operation- end ==================== */
	NOTICE_LOG(MODULE_NAME, "String utility test finished");
}

void TimeTest()
{
	if (!timeTestEnabled)
	{
		return;
	}

	NOTICE_LOG(MODULE_NAME, "Time span test started");
	/* ==================== TIME TEST #1- TimeSpan operations- begin ==================== */
	timing::TimeSpan timeSpan1(3);
	timing::TimeSpan timeSpan2(4, timing::MILLISECOND);
	timing::TimeSpan timeSpan3(4000, timing::MICROSECOND);
	timing::TimeSpan timeSpan4(3, timing::NANOSECOND);
	timing::TimeSpan timeSpan5(timeSpan1);
	timing::TimeSpan timeSpan6(2, timing::MILLISECOND);

	TestReport(!(timeSpan1 == timeSpan2), "The operator == returned incorrect result. TimeSpan: " + timeSpan1.ToString(timing::NANOSECOND) + " is not equal to timespan: " + timeSpan2.ToString(timing::NANOSECOND));
	TestReport(timeSpan1 != timeSpan2, "The operator != returned incorrect result. TimeSpan: " + timeSpan1.ToString(timing::NANOSECOND) + " is not equal to timespan: " + timeSpan2.ToString(timing::NANOSECOND));
	TestReport(timeSpan2 > timeSpan1, "The operator > returned incorrect result. TimeSpan: " + timeSpan2.ToString(timing::NANOSECOND) + " should be bigger than timespan: " + timeSpan1.ToString(timing::NANOSECOND));
	TestReport(timeSpan1 < timeSpan2, "The operator < returned incorrect result. TimeSpan: " + timeSpan1.ToString(timing::NANOSECOND) + " should be smaller than timespan: " + timeSpan2.ToString(timing::NANOSECOND));

	TestReport(timeSpan1 == timeSpan4, "The operator == returned incorrect result. TimeSpan: " + timeSpan1.ToString(timing::NANOSECOND) + " is equal to timespan: " + timeSpan4.ToString(timing::NANOSECOND));
	TestReport(!(timeSpan1 != timeSpan4), "The operator != returned incorrect result. TimeSpan: " + timeSpan1.ToString(timing::NANOSECOND) + " is equal to timespan: " + timeSpan4.ToString(timing::NANOSECOND));
	TestReport(!(timeSpan4 > timeSpan1), "The operator > returned incorrect result. TimeSpan: " + timeSpan4.ToString(timing::NANOSECOND) + " should not be bigger than timespan: " + timeSpan1.ToString(timing::NANOSECOND));
	TestReport(!(timeSpan1 < timeSpan4), "The operator < returned incorrect result. TimeSpan: " + timeSpan1.ToString(timing::NANOSECOND) + " should not be smaller than timespan: " + timeSpan4.ToString(timing::NANOSECOND));

	TestReport(timeSpan1 == timeSpan5, "The operator == returned incorrect result. TimeSpan: " + timeSpan1.ToString(timing::NANOSECOND) + " is equal to timespan: " + timeSpan5.ToString(timing::NANOSECOND));
	TestReport(!(timeSpan1 != timeSpan5), "The operator != returned incorrect result. TimeSpan: " + timeSpan1.ToString(timing::NANOSECOND) + " is equal to timespan: " + timeSpan5.ToString(timing::NANOSECOND));
	TestReport(!(timeSpan5 > timeSpan1), "The operator > returned incorrect result. TimeSpan: " + timeSpan5.ToString(timing::NANOSECOND) + " should not be bigger than timespan: " + timeSpan1.ToString(timing::NANOSECOND));
	TestReport(!(timeSpan1 < timeSpan5), "The operator < returned incorrect result. TimeSpan: " + timeSpan1.ToString(timing::NANOSECOND) + " should not be smaller than timespan: " + timeSpan5.ToString(timing::NANOSECOND));

	TestReport(timeSpan2 == timeSpan3, "The operator == returned incorrect result. TimeSpan: " + timeSpan2.ToString(timing::NANOSECOND) + " is equal to timespan: " + timeSpan3.ToString(timing::NANOSECOND));
	TestReport(!(timeSpan2 != timeSpan3), "The operator != returned incorrect result. TimeSpan: " + timeSpan2.ToString(timing::NANOSECOND) + " is equal to timespan: " + timeSpan3.ToString(timing::NANOSECOND));
	TestReport(!(timeSpan3 > timeSpan2), "The operator > returned incorrect result. TimeSpan: " + timeSpan3.ToString(timing::NANOSECOND) + " should not be bigger than timespan: " + timeSpan2.ToString(timing::NANOSECOND));
	TestReport(!(timeSpan2 < timeSpan3), "The operator < returned incorrect result. TimeSpan: " + timeSpan2.ToString(timing::NANOSECOND) + " should not be smaller than timespan: " + timeSpan3.ToString(timing::NANOSECOND));

	//timeSpan2 /= 2.0;

	//TestReport(timeSpan2 == timeSpan6, "The operator == returned incorrect result. TimeSpan: " + timeSpan2.ToString() + " is equal to timespan: " + timeSpan6.ToString());
	//TestReport(!(timeSpan2 != timeSpan6), "The operator != returned incorrect result. TimeSpan: " + timeSpan2.ToString() + " is equal to timespan: " + timeSpan6.ToString());
	//TestReport(!(timeSpan6 > timeSpan2), "The operator > returned incorrect result. TimeSpan: " + timeSpan6.ToString() + " should not be bigger than timespan: " + timeSpan2.ToString());
	//TestReport(!(timeSpan2 < timeSpan6), "The operator < returned incorrect result. TimeSpan: " + timeSpan2.ToString() + " should not be smaller than timespan: " + timeSpan6.ToString());
	/* ==================== TIME TEST #1- TimeSpan operations- end ==================== */

	/* ==================== TIME TEST #2- DateTime operations- begin ==================== */
	utility::timing::DateTime dateTime1(2016, 2, 22, 10, 0, 0);
	utility::timing::DateTime dateTime2(dateTime1);
	utility::timing::DateTime dateTime3(2016, 2, 22, 9, 0, 0);
	utility::timing::DateTime dateTime4(2016, 1, 0, 9, 0, 0);
	utility::timing::DateTime dateTime5(2016, 11, 30, 23, 59, 59);

	TestReport(dateTime1 == dateTime2, "The operator == returned incorrect result. DateTime: " + dateTime1.ToString() + " should be equal to dateTime: " + dateTime2.ToString());
	TestReport(!(dateTime1 != dateTime2), "The operator != returned incorrect result. DateTime: " + dateTime1.ToString() + " should be equal to dateTime: " + dateTime2.ToString());
	TestReport(dateTime1 > dateTime3, "The operator > returned incorrect result. DateTime: " + dateTime1.ToString() + " should be later than dateTime: " + dateTime3.ToString());
	TestReport(dateTime1 > dateTime4, "The operator > returned incorrect result. DateTime: " + dateTime1.ToString() + " should be later than dateTime: " + dateTime4.ToString());
	TestReport(dateTime1 < dateTime5, "The operator < returned incorrect result. DateTime: " + dateTime1.ToString() + " should be earlier than dateTime: " + dateTime5.ToString());

	//TestReport(dateTime1.GetDayTime() > dateTime3.GetDayTime(), "Incorrect day time calculationg. Day time for dateTime: " +
	//	dateTime1.ToString("%H:%M:%S") + " should be greater than day time for dateTime: " + dateTime3.ToString("%H:%M:%S"));
	//TestReport(dateTime2.GetDayTime() < dateTime5.GetDayTime(), "Incorrect day time calculationg. Day time for dateTime: " +
	//	dateTime2.ToString("%H:%M:%S") + " should be less than day time for dateTime: " + dateTime5.ToString("%H:%M:%S"));
	//TestReport(dateTime4.GetDayInYear() == 0, "The dateTime: " + dateTime4.ToString("%Y:%m:%d") + " should be the first day in year.");
	//TestReport(dateTime5.GetDayInYear() == 364, "The dateTime: " + dateTime4.ToString("%Y:%m:%d") + " should be the last day in year.");

	dateTime1 += timing::TimeSpan(64, timing::MINUTE);
	TestReport(dateTime1 > dateTime2, "The operator > returned incorrect result. DateTime: " + dateTime1.ToString() + " should be later than dateTime: " + dateTime2.ToString());
	TestReport(!(dateTime1 == dateTime2), "The operator == returned incorrect result. DateTime: " + dateTime1.ToString() + " should not be equal to dateTime: " + dateTime2.ToString());
	TestReport(dateTime1 != dateTime2, "The operator != returned incorrect result. DateTime: " + dateTime1.ToString() + " should not be equal to dateTime: " + dateTime2.ToString());
	TestReport(dateTime1 > dateTime3, "The operator > returned incorrect result. DateTime: " + dateTime1.ToString() + " should be later than dateTime: " + dateTime3.ToString());
	TestReport(dateTime1 > dateTime4, "The operator > returned incorrect result. DateTime: " + dateTime1.ToString() + " should be later than dateTime: " + dateTime4.ToString());
	TestReport(dateTime1 < dateTime5, "The operator < returned incorrect result. DateTime: " + dateTime1.ToString() + " should be earlier than dateTime: " + dateTime5.ToString());

	dateTime3 += timing::TimeSpan(3600, timing::SECOND);
	TestReport(dateTime3 == dateTime2, "The operator == returned incorrect result. DateTime: " + dateTime3.ToString() + " should be equal to dateTime: " + dateTime2.ToString());
	TestReport(!(dateTime3 != dateTime2), "The operator != returned incorrect result. DateTime: " + dateTime3.ToString() + " should be equal to dateTime: " + dateTime2.ToString());
	dateTime3 += timing::TimeSpan(42141, timing::HOUR);
	const timing::DateTime dateTime6(2020, 12, 13, 7, 0, 0);
	TestReport(dateTime3 == dateTime6, "The operator == returned incorrect result. DateTime: " + dateTime3.ToString() + " should be equal to dateTime: " + dateTime6.ToString());
	TestReport(!(dateTime3 != dateTime6), "The operator != returned incorrect result. DateTime: " + dateTime3.ToString() + " should be equal to dateTime: " + dateTime6.ToString());
	TestReport(dateTime3 > dateTime4, "The operator > returned incorrect result. DateTime: " + dateTime3.ToString() + " should be later than dateTime: " + dateTime4.ToString());
	TestReport(dateTime4 < dateTime3, "The operator < returned incorrect result. DateTime: " + dateTime4.ToString() + " should be earlier than dateTime: " + dateTime3.ToString());
	TestReport(dateTime3 > dateTime5, "The operator > returned incorrect result. DateTime: " + dateTime3.ToString() + " should be later than dateTime: " + dateTime5.ToString());
	TestReport(dateTime5 < dateTime3, "The operator < returned incorrect result. DateTime: " + dateTime5.ToString() + " should be earlier than dateTime: " + dateTime3.ToString());

	dateTime3 -= timing::TimeSpan(42141, timing::HOUR);
	TestReport(dateTime3 == dateTime2, "The operator == returned incorrect result. DateTime: " + dateTime3.ToString() + " should be equal to dateTime: " + dateTime2.ToString());
	TestReport(!(dateTime3 != dateTime2), "The operator != returned incorrect result. DateTime: " + dateTime3.ToString() + " should be equal to dateTime: " + dateTime2.ToString());
	const timing::DateTime dateTime7 = dateTime3 - timing::TimeSpan(3600, timing::SECOND);
	TestReport(dateTime3 > dateTime7, "The operator > returned incorrect result. DateTime: " + dateTime3.ToString() + " should be later than dateTime: " + dateTime7.ToString());
	TestReport(dateTime7 < dateTime3, "The opearator < returned incorrect result. DateTime: " + dateTime7.ToString() + " should be earlier than dateTime: " + dateTime3.ToString());

	dateTime4 += timing::TimeSpan(5200, timing::MILLISECOND);
	dateTime4 += timing::TimeSpan(5200, timing::MILLISECOND);
	dateTime4 += timing::TimeSpan(5200, timing::MILLISECOND);
	dateTime4 += timing::TimeSpan(5200, timing::MILLISECOND);
	dateTime4 += timing::TimeSpan(5200, timing::MILLISECOND);
	const timing::DateTime dateTime8(2016, 1, 0, 9, 0, 21);
	TestReport(dateTime4 == dateTime8, "The opearator == returned incorrect result. DateTime: " + dateTime4.ToString() + " should be equal to dateTime: " + dateTime8.ToString());
	/* ==================== TIME TEST #2- DateTime operations- end ==================== */

	/* ==================== TIME TEST #3- Timer operations- begin ==================== */
	timing::Timer timer;
	timer.Start();
	std::this_thread::sleep_for(std::chrono::seconds(2));
	timer.Stop();
	TestReport(abs(timer.GetDuration(timing::NANOSECOND) - 2000000000) < 250000, "The timer doesn't calculate its duration properly. Duration equals " +
		timer.ToString() + " when it should be roughly equal to 2000000000 [ns]");
	TestReport(abs(timer.GetDuration(timing::MICROSECOND) - 2000000) < 250, "The timer doesn't calculate its duration properly. Duration equals " +
		timer.ToString(timing::MICROSECOND) + " when it should be roughly equal to 2000000 [us]");
	TestReport(abs(timer.GetDuration(timing::MILLISECOND) - 2000) < 1, "The timer doesn't calculate its duration properly. Duration equals " +
		timer.ToString(timing::MILLISECOND) + " when it should be roughly equal to 2000 [ms]");
	TestReport(timer.GetDuration(timing::SECOND) == 2, "The timer doesn't calculate its duration properly. Duration equals " +
		timer.ToString(timing::SECOND) + " when it should be equal to 2 [s]");
	TestReport(timer.GetDuration(timing::MINUTE) == 0, "The timer doesn't calculate its duration properly. Duration equals " +
		timer.ToString(timing::MINUTE) + " when it should be equal to 0 [M]");
	TestReport(timer.GetDuration(timing::HOUR) == 0, "The timer doesn't calculate its duration properly. Duration equals " +
		timer.ToString(timing::HOUR) + " when it should be equal to 0 [h]");
	/* ==================== TIME TEST #3- Timer operations- end ==================== */
	NOTICE_LOG(MODULE_NAME, "Time test finished");
}

void BuilderTest()
{
	HawaiianPizzaBuilder hawaiianPizzaBuilder;
	hawaiianPizzaBuilder.AddSauceIngredient("pineapple");
	BuilderDirector<Pizza> waiter(&hawaiianPizzaBuilder);
	Pizza pizza = waiter.Construct();
	NOTICE_LOG(MODULE_NAME, pizza);

	SpicyPizzaBuilder spicyPizzaBuilder;
	spicyPizzaBuilder.AddSauceIngredient("jalapeno").AddSauceIngredient("chili");
	spicyPizzaBuilder.SetTopping("someCustomSpicyTopping").SetDough("someCustomSpicyDough");
	waiter.SetBuilder(&spicyPizzaBuilder);
	pizza = waiter.Construct();
	NOTICE_LOG(MODULE_NAME, pizza);
	spicyPizzaBuilder.SetDefault();
	pizza = waiter.Construct();
	NOTICE_LOG(MODULE_NAME, pizza);
}

int main(int argc, char* argv[])
{
	srand(static_cast<unsigned int>(time(nullptr)));
	std::unique_ptr<ICommandLineMapper> commandLineMapper = ICommandLineMapper::CreateCommandLineMapper(argc, argv);
	if (commandLineMapper->IsPresent("-help"))
	{
		PrintHelp();
		system("pause");
		return 0;
	}
	logging::ILogger::GetLogger(MODULE_NAME).Fill(commandLineMapper->Get("-log", ""), logging::INFO);

	ArrayTest();
	VectorTest();
	//StringUtilityTest();
	//TimeTest();
	BuilderTest();

	logging::ILogger::GetLogger(MODULE_NAME).ResetConsoleColor();
	std::cout << "Bye!" << std::endl;

	return 0;
}
