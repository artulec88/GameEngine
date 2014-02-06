/* MATH begin */
#include "Math\Math.h"
#include "Math\Point.h"
/* MATH end */

using namespace std;
using namespace Math;

int main (int argc, char* argv[])
{
	// TODO: Create testing function
	float value1 = -3.1415f;
	double value2 = 3.1415f;
	cout << "value1 = " << value1 << "; Abs(value1) = " << Absolute(value1) << endl;
	cout << "value2 = " << value2 << "; Abs(value2) = " << Absolute(value2) << endl;
	cout << "ToDeg(value1) = " << ToDeg(value1) << endl;
	cout << "ToDeg(value2) = " << ToDeg(value2) << endl;
	cout << "ToRad(value1) = " << ToRad(value1) << endl;
	cout << "ToRad(value2) = " << ToRad(value2) << endl;

	Point p(4.3, 15.6);
	cout << p.ToString() << endl;

	system("pause");
	return 0;
}