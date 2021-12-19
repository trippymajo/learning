#include <iostream>

using namespace std;
class B;
class A {
public:
    int inta = 1;
    int showb();
    B* ptrA2B;
};

class B {
public:
    int intb = 2;
    void showa();
    A* ptrB2A;
};

class C {
public:
    C();
    ~C();
    A* ptrC2A;
    B* ptrC2B;
    int showab();
};

C::C()
{
    ptrC2A = new A;
    ptrC2B = new B;
    cout << "Pointers for class C created\n";
}

C::~C()
{
    cout << "Destructed\n";
}
int C::showab()
{
    cout << "\nA class & B class out :" << ptrC2A->inta << " & " << ptrC2B->intb << "\n";
};

int A::showb()
{
    cout << "\nB class out :" << ptrA2B->intb << "\n";
};

void B::showa()
{
    cout << "\nA class out :" << ptrB2A->inta << "\n";
};


int main()
{
    A a;
    B b;
    C c;

    a.ptrA2B = &b;
    b.ptrB2A = &a;

    a.showb();
    b.showa();
    c.showab();
    return 1;
}
