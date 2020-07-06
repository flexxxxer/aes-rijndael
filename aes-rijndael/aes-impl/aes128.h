#pragma once

#include "aes_traits.h"
#include "aes_transformations.h"

#include <stdexcept>
#include <vector>

class aes128 : public aes_traits
{
public:
	std::string encrypt(std::string message, std::string key) override
	{
		if (key.size() > 16)
			throw std::invalid_argument("key");

		if (key.size() < 16)
			key.resize(16);

		std::vector<aes_block<4, 4>> states = aes_transformations::message_to_blocks<4, 4>(message);
		const aes_block<4, 4> cipher_key = aes_transformations::message_to_block<4, 4>(key);
				
		const std::array<aes_block<4, 4>, 10> round_keys = aes_transformations::individual::key_expansion_128(cipher_key);

		for(aes_block<4, 4>& state : states)
		{
			// initial round
			state = aes_transformations::add_round_key(state, cipher_key);

			// first 9 rounds
			for(auto round_key_it = round_keys.cbegin(); round_key_it != std::prev(round_keys.cend()); ++round_key_it)
			{
				state = aes_transformations::sub_bytes(state);
				state = aes_transformations::shift_rows(state);
				state = aes_transformations::individual::mix_columns_128(state);
				state = aes_transformations::add_round_key(state, *round_key_it);
			}

			// final 10 round
			state = aes_transformations::sub_bytes(state);
			state = aes_transformations::shift_rows(state);
			state = aes_transformations::add_round_key(state, round_keys.back());
		}

		// build encrypted message
		return aes_transformations::blocks_to_message(states);
	}
	std::string decrypt(std::string message, std::string key) override
	{
		if (key.size() > 16)
			throw std::invalid_argument("key");

		if (key.size() < 16)
			key.resize(16);

		std::vector<aes_block<4, 4>> states = aes_transformations::message_to_blocks<4, 4>(message);
		const aes_block<4, 4> cipher_key = aes_transformations::message_to_block<4, 4>(key);

		const std::array<aes_block<4, 4>, 10> round_keys = aes_transformations::individual::key_expansion_128(cipher_key);

		for (aes_block<4, 4> & state : states)
		{
			// initial round
			state = aes_transformations::add_round_key(state, round_keys.back());

			// first 9 rounds
			for (auto round_key_it = std::next(round_keys.crbegin()); round_key_it != round_keys.crend(); ++round_key_it)
			{
				state = aes_transformations::inv_shift_rows(state);
				state = aes_transformations::inv_sub_bytes(state);
				state = aes_transformations::add_round_key(state, *round_key_it);
				state = aes_transformations::individual::inv_mix_columns_128(state);
			}

			// final 10 round
			state = aes_transformations::inv_shift_rows(state);
			state = aes_transformations::inv_sub_bytes(state);
			state = aes_transformations::add_round_key(state, cipher_key);
		}

		return aes_transformations::blocks_to_message(states);
	}
};