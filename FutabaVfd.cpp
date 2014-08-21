
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







