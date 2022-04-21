  1 #ifndef _MYTINYSTL_ITERATOR_H_
  2 #define _MYTINYSTL_ITERATOR_H_
  3 
  4 #include <cstddef>
  5 
  6 #include "type_traits.h"
  7 
  8 namespace mystl
  9 {
 10 
 11 // 五种迭代器类型
 12 
 13 
 14 template<class Category, class T, class Distance=ptrdiff_t,     class Pointer=T*, class Reference=T&>
 15 struct iterator
 16 {
 17     typedef Category iterator_category;
 18     using value_type = T;
 19     typedef Pointer pointer;
 20     using reference = Reference;
 21     typedef Distance difference_type;
 22 };
 23 
 24 // iterator traits
 25 
 26 template<class T>
 27 struct has_iterator_cat
 28 {
 29 private:
 30     struct two { char a; char b; }
