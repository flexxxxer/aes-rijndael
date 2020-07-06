#pragma once

#include <vector> // use std::vector
#include <stdexcept> // use std::out_of_range

template <typename T, size_t RowsCount, size_t ColumnsCount>
class matrix final
{
public:
	std::vector<T> data_array;

	matrix() : data_array(RowsCount* ColumnsCount) {}

	[[nodiscard]] size_t get_columns_count() const
	{
		return ColumnsCount;
	}

	[[nodiscard]] size_t get_rows_count() const
	{
		return RowsCount;
	}

	[[nodiscard]] T& at(const size_t row, const size_t column)
	{
		const size_t index = (ColumnsCount * row) + column;

		if (index >= RowsCount * ColumnsCount ||
			row >= RowsCount || column >= ColumnsCount)
			throw std::out_of_range("");

		return data_array[index];
	}

	[[nodiscard]] T value_at(const size_t row, const size_t column) const
	{
		const size_t index = (ColumnsCount * row) + column;

		if (index >= RowsCount * ColumnsCount ||
			row >= RowsCount || column >= ColumnsCount)
			throw std::out_of_range("");

		return data_array[index];
	}

	[[nodiscard]] std::array<T, RowsCount> get_column(const size_t column_number) const
	{
		std::array<T, RowsCount> column{};

		for (size_t i = 0; i < RowsCount; i++)
			column[i] = this->value_at(i, column_number);

		return column;
	}

	auto begin()
	{
		return this->data_array.begin();
	}

	auto end()
	{
		return this->data_array.end();
	}

	void set_column(std::array<T, RowsCount> arr, const size_t column_number)
	{
		for (size_t i = 0; i < RowsCount; i++)
			this->at(i, column_number) = arr[i];
	}
};