#include "ObjectManager.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

typedef struct NODE Node;
typedef struct OBJECT_DATA ObjectData;
typedef struct LIST List;

// Object Struct
struct OBJECT_DATA
{
    Ref id;
    ulong offset;
    ulong refCnt;
    ulong size;
};

// Node Struct
struct NODE
{
    ObjectData object;
    Node *next;
};

// List Struct
struct LIST
{
    Node *root;
    ulong numOfItems;
    ulong size;
    ulong collected;
    uchar *buffer;
};

List *pool;
uchar buffer1[MEMORY_SIZE];
uchar buffer2[MEMORY_SIZE];

// Prototypes
static void checkPool();
static void checkNode(Node *node);
static void checkObjectData(ObjectData obj);
static void checkSize(ulong size);

static Node *makeNode(ulong size, ulong offset);
static void destroyNode(Node *node);
static bool memoryNotFull(ulong size);
static Ref makeObjData(ulong size, ulong offset);
static ulong mymalloc(ulong size);
static Node *findObjData(Ref ref);
static void dropObject(Ref ref);
static void compact();

/*
 * This function is responsible for initializing the object manager.
 * It ensures that all necessary resources are allocated and prepared
 * when the application starts.
 */
void initPool()
{
   
    // assert(pool == NULL);

    pool = (List *)malloc(sizeof(List));
    assert(pool != NULL);
    if (pool != NULL)
    {
        // Initialize all struct properties.
        pool->root = NULL;
        pool->size = 0;
        pool->collected = 0;
        pool->numOfItems = 0;
        pool->buffer = buffer1;
    }

    checkPool();
}

/*
 * Destroys the memory pool and frees all associated resources.
 * Parameters:
 *   None.
 * Returns:
 *   Nothing.
 *
 */
void destroyPool()
{
    checkPool();
    Node *curr = pool->root;

    while (curr != NULL)
    {
        Node *temp = curr;
        curr = curr->next;
        destroyNode(temp);
    }
    pool->root = NULL;
    memset(buffer1, 0, MEMORY_SIZE);
    memset(buffer2, 0, MEMORY_SIZE);
}

/*
 * Requests a memory block of the specified size from the object manager.
 * Parameters:
 *   size - The size of the memory block to allocate.
 * Returns:
 *   A pointer to the allocated memory block, or null if allocation fails.
 */
Ref insertObject(ulong size)
{
    if (size < 1)
    {
        return NULL_REF;
    }
    // Preconditions
    checkSize(size);
    Ref newId = NULL_REF;
    ulong memoryOffset = 0;

    // Memory is full
    if (!memoryNotFull(size))
    {
        compact();
    }

    // if memory is not full even after running gabage collection.
    if (memoryNotFull(size))
    {
        memoryOffset = mymalloc(size);
        checkSize(memoryOffset);

        newId = makeObjData(size, memoryOffset);
    }
    else
    {
        printf("\nUnable to successfully complete memory allocation request.\n");
    }

    assert(newId >= 0);

    return newId;
}

/*
 * Retrieves an object from the memory pool using its reference.
 * Parameters:
 *   ref - The reference of the object to retrieve.
 * Returns:
 *   A pointer to the object if found, or NULL if the object does not exist.
 */
void *retrieveObject(Ref ref)
{

    assert(ref >= NULL_REF);
    void *object = NULL;
    Node *objectNode = findObjData(ref);

    // if object with that reference assign object
    if (objectNode != NULL)
    {
        object = &(pool->buffer[objectNode->object.offset]);
    }
    else
    {
        printf("\nInvalid reference exception with reference %ld, terminating process", ref);
        return NULL;
        exit(0);
    }

    return object;
}

/*
 * Increments the reference count for the object with the given reference ID.
 *
 * Parameters:
 *   id - The reference ID of the object.
 */
void addReference(Ref ref)
{
    assert(ref >= NULL_REF);

    Node *objecNode = findObjData(ref);

    if (objecNode != NULL)
    {
        objecNode->object.refCnt++;
        checkNode(objecNode);
    }
}

/*
 * Decrements the reference count for the object with the given reference ID.
 * Parameters:
 *   id - The reference ID of the object.
 */
void dropReference(Ref ref)
{
    assert(ref >= NULL_REF);

    Node *objectNode = findObjData(ref);
    if (objectNode != NULL)
    {
        // Once a reference count for an object reaches 0, remove from linked list.
        if (objectNode->object.refCnt - 1 == 0)
        {
            dropObject(objectNode->object.id);
        }
        else
        {
            objectNode->object.refCnt--;
        }
    }
}

void dumpPool()
{
    checkPool();

    Node *curr = pool->root;
    while (curr != NULL)
    {
        printf("ID: %ld \nsize: %ld\noffset: %ld\nrefCount: %ld\n", curr->object.id, curr->object.size, curr->object.offset, curr->object.refCnt);
        curr = curr->next;
    }
}

/*
 * Retrieves the alternate buffer in the memory pool.
 * This function checks the current buffer in use and returns the other buffer.
 * Parameters:
 *   None.
 * Returns:
 *   A pointer to the alternate buffer.
 */
static uchar *getCurrentBuffer()
{
    uchar *newBuffer = NULL;
    if (pool->buffer == buffer1)
    {
        newBuffer = buffer2;
        memset(buffer1, 0, MEMORY_SIZE);
    }
    else
    {
        newBuffer = buffer1;
        memset(buffer2, 0, MEMORY_SIZE);
    }

    assert(newBuffer != NULL);
    return newBuffer;
}

/*
 * Compacts the memory pool by reorganizing objects into a contiguous block.
 * Updates object offsets, switches active buffer, and recalculates pool size and collected bytes.
 * Parameters:
 *   None.
 * Returns:
 *   Nothing.
 */
static void compact()
{
    checkPool();
    // Get Next buffer to use.
    uchar *currBuffer = getCurrentBuffer();
    Node *curr = pool->root;

    ulong newOffset = 0;
    Ref numberOfObj = 0;

    while (curr != NULL)
    {
        curr->object.offset = newOffset;
        currBuffer[newOffset] = (uchar)curr->object.size;
        newOffset += curr->object.size;
        numberOfObj++;
        curr = curr->next;
    }

    // Bytes collected
    pool->collected = pool->size - newOffset;

    pool->size = newOffset;
    pool->buffer = currBuffer;

    checkPool();

    printf("\nNumber of Object: %ld\nCurrent Number of Bytes: %ld\nNumber of Bytes Collected: %ld\n", numberOfObj, pool->size, pool->collected);
}

// ====================== Helper Functions ============================ //

// Calculates if there is still enough Memeory
static bool memoryNotFull(ulong size)
{
    checkSize(size);
    // Validate pool here
    return pool->size + size <= MEMORY_SIZE;
}

/*
 * Allocates a memory block of the given size in the pool.
 * Parameters:
 *   size - Size of the memory block to allocate.
 * Returns:
 *   Offset to the allocated memory block.
 */
static ulong mymalloc(ulong size)
{
    checkSize(size);
    ulong offset = pool->size;
    pool->buffer[offset] = (uchar)size;
    return offset;
}

static Ref makeObjData(ulong size, ulong offset)
{
    // Pre-conditions
    checkSize(size);
    checkPool();

    Node *curr = pool->root;
    Node *prev = NULL;

    while (curr != NULL)
    {
        prev = curr;
        curr = curr->next;
    }

    // Create new node with allocated object
    Node *newNode = makeNode(size, offset);
    if (prev == NULL)
    {
        pool->root = newNode;
    }
    else
    {
        prev->next = newNode;
    }

    // Increment pool size.

    pool->size += size;
    pool->numOfItems++;

    checkPool();
    return newNode->object.id;
}

/*
 * Finds and retrieves a node from the memory pool using its reference.
 * Parameters:
 *   ref - The reference ID of the object to retrieve. Must be >= NULL_REF.
 * Returns:
 *   A pointer to the node if found, or NULL if the node does not exist.
 */
static Node *findObjData(Ref ref)
{

    assert(ref >= NULL_REF);

    Node *curr = pool->root;
    Node *found = NULL;

    while (curr != NULL && found == NULL)
    {
        checkNode(curr);
        if (curr->object.id == ref)
        {
            found = curr;
            checkNode(found);
        }
        else
        {
            curr = curr->next;
        }
    }

    return found;
}

/*
 * Creates a new node with the specified object properties.
 * Parameters:
 *   id      - Identifier for the object.
 *   size    - Size of the object.
 *   offset  - Offset of the object in memory.
 * Returns:
 *   A pointer to the newly created node.
 */
static Node *makeNode(ulong size, ulong offset)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    assert(newNode != NULL);

    if (newNode != NULL)
    {

        newNode->object.id = pool->numOfItems + 1;
        newNode->object.size = size;
        newNode->object.refCnt = 1;
        newNode->object.offset = offset;
        newNode->next = NULL;
    }
    checkNode(newNode);
    return newNode;
}

/*
 * Destroys a node by validating it and freeing its memory.
 * Parameters:
 *   node - A pointer to the node to be destroyed. Must be non-NULL and valid.
 * Returns:
 *   Nothing.
 */
static void destroyNode(Node *node)
{
    checkNode(node);
    free(node);
    node = NULL;
}

/*
 * Removes the object with the specified reference ID from the pool.
 *
 * Parameters:
 *   ref - Reference ID of the object to remove.
 */
static void dropObject(Ref ref)
{
    assert(ref >= NULL_REF);

    Node *prev = NULL;
    Node *curr = pool->root;

    while (curr != NULL && curr->object.id != ref)
    {
        prev = curr;
        curr = curr->next;
    }

    // Perfrom delete if its found
    if (curr != NULL && curr->object.id == ref)
    {
        if (prev != NULL)
        {
            prev->next = curr->next;
        }
        else
        {
            pool->root = curr->next;
        }

        destroyNode(curr);
    }
}

// Invariants

/*
 * Verifies the memory pool's integrity:
 * - Ensures the pool is not null.
 * - Checks each node and compares total used memory with the pool's size.
 */
void checkPool()
{
    assert(pool != NULL);

    Node *curr = pool->root;

    while (curr != NULL)
    {
        checkNode(curr);
        curr = curr->next;
    }

    checkSize(pool->size);
    checkSize(pool->collected);

    assert(pool->buffer != NULL);
    assert(pool->numOfItems >= 0);
}

/*
 * Verifies the validity of a given node:
 * - Ensures the node is not null.
 * - Checks the associated object for correctness.
 */
static void checkNode(Node *curr)
{
    assert(curr != NULL);
    checkObjectData(curr->object);
}

/*
 * Verifies the validity of a given object:
 * - Checks that the object meets all required conditions and invariants.
 */
static void checkObjectData(ObjectData obj)
{
    assert(obj.id >= 0);
    assert(obj.refCnt >= 0);
    assert(obj.size >= 0);
    assert(obj.size <= MEMORY_SIZE);
}

/*
 * Validates the size parameter to ensure it is within acceptable bounds.
 * Parameters:
 *   size - The size value to validate. Must be non-negative and not exceed MEMORY_SIZE.
 * Returns:
 *   Nothing.
 */
static void checkSize(ulong size)
{
    assert(size >= 0);
    // assert(size <= MEMORY_SIZE);
}
