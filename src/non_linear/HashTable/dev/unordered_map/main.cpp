#include <iostream>
#include <unordered_map>
#include <string>

/*

    Value must be Default Constructable
    Because table[key] creates new default Value object if key isn't in the table

*/

struct Int {
    int value;
    Int() = delete;
    // Int() = default;
    Int(int value) : value(value) {}
    int& operator = (int value) {
        this->value = value;
        return this->value;
    }
    friend std::ostream& operator << (std::ostream& os, const Int& i);
};

std::ostream& operator << (std::ostream& os, const Int& i){
    os << i.value;
    return os;
}

int main() {
    // test Int
    // Int a = 123;
    // std::cout << a << std::endl;

    // Создаем unordered_map с ключами типа string и значениями типа int
    std::unordered_map<std::string, Int> age;

    // Добавление элементов
    age["Alice"] = 25;
    age["Bob"] = 30;
    age["Charlie"] = 28;

    // Доступ к элементу
    std::cout << "Возраст Bob: " << age["Bob"] << std::endl;

    // Проверка наличия ключа
    if (age.find("David") == age.end()) {
        std::cout << "David не найден в map." << std::endl;
    }

    // Перебор всех элементов
    for (const auto& pair : age) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    // Удаление элемента
    age.erase("Alice");

    return 0;
}

