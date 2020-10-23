#pragma once
#include <cmath>
#include <iostream>
#include <vector>
using std::cin;
using std::cout;
using std::endl;
using std::vector;

const double EPSILON = 0.000000001;

namespace task {

std::vector<double> operator+(const vector<double>& a,
                              const vector<double>& b) {
  vector<double> c(a.size());
  for (size_t i = 0; i < a.size(); i++) {
    c[i] = a[i] + b[i];
  }
  return c;
}

std::vector<double> operator+(const vector<double>& a) { 
  return a; 
}

std::vector<double> operator-(const vector<double>& a) {
  vector<double> c(a.size());
  for (size_t i = 0; i < a.size(); i++) {
    c[i] = -a[i];
  }
  return c;
}
std::vector<double> operator-(const vector<double>& a,
                              const vector<double>& b) {
  vector<double> c(a.size());
  for (size_t i = 0; i < a.size(); i++) {
    c[i] = (a[i] - b[i]);
  }
  return c;
}

double operator*(const vector<double>& a, const vector<double>& b) {
  double result = 0;
  for (size_t i = 0; i < a.size(); i++) {
    result += (a[i] * b[i]);
  }
  return result;
}

std::vector<double> operator%(const vector<double>& a,
                              const vector<double>& b) {
  vector<double> c(3);
  c[0] = a[1] * b[2] - a[2] * b[1];
  c[1] = a[2] * b[0] - a[0] * b[2];
  c[2] = a[0] * b[1] - a[1] * b[0];
  return c;
}

std::vector<int> operator&(const vector<int>& a, const vector<int>& b) {
  vector<int> c(a.size());
  for (size_t i = 0; i < a.size(); i++) {
    c[i] = a[i] & b[i];
  }
  return c;
}

std::vector<int> operator|(const vector<int>& a, const vector<int>& b) {
  vector<int> c(a.size());
  for (size_t i = 0; i < a.size(); i++) {
    c[i] = a[i] | b[i];
  }
  return c;
}

bool operator||(const vector<double>& a, const vector<double>& b) {
  for (size_t i = 0; i < a.size() - 1; i++) {
    if (fabs(b[i]) > EPSILON && fabs(b[i + 1]) > EPSILON) {
      if (fabs(a[i] / b[i] - a[i + 1] / b[i + 1]) > EPSILON) {
        return false;
      }
    } else {
      for (size_t i = 0; i < b.size(); i++) {
        if (fabs(b[i]) > EPSILON) {
          return false;
        }
      }
      return true;
    }
  }
  return true;
}

bool operator&&(const vector<double>& a, const vector<double>& b) {
  if (fabs(b[0]) > EPSILON) {
    if (a[0] / b[0] >= 0) {
      return a || b;
    }
    return false;
  } else {
    return a || b;
  }
}

std::istream& operator>>(std::istream& stream, vector<double>& a) {
  size_t size;
  stream >> size;

  a.resize(size);

  for (size_t i = 0; i < size; i++) {
    stream >> a[i];
  }

  return stream;
}

std::ostream& operator<<(std::ostream& stream, const vector<double>& a) {
  for (size_t i = 0; i < a.size(); i++) {
    stream << a[i] << " ";
  }
  stream << "\n";
  return stream;
}

void reverse(vector<double>& a) {
  size_t n = a.size();
  for (size_t i = 0; i < n / 2; i++) {
    std::swap(a[i], a[n - 1 - i]);
  }
}

}  // namespace task
