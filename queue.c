#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
#include "time.h"

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

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    int size = q_size(head);
    int iteration = size / k;

    struct list_head *start_node = head;
    for (int i = 0; i < iteration; ++i) {
        struct list_head *node = start_node->next;
        struct list_head *end_node;
        /* Modiy prev pointers */
        for (int j = 0; j < k - 1; ++j) {
            node->prev = node->next;
            node = node->next;
        }
        node->prev = start_node;
        end_node = node->next;
        end_node->prev = start_node->next;
        /* Modify next pointers */
        node = end_node->prev;
        node->next = end_node;
        for (int j = 0; j < k - 1; ++j) {
            struct list_head *tmp = node;
            node = node->prev;
            node->next = tmp;
        }
        start_node->next = node;
        /* Reset start_node */
        start_node = end_node->prev;
    }
}

struct list_head *merge_two_modified_lists(struct list_head *head1,
                                           struct list_head *head2)
{
    if (!head1 && !head2) {
        return NULL;
    }

    struct list_head *node1 = head1, *node2 = head2;
    struct list_head *head = NULL;
    struct list_head **node = &head;
    struct list_head **node_prev = &head;
    struct list_head *tail1 = NULL;
    if (head1) {
        tail1 = head1->prev;
    }
    struct list_head *tail2 = NULL;
    if (head2) {
        tail2 = head2->prev;
    }

    while (node1 && node2) {
        element_t *element1 = list_entry(node1, element_t, list);
        element_t *element2 = list_entry(node2, element_t, list);
        struct list_head **old_node =
            strcmp(element1->value, element2->value) <= 0 ? &node1 : &node2;
        *node = *old_node;
        (*node)->prev = *node_prev;
        *old_node = (*old_node)->next;
        node_prev = node;
        node = &(*node)->next;
    }
    *node = node1 ? node1 : node2;
    (*node)->prev = *node_prev;
    head->prev = node1 ? tail1 : tail2;
    return head;
}

struct list_head *sort_recur(struct list_head *head)
{
    if (!head->next) {
        return head;
    }

    struct list_head *slow = head;
    for (struct list_head *fast = slow; fast && fast->next;
         fast = fast->next->next) {
        slow = slow->next;
    }
    struct list_head *mid = slow;
    struct list_head *left_tail = mid->prev;
    struct list_head *right_tail = head->prev;
    left_tail->next = NULL;
    head->prev = left_tail;
    mid->prev = right_tail;
    struct list_head *left = sort_recur(head), *right = sort_recur(mid);
    return merge_two_modified_lists(left, right);
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (!head || list_empty(head)) {
        return;
    }

    struct list_head *modified_list = head->next;
    struct list_head *tail = head->prev;
    modified_list->prev = tail;
    tail->next = NULL;

    modified_list = sort_recur(modified_list);

    head->next = modified_list;
    tail = modified_list->prev;
    modified_list->prev = head;
    head->prev = tail;
    tail->next = head;
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    struct list_head *node = head->next;

    while (node->next != head) {
        element_t *element = list_entry(node, element_t, list);
        struct list_head *next_node = node->next;
        struct list_head *node_to_the_right = next_node;
        while (node_to_the_right != head) {
            element_t *element_to_the_right =
                list_entry(node_to_the_right, element_t, list);
            if (strcmp(element_to_the_right->value, element->value) > 0) {
                node->prev->next = next_node;
                next_node->prev = node->prev;
                free(element->value);
                free(element);
                break;
            }
            node_to_the_right = node_to_the_right->next;
        }
        node = next_node;
    }

    return 0;
}

struct list_head *merge_two_lists(struct list_head *head,
                                  struct list_head *merged_head)
{
    struct list_head *node1 = head->next, *node2 = merged_head->next;
    struct list_head **indirect_node = NULL;
    struct list_head *prev_node = head;

    while (node1 != head && node2 != merged_head) {
        element_t *element1 = list_entry(node1, element_t, list);
        element_t *element2 = list_entry(node2, element_t, list);
        indirect_node =
            strcmp(element1->value, element2->value) <= 0 ? &node1 : &node2;
        (*indirect_node)->prev = prev_node;
        prev_node->next = *indirect_node;
        prev_node = *indirect_node;
        *indirect_node = (*indirect_node)->next;
    }

    indirect_node = node1 == head ? &node2 : &node1;
    (*indirect_node)->prev = prev_node;
    prev_node->next = (*indirect_node);
    if (node1 == head) {
        merged_head->prev->next = head;
        head->prev = merged_head->prev;
    }
    merged_head->next = merged_head;
    merged_head->prev = merged_head;

    return head;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    struct list_head *first_node = head->next;
    struct list_head *node = first_node->next;
    queue_contex_t *first_ctx = list_entry(first_node, queue_contex_t, chain);

    while (node != head) {
        queue_contex_t *node_ctx = list_entry(node, queue_contex_t, chain);
        first_ctx->q = merge_two_lists(first_ctx->q, node_ctx->q);
        node = node->next;
    }

    return q_size(first_ctx->q);
}

void q_shuffle(struct list_head *head)
{
    if (head == NULL)
        return;

    int size = q_size(head);

    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    long long nanoseconds = ts.tv_sec * 1000000000LL + ts.tv_nsec;

    srand(nanoseconds);

    for (int i = 0; i < size - 1; ++i) {
        int j = i + rand() % (size - i);

        struct list_head *node_i = head->next, *prev_i = NULL, *next_i = NULL;
        int k = 0;
        for (; k < i; ++k) {
            node_i = node_i->next;
        }
        prev_i = node_i->prev;
        next_i = node_i->next;

        struct list_head *node_j = node_i, *prev_j = NULL, *next_j = NULL;
        for (; k < j; ++k) {
            node_j = node_j->next;
        }
        prev_j = node_j->prev;
        next_j = node_j->next;

        if (next_i == node_j) {
            next_i = node_i;
            prev_j = node_j;
        }

        if (node_i != node_j) {
            prev_i->next = node_j;
            node_j->prev = prev_i;
            node_j->next = next_i;
            next_i->prev = node_j;

            prev_j->next = node_i;
            node_i->prev = prev_j;
            node_i->next = next_j;
            next_j->prev = node_i;
        }
    }
}