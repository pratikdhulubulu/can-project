#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Generic queue types.
 */
typedef enum
{
    CAN_HIGH_PRIORITY_QUEUE = 0U,
    CAN_LOW_PRIORITY_QUEUE,
    DATA_LOW_PRIORITY_QUEUE,
    MAX_NUMBER_OF_QUEUES
} QueueType_e;

/**
 * @brief Queue configuration details.
 */
typedef struct
{
    uint8_t *start_address;
    uint16_t queue_size;
    uint16_t message_data_size;
} QueueDetails_t;

/**
 * @brief Queue runtime information.
 */
typedef struct
{
    uint16_t write_index;
    uint16_t read_index;
    uint16_t count;
    uint8_t overflow;
} QueueStats_t;

/**
 * @brief Queue status information.
 */
typedef struct
{
    uint16_t number_of_elements;
    uint8_t queue_full;
    uint8_t queue_empty;
    uint8_t queue_overflow;
} QueueStatus_t;

/**
 * @brief Queue configuration table.
 */
extern const QueueDetails_t QueueDetails[MAX_NUMBER_OF_QUEUES];

/**
 * @brief Queue runtime information table.
 */
extern QueueStats_t QueueStats[MAX_NUMBER_OF_QUEUES];

void queueInit(void);
uint8_t queueEnqueue(QueueType_e queue_type, const void *message, uint16_t message_size);
uint8_t queueDequeue(QueueType_e queue_type, void *message, uint16_t message_size);
uint8_t queueIsEmpty(QueueType_e queue_type);
uint8_t queueIsFull(QueueType_e queue_type);
uint16_t queueGetCount(QueueType_e queue_type);
uint8_t queueGetStatus(QueueType_e queue_type, QueueStatus_t *status);

#endif /* QUEUE_H */