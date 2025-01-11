// #define NDEBUG

#include "container.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

typedef struct NODE Node;

// Container Struct
struct CONTAINER
{
    Node *top;
    int size;
};

// Node Struct
struct NODE
{
    char *item;
    Node *next;
};

// Prototypes
char *makeItem(char *item);
Node *makeNode();
void destroyNode(Node *node);

// Invairants
void checkContainer(Container *container);
void checkItem(char *item);
void checkNode(Node *node);

// Create Container - Container constructor.
Container *createContainer()
{
    // Allocate memeory to container
    Container *newContainer = (Container *)malloc(sizeof(Container));
    assert(newContainer != NULL);

    // Initalize Container / Container Constructor
    if (newContainer != NULL)
    {
        newContainer->top = NULL;
        newContainer->size = 0;
    }

    checkContainer(newContainer);
    return newContainer;
}

// Destroy Container - Container destructor
void destroyContainer(Container *container)
{
    checkContainer(container);
    if (container != NULL)
    {
        // Clear container
        clear(container);

        // Free container of memeory.
        free(container);
    }
}

// Add item to Container.
void insertItem(Container *container, char *item)
{
    checkContainer(container);
    checkItem(item);


    if (!contains(container, item))
    {
        // Create node for item and add it to container.
        Node *newNode = makeNode();
        if (newNode != NULL)
        {
            newNode->item = makeItem(item);
            newNode->next = container->top;
            container->top = newNode;
            container->size++;
        }
    }
    checkContainer(container);
}

// Delete item - removes item from container.
void deleteItem(Container *container, char *delItem)
{
    checkContainer(container);
    checkItem(delItem);

    Node *prev = NULL;
    Node *curr = container->top;

    // Get item to delete position.
    while (curr != NULL && strcmp(delItem, curr->item) != 0)
    {
        prev = curr;
        curr = curr->next;
    }

    // Perfrom delete if its found
    if (curr != NULL && strcmp(delItem, curr->item) == 0)
    {
        if (prev != NULL)
        {
            prev->next = curr->next;
        }
        else
        {
            container->top = curr->next;
        }
        container->size--;
        destroyNode(curr);
    }
}

// Clear container - remove all items in Container.
void clear(Container *container)
{
    checkContainer(container);

    Node *prev;
    Node *curr = container->top;

    while (curr != NULL)
    {
        prev = curr;
        curr = curr->next;
        destroyNode(prev);
    }

    container->top = NULL;
    container->size = 0;

    assert(container->top == NULL);
}

// First item - Return the first item in the container.
char *firstItem(Container *container)
{
    checkContainer(container);

    char *item = NULL;

    if (container->top != NULL && container->size > 0)
    {
        item = container->top->item;
    }

    return item;
}

// Next item - Retunr item nex to the item recieved
char *nextItem(Container *container, char *item)
{
    checkContainer(container);
    checkItem(item);

    char *nextItem = NULL;

    Node *curr = container->top;

    while (curr != NULL && strcmp(item, curr->item) != 0)
    {
        curr = curr->next;
    }

    if (curr != NULL && curr->next != NULL)
    {
        nextItem = curr->next->item;
    }

    return nextItem;
}

// Contains - check if item exist in list
Boolean contains(Container *container, char *item)
{
    checkContainer(container);
    checkItem(item);

    Boolean isFound = false;

    // Traverse over list and check if item exist.
    Node *curr = container->top;
    while (curr != NULL && !isFound)
    {
        if (strcmp(item, curr->item) == 0)
        {
            isFound = true;
        }
        curr = curr->next;
    }

    return isFound;
}

// Size - return the current size of container.
int size(Container *container)
{
    checkContainer(container);
    return container->size;
}

// Make item - allocate memory to item.
char *makeItem(char *item)
{
    checkItem(item);

    // Allocate memory to item
    int itemlength = strlen(item);
    char *newItem = (char *)malloc(sizeof(char) * (itemlength + 1));
    if (newItem != NULL)
    {
        // Copy item into new item.
        strcpy(newItem, item);
    }

    return newItem;
}

// Make Node - allocate memory to node.
Node *makeNode()
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    assert(newNode != NULL);

    return newNode;
}

// Destroy the Node and its items.
void destroyNode(Node *node)
{
    checkNode(node);

    // If not is not empty we free its item and free the Node.
    if (node != NULL)
    {
        free(node->item);
        free(node);
    }
}

// Invaiarants

// Check if container is always valid
void checkContainer(Container *container)
{
    assert(container != NULL);
    Node *curr = container->top;

    // int count = 0;
    while (curr != NULL)
    {
        checkNode(curr);
        curr = curr->next;
        // count++;
    }
    
    assert(container->size >= 0);
    // assert(count == container->size);
}

// Check the item type invairant
void checkItem(char *item)
{
    assert(item != NULL);
    assert(strlen(item) >= 0);
}

// Check the node invariant.
void checkNode(Node *node)
{
    assert(node != NULL);
    checkItem(node->item);
}