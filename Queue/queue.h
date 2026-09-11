/**
 * @file    queue.h
 * @author  Pratik Dhulubulu
 * @brief   Queue Interface Header File.
 *          This file contains the interface for a generic queue implementation.
 */

#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>

/**
 * @section Public Constants and Macro Definitions.
 */
#define NULL ((void *)0)

/**
 * @section Public Type Declaration.
 */

/* Generic queue types. */
typedef enum
{
    CAN_HIGH_PRIORITY_QUEUE = 0U,
    CAN_LOW_PRIORITY_QUEUE,
    DATA_LOW_PRIORITY_QUEUE,
    MAX_NUMBER_OF_QUEUES
} QueueType_e;

/* Queue configuration details for each queue. */
typedef struct
{
    uint8_t *start_address;
    uint16_t queue_size;
    uint16_t message_data_size;
} QueueDetails_t;

/* Queue runtime information for each queue. */
typedef struct
{
    uint16_t write_index;
    uint16_t read_index;
    uint16_t count;
    uint8_t overflow;
} QueueStats_t;

/* Queue status information for each queue. */
typedef struct
{
    uint16_t number_of_elements;
    uint8_t queue_full;
    uint8_t queue_empty;
    uint8_t queue_overflow;
} QueueStatus_t;

/**
 * @section Public Data Declaration.
 */
/* Queue configuration table for all queues. */
extern const QueueDetails_t QueueDetails[MAX_NUMBER_OF_QUEUES];

/* Queue runtime information table for all queues. */
extern QueueStats_t QueueStats[MAX_NUMBER_OF_QUEUES];

/**
 * @section Public Function Declarations.
 */
void queueInit(void);
uint8_t queueEnqueue(QueueType_e queue_type, const void *message, uint16_t message_size);
uint8_t queueDequeue(QueueType_e queue_type, void *message, uint16_t message_size);
uint8_t queueIsEmpty(QueueType_e queue_type);
uint8_t queueIsFull(QueueType_e queue_type);
uint16_t queueGetCount(QueueType_e queue_type);
uint8_t queueGetStatus(QueueType_e queue_type, QueueStatus_t *status);

#endif /* QUEUE_H */