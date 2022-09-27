#include <iostream>
using namespace std;

template <class ArrType, int n> void output(ArrType (&arr) [n])
{
    for (int i = 0; i < n; ++i)
    {
        cout << arr[i] << ' ';
    }
}
template <class ArrType, int n> void bublesort(ArrType (&arr) [n])
{
    ArrType temp;
    for (int i = 0; i<n;i++)
        for (int y = n - 1; y >= i; y--)
            if (arr[y - 1] > arr[y]) 
            {
                temp = arr[y - 1];
                arr[y - 1] = arr[y];
                arr[y] = temp;
            }
}

int main()
{
    int iarray[5] = { 55, 25, 13, 66, 31 };
    char charray[4] = { 'c', 'b', 'z', 'a'};
    
    cout << "Before sort:" << endl;
    output(iarray);
    output(charray);

    bublesort(iarray);
    bublesort(charray);

    cout << "\nAfter sort:" << endl;
    output(iarray);
    output(charray);

}

