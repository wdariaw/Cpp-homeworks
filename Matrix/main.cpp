#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cstdlib>

class MatrixAllocationError {
};

class MatrixWrongSizeError {
	int rows_;
	int cols_;
public:
	MatrixWrongSizeError(int rows, int cols) {
		rows_ = rows;
		cols_ = cols;
	}
};

class MatrixIndexError {
	int row_idx_;
	int col_idx_;
public:
	MatrixIndexError(int row_idx, int col_idx) {
		row_idx_ = row_idx;
		col_idx_ = col_idx;
	}
};

class MatrixIsDegenerateError {
public:
	MatrixIsDegenerateError() {}
};


template <typename T> T getZero() {
	return T(0);
}

template <typename T> T getOne() {
	return T(1);
}

//=============== Matrix class ===============//

template <typename T>
class Matrix {
protected:
	int rowsCnt;
	int colsCnt;
	T **array;
public:
	int getRowsNumber() const {
		return rowsCnt;
	}

	int getColumnsNumber() const {
		return colsCnt;
	}

	Matrix(int rows = 0, int cols = 0) : rowsCnt(rows), colsCnt(cols) {
		array = new T*[rows];
		for (int idx = 0; idx < rows; idx++) {
			array[idx] = new T[cols];
		}
		for (int row_idx = 0; row_idx < rows; row_idx++) {
			for (int col_idx = 0; col_idx < cols; col_idx++) {
				array[row_idx][col_idx] = 0;
			}
		}
	}

	Matrix(const Matrix<T>& matrix) : rowsCnt(matrix.rowsCnt), colsCnt(matrix.colsCnt) {
		array = new T*[matrix.rowsCnt];
		for (int idx = 0; idx < rowsCnt; idx++) {
			array[idx] = new T[matrix.colsCnt];
		}
		for (int row_idx = 0; row_idx < matrix.rowsCnt; row_idx++) {
			for (int col_idx = 0; col_idx < matrix.colsCnt; col_idx++) {
				array[row_idx][col_idx] = matrix.array[row_idx][col_idx];
			}
		}
	}

	Matrix<T>& operator=(const Matrix<T>& matrix) {
		if (this != &matrix) {
			if (colsCnt != matrix.colsCnt && rowsCnt != matrix.rowsCnt) {
				for (int idx = 0; idx < rowsCnt; idx++) {
					delete[] array[idx];
				}
				delete[] array;
				array = new T*[matrix.rowsCnt];
				for (int idx = 0; idx < matrix.rowsCnt; idx++) {
					array[idx] = new T[matrix.colsCnt];
				}
			}
			rowsCnt = matrix.rowsCnt;
			colsCnt = matrix.colsCnt;
			for (int row_idx = 0; row_idx < rowsCnt; row_idx++) {
				for (int col_idx = 0; col_idx < colsCnt; col_idx++) {
					array[row_idx][col_idx] = matrix.array[row_idx][col_idx];
				}
			}
		}
		return *this;
	}

	~Matrix() {
		for (int i = 0; i < rowsCnt; ++i) {
			delete[] array[i];
		}
		delete[] array;
		rowsCnt = 0;
		colsCnt = 0;
	}

	friend std::istream& operator>>(std::istream& stream, const Matrix<T>& matrix) {
		T temp;
		for (size_t i = 0; i < matrix.rowsCnt; ++i) {
			for (size_t j = 0; j < matrix.colsCnt; ++j) {
				stream >> temp;
				matrix.array[i][j] = temp;
			}
		}
		return stream;
	}

	friend std::ostream& operator<<(std::ostream& stream, const Matrix<T>& matrix) {
		for (size_t i = 0; i < matrix.rowsCnt; ++i) {
			for (size_t j = 0; j < matrix.colsCnt; ++j) {
				stream << matrix.array[i][j] << " ";
			}
			stream << "\n";
		}
		return stream;
	}

	Matrix<T> operator+(const Matrix<T>& matrix) const {
		if ((colsCnt == matrix.colsCnt) && (rowsCnt == matrix.rowsCnt)) {
			Matrix<T> result(rowsCnt, colsCnt);
			for (int i = 0; i < rowsCnt; i++) {
				for (int j = 0; j < colsCnt; j++) {
					result.array[i][j] = array[i][j] + matrix.array[i][j];
				}
			}
			return result;
		}
		else {
			throw MatrixWrongSizeError(matrix.rowsCnt, matrix.colsCnt);
		}
	}

	Matrix<T> operator*(const T& num) const {
		Matrix<T> result(rowsCnt, colsCnt);
		for (int i = 0; i < rowsCnt; i++) {
			for (int j = 0; j < colsCnt; j++) {
				result.array[i][j] = array[i][j] * num;
			}
		}
		return result;
	}

	Matrix<T> operator/(const T& num) {
		Matrix<T> result(rowsCnt, colsCnt);
		for (int i = 0; i < rowsCnt; i++) {
			for (int j = 0; j < colsCnt; j++) {
				result.array[i][j] /= num;
			}
		}
		return result;
	}

	Matrix<T>& operator*=(const T& num) {
		*this = *this * num;
		return *this;
	}

	friend Matrix<T> operator*(const T& num, const Matrix<T>& matrix) {
		Matrix<T> result(matrix);
		result *= num;
		return result;
	}

	Matrix<T> operator-(const Matrix<T>& matrix) const {
		if (colsCnt == matrix.colsCnt && rowsCnt == matrix.rowsCnt) {
			Matrix result(matrix);
			result *= -1;
			result = *this + result;
			return result;
		}
		else {
			throw MatrixWrongSizeError(matrix.rowsCnt, matrix.colsCnt);
		}
	}

	Matrix<T>& operator-=(const Matrix<T>& matrix) {
		if (colsCnt == matrix.colsCnt && rowsCnt == matrix.rowsCnt) {
			*this = *this - matrix;
			return *this;
		}
		else {
			throw MatrixWrongSizeError(matrix.rowsCnt, matrix.colsCnt);
		}
	}

	Matrix<T>& operator+=(const Matrix<T>& matrix) {
		if (colsCnt == matrix.colsCnt && rowsCnt == matrix.rowsCnt) {
			*this = *this + matrix;
			return *this;
		}
		else {
			throw MatrixWrongSizeError(matrix.rowsCnt, matrix.colsCnt);
		}
	}

	Matrix<T> operator*(const Matrix<T>& other) const {
		if (colsCnt == other.rowsCnt) {
			Matrix<T> result(rowsCnt, other.colsCnt);
			for (int i = 0; i < result.rowsCnt; i++) {
				for (int j = 0; j < result.colsCnt; j++) {
					for (int k = 0; k < colsCnt; k++) {
						result.array[i][j] = result.array[i][j] + array[i][k] * other.array[k][j];
					}
				}
			}
			return result;
		}
		else {
			throw MatrixWrongSizeError(other.rowsCnt, other.colsCnt);
		}
	}

	Matrix<T>& operator*=(const Matrix<T>& matrix) {
		if (colsCnt == matrix.rowsCnt) {
			*this = *this * matrix;
			return *this;
		}
		else {
			throw MatrixWrongSizeError(matrix.rowsCnt, matrix.colsCnt);
		}
	}

	Matrix<T>& operator/=(const T& num) {
		*this = *this / num;
		return *this;
	}

	T operator()(int i, int j) const {
		if ((i >= 0 && i < rowsCnt) && (j >= 0 && j < colsCnt)) {
			return array[i][j];
		}
		else {
			throw MatrixIndexError(i, j);
		}
	}

	T& operator()(int i, int j) {
		if ((i >= 0 && i < rowsCnt) && (j >= 0 && j < colsCnt)) {
			return array[i][j];
		}
		else {
			throw MatrixIndexError(i, j);
		}
	}

	Matrix<T>& transpose() {
		T t;
		Matrix transposed_matrix(colsCnt, rowsCnt);
		int min_size = std::min(rowsCnt, colsCnt);
		for (int i = 0; i < min_size; ++i) {
			for (int j = 0; j < min_size; ++j) {
				transposed_matrix.array[i][j] = array[i][j];
			}
		}

		for (int i = colsCnt - 1; i >= 0; --i) {
			int transposed_col_idx = 0;
			for (int j = 0; j < rowsCnt; ++j) {
				t = transposed_matrix.array[i][transposed_col_idx];
				transposed_matrix.array[i][transposed_col_idx] = array[transposed_col_idx][i];
				array[transposed_col_idx][i] = t;
				++transposed_col_idx;
			}
		}

		for (int idx = 0; idx < rowsCnt; idx++) {
			delete[] array[idx];
		}
		delete[] array;
		array = new T*[colsCnt];
		for (int i = 0; i < colsCnt; i++) {
			array[i] = new T[rowsCnt];
		}

		for (int row_idx = 0; row_idx < transposed_matrix.rowsCnt; row_idx++) {
			for (int col_idx = 0; col_idx < transposed_matrix.colsCnt; col_idx++) {
				array[row_idx][col_idx] = transposed_matrix.array[row_idx][col_idx];
			}
		}
		rowsCnt = transposed_matrix.rowsCnt;
		colsCnt = transposed_matrix.colsCnt;
		return *this;
	}

	Matrix<T> getTransposed() {
		T t;
		Matrix transposed_matrix(colsCnt, rowsCnt);
		int min_size = std::min(rowsCnt, colsCnt);
		for (int i = 0; i < min_size; ++i) {
			for (int j = 0; j < min_size; ++j) {
				transposed_matrix.array[i][j] = array[i][j];
			}
		}
		for (int i = colsCnt - 1; i >= 0; --i) {
			int transposed_col_idx = 0;
			for (int j = 0; j < rowsCnt; ++j) {
				transposed_matrix.array[i][transposed_col_idx] = array[transposed_col_idx][i];
				++transposed_col_idx;
			}
		}
		return transposed_matrix;
	}
};
//=============== SquareMatrix class ===============//

template <typename T>
class SquareMatrix : public Matrix<T> {
public:
	SquareMatrix(int size = 0) : Matrix<T>(size, size) {}

	SquareMatrix(const SquareMatrix<T>& matrix) : Matrix<T>(matrix) {}

	SquareMatrix(const Matrix<T>& matrix) : Matrix<T>(matrix) {}

	int getSize() const{
		return this->rowsCnt;
	}

	SquareMatrix<T>& operator=(const SquareMatrix<T>& matrix) {
		if (this != &matrix) {
			for (int idx = 0; idx < this->rowsCnt; idx++) {
				delete[] this->array[idx];
			}
			delete[] this->array;
			this->array = new T*[matrix.rowsCnt];
			for (int idx = 0; idx < matrix.rowsCnt; idx++) {
				this->array[idx] = new T[matrix.colsCnt];
			}
			this->rowsCnt = matrix.rowsCnt;
			this->colsCnt = matrix.colsCnt;
			for (int row_idx = 0; row_idx < this->rowsCnt; row_idx++) {
				for (int col_idx = 0; col_idx < this->colsCnt; col_idx++) {
					this->array[row_idx][col_idx] = matrix.array[row_idx][col_idx];
				}
				}
		}
		return *this;
	}


	SquareMatrix<T> operator*(const SquareMatrix<T>& other) const {
		if (this->colsCnt == other.rowsCnt) {
			SquareMatrix<T> result(this->rowsCnt);
			for (int i = 0; i < result.rowsCnt; i++) {
				for (int j = 0; j < result.rowsCnt; j++) {
					for (int k = 0; k < this->rowsCnt; k++) {
						result.array[i][j] += this->array[i][k] * other.array[k][j];
					}
				}
			}
			return result;
		}
		else {
			throw MatrixWrongSizeError(other.rowsCnt, other.colsCnt);
		}
	}

	SquareMatrix<T>& operator*=(const SquareMatrix<T>& matrix) {
		if (this->colsCnt == matrix.rowsCnt) {
			*this = *this * matrix;
			return *this;
		}
		else {
			throw MatrixWrongSizeError(matrix.rowsCnt, matrix.colsCnt);
		}
	}

	SquareMatrix<T> operator*(const T& num) const {
		SquareMatrix<T> result(this->rowsCnt);
		for (int i = 0; i < this->rowsCnt; i++) {
			for (int j = 0; j < this->colsCnt; j++) {
				result(i, j) = this->array[i][j] * num;
			}
		}
		return result;
	}

	SquareMatrix<T>& operator*=(const T& num) {
		*this = *this * num;
		return *this;
	}

	friend SquareMatrix<T> operator*(const T& num, const SquareMatrix<T>& matrix) {
		SquareMatrix<T> result(matrix);
		result *= num;
		return result;
	}

	SquareMatrix<T> operator+(const SquareMatrix<T>& matrix) const {
		if ((this->colsCnt == matrix.colsCnt) && (this->rowsCnt == matrix.rowsCnt)) {
			SquareMatrix<T> result(this->rowsCnt);
			for (int i = 0; i < this->rowsCnt; i++) {
				for (int j = 0; j < this->colsCnt; j++) {
					result.array[i][j] = this->array[i][j] + matrix.array[i][j];
				}
			}
			return result;
		}
		else {
			throw MatrixWrongSizeError(matrix.rowsCnt, matrix.colsCnt);
		}
	}

	SquareMatrix<T> operator-(const SquareMatrix<T>& matrix) const {
		if (this->colsCnt == matrix.colsCnt && this->rowsCnt == matrix.rowsCnt) {
			SquareMatrix<T> result = SquareMatrix<T>(matrix);
			result *= static_cast<T>(-1);
			result = *this + result;
			return SquareMatrix(result);
		}
		else {
			throw MatrixWrongSizeError(matrix.rowsCnt, matrix.colsCnt);
		}
	}

	T getDeterminant() const {
		T det = 0;
		if (this->rowsCnt == 1) {
			det = this->array[0][0];
		}
		else if (this->rowsCnt == 2) {
			det = this->array[0][0] * this->array[1][1] - this->array[1][0] * this->array[0][1];
		}
		else {
			SquareMatrix<T> temp(this->rowsCnt - 1);
			for (int neglected_col_idx = 0; neglected_col_idx < this->rowsCnt; ++neglected_col_idx) {
				for (int row_idx = 0; row_idx < this->rowsCnt - 1; ++row_idx) {
					for (int col_idx = 0; col_idx < this->rowsCnt - 1; ++col_idx) {
						if (col_idx < neglected_col_idx) {
							temp.array[row_idx][col_idx] = this->array[row_idx + 1][col_idx];
						}
						else {
							temp.array[row_idx][col_idx] = this->array[row_idx + 1][col_idx + 1];
						}
					}
				}
				det += static_cast<T>(pow(-1, neglected_col_idx)) * temp.getDeterminant() * this->array[0][neglected_col_idx];
			}
		}
		return det;
	}

	T getTrace() const {
		T trace = 0;
		for (int i = 0; i < this->rowsCnt; ++i) {
			trace += this->array[i][i];
		}
		return trace;
	}

	SquareMatrix<T> getInverse() const {
		T det = getDeterminant();
		SquareMatrix<T> inverse_matrix(this->rowsCnt);
		if (det != 0) {
			SquareMatrix<T> temp(this->rowsCnt - 1);

			for (int neglected_row_idx = 0; neglected_row_idx < this->rowsCnt; ++neglected_row_idx) {
				for (int neglected_col_idx = 0; neglected_col_idx < this->rowsCnt; ++neglected_col_idx) {
					for (int row_idx = 0; row_idx < this->rowsCnt - 1; ++row_idx) {
						for (int col_idx = 0; col_idx < this->rowsCnt - 1; ++col_idx) {
							if (col_idx < neglected_col_idx) {
								temp.array[row_idx][col_idx] = (row_idx < neglected_row_idx) ?
									this->array[row_idx][col_idx] : this->array[row_idx + 1][col_idx];
							}
							else {
								temp.array[row_idx][col_idx] = (row_idx < neglected_row_idx) ?
									this->array[row_idx][col_idx + 1] : this->array[row_idx + 1][col_idx + 1];
							}
						}
					}
					inverse_matrix.array[neglected_row_idx][neglected_col_idx] = static_cast<T>(pow(-1, neglected_col_idx +
						neglected_row_idx)) * temp.getDeterminant() / det;
				}
			}
			inverse_matrix.transpose();
			return inverse_matrix;
		}
		else {
			throw MatrixIsDegenerateError();
		}
	}

	SquareMatrix<T>& invert() {
		T det = getDeterminant();
		SquareMatrix<T> inverse_matrix(this->rowsCnt);
		if (det != 0) {
			SquareMatrix<T> temp(this->rowsCnt - 1);

			for (int neglected_row_idx = 0; neglected_row_idx < this->rowsCnt; ++neglected_row_idx) {
				for (int neglected_col_idx = 0; neglected_col_idx < this->rowsCnt; ++neglected_col_idx) {
					for (int row_idx = 0; row_idx < this->rowsCnt - 1; ++row_idx) {
						for (int col_idx = 0; col_idx < this->rowsCnt - 1; ++col_idx) {
							if (col_idx < neglected_col_idx) {
								temp.array[row_idx][col_idx] = (row_idx < neglected_row_idx) ?
									this->array[row_idx][col_idx] : this->array[row_idx + 1][col_idx];
							}
							else {
								temp.array[row_idx][col_idx] = (row_idx < neglected_row_idx) ?
									this->array[row_idx][col_idx + 1] : this->array[row_idx + 1][col_idx + 1];
							}
						}
					}
					inverse_matrix.array[neglected_row_idx][neglected_col_idx] = static_cast<T>(pow(-1, neglected_col_idx +
						neglected_row_idx)) * temp.getDeterminant() / det;
				}
			}
			inverse_matrix.transpose();
			for (int i = 0; i < this->rowsCnt; ++i) {
				for (int j = 0; j < this->rowsCnt; ++j) {
					this->array[i][j] = inverse_matrix.array[i][j];
				}
			}
			return *this;
		}
		else {
			throw MatrixIsDegenerateError();
		}
	}


	SquareMatrix<T>& transpose() {
		T t;
		SquareMatrix<T> transposed_matrix(this->colsCnt);
		int min_size = std::min(this->rowsCnt, this->colsCnt);
		for (int i = 0; i < min_size; ++i) {
			for (int j = 0; j < min_size; ++j) {
				transposed_matrix.array[i][j] = this->array[i][j];
			}
		}

		for (int i = this->colsCnt - 1; i >= 0; --i) {
			int transposed_col_idx = 0;
			for (int j = 0; j < this->rowsCnt; ++j) {
				t = transposed_matrix.array[i][transposed_col_idx];
				transposed_matrix.array[i][transposed_col_idx] = this->array[transposed_col_idx][i];
				this->array[transposed_col_idx][i] = t;
				++transposed_col_idx;
			}
		}

		for (int idx = 0; idx < this->rowsCnt; idx++) {
			delete[] this->array[idx];
		}
		delete[] this->array;
		this->array = new T*[this->colsCnt];
		for (int i = 0; i < this->colsCnt; i++) {
			this->array[i] = new T[this->rowsCnt];
		}

		for (int row_idx = 0; row_idx < transposed_matrix.rowsCnt; row_idx++) {
			for (int col_idx = 0; col_idx < transposed_matrix.colsCnt; col_idx++) {
				this->array[row_idx][col_idx] = transposed_matrix.array[row_idx][col_idx];
			}
		}
		this->rowsCnt = transposed_matrix.rowsCnt;
		this->colsCnt = transposed_matrix.colsCnt;
		return *this;
	}


	SquareMatrix<T> getTransposed() {
		T t;
		SquareMatrix<T> transposed_matrix(this->colsCnt);
		int min_size = std::min(this->rowsCnt, this->colsCnt);
		for (int i = 0; i < min_size; ++i) {
			for (int j = 0; j < min_size; ++j) {
				transposed_matrix.array[i][j] = this->array[i][j];
			}
		}
		for (int i = this->colsCnt - 1; i >= 0; --i) {
			int transposed_col_idx = 0;
			for (int j = 0; j < this->rowsCnt; ++j) {
				transposed_matrix.array[i][transposed_col_idx] = this->array[transposed_col_idx][i];
				++transposed_col_idx;
			}
		}
		return transposed_matrix;
	}
};

//================ class Rational ===============//

int GCD(int lhs, int rhs) {
	int remainder;
	while (rhs != 0) {
		remainder = lhs % rhs;
		lhs = rhs;
		rhs = remainder;
	}
	return lhs;
}

class RationalDivisionByZero {
public:
	RationalDivisionByZero() {}
};

class Rational {
private:
	int p_;
	int q_;

	void reduce() {
		bool is_negative_ = (p_ > 0 && q_ < 0) || (p_ < 0 && q_ > 0);
		p_ = std::abs(p_);
		q_ = std::abs(q_);
		int gcd = GCD(p_, q_);
		p_ /= gcd;
		q_ /= gcd;
		if (is_negative_) {
			p_ *= -1;
		}
	}

public:
	Rational() {}

	Rational(int p, int q = 1) {
		p_ = p;
		q_ = q;
		reduce();
	}

	Rational(const Rational& num) {
		p_ = num.p_;
		q_ = num.q_;
	}

	int getNumerator() const {
		return p_;
	}

	int getDenominator() const {
		return q_;
	}

	friend std::istream& operator>>(std::istream &stream, Rational& num) {
		int p, q;
		int number = std::scanf("%d/%d", &p, &q);
		if (p == 0 || number <= 1) {
			q = 1;
		}
		num = Rational(p, q);
		return stream;
	}

	friend std::ostream& operator<<(std::ostream &stream, const Rational& num) {
		if (num.q_ == 1) {
			stream << num.p_;
		}
		else {
			stream << num.p_ << '/' << num.q_;
		}
		return stream;
	}

	Rational operator-() const {
		return Rational(-p_, q_);
	}

	Rational operator+() const {
		return *this;
	}

	bool operator==(const Rational& num) const {
		if (p_ == num.p_ && q_ == num.q_) {
			return true;
		}
		return false;
	}

	bool operator!=(const Rational& num) const {
		if (!(*this == num)) {
			return true;
		}
		return false;
	}

	bool operator<(const Rational& num) const {
		if (p_ * num.q_ - num.p_ * q_ < 0) {
			return true;
		}
		return false;
	}

	bool operator<=(const Rational& num) const {
		if (*this < num || *this == num) {
			return true;
		}
		return false;
	}

	bool operator>(const Rational& num) const {
		if (p_ * num.q_ - num.p_ * q_ > 0) {
			return true;
		}
		return false;
	}

	bool operator>=(const Rational& num) const {
		if (*this > num || *this == num) {
			return true;
		}
		return false;
	}

	Rational operator/(const Rational& num) const {
		if (num == 0) {
			throw RationalDivisionByZero();
		}
		else {
			Rational result(p_ * num.q_, q_ * num.p_);
			return result;
		}
	}

	Rational operator+(const Rational& num) const {
		Rational result(p_ * num.q_ + num.p_ * q_, q_ * num.q_);
		return result;
	}

	Rational operator*(const Rational& num) const {
		Rational result(p_ * num.p_, q_ * num.q_);
		return result;
	}

	Rational operator-(const Rational& num) const {
		Rational result(p_ * num.q_ - num.p_ * q_, q_ * num.q_);
		return result;
	}

	Rational& operator=(const Rational& num) {
		if (this != &num) {
			p_ = num.p_;
			q_ = num.q_;
		}
		return *this;
	}

	Rational& operator+=(const Rational& num) {
		*this = *this + num;
		reduce();
		return *this;
	}

	Rational& operator-=(const Rational& num) {
		*this = *this - num;
		reduce();
		return *this;
	}

	Rational& operator/=(const Rational& num) {
		if (num != 0) {
			*this = *this / num;
			reduce();
			return *this;
		}
		else {
			throw RationalDivisionByZero();
		}
	}

	Rational& operator*=(const Rational& num) {
		*this = *this * num;
		reduce();
		return *this;
	}

	Rational& operator++() {
		*this += Rational(1, 1);
		reduce();
		return *this;
	}

	Rational& operator--() {
		*this -= Rational(1, 1);
		reduce();
		return *this;
	}

	Rational operator++(int) {
		Rational tmp(*this);
		++(*this);
		return tmp;
	}

	Rational operator--(int) {
		Rational tmp(*this);
		--(*this);
		return tmp;
	}

	friend Rational operator/(int val, const Rational& num) {
		if (num == 0) {
			RationalDivisionByZero ex;
			throw ex;
		}
		else {
			Rational result(val * num.q_, num.p_);
			return result;
		}
	}

	friend Rational operator+(int val, const Rational& num) {
		return Rational(num + val);
	}

	friend bool operator==(int val, const Rational& num) {
		return (num == val);
	}

	friend bool operator!=(int val, const Rational& num) {
		return (num != val);
	}

	friend bool operator<(int val, const Rational& num) {
		return (num > val);
	}

	friend bool operator<=(int val, const Rational& num) {
		return (num >= val);
	}

	friend bool operator>(int val, const Rational& num) {
		return (num < val);
	}

	friend bool operator>=(int val, const Rational& num) {
		return (num <= val);
	}

};

//=================== main() ===============//

using namespace std;

int main() {
	int m, n, p;
	Rational r;
	cin >> m >> n >> p >> r;

	Matrix<Rational> A(m, n);
	SquareMatrix<Rational> S(p);
	cin >> A >> S;

	try {
		cout << (A * S) * A.getTransposed() << endl;
	}
	catch (const MatrixWrongSizeError&) {
		cout << "A and S have not appropriate sizes for multiplication." << endl;
	}

	cout << (r * (S = S) * S).getSize() << endl;

	SquareMatrix<Rational> P(S);

	cout << (P * (S + S - 3 * P)).getDeterminant() << endl;
	
	const SquareMatrix<Rational>& rS = S;

	cout << rS.getSize() << ' ' << rS.getDeterminant() << ' ' << rS.getTrace() << endl;
	cout << (S = S) * (S + rS) << endl;
	cout << (S *= S) << endl;

	try {
		cout << rS.getInverse() << endl;
		cout << P.invert().getTransposed().getDeterminant() << endl;
		cout << P << endl;
	}
	catch (const MatrixIsDegenerateError&) {
		cout << "Cannot inverse matrix." << endl;
	}

	return 0;
}