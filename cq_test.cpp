#include <iostream>
#include <sstream>
#include <functional>
#include <thread>
#include <cassert>
#include "include/ConcurrentQueue.h"

using MyQ = ConcurrentQueue<int>; // define the type of queue here

unsigned int unique = 0;	// unique item id

void produce(MyQ& q, unsigned int producer, unsigned int iter) {
    for (size_t i = 0; i < iter; ++i) {
        q.push(++unique);

        std::stringstream msg; // operator<< is non thread-safe
        msg << "Producer " << producer << " ==> item " << unique << ", size = " << q.size() << std::endl;
        std::cout << msg.str();
    }
}

void consume(MyQ& q, unsigned int consumer, unsigned int iter) {
    for (size_t i = 0; i< iter; ++i) {
        auto item = q.pop();

        std::stringstream msg;
        msg << "Consumer " << consumer << " <== id " << item << ", size = " << q.size() << std::endl;
        std::cout << msg.str();
    }
}

int main()
{
    MyQ q;
    const int iter_prod = 10;
    const int iter_cons = 10;

    // producer threads
    std::thread prod1(std::bind(&produce, std::ref(q), 1, iter_prod));
    std::thread prod2(std::bind(&produce, std::ref(q), 2, iter_prod));
    std::thread prod3(std::bind(&produce, std::ref(q), 3, iter_prod));

    // consumer threads
    std::thread consumer1(std::bind(&consume, std::ref(q), 1, iter_cons));
    std::thread consumer2(std::bind(&consume, std::ref(q), 2, iter_cons));
    std::thread consumer3(std::bind(&consume, std::ref(q), 3, iter_cons));

    prod1.join();
    prod2.join();
    prod3.join();
    consumer1.join();
    consumer2.join();
    consumer3.join();

    assert(q.empty());
}


