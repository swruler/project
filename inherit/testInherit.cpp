//
// testInherit.cpp
//
// This example tests these ideas.
// - the "using" construct for a derived class to explicitly use a base class function
// - the user defined copy constructor
// - the user defined overloaded assignment operator
// - polymorphic function call parameter
// - global friend function
// - protected member access
// - base class member access
//
#include <iostream>
using namespace std;

// For some reason the compiler does not allow a class to make friends with another class's member function.
class Base;

class Dummy {
public:
	int val;
	Dummy() {val = 0; cout << "Dummy class constructor" << endl;}

	~Dummy() {cout << "Dummy class destructor" << endl;}

    Dummy& operator=(const Dummy& rhs)  // assignment operator overload
    {
    	cout << "Dummy class assignment right hand side value " << rhs.val << endl;
    	cout << "left hand side value before " << val << endl;
    	val = rhs.val+3;
    	cout << "left hand side value after assignment " << val << endl;
        return *this;
    }

	void friendWithBaseClass(Base b);
};

class Base {
protected:
	int val;

public:
	void publicNonVirtualBaseFunc(){cout << "called publicNonVirtualBaseFunc()" << endl;}

	// See which overridden function is called.
    virtual void func(int a){cout << "BASE class func (" << a << ")" << " val=" << val << endl;}
    virtual void func(){cout << "BASE class func()" << endl;}

	// See which base class constructor is needed by derived class constructor.
    Base() {val = 10; cout << "Base class constructor() val=" << val << endl;}  // constructor
    Base(int inVal) {val = inVal; cout << "Base class constructor (" << val << ")" << endl;}  // constructor

    Base(const Base& srcClass) {val = srcClass.val+1; cout << "Base class copy constructor val=" << val << endl;}  // copy constructor

    Base& operator=(const Base& rhs)  // assignment operator overload
    {
    	cout << "Base class assignment right hand side value " << rhs.val << endl;
    	cout << "left hand side value before " << val << endl;
    	val = rhs.val+2;
    	cout << "left hand side value after assignment " << val << endl;
        return *this;
    }

    virtual ~Base() {cout << "Base class destructor for val=" << val << endl;}  // destructor

    friend void friendOfBaseClass(Base b);  // Base makes friends with global function

    friend void Dummy::friendWithBaseClass(Base b);  // Base makes Dummy a friend
};

void Dummy::friendWithBaseClass(Base b) {
	cout <<
	"Base made friends with Dummy, giving Dummy access to Base's protected val=" <<
	b.val <<
	endl;
}

class Derived: public Base
{
protected:
	int val;

public:
	// See which overridden function is called. Display Derived class's value since it hides Base class's value.
	using Base::func;
	//void func(){cout << "DERIVED class func()" << endl;}
	void func(int a){cout << "DERIVED class func (" << a << ")" << " val=" << val << endl;}

	// See which base class constructor gets called.
    Derived() {val = 20; cout << "Derived class constructor() val=" << val << ", base val=" << Base::val << endl;}  // constructor
    Derived(int inVal) : Base(inVal+100) {val = inVal; cout << "Derived class constructor val=(" << val << "), base val=(" << Base::val << ")" << endl;}  // constructor
    //Derived(int inVal) : Base() {val = inVal; cout << "Derived class constructor val=(" << val << "), base val=" << Base::val << endl;}  // constructor
    //Derived(int inVal) {val = inVal; cout << "Derived class constructor val=(" << val << "), base val=" << Base::val << endl;}  // constructor

    Derived(const Derived& srcClass) : Base(srcClass) {val = srcClass.val+1; cout << "Derived class copy constructor val=" << val << endl;}  // copy constructor

    Derived& operator=(const Derived& rhs)  // assignment operator
    {
    	// Future: consider making Base class assignment here.

    	cout << "Derived class assignment right hand side value " << rhs.val << endl;
    	cout << "left hand side value before " << val << endl;
    	val = rhs.val+2;
    	cout << "left hand side value after assignment " << val << endl;
        return *this;
    }

    ~Derived() {cout << "Derived class destructor for val=" << val << endl;}  // destructor
};

class DerivedPrime: public Derived
{
private:
	int val;
public:
	Dummy dummyVar;

	// See which overridden function is called. Display Derived class's value since it hides Base class's value.
	using Derived::func;
	//void func(){cout << "DERIVEDPRIME class func()" << endl;}
	void func(int a){cout << "DERIVEDPRIME class func (" << a << ")" << " val=" << val << endl;}

	// See which base class constructor gets called.
	DerivedPrime();
	DerivedPrime(int inVal);

	DerivedPrime(const DerivedPrime& srcClass);  // copy constructor

	DerivedPrime& operator=(const DerivedPrime& rhs);  // assignment operator

    ~DerivedPrime() {cout << "DerivedPrime class destructor for val=" << val << endl;}  // destructor
};

// constructor
DerivedPrime::DerivedPrime() {
	val = 30;
	cout <<
		"DerivedPrime class constructor val=" << val <<
		", derived val=" <<	Derived::val <<
		", base val=" << Base::val
	<< endl;
}

// constructor
DerivedPrime::DerivedPrime(int inVal) : Derived(inVal+200) {
	val = inVal;
	cout << "DerivedPrime class constructor val=(" <<
			val << "), derived val=(" <<
			Derived::val << "), base val=" <<
			Base::val << endl;
}

// constructor
//DerivedPrime::DerivedPrime(int inVal) : Derived() {
//	val = inVal;
//	cout << "DerivedPrime class constructor val=(" <<
//	val << "), derived val=" <<
//	Derived::val << ", base val=" <<
//	Base::val << endl;
//}

// constructor
//DerivedPrime::DerivedPrime(int inVal) {
//	val = inVal;
//	cout << "DerivedPrime class constructor val=(" <<
//	val << "), derived val=" <<
//	Derived::val << ", base val=" <<
//	Base::val << endl;
//}

// copy constructor
DerivedPrime::DerivedPrime(const DerivedPrime& srcClass) : Derived(srcClass) {
	val = srcClass.val+1; dummyVar = srcClass.dummyVar; cout << "DerivedPrime class copy constructor val=" << val << endl;
}

// assignment operator
DerivedPrime& DerivedPrime::operator=(const DerivedPrime& rhs) {
	cout << "DerivedPrime class assignment right hand side value " << rhs.val << endl;
	cout << "left hand side value before " << val << endl;
	val = rhs.val+2;
	cout << "left hand side value after assignment " << val << endl;
    return *this;
}

void passByPointer(Base* b, Derived* d)
{
  cout << "In passByPointer function" << endl;

  b->func(51);
  d->func(51);
  b->func();
  d->func();
}

void passByReference(Base& b, Derived& d)
{
  cout << "In passByReference function" << endl;

  b.func(52);
  d.func(52);
  b.func();
  d.func();
}

// Call d's Base class then Derived class copy constructors,
// then b's Base class copy constructor.
void passByValue(Base b, Derived d)
{
  cout << "In passByValue function" << endl;

  b.func(53);
  d.func(53);
  b.func();
  d.func();

  // Upon return call the destructors in the opposite order.
  // That is b's Base class destructor, then d's Derived class
  // then Base class destructors.
}

void friendOfBaseClass(Base b) {
	cout <<
	"Base made friendOfBaseClass its friend, giving it access to Base's protected val=" <<
	b.val <<
	endl;
}

int main()
{
  // Try different constructors
  //Derived d1(1);  // Call Base class constructor then Derived class constructor
  Derived d1;
  cout << endl;

  Derived d2 = d1;  // Call Base class copy constructor then Derived class copy constructor.
  cout << endl;

  // Try different constructors
  //Derived d3(3);  // Call Base class constructor then Derived class constructor
  Derived *pd3 = new Derived(3); // Call Base class constructor then Derived class constructor
  Derived d3 = *pd3;  // Call Base class copy constructor then Derived class copy constructor.
  delete pd3;  // Call Derived class destructor then base class destructor.
  cout << endl;

  d3 = d1;  // Call only Derived class assignment operator.
  cout << endl;

  Base b1(0);  // Call Base class constructor
  friendOfBaseClass(b1);
  cout << endl;

  // Pass by pointer
  cout << "Non-polymorphic parameter pass by pointer" << endl;
  passByPointer(&b1, &d2);
  cout << endl;

  // Pass by reference
  cout << "Non-polymorphic parameter pass by reference" << endl;
  passByReference(b1, d2);
  cout << endl;

  // Pass by value to test copy constructor
  cout << "Non-polymorphic parameter pass by value" << endl;
  passByValue(b1, d2);
  cout << endl;

  // Pass by pointer with polymorphic parameter
  cout << "Polymorphic parameter pass by pointer" << endl;
  passByPointer(&d1, &d2);
  cout << endl;

  // Pass by reference with polymorphic parameter
  cout << "Polymorphic parameter pass by reference" << endl;
  passByReference(d1, d2);
  cout << endl;

  // Pass by value with polymorphic parameter
  cout << "Polymorphic parameter defeated when passed by value" << endl << " since only d1's base class is copied" << endl;
  passByValue(d1, d2);
  cout << endl;

  // Triple inheritance test which virtual gets called.
  cout << "Triple inheritance polymorphic parameter pass by reference" << endl;
  DerivedPrime dp1; // Call Base class then Derived class then DerivedPrime class constructor.
  DerivedPrime dp2 = dp1;  // Call Base class then Derived class then DerivedPrime class copy constructor.
  passByReference(dp1, dp2);
  cout << endl;

  // Test visibility of base class public members.
  dp1.publicNonVirtualBaseFunc();
  cout << endl;

  // Test friend of base class.
  dp1.dummyVar.friendWithBaseClass(b1);
  cout << endl;
}
// Call destructors in following order:
// dp2's DerivedPrime then Derived then Base,
// dp1's DerivedPrime then Derived then Base,
// b1's Base,
// d3's Derived then Base,
// d2's Derived then Base,
// d1's Derived then Base.
