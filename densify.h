#ifndef DENSIFY_H
#define DENSIFY_H


#include <vector>
#include <thread>
#include <future>
#include <functional>

#include <eigen3/Eigen/Dense>

#include <QDebug>
#include <qglobal.h>


enum e_base_vals_mode
{
	e_edges,
	e_middle
};


double calc_middle_num(double begin, double end, size_t mid_num, size_t all_num);

double calc_side_num(double base_num, size_t base_i, size_t base_j, size_t i, size_t j);

template<typename T>
void threading_fun(double value, T & output, size_t base_i, size_t base_j, size_t size_col, size_t size_row)
{
	for(size_t row_num = 0; row_num < size_row; ++row_num) {
		for(size_t col_num = 0; col_num < size_col; ++col_num) {
			output(row_num, col_num) += calc_side_num(value, base_i, base_j, row_num, col_num);
		}
	}
}

template<typename T>
int densify(T & input, T & output, size_t size_col, size_t size_row, e_base_vals_mode mode)
{
	if (mode == e_edges) {
		std::vector<std::shared_future<double>> output_sf;
		output_sf.resize(size_col * size_row);

		// 0  .  .  3
		// .  .  .  .
		// .  .  .. ..
		// 12 .. .. 15

		//calculate row middle elems
		for(size_t row_num = 0; row_num < size_row; row_num += size_row - 1) {
			for(size_t col_num = 0; col_num < size_col; ++col_num) {
				output_sf[row_num * size_col + col_num] = std::async(calc_middle_num,
																	 input(0, row_num), input(size_col - 1, row_num),
																	 col_num, size_col - 1);
			}
		}

		//calculate col middle elems
		for(size_t row_num = 1; row_num < size_row - 1; ++row_num) {
			for(size_t col_num = 0; col_num < size_col; ++col_num) {
				output_sf[row_num * size_col + col_num] = std::async(calc_middle_num,
																	 output_sf[col_num].get(),
																	 output_sf[(size_row - 1) * size_col + col_num].get(),
																	 row_num, size_row);
			}
		}

		for(size_t row_num = 0; row_num < size_row; ++row_num) {
			for(size_t col_num = 0; col_num < size_col; ++col_num) {
				output(col_num, row_num) = output_sf[row_num * 4 + col_num].get(); //transposed?
			}
		}
	} else {

		//works for < std::thread::hardware_concurrency() points, others will be ignored
		std::vector<std::thread> t;

		for(size_t row_num = 0; row_num < size_row; ++row_num) {
			for(size_t col_num = 0; col_num < size_col; ++col_num) {
				if (std::abs(input(row_num, col_num)) > 1e-15) {
					if (t.size() >= std::thread::hardware_concurrency() - 1) {
						break;
					}
					double val = input(row_num, col_num);
					t.push_back(std::thread(threading_fun<Eigen::MatrixXd>, val, std::ref(output),
															  col_num, row_num, size_col, size_row));
				}
			}
			if (t.size() >= std::thread::hardware_concurrency()) {
				break;
			}
		}
		std::for_each(t.begin(), t.end(), std::mem_fn(&std::thread::join));

	}
	return 0;
}


#endif // DENSIFY_H
