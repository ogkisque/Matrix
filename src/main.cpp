#include <iostream>
#include <cassert>
#include <iomanip>
#include <limits>
#include <vector>

#include "matrix.hpp"

namespace
{
    bool GetInput(size_t &size, std::vector<double> &nums)
    {
        std::cin >> size;
        if (!std::cin.good() || size <= 0)
        {
            std::cout << "Incorrect data" << std::endl;
            return false;
        }

        size_t num_elems = size * size;
        nums.reserve(num_elems);
        for (size_t i = 0; i < num_elems; i++)
        {
            double x = 0;
            std::cin >> x;
            nums.push_back(x);
            if (!std::cin.good())
            {
                std::cout << "Incorrect data" << std::endl;
                return false;
            }
        }

        return true;
    }
} // namespace


int main()
{
    size_t size = 0;
    std::vector<double> nums;
    if (!GetInput(size, nums))
        return 1;

    matrix::Matrix<double> matrix{size, nums.begin(), nums.end()};

    std::cout << std::setprecision(std::numeric_limits<double>::max_digits10) << matrix.GetDeterminant() << std::endl;

    return 0;
}