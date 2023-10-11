/*
 * Change Logs:
 * Date           Author       Notes
 * 2023-10-10     RiceChen     the first version
 */

#ifndef __REB_BROKER_H__
#define __REB_BROKER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "reb_observer.h"

typedef struct {
    int a;
} broker_base;

int broker_create(void);

int broker_delete(void);

int broker_observer_attach_once(observer_base *obs);

int broker_observer_attach(observer_base *obs);

int broker_observer_detach(observer_base *obs);

int broker_publisher_send(uint16_t type, uint16_t sub_type, void *data, reb_time_t timeout);

#ifdef __cplusplus
}
#endif

#endif
