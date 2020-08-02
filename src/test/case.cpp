#include "case.h"
#include <string>
#include "../../build/test.pb.h"

// Returns n! (the factorial of n).  For negative n, n! is defined to be 1.
int Factorial(int n) {
    int result = 1;
    for (int i = 1; i <= n; i++) {
        result *= i;
    }

    return result;
}

bool SerializeAndParse(std::string name, int age)
{
    test::man man1;
    man1.set_age(age);
    man1.set_name(name);
    std::string str;
    man1.SerializeToString(&str);

    test::man man2;
    man2.ParseFromString(str);
    return man1.age() == man2.age() && man1.name() == man2.name();
}

// Returns true if and only if n is a prime number.
bool IsPrime(int n) {
    // Trivial case 1: small numbers
    if (n <= 1) return false;

    // Trivial case 2: even numbers
    if (n % 2 == 0) return n == 2;

    // Now, we have that n is odd and n >= 3.

    // Try to divide n by every odd number i, starting from 3
    for (int i = 3; ; i += 2) {
        // We only have to try i up to the square root of n
        if (i > n / i) break;

        // Now, we have i <= n/i < n.
        // If n is divisible by i, n is not prime.
        if (n % i == 0) return false;
    }

    // n has no integer factor in the range (1, n), and thus is prime.
    return true;
}
