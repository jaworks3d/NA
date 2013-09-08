/*
* bmt to xml conversion
*
* Copyright 2010 Jan-Peter Nilsson
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmxdata.h"
#include "bmxconstants.h"


int main(int argc, char **argv) {
    bmxfile_t* bmx;
    int printBlocks = 0;
    int printHeader = 0;
    int printColl   = 0;
    int printXml    = 1;
    int rval;
    FILE* fp;

    if(argc != 2) {
        printf("Usage: bmt2xml <file.bmt>\n");
        return 0;
    }

    char *file = argv[1];
    fp = fopen(file, "rb");
    if(!fp) {
        printf("Error: Failed to open file: %s!\n", file);
        return 1;
    }
    
    rval = readBmt(&bmx, fp);

    fclose(fp);
    if(rval != 0) {
        printf("Error: Failed to read data\n");
        return 1;
    }
   
    if(printBlocks) {
        int i;
        printf("BLOCKS:\n");
        for(i = 0; i <= bmx->firstBlock.num_headers; i++) {
            printf("\t%.*s\t%d\t%02d\t%d\n",
                    4, (char*)&(bmx->blocks[i].block),
                    bmx->blocks[i].start,
                    bmx->blocks[i].length,
                    bmx->blocks[i].unknown);
        }
    }
  
    if(printHeader) {
        unsigned int i;
        printf("HEADER:\n");
        for(i = 0; i < (sizeof(header_t)/sizeof(int))-1; i++) {
            printf("\tNum %d (%.*s?):   %02d\n",i, 4, (char*)&(bmx->blocks[i+1].block), *(((int*)((bmx->header)))+i));
        }
        printf("\tNum 6 (?\??\?\?):   %02d\n", bmx->header->unknown); //NumElements -1

        printf("\n\n");
        printf("Elements:    %lu\n", (findBlock(bmx, ELMT)->length)/sizeof(element_t));
        printf("Attriubutes: %lu\n", (findBlock(bmx, ATTR)->length)/sizeof(attribute_t));
        printf("COLL:        %lu\n", (findBlock(bmx, COLL)->length)/sizeof(coll_t));
        printf("Numbers:     %lu\n", (findBlock(bmx, NUMB)->length)/sizeof(float));
        printf("Booleans:    %d\n", (findBlock(bmx, BOOL)->length)*8);
        printf("Strings:     %d\n", (findBlock(bmx, STRS)->length));
        printf("\n");
    }

    if(printColl) {
        printf("COLL\n");
        unsigned int i = 0;
        for(i = 0; i < (findBlock(bmx, COLL)->length)/sizeof(coll_t); i++) {
            const char* name;
            name = getBmtString(bmx->coll[i].name);
            if(bmx->coll[i].u3 != -1) {
                printf("u3 %d!= -1!!!!\n", bmx->coll[i].u3);
                return 0;
            }
            if(name) {
                printf("\t%s\t%d\t%d\n", name, bmx->coll[i].u1, bmx->coll[i].u2);
            }
            else {
                printf("\t[0x%08X]\t%d\t%d\n", bmx->coll[i].name, bmx->coll[i].u1, bmx->coll[i].u2);
            }
        }
    }
    
    
    if(printXml) {
        int theElement = bmx->header->num_elements-1;
        printElement(bmx, theElement, 0);
    }
    freeBmt(bmx);
    return 0;
}

