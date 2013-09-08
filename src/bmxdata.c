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


uint32_t swapByteOrder(uint32_t parValue) {
    uint32_t theResult = 0;
    uint8_t* tmp = (uint8_t*)(&parValue);

    theResult |= (uint32_t)(*tmp  << 24  & 0xff000000); tmp++;
    theResult |= (uint32_t)((*tmp << 16) & 0x00ff0000); tmp++;
    theResult |= (uint32_t)((*tmp <<  8) & 0x0000ff00); tmp++;
    theResult |= (uint32_t)((*tmp      ) & 0x000000ff);

    return theResult;
} 

const char *getString(bmxfile_t* parBmx, size_t parOffset, int parNum) {
    return (parBmx->strings)+parOffset;
}


const char *getNumber(bmxfile_t* parBmx, int parPos, int parNum) {
    static char data[500];
    int i = 0;
    int soff = 0;
    for(i = parPos; i < parPos+parNum; i++) {
        int theDecimals = 0;
        
        float tmpValue = parBmx->numbers[i];
        while(tmpValue-((int)tmpValue) && theDecimals < 10) {
            theDecimals++;
            tmpValue *= 10;
        }

        if(i == parPos) {
            soff += sprintf(data+soff,"%0.*f", theDecimals, parBmx->numbers[i]);
        }
        else {
            soff += sprintf(data+soff," %0.*f", theDecimals, parBmx->numbers[i]);
        }
    }
    return data;
}


const char *getBool(bmxfile_t* parBmx, int parPos, int parNum) {
    static const char *TRUE = "true";
    static const char *FALSE = "false";

    if(((*(parBmx->booleans))>>parPos)&1)
        return TRUE;
    return FALSE;
}


const block_t* findBlock(bmxfile_t* parBmx, uint32_t parName) {
    int i;
    int numBlocks = parBmx->firstBlock.num_headers;
    for(i = 0; i <= numBlocks; i++) {
        if(parName == parBmx->blocks[i].block)
                return &(parBmx->blocks[i]);
    }
    return NULL;
}


const char* getAttrValue(bmxfile_t* parBmx, int parPos) {
    attribute_t* theAttr = &(parBmx->attributes[parPos]);

    switch(theAttr->type)
    {
        case TYPE_STRING:
            return getString(parBmx, theAttr->value, theAttr->num_values);
        case TYPE_FLOAT:
            return getNumber(parBmx, theAttr->value, theAttr->num_values);
        case TYPE_BOOL:
            return getBool(parBmx, theAttr->value, theAttr->num_values);
    };
    printf("\n\nUnknown data type: %d\n\n", theAttr->type);
    return NULL;
}


void* readData(FILE* fp, size_t parStart, size_t parLength, int parByteOrder) {
    void* theData;
    size_t theOrgPos;

    theData = calloc(1, parLength);
    if(!theData)
        return NULL;

    theOrgPos = ftell(fp);
    fseek(fp, parStart, SEEK_SET);
    fread(theData, 1, parLength, fp);
    fseek(fp, theOrgPos, SEEK_SET);

    if(parByteOrder) {
        unsigned int i;
        uint32_t* theDataPtr = theData;

        for(i=0; i < (parLength)/sizeof(uint32_t); i++) {
            *theDataPtr = swapByteOrder(*theDataPtr);
            theDataPtr++;
        }
    }

    return theData;
}


void* readBlock(bmxfile_t* parBmx, FILE* parFp, size_t parOffset, uint32_t parBlock, int parByteOrder) {
    const block_t* theBlock = findBlock( parBmx, parBlock);
    if(theBlock)
        return readData( parFp, parOffset+theBlock->start, theBlock->length, parByteOrder);
    return NULL;
}


void printAttribute(bmxfile_t* parBmx, int parPos) {
    const char* theName = getBmtString(parBmx->attributes[parPos].name);
    const char* theValue = getAttrValue(parBmx, parPos);
    if(theName)
        printf(" %s=\"%s\"", theName, theValue);
    else
        printf(" [0x%08X]=\"%s\"", parBmx->attributes[parPos].name, theValue);
}


void printElement(bmxfile_t* parBmx, int parPos, int parLevel) {
    int i;
    const char* indentation="                              ";
    element_t* theElement = &(parBmx->elements[parPos]);
    const char* theElementName = getBmtString(theElement->name);
    
    if(theElementName)
        printf("%.*s<%s",parLevel*2, indentation, theElementName);
    else
        printf("%.*s<[0x%08X]",parLevel*2, indentation, theElement->name);
    for(i = 0; i < theElement->attr_num ; i++)
        printAttribute(parBmx, theElement->attr_start+i);

    if(parBmx->elements[parPos].child_num > 0) {
        printf(">\n");
        printElement(parBmx, theElement->child_first, parLevel+1);
        if(theElementName)
            printf("%.*s</%s>\n",parLevel*2, indentation, theElementName);
        else
            printf("%.*s</[0x%08X]>\n",parLevel*2, indentation, theElement->name);
    }
    else {
        printf(" />\n");
    }

    if(theElement->next_sibling != -1)
        printElement(parBmx, theElement->next_sibling, parLevel);
}


int readBmt(bmxfile_t** parBmx, FILE* parFp) {
    bmxfile_t* bmx;
    int identifier;
    size_t theOffset;
    int theByteOrder = 0;
    
    theOffset = ftell(parFp);
    
    fread(&identifier, sizeof(int), 1, parFp);
    
    if(identifier != BLMY) {
        if(swapByteOrder(identifier) != BLMY) {
            printf("Unknown file type %08X (%08X)\n", identifier, swapByteOrder(identifier));
            return 1;
        }
        theByteOrder = 1;
    }
    
    bmx = calloc(1, sizeof(bmxfile_t));

    fseek(parFp, theOffset, SEEK_SET);
    fread(&(bmx->firstBlock), sizeof(fileheader_t), 1, parFp);
    if(theByteOrder) {
        bmx->firstBlock.block       = swapByteOrder(bmx->firstBlock.block);
        bmx->firstBlock.num_headers = swapByteOrder(bmx->firstBlock.num_headers);
        bmx->firstBlock.file_size   = swapByteOrder(bmx->firstBlock.file_size);
    }
    bmx->blocks     = readData(parFp, ftell(parFp), sizeof(block_t)*bmx->firstBlock.num_headers, theByteOrder);
    bmx->header     = readBlock(bmx, parFp, theOffset, HEAD, theByteOrder);
    bmx->elements   = readBlock(bmx, parFp, theOffset, ELMT, theByteOrder);
    bmx->attributes = readBlock(bmx, parFp, theOffset, ATTR, theByteOrder);
    //TODO figure out what to do with COLL
    bmx->coll       = readBlock(bmx, parFp, theOffset, COLL, theByteOrder);
    bmx->numbers    = readBlock(bmx, parFp, theOffset, NUMB, theByteOrder);
    bmx->booleans   = readBlock(bmx, parFp, theOffset, BOOL, 0);
    bmx->strings    = readBlock(bmx, parFp, theOffset, STRS, 0);
    
    if(!(bmx->header && bmx->elements && bmx->attributes && bmx->coll && bmx->numbers && bmx->booleans && bmx->strings))
        return 1;
    
    
    *parBmx = bmx;
    return 0;
}


void freeBmt(bmxfile_t* parBmx) {
    if(parBmx->blocks)
        free(parBmx->blocks);
    
    if(parBmx->header)
        free(parBmx->header);
    
    if(parBmx->elements)
        free(parBmx->elements);
    
    if(parBmx->attributes)
        free(parBmx->attributes);

    if(parBmx->coll)
        free(parBmx->coll);
    
    if(parBmx->numbers)
        free(parBmx->numbers);
    
    if(parBmx->booleans)
        free(parBmx->booleans);
    
    if(parBmx->strings)
        free(parBmx->strings);
    free(parBmx);
}
