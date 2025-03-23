/*
    SPDX-License-Identifier: MPL-2.0
    --------------------------------
    This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
    If a copy of the MPL was not distributed with this file,
    You can obtain one at https://mozilla.org/MPL/2.0/.

    Provided “as is”, without warranty of any kind.

    Copyright © 2025 Alar Akilbekov. All rights reserved.
 */

#pragma once
#ifndef _ARRAY_LIST_TPP_
#define _ARRAY_LIST_TPP_

#include <iostream>
#include <iterator> // random_access_iterator_tag, reverse_iterator
#include <cstddef> // ptrdiff_t

namespace arraylist {

/* --- iterator ---
    From https://github.com/alarxx/Tensor-library

    Main article:
        https://www.internalpointers.com/post/writing-custom-iterators-modern-cpp

    iterator_tags:
        https://en.cppreference.com/w/cpp/iterator/iterator_tags
        - Input (read-only)         input_iterator_tag
        - Output (write-only)       output_iterator_tag
        - Forward Iterator          forward_iterator_tag
        - Bidirectional Iterator    bidirectional_iterator_tag
        - Random Access Iterator    random_access_iterator_tag
        - Contiguous Iterator       contiguous_iterator_tag

    The properties of each iterator category are:
        https://cplusplus.com/reference/iterator/

    - iterator
    - constant_iterator - это по сути Input Iterator (read-only)
    - reverse_iterator
    - constant_reverse_iterator

    --- Output Iterator ---
    Мы здесь не реализуем чистый Output Iterator.
    Мы же dereferencing делаем так *_ptr,
    а при Ouput Iterator мы делаем dereferencing так *this,
    то есть это фиктивное разыменование, it returns itself,
    то есть возвращаем сам iterator, чтобы потом воспользовать assign operator =:
    *it = 42;

        struct Output {
            T* _ptr;
            Output& operator * (){ return *this; }
            Output& operator = (const T& data){
                *_ptr = data;
            }
        };
    ------
*/
template <typename T>
class iterator {
public:
    // https://en.cppreference.com/w/cpp/iterator/iterator_traits
    // https://en.cppreference.com/w/cpp/iterator/iterator_tags
    using iterator_category = std::random_access_iterator_tag;
    // https://en.cppreference.com/w/cpp/types/ptrdiff_t
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T*; // value_type*
    using reference = T&; // value_type&
private:
    pointer _ptr;
public:
    iterator(pointer ptr) : _ptr(ptr) {}

    reference operator * (){ return *_ptr; }
    pointer operator -> (){ return _ptr; }

    // prefix increment
    iterator& operator ++ (){ _ptr++; return *this; }
    // postfix increment
    iterator operator ++ (int){
        iterator tmp = *this; // overhead on creation of temporary object
        ++(*this);
        return tmp;
    }

    // prefix decrement
    iterator& operator -- (){ _ptr--; return *this; }
    // postfix decrement
    iterator operator -- (int){
        iterator tmp = *this; // overhead on creation of temporary object
        --(*this);
        return tmp;
    }

    iterator operator + (difference_type n){ return iterator(_ptr + n); }
    iterator operator - (difference_type n){ return iterator(_ptr - n); }

    difference_type operator - (const iterator& other){ return _ptr - other._ptr; }

    reference operator [] (difference_type index){ return _ptr[index]; }

    friend bool operator == (const iterator& a, const iterator& b){ return a._ptr == b._ptr; }
    friend bool operator != (const iterator& a, const iterator& b){ return a._ptr != b._ptr; }
    friend bool operator <  (const iterator& a, const iterator& b){ return a._ptr <  b._ptr; }
    friend bool operator >  (const iterator& a, const iterator& b){ return a._ptr >  b._ptr; }
    friend bool operator <= (const iterator& a, const iterator& b){ return a._ptr <= b._ptr; }
    friend bool operator >= (const iterator& a, const iterator& b){ return a._ptr >= b._ptr; }
};

// --- ArrayList in Java / <vector> in C++ ---
template <typename T>
class ArrayList {
private:
    // Actually, it would be better to initialize these constants outside class
    static constexpr double INCREASE_FACTOR = 1.5;
    static constexpr int INITIAL_CAPACITY = 5;

    T * _coeffs;
    int _size;
    int _capacity;

public:
    explicit ArrayList(int capacity = INITIAL_CAPACITY) : _size(0), _capacity(capacity) {
        _coeffs = new T[capacity];
    }

    ~ArrayList(){
        delete[] _coeffs;
        _coeffs = nullptr;
    }

    int size() const { return _size; }

    T& operator [] (const int index) /*const*/ {
        return _coeffs[index];
    }

    T get(const int index) const {
        return _coeffs[index];
    }

    void set(T item, int index){
        _coeffs[index] = item;
    }

    // append, not insert which needs shifting all elements
    void add(const T item){
        if(_size == _capacity){
            _increaseBuffer();
        }
        _coeffs[_size++] = item; // post increment
    }
private:
    void _increaseBuffer(){
        std::cout << "_increaseBuffer function" << std::endl;
        _capacity = (int) (INCREASE_FACTOR * _capacity);
        T * new_coeffs = new T[_capacity];
        for(int i = 0; i < _size; i++){
            new_coeffs[i] = _coeffs[i];
        }
        delete[] _coeffs; _coeffs = nullptr;
        _coeffs = new_coeffs;
    }

public:
    // --- iterator ---

    using iterator = arraylist::iterator<T>; // iterator variable shadowing, so we use :: - global namespace.
    using reverse_iterator = std::reverse_iterator<iterator>;
    // using constant_iterator
    // using constant_reverse_iterator

    iterator begin(){ return iterator(_coeffs); }
    iterator end(){ return iterator(&_coeffs[_size]); }
    reverse_iterator rbegin(){ return reverse_iterator(end()); }
    reverse_iterator rend(){ return reverse_iterator(begin()); }
};

} // namespace ArrayList

#endif
