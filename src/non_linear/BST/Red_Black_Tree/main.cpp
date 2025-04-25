#include <iostream>
#include <string>

#include "RBT.tpp"

int main(){
    std::cout << "--- RBT/main.cpp execution started! ---" << std::endl;

    {
        /*
            Проверка работы алгоритма балансировки по step-ам.
        */
        RBT<int, std::string> rbt;

        rbt.insert(50, "50");
        rbt.insert(40, "40");
        rbt.insert(60, "60");
        rbt.insert(30, "30");
        rbt.insert(45, "45");
        rbt.insert(55, "55");
        rbt.insert(70, "70");

        // rbt.hprint();
        rbt.vprint();

        // rbt.insert(56, "56");
        // rbt.insert(65, "65");
        std::cout << "--- (80) ---" << std::endl;
        rbt.insert(80, "80", true);
        std::cout << "--- (90) ---" << std::endl;
        rbt.insert(90, "90", true);
        std::cout << "--- (100) ---" << std::endl;
        rbt.insert(100, "100", true);
        std::cout << "--- (110) ---" << std::endl;
        rbt.insert(110, "110", true);
        std::cout << "--- (120) ---" << std::endl;
        rbt.insert(120, "120", true);
        // rbt.hprint();
        // rbt.vprint();
    }

    {
        /*
            Проверил, что height <= 2*log2(N + 1). Вроде работает
        */
        // RBT<int, std::string> rbt;
        // for(int i = 0; i < 2000000; i++){
        //     rbt.insert(i, std::to_string(i));
        // }
        // rbt.hprint();
        // std::cout << rbt.get_height() << std::endl;
    }

    std::cout << "\n--- RBT/main.cpp execution ended! ---" << std::endl;
}

/*

g++ main.cpp -Wall -std=c++23 -o app.out && ./app.out

*/

