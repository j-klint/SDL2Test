#pragma once

#include <initializer_list>
#include <array>
#include <iostream>
#include <iomanip>
#include <string>
#include <SDL.h>
#include <type_traits>

#ifndef NDEBUG
//#include <cassert>
#include <stdexcept>
#endif

template<size_t n, size_t m, typename TElem = int>
class Matrix
{
public:
	constexpr Matrix() {}
	explicit constexpr Matrix(const std::initializer_list<const std::initializer_list<TElem> > L);
	explicit constexpr Matrix(const std::initializer_list<TElem> elements);
	template<typename T2> explicit constexpr Matrix(const Matrix<n, m, T2>& src);
	explicit constexpr Matrix(const std::array<TElem, n* m>& elements);

	constexpr size_t rows() const { return n; }
	constexpr size_t height() const { return n; }
	constexpr size_t columns() const { return m; }
	constexpr size_t width() const { return m; }

	TElem* operator[](size_t i);
	constexpr TElem const* operator[](size_t i) const;

	constexpr auto operator+(const Matrix& other) const;
	auto& operator+=(const Matrix& other);
	constexpr auto operator-(const Matrix& other) const;
	auto& operator-=(const Matrix& other);
	constexpr auto operator-() const;

	template<size_t p> constexpr auto operator*(const Matrix<m, p, TElem>& other) const;
	auto& operator*=(const Matrix& other);
	auto& operator*=(const TElem& scalar);
	constexpr auto operator*(const TElem& scalar) const;
	friend constexpr auto operator*(const TElem& scalar, const Matrix& matrix);

	constexpr auto T() const;

	constexpr bool operator==(const Matrix& r) const;
	constexpr bool operator!=(const Matrix& r) const;

	constexpr SDL_FPoint operator()(const SDL_FPoint&) const;

private:
	TElem data[n][m]{};
	friend class Matrix;
#ifndef NDEBUG
	constexpr void BoundsCheck(size_t i, size_t j) const;
#endif
};

template<typename To, size_t n, size_t m, typename From>
constexpr Matrix<n, m, To> Matrix_cast(const Matrix<n, m, From>& M);

template<size_t n, size_t m, typename T>
std::ostream& operator<<(std::ostream& out, const Matrix<n, m, T>& M);

template<size_t n, size_t m>
std::ostream& operator<<(std::ostream& out, const Matrix<n, m, int>& M);





// Here start the definitions:

#ifndef NDEBUG
template<size_t n, size_t m, typename TElem>
inline constexpr void Matrix<n, m, TElem>::BoundsCheck(size_t i, size_t j) const
{
	//assert(0 <= i && i < n);
	//assert(0 <= j && j < m);
	if ( i < 0 || i >= n || j < 0 || j >= n )
		throw std::out_of_range("Array access with naughty indexes\n");
}
#endif

template<size_t n, size_t m, typename TElem>
inline constexpr Matrix<n, m, TElem>::Matrix(const std::initializer_list<const std::initializer_list<TElem>> L)
{
#ifndef NDEBUG
	if ( L.size() != n )
		throw std::invalid_argument("Wrong number of rows in a matrix initializer\n");
#endif

	int i{ 0 }, j{ 0 };
	for ( const auto row : L )
	{
#ifndef NDEBUG
		if ( row.size() != m )
			throw std::invalid_argument("Wrong lenght of row in a matrix initializer\n");
#endif
		if ( i == n ) break;
		for ( const auto& x : row )
		{
			if ( i == n ) break;
			data[i][j++] = x;
			if ( j == m )
			{
				j = 0;
				++i;
			}
		}
	}
}

template<size_t n, size_t m, typename TElem>
inline constexpr Matrix<n, m, TElem>::Matrix(const std::initializer_list<TElem> elements)
{
#ifndef NDEBUG
	if ( elements.size() < n*m )
		throw std::invalid_argument("Not enough elements in matrix initializer\n");
#endif

	int i{ 0 }, j{ 0 };
	for ( const auto& x : elements )
	{
		if ( i == n ) break;
		data[i][j++] = x;
		if ( j == m )
		{
			j = 0;
			++i;
		}
	}
}

template<size_t n, size_t m, typename TElem>
inline constexpr Matrix<n, m, TElem>::Matrix(const std::array<TElem, n*m>& elements)
{
	int i{ 0 }, j{ 0 };
	for ( const auto& x : elements )
	{
		if ( i == n ) break;
		data[i][j++] = x;
		if ( j == m )
		{
			++i;
			j = 0;
		}
	}
}

template<size_t n, size_t m, typename TElem>
template<typename T2>
inline constexpr Matrix<n, m, TElem>::Matrix(const Matrix<n, m, T2>& src)
{
	for ( int i = 0; i < n; i++ )
		for ( size_t j = 0; j < m; j++ )
			data[i][j] = static_cast<TElem>(src[i][j]);
}

template<size_t n, size_t m, typename TElem>
TElem* Matrix<n, m, TElem>::operator[](size_t i)
{
#ifndef NDEBUG
	BoundsCheck(i, 0);
#endif
	return data[i];
}

template<size_t n, size_t m, typename TElem>
constexpr TElem const* Matrix<n, m, TElem>::operator[](size_t i) const
{
#ifndef NDEBUG
	BoundsCheck(i, 0);
#endif
	return data[i];
}

template<size_t n, size_t m, typename TElem>
constexpr auto Matrix<n, m, TElem>::operator+(const Matrix& other) const
{
	Matrix<n, m, TElem> sum{};
	for ( size_t i = 0; i < n; i++ )
		for ( size_t j = 0; j < m; j++ )
			sum.data[i][j] = data[i][j] + other[i][j];
	return sum;
}

template<size_t n, size_t m, typename TElem>
auto& Matrix<n, m, TElem>::operator+=(const Matrix& other)
{
	for ( size_t i = 0; i < n; i++ )
		for ( size_t j = 0; j < m; j++ )
			data[i][j] += other[i][j];
	return *this;
}

template<size_t n, size_t m, typename TElem>
constexpr auto Matrix<n, m, TElem>::operator-(const Matrix& other) const
{
	Matrix<n, m, TElem> diff{};
	for ( size_t i = 0; i < n; i++ )
		for ( size_t j = 0; j < m; j++ )
			diff.data[i][j] = data[i][j] - other[i][j];
	return diff;
}

template<size_t n, size_t m, typename TElem>
auto& Matrix<n, m, TElem>::operator-=(const Matrix& other)
{
	for ( size_t i = 0; i < n; i++ )
		for ( size_t j = 0; j < m; j++ )
			data[i][j] -= other[i][j];
}

template<size_t n, size_t m, typename TElem>
constexpr auto Matrix<n, m, TElem>::operator-() const
{
	Matrix<n, m, TElem> neg{};
	for ( size_t i = 0; i < n; i++ )
		for ( size_t j = 0; j < m; j++ )
			neg.data[i][j] = -data[i][j];
	return neg;
}

template<size_t n, size_t m, typename TElem>
template<size_t p>
constexpr auto Matrix<n, m, TElem>::operator*(const Matrix<m, p, TElem>& other) const
{
	Matrix<n, p, TElem> C{};
	for ( size_t i = 0; i < n; i++ )
		for ( size_t j = 0; j < p; j++ )
			for ( size_t k = 0; k < m; k++ )
				C.data[i][j] += data[i][k] * other[k][j];
	return C;
}

template<size_t n, size_t m, typename TElem>
auto& Matrix<n, m, TElem>::operator*=(const Matrix& other)
{
	static_assert(n == m, "operator*= only for square matrixes");
	Matrix<n, n, TElem> copy = *this;
	Matrix<n, n, TElem>& rhs = (this == &other) ? copy : other;
	for ( size_t i = 0; i < n; i++ )
		for ( size_t j = 0; j < n; j++ )
		{
			data[i][j] = TElem{};
			for ( size_t k = 0; k < n; k++ )
				data[i][j] += copy[i][k] * rhs[k][j];
		}
	return *this;
}

template<size_t n, size_t m, typename TElem>
auto& Matrix<n, m, TElem>::operator*=(const TElem& scalar)
{
	for ( size_t i = 0; i < n; i++ )
		for ( size_t j = 0; j < m; j++ )
			data[i][j] *= scalar;
	return *this;
}

template<size_t n, size_t m, typename TElem>
constexpr auto Matrix<n, m, TElem>::operator*(const TElem& scalar) const
{
	Matrix<n, m, TElem> result{};
	for ( size_t i = 0; i < n; i++ )
		for ( size_t j = 0; j < m; j++ )
			result.data[i][j] = data[i][j] * scalar;
	return result;
}

template<size_t n, size_t m, typename TElem>
constexpr auto operator*(const TElem& scalar, const Matrix<n, m, TElem>& matrix)
{
	return matrix * scalar;
}

template<size_t n, size_t m, typename TElem>
constexpr auto Matrix<n, m, TElem>::T() const
{
	Matrix<m, n, TElem> transpose{};
	for ( size_t i = 0; i < n; i++ )
		for ( size_t j = 0; j < m; j++ )
			transpose.data[j][i] = data[i][j];
	return transpose;
}

template<size_t n, size_t m, typename TElem>
constexpr bool Matrix<n, m, TElem>::operator==(const Matrix& r) const
{
	for ( size_t i = 0; i < n; i++ )
		for ( size_t j = 0; j < m; j++ )
			if ( data[i][j] != r[i][j] ) return false;
	return true;
}

template<size_t n, size_t m, typename TElem>
constexpr bool Matrix<n, m, TElem>::operator!=(const Matrix& r) const
{
	return !(*this == r);
}

template<size_t n, size_t m, typename TElem>
constexpr SDL_FPoint Matrix<n, m, TElem>::operator()(const SDL_FPoint& pt) const
{
	static_assert(n == 3 && m == 3);
	static_assert(std::is_floating_point_v<TElem>);
	auto temp = (*this) * Matrix<3, 1, TElem>{pt.x, pt.y, 1};
	return { static_cast<float>(temp[0][0]), static_cast<float>(temp[1][0]) };
}



// Non-members

template<typename To, size_t n, size_t m, typename From>
constexpr Matrix<n, m, To> Matrix_cast(const Matrix<n, m, From>& M)
{
	return Matrix<n, m, To>{M};
}

template<size_t n, size_t m, typename T>
std::ostream& operator<<(std::ostream& out, const Matrix<n, m, T>& M)
{
	size_t space{ 76 / m - 1 };
	std::string word;
	//out << std::right;

	out << "\n";
	for ( size_t i = 0; i < n; i++ )
	{
		for ( size_t j = 0; j < m; j++ )
		{

			word = std::to_string(M[i][j]);
			if ( word.size() > space ) word.resize(space);
			if ( word.back() == '.' )
			{
				word.resize(word.size() - 1);
				word.insert(word.begin(), ' ');
			}
			out << (j == 0 ? "[ " : " ") << word;
		}
		out << " ]\n";
	}
	//out << "\n";
	return out;
}

template<size_t n, size_t m>
std::ostream& operator<<(std::ostream& out, const Matrix<n, m, int>& M)
{
	size_t maxWidth[m]{};

	auto LengthOfInt = [](int x)
	{
		size_t count = x <= 0 ? 1 : 0;
		for ( ; x != 0; x /= 10 ) ++count;
		return count;
	};

	for ( size_t j = 0; j < m; j++ )
		for ( size_t i = 0; i < n; i++ )
			if ( size_t len{ LengthOfInt(M[i][j]) }; len > maxWidth[j] )
				maxWidth[j] = len;

	out << "\n";
	for ( size_t i = 0; i < n; i++ )
	{
		for ( size_t j = 0; j < m; j++ )
		{
			out << (j == 0 ? "[ " : " ") << std::setw(maxWidth[j]) << M[i][j];
		}
		out << " ]\n";
	}
	//out << "\n";
	return out;
}
