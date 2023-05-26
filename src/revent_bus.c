/*
 * Change Logs:
 * Date           Author       Notes
 * 2023-05-12     RiceChen     the first version
 */

#include "revent_bus.h"
#include "rthread_pool.h"

typedef struct {
    reb_hub_node *hub_node;
    uint32_t lenght;
    char payload[REB_PAYLOAD_LEN];
} reb_pub_info;

typedef struct {
    pf_list_t hub_list;
    pf_mutex_id mutex;
    rtp thread_pool;
}reb_broker;

static reb_broker g_broker = {0};

static reb_hub_node *reb_find_hub_node(pf_list_t *list, const char *event)
{
    reb_hub_node *hub_node = NULL;
    pf_list_t *event_node = NULL;

    pf_list_for_each(event_node, list) {
        hub_node = pf_list_entry(event_node, reb_hub_node, list);
        if(hub_node != NULL && strcmp(hub_node->event, event) == 0) {
            return hub_node;
        }
    }
    return NULL;
}

static reb_event_node *reb_find_event_node(pf_list_t *event_list, reb_handler_cb handler)
{
    reb_event_node *event_node = NULL;
    pf_list_t *node = NULL;

    pf_list_for_each(node, event_list) {
        event_node = pf_list_entry(node, reb_event_node, list);
        if(event_node != NULL && handler == event_node->handler) {
            return event_node;
        }
    }
    return NULL;
}

pf_err_t reb_subscribe(const char *event, reb_handler_cb handler)
{
    reb_hub_node *hub_node = NULL;
    reb_event_node *event_node = NULL;

    // Lock and add hub node
    pf_mutex_lock(g_broker.mutex);
    hub_node = reb_find_hub_node(&g_broker.hub_list, event);
    if(hub_node == NULL) {
        hub_node = (reb_hub_node *)PF_MALLOC(sizeof(reb_hub_node));
        if(hub_node == NULL) {
            PF_LOGE("Hub node(%s) malloc failed", event);
            return PF_ENOMEM;
        }
        hub_node->mutex = pf_mutex_create();
        if(hub_node->mutex == NULL) {
            PF_LOGE("Hub node(%s) mutex create failed", event);
            return PF_EINVAL;
        }
        memset(hub_node->event, 0, REB_NAME_LEN);
        memcpy(hub_node->event, event, strlen(event));
        pf_list_init(&(hub_node->event_list));
        pf_list_insert_after(&g_broker.hub_list, &(hub_node->list));
    }
    pf_mutex_unlock(g_broker.mutex);

    event_node = reb_find_event_node(&hub_node->event_list, handler);
    if(event_node != NULL) {
        PF_LOGD("Event(%s) already subscribe", event);
        return PF_EOK;
    }

    event_node = (reb_event_node *)PF_MALLOC(sizeof(reb_event_node));
    if(event_node == NULL) {
        PF_LOGE("Event(%s) malloc failed", event);
        return PF_ENOMEM;
    }
    event_node->handler = handler;
    // Lock and add event node
    pf_mutex_lock(hub_node->mutex);
    pf_list_insert_after(&hub_node->event_list, &(event_node->list));
    pf_mutex_unlock(hub_node->mutex);

    PF_LOGD("Event(%s) Subscribe success", event);
    return PF_EOK;
}

pf_err_t reb_unsbscribe(const char *event, reb_handler_cb handler)
{
    reb_hub_node *hub_node = NULL;
    reb_event_node *event_node = NULL;

    hub_node = reb_find_hub_node(&g_broker.hub_list, event);
    if(hub_node == NULL) {
        PF_LOGE("Event(%s) find failed", event);
        return PF_EEMPTY;
    }

    event_node = reb_find_event_node(&(hub_node->event_list), handler);
    if(event_node == NULL) {
        PF_LOGE("Event node(%s) find failed", event);
        return PF_EEMPTY;
    }
    // Lock and delete event node
    pf_mutex_lock(hub_node->mutex);
    pf_list_remove(&(event_node->list));
    PF_FREE(event_node);
    event_node = NULL;
    pf_mutex_unlock(hub_node->mutex);

    // Lock and delete hub node
    pf_mutex_lock(g_broker.mutex);
    if(pf_list_len(&(hub_node->event_list)) == 0) {
        pf_mutex_delete(hub_node->mutex);
        pf_list_remove(&(hub_node->list));
        PF_FREE(hub_node);
        hub_node = NULL;
    }
    pf_mutex_unlock(g_broker.mutex);

    PF_LOGD("Event(%s) Unsubscribe success", event);
    return PF_EOK;
}

static void reb_dispatch_event(reb_pub_info *info)
{
    reb_hub_node *hub_node = NULL;
    reb_event_node *event_node = NULL;
    pf_list_t *node = NULL;

    hub_node = info->hub_node;
    if(hub_node == NULL) {
        PF_LOGE("Hub node is NULL");
        return;
    }
    pf_mutex_lock(hub_node->mutex);
    pf_list_for_each(node, &(hub_node->event_list)) {
        event_node = pf_list_entry(node, reb_event_node, list);
        if(event_node->handler) {
            event_node->handler(hub_node->event, info->payload, info->lenght);
        }
    }
    pf_mutex_unlock(hub_node->mutex);
    PF_FREE(info);
    info = NULL;
}

pf_err_t reb_publish(const char *event, void *payload, uint16_t lenght)
{
    reb_hub_node *hub_node = NULL;

    PF_LOGD("Sync publish event: %s, payload: %.*s", event, lenght, (char *)payload);

    pf_mutex_lock(g_broker.mutex);
    hub_node = reb_find_hub_node(&(g_broker.hub_list), event);
    if(hub_node == NULL) {
        PF_LOGE("Hub node(%s) find failed", event);
        return PF_EEMPTY;
    }
    pf_mutex_unlock(g_broker.mutex);

    reb_pub_info *info = (reb_pub_info *)PF_MALLOC(sizeof(reb_pub_info));
    if(info == NULL) {
        PF_LOGE("Publish info malloc failed");
        return PF_ENOMEM;
    }
    info->hub_node = hub_node;
    info->lenght = lenght;
    memcpy(info->payload, payload, lenght);

    reb_dispatch_event(info);
    return PF_EOK;
}

static void reb_event_pub_handle(void *argv)
{
    reb_pub_info *info = (reb_pub_info *)argv;
    reb_dispatch_event(info);
}

pf_err_t reb_async_publish(const char *event, reb_priority priority, void *payload, uint16_t lenght)
{
    reb_hub_node *hub_node = NULL;

    PF_LOGD("Async publish event: %s, payload: %.*s", event, lenght, (char *)payload);

    pf_mutex_lock(g_broker.mutex);
    hub_node = reb_find_hub_node(&(g_broker.hub_list), event);
    if(hub_node == NULL) {
        PF_LOGE("Hub node(%s) find failed", event);
        return PF_EEMPTY;
    }
    pf_mutex_unlock(g_broker.mutex);

    reb_pub_info *info = (reb_pub_info *)PF_MALLOC(sizeof(reb_pub_info));
    if(info == NULL) {
        PF_LOGE("Publish info malloc failed");
        return PF_ENOMEM;
    }
    info->hub_node = hub_node;
    info->lenght = lenght;
    memcpy(info->payload, payload, lenght);

    rtp_add_task(&(g_broker.thread_pool), reb_event_pub_handle, (void *)info);
    return PF_EOK;
}

void reb_info_dump(void)
{
    reb_hub_node *hub_node = NULL;
    reb_event_node *event_node = NULL;
    pf_list_t *hub_list = NULL;
    pf_list_t *event_list = NULL;
    int event_node_count = 0;

    PF_LOGI("revent bus list:");
    pf_list_for_each(hub_list, &(g_broker.hub_list)) {
        hub_node = pf_list_entry(hub_list, reb_hub_node, list);
        if(hub_node == NULL) {
            continue;
        }
        event_node_count = 0;
        pf_list_for_each(event_list, &(hub_node->event_list)) {
            event_node = pf_list_entry(event_list, reb_event_node, list);
            if(event_node == NULL) {
                continue;
            }
            event_node_count++;
        }
        PF_LOGI("revent bus event(%s), node count: %d", hub_node->event, event_node_count);
    }
}

pf_err_t reb_init(void)
{
    if(rtp_create(&g_broker.thread_pool, "reb", REB_TP_STACK_SIZE, REB_TP_MAX) != PF_EOK) {
        PF_LOGE("Thread pool create failed");
        return PF_ERROR;
    }

    g_broker.mutex = pf_mutex_create();
    if(g_broker.mutex == NULL) {
        PF_LOGE("Hub mutex create faile");
        return PF_EINVAL;
    }

    pf_list_init(&g_broker.hub_list);
    PF_LOGI("revent bus init success");

    return PF_EOK;
}

pf_err_t reb_deinit(void)
{
    pf_mutex_delete(g_broker.mutex);
    rtp_destroy(&g_broker.thread_pool);
    return PF_EOK;
}
