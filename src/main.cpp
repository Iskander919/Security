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

long generateRandom(unsigned int bits);
long getPrimeCandidate(unsigned int bits);
void generatePrimes(int n);
bool millerRabinPassed();

//service functions for rabinMillerTest:
//-----------------------------------------------------//

int expmod(int base, int exp, int mod);
bool trialComposite(int round_tester, int evenComponent, int miller_rabin_candidate, int maxDivisionsByTwo);

//-----------------------------------------------------//

int main() {


	BigInt num1("313");
	BigInt num2("3");
	num1 += num2;

	return 0;

}


//the function for generating n-bit random number (n = bits)
//between 2**(n-1)+1 and 2**n-1

long int generateRandom(unsigned int bits) {

	srand(time(NULL));

	long max = (long)powl(2, bits) - 1;		//calculate upper bound
	long min = (long)powl(2, bits-1) + 1;	//calculate lower bound

	return min + (rand() % (max - min + 1));

}


//the function for checking if obtainded random number
//divisible by few hundred first primes using
//Sieve of Eratosthenes. Returns a candidate to be a prime

long int getPrimeCandidate(unsigned int bits) {

	while(1) {

		long int primeCandidate = generateRandom(bits);


		for (int divisor : fPrimeNums) {

			if ((primeCandidate % divisor == 0) && (divisor*divisor <= primeCandidate)) break;

			else return primeCandidate;

		}

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
bool millerRabinPassed(int miller_rabin_candidate, int rounds) {

  int maxDivisionsByTwo = 0;
  int evenComponent = miller_rabin_candidate-1;

  while (evenComponent % 2 == 0)
  {

    evenComponent >>= 1;
    maxDivisionsByTwo += 1;

  }

  // Set number of trials here

  for (int i = 0; i < (rounds) ; i++)
  {

    int round_tester = rand() * (miller_rabin_candidate - 2) + 2;

    if (trialComposite(round_tester, evenComponent, miller_rabin_candidate, maxDivisionsByTwo))
      return false;

  }

  return true;

}

//-----------------------------------------------------//
//service functions for Rabin-Miller test

int expmod(int base, int exp, int mod) {

	if (exp == 0) return 1;

	if (exp % 2 == 0) {

		return (int)pow(expmod(base, (exp/2), mod), 2) % mod;

	}

	else { return (base * expmod(base, (exp-1), mod)) % mod; }

}

bool trialComposite(int round_tester, int evenComponent, int miller_rabin_candidate, int maxDivisionsByTwo) {

  if (expmod(round_tester, evenComponent, miller_rabin_candidate) == 1 ) return false;

  for (int i = 0; i < maxDivisionsByTwo; i++)
  {

    if (expmod(round_tester, (1 << i) * evenComponent, miller_rabin_candidate) == miller_rabin_candidate - 1)
      return false;

  }

  return true;

}













