#include <iostream>

#include "aes-rijndael.hpp"

using namespace std;

int main()
{
	aes_128 p;

	const std::string original_message = "hello world";
	
	const auto encrypted_message = p.encrypt(original_message, "goodby");
	const auto decrypted_message = p.decrypt(encrypted_message, "goodby");

	cout << encrypted_message << endl << decrypted_message << endl;
	
	return 0;
}