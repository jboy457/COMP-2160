
#ifndef _container_h
#define _container_h
// ==========  TYPE DEFINATIONS AND STRTUCT ============

typedef struct CONTAINER Container;
typedef enum BOOL { false, true } Boolean;

// ========= END STRUCT =============


// ========= PROTOTYPES =============

/*
 * Create a new instance of the container and initialize any necessary resources.
 *
 * Returns:
 *   A pointer to a new container instance.
 */
Container *createContainer();

/*
 * Destroy the container and release any resources or memory it is using.
 *
 * Parameters:
 *   container - Pointer to the container instance to destroy.
 */
void destroyContainer(Container *container);

/*
 * Insert an item into the container.
 *
 * Parameters:
 *   container - Pointer to the container where the item should be inserted.
 *   item - Pointer to the character string representing the item to be inserted.
 *
 */
void insertItem(Container *container, char *item);

/*
 * Remove the specified item from the container. Only one item is removed if there are duplicates.
 *
 * Parameters:
 *   container - Pointer to the container from which the item should be removed.
 *   item - Pointer to the string item to be removed.
 */
void deleteItem(Container *container, char *item);

/*
 * Remove all items from the container.
 *
 * Parameters:
 *   container - Pointer to the container to clear of all items.
 */
void clear(Container *container);

/*
 * Begin iterating over items in the container and return the first item.
 *
 * Parameters:
 *   container - Pointer to the container to iterate.
 *
 * Returns:
 *   A pointer to the first item in the container, or NULL if empty.
 */
char *firstItem(Container *container);

/*
 * Continue iterating over items in the container and return the next item.
 *
 * Parameters:
 *   container - Pointer to the container being iterated.
 *
 * Returns:
 *   A pointer to the next item in the container, or NULL if no more items.
 */
char *nextItem(Container *container, char* item);

/*
 * Check if the container contains the specified item.
 *
 * Parameters:
 *   container - Pointer to the container to check.
 *   item - Pointer to the string item to search for.
 *
 * Returns:
 *   1 if the item is in the container, 0 otherwise.
 */
Boolean contains(Container *container, char *item);

/*
 * Determine how many items are currently in the container.
 *
 * Parameters:
 *   container - Pointer to the container to count items in.
 *
 * Returns:
 *   The number of items in the container.
 */
int size(Container *container);


// ========= END PROTOTYPES =========



#endif