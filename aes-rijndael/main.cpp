#include <iostream>

#include "aes-impl/aes_transformations.h"
#include "aes-impl/aes128.h"

using namespace std;

int main()
{
	const std::string original_message = "hello world";

	aes128 aes;

	const auto encrypted_message = aes.encrypt(original_message, "goodby");
	const auto decrypted_message = aes.decrypt(encrypted_message, "goodby");

	cout << encrypted_message << endl << decrypted_message << endl;

	return 0;
}
