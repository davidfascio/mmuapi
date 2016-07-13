/* 
 * File:   MMUHandler.h
 * Author: fascio
 *
 * Created on 12 de julio de 2016, 05:30 PM
 */

#ifndef __MMU_HANDLER_H__
#define	__MMU_HANDLER_H__

#include "Utility.h"
#include "SystemLog.h"


#define MMU_HANDLER_QUEUE_MAX_SIZE                                      (10)

#define MMU_HANDLER_NO_ERROR_CODE                                       (0)
#define MMU_HANDLER_NULL_PTR_ERROR_CODE                                 (-1)
#define MMU_HANDLER_QUEUE_MAX_SIZE_ERROR_CODE                           (-2)
#define MMU_HANDLER_WAITING_FOR_FREE_MEM_ERROR_CODE                     (-3)
#define MMU_HANDLER_ITEM_NOT_FOUND_ERROR_CODE                           (-4)

typedef struct{
    BYTE * heapAddress[MMU_HANDLER_QUEUE_MAX_SIZE];
    BYTE * queue[MMU_HANDLER_QUEUE_MAX_SIZE];
    BYTE counter;
}MMU_HANDLER, * MMU_HANDLER_PTR;

INT16 MMUHandler_FindHeapAddressItem(BYTE * address);
void MMUHandler_SetHeapAddress(BYTE * address);
void MMUHandler_RemoveHeapAddress(void);
BYTE * MMUHandler_GetHeapAddress(void);
void MMUHandler_PrintHeapAddress(void);

INT16 MMUHandler_FindAddressItemIntoQueue(BYTE * address);
void MMUHandler_SetQueueItemByIndex(BYTE * address, BYTE index);
BYTE * MMUHandler_GetQueueItemByIndex(BYTE index);

void MMUHandler_SetCounter(BYTE counter);
BYTE MMUHandler_GetCounter(void);
void MMUHandler_PrintFreeAddressQueue(void);

void MMUHandler_Init(void);
INT16 MMUHandler_Free(BYTE * address);
INT16 MMUHandler_Update(void);

#endif	/* __MMU_HANDLER_H__ */

