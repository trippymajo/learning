#include <iostream>
#include <cstdlib>
using namespace std;
int randomnum, K;

int main(void)
{
	randomnum = rand() % 200 + 100;
	cout << randomnum << "\n";
	int *array = (int*)malloc(randomnum * sizeof(int));
		for (int x = 0; x < randomnum; x++) array[x] = rand() %9 +1;
		array[randomnum - 1] = '\0';
		cout << "Input int:\n";
		cin >> K;
		for (int i = 0; i < randomnum; i++)
		{
			for (int j = i + 1; j < randomnum; j++)
			{
				int elementsum = array[i] + array[j];
				if (K == elementsum)
				{
					cout << array[i] << "and" << array[j];
					free(array);
					return 0;
				}
			}
		}
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
