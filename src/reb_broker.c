/*
 * Change Logs:
 * Date           Author       Notes
 * 2023-10-10     RiceChen     the first version
 */

#include "reb_broker.h"
#include "reb_publisher.h"
#include "reb_def.h"

static broker_base base;

static bool brober_event_type_cmp(uint32_t pub_event, uint32_t obs_event)
{
    if (REB_EVENT_TYPE_MOJOR_CMP(pub_event, obs_event) && 
        (REB_EVENT_MINOR_TYPE(obs_event) == REB_ALL_MINOR_TYPE || pub_event == obs_event))
    {
        return true;
    }
    return false;
}

static reb_status _broker_observer_attach(observer_base *obs, observer_state state)
{
    obs->state = state;
    reb_mutex_lock(base.lock);
    reb_list_insert_after(&base.obs_head, &obs->node);
    reb_mutex_unlock(base.lock);

	REB_LOGD("Broker: attach observer event, event type: 0x%08x", obs->event);
    return REB_OK;
}

reb_status broker_observer_attach_once(observer_base *obs)
{
    return _broker_observer_attach(obs, OBS_STATE_ATTACH_ONCE);
}

reb_status broker_observer_attach(observer_base *obs)
{
    return _broker_observer_attach(obs, OBS_STATE_ATTACH);
}

reb_status broker_observer_detach(observer_base *obs)
{
    reb_mutex_lock(base.lock);
    obs->state = OBS_STATE_DETACH;
    reb_list_remove(&obs->node);
    reb_mutex_unlock(base.lock);

    REB_LOGD("Broker: detach observer event, event type: 0x%08x", obs->event);
    return REB_OK;
}

static void broker_publisher_notify(uint32_t event, uint32_t data, bool need_free)
{
    observer_base *obs = NULL;
    reb_list_t *node = NULL;

    reb_mutex_lock(base.lock);
    reb_list_for_each(node, &base.obs_head) {
        obs = reb_list_entry(node, observer_base, node);
        if(obs == NULL) {
            REB_LOGE("Broker: find observer node failed");
            continue;
        }

        if(brober_event_type_cmp(event, obs->event) && 
           (obs->state == OBS_STATE_ATTACH || obs->state == OBS_STATE_ATTACH_ONCE)) {
            if(obs->trigger) {
                obs->trigger(obs, event, data);
            }
            if(obs->state == OBS_STATE_ATTACH_ONCE) {
                obs->state = OBS_STATE_IDLE;
            }
        }
    }

    reb_mutex_unlock(base.lock);

    if(need_free) {
        REB_FREE((void *)data);
    }
}

reb_status broker_create(void)
{
    reb_status status = REB_OK;
    base.lock = reb_mutex_create();
    if(base.lock == NULL) {
        REB_LOGE("Broker: create queue failed");
        return REB_ERROR;
    }

    reb_list_init(&base.obs_head);
    status = publisher_factory_create(broker_publisher_notify);
    if(status != REB_OK) {
        REB_LOGE("Broker: Create puvlisher factory failed");
        return status;
    }

    REB_LOGD("Briker: init success");
    return status;
}

reb_status broker_delete(void)
{
    reb_status status = REB_OK;
    
    reb_list_remove(&base.obs_head);
    
    if(base.lock) {
        reb_mutex_delete(base.lock);
    }

    return status;
}
