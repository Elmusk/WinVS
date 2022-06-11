#ifndef _MYTINYSTL_ALGOBASE_H_
#define _MYTINYSTL_ALGOBASE_H_

#include <cstring>

#include "iterator.h"
#include "utils.h"

namespace mystl
{

#ifdef max
#pragma message("#undefing marco max")
#undef max
#endif

#ifdef min
#pragma message("#undefing marco min")
#undef min
#endif

/*
 max function: get bigger value between two values
*/
template<typename T>
const T& max(const T& lhs, const T& rhs)
{
    return lhs < rhs ? rhs : lhs;
}


/*
 min function: get smaller value between two values
*/
template<typename T>
const T& min(const T& lhs, const T& rhs)
{
    return rhs < lhs ? rhs : lhs;
}

/*****************************************************************************************/
// iter_swap
// 将两个迭代器所指对象对调
/*****************************************************************************************/
template <class FIter, class LIter>
void iter_swap(FIter lhs, LIter rhs)
{
    mystl::swap(lhs, rhs);
}



/*****************************************************************************************/
// copy
// 把 [first, last)区间内的元素拷贝到 [result, result + (last - first))内
/*****************************************************************************************/
// input_iterator_tag 版本
template<class InputIter, class OutputIter>
OutputIter unchecked_copy_cat(InputIter first, InputIter last, OutputIter result, mystl::input_iterator_tag)
{
    for (; first != last; ++first, ++result)
        *result = *first;
    return result;
}

// random_access_iterator_tag 版本
template<class RandomIter, class OutputIter>
OutputIter unchecked_copy_cat(RandomIter first, RandomIter last, OutputIter result, mystl::random_access_iterator_tag)
{
    for (auto n = last - first; n > 0; --n, ++first, ++result)
        *result = *first;
    return result;
}

template<class InputIter, class OutputIter>
OutputIter unchecked_copy(InputIter first, InputIter last, OutputIter result)
{
    return unchecked_copy_cat(first, last, result, iterator_category(first));
}

// 为 trivially_copy_assignable 类型提供特化版本
template<class Tp, class Up>
typename std::enable_if<std::is_same<typename std::remove_const<Tp>::type, Up>::value && std::is_trivially_copy_assignable<Up>::value, Up*>::type unchecked_copy(Tp* first, Tp* last, Up* result)
{
    const auto n = static_cast<size_t>(last - first);
    if (n != 0)
        std::memmove(result, first, n * sizeof(Up));
    return result + n;
}

template<class InputIter, class OutputIter>
OutputIter copy(InputIter first, InputIter last, OutputIter result)
{
    return unchecked_copy(first, last, result);
}

/*****************************************************************************************/
// copy_n
// 把 [first, first + n)区间上的元素拷贝到 [result, result + n)上
// 返回一个 pair 分别指向拷贝结束的尾部
/*****************************************************************************************/
template <class InputIter, class Size, class OutputIter>
mystl::pair<InputIter, OutputIter> unchecked_copy_n(InputIter first, Size n, OutputIter result, mystl::input_iterator_tag)
{
    for (; n > 0; --n, ++first, ++result)
    {
        *result = *first;
    }
    return mystl::pair<InputIter, OutputIter>(first, result);
}

template <class RandomIter, class Size, class OutputIter>
mystl::pair<RandomIter, OutputIter> unchecked_copy_n(RandomIter first, Size n, OutputIter result, mystl::random_access_iterator_tag)
{
    auto last = first + n;
    return mystl::pair<RandomIter, OutputIter>(last, mystl::copy(first, last, result));
}

template <class InputIter, class Size, class OutputIter>
mystl::pair<InputIter, OutputIter> copy_n(InputIter first, Size n, OutputIter result)
{
    return unchecked_copy_n(first, n, result, iterator_category(first));
}







} // namespace mystl
#endif
