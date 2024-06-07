#include "TableAbstract.h"
#include "List.h"

size_t HASH_TABLE_SIZE = 9973;

class _MapIterator // : Container::Iterator
{
    ListItem *current;
    LinkedList **hashMap;

public:
    _MapIterator(LinkedList **hashMap, ListItem* curr) : hashMap(hashMap), current(curr) {}
    _MapIterator(LinkedList **hashMap)
    {
        this->hashMap = hashMap;
        for (size_t i = 0; i != HASH_TABLE_SIZE; ++i)
        {
            if (hashMap[i]->head)
            {
                current = hashMap[i]->head;
                break;
            }
        }
    }

    void *getElement(size_t &size)
    {
        size = current->value->value_size;
        return static_cast<void *>(current->value);
    }

    bool hasNext()
    {
        return current->next == NULL;
    }

    void goToNext()
    {
        if (current->next)
        {
            current = current->next;
            return;
        }
        size_t idx = current->value->hash;
        for (size_t i = idx + 1; i != HASH_TABLE_SIZE; ++i)
        {
            if (hashMap[i]->head)
            {
                current = hashMap[i]->head;
                return;
            }
        }
    }

    bool equals(_MapIterator *right)
    {
        size_t size;
        Map *map = static_cast<Map *>(right->getElement(size));
        return Map::is_equals(current->value, map);
    }

    ~_MapIterator() = default;
};

class _Map // : AbstractTable
{
    LinkedList **hashMap;
    size_t numberOfPairs;

public:
    _Map() //(MemoryManager &mem) : AbstractTable(mem)
    {
        hashMap = (LinkedList **)malloc(sizeof(LinkedList *) * HASH_TABLE_SIZE);
        numberOfPairs = 0;
        for (int i = 0; i != HASH_TABLE_SIZE; ++i)
        {
            hashMap[i] = createLinkedList();
        }
    }
    int insertByKey(void *key, size_t keySize, void *elem, size_t elemSize); // implemented

    _MapIterator *findByKey(void *key, size_t keySize);

    void removeByKey(void *key, size_t keySize); // implemented

    void *at(void *key, size_t keySize, size_t &valueSize); // implemented

    int size(); // implemented

    size_t max_bytes(); // implemented

    _MapIterator *find(void *elem, size_t size);

    _MapIterator *newIterator();

    void remove(_MapIterator *iter);

    void clear(); // implemented

    bool empty(); // implemented

    ~_Map()
    {
        if (hashMap)
        {
            clear();
        }
    }

private:
    size_t hash(void *key, size_t keySize); // implemented
};
