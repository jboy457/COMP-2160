#include "ObjectManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>


static ulong bytesCollected = 0;
static ulong bytesInUse = 0;


typedef struct Node Node;


typedef struct ObjectManager
{
    uchar* buffer; //memory pool
    ulong offset; //offset. used to keep track of the start size of each object
    Ref nextRef; //Next reference id for objects
    ulong noOfObjects; //no of allocated objects in the memory pool
    Node* head; //top of linked list
}ObjectManager;


//buffer variables used to implement double buffering
uchar buffer1[MEMORY_SIZE];
uchar buffer2[MEMORY_SIZE];


//Global object manager variable
ObjectManager* om;


// typedef enum BOOl bool;
// enum BOOl{
//     false,
//     true
// };


struct Node{
    Ref refID; //reference ID for the object
    ulong size; // size of the allocated memory block
    ulong refCount; //reference count
    ulong start; //starting address of the object in the memory pool in bytes
    Node* next; //pointer to next node in list
};


//function prototypes
void initPool();
void switchBuffer(uchar* used, uchar* unused);
void destroyPool();
Ref insertObject(ulong size);
Ref initializeNewObject(ulong size);
void *retrieveObject( Ref ref );
void addReference( Ref ref );
void dropReference( Ref ref );
void compact();
void dumpPool();
static void add(Node* object);
Node* searchLL(Ref ref);
void removeeee(Ref ref);
void printGCData();


/*---------------------------------------DBC IMPLEMENTATIONS----------------------------------------------------------------*/
void validObjectManager(){
    assert(om != NULL);
    assert(om->buffer != NULL);
    assert(om->offset >= 0);
    assert(om->nextRef > 0);
    assert(om->noOfObjects >=0);
}


void validNode(Node* newObject){

    assert(newObject != NULL);
    assert(newObject->refID >= 0);
    assert(newObject->start >= 0);

}




/*-----------------------------------FUNCTION IMPLEMENTATIONS FOR OBJECT MANAGER---------------------------------------*/

// initialize the object manager
void initPool(){
    // assert(om == NULL);

    om = (ObjectManager*)malloc(sizeof(ObjectManager));
    assert(om != NULL);

    if (om != NULL)
    {
        om->buffer = buffer1;
        om->nextRef = 1;
        om->head = NULL;
        om->offset = 0;
        om->noOfObjects = 0;
    }
    

    validObjectManager();

    
}




// cleans up the object manager (before exiting)
void destroyPool(){
    validObjectManager();

    Node* curr = om->head;

    while (curr != NULL)
    {   
        Node* temp = curr;
        
        curr = curr->next;

        free(temp);

        
    }

    free(om);
    memset(buffer1, 0, MEMORY_SIZE);
    memset(buffer2, 0, MEMORY_SIZE);

}




/* This function trys to allocate a block of given size from our buffer.
 * It will fire the garbage collector as required.
 * We always assume that an insert always creates a new object...
 * On success it returns the reference number for the block of memory
 * allocated for the object.
 * On failure it returns NULL_REF (0)
 */
Ref insertObject(ulong size){

    validObjectManager();
    assert(size >= 0);

    Ref ret = NULL_REF;


    if (om->offset + size <= MEMORY_SIZE)
    {

        ret = initializeNewObject(size);
        
    }else{ //not enough memory
       
        compact();
        if (om->offset + size <= MEMORY_SIZE)
        {
            ret = initializeNewObject(size);
            // printGCData();
        }else{
            printf("Unable to successfully complete memory allocation request.\n");
        }

    }


    validObjectManager();

    return ret;
    
}



//initializes the new object to be inserted and returns address to the object
Ref initializeNewObject(ulong size){
    Node* newNode = (Node*)malloc(sizeof(Node));

    assert(newNode != NULL);

    if (newNode != NULL)
    {
        newNode->refID = om->nextRef;
        newNode->size = size;
        newNode->refCount = 1;
        newNode->start = om->offset;
    
        add(newNode);
    }
    
    validNode(newNode);


    om->buffer[om->offset] = (uchar)size;
    om->offset+=size; //increments start address for the next object to be stored in memory
    Ref refRetID = newNode->refID; //stores current objects reference ID
    om->nextRef++;
    om->noOfObjects++;
    
    return refRetID;
}




// returns a pointer to the object being requested given by the reference id
void *retrieveObject( Ref ref ){
    assert(ref >= 0);
    Node* node = searchLL(ref); //Node with reference id ref
    void* ret = NULL;

    if (node != NULL)
    {
        ret = &(om->buffer[node->start]);
        
    }else{
        if (om->head == NULL)
        {
            printf("Invalid reference exception with reference %ld, terminating process\n", ref);
            // ret = NULL;
            exit(0);
        }
        else
        {
            ret = NULL;

        }
        
        
    }

    return ret;
    
}



// update our index to indicate that we have another reference to the given object
void addReference( Ref ref ){
    assert(ref >= 0);
    Node* node = searchLL(ref); //Node with reference id ref
    if (node != NULL)
    {
        node->refCount+=1;
        om->noOfObjects+=1;
        
        validNode(node);

    }
    
    

}



// update our index to indicate that a reference is gone
void dropReference( Ref ref ){
    assert(ref >= 0);
    Node* node = searchLL(ref); //Node with reference id ref

    if (node != NULL)
    {
        node->refCount = node->refCount - 1;
        om->noOfObjects = om->noOfObjects - 1;   

        if (node->refCount == 0)
        {
            removeeee(ref);
        }

        validNode(node);

    }
    
    
}



//implements garbage collection
void compact() {
    ulong prevBuffSize;
    // Switch to the alternate buffer
    if (om->buffer == buffer1) {
        prevBuffSize = om->offset;
        om->buffer = buffer2;
        memset(buffer1, 0, MEMORY_SIZE);

    } else {
        prevBuffSize = om->offset;
        om->buffer = buffer1;
        memset(buffer2, 0, MEMORY_SIZE);

    }

    om->offset = 0;  // Reset the free pointer
    bytesInUse = 0;
    bytesCollected= 0;

    Node* temp = om->head;

    while (temp != NULL) {
        om->buffer[om->offset] = (uchar)temp->size;


        // Advance the free pointer
        om->offset += temp->size;
        bytesInUse += temp->size;

        // Move to the next node
        temp = temp->next;
    }

    // Update om->nextRef based on the last valid Node in the list
    if (om->head != NULL) {
        Node* lastNode = om->head;
        while (lastNode->next != NULL) {
            lastNode = lastNode->next;
        }
        om->nextRef = lastNode->refID + 1;  // Increment last node's refID for the next reference
    }

    bytesCollected = prevBuffSize - om->offset;

    printGCData();
}



//prints the garbage collector statistics
void printGCData(){
    printf("\nGarbage collector statistics:\nobjects: %ld\nbytes in use: %ld\nfreed: %ld\n\n", om->noOfObjects, bytesInUse, bytesCollected);

}



/*
 * This function traverses the index and prints the info in each entry
 * corresponding to a block of allocated memory. You should print the
 * block's reference id, its starting address, and its size (in
 * bytes).
 */
void dumpPool(){
    validObjectManager();
    Node* temp = om->head;

    while (temp != NULL)
    {
        validNode(temp);
        printf("RefID: %lu \n StartAddress: %lu \n Size: %lu\n\n", temp->refID, temp->start, temp->size);
        temp = temp->next;
    }
    
    validObjectManager();

}








/*-----------------------------------FUNCTION IMPLEMENTATIONS FOR THE LINKED LIST------------------------------------*/

//adds to the end of the linked list
void add(Node* object){
    validNode(object);

    Node* prev = NULL;
    Node* curr = om->head;

    // traverse till end of list
    while (curr != NULL) {
        validNode(curr);
        prev = curr;
        curr = curr->next;
    }

    if (prev == NULL)
    {
        object->next = om->head;
        om->head = object;
    }else{
        object->next = curr;
        prev->next = object;
    }

    
    assert(object->next == NULL);

    
}



//searches for a specific object with refID ref in the list 
Node* searchLL(Ref ref){ //searches linked list based on refID and returns the node of desired object in the list
    assert(ref >= 0);
    Node* temp = om->head;
    bool found = false;

    while (temp != NULL && !found)
    {
        validNode(temp);
        if (temp->refID == ref)
        {
            found = true;
        }else{
            temp = temp->next;
        }
        
    }



    return temp;
    
    
}



//removeeees an object with refID ref from the list
void removeeee(Ref ref){
    assert(ref > 0);

    Node* prev = NULL;
    Node* temp = om->head;

    bool found = false;

    while (temp != NULL && !found)
    {
        validNode(temp);

        if (temp->refID == ref)
        {
            found = true;
        }else
        {
            prev = temp;
            temp = temp->next;
        }
        
        
    }

    if (temp != NULL) 
    {
        if (prev == NULL) //beginning of list
        {
            om->head = om->head->next;
        }else{
            prev->next = temp->next;
        }
        
    }

    
    
}




