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

    try {
        matrix::Matrix<double> matrix{size, nums.begin(), nums.end()};
        std::cout << std::setprecision(std::numeric_limits<double>::max_digits10) << matrix.GetDeterminant() << std::endl;
    }
    catch (std::logic_error &logic_ex) {
        std::cout << "Logic error: " << std::endl << logic_ex.what() << std::endl;
    }
    catch (std::range_error &range_ex) {
        std::cout << "Range error: " << std::endl << range_ex.what() << std::endl;
    }
    catch (std::runtime_error &rt_ex) {
        std::cout << "Runtime error: " << std::endl << rt_ex.what() << std::endl;
    }
    catch (std::bad_alloc &bad_alloc_ex) {
        std::cout << "Bad alloc error: " << std::endl << bad_alloc_ex.what() << std::endl;
    }
    catch (std::exception &ex)
    {
        std::cout << "Exception: " << std::endl << ex.what() << std::endl;
    }

    return 0;
}