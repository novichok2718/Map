struct Map
{
    void *key;
    size_t key_size;
    void *value;
    size_t value_size;
    size_t hash;
 
    Map(void* key, size_t key_size, void* value, size_t value_size) : key(key), key_size(key_size), value(value), value_size(value_size) {}
    static bool is_equals(Map *lhs, Map *rhs)
    {
        if (!lhs || !rhs)
        {
            return false;
        }

        if (lhs->key_size != rhs->key_size || lhs->value_size != rhs->value_size)
        {
            return false;
        }

        if (memcmp(lhs->key, rhs->key, lhs->key_size))
        {
            return false;
        }

        if (memcmp(lhs->value, rhs->value, lhs->value_size))
        {
            return false;
        }

        return true;
    }
};

struct ListItem
{
    Map *value;
    ListItem *next;
};

struct LinkedList
{
    ListItem *head;
    size_t size;
};