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

#include "StatsTest.h"

#include <ctime>
#include <string>
//#include <iostream>
//#include <fstream>

using namespace Math;
using namespace Utility;
using namespace std;

const int NUMBER_OF_MICROSECONDS_IN_MILISECOND = 1000;
const int NUMBER_OF_MILISECONDS_IN_SECOND = 1000;
const int NUMBER_OF_MICROSECONDS_IN_SECOND = 1000000;
const int NUMBER_OF_NANOSECONDS_IN_SECOND = 1000000000;

unsigned int innerLoops = 10000;
unsigned int outerLoops = 10000;
double elapsedTime;

unsigned int testNumber = 0;
bool angleTestEnabled = false;
bool vectorTestEnabled = false;
bool matrixTestEnabled = false;
bool quaternionTestEnabled = false;
bool sortingTestEnabled = false;
bool kdTreeTestEnabled = false;
bool statsTestEnabled = true;
bool otherTestEnabled = true;

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

void TimeReport(const std::string& reportStr, Timing::Timer& timer, Timing::TimeUnit timeUnit, const int NUMBER_OF_ITERATIONS = 1)
{
	timer.Stop();
	Timing::TimeSpan timeSpan = timer.getTimeSpan(timeUnit);
	timeSpan /= NUMBER_OF_ITERATIONS;
	timeSpan.AdjustUnitToValue();
	NOTICE_LOG("%s:\t%.3f %s", reportStr.c_str(), timeSpan.ToString().c_str());
}

void AngleTest()
{
	if (!angleTestEnabled)
	{
		return;
	}

	NOTICE_LOG("Angle test started");
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
	NOTICE_LOG("Angle test finished");
}

void VectorTest()
{
	if (!vectorTestEnabled)
	{
		return;
	}

	NOTICE_LOG("Vector test started");
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

	NOTICE_LOG("Vector test finished");
}

Angle RandomAngle()
{
	const Real MIN_ANGLE = REAL_ZERO;
	const Real MAX_ANGLE = 360.0f;
	Angle angle(MIN_ANGLE + static_cast<Math::Real>(rand()) /  static_cast<Math::Real>(RAND_MAX / (MAX_ANGLE - MIN_ANGLE)), Unit::DEGREE);
	return angle;
}

Matrix4D RandomMatrix(Real min, Real max)
{
	if (min < max)
	{
		Real temp = min;
		min = max;
		max = temp;
	}
	Matrix4D matrix;
	for (int i = 0; i < MATRIX_SIZE; ++i)
	{
		for (int j = 0; j < MATRIX_SIZE; ++j)
		{
			Real value = min + static_cast<Math::Real>(rand()) /  static_cast<Math::Real>(RAND_MAX / (max - min));
			matrix.SetElement(i, j, value);
		}
	}
	return matrix;
}

void MatrixTest()
{
	if (!matrixTestEnabled)
	{
		return;
	}

	Matrix4D identityMatrix1 = Matrix4D::Identity();
	Matrix4D identityMatrix2 = Matrix4D::Identity();

	/* ==================== MATRIX TEST #1 begin ==================== */
	const int NUMBER_OF_IDENTITY_MATRIX_CREATION_ITERATIONS = 1000000;
	Matrix4D matrix1 = Matrix4D::Identity();
	TestReport(matrix1.IsIdentity(), "The function Matrix::IsIdentity() failed.");

	Timing::Timer timer;
	timer.Start();
	for (unsigned int i = 0; i < NUMBER_OF_IDENTITY_MATRIX_CREATION_ITERATIONS; ++i)
	{
		Matrix4D matrix2 = Matrix4D::Identity();
	}
	timer.Stop();
	TimeReport("Average time for identity matrix creation:\t", timer, Timing::MICROSECOND, NUMBER_OF_IDENTITY_MATRIX_CREATION_ITERATIONS);
	/* ==================== MATRIX TEST #1 end ==================== */

	/* ==================== MATRIX TEST #2 begin ==================== */
	const int NUMBER_OF_IDENTITY_MATRIX_MULTIPLICATION_ITERATIONS = 2000000;
	timer.Reset();
	for (unsigned int i = 0; i < NUMBER_OF_IDENTITY_MATRIX_MULTIPLICATION_ITERATIONS; ++i)
	{
		Matrix4D result = identityMatrix1 * identityMatrix2;
		CHECK_CONDITION(result == identityMatrix1 * identityMatrix2, Utility::Error, "Identity matrix multiplication result is incorrect.");
	}
	timer.Stop();
	TimeReport("Average time for identity matrices multiplication:\t", timer, Timing::MICROSECOND, NUMBER_OF_IDENTITY_MATRIX_MULTIPLICATION_ITERATIONS);
	/* ==================== MATRIX TEST #2 end ==================== */

	/* ==================== MATRIX TEST #3 begin ==================== */
	const int NUMBER_OF_RANDOM_MATRIX_MULTIPLICATION_ITERATIONS = 2000000;
	Matrix4D mat1 = RandomMatrix(REAL_ZERO, REAL_ONE);
	Matrix4D mat2 = RandomMatrix(REAL_ZERO, REAL_ONE);
	timer.Reset();
	for (unsigned int i = 0; i < NUMBER_OF_RANDOM_MATRIX_MULTIPLICATION_ITERATIONS; ++i)
	{
		Matrix4D result = mat1 * mat2;
		//CHECK_CONDITION(result == mat1 * mat2, Utility::Error, "Random matrix multiplication result is incorrect.");
	}
	timer.Stop();
	TimeReport("Average time for random matrices multiplication:\t", timer, Timing::MICROSECOND, NUMBER_OF_RANDOM_MATRIX_MULTIPLICATION_ITERATIONS);
	/* ==================== MATRIX TEST #3 end ==================== */

	/* ==================== MATRIX TEST #4 begin ==================== */
	const int NUMBER_OF_ROTATION_EULER_ITERATIONS = 100000;
	timer.Reset();
	for (unsigned int i = 0; i < NUMBER_OF_ROTATION_EULER_ITERATIONS; ++i)
	{
		Matrix4D result = Matrix4D::RotationEuler(RandomAngle(), RandomAngle(), RandomAngle());
	}
	timer.Stop();
	TimeReport("Average time for rotation Euler matrix calculation:\t", timer, Timing::MICROSECOND, NUMBER_OF_ROTATION_EULER_ITERATIONS);
	/* ==================== MATRIX TEST #4 end ==================== */
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

	NOTICE_LOG("Sorting test started");
	const int NUMBER_OF_VECTORS = 200000;
	const Real LOWER_BOUND_X = -20.0f;
	const Real HIGHER_BOUND_X = 20.0f;
	const Real LOWER_BOUND_Y = -20.0f;
	const Real HIGHER_BOUND_Y = 20.0f;	
	Vector2D* initialVectors = new Vector2D[NUMBER_OF_VECTORS];
	Vector2D* vectors = new Vector2D[NUMBER_OF_VECTORS];
	//initialVectors[0].SetX(REAL_ONE); initialVectors[5].SetY(REAL_ONE); // sumOfComponents = 2	sumOfAbsComponents = 2	sumOfSquaredComponents = 2
	//initialVectors[1].SetX(REAL_ONE); initialVectors[6].SetY(2.0f); // sumOfComponents = 3	sumOfAbsComponents = 3	sumOfSquaredComponents = 5
	//initialVectors[2].SetX(REAL_ONE); initialVectors[7].SetY(3.0f); // sumOfComponents = 4	sumOfAbsComponents = 4	sumOfSquaredComponents = 10
	//initialVectors[3].SetX(REAL_ONE); initialVectors[8].SetY(4.0f); // sumOfComponents = 5	sumOfAbsComponents = 5	sumOfSquaredComponents = 17
	//initialVectors[4].SetX(REAL_ONE); initialVectors[9].SetY(5.0f); // sumOfComponents = 6	sumOfAbsComponents = 6	sumOfSquaredComponents = 26
	//initialVectors[5].SetX(REAL_ZERO); initialVectors[0].SetY(REAL_ONE); // sumOfComponents = 1	sumOfAbsComponents = 1	sumOfSquaredComponents = 1
	//initialVectors[6].SetX(REAL_ZERO); initialVectors[1].SetY(2.0f); // sumOfComponents = 2	sumOfAbsComponents = 2	sumOfSquaredComponents = 4
	//initialVectors[7].SetX(REAL_ZERO); initialVectors[2].SetY(3.0f); // sumOfComponents = 3	sumOfAbsComponents = 3	sumOfSquaredComponents = 9
	//initialVectors[8].SetX(REAL_ZERO); initialVectors[3].SetY(4.0f); // sumOfComponents = 4	sumOfAbsComponents = 4	sumOfSquaredComponents = 16
	//initialVectors[9].SetX(REAL_ZERO); initialVectors[4].SetY(5.0f); // sumOfComponents = 5	sumOfAbsComponents = 5	sumOfSquaredComponents = 25
	//initialVectors[10].SetX(-1.0f); initialVectors[4].SetY(2.0f); // sumOfComponents = 1	sumOfAbsComponents = 3	sumOfSquaredComponents = 5
	//initialVectors[11].SetX(-2.0f); initialVectors[4].SetY(5.0f); // sumOfComponents = 3	sumOfAbsComponents = 7	sumOfSquaredComponents = 29
	for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
	{
		initialVectors[i].SetX(LOWER_BOUND_X + static_cast<Math::Real>(rand()) /  static_cast<Math::Real>(RAND_MAX / (HIGHER_BOUND_X - LOWER_BOUND_X)));
		initialVectors[i].SetY(LOWER_BOUND_Y + static_cast<Math::Real>(rand()) /  static_cast<Math::Real>(RAND_MAX / (HIGHER_BOUND_Y - LOWER_BOUND_Y)));
		DEBUG_LOG("initialVectors[%d] = %s", i, initialVectors[i].ToString().c_str());
	}

	const int NUMBER_OF_SORTING_METHODS = 1; /* the number of sorting methods in the Math library we want to check (10 means we want to check all of them) */
	const int chosenSortingMethodIndices[] = { 3 }; // its length must match the value of NUMBER_OF_SORTING_METHODS variable
	Sorting::SortingAlgorithm sortingMethods[] = { Sorting::BUBBLE_SORT, Sorting::INSERTION_SORT, Sorting::SELECTION_SORT,
		Sorting::MERGE_SORT, Sorting::HEAP_SORT, Sorting::QUICK_SORT, Sorting::SHELL_SORT, Sorting::COMB_SORT,
		Sorting::COUNTING_SORT, Sorting::RADIX_SORT, Sorting::BUCKET_SORT };
	std::string sortingMethodsStr[] = { "Bubble sort", "Insertion sort", "Selection sort", "Merge sort",
		"Heap sort", "Quick sort", "Shell sort", "Comb sort", "Counting sort", "Radix sort", "Bucket sort" };

	const int NUMBER_OF_TIME_TESTS_ITERATIONS = 1;
	/**
	 * TODO: Instead of measuring the time needed to perform NUMBER_OF_TIME_TESTS_ITERATION sorts it should be more efficient to count the number of successful sorts within given period of time.
	 * If this time limit is passed then sorting is stopped no matter if it's finished or not.
	 */
	for (int sortingMethodIndex = 0; sortingMethodIndex < NUMBER_OF_SORTING_METHODS; ++sortingMethodIndex)
	{
		Sorting::ISort* sorter = Sorting::ISort::GetSortingObject(sortingMethods[chosenSortingMethodIndices[sortingMethodIndex]]);
		/* ==================== SORTING TEST #1- sorting Vector2D objects by X component ascending begin ==================== */
		bool sortingTestCasePassed = true;
		Sorting::SortingParametersChain sortingParameters(Sorting::COMPONENT_X, Sorting::ASCENDING);
		sortingParameters.AddChainLink(new Sorting::SortingParametersChain(Sorting::COMPONENT_Y, Sorting::DESCENDING));
		Timing::Timer timer;
		timer.Start();
		for (int k = 0; k < NUMBER_OF_TIME_TESTS_ITERATIONS; ++k)
		{
			for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
			{
				vectors[i] = initialVectors[i];
			}
			sorter->Sort(vectors, NUMBER_OF_VECTORS, sortingParameters);
		}
		timer.Stop();
		TimeReport("Average time for ASCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by X component", timer, Timing::SECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			bool firstCriterionEqual = AlmostEqual(vectors[i + 1].GetX(), vectors[i].GetX());
			if ((!firstCriterionEqual && vectors[i + 1].GetX() < vectors[i].GetX()) ||
				(firstCriterionEqual && (vectors[i + 1].GetY() > vectors[i].GetY())))
			{
				ERROR_LOG("%s in ASCENDING order by X component failed. Vectors[%d] (%s) (sorting key value = %.4f) should precede Vectors[%d] (%s) (sorting key value = %.4f)",
					sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str(), i + 1, vectors[i + 1].ToString().c_str(), vectors[i + 1].GetX(), i, vectors[i].ToString().c_str(), vectors[i].GetX());
				sortingTestCasePassed = false;
				break;
			}
		}
		DEBUG_LOG("%s results (ASCENDING order by X component):\n", sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str());
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			DEBUG_LOG("Vectors[%d] = %s", i, vectors[i].ToString().c_str());
		}
		TestReport(sortingTestCasePassed, sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " in ASCENDING order by X component failed.");
		/* ==================== SORTING TEST #1- sorting Vector2D objects by X component ascending end ==================== */

		/* ==================== SORTING TEST #2- sorting Vector2D objects by X component descending begin ==================== */
		sortingTestCasePassed = true;
		sortingParameters.SetSortingKey(Sorting::COMPONENT_X);
		sortingParameters.SetSortingDirection(Sorting::DESCENDING);
		timer.Reset();
		for (int k = 0; k < NUMBER_OF_TIME_TESTS_ITERATIONS; ++k)
		{
			for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
			{
				vectors[i] = initialVectors[i];
			}
			sorter->Sort(vectors, NUMBER_OF_VECTORS, sortingParameters);
		}
		timer.Stop();
		TimeReport("Average time for DESCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by X component", timer, Timing::SECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			if ((vectors[i + 1].GetX() > vectors[i].GetX()) || ((AlmostEqual(vectors[i + 1].GetX(), vectors[i].GetX())) && (vectors[i + 1].GetY() > vectors[i].GetY())))
			{
				ERROR_LOG("%s in DESCENDING order by X component failed. Vectors[%d] (%s) (sorting key value = %.4f) should precede Vectors[%d] (%s) (sorting key value = %.4f)",
					sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str(), i + 1, vectors[i + 1].ToString().c_str(), vectors[i + 1].GetX(), i, vectors[i].ToString().c_str(), vectors[i].GetX());
				sortingTestCasePassed = false;
				break;
			}
		}
		DEBUG_LOG("%s results (DESCENDING order by X component):\n", sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str());
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			DEBUG_LOG("Vectors[%d] = %s", i, vectors[i].ToString().c_str());
		}
		TestReport(sortingTestCasePassed, sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " in DESCENDING order by X component failed.");
		/* ==================== SORTING TEST #2- sorting Vector2D objects by X component descending end ==================== */

		/* ==================== SORTING TEST #3- sorting Vector2D objects by Y component ascending begin ==================== */
		sortingTestCasePassed = true;
		sortingParameters.ResetChainLink();
		sortingParameters.SetSortingKey(Sorting::COMPONENT_Y);
		sortingParameters.SetSortingDirection(Sorting::ASCENDING);
		sortingParameters.AddChainLink(new Sorting::SortingParametersChain(Sorting::COMPONENT_X, Sorting::DESCENDING));
		timer.Reset();
		for (int k = 0; k < NUMBER_OF_TIME_TESTS_ITERATIONS; ++k)
		{
			for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
			{
				vectors[i] = initialVectors[i];
			}
			sorter->Sort(vectors, NUMBER_OF_VECTORS, sortingParameters);
		}
		timer.Stop();
		TimeReport("Average time for ASCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by Y component", timer, Timing::SECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			if ((vectors[i + 1].GetY() < vectors[i].GetY()) || ((AlmostEqual(vectors[i + 1].GetY(), vectors[i].GetY())) && (vectors[i + 1].GetX() > vectors[i].GetX())))
			{
				ERROR_LOG("%s in ASCENDING order by Y component failed. Vectors[%d] (%s) (sorting key value = %.4f) should precede Vectors[%d] (%s) (sorting key value = %.4f)",
					sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str(), i + 1, vectors[i + 1].ToString().c_str(), vectors[i + 1].GetY(), i, vectors[i].ToString().c_str(), vectors[i].GetY());
				sortingTestCasePassed = false;
				break;
			}
		}
		DEBUG_LOG("%s results (ASCENDING order by Y component):\n", sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str());
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			DEBUG_LOG("Vectors[%d] = %s", i, vectors[i].ToString().c_str());
		}
		TestReport(sortingTestCasePassed, sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " in ASCENDING order by Y component failed.");
		/* ==================== SORTING TEST #3- sorting Vector2D objects by Y component ascending end ==================== */

		/* ==================== SORTING TEST #4- sorting Vector2D objects by Y component descending begin ==================== */
		sortingTestCasePassed = true;
		sortingParameters.SetSortingKey(Sorting::COMPONENT_Y);
		sortingParameters.SetSortingDirection(Sorting::DESCENDING);
		timer.Reset();
		for (int k = 0; k < NUMBER_OF_TIME_TESTS_ITERATIONS; ++k)
		{
			for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
			{
				vectors[i] = initialVectors[i];
			}
			sorter->Sort(vectors, NUMBER_OF_VECTORS, sortingParameters);
		}
		timer.Stop();
		TimeReport("Average time for DESCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by Y component", timer, Timing::SECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			if ((vectors[i + 1].GetY() > vectors[i].GetY()) || ((AlmostEqual(vectors[i + 1].GetY(), vectors[i].GetY())) && (vectors[i + 1].GetX() > vectors[i].GetX())))
			{
				ERROR_LOG("%s in DESCENDING order by Y component failed. Vectors[%d] (%s) (sorting key value = %.4f) should precede Vectors[%d] (%s) (sorting key value = %.4f)",
					sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str(), i + 1, vectors[i + 1].ToString().c_str(), vectors[i + 1].GetY(), i, vectors[i].ToString().c_str(), vectors[i].GetY());
				sortingTestCasePassed = false;
				break;
			}
		}
		DEBUG_LOG("%s results (DESCENDING order by Y component):\n", sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str());
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			DEBUG_LOG("Vectors[%d] = %s", i, vectors[i].ToString().c_str());
		}
		TestReport(sortingTestCasePassed, sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " in DESCENDING order by Y component failed.");
		/* ==================== SORTING TEST #4- sorting Vector2D objects by Y component descending end ==================== */

		/* ==================== SORTING TEST #5- sorting Vector2D objects by sum of components ascending begin ==================== */
		sortingTestCasePassed = true;
		sortingParameters.SetSortingKey(Sorting::SUM_OF_COMPONENTS);
		sortingParameters.SetSortingDirection(Sorting::ASCENDING);
		timer.Reset();
		for (int k = 0; k < NUMBER_OF_TIME_TESTS_ITERATIONS; ++k)
		{
			for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
			{
				vectors[i] = initialVectors[i];
			}
			sorter->Sort(vectors, NUMBER_OF_VECTORS, sortingParameters);
		}
		timer.Stop();
		TimeReport("Average time for ASCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by sum of components", timer, Timing::SECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			Math::Real sumOfComponents1 = vectors[i].SumOfComponents();
			Math::Real sumOfComponents2 = vectors[i + 1].SumOfComponents();
			bool firstCriterionEqual = AlmostEqual(sumOfComponents2, sumOfComponents1);
			if ((!firstCriterionEqual && sumOfComponents2 < sumOfComponents1) ||
				(firstCriterionEqual && (vectors[i + 1].GetX() > vectors[i].GetX())))
			{
				ERROR_LOG("%s in ASCENDING order by sum of components failed. Vectors[%d] (%s) (sorting key value = %.5f) should precede Vectors[%d] (%s) (sorting key value = %.5f)",
					sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str(), i + 1, vectors[i + 1].ToString().c_str(), sumOfComponents2, i, vectors[i].ToString().c_str(), sumOfComponents1);
				sortingTestCasePassed = false;
				break;
			}
		}
		DEBUG_LOG("%s results (ASCENDING order by sum of components):\n", sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str());
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			DEBUG_LOG("Vectors[%d] = %s", i, vectors[i].ToString().c_str());
		}
		TestReport(sortingTestCasePassed, sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " in ASCENDING order by sum of components failed.");
		/* ==================== SORTING TEST #5- sorting Vector2D objects by sum of components ascending end ==================== */

		/* ==================== SORTING TEST #6- sorting Vector2D objects by sum of components descending begin ==================== */
		sortingTestCasePassed = true;
		sortingParameters.SetSortingKey(Sorting::SUM_OF_COMPONENTS);
		sortingParameters.SetSortingDirection(Sorting::DESCENDING);
		timer.Reset();
		for (int k = 0; k < NUMBER_OF_TIME_TESTS_ITERATIONS; ++k)
		{
			for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
			{
				vectors[i] = initialVectors[i];
			}
			sorter->Sort(vectors, NUMBER_OF_VECTORS, sortingParameters);
		}
		timer.Stop();
		TimeReport("Average time for DESCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by sum of components", timer, Timing::SECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			Math::Real sumOfComponents1 = vectors[i].SumOfComponents();
			Math::Real sumOfComponents2 = vectors[i + 1].SumOfComponents();
			bool firstCriterionEqual = AlmostEqual(sumOfComponents2, sumOfComponents1);
			if ((!firstCriterionEqual && sumOfComponents2 > sumOfComponents1) ||
				(firstCriterionEqual && (vectors[i + 1].GetX() > vectors[i].GetX())))
			{
				ERROR_LOG("%s in DESCENDING order by sum of components failed. Vectors[%d] (%s) (sorting key value = %.4f) should precede Vectors[%d] (%s) (sorting key value = %.4f)",
					sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str(), i + 1, vectors[i + 1].ToString().c_str(), sumOfComponents2, i, vectors[i].ToString().c_str(), sumOfComponents1);
				sortingTestCasePassed = false;
				break;
			}
		}
		DEBUG_LOG("%s results (DESCENDING order by sum of components):\n", sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str());
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			DEBUG_LOG("Vectors[%d] = %s", i, vectors[i].ToString().c_str());
		}
		TestReport(sortingTestCasePassed, sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " in DESCENDING order by sum of components failed.");
		/* ==================== SORTING TEST #6- sorting Vector2D objects by sum of components descending end ==================== */

		/* ==================== SORTING TEST #7- sorting Vector2D objects by sum of absolute components ascending begin ==================== */
		sortingTestCasePassed = true;
		sortingParameters.SetSortingKey(Sorting::SUM_OF_ABSOLUTE_COMPONENTS);
		sortingParameters.SetSortingDirection(Sorting::ASCENDING);
		timer.Reset();
		for (int k = 0; k < NUMBER_OF_TIME_TESTS_ITERATIONS; ++k)
		{
			for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
			{
				vectors[i] = initialVectors[i];
			}
			sorter->Sort(vectors, NUMBER_OF_VECTORS, sortingParameters);
		}
		timer.Stop();
		TimeReport("Average time for ASCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by sum of absolute components", timer, Timing::SECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			Math::Real sumOfAbsoluteComponents1 = vectors[i].SumOfAbsoluteComponents();
			Math::Real sumOfAbsoluteComponents2 = vectors[i + 1].SumOfAbsoluteComponents();
			bool firstCriterionEqual = AlmostEqual(sumOfAbsoluteComponents2, sumOfAbsoluteComponents1);
			if ((!firstCriterionEqual && sumOfAbsoluteComponents2 < sumOfAbsoluteComponents1) ||
				(firstCriterionEqual && (vectors[i + 1].GetX() > vectors[i].GetX())))
			{
				ERROR_LOG("%s in ASCENDING order by sum of absolute components failed. Vectors[%d] (%s) (sorting key value = %.4f) should precede Vectors[%d] (%s) (sorting key value = %.4f)",
					sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str(), i + 1, vectors[i + 1].ToString().c_str(), sumOfAbsoluteComponents2, i, vectors[i].ToString().c_str(), sumOfAbsoluteComponents1);
				sortingTestCasePassed = false;
				break;
			}
		}
		DEBUG_LOG("%s results (ASCENDING order by sum of absolute components):\n", sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str());
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			DEBUG_LOG("Vectors[%d] = %s", i, vectors[i].ToString().c_str());
		}
		TestReport(sortingTestCasePassed, sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " in ASCENDING order by sum of absolute components failed.");
		/* ==================== SORTING TEST #7- sorting Vector2D objects by sum of absolute components ascending end ==================== */

		/* ==================== SORTING TEST #8- sorting Vector2D objects by sum of absolute components descending begin ==================== */
		sortingTestCasePassed = true;
		sortingParameters.SetSortingKey(Sorting::SUM_OF_ABSOLUTE_COMPONENTS);
		sortingParameters.SetSortingDirection(Sorting::DESCENDING);
		timer.Reset();
		for (int k = 0; k < NUMBER_OF_TIME_TESTS_ITERATIONS; ++k)
		{
			for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
			{
				vectors[i] = initialVectors[i];
			}
			sorter->Sort(vectors, NUMBER_OF_VECTORS, sortingParameters);
		}
		timer.Stop();
		TimeReport("Average time for DESCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by sum of absolute components", timer, Timing::SECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			Math::Real sumOfAbsoluteComponents1 = vectors[i].SumOfAbsoluteComponents();
			Math::Real sumOfAbsoluteComponents2 = vectors[i + 1].SumOfAbsoluteComponents();
			bool firstCriterionEqual = AlmostEqual(sumOfAbsoluteComponents2, sumOfAbsoluteComponents1);
			if ((!firstCriterionEqual && sumOfAbsoluteComponents2 > sumOfAbsoluteComponents1) ||
				(firstCriterionEqual && (vectors[i + 1].GetX() > vectors[i].GetX())))
			{
				ERROR_LOG("%s in DESCENDING order by sum of absolute components failed. Vectors[%d] (%s) (sorting key value = %.4f) should precede Vectors[%d] (%s) (sorting key value = %.4f)",
					sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str(), i + 1, vectors[i + 1].ToString().c_str(), sumOfAbsoluteComponents2, i, vectors[i].ToString().c_str(), sumOfAbsoluteComponents1);
				sortingTestCasePassed = false;
				break;
			}
		}
		DEBUG_LOG("%s results (DESCENDING order by sum of absolute components):\n", sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str());
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			DEBUG_LOG("Vectors[%d] = %s", i, vectors[i].ToString().c_str());
		}
		TestReport(sortingTestCasePassed, sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " in DESCENDING order by sum of absolute components failed.");
		/* ==================== SORTING TEST #8- sorting Vector2D objects by sum of absolute components descending end ==================== */

		/* ==================== SORTING TEST #9- sorting Vector2D objects by sum of squared components ascending begin ==================== */
		sortingTestCasePassed = true;
		sortingParameters.SetSortingKey(Sorting::SUM_OF_SQUARED_COMPONENTS);
		sortingParameters.SetSortingDirection(Sorting::ASCENDING);
		timer.Reset();
		for (int k = 0; k < NUMBER_OF_TIME_TESTS_ITERATIONS; ++k)
		{
			for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
			{
				vectors[i] = initialVectors[i];
			}
			sorter->Sort(vectors, NUMBER_OF_VECTORS, sortingParameters);
		}
		timer.Stop();
		TimeReport("Average time for ASCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by sum of squared components", timer, Timing::SECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			Math::Real lengthSquared1 = vectors[i].LengthSquared();
			Math::Real lengthSquared2 = vectors[i + 1].LengthSquared();
			bool firstCriterionEqual = AlmostEqual(lengthSquared2, lengthSquared1);
			if ((!firstCriterionEqual && lengthSquared2 < lengthSquared1) ||
				(firstCriterionEqual && (vectors[i + 1].GetX() > vectors[i].GetX())))
			{
				ERROR_LOG("%s in ASCENDING order by sum of squared components failed. Vectors[%d] (%s) (sorting key value = %.4f) should precede Vectors[%d] (%s) (sorting key value = %.4f)",
					sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str(), i + 1, vectors[i + 1].ToString().c_str(), lengthSquared2, i, vectors[i].ToString().c_str(), lengthSquared1);
				sortingTestCasePassed = false;
				break;
			}
		}
		DEBUG_LOG("%s results (ASCENDING order by sum of squared components):\n", sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str());
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			DEBUG_LOG("Vectors[%d] = %s", i, vectors[i].ToString().c_str());
		}
		TestReport(sortingTestCasePassed, sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " in ASCENDING order by sum of squared components failed.");
		/* ==================== SORTING TEST #9- sorting Vector2D objects by sum of squared components ascending end ==================== */

		/* ==================== SORTING TEST #10- sorting Vector2D objects by sum of squared components descending begin ==================== */
		sortingTestCasePassed = true;
		sortingParameters.SetSortingKey(Sorting::SUM_OF_SQUARED_COMPONENTS);
		sortingParameters.SetSortingDirection(Sorting::DESCENDING);
		timer.Reset();
		for (int k = 0; k < NUMBER_OF_TIME_TESTS_ITERATIONS; ++k)
		{
			for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
			{
				vectors[i] = initialVectors[i];
			}
			sorter->Sort(vectors, NUMBER_OF_VECTORS, sortingParameters);
		}
		timer.Stop();
		TimeReport("Average time for DESCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by sum of squared components", timer, Timing::SECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			Math::Real lengthSquared1 = vectors[i].LengthSquared();
			Math::Real lengthSquared2 = vectors[i + 1].LengthSquared();
			bool firstCriterionEqual = AlmostEqual(lengthSquared2, lengthSquared1);
			if ((!firstCriterionEqual && lengthSquared2 > lengthSquared1) ||
				(firstCriterionEqual && (vectors[i + 1].GetX() > vectors[i].GetX())))
			{
				ERROR_LOG("%s in DESCENDING order by sum of squared components failed. Vectors[%d] (%s) (sorting key value = %.4f) should precede Vectors[%d] (%s) (sorting key value = %.4f)",
					sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str(), i + 1, vectors[i + 1].ToString().c_str(), lengthSquared2, i, vectors[i].ToString().c_str(), lengthSquared1);
				sortingTestCasePassed = false;
				break;
			}
		}
		DEBUG_LOG("%s results (DESCENDING order by sum of squared components):\n", sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str());
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			DEBUG_LOG("Vectors[%d] = %s", i, vectors[i].ToString().c_str());
		}
		TestReport(sortingTestCasePassed, sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " in DESCENDING order by sum of squared components failed.");
		/* ==================== SORTING TEST #10- sorting Vector2D objects by sum of squared components descending end ==================== */
		SAFE_DELETE(sorter);
	}

	SAFE_DELETE_JUST_TABLE(initialVectors);
	SAFE_DELETE_JUST_TABLE(vectors);
	NOTICE_LOG("Sorting test finished");
}

void KDTreeTest()
{
	if (!kdTreeTestEnabled)
	{
		return;
	}

	NOTICE_LOG("K-d tree test started");
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
	//	DEBUG_LOG("positions[%d] = %s", i, positions[i].ToString().c_str());
	//}

	KDTree* kdTree = new KDTree(positions, NUMBER_OF_POSITIONS, NUMBER_OF_INTERPOLATED_NEAREST_POINTS);

	//DEBUG_LOG("K-d tree structure:\n%s", kdTree->ToString().c_str());

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
		//INFO_LOG("The nearest point for search point (%s) is (%s). The minimum distance equals %.2f and returned height is %.2f",
		//	searchPositions[i].ToString().c_str(), minDistancePositions[0].ToString().c_str(), minDistances[0], height);
		INFO_LOG("The final result for position (%s) in k-d tree search is %.3f", searchPositions[i].ToString().c_str(), height);
	}

	SAFE_DELETE(kdTree);
	SAFE_DELETE_JUST_TABLE(searchPositions);
	SAFE_DELETE_JUST_TABLE(positions);
}

void StatsTest()
{
	Timing::Timer timer;
	timer.Start();

	const int STATS_TEST_1_METHOD_1_INVOCATIONS_COUNT = 5;
	const int STATS_TEST_1_METHOD_2_INVOCATIONS_COUNT = 150;
	const int STATS_TEST_1_METHOD_3_INVOCATIONS_COUNT = 4;
	const int STATS_TEST_2_METHOD_1_INVOCATIONS_COUNT = 2;
	const int STATS_TEST_2_METHOD_2_INVOCATIONS_COUNT = 1000;
	const int STATS_TEST_2_METHOD_3_INVOCATIONS_COUNT = 8;

	MathTest::StatsTest1 statsTest1;
	for (int i = 0; i < STATS_TEST_1_METHOD_1_INVOCATIONS_COUNT; ++i)
	{
		float floatValue1 = statsTest1.Method1();
	}
	for (int i = 0; i < STATS_TEST_1_METHOD_2_INVOCATIONS_COUNT; ++i)
	{
		float floatValue2 = statsTest1.Method2();
	}
	for (int i = 0; i < STATS_TEST_1_METHOD_3_INVOCATIONS_COUNT; ++i)
	{
		float floatValue3 = statsTest1.Method3();
	}
	
	MathTest::StatsTest2 statsTest2;
	for (int i = 0; i < STATS_TEST_2_METHOD_1_INVOCATIONS_COUNT; ++i)
	{
		float floatValue1 = statsTest2.Method1();
	}
	for (int i = 0; i < STATS_TEST_2_METHOD_2_INVOCATIONS_COUNT; ++i)
	{
		float floatValue2 = statsTest2.Method2();
	}
	for (int i = 0; i < STATS_TEST_2_METHOD_3_INVOCATIONS_COUNT; ++i)
	{
		float floatValue3 = statsTest2.Method3();
	}

	timer.Stop();

	//double anotherTotalElapsedTime = CalculateElapsedTime(outerBegin, outerEnd, SECONDS);
	//std::cout << "Outer end = " << outerEnd << "[ms]. OuterBegin = " << outerBegin << "[ms]." << std::endl;
	//std::cout << "Total elapsed time = " << totalElapsedTime << "[s]. Another total elapsed time = " << anotherTotalElapsedTime << "[s]." << std::endl;
	STATS_STORAGE.PrintReport(timer.GetTimeSpan(Timing::SECOND).GetValue(););
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

	AngleTest();
	VectorTest();
	MatrixTest();
	SortTest();
	//SortTestTime();

	KDTreeTest();

	StatsTest();

	ILogger::GetLogger().ResetConsoleColor();
	std::cout << "Bye!" << std::endl;

	return 0;
}
