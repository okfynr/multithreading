#ifndef DENSIFY_H
#define DENSIFY_H


#include <vector>
#include <thread>
#include <future>

#include <qglobal.h>


enum e_base_vals_mode
{
	e_edges,
	e_middle
};


double calc_middle_num(double begin, double end, size_t mid_num, size_t all_num)
{
	double temp = (end - begin) * mid_num;
	return begin + temp / all_num;
}


//wait for smth, calculating

template<typename T>
int densify(T & input, T & output, size_t size_col, size_t size_row, e_base_vals_mode mode)
{
	Q_UNUSED(mode) //e_edges

	std::vector<std::shared_future<double>> output_sf;
	output_sf.resize(size_col * size_row);

	// 0  .  .  3
	// .  .  .  .
	// .  .  .. ..
	// 12 .. .. 15

	//calculate row middle elems
	for(size_t row_num = 0; row_num < size_row; row_num += size_row - 1)
	{
		for(size_t col_num = 0; col_num < size_col; ++col_num)
		{
			output_sf[row_num * size_col + col_num] = std::async(calc_middle_num,
																 input(0, row_num), input(size_col - 1, row_num),
																 col_num, size_col);
		}
	}

	//calculate col middle elems
	for(size_t row_num = 1; row_num < size_row - 1; ++row_num)
	{
		for(size_t col_num = 0; col_num < size_col; ++col_num)
		{
			output_sf[row_num * size_col + col_num] = std::async(calc_middle_num,
																 output_sf[col_num].get(),
																 output_sf[(size_row - 1) * size_col + col_num].get(),
																 row_num, size_row);
		}
	}

	for(size_t row_num = 0; row_num < size_row; ++row_num)
	{
		for(size_t col_num = 0; col_num < size_col; ++col_num)
		{
			output(col_num, row_num) = output_sf[row_num * 4 + col_num].get();
		}
	}
	return 0;
}


#endif // DENSIFY_H
