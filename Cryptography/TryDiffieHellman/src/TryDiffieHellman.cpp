//============================================================================
// Name        : TryDiffieHellman.cpp
// Author      : Nandax
// Version     : 2.0 - Adding simple encryption feature successfully
// Date        : December 26, 2021
// Copyright   : This is trial code, fell free to use with reference.
//                I am also referring some resources. Please find list of some
//                references that I used at the end of this file.
// Description : Try to implement basic simple shared key exchange DH algorithm
//============================================================================

#include <iostream>
#include <string>

//#define DEBUG
#define INT_BITS 32

#ifdef DEBUG
#include <climits>

#endif
//#include <cmath>
//using namespace std;
using std::cout;
using std::cin;
using std::istream;
//using std::getline;
using std::string;

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

		// Rotate right by 8 bits
		unsigned int rotate_8_bits(unsigned int num)
		{
			//Implementation on reference:
			//https://www.geeksforgeeks.org/rotate-bits-of-an-integer/
			return (num >> 8)|(num << (INT_BITS - 8));
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
				cin >> chosenModuloIdx;

				if(chosenModuloIdx < 1 || chosenModuloIdx > 3)
					cout << "Wrong choice, please input the correct value :)\n";

				cout << "\n";

			} while(chosenModuloIdx < 1 || chosenModuloIdx > 3);

			modulo = moduloChoose[chosenModuloIdx-1];
			cout << "The primitive root used is '2' for modulo '" << modulo << "'\n";
		}

		void calcPublicKeys()
		{
			Util util;

			pubKeyA = util.mod_exponent_v2(primitiveRoot, secKeyA, modulo);
			pubKeyB = util.mod_exponent_v2(primitiveRoot, secKeyB, modulo);
		}

		void calcSharedSecretKey()
		{
			Util util;

			unsigned int sskA = util.mod_exponent(pubKeyB, secKeyA, modulo);
			unsigned int sskB = util.mod_exponent(pubKeyA, secKeyB, modulo);

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


		unsigned int getModulo()
		{
			return modulo;
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
		short chosenModuloIdx = 1;	//choose which number: 1->19, 2->227, 3->797
		static unsigned int modulo;
};

class SymmetricEncryption
{
	public:
		string doSimpleEncryptOrDecrypt(unsigned int mySecKey, unsigned int otherPubKey)
		{
			Util util;
			DiffieHellman dh;

			string outputTxt = "";

			unsigned int theKey = util.mod_exponent(otherPubKey, mySecKey, dh.getModulo());
#ifdef DEBUG
			cout << "\nThe key used for encryption/decryption = " << theKey << "\n\n";
#endif
			for (int i = 0; i < int(inputTxt.length()); i++)
			{
				outputTxt.push_back(inputTxt[i] ^ theKey);
			}

			return outputTxt;
		}

		void messageFromSender(istream &in)
		{
			char inp[1024];
			//in >> inp;
			//std::getline(in, inp);
			//in.clear();
			//in.sync();
			in.ignore();
			in.getline(inp, 1024);

			setInputTxt(inp);
		}

		void setInputTxt(string inputTxt)
		{
			this->inputTxt = inputTxt;
		}

	private:
		string inputTxt;
};

//Static variable from DiffieHellman class
unsigned int DiffieHellman::modulo;

int main()
{
	DiffieHellman dh;
	SymmetricEncryption sEnc;

	string cipheredTxt, receivedTxt;

	//cout << "Assalamu'alaikum" << endl; // prints Assalamu'alaikum
	cout << "\n===========================================\n";
	cout << "\n  Try Diffie-Hellman Shared Key Algorithm  \n";
	cout << "\n===========================================\n\n";

	/** Shared key generation **/
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

	//Calculate SSK
	dh.calcSharedSecretKey();
	cout << "\nThe shared secret key = " << dh.getSsk() << "\n\n";


	/** Do simple encryption: Aisyah send message to Bukhori **/
	//Send message to be encrypted
	cout << "Message want to send by Aisyah: ";
	sEnc.messageFromSender(cin);
	cipheredTxt = sEnc.doSimpleEncryptOrDecrypt(dh.getSecretKeyA(), dh.getPublicKeyB());
	cout << "\nThe ciphered message: \"" << cipheredTxt << "\"\n";

	//Decipher the message
	sEnc.setInputTxt(cipheredTxt);
	receivedTxt = sEnc.doSimpleEncryptOrDecrypt(dh.getSecretKeyB(), dh.getPublicKeyA());
	cout << "\nBukhori got message: \"" << receivedTxt << "\"\n\n";

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

	unsigned char ch1 = 'a';
	unsigned char ch_key = 'Z';
	unsigned char encrypt_by_xor = ch1 ^ ch_key;

	cout << "\nEncrypted char = '" << encrypt_by_xor << "'\n";

	cout << "\nDecrypted char = '" << char(encrypt_by_xor ^ ch_key) << "'\n";

	//string str1 = "Te";//"Test123 relase 5";
	char str1[] = "Te";
	//string ch_key_str = "Za";
	char ch_key_str[] = "Za";
	//string str_encrypted = string(str1 ^ ch_key_str);
	//char str_encrypted[] = str1 ^ ch_key;

	//cout << "\nEncrypted string = '" << str_encrypted << "'\n";

//	unsigned __int128 try128_1 = -1;
//	//unsigned __int128 try128_2 = 340282366920938463463374607431768211456;
//
//	//cout << "\nInt try128_1 = " << try128_1 << "\nInt try128_1 = " << try128_2 << "\n";
//	cout << "\nInt try128_1 = " << to_string(try128_1) << "\n";

//	cout << "\nSecret key A = " << secKeyA << "\nSecret key B = " << secKeyB << "\n";
#endif

	return 0;
}

//============================================================================
//
// Main references:
// 1. https://en.wikipedia.org/wiki/Diffie%E2%80%93Hellman_key_exchange
// 2. https://en.wikipedia.org/wiki/Primitive_root_modulo_n
// 3. https://mathworld.wolfram.com/PrimitiveRoot.html
// 4. http://oeis.org/A001122/list
// 5. Stallings, William, "Cryptography and Network Security Principle and Practice 5th ed", Pearson Education, Inc., 2011.
// 6. https://www.geeksforgeeks.org/modular-exponentiation-power-in-modular-arithmetic/
// 7. https://www.geeksforgeeks.org/rotate-bits-of-an-integer/
// 8. https://stackoverflow.com/questions/64369972/using-getters-and-setters-with-user-input-in-c
// 9. https://www.geeksforgeeks.org/iterate-over-characters-of-a-string-in-c/
// 10.https://stackoverflow.com/questions/7786994/c-getline-isnt-waiting-for-input-from-console-when-called-multiple-times
//
// ============================== End of File ================================
//============================================================================
