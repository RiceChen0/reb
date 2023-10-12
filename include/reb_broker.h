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
    reb_list_t obs_head;
} broker_base;

int broker_create(void);

int broker_delete(void);

int broker_observer_attach_once(observer_base *obs);

int broker_observer_attach(observer_base *obs);

int broker_observer_detach(observer_base *obs);

#ifdef __cplusplus
}
#endif

#endif
