#include "Map.h"
#include <new>
#include <cstdint>

size_t _Map::hash(void *key, size_t len)
{
    const uint8_t *data = static_cast<const uint8_t *>(key);
    const int nblocks = len / 4;
    size_t h1 = 0;

    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;

    const uint32_t *blocks = reinterpret_cast<const uint32_t *>(data + nblocks * 4);

    for (int i = -nblocks; i; i++)
    {
        uint32_t k1 = blocks[i];

        k1 *= c1;
        k1 = (k1 << 15) | (k1 >> (32 - 15));
        k1 *= c2;

        h1 ^= k1;
        h1 = (h1 << 13) | (h1 >> (32 - 13));
        h1 = h1 * 5 + 0xe6546b64;
    }

    const uint8_t *tail = data + nblocks * 4;
    uint32_t k1 = 0;

    switch (len & 3)
    {
    case 3:
        k1 ^= tail[2] << 16;
    case 2:
        k1 ^= tail[1] << 8;
    case 1:
        k1 ^= tail[0];
        k1 *= c1;
        k1 = (k1 << 15) | (k1 >> (32 - 15));
        k1 *= c2;
        h1 ^= k1;
    }

    h1 ^= len;
    h1 ^= h1 >> 16;
    h1 *= 0x85ebca6b;
    h1 ^= h1 >> 13;
    h1 *= 0xc2b2ae35;
    h1 ^= h1 >> 16;

    return h1;
}

int _Map::insertByKey(void *key, size_t keySize, void *elem, size_t elemSize)
{
    int flag = 0;
    if (key && elem && keySize > 0 && elemSize > 0)
    {
        if (numberOfPairs / HASH_TABLE_SIZE > 5)
        {
            rehash();
        }
        size_t idx = hash(key, keySize) % HASH_TABLE_SIZE;
        Map *map = (Map *)malloc(sizeof(Map));
        map->key = malloc(keySize);
        map->value = malloc(elemSize);
        memcpy(map->key, key, keySize);
        memcpy(map->value, elem, elemSize);
        map->key_size = keySize;
        map->value_size = elemSize;
        map->hash = idx;
        ListItem *iter = hashMap[idx]->head;
        while (iter)
        {
            if (Map::is_equals(map, iter->value))
            {
                deleteMap(&map);
                flag = 1;
                break;
            }
            iter = iter->next;
        }
        pushBack(hashMap[idx], map);
        ++numberOfPairs;
    }
    return flag;
}

void _Map::removeByKey(void *key, size_t keySize)
{
    if (key)
    {
        size_t idx = hash(key, keySize) % HASH_TABLE_SIZE;
        if (hashMap[idx]->head && erase(hashMap[idx], key, keySize))
        {
            --numberOfPairs;
        }
    }
}

void *_Map::at(void *key, size_t keySize, size_t &valueSize)
{
    if (key)
    {
        size_t idx = hash(key, keySize) % HASH_TABLE_SIZE;
        ListItem *iter = hashMap[idx]->head;
        while (iter)
        {
            if (iter->value->key_size == keySize && !memcmp(iter->value->key, key, keySize))
            {
                valueSize = iter->value->value_size;
                return iter->value->value;
            }
            iter = iter->next;
        }
    }
    std::cout << "Key Not found" << std::endl;
    return NULL;
}

int _Map::size() { return numberOfPairs; }

size_t _Map::max_bytes() { return HASH_TABLE_SIZE; }

bool _Map::empty() { return numberOfPairs == 0; }

void _Map::clear()
{
    if (hashMap)
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
}

Container::Iterator *_Map::findByKey(void *key, size_t keySize)
{
    if (key)
    {
        size_t idx = hash(key, keySize) % HASH_TABLE_SIZE;
        ListItem *iter = hashMap[idx]->head;
        while (iter)
        {
            if (keySize == iter->value->key_size && !memcmp(iter->value->key, key, keySize))
            {
                return new _MapIterator(hashMap, iter);
            }
            iter = iter->next;
        }
    }
    return NULL;
}

Container::Iterator *_Map::find(void *elem, size_t size)
{
    Map *map = static_cast<Map *>(elem);
    if (map && map->key && map->key_size > 0 && map->value && map->value_size > 0)
    {
        ListItem *iter = hashMap[map->hash]->head;
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

Container::Iterator *_Map::newIterator()
{
    _MapIterator *iter = new _MapIterator(hashMap);
    size_t size;
    Map *ans = static_cast<Map *>(iter->getElement(size));
    if (ans)
    {
        return iter;
    }
    delete iter;
    return NULL;
}

void _Map::remove(Container::Iterator *iter)
{
    if (iter)
    {
        _MapIterator *it = dynamic_cast<_MapIterator *>(iter);
        size_t size;
        Map *map = static_cast<Map *>(it->getElement(size));
        size_t idx = map->hash;

        if (!hashMap[idx]->head)
        {
            return;
        }

        ListItem *current = hashMap[idx]->head;
        ListItem *previous = NULL;

        while (current)
        {
            if (current->value->key_size == map->key_size && !memcmp(current->value->key, map->key, map->key_size))
            {
                ListItem *nextItem = current->next;

                if (current == hashMap[idx]->head)
                {
                    hashMap[idx]->head = nextItem;
                }
                else
                {
                    previous->next = nextItem;
                }

                deleteMap(&current->value);
                free(current);
                --hashMap[idx]->size;
                --numberOfPairs;

                if (nextItem)
                {
                    it->setCurrent(nextItem);
                }
                else
                {
                    for (size_t i = idx + 1; i < HASH_TABLE_SIZE; ++i)
                    {
                        if (hashMap[i]->head)
                        {
                            it->setCurrent(hashMap[i]->head);
                            return;
                        }
                    }
                    it->setCurrent(NULL);
                }

                return;
            }
            previous = current;
            current = current->next;
        }
    }
}

void _Map::rehash()
{
    size_t newHashTableSize = HASH_TABLE_SIZE * 5;

    LinkedList **newHashMap = (LinkedList **)malloc(sizeof(LinkedList *) * newHashTableSize);
    for (size_t i = 0; i < newHashTableSize; ++i)
    {
        newHashMap[i] = createLinkedList();
    }

    for (size_t i = 0; i < HASH_TABLE_SIZE; ++i)
    {
        ListItem *current = hashMap[i]->head;
        while (current && current->value)
        {
            Map *map = current->value;

            size_t newIdx = hash(map->key, map->key_size) % newHashTableSize;
            map->hash = newIdx;
            pushBack(newHashMap[newIdx], map);
            current = current->next;
        }
    }

    for (size_t i = 0; i < HASH_TABLE_SIZE; ++i)
    {

        ListItem *current = hashMap[i]->head;
        while (current)
        {
            ListItem *next = current->next;
            free(current);
            current = next;
        }
        free(hashMap[i]);
    }
    free(hashMap);

    hashMap = newHashMap;
    HASH_TABLE_SIZE = newHashTableSize;
}

int _Map::getMaxCollisionCount()
{
    int max = 0;
    for (int i = 0; i != HASH_TABLE_SIZE; ++i)
    {
        if (hashMap[i]->head && hashMap[i]->size > max)
        {
            max = hashMap[i]->size;
        }
    }
    return max;
}