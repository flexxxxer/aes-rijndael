#pragma once

#include <array>

#include "aes.h"
#include "structs/aes_block.h"
#include "structs/fundamental/md5_hash.h"

class aes_128 final : public aes
{
public:

	/**
	 * \param message message to encryption
	 * \param key cipher key
	 * \return encrypted message
	 */
	std::string encrypt(std::string message, std::string key) override
	{
		struct encrypt_128_core
		{
			static aes_block<4, 4> hashed_key_to_block(std::array<uint8_t, 16> hash)
			{
				aes_block<4, 4> state;

				std::copy(hash.begin(), hash.end(), state.matrix.data_array.begin());

				return state;
			}
			static std::vector<aes_block<4, 4>> message_to_blocks(std::string message)
			{
				if (message.size() < 16)
					message.resize(16);

				if (message.size() % 16 != 0)
					message.resize(message.size() + (16 - message.size() % 16));

				std::vector<aes_block<4, 4>> blocks(message.size() / 16);

				for (size_t block_i = 0; block_i < blocks.size(); block_i++)
				{
					std::copy(message.data() + block_i * 16, message.data() + block_i * 16 + 16,
						blocks[block_i].matrix.data_array.begin()
					);
				}

				return blocks;
			}

			static std::string blocks_to_message(std::vector<aes_block<4, 4>> states)
			{
				std::string message;

				for (aes_block<4, 4> & state : states)
					for (size_t j = 0; j < 16; j++)
						message += state.matrix.data_array[j];

				return message;
			}
		};

		std::vector<aes_block<4, 4>> states = encrypt_128_core::message_to_blocks(message); // split message to blocks for encryption
		const std::array<uint8_t, 16> hash = md5_hash::compute(key); // getting hash from key
		const aes_block<4, 4> cipher_key = encrypt_128_core::hashed_key_to_block(hash); // getting cipher key

		const std::array<aes_block<4, 4>, 10> round_keys = aes_block_util::key_util::key_aes128_util::key_expansion(cipher_key);

		for (aes_block<4, 4>& state : states)
		{
			// initial round
			state = aes_block_util::key_util::add_round_key(state, cipher_key);

			// first 9 rounds
			for (size_t j = 0; j < 9; j++)
			{
				state = aes_block_util::sub_bytes(state);
				state = aes_block_util::shift_rows(state);

				state = aes_block_util::aes_128_util::mix_columns(state);

				state = aes_block_util::key_util::add_round_key(state, round_keys[j]);
			}

			// final 10 round
			state = aes_block_util::sub_bytes(state);
			state = aes_block_util::shift_rows(state);
			state = aes_block_util::key_util::add_round_key(state, round_keys.back());
		}

		// build encrypted message
		return encrypt_128_core::blocks_to_message(states);
	}

	/**
	 * \param message message to decryption
	 * \param key cipher key
	 * \return decrypted message
	 */
	std::string decrypt(std::string message, std::string key) override
	{
		struct decrypt_128_core
		{
			static aes_block<4, 4> hashed_key_to_block(std::array<uint8_t, 16> hash)
			{
				aes_block<4, 4> state;

				std::copy(hash.begin(), hash.end(), state.matrix.data_array.begin());

				return state;
			}
			static std::vector<aes_block<4, 4>> message_to_blocks(std::string message)
			{
				if (message.size() < 16)
					throw std::exception("ciphered message is not valid");

				if (message.size() % 16 != 0)
					throw std::exception("ciphered message is not valid");

				std::vector<aes_block<4, 4>> blocks(message.size() / 16);

				for (size_t block_i = 0; block_i < blocks.size(); block_i++)
				{
					std::copy(message.data() + block_i * 16, message.data() + block_i * 16 + 16,
						blocks[block_i].matrix.data_array.begin()
					);
				}

				return blocks;
			}

			static std::string blocks_to_message(std::vector<aes_block<4, 4>> states)
			{
				std::string message;

				for (aes_block<4, 4> & state : states)
					for (size_t j = 0; j < 16; j++)
						message += state.matrix.data_array[j];

				return message;
			}
		};

		std::vector<aes_block<4, 4>> states = decrypt_128_core::message_to_blocks(message); // split message to blocks for encryption
		const std::array<uint8_t, 16> hash = md5_hash::compute(key); // getting hash from key
		const aes_block<4, 4> cipher_key = decrypt_128_core::hashed_key_to_block(hash); // getting cipher key

		const std::array<aes_block<4, 4>, 10> round_keys = aes_block_util::key_util::key_aes128_util::key_expansion(cipher_key);

		for (aes_block<4, 4>& state : states)
		{
			state = aes_block_util::key_util::add_round_key(state, round_keys.back());

			for (size_t j = 8; j != std::numeric_limits<size_t>::max(); j--)
			{
				state = aes_block_util::inv_shift_rows(state);
				state = aes_block_util::inv_sub_bytes(state);

				state = aes_block_util::key_util::add_round_key(state, round_keys[j]);

				state = aes_block_util::aes_128_util::inv_mix_columns(state);
			}

			state = aes_block_util::inv_shift_rows(state);
			state = aes_block_util::inv_sub_bytes(state);
			state = aes_block_util::key_util::add_round_key(state, cipher_key);
		}

		// build decrypted message
		return decrypt_128_core::blocks_to_message(states);
	}
};