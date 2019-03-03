#ifndef __LIST_MT_H
#define __LIST_MT_H

/**
 * Define an opaque struct so users
 * are not tempted to fiddle with the bits
 * see https://en.wikipedia.org/wiki/Opaque_pointer for more
 * information regarding opaque structs.
 * We will declare these in the headers and define them
 * in a c file.
 */
struct list_mt;

/**
 * Initilizes the lst struct to a sane starting state.
 * The user will need to pass a reference to a valid
 * memory location. This can be heap allocated stack
 * allocated or in the data segment.
 * @return NULL on error
 */
int createList_mt(struct list_mt **lst,
	       void (*freeObject)(void *),
	       int poolsize);

/**
 * Frees all elements of the given list.
 * Does nothing if list is <code>NULL</code>.
 *
 * @param list a pointer to a <code>List</code>.
 */
void freeList_mt(struct list_mt *list);

/**
 * Returns the size of the given list.
 *
 * @param list a pointer to a <code>List</code>.
 * @return The current size of the list.
 */
int getSize_mt(const struct list_mt *list);

/**
 * Checks if the list is empty.
 *
 * @param  list a pointer to a <code>List</code>.
 * @return true if the list is empty; false otherwise.
 */
int isEmpty_mt(const struct list_mt *list);

/**
 * Adds a data object to the front of the list. After this method is called,
 * the given data object will be the head of the list.
 * the Data object should not point to something on the stack you
 * should allocate data on the heap.
 *
 * @param list a pointer to a <code>List</code>.
 * @param data a pointer to the data object to add.
 */
void addAtFront_mt(struct list_mt *list, void *data);

/**
 * Adds a data object to the rear of the list. After this method is called,
 * the given data object will be the head of the list.
 * the Data object should not point to something on the stack you
 * should allocate data on the heap.
 *
 * @param list a pointer to a <code>List</code>.
 * @param data a pointer to the data object to add.
 */
void addAtRear_mt(struct list_mt *list, void *data);

/**
 * Removes the data object from the front of the list (the head data object) and returns
 * a pointer to the data object that was removed. If the list is NULL or empty,
 * the function will do nothing and return NULL.
 *
 * @param list a pointer to a <code>List</code>.
 * @return a pointer to the data object that was removed.
 */
void *removeFront_mt(struct list_mt *list);

/**
 * Removes the data object from the rear of the list (the tail data object) and returns
 * a pointer to the data object that was removed. If the list is NULL or empty,
 * the function will do nothing and return NULL.
 *
 * @param list a pointer to a <code>List</code>.
 * @return a pointer to the data object that was removed.
 */
void *removeRear_mt(struct list_mt *list);

/**
 * Force a signal for testing purposes only
 */
void finishUp_mt(struct list_mt *list);





#endif

