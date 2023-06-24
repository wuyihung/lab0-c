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
void q_free(struct list_head *l)
{
    if (!l) {
        return;
    }
    struct list_head *node = NULL;
    element_t *entry = NULL;
    struct list_head tmp;
    list_for_each (node, l) {
        entry = list_entry(node, element_t, list);
        free(entry->value);
        tmp.next = node->next;
        free(entry);
        node = &tmp;
    }
    free(l);
}

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
    if (!head || list_empty(head)) {
        return NULL;
    }
    struct list_head *removed = head->next;
    struct list_head *next = removed->next;
    head->next = next;
    next->prev = head;
    element_t *removed_element = list_entry(removed, element_t, list);
    if (sp) {
        size_t legal_bufsize = bufsize > strlen(removed_element->value) + 1
                                   ? strlen(removed_element->value) + 1
                                   : bufsize;
        strlcpy(sp, removed_element->value, legal_bufsize);
    }
    return removed_element;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head)) {
        return NULL;
    }
    struct list_head *removed = head->prev;
    struct list_head *prev = removed->prev;
    head->prev = prev;
    prev->next = head;
    element_t *removed_element = list_entry(removed, element_t, list);
    if (sp) {
        size_t legal_bufsize = bufsize > strlen(removed_element->value) + 1
                                   ? strlen(removed_element->value) + 1
                                   : bufsize;
        strlcpy(sp, removed_element->value, legal_bufsize);
    }
    return removed_element;
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
    if (!head || list_empty(head)) {
        return false;
    }
    struct list_head **indirect = &head->next;
    for (struct list_head *fast = *indirect; fast != head && fast->next != head;
         fast = fast->next->next) {
        indirect = &(*indirect)->next;
    }
    struct list_head *deleted = *indirect;
    *indirect = (*indirect)->next;
    element_t *deleted_element = list_entry(deleted, element_t, list);
    free(deleted_element->value);
    free(deleted_element);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    if (!head) {
        return false;
    }

    struct list_head *node;
    bool is_node_dup = false;
    list_for_each (node, head) {
        struct list_head *next = node->next;
        element_t *element = list_entry(node, element_t, list);
        bool is_next_dup =
            next != head &&
            strcmp(element->value, list_entry(next, element_t, list)->value) ==
                0;
        if (is_next_dup || is_node_dup) {
            struct list_head *prev = node->prev;
            prev->next = next;
            next->prev = prev;
            free(element->value);
            free(element);
            node = prev;
        }
        is_node_dup = is_next_dup;
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    if (!head || list_empty(head)) {
        return;
    }
    struct list_head *prev, *next, *next_next;
    for (struct list_head *node = head->next;
         node != head && node->next != head; node = node->next) {
        prev = node->prev;
        next = node->next;
        next_next = next->next;
        prev->next = next;
        next->prev = prev;
        next->next = node;
        node->prev = next;
        node->next = next_next;
        next_next->prev = node;
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head)) {
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

static struct list_head *merge(struct list_head *a, struct list_head *b)
{
    struct list_head *head = NULL, **tail = &head;

    for (;;) {
        element_t *a_entry = list_entry(a, element_t, list);
        element_t *b_entry = list_entry(b, element_t, list);
        if (strcmp(a_entry->value, b_entry->value) <= 0) {
            *tail = a;
            tail = &a->next;
            a = a->next;
            if (!a) {
                *tail = b;
                break;
            }
        } else {
            *tail = b;
            tail = &b->next;
            b = b->next;
            if (!b) {
                *tail = a;
                break;
            }
        }
    }
    return head;
}

static void merge_final(struct list_head *head,
                        struct list_head *a,
                        struct list_head *b)
{
    struct list_head *tail = head;

    for (;;) {
        element_t *a_entry = list_entry(a, element_t, list);
        element_t *b_entry = list_entry(b, element_t, list);
        if (strcmp(a_entry->value, b_entry->value) <= 0) {
            tail->next = a;
            a->prev = tail;
            tail = a;
            a = a->next;
            if (!a)
                break;
        } else {
            tail->next = b;
            b->prev = tail;
            tail = b;
            b = b->next;
            if (!b) {
                b = a;
                break;
            }
        }
    }

    tail->next = b;
    do {
        b->prev = tail;
        tail = b;
        b = b->next;
    } while (b);

    tail->next = head;
    head->prev = tail;
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head)
{
    if (!head)
        return;

    struct list_head *list = head->next, *pending = NULL;
    size_t count = 0;

    if (list == head->prev)
        return;

    head->prev->next = NULL;

    do {
        size_t bits;
        struct list_head **tail = &pending;

        for (bits = count; bits & 1; bits >>= 1)
            tail = &(*tail)->prev;

        if (bits) {
            struct list_head *a = *tail, *b = a->prev;
            a = merge(b, a);
            a->prev = b->prev;
            *tail = a;
        }

        list->prev = pending;
        pending = list;
        list = list->next;
        pending->next = NULL;
        count++;
    } while (list);

    list = pending;
    pending = pending->prev;
    for (;;) {
        struct list_head *next = pending->prev;

        if (!next)
            break;
        list = merge(pending, list);
        pending = next;
    }
    merge_final(head, pending, list);
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending order */
int q_merge(struct list_head *head)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
