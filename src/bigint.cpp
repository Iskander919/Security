#include "bigint.h"
#include <bits/stdc++.h>

BigInt::BigInt(std::string & s) {

	this -> digits = "";

	int n = s.size();

	for (int i = n - 1; i >= 0; i--) {

		this -> digits.push_back(s[i] - '0');

	}
}

BigInt::BigInt(const char *s) {

	this -> digits = "";

	int n = strlen(s);

	for (int i = n - 1; i >= 0; i--) {

		this -> digits.push_back(s[i] - '0');

	}

}

BigInt::BigInt(unsigned long int num) {

	do {

		this -> digits.push_back(num % 10);
		num /= 10;
	}

	while(num);

}

BigInt::BigInt(BigInt & obj) {

	this -> digits = obj.digits;

}

/*
BigInt::BigInt() {

	this -> digits = "0";

}
*/

int Length(const BigInt & num) {

	return num.digits.size();

}

bool Null(const BigInt & num) {

	if (num.digits.size() == 1 && num.digits[0] == 0) return true;
	else return false;

}

void divide_by_2(BigInt & num) {

    int add = 0;

    for (int i = num.digits.size() - 1; i >= 0;i--) {

        int digit = (num.digits[i] >> 1) + add;
        add = ((num.digits[i] & 1) * 5);
        num.digits[i] = digit;

    }

    while(num.digits.size() > 1 && !num.digits.back()) {num.digits.pop_back();}

}


void Print(const BigInt & num) {

	for (int i = 0; i < Length(num); i++) {

		std::cout << num.digits[i];

	}
}

BigInt &BigInt::operator =(const BigInt & num) {

	this -> digits = num.digits;
	return *this;

}

bool operator == (const BigInt &num1, const BigInt &num2) {

	return num1.digits == num2.digits;

}

bool operator < (const BigInt &num1, const BigInt &num2) {

	long int numOneLength = Length(num1);

	if (numOneLength != Length(num2)) return (Length(num1) < Length(num2));

	while(numOneLength--) {

		if(num1.digits[numOneLength] != num2.digits[numOneLength]) {

			return num1.digits[numOneLength] < num2.digits[numOneLength];

		}
	}

	return false;

}

bool operator > (const BigInt & num1, const BigInt & num2) {

	return num2 < num1;

}

bool operator <= (const BigInt & num1, const BigInt & num2) {

	return !(num1 > num2);

}

bool operator >= (const BigInt & num1, const BigInt & num2) {

	return !(num1 > num2);

}

BigInt &operator +=(BigInt & num1, const BigInt & num2) {

	int t = 0, s, i;

	int n = Length(num1), m = Length(num2);

	if(m > n) num1.digits.append(m - n, 0);

	n = Length(num1);

	for (i = 0; i < n; i++) {

		if(i < m) s = (num1.digits[i] + num2.digits[i]) + t;

		else s = num1.digits[i] + t;

	    t = s / 10;

	    num1.digits[i] = (s % 10);

	}

	if(t) num1.digits.push_back(t);

	return num1;

}

BigInt operator + (const BigInt & num1, const BigInt & num2) {

	BigInt tempObj;
	tempObj = num1;

	tempObj += num2;

	return tempObj;

}

BigInt &operator -= (BigInt & num1, const BigInt & num2) {

	if(num1 < num2) throw("UNDERFLOW");

	int n = Length(num1), m = Length(num2);
	int i, t = 0, s;

	for (i = 0; i < n; i++) {

		if(i < m) s = num1.digits[i] - num2.digits[i] + t;

	    else s = num1.digits[i]+ t;

	    if(s < 0) s += 10, t = -1;

	    else   t = 0;

	    num1.digits[i] = s;

	}

	while(n > 1 && num1.digits[n-1] == 0) {

		num1.digits.pop_back();
		n--;

	}

	return num1;

}

BigInt operator - (const BigInt & num1, const BigInt & num2) {

	BigInt temp;
	temp = num1;
	temp -= num2;
	return temp;
}



BigInt operator * (const BigInt & num1, const BigInt & num2) {

	BigInt temp;
	temp = num1;
	temp *= num2;
	return temp;

}

BigInt &operator *= (BigInt & num1, const BigInt & num2) {

	if(Null(num1) || Null(num2)) {

	        num1 = BigInt();
	        return num1;

	    }

	    int n = num1.digits.size(), m = num2.digits.size();

	    std::vector<int> v(n + m, 0);

	    for (int i = 0; i < n; i++)

	        for (int j = 0; j < m; j++) {

	            v[i + j] += (num1.digits[i] ) * (num2.digits[j]);

	        }

	    n += m;

	    num1.digits.resize(v.size());

	    for (int s, i = 0, t = 0; i < n; i++)
	    {

	        s = t + v[i];
	        v[i] = s % 10;
	        t = s / 10;
	        num1.digits[i] = v[i];

	    }

	    for (int i = n - 1; i >= 1 && !v[i]; i--)

	            num1.digits.pop_back();

	    return num1;

}


BigInt &operator /= (BigInt & num1, const BigInt & num2) {

	if(Null(num2)) throw("Arithmetic Error: Division By 0");

	if(num1 < num2) {

	    num1 = BigInt();
	    return num1;

	}

	if(num1 == num2) {

	    num1 = BigInt(1);
	    return num1;

	}

	int i, lgcat = 0, cc;

	int n = Length(num1);//, m = Length(num2);

	std::vector<int> cat(n, 0);

	BigInt t;

	for (i = n - 1; t * 10 + num1.digits[i] < num2; i--) {

		t *= 10;
		t += num1.digits[i];

	}

	for (; i >= 0; i--) {

	    t = t * 10 + num1.digits[i];
	    for (cc = 9; cc * num2 > t; cc--);
	    t -= cc * num2;
	    cat[lgcat++] = cc;

	}

	num1.digits.resize(cat.size());

	for (i = 0; i < lgcat; i++) num1.digits[i] = cat[lgcat - i - 1];

	num1.digits.resize(lgcat);

	return num1;

}

BigInt operator / (const BigInt &num1, const BigInt &num2) {

	BigInt temp;

	temp = num1;
	temp /= num2;

	return temp;
}

BigInt &operator %= (BigInt &num1, const BigInt &num2) {

    if(Null(num2)) throw("Arithmetic Error: Division By 0");

    if(num1 < num2) {

        return num1;

    }

    if(num1 == num2) {

        num1 = BigInt();
        return num1;

    }

    int i, lgcat = 0, cc;

    int n = Length(num1);//, m = Length(num2);

    std::vector<int> cat(n, 0);

    BigInt t;

    for (i = n - 1; t * 10 + num1.digits[i] < num2; i--) {

        t *= 10;
        t += num1.digits[i];

    }

    for (; i >= 0; i--) {

        t = t * 10 + num1.digits[i];
        for (cc = 9; cc * num2 > t; cc--);
        t -= cc * num2;
        cat[lgcat++] = cc;

    }

    num1 = t;
    return num1;

}

BigInt operator % (const BigInt & num1, const BigInt &num2) {

	BigInt temp;

	temp = num1;
	temp %= num2;

	return temp;
}

BigInt &operator ^= (BigInt & num1, const BigInt &num2) {

    BigInt Exponent, Base(num1);


    Exponent = num2;
    num1 = 1;

    while(!Null(Exponent)) {

        if(Exponent[0] & 1) num1 *= Base;

        Base *= Base;

        divide_by_2(Exponent);

    }

    return num1;

}

BigInt operator ^ (const BigInt &num1, const BigInt &num2) {

	BigInt temp;

	temp = num1;
	temp ^= num2;

	return temp;

}

int BigInt::operator[] (const int index) const {

		if(index < 0 || int(this -> digits.size()) < index) return 0;

		return this -> digits[index];

	}


std::ostream &operator << (std::ostream &out, const BigInt & num) {

    for (int i = num.digits.size() - 1; i >= 0; i--)

        std::cout << (short)num.digits[i];

    return std::cout;

}

std::istream &operator >> (std::istream & in, BigInt & num) {

	std::string s;

	in >> s;

	int n = s.size();

	for (int i = n - 1; i >= 0; i--) {

	    if(!isdigit(s[i])) throw("INVALID NUMBER");

	    num.digits[n - i - 1] = s[i];

	}

	return in;

}

