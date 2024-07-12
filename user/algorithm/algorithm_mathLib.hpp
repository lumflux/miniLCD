#ifndef _ALGORITHM_MATHLIB_HPP
#define _ALGORITHM_MATHLIB_HPP
namespace Algorithm
{
    namespace MathLib
    {
        template <typename T>
        inline T const &Min(T const &a, T const &b)
        {
            return a < b ? a : b;
        }

        template <typename T>
        inline T const &Max(T const &a, T const &b)
        {
            return a < b ? b : a;
        }

        template <typename T>
        inline T const Abs(T const a)
        {
            return a >= 0 ? a : -a;
        }

    } // namespace MathLib
} // namespace Algorithm

#endif // _ALGORITHM_MATHLIB_HPP
