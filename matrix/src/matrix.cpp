#include "matrix.h"

using namespace task;

Matrix::Matrix()
{
	this->elems = new Row[1];
	this->elems[0] = Row();
	this->elems[0][0] = 1.0;
	this->cols = 1;
	this->rows = 1;
}

Matrix::Matrix(const Matrix& copy) {
	this->elems = new Row[copy.rows];

	cols = copy.cols;
	rows = copy.rows;

	for (size_t i = 0; i < this->rows; i++)
	{
		elems[i] = Row(copy[i]);
	}
}

Matrix::~Matrix() {
	delete[] this->elems;
}

Matrix::Matrix(size_t rows, size_t cols) {
	this->elems = new Row[rows];
	this->cols = cols;
	this->rows = rows;

	for (size_t i = 0; i < rows; i++)
		this->elems[i] = Row(cols);

	if (cols < rows)
	{
		for (size_t i = 0; i < cols; i++)
			this->elems[i][i] = 1.0;
	}
	else
	{
		for (size_t i = 0; i < rows; i++)
			this->elems[i][i] = 1.0;
	}
}

Matrix& Matrix::operator= (const Matrix& a) {
	if (&a != this)
	{
		delete[] elems;

		elems = new Row(a.rows);
		cols = a.cols;
		rows = a.rows;

		for (size_t i = 0; i < rows; i++)
		{
			elems[i] = Row(a[i]);
		}

	}

	return *this;
}

double& Matrix::get(size_t row, size_t col) {
	if (row < rows && col < cols)
	{
		return elems[row][col];
	}
	else
	{
		throw OutOfBoundsException();
	}
}

const double& Matrix::get(size_t row, size_t col) const {
	if (row < rows && col < cols)
	{
		return elems[row][col];
	}
	else
	{
		throw OutOfBoundsException();
	}
}

void Matrix::set(size_t row, size_t col, const double& value) {
	if (row < rows && col < cols)
	{
		elems[row][col] = value;
	}
	else
	{
		throw OutOfBoundsException();
	}
}

void Matrix::resize(size_t new_rows, size_t new_cols) {
	Row* new_matrix = new Row[new_rows];

	for (size_t i = 0; i < new_rows; i++)
		new_matrix[i] = Row(new_cols);

	for (size_t i = 0; i < new_rows; i++)
	{
		for (size_t j = 0; j < new_cols; j++)
		{
			if (i < rows && j < cols)
			{
				new_matrix[i][j] = elems[i][j];
			}
			else
			{
				new_matrix[i][j] = 0.0;
			}
		}
	}

	delete[] elems;

	cols = new_cols;
	rows = new_rows;
	elems = new_matrix;
}

Row& Matrix::operator[](size_t row) {
	if (row < rows)
	{
		return elems[row];
	}
	else
	{
		throw OutOfBoundsException();
	}
}

const Row& Matrix::operator[](size_t row) const {
	if (row < rows)
	{
		return elems[row];
	}
	else
	{
		throw OutOfBoundsException();
	}
}

Matrix& Matrix::operator+= (const Matrix& a) {
	if (cols == a.cols && rows == a.rows)
	{
		for (size_t i = 0; i < rows; i++)
		{
			for (size_t j = 0; j < cols; j++)
			{
				elems[i][j] += a[i][j];
			}
		}
	}
	else {
		throw SizeMismatchException();
	}
	return *this;
}

Matrix& Matrix::operator-= (const Matrix& a) {
	if (cols == a.cols && rows == a.rows)
	{
		for (size_t i = 0; i < rows; i++)
		{
			for (size_t j = 0; j < cols; j++)
			{
				elems[i][j] -= a[i][j];
			}
		}
	}
	else {
		throw SizeMismatchException();
	}
	return *this;
}

Matrix& Matrix::operator*= (const double& number) {

	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			elems[i][j] *= number;
		}
	}

	return *this;
}

Matrix Matrix::operator* (const Matrix& a) const {
	Matrix mul(a.rows, a.cols);

	if (cols == a.rows)
	{


		for (size_t i = 0; i < rows; i++)
		{
			for (size_t j = 0; j < cols; j++)
			{
				mul[i][j] = 0.0;

				for (size_t k = 0; k < cols; k++)
				{
					mul[i][j] += elems[i][k] * a[k][j];
				}
				if (fabs(mul[i][j]) < EPS)
				{
					mul[i][j] = 0.0;
				}
			}
		}
	}
	else
	{
		throw SizeMismatchException();
	}

	return mul;
}

Matrix Matrix::operator+ (const Matrix& a) const {
	Matrix sum(rows, cols);
	if (rows == a.rows && cols == a.cols)
	{
		for (size_t i = 0; i < this->rows; i++)
		{
			for (size_t j = 0; j < this->cols; j++)
			{
				sum[i][j] = elems[i][j] + a[i][j];
			}
		}
	}
	else
	{
		throw SizeMismatchException();
	}
	return sum;
}

Matrix Matrix::operator- (const Matrix& a) const {
	Matrix sum(rows, cols);
	if (rows == a.rows && cols == a.cols)
	{
		for (size_t i = 0; i < rows; i++)
		{
			for (size_t j = 0; j < cols; j++)
			{
				sum[i][j] = elems[i][j] - a[i][j];
			}
		}
	}
	else
	{
		throw SizeMismatchException();
	}
	return sum;
}

Matrix Matrix::operator* (const double& a) const {
	Matrix sum(rows, cols);
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			sum[i][j] = elems[i][j] * a;
		}
	}
	return sum;
}

Matrix& Matrix::operator*= (const Matrix& a) {

	if (this->cols == a.rows)
	{
		*this = *this * a;
	}
	else
	{
		throw SizeMismatchException();
	}

	return *this;
}

Matrix Matrix::operator-() const {
	Matrix neg(rows, cols);

	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			neg[i][j] = elems[i][j] * -1.0;
		}
	}
	return neg;
}

Matrix Matrix::operator+() const {
	return Matrix(*this);
}

double Matrix::det() const {
	if (cols != rows)
	{
		throw SizeMismatchException();
	}

	double det = 0;

	if (cols == 1)
	{
		return elems[0][0];
	}
	else if (cols == 2)
	{
		return elems[0][0] * elems[1][1] - elems[0][1] * elems[1][0];
	}
	else {
		Matrix new_matrix(*this);

		for (size_t j = 0; j < cols; j++)
		{
			size_t new_i = 0;
			for (size_t i = 1; i < cols; i++)
			{
				size_t new_k = 0;
				for (size_t k = 0; k < cols; k++)
				{
					if (j == k)
						continue;
					new_matrix[new_i][new_k] = elems[i][k];
					new_k++;
				}
				new_i++;
			}
			det += pow(-1.0, j) * elems[0][j] * new_matrix.det();
		}
	}
	return det;
}

void Matrix::transpose() {
	Matrix new_matrix(*this);

	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			set(i,j, new_matrix[i][j]);
		}
	}
}

Matrix Matrix::transposed() const {
	Matrix new_matrix(*this);

	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			new_matrix[j][i] = elems[i][j];
		}
	}
	return new_matrix;
}

double Matrix::trace() const {
	if (cols != rows)
	{
		throw SizeMismatchException();
	}

	double sum = 0.0;

	for (size_t i = 0; i < rows; i++)
	{
		sum += elems[i][i];
	}
	return sum;
}

std::vector<double> Matrix::getRow(size_t row) {
	if (row >= rows)
	{
		throw OutOfBoundsException();
	}

	std::vector<double> res(cols);

	for (size_t i = 0; i < cols; i++)
	{
		res[i] = elems[row][i];
	}

	return res;
}

std::vector<double> Matrix::getColumn(size_t column) {
	if (column >= cols)
	{
		throw OutOfBoundsException();
	}

	std::vector<double> res(rows);

	for (size_t i = 0; i < rows; i++)
	{
		res[i] = elems[i][column];
	}

	return res;
}

bool Matrix::operator==(const Matrix& a) const {
	if (cols != a.cols || rows != a.rows)
	{
		throw SizeMismatchException();
	}
	else
	{
		for (size_t i = 0; i < rows; i++)
		{
			for (size_t j = 0; j < cols; j++)
			{
				if (fabs(elems[i][j] - a[i][j]) >= EPS)
				{
					return false;
				}
			}
		}
		return true;
	}
}

bool Matrix::operator!=(const Matrix& a) const {
	return !operator==(a);
}

size_t Matrix::sizeRows() const {
	return rows;
}

size_t Matrix::sizeCols() const {
	return cols;
}

std::ostream& task::operator<<(std::ostream& output, const Matrix& matrix) {
	size_t cols = matrix.sizeCols();
	size_t rows = matrix.sizeRows();

	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			output << matrix[i][j] << " ";
		}
		output << std::endl;
	}
	return output;
}
std::istream& task::operator>>(std::istream& input, Matrix& matrix) {
	size_t cols, rows;

	input >> rows >> cols;

	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			input >> matrix[i][j];;
		}
	}
	return input;
}

Matrix  task::operator* (const double& a, const Matrix& b) {
	Matrix mul(b);

	mul *= a;

	return mul;
}
