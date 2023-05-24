/*
 * Change Logs:
 * Date           Author       Notes
 * 2023-05-12     RiceChen     the first version
 */

#ifndef __REVENTBUS_DEF_H__
#define __REVENTBUS_DEF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef enum {
    REVENTBUS_EOK = 0,                  /**< There is no error */
    REVENTBUS_ERROR,                    /**< A generic error happens */
    REVENTBUS_EFULL,                    /**< The resource is full */
    REVENTBUS_EEMPTY,                   /**< The resource is empty */
    REVENTBUS_ENOMEM,                   /**< No memory */
    REVENTBUS_EINVAL,                   /**< Invalid argument */
}reb_err_t;

#define REVENTBUS_INLINE                static __inline

/**
 * memory API
*/
#ifndef REVENTBUS_MALLOC
    #define REVENTBUS_MALLOC            malloc
#endif

#ifndef REVENTBUS_FREE
    #define REVENTBUS_FREE              free
#endif

#ifndef REVENTBUS_PRINT
    #define REVENTBUS_PRINT             printf
#endif
#define REVENTBUS_LOGE(...)             REVENTBUS_PRINT("\033[31;22m[E/REVENTBUS](%s:%d) ", __FUNCTION__, __LINE__);  \
                                        REVENTBUS_PRINT(__VA_ARGS__);                                            \
                                        REVENTBUS_PRINT("\033[0m\n")
#define REVENTBUS_LOGI(...)             REVENTBUS_PRINT("\033[32;22m[I/REVENTBUS](%s:%d) ", __FUNCTION__, __LINE__);  \
                                        REVENTBUS_PRINT(__VA_ARGS__);                                            \
                                        REVENTBUS_PRINT("\033[0m\n")
#define REVENTBUS_LOGD(...)             REVENTBUS_PRINT("[D/REVENTBUS](%s:%d) ", __FUNCTION__, __LINE__);             \
                                        REVENTBUS_PRINT(__VA_ARGS__);                                            \
                                        REVENTBUS_PRINT("\n")

typedef void *reb_task_id;
typedef void *reb_mutex_id;
typedef void *reb_event_id;

struct reb_task_attr{
    const char *name;       // name of the task
    uint32_t stack_size;     // size of stack
    uint8_t priority;       // initial task priority
};

typedef void(*reb_task_func)(void *arg);

reb_task_id reb_task_create(reb_task_func func, void *arg, const struct reb_task_attr *attr);
void reb_task_delete(reb_task_id thread);

/**
 * Mutex API
*/
reb_mutex_id reb_mutex_create(void);
reb_err_t reb_mutex_lock(reb_mutex_id mutex);
reb_err_t reb_mutex_unlock(reb_mutex_id mutex);
void reb_mutex_delete(reb_mutex_id mutex);

/**
 * Event API
*/
reb_event_id reb_event_create(void);
uint32_t reb_event_recv(reb_event_id event, uint32_t flags);
reb_err_t reb_event_send(reb_event_id event, uint32_t flags);
void reb_event_delete(reb_event_id event);

struct reb_list_node {
    struct reb_list_node *next;
    struct reb_list_node *prev;
};
typedef struct reb_list_node reb_list_t;

REVENTBUS_INLINE void reb_list_init(reb_list_t *l)
{
    l->next = l->prev = l;
}

REVENTBUS_INLINE void reb_list_insert_after(reb_list_t *l, reb_list_t *n)
{
    l->next->prev = n;
    n->next = l->next;
    l->next = n;
    n->prev = l;
}

REVENTBUS_INLINE void reb_list_insert_before(reb_list_t *l, reb_list_t *n)
{
    l->prev->next = n;
    n->prev = l->prev;
    l->prev = n;
    n->next = l;
}

REVENTBUS_INLINE void reb_list_remove(reb_list_t *n)
{
    n->next->prev = n->prev;
    n->prev->next = n->next;
    n->next = n->prev = n;
}

REVENTBUS_INLINE int reb_list_is_empty(const reb_list_t *l)
{
    return l->next == l;
}

REVENTBUS_INLINE int reb_list_Len(const reb_list_t *l)
{
    int len = 0;
    const reb_list_t *p = l;
    while (p->next != l) {
        p = p->next;
        len ++;
    }
    return len;
}

#define reb_container_of(ptr, type, member)                                   \
    ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))

#define reb_list_obj_init(obj) {&(obj), &(obj)}

#define reb_list_entry(node, type, member)                                    \
    reb_container_of(node, type, member)

#define reb_list_for_each(pos, head)                                           \
    for (pos = (head)->next; pos != (head); pos = pos->next)


#ifdef __cplusplus
}
#endif

#endif
