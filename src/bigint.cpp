/*
 * bigint.cpp
 *
 *  Created on: 30 окт. 2025 г.
 *      Author: iskan
 */

#include "../inc/bigint.h"
#include <cstring>
#include <string>


BigInt::BigInt(std::string s) {                                     //initializing constructor

	this -> digits = "";

	for (int i = 0; i < (int)s.size(); i++) {

		if(!isdigit(s[i] - '0')) { throw("Not a digit"); }

		this -> digits[i] = s[i];

	}

}

int bgLength(const BigInt&  num) {

	return num.digits.size();

}

//operators overload:

BigInt operator += (BigInt& numA, const BigInt& numB) {

	int sizeA = bgLength(numA), sizeB = bgLength(numB);
	int s = 0, t = 0;


	if (sizeB > sizeA) {

		numA.digits.append(sizeA, 0);

	}

	for (int i = 0; i < sizeA; i++) {

		if (i < sizeA) s = numA.digits[i] + numB.digits[i] + t;

		else s = numA.digits[i] + numB.digits[i];

	}

	t = s/10;
	if (t) numA.digits.push_back(t);

	return numA;

}



