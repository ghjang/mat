#ifndef MAT_FACTORIAL_H
#define MAT_FACTORIAL_H


namespace mat
{
    constexpr auto factorial(int n)
    {
        int v = n ? n : 1;
        while (n > 1) {
            v *= --n;
        }
        return v;
    }
} // namespace mat


#endif // MAT_FACTORIAL_H
