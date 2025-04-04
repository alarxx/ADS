#include <iostream>
#include <string>

#include "HashTable.tpp"

/*
    Value must be Default Constructable
    Because table[key] creates new default Value object if key isn't in the table
*/
struct Int {
    int value;
    // Int() = delete;
    Int() = default;
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


int main(){
    std::cout << "--- HashTable/main.cpp execution started! ---" << std::endl;

    using value_type = Int;

    int capacity = 3; // 1 to conviniently check LinkedList (bucket)

    // HashTable<std::string, value_type> table(1);
    HashTable<std::string, value_type> table(capacity);

     /* Uniform Distribution */ {
        std::cout << "\n--- Distribution: --- " << std::endl;
        int pool[capacity];
        for(int & i : pool){ i = 0; }
        for(int i = 1; i <= capacity * 10000; ++i){
            std::string key = "_" + std::to_string(i);
            int bucket = table.hash(key);
            pool[bucket]++;
        }
        for(int i = 0; i < capacity; i++){
            std::cout << i << ": " << pool[i] / (double) (capacity * 10000)  << std::endl;
        }
    }

    /* Main Example : */ {
        std::cout << "\n--- (start) main put/at example: --- " << std::endl;

        // put values
        for(int i = 1; i <= 5; ++i){
            std::string key = "_" + std::to_string(i);
            // std::cout << key << ": " << i << std::endl;
            table.put(key, i);
        }

        table.put("_2", 22);

        // print values
        for(int i = 1; i <= 10; ++i){
            std::string key = "_" + std::to_string(i);
            // int * valueptr = table.at("Hello"); // Можно было бы возвращать nullptr, когда нет ключа
            try {
                value_type & value = table.at(key);
                std::cout << key << ": " <<  value << std::endl;
            }
            catch (const std::out_of_range & e) {
                std::cerr << e.what() << '\n';
            }
        }
    }

    /* index operator [] */ {
        std::cout << "\n--- (start) index operator [] example: --- " << std::endl;
        std::cout << std::boolalpha;
        std::cout << "table.contains(\"_10\"): " << table.contains("_10") << std::endl;

        value_type & value = table["_10"]; // "_10" key is not in table, so it will put it there

        std::cout << "(default) table[\"_10\"]: " << value << std::endl;

        table["_10"] = 10;
        std::cout << "table[\"_10\"]: " << value << std::endl;

        std::cout << "table.contains(\"_10\"): " << table.contains("_10") << std::endl;

        std::cout << " --- (end) index operator [] example. --- \n" << std::endl;
    }

    std::cout << "capacity: " << table.capacity << std::endl;
    std::cout << "N: " << table.N << std::endl;
    std::cout << "load: " << table.load << std::endl;

    std::cout << "\n--- HashTable/main.cpp execution ended! ---" << std::endl;
}

/*

- [x] load_factor
- [ ] rehash
- [ ] reserve

*/

/*

g++ main.cpp -Wall -std=c++23 -o app.out && ./app.out

*/
