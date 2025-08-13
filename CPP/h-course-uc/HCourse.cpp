// HCourse.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>

using namespace std;

int main()
{
	int varInt = 123456;
	string varString = "DefaultString";
	char arrChar[128] = "Long char array right there ->";
	bool end = false;

	//pointers
	int* ptr2int = &varInt;
	int* ptr2ptr = ptr2int;
	int* ptr2ptr2ptr = ptr2ptr;

	do {
		cout << "Process ID:  " << dec << GetCurrentProcessId() << endl << endl;
		cout << "varInt  " << hex << (uintptr_t)&varInt << " = " << dec << varInt << endl;
		cout << "varString  " << hex << (uintptr_t)&varString << " = " << varString << endl;
		cout << "arrChar  " << hex << (uintptr_t)&arrChar << " = " << arrChar << endl;
		cout << "bool end  " << hex << (uintptr_t)&end << " = " << end << endl << endl;
		cout << "ptr2int  " << hex << (uintptr_t)&ptr2int << " = " << ptr2int << endl;
		cout << "ptr2ptr  " << hex << (uintptr_t)&ptr2ptr << " = " << ptr2ptr << endl;
		cout << "ptr2ptr2ptr  " << hex << (uintptr_t)&ptr2ptr2ptr << " = " << ptr2ptr2ptr << endl << endl;
		cout << "Press Enter to Print again.  " << endl;
		getchar();
		cout << "-------------------------------------------------------------" << endl;
	} while (true);

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
