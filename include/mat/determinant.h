#ifndef MAT_DETERMINANT_H
#define MAT_DETERMINANT_H


#include <array>
#include <tuple>


namespace mat::detail
{
    constexpr auto factorial(int n)
    {
        int v = n;
        while (n > 1) {
            v *= --n;
        }
        return v;
    }

    template <std::size_t N, int i, std::size_t M>
    constexpr void permutation_index_impl(std::array<int, N> arr,
                                        std::integral_constant<int, i>,
                                        std::array<std::array<int, N>, M> & indices,
                                        int & cnt)
    {
        if constexpr (i == N - 1) {
            indices[cnt++] = arr;
        } else {
            int j = i + 1;
            for (int k = N - i; k > 0; --k) {
                permutation_index_impl(arr, std::integral_constant<int, i + 1>{}, indices, cnt);
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
                if (j + 1 == N) {
                    j = i;
                } else {
                    ++j;
                }
            }
        }
    }

    template <int N>
    constexpr auto permutation_index()
    {
        static_assert(N > 0);

        using arr_t = std::array<int, N>;

        arr_t arr{};
        for (int i = 0; i < arr.size(); ++i) {
            arr[i] = i;
        }

        constexpr auto numOfPerm = factorial(N);
        std::array<arr_t, numOfPerm> indices{};
        int cnt = 0;

        permutation_index_impl(arr, std::integral_constant<int, 0>{}, indices, cnt);

        return indices;
    }

    template <typename T, std::size_t N, std::size_t M>
    constexpr auto term_sign(std::array<std::array<T, N>, M> const & perm)
    {
        std::array<int, M> signs{};

        for (int i = 0; i < M; ++i) {
            int reverseOrderCnt = 0;
            auto & p = perm[i];
            for (int j = 0; j < N - 1; ++j) {
                for (int k = j + 1; k < N; ++k) {
                    if (p[j] > p[k]) {
                        ++reverseOrderCnt;
                    }
                }
            }
            signs[i] = (reverseOrderCnt % 2 == 0) ? 1 : -1;
        }

        return signs;
    }

    template <int MatrixSize>
    struct det_impl
    {
        static_assert(MatrixSize >= 2);

        constexpr static auto indices_ = permutation_index<MatrixSize>();
        constexpr static auto signs_ = term_sign(indices_);

        template <typename T>
        struct multiply
        {
            template <typename U, U... m, typename... Int>
            constexpr auto mul_impl(std::integer_sequence<U, m...>, Int... n) const
            {
                return (sign_ * ... * mat_[m][n]);
            }

            template <typename... Int>
            constexpr auto operator () (Int... n) const
            {
                return mul_impl(std::index_sequence_for<Int...>{}, n...);
            }

            T & mat_;
            int sign_;
        };

        template <typename T>
        struct add
        {
            template <typename U, U... i, typename... Permutation>
            constexpr auto add_impl(std::integer_sequence<U, i...>, Permutation &... p) const
            {
                return (... + std::apply(multiply<T>{ mat_, signs_[i] }, p));
            }

            template <typename... Permutation>
            constexpr auto operator () (Permutation &... p) const
            {
                return add_impl(std::index_sequence_for<Permutation...>{}, p...);
            }

            T & mat_;
        };

        template <typename T>
        constexpr auto operator () (T & mat) const
        {
            return std::apply(add<T>{ mat }, indices_);
        }
    };
} // namespace mat::detail


namespace mat
{
    template <typename T, int M>
    constexpr auto det(T (& mat)[M][M])
    {
        return detail::det_impl<M>{}(mat);
    }

    template <typename T, std::size_t M>
    constexpr auto det(std::array<std::array<T, M>, M> const & mat)
    {
        return detail::det_impl<M>{}(mat);
    }

    /*
    * NOTE: The most generic version.
    *        It will work OK only if the mat's size is M x M.
    *        And the mat should provide 'operator []' for accessing the elements.
    */
    template <int M, typename T>
    constexpr auto det(T & mat)
    {
        return detail::det_impl<M>{}(mat);
    }
} // namespace mat


#endif // MAT_DETERMINANT_H
