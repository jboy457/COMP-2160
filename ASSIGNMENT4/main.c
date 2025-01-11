#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "ObjectManager.h"

int testsFailed = 0;
int testsPassed = 0;

void testInitPool();
void testMultipleInit();

void testInsertObject();
void testInsertLargeObj();
void testInsertExactObject();
void testInsertZeroObject();
void testInsufficientMemory();

void testRetrieveObject();
void testRetrieveNullRef();
void testRetrieveDroppedRef();
void testRetrieveMuliRef();

void testAddReference();
void testAddReferenceInvalidRef();

void testDropReference();
void testInvalidDropReference();

void testDumpPool();

void testDestroyPoolWithItem();
void testDestroyPoolWithoutItem();

int main()
{
    printf("\n// =========== InitPool Test ============== //\n");
    testInitPool();

    printf("\n// =========== Insert Object Test ============== //\n");
    testInsertObject();
    testInsertLargeObj();
    testInsertExactObject();
    testInsertZeroObject();
    testInsufficientMemory();

    printf("\n// =========== Retrieve Object Test ============== //\n");
    testRetrieveObject();
    testRetrieveNullRef();
    testRetrieveDroppedRef();
    testRetrieveMuliRef();

    printf("\n// =========== Add Reference Object Test ============== //\n");
    testAddReference();
    testAddReferenceInvalidRef();

    printf("\n// =========== Drop Reference Object Test ============== //\n");
    testDropReference();
    testInvalidDropReference();

    printf("\n// =========== Dump pool Test ============== //\n");
    testDumpPool();

    printf("\n// =========== Destory pool Test ============== //\n");
    testDestroyPoolWithItem();
    testDestroyPoolWithoutItem();

    return 0;
}

// Records test case
void testManager(bool passed, const char *expected)
{
    if (passed)
    {
        printf("\nSUCCESS: expected to %s and did it.", expected);
        testsPassed++;
    }
    else
    {
        printf("\nFAILED: expected to %s, but could not do it.", expected);
        testsFailed++;
    }
}

void printTestSummary()
{
    printf("\n\nTEST RESULT");
    printf("\nNumber of test cases: %d", testsPassed + testsFailed);
    printf("\nTest passed: %d", testsPassed);
    printf("\nTest Failed: %d\n", testsFailed);
}

// Test Initialize Pool
void testInitPool()
{
    const char *describe = "Test Initialize Pool";
    bool testPassed = false;

    initPool();
    ulong size = 100;

    // Test if insert works.
    Ref id = insertObject(size);
    if (id == 1)
    {

        testPassed = true;
        destroyPool();
    }

    testManager(testPassed, describe);
}

// Test multiple initialize
void testMultipleInit()
{
    const char *describe = "Test Multiple Initialize Pool";
    bool testPassed = false;
    initPool();

    ulong size = 100;
    Ref id = insertObject(size);
    // second init
    initPool();

    void *obj = retrieveObject(id);
    if (*((Ref *)(obj)) == size)
    {
        testPassed = true;
        destroyPool();
    }

    testManager(testPassed, describe);
}

// Test insertObject
void testInsertObject()
{
    const char *describe = "Test insert object into pool";
    bool testPassed = false;

    ulong size = 25;
    initPool();
    Ref id = insertObject(size);

    if (id != NULL_REF)
    {
        testPassed = true;
    }
    destroyPool();

    testManager(testPassed, describe);
}

// Test large insertObject
void testInsertLargeObj()
{
    const char *describe = "Test very large object into pool";
    bool testPassed = false;

    ulong size = MEMORY_SIZE + 1;
    initPool();
    Ref id = insertObject(size);

    if (id == NULL_REF)
    {
        testPassed = true;
    }
    destroyPool();
    testManager(testPassed, describe);
}

// Test exact memory size insertObject
void testInsertExactObject()
{
    const char *describe = "Test insert exact object into pool";
    bool testPassed = false;

    ulong size = MEMORY_SIZE;
    initPool();
    Ref id = insertObject(size);

    if (id != NULL_REF)
    {
        testPassed = true;
    }
    destroyPool();

    testManager(testPassed, describe);
}

// Test zero allocation size insertObject
void testInsertZeroObject()
{
    const char *describe = "Test zero size object in pool";
    bool testPassed = false;

    ulong size = 0;
    initPool();
    Ref id = insertObject(size);

    if (id == NULL_REF)
    {
        testPassed = true;
    }
    destroyPool();

    testManager(testPassed, describe);
}

// Test insert unitl insuffiencent.
void testInsufficientMemory()
{
    const char *describe = "Test insufficeint Memory object in pool";
    bool testPassed = false;

    initPool();
    bool isFull = false;
    for (ulong i = 1; i < MEMORY_SIZE * 2 && !isFull; i *= 10)
    {
        Ref temp = insertObject(i);

        if (temp == NULL_REF)
        {
            isFull = true;
        }
    }

    if (isFull)
    {
        testPassed = true;
    }

    destroyPool();
    testManager(testPassed, describe);
}

void testRetrieveObject()
{
    const char *describe = "Test retrieve object in pool";
    bool testPassed = false;

    ulong size = 10;

    initPool();
    Ref id = insertObject(size);
    void *obj = retrieveObject(id);
    if (*((ulong *)(obj)) == size)
    {
        testPassed = true;
    }
    destroyPool();

    testManager(testPassed, describe);
}

void testRetrieveNullRef()
{
    const char *describe = "Test doesnt retrieve object in pool";
    bool testPassed = false;

    initPool();
    void *obj = retrieveObject(NULL_REF);
    if (obj == NULL)
    {
        testPassed = true;
    }
    destroyPool();

    testManager(testPassed, describe);
}

void testRetrieveDroppedRef()
{
    const char *describe = "Test doesnt retrieve object of dropped Item in pool";
    bool testPassed = false;
    ulong size = 123;

    initPool();
    Ref id = insertObject(size);
    dropReference(id);

    void *obj = retrieveObject(id);
    if (obj == NULL)
    {
        testPassed = true;
    }
    destroyPool();

    testManager(testPassed, describe);
}

void testRetrieveMuliRef()
{
    const char *describe = "Test retrieve different items with mulitiple Ref in pool";
    bool testPassed = false;
    ulong size1 = 123;
    ulong size2 = 20;

    initPool();
    Ref id1 = insertObject(size1);
    Ref id2 = insertObject(size2);

    void *obj1 = retrieveObject(id1);
    void *obj2 = retrieveObject(id2);
    if (obj1 != obj2 && *((ulong *)(obj1)) != *((ulong *)(obj2)))
    {
        testPassed = true;
    }
    destroyPool();

    testManager(testPassed, describe);
}

void testAddReference()
{
    const char *describe = "Test Add Reference";
    bool testPassed = false;
    ulong size1 = 123;
    initPool();

    Ref id1 = insertObject(size1);
    if (id1 > 0)
    {
        addReference(id1);

        dropReference(id1);
        void *obj1 = retrieveObject(id1);
        if (obj1 != NULL)
        {
            testPassed = true;
        }
    }

    destroyPool();
    testManager(testPassed, describe);
}

void testAddReferenceInvalidRef()
{
    const char *describe = "Test Add Reference to an invalid ref";
    bool testPassed = false;
    initPool();

    Ref invalidRef = 24;

    addReference(invalidRef);
    void *invalid = retrieveObject(invalidRef);
    if (invalid == NULL)
    {
        testPassed = true;
    }

    destroyPool();
    testManager(testPassed, describe);
}

void testDropReference()
{
    const char *describe = "Test Drop Reference to a valid ref";
    bool testPassed = false;

    ulong size1 = 123;
    initPool();

    Ref id1 = insertObject(size1);
    if (id1 > 0)
    {
        dropReference(id1);
        void *obj1 = retrieveObject(id1);
        if (obj1 == NULL)
        {
            testPassed = true;
        }
    }
    destroyPool();
    testManager(testPassed, describe);
}

void testInvalidDropReference()
{
    const char *describe = "Test Drop Reference to an invalid ref";
    bool testPassed = false;
    initPool();

    Ref invalidRef = 24;

    dropReference(invalidRef);
    void *invalid = retrieveObject(invalidRef);
    if (invalid == NULL)
    {
        testPassed = true;
    }

    destroyPool();
    testManager(testPassed, describe);
}

void testDumpPool()
{
    const char *describe = "Test Dump Pool and Print item in Pool";
    bool testPassed = false;
    ulong size1 = 123;
    initPool();

    Ref id1 = insertObject(size1);
    if (id1 > 0)
    {
        addReference(id1);

        dumpPool();
        testPassed = true;
    }

    destroyPool();
    testManager(testPassed, describe);
}

void testDestroyPoolWithItem()
{
    const char *describe = "Test destroy Pool with items";
    bool testPassed = false;
    ulong size1 = 123;
    ulong size2 = 122;
    initPool();

    Ref id1 = insertObject(size1);
    Ref id2 = insertObject(size2);
    if (id1 > 0 && id2 > 0)
    {
        addReference(id1);
        destroyPool();

        void *item1 = retrieveObject(size1);
        void *item2 = retrieveObject(size2);
        if (item1 == NULL && item2 == NULL)
        {
            testPassed = true;
        }
    }

    testManager(testPassed, describe);
}

void testDestroyPoolWithoutItem()
{
    const char *describe = "Test destroy Pool without items";
    bool testPassed = false;
    initPool();

    destroyPool();

    testPassed = true;

    testManager(testPassed, describe);
}