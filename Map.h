#include "TableAbstract.h"
#include "List.h"

size_t HASH_TABLE_SIZE = 1;

class _Map
{
    LinkedList **hashMap;
    size_t numberOfPairs;

public:
    _Map()
    {
        hashMap = (LinkedList **)malloc(sizeof(LinkedList *) * HASH_TABLE_SIZE);
        numberOfPairs = 0;
        for (int i = 0; i != HASH_TABLE_SIZE; ++i)
        {
            hashMap[i] = createLinkedList();
        }
    }
    int insertByKey(void *key, size_t keySize, void *elem, size_t elemSize);// implemented

    //Iterator* findByKey(void *key, size_t keySize) = 0;


    void removeByKey(void *key, size_t keySize);// implemented

    void *at(void *key, size_t keySize, size_t &valueSize);// implemented

    int size(); //implemented

    size_t max_bytes(); //implemented

    //Iterator *find(void *elem, size_t size) = 0;

    //Iterator *newIterator() = 0;

    //void remove(Iterator *iter) = 0;

    void clear(); //implemented

    bool empty(); //implemented

    ~_Map()
    {
        clear();
    }

private:
    size_t hash(void *key, size_t keySize);// implemented
};
