/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Library General Public
* License as published by the Free Software Foundation; either
* version 2 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU Library General Public License
* along with this library; see the file COPYING.LIB.  If not, write to
* the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
* Boston, MA 02110-1301, USA.
**/

#include "colors.h"

/* QtCore */
#include <QtCore/QDebug>

/**
* Sichere Webfarben
*/
const QVector<QColor> Colors::webColors()
{
  int size = 216;
  QVector<QColor> map;
  int array[216][3] =
  {
    {255,255,255},
    {255,255,204},
    {255,255,153},
    {255,255,102},
    {255,255,51},
    {255,255,0},
    {255,204,255},
    {255,204,204},
    {255,204,153},
    {255,204,102},
    {255,204,51},
    {255,204,0},
    {255,153,255},
    {255,153,204},
    {255,153,153},
    {255,153,102},
    {255,153,51},
    {255,153,0},
    {255,102,255},
    {255,102,204},
    {255,102,153},
    {255,102,102},
    {255,102,51},
    {255,102,0},
    {255,51,255},
    {255,51,204},
    {255,51,153},
    {255,51,102},
    {255,51,51},
    {255,51,0},
    {255,0,255},
    {255,0,204},
    {255,0,153},
    {255,0,102},
    {255,0,51},
    {255,0,0},
    {204,255,255},
    {204,255,204},
    {204,255,153},
    {204,255,102},
    {204,255,51},
    {204,255,0},
    {204,204,255},
    {204,204,204},
    {204,204,153},
    {204,204,102},
    {204,204,51},
    {204,204,0},
    {204,153,255},
    {204,153,204},
    {204,153,153},
    {204,153,102},
    {204,153,51},
    {204,153,0},
    {204,102,255},
    {204,102,204},
    {204,102,153,},
    {204,102,102},
    {204,102,51},
    {204,102,0},
    {204,51,255},
    {204,51,204},
    {204,51,153},
    {204,51,102},
    {204,51,51},
    {204,51,0},
    {204,0,255},
    {204,0,204},
    {204,0,153},
    {204,0,102},
    {204,0,51},
    {204,0,0},
    {153,255,255},
    {153,255,204},
    {153,255,153},
    {153,255,102},
    {153,255,51},
    {153,255,0},
    {153,204,255},
    {153,204,204},
    {153,204,153},
    {153,204,102},
    {153,204,51},
    {153,204,0},
    {153,153,255},
    {153,153,204},
    {153,153,153},
    {153,153,102},
    {153,153,51},
    {153,153,0},
    {153,102,255},
    {153,102,204},
    {153,102,153},
    {153,102,102},
    {153,102,51},
    {153,102,0},
    {153,51,255},
    {153,51,204},
    {153,51,153},
    {153,51,102},
    {153,51,51},
    {153,51,0},
    {153,0,255},
    {153,0,204},
    {153,0,153},
    {153,0,102},
    {153,0,51},
    {153,0,0},
    {102,255,255},
    {102,255,204},
    {102,255,153},
    {102,255,102},
    {102,255,51},
    {102,255,0},
    {102,204,255},
    {102,204,204},
    {102,204,153},
    {102,204,102},
    {102,204,51},
    {102,204,0},
    {102,153,255},
    {102,153,204},
    {102,153,153},
    {102,153,102},
    {102,153,51},
    {102,153,0},
    {102,102,255},
    {102,102,204},
    {102,102,153},
    {102,102,102},
    {102,102,51},
    {102,102,0},
    {102,51,255},
    {102,51,204},
    {102,51,153},
    {102,51,102},
    {102,51,51},
    {102,51,0},
    {102,0,255},
    {102,0,204},
    {102,0,153},
    {102,0,102},
    {102,0,51},
    {102,0,0},
    {51,255,255},
    {51,255,204},
    {51,255,153},
    {51,255,102},
    {51,255,51},
    {51,255,0},
    {51,204,255},
    {51,204,204},
    {51,204,153},
    {51,204,102},
    {51,204,51},
    {51,204,0},
    {51,153,255},
    {51,153,204},
    {51,153,153},
    {51,153,102},
    {51,153,51},
    {51,153,0},
    {51,102,255},
    {51,102,204},
    {51,102,153},
    {51,102,102},
    {51,102,51},
    {51,102,0},
    {51,51,255},
    {51,51,204},
    {51,51,153},
    {51,51,102},
    {51,51,51},
    {51,51,0},
    {51,0,255},
    {51,0,204},
    {51,0,153},
    {51,0,102},
    {51,0,51},
    {51,0,0},
    {0,255,255},
    {0,255,204},
    {0,255,153},
    {0,255,102},
    {0,255,51},
    {0,255,0},
    {0,204,255},
    {0,204,204},
    {0,204,153},
    {0,204,102},
    {0,204,51},
    {0,204,0},
    {0,153,255},
    {0,153,204},
    {0,153,153},
    {0,153,102},
    {0,153,51},
    {0,153,0},
    {0,102,255},
    {0,102,204},
    {0,102,153},
    {0,102,102},
    {0,102,51},
    {0,102,0},
    {0,51,255},
    {0,51,204},
    {0,51,153},
    {0,51,102},
    {0,51,51},
    {0,51,0},
    {0,0,255},
    {0,0,204},
    {0,0,153},
    {0,0,102},
    {0,0,51},
    {0,0,0}
  };

  for ( int i = 0; i < size ; i ++ )
  {
    map.insert ( i, QColor ( array[i][0], array[i][1], array[i][2] ) );
  }
  return map;
}

/**
* Royal Webfarben
*/
const QVector<QColor> Colors::royalColors()
{
  int size = 256;
  QVector<QColor> map;
  int array[256][3] =
  {
    {0,0,0},
    {60,0,80},
    {60,0,80},
    {60,0,84},
    {64,0,84},
    {64,0,84},
    {64,0,88},
    {64,0,88},
    {68,0,88},
    {68,0,92},
    {68,0,92},
    {68,0,92},
    {72,0,96},
    {72,0,96},
    {72,0,96},
    {72,0,100},
    {76,0,100},
    {76,0,100},
    {76,0,104},
    {76,0,104},
    {76,0,104},
    {80,0,104},
    {80,0,108},
    {80,0,108},
    {80,0,108},
    {84,0,112},
    {84,0,112},
    {84,0,112},
    {84,0,116},
    {88,0,116},
    {88,0,116},
    {88,0,120},
    {88,0,120},
    {92,0,120},
    {92,0,124},
    {92,0,124},
    {92,0,124},
    {96,0,128},
    {96,0,128},
    {96,0,128},
    {96,0,132},
    {96,0,132},
    {100,0,132},
    {100,0,132},
    {100,0,136},
    {100,0,136},
    {104,0,136},
    {104,0,140},
    {104,0,140},
    {104,0,140},
    {108,0,144},
    {108,0,144},
    {108,0,144},
    {108,0,148},
    {112,0,148},
    {112,0,148},
    {112,0,152},
    {112,0,152},
    {116,0,152},
    {116,0,156},
    {116,0,156},
    {116,0,156},
    {120,0,160},
    {120,0,160},
    {124,4,160},
    {124,8,164},
    {128,12,164},
    {128,16,164},
    {132,20,168},
    {132,24,168},
    {136,28,168},
    {136,32,172},
    {140,36,172},
    {140,40,172},
    {144,44,176},
    {144,48,176},
    {148,52,180},
    {148,56,180},
    {152,60,180},
    {152,64,184},
    {156,68,184},
    {156,72,184},
    {160,76,188},
    {160,80,188},
    {164,84,188},
    {164,88,192},
    {168,92,192},
    {168,96,192},
    {172,100,196},
    {172,104,196},
    {176,108,200},
    {176,112,200},
    {180,116,200},
    {180,120,204},
    {184,124,204},
    {188,128,204},
    {188,132,208},
    {192,136,208},
    {192,140,208},
    {196,144,212},
    {196,148,212},
    {200,152,216},
    {200,156,216},
    {204,160,216},
    {204,164,220},
    {208,168,220},
    {208,172,220},
    {212,176,224},
    {212,180,224},
    {216,184,224},
    {216,188,228},
    {220,192,228},
    {220,196,228},
    {224,200,232},
    {224,204,232},
    {228,208,236},
    {228,212,236},
    {232,216,236},
    {232,220,240},
    {236,224,240},
    {236,228,240},
    {240,232,244},
    {240,236,244},
    {244,240,244},
    {244,244,248},
    {248,248,248},
    {252,252,252},
    {252,252,252},
    {252,252,248},
    {252,252,244},
    {252,252,240},
    {252,252,236},
    {252,252,232},
    {252,252,228},
    {252,252,224},
    {252,252,220},
    {252,252,216},
    {252,252,212},
    {252,252,208},
    {252,252,204},
    {252,252,200},
    {252,252,196},
    {252,252,192},
    {252,252,188},
    {252,252,184},
    {252,252,180},
    {252,252,176},
    {252,252,172},
    {252,252,168},
    {252,252,164},
    {252,252,160},
    {252,252,156},
    {252,252,152},
    {252,252,148},
    {252,252,144},
    {252,252,140},
    {252,252,136},
    {252,252,132},
    {252,252,128},
    {252,252,124},
    {252,252,120},
    {252,252,116},
    {252,252,112},
    {252,252,108},
    {252,252,104},
    {252,252,100},
    {252,252,96},
    {252,252,92},
    {252,252,88},
    {252,252,84},
    {252,252,80},
    {252,252,76},
    {252,252,72},
    {252,252,68},
    {252,252,64},
    {252,252,60},
    {252,252,56},
    {252,252,52},
    {252,252,48},
    {252,252,44},
    {252,252,40},
    {252,252,36},
    {252,252,32},
    {252,252,28},
    {252,252,24},
    {252,252,20},
    {252,252,16},
    {252,252,12},
    {252,252,8},
    {252,252,4},
    {252,252,0},
    {252,248,0},
    {248,244,0},
    {244,240,0},
    {240,236,4},
    {240,232,4},
    {236,228,4},
    {232,224,8},
    {228,220,8},
    {228,216,8},
    {224,212,12},
    {220,208,12},
    {216,204,12},
    {212,200,16},
    {212,196,16},
    {208,192,16},
    {204,188,20},
    {200,184,20},
    {200,180,20},
    {196,176,24},
    {192,172,24},
    {188,168,24},
    {184,164,28},
    {184,160,28},
    {180,156,28},
    {176,152,32},
    {172,148,32},
    {172,144,32},
    {168,140,36},
    {164,136,36},
    {160,132,36},
    {160,128,36},
    {156,124,40},
    {152,120,40},
    {148,116,40},
    {144,112,44},
    {144,108,44},
    {140,104,44},
    {136,100,48},
    {132,96,48},
    {132,92,48},
    {128,88,52},
    {124,84,52},
    {120,80,52},
    {116,76,56},
    {116,72,56},
    {112,68,56},
    {108,64,60},
    {104,60,60},
    {104,56,60},
    {100,52,64},
    {96,48,64},
    {92,44,64},
    {88,40,68},
    {88,36,68},
    {84,32,68},
    {80,28,72},
    {76,24,72},
    {76,20,72},
    {72,16,76},
    {68,12,76},
    {64,8,76},
    {60,0,80},
    {60,0,80},
    {60,0,80},
    {60,0,80}
  };
  for ( int i = 0; i < size ; i ++ )
  {
    map.insert ( i, QColor ( array[i][0], array[i][1], array[i][2] ) );
  }
  return map;
}

/**
* Regenbogen Webfarben
*/
const QVector<QColor> Colors::rainbowColors()
{
  int size = 117;
  QVector<QColor> map;
  int array[117][3] =
  {
    {255,204,204},
    {255,230,204},
    {255,255,204},
    {230,255,204},
    {204,255,204},
    {204,255,230},
    {204,255,255},
    {204,230,255},
    {204,204,255},
    {230,204,255},
    {255,204,255},
    {255,204,230},
    {255,255,255},
    {255,153,153},
    {255,204,153},
    {255,255,153},
    {204,255,153},
    {153,255,153},
    {153,255,204},
    {153,255,255},
    {153,204,255},
    {153,153,255},
    {204,153,255},
    {255,153,255},
    {255,153,204},
    {224,224,224},
    {255,102,102},
    {255,179,102},
    {255,255,102},
    {179,255,102},
    {102,255,102},
    {102,255,179},
    {102,255,255},
    {102,179,255},
    {102,102,255},
    {179,102,255},
    {255,102,255},
    {255,102,179},
    {192,192,192},
    {255,51,51},
    {255,153,51},
    {255,255,51},
    {153,255,51},
    {51,255,51},
    {51,255,153},
    {51,255,255},
    {51,153,255},
    {51,51,255},
    {153,51,255},
    {255,51,255},
    {255,51,153},
    {160,160,160},
    {255,0,0},
    {255,128,0},
    {255,255,0},
    {128,255,0},
    {0,255,0},
    {0,255,128},
    {0,255,255},
    {0,128,255},
    {0,0,255},
    {128,0,255},
    {255,0,255},
    {255,0,128},
    {128,128,128},
    {204,0,0},
    {204,102,0},
    {204,204,0},
    {102,204,0},
    {0,204,0},
    {0,204,102},
    {0,204,204},
    {0,102,204},
    {0,0,204},
    {102,0,204},
    {204,0,204},
    {204,0,102},
    {96,96,96},
    {153,0,0},
    {153,77,0},
    {153,153,0},
    {77,153,0},
    {0,153,0},
    {0,153,77},
    {0,153,153},
    {0,77,153},
    {0,0,153},
    {77,0,153},
    {153,0,153},
    {153,0,77},
    {64,64,64},
    {102,0,0},
    {102,51,0},
    {102,102,0},
    {51,102,0},
    {0,102,0},
    {0,102,51},
    {0,102,102},
    {0,51,102},
    {0,0,102},
    {51,0,102},
    {102,0,102},
    {102,0,51},
    {32,32,32},
    {51,0,0},
    {51,26,0},
    {51,51,0},
    {26,51,0},
    {0,51,0},
    {0,51,26},
    {0,51,51},
    {0,26,51},
    {0,0,51},
    {26,0,51},
    {51,0,51},
    {51,0,26},
    {0,0,0}
  };
  for ( int i = 0; i < size ; i ++ )
  {
    map.insert ( i, QColor ( array[i][0], array[i][1], array[i][2] ) );
  }
  return map;
}