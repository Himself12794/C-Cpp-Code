//============================================================================
// Name        : main.cpp
// Author      : Philip Whiting
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <functional>

#include "bugger.h"
#include "shapes.h"
#include "heap.hxx"

using std::cout;
using std::endl;

using namespace buggered;

typedef const int (*func)(const long unsigned a, const short unsigned b);

void modify_literal(int &a) {
	a = 7;
}

void callback(func fun) {
	cout << fun(3, 5) << endl;
}

void print_shape_area(shapes::shape& shape) {
	cout << shape.getArea() << endl;
}

const int do_math(const long unsigned a, const short unsigned b) {
	return a * b;
}

void things() {
	const object_a a;
	algorithm( a );

	const object_b b;
	algorithm( b );

	shapes::rectangle* rect;

	shapes::square sq(3);
	shapes::triangle tri(4, 5);

	rect = &sq;

	print_shape_area(*rect);
	print_shape_area(sq);
	print_shape_area(tri);

	callback([](long unsigned int a, short unsigned int b) -> const int {return a - b;});
	callback(do_math);
}

template<typename A, typename B, typename C>
std::function<C (A)> compose(const std::function<B (A)> f, const std::function<C (B)> g) {
	return [&f, &g](A a) { return g(f(a)); };
}


/*template<>
struct composer<void> {

	using functor = std::function<void (void)>;

	static functor compose(functor funct1, functor funct2) {
		return [&funct1, &funct2]() { funct1(); funct2(); };
	}
};*/


typedef struct { int a; } A;
typedef struct { int b; } B;
typedef struct { int c; } C;

inline std::ostream& operator<<(std::ostream& str, const C& c) {
	return str << c.c;
}

int main(int argc, char** argv) {
	//
	auto* b = new bugger(7);
	b->do_bugger();
	cout << b->do_math() << endl;
	b->set_x(52);
	cout << b->do_math() << endl;

	int a = 5;
	cout << a << endl;
	modify_literal(a);
	cout << a << endl;

	line l{ 5, 6 };
	line l2{ 4, 3 };

	cout << "Lines" << endl;
	cout << l << endl;
	cout << l2 << endl << endl;

	cout << "Using methods" << endl;
	cout << l.intercept(l2) << endl;
	cout << l * 4 << endl;

	cout << "Testing traits" << endl;


	std::function<B(A)> f1 = [](A a){ return B{a.a + 5}; };
	std::function<C(B)> f2 = [](B b){ return C{b.b * 5}; };

	cout << compose(f1, f2)(A{3}) << endl;

	//comp->compose([](int a){ return a * 5; });

	//cout << comp->act(3) << endl;

	return heap_main(argc, argv);
}
