/*
 * Change Logs:
 * Date           Author       Notes
 * 2023-05-16     RiceChen     the first version
 */

#ifndef __REVENTBUS_H__
#define __REVENTBUS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "platform_def.h"

#define REB_NAME_LEN                    16
#define REB_PAYLOAD_LEN                 512

#define REB_TP_MAX                      3
#define REB_TP_STACK_SIZE               4096

typedef void (*reb_handler_cb)(const char *event, void *payload, uint16_t lenght);

typedef enum {
    REB_PRIORITY_HIGH,
    REB_PRIORITY_ORDINARY,
}reb_priority;

typedef struct {
    reb_handler_cb handler;
    pf_list_t list;
}reb_event_node;

typedef struct {
    char event[REB_NAME_LEN];
    pf_mutex_id mutex;
    pf_list_t event_list;
    pf_list_t list;
}reb_hub_node;

/**
 * reb init function
*/
pf_err_t reb_init(void);

/**
 * reb deinit function
*/
pf_err_t reb_deinit(void);

/**
 * reb subscribe event function
 *
 * @param event  event name
 * @param handle event handle function
 *
 * @return return subscribe result
*/
pf_err_t reb_subscribe(const char *event, reb_handler_cb handler);

/**
 * reb unsubscribe event function
 *
 * @param event  event name
 * @param handle event handle function
 *
 * @return return unsubscribe result
*/
pf_err_t reb_unsbscribe(const char *event, reb_handler_cb handler);

/**
 * reb synchronization publish event function
 *
 * @param event  event name
 * @param payload publish content
 * @param lenght content lenght
 *
 * @return return publish result
*/
pf_err_t reb_publish(const char *event, void *payload, uint16_t lenght);

/**
 * reb asynchronous publish event function -- Recommend
 *
 * @param event  event name
 * @param priority publish priority
 * @param payload publish content
 * @param lenght content lenght
 *
 * @return return publish result
*/
pf_err_t reb_async_publish(const char *event, reb_priority priority, void *payload, uint16_t lenght);

/**
 * reb infomation dump
*/
void reb_info_dump(void);

#ifdef __cplusplus
}
#endif

#endif
