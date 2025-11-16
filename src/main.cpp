/*
 * main.cpp
 *
 *  Created on: 25 окт. 2025 г.
 *      Author: iskan
 */

#include <algorithm>
#include <cmath>
#include <ctime>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "../inc/bigint.h"

std::vector<int> fPrimeNums;

// generating prime numbers functions:
BigInt generateRandom(int bits);
BigInt getPrimeCandidate(int bits);
BigInt modPow(BigInt base, BigInt exponent, BigInt mod);
void   generatePrimes(int n);
bool   millerRabinPassed(BigInt candidate, int rounds);

// RSA implementing functions:
BigInt pqMultiplication(BigInt p, BigInt q);
BigInt EulerFunction(BigInt p, BigInt q);
BigInt extendedGCD(BigInt a, BigInt b, signedBigInt& x, signedBigInt& y);

int main() {


	generatePrimes(10000); // generating several thousand first primes
	int bits = 4, rounds = 20;
	BigInt num1(19); // p
	BigInt num2(41); // q
	BigInt pq;   // p*q
	BigInt phi;  // (p-1)(q-1)
	BigInt d;
	BigInt gcd;  // result of Euclid algorhitm (finding greatest common divisor)
	BigInt e("7");

	signedBigInt x;
	signedBigInt y;

	/*
	signedBigInt r;

	gcd = extendedGCD(BigInt("691"), BigInt("720"), x, y);

	if (x.sign == -1) {

		d = BigInt("720") - (x.value % BigInt("720"));

	}

	else {

		d = x.value % BigInt("720");

	}

	std::cout << "gcd= " << gcd << std::endl;
	std::cout << "x= "   << x.value << std::endl;
	std::cout << "y= " 	 << y.value << std::endl;

*/





	// generating p and q:
	do {

		num1 = getPrimeCandidate(bits);

	}

	while (!millerRabinPassed(num1, rounds));

	while (1) {

		num2 = getPrimeCandidate(bits);
		if(num2 == num1) continue;

		if (millerRabinPassed(num2, rounds)) break;

	}

	std::cout << "p = " << num1 << std::endl;
	std::cout << "q = " << num2 << std::endl;

	pq  = pqMultiplication(num1, num2);
	phi = EulerFunction(num1, num2);
	gcd = extendedGCD(e, phi, x, y);

	std::cout << "pq = "  << pq << std::endl;
	std::cout << "phi = " << phi << std::endl;

	std::cout << "GCD = " << gcd << std::endl;

	std::cout << "x = "   << x.value << std::endl;
	std::cout << "y = "   << y.value << std::endl;

	if (x.sign == -1) {

		d = phi - (x.value % phi);

	}

	else {

		d = x.value % phi;

	}

	std::cout << "d = " << d << std::endl;
	std::cout << "e = " << e << std::endl;

	std::cout << "----------------------------------" << std::endl;

	std::cout << "Public key:  (" << pq << ", " << e << ")" << std::endl;
	std::cout << "Private key: (" << pq << ", " << d << ")" << std::endl;

	int message = 0;
	std::cout << "Enter message: \n";
	std::cin >> message;

	BigInt encrypt("0");
	BigInt decrypt("0");
	BigInt msg = BigInt(message);

	encrypt = (msg ^ e) % pq;
	decrypt = (encrypt ^ d) % pq;

	std::cout << "Encrypt: " << encrypt << std::endl;
	std::cout << "Decrypt: " << decrypt << std::endl;

	return 0;

}


//the function for generating n-bit random number (n = bits)
//between 2**(n-1)+1 and 2**n-1

BigInt generateRandom(int bits) {

	srand(time(NULL));

	BigInt max("0");
	BigInt min("0");

	max = (BigInt("2")^BigInt(bits)) - BigInt("1");
	min = (BigInt("2")^(BigInt(bits) - BigInt(1))) + BigInt("1");

	return min + (BigInt(rand()) % (max - min + BigInt("1")));

}


//the function for checking if obtainded random number
//divisible by few hundred first primes using
//Sieve of Eratosthenes. Returns a candidate to be a prime

BigInt getPrimeCandidate(int bits) {

	while(1) {

		BigInt primeCandidate = generateRandom(bits);
		bool isDivisible = false;

		for (int divisor : fPrimeNums) {

			if(primeCandidate % BigInt(divisor) == BigInt("0")) {

				isDivisible = true;
				break;

			}

			if(BigInt(divisor) * BigInt(divisor) > primeCandidate) break;

		}

		if (!isDivisible) return primeCandidate;

	}

}

//the function to fill an array of int with first n
//prime numbers using Sieve of Eratosthenes
void generatePrimes(int n) {

	int i = 0;
	int j = 0;

	for (i = 0; i <= n; i++) {								//filling vector with all numbers from 0 to n

		if(i == 0 || i == 1) {fPrimeNums.push_back(0);}

		else {fPrimeNums.push_back(i);}

	}

	i = 0;

	while(i <= n) {


		if (fPrimeNums[i] != 0) {

			j = i + i;

			while(j <= n) {

				fPrimeNums[j] = 0;
				j += i;

			}

		}

		i++;

	}

	fPrimeNums.erase(std::remove(fPrimeNums.begin(), fPrimeNums.end(), 0), fPrimeNums.end());


	/*
	for(int p = 0; p <= fPrimeNums.size(); p++) {

		std::cout << fPrimeNums[p] << std::endl;
	}
	*/
}

//the function that implements Rabin-Miller test.
//returns true if the primeCandidate probably prime and false if it isn't prime.
//The more rounds value, the more precise the result
bool millerRabinPassed(BigInt candidate, int rounds) {

	if (candidate < BigInt("2") || candidate % BigInt("2") == BigInt("0")) return false;
	if (candidate == BigInt("3")) return true;

	BigInt candidateOne("0");
	candidateOne = candidate - BigInt("1");
	BigInt divisionTwoRounds("0");
	BigInt multiplier("0");

	while(candidateOne % BigInt("2") == BigInt("0")) { //represent in the form of (2**divisionTwoRounds) * multiplier

		candidateOne /= BigInt("2");
		divisionTwoRounds += 1;

	}

	multiplier = (candidate - BigInt("1")) / (BigInt("2") ^ divisionTwoRounds);

	for (int i = 0; i < rounds; i++) {

		BigInt randomNum;
		BigInt x;
		bool flag = false;

		//generating a random number between 2 and candidate - 2:
		randomNum = BigInt("2") + BigInt(rand()) % (candidate - BigInt("2")  -  BigInt("2")  +  BigInt("1"));

		//calculating x = randomNum ^ multiplier % candidate
		x = modPow(randomNum, multiplier, candidate);

		if (x == BigInt("1") || x == candidate - BigInt("1")) {

			continue;

		}

		for (int j = 0; j < divisionTwoRounds - 1; j++) {

			x = modPow(x, BigInt("2"), candidate);
			if (x == BigInt("1")) continue;

			if (x == candidate - BigInt("1")) {

				flag = true;
				break;

			}

		}

		if (flag == true) continue;

		return false;

	}

	return true;

}

BigInt modPow(BigInt base, BigInt exponent, BigInt mod) {

	base %= mod;
	BigInt result("1");

	while (exponent > BigInt("0")) {

	    if ((exponent % 2) != BigInt("0")) result = (result * base) % mod;
	    base = (base * base) % mod;

	    exponent /= 2;

	}

	return result;
}

BigInt pqMultiplication(BigInt p, BigInt q) {

	return p * q;

}

BigInt EulerFunction(BigInt p, BigInt q) {

	return (p - BigInt("1")) * (q - BigInt("1"));

}


BigInt extendedGCD(BigInt a, BigInt b, signedBigInt &x, signedBigInt &y)
{
    if (b == BigInt("0")) {
        x.value = BigInt("1");
        x.sign  = 1;

        y.value = BigInt("0");
        y.sign  = 1;

        return a;
    }

    signedBigInt x1, y1;
    BigInt g;

    g = extendedGCD(b, a % b, x1, y1);

    x = y1;

    signedBigInt temp = mul(y1, a / b);
    y = sub(x1, temp);

    return g;

}

BigInt GCD(BigInt num1, BigInt num2) {

	if (num1 % num2 == BigInt("0")) return num2;

	if (num2 % num1 == BigInt("0")) return num1;

	if (num1 > num2) return GCD(num1 % num2, num2);

	return GCD(num1, num2 % num1);


}













