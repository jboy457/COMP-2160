
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include<ctype.h>
//-------------------------------------------------------------------------------------
// CONSTANTS and TYPES
//-------------------------------------------------------------------------------------

#define MAX_DIMENSION 20

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
  // Other Zone Metadata information
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

CellNode top;
probeData probe;

//-------------------------------------------------------------------------------------
// PROTOTYPES
//-------------------------------------------------------------------------------------

// basic cell manipulation

// returns true if the cells are at the same position in our zone
Boolean equalCells(const Cell cell1, const Cell cell2);
// returns a new cell object
Cell makeCell(const int row, const int col);
// returns true if the cell is within our zone
Boolean validCell(const Cell theCell);

// routines for managing our backtracking

// returns true if there are no more cells to try
Boolean noMoreCells();
// returns the next cell to try for exploring the zone
Cell nextCell();
// introduces a new cell to try
void addCell(const Cell cell);

//////////////////////////////////////////////
// routines for managing the zone and probe
//////////////////////////////////////////////
// print the zone
void printZoneSummary(zoneData zone);
// Read the zone information from STDIN into the probe
Boolean loadZone(probeData *probe);
// explores the zone and returns true if the probe explores it successfully
Boolean exploreZone();
// Estimates the unreadable resources in the zone
void estimateResources(zoneData *zone);
// Insert the zone into the probe data
void insertInOrder(probeData *probe, zoneData *zone);
// Print the summary of the probe findings
void printProbeSummary(probeData *probe);
//-------------------------------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
  int numberOfZones = 0;
  while (loadZone(&probe))
  {
    numberOfZones++;
    printf("%d Zone(s) loaded.\n", numberOfZones);
  }
  // printSummary(&probe);

  printf("End of processing");
  return EXIT_SUCCESS;
}

//////////////////////////////////////////////
// Cell routines
//////////////////////////////////////////////

// Boolean equalCells(const Cell cell1, const Cell cell2) {
//   Boolean equal = false;
//   if((cell1.row == cell2.row) && (cell2.col == cell2.col)) {
//     equal = true;
//   }
//   return equal;
// }

// Cell makeCell(const int row, const int col) {
//   Cell newCell;
//   newCell.row = row;
//   newCell.col = col;
//   return newCell;
// }

// Boolean validCell (const Cell theCell) {
  
// }

//////////////////////////////////////////////
// List routines
//////////////////////////////////////////////

//////////////////////////////////////////////
// Zone routines
//////////////////////////////////////////////
Boolean loadZone(probeData *probe) {
  char line[MAX_DIMENSION];
  zoneData newZone;
  char zoneName[10];

  int lineId = 1;
  int row = 0;
  while(fgets(line, MAX_DIMENSION, stdin) != NULL) {
    // lastCharIndex = strlen(line) - 1;
    // if(line[lastCharIndex] == '\n') {
    //   line[lastCharIndex] = '\0'
    // }

    // Manange the error there.
    if(lineId == 1) {
      sscanf(line, "%s %d", zoneName, &newZone.zoneID);
    } else if(lineId == 2) {
      sscanf(line, "%d %d", &newZone.zoneRows, &newZone.zoneCols);
    } else {
      // Read the zone data line by line

      // Check for array bounds.
      int column = 0;
      for(int i = 0; line[i] != '\0'; i++) {
        if(isspace(line[i]) == 0 && line[i] != '\n') {
          Cell newCell;
          newZone.zoneArea[row][column] = line[i];
          if(line[i] == PROBE) {
            newCell.row = line[row];
            newCell.col = line[col];
            newZone.probe = newCell;
          }
          column++;
        } 
      }
      row++;
    }
    lineId++;
  }

  estimateResources(&newZone);
  printZoneSummary(newZone);

  probe->probedZone = &newZone;
  return false;
} 

void estimateResources(zoneData *zone) {
  zone->richArea = 0;
  zone->unreadable = 0;
  zone->accessible = 0;
  zone->avaialble = 0;

  for(int row = 0; row < zone->zoneRows; row++) {
    for(int col = 0; col < zone->zoneCols; col++) {
      char cell = zone->zoneArea[row][col];
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
  }
}

void printZoneSummary(zoneData zone) {
  printf("Zone %d Map:\n\n", zone.zoneID);

  for(int row = 0; row < zone.zoneRows; row++) {
      for(int col = 0; col < zone.zoneCols; col++) {
        printf("%c ", zone.zoneArea[row][col]);
      }
      printf("\n");
  }

  printf("\nSummary Zone %d: \n", zone.zoneID);
  printf("-> Avaialble areas: %d \n", zone.avaialble);
  printf("-> Resource rich areas: %d \n", zone.richArea);
  printf("-> Resource accessible: %d \n", zone.accessible);
  printf("-> Resource unreadable: %d \n", zone.unreadable);
}

// Insert
void insertInOrder(probeData * probe, zoneData *zone) {
  
}

//////////////////////////////////////////////
// Probe routines
//////////////////////////////////////////////
