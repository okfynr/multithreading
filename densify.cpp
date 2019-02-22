#include "densify.h"

#include <cmath>


double calc_middle_num(double begin, double end, size_t mid_num, size_t all_num)
{
	double temp = (end - begin) * mid_num;
	return begin + temp / all_num;
}

double calc_side_num(double base_num, size_t base_i, size_t base_j, size_t i, size_t j)
{
	auto dist = static_cast<double>((i - base_i) * (i - base_i) + (j - base_j) * (j - base_j));
	if (dist == 0.0)
		return 0;
	double base_dist(5);
	return base_num * std::exp(- dist / base_dist);
}
