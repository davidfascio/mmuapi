/* 
 * File:   main.c
 * Author: fascio
 *
 * Created on 12 de julio de 2016, 05:27 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "MMUHandler.h"

/*
 * 
 */
int main(int argc, char** argv) {

    BYTE * mem[10];
    BYTE index;

    MMUHandler_Init();

    for (index = 0; index < 11; index++) {
        mem[index] = malloc(100);
        
        MMUHandler_SetHeapAddress(mem[0]);
    }
    
    MMUHandler_Free(mem[5]);
    MMUHandler_Free(mem[1]);
    MMUHandler_Free(mem[9]);
    MMUHandler_Free(mem[7]);
    MMUHandler_Free(mem[8]);
    MMUHandler_Free(mem[6]);
    MMUHandler_Free(mem[3]);
    MMUHandler_Free(mem[2]);
    MMUHandler_Free(mem[4]);
    MMUHandler_Free(mem[9]);
    
    while(TRUE){
        MMUHandler_Update();
    }
    return (EXIT_SUCCESS);
}

