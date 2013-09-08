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

#ifndef BMXCONSTANTS_H
#define BMXCONSTANTS_H

#include <stdint.h>

#define count(A) (sizeof(A)/sizeof(*A))

#define BLMY 0x594D4C42
#define HEAD 0x44414548
#define ELMT 0x544d4c45
#define ATTR 0x52545441
#define COLL 0x4c4c4f43
#define NUMB 0x424d554e
#define BOOL 0x4c4f4f42
#define STRS 0x53525453

enum BMX_TYPES {
    TYPE_FLOAT  = 0,
    TYPE_BOOL   = 1,
    TYPE_STRING = 2
};

struct idValuePair {
    int32_t id;
    const char* value;
};

const char* getBmtString(int parElementId);

#endif

