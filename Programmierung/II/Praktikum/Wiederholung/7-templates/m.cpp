#include <iostream>
#include "queue.h"

int main() {
    Queue<const char*> q1 = Queue<const char*>();
    q1.push("Hallo");
    q1.push("World");
    std::cout << q1;
    return 0;
}