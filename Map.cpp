#include "Map.h"
#include <new>

size_t _Map::hash(void *key, size_t keySize)
{
    size_t hash = 1;
    u_int8_t *str = static_cast<u_int8_t *>(key);
    for (size_t i = 0; i != keySize; ++i)
    {
        int s = hash + str[i];
        hash ^= (str[i] << 5) * 31;
        hash ^= (str[i] >> 3) * 19;
    }
    return hash % HASH_TABLE_SIZE;
}

int _Map::insertByKey(void *key, size_t keySize, void *elem, size_t elemSize)
{
    size_t idx = hash(key, keySize);
    Map* map = (Map*)malloc(sizeof(Map));
    map->key = malloc(keySize);
    map->value = malloc(elemSize);
    memcpy(map->key, key, keySize);
    memcpy(map->value, elem, elemSize);
    map->key_size = keySize;
    map->value_size = elemSize;
    map->hash = idx;
    ListItem* iter = hashMap[idx]->head;
    while (iter)
    {
        if (Map::is_equals(map, iter->value))
        {
            return 1;
        }
        iter = iter->next;
    }
    pushBack(hashMap[idx], map);
    ++numberOfPairs;
    return 0;
}

void _Map::removeByKey(void* key, size_t keySize)
{
    size_t idx = hash(key, keySize);
    erase(hashMap[idx], key, keySize);
}

void* _Map::at(void* key, size_t keySize, size_t &valueSize)
{
    size_t idx = hash(key, keySize);
    ListItem* iter = hashMap[idx]->head;
    while (iter)
    {
        if(iter->value->key_size == keySize && !memcmp(iter->value->key, key, keySize))
        {
            valueSize = iter->value->value_size;
            return iter->value->value;
        }
        iter = iter->next;
    }
    return NULL;
}

int _Map::size() { return numberOfPairs; }

size_t _Map::max_bytes() { return HASH_TABLE_SIZE; }

bool _Map::empty() { return numberOfPairs == 0; }

void _Map::clear()
{
    for (size_t i = 0; i != HASH_TABLE_SIZE; ++i)
    {
        deleteLinkedList(&hashMap[i]);
        hashMap[i] = NULL;
    }    
    free(hashMap);
    hashMap = NULL;
    numberOfPairs = 0;
}

_MapIterator* _Map::findByKey(void* key, size_t keySize)
{
    size_t idx = hash(key, keySize);
    ListItem* iter = hashMap[idx]->head;
    while (iter)
    {
        if (keySize == iter->value->key_size && !memcmp(iter->value->key, key, keySize))
        {
            return new _MapIterator(hashMap, iter);
        }
        iter = iter->next;
    }
    return NULL;
}

_MapIterator* _Map::find(void* elem, size_t size)
{
    Map* map = static_cast<Map*>(elem);
    for (size_t i = 0; i != HASH_TABLE_SIZE; ++i)
    {
        ListItem* iter = hashMap[i]->head;
        while (iter)
        {
            if (Map::is_equals(iter->value, map))
            {
                return new _MapIterator(hashMap, iter);
            }
            iter = iter->next;
        }
    }
    return NULL;
}

_MapIterator* _Map::newIterator()
{
    return new _MapIterator(hashMap);
}

void _Map::remove(_MapIterator* iter)
{
    size_t size;
    Map* map = static_cast<Map*>(iter->getElement(size));
    erase(hashMap[map->hash], map->key, map->key_size);
}
