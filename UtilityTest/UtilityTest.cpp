#include "Utility\ICommand.h"
#include "Utility\ILogger.h"
#include "Utility\Time.h"
#include "Utility\Array.h"
//#include "Utility\Array_impl.h"
#include "Utility\StringUtility.h"

#include <ctime>
#include <string>

using namespace std;
using namespace Utility;

unsigned int testNumber = 0;

bool arrayTestEnabled = true;
bool stringUtilityTestEnabled = true;

void PrintHelp()
{
	cout << "usage: UtilityTest.exe [OPTIONS]" << endl;
	cout << "\t-log LoggingLevel" << endl;
	cout << endl;
}

void ReportError(const std::string& reportStr)
{
	ERROR_LOG("Test #%d FAILED. %s", testNumber, reportStr.c_str());
}

void TestReport(bool statusCode /* false if error */, const std::string& reportErrorStr)
{
	testNumber++;
	if (statusCode == false)
	{
		ReportError(reportErrorStr);
	}
	INFO_LOG("Test #%d passed", testNumber);
}

void ArrayTest()
{
	if (!arrayTestEnabled)
	{
		return;
	}

	NOTICE_LOG("Array test started");
	/* ==================== ARRAY TEST #1- integer array testing- begin ==================== */
	Container::Array<int> intArray(8, 2);
	intArray.SetElement(0, 1);
	intArray.SetElement(2, 5);
	TestReport(intArray.GetSize() == intArray.GetMaxSize(), "The size of the integer array and its maximum size should be equal at this point.");
	for (int i = 0; i < intArray.GetSize(); ++i)
	{
		INFO_LOG("intArray[%d] = %d", i, intArray[i]);
	}
	/* ==================== ARRAY TEST #1- integer array testing- end ==================== */

	/* ==================== ARRAY TEST #2- float array testing- begin ==================== */
	Container::Array<float> floatArray(4, 2);
	floatArray.SetElement(0, 1.2f);
	floatArray.SetElement(2, 5.5f);
	floatArray[1] = 2.4f;
	floatArray[3] = 4.8f;
	TestReport(floatArray.GetSize() == floatArray.GetMaxSize(), "The size of the float array and its maximum size should be equal at this point.");
	for (int i = 0; i < floatArray.GetSize(); ++i)
	{
		INFO_LOG("floatArray[%d] = %.3f", i, floatArray[i]);
	}
	/* ==================== ARRAY TEST #2- float array testing- end ==================== */
	NOTICE_LOG("Array test finished");
}

void StringUtilityTest()
{
	if (!stringUtilityTestEnabled)
	{
		return;
	}

	NOTICE_LOG("String utility test started");
	/* ==================== STRING UTILITY TEST #1- left trimming operation- begin ==================== */
	std::string str1("	\n\t  str1  \t	");
	StringUtility::LeftTrim(str1);
	std::string str2("str1");
	std::string str3("str1  \t	");
	TestReport(str1.compare(str2) != 0, "Left-trimming operation failed. The given strings should not be equal to each other but they are.");
	TestReport(str1.compare(str3) == 0, "Left-trimming operation failed. The given strings should be equal to each other but they are not.");
	/* ==================== STRING UTILITY TEST #1- left trimming operation- end ==================== */

	/* ==================== STRING UTILITY TEST #2- right trimming operation- begin ==================== */
	str1 = "	\n\t  str4		\n	\t ";
	StringUtility::RightTrim(str1);
	str2 = "str1";
	str3 = "	\n\t  str4";
	TestReport(str1.compare(str2) != 0, "Right-trimming operation failed. The given strings should not be equal to each other but they are.");
	TestReport(str1.compare(str3) == 0, "Right-trimming operation failed. The given strings should be equal to each other but they are not.");
	/* ==================== STRING UTILITY TEST #2- right trimming operation- end ==================== */

	/* ==================== STRING UTILITY TEST #3- trimming operation- begin ==================== */
	/* ==================== STRING UTILITY TEST #3- trimming operation- end ==================== */
	NOTICE_LOG("String utility test finished");
}

int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));
	ICommand::SetCommand(argc, argv);
	if (ICommand::GetCommand().IsPresent("-help"))
	{
		PrintHelp();
		system("pause");
		return 0;
	}
	ILogger::GetLogger().Fill(ICommand::GetCommand().Get("-log", ""), Info);

	ArrayTest();
	StringUtilityTest();

	ILogger::GetLogger().ResetConsoleColor();
	std::cout << "Bye!" << std::endl;

	return 0;
}
