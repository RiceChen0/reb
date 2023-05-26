/*
 * Change Logs:
 * Date           Author       Notes
 * 2023-05-12     RiceChen     the first version
 */

#ifndef __PLATFORM_DEF_H__
#define __PLATFORM_DEF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef enum {
    PF_EOK = 0,                         /**< There is no error */
    PF_ERROR,                           /**< A generic error happens */
    PF_EFULL,                           /**< The resource is full */
    PF_EEMPTY,                          /**< The resource is empty */
    PF_ENOMEM,                          /**< No memory */
    PF_EINVAL,                          /**< Invalid argument */
}pf_err_t;

#define PF_INLINE                       static __inline

/**
 * memory API
*/
#ifndef PF_MALLOC
    #define PF_MALLOC                   malloc
#endif

#ifndef PF_FREE
    #define PF_FREE                     free
#endif

#ifndef PF_PRINT
    #define PF_PRINT                    printf
#endif

#ifndef PF_PRINT_TAG
    #define PF_PRINT_TAG                "PF"
#endif

#define PF_LOGE(...)                    PF_PRINT("\033[31;22m[E/%s](%s:%d) ", PF_PRINT_TAG, __FUNCTION__, __LINE__);     \
                                        PF_PRINT(__VA_ARGS__);                                                           \
                                        PF_PRINT("\033[0m\n")
#define PF_LOGI(...)                    PF_PRINT("\033[32;22m[I/%s](%s:%d) ", PF_PRINT_TAG, __FUNCTION__, __LINE__);     \
                                        PF_PRINT(__VA_ARGS__);                                                           \
                                        PF_PRINT("\033[0m\n")
#define PF_LOGD(...)                    PF_PRINT("[D/%s](%s:%d) ", PF_PRINT_TAG, __FUNCTION__, __LINE__);                \
                                        PF_PRINT(__VA_ARGS__);                                                           \
                                        PF_PRINT("\n")

typedef void *pf_task_id;
typedef void *pf_mutex_id;
typedef void *pf_event_id;
typedef void *pf_sem_id;

struct pf_task_attr{
    char *name;       // name of the task
    uint32_t stack_size;     // size of stack
    uint8_t priority;       // initial task priority
};

typedef void(*pf_task_func)(void *arg);

pf_task_id pf_task_create(pf_task_func func, void *arg, const struct pf_task_attr *attr);
void pf_task_delete(pf_task_id thread);

/**
 * Mutex API
*/
pf_mutex_id pf_mutex_create(void);
pf_err_t pf_mutex_lock(pf_mutex_id mutex);
pf_err_t pf_mutex_unlock(pf_mutex_id mutex);
void pf_mutex_delete(pf_mutex_id mutex);

/**
 * Sem API
*/
pf_sem_id pf_sem_create(uint32_t value);
pf_err_t pf_sem_lock(pf_sem_id sem);
pf_err_t pf_sem_unlock(pf_sem_id sem);
void pf_sem_delete(pf_sem_id sem);

/**
 * Event API
*/
pf_event_id pf_event_create(void);
uint32_t pf_event_recv(pf_event_id event, uint32_t flags);
pf_err_t pf_event_send(pf_event_id event, uint32_t flags);
void pf_event_delete(pf_event_id event);

struct pf_list_node {
    struct pf_list_node *next;
    struct pf_list_node *prev;
};
typedef struct pf_list_node pf_list_t;

PF_INLINE void pf_list_init(pf_list_t *l)
{
    l->next = l->prev = l;
}

PF_INLINE void pf_list_insert_after(pf_list_t *l, pf_list_t *n)
{
    l->next->prev = n;
    n->next = l->next;
    l->next = n;
    n->prev = l;
}

PF_INLINE void pf_list_insert_before(pf_list_t *l, pf_list_t *n)
{
    l->prev->next = n;
    n->prev = l->prev;
    l->prev = n;
    n->next = l;
}

PF_INLINE void pf_list_remove(pf_list_t *n)
{
    n->next->prev = n->prev;
    n->prev->next = n->next;
    n->next = n->prev = n;
}

PF_INLINE int pf_list_is_empty(const pf_list_t *l)
{
    return l->next == l;
}

PF_INLINE int pf_list_len(const pf_list_t *l)
{
    int len = 0;
    const pf_list_t *p = l;
    while (p->next != l) {
        p = p->next;
        len ++;
    }
    return len;
}

#define pf_container_of(ptr, type, member)                                   \
    ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))

#define pf_list_obj_init(obj) {&(obj), &(obj)}

#define pf_list_entry(node, type, member)                                    \
    pf_container_of(node, type, member)

#define pf_list_for_each(pos, head)                                           \
    for (pos = (head)->next; pos != (head); pos = pos->next)


#ifdef __cplusplus
}
#endif

#endif
