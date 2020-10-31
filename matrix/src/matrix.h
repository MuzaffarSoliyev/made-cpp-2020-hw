#pragma once

#include <vector>
#include <iostream>
#include <cmath>


namespace task {

    const double EPS = 1e-6;


    class OutOfBoundsException : public std::exception {};
    class SizeMismatchException : public std::exception {};

    class Row
    {
    public:
        friend class Matrix;
        Row() {
            row = new double[1];
            row[0] = 1.0;
            row_size = 1;
        }

        Row(const Row& new_row) {
            row = new double[new_row.row_size];
            row_size = new_row.row_size;

            for (size_t i = 0; i < this->row_size; i++)
            {
                row[i] = new_row[i];
            }

        }
        
        explicit Row(size_t size) {
            row = new double[size];
            this->row_size = size;
            for (size_t i = 0; i < size; i++)
            {
                row[i] = 0.0;
            }
        }


        double& operator[] (size_t col_index) {
            return row[col_index];
        }

        const double& operator[] (size_t col_index) const {
            return row[col_index];
        }

        const size_t size() const {
            return row_size;
        }

        Row& operator= (const Row& new_row) {
            if (&new_row != this)
            {
                delete[] row;
                row = new double[new_row.size()];
                row_size = new_row.size();
                for (size_t i = 0; i < row_size; i++)
                {
                    row[i] = new_row[i];
                }
            }
            return *this;

        }

        ~Row() {
            delete[] row;
        }

    private:
        double* row;
        size_t row_size;

    };

    class Matrix {
    public:

        Matrix();
        ~Matrix();
        Matrix(size_t rows, size_t cols);
        Matrix(const Matrix& copy);
        Matrix& operator=(const Matrix& a);

        double& get(size_t row, size_t col);
        const double& get(size_t row, size_t col) const;
        void set(size_t row, size_t col, const double& value);
        void resize(size_t new_rows, size_t new_cols);

        Row& operator[](size_t row);
        const Row& operator[](size_t row) const;

        Matrix& operator+=(const Matrix& a);
        Matrix& operator-=(const Matrix& a);
        Matrix& operator*=(const Matrix& a);
        Matrix& operator*=(const double& number);

        Matrix operator+(const Matrix& a) const;
        Matrix operator-(const Matrix& a) const;
        Matrix operator*(const Matrix& a) const;
        Matrix operator*(const double& a) const;

        Matrix operator-() const;
        Matrix operator+() const;

        double det() const;
        void transpose();
        Matrix transposed() const;
        double trace() const;

        std::vector<double> getRow(size_t row);
        std::vector<double> getColumn(size_t column);

        bool operator==(const Matrix& a) const;
        bool operator!=(const Matrix& a) const;

         size_t sizeRows() const;
         size_t sizeCols() const;
        // Your code goes here...
    private:
        Row* elems;
        size_t cols;
        size_t rows;
    };


    Matrix operator*(const double& a, const Matrix& b);

    std::ostream& operator<<(std::ostream& output, const Matrix& matrix);
    std::istream& operator>>(std::istream& input, Matrix& matrix);



}  // namespace task
