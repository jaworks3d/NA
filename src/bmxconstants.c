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

#include <stddef.h>
#include "bmxconstants.h"

static struct idValuePair bmtStrings[] =  {
    { 0x00000065, "e" },
    { 0x00000066, "f" },
    { 0x00000074, "t" },
    { 0x00000076, "v" },
    { 0x061B2AC5, "name" },
    { 0x64A09665, "type" },
    { 0x1F0F4EA6, "behaviour" },
    { 0xA7AB72D0, "technique" },
    { 0x0001BE02, "sb" },
    { 0x000183E2, "db" },
    { 0x00017C2F, "bo" },
    { 0xB260F2BF, "shader" },
    { 0x873CF6EC, "cull" },
    { 0x25A11D8A, "numparams" },
    { 0x00000077, "w" },
    { 0x05FD4687, "fog" },
    { 0x69753170, "antialias" },
    { 0xD345702C, "deferredID" },
    { 0x4BE14205, "base" },
    { 0xC04045E1, "data" },
    { 0x32DFF53A, "elements" },
    { 0xE6F9DDA1, "enabled"},
    { 0xED618B4F, "override"},
    { 0x26ACEF06, "shaderparam"},
    { 0x69B6F8EB, "value"},
    { 0xCA6C420B, "depthparams"},
    { 0xD65A4663, "material"},
    { 0x00E38703, "alphablendparams"},
    { 0x7D3E56F9, "writeenabled" },
    { 0xED653EEC, "sourceblend" },
    { 0xC357F2C1, "destblend" },
    { 0x1CFD9605, "blendop" },
    { 0xCB729509, "define" },
    { 0x2539DE0E, "alphatestparams" },
    { 0x15E108AF, "function" },
    { 0xA8F7F71D, "Reflection" },
    { 0xFFEC4403, "class" },
    { 0x7B797690, "prop" },
    { 0xF4A31093, "funcpropdata" },
    { 0x00019744, "id" },
    { 0x03EF0832, "CAR" },
    { 0xBF1C43C5, "NODE" },
    { 0xA0067FEC, "CONTROL" },
    { 0xCA1BD0E5, "Distances" },
    { 0x00767601, "SPHERE" },
    { 0x9F8C4C99, "Centre" },
    { 0x1AD24663, "Radius" },
    { 0xB1A8A13D, "RESOURCE" },
    { 0x4B1714AD, "Filename" },
    { 0x9B40709B, "userflags" },
    { 0xC02B2AC5, "Name" },
    { 0xE2AF7060, "MatrixNumber" },
    { 0xD9AA9A0E, "instances" },
    { 0x3E7DE89C, "MATRIX" },
    { 0x2E1BE0AC, "Offset" },
    { 0xF256FC36, "subobjects" },
    { 0x8D2C4AE1, "parent" },
    { 0xEB0874B9, "Orientation" },
    { 0xE32535EB, "matrices" },
    { 0xF049087D, "ExporterVersion" }
};

const char* getBmtString(int parId) {
    unsigned int i;
    for(i = 0; i < count(bmtStrings); i++) {
        if(bmtStrings[i].id == parId)
            return bmtStrings[i].value;
    }
    return NULL;
}

