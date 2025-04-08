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
#ifndef _BST_TPP_
#define _BST_TPP_

#include <iostream>
#include <stdexcept> // runtime_error, out_of_range
#include <type_traits>
#include <concepts>
#include <functional> // std::hash
#include <cstddef> // std::size_t
#include <algorithm> // swap, max
#include <new> // bad_alloc

#include <vector>
#include <stack>
#include <queue>
#include <set>
#include <unordered_set>
#include <iomanip> // std::setw

template <typename T>
concept Comparable = requires(T a, T b){
    {a >  b} -> std::convertible_to<bool>;
    {a <  b} -> std::convertible_to<bool>;
    {a <= b} -> std::convertible_to<bool>;
    {a >= b} -> std::convertible_to<bool>;
    {a == b} -> std::convertible_to<bool>;
    {a != b} -> std::convertible_to<bool>;
};

template <Comparable K, typename V>
class BST {

public:
    template <Comparable K_, typename V_>
    class Node {
    public:
        K_ key;
        V_ value;
        Node<K_, V_> *left, *right;
        /*
        What does `Object& &&` mean?
        Object &  &  = Object &
        Object &  && = Object &
        Object && &  = Object &
        Object && && = Object &&
        */
        Node(const K_ & key, const V_ & value) : key(key), value(value) {} // 1 copy
        Node(const K_ && key, const V_ && value) = delete;
        ~Node(){
            std::cout << "Node Destructor: key = " << key << ", value = " << value << std::endl;
        }
    };

private:
    Node<K, V> * root;

public:
    BST() : root(nullptr) {}


    ~BST() {
        std::stack<Node<K, V>*> stack;
        stack.push(root);
        while(!stack.empty()){
            Node<K, V> * node = stack.top();
            stack.pop();
            if(node->left != nullptr){
                stack.push(node->left);
            }
            if(node->right != nullptr){
                stack.push(node->right);
            }
            delete node;
        }
    }


    // --- Insert ---
    Node<K, V>& insert(const K && key, const V && value){
        return insert(key, value); // передаем как lvalue, потому что key и value - имена переменных
    }
    Node<K, V>& insert(const K & key, const V & value){
        Node<K, V> * putty = new Node<K, V>(key, value);
        if(root == nullptr){
            root = putty;
            return *putty;
        }
        else {
            // traverse until nullptr
            Node<K, V> * node = root;
            while(true){
                if(putty->key < node->key){
                    if(node->left == nullptr){
                        node->left = putty;
                        return *putty;
                    }
                    node = node->left;
                }
                else {
                    if(node->right == nullptr){
                        node->right = putty;
                        return *putty;
                    }
                    node = node->right;
                }
            }
        }
    }


    // --- Get ---
    Node<K, V>& get(const K && key){ return get(key); }
    Node<K, V>& get(const K & key){
        if(root == nullptr){
            throw std::out_of_range("Error: BST is empty!");
        }
        // traverse until key == node.key
        Node<K, V> * node = root;
        int count = 0;
        while(node != nullptr && node->key != key){
            if(key < node->key){
                node = node->left;
            }
            else {
                node = node->right;
            }
            count++;
        }
        std::cout << "get() count: " << count << std::endl;
        if(node == nullptr){
            throw std::out_of_range("Error: Key doesn't exist!");
        }
        return *node;
    }


    // --- Printing ---
    // print tree? Как можно визуализировать дерево?
    // Просто способ это рекурсивно вывести горизонтально, сложнее будет вывести вертикально

    // Horizontal Print
    private: void hprint(Node<K, V> * node, int spacing){
        if(node == nullptr){ return; }
        hprint(node->right, spacing + 1);
        /*
            iomanip - IO Manipulations
                В основном я использую тут std::setw(amount)

                std::cout.width(5); // можно так сделать
                std::cout << "BST" << std::endl; // Output: "  BST"

                // std::cout.fill('-'); // Можно так сделать
                std::cout << std::setfill('-') << std::setw(7) << std::quoted("BST") << std::setw(7) << "BST" << std::endl;
                // Output: "--"BST"----BST"

                // base: 8, 10, 16
                std::cout << std::setbase(8) << 8  << std::endl; // 10
                std::cout << std::setbase(8) << 16 << std::endl; // 20
                std::cout << std::setbase(8) << 64 << std::endl; // 100

        */
        // 4 - примерно сколько символов занимает вывод value
        std::cout << std::setw(spacing * 4) << node->value << std::endl;
        hprint(node->left, spacing + 1);
    }
    // default
    public: void hprint(){ hprint(root, 0); }

    // Vertical Print
    void vprint(){
        /*
            height: 3, 2^3=8
            |(----)(----)(----)(---1)|(----)(----)(----)(----)
            |(----)(---2)(----)(----)|(----)(---3)(----)(----)
            |(---4)(----)(---5)(----)|(---6)(----)(---7)(----)

            height: 4, 2^4=16
            |(----)(----)(----)(----)(----)(----)(----)(---1)(----)(----)(----)(----)(----)(----)(----)(----) - 2^0=1, 16/1=16, 16/2=8, 2^4=16
            |(----)(----)(----)(---2)(----)(----)(----)(----)(----)(----)(----)(---3)(----)(----)(----)(----) - 2^1=2, 16/2=8,  8/2=4,  2^3=8
            |(----)(---4)(----)(----)(----)(---5)(----)(----)(----)(---6)(----)(----)(----)(---7)(----)(----) - 2^2=4, 16/4=4,  4/2=2,  2^2=4
            |(---8)(----)(---9)(----)(--10)(----)(--11)(----)(--12)(----)(--13)(----)(--14)(----)(--15)(----) - 2^3=8, 16/8=2,  2/2=1,  2^1=2
        */
        std::queue<Node<K, V>*> queue;
        queue.push(root);

        int height = get_height();
        // int max_width = (1 << height); // 2^height: 2^0=1, 2^1=2, 2^2=4, 2^3=8, 2^4=16
        int symbols = 2;
        // std::cout << std::setfill('-');

        for(int level = 0; level < height; ++level){
            int space = (1 << (height - level)) * symbols;
            int n = (1 << level); // 1 2 4 8 16 ...
            for(int i = 0; i < n; ++i){
                if(i == 0){
                    std::cout << std::setw(space / 2);
                }
                else {
                    std::cout << std::setw(space);
                }
                Node<K, V> * node = queue.front();
                queue.pop();
                if(node != nullptr){
                    std::cout << node->value;
                    queue.push(node->left);
                    queue.push(node->right);
                }
                else {
                    std::cout << "";
                    queue.push(nullptr); // left
                    queue.push(nullptr); // right
                }
            }
            std::cout << std::endl;
        }
    }

    // --- Get Height ---
    int get_height(){ return get_height(root); }
    int get_height(Node<K, V> * root){
        if(root == nullptr){ return 0; }
        return 1 + std::max(get_height(root->left), get_height(root->right));
    }


    // --- Inorder Traversal ---
    // Inorder Traversal in increasing order (Left - Root - Right), правильный traversal доказывает правильность дерево?
    std::vector<Node<K, V>*> inorder_traversal() const {
        std::vector<Node<K, V>*> nodes;
        // traversal
        std::stack<Node<K, V>*> stack;
        // std::set<K> has_been;
        Node<K, V> * node = root; // copy of pointer

        while(node != nullptr || !stack.empty()){
            // get to most left and memorize all intermediate nodes
            while(node != nullptr){
                stack.push(node);
                node = node->left;
            }
            // node = nullptr

            // get last left
            node = stack.top();
            stack.pop();
            nodes.push_back(node); // copy of pointer
            std::cout << node->value << std::endl;

            // get right
            node = node->right;
            // if(node->right != nullptr){ stack.push(node->right); }
        }

        return nodes;
    }


    // --- Get Min - most left ---
    V& get_min(){
        if(root == nullptr){
            throw std::out_of_range("Error: BST is empty!");
        }
        Node<K, V> * node = root; // copy of pointer
        // Go left until left is null, i.e. there is no left
        while(node->left != nullptr){
            node = node->left;
        }
        return node->value;
    }

    // --- Get Max - most right ---
    V& get_max(){
        if(root == nullptr){
            throw std::out_of_range("Error: BST is empty!");
        }
        Node<K, V> * node = root; // copy of pointer
        // Go right until right is null, i.e. there is no right
        while(node->right != nullptr){
            node = node->right;
        }
        return node->value;
    }


    // --- Delete ---
    Node<K, V> remove(const K& key){
        if(root == nullptr){
            throw std::out_of_range("Error: BST is empty!");
        }

        // Find `node` and it's `parent`
        // traverse until `key == node.key`
        Node<K, V> * parent;
        Node<K, V> * node = root;
        while(node != nullptr && node->key != key){
            parent = node;
            if(key < parent->key){
                node = parent->left;
            }
            else {
                node = parent->right;
            }
        }

        // Not Found
        if(node == nullptr){
            throw std::out_of_range("Error: Key doesn't exist!");
        }

        // root == null => parent == null
        // parent == null => root = replacement

        // replace with 1 right and then most left
        Node<K, V> * replacement = node;
        if(node->right == nullptr){
            replacement = node->left; // node->left or nullptr
        }
        else {
            replacement = node->right; // not nullptr

            if(replacement->left == nullptr){
                replacement->left = node->left;
            }
            else {
                while(replacement->left != nullptr){
                    Node<K, V> * prev = replacement;

                    replacement = replacement->left; // not nullptr

                    if(replacement->left == nullptr){
                        prev->left = replacement->right; // replacement->right or nullptr

                        replacement->right = nullptr;
                        // replacement->left = nullptr; // already nullptr

                        replacement->left = node->left;
                        replacement->right = node->right;
                        node->left = nullptr;
                        node->right = nullptr;
                        // break; // no need, it will break
                    }
                }
            }
        }

        if(key < parent->key){
            parent->left = replacement;
        }
        else {
            parent->right = replacement;
        }

        Node<K, V> d = std::move(*node); // d is stealed Node in Stack Memory, so it will automatically freed
        delete node;

        return d; // RVO
    }

};

#endif
