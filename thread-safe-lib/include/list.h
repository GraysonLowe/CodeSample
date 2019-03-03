#ifndef __LIST_H
#define __LIST_H
/**
 * @file   list.h
 * @author Shane Panter
 * @date   Wed Sep 15 20:49:13 2010
 * @updated Mon 12 Mar 2018 05:32:06 PM MDT
 *
 * @brief  Defines an interface for a doubly-linked list
 */

#include <stdio.h>
#include <stdlib.h>

///Structure to represent a doubly linked list in memory
struct list {
	int size;		/**< The size of the list */
	struct node * head;		/**< The head of the list  */
	struct node * tail;		/**< The tail of the list  */
	void (*freeObject)(void*);
};

///Structure to represent a node in a linked list
struct node {
	void *data;		/**< The nodes data */
	struct node * next;		/**< The next node in the list */
	struct node * prev;		/**< The prev node in the list */
};

/**
 * Creates a new Node
 *
 * @param data The data that the node should contain
 *
 * @return The created Node
 */
struct node * createNode(void *obj);

/**
 * Frees a node
 *
 * @param node the node to free
 */
void freeNode(struct node * node);

/**
 * Creates a new list which can store any type of data
 * @param freeObject Function pointer to free an oject
 * @return
 */
struct list* createList(void (*freeObject) (void *));

/**
 * Frees a given list
 * @param L The list to free
 */
void freeList(struct list* L);

/**
 * Returns the current list size
 * @param L the list to get the size of
 * @return
 */
int getSize(struct list* L);

/**
 * Returns true if the list is empty false otherwise
 * @param L The list to check
 * @return true if the list is empty
 */
int isEmpty(struct list* L);

/**
 * Adds a new node the the front of the list
 * @param list the list to add to
 * @param node the node to add to the list
 */
void addAtFront(struct list* list, struct node * node);

/**
 * Adds a new node to the rear of the list
 * @param list the list to add to
 * @param node the node to add to the list
 */
void addAtRear(struct list* list, struct node * node);

/**
 * Removes a node from the front of the list
 * @param list the list to remove a node from
 * @return the node that was removed
 */
struct node * removeFront(struct list* list);

/**
 * Removes a node from the rear of a list
 * @param list the list to remove a node from
 * @return the node that was removed
 */
struct node * removeRear(struct list* list);

/**
 * Removes an existing node from the list
 * @param list the list to remove from
 * @param node the node to remove
 * @return the removed node
 */
struct node * removeNode(struct list* list, struct node * node);

/**
 * Search a given list for a specified key
 * @param list The list to search
 * @param key The key to search for
 * @return A pointer to the node found or null if none was found
 */
struct node * search(struct list* list, int key);

/**
 * Reverses a list
 * @param L The list to reverse
 */
void reverseList(struct list* L);

/**
 * Prints a given list by calling the toString function on each node
 * @param L The list to print
 */
void printList(struct list* L);


#endif				/* __LIST_H */
