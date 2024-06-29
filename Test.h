#include "Map.cpp"
#include <chrono>
#include <cassert>

class Test
{
    _Map *map;

public:
    Test()
    {
        Mem mem(100);
        map = new _Map(mem);
    }

    void insertByKeyTest(size_t nums);

    void findTest(void *elem, size_t elemSize);

    void newIteratorTest(int mod);

    void deleteTableTest();

    void removeByKeyTest(void *key, size_t keySize);

    void deleteEverySecondElemTest(size_t nums);

    void emptyTest();
    
    void max_bytesTest();

    void atTest(size_t nums);

    void sizeTest();

    ~Test();
};