#pragma once

#include <string>

class BigInt {
private:
	std::string digits;

public:

	BigInt(std::string s);

	//friend functions
	friend int bgLength(const BigInt & num);

	//operators
	friend BigInt operator += (BigInt& numA, const BigInt& numB);

};
