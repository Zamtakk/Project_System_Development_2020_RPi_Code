#include <iostream>
#include "functions/GreetingsLib.cpp"

using namespace std;

int main() {
    cout << "Hello world!" << endl;
    int i = 5;
    for (int q = 0; q < i; q++){
        cout << "Counting: " << q << endl;
    }
    printGoodbye();
}