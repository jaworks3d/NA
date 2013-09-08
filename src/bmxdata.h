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

#include <stdint.h>

typedef struct {
    uint32_t block;
    int32_t  num_headers;
    int32_t  file_size;
    int32_t  unknown;
} fileheader_t;

typedef struct {
    uint32_t block;
    int32_t length;
    int32_t start;
    int32_t unknown;
} block_t;

typedef struct {
    int32_t num_elements;
    int32_t num_attribues;
    int32_t num_COLL;
    int32_t num_numbers;
    int32_t num_strings;
    int32_t num_booleans;
    int32_t unknown;
} header_t;

typedef struct {
    int32_t name;
    int32_t type;
    int32_t value;
    int32_t num_values;
    int32_t next_same;
} attribute_t;

typedef struct {
    int32_t name;
    int32_t attr_start;
    int32_t attr_num;
    int32_t child_num;
    int32_t child_first;
    int32_t next_sibling;
    int32_t next_same_tag;
} element_t;

typedef struct {
    int32_t name;
    int32_t u1;
    int32_t u2;
    int32_t u3;
} coll_t;

typedef struct {
    fileheader_t    firstBlock;
    block_t*        blocks;
    header_t*       header;
    element_t*      elements;
    attribute_t*    attributes;
    coll_t*         coll;
    char*           strings;
    float*          numbers;
    int*            booleans;

} bmxfile_t;

void printElement(bmxfile_t* bmx, int pos, int level);
int readBmt(bmxfile_t** parBmx, FILE* parFp);
void freeBmt(bmxfile_t* parBmx);
const block_t* findBlock(bmxfile_t* parBmx, uint32_t parName);

