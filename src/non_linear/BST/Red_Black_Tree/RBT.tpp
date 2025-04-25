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
#ifndef _RBT_TPP_
#define _RBT_TPP_

/*

https://www.cs.usfca.edu/~galles/visualization/RedBlack.html

*/

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
class RBT {

public:
    template <Comparable K_, typename V_>
    class Node {
    public:
        K_ key;
        V_ value;
        bool is_black;
        Node<K_, V_> *left, *right;
        /*
        What does `Object& &&` mean?
        Object &  &  = Object &
        Object &  && = Object &
        Object && &  = Object &
        Object && && = Object &&
        */
        Node(const K_ & key, const V_ & value, bool is_black = false /*red by default*/) : key(key), value(value), is_black(is_black), left(nullptr), right(nullptr) {} // 1 copy
        Node(const K_ && key, const V_ && value) = delete;
        ~Node(){
            // std::cout << "Node Destructor: key = " << key << ", value = " << value << std::endl;
        }
    };

private:
    Node<K, V> * root;

public:
    RBT() : root(nullptr) {}

    ~RBT() {
        if(root == nullptr){ return; }
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

public:
    bool is_red(Node<K, V> * node) const {
        return node != nullptr && !node->is_black;
    }


    void flip_colors(Node<K, V> * grandparent) {
        grandparent->is_black = false; // !grandparent->is_black;
        if(grandparent->left  != nullptr) grandparent->left->is_black  = true; // !grandparent->left->is_black;
        if(grandparent->right != nullptr) grandparent->right->is_black = true; // !grandparent->right->is_black;
    }

public:
    // --- Insert ---
    Node<K, V>& insert(const K && key, const V && value, bool print = false){
        return insert(key, value, print); // передаем как lvalue, потому что key и value - имена переменных
    }

    Node<K, V>& insert(const K & key, const V & value, bool print = false){
        if(root == nullptr){
            root = new Node<K, V>(key, value, true);
            return *root;
        }

        // traverse until nullptr
        Node<K, V> * node = root;

        std::vector<Node<K, V>*> path;
        // stack.reserve(2 * log2(N + 1)); // log is hard operation to calculate

        // O(log(N))
        while(true){
            path.push_back(node);

            if(key == node->key){
                node->value = value;
                return *node;
            }

            if(key < node->key){
                if(node->left == nullptr){
                    node->left = new Node<K, V>(key, value, false); // red
                    // return *(node->left);
                    node = node->left;
                    break;
                }
                node = node->left;
            }
            else { // key > node->right
                if(node->right == nullptr){
                    node->right = new Node<K, V>(key, value, false); // red
                    // return *(node->right);
                    node = node->right;
                    break;
                }
                node = node->right;
            }
        }

        path.push_back(node);

        if(path.size() < 3){ return *node; }

        // O(log(N))
        // path снизу вверх к root-у
        Node<K, V> * grandgrandparent   = nullptr;
        Node<K, V> * grandparent        = nullptr;
        Node<K, V> * parent             = nullptr;
        Node<K, V> * child              = nullptr; // actually could be node

        // child is red
        // if(!is_red(parent)) return *node;
        if(print) { vprint(); std::cout << "---" << std::endl; }

        while(path.size() >= 4){
            grandgrandparent    = path[path.size() - 4];
            grandparent         = path[path.size() - 3];
            parent              = path[path.size() - 2];
            child               = path[path.size() - 1];

            path.pop_back();

            if(grandparent->key < grandgrandparent->key){ // child is parent->left
                grandgrandparent->left  = balance(grandparent, parent, child);
                if(print) { vprint(); std::cout << "---" << std::endl; }
                if(!is_red(grandgrandparent->left)){
                    /*
                        Вращения возможны только единожды, после этого баланс восстановлен.
                        Push redness up / blackness down, возможно множество раз и они не меняют структуру дерева,
                        поэтому мы можем подниматься по path.
                        НО, если мы сделали вращение, то структура изменится и идти наверх по path не получится,
                        потому что parent-child relationship нарушится.
                    */
                    return *node;
                }
            }
            else { // child is parent->right
                grandgrandparent->right = balance(grandparent, parent, child);
                if(print) { vprint(); std::cout << "---" << std::endl; }
                if(!is_red(grandgrandparent->right)){
                    return *node;
                }
            }
        }
        std::cout << "root" << std::endl;
        parent      = path[path.size() - 2];
        child       = path[path.size() - 1];
        root = balance(root, parent, child);
        root->is_black = true;

        if(print) { vprint(); std::cout << "---" << std::endl; }

        return *node;
    }

    Node<K, V> * balance(Node<K, V> * grandparent, Node<K, V> * parent, Node<K, V> * child){
        // return grandparent;

        std::cout << "grandparent: "    << grandparent->value   << (grandparent->is_black   ? "B" : "R")  << std::endl;
        std::cout << "parent: "         << parent->value        << (parent->is_black        ? "B" : "R")  << std::endl;
        std::cout << "child: "          << child->value         << (child->is_black         ? "B" : "R")  << std::endl;

        if(!is_red(parent) || !is_red(child)){
            std::cout << std::boolalpha;
            std::cout << "(nothing) : is_red(parent) = " << is_red(parent) << ", is_red(child) = " << is_red(child) << std::endl;
            return grandparent;
        }
        else {
            std::cout << "(something) : is_red(parent) = " << is_red(parent) << ", is_red(child) = " << is_red(child) << std::endl;
        }

        bool is_parent_right = parent->key > grandparent->key;
        Node<K, V> * uncle = is_parent_right ? grandparent->left : grandparent->right;
        std::cout << "uncle: " << (uncle != nullptr ? uncle->value : "") << (uncle != nullptr ? (uncle->is_black ? "B" : "R") : "")  << std::endl;

        if(is_red(parent) && is_red(uncle)){
            std::cout << "flip" << std::endl;
            flip_colors(grandparent);
            return grandparent;
        }

        else if(is_parent_right){
            if(child->key < parent->key){ // Right Left Zig-Zag
                // if(is_parent_right)
                grandparent->right = rotate_right(parent);
                // else grandparent->left = rotate_right(parent);
            }
            return rotate_left(grandparent);
        }

        else {
            if(child->key > parent->key){ // Left Right Zig-Zag
                // if(is_parent_right) grandparent->right = rotate_left(parent);
                // else
                grandparent->left = rotate_left(parent);
            }
            return rotate_right(grandparent);
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
        std::cout << std::setw(spacing * 4) << node->value << (node->is_black ? "B" : "R") << std::endl;
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
        int symbols = 4;
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
                    std::cout << node->value << (node->is_black ? "B" : "R");
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


    // --- Get Height Recursively ---
    int get_height(){ return get_height(root); }
    int get_height(Node<K, V> * root){
        if(root == nullptr){ return 0; }
        return 1 + std::max(get_height(root->left), get_height(root->right));
    }


    // --- Rotate Right : O(1) ---
    Node<K, V> * rotate_right(Node<K, V> * root){
        std::cout << "rotate_right" << std::endl;
        Node<K, V> * left = root->left;
        Node<K, V> * tmp = left->right;

        // rotation
        left->right = root;
        root->left = tmp;

        // update heights, order matters
        // update_height(root); // first update right subtree
        // update_height(left); // then update new root's height

        // push redness down
        left->is_black = true; // black
        root->is_black = false; // red

        return left;
    }

    // --- Rotate Left : O(1) ---
    Node<K, V> * rotate_left(Node<K, V> * root){
        std::cout << "rotate_left" << std::endl;
        Node<K, V> * right = root->right;
        Node<K, V> * tmp = right->left;

        // rotation
        right->left = root;
        root->right = tmp;

        // update heights, update order matters
        // update_height(root); // first update left subtree
        // update_height(right); // then update new root's height

        // push redness down
        right->is_black = true; // black
        root->is_black = false; // red

        return right;
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


    // --- Delete (BST) ---
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
