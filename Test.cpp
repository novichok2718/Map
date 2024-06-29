#include "Test.h"
#include <vector>
#include <algorithm>

void Test::insertByKeyTest(size_t nums)
{
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i != nums; ++i)
    {
        map->insertByKey(&i, 8, &i, 8);
    }
    assert(map->size() == nums);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << (end - start).count() / 1e6 << "  miliseconds" << std::endl;
    std::cout << "InsertByKeyTest passed" << std::endl;
}

void Test::findTest(void *elem, size_t elemSize)
{
    auto start = std::chrono::high_resolution_clock::now();
    auto iter = map->find(elem, elemSize);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << (end - start).count() / 1e6 << "  miliseconds" << std::endl;
    size_t size;
    assert(memcmp(iter->getElement(size), elem, elemSize) && size == elemSize);
    Map* map = (Map*)elem;
    Map* map1 = (Map*)iter->getElement(size);
    std::cout<<"iter key = "<<*((size_t*)map1->key)<<" iter value = "<<*((size_t*)map1->value)<<std::endl;
    std::cout<<"elem key = "<<*((size_t*)map->key)<<" elem value = "<<*((size_t*)map->value)<<std::endl;
    delete dynamic_cast<_MapIterator *>(iter);
    std::cout << "findTest passed" << std::endl;
}

void Test::newIteratorTest(int mod)
{
    auto iter = map->newIterator();
    if (mod)
    {
        assert(iter != NULL);
    }
    else
    {
        assert(iter == NULL);
    }
    std::cout << "newIteratorTest passed" << std::endl;
}

void Test::deleteTableTest()
{
    auto start = std::chrono::high_resolution_clock::now();
    map->clear();
    assert(map->size() == 0);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << (end - start).count() / 1e6 << "  miliseconds" << std::endl;
    std::cout << "deleteTableTest passed" << std::endl;
}

void Test::removeByKeyTest(void *key, size_t keySize)
{
    auto start = std::chrono::high_resolution_clock::now();
    size_t size = map->size();
    map->removeByKey(key, keySize);
    assert((size - 1) == map->size());
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << (end - start).count() / 1e6 << "  miliseconds" << std::endl;
    std::cout << "removeByKeyTest passed" << std::endl;
}

void Test::deleteEverySecondElemTest(size_t nums)
{
    std::vector<size_t> vec;
    vec.reserve(500000);
    size_t size;
    auto start = std::chrono::high_resolution_clock::now();
    Container::Iterator *iter = map->newIterator();
    int size1 = map->size();
    int cnt = 1;
    while (iter->getElement(size))
    {
        Map* tmp = (Map*)iter->getElement(size);
        size_t* elem = (size_t*)tmp->key;
        if (*elem % 2 == 0)
        {
            map->remove(iter);
        }
        else
        {
            iter->goToNext();
        }
    }
    delete dynamic_cast<_MapIterator *>(iter);

    size_t valueSize;
    for (size_t i = 1; i < HASH_TABLE_SIZE; i += 2)
    {
        size_t* value = (size_t*)map->at(&i, 8, valueSize);
        vec.push_back(*value);
    }

    sort(vec.begin(), vec.end());

    size_t j = 0;
    for(size_t i = 1; i < HASH_TABLE_SIZE; i += 2)
    {
        if (vec[j] != i)
        {
            std::cout<<"Test Failed"<<std::endl;
        }
        j++;
    }
    std::cout<<"Test passed"<<std::endl;
    

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << (end - start).count() / 1e6 << "  miliseconds" << std::endl;
    std::cout << "deleteEverySecondElemTest passed" << std::endl;
}

void Test::emptyTest()
{
    auto start = std::chrono::high_resolution_clock::now();
    map->clear();
    assert(map->empty());
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << (end - start).count() / 1e6 << "  miliseconds" << std::endl;
    std::cout << "emptyTest passed" << std::endl;
}

void Test::max_bytesTest()
{
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << map->max_bytes() << std::endl;
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << (end - start).count() / 1e6 << "  miliseconds" << std::endl;
}

void Test::atTest(size_t nums)
{
    auto start = std::chrono::high_resolution_clock::now();
    size_t size;
    for (size_t i = 0; i != nums; ++i)
    {
        int *elem = (int *)map->at(&i, 8, size);
        if (elem)
        {
            std::cout << *elem << std::endl;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << (end - start).count() / 1e6 << "  miliseconds" << std::endl;
}

void Test::sizeTest()
{
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << map->size() << std::endl;
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << (end - start).count() / 1e6 << "  miliseconds" << std::endl;
}

Test::~Test()
{
    map->clear();
    delete map;
}
