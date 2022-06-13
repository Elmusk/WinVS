
#ifndef _MYTINYSTL_UTILS_H_
#define _MYTINYSTL_UTILS_H_

// 包含一些通用工具

#include <cstddef>

#include "type_traits.h"

namespace mystl
{

// move
template<typename T>
typename std::remove_reference<T>::type&& move(T&& arg) noexcept
{
    return static_cast<typename std::remove_reference<T>::type&&>(arg);
}

// forward
template<typename T>
T&& forward(typename std::remove_reference<T>::type& arg) noexcept
{
    return static_cast<T&&>(arg);
}

template<typename T>
T&& forward(typename std::remove_reference<T>::type&& arg) noexcept
{
    static_assert(!std::is_lvalue_reference<T>::value, "bad forward");
    return static_cast<T&&>(arg);
}

// swap：交换两个值
// 输入参数是具体的值
template<class Tp>
void swap(Tp& lhs, Tp& rhs)
{
    auto tmp(mystl::move(lhs));
    lhs = mystl::move(rhs);
    rhs = mystl::move(tmp);
}

// swap_range
// 这里没有做判断？？
template<class ForwardIter1, class ForwardIter2>
ForwardIter2 swap_range(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2)
{
    for (; first1 != last1; ++first1, (void) ++first2)
        mystl::swap(*first, *first2);
    return first2;

// swap: 
// Tp(&a)[N]: N 个 const T 类型的对象的数组, 其中 a 为这个数组的引用，即为数组的首地址，
template<class Tp, size_t N>
void swap(Tp (&a)[N], Tp (&b)[N])
{
    mystl::swap_range(a, a+N, b);
}


// --------------------------------------------------------------------------------------
// pair
// 结构体模板 : pair
// 两个模板参数分别表示两个数据的类型
// 用 first 和 second 来分别取出第一个数据和第二个数据
template<class Ty1, class Ty2>
struct pair
{
    typedef Ty1 first_type;
    typedef Ty2 second_type;

    first_type first;
    second_type second;

    // default constructiable
    // 涉及到默认模板参数：在函数中可能不会用到的模板，因此不用写名称
    // 只有但满足条件的时候才会调用该构造函数，否则会报错？？？
    template<class Other1=Ty1, class Other2=Ty2, typename = typename std::enable_if<std::is_default_constructible<Other1>::value && std::is_default_constructible<Other2>::value, void>::type>
    constexpr pair() : first(), second() { }


}


}

#endif // !MYTINYSTL_UTIL_H_
