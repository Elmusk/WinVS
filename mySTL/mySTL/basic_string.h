#pragma once

#ifndef _MYTINYSTL_BASIC_STRING_H_
#define _MYTINYSTL_BASIC_STRING_H_

#include <iostream>

#include "iterator.h"
#include "memory.h"
#include "functional.h"
#include "exceptdef.h"

namespace mystl
{

template<typename CharType>
struct char_traits
{
	using char_type = CharType;

	// 
	static size_t length(const char_type* str)
	{
		size_t len = 0;
		for (; *str != char_type(0); ++str)
			++len;
		return len;
	}
};

#define STRING_INIT_SIZE 32

template<class CharType, class CharTraits = mystl::char_traits<CharType>>
class basic_string
{
public:
	using traits_type = CharTraits;
	using char_traits = CharTraits;

	using allocator_type = mystl::allocator<CharType>;
	using data_allocator = mystl::allocator<CharType>;

	using value_type = typename allocator_type::value_type;
	using pointer = typename allocator_type::value_type;
	using const_pointor = typename allocator_type::const_pointor;
	using reference = typename allocator_type::reference;
	using const_reference = typename allocator_type::const_reference;
	using size_type = typename allocator_type::size_type;
	using difference_type = typename allocator_type::difference_type;

	using iterator = value_type*;
	using const_iterator = const value_type*;
	using reverse_iterator = mystl::reverse_iterator<iterator>;
	using const_reverse_iterator = mystl::reverse_iterator<const_iterator>;

	allocator_type get_allocator() { return allocator_type(); }

	static_assert(std::is_pod<CharType>::value, "Character type of basic_string must be a POD!");


	
};

}

#endif
