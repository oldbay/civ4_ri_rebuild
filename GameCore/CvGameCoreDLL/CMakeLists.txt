cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

project(CvGameCoreDLL.2012 CXX)

################################################################################
# Set target arch type if empty. Visual studio solution generator provides it.
################################################################################
if(NOT CMAKE_VS_PLATFORM_NAME)
    set(CMAKE_VS_PLATFORM_NAME "x64")
endif()
message("${CMAKE_VS_PLATFORM_NAME} architecture in use")

################################################################################
# Include headers dirs
################################################################################

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    find_package(PythonLibs REQUIRED)
    include_directories(${PYTHON_INCLUDE_DIRS})
    link_directories(${PYTHON_LIBRARY})
    #include_directories("/usr/include/python3.9")
    #link_directories("/usr/lib/x86_64-linux-gnu/")
endif()


################################################################################
# Global configuration types
################################################################################
set(CMAKE_CONFIGURATION_TYPES
    "Assert"
    "Assertfast"
    "Debug"
    "Profile"
    "Release"
    "Releasefast"
    "ReleaseOpti"
    CACHE STRING "" FORCE
)

################################################################################
# Global compiler options
################################################################################
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    #set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIE")
elseif(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_CXX_FLAGS "")
    set(CMAKE_CXX_FLAGS_ASSERT "")
    set(CMAKE_CXX_FLAGS_ASSERTFAST "")
    set(CMAKE_CXX_FLAGS_DEBUG "")
    set(CMAKE_CXX_FLAGS_PROFILE "")
    set(CMAKE_CXX_FLAGS_RELEASE "")
    set(CMAKE_CXX_FLAGS_RELEASEFAST "")
    set(CMAKE_CXX_FLAGS_RELEASEOPTI "")
endif()

################################################################################
# Global linker options
################################################################################
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_EXE_LINKER_FLAGS "")
    set(CMAKE_MODULE_LINKER_FLAGS "")
    set(CMAKE_SHARED_LINKER_FLAGS "")
    set(CMAKE_STATIC_LINKER_FLAGS "")
    set(CMAKE_EXE_LINKER_FLAGS_ASSERT "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_ASSERT "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_ASSERT "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_ASSERT "${CMAKE_STATIC_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS_ASSERTFAST "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_ASSERTFAST "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_ASSERTFAST "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_ASSERTFAST "${CMAKE_STATIC_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS_PROFILE "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_PROFILE "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_PROFILE "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_PROFILE "${CMAKE_STATIC_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASEFAST "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_RELEASEFAST "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASEFAST "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_RELEASEFAST "${CMAKE_STATIC_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASEOPTI "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_RELEASEOPTI "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASEOPTI "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_RELEASEOPTI "${CMAKE_STATIC_LINKER_FLAGS}")
endif()

################################################################################
# Nuget packages function stub.
################################################################################
function(use_package TARGET PACKAGE VERSION)
    message(WARNING "No implementation of use_package. Create yours. "
                    "Package \"${PACKAGE}\" with version \"${VERSION}\" "
                    "for target \"${TARGET}\" is ignored!")
endfunction()

################################################################################
# Common utils
################################################################################
include(CMake/Utils.cmake)

################################################################################
# Additional Global Settings(add specific info there)
################################################################################
include(CMake/GlobalSettingsInclude.cmake OPTIONAL)

################################################################################
# Use solution folders feature
################################################################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

################################################################################
# Sub-projects
################################################################################
set(PROJECT_NAME CvGameCoreDLL.2012)

################################################################################
#options suported by the cmake builder
################################################################################
option(WITH_LOG_AI "Logging AI enable" OFF)


###############################################################################
# Project variables
###############################################################################

if(WITH_LOG_AI)
  set(LOG_AI True)
endif(WITH_LOG_AI)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "Makefile"
    "Makefile.settings.default.mk"
    "Makefile.settings.mk"
)
source_group("" FILES ${no_group_source_files})

set(Headers
    "CvDepends.h"
    "AI_Defines.h"
    "BetterBTSAI.h"
    "CvArea.h"
    "CvArtFileMgr.h"
    "CvBugOptions.h"
    "CvCity.h"
    "CvCityAI.h"
    "CvDeal.h"
    "CvDefines.h"
    "CvDiploParameters.h"
    "CvDLLButtonPopup.h"
    "CvDLLDataStreamIFaceBase.h"
    "CvDLLEngineIFaceBase.h"
    "CvDLLEntity.h"
    "CvDLLEntityIFaceBase.h"
    "CvDLLEventReporterIFaceBase.h"
    "CvDLLFAStarIFaceBase.h"
    "CvDLLFlagEntityIFaceBase.h"
    "CvDLLIniParserIFaceBase.h"
    "CvDLLInterfaceIFaceBase.h"
    "CvDLLPlotBuilderIFaceBase.h"
    "CvDllPythonEvents.h"
    "CvDLLPythonIFaceBase.h"
    "CvDLLSymbolIFaceBase.h"
    "CvDllTranslator.h"
    "CvDLLUtilityIFaceBase.h"
    "CvDLLWidgetData.h"
    "CvDLLXMLIFaceBase.h"
    "CvEnums.h"
    "CvEventReporter.h"
    "CvFractal.h"
    "CvGame.h"
    "CvGameAI.h"
    "CvGameCoreDLL.h"
    "CvGameCoreDLLDefNew.h"
    "CvGameCoreDLLUndefNew.h"
    "CvGameCoreUtils.h"
    "CvGameTextMgr.h"
    "CvGlobals.h"
    "CvHallOfFameInfo.h"
    "CvInfos.h"
    "CvInfoWater.h"
    "CvInitCore.h"
    "CvMap.h"
    "CvMapGenerator.h"
    "CvMessageControl.h"
    "CvMessageData.h"
    "CvPlayer.h"
    "CvPlayerAI.h"
    "CvPlot.h"
    "CvPlotGroup.h"
    "CvPopupInfo.h"
    "CvPopupReturn.h"
    "CvRandom.h"
    "CvReplayInfo.h"
    "CvReplayMessage.h"
    "CvSelectionGroup.h"
    "CvSelectionGroupAI.h"
    "CvStatistics.h"
    "CvString.h"
    "CvStructs.h"
    "CvStructs.inl"
    "CvTalkingHeadMessage.h"
    "CvTeam.h"
    "CvTeamAI.h"
    "CvTechGraph.h"
    "CvUnit.h"
    "CvUnitAI.h"
    "CvXMLLoadUtility.h"
    "CyArea.h"
    "CyArgsList.h"
    "CyArtFileMgr.h"
    "CyCity.h"
    "CyDeal.h"
    "CyGame.h"
    "CyGameCoreUtils.h"
    "CyGameTextMgr.h"
    "CyGlobalContext.h"
    "CyHallOfFameInfo.h"
    "CyMap.h"
    "CyMapGenerator.h"
    "CyMessageControl.h"
    "CyPlayer.h"
    "CyPlot.h"
    "CyReplayInfo.h"
    "CySelectionGroup.h"
    "CyTeam.h"
    "CyTechGraph.h"
    "CyUnit.h"
    "FAssert.h"
    "FAStarNode.h"
    "FDataStreamBase.h"
    "FDialogTemplate.h"
    "KmodPathFinder.h"
    "FFreeListArray.h"
    "FFreeListArrayBase.h"
    "FFreeListTrashArray.h"
    "FInputDevice.h"
    "FProfiler.h"
    "FVariableSystem.h"
    "FVariableSystem.inl"
    "LinkedList.h"
    "resource.h"
)
source_group("Headers" FILES ${Headers})

set(Sources
    "_precompile.cpp"
    "BetterBTSAI.cpp"
    "CvArea.cpp"
    "CvArtFileMgr.cpp"
    "CvBugOptions.cpp"
    "CvCity.cpp"
    "CvCityAI.cpp"
    "CvDeal.cpp"
    "CvDiploParameters.cpp"
    "CvDLLButtonPopup.cpp"
    "CvDLLEntity.cpp"
    "CvDLLPython.cpp"
    "CvDllPythonEvents.cpp"
    "CvDllTranslator.cpp"
    "CvDLLWidgetData.cpp"
    "CvEventReporter.cpp"
    "CvFractal.cpp"
    "CvGame.cpp"
    "CvGameAI.cpp"
    "CvGameCoreDLL.cpp"
    "CvGameCoreUtils.cpp"
    "CvGameInterface.cpp"
    "CvGameTextMgr.cpp"
    "CvGlobals.cpp"
    "CvHallOfFameInfo.cpp"
    "CvInfos.cpp"
    "CvInfoWater.cpp"
    "CvInitCore.cpp"
    "CvMap.cpp"
    "CvMapGenerator.cpp"
    "CvMessageControl.cpp"
    "CvMessageData.cpp"
    "CvPlayer.cpp"
    "CvPlayerAI.cpp"
    "CvPlot.cpp"
    "CvPlotGroup.cpp"
    "CvPopupInfo.cpp"
    "CvPopupReturn.cpp"
    "CvRandom.cpp"
    "CvReplayInfo.cpp"
    "CvReplayMessage.cpp"
    "CvSelectionGroup.cpp"
    "CvSelectionGroupAI.cpp"
    "CvStatistics.cpp"
    "CvStructs.cpp"
    "CvTalkingHeadMessage.cpp"
    "CvTeam.cpp"
    "CvTeamAI.cpp"
    "CvTechGraph.cpp"
    "CvUnit.cpp"
    "CvUnitAI.cpp"
    "CvXMLLoadUtility.cpp"
    "CvXMLLoadUtilityGet.cpp"
    "CvXMLLoadUtilityInit.cpp"
    "CvXMLLoadUtilitySet.cpp"
    "CyArea.cpp"
    "CyAreaInterface.cpp"
    "CyArgsList.cpp"
    "CyArtFileMgr.cpp"
    "CyArtFileMgrInterface.cpp"
    "CyCity.cpp"
    "CyCityInterface1.cpp"
    "CyDeal.cpp"
    "CyEnumsInterface.cpp"
    "CyGame.cpp"
    "CyGameCoreUtils.cpp"
    "CyGameCoreUtilsInterface.cpp"
    "CyGameInterface.cpp"
    "CyGameTextMgr.cpp"
    "CyGameTextMgrInterface.cpp"
    "CyGlobalContext.cpp"
    "CyGlobalContextInterface1.cpp"
    "CyGlobalContextInterface2.cpp"
    "CyGlobalContextInterface3.cpp"
    "CyGlobalContextInterface4.cpp"
    "CyHallOfFameInfo.cpp"
    "CyHallOfFameInterface.cpp"
    "CyInfoInterface1.cpp"
    "CyInfoInterface2.cpp"
    "CyInfoInterface3.cpp"
    "CyInfoInterface4.cpp"
    "CyMap.cpp"
    "CyMapGenerator.cpp"
    "CyMapGeneratorInterface.cpp"
    "CyMapInterface.cpp"
    "CyMessageControl.cpp"
    "CyMessageControlInterface.cpp"
    "CyPlayer.cpp"
    "CyPlayerInterface1.cpp"
    "CyPlayerInterface2.cpp"
    "CyPlot.cpp"
    "CyPlotInterface1.cpp"
    "CyRandomInterface.cpp"
    "CyReplayInfo.cpp"
    "CySelectionGroup.cpp"
    "CySelectionGroupInterface.cpp"
    "CyStructsInterface1.cpp"
    "CyTeam.cpp"
    "CyTeamInterface.cpp"
    "CyTechGraph.cpp"
    "CyTechGraphInterface.cpp"
    "CyUnit.cpp"
    "CyUnitInterface1.cpp"
    "FAssert.cpp"
    "FDialogTemplate.cpp"
    "KmodPathFinder.cpp"
)
source_group("Sources" FILES ${Sources})

set(ALL_FILES
    ${no_group_source_files}
    ${Headers}
    ${Sources}
)

################################################################################
# Depands
################################################################################
   #"/usr/lib/x86_64-linux-gnu/"
   #"boost_program_options"
   #"boost_system"
   #"boost_python39"
   #"/usr/lib/python3.9/config-3.9-x86_64-linux-gnu"
   #"/usr/lib"
   #"python3.9"
   #"crypt"
   #"pthread"
   #"dl"
   #"util"
   #"m"

set (Depends
   "boost_program_options"
   "boost_system"
   "boost_python39"
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "MakeFileProj"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASEOPTI "TRUE"
)
################################################################################
# Compile definitions
################################################################################

################################################################################
# Compile and link options
################################################################################
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_library(
        CvGameCoreDLL SHARED ${Sources}
    )
    target_link_libraries(
        #CvGameCoreDLL ${Depends}
        CvGameCoreDLL ${Depends} ${PYTHON_LIBRARIES}
    )
elseif(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        /DEBUG:FULL
    )
endif()
