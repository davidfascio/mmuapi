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
    } else {
        
       print_log("[Heap Address already exists into MMU Handler] Index: %d\tHeapAdrress %08X", error_code, address); 
    }
}

void MMUHandler_RemoveHeapAddress(void){
    
    BYTE counter = MMUHandler_GetCounter();
    
    if (counter >  MMU_HANDLER_QUEUE_MAX_SIZE)
        return;
    
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

INT16 MMUHandler_Free(BYTE * address){
    
    BYTE index;
    BYTE * queueAddress;
    
    for (index = 0; index < MMU_HANDLER_QUEUE_MAX_SIZE; index++){
        
        queueAddress = MMUHandler_GetQueueItemByIndex(index);
        
        if(queueAddress == NULL){
            MMUHandler_SetQueueItemByIndex(address, index);
            break;        
        }
    }
    
    if(index == MMU_HANDLER_QUEUE_MAX_SIZE){
        print_error("No more free space into mmu handler queue");
        return MMU_HANDLER_QUEUE_MAX_SIZE_ERROR_CODE;
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