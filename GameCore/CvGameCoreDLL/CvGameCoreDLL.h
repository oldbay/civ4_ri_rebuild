#pragma once

#ifndef CvGameCoreDLL_h
#define CvGameCoreDLL_h

//
// includes (pch) for gamecore dll files
// Author - Mustafa Thamer
//

#if defined(__GNUC__)
#include <iostream>
#include <unordered_map>
#include <vector>
#include <stdint.h>

#ifdef UNICODE //replace tchar.h
#define _tcslen     wcslen
#define _tcscpy     wcscpy
#define _tcscpy_s   wcscpy_s
#define _tcsncpy    wcsncpy
#define _tcsncpy_s  wcsncpy_s
#define _tcscat     wcscat
#define _tcscat_s   wcscat_s
#define _tcsupr     wcsupr
#define _tcsupr_s   wcsupr_s
#define _tcslwr     wcslwr
#define _tcslwr_s   wcslwr_s
#define _stprintf_s swprintf_s
#define _stprintf   swprintf
#define _tprintf    wprintf
#define _vstprintf_s    vswprintf_s
#define _vstprintf      vswprintf
#define _tscanf     wscanf
#define TCHAR wchar_t

#else

#define _tcslen     strlen
#define _tcscpy     strcpy
#define _tcscpy_s   strcpy_s
#define _tcsncpy    strncpy
#define _tcsncpy_s  strncpy_s
#define _tcscat     strcat
#define _tcscat_s   strcat_s
#define _tcsupr     strupr
#define _tcsupr_s   strupr_s
#define _tcslwr     strlwr
#define _tcslwr_s   strlwr_s
#define _stprintf_s sprintf_s
#define _stprintf   sprintf
#define _tprintf    printf
#define _vstprintf_s    vsprintf_s
#define _vstprintf      vsprintf
#define _tscanf     scanf
#define TCHAR char
#endif //end replace tchar

#else
#pragma warning( disable: 4530 )	// C++ exception handler used, but unwind semantics are not enabled
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <MMSystem.h>
#include <tchar.h>
#endif

#if defined _DEBUG && !defined USE_MEMMANAGER
#define USE_MEMMANAGER
#include <crtdbg.h>
#endif

#include <vector>
#include <list>
#include <math.h>
#include <assert.h>
#include <map>
#include <hash_map>
// K-Mod
#include <set>
#include <utility>
#include <algorithm>
// K-Mod end

#if defined(__GNUC__)
#define DllExport __attribute__((visibility("default")))
#else
#define DllExport   __declspec( dllexport )
#endif
//
// GameBryo
//
class NiColor
{
public:
	float r, g, b;
};
class NiColorA 
{
public:
	NiColorA(float fr, float fg, float fb, float fa) : r(fr), g(fg), b(fb), a(fa) {}
	NiColorA() {}
	float r, g, b, a;
};
class NiPoint2
{
public:
	NiPoint2() {}
	NiPoint2(float fx, float fy) : x(fx),y(fy) {}

	float x, y;
};
class NiPoint3
{
public:
	NiPoint3() {}
	NiPoint3(float fx, float fy, float fz) : x(fx),y(fy),z(fz) {} 

    bool operator== (const NiPoint3& pt) const
	{	return (x == pt.x && y == pt.y && z == pt.z);	}

    inline NiPoint3 operator+ (const NiPoint3& pt) const
	{	return NiPoint3(x+pt.x,y+pt.y,z+pt.z);	}

    inline NiPoint3 operator- (const NiPoint3& pt) const
	{	return NiPoint3(x-pt.x,y-pt.y,z-pt.z);	}

    inline float operator* (const NiPoint3& pt) const
	{	return x*pt.x+y*pt.y+z*pt.z;	}

    inline NiPoint3 operator* (float fScalar) const
	{	return NiPoint3(fScalar*x,fScalar*y,fScalar*z);	}

    inline NiPoint3 operator/ (float fScalar) const
	{
		float fInvScalar = 1.0f/fScalar;
		return NiPoint3(fInvScalar*x,fInvScalar*y,fInvScalar*z);
	}

    inline NiPoint3 operator- () const
	{	return NiPoint3(-x,-y,-z);	}

	inline float Length() const
	{ return sqrt(x * x + y * y + z * z); }

	inline float Unitize()
	{
		float length = Length();
		if(length != 0)
		{
			x /= length;
			y /= length;
			z /= length;
		}
		return length;
	}

//	inline NiPoint3 operator* (float fScalar, const NiPoint3& pt)
//	{	return NiPoint3(fScalar*pt.x,fScalar*pt.y,fScalar*pt.z);	}
	float x, y, z;
};

namespace NiAnimationKey
{
	enum KeyType
	{
		NOINTERP,
		LINKEY,
		BEZKEY,
		TCBKEY,
		EULERKEY,
		STEPKEY,
		NUMKEYTYPES
	};
};

#if defined(__GNUC__)
typedef unsigned int qword;
#else
typedef unsigned __int64 qword;
#endif //Os Select

typedef unsigned char    byte;
typedef unsigned short   word;
typedef unsigned int     uint;
typedef unsigned long    dword;
typedef wchar_t          wchar;

#define MAX_CHAR                            (0x7f)
#define MIN_CHAR                            (0x80)
#define MAX_SHORT                           (0x7fff)
#define MIN_SHORT                           (0x8000)
#define MAX_INT                             (0x7fffffff)
#define MIN_INT                             (0x80000000)
#define MAX_UNSIGNED_CHAR                   (0xff)
#define MIN_UNSIGNED_CHAR                   (0x00)
#define MAX_UNSIGNED_SHORT                  (0xffff)
#define MIN_UNSIGNED_SHORT                  (0x0000)
#define MAX_UNSIGNED_INT                    (0xffffffff)
#define MIN_UNSIGNED_INT                    (0x00000000)

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

#define SQR(x)      ( (x) * (x) )
#define DEGTORAD(x) ( (float)( (x) * (M_PI / 180) ))
#define LIMIT_RANGE(low, value, high) value = (value < low ? low : (value > high ? high : value));
#define M_PI       3.14159265358979323846
#define fM_PI		3.141592654f		//!< Pi (float)

#if defined(__GNUC__)
inline uint32_t FtoDW( float f ) { return *(uint32_t*)&f; }
inline float DWtoF( dword n ) { return *(float*)&n; }
inline float MaxFloat() { return DWtoF(0x7f7fffff); }
#else
__forceinline DWORD FtoDW( float f ) { return *(DWORD*)&f; }
__forceinline float DWtoF( dword n ) { return *(float*)&n; }
__forceinline float MaxFloat() { return DWtoF(0x7f7fffff); }
#endif

void startProfilingDLL(bool longLived);
void stopProfilingDLL(bool longLived);

#if not defined(__GNUC__)
#ifdef USE_INTERNAL_PROFILER
struct ProfileSample;
void IFPBeginSample(ProfileSample* sample);
void IFPEndSample(ProfileSample* sample);
void dumpProfileStack(void);
void EnableDetailedTrace(bool enable);
#endif

#ifdef _DEBUG
//#define MEMORY_TRACKING
#endif

#ifdef MEMORY_TRACKING
class CMemoryTrack
{
#define	MAX_TRACKED_ALLOCS	1000
	void*	m_track[MAX_TRACKED_ALLOCS];
	int		m_highWater;
	const char* m_name;
	bool	m_valid;
#define MAX_TRACK_DEPTH		50
	static	CMemoryTrack*	trackStack[MAX_TRACK_DEPTH];
	static	m_trackStackDepth;

public:
	CMemoryTrack(const char* name, bool valid);

	~CMemoryTrack();

	void NoteAlloc(void* ptr);
	void NoteDeAlloc(void* ptr);

	static CMemoryTrack* GetCurrent(void);
};

class CMemoryTrace
{
	int		m_start;
	const char* m_name;

public:
	CMemoryTrace(const char* name);

	~CMemoryTrace();
};

void DumpMemUsage(const char* fn, int line);

#define DUMP_MEMORY_USAGE()	DumpMemUsage(__FUNCTION__,__LINE__);
#define MEMORY_TRACK()	CMemoryTrack __memoryTrack(__FUNCTION__, true);
#define MEMORY_TRACK_EXEMPT()	CMemoryTrack __memoryTrackExemption(NULL, false);
#define MEMORY_TRACE_FUNCTION()	CMemoryTrace __memoryTrace(__FUNCTION__);
#else
#define DUMP_MEMORY_USAGE()	
#define	MEMORY_TRACK()
#define MEMORY_TRACK_EXEMPT()
#define MEMORY_TRACE_FUNCTION()
#endif
#endif //OS Select


//
// Boost Python
//
# include <boost/python/list.hpp>
# include <boost/python/tuple.hpp>
# include <boost/python/class.hpp>
# include <boost/python/manage_new_object.hpp>
# include <boost/python/return_value_policy.hpp>
# include <boost/python/object.hpp>
# include <boost/python/def.hpp>

namespace python = boost::python;

#include "FAssert.h"
#include "CvGameCoreDLLDefNew.h"
#include "FDataStreamBase.h"
#include "FFreeListArrayBase.h"
#include "FFreeListTrashArray.h"
#include "FFreeListArray.h"
//#include "FVariableSystem.h"
#include "CvString.h"
#include "CvEnums.h"
#include "CvStructs.h"
#include "CvDLLUtilityIFaceBase.h"

//jason tests
#include "CvPlayerAI.h"
#include "CvGameCoreUtils.h"
#include "CvMap.h"
#include "CvGameAI.h"
#include "CvPlot.h"
#include "CvUnit.h"
#include "CvGlobals.h"
#include "CvCity.h"
#include "FProfiler.h"
#include "CyCity.h"
#include "CvInfos.h"
#include "CvTeamAI.h"
#include "CvDLLPythonIFaceBase.h"
#include "CvRandom.h"
#include "CvArea.h"
#include "CvDLLEntity.h"
#include "CvDeal.h"
#include "CvDLLEntityIFaceBase.h"
#include "CvGame.h"
#include "CyGlobalContext.h"
#include "CvSelectionGroup.h"
#include "CvTalkingHeadMessage.h"
#include "CvPlotGroup.h"
#include "CvCityAI.h"
#include "CvSelectionGroupAI.h"
#include "CvUnitAI.h"
// MOD - START - Tech Graph
#include "CvTechGraph.h"
// MOD - END - Tech Graph


#if not defined(__GNUC__)
#ifdef FINAL_RELEASE
// Undefine OutputDebugString in final release builds
#undef OutputDebugString
#define OutputDebugString(x)
#endif //FINAL_RELEASE
#endif //OS Select

#endif	// CvGameCoreDLL_h