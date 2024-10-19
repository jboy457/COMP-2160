
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
  // printProbeSummary();
  printf("End of processing");
  return EXIT_SUCCESS;
}

//////////////////////////////////////////////
// Cell routines
//////////////////////////////////////////////


Cell nextCell() {
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

zoneData *makeZone() {
  zoneData* newZone = (zoneData*)malloc(sizeof(zoneData));

  // Check if memory was allocated to newZone.
  checkzone(newZone);

  return newZone;
}

Boolean loadZone() {
  // Check if file was passed into stdin
  char line[BUFFERSIZE];
  char zoneName[BUFFERSIZE];

  // Allocate memoery to probe 
  probe = (probeData*)malloc(sizeof(probeData));
  zoneData zoneToLoad = *makeZone();

  checkzone(&zoneToLoad);

  // Read Zone Name and ID
  if(fgets(line, BUFFERSIZE, stdin) != NULL) {
    if(sscanf(line, "%s %d", zoneName, &zoneToLoad.zoneID) != 2) {
      printf("Error zone format inputed.\n");
      return false;
    };
  }

  // Read Rows and Column
  if(fgets(line, BUFFERSIZE, stdin) != NULL) {
    if(sscanf(line, "%d %d", &zoneToLoad.zoneRows, &zoneToLoad.zoneCols) != 2) {
      printf("Error zone format inputed.\n");
      return false;
    };
  }
  
  // Read Zone Grid
  int tempRow = 0;
  while (tempRow < zoneToLoad.zoneRows) {
    if(fgets(line, BUFFERSIZE, stdin) != NULL) {
      int tempCol = 0;
      for(int i = 0; line[i] != '\0'; i++) {
        // Check if the char is no a space or not equal to new line
        if(isspace(line[i]) == 0 && line[i] != '\n') {
          // Assign char to cell.
          zoneToLoad.zoneArea[tempRow][tempCol] = line[i];

          // Assign Probe Point.
          if(line[i] == PROBE) {
            Cell newCell;
            newCell.row = tempRow;
            newCell.column = tempCol;
            zoneToLoad.probe = newCell;
          }
          tempCol++;
        } 
      }
      tempRow++;
    }
  }


  // Check if zone has a place to land.
  if(zoneToLoad.probe.row < 1) {
    printf("\nZone %d has no landing cell\n", zoneToLoad.zoneID);
  } else {
    printf("Zone %d Map:\n\n", zoneToLoad.zoneID);

    // Check Zone 
    printZone(&zoneToLoad);
    exploreZone(&zoneToLoad);

    printZoneSummary(&zoneToLoad);
    printf("\nProbed Mapping: \n");
    printZone(&zoneToLoad);
    insertInOrder()
  }


  if(feof(stdin)) {
    return false;
  } else {
    return true;
  }  
}


void addToProbe(zoneData *zone, Cell currentCell) {
  checkzone(zone);
  checkCell(currentCell);

  // Current cell has to be in zone.
  assert(currentCell.row < zone->zoneRows);
  assert(currentCell.column < zone->zoneCols);

  //For vertical check 
  if(currentCell.row > 0 && 
      currentCell.row < zone->zoneRows && 
      currentCell.column > 0 && 
      currentCell.column < zone->zoneCols
    ) {

      // Check UP
      if (zone->zoneArea[currentCell.row - 1][currentCell.column] != WALL && 
          zone->zoneArea[currentCell.row - 1][currentCell.column] != VISITED &&
          currentCell.row - 1 > -1
        ) {
          Cell newCell = makeCell(currentCell.row - 1, currentCell.column);
          checkCell(newCell);
          
          pushProbe(newCell);
      } 
      
      // Check DOWN
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


void exploreZone(zoneData *zone) {
  // Pre condidtion check .
  checkzone(zone);
  // Also need to check if zone->probe is not empty i.e no landing space.
  validProbeCell(zone);

  pushProbe(zone->probe);

  // Check if top is not empty
  assert(top != NULL);

  Cell* currentCell = &zone->probe;
  while(currentCell->row != -1) {
    // Compute resources for zone and visit it.
    computeResource(zone, *currentCell);
    zone->zoneArea[currentCell->row][currentCell->column] = VISITED;
    
    // Check is cell has been truely marked as visited.
    assert(zone->zoneArea[currentCell->row][currentCell->column] == VISITED);

    // Remove item probbed Cell for list
    popProbe();
    
    // search and add to probe.
    addToProbe(zone, *currentCell);

    Cell temp = nextCell();
    currentCell = &temp;

    assert(currentCell->row == temp.row);
    assert(currentCell->column == temp.column);
  }
}

void computeResource(zoneData *zone, Cell rcell) {
  checkzone(zone);
  checkCell(rcell);

  char cell = zone->zoneArea[rcell.row][rcell.column];
  if(cell != WALL) {
      zone->avaialble++;
    if(cell == OPEN_SPACE) {
      zone->richArea++;
    } else if(cell == UNREADABLE) {
      zone->unreadable++;
    } else {
      zone->accessible++;
    }
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

void printZoneSummary(zoneData *zone) {
  checkzone(zone);

  printf("\nSummary Zone %d: \n", zone->zoneID);
  printf("-> Avaialble areas: %d \n", zone->avaialble);
  printf("-> Resource rich areas: %d \n", zone->richArea);
  printf("-> Resource accessible: %d \n", zone->accessible);
  printf("-> Resource unreadable: %d \n", zone->unreadable);
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