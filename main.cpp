#include <iostream>
#include "Map.cpp"


int main()
{
    _Map a = _Map();
    const char* str = "Hell";
    void* ptr = (void*)str;
    const char* str2 = "Hell2";
    void* ptr2 = (void*)str2;

    const char* str3 = "Hells";
    void* ptr3 = (void*)str3;
    const char* str4 = "Hells2";
    void* ptr4 = (void*)str4;

    a.insertByKey(ptr, 4, ptr2, 5);
    a.insertByKey(ptr3,5, ptr4, 6);
    a.removeByKey(ptr, 4);
    //a.clear();

    return 1;
}