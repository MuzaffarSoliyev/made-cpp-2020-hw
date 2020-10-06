#pragma once
#include <vector>
#include <iostream>
#include <cmath>
using std::vector;
using std::cin;
using std::cout;
using std::endl;


namespace task {
	
	std::vector<double> operator+ (const vector<double>& a, const vector<double>& b) {
		vector<double> c;
		for (size_t i = 0; i < a.size(); i++)
		{
			c.push_back(a[i] + b[i]);
		}
		return c;
	}
	
	std::vector<double> operator+ (const vector<double>& a) {
		return a;
	}

	std::vector<double> operator- (const vector<double>& a) {
		vector<double> c;
		for (size_t i = 0; i < a.size(); i++)
		{
			c.push_back(-1.0 * a[i]);
		}
		return c;
	}
	std::vector<double> operator- (const vector<double>& a, const vector<double>& b) {
		vector<double> c;
		for (size_t i = 0; i < a.size(); i++)
		{
			c.push_back(a[i] - b[i]);
		}
		return c;
	}

	double operator* (const vector<double>& a, const vector<double>& b) {
		double result = 0;
		for (size_t i = 0; i < a.size(); i++)
		{
			result += (a[i] * b[i]);
		}
		return result;
	}

	std::vector<double> operator% (const vector<double>& a, const vector<double>& b) {
		vector<double> c(3);
		c[0] = a[1] * b[2] - a[2] * b[1];
		c[1] = a[2] * b[0] - a[0] * b[2];
		c[2] = a[0] * b[1] - a[1] * b[0];
		return c;
	}

	std::vector<int> operator& (const vector<int>& a, const vector<int>& b) {
		vector<int> c;
		for (size_t i = 0; i < a.size(); i++)
		{
			c.push_back(a[i] & b[i]);
		}
		return c;
	}

	std::vector<int> operator| (const vector<int>& a, const vector<int>& b) {
		vector<int> c;
		for (size_t i = 0; i < a.size(); i++)
		{
			c.push_back(a[i] | b[i]);
		}
		return c;
	}

	bool operator|| (const vector<double>& a, const vector<double>& b) {
		for (size_t i = 0; i < a.size() - 1; i++)
		{ 
			if (fabs( a[i] / b[i] - a[i + 1] / b[i + 1] ) > 0.000000001)
			{
				return false;
			}
		}
		return true;
	}

	bool operator&& (const vector<double>& a, const vector<double>& b) {
		if (a[0] / b[0] > 0)
		{
			return a || b;
		}
		return false;
	}

	std::istream& operator>> (std::istream& stream, vector<double>& a) {
		size_t size;
		stream >> size;

		a.resize(size);
		
		for (size_t i = 0; i < size; i++)
		{
			stream >> a[i];
		}

		return stream;
	}

	std::ostream& operator<< (std::ostream& stream, const vector<double>& a) {
		for (size_t i = 0; i < a.size(); i++)
		{
			stream << a[i] << " ";
		}
		stream << "\n";
		return stream;
	}

	void reverse(vector<double>& a) {
		double tmp;
		size_t n = a.size();
		for (size_t i = 0; i < n / 2; i++)
		{
			tmp = a[n - 1 - i];
			a[n - 1 - i] = a[i];
			a[i] = tmp;
		}
	}
		 
}