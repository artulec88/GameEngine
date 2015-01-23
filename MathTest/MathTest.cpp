#include "Math\Angle.h"
#include "Math\FloatingPoint.h"
#include "Math\Matrix.h"
#include "Math\Quaternion.h"
#include "Math\Vector.h"
#include "Math\ISort.h"

#include "Utility\ICommand.h"
#include "Utility\ILogger.h"
#include "Utility\Time.h"

#include <ctime>
#include <string>

using namespace Math;
using namespace Utility;
using namespace std;

unsigned int innerLoops = 200;
unsigned int outerLoops = 200;
clock_t outerBegin, outerEnd;
double elapsedSecs;

unsigned int testNumber = 0;
bool angleTestEnabled = true;
bool vectorTestEnabled = true;
bool matrixTestEnabled = true;
bool quaternionTestEnabled = true;
bool sortingTestEnabled = true;
bool otherTestEnabled = true;

void ReportError(const std::string& reportStr)
{
	LOG(Error, LOGPLACE, "Test #%d FAILED. %s", testNumber, reportStr.c_str());
}

void TestReport(bool statusCode /* false if error */, const std::string& reportErrorStr)
{
	testNumber++;
	if (statusCode == false)
	{
		ReportError(reportErrorStr);
	}
	LOG(Info, LOGPLACE, "Test #%d passed", testNumber);
}

void TimeReport(const std::string& reportStr, double elapsedSecs, const std::string& timeUnitStr = "[us]")
{
	LOG(Notice, LOGPLACE, "%s:\t%.3f %s", reportStr.c_str(), 1000000.0 * elapsedSecs, timeUnitStr.c_str());
}

void AngleTest()
{
	if (!angleTestEnabled)
	{
		return;
	}

	LOG (Notice, LOGPLACE, "Angle test started");
	/* ==================== ANGLE TEST #1- comparing angles- begin ==================== */
	Angle angle1(90.0);
	Angle angle2(90.0, Angle::DEGREE);
	Angle angle3(M_PI / 2.0f, Angle::RADIAN); // angle1 == angle2 == angle3
	Angle angle4(M_PI / 2.0f, Angle::DEGREE); // angle4 is different than angle1, angle2, angle3
	TestReport(angle1 == angle2, "The comparison operators of the Angle class are incorrect. angle1 == angle2");
	TestReport(angle1 == angle3, "The comparison operators of the Angle class are incorrect. angle1 == angle3");
	TestReport(angle2 == angle3, "The comparison operators of the Angle class are incorrect. angle2 == angle3");
	TestReport(angle1 != angle4, "The comparison operators of the Angle class are incorrect. angle1 != angle4");
	TestReport(angle2 != angle4, "The comparison operators of the Angle class are incorrect. angle2 != angle4");
	TestReport(angle3 != angle4, "The comparison operators of the Angle class are incorrect. angle3 != angle4");
	Angle angle5(45.0f);
	Angle angle6(M_PI / 4.0f, Angle::RADIAN); // angle5 is equal to angle6
	TestReport(angle1 > angle5, "The comparison operators of the Angle class are incorrect. angle1 > angle5");
	TestReport(angle1 > angle6, "The comparison operators of the Angle class are incorrect. angle1 > angle6");
	TestReport(angle1 >= angle5, "The comparison operators of the Angle class are incorrect. angle1 >= angle5");
	TestReport(angle1 >= angle6, "The comparison operators of the Angle class are incorrect. angle1 >= angle6");
	TestReport(angle5 < angle1, "The comparison operators of the Angle class are incorrect. angle5 < angle1");
	TestReport(angle6 < angle1, "The comparison operators of the Angle class are incorrect. angle6 < angle1");
	TestReport(angle5 <= angle1, "The comparison operators of the Angle class are incorrect. angle5 <= angle1");
	TestReport(angle6 <= angle1, "The comparison operators of the Angle class are incorrect. angle6 <= angle1");
	TestReport(angle5 == angle6, "The comparison operators of the Angle class are incorrect. angle5 == angle6");
	TestReport(angle5 >= angle6, "The comparison operator of the Angle class are incorrect. angle5 >= angle6");
	TestReport(angle5 <= angle6, "The comparison operator of the Angle class are incorrect. angle5 <= angle6");
	TestReport(angle6 >= angle5, "The comparison operator of the Angle class are incorrect. angle6 >= angle5");
	TestReport(angle6 <= angle5, "The comparison operator of the Angle class are incorrect. angle6 <= angle5");
	/* ==================== ANGLE TEST #1- comparing angles- end ==================== */
	LOG (Notice, LOGPLACE, "Angle test finished");
}

void VectorTest()
{
	if (!vectorTestEnabled)
	{
		return;
	}

	LOG (Notice, LOGPLACE, "Vector test started");
	/* ==================== VECTOR2D TEST #1- comparing vectors- begin ==================== */
	Vector2D vec1(90.0, 90.0);
	Vector2D vec2(45.0, 90.0);
	Vector2D vec3(90.0, 45.0);
	Vector2D vec4(90.0, 90.0);
	TestReport(vec1 != vec2, "The comparison operators of the Vector2D class are incorrect. vec1 != vec2");
	TestReport(vec1 != vec3, "The comparison operators of the Vector2D class are incorrect. vec1 != vec3");
	TestReport(vec2 != vec3, "The comparison operators of the Vector2D class are incorrect. vec2 != vec3");
	TestReport(vec1 == vec4, "The comparison operators of the Vector2D class are incorrect. vec1 == vec4");
	TestReport(vec2 != vec3, "The comparison operators of the Vector2D class are incorrect. vec2 != vec3");
	TestReport(vec2 != vec4, "The comparison operators of the Vector2D class are incorrect. vec2 != vec4");
	TestReport(vec3 != vec4, "The comparison operators of the Vector2D class are incorrect. vec3 != vec4");
	//Angle angle5(45.0f);
	//Angle angle6(M_PI / 4.0f, false); // angle5 is equal to angle6
	//TestReport(angle1 > angle5, "The comparison operators of the Angle class are incorrect. angle1 > angle5");
	//TestReport(angle1 > angle6, "The comparison operators of the Angle class are incorrect. angle1 > angle6");
	//TestReport(angle1 >= angle5, "The comparison operators of the Angle class are incorrect. angle1 >= angle5");
	//TestReport(angle1 >= angle6, "The comparison operators of the Angle class are incorrect. angle1 >= angle6");
	//TestReport(angle5 < angle1, "The comparison operators of the Angle class are incorrect. angle5 < angle1");
	//TestReport(angle6 < angle1, "The comparison operators of the Angle class are incorrect. angle6 < angle1");
	//TestReport(angle5 <= angle1, "The comparison operators of the Angle class are incorrect. angle5 <= angle1");
	//TestReport(angle6 <= angle1, "The comparison operators of the Angle class are incorrect. angle6 <= angle1");
	//TestReport(angle5 == angle6, "The comparison operators of the Angle class are incorrect. angle5 == angle6");
	//TestReport(angle5 >= angle6, "The comparison operator of the Angle class are incorrect. angle5 >= angle6");
	//TestReport(angle5 <= angle6, "The comparison operator of the Angle class are incorrect. angle5 <= angle6");
	//TestReport(angle6 >= angle5, "The comparison operator of the Angle class are incorrect. angle6 >= angle5");
	//TestReport(angle6 <= angle5, "The comparison operator of the Angle class are incorrect. angle6 <= angle5");
	/* ==================== VECTOR2D TEST #1- comparing vectors- end ==================== */

	LOG (Notice, LOGPLACE, "Vector test finished");
}

void SortTest()
{
	if (!sortingTestEnabled)
	{
		return;
	}

	LOG (Notice, LOGPLACE, "Sorting test started");
	srand((unsigned int)time(NULL));
	const int NUMBER_OF_VECTORS = 10;
	const Real LOWER_BOUND_X = -20.0f;
	const Real HIGHER_BOUND_X = 20.0f;
	const Real LOWER_BOUND_Y = -20.0f;
	const Real HIGHER_BOUND_Y = 20.0f;	
	Vector2D* initialVectors = new Vector2D[NUMBER_OF_VECTORS];
	Vector2D* vectors = new Vector2D[NUMBER_OF_VECTORS];
	for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
	{
		initialVectors[i].SetX(LOWER_BOUND_X + static_cast<Math::Real>(rand()) /  static_cast<Math::Real>(RAND_MAX / (HIGHER_BOUND_X - LOWER_BOUND_X)));
		initialVectors[i].SetY(LOWER_BOUND_Y + static_cast<Math::Real>(rand()) /  static_cast<Math::Real>(RAND_MAX / (HIGHER_BOUND_Y - LOWER_BOUND_Y)));
		LOG(Debug, LOGPLACE, "initialVectors[%d] = %s", i, initialVectors[i].ToString().c_str());
	}

	const int NUMBER_OF_SORTING_METHODS = 1; /* the number of sorting methods in the Math library we want to check (10 means we want to check all of them) */
	const int chosenSortingMethodIndices[] = { 10 }; // its length must match the value of NUMBER_OF_SORTING_METHODS variable
	ISort::SortingMethod sortingMethods[] = { ISort::BUBBLE_SORT, ISort::INSERTION_SORT, ISort::SELECTION_SORT,
		ISort::MERGE_SORT, ISort::HEAP_SORT, ISort::QUICK_SORT, ISort::SHELL_SORT, ISort::COMB_SORT,
		ISort::COUNTING_SORT, ISort::RADIX_SORT, ISort::BUCKET_SORT };
	std::string sortingMethodsStr[] = { "Bubble sort", "Insertion sort", "Selection sort", "Merge sort",
		"Heap sort", "Quick sort", "Shell sort", "Comb sort", "Counting sort", "Radix sort", "Bucket sort" };

	const int NUMBER_OF_MICROSECONDS_IN_MILISECOND = 1000;
	const int NUMBER_OF_TIME_TESTS_ITERATION = 1;
	/**
	 * TODO: Instead of measuring the time needed to perform NUMBER_OF_TIME_TESTS_ITERATION sorts it should be more efficient to count the number of successful sorts within given period of time.
	 * If this time limit is passed then sorting is stopped no matter if it's finished or not.
	 */
	for (int sortingMethodIndex = 0; sortingMethodIndex < NUMBER_OF_SORTING_METHODS; ++sortingMethodIndex)
	{
		ISort* sorter = ISort::GetSortingObject(sortingMethods[chosenSortingMethodIndices[sortingMethodIndex]]);
		/* ==================== SORTING TEST #1- sorting Vector2D objects by X component ascending begin ==================== */
		bool sortingTestCasePassed = true;
		ISort::SortingKey sortingKey = ISort::COMPONENT_X;
		ISort::SortingDirection sortingDirection = ISort::ASCENDING;
		outerBegin = clock();
		for (int k = 0; k < NUMBER_OF_TIME_TESTS_ITERATION; ++k)
		{
			for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
			{
				vectors[i] = initialVectors[i];
			}
			sorter->Sort(vectors, NUMBER_OF_VECTORS, sortingKey, sortingDirection);
		}
		outerEnd = clock();
		elapsedSecs = static_cast<double>((outerEnd - outerBegin)) / (NUMBER_OF_MICROSECONDS_IN_MILISECOND * CLOCKS_PER_SEC * NUMBER_OF_TIME_TESTS_ITERATION);
		TimeReport("Average time for ASCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by X component", elapsedSecs, "[ms]");

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			if (vectors[i + 1].GetX() < vectors[i].GetX())
			{
				LOG(Error, LOGPLACE, "%s in ASCENDING order by X component failed. Vectors[%d] (%s) should precede Vectors[%d] (%s)",
					sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str(), i + 1, vectors[i + 1].ToString().c_str(), i, vectors[i].ToString().c_str());
				sortingTestCasePassed = false;
				break;
			}
		}
		LOG(Debug, LOGPLACE, "%s results (ASCENDING order by X component):\n", sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str());
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			LOG(Debug, LOGPLACE, "Vectors[%d] = %s", i, vectors[i].ToString().c_str());
		}
		TestReport(sortingTestCasePassed, sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " in ASCENDING order by X component failed.");
		/* ==================== SORTING TEST #1- sorting Vector2D objects by X component ascending end ==================== */

		/* ==================== SORTING TEST #2- sorting Vector2D objects by X component descending begin ==================== */
		sortingTestCasePassed = true;
		sortingKey = ISort::COMPONENT_X;
		sortingDirection = ISort::DESCENDING;
		outerBegin = clock();
		for (int k = 0; k < NUMBER_OF_TIME_TESTS_ITERATION; ++k)
		{
			for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
			{
				vectors[i] = initialVectors[i];
			}
			sorter->Sort(vectors, NUMBER_OF_VECTORS, sortingKey, sortingDirection);
		}
		outerEnd = clock();
		elapsedSecs = static_cast<double>((outerEnd - outerBegin)) / (NUMBER_OF_MICROSECONDS_IN_MILISECOND * CLOCKS_PER_SEC * NUMBER_OF_TIME_TESTS_ITERATION);
		TimeReport("Average time for DESCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by X component", elapsedSecs, "[ms]");

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			if (vectors[i + 1].GetX() > vectors[i].GetX())
			{
				LOG(Error, LOGPLACE, "%s in DESCENDING order by X component failed. Vectors[%d] (%s) should precede Vectors[%d] (%s)",
					sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str(), i + 1, vectors[i + 1].ToString().c_str(), i, vectors[i].ToString().c_str());
				sortingTestCasePassed = false;
				break;
			}
		}
		LOG(Debug, LOGPLACE, "%s results (DESCENDING order by X component):\n", sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str());
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			LOG(Debug, LOGPLACE, "Vectors[%d] = %s", i, vectors[i].ToString().c_str());
		}
		TestReport(sortingTestCasePassed, sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " in DESCENDING order by X component failed.");
		/* ==================== SORTING TEST #2- sorting Vector2D objects by X component descending end ==================== */

		/* ==================== SORTING TEST #3- sorting Vector2D objects by Y component ascending begin ==================== */
		sortingTestCasePassed = true;
		sortingKey = ISort::COMPONENT_Y;
		sortingDirection = ISort::ASCENDING;
		outerBegin = clock();
		for (int k = 0; k < NUMBER_OF_TIME_TESTS_ITERATION; ++k)
		{
			for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
			{
				vectors[i] = initialVectors[i];
			}
			sorter->Sort(vectors, NUMBER_OF_VECTORS, sortingKey, sortingDirection);
		}
		outerEnd = clock();
		elapsedSecs = static_cast<double>((outerEnd - outerBegin)) / (NUMBER_OF_MICROSECONDS_IN_MILISECOND * CLOCKS_PER_SEC * NUMBER_OF_TIME_TESTS_ITERATION);
		TimeReport("Average time for ASCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by Y component", elapsedSecs, "[ms]");

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			if (vectors[i + 1].GetY() < vectors[i].GetY())
			{
				LOG(Error, LOGPLACE, "%s in ASCENDING order by Y component failed. Vectors[%d] (%s) should precede Vectors[%d] (%s)",
					sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str(), i + 1, vectors[i + 1].ToString().c_str(), i, vectors[i].ToString().c_str());
				sortingTestCasePassed = false;
				break;
			}
		}
		LOG(Debug, LOGPLACE, "%s results (ASCENDING order by Y component):\n", sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str());
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			LOG(Debug, LOGPLACE, "Vectors[%d] = %s", i, vectors[i].ToString().c_str());
		}
		TestReport(sortingTestCasePassed, sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " in ASCENDING order by Y component failed.");
		/* ==================== SORTING TEST #3- sorting Vector2D objects by Y component ascending end ==================== */

		/* ==================== SORTING TEST #4- sorting Vector2D objects by Y component descending begin ==================== */
		sortingTestCasePassed = true;
		sortingKey = ISort::COMPONENT_Y;
		sortingDirection = ISort::DESCENDING;
		outerBegin = clock();
		for (int k = 0; k < NUMBER_OF_TIME_TESTS_ITERATION; ++k)
		{
			for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
			{
				vectors[i] = initialVectors[i];
			}
			sorter->Sort(vectors, NUMBER_OF_VECTORS, sortingKey, sortingDirection);
		}
		outerEnd = clock();
		elapsedSecs = static_cast<double>((outerEnd - outerBegin)) / (NUMBER_OF_MICROSECONDS_IN_MILISECOND * CLOCKS_PER_SEC * NUMBER_OF_TIME_TESTS_ITERATION);
		TimeReport("Average time for DESCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by Y component", elapsedSecs, "[ms]");

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			if (vectors[i + 1].GetY() > vectors[i].GetY())
			{
				LOG(Error, LOGPLACE, "%s in DESCENDING order by Y component failed. Vectors[%d] (%s) should precede Vectors[%d] (%s)",
					sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str(), i + 1, vectors[i + 1].ToString().c_str(), i, vectors[i].ToString().c_str());
				sortingTestCasePassed = false;
				break;
			}
		}
		LOG(Debug, LOGPLACE, "%s results (DESCENDING order by Y component):\n", sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str());
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			LOG(Debug, LOGPLACE, "Vectors[%d] = %s", i, vectors[i].ToString().c_str());
		}
		TestReport(sortingTestCasePassed, sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " in DESCENDING order by Y component failed.");
		/* ==================== SORTING TEST #4- sorting Vector2D objects by Y component descending end ==================== */

		/* ==================== SORTING TEST #5- sorting Vector2D objects by sum of squared components ascending begin ==================== */
		sortingTestCasePassed = true;
		sortingKey = ISort::SUM_OF_SQUARED_COMPONENTS;
		sortingDirection = ISort::ASCENDING;
		outerBegin = clock();
		for (int k = 0; k < NUMBER_OF_TIME_TESTS_ITERATION; ++k)
		{
			for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
			{
				vectors[i] = initialVectors[i];
			}
			sorter->Sort(vectors, NUMBER_OF_VECTORS, sortingKey, sortingDirection);
		}
		outerEnd = clock();
		elapsedSecs = static_cast<double>((outerEnd - outerBegin)) / (NUMBER_OF_MICROSECONDS_IN_MILISECOND * CLOCKS_PER_SEC * NUMBER_OF_TIME_TESTS_ITERATION);
		TimeReport("Average time for ASCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by sum of squared components", elapsedSecs, "[ms]");

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			if (vectors[i + 1].LengthSquared() < vectors[i].LengthSquared())
			{
				LOG(Error, LOGPLACE, "%s in ASCENDING order by sum of squared components failed. Vectors[%d] (%s) should precede Vectors[%d] (%s)",
					sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str(), i + 1, vectors[i + 1].ToString().c_str(), i, vectors[i].ToString().c_str());
				sortingTestCasePassed = false;
				break;
			}
		}
		LOG(Debug, LOGPLACE, "%s results (ASCENDING order by sum of squared components):\n", sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str());
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			LOG(Debug, LOGPLACE, "Vectors[%d] = %s", i, vectors[i].ToString().c_str());
		}
		TestReport(sortingTestCasePassed, sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " in ASCENDING order by sum of squared components failed.");
		/* ==================== SORTING TEST #5- sorting Vector2D objects by sum of squared components ascending end ==================== */

		/* ==================== SORTING TEST #6- sorting Vector2D objects by sum of squared components descending begin ==================== */
		sortingTestCasePassed = true;
		sortingKey = ISort::SUM_OF_SQUARED_COMPONENTS;
		sortingDirection = ISort::DESCENDING;
		outerBegin = clock();
		for (int k = 0; k < NUMBER_OF_TIME_TESTS_ITERATION; ++k)
		{
			for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
			{
				vectors[i] = initialVectors[i];
			}
			sorter->Sort(vectors, NUMBER_OF_VECTORS, sortingKey, sortingDirection);
		}
		outerEnd = clock();
		elapsedSecs = static_cast<double>((outerEnd - outerBegin)) / (NUMBER_OF_MICROSECONDS_IN_MILISECOND * CLOCKS_PER_SEC * NUMBER_OF_TIME_TESTS_ITERATION);
		TimeReport("Average time for DESCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by sum of squared components", elapsedSecs, "[ms]");

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			if (vectors[i + 1].LengthSquared() > vectors[i].LengthSquared())
			{
				LOG(Error, LOGPLACE, "%s in DESCENDING order by sum of squared components failed. Vectors[%d] (%s) should precede Vectors[%d] (%s)",
					sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str(), i + 1, vectors[i + 1].ToString().c_str(), i, vectors[i].ToString().c_str());
				sortingTestCasePassed = false;
				break;
			}
		}
		LOG(Debug, LOGPLACE, "%s results (DESCENDING order by sum of squared components):\n", sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str());
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			LOG(Debug, LOGPLACE, "Vectors[%d] = %s", i, vectors[i].ToString().c_str());
		}
		TestReport(sortingTestCasePassed, sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " in DESCENDING order by sum of squared components failed.");
		/* ==================== SORTING TEST #6- sorting Vector2D objects by sum of squared components descending end ==================== */
		delete sorter;
		sorter = NULL;
	}

	delete [] initialVectors;
	delete [] vectors;
	LOG (Notice, LOGPLACE, "Sorting test finished");
}

int main (int argc, char* argv[])
{
	ICommand::SetCommand(argc, argv);
	//if (Command.IsPresent("-help"))
	//{
	//	PrintHelp();
	//	system("pause");
	//	return 0;
	//}
	ILogger::GetLogger().Fill(ICommand::GetCommand().Get("-log", ""), Debug);

	Matrix4D identityMatrix1 = Matrix4D::Identity();
	Matrix4D identityMatrix2 = Matrix4D::Identity();

	AngleTest();
	VectorTest();

	/* ==================== MATRIX TEST #1 begin ==================== */
	if (matrixTestEnabled)
	{
		Matrix4D matrix1 = Matrix4D::Identity();
		TestReport(matrix1.IsIdentity(), "The function Matrix::IsIdentity() failed.");
		
		outerBegin = clock();
		for (unsigned int i = 0; i < outerLoops; ++i)
		{
			for (unsigned int j = 0; j < innerLoops; ++j)
			{
				Matrix4D matrix2 = Matrix4D::Identity();
			}
		}
		outerEnd = clock();
		elapsedSecs = static_cast<double>((outerEnd - outerBegin)) / (CLOCKS_PER_SEC * innerLoops * outerLoops);
		TimeReport("Average time for identity matrix creation:\t", elapsedSecs);
	}
	/* ==================== MATRIX TEST #1 end ==================== */

	/* ==================== MATRIX TEST #2 begin ==================== */
	if (matrixTestEnabled)
	{
		outerBegin = clock();
		for (unsigned int i = 0; i < outerLoops; ++i)
		{
			for (unsigned int j = 0; j < innerLoops; ++j)
			{
				Matrix4D result = identityMatrix1 * identityMatrix2;
				ASSERT(result == identityMatrix1 * identityMatrix2);
			}
		}
		outerEnd = clock();
		elapsedSecs = static_cast<double>((outerEnd - outerBegin)) / (CLOCKS_PER_SEC * innerLoops * outerLoops);
		TimeReport("Average time for identity matrices multiplication:\t", elapsedSecs);
	}
	/* ==================== MATRIX TEST #2 end ==================== */


	SortTest();
	//SortTestTime();

	return 0;
}
