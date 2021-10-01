#include <cstring>
#include <math.h>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <algorithm>

class BigIntegerOverflow {
public:
	BigIntegerOverflow() {}
};

void EquateStrings(uint32_t* dest_str, const uint32_t* src_str, size_t beg_idx, int lngth) {
	size_t src_size = lngth;
	for (size_t index = beg_idx; index < src_size; ++index) {
		dest_str[index] = src_str[index];
	}
}

class BigInteger {
private:
	const static size_t maxLength = 3000; // number of digits
	const static uint8_t baseLog = 10; // use 1 or 2 for debug

	typedef uint32_t digit_type;
	typedef uint64_t double_digit_type; // for multiplication

	//const static uint32_t base = static_cast<digit_type>(pow(10, baseLog));  // base of numeral system
	const static size_t maxReadSize = 20010; // for reading (buffer size)
	const static uint32_t base = 10;

	digit_type digits[maxLength];

	size_t size_;
	bool is_negative_;

	digit_type operator[](size_t idx) const {
		return digits[idx];
	}

	digit_type& operator[](size_t idx) {
		return digits[idx];
	}

	static BigInteger SumPositives(const BigInteger& lhs, const BigInteger& rhs) {
		BigInteger sum;
		sum.is_negative_ = false;
		digit_type transfer = 0;
		size_t size = std::max(lhs.size_, rhs.size_);
		for (size_t i = 0; i < size; ++i) {
			digit_type new_digit = lhs[i] + rhs[i] + transfer;
			sum[i] = new_digit % base;
			transfer = new_digit / base;
		}
		sum[size] = transfer;
		sum.size_ = (transfer > 0) ? size + 1 : size;
		if (sum.size_ >= maxLength) {
			throw BigIntegerOverflow();
		}
		BigInteger::SetEndOfNum(sum.digits, sum.size_);
		return sum;
	}

	static BigInteger SubtractPositives(const BigInteger& lhs, const BigInteger& rhs) {
		BigInteger first;
		BigInteger second;
		if (abs(lhs) >= abs(rhs)) {
			first = lhs;
			second = rhs;
		}
		else {
			first = rhs;
			second = lhs;
		}
		if (abs(lhs) == abs(rhs)) {
			return BigInteger(0);
		}
		bool value = false;
		int diff = 0;
		for (size_t i = 0; i < second.size_ || value; i++) {
			int num = first.digits[i];
			num -= value;
			if (i < second.size_) {
				num -= second.digits[i];
			}
			value = num < 0;
			if (value) {
				num += 10;
			}
			first.digits[i] = num;
		}
		size_t counter = 0;
		while (first.digits[first.size_ - 1 - counter] == 0) {
			++counter;
		}
		first.size_ = first.size_ - counter;
		if (first.size_ >= maxLength) {
			throw BigIntegerOverflow();
		}
		BigInteger::SetEndOfNum(first.digits, first.size_);
		return first;
	}

public:

	static void SetEndOfNum(digit_type* num, size_t idx) {
		for (size_t i = idx; i < 100; ++i) {
			num[i] = 0;
		}
	}

	static void SetEndOfString(char* str, size_t idx) {
		str[idx] = '\0';
	}

	friend BigInteger abs(const BigInteger& value) {
		BigInteger result(value);
		result.is_negative_ = false;
		return result;
	}

	BigInteger(int number) {
		int number_copy = number;
		is_negative_ = false;
		if (number_copy < 0) {
			is_negative_ = true;
			number_copy *= -1;
		}

		int number_length = 0;
		while (number != 0) {
			++number_length;
			number /= base;
		}

		if (number_copy == 0) {
			number_length = 1;
		}

		size_ = number_length;
		int i = 0;
		do {
			digits[i] = number_copy % base;
			number_copy /= base;
			++i;
		} while (number_copy != 0);
		SetEndOfNum(digits, number_length);
	}

	BigInteger(long long number) {
		long long number_copy = number;
		is_negative_ = false;
		if (number_copy < 0) {
			is_negative_ = true;
			number_copy *= -1;
		}

		int number_length = 0;
		while (number != 0) {
			++number_length;
			number /= base;
		}

		if (number_copy == 0) {
			number_length = 1;
		}

		size_ = number_length;
		int i = 0;
		do {
			digits[i] = number_copy % base;
			number_copy /= base;
			++i;
		} while (number_copy != 0);
		SetEndOfNum(digits, number_length);
	}

	BigInteger(unsigned int number) {
		unsigned int number_copy = number;
		is_negative_ = false;

		int number_length = 0;
		while (number != 0) {
			++number_length;
			number /= base;
		}

		if (number_copy == 0) {
			number_length = 1;
		}

		size_ = number_length;
		int i = 0;
		do {
			digits[i] = number_copy % base;
			number_copy /= base;
			++i;
		} while (number_copy != 0);
		SetEndOfNum(digits, number_length);
	}

	BigInteger(unsigned long long number) {
		unsigned long long number_copy = number;
		is_negative_ = false;

		int number_length = 0;
		while (number != 0) {
			++number_length;
			number /= base;
		}

		if (number_copy == 0) {
			number_length = 1;
		}

		size_ = number_length;
		int i = 0;
		do {
			digits[i] = number_copy % base;
			number_copy /= base;
			++i;
		} while (number_copy != 0);
		SetEndOfNum(digits, number_length);
	}

	BigInteger(const BigInteger& other) {
		size_ = other.size_;
		EquateStrings(digits, other.digits, 0, other.size_);
		SetEndOfNum(digits, size_);
		is_negative_ = other.is_negative_;
	}

	BigInteger() {
		SetEndOfNum(digits, 0);
		is_negative_ = false;
		size_ = 0;
	}

	BigInteger(const char* number_str) {
		size_t str_size = strlen(number_str);
		if (str_size == 2 && number_str[1] == 0 && number_str[0] == '-') {
			is_negative_ = false;
			digits[0] = 0;
			SetEndOfNum(digits, 1);
			size_ = 1;
		}
		else if (number_str[0] != '-') {
			is_negative_ = false;
			for (size_t i = str_size - 1; i > 0; --i) {
				digits[str_size - 1 - i] = (number_str[i] - '0') % base;
			}
			digits[str_size - 1] = (number_str[0] - '0') % base;
			SetEndOfNum(digits, str_size);
			size_ = str_size;
		}
		else {
			is_negative_ = true;
			for (size_t i = str_size - 1; i > 0; --i) {
				digits[str_size - 1 - i] = (number_str[i] - '0') % base;
			}
			SetEndOfNum(digits, str_size - 1);
			size_ = str_size - 1;
		}
	}

	~BigInteger() {}

	friend bool HaveSameSign(const BigInteger& first, const BigInteger& second) {
		return first.is_negative_ == second.is_negative_;
	}

	BigInteger& operator=(const BigInteger& value) {
		if (this != &value) {
			EquateStrings(digits, value.digits, 0, value.size_);
			SetEndOfNum(digits, value.size_);
			is_negative_ = value.is_negative_;
			size_ = value.size_;
		}
		return *this;
	}

	friend BigInteger operator*(const BigInteger &lhs, const BigInteger &rhs) {
		BigInteger result;
		result.is_negative_ = (lhs.is_negative_ && !rhs.is_negative_) || (!lhs.is_negative_ && rhs.is_negative_);
		result.size_ = lhs.size_ + rhs.size_;
		for (size_t i = 0; i < lhs.size_; ++i) {
			for (size_t j = 0; j < rhs.size_; ++j) {
				result[i + j] += lhs.digits[i] * rhs.digits[j];
			}
		}

		for (size_t i = 0; i < lhs.size_ + rhs.size_; ++i) {
			result[i + 1] += result[i] / base;
			result.digits[i] %= base;
		}

		result.digits[lhs.size_ + rhs.size_] %= base;
		size_t counter = 0;
		while (result.size_ - 1 - counter > 0 && result.digits[result.size_ - 1 - counter] == 0) {
			++counter;
		}
		result.size_ = result.size_ - counter;
		if (result.size_ >= maxLength) {
			throw BigIntegerOverflow();
		}
		if (result.size_ == 1 && result.digits[0] == 0) {
			result.is_negative_ = false;
		}
		return result;
	}

	BigInteger& operator*=(const BigInteger& other) {
		*this = *this * other;
		return *this;
	}

	bool operator==(const BigInteger &other) const {
		if ((!is_negative_ && other.is_negative_) || (is_negative_ && !other.is_negative_)) {
			return false;
		}
		if (other.size_ != size_) {
			return false;
		}
		for (int i = size_ - 1; i >= 0; i--) {
			if (digits[i] != other.digits[i]) {
				return false;
			}
		}
		return true;
	}

	bool operator!=(const BigInteger &other) const {
		return !(*this == other);
	}

	bool operator<(const BigInteger &other) const {
		if (!is_negative_ && other.is_negative_) {
			return false;
		}
		if (is_negative_ && !other.is_negative_) {
			return true;
		}
		if (size_ > other.size_) {
			if (!is_negative_) {
				return false;
			}
			else {
				return true;
			}
		}
		if (size_ < other.size_) {
			if (is_negative_) {
				return false;
			}
			else {
				return true;
			}
		}
		int module = 0;
		bool is_less = false;
		bool is_more = false;
		for (int i = size_ - 1; i >= 0; i--) {
			if (digits[i] < other.digits[i]) {
				is_less = true;
				break;
			}
			if (digits[i] > other.digits[i]) {
				is_more = true;
				break;
			}
		}
		if (is_less && !is_negative_) {
			return true;
		}
		if (is_more && is_negative_) {
			return true;
		}
		return false;
	}

	bool operator>(const BigInteger &other) const {
		return (*this != other) && !(*this < other);
	}

	bool operator<=(const BigInteger &other) const {
		return (*this == other) || (*this < other);
	}

	bool operator>=(const BigInteger &other) const {
		return (*this == other) || (*this > other);
	}

	friend BigInteger operator+(const BigInteger& lhs, const BigInteger& rhs) {
		BigInteger result;
		if (HaveSameSign(lhs, rhs)) {
			result = BigInteger::SumPositives(lhs, rhs);
			result.is_negative_ = lhs.is_negative_;
		}
		else {
			result = BigInteger::SubtractPositives(rhs, lhs);
		}
		return result;
	}

	BigInteger& operator+=(const BigInteger& other) {
		*this = *this + other;
		return *this;
	}

	BigInteger operator-() const {
		BigInteger other = *this;
		if (other.digits[0] == 0 && other.size_ == 1) {
			other.is_negative_ = false;
		}
		else {
			other.is_negative_ = !other.is_negative_;
		}
		return other;
	}

	friend BigInteger operator-(const BigInteger& lhs, const BigInteger& rhs) {
		return BigInteger(lhs + (-rhs));
	}

	BigInteger& operator-=(const BigInteger& other) {
		*this = *this - other;
		return *this;
	}

	BigInteger& operator++() {
		*this += BigInteger(1);
		return *this;
	}

	BigInteger& operator--() {
		*this -= BigInteger(1);
		return *this;
	}

	BigInteger operator++(int) {
		BigInteger result(*this);
		++(*this);
		return result;
	}

	BigInteger operator--(int) {
		BigInteger result(*this);
		--(*this);
		return result;
	}

	char* ToString() const {
		char* str = new char[maxLength + 1];
		str[0] = '\n';
		size_t first_idx = 0;
		if (is_negative_) {
			str[0] = '-';
			first_idx = 1;
		}
		for (size_t i = first_idx; i < size_; ++i) {
			str[i] += digits[i] + '0';
		}
		SetEndOfString(str, size_);
		if (size_ == 0 || (str[0] == '-' && size_ == 2)) {
			str[0] = '0';
		}
		return str;
	}

	friend std::ostream& operator<<(std::ostream &stream, const BigInteger& value) {
		if (value.is_negative_) {
			stream << '-';
		}
		for (int i = value.size_ - 1; i >= 0; --i) {
			stream << value.digits[i];
		}
		return stream;
	}

	friend std::istream& operator>>(std::istream &stream, BigInteger& value) {
		char str[maxLength];
		std::cin.getline(str, maxLength);
		value = BigInteger(str);
		return stream;
	}
};

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);
	BigInteger a, b;
	std::cin >> a >> b;
	a *= b;
	std::cout << a;
}
