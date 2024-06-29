#include <stdexcept>
#include "TableAbstract.h"
#include "Mem.h"
#include "List.h"



void deleteMap(Map **elem)
{
    if (elem && *elem)
    {
        free((*elem)->key);
        free((*elem)->value);
        (*elem)->key = NULL;
        (*elem)->value = NULL;
        free(*elem);
        *elem = NULL;
    }
}

LinkedList *createLinkedList()
{
    LinkedList *lst = (LinkedList *)malloc(sizeof(LinkedList));
    lst->size = 0;
    lst->head = NULL;
    return lst;
}

void deleteLinkedList(LinkedList **lst)
{
    if (!lst || !*lst)
    {
        return;
    }

    if ((*lst)->head == NULL)
    {
        free(*lst);
        return;
    }

    ListItem *head = (*lst)->head;
    ListItem *next = NULL;
    while (head != NULL)
    {
        deleteMap(&(head->value));
        next = head->next;
        free(head);
        head = next;
    }
    free(*lst);
    *lst = NULL;
}

void pushBack(LinkedList *lst, Map *map)
{
    ListItem *item = (ListItem *)malloc(sizeof(ListItem));
    item->value = map;
    item->next = NULL;
    ++lst->size;
    if (!lst->head)
    {
        lst->head = item;
        return;
    }

    ListItem *iter = lst->head;
    while (iter->next)
    {
        iter = iter->next;
    }
    iter->next = item;
}

bool erase(LinkedList *lst, void *key, size_t keySize)
{
    if (!lst->head)
    {
        return false;
    }

    ListItem *iter = lst->head;
    ListItem *prev = NULL;

    while (iter)
    {
        if (iter->value->key_size == keySize && !memcmp(iter->value->key, key, keySize))
        {
            if (iter == lst->head)
            {
                lst->head = iter->next;
            }
            else
            {
                prev->next = iter->next;
            }

            deleteMap(&iter->value);
            free(iter);
            --lst->size;
            return true;
        }
        prev = iter;
        iter = iter->next;
    }
    return false;
}
