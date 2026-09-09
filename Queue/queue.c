/**
 * @file    queue.c
 * @author  Pratik Dhulubulu
 * @brief   This file implements queue initialization, enqueue, dequeue, and
 *          status retrieval functions for a generic queue implementation.
 */

#include "queue.h"
#include "can_driver.h"
/**
 * @section Public Constants and Macro Definitions.
 */
/* Queue sizes for each queue */
#define CAN_HIGH_PRIORITY_QUEUE_SIZE    (10U)
#define CAN_LOW_PRIORITY_QUEUE_SIZE     (10U)
#define DATA_LOW_PRIORITY_QUEUE_SIZE    (20U)

/**
 * @section Private Data Declaration.
 */
/* Queue memory allocation for each queue */
static CanMessage_t CanHighPriorityQueue[CAN_HIGH_PRIORITY_QUEUE_SIZE];
static CanMessage_t CanLowPriorityQueue[CAN_LOW_PRIORITY_QUEUE_SIZE];
static uint8_t DataLowPriorityQueue[DATA_LOW_PRIORITY_QUEUE_SIZE];

/* Queue configuration table */
const QueueDetails_t QueueDetails[MAX_NUMBER_OF_QUEUES] =
{
    { (uint8_t *)CanHighPriorityQueue, CAN_HIGH_PRIORITY_QUEUE_SIZE, sizeof(CanMessage_t) },
    { (uint8_t *)CanLowPriorityQueue, CAN_LOW_PRIORITY_QUEUE_SIZE, sizeof(CanMessage_t) },
    { (uint8_t *)DataLowPriorityQueue, DATA_LOW_PRIORITY_QUEUE_SIZE, sizeof(uint8_t) }
};

/* Queue runtime information */
QueueStats_t QueueStats[MAX_NUMBER_OF_QUEUES];

/**
 * @section Private Function Declarations.
 */
static void queueCopyData(uint8_t *destination, const uint8_t *source, uint16_t data_size);
static uint8_t queueEnqueueData(const QueueDetails_t *queue_details_ptr, QueueStats_t *queue_ptr, const void *message);
static uint8_t queueDequeueData(const QueueDetails_t *queue_details_ptr, QueueStats_t *queue_ptr, void *message);

/**
 * @section Public Function Definations.
 */

/**
 * @brief   This function initializes all queues and resets their runtime information.
 * @param   void 
 * @return  void
 */
void queueInit(void)
{
    uint8_t queue_index = 0U;

    for (queue_index = 0U; queue_index < MAX_NUMBER_OF_QUEUES; queue_index++)
    {
        QueueStats[queue_index].write_index = 0U;
        QueueStats[queue_index].read_index = 0U;
        QueueStats[queue_index].count = 0U;
        QueueStats[queue_index].overflow = 0U;
    }
}

/**
 * @brief This function adds an element to the specified queue.
 * @param queue_type The type of the queue to which the element should be added.
 * @param message Pointer to the message to be added to the queue.
 * @param message_size Size of the message to be added.
 * @return 1 if the element was added successfully, 0 otherwise.
 */
uint8_t queueEnqueue(QueueType_e queue_type, const void *message, uint16_t message_size)
{
    if ((message == NULL) || ((uint32_t)queue_type >= (uint32_t)MAX_NUMBER_OF_QUEUES) || (message_size != QueueDetails[queue_type].message_data_size))
    {
        return FALSE;
    }

    return queueEnqueueData(&QueueDetails[queue_type], &QueueStats[queue_type], message);
}

/**
 * @brief Remove an element from the specified queue.
 * @param queue_type The type of the queue from which the element should be removed.
 * @param message Pointer to the buffer where the dequeued message should be stored.
 * @param message_size Size of the message buffer.
 * @return 1 if the element was removed successfully, 0 otherwise.
 */
uint8_t queueDequeue(QueueType_e queue_type, void *message, uint16_t message_size)
{
    if ((message == NULL) || ((uint32_t)queue_type >= (uint32_t)MAX_NUMBER_OF_QUEUES) || (message_size != QueueDetails[queue_type].message_data_size))
    {
        return FALSE;
    }
    
    return queueDequeueData(&QueueDetails[queue_type], &QueueStats[queue_type], message);
}

/**
 * @brief This function checks whether the specified queue is empty.
 * @param queue_type The type of the queue to check.
 * @return 1 if the queue is empty, 0 otherwise.
 */
uint8_t queueIsEmpty(QueueType_e queue_type)
{
    if ((uint32_t)queue_type >= (uint32_t)MAX_NUMBER_OF_QUEUES)
    {
        return TRUE;
    }

    return (QueueStats[queue_type].count == 0U) ? TRUE : FALSE;
}

/**
 * @brief This function checks whether the specified queue is full.
 * @param queue_type The type of the queue to check.
 * @return 1 if the queue is full, 0 otherwise.
 */
uint8_t queueIsFull(QueueType_e queue_type)
{
    if ((uint32_t)queue_type >= (uint32_t)MAX_NUMBER_OF_QUEUES)
    {
        return FALSE;
    }

    return (QueueStats[queue_type].count >= QueueDetails[queue_type].queue_size) ? TRUE : FALSE;
}

/**
 * @brief This function gets the number of elements currently stored in the queue.
 * @param queue_type The type of the queue for which to get the count.
 * @return The number of elements in the queue.
 */
uint16_t queueGetCount(QueueType_e queue_type)
{
    if ((uint32_t)queue_type >= (uint32_t)MAX_NUMBER_OF_QUEUES)
    {
        return 0U;
    }

    return QueueStats[queue_type].count;
}

/**
 * @brief This function gets the complete status information of the specified queue.
 * @param queue_type The type of the queue for which to get the status.
 * @param status Pointer to the structure where the status information should be stored.
 * @return 1 if the status information was retrieved successfully, 0 otherwise.
 */
uint8_t queueGetStatus(QueueType_e queue_type, QueueStatus_t *status)
{
    if ((status == NULL) || ((uint32_t)queue_type >= (uint32_t)MAX_NUMBER_OF_QUEUES))
    {
        return 0U;
    }

    status->number_of_elements = QueueStats[queue_type].count;
    status->queue_full = (QueueStats[queue_type].count >= QueueDetails[queue_type].queue_size) ? 1U : 0U;
    status->queue_empty = (QueueStats[queue_type].count == 0U) ? 1U : 0U;
    status->queue_overflow = QueueStats[queue_type].overflow;

    return 1U;
}

/**
 * @brief This function enqueues data into the queue.
 * @param queue_details_ptr Pointer to the queue details structure.
 * @param queue_ptr Pointer to the queue statistics structure.
 * @param message Pointer to the message to be enqueued.
 * @return 1 if the message was enqueued successfully, 0 otherwise.
 */
static uint8_t queueEnqueueData(const QueueDetails_t *queue_details_ptr, QueueStats_t *queue_ptr, const void *message)
{
    uint8_t *queue_address;

    if (queue_ptr->count >= queue_details_ptr->queue_size)
    {
        queue_ptr->overflow = 1U;
        return 0U;
    }

    queue_address = queue_details_ptr->start_address;
    queue_address = &queue_address[((uint32_t)queue_ptr->write_index * (uint32_t)queue_details_ptr->message_data_size)];
    queueCopyData(queue_address, (const uint8_t *)message, queue_details_ptr->message_data_size);
    queue_ptr->write_index++;

    if (queue_ptr->write_index >= queue_details_ptr->queue_size)
    {
        queue_ptr->write_index = 0U;
    }

    queue_ptr->count++;

    return 1U;
}

/**
 * @brief This function dequeues data from the queue.
 * @param queue_details_ptr Pointer to the queue details structure.
 * @param queue_ptr Pointer to the queue statistics structure.
 * @param message Pointer to the buffer where the dequeued message should be stored.
 * @return 1 if the message was dequeued successfully, 0 otherwise.
 */
static uint8_t queueDequeueData(const QueueDetails_t *queue_details_ptr, QueueStats_t *queue_ptr, void *message)
{
    uint8_t *queue_address;

    if (queue_ptr->count == 0U)
    {
        return 0U;
    }

    queue_address = queue_details_ptr->start_address;
    queue_address = &queue_address[((uint32_t)queue_ptr->read_index * (uint32_t)queue_details_ptr->message_data_size)];
    queueCopyData((uint8_t *)message, queue_address, queue_details_ptr->message_data_size);
    queue_ptr->read_index++;

    if (queue_ptr->read_index >= queue_details_ptr->queue_size)
    {
        queue_ptr->read_index = 0U;
    }

    queue_ptr->count--;

    return 1U;
}

/**
 * @brief This function copies data between memory locations.
 * @param destination Pointer to the destination buffer.
 * @param source Pointer to the source buffer.
 * @param data_size Size of the data to be copied.
 */
static void queueCopyData(uint8_t *destination, const uint8_t *source, uint16_t data_size)
{
    uint16_t index;

    for (index = 0U; index < data_size; index++)
    {
        destination[index] = source[index];
    }
}