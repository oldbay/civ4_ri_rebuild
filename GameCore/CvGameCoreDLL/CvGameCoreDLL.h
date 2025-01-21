#pragma once

#ifndef CvGameCoreDLL_h
#define CvGameCoreDLL_h

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
