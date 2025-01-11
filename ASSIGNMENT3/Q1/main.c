//-----------------------------------------
// NAME: Adejare Taiwo
// STUDENT NUMBER: 7980132
// COURSE: COMP 2160, SECTION: A01
// INSTRUCTOR: Saulo Lectures
// ASSIGNMENT: Assignment 3
//
//-----------------------------------------

#include "container.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Test Scores.
int testsFailed = 0;
int testsPassed = 0;

// ============ TEST PROTOTYPES ===============//
void testCreateContainer();
void testCreateMultipleCon();

void testDestroyEmptyCon();
void testMutipleCreateDestroy();
void testDestroyWithItem();

void testSingleInsert();
void testDuplicateInsert();
void testInsertLarge();

void testDeleteSingle();
void testDelItemNotFound();
void testDeleteMutiple();
void testDeleteEmptyCon();
void clearWithItems();
void clearWithoutItems();
void clearWithLargeItem();
void testNextItem();

void testFirstItem();
void emptyConFirstItem();
void emptyConNextItem();

void testContains();
void testEmptyContain();

void printTestSummary();

int main()
{
    printf("\n// =========== Create container Test ============== //\n");
    testCreateContainer();
    testCreateMultipleCon();

    printf("\n\n// =========== Destroy container Test ============== //\n");
    testDestroyEmptyCon();
    testMutipleCreateDestroy();
    testDestroyWithItem();

    printf("\n\n// =========== Insert Item into container Test ============== //\n");
    testSingleInsert();
    testDuplicateInsert();
    testInsertLarge();

    printf("\n\n// =========== Delete Item into container Test ============== //\n");
    testDeleteSingle();
    testDelItemNotFound();
    testDeleteMutiple();
    testDeleteEmptyCon();

    printf("\n\n// =========== Clear item in container Test ============== //\n");
    clearWithItems();
    clearWithoutItems();
    clearWithLargeItem();

    printf("\n\n// =========== Get first and next item in container Test ============== //\n");
    testFirstItem();
    emptyConFirstItem();
    testNextItem();
    emptyConNextItem();

    printf("\n\n// =========== Contains and size test ============== //\n");
    testContains();
    testEmptyContain();

    printTestSummary();
    printf("End of processing");
    return EXIT_SUCCESS;
}

void printTestSummary()
{
    printf("\n\nTEST RESULT");
    printf("\nNumber of test cases: %d", testsPassed + testsFailed);
    printf("\nTest passed: %d", testsPassed);
    printf("\nTest Failed: %d\n", testsFailed);
}

void testManager(Boolean passed, char *expected)
{
    if (passed)
    {
        printf("\nSUCCESS: - %s", expected);
        testsPassed++;
    }
    else
    {
        printf("\nFAILED: - %s", expected);
        testsFailed++;
    }
}

// Test Container - Check if container retunr a valid pointer and size = 0
void testCreateContainer()
{
    char *describe = "Creating container and also initialize size to 0";
    Container *container = createContainer();
    Boolean testPassed = false;

    // Check if container is initalized and the size is greater/equals zero
    if (container != NULL && size(container) >= 0)
    {
        testPassed = true;
        destroyContainer(container);
    }

    testManager(testPassed, describe);
}

// Test Create Multiple Container - Check if containers are independent.
void testCreateMultipleCon()
{
    char *describe = "Creating multiple container and check if thier indepedent";
    Container *container1 = createContainer();
    Container *container2 = createContainer();
    Boolean testPassed = false;

    // Check if address of container 1 and 2 are not equal.
    if (&container1 != &container2)
    {
        testPassed = true;
        destroyContainer(container1);
        destroyContainer(container2);
    }
    testManager(testPassed, describe);
}

// Test destroy container - Check if container is properly released
void testDestroyEmptyCon()
{
    char *describe = "Destroy Container with no item in it";
    Boolean testPassed = false;

    Container *container = createContainer();
    if (container != NULL)
    {
        destroyContainer(container);
        container = NULL;

        testPassed = container == NULL;
    }

    testManager(testPassed, describe);
}

// Test Mulitple creating and destorying.
void testMutipleCreateDestroy()
{
    char *describe = "Create Multiple container (100) and destory it all.";
    Boolean testPassed = true;
    for (int i = 0; i < 100 && testPassed; i++)
    {
        Container *container = createContainer();
        if (container != NULL)
        {
            destroyContainer(container);
            container = NULL;

            if (container != NULL)
            {
                testPassed = false;
            }
        }
        else
        {
            testPassed = false;
        }
    }

    testManager(testPassed, describe);
}

// Test Destroy container with items in it - clear all item in it and destory the container.
void testDestroyWithItem()
{
    char *describe = "Destory Container with Items in it";
    Boolean testPassed = false;
    Container *container = createContainer();

    if (container != NULL)
    {
        // Items to insert.
        char *items[3] = {"ITEM 1", "ITEM 2", "ITEM 3"};

        for (int i = 0; i < 3; i++)
        {
            insertItem(container, items[i]);
        }

        destroyContainer(container);
        container = NULL;
        testPassed = container == NULL;
    }

    testManager(testPassed, describe);
}

// Test Single Insert - Inser into an empty list.
void testSingleInsert()
{
    char *describe = "It should Insert into an emptylist.";
    Boolean testPassed = false;

    Container *container = createContainer();
    if (container != NULL)
    {
        char *TEST_VALUE = "Item 1";

        insertItem(container, TEST_VALUE);

        // Check if item size is now 1 and first tiem return the test value.
        if (size(container) == 1 && strcmp(firstItem(container), TEST_VALUE) == 0)
        {
            testPassed = true;
        }
        destroyContainer(container);
    }

    testManager(testPassed, describe);
}

// Insert same item into the list to check - It shoudl not insert duplicate
void testDuplicateInsert()
{
    char *describe = "It shoudl not insert duplicate item";
    Boolean testPassed = false;

    Container *container = createContainer();
    if (container != NULL)
    {
        char *TEST_VALUE1 = "Item 1";
        char *TEST_VALUE2 = "Item 1";

        // insert Test values
        insertItem(container, TEST_VALUE1);
        insertItem(container, TEST_VALUE2);

        // check if item size is now 1 and first tiem return the test value
        if (size(container) == 1 && strcmp(firstItem(container), TEST_VALUE1) == 0)
        {
            testPassed = true;
        }

        destroyContainer(container);
    }

    testManager(testPassed, describe);
}

// Tests insertion of a large number of elements into the container
void testInsertLarge()
{
    char *describe = "Insert Mulitple large items into the container.";
    Boolean testPassed = false;
    Container *container = createContainer();
    if (container != NULL)
    {
        char *testVal[100];
        char *TEST_VALUE = "ajjc89j43p89quvq03 98fgqu3498fufq9843muq8493xuq0438rum0q3rq43uf3q4f34fq34f8q34fmq948fqo943mfqu4398fqu4fxp9q43ufnq34987yfqno438fq7yxn43o8yqf7ni4f7xmo438f4830fw74yfq387yqf9348yqf947fq034f89q34u0q34f8q8374q384fyq03m4f80q34f9";
        for (int i = 0; i < 100; i++)
        {
            char *uniqueVal = malloc(sizeof(char) * (strlen(TEST_VALUE) + 3));
            sprintf(uniqueVal, "%d", i);
            testVal[i] = strcat(uniqueVal, TEST_VALUE);
            insertItem(container, testVal[i]);
        }

        // Check if size is equal to to the inserted size.
        if (size(container) == 100 && strcmp(testVal[99], firstItem(container)) == 0)
        {
            testPassed = true;
        }

        for (int i = 0; i < 100; i++)
        {
            free(testVal[i]);
        }
        destroyContainer(container);
    }
    testManager(testPassed, describe);
}

// Tests the deletion of a single element from container.
void testDeleteSingle()
{
    char *describe = "Deletes Single item from the container";
    Boolean testPassed = false;

    Container *container = createContainer();

    if (container != NULL)
    {
        char *TEST_VALUE = "Item to delete";
        insertItem(container, TEST_VALUE);
        if (strcmp(firstItem(container), TEST_VALUE) == 0 && size(container) == 1)
        {
            deleteItem(container, TEST_VALUE);
            if (size(container) == 0 && !contains(container, TEST_VALUE))
            {
                testPassed = true;
            }
        }
        destroyContainer(container);
    }

    testManager(testPassed, describe);
}

// Tests the deletion of an item that is not present in the data structure.
void testDelItemNotFound()
{
    char *describe = "It shouldnt delete item that is not present in container";
    Boolean testPassed = false;

    Container *container = createContainer();

    if (container != NULL)
    {
        char *TEST_VALUE = "Itemtodelete";
        char *NOT_FOUND = "notfound";
        insertItem(container, TEST_VALUE);

        deleteItem(container, NOT_FOUND);
        if (size(container) == 1 && contains(container, TEST_VALUE))
        {
            testPassed = true;
        }
        destroyContainer(container);
    }

    testManager(testPassed, describe);
}

// Tests the deletion of multiple elements from the container
void testDeleteMutiple()
{
    char *describe = "Delete Multiple Items from container";
    Boolean testPassed = false;

    Container *container = createContainer();

    if (container != NULL)
    {
        char *items[3] = {"DeleteItem 1", "DeleteItem 2", "DeleteItem 3"};

        for (int i = 0; i < 3; i++)
        {
            insertItem(container, items[i]);
        }
        int i = 2;
        while (size(container) > 1)
        {
            deleteItem(container, items[i]);
            i--;
        }

        if (strcmp(firstItem(container), items[0]) == 0)
        {
            testPassed = true;
        }

        destroyContainer(container);
    }

    testManager(testPassed, describe);
}

// Test case to verify deletion of a non-existent item from an expty list.
void testDeleteEmptyCon()
{
    char *describe = "Its not delete Not found Item";
    Boolean testPassed = false;

    Container *container = createContainer();

    if (container != NULL)
    {
        char *NOT_FOUND = "notfound";

        deleteItem(container, NOT_FOUND);
        // check if contianer size is 0
        if (size(container) == 0)
        {
            testPassed = true;
        }
        destroyContainer(container);
    }

    testManager(testPassed, describe);
}

// Clears the 'container' and frees memory for each item in the list
void clearWithItems()
{
    char *describe = "Clear Container with Items";
    Boolean testPassed = false;

    Container *container = createContainer();

    if (container != NULL)
    {
        char *items[3] = {"ClearItem 1", "ClearItem 2", "ClearItem 3"};

        for (int i = 0; i < 3; i++)
        {
            insertItem(container, items[i]);
        }

        clear(container);

        // Check if size of item is zero and first item is NULL
        if (size(container) == 0 && firstItem(container) == NULL)
        {
            testPassed = true;
        }

        destroyContainer(container);
    }

    testManager(testPassed, describe);
}

// Clears the 'container' without items in it
void clearWithoutItems()
{
    char *describe = "Not clear container without items";
    Boolean testPassed = false;

    Container *container = createContainer();

    if (container != NULL)
    {

        clear(container);

        if (size(container) == 0 && firstItem(container) == NULL)
        {
            testPassed = true;
        }

        destroyContainer(container);
    }

    testManager(testPassed, describe);
}

// Tests clearing the 'container' while handling large items
void clearWithLargeItem()
{
    char *describe = "Clear container without large items";
    Boolean testPassed = false;

    Container *container = createContainer();

    if (container != NULL)
    {
        char *testVal[100];
        char *TEST_VALUE = "ajjc89j43p89quvq03 98fgqu3498fufq9843muq8493xuq0438rum0q3rq43uf3q4f34fq34f8q34fmq948fqo943mfqu4398fqu4fxp9q43ufnq34987yfqno438fq7yxn43o8yqf7ni4f7xmo438f4830fw74yfq387yqf9348yqf947fq034f89q34u0q34f8q8374q384fyq03m4f80q34f9";
        for (int i = 0; i < 100; i++)
        {
            char *uniqueVal = malloc(sizeof(char) * (strlen(TEST_VALUE) + 3));
            sprintf(uniqueVal, "%d", i);
            testVal[i] = strcat(uniqueVal, TEST_VALUE);
            insertItem(container, testVal[i]);
        }

        clear(container);
        if (size(container) == 0 && firstItem(container) == NULL)
        {
            testPassed = true;
        }

        destroyContainer(container);
    }
    testManager(testPassed, describe);
}

// Tests retrieval of the first item from a non-empty 'container'
void testFirstItem()
{
    char *describe = "First item from container";
    Boolean testPassed = false;

    Container *container = createContainer();
    if (container != NULL)
    {
        char *items[3] = {"Item 1", "Item 2", "Item 3"};

        for (int i = 0; i < 3; i++)
        {
            insertItem(container, items[i]);
        }

        if (strcmp(firstItem(container), items[2]) == 0 && size(container) == 3)
        {
            testPassed = true;
        }

        destroyContainer(container);
    }

    testManager(testPassed, describe);
}

// Tests handling of an empty 'container' when attempting to get the first item
void emptyConFirstItem()
{
    char *describe = "Not get first item from empty container";
    Boolean testPassed = false;

    Container *container = createContainer();
    if (container != NULL)
    {
        if (firstItem(container) == NULL && size(container) == 0)
        {
            testPassed = true;
        }

        destroyContainer(container);
    }

    testManager(testPassed, describe);
}

// Tests retrieval of the next item from a non-empty 'container'
void testNextItem()
{
    char *describe = "Next item from container";
    Boolean testPassed = false;

    Container *container = createContainer();
    if (container != NULL)
    {
        char *items[3] = {"Item 1", "Item 2", "Item 3"};

        for (int i = 0; i < 3; i++)
        {
            insertItem(container, items[i]);
        }

        Boolean isNext = true;
        for (int i = 2; i > 0 && isNext; i--)
        {
            if (strcmp(nextItem(container, items[i]), items[i - 1]) != 0)
            {
                isNext = false;
            }
        }

        testPassed = isNext;

        destroyContainer(container);
    }

    testManager(testPassed, describe);
}

// Tests handling of an empty 'container' when attempting to get the next item
void emptyConNextItem()
{
    char *describe = "Not get first Item in empty container";
    Boolean testPassed = false;

    Container *container = createContainer();
    if (container != NULL)
    {
        char *TEST_VAL = "notFound";
        if (nextItem(container, TEST_VAL) == NULL)
        {
            testPassed = true;
        }

        destroyContainer(container);
    }

    testManager(testPassed, describe);
}

void testContains()
{
    char *describe = "it should Contain item";
    Boolean testPassed = false;

    Container *container = createContainer();
    if (container != NULL)
    {
        char *items[3] = {"Item 1", "Item 2", "Item 3"};

        for (int i = 0; i < 3; i++)
        {
            printf("\nInserting: %s", items[i]);
            insertItem(container, items[i]);
        }

        if (contains(container, items[1])  && contains(container, items[1]) )
        {
            testPassed = true;
        }

        destroyContainer(container);
    }

    testManager(testPassed, describe);
}

void testEmptyContain()
{
    char *describe = "Not to contain item on empty list";
    Boolean testPassed = false;

    Container *container = createContainer();
    if (container != NULL)
    {
        char *TEST_VAL = "notFound";

        if (!contains(container, TEST_VAL) )
        {
            testPassed = true;
        }

        destroyContainer(container);
    }

    testManager(testPassed, describe);
}