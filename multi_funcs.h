#ifndef MULTI_FUNCS_H
#define MULTI_FUNCS_H

#include <vector>
#include <thread>
#include <future>
#include <iostream>
#include <eigen3/Eigen/Dense>

#include <QDebug>

#include "thrsafe_pushable_vector.h"
#include "densify.h"


template<typename T>
void mul(T & input1, T & input2, size_t ind_x, size_t ind_y, double & result,
		 thrsafe_vector<std::string> & log)
{
	if(input2.cols() != input1.rows()) {
		result = qQNaN();
		return;
	}
	double res = 0;
	for (auto i = 0; i < input1.rows(); ++i) {
		res += input1(ind_x, i) * input2(i, ind_y);
	}
	//qDebug() << "at" << ind_x << ind_y << "res: " << res;
	log.push_back(std::string("at ") + std::to_string(ind_x) + std::string(" ")
				  + std::to_string(ind_y) + std::string(" res: ") + std::to_string(res));
	//std::cout << "at " << ind_x << " " << ind_y << "res: " << res << std::endl;

	result = res;
}

template<typename T>
void multi_mul(T & input1, T & input2, size_t num, size_t range, std::vector<double> & results,
			   thrsafe_vector<std::string> & log)
{
	//qDebug() << "from" << num << "to " << (num + range);
	log.push_back(std::string("from ") + std::to_string(num) + std::string(" to ") + std::to_string(num + range));
	for(size_t i = 0; i < range; ++i) {
		double result;
		size_t ind_y = static_cast<size_t>(std::floor(static_cast<double>(num + i) / static_cast<double>(input1.cols())));
		size_t ind_x = num + i - ind_y * static_cast<size_t>(input1.cols());
		mul(input1, input2, ind_x, ind_y, result, log);
		results[num + i] = result;
	}
	//qDebug() << "okay";
	log.push_back(std::string("okay"));

}

void result(std::vector<double> & input1, std::vector<double> & input2, std::vector<double> & output, size_t mat_size)
{
	if(input1.size() < 4)
	{
		qDebug() << "input1 size: " << input1.size() << ", returning";
		return;
	}
	if(input2.size() < 4)
	{
		qDebug() << "input2 size: " << input2.size() << ", returning";
		return;
	}
	output.clear();
	output.resize(4);

	Eigen::Index matsize(static_cast<Eigen::Index>(mat_size));


	Eigen::MatrixXd input1e;
	input1e.resize(matsize, matsize);
	input1e.setZero();
	input1e(0, 0) = input1[0];
	input1e(0, matsize - 1) = input1[1];
	input1e(matsize - 1, 0) = input1[2];
	input1e(matsize - 1, matsize - 1) = input1[3];
	Eigen::MatrixXd input2e;
	input2e.resize(matsize, matsize);
	input2e.setZero();
	input2e(0, 0) = input2[0];
	input2e(0, matsize - 1) = input2[1];
	input2e(matsize - 1, 0) = input2[2];
	input2e(matsize - 1, matsize - 1) = input2[3];

	densify(input1e, input1e, mat_size, mat_size, e_edges);
	densify(input2e, input2e, mat_size, mat_size, e_edges);

//	std::cout << "first matrix:" << std::endl;
//	std::cout << input1e << std::endl;
//	std::cout << "second matrix:" << std::endl;
//	std::cout << input2e << std::endl;


	std::vector<double> output_1(mat_size * mat_size);

	const auto num_threads(std::thread::hardware_concurrency() < (input1e.cols() * input1e.rows())
							? (std::thread::hardware_concurrency() - 1)
							: (input1e.cols() * input1e.rows()));

	thrsafe_vector<std::string> log;


	if(std::thread::hardware_concurrency() < (input1e.cols() * input1e.rows())){
		std::vector<std::thread> t;
		auto iters_per_thread = (input1e.cols() * input1e.rows() - 1) / num_threads + 1;
		for(auto i = 0; i < num_threads - 1; ++i) {
			auto iters_last = input1e.cols() * input1e.rows() - i * iters_per_thread;
			t.push_back(std::thread(multi_mul<Eigen::MatrixXd>, std::ref(input1e), std::ref(input2e),
									i * iters_per_thread,
									(iters_per_thread < iters_last ? iters_per_thread : iters_last),
									std::ref(output_1), std::ref(log)));
		}
		std::for_each(t.begin(), t.end(), std::mem_fn(&std::thread::join));
	}
	else {
		std::vector<std::thread> t;
		for(size_t i = 0; i < input1e.rows(); ++i) {
			for(size_t j = 0; j < input2e.cols(); ++j) {
				t.push_back(std::thread(mul<Eigen::MatrixXd>, std::ref(input1e), std::ref(input2e), i, j,
										std::ref(output[j * static_cast<size_t>(input1e.cols()) + i]), std::ref(log)));
			}
		}
		std::for_each(t.begin(), t.end(), std::mem_fn(&std::thread::join));
	}
//	std::vector<std::string> static_log(log.get_vector());
//	for (auto & entry : static_log) {
//		std::cout << entry << std::endl;
//	}

//	qDebug() << "output:";
//	for (size_t i = 0; i < mat_size; ++i) {
//		for (size_t j = 0; j < mat_size; ++j) {
//			std::cout << output_1[i * mat_size + j];
//		}
//		std::cout << std::endl;
//	}


}


#endif // MULTI_FUNCS_H
