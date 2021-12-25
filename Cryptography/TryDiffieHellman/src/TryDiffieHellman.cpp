//============================================================================
// Name        : TryDiffieHellman.cpp
// Author      : Nandax
// Version     : 1.0 - Successfully implement Shared Secret Key (SSK) and
//                      keep same type for some variables
// Copyright   : This is trial code, fell free to use with reference
// Description : Try to implement basic simple shared key exchange DH algorithm
//============================================================================

//#define DEBUG

#include <iostream>

#ifdef DEBUG
#include <climits>
//#include <string>
//
//using std::to_string;
#endif
//#include <cmath>
//using namespace std;
using std::cout;
using std::cin;
using std::istream;

class Util
{
	public:
		// This method calculate power / exponent for integer,
		// negative parameters will not working
		unsigned long pow_positive_int(unsigned short base, unsigned int exponent)
		{
			unsigned long result = 1;

			if(exponent != 0)
				result = base;

			for(unsigned int exp = 2; exp <= exponent; exp++)
				result = result * base;

			return result;
		}

		// This method calculate modulo of exponent integer,
		// negative parameters will not working
		unsigned short mod_exponent(unsigned short base, unsigned int exponent, unsigned short modulo)
		{
			unsigned short result = 1;

			if(exponent > 0)
				result = base;

			for(unsigned int exp = 2; exp <= exponent; exp++)
			{
				//This formula is get from
				//https://www.geeksforgeeks.org/modular-exponentiation-power-in-modular-arithmetic/
				result = ((result % modulo) * (base % modulo)) % modulo;
			}

			return result;
		}

		// Improvement of method to calculate modulo of exponent integer for base 2,
		// negative parameters will not working
		unsigned short mod_exponent_v2(unsigned short base, unsigned int exponent, unsigned short modulo)
		{
			unsigned short result = 1;
			unsigned int exp = exponent;

			if(exponent >= 64)
			{
				result = pow_positive_int(base, 63) % modulo;
				exp = exp - 63;
			}
			else if(exponent > 0)
			{
				result = base;
				exp = exp - 1;
			}

			while(exp >= 64)
			{
				//This formula is get from
				//https://www.geeksforgeeks.org/modular-exponentiation-power-in-modular-arithmetic/
				result = (result * (pow_positive_int(base, 63) % modulo)) % modulo;

				exp = exp - 63;
			}
			result = (result * (pow_positive_int(base, exp) % modulo)) % modulo;


			return result;
		}
};

class DiffieHellman
{
	public:
		DiffieHellman(){}

		void setUpPrimitiveRootModulo()
		{
			//Below modulo numbers which primitive root is '2',
			//is get from http://oeis.org/A001122/list
			moduloChoose[0] = 19;
			moduloChoose[1] = 227;
			moduloChoose[2] = 797;

			do {
				cout << "Primitive root '2' for modulo:\n[1] '19'\n[2] '227'\n[3] '797'\n(please select one number 1-3): ";
				cin >> chosenModulo;

				if(chosenModulo < 1 || chosenModulo > 3)
					cout << "Wrong choice, please input the correct value :)\n";

				cout << "\n";

			} while(chosenModulo < 1 || chosenModulo > 3);

			cout << "The primitive root used is '2' for modulo '" << moduloChoose[chosenModulo-1] << "'\n";
		}

		void calcPublicKeys()
		{
			Util util;

			pubKeyA = util.mod_exponent_v2(primitiveRoot, secKeyA, moduloChoose[chosenModulo-1]);
			pubKeyB = util.mod_exponent_v2(primitiveRoot, secKeyB, moduloChoose[chosenModulo-1]);
		}

		void calcSharedSecretKey()
		{
			Util util;

			unsigned int sskA = util.mod_exponent(pubKeyB, secKeyA, moduloChoose[chosenModulo-1]);
			unsigned int sskB = util.mod_exponent(pubKeyA, secKeyB, moduloChoose[chosenModulo-1]);

			if (sskA == sskB)
				ssk = sskA;
		}

		void setSecretKeyA(istream &in)
		{
			in >> secKeyA;
		}
		void setSecretKeyB(istream &in)
		{
			in >> secKeyB;
		}

		unsigned int getSecretKeyA()
		{
			return secKeyA;
		}
		unsigned int getSecretKeyB()
		{
			return secKeyB;
		}

		unsigned int getPublicKeyA()
		{
			return pubKeyA;
		}
		unsigned int getPublicKeyB()
		{
			return pubKeyB;
		}

		unsigned int getSsk()
		{
			return ssk;
		}

	private:
		unsigned int secKeyA = 0;
		unsigned int secKeyB = 0;
		unsigned int pubKeyA = 1;
		unsigned int pubKeyB = 1;
		unsigned int ssk = 1;
		const unsigned short primitiveRoot = 2;
		unsigned int moduloChoose[3];
		short chosenModulo = 1;	//choose which number: 1->19, 2->227, 3->797
};

int main()
{
	DiffieHellman dh;

	//cout << "Assalamu'alaikum" << endl; // prints Assalamu'alaikum
	cout << "\n===========================================\n";
	cout << "\n  Try Diffie-Hellman Shared Key Algorithm  \n";
	cout << "\n===========================================\n\n";

	//Input secret key A
	cout << "Please input A secret key: ";
	dh.setSecretKeyA(cin);
	cout << "\n";

	//Input secret key B
	cout << "Please input B secret key: ";
	dh.setSecretKeyB(cin);
	cout << "\n";

	//Choose modulo
	dh.setUpPrimitiveRootModulo();

	//Calculate public keys
	dh.calcPublicKeys();

	cout << "\nPublic key A = " << dh.getPublicKeyA() << "\nPublic key B = " << dh.getPublicKeyB() << "\n";

	dh.calcSharedSecretKey();
	cout << "\nThe shared secret key = " << dh.getSsk() << "\n\n";

#ifdef DEBUG
	cout << "\n\n======= Below values are to try and debug! ========\n\n";

//	cout << "\nTry: 2^20 = " << util.pow_positive_int(2, 20);
//	cout << "\nMax number in exponent = " << UINT_MAX;

	unsigned int try1 = 4294967295;
	unsigned int try2 = 4294967296;

	cout << "\nInt try1 = " << try1 << "\nInt try2 = " << try2 << "\n";

	unsigned int try3 = -1;
	cout << "\nInt try3 = " << try3 << "\n";

	unsigned long tryLong = -1;
	cout << "\nInt tryLong = " << tryLong << "\n";

	unsigned long tryLong1 = 18446744073709551615;
	cout << "\nInt tryLong1 = " << tryLong1 << "\n";

	unsigned long tryLong2 = 18446744073709551615 + 1;
	cout << "\nInt tryLong2 = " << tryLong2 << "\n";

	unsigned long long tryLongLong = -1;
	cout << "\nInt tryLongLong = " << tryLongLong << "\n";

	unsigned short tryShort = -1;
	cout << "\nInt tryShort = " << tryShort << "\n";

//	unsigned __int128 try128_1 = -1;
//	//unsigned __int128 try128_2 = 340282366920938463463374607431768211456;
//
//	//cout << "\nInt try128_1 = " << try128_1 << "\nInt try128_1 = " << try128_2 << "\n";
//	cout << "\nInt try128_1 = " << to_string(try128_1) << "\n";

//	cout << "\nSecret key A = " << secKeyA << "\nSecret key B = " << secKeyB << "\n";
#endif

	return 0;
}
