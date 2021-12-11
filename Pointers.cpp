#include <iostream>

using namespace std;
class B;
class A {
public:
    int inta = 1;
    int showb();
    B* b;
};

class B {
public:
    int intb = 2;
    void showa();
    A* a;
};

int A::showb()
{
    cout << "\nB class out :" << b->intb << "\n";
};

void B::showa()
{
    cout << "\nA class out :" << a->inta << "\n";
};

int main()
{
    A a;
    B b;

    a.b = &b;
    b.a = &a;
    a.showb();
    b.showa();
    return 1;
}
