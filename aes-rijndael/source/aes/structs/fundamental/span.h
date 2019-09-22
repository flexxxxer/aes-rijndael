#pragma once

#include <vector> // std::vector

namespace fundamental_structs
{
	template <typename T>
	class span
	{
	public:
		span(T* pointer, size_t size) : data(pointer), size(size) { }
		span(std::vector<T> vector, size_t from, size_t to) : data(vector.data() + from), size(to) { }

		T& operator [] (size_t index)
		{
			return data[index];
		}

		T* begin()
		{
			return data;
		}

		T* end()
		{
			return data + size;
		}
	private:
		T* data;
		size_t size;
	};
}