/*
 * Change Logs:
 * Date           Author       Notes
 * 2023-10-10     RiceChen     the first version
 */

#ifndef __REB_DEF_H__
#define __REB_DEF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef enum {
    REB_OK = 0,                 /**< There is no error */
    REB_ERROR,                   /**< A generic error happens */
    REB_FULL,                   /**< The resource is full */
    REB_EMPTY,                  /**< The resource is empty */
    REB_NOMEM,                  /**< No memory */
    REB_INVAL,                  /**< Invalid argument */
} reb_status;

#define REB_INLINE              static __inline

/**
 * memory API
*/
#ifndef REB_MALLOC
    #define REB_MALLOC          malloc
#endif

#ifndef REB_FREE
    #define REB_FREE            free
#endif

/**
 * debug API
*/
#define REB_LOGE_EN             1
#define REB_LOGD_EN             1
#define REB_LOGI_EN             1

#ifndef REB_PRINT
    #define REB_PRINT           printf
#endif

#ifndef REB_PRINT_TAG
    #define REB_PRINT_TAG       "REB"
#endif

#if REB_LOGE_EN
#define REB_LOGE(...)           REB_PRINT("\033[31;22m[E/%s](%s:%d) ", REB_PRINT_TAG, __FUNCTION__, __LINE__); \
                                REB_PRINT(__VA_ARGS__); \
                                REB_PRINT("\033[0m\n")
#else
#define REB_LOGE(x)
#endif

#if REB_LOGI_EN
#define REB_LOGI(...)           REB_PRINT("\033[32;22m[I/%s](%s:%d) ", REB_PRINT_TAG, __FUNCTION__, __LINE__); \
                                REB_PRINT(__VA_ARGS__); \
                                REB_PRINT("\033[0m\n")
#else
#define REB_LOGI(x)
#endif

#if REB_LOGD_EN
#define REB_LOGD(...)           REB_PRINT("[D/%s](%s:%d) ", REB_PRINT_TAG, __FUNCTION__, __LINE__); \
                                REB_PRINT(__VA_ARGS__);   
#else
#define REB_LOGD(x)
#endif

typedef uint32_t reb_time_t;

/**
 * Task API
*/
typedef void *reb_task_id;

typedef struct reb_task_attr{
    char *name;                 // name of the task
    uint32_t stack_size;        // size of stack
    uint8_t priority;           // task priority
} reb_task_attr;

typedef void(*reb_task_func)(void *arg);

reb_task_id reb_task_create(reb_task_func func, void *arg, reb_task_attr *attr);
void reb_task_delete(reb_task_id task);

/**
 * Mutex API
*/
typedef void *reb_mutex_id;

reb_mutex_id reb_mutex_create(void);
reb_status reb_mutex_lock(reb_mutex_id mutex);
reb_status reb_mutex_unlock(reb_mutex_id mutex);
void reb_mutex_delete(reb_mutex_id mutex);

/**
 * Sem API
*/
typedef void *reb_sem_id;

reb_sem_id reb_sem_create(uint32_t value);
reb_status reb_sem_lock(reb_sem_id sem, reb_time_t time);
reb_status reb_sem_unlock(reb_sem_id sem);
void reb_sem_delete(reb_sem_id sem);

/**
 * List Api
*/
struct reb_list_node {
    struct reb_list_node *next;
    struct reb_list_node *prev;
};
typedef struct reb_list_node reb_list_t;

REB_INLINE void reb_list_init(reb_list_t *l)
{
    l->next = l->prev = l;
}

REB_INLINE void reb_list_insert_after(reb_list_t *l, reb_list_t *n)
{
    l->next->prev = n;
    n->next = l->next;
    l->next = n;
    n->prev = l;
}

REB_INLINE void reb_list_insert_before(reb_list_t *l, reb_list_t *n)
{
    l->prev->next = n;
    n->prev = l->prev;
    l->prev = n;
    n->next = l;
}

REB_INLINE void reb_list_remove(reb_list_t *n)
{
    n->next->prev = n->prev;
    n->prev->next = n->next;
    n->next = n->prev = n;
}

REB_INLINE int reb_list_is_empty(const reb_list_t *l)
{
    return l->next == l;
}

REB_INLINE int reb_list_len(const reb_list_t *l)
{
    int len = 0;
    const reb_list_t *p = l;
    while (p->next != l) {
        p = p->next;
        len ++;
    }
    return len;
}

#define reb_container_of(ptr, type, member)                                     \
    ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))

#define reb_list_obj_init(obj) {&(obj), &(obj)}

#define reb_list_entry(node, type, member)                                      \
    reb_container_of(node, type, member)

#define reb_list_for_each(pos, head)                                            \
    for (pos = (head)->next; pos != (head); pos = pos->next)

#define REB_ALL_MINOR_TYPE                                  (0xFFFF)

#define REB_MK_EVENT_TYPE(mojor, minor) \
                                (((uint32_t)(mojor) << 16) | ((uint32_t)(minor) & 0xFFFF))
#define REB_EVENT_MOJOR_TYPE(event)                         ((uint16_t)((event) >> 16))  
#define REB_EVENT_MINOR_TYPE(event)                         ((uint32_t)(event) & 0xFFFF)
#define REB_EVENT_TYPE_MOJOR_CMP(pub_event, sub_event)      (!((pub_event ^ sub_event) & 0xFFFF0000))

#ifdef __cplusplus
}
#endif

#endif
