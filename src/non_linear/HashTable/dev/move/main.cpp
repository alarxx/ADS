#include <iostream>

template <typename T>
struct Data {
    T _theData;
    Data(T data) : _theData(data) {
        std::cout << "Data Constructor: " << _theData << std::endl;
    }
    ~Data(){ std::cout << "Data Destructor: " << _theData << std::endl; }
};

/*

    По-умолчанию, если не реализовывать copy/move constructor/assignment:

    Default behaviour is Shallow Copy,

    То есть он сделает копию каждого field-а

    В случае с указателем, он сделает другой указатель, но указывать он будет на тот же самый адрес - область памяти,
    Это приведет к ошибке при освобождении памяти - 2 объекта попытаются освободить один и тот же адрес:
        free(): double free detected in tcache 2
        Aborted (core dumped)

*/
struct ComplexData {
    int * intptr;
    ComplexData(int value = 0){
        intptr = new int(value);
    }
    ~ComplexData(){
        if(intptr != nullptr){
            delete intptr;
        }// Shallow Copy
    }
};

int main(){
    std::cout << "--- main.cpp execution started! ---" << std::endl;

    // std::cout << "\n--- move example: ---" << std::endl;
    //
    // Data data(123);
    // Data stealed = std::move(data);
    // stealed._theData = 124;
    // Data copy = data;
    //
    // std::cout << "&data: " << &data << ", &_theData: " << &(data._theData) << " (same)" << std::endl;
    // std::cout << &(stealed._theData) << std::endl;
    // std::cout << &(copy._theData) << std::endl;
    //
    //
    // std::cout << "\n--- memalloc example: ---" << std::endl;
    // int * intptr = new int(123);
    // std::cout << *intptr << std::endl;

    std::cout << "\n--- move with complex data example: ---" << std::endl;
    ComplexData cdata(123);
    std::cout << *cdata.intptr << std::endl;
    ComplexData cstealed = std::move(cdata); // Shallow Copy
    ComplexData ccopy = cdata; // Shallow Copy
    std::cout << "&cdata: " << &cdata << ", &intptr: " << &cdata.intptr << ", intptr: " << cdata.intptr << " (same)" << std::endl;
    std::cout << "&cdata: " << &cstealed << ", &intptr: " << &cstealed.intptr << ", intptr: " << cstealed.intptr << " (same)" << std::endl;
    std::cout << "&cdata: " << &ccopy << ", &intptr: " << &ccopy.intptr << ", intptr: " << ccopy.intptr << " (same)" << std::endl;

    std::cout << "\n--- main.cpp execution ended! ---" << std::endl;
}
