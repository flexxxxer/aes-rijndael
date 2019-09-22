#pragma once

#include <string>

class aes  // NOLINT
{
public:

	virtual ~aes() = default;

	/**
	 * \param message message to encryption
	 * \param key cipher key
	 * \return encrypted message
	 */
	virtual std::string encrypt(std::string message, std::string key) = 0;

	/**
	 * \param message message to decryption
	 * \param key cipher key
	 * \return decrypted message
	 */
	virtual std::string decrypt(std::string message, std::string key) = 0;
};