/**
 * \file queue.h
 *
 * \brief A FIFO queue implementation.
 *
 * \copyright (C) Copyright 2022 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

/**
 * \brief Object type to handle all queue operations.
 */
typedef struct queue_list queue_t;

/**
 * \brief Initialize a new queue.
 *
 * This function prepares a queue prior use. The provided \p item_size must be
 * greater than 0.
 *
 * \param[in] item_size Size of each item to be stored in the queue.
 *
 * \return Pointer to the initialized queue, or NULL if initialization failed.
 */
queue_t*
queue_init(size_t item_size);

/**
 * \brief Enqueues an item.
 *
 * This function copies the data of an item into the queue.
 *
 * \pre The given queue \p q must be valid, i.e., initialized with
 *      \ref queue_init() and \p item must point to a valid memory location.
 *
 * \param[in,out] q    Pointer to the queue.
 * \param[in]     item Pointer to the item to be enqueued.
 *
 * \retval true  Success.
 * \retval false Failure, e.g., memory allocation failure.
 */
bool
queue_enqueue(queue_t* q, const void* item);

/**
 * \brief Dequeues oldest item.
 *
 * This function copies the data of the oldest item of the queue to the given
 * location \p item and removed it from the queue.
 *
 * \pre The given queue \p q must be valid, i.e., initialized with
 *      \ref queue_init() and \p item must point to a valid memory location.
 *
 * \param[in,out] q    Poiner to the queue.
 * \param[out]    item Pointer to store the enqueued item.
 *
 * \retval true  Success.
 * \retval false Failure, queue empty.
 */
bool
queue_dequeue(queue_t* q, void* item);

/**
 * \brief Peek at the oldest item of the queue without removing it.
 *
 * This function copies the data of the oldest item of the queue to the given
 * location \p item without removing it from the queue.
 *
 * \pre The given queue \p q must be valid, i.e., initialized with
 *      \ref queue_init() and \p item must point to a valid memory location.
 *
 * \param[in]  q    Pointer to the queue.
 * \param[out] item Pointer to store the peeked item.
 *
 * \retval true  Success.
 * \retval false Failure, queue empty.
 */
bool
queue_peek(const queue_t* q, void* item);

/**
 * \brief Returns the number of items in the queue.
 *
 * \pre The given queue \p q must be valid, i.e., initialized with
 *      \ref queue_init().
 *
 * \param[in] q Pointer to the queue.
 *
 * \return Number of items in the queue.
 */
size_t
queue_count(const queue_t* q);

/**
 * \brief Destroys the queue and frees the allocated memory.
 *
 * \param[in,out] q Pointer to the queue.
 */
void
queue_destroy(queue_t* q);

#ifdef __cplusplus
}
#endif

#endif /* QUEUE_H_ */
