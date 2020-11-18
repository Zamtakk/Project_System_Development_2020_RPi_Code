#include "greeting.hpp"
#include <iostream>

int main() {
    std::cout << "Hello World!" << std::endl;
    for(int i = 0; i < 5; i++){
        std::cout << "Counting " << i << std::endl;
    }
    printGoodbye();
}
