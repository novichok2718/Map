#include <iostream>
#include <chrono>
#include "Map.cpp"

int main()
{
    // Mem* mem = new Mem(1000);
    _Map a = _Map();
    const char *str = "Hell";
    void *ptr = (void *)str;
    const char *str2 = "Hell2";
    void *ptr2 = (void *)str2;

    const char *str3 = "Hells";
    void *ptr3 = (void *)str3;
    const char *str4 = "Hells2";
    void *ptr4 = (void *)str4;
    // auto start = std::chrono::high_resolution_clock::now();
    // for (size_t i = 0; i != 100000; ++i)
    // {
    //     a.insertByKey(ptr, 4, ptr2, 5);
    //     a.insertByKey(ptr3, 5, ptr4, 6);
    //     a.removeByKey(ptr, 4);
        
    // }
    // auto end = std::chrono::high_resolution_clock::now();
    a.insertByKey(ptr, 4, ptr2, 5);
    a.insertByKey(ptr3, 5, ptr4, 6);
    auto iter = a.findByKey(ptr, 4);
    iter->goToNext();
    a.remove(iter);
    //std::cout<<(end - start).count() / 1e6<<"  miliseconds"<<std::endl;
    a.clear();
    return 1;
}