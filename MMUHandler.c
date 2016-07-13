#include "MMUHandler.h"

MMU_HANDLER mmuHandlerControl;

INT16 MMUHandler_FindHeapAddressItem(BYTE * address){

    BYTE index;
    
    for(index = 0; index < MMU_HANDLER_QUEUE_MAX_SIZE; index++ ){
        
        if(mmuHandlerControl.heapAddress[index] == address)
            return index;
    }
    
    return MMU_HANDLER_ITEM_NOT_FOUND_ERROR_CODE;
}

void MMUHandler_SetHeapAddress(BYTE * address){
    
    BYTE counter = MMUHandler_GetCounter();
    INT16 error_code;
    
    if (counter >=  MMU_HANDLER_QUEUE_MAX_SIZE){
     
        print_error("Heap Address Counter Overflow");
        return;
    }
    
    error_code = MMUHandler_FindHeapAddressItem(address);
    if( error_code == MMU_HANDLER_ITEM_NOT_FOUND_ERROR_CODE ){
        
        mmuHandlerControl.heapAddress[counter++] = address;
        MMUHandler_SetCounter(counter);
        print_info("[Adding Address into MMU Handler] Counter: %d\tHeapAdrress %08X", counter, address);
        MMUHandler_PrintHeapAddress();
    } else {
        
       print_log("[Heap Address already exists into MMU Handler] Index: %d\tHeapAdrress %08X", error_code, address); 
    }
}

void MMUHandler_PrintHeapAddress(void){
    
    BYTE index;
    
    println("[HEAP ADDRESS STACK] ");
    
    for(index = 0; index < MMU_HANDLER_QUEUE_MAX_SIZE; index++){
        
        print("0x%08X ", mmuHandlerControl.heapAddress[index]);
    }
    //println("");
}

void MMUHandler_RemoveHeapAddress(void){
    
    BYTE counter = MMUHandler_GetCounter();
    BYTE * heapAddress = MMUHandler_GetHeapAddress();
    
    if (counter >  MMU_HANDLER_QUEUE_MAX_SIZE)
        return;
    
    print_info("[Remove Address from MMU Handler] Counter: %d\tHeapAdrress %08X", counter, heapAddress);
    MMUHandler_PrintHeapAddress();
    MMUHandler_PrintFreeAddressQueue();
    mmuHandlerControl.heapAddress[--counter] = NULL;
    MMUHandler_SetCounter(counter);
}

BYTE * MMUHandler_GetHeapAddress(void){
    
    BYTE counter = MMUHandler_GetCounter();
    
    if( counter == 0)
        return NULL;
    
    return mmuHandlerControl.heapAddress[--counter];
}

void MMUHandler_SetQueueItemByIndex(BYTE * address, BYTE index) {

    if (index >= MMU_HANDLER_QUEUE_MAX_SIZE)
        return;

    mmuHandlerControl.queue[index] = address;
}

BYTE * MMUHandler_GetQueueItemByIndex(BYTE index) {

    if (index >= MMU_HANDLER_QUEUE_MAX_SIZE)
        return NULL;

    return mmuHandlerControl.queue[index];
}

void MMUHandler_SetCounter(BYTE counter){
    
    mmuHandlerControl.counter = counter;
}

BYTE MMUHandler_GetCounter(void){
    
    return mmuHandlerControl.counter;
}

void MMUHandler_Init(void){

    BYTE index;
    
    for (index = 0; index < MMU_HANDLER_QUEUE_MAX_SIZE; index++){
        MMUHandler_SetQueueItemByIndex(NULL, index);
    }
    
    MMUHandler_SetCounter(0);
}

INT16 MMUHandler_FindAddressItemIntoQueue(BYTE * address){

    BYTE index;
    BYTE * queueAddress;
    
    for(index = 0; index < MMU_HANDLER_QUEUE_MAX_SIZE; index++ ){
        
        queueAddress = MMUHandler_GetQueueItemByIndex(index);
        
        if(queueAddress == address)
            return index;
    }
    
    return MMU_HANDLER_ITEM_NOT_FOUND_ERROR_CODE;
}

void MMUHandler_PrintFreeAddressQueue(void){
    
    BYTE index;
    
    println("[FREE ADDRESS QUEUE] ");
    
    for(index = 0; index < MMU_HANDLER_QUEUE_MAX_SIZE; index++){
        
        print("0x%08X ", mmuHandlerControl.queue[index]);
    }
    //println("");
}

INT16 MMUHandler_Free(BYTE * address){
    
    BYTE index;
    BYTE * queueAddress;
    INT16 error_code;
    
    error_code = MMUHandler_FindHeapAddressItem(address);
    
    if( error_code == MMU_HANDLER_ITEM_NOT_FOUND_ERROR_CODE ){
        print_error("[Free Address does not exist into MMU Handler] FreeAdrress %08X", address);
        return error_code;
    }
    
    error_code = MMUHandler_FindAddressItemIntoQueue(address);
    
    if( error_code == MMU_HANDLER_ITEM_NOT_FOUND_ERROR_CODE ){

        for (index = 0; index < MMU_HANDLER_QUEUE_MAX_SIZE; index++){

            queueAddress = MMUHandler_GetQueueItemByIndex(index);

            if(queueAddress == NULL){
                MMUHandler_SetQueueItemByIndex(address, index);
                print_info("[Adding Free Address into MMU Handler] Index: %d\tFreeAdrress %08X", index, address);
                MMUHandler_PrintFreeAddressQueue();
                break;        
            }
        }

        if(index == MMU_HANDLER_QUEUE_MAX_SIZE){
            print_error("No more free space into mmu handler queue");
            return MMU_HANDLER_QUEUE_MAX_SIZE_ERROR_CODE;
        }
        
    } else {
        
       print_log("[Free Address already exists into MMU Handler] Index: %d\tFreeAdrress %08X", error_code, address); 
    }

    return MMU_HANDLER_NO_ERROR_CODE;
}

INT16 MMUHandler_Update(void){
    
    BYTE index;
    BYTE * queueAddress;
    BYTE * heapAddress;
    
    heapAddress = MMUHandler_GetHeapAddress();
    
    if(heapAddress == NULL)
        return MMU_HANDLER_NULL_PTR_ERROR_CODE;
    
    for (index = 0; index < MMU_HANDLER_QUEUE_MAX_SIZE; index++){
        
        queueAddress = MMUHandler_GetQueueItemByIndex(index);
        
        if(queueAddress == heapAddress){
            
            free(queueAddress);
            MMUHandler_SetQueueItemByIndex(NULL, index);
            MMUHandler_RemoveHeapAddress();
            break;
        }
    }    
    
    if(index == MMU_HANDLER_QUEUE_MAX_SIZE)
        return MMU_HANDLER_WAITING_FOR_FREE_MEM_ERROR_CODE;
            
    return MMU_HANDLER_NO_ERROR_CODE;
}