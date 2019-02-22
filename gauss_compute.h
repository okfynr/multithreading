#ifndef GAUSS_COMPUTE_H
#define GAUSS_COMPUTE_H

#include <QDebug>
#include <vector>
#include <thread>
#include <future>
#include <iostream>

#include <eigen3/Eigen/Dense>

#include "thrsafe_pushable_vector.h"
#include "densify.h"

template<typename T>
void coeff_use(T & input, Eigen::Index elem_row, Eigen::Index diag_num, double coeff,
			   thrsafe_vector<std::string> & log)
{
	for (auto elem_col = 0; elem_col < input.cols(); ++ elem_col) {
		input(elem_row, elem_col) = input(elem_row, elem_col) - coeff * input(diag_num, elem_col);
		if (input(elem_row, elem_col) < 1e-15) {
			input(elem_row, elem_col) = 0;
		}
	}

	log.push_back(std::string("at comp_coeff row: ") + std::to_string(elem_row) + std::string(" ")
				  + std::string(" iter: ") + std::to_string(input(elem_row, diag_num)));
}

template<typename T>
double comp_coeff(T & input, Eigen::Index diag_num, Eigen::Index comp_row_num,
				  thrsafe_vector<std::string> & log)
{
	log.push_back(std::string("at comp_coeff: ") + std::to_string(diag_num) + std::string(" ")
				  + std::to_string(comp_row_num));

	return input(comp_row_num, diag_num) / input(diag_num, diag_num);
}

template<typename T>
void multi_part(T & input, Eigen::Index init_row_ind,
				thrsafe_vector<std::string> & log)
{
//needed: look after std::thread::hardware_concurrency()

//	const auto num_threads(std::thread::hardware_concurrency() < (input1e.cols() * input1e.rows())
//							? (std::thread::hardware_concurrency() - 1)
//							: (input1e.cols() * input1e.rows()));

	std::vector<std::thread> t;
	for(auto j = init_row_ind; j < input.rows(); ++j) {
		double coeff = comp_coeff(input, init_row_ind - 1, j, log);

		t.push_back(std::thread(coeff_use<T>, std::ref(input), j, init_row_ind - 1, coeff, std::ref(log)));


	}
	std::for_each(t.begin(), t.end(), std::mem_fn(&std::thread::join));

}


void simple_gauss_computation(std::vector<double> & input, size_t mat_size)
{

	if(input.size() < 9)
	{
		qDebug() << "input1 size: " << input.size() << ", returning";
		return;
	}

	Eigen::Index matsize(static_cast<Eigen::Index>(mat_size));


	Eigen::MatrixXd input_e;
	input_e.resize(matsize, matsize);
	input_e.setZero();
	input_e(0, 0) = input[0];
	input_e(0, matsize - 1) = input[2];
	input_e(matsize - 1, 0) = input[6];
	input_e(matsize - 1, matsize - 1) = input[8];

	if(matsize == 3)
	{
		input_e(0, 1) = input[1];
		input_e(1, 0) = input[3];
		input_e(1, 1) = input[4];
		input_e(1, 2) = input[5];
		input_e(2, 1) = input[7];
	}
	else
	{
		densify(input_e, input_e, mat_size, mat_size, e_middle);
	}


	std::cout << "after densifying:" << std::endl;
	for (auto i = 0; i < matsize; ++i) {
		for (auto j = 0; j < matsize; ++j) {
			std::cout << input_e(i, j) << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;


	std::vector<double> output_1(mat_size * mat_size);


	thrsafe_vector<std::string> log;

	for (auto rows_outer_ind = 0; rows_outer_ind < input_e.rows(); ++rows_outer_ind) {

		if (input_e(rows_outer_ind, rows_outer_ind) != 0.0) {

			if (rows_outer_ind != 0) {
				multi_part(input_e, rows_outer_ind, log);
			}

		} else {

			for (auto rows_last_ind = rows_outer_ind; rows_last_ind < input_e.rows(); ++rows_last_ind) {

				if(input_e(rows_last_ind, rows_last_ind) != 0.0) {
					input_e.row(rows_last_ind).swap(input_e.row(rows_outer_ind));
				} else if(rows_last_ind == input_e.rows() - 1) {
					qDebug() << "null column in remainder matrix";
				}

			}

		}
	}


//	std::vector<std::string> static_log(log.get_vector());
//	for (auto & entry : static_log) {
//		std::cout << entry << std::endl;
//	}
	std::cout << "output:" << std::endl;
	for (auto i = 0; i < matsize; ++i) {
		for (auto j = 0; j < matsize; ++j) {
			std::cout << input_e(i, j) << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

}




#endif // GAUSS_COMPUTE_H
