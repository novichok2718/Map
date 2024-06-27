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

    void insertByKeyTest(size_t nums)
    {
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i != nums; ++i)
        {
            map->insertByKey(&i, 4, &i, 4);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << (end - start).count() / 1e6 << "  miliseconds" << std::endl;
    }

    void findByKeyTest(void* key, size_t keySize)
    {
        
    }

    void findTest(void* elem, size_t elemSize)
    {
        
    }

    void deleteTableTest()
    {
        auto start = std::chrono::high_resolution_clock::now();
        map->clear();
        assert(map->size() == 0);
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << (end - start).count() / 1e6 << "  miliseconds" << std::endl;
    }

    void removeByKeyTest(void* key, size_t keySize)
    {
        auto start = std::chrono::high_resolution_clock::now();
        size_t size = map->size();
        map->removeByKey(key, keySize);
        assert((size - 1) == map->size());
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << (end - start).count() / 1e6 << "  miliseconds" << std::endl;
    }

    void deleteEverySecondElemTest()
    {
        size_t size;
        auto start = std::chrono::high_resolution_clock::now();
        Container::Iterator* iter = map->newIterator();
        int cnt = 1;
        while (iter->getElement(size))
        {
            if (cnt % 2 == 0)
            {
                map->remove(iter);
            }
            else
            {
                iter->goToNext();
            }
            ++cnt;
        }
        delete dynamic_cast<_MapIterator*>(iter);
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << (end - start).count() / 1e6 << "  miliseconds" << std::endl;
    }

    void sizeTest()
    {
        auto start = std::chrono::high_resolution_clock::now();
        std::cout<<map->size()<<std::endl;
        auto end = std::chrono::high_resolution_clock::now();
        std::cout<<(end - start).count() / 1e6<<"  miliseconds"<<std::endl;
    }

    void emptyTest()
    {
        auto start = std::chrono::high_resolution_clock::now();
        map->clear();
        assert(map->empty());
        auto end = std::chrono::high_resolution_clock::now();
        std::cout<<(end - start).count() / 1e6<<"  miliseconds"<<std::endl;
    
    }

    void max_bytesTest()
    {
        auto start = std::chrono::high_resolution_clock::now();
        std::cout<<map->max_bytes()<<std::endl;
        auto end = std::chrono::high_resolution_clock::now();
        std::cout<<(end - start).count() / 1e6<<"  miliseconds"<<std::endl;
    }

    void atTest(size_t nums)
    {
        auto start = std::chrono::high_resolution_clock::now();
        size_t size;
        for (int i = 0; i != nums; ++i)
        {
            int *elem = (int *)map->at(&i, 4, size);
            if (elem)
            {
                std::cout << *elem << std::endl;
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::cout<<(end - start).count() / 1e6<<"  miliseconds"<<std::endl;
    }

    ~Test()
    {
        map->clear();
        delete map;
    }
};
