//
// Copyright (C) 2014-2015 Stéphane Lenclud.
//
// This file is part of MiniDisplay.
//
// MiniDisplay is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// MiniDisplay is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with MiniDisplay.  If not, see <http://www.gnu.org/licenses/>.
//


#include "FutabaVfd.h"
//#include <stdlib.h>
#include <string.h>


#ifdef DEBUG_FRAME_DIFF
#include <QImage>
#include <QTextStream>
#endif


//
//
//

FutabaVfdCommand::FutabaVfdCommand():/*iBuffer(NULL),*/iSize(0),iMaxSize(0)
    {
    }

FutabaVfdCommand::~FutabaVfdCommand()
    {
    //Delete();
    }


/**

*/
void FutabaVfdCommand::Reset()
    {
    memset(iReports,0,sizeof(iReports));
    }



/**

*/
/*
void FutabaVfdCommand::Create(int aMaxSize)
    {
    iBuffer=new unsigned char[aMaxSize];
    if (iBuffer)
        {
        iMaxSize = aMaxSize;
        iSize = 0;
        }
    }
*/

/**

*/
/*
void FutabaVfdCommand::Delete()
{
    delete[] iBuffer;
    iBuffer = NULL;
    iMaxSize = 0;
    iSize = 0;
}
*/


//
// class FutabaDisplay
//

void FutabaGraphicDisplay::Close()
	{
	HidDevice::Close();
	}




