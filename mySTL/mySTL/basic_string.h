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
//声明为静态方法可能是由于这些函数会被经常使用？
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
    //右值引用
    basic_string(basic_string&& rhs) noexcept
        : buffer_(rhs.buffer_), size_(rhs.size_), cap_(rhs.cap_)
    {
        rhs.buffer_ = nullptr;
        rhs.size_ = 0;
        rhs.cap_ = 0;
    }

    // 迭代器构造
    template<class Iter, typename std::enable_if<mystl::is_input_iterator<Iter>::value, int>::type = 0>
    basic_string(Iter first, Iter last)
    {
        copy_init(first, last, iterator_category(first));
    }

    // operators override 复制构造
    basic_string& operator=(const basic_string& rhs);
    basic_string& operator=(basic_string&& rhs) noexcept;

    basic_string& operator=(const_pointer str);
    basic_string& operator=(value_type ch);    

    ~basic_string() { destroy_buffer(); }

public:
    // iterator operations
    iterator               begin()         noexcept
    { return buffer_; }
    const_iterator         begin()   const noexcept
    { return buffer_; }
    iterator               end()           noexcept
    { return buffer_ + size_; }
    const_iterator         end()     const noexcept
    { return buffer_ + size_; }

    reverse_iterator       rbegin()        noexcept
    { return reverse_iterator(end()); }
    const_reverse_iterator rbegin()  const noexcept
    { return const_reverse_iterator(end()); }
    reverse_iterator       rend()          noexcept
    { return reverse_iterator(begin()); }
    const_reverse_iterator rend()    const noexcept
    { return const_reverse_iterator(begin()); }

    const_iterator         cbegin()  const noexcept
    { return begin(); }
    const_iterator         cend()    const noexcept
    { return end(); }
    const_reverse_iterator crbegin() const noexcept
    { return rbegin(); }
    const_reverse_iterator crend()   const noexcept
    { return rend(); }


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
    reference front()
    {
        MYSTL_DEBUG(!empty());
        return *begin();
    }
    const_reference front() const
    {
        MYSTL_DEBUG(!empty());
        return *begin();
    }

    reference back()
    {
        MYSTL_DEBUG(!empty());
        return *(end() - 1);
    }
    const_reference back() const
    {
        MYSTL_DEBUG(!empty());
        return *(end() - 1);
    }

    // 生成一个const char*指针，指向以空字符终止的数组。    
    const_pointer data() const noexcept { return to_raw_pointer(); }
    const_pointer c_str() const noexcept { return to_raw_pointer(); }

    // Operations of adding and removing
    
    // insert
    iterator insert(const_iterator pos, value_type ch);
    iterator insert(const_iterator pos, size_type count, value_type ch);

    template<class Iter>
    iterator insert(const_iterator pos, Iter first, Iter last);

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
        return append(str, pos, str.size_ - pos);
    }

    basic_string& append(const_pointer s)
    {
        return append(s, char_traits::length(s));
    }
    
    // if enable_if value = false, then can't pass compile
    template<class Iter, typename std::enable_if<mystl::is_input_iterator<Iter>::value, int>::type = 0>
    basic_string& append(Iter first, Iter last)
    {
        return append_range(first, last);
    }

    // erase / clear
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    
    //resize
    void resize(size_type count)
    {
        resize(count, value_type());
    }
    void resize(size_type count, value_type ch);
    
    // 将string大小变为0 
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

    template<class Iter>
    void copy_init(Iter first, Iter last, mystl::input_iterator_tag);

    template<class Iter>
    void copy_init(Iter first, Iter last, mystl::forward_iterator_tag);

    void destroy_buffer();

    // get raw pointer
    const_pointer to_raw_pointer() const;
    // shrink to fit
    void reinsert(size_type size);

    // append
    template<class Iter>
    basic_string& append_range(Iter first, Iter last);

    // compare, 加const是因为比较操作不涉及写，为保证安全，确保不会修改
    int compare_cstr(const_pointer s1, size_type n1, const_pointer s2, size_type n2) const;

    // reallocate
    void reallocate(size_type need);
    iterator reallocate_and_fill(iterator pos, size_type n, value_type ch);
    iterator reallocate_and_copy(iterator pos, const_iterator first, const_iterator last);

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
template<class CharType, class CharTraits>
void basic_string<CharType, CharTraits>::shrink_to_fit()
{
    if (size_ != cap_)
        reinsert(size_);
}

// 在pos处插入一个元素
template<class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::iterator basic_string<CharType, CharTraits>::insert(const_iterator pos, value_type ch)
{
    iterator r = const_cast<iterator>(pos);
    if (size_ == cap_)
        return reallocate_and_fill(r, 1, ch);
    char_traits::move(r+1, r, end() - r);
    ++size_;
    *r = ch;
    return r;
}

// 在pos处插入 count 个元素
template<class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::iterator basic_string<CharType, CharTraits>::insert(const_iterator pos, size_type count, value_type ch)
{
    iterator r = const_cast<iterator>(pos);
    if (count == 0)
        return r;
    if (size_ + count > cap_)
        return reallocate_and_fill(r, count, ch);
    if (pos == end())
    {
        char_traits::fill(end(), ch, count);
        size_ += count;
        return r;
    }
    char_traits::move(r + count, r, end() - r);
    char_traits::fill(r, ch, count);
    size_ += count;
    return r;
}

// 在 pos 处插入[first, last)内的元素
template<class CharType, class CharTraits>
template<class Iter>
typename basic_string<CharType, CharTraits>::iterator basic_string<CharType, CharTraits>::insert(const_iterator pos, Iter first, Iter last)
{
    iterator r = const_cast<iterator>(pos);
    const size_type len = mystl::distance(first, last);
    if (len == 0)
        return r;
    if (cap_ - size_ < len)
        return reallocate_and_copy(r, first, last);
    if (pos == end())
    {
        mystl::uninitialized_copy(first, last, end());
        size_ += len;
        return r;
    }
    char_traits::move(r+len, r, end()-r);
    mystl::uninitialized_copy(first, last, r);
    size_ += len;
    return r;
}


/***************************************/
// append 函数
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
    // 为什么不用 move 函数呢？
    // move函数一般用在重新分配了一个内存后，新内存和旧内存的复制
    // copy函数主要用在已存在的两个对象间的复制
    // char_traits::move(buffer_ + size_, str.buffer_ + pos, count);
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

// 删除 pos 处的元素
template<class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::iterator basic_string<CharType, CharTraits>::erase(const_iterator pos)
{
    MYSTL_DEBUG(pos != end());
    iterator r = const_cast<iterator>(pos);
    char_traits::move(r, r + 1, end() - pos - 1); // 为什么多减个1 ？？
    --size_;
    return r;
}

// 删除 [first, last) 的元素，注意是左闭右开的区间，明白后面移动的长度了吧！！
template<class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::iterator basic_string<CharType, CharTraits>::erase(const_iterator first, const_iterator last)
{
    if (first == begin() && last == end())
    {
        clear();
        return end();
    }
    const size_type n = mystl::distance(first, last);
    iterator r = const_cast<iterator>(first);
    char_traits::move(r, last, end() - last);
    size_ -= n;
    return r;
}

// 重置容器大小: 将容器size_修改为count，多的删除，少了添加。
template <class CharType, class CharTraits>
void basic_string<CharType, CharTraits>::resize(size_type count, value_type ch)
{
    if (count < size_)
    {
        erase(buffer_ + count, buffer_ + size_);
    }
    else
    {
        append(count - size_, ch);
        // char_traits::fill(buffer_ + size_, ch, count); 该语句无判断
    }
}

// compare functions
// 比较两个 basic_string, 小于返回-1，大于返回1，相等返回0
template<class CharType, class CharTraits>
int basic_string<CharType, CharTraits>::compare(const basic_string& other) const
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

// 从pos1下标开始的count1个字符与另一个basic_string从pos2下标开始的count2个字符比较 
template<class CharType, class CharTraits>
int basic_string<CharType, CharTraits>::compare(size_type pos1, size_type count1, const basic_string& other, size_type pos2, size_type count2) const
{
    auto n1 = mystl::min(count1, size_ - pos1);
    auto n2 = mystl::min(count2, other.size_ - pos2);
    // 源代码里没加 pos1 和 pos2 ？？
    return compare_cstr(buffer_ + pos1, n1, other.buffer_ + pos2, n2);
}

// 跟一个字符串比较
template<class CharType, class CharTraits>
int basic_string<CharType, CharTraits>::compare(const_pointer s) const
{
    return compare_cstr(buffer_, size_, s, char_traits::length(s));
}

// 从下标 pos1 开始的 count1 个字符跟另一个字符串比较
template <class CharType, class CharTraits> 
int basic_string<CharType, CharTraits>::compare(size_type pos, size_type count, const_pointer s) const
{
    auto n1 = mystl::min(count, size_ - pos);
    auto n2 = char_traits::length(s);
    return compare_cstr(buffer_ + pos, n1, s, n2)
}

// 从下标 pos1 开始的 count1 个字符跟另一个字符串的前 count2 个字符比较
template <class CharType, class CharTraits>
int basic_string<CharType, CharTraits>::compare(size_type pos1, size_type count1, const_pointer s, size_type count2) const
{
    auto n1 = mystl::min(count1, size_ - pos1);
    auto n2 = mystl::min(count2, char_traits::length(s));
    return compare_cstr(buffer_ + pos1, n1, s, n2);
}


/*******************************************************************/
// help function
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

// copy_init 函数
template<class CharType, class CharTraits>
template<class Iter>
void basic_string<CharType, CharTraits>::copy_init(Iter first, Iter last, mystl::input_iterator_tag)
{
    size_type n = mystl::distance(first, last);
    const auto init_size = mystl::max(static_cast<size_type>(STRING_INIT_SIZE), n+1);
    try
    {
        buffer_ = data_allocator::allocate(init_size);
        size_ = n;
        cap_ = init_size;
    }
    catch(...)
    {
        buffer_ = nullptr;
        size_ = 0;
        cap_ = 0;
        throw;
    }
    for (; n > 0; --n, ++first)
        append(*first);       // append函数使用了buffer_
}

template<class CharType, class CharTraits>
template<class Iter>
void basic_string<CharType, CharTraits>::copy_init(Iter first, Iter last, mystl::forward_iterator_tag)
{
    const size_type n = mystl::distance(first, last);
    const auto init_size = mystl::max(static_cast<size_type>(STRING_INIT_SIZE), n+1);
    try
    {
        buffer_ = data_allocator::allocate(init_size);
        size_ = n;
        cap_ = init_size;
        mystl::uninitialized_copy(first, last, buffer_);
    }
    catch(...)
    {
        buffer_ = nullptr;
        size_ = 0;
        cap_ = 0;
        throw;
    }
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

// to_raw_pointer
template<class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::const_pointer basic_string<CharType, CharTraits>::to_raw_pointer() const
{
    *(buffer_ + size_) = value_type();
    return buffer_;
}

// reinsert 函数
template<class CharType, class CharTraits>
void basic_string<CharType, CharTraits>::reinsert(size_type size)
{
    auto new_buffer = data_allocator::allocate(size);
    try
    {
        char_traits::move(new_buffer, buffer_, size);
    }
    catch(...)
    {
        data_allocator::deallocate(new_buffer);
    }
    buffer_ = new_buffer;
    size_ = size;
    cap_ = size;
}

// append_range，末尾追加一段 [first, last) 内的字符
template <class CharType, class CharTraits>
template <class Iter>
basic_string<CharType, CharTraits>& basic_string<CharType, CharTraits>::append_range(Iter first, Iter last)
{
    const size_type n = mystl::distance(first, last);
    THROW_LENGTH_ERROR_IF(size_ > max_size() - n, "basic_string<Char, Traits>'s size is too big");

    if (cap_ - size_ < n)
        reallocate(n);
    mystl::uninitialized_copy(first, last, buffer_ + size_); 
    // mystl::uninitialized_copy_n(first, n, buffer_ + size_);
    size_ += n;
    return *this;
}

// 比较两个字符串大小
template <class CharType, class CharTraits>
int basic_string<CharType, CharTraits>::compare_cstr(const_pointer s1, size_type n1, const_pointer s2, size_type n2)
{
    auto rlen = mystl::min(n1, n2);
    auto res = char_traits::compare(s1, s2, rlen)
    if (res != 0)
        return res;
    if (n1 > n2)
        return 1;
    if (n1 < n2)
        return -1;
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


// reallocate_and_fill函数
template<class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::iterator basic_string<CharType, CharTraits>::reallocate_and_fill(iterator pos, size_type n, value_type ch)
{
    const auto r = pos - buffer_;
    const auto old_cap = cap_;
    // 这里就涉及到重新分配内存的机制，是增加原来的一半，还是增加n ?
    const auto new_cap = mystl::max(old_cap + n, old_cap + (old_cap >> 1));
    auto new_buffer = data_allocator::allocate(new_cap);
    auto e1 = char_traits::move(new_buffer, buffer_, r) + r;
    auto e2 = char_traits::fill(e1, ch, n) + n;
    char_traits::move(e2, buffer_ + r, size_ - r);
    data_allocator::deallocate(buffer_, old_cap);
    buffer_ = new_buffer;
    size_ += n;
    cap_ = new_cap;
    return buffer_ + r;
}

// reallocate_and_copy 函数
template<class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::iterator basic_string<CharType, CharTraits>::reallocate_and_copy(iterator pos, const_iterator first, const_iterator last)
{
    const auto r = pos - buffer_;
    const auto old_cap = cap_;
    const size_type len = mystl::distance(first, last);
    const auto new_cap = mystl::max(old_cap + len, old_cap + (old_cap >> 1));
    auto new_buffer = data_allocator::allocate(new_cap);
    auto e1 = char_traits::move(new_buffer, buffer_, r) + r;
    auto e2 = mystl::uninitialized_copy(first, last, e1) + len;
    char_traits::move(e2, buffer_ + r, size_ - r);
    data_allocator::deallocate(buffer_, old_cap);
    size_ += len;
    buffer_ = new_buffer;
    cap_ = new_cap;
    return buffer_ + r;
}







}

#endif
