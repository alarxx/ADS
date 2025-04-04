#include <iostream>
#include <string>
#include <type_traits>

// (1) enable_if
// C++11 in type_traits
template <bool Condition, typename Type = void>
class enable_if {
// type is not defined if Condition == false
};

template <typename Type>
class enable_if<true, Type> {
public:
    using type = Type;
};

template <bool Condition, typename Type>
using enable_if_t = typename enable_if<Condition, Type>::type;


// (2) false_type / true_type
// C++11 in type_traits
template <typename T, T v>
struct integral_constant {
    static constexpr T value = v; // false/true
    using value_type = T; // bool
    using type = integral_constant<T, v>;
    // constexpr operator T() const noexcept { return v; } // typecasting
    // constexpr T operator()() const noexcept { return v; } // function call
};

class false_type : public integral_constant<bool, false> {};
class true_type  : public integral_constant<bool, true>  {};


// (3) void_t
// C++17 in type_traits
template <typename ... TArgs>
using void_t = void;


// (4) Custom SFINAE check

template <typename T, typename U = void>
class has_size : public false_type {};

template <typename T>
class has_size <T, void_t<decltype(std::declval<T>().size())> > : public true_type {};
// Здесь получается мы вызываем size() и в зависимости от наличия выберется одна из реализаций

template <typename T>
constexpr bool has_size_v = has_size<T>::value;


// (5)

template <typename T, enable_if_t<!has_size_v<T>, int> = 0>
void fun(T item){
    std::cout << "No size!" << std::endl;
}

template <typename T, enable_if_t<has_size_v<T>, int> = 0>
// template <typename T>
// enable_if_t<has_size_v<T>, void>
// fun(T item){
void fun(T item){
    std::cout << item.size() << std::endl;
}


// (6)

template <typename T, typename U = void>
class F; // base template
// {
    // F(T){} // replace with deduction guide
// };

// Partial Specialization
template <typename T>
class F<T, enable_if_t<!has_size_v<T>, void>> {
public:
    T _item;
    F(T item) : _item(item) {}
    void fun(){ std::cout << "No size!" << std::endl; }
};

template <typename T>
class F<T, enable_if_t<has_size_v<T>, void>> {
public:
    T _item;
    F(T item) : _item(item) {}
    void fun(){
        std::cout << _item.size() << std::endl;

        /*
        // Можно было бы и просто сделать constexpr проверку
        if constexpr (has_size_v<T>) {
            std::cout << _item.size() << std::endl;
        } else {
            std::cout << "No size!" << std::endl;
        }
        */
    }
};

// Deduction guide
template <typename T>
F(T) -> F<T>;

int main(){
    // (1)
    enable_if_t<(2+2==4), int> a = (float) 42.f;
    std::cout << a << typeid(a).name() << std::endl; // 42i

    // (2)
    std::cout << std::boolalpha << true_type::value << std::endl; // true

    // (4)
    decltype('A') ch = (char) /*( decltype(std::declval<char>()) )*/ 'B';
    std::cout << typeid(ch).name() << std::endl; // c

    // (5)
    std::string str = "12345";
    fun(str); // 5

    fun(a); // No size!

    // (6)
    F f(str);
    f.fun();

    F fa(a);
    fa.fun();

}
