/*
    SPDX-License-Identifier: MPL-2.0
    --------------------------------
    This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
    If a copy of the MPL was not distributed with this file,
    You can obtain one at https://mozilla.org/MPL/2.0/.

    Provided “as is”, without warranty of any kind.

    Copyright © 2025 Alar Akilbekov. All rights reserved.
 */

#include <optional> // optional, nullopt

#include "../LinkedList/LinkedList.tpp"

using linkedlist::LinkedList;

// LIFO
// only one end named as "top"
template <typename T>
class Stack {
    // push / offer
    // element / peek
    // pop / poll
private:
    LinkedList<T> list;

public:
    // --- APPEND ---
    T& push(T& lvalue){
        return list.addFirst(lvalue); // can throw error
    }
    T& push(T&& rvalue){
        return list.addFirst(std::move(rvalue)); // can throw error
    }

    bool offer(T& lvalue){
        try{
            return list.addFirst(lvalue);
            return true;
        } catch(...){
            return false;
        }
    }
    bool offer(T&& rvalue){
        try{
            return list.addFirst(std::move(rvalue));
            return true;
        } catch(...){
            return false;
        }
    }
    // ------

    // --- GET ---
    T& element(){
        return list.get(0); // throws error if list is empty
    }

    // nullptr if list is empty
    std::optional<T> peek(){ // std::optional creates a copy, so it is more optimized to use element with try-catch
        try{
            return list.getFirst(); // returns T&
        } catch(...){
            return std::nullopt;
        }
    }
    // ------

    // --- REMOVE ---
    T pop(){
        return list.removeFirst(); // RVO
    }

    std::optional<T> poll(){ // here though std::optional moves data because removeFirst returns T which is like rvalue
        try{
            return list.removeFirst(); // returns T
        } catch(...){
            return std::nullopt;
        }
    }
    // ------

    int size(){ return list.size(); }
    int empty(){ return list.size() == 0; }
};
