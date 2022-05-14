#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */

#ifndef strlcpy
#define strlcpy(dst, src, sz) snprintf((dst), (sz), "%s", (src))
#endif

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head =
        (struct list_head *) malloc(sizeof(struct list_head));
    if (!head) {
        return NULL;
    }
    INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *l) {}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head) {
        return false;
    }
    element_t *inserted_element = (element_t *) malloc(sizeof(element_t));
    if (!inserted_element) {
        return false;
    }
    if (s) {
        inserted_element->value = (char *) malloc(strlen(s) + 1);
        if (!inserted_element->value) {
            free(inserted_element);
            return false;
        }
        strlcpy(inserted_element->value, s, strlen(s) + 1);
    }
    struct list_head *next = head->next;
    inserted_element->list.next = next;
    inserted_element->list.prev = head;
    head->next = &inserted_element->list;
    next->prev = &inserted_element->list;
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head) {
        return false;
    }
    element_t *inserted_element = (element_t *) malloc(sizeof(element_t));
    if (!inserted_element) {
        return false;
    }
    if (s) {
        inserted_element->value = (char *) malloc(strlen(s) + 1);
        if (!inserted_element->value) {
            free(inserted_element);
            return false;
        }
        strlcpy(inserted_element->value, s, strlen(s) + 1);
    }
    struct list_head *prev = head->prev;
    inserted_element->list.next = head;
    inserted_element->list.prev = prev;
    prev->next = &inserted_element->list;
    head->prev = &inserted_element->list;
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    return NULL;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    return NULL;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int len = 0;
    struct list_head *li;

    list_for_each (li, head)
        len++;
    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!q_size(head)) {
        return;
    }

    struct list_head *node, *tmp;
    for (node = head->next; node != head; node = node->prev) {
        tmp = node->next;
        node->next = node->prev;
        node->prev = tmp;
    }
    tmp = head->next;
    head->next = head->prev;
    head->prev = tmp;
}

/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head) {}
