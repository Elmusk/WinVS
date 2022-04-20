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

} // namespace mystl
#endif
