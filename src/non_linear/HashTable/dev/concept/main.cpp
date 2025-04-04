#include <iostream>
#include <string>
#include <type_traits>
#include <concepts>

// Trailing Return Type
auto fun() -> int {
    return 42;
}

template <typename K> // C++20
concept Hashable = requires(K key) {
    // // SFINAE constrains
    // typename T::type;
    // a.x;
    // a.f();
    // a[0];
    // a + b;
    // // { expression } -> constraint;
    // {*a + 1} -> std::convertible_to<float>;
    // {a * a} -> std::same_as<int>;
    // // requires clause
    // requires std::is_arithmetic_v<T>; // SFINAE
    // requires Arithmetic<T>; // concept
    // requires requires (T t) { t.x; }; // Nested requires expressions


    // specific check for what we need
    key == key;
    { key.hashCode() } -> std::same_as<int>;
};

int main(){
    std::cout << fun() << std::endl; // 42
}
