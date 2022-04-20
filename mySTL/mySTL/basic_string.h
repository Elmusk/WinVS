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

template<class CharType>
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

    static int compare(const char_type* s1, const char_type* s2, size_t n)
    {
        for (; n != 0; --n, ++s1, ++s2)
        {
            if (*s1 < *s2)
                return -1;
            if (*s1 > *s2)
                return 1;
        }
        return 0;
    }
    
    // 将已知string复制到新string 
    // doesn't exit overlap
    static char_type* copy(char_type* dst, const char_type* src, size_t n)
    {
        MYSTL_DEBUG(src + n <= dst || dst + n <= src);
        char_type* r = dst;
        for (; n != 0; --n, ++dst, ++src)
            *dst = *src;
        return r;
    }

    // 将旧内存中数据移动到重新分配的新内存中，新旧内存可能存在重叠部分？
    static char_type* move(char_type* dst, const char_type* src, size_t n)
    {
        char_type* r = dst;
        // 这是因为src与dst的内存段可能存在重叠部分，可能导致数据在复制的过程中被覆盖,这里分成两种情况
        if (dst < src)
        {
            for (; n != 0; --n, ++dst, ++src)
                *dst = *src;
        }
        else if (src < dst)
        {
            dst += n;
            src += n;
            for (; n!= 0; --n)
                *--dst = *--src;
        }
        return r;
    }

    static char_type* fill(char_type* dst, char_type ch, size_t count)
    {
        char_type* r = dst;
        for (; count > 0; --count, ++dst)
            *dst = ch;
        return r;
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
	static_assert(std::is_same<CharType, typename traits_type::char_type>::value, "CharType must be same as traits_type::char_type");

public:
    static constexpr size_type npos = static_cast<size_type>(-1);

private:
    iterator buffer_;
    size_type size_;
    size_type cap_;

public:
    basic_string() noexcept { try_init(); }

    basic_string(size_type n, value_type ch)
        : buffer_(nullptr), size_(0), cap_(0)
    { 
        fill_init(n, ch); 
    }

    basic_string(const basic_string& other, size_type pos)
        : buffer_(nullptr), size_(0), cap_(0)
    {
        init_from(other.buffer_, pos, other.size_ - pos);
    }

    basic_string(const basic_string& other, size_type pos, size_type count)
        : buffer_(nullptr), size_(0), cap_(0)
    {
        init_from(other.buffer_, pos, count);
    }

    basic_string(const_pointor str)
        : buffer_(nullptr), size_(0), cap_(0)
    {
        init_from(str, 0, char_traits::length(str));
    }
    
    basic_string(const_pointor str, size_type count)
        : buffer_(nullptr), size_(0), cap_(0)
    {
        init_from(str, 0, count);
    }

    basic_string(const basic_string& lhs)
        : buffer_(nullptr), size_(0), cap_(0)
    {
        init_from(lhs.buffer_, 0, lhs.size_);
    }

    basic_string(basic_string&& rhs) noexcept
        : buffer_(rhs.buffer_), size_(rhs.size_), cap_(rhs.cap_)
    {
        rhs.buffer_ = nullptr;
        rhs.size_ = 0;
        rhs.cap_ = 0;
    }

    // 迭代器构造
    
    // operators override

    
    ~basic_string() { destroy_buffer(); }

public:
    // iterator operations
    
    // capacity operations
    bool empty() const noexcept { return size_ == 0; }

    size_type size() const noexcept { return size_; }
    size_type length() const noexcept { return size_; }
    
    size_type capacity() const noexcept { return cap_; }

    size_type max_size() const noexcept { return static_cast<size_type>(-1); }

    void reserve(size_type n);
    void shrink_to_fit();

    // operations to access to the data contained in th %string
    reference operator[](size_type n)
    {
        MYSTL_DEBUG(n <= size_);
        if (n == size_)
            *(buffer_ + n) = value_type();  // null
        return *(buffer_ + n); 
    }
    
    const_reference operator[](size_type n) const
    {
        MYSTL_DEBUG(n <= size_);
        if (n == size_)
            *(buffer_ + n) = value_type();
        return *(buffer_ + n);
    }

    reference at(size_type n)
    {
        THROW_OUT_OF_RANGE_IF(n >= size_, "basic_string<Char, Traits>::at() subscript out of range");
        return (*this)[n];
    }

    const_reference at(size_type n) const
    {
        THROW_OUT_OF_RANGE_IF(n >= size_, "basic_string<Char, Traits>::at() subscript out of range");
        return (*this)[n];
    }

    // front(), back(), data(), c_str()

    // Operations of adding and removing
    
    // insert

    // push_back or pop_back
    void push_back(value_type ch) { append(1, ch); }
    void pop_back()
    {
        MYSTL_DEBUG(!empty());
        --size_;
    }

    // append
    basic_string& append(size_type count, value_type ch);
    basic_string& append(const basic_string& str, size_type pos, size_type count);
    basic_string& append(const_pointer s, size_type count);
    
    basic_string& append(const basic_string& str)
    {
        return append(str, 0, str.size_);
    }
    basic_string& append(const basic_string& str, size_type pos)
    {
        return append(str, pos str.size_ - pos);
    }
    basic_string& append(const_pointer s)
    {
        return append(s, char_traits::length(s));
    }

    // erase / clear
    
    //resize
    
    
    void clear() noexcept { size_ = 0; }

    // basic_string 的操作
    int compare(const basic_string& other) const;
    int compare(size_type pos1, size_type count1, const basic_string& other) const;
    int compare(size_type pos1, size_type count1, const basic_string& other, size_type pos2, size_type count2=npos) const;
    int compare(const_pointer s) const;
    int compare(size_type pos1, size_type count1, const_pointer s) const;
    int compare(size_type pos1, size_type count1, const_pointer s, size_type count2) const;


private:
    // help functions
    void try_init() noexcept;
    void fill_init(size_type n, value_type ch);
    
    void init_from(const_pointer src, size_type pos, size_type n);

    void destroy_buffer();

    // compare
    int compare_cstr(const_pointer s1, size_type n1, const_pointer s2, size_type n2) const;

    // reallocate
    void reallocate(size_type need);
};

// Reserve storage space
// 预留存储空间
template<class CharType, class CharTraits>
void basic_string<CharType, CharTraits>::reserve(size_type n)
{
    if (cap_ < n)
    {
        THROW_LENGTH_ERROR_IF(n > max_size(), "n cann't larger than max_size() in basic_string<Char, Traits>::reserve(n)");
        auto new_buffer = data_allocator::allocate(n);
        char_traits::move(new_buffer, buffer_, size_);
        data_allocator::deallocate(buffer_);
        buffer_ = new_buffer;
        cap_ = n;
    }
}

// 减少不用的空间


// 尝试初始化一段 buffer 
template<class CharType, class CharTraits>
void basic_string<CharType, CharTraits>::try_init() noexcept
{
    try
    {
        buffer_ = data_allocator::allocate(static_cast<size_type>(STRING_INIT_SIZE));
        size_ = 0;
        cap_ = 0;
    }
    catch (...)
    {
        buffer_ = nullptr;
        size_ = 0;
        cap_ = 0;
    }
}

// @breif   construct string using a char
template<class CharType, class CharTraits>
void basic_string<CharType, CharTraits>::fill_init(size_type n, value_type ch)
{
    const auto init_size = mystl::max(static_cast<size_type>(STRING_INIT_SIZE), n + 1);
    buffer_ = data_allocator::allocate(init_size);
    char_traits::fill(buffer_, ch, n);
    size_ = n;
    cap_ = init_size;
}

// @brief   construct string as copy of a substring
template<class CharType, class CharTraits>
void basic_string<CharType, CharTraits>::init_from(const_pointer src, size_type pos, size_type count)
{
    const auto init_size = mystl::max(static_cast<size_type>(STRING_INIT_SIZE), n + 1);
    buffer_ = data_allocator::allocate(init_size);
    char_traits::copy(buffer_, src + pos, count);
    size_ = count;
    cap_ = init_size;
}

// destroy_buffer  
template<class CharType, class CharTraits>
void basic_string<CharType, CharTraits>::destroy_buffer()
{
    if (buffer_ != nullptr)
    {
        data_allocator::deallocate(buffer_, cap_);
        buffer_ = nullptr;
        size_ = 0;
        cap_ = 0;
    }
}

// 在末尾添加 count 个 ch
template<class CharType, class CharTraits>
basic_string<CharType, CharTraits>& basic_string<CharType, CharTraits>::append(size_type count, value_type ch)
{
    THROW_LENGTH_ERROR_IF(size_ > max_size() - count, "basic_string<Char, Traits>'s size too big")
    if (cap_ - size_ < count)
        reallocate(count);
    char_traits::fill(buffer_ + size_, ch, count);
    size_ += count;
    return *this;
}

// 在末尾添加 [str[pos], str[pos+count]] 一段
template<class CharType, class CharTraits>
basic_string<CharType, CharTraits>& basic_string<CharType, CharTraits>::append(const basic_string& str, size_type pos, size_type count)
{
    THROW_LENGTH_ERROR_IF(size_ > max_size() - count, "basic_string<Char, Traits>'s size too big")
    if (count == 0)
        return *this;
    if (cap_ - size_ < count)
        reallocate(count);
    char_traits::copy(buffer_ + size_, str.buffer_ + pos, count);
    size_ += count;
    return *this;
}

// 在末尾添加 [s, s+count] 一段
template<class CharType, class CharTraits>
basic_string<CharType, CharTraits>& basic_string<CharType, CharTraits>::append(const_pointer s, size_type count)
{
    THROW_LENGTH_ERROR_IF(size_ > max_size() - count, "basic_string<Char, Traits>'s size too big")
    if (cap_ - size_ < count)
        reallocate(count);
    char_traits::copy(buffer_ + size_, s, count);
    size_ += count;
    return *this;
}

// compare functions
// 比较两个 basic_string, 小于返回-1，大于返回1，相等返回0
template<class CharType, class CharTraits>
int basic_string<CharType, CharTraits>::compare(const basic_string& other)
{
    return compare_cstr(buffer_, size_, other.buffer_, other.size_);
}
// 从pos1下标开始，与other比较count1个字符
template<class CharType, class CharTraits>
int basic_string<CharType, CharTraits>::compare(size_type pos1, size_type count1, const basic_string& other) const
{
    auto n1 = mystl::min(count1, size_ - pos1);
    return compare_cstr(buffer_ + pos1, n1, other.buffer_, other.size_);
}
// 
template<class CharType, class CharTraits>
int basic_string<CharType, CharTraits>::compare(size_type pos1, size_type count1, const basic_string& other, size_type pos2, size_type count2) const


// compare_cstr function
template<class CharType, class CharTraits>
int basic_string<CharType, CharTraits>::compare_cstr(const_pointer s1, size_type n1, const_pointer s2, size_type n2) const
{
    auto rlen = mystl::min(n1, n2);
    auto res = char_traits::compare(s1, s2, rlen);
    if (res != 0) return res;
    if (n1 < n2) return -1;
    if (n1 > n2) return 1;
    return 0;
}


// reallocate function()
// strategy: 新容量为原容量加上原容量的一半
template<class CharType, class CharTraits>
void basic_string<CharType, CharTraits>::reallocate(size_type need)
{
    const auto new_cap = mystl::max(cap_ + need, cap_ + (cap_ >> 1));
    auto new_buffer = data_allocator::allocate(new_cap);
    char_traits::move(new_buffer, buffer_, size_);
    data_allocator::deallocate(buffer_);
    buffer_ = new_buffer;
    cap_ = new_cap;
}



}

#endif
