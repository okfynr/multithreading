#ifndef THRSAFE_H
#define THRSAFE_H


#include <vector>
#include <mutex>

template <typename T>
class thrsafe_vector
{
private:
	std::vector<T> data;
	mutable std::mutex m;
public:
	thrsafe_vector(){}
	thrsafe_vector(const thrsafe_vector & other)
	{
		std::lock_guard<std::mutex> lock(other.m);
		data = other.data;
	}
	thrsafe_vector(thrsafe_vector && other) = delete;
	thrsafe_vector & operator=(const thrsafe_vector &) = delete;
	void push_back(T value)
	{
		std::lock_guard<std::mutex> lock(m);
		data.push_back(value);
	}
	const thrsafe_vector & operator[](size_t num) const
	{
		std::lock_guard<std::mutex> lock(m);
		return data[num];
	}
	std::vector<T> get_vector()
	{
		std::lock_guard<std::mutex> lock(m);
		return data;
	}
	void operator()();
	void operator[](size_t);
	void resize();
};

template <typename T>
class matrix
{
private:
	thrsafe_vector<thrsafe_vector<T>> data;
	mutable std::mutex m;

public:
	matrix() {}
	matrix & operator=(const matrix &) = delete;
	void operator()();
	void swap();
	void row();
	void rows();
	void resize();
	void setZero();
	void cols();
	void col(); //??
};

#endif // THRSAFE_H
