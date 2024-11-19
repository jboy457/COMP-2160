//-----------------------------------------
// NAME: Adejare Taiwo
// STUDENT NUMBER: 7980132
// COURSE: COMP 2160, SECTION: A01
// INSTRUCTOR: Saulo Lectures
// ASSIGNMENT: Assignment 3
//
//-----------------------------------------

#include "set.h"

#include <stdio.h>
#include <stdlib.h>

void printTestSummary();
void testManager(Boolean passed, char *expected);

// Sets
void testCreateNewSet();
void testCreateMutipleSet();

void testDeleteEmpty();
void testMulitpleCreateDelete();
void testDeleteWithItem();
void testDelLargeSet();

void testSingleInsert();
void testMultipleInsert();
void testInsertLargeItem();
void testDuplicateInsert();
void testInsertNegtiveItem();

void testRemoveItem();
void testRemoveWithMuliItem();
void testRemoveNoExist();
void testEmptySetRemove();
void testMulipleRemove();

void testEqualEmpty();
void testEqualNonEmpty();
void testEqualDiffInsert();
void testEqualDiffSize();
void testEqualDiffItems();
void testEqualLargeSet();

void testEmptyDisjoint();
void testNonEmptyDisjoint();
void testNonDisjoint();
void testEqualDisjointSet();

void testEmptySetUnion();
void testOneEmptyUnion();
void testNonEmptyUnion();
void testEqualUnionSet();

void testEmptySymDiff();
void testOneEmptySymDiff();
void testNonEmptySymDiff();
void testNonEmptySymDiff1();
void testOneEmptySymDiff2();

void testMemoryLeakage();

int testsFailed = 0;
int testsPassed = 0;

int main()
{
    printf("\n// =========== Create Set Test ============== //\n");
    testCreateNewSet();
    testCreateMutipleSet();

    printf("\n\n// =========== Delete Set Test ============== //\n");
    testDeleteEmpty();
    testMulitpleCreateDelete();
    testDeleteWithItem();
    testDelLargeSet();

    printf("\n\n// =========== Insert Set Test ============== //\n");
    testSingleInsert();
    testMultipleInsert();
    testInsertLargeItem();
    testDuplicateInsert();
    testInsertNegtiveItem();

    printf("\n\n// =========== Remove Set Test ============== //\n");
    testRemoveItem();
    testRemoveWithMuliItem();
    testRemoveNoExist();
    testEmptySetRemove();
    testMulipleRemove();

    printf("\n\n// =========== Equal Set Test ============== //\n");
    testEqualEmpty();
    testEqualNonEmpty();
    testEqualDiffInsert();
    testEqualDiffSize();
    testEqualDiffItems();
    testEqualLargeSet();

    printf("\n\n// =========== Disjoint Set Test ============== //\n");
    testEmptyDisjoint();
    testNonEmptyDisjoint();
    testNonDisjoint();
    testEqualDisjointSet();

    printf("\n\n// =========== Union Set Test ============== //\n");
    testEmptySetUnion();
    testOneEmptyUnion();
    testNonEmptyUnion();
    testEqualUnionSet();

    printf("\n\n// =========== Symmetric Diff Set Test ============== //\n");
    testEmptySymDiff();
    testOneEmptySymDiff();
    testNonEmptySymDiff();
    testNonEmptySymDiff1();
    testOneEmptySymDiff2();

    printf("\n\n// =========== Validate Memory Set Test ============== //\n");
    testMemoryLeakage();

    printTestSummary();
    printf("\nEnd of processing");
    return EXIT_SUCCESS;
}

// Records test case
void testManager(Boolean passed, char *expected)
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

// Test the creation of a new set
void testCreateNewSet()
{
    char *describe = "Create a new set";
    Boolean testPassed = false;
    Set *set = newSet();

    if (set != NULL)
    {
        testPassed = true;
        // Free up the test process - ie delete the test set.
        deleteSet(set);
    }

    testManager(testPassed, describe);
    testMemoryLeakage();
}

// Tests the creation of multiple sets are independent.
void testCreateMutipleSet()
{
    char *describe = "Create a multiple new set";
    Boolean testPassed = false;

    Set *set1 = newSet();
    Set *set2 = newSet();

    if (&set1 != &set2)
    {
        testPassed = true;
        // Clean up test data used in testing
        deleteSet(set1);
        deleteSet(set2);
    }

    testManager(testPassed, describe);
    testMemoryLeakage();
}

// Tests deleting an empty set.
void testDeleteEmpty()
{
    char *describe = "Delete Empty Set";
    Boolean testPassed = false;

    Set *set = newSet();
    if (set != NULL)
    {
        // Free up the test process - ie delete the test set.
        deleteSet(set);
    }
    testPassed = validateMemUse();

    testManager(testPassed, describe);
}

// Tests creating and deleting multiple sets in a loop.
void testMulitpleCreateDelete()
{
    char *describe = "Create and Delete mulitiple set";
    Boolean testPassed = false;

    for (int i = 1; i <= 50; i++)
    {
        Set *set = newSet();

        if (set != NULL)
        {
            // Free up the test process - ie delete the test set.
            deleteSet(set);
        }
    }

    testPassed = validateMemUse();

    testManager(testPassed, describe);
}

// Tests deleting a set that contains items.
void testDeleteWithItem()
{
    char *describe = "Delete set with items";
    Boolean testPassed = false;

    Set *set = newSet();
    if (set != NULL)
    {
        const int TEST_VAL = 5;
        Boolean inserted = insertItem(set, TEST_VAL);
        if (inserted)
        {
            // Free up the test process - ie delete the test set.
            deleteSet(set);
        }
    }

    testPassed = validateMemUse();

    testManager(testPassed, describe);
}

// Tests deleting a set with a large number of items.
void testDelLargeSet()
{
    char *describe = "Delete set with large items";
    Boolean testPassed = false;

    Set *set = newSet();
    if (set != NULL)
    {
        const int NUM_OF_ITEMS = 100;
        for (int i = 1; i <= NUM_OF_ITEMS; i++)
        {
            insertItem(set, i);
        }

        // Free up the test process - ie delete the test set.
        deleteSet(set);
    }

    testPassed = validateMemUse();

    testManager(testPassed, describe);
}

// Tests inserting a single item into a set.
void testSingleInsert()
{
    char *describe = "Insert single Item";
    Boolean testPassed = false;

    Set *set = newSet();
    if (set != NULL)
    {
        const int TEST_VALUE = 10;
        testPassed = insertItem(set, TEST_VALUE);
        // Free up the test process - ie delete the test set.
        deleteSet(set);
    }

    testManager(testPassed, describe);
    testMemoryLeakage();
}

// Tests inserting multiple items into a set.
void testMultipleInsert()
{
    char *describe = "Insert mulitple Item";
    Boolean testPassed = false;

    Set *set = newSet();
    if (set != NULL)
    {
        const int NUM_OF_ITEMS = 100;
        Boolean inserted = true;
        for (int i = 0; i < NUM_OF_ITEMS && inserted; i++)
        {
            inserted = insertItem(set, i);
        }

        testPassed = inserted;
        // Free up the test process - ie delete the test set.
        deleteSet(set);
    }

    testManager(testPassed, describe);
    testMemoryLeakage();
}

// Tests inserting a large item into a set.
void testInsertLargeItem()
{
    char *describe = "Insert large Item";
    Boolean testPassed = false;

    Set *set = newSet();
    if (set != NULL)
    {
        const int TEST_VALUE = 2147483646;
        testPassed = insertItem(set, TEST_VALUE);
        // Free up the test process - ie delete the test set.
        deleteSet(set);
    }

    testManager(testPassed, describe);
    testMemoryLeakage();
}

// Test case to verify that duplicate items are not inserted into the data structure.
void testDuplicateInsert()
{
    char *describe = "Not Insert Duplicate Item";
    Boolean testPassed = false;

    Set *set = newSet();
    if (set != NULL)
    {
        const int TEST_VALUE = 10;
        Boolean insert = insertItem(set, TEST_VALUE);
        if (insert)
        {
            testPassed = !(insertItem(set, TEST_VALUE));
        }

        // Free up the test process - ie delete the test set.
        deleteSet(set);
    }

    testManager(testPassed, describe);
    testMemoryLeakage();
}

// Test case to verify that negative items are handled properly during insertion.
void testInsertNegtiveItem()
{
    char *describe = "Insert Negative Item";
    Boolean testPassed = false;

    Set *set = newSet();
    if (set != NULL)
    {
        const int TEST_VALUE = -20;
        testPassed = insertItem(set, TEST_VALUE);
        // Free up the test process - ie delete the test set.
        deleteSet(set);
    }

    testManager(testPassed, describe);
    testMemoryLeakage();
}

// Test case to verify the removal of an existing item from the data structure.
void testRemoveItem()
{
    char *describe = "Remove exisiting item";
    Boolean testPassed = false;

    Set *set = newSet();
    if (set != NULL)
    {
        const int TEST_VALUE = 20;
        Boolean inserted = insertItem(set, TEST_VALUE);
        if (inserted)
        {
            testPassed = removeItem(set, TEST_VALUE);
        }
        // Free up the test process - ie delete the test set.
        deleteSet(set);
    }

    testManager(testPassed, describe);
    testMemoryLeakage();
}

void testRemoveWithMuliItem()
{
    char *describe = "Remove with mutiple item in set";
    Boolean testPassed = false;

    Set *set = newSet();
    if (set != NULL)
    {
        const int TEST_VALUE = 20;
        const int TEST_VALUE1 = 21;
        Boolean inserted = insertItem(set, TEST_VALUE);
        Boolean inserted1 = insertItem(set, TEST_VALUE1);
        if (inserted && inserted1)
        {
            testPassed = removeItem(set, TEST_VALUE);
        }
        // Free up the test process - ie delete the test set.
        deleteSet(set);
    }

    testManager(testPassed, describe);
    testMemoryLeakage();
}

// Test case to verify that an item that doesn't exist in the set cannot be removed.
void testRemoveNoExist()
{
    char *describe = "Not Remove item that doesnt exist";
    Boolean testPassed = false;
    Set *set = newSet();
    if (set != NULL)
    {
        const int TEST_VALUE = 20;
        Boolean inserted = insertItem(set, 10);
        if (inserted)
        {
            testPassed = !(removeItem(set, TEST_VALUE));
        }
        // Free up the test process - ie delete the test set.
        deleteSet(set);
    }
    testManager(testPassed, describe);
    testMemoryLeakage();
}

// Test case to verify that no items can be removed from an empty set.
void testEmptySetRemove()
{
    char *describe = "Not Remove item from empty set";
    Boolean testPassed = false;

    Set *set = newSet();
    if (set != NULL)
    {
        const int TEST_VALUE = 20;
        testPassed = !(removeItem(set, TEST_VALUE));
        // Free up the test process - ie delete the test set.
        deleteSet(set);
    }

    testManager(testPassed, describe);
    testMemoryLeakage();
}

// Test case to verify the removal of multiple items from a set.
void testMulipleRemove()
{
    char *describe = "Remove mulitple Item";
    Boolean testPassed = false;

    Set *set = newSet();
    if (set != NULL)
    {
        const int NUM_OF_ITEMS = 100;
        Boolean inserted = true;
        for (int i = 1; i <= NUM_OF_ITEMS && inserted; i++)
        {
            inserted = insertItem(set, i);
        }

        Boolean remove = true;
        if (inserted)
        {
            for (int i = 100; i > 0 && remove; i--)
            {
                remove = removeItem(set, i);
            }
        }

        testPassed = remove;
        // Free up the test process - ie delete the test set.
        deleteSet(set);
    }

    testManager(testPassed, describe);
    testMemoryLeakage();
}

// Test case to verify that two empty sets are equal.
void testEqualEmpty()
{
    char *describe = "Be equal when two set are empty";
    Boolean testPassed = false;

    Set *set1 = newSet();
    Set *set2 = newSet();

    if (set1 != NULL && set2 != NULL)
    {
        testPassed = areEqual(set1, set2);

        // Clean up test data used in testing
        deleteSet(set1);
        deleteSet(set2);
    }

    testManager(testPassed, describe);
    testMemoryLeakage();
}

// Test case to verify that two non-empty sets with the same items are equal.
void testEqualNonEmpty()
{
    char *describe = "Be equal when two non empty set with same item";
    Boolean testPassed = false;

    Set *set1 = newSet();
    Set *set2 = newSet();
    if (set1 != NULL && set2 != NULL)
    {
        const int NUM_OF_ITEMS = 3;
        for (int i = 1; i <= NUM_OF_ITEMS; i++)
        {
            insertItem(set1, i);
            insertItem(set2, i);
        }
        testPassed = areEqual(set1, set2);

        // Clean up test data used in testing
        deleteSet(set1);
        deleteSet(set2);
    }

    testManager(testPassed, describe);
    testMemoryLeakage();
}

// Test case to verify that two sets with the same items in different orders are equal.
void testEqualDiffInsert()
{
    char *describe = "Be equal when two non empty set with same item in different order";
    Boolean testPassed = false;

    Set *set1 = newSet();
    Set *set2 = newSet();
    if (set1 != NULL && set2 != NULL)
    {
        const int NUM_OF_ITEMS = 3;
        for (int i = 1; i <= NUM_OF_ITEMS; i++)
        {
            insertItem(set1, i);
        }
        for (int i = NUM_OF_ITEMS; i > 0; i--)
        {
            insertItem(set2, i);
        }
        testPassed = areEqual(set1, set2);

        // Clean up test data used in testing
        deleteSet(set1);
        deleteSet(set2);
    }

    testManager(testPassed, describe);
    testMemoryLeakage();
}

// Test case to verify that two sets with different sizes are not equal.
void testEqualDiffSize()
{
    char *describe = "Be not be equal with two different size";
    Boolean testPassed = false;

    Set *set1 = newSet();
    Set *set2 = newSet();

    if (set1 != NULL && set2 != NULL)
    {
        const int NUM_OF_ITEMS = 3;
        for (int i = 1; i <= NUM_OF_ITEMS; i++)
        {
            insertItem(set1, i);
        }

        for (int i = 1; i <= NUM_OF_ITEMS - 1; i++)
        {
            insertItem(set2, i);
        }
        testPassed = !(areEqual(set1, set2));

        // Clean up test data used in testing
        deleteSet(set1);
        deleteSet(set2);
    }

    testManager(testPassed, describe);
    testMemoryLeakage();
}

// Test case to verify that two sets with different items are not equal.
void testEqualDiffItems()
{
    char *describe = "Be not be equal with two different items";
    Boolean testPassed = false;

    Set *set1 = newSet();
    Set *set2 = newSet();

    if (set1 != NULL && set2 != NULL)
    {
        const int NUM_OF_ITEMS = 3;
        for (int i = 1; i <= NUM_OF_ITEMS; i++)
        {
            insertItem(set1, i);
        }

        for (int i = 1; i <= NUM_OF_ITEMS; i *= 3)
        {
            insertItem(set2, i);
        }
        testPassed = !(areEqual(set1, set2));

        // Clean up test data used in testing
        deleteSet(set1);
        deleteSet(set2);
    }

    testManager(testPassed, describe);
    testMemoryLeakage();
}

// Test case to verify that two large sets with identical elements are equal.
void testEqualLargeSet()
{
    char *describe = "Be equal when two non empty set with same item when large";
    Boolean testPassed = false;

    Set *set1 = newSet();
    Set *set2 = newSet();
    if (set1 != NULL && set2 != NULL)
    {
        const int NUM_OF_ITEMS = 1000;
        for (int i = 1; i <= NUM_OF_ITEMS; i++)
        {
            insertItem(set1, i);
            insertItem(set2, i);
        }
        testPassed = areEqual(set1, set2);

        // Clean up test data used in testing
        deleteSet(set1);
        deleteSet(set2);
    }

    testManager(testPassed, describe);
    testMemoryLeakage();
}

// Test case to verify that two empty sets are disjoint.
void testEmptyDisjoint()
{
    char *describe = "Be disjoint when two set are empty";
    Boolean testPassed = false;

    Set *set1 = newSet();
    Set *set2 = newSet();

    if (set1 != NULL && set2 != NULL)
    {
        testPassed = areDisjoint(set1, set2);

        // Clean up test data used in testing
        deleteSet(set1);
        deleteSet(set2);
    }
    testManager(testPassed, describe);
    testMemoryLeakage();
}

// Test case to verify that two sets with no common items are disjoint.
void testNonEmptyDisjoint()
{
    char *describe = "Be disjoint when two set dont have anything common item";
    Boolean testPassed = false;

    Set *set1 = newSet();
    Set *set2 = newSet();

    if (set1 != NULL && set2 != NULL)
    {
        const int NUM_OF_ITEMS = 5;
        for (int i = 1; i <= NUM_OF_ITEMS; i++)
        {
            insertItem(set1, i);
        }

        for (int i = NUM_OF_ITEMS + 1; i <= NUM_OF_ITEMS * 2; i++)
        {
            insertItem(set2, i);
        }

        testPassed = areDisjoint(set1, set2);
        // Clean up test data used in testing
        deleteSet(set1);
        deleteSet(set2);
    }

    testManager(testPassed, describe);
    testMemoryLeakage();
}

// Test case to verify that two sets with common items are not disjoint.
void testNonDisjoint()
{
    char *describe = "Not be disjoint when two set  have anything common item";
    Boolean testPassed = false;

    Set *set1 = newSet();
    Set *set2 = newSet();

    if (set1 != NULL && set2 != NULL)
    {
        const int NUM_OF_ITEMS = 5;
        for (int i = 1; i <= NUM_OF_ITEMS; i++)
        {
            insertItem(set1, i);
        }

        for (int i = NUM_OF_ITEMS; i <= NUM_OF_ITEMS * 2; i++)
        {
            insertItem(set2, i);
        }

        testPassed = !(areDisjoint(set1, set2));
        // Clean up test data used in testing
        deleteSet(set1);
        deleteSet(set2);
    }

    testManager(testPassed, describe);
    testMemoryLeakage();
}

// Test case to verify that two equal sets are not disjoint.
void testEqualDisjointSet()
{
    char *describe = "Not be disjoint when two set are equal";
    Boolean testPassed = false;

    Set *set1 = newSet();
    Set *set2 = newSet();
    if (set1 != NULL && set2 != NULL)
    {
        const int NUM_OF_ITEMS = 3;
        for (int i = 1; i <= NUM_OF_ITEMS; i++)
        {
            insertItem(set1, i);
            insertItem(set2, i);
        }
        testPassed = areEqual(set1, set2) && !(areDisjoint(set1, set2));

        // Clean up test data used in testing
        deleteSet(set1);
        deleteSet(set2);
    }

    testManager(testPassed, describe);
    testMemoryLeakage();
}

// Test case to verify the union of two empty sets.
void testEmptySetUnion()
{
    char *describe = "Be Union when both sets are empty";
    Boolean testPassed = false;

    Set *set1 = newSet();
    Set *set2 = newSet();
    if (set1 != NULL && set2 != NULL)
    {
        Set *unionSet = unionOf(set1, set2);
        testPassed = areEqual(set1, unionSet);

        // Clean up test data used in testing
        deleteSet(set1);
        deleteSet(set2);
        deleteSet(unionSet);
    }

    testManager(testPassed, describe);
    testMemoryLeakage();
}

// Test case to verify the union when one set is empty and the other is not.
void testOneEmptyUnion()
{
    char *describe = "Be Union of two set to equal set B when set A is empty";
    Boolean testPassed = false;

    Set *set1 = newSet();
    Set *set2 = newSet();
    if (set1 != NULL && set2 != NULL)
    {
        const int NUM_OF_ITEMS = 3;
        for (int i = 1; i <= NUM_OF_ITEMS; i++)
        {
            insertItem(set2, i);
        }

        Set *unionSet = unionOf(set1, set2);
        testPassed = areEqual(set2, unionSet);

        // Clean up test data used in testing
        deleteSet(set1);
        deleteSet(set2);
        deleteSet(unionSet);
    }

    testManager(testPassed, describe);
    testMemoryLeakage();
}

// Test case to verify the union of two non-empty sets.
void testNonEmptyUnion()
{
    char *describe = "Be Union of two non empty set";
    Boolean testPassed = false;

    Set *set1 = newSet();
    Set *set2 = newSet();
    if (set1 != NULL && set2 != NULL)
    {
        const int NUM_OF_ITEMS = 5;
        for (int i = 1; i <= NUM_OF_ITEMS; i++)
        {
            insertItem(set1, i);
        }

        for (int i = NUM_OF_ITEMS; i <= NUM_OF_ITEMS * 2; i++)
        {
            insertItem(set2, i);
        }

        Set *expetedSet = newSet();

        for (int i = 1; i <= NUM_OF_ITEMS * 2; i++)
        {
            insertItem(expetedSet, i);
        }
        Set *unionSet = unionOf(set1, set2);
        testPassed = areEqual(expetedSet, unionSet);

        // Clean up test data used in testing
        deleteSet(set1);
        deleteSet(set2);
        deleteSet(expetedSet);
        deleteSet(unionSet);
    }
    testManager(testPassed, describe);
    testMemoryLeakage();
}

// Test case to verify the union of two equal sets.
void testEqualUnionSet()
{
    char *describe = "Be union of two equal set1 or set2. (equal to either set1 or set2)";
    Boolean testPassed = false;

    Set *set1 = newSet();
    Set *set2 = newSet();
    if (set1 != NULL && set2 != NULL)
    {
        const int NUM_OF_ITEMS = 3;
        for (int i = 1; i <= NUM_OF_ITEMS; i++)
        {
            insertItem(set1, i);
            insertItem(set2, i);
        }
        Set *unionSet = unionOf(set1, set2);
        testPassed = areEqual(set1, unionSet) && areEqual(set2, unionSet);

        // Clean up test data used in testing
        deleteSet(set1);
        deleteSet(set2);
        deleteSet(unionSet);
    }

    testManager(testPassed, describe);
    testMemoryLeakage();
}

// Test case to verify the symmetric difference when both sets are empty.
void testEmptySymDiff()
{
    char *describe = "Be Symmetric when both sets are empty";
    Boolean testPassed = false;

    Set *set1 = newSet();
    Set *set2 = newSet();
    if (set1 != NULL && set2 != NULL)
    {
        Set *symDiff = symmetricDifferenceOf(set1, set2);
        testPassed = areEqual(set1, symDiff);

        // Clean up test data used in testing
        deleteSet(set1);
        deleteSet(set2);
        deleteSet(symDiff);
    }

    testManager(testPassed, describe);
    testMemoryLeakage();
}

// Test case to verify the symmetric difference when one set is empty and the other is not.
void testOneEmptySymDiff()
{
    char *describe = "Be symmetric of two set to equal set B when set A is empty";
    Boolean testPassed = false;

    Set *set1 = newSet();
    Set *set2 = newSet();
    if (set1 != NULL && set2 != NULL)
    {
        const int NUM_OF_ITEMS = 3;
        for (int i = 1; i <= NUM_OF_ITEMS; i++)
        {
            insertItem(set2, i);
        }

        Set *symDiff = symmetricDifferenceOf(set1, set2);
        testPassed = areEqual(set2, symDiff);

        // Clean up test data used in testing
        deleteSet(set1);
        deleteSet(set2);
        deleteSet(symDiff);
    }

    testManager(testPassed, describe);
    testMemoryLeakage();
}

// Test case to verify the symmetric difference between two sets with no elements in common.
void testNonEmptySymDiff()
{
    char *describe = "Be Symmetric diffrence with no element in common";
    Boolean testPassed = false;

    Set *set1 = newSet();
    Set *set2 = newSet();
    if (set1 != NULL && set2 != NULL)
    {
        const int NUM_OF_ITEMS = 5;
        for (int i = 1; i <= NUM_OF_ITEMS; i++)
        {
            insertItem(set1, i);
        }

        for (int i = NUM_OF_ITEMS + 1; i <= NUM_OF_ITEMS * 2; i++)
        {
            insertItem(set2, i);
        }

        Set *expetedSet = unionOf(set1, set2);
        Set *symDiff = symmetricDifferenceOf(set1, set2);
        testPassed = areEqual(expetedSet, symDiff);

        // Clean up test data used in testing
        deleteSet(set1);
        deleteSet(set2);
        deleteSet(expetedSet);
        deleteSet(symDiff);
    }

    testManager(testPassed, describe);
    testMemoryLeakage();
}

// Test case to verify the symmetric difference between two sets with elements in common.
void testNonEmptySymDiff1()
{
    char *describe = "Be Symmetric diffrence with element in common";
    Boolean testPassed = false;

    Set *set1 = newSet();
    Set *set2 = newSet();
    if (set1 != NULL && set2 != NULL)
    {
        const int NUM_OF_ITEMS = 3;
        for (int i = 1; i <= NUM_OF_ITEMS; i++)
        {
            insertItem(set1, i);
        }

        for (int i = NUM_OF_ITEMS; i <= NUM_OF_ITEMS * 2; i++)
        {
            insertItem(set2, i);
        }

        Set *expetedSet = newSet();
        int expected[5] = {1, 2, 4, 5, 6};

        for (int i = 0; i < 5; i++)
        {
            insertItem(expetedSet, expected[i]);
        }

        Set *symDiff = symmetricDifferenceOf(set1, set2);
        testPassed = areEqual(expetedSet, symDiff);

        // Clean up test data used in testing
        deleteSet(set1);
        deleteSet(set2);
        deleteSet(expetedSet);
        deleteSet(symDiff);
    }

    testManager(testPassed, describe);
    testMemoryLeakage();
}

// Test case to verify that the symmetric difference between two equal sets results in an empty set.
void testOneEmptySymDiff2()
{
    char *describe = "Be emmpty set for symmetic differnce of equal sets";
    Boolean testPassed = false;

    Set *set1 = newSet();
    Set *set2 = newSet();
    if (set1 != NULL && set2 != NULL)
    {
        const int NUM_OF_ITEMS = 3;
        for (int i = 1; i <= NUM_OF_ITEMS; i++)
        {
            insertItem(set1, i);
            insertItem(set2, i);
        }

        Set *expected = newSet();

        Set *symDiff = symmetricDifferenceOf(set1, set2);
        testPassed = areEqual(expected, symDiff);

        // Clean up test data used in testing
        deleteSet(set1);
        deleteSet(set2);
        deleteSet(expected);
        deleteSet(symDiff);
    }

    testManager(testPassed, describe);
    testMemoryLeakage();
}

void testMemoryLeakage()
{
    Boolean leakageCheck = validateMemUse();
    if (!leakageCheck)
    {
        printf("\n\t Memeory leak!!!!!");
    } 
}