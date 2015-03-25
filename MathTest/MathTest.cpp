#include "Math\Angle.h"
#include "Math\FloatingPoint.h"
#include "Math\Matrix.h"
#include "Math\Quaternion.h"
#include "Math\Vector.h"
#include "Math\ISort.h"
#include "Math\KDTree.h"

#include "Utility\ICommand.h"
#include "Utility\ILogger.h"
#include "Utility\Time.h"

#include <ctime>
#include <string>
//#include <iostream>
//#include <fstream>

using namespace Math;
using namespace Utility;
using namespace std;

unsigned int innerLoops = 200;
unsigned int outerLoops = 200;
clock_t outerBegin, outerEnd;
double elapsedSecs;

unsigned int testNumber = 0;
bool angleTestEnabled = false;
bool vectorTestEnabled = false;
bool matrixTestEnabled = false;
bool quaternionTestEnabled = false;
bool sortingTestEnabled = false;
bool kdTreeTestEnabled = true;
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
	Angle angle2(90.0, Unit::DEGREE);
	Angle angle3(M_PI / 2.0f, Unit::RADIAN); // angle1 == angle2 == angle3
	Angle angle4(M_PI / 2.0f, Unit::DEGREE); // angle4 is different than angle1, angle2, angle3
	TestReport(angle1 == angle2, "The comparison operators of the Angle class are incorrect. angle1 == angle2");
	TestReport(angle1 == angle3, "The comparison operators of the Angle class are incorrect. angle1 == angle3");
	TestReport(angle2 == angle3, "The comparison operators of the Angle class are incorrect. angle2 == angle3");
	TestReport(angle1 != angle4, "The comparison operators of the Angle class are incorrect. angle1 != angle4");
	TestReport(angle2 != angle4, "The comparison operators of the Angle class are incorrect. angle2 != angle4");
	TestReport(angle3 != angle4, "The comparison operators of the Angle class are incorrect. angle3 != angle4");
	Angle angle5(45.0f);
	Angle angle6(M_PI / 4.0f, Unit::RADIAN); // angle5 is equal to angle6
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

	ILogger::GetLogger().AddFile("sortingOutput.txt");
	//ofstream sortingOutputFile;
	//sortingOutputFile.open("sortingOutput.txt");

	LOG (Notice, LOGPLACE, "Sorting test started");
	const int NUMBER_OF_VECTORS = 1000000;
	const Real LOWER_BOUND_X = -20.0f;
	const Real HIGHER_BOUND_X = 20.0f;
	const Real LOWER_BOUND_Y = -20.0f;
	const Real HIGHER_BOUND_Y = 20.0f;	
	Vector2D* initialVectors = new Vector2D[NUMBER_OF_VECTORS];
	Vector2D* vectors = new Vector2D[NUMBER_OF_VECTORS];
	//initialVectors[0].SetX(0.0f); initialVectors[0].SetY(1.0f);
	//initialVectors[1].SetX(0.0f); initialVectors[1].SetY(2.0f);
	//initialVectors[2].SetX(0.0f); initialVectors[2].SetY(3.0f);
	//initialVectors[3].SetX(0.0f); initialVectors[3].SetY(4.0f);
	//initialVectors[4].SetX(0.0f); initialVectors[4].SetY(5.0f);
	//initialVectors[5].SetX(1.0f); initialVectors[5].SetY(1.0f);
	//initialVectors[6].SetX(1.0f); initialVectors[6].SetY(2.0f);
	//initialVectors[7].SetX(1.0f); initialVectors[7].SetY(3.0f);
	//initialVectors[8].SetX(1.0f); initialVectors[8].SetY(4.0f);
	//initialVectors[9].SetX(1.0f); initialVectors[9].SetY(5.0f);
	for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
	{
		initialVectors[i].SetX(LOWER_BOUND_X + static_cast<Math::Real>(rand()) /  static_cast<Math::Real>(RAND_MAX / (HIGHER_BOUND_X - LOWER_BOUND_X)));
		initialVectors[i].SetY(LOWER_BOUND_Y + static_cast<Math::Real>(rand()) /  static_cast<Math::Real>(RAND_MAX / (HIGHER_BOUND_Y - LOWER_BOUND_Y)));
		LOG(Debug, LOGPLACE, "initialVectors[%d] = %s", i, initialVectors[i].ToString().c_str());
	}

	const int NUMBER_OF_SORTING_METHODS = 6; /* the number of sorting methods in the Math library we want to check (10 means we want to check all of them) */
	const int chosenSortingMethodIndices[] = { 3, 4, 5, 6, 7, 10 }; // its length must match the value of NUMBER_OF_SORTING_METHODS variable
	Sorting::SortingAlgorithm sortingMethods[] = { Sorting::BUBBLE_SORT, Sorting::INSERTION_SORT, Sorting::SELECTION_SORT,
		Sorting::MERGE_SORT, Sorting::HEAP_SORT, Sorting::QUICK_SORT, Sorting::SHELL_SORT, Sorting::COMB_SORT,
		Sorting::COUNTING_SORT, Sorting::RADIX_SORT, Sorting::BUCKET_SORT };
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
		SortingParametersChain sortingParameters(COMPONENT_X, ASCENDING);
		sortingParameters.AddChainLink(new SortingParametersChain(COMPONENT_Y, DESCENDING));
		outerBegin = clock();
		for (int k = 0; k < NUMBER_OF_TIME_TESTS_ITERATION; ++k)
		{
			for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
			{
				vectors[i] = initialVectors[i];
			}
			sorter->Sort(vectors, NUMBER_OF_VECTORS, sortingParameters);
		}
		outerEnd = clock();
		elapsedSecs = static_cast<double>((outerEnd - outerBegin)) / (NUMBER_OF_MICROSECONDS_IN_MILISECOND * CLOCKS_PER_SEC * NUMBER_OF_TIME_TESTS_ITERATION);
		TimeReport("Average time for ASCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by X component", elapsedSecs, "[ms]");

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			if ((vectors[i + 1].GetX() < vectors[i].GetX()) || ((AlmostEqual(vectors[i + 1].GetX(), vectors[i].GetX())) && (vectors[i + 1].GetY() > vectors[i].GetY())))
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
		sortingParameters.SetSortingKey(COMPONENT_X);
		sortingParameters.SetSortingDirection(DESCENDING);
		outerBegin = clock();
		for (int k = 0; k < NUMBER_OF_TIME_TESTS_ITERATION; ++k)
		{
			for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
			{
				vectors[i] = initialVectors[i];
			}
			sorter->Sort(vectors, NUMBER_OF_VECTORS, sortingParameters);
		}
		outerEnd = clock();
		elapsedSecs = static_cast<double>((outerEnd - outerBegin)) / (NUMBER_OF_MICROSECONDS_IN_MILISECOND * CLOCKS_PER_SEC * NUMBER_OF_TIME_TESTS_ITERATION);
		TimeReport("Average time for DESCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by X component", elapsedSecs, "[ms]");

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			if ((vectors[i + 1].GetX() > vectors[i].GetX()) || ((AlmostEqual(vectors[i + 1].GetX(), vectors[i].GetX())) && (vectors[i + 1].GetY() > vectors[i].GetY())))
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
		sortingParameters.ResetChainLink();
		sortingParameters.SetSortingKey(COMPONENT_Y);
		sortingParameters.SetSortingDirection(ASCENDING);
		sortingParameters.AddChainLink(new SortingParametersChain(COMPONENT_X, DESCENDING));
		outerBegin = clock();
		for (int k = 0; k < NUMBER_OF_TIME_TESTS_ITERATION; ++k)
		{
			for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
			{
				vectors[i] = initialVectors[i];
			}
			sorter->Sort(vectors, NUMBER_OF_VECTORS, sortingParameters);
		}
		outerEnd = clock();
		elapsedSecs = static_cast<double>((outerEnd - outerBegin)) / (NUMBER_OF_MICROSECONDS_IN_MILISECOND * CLOCKS_PER_SEC * NUMBER_OF_TIME_TESTS_ITERATION);
		TimeReport("Average time for ASCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by Y component", elapsedSecs, "[ms]");

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			if ((vectors[i + 1].GetY() < vectors[i].GetY()) || ((AlmostEqual(vectors[i + 1].GetY(), vectors[i].GetY())) && (vectors[i + 1].GetX() > vectors[i].GetX())))
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
		sortingParameters.SetSortingKey(COMPONENT_Y);
		sortingParameters.SetSortingDirection(DESCENDING);
		outerBegin = clock();
		for (int k = 0; k < NUMBER_OF_TIME_TESTS_ITERATION; ++k)
		{
			for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
			{
				vectors[i] = initialVectors[i];
			}
			sorter->Sort(vectors, NUMBER_OF_VECTORS, sortingParameters);
		}
		outerEnd = clock();
		elapsedSecs = static_cast<double>((outerEnd - outerBegin)) / (NUMBER_OF_MICROSECONDS_IN_MILISECOND * CLOCKS_PER_SEC * NUMBER_OF_TIME_TESTS_ITERATION);
		TimeReport("Average time for DESCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by Y component", elapsedSecs, "[ms]");

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			if ((vectors[i + 1].GetY() > vectors[i].GetY()) || ((AlmostEqual(vectors[i + 1].GetY(), vectors[i].GetY())) && (vectors[i + 1].GetX() > vectors[i].GetX())))
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

		/* ==================== SORTING TEST #5- sorting Vector2D objects by sum of components ascending begin ==================== */
		sortingTestCasePassed = true;
		sortingParameters.SetSortingKey(SUM_OF_COMPONENTS);
		sortingParameters.SetSortingDirection(ASCENDING);
		outerBegin = clock();
		for (int k = 0; k < NUMBER_OF_TIME_TESTS_ITERATION; ++k)
		{
			for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
			{
				vectors[i] = initialVectors[i];
			}
			sorter->Sort(vectors, NUMBER_OF_VECTORS, sortingParameters);
		}
		outerEnd = clock();
		elapsedSecs = static_cast<double>((outerEnd - outerBegin)) / (NUMBER_OF_MICROSECONDS_IN_MILISECOND * CLOCKS_PER_SEC * NUMBER_OF_TIME_TESTS_ITERATION);
		TimeReport("Average time for ASCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by sum of components", elapsedSecs, "[ms]");

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			Math::Real sumOfComponents1 = vectors[i].SumOfComponents();
			Math::Real sumOfComponents2 = vectors[i + 1].SumOfComponents();
			if ((sumOfComponents2 < sumOfComponents1) || ((AlmostEqual(sumOfComponents2, sumOfComponents1)) && (vectors[i + 1].GetX() > vectors[i].GetX())))
			{
				//bool almostEqual = ((AlmostEqual(sumOfComponents2, sumOfComponents1)) && (vectors[i + 1].GetX() > vectors[i].GetX()));
				LOG(Error, LOGPLACE, "%s in ASCENDING order by sum of components failed. Vectors[%d] (%s) should precede Vectors[%d] (%s)",
					sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str(), i + 1, vectors[i + 1].ToString().c_str(), i, vectors[i].ToString().c_str());
				sortingTestCasePassed = false;
				break;
			}
		}
		LOG(Debug, LOGPLACE, "%s results (ASCENDING order by sum of components):\n", sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str());
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			LOG(Debug, LOGPLACE, "Vectors[%d] = %s", i, vectors[i].ToString().c_str());
		}
		TestReport(sortingTestCasePassed, sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " in ASCENDING order by sum of components failed.");
		/* ==================== SORTING TEST #5- sorting Vector2D objects by sum of components ascending end ==================== */

		/* ==================== SORTING TEST #6- sorting Vector2D objects by sum of components descending begin ==================== */
		sortingTestCasePassed = true;
		sortingParameters.SetSortingKey(SUM_OF_COMPONENTS);
		sortingParameters.SetSortingDirection(DESCENDING);
		outerBegin = clock();
		for (int k = 0; k < NUMBER_OF_TIME_TESTS_ITERATION; ++k)
		{
			for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
			{
				vectors[i] = initialVectors[i];
			}
			sorter->Sort(vectors, NUMBER_OF_VECTORS, sortingParameters);
		}
		outerEnd = clock();
		elapsedSecs = static_cast<double>((outerEnd - outerBegin)) / (NUMBER_OF_MICROSECONDS_IN_MILISECOND * CLOCKS_PER_SEC * NUMBER_OF_TIME_TESTS_ITERATION);
		TimeReport("Average time for DESCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by sum of components", elapsedSecs, "[ms]");

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			Math::Real sumOfComponents1 = vectors[i].SumOfComponents();
			Math::Real sumOfComponents2 = vectors[i + 1].SumOfComponents();
			if ((sumOfComponents2 > sumOfComponents1) || ((AlmostEqual(sumOfComponents2, sumOfComponents1)) && (vectors[i + 1].GetX() > vectors[i].GetX())))
			{
				//bool almostEqual = ((AlmostEqual(sumOfComponents2, sumOfComponents1)) && (vectors[i + 1].GetX() > vectors[i].GetX()));
				LOG(Error, LOGPLACE, "%s in DESCENDING order by sum of components failed. Vectors[%d] (%s) should precede Vectors[%d] (%s)",
					sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str(), i + 1, vectors[i + 1].ToString().c_str(), i, vectors[i].ToString().c_str());
				sortingTestCasePassed = false;
				break;
			}
		}
		LOG(Debug, LOGPLACE, "%s results (DESCENDING order by sum of components):\n", sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str());
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			LOG(Debug, LOGPLACE, "Vectors[%d] = %s", i, vectors[i].ToString().c_str());
		}
		TestReport(sortingTestCasePassed, sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " in DESCENDING order by sum of components failed.");
		/* ==================== SORTING TEST #6- sorting Vector2D objects by sum of components descending end ==================== */

		/* ==================== SORTING TEST #7- sorting Vector2D objects by sum of absolute components ascending begin ==================== */
		sortingTestCasePassed = true;
		sortingParameters.SetSortingKey(SUM_OF_ABSOLUTE_COMPONENTS);
		sortingParameters.SetSortingDirection(ASCENDING);
		outerBegin = clock();
		for (int k = 0; k < NUMBER_OF_TIME_TESTS_ITERATION; ++k)
		{
			for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
			{
				vectors[i] = initialVectors[i];
			}
			sorter->Sort(vectors, NUMBER_OF_VECTORS, sortingParameters);
		}
		outerEnd = clock();
		elapsedSecs = static_cast<double>((outerEnd - outerBegin)) / (NUMBER_OF_MICROSECONDS_IN_MILISECOND * CLOCKS_PER_SEC * NUMBER_OF_TIME_TESTS_ITERATION);
		TimeReport("Average time for ASCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by sum of absolute components", elapsedSecs, "[ms]");

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			Math::Real sumOfAbsoluteComponents1 = vectors[i].SumOfAbsoluteComponents();
			Math::Real sumOfAbsoluteComponents2 = vectors[i + 1].SumOfAbsoluteComponents();
			if ((sumOfAbsoluteComponents2 < sumOfAbsoluteComponents1) || ((AlmostEqual(sumOfAbsoluteComponents2, sumOfAbsoluteComponents1)) && (vectors[i + 1].GetX() > vectors[i].GetX())))
			{
				//bool almostEqual = ((AlmostEqual(sumOfAbsoluteComponents2, sumOfAbsoluteComponents1)) && (vectors[i + 1].GetX() > vectors[i].GetX()));
				LOG(Error, LOGPLACE, "%s in ASCENDING order by sum of absolute components failed. Vectors[%d] (%s) should precede Vectors[%d] (%s)",
					sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str(), i + 1, vectors[i + 1].ToString().c_str(), i, vectors[i].ToString().c_str());
				sortingTestCasePassed = false;
				break;
			}
		}
		LOG(Debug, LOGPLACE, "%s results (ASCENDING order by sum of absolute components):\n", sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str());
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			LOG(Debug, LOGPLACE, "Vectors[%d] = %s", i, vectors[i].ToString().c_str());
		}
		TestReport(sortingTestCasePassed, sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " in ASCENDING order by sum of absolute components failed.");
		/* ==================== SORTING TEST #7- sorting Vector2D objects by sum of absolute components ascending end ==================== */

		/* ==================== SORTING TEST #8- sorting Vector2D objects by sum of absolute components descending begin ==================== */
		sortingTestCasePassed = true;
		sortingParameters.SetSortingKey(SUM_OF_ABSOLUTE_COMPONENTS);
		sortingParameters.SetSortingDirection(DESCENDING);
		outerBegin = clock();
		for (int k = 0; k < NUMBER_OF_TIME_TESTS_ITERATION; ++k)
		{
			for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
			{
				vectors[i] = initialVectors[i];
			}
			sorter->Sort(vectors, NUMBER_OF_VECTORS, sortingParameters);
		}
		outerEnd = clock();
		elapsedSecs = static_cast<double>((outerEnd - outerBegin)) / (NUMBER_OF_MICROSECONDS_IN_MILISECOND * CLOCKS_PER_SEC * NUMBER_OF_TIME_TESTS_ITERATION);
		TimeReport("Average time for DESCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by sum of absolute components", elapsedSecs, "[ms]");

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			Math::Real sumOfAbsoluteComponents1 = vectors[i].SumOfAbsoluteComponents();
			Math::Real sumOfAbsoluteComponents2 = vectors[i + 1].SumOfAbsoluteComponents();
			if ((sumOfAbsoluteComponents2 > sumOfAbsoluteComponents1) || ((AlmostEqual(sumOfAbsoluteComponents2, sumOfAbsoluteComponents1)) && (vectors[i + 1].GetX() > vectors[i].GetX())))
			{
				//bool almostEqual = ((AlmostEqual(sumOfAbsoluteComponents2, sumOfAbsoluteComponents1)) && (vectors[i + 1].GetX() > vectors[i].GetX()));
				LOG(Error, LOGPLACE, "%s in DESCENDING order by sum of absolute components failed. Vectors[%d] (%s) should precede Vectors[%d] (%s)",
					sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str(), i + 1, vectors[i + 1].ToString().c_str(), i, vectors[i].ToString().c_str());
				sortingTestCasePassed = false;
				break;
			}
		}
		LOG(Debug, LOGPLACE, "%s results (DESCENDING order by sum of absolute components):\n", sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str());
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			LOG(Debug, LOGPLACE, "Vectors[%d] = %s", i, vectors[i].ToString().c_str());
		}
		TestReport(sortingTestCasePassed, sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " in DESCENDING order by sum of absolute components failed.");
		/* ==================== SORTING TEST #8- sorting Vector2D objects by sum of absolute components descending end ==================== */

		/* ==================== SORTING TEST #9- sorting Vector2D objects by sum of squared components ascending begin ==================== */
		sortingTestCasePassed = true;
		sortingParameters.SetSortingKey(SUM_OF_SQUARED_COMPONENTS);
		sortingParameters.SetSortingDirection(ASCENDING);
		outerBegin = clock();
		for (int k = 0; k < NUMBER_OF_TIME_TESTS_ITERATION; ++k)
		{
			for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
			{
				vectors[i] = initialVectors[i];
			}
			sorter->Sort(vectors, NUMBER_OF_VECTORS, sortingParameters);
		}
		outerEnd = clock();
		elapsedSecs = static_cast<double>((outerEnd - outerBegin)) / (NUMBER_OF_MICROSECONDS_IN_MILISECOND * CLOCKS_PER_SEC * NUMBER_OF_TIME_TESTS_ITERATION);
		TimeReport("Average time for ASCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by sum of squared components", elapsedSecs, "[ms]");

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			Math::Real lengthSquared1 = vectors[i].LengthSquared();
			Math::Real lengthSquared2 = vectors[i + 1].LengthSquared();
			if ((lengthSquared2 < lengthSquared1) || ((AlmostEqual(lengthSquared2, lengthSquared1)) && (vectors[i + 1].GetX() > vectors[i].GetX())))
			{
				//bool almostEqual = ((AlmostEqual(lengthSquared2, lengthSquared1)) && (vectors[i + 1].GetX() > vectors[i].GetX()));
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
		/* ==================== SORTING TEST #9- sorting Vector2D objects by sum of squared components ascending end ==================== */

		/* ==================== SORTING TEST #10- sorting Vector2D objects by sum of squared components descending begin ==================== */
		sortingTestCasePassed = true;
		sortingParameters.SetSortingKey(SUM_OF_SQUARED_COMPONENTS);
		sortingParameters.SetSortingDirection(DESCENDING);
		outerBegin = clock();
		for (int k = 0; k < NUMBER_OF_TIME_TESTS_ITERATION; ++k)
		{
			for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
			{
				vectors[i] = initialVectors[i];
			}
			sorter->Sort(vectors, NUMBER_OF_VECTORS, sortingParameters);
		}
		outerEnd = clock();
		elapsedSecs = static_cast<double>((outerEnd - outerBegin)) / (NUMBER_OF_MICROSECONDS_IN_MILISECOND * CLOCKS_PER_SEC * NUMBER_OF_TIME_TESTS_ITERATION);
		TimeReport("Average time for DESCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by sum of squared components", elapsedSecs, "[ms]");

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			Math::Real lengthSquared1 = vectors[i].LengthSquared();
			Math::Real lengthSquared2 = vectors[i + 1].LengthSquared();
			if ((lengthSquared2 > lengthSquared1) || ((AlmostEqual(lengthSquared2, lengthSquared1)) && (vectors[i + 1].GetX() > vectors[i].GetX())))
			{
				//bool almostEqual = ((AlmostEqual(lengthSquared2, lengthSquared1)) && (vectors[i + 1].GetX() > vectors[i].GetX()));
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
		/* ==================== SORTING TEST #10- sorting Vector2D objects by sum of squared components descending end ==================== */
		delete sorter;
		sorter = NULL;
	}

	delete [] initialVectors;
	delete [] vectors;
	LOG (Notice, LOGPLACE, "Sorting test finished");
}

void KDTreeTest()
{
	if (!kdTreeTestEnabled)
	{
		return;
	}

	LOG (Notice, LOGPLACE, "K-d tree test started");
	const int NUMBER_OF_POSITIONS = 6;
	const int NUMBER_OF_SEARCH_POSITIONS = 10;
	const int NUMBER_OF_INTERPOLATED_NEAREST_POINTS = 1; // number of nearest points which will be used for calculating the final result
	const Real LOWER_BOUND_X = -20.0f; const Real HIGHER_BOUND_X = 20.0f;
	const Real LOWER_BOUND_Y = -20.0f; const Real HIGHER_BOUND_Y = 20.0f;
	const Real LOWER_BOUND_Z = -20.0f; const Real HIGHER_BOUND_Z = 20.0f;
	Vector3D* positions = new Vector3D[NUMBER_OF_POSITIONS];
	positions[0].SetX(2.0f); positions[0].SetY(3.0f); positions[0].SetZ(2.0f);
	positions[1].SetX(5.0f); positions[1].SetY(4.0f); positions[1].SetZ(4.0f);
	positions[2].SetX(9.0f); positions[2].SetY(6.0f); positions[2].SetZ(-2.0f);
	positions[3].SetX(4.0f); positions[3].SetY(7.0f); positions[3].SetZ(1.0f);
	positions[4].SetX(8.0f); positions[4].SetY(1.0f); positions[4].SetZ(12.0f);
	positions[5].SetX(7.0f); positions[5].SetY(2.0f); positions[5].SetZ(6.0f);
	//for (int i = 0; i < NUMBER_OF_POSITIONS; ++i)
	//{
	//	positions[i].SetX(LOWER_BOUND_X + static_cast<Math::Real>(rand()) /  static_cast<Math::Real>(RAND_MAX / (HIGHER_BOUND_X - LOWER_BOUND_X)));
	//	positions[i].SetY(LOWER_BOUND_Y + static_cast<Math::Real>(rand()) /  static_cast<Math::Real>(RAND_MAX / (HIGHER_BOUND_Y - LOWER_BOUND_Y)));
	//	positions[i].SetZ(LOWER_BOUND_Z + static_cast<Math::Real>(rand()) /  static_cast<Math::Real>(RAND_MAX / (HIGHER_BOUND_Z - LOWER_BOUND_Z)));
	//	LOG(Debug, LOGPLACE, "positions[%d] = %s", i, positions[i].ToString().c_str());
	//}

	KDTree* kdTree = new KDTree(positions, NUMBER_OF_POSITIONS, NUMBER_OF_INTERPOLATED_NEAREST_POINTS);

	//LOG(Utility::Debug, LOGPLACE, "K-d tree structure:\n%s", kdTree->ToString().c_str());

	Math::Vector2D* searchPositions = new Math::Vector2D[NUMBER_OF_SEARCH_POSITIONS];
	searchPositions[0].SetX(2.0f); searchPositions[0].SetY(3.0f);
	searchPositions[1].SetX(5.0f); searchPositions[1].SetY(4.0f);
	searchPositions[2].SetX(9.0f); searchPositions[2].SetY(6.0f);
	searchPositions[3].SetX(4.0f); searchPositions[3].SetY(7.0f);
	searchPositions[4].SetX(8.0f); searchPositions[4].SetY(1.0f);
	searchPositions[5].SetX(7.0f); searchPositions[5].SetY(2.0f);
	searchPositions[6].SetX(6.0f); searchPositions[6].SetY(2.0f);
	searchPositions[7].SetX(7.0f); searchPositions[7].SetY(3.0f);
	searchPositions[8].SetX(8.0f); searchPositions[8].SetY(4.0f);
	searchPositions[9].SetX(9.0f); searchPositions[9].SetY(5.0f);
	for (int i = 0; i < NUMBER_OF_SEARCH_POSITIONS; ++i)
	{
		Real height = kdTree->SearchNearestValue(searchPositions[i]);
		//LOG(Utility::Info, LOGPLACE, "The nearest point for search point (%s) is (%s). The minimum distance equals %.2f and returned height is %.2f",
		//	searchPositions[i].ToString().c_str(), minDistancePositions[0].ToString().c_str(), minDistances[0], height);
		LOG(Utility::Info, LOGPLACE, "The final result for position (%s) in k-d tree search is %.3f", searchPositions[i].ToString().c_str(), height);
	}

	delete kdTree;
	delete [] searchPositions;
	delete [] positions;
}

int main (int argc, char* argv[])
{
	srand((unsigned int)time(NULL));
	ICommand::SetCommand(argc, argv);
	//if (Command.IsPresent("-help"))
	//{
	//	PrintHelp();
	//	system("pause");
	//	return 0;
	//}
	ILogger::GetLogger().Fill(ICommand::GetCommand().Get("-log", ""), Info);

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

	KDTreeTest();

	ILogger::GetLogger().ResetConsoleColor();
	std::cout << "Bye!" << std::endl;

	return 0;
}
