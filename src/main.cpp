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

BigInt generateRandom(int bits);
BigInt getPrimeCandidate(int bits);
BigInt modPow(BigInt base, BigInt exponent, BigInt mod);
void   generatePrimes(int n);
bool   millerRabinPassed(BigInt candidate, int rounds);

int main() {

	generatePrimes(10000);
	int bits = 16, rounds = 20;
	BigInt num;

	do {

		num = getPrimeCandidate(bits);

	}

	while (!millerRabinPassed(num, rounds));

	std::cout << num;

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















