#pragma once
#include <string>

// Returns n! (the factorial of n).  For negative n, n! is defined to be 1.
int Factorial(int n);

// Returns true if and only if n is a prime number.
bool IsPrime(int n);

bool SerializeAndParse(std::string name, int age);
