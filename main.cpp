#include <iostream>
#include <iomanip>
#include "vers.h"
#include "matrix.h"

using namespace std::literals::string_literals;
using Matrix_t = otus_hw6::Matrix<int, 0>;

std::ostream& print_matrix(std::ostream& os, Matrix_t const& m, Matrix_t::coord_t const& LT, Matrix_t::coord_t const& BR);

int main(int, char **) 
{
	using namespace otus_hw6;
	constexpr const int N = 10;
	try
	{
		Matrix_t m;
		for(int i = 0; i < N; ++i)
		{
			m[i][i] = i;
			m[i][N - i - 1] = N - i - 1;
		}
		print_matrix(std::cout, m, Matrix_t::coord_t{0, 0}, Matrix_t::coord_t{9, 9});
		std::cout << m. size() << std::endl;
		for( auto cell: m )
		{
			for( auto coord : cell.coord_)
				std::cout << coord << " ";
			std::cout << cell.val_ << std::endl;
		}
	}	
	catch(const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}

std::ostream& print_matrix(std::ostream& os, Matrix_t const& m, Matrix_t::coord_t const& LT, Matrix_t::coord_t const& BR)
{
	for( size_t i = LT[0]; i <= BR[0]; ++i )
	{
		for(size_t j = LT[1]; j <= BR[1]; ++j)
			os << m[i][j] << " ";
		os << std::endl;
	}
	return os;
}
