#pragma once

#ifndef BETTERBTSAI_H
#define BETTERBTSAI_H

//#include "CvGameCoreDLL.h"
#include "CvDepends.h"

// AI decision making logging

// Log levels:
// 0 - None
// 1 - Important decisions only
// 2 - Many decisions
// 3 - All logging
#ifdef LOG_AI
#define gPlayerLogLevel 3
#define gTeamLogLevel   3
#define gCityLogLevel   3
#define gUnitLogLevel   3
#define gMapLogLevel    3 // K-Mod
#else
#define gPlayerLogLevel 0
#define gTeamLogLevel   0
#define gCityLogLevel   0
#define gUnitLogLevel   0
#define gMapLogLevel    0 // K-Mod
#endif

void logBBAI(TCHAR* format, ... );

#endif  //BETTERBTSAI_H
