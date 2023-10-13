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
    reb_mutex_id lock;
} broker_base;

reb_status broker_create(void);

reb_status broker_delete(void);

reb_status broker_observer_attach_once(observer_base *obs);

reb_status broker_observer_attach(observer_base *obs);

reb_status broker_observer_detach(observer_base *obs);

#ifdef __cplusplus
}
#endif

#endif
