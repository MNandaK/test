//============================================================================
// Name        : TryDiffieHellman.cpp
// Author      : Nandax
// Version     : 0.5 - Minimal version only to get public key
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
		unsigned long pow_positive_int(unsigned short base, unsigned short exponent)
		{
			unsigned long result = 1;

			if(exponent != 0)
				result = base;

			for(unsigned short exp = 2; exp <= exponent; exp++)
				result = result * base;

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
			//was get from http://oeis.org/A001122/list
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

			pubKeyA = util.pow_positive_int(primitiveRoot, secKeyA) % moduloChoose[chosenModulo-1];
			pubKeyB = util.pow_positive_int(primitiveRoot, secKeyB) % moduloChoose[chosenModulo-1];
		}

		void setSecretKeyA(istream &in)
		{
			in >> secKeyA;
		}
		void setSecretKeyB(istream &in)
		{
			in >> secKeyB;
		}

		unsigned short getSecretKeyA()
		{
			return secKeyA;
		}
		unsigned short getSecretKeyB()
		{
			return secKeyB;
		}

		unsigned short getPublicKeyA()
		{
			return pubKeyA;
		}
		unsigned short getPublicKeyB()
		{
			return pubKeyB;
		}

	private:
		unsigned short secKeyA = 0;
		unsigned short secKeyB = 0;
		unsigned int pubKeyA, pubKeyB;
		const unsigned short primitiveRoot = 2;
		unsigned int moduloChoose[3];
		short chosenModulo = 1;	//choose which number: 1->19, 2->227, 3->797
//		const unsigned int moduloChoose[] = {19, 227, 797};	//modulo numbers which primitive root is '2',
//															//get from http://oeis.org/A001122/list
};

int main()
{
	DiffieHellman dh;

//	unsigned int primitiveRootPair[] = {2, 19};


	//cout << "Assalamu'alaikum" << endl; // prints Assalamu'alaikum
	cout << "\n===========================================\n";
	cout << "\n  Try Diffie-Hellman Shared Key Algorithm  \n";
	cout << "\n===========================================\n\n";

	//Input secret key A
	do {
		cout << "Please input A secret key (0-63 value): ";
		dh.setSecretKeyA(cin);

		if(dh.getSecretKeyA() < 0 || dh.getSecretKeyA() > 63)
			cout << "Key value is out of range, please input the correct value :)\n";

		cout << "\n";

	} while(dh.getSecretKeyA() < 0 || dh.getSecretKeyA() > 63);

	//Input secret key B
	do {
		cout << "Please input B secret key (0-63 value): ";
		dh.setSecretKeyB(cin);

		if(dh.getSecretKeyB() < 0 || dh.getSecretKeyB() > 63)
			cout << "Key value is out of range, please input the correct value :)\n";

		cout << "\n";

	} while(dh.getSecretKeyB() < 0 || dh.getSecretKeyB() > 63);

	//Choose modulo
	dh.setUpPrimitiveRootModulo();

	//Calculate public keys
	dh.calcPublicKeys();

	cout << "\nPublic key A = " << dh.getPublicKeyA() << "\nPublic key B = " << dh.getPublicKeyB() << "\n";


#ifdef DEBUG

	cout << "\n\n======= Below values are to try and debug! ========\n\n";

	cout << "\nTry: 2^20 = " << util.pow_positive_int(2, 20);
	cout << "\nMax number in exponent = " << UINT_MAX;

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

//	unsigned __int128 try128_1 = -1;
//	//unsigned __int128 try128_2 = 340282366920938463463374607431768211456;
//
//	//cout << "\nInt try128_1 = " << try128_1 << "\nInt try128_1 = " << try128_2 << "\n";
//	cout << "\nInt try128_1 = " << to_string(try128_1) << "\n";

	cout << "\nSecret key A = " << secKeyA << "\nSecret key B = " << secKeyB << "\n";
#endif

	return 0;
}
