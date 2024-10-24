//-----------------------------------------
// NAME: Adejare Taiwo 
// STUDENT NUMBER: 7980132
// COURSE: COMP 2160, SECTION: A01
// INSTRUCTOR: Saulo Lectures 
// ASSIGNMENT: assignment 2, QUESTION: question 1, 2
// 
// REMARKS: Implement a simulation where a robot probe explores a series of zones on an asteroid
//
//-----------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include<ctype.h>
//-------------------------------------------------------------------------------------
// CONSTANTS and TYPES
//-------------------------------------------------------------------------------------

#define MAX_DIMENSION 20
#define BUFFERSIZE 128
// constant definitions for the different cell states
const char WALL = 'W';
const char OPEN_SPACE = '0';
const char VISITED = '.';
const char PROBE = 'L';
const char UNREADABLE = 'X';

// Boolean enum
typedef enum BOOL
{
  false,
  true
} Boolean;

// struct CELL
struct CELL
{
  int row;
  int column;
};
typedef struct CELL Cell;

// struct CELL_NODE
typedef struct CELL_NODE CellNode;
struct CELL_NODE
{
  Cell cell;
  CellNode *next;
};

// Represents a zone
typedef struct ZONE
{
  char zoneArea[MAX_DIMENSION][MAX_DIMENSION];
  int zoneID;
  Cell probe;
  int zoneRows;
  int zoneCols;
  // Add the information about the zone you want to store
  int avaialble;
  int richArea;
  int accessible;
  int unreadable;
  int esitmated;
  int totalAvailable;

} zoneData;

// represents a probe data (List of ZoneData)
typedef struct PROBE probeData;
struct PROBE
{
  zoneData *probedZone;
  // Other Probe Metadata information
  probeData *next;
};

//-------------------------------------------------------------------------------------
// VARIABLES
//-------------------------------------------------------------------------------------

// Global top for the backtracking alg
CellNode *top = NULL;
// Global probe data
probeData *probe = NULL;

//-------------------------------------------------------------------------------------
// PROTOTYPES
//-------------------------------------------------------------------------------------

// basic cell manipulation routines for backtracking
// returns a new cell object
Cell makeCell(const int row, const int col);

// returns true if there are no more cells to try
Boolean noMoreCells();

// returns the next cell to try for exploring the zone
Cell nextCell();

// introduces a new cell to try
void addCell(const Cell);

void pushCell(zoneData *zone, Cell currentCell);
 /////////
// routines for managing the zone and probe
//////////////////////////////////////////////

// Initialize a zone and return a pointer to it.
zoneData *makeZone();

// returns true if the cell is a resource
Boolean isResource(char ch);

void addToProbe(zoneData *zone, Cell currentCell);

// print the zone information
void printZoneSummary(zoneData *zone);
void printZone(zoneData *zone);

// Read a zone information from STDIN into the probe
Boolean loadZone();

// explores the zone
void exploreZone(zoneData *zone);

// function to compute the resources in the zone
void computeResource(zoneData *zone, Cell cell);

// Estimates the unreadable resources in the zone
void estimateResources(zoneData *zone);

// Insert the zone into the probe data
void insertInOrder(zoneData *zone);

// Print the summary of the probe findings
void printProbeSummary();

// Invariants

void checkzone(zoneData *zone);
void checkState();
void validProbeCell(zoneData *zone);
void checkCell(Cell cell);

//-------------------------------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
  int numberOfZones = 0;
  while (loadZone())
  {
    numberOfZones++;
    printf("%d Zone(s) loaded.\n", numberOfZones);
    printf("====================================\n");
  }
  printProbeSummary();
  free(probe);
  printf("End of processing");
  return EXIT_SUCCESS;
}

//////////////////////////////////////////////
// Cell routines
//////////////////////////////////////////////


Cell nextCell() 
{
  Cell* next = NULL;
  if(!noMoreCells()) {
    next = &top->cell;
  } else {
    next = (Cell*)malloc(sizeof(Cell));
    next->row = -1;
    next->column = -1;
  }
  assert(next != NULL);
  return *next;
}


Cell makeCell(const int row, const int col) {
  Cell* newCell = (Cell*)malloc(sizeof(Cell));
  newCell->row = row;
  newCell->column = col;

  return *newCell;
}


Boolean noMoreCells() {
  return top == NULL ? true : false;
}

//////////////////////////////////////////////
// List routines
//////////////////////////////////////////////

//------------------------------------------------------
// pushProbe
//
// PURPOSE: Pushes a cell node onto the probe.
//
// INPUT PARAMETERS:
// - cell: The Cell structure to be pushed onto the cell Node to be probed.
//
// OUTPUT PARAMETERS:
// None
//------------------------------------------------------
void pushProbe(const Cell cell) {
  // Check if the cell is not empty
 checkCell(cell);

  CellNode* newCellNode = (CellNode*)malloc(sizeof(CellNode));

  // Check if memory was allocated to new Cell node.
  assert(newCellNode != NULL);

  newCellNode->cell = cell;
  newCellNode->next = top;
  top = newCellNode;

  // Check if top truely points to newCellNode.
  assert(top == newCellNode);
}

void popProbe() {
  // Make sure list isnt empty.
  assert(top != NULL);

  if(top != NULL) {
    CellNode* cellToRemove = top;
    top = top->next;
    // Confirm that top now point to its next value in the list
    assert(top != cellToRemove);
    free(cellToRemove);
  } else {
    printf("Probe list is empty.");
  }
}

//////////////////////////////////////////////
// Zone routines
//////////////////////////////////////////////



//------------------------------------------------------
// loadZone
//
// PURPOSE: Loads zone information from STDIN into the probe.
//
// INPUT PARAMETERS:
// None
//
// OUTPUT PARAMETERS:
// Returns TRUE if successful, FALSE if an error occurs or not reached end of file..
//------------------------------------------------------
Boolean loadZone() 
{
  char line[BUFFERSIZE];
  char zoneName[BUFFERSIZE];

  // Allocate memoery to zone. 
  zoneData *zoneToLoad = makeZone();

  // Check if memory is allocated to new zone to load.
  checkzone(zoneToLoad);

  // Read Zone Name and ID
  if(fgets(line, BUFFERSIZE, stdin) != NULL) {
    if(sscanf(line, "%s %d", zoneName, &zoneToLoad->zoneID) != 2) {
      printf("Error zone format inputed.\n");
      return false;
    };
  }

  // Read Rows and Column
  if(fgets(line, BUFFERSIZE, stdin) != NULL) {
    if(sscanf(line, "%d %d", &zoneToLoad->zoneRows, &zoneToLoad->zoneCols) != 2) {
      printf("Error zone row and column format inputed.\n");
      return false;
    };
  }
  
  // Read Zone Grid
  int tempRow = 0; // For row placement.
  while (tempRow < zoneToLoad->zoneRows) {
    if(fgets(line, BUFFERSIZE, stdin) != NULL) {
      int tempCol = 0; // For column placement.

      for(int i = 0; i < strlen(line); i++) {
        // Check if the char is no a space
        if(isspace(line[i]) == 0) {
          // Assign char to cell.
          zoneToLoad->zoneArea[tempRow][tempCol] = line[i];

          // Assign Probe Point.
          if(line[i] == PROBE) {
            zoneToLoad->probe = makeCell(tempRow, tempCol);
          }
          tempCol++;
        } 
      }
      tempRow++;
    }
  }


  // Check if zone has a place to land.
  if(zoneToLoad->probe.row < 1) {
    printf("\nZone %d has no landing cell\n", zoneToLoad->zoneID);
  } else {
    printf("Zone %d Map:\n\n", zoneToLoad->zoneID);
    printZone(zoneToLoad);

    // Probe Zone Process.
    exploreZone(zoneToLoad);
    estimateResources(zoneToLoad);
    insertInOrder(zoneToLoad);
  }


  if(feof(stdin)) {
    return false;
  } else {
    return true;
  }  
}

//------------------------------------------------------
// makeZone
//
// PURPOSE: Initializes and allocates memory a new zone and returns a pointer to it.
//
// INPUT PARAMETERS:
// None
//
// OUTPUT PARAMETERS:
// Returns a pointer to a zoneData structure.
//------------------------------------------------------
zoneData *makeZone() {
  zoneData* newZone = (zoneData*)malloc(sizeof(zoneData));
  // Check if memory was allocated to newZone.
  checkzone(newZone);

  return newZone;
}


//------------------------------------------------------
// exploreZone
//
// PURPOSE: Explores the zone.
//
// INPUT PARAMETERS:
// - zone: A pointer to the zoneData structure to be explored.
//
// OUTPUT PARAMETERS:
// - void
//------------------------------------------------------
void exploreZone(zoneData *zone) {
  // Pre condidtion check on zone data.
  checkzone(zone);
  validProbeCell(zone);

  // Initalize linked list with probe cell i.e start cell.
  pushProbe(zone->probe);

  // Check if cell node is already initailzed
  assert(top != NULL);

  Cell* currentCell = &zone->probe;

  // NOTE: if probe has been checked 
  while(!noMoreCells()) {
    // Compute resources for zone and visit it.
    computeResource(zone, *currentCell);
    zone->zoneArea[currentCell->row][currentCell->column] = VISITED;
    
    // Check is cell has been truely marked as visited.
    assert(zone->zoneArea[currentCell->row][currentCell->column] == VISITED);

    // Remove item probbed Cell form list
    popProbe();
    
    // search and add free cells around to cell node.
    addToProbe(zone, *currentCell);

    Cell temp = nextCell();
    currentCell = &temp;

    assert(currentCell->row == temp.row);
    assert(currentCell->column == temp.column);
  }
}

//------------------------------------------------------
// addToProbe
//
// PURPOSE: Adds cell to probe after checking top, down, left, right.
//
// INPUT PARAMETERS:
// - zone: A pointer to the zoneData structure.
// - currentCell: The current Cell structure to be checked
//
// OUTPUT PARAMETERS:
// None
//------------------------------------------------------
void addToProbe(zoneData *zone, Cell currentCell) {
  // Pre-condtional check to make sure zone and currentcell is valid.
  checkzone(zone);
  checkCell(currentCell);

  // Current cell has to be in zone.
  assert(currentCell.row < zone->zoneRows);
  assert(currentCell.column < zone->zoneCols);

  //Make sure is within the Zone Grid.
  if(currentCell.row > 0 && 
      currentCell.row < zone->zoneRows && 
      currentCell.column > 0 && 
      currentCell.column < zone->zoneCols
    ) {

      // Check UP and push if resource exist
      if (zone->zoneArea[currentCell.row - 1][currentCell.column] != WALL && 
          zone->zoneArea[currentCell.row - 1][currentCell.column] != VISITED &&
          currentCell.row - 1 > -1
        ) {
          Cell newCell = makeCell(currentCell.row - 1, currentCell.column);
          checkCell(newCell);
          
          pushProbe(newCell);
      } 
      
      // Check DOWN and push if resource exist
      if (zone->zoneArea[currentCell.row + 1][currentCell.column] != WALL &&
          zone->zoneArea[currentCell.row + 1][currentCell.column] != VISITED &&
          currentCell.row + 1 < zone->zoneRows
        ) {
          Cell newCell = makeCell(currentCell.row + 1, currentCell.column);
          checkCell(newCell); 
          pushProbe(newCell);
      } 
      
      // Check LEFT
      if (zone->zoneArea[currentCell.row][currentCell.column - 1] != WALL &&
          zone->zoneArea[currentCell.row][currentCell.column - 1] != VISITED && 
          currentCell.column - 1 > -1
        ) {
          Cell newCell = makeCell(currentCell.row, currentCell.column - 1);
          checkCell(newCell);
          pushProbe(newCell);
      }
      
      // Check RIGHT
      if (zone->zoneArea[currentCell.row][currentCell.column + 1] != WALL &&
          zone->zoneArea[currentCell.row][currentCell.column + 1] != VISITED &&
          currentCell.column + 1  < zone->zoneCols
        ) {
          Cell newCell = makeCell(currentCell.row, currentCell.column + 1);
          checkCell(newCell);
          pushProbe(newCell);
      }
    }
}

//------------------------------------------------------
// computeResource
//
// PURPOSE: Computes the resources in the specified zone based on the given cell.
//
// INPUT PARAMETERS:
// - zone: A pointer to the zoneData structure.
// - cell: The Cell structure used for resource computation.
//
// OUTPUT PARAMETERS:
// None
//------------------------------------------------------
void computeResource(zoneData *zone, Cell rcell) {
  // Pre condidtion check on zone data and cell is valid.
  checkzone(zone);
  checkCell(rcell);

  char cell = zone->zoneArea[rcell.row][rcell.column];

  // Number of areas accessible to the probe
  zone->avaialble++;

  // Rich Areas and also sum with accessible.
  if(isResource(cell)) {
    zone->richArea++;

    // Sum up all the resouces
    zone->accessible +=  cell - '0';

  }

  // Unreadable Cells with poetentials.
  if(cell == UNREADABLE) {
    zone->unreadable++;
    // Potential resource
    zone->richArea++;
  }
}

void estimateResources(zoneData *zone) {
  // Check if zone is valid.
  checkzone(zone);

  // Calculuate the estimated and total available resource.
  zone->esitmated = zone->accessible / (zone->richArea - zone->unreadable);
  zone->totalAvailable = zone->accessible + zone->esitmated;

  // Post condition to make sure calculation is right.
  assert(zone->esitmated == zone->accessible / (zone->richArea - zone->unreadable));
}

//------------------------------------------------------
// isResource
//
// PURPOSE: Returns TRUE if the character represents a resource; otherwise, FALSE.
//
// INPUT PARAMETERS:
// - ch: The character to be checked.
//
// OUTPUT PARAMETERS:
// Returns a Boolean value indicating if the character is a resource.
//------------------------------------------------------
Boolean isResource(char ch) {
  assert(ch != '\0');

  // Resource range.
  const int RESOURCE_START = 1;
  const int RESOURCE_END = 9;
  Boolean isResource = false;
  
  if(isdigit(ch) != 0) {
    // make sure its a digit before converting to int;
    assert(isdigit(ch) != 0);

    int num = ch - '0';
    // Check if within 0 - 9 i.e resource range.
    if(num >= RESOURCE_START && num <= RESOURCE_END) {
      isResource = true;
    }
  }
  return isResource;
}

//------------------------------------------------------
// insertInOrder
//
// PURPOSE: Inserts the specified zone into the probe data in an ordered manner.
//
// INPUT PARAMETERS:
// - zone: A pointer to the probed zoneData structure to be inserted.
//
// OUTPUT PARAMETERS:
// None
//------------------------------------------------------
void insertInOrder(zoneData *zone) {
  checkzone(zone);
  probeData* curr = probe;
  probeData* prev = NULL;

  while (curr != NULL && curr->probedZone->totalAvailable > zone->totalAvailable) {
      assert(curr != NULL);
      prev = curr;
      curr = curr->next;   
  }

  probeData* newProbe = (probeData*)malloc(sizeof(probeData));
  newProbe->probedZone = zone;

  // printZone(newProbe->probedZone);

  if(prev == NULL) {
    newProbe->next = curr;
    probe = newProbe;
  } else {
    newProbe->next = curr;
    prev->next = newProbe;
  }
}

void printZoneSummary(zoneData *zone) {
  checkzone(zone);

  printf("\nSummary Zone %d: \n", zone->zoneID);
  printf("-> Avaialble areas: %d \n", zone->avaialble);
  printf("-> Resource rich areas: %d \n", zone->richArea);
  printf("-> Resource accessible: %d \n", zone->accessible);
  printf("-> Resource unreadable: %d \n", zone->unreadable);
  printf("-> Esitmated potential resources: %d \n", zone->esitmated);
  printf("-> Total resource availability: %d \n", zone->totalAvailable);

}



void printProbeSummary() {
  probeData* curr = probe;

  while(curr != NULL) {
    printZoneSummary(curr->probedZone);
    printf("\nProbed Mapping: \n");
    printZone(curr->probedZone);
    curr = curr->next;
    // free each probe after printing summary
    // free(curr);
  }
}

void printZone(zoneData *zone) {
  checkzone(zone);

  for(int row = 0; row < zone->zoneRows; row++) {
      for(int col = 0; col < zone->zoneCols; col++) {
        printf("%c ", zone->zoneArea[row][col]);
      }
      printf("\n");
  }
}



//////////////////////////////////////////////
// DbC routines
//////////////////////////////////////////////

void checkzone(zoneData *zone) {
  assert(zone != NULL);
}

void validProbeCell(zoneData *zone) {
 checkCell(zone->probe);

}

void checkCell(Cell cell) {
  assert(cell.row > 0);
  assert(cell.column > 0);
}