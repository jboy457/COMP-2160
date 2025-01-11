# Test Suite for Set Operations

This document provides a summary of test cases and their outcomes for the `SymmetricDifferenceOf`, `areDisjoint`, `insertItem`, `removeItem`, and `unionOf` functions across different sets.

---

## **Set1**
- **Status**: All test cases passed.
- **Notes**: The implementation for this set is functioning as expected.

---

## **Set2**
### areDisjoint()
- **Issue**: Memory leaks observed during the implementation.

---

## **Set3**
### insertItem()
- **FAILED**: Expected not to insert duplicate items, but duplicates were still added.

### removeItem()
- **FAILED**: Expected to:
  - Remove an existing item, but failed.
  - Remove items when multiple are present in the set, but failed.
  - Remove multiple items, but failed.

---

## **Set4**
### SymmetricDifferenceOf()
- **FAILED**:
  - Expected to calculate the symmetric difference with elements in common, but failed.
  - Expected to return an empty set for the symmetric difference of two equal sets, but failed.

### areDisjoint()
- **FAILED**:
  - Expected to be disjoint when two sets have no common items, but failed.

---

## **Set5**
### areDisjoint()
- **FAILED**:
  - Expected to **not** be disjoint when two sets have any common items, but failed.
  - Expected to **not** be disjoint when two sets are equal, but failed.

---

## **Set6**
### Equality Tests
- **FAILED**:
  - Expected to be equal when:
    - Both sets are empty, but failed.
    - Both non-empty sets have the same items, but failed.
    - Non-empty sets have the same items in different orders, but failed.
    - Non-empty sets are large but have identical items, but failed.

### areDisjoint()
- **FAILED**:
  - Expected to be disjoint when:
    - Both sets are empty, but failed.
    - Two sets have no common items, but failed.
  - Expected to **not** be disjoint when two sets are equal, but failed.

### unionOf()
- **FAILED**:
  - Expected union to:
    - Be correct when both sets are empty, but failed.
    - Equal set B when set A is empty, but failed.
    - Handle two non-empty sets, but failed.
    - Handle two equal sets correctly (result equal to either set), but failed.

### SymmetricDifferenceOf()
- **FAILED**:
  - Expected symmetric difference to:
    - Be correct when both sets are empty, but failed.
    - Equal set B when set A is empty, but failed.
    - Handle sets with no elements in common, but failed.
    - Handle sets with elements in common, but failed.
    - Return an empty set for the symmetric difference of two equal sets, but failed.

---

## Summary of Failures
### Major Issues Identified:
1. **SymmetricDifferenceOf**:
   - Incorrect results for common elements, empty sets, and equal sets.
2. **areDisjoint**:
   - Fails in disjointness checks for empty sets, equal sets, and sets with common elements.
3. **insertItem**:
   - Duplicate item insertion is not handled properly.
4. **removeItem**:
   - Cannot remove existing items reliably, especially in sets with multiple items.
5. **unionOf**:
   - Union logic fails for empty, non-empty, and equal sets.

### Memory Management
- Memory leaks detected in the implementation of `areDisjoint` in Set2.

---

## Recommendations
1. Investigate and resolve the logic errors in:
   - Symmetric difference calculations.
   - Disjointness checks.
   - Union handling for edge cases.
2. Improve handling of duplicate and removal operations.
3. Optimize memory management, especially in `areDisjoint`.

---

This document provides a comprehensive overview of the current state of testing and should serve as a guide for further debugging and development.
