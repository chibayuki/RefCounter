#include <iostream>

#include "RefCounter.h"

using namespace std;

#define bool_to_str(val) ((val) ? ("true") : ("false"))

int main()
{
	Ref<int> r1 = 42; // r1是指向值为42的int对象的引用
	Ref<int> r2 = r1; // 使r2与r1指向同一个int对象
	Ref<int> r3 = *r1; // r3指向r1所指对象的浅表副本

	cout << "r1 = " << r1 << endl; // Output: 42
	cout << "r2 = " << r2 << endl; // Output: 42
	cout << "r3 = " << r3 << endl; // Output: 42
	cout << "r1 == r2: " << bool_to_str(r1 == r2) << endl; // Output: true
	cout << "r1 == r3: " << bool_to_str(r1 == r3) << endl; // Output: false
	cout << endl;

	*r1 = r3 / 2; // 更改r1所指对象的值

	cout << "r1 = " << r1 << endl; // Output: 21
	cout << "r2 = " << r2 << endl; // Output: 21
	cout << "r3 = " << r3 << endl; // Output: 42
	cout << endl;

	r1 = 1024; // 使r1指向新的对象

	cout << "r1 = " << r1 << endl; // Output: 1024
	cout << "r2 = " << r2 << endl; // Output: 21
	cout << "r3 = " << r3 << endl; // Output: 42
	cout << endl;
}