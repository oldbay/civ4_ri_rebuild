
//#include "CvGameCoreDLL.h"
#include "BetterBTSAI.h"

// AI decision making logging

//void logBBAI(TCHAR* format, ... ) //PORT OLD
void logBBAI(const TCHAR* format, ... ) //PORT NEW
{
#ifdef LOG_AI
	static char buf[2048];
	va_list args;
    va_start(args, format);
    #if defined(__GNUC__)
    vsnprintf(buf, 2048-4, format, args);
    #else
    _vsnprintf(buf, 2048-4, format, args);
    #endif
	va_end(args);
    //gDLL->logMsg("BBAI.log", buf); //PORT OLD ??? cross include CvDLLUtilityIFaceBase.h
#endif
}
