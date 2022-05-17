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











} // namespace mystl
#endif
