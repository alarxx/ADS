/*
    SPDX-License-Identifier: MPL-2.0
    --------------------------------
    This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
    If a copy of the MPL was not distributed with this file,
    You can obtain one at https://mozilla.org/MPL/2.0/.

    Provided “as is”, without warranty of any kind.

    Copyright © 2025 Alar Akilbekov. All rights reserved.
 */

#include <iostream>
#include <string>
#include <functional> // std::hash
#include <cstddef> // std::size_t
#include <algorithm> // swap

/*
    https://en.cppreference.com/w/cpp/utility/hash

    std::hash<T>() -> std::size_t

    std::hash<T> используется в:
    - std::unordered_set
    - std::unordered_multiset
    - std::unordered_map
    - std::unordered_multimap

    - If k1 == k2 is true, h(k1) == h(k2) is also true.
    - Unless std::hash<Key> is a program-defined specialization, h(k1) will never throw an exception.

*/

struct Person {
    std::string name;
    int age;
    // if k1 == k2, then h(k1) == h(k2)
    // bool operator == (const Person&) const = default; // C++20
    bool operator == (const Person & other) const {
        return name == other.name && age == other.age;
    }
    // bool operator != (const Person & other) const { return !(*this == other); }
};

/*

    Custom specialization of std::hash can be injected in namespace std.

    Hash:
         FunctionObject,
         CopyConstructible,
         Destructible
    Default Constructable
    Copy Assignable
    Swappable

*/
namespace std {
    template <>
    struct hash<Person> {
        std::size_t seed = 17;

        hash() = default; // Default Constructable
        hash(std::size_t s) : seed(s) {}

        ~hash() = default; // Destructor
        hash(const hash&) = default; // Copy Constructable
        hash(hash&&) noexcept = default; // Move Constructable
        hash& operator=(const hash&) = default; // Copy Assignable
        hash& operator=(hash&&) noexcept = default; // Move Assignable

        /*
            // Standard Implementation
            template <typename T>
            void swap(T& lhs, T& rhs){
                T tmp = std::move(lhs);
                lhs = std::move(rhs);
                rhs = std::move(tmp);
            }
        */
        friend void swap(hash& a, hash& b) noexcept { // Swappable
            std::swap(a.seed, b.seed);
        }

    private:
        static void hash_combine(std::size_t & seed, std::size_t value){
            seed = ((seed << 5) - seed) + value;
        }

    public:
        // h(k1) will never throw an exception
        std::size_t operator () (const Person & p) const noexcept {
            std::size_t hash = seed;
            // 31*x + y rule
            // hash = ((hash << 5) - hash) + std::hash<std::string>{}(p.name);
            // hash = ((hash << 5) - hash) + std::hash<int>{}(p.age);
            hash_combine(hash, std::hash<std::string>{}(p.name));
            hash_combine(hash, std::hash<int>{}(p.age));
            return hash;
        }

    };
}

int main() {
    Person alar = {.name = "Alar", .age = 20};
    std::hash<Person> hash_fn;

    std::size_t alar_hash = hash_fn(alar);
    std::cout << "1) hash(alar ): " << alar_hash << std::endl;

    Person clone = {.name = "Alar", .age = 20};
    std::size_t clone_hash = hash_fn(clone);
    std::cout << "2) hash(clone): " << clone_hash << std::endl;

    std::cout << std::boolalpha;
    std::cout << "clone == alar: " << (clone == alar) << std::endl;
    std::cout << "hash(clone) == hash(alar): " << (clone_hash == alar_hash) << std::endl;

    return 0;
}
