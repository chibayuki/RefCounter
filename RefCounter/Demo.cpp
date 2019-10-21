﻿#include <iostream>

#include "RefCounter.h"

using namespace std;

#define bool_to_str(val) ((val) ? ("true") : ("false"))

int f()
{
	return 61565;
}

int main()
{
	Ref<int> r1 = 42; // r1 指向值为 42 的 int 对象，等效于 Ref<int> r1 = new int(42);
	Ref<int> r2 = r1; // r2 指向 r1 所指的对象
	Ref<int> r3 = *r1; // r3 指向 r1 所指对象的浅表副本

	cout << "r1 = " << r1 << endl; // Output: 42
	cout << "r2 = " << r2 << endl; // Output: 42
	cout << "r3 = " << r3 << endl; // Output: 42
	cout << "r1 == r2: " << bool_to_str(r1 == r2) << endl; // Output: true
	cout << "r1 == r3: " << bool_to_str(r1 == r3) << endl; // Output: false
	cout << endl;

	*r1 = r3 / 2; // 更改 r1 所指对象的值

	cout << "r1 = " << r1 << endl; // Output: 21
	cout << "r2 = " << r2 << endl; // Output: 21
	cout << "r3 = " << r3 << endl; // Output: 42
	cout << endl;

	int i1 = 1024;
	int* i2 = new int(2019);
	r1 = i1; // r1 指向新的 int 对象
	r2 = i2; // r2 指向新的 int 对象

	cout << "r1 = " << r1 << endl; // Output: 1024
	cout << "r2 = " << r2 << endl; // Output: 2019
	cout << endl;

	i1 = 255; // 更改 i1 的值并不会引起 r1 的变化
	*i2 = 255; // 更改 i2 的值会引起 r2 的变化

	cout << "r1 = " << r1 << endl; // Output: 1024
	cout << "r2 = " << r2 << endl; // Output: 255
	cout << "r1 == &i1: " << bool_to_str(r1 == &i1) << endl; // Output: false
	cout << "r2 == i2: " << bool_to_str(r2 == i2) << endl; // Output: true
	cout << endl;

	// r3 = &i1; // 不要引用栈空间对象的地址
}