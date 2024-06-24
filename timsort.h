#ifndef TIMSORT_H
#define TIMSORT_H

typedef int (*list_cmp_func_t)(void *,
                               const struct list_head *,
                               const struct list_head *);

void timsort(void *priv, struct list_head *head, list_cmp_func_t cmp);

#endif /* TIMSORT_H */
