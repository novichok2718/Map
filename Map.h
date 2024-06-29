#include "TableAbstract.h"
#include "List.cpp"
size_t HASH_TABLE_SIZE = 1000000;

class _MapIterator  : public Container::Iterator
{
    ListItem *current;
    LinkedList **hashMap;

public:
    _MapIterator(LinkedList **hashMap, ListItem *curr) : hashMap(hashMap), current(curr) {}
    _MapIterator(LinkedList **hashMap)
    {
        this->hashMap = hashMap;
        this->current = NULL;
        for (size_t i = 0; i != HASH_TABLE_SIZE; ++i)
        {
            if (hashMap[i]->head && hashMap[i]->head->value)
            {
                current = hashMap[i]->head;
                break;
            }
        }
    }

    void setCurrent(ListItem *item)
    {
        current = item;
    }

    void *getElement(size_t &size)
    {
        if (current && current->value)
        {
            size = 2 * sizeof(size_t) + current->value->key_size + current->value->value_size;
            return static_cast<void *>(current->value);
        }
        return NULL;
    }

    bool hasNext()
    {
        if (current)
        {
            if (current->next) { return true; }

            size_t idx = current->value->hash;
            for (size_t i = idx + 1; i != HASH_TABLE_SIZE; ++i)
            {
                if (hashMap[i] && hashMap[i]->head)
                {
                    return true;
                }
            }
        }
        return false;
    }

    void goToNext()
    {
        if (current)
        {
            if (current->next)
            {
                current = current->next;
                return;
            }
            size_t idx = current->value->hash;
            for (size_t i = idx + 1; i != HASH_TABLE_SIZE; ++i)
            {
                if (hashMap[i] && hashMap[i]->head)
                {
                    current = hashMap[i]->head;
                    return;
                }
            }
        }
        current = NULL;
    }

    bool equals(Container::Iterator *right)
    {
        size_t size;
        _MapIterator* r = dynamic_cast<_MapIterator*>(right);
        Map *map = static_cast<Map *>(r->getElement(size));
        return Map::is_equals(current->value, map);
    }

    ~_MapIterator() = default;
};

class _Map  : AbstractTable
{
    LinkedList **hashMap;
    size_t numberOfPairs;

public:
    _Map(MemoryManager &mem) : AbstractTable(mem)
    {
        hashMap = (LinkedList **)malloc(sizeof(LinkedList *) * HASH_TABLE_SIZE);
        numberOfPairs = 0;
        for (int i = 0; i != HASH_TABLE_SIZE; ++i)
        {
            hashMap[i] = createLinkedList();
        }
    }
    int insertByKey(void *key, size_t keySize, void *elem, size_t elemSize); // implemented

    Container::Iterator *findByKey(void *key, size_t keySize);

    void removeByKey(void *key, size_t keySize); // implemented

    void *at(void *key, size_t keySize, size_t &valueSize); // implemented

    int size(); // implemented

    size_t max_bytes(); // implemented

    Container::Iterator *find(void *elem, size_t size);

    Container::Iterator *newIterator();

    void remove(Container::Iterator *iter);

    void clear(); // implemented

    bool empty(); // implemented

    size_t hash(void *key, size_t keySize); // implemented

    int getMaxCollisionCount();

    void rehash();

    ~_Map()
    {
        if (hashMap)
        {
            clear();
        }
    }
};
