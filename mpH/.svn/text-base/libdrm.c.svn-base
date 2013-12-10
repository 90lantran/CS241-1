/** @file libdrm.c */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include "libdrm.h"
#include "libwfg.h"
#include "queue.h"


/**
 * Set the mode of the deadlock resilient mutex.
 *
 * This function will only be called once, before any of the other libdrm
 * functions.  This functions sets the global state of all drm mutexes.
 *
 * @param mode
 *   One of the following deadlock resilient modes:
 *   - NO_DEADLOCK_CHECKING
 *   - DEADLOCK_PREVENTION
 *   - DEADLOCK_DETECTION
 *   - DEADLOCK_AVOIDANCE
 */
void drm_setmode(enum drmmode_t mode)
{
	
}

/**
 * Frees all memory used by libdrm.
 *
 * This function will be called last, after all mutexes have been destroyed.
 */
void drm_cleanup()
{

}



/**
 * Initialize a deadlock resilient mutex.
 *
 * This function should function similarly to pthread_mutex_init().
 *
 * @param mutex
 *   A deadlock resilient mutex.
 */
void drm_init(drm_t *mutex)
{

}


/**
 * Lock a deadlock resilient mutex.
 *
 * This function should function similarly to pthread_mutex_lock(), except it
 * must also follow the specific rules for the deadlock resilient mode that
 * has been set by the user.
 *
 * When you call the underlying pthread_mutex_lock(), make sure to check its
 * return value.  If pthread_mutex_lock() fails, the lock was not acquired
 * and you must return a failure from your drm_lock() function (don't simply
 * retry again and again).
 *
 * @param mutex
 *   A deadlock resilient mutex.
 *
 * @return
 *   When the lock is successfully acquired, this function returns zero (0).
 *   Otherwise, returns non-zero.
 */
int drm_lock(drm_t *mutex)
{
	return 1;
}


/**
 * Unlock a deadlock resilient mutex.
 *
 * This function should function similarly to pthread_mutex_unlock().
 *
 * @param mutex
 *   A deadlock resilient mutex.
 *
 * @return
 *   When the lock is successfully unlocked, this function returns zero (0).
 *   Otherwise, returns non-zero.
 */
int drm_unlock(drm_t *mutex)
{
	return 1;
}


/**
 * Destroy a deadlock resilient mutex.
 *
 * This function should function similarly to pthread_mutex_destory().
 *
 * @param mutex
 *   A deadlock resilient mutex.
 */
void drm_destroy(drm_t *mutex)
{

}

