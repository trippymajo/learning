// HCourseMemReader.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>


using namespace std;

void errCatcher(int returnValue)
{
	if (returnValue == NULL)
	{
		cout << "Something gone wrong. Try the magic again. Err = " << dec << GetLastError() << endl;
		getchar();
		exit(1);
	}
}

BOOL bProcRead(HANDLE hProcess, DWORD dID, uintptr_t dMem, int iType)
{
	switch (iType)
	{
	case 1: //bool
	{
		bool boolRead;
		BOOL rpmReturn = ReadProcessMemory(hProcess, (LPCVOID)dMem, &boolRead, sizeof(bool), NULL);
		errCatcher((int)rpmReturn);
		cout << "boolRead = " << boolRead << endl;
		break;
	}
	case 2: //int
	{
		int intRead;
		BOOL rpmReturn = ReadProcessMemory(hProcess, (LPCVOID)dMem, &intRead, sizeof(int), NULL);
		errCatcher((int)rpmReturn);
		cout << "intRead = " << dec << intRead << endl;
		break;
	}
	case 3: //char
	{
		char charRead[500];
		BOOL rpmReturn = ReadProcessMemory(hProcess, (LPCVOID)dMem, &charRead, 500, NULL);
		errCatcher((int)rpmReturn);
		cout << "charRead = " << charRead << endl;
		break;
	}
	case 4: //string 
	{
		char strRead[500];
		BOOL rpmReturn = ReadProcessMemory(hProcess, (LPCVOID)dMem, &strRead, 500, NULL);
		errCatcher((int)rpmReturn);
		cout << "strRead = " << strRead << endl;
		break;
	}
	case 5: //intpointer(x86)
	{
		int* pointerRead = NULL;
		BOOL rpmReturn = ReadProcessMemory(hProcess, (LPCVOID)dMem, &pointerRead, sizeof(int*), NULL);
		errCatcher((int)rpmReturn);
		cout << "pointerRead = " << pointerRead << endl;

		int intRead;
		rpmReturn = ReadProcessMemory(hProcess, (LPCVOID)pointerRead, &intRead, sizeof(int), NULL);
		errCatcher((int)rpmReturn);
		cout << "intRead = " << intRead << endl;
		break;
	}
	case 6: //inpointer(x64)
	{
		int* pointerRead = NULL;
		BOOL rpmReturn = ReadProcessMemory(hProcess, (LPCVOID)dMem, &pointerRead, sizeof(int*), NULL);
		errCatcher((int)rpmReturn);
		cout << "pointerRead = " << pointerRead << endl;

		int intRead;
		rpmReturn = ReadProcessMemory(hProcess, (LPCVOID)pointerRead, &intRead, sizeof(int), NULL);
		errCatcher((int)rpmReturn);
		cout << "intRead = " << intRead << endl;
		break;
	}
	}
	return TRUE;

};

BOOL bProcWrite(HANDLE hProcess, DWORD dID, uintptr_t dMem, int iType)
{
	switch (iType)
	{
	case 1: //bool
	{
		bool bWrite;
		cout << "Input new value [true/false]" << endl << flush;
		do
		{
			cin >> boolalpha >> bWrite;

		} while (bWrite != true && bWrite != false);
		BOOL rpmReturn = WriteProcessMemory(hProcess, (LPVOID)dMem, &bWrite, sizeof(bool), NULL); 
		errCatcher((int)rpmReturn);
		break;
	}
	case 2: //int
	{
		int intWrite;
		cout << "Input new value of int: " << endl << flush;
		cin >> dec >> intWrite;
		BOOL rpmReturn = WriteProcessMemory(hProcess, (LPVOID)dMem, &intWrite, sizeof(int), NULL); 
		errCatcher((int)rpmReturn);
		break;
	}
	case 3: //char
	{
		char charWrite[500];
		cout << "Input new value of char: " << endl;
		cin.ignore();
		cin.getline(charWrite, sizeof(charWrite));
		BOOL rpmReturn = WriteProcessMemory(hProcess, (LPVOID)dMem, &charWrite, sizeof(charWrite), NULL); 
		errCatcher((int)rpmReturn);
		break;
	}
	case 4: //string 
	{
		char strWrite[500];
		cout << "Input new value of char: " << endl;
		cin.ignore();
		cin.getline(strWrite, sizeof(strWrite));
		BOOL rpmReturn = WriteProcessMemory(hProcess, (LPVOID)dMem, &strWrite, strlen(strWrite) + 1, NULL);
		errCatcher((int)rpmReturn);
		break;
	}
	/* UNDER CONSTRUCTION
	case 5: //intpointer(x86)
	{
		int* pointerRead = NULL;
		int MemAddress;
		cout << "Input new memory for the pointer: " << endl;
		cin >> hex >> MemAddress;
		pointerRead = &MemAddress;
		BOOL rpmReturn = WriteProcessMemory(hProcess, (LPVOID)dMem, &pointerRead, sizeof(int*), NULL);
		errCatcher((int)rpmReturn);
		break;
	}
	case 6: //inpointer(x64)
	{
		int* pointerRead = NULL;
		BOOL rpmReturn = ReadProcessMemory(hProcess, (LPCVOID)dMem, &pointerRead, sizeof(int*), NULL);
		errCatcher((int)rpmReturn);
		cout << "pointerRead = " << pointerRead << endl;

		int intRead;
		rpmReturn = ReadProcessMemory(hProcess, (LPCVOID)pointerRead, &intRead, sizeof(int), NULL);
		errCatcher((int)rpmReturn);
		cout << "intRead = " << intRead << endl;
		break;
	}
	*/
	}
	return TRUE;

};

int main()
{
	//inputs like ProcessID, Mem.
	DWORD inputID;
	uintptr_t inputMem = 0x0;

	//Read Memory process section
	cout << "Input Process ID" << endl << flush;
	cin >> inputID;
	cout << "Input Mem address" << endl << flush;
	cin >> hex >> inputMem;
	int inputType;

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, inputID); //Get handle to the process
	errCatcher((int)hProcess);

	do
	{
		cout << "Input type of the variable 1-bool 2-int 3-char 4-string(broken) 5-intpointer(x86) 6-intpointer(x64)" << endl << flush;
		cin >> dec >> inputType;
	} while (inputType < 0 || inputType > 6);
	bProcRead(hProcess, inputID, inputMem, inputType);

	//Write Memory process section
	char inputWriteQ;
	do
	{
		cout << "Would you like to write memory of this value [Y/N]?"<< endl << flush;
		cin >> inputWriteQ;
		inputWriteQ = toupper(inputWriteQ);
	} while ((inputWriteQ != 'Y') && (inputWriteQ != 'N'));
	if (toupper(inputWriteQ) == 'Y')
	{
		if (inputType < 5)
			bProcWrite(hProcess, inputID, inputMem, inputType); //cos having some difficulties with writtening new address (Under construction section)
		else
			cout << "Having some difficulties with writtening new address (Under construction section)" << endl << flush;
	}
		
	CloseHandle(hProcess);
	cout << "Press ENTER to quit.";
	system("pause > nul");
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
