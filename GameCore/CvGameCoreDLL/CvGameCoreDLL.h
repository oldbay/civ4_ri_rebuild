#pragma once

#ifndef CvGameCoreDLL_h
#define CvGameCoreDLL_h


void startProfilingDLL(bool longLived);
void stopProfilingDLL(bool longLived);

#if not defined(__GNUC__) // windows memory manager //PORT OLD
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

//ignore BOOST deprecated headers
#define BOOST_BIND_GLOBAL_PLACEHOLDERS // rewrite global placeholders //PORT NEW
#define BOOST_ALLOW_DEPRECATED_HEADERS //to doo rewrite boost deprecated headers //PORT NEW

# include <boost/python/list.hpp>
# include <boost/python/tuple.hpp>
# include <boost/python/class.hpp>
# include <boost/python/manage_new_object.hpp>
# include <boost/python/return_value_policy.hpp>
# include <boost/python/object.hpp>
# include <boost/python/def.hpp>

#include "CvDepends.h"

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
