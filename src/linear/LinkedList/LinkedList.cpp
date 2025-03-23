#include "LinkedList.tpp"

using linkedlist::LinkedList;

class Data {
public:
    double value;
    // ~Data(){ std::cout << "~Data {" << value << "}" << std::endl; }
};

int main(){
    // Data a {.value = 42.};
    // Data b = std::move(a);
    // b.value += 1;
    // std::cout << "a: " << a.value << std::endl;
    // std::cout << "b: " << b.value << std::endl;

    LinkedList<Data> ll;
    for(int i = 0; i < 5; ++i){
        // Data d;
        // d.value = 42. + i;
        // ll.addLast(d);
        // std::cout << d.value << std::endl;

        ll.addLast( Data({ .value = 42. + i }) );
        std::cout << ll.get(i).value << std::endl;

        // Data& t = ll.addLast(d); // 42, 43, 44, 45, 46
        // t.value *= 2; // 84 86 88 90 92
    }

    std::cout << "\nRange-based for loop example:" << std::endl;
    for(auto & e : ll){ // e is Node&
        std::cout << e.data.value << std::endl;
    }

    std::cout << "--- LinkedList.cpp execution ended! ---" << std::endl;
}

// int main(){
//     std::cout << "--- LinkedList.cpp execution started! ---" << std::endl;
//
//     LinkedList<double> ll;
//     for(int i = 0; i < 5; ++i){
//         ll.addLast(42 + i); // 42, 43, 44, 45, 46
//         // ll.addFirst(42 + i); // 42, 43, 44, 45, 46
//     }
//
//     ll.get(0) = 1;
//
//     // delete all: ( ll.clean(); )
//     // std::cout << "\nClean example:" << std::endl;
//     // for(int n = ll.size(), i = 0; i < n; i++){ // 0 1 2 3 4 5
//     //     ll.removeLast();
//     // }
//     ll.removeLast(); // if deleted all then this should throw an exception
//     // ll.removeFirst();
//
//     // O(N^2) = O(N) * O(N)
//     std::cout << "\nPrint all elements is O(N^2) example:" << std::endl;
//     for(int i = 0; i < ll.size(); ++i){ // O(N)
//         std::cout << ll.get(i) << std::endl; // O(N)
//     }
//
//     // LinkedList<double>::iterator it = ll.begin();
//     // std::cout << "(*it).data: " << (*it).data << std::endl;
//     // ++it;
//     // std::cout << "(*it).data: " << (*it).data << std::endl;
//
//     std::cout << "\nRange-based for loop example:" << std::endl;
//     for(auto & e: ll){
//         std::cout << e.data << std::endl;
//     }
//
//     std::cout << "\nFind example:" << std::endl;
//     int find_index = ll.find(43);
//     std::cout << "find_index: " << find_index << std::endl;
//
//     std::cout << "\nReverse example:" << std::endl;
//     ll.reverse(); // reverse
//     ll.reverse(); // normal
//     ll.reverse(); // reverse
//     for(auto & e: ll){
//         std::cout << e.data << std::endl;
//     }
//
//     std::cout << "--- LinkedList.cpp execution ended! ---" << std::endl;
//
// }

/*
Build and Run:
g++ LinkedList.cpp -Wall -o LinkedList.o && ./LinkedList.o
*/
