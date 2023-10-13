/*
 * Change Logs:
 * Date           Author       Notes
 * 2023-10-10     RiceChen     the first version
 */

#ifndef __REB_CFG_H__
#define __REB_CFG_H__

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * @brief The stack size for the publisher to handle event tasks. 
 */
#define REB_PUBLISHER_TASK_STACK_SIZE                 (2 * 1024)

/** 
 * @brief The priority for the publisher to handle event tasks.
 */
#define REB_PUBLISHER_TASK_PRIO                       (15)

/**
 * @brief Number of message buffers processed by the publisher for events.
 */
#define REB_PUBLISHER_QUEUE_MSG_COUNT                 (10)

#ifdef __cplusplus
}
#endif

#endif
