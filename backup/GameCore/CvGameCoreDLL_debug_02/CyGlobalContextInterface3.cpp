//
// published python interface for CyGlobalContext
//

#include "CvGameCoreDLL.h"
#include "CyMap.h"
#include "CyPlayer.h"
#include "CyGame.h"
#include "CyGlobalContext.h"
#include "CvRandom.h"
//#include "CvStructs.h"
#include "CvInfos.h"
#include "CyTeam.h"


void CyGlobalContextPythonInterface3(python::class_<CyGlobalContext>& x)
{
    #if defined(__GNUC__)
    std::clog << "Python Extension Module - CyGlobalContextPythonInterface3\n";
    #else
    OutputDebugString("Python Extension Module - CyGlobalContextPythonInterface3\n");
    #endif

	x
		.def("getAttitudeInfo", &CyGlobalContext::getAttitudeInfo, python::return_value_policy<python::reference_existing_object>(), "AttitudeInfo (int id)")
		.def("getMemoryInfo", &CyGlobalContext::getMemoryInfo, python::return_value_policy<python::reference_existing_object>(), "MemoryInfo (int id)")

		.def("getNumPlayerOptionInfos", &CyGlobalContext::getNumPlayerOptionInfos)
		.def("getPlayerOptionsInfo", &CyGlobalContext::getPlayerOptionsInfoByIndex, python::return_value_policy<python::reference_existing_object>(), "(PlayerOptionsInfoID) - PlayerOptionsInfo for PlayerOptionsInfo")
		.def("getPlayerOptionsInfoByIndex", &CyGlobalContext::getPlayerOptionsInfoByIndex, python::return_value_policy<python::reference_existing_object>(), "(PlayerOptionsInfoID) - PlayerOptionsInfo for PlayerOptionsInfo")

		.def("getGraphicOptionsInfo", &CyGlobalContext::getGraphicOptionsInfoByIndex, python::return_value_policy<python::reference_existing_object>(), "(GraphicOptionsInfoID) - GraphicOptionsInfo for GraphicOptionsInfo")
		.def("getGraphicOptionsInfoByIndex", &CyGlobalContext::getGraphicOptionsInfoByIndex, python::return_value_policy<python::reference_existing_object>(), "(GraphicOptionsInfoID) - GraphicOptionsInfo for GraphicOptionsInfo")

		.def("getNumHurryInfos", &CyGlobalContext::getNumHurryInfos, "() - Total Hurry Infos")

		.def("getNumConceptInfos", &CyGlobalContext::getNumConceptInfos, "int () - NumConceptInfos")
		.def("getConceptInfo", &CyGlobalContext::getConceptInfo, python::return_value_policy<python::reference_existing_object>(), "Concept Info () - Returns info object")

		.def("getNumNewConceptInfos", &CyGlobalContext::getNumNewConceptInfos, "int () - NumNewConceptInfos")
		.def("getNewConceptInfo", &CyGlobalContext::getNewConceptInfo, python::return_value_policy<python::reference_existing_object>(), "New Concept Info () - Returns info object")

		.def("getNumCityTabInfos", &CyGlobalContext::getNumCityTabInfos, "int () - Returns NumCityTabInfos")
		.def("getCityTabInfo", &CyGlobalContext::getCityTabInfo, python::return_value_policy<python::reference_existing_object>(), "CityTabInfo - () - Returns Info object")

		.def("getNumCalendarInfos", &CyGlobalContext::getNumCalendarInfos, "int () - Returns NumCalendarInfos")
		.def("getCalendarInfo", &CyGlobalContext::getCalendarInfo, python::return_value_policy<python::reference_existing_object>(), "CalendarInfo () - Returns Info object")
		 
		.def("getNumGameOptionInfos", &CyGlobalContext::getNumGameOptionInfos, "int () - Returns NumGameOptionInfos")
		.def("getGameOptionInfo", &CyGlobalContext::getGameOptionInfo, python::return_value_policy<python::reference_existing_object>(), "GameOptionInfo () - Returns Info object")

		.def("getNumMPOptionInfos", &CyGlobalContext::getNumMPOptionInfos, "int () - Returns NumMPOptionInfos")
		.def("getMPOptionInfo", &CyGlobalContext::getMPOptionInfo, python::return_value_policy<python::reference_existing_object>(), "MPOptionInfo () - Returns Info object")

		.def("getNumForceControlInfos", &CyGlobalContext::getNumForceControlInfos, "int () - Returns NumForceControlInfos")
		.def("getForceControlInfo", &CyGlobalContext::getForceControlInfo, python::return_value_policy<python::reference_existing_object>(), "ForceControlInfo () - Returns Info object")

		.def("getNumSeasonInfos", &CyGlobalContext::getNumSeasonInfos, "int () - Returns NumSeasonInfos")
		.def("getSeasonInfo", &CyGlobalContext::getSeasonInfo, python::return_value_policy<python::reference_existing_object>(), "SeasonInfo () - Returns Info object")

		.def("getNumMonthInfos", &CyGlobalContext::getNumMonthInfos, "int () - Returns NumMonthInfos")
		.def("getMonthInfo", &CyGlobalContext::getMonthInfo, python::return_value_policy<python::reference_existing_object>(), "MonthInfo () - Returns Info object")

		.def("getNumDenialInfos", &CyGlobalContext::getNumDenialInfos, "int () - Returns NumDenialInfos")
		.def("getDenialInfo", &CyGlobalContext::getDenialInfo, python::return_value_policy<python::reference_existing_object>(), "DenialInfo () - Returns Info object")

		.def("getNumMissionInfos", &CyGlobalContext::getNumMissionInfos, "() - Total Mission Infos XML\\Units\\CIV4MissionInfos.xml")
		.def("getMissionInfo", &CyGlobalContext::getMissionInfo, python::return_value_policy<python::reference_existing_object>(), "(MissionID) - CvInfo for MissionID")

		.def("getNumAutomateInfos", &CyGlobalContext::getNumAutomateInfos, "() - Total Automate Infos XML\\Units\\CIV4AutomateInfos.xml")
		.def("getAutomateInfo", &CyGlobalContext::getAutomateInfo, python::return_value_policy<python::reference_existing_object>(), "(AutomateID) - CvInfo for AutomateID")

		.def("getNumCommandInfos", &CyGlobalContext::getNumCommandInfos, "() - Total Command Infos XML\\Units\\CIV4CommandInfos.xml")
		.def("getCommandInfo", &CyGlobalContext::getCommandInfo, python::return_value_policy<python::reference_existing_object>(), "(CommandID) - CvInfo for CommandID")

		.def("getNumControlInfos", &CyGlobalContext::getNumControlInfos, "() - Total Control Infos XML\\Units\\CIV4ControlInfos.xml")
		.def("getControlInfo", &CyGlobalContext::getControlInfo, python::return_value_policy<python::reference_existing_object>(), "(ControlID) - CvInfo for ControlID")

		.def("getNumPromotionInfos", &CyGlobalContext::getNumPromotionInfos, "() - Total Promotion Infos XML\\Units\\CIV4PromotionInfos.xml")
		.def("getPromotionInfo", &CyGlobalContext::getPromotionInfo, python::return_value_policy<python::reference_existing_object>(), "(PromotionID) - CvInfo for PromotionID")

		// MOD - START - Aid
		.def("getNumAidInfos", &CyGlobalContext::getNumAidInfos, "() - Total Aid Infos XML\\Units\\CIV4AidInfos.xml")
		.def("getAidInfo", &CyGlobalContext::getAidInfo, python::return_value_policy<python::reference_existing_object>(), "(AidID) - CvInfo for AidID")
		// MOD - END - Aid

		// MOD - START - Detriments
		.def("getNumDetrimentInfos", &CyGlobalContext::getNumDetrimentInfos, "() - Total Detriment Infos XML\\Units\\CIV4DetrimentInfos.xml")
		.def("getDetrimentInfo", &CyGlobalContext::getDetrimentInfo, python::return_value_policy<python::reference_existing_object>(), "(DetrimentID) - CvInfo for DetrimentID")
		// MOD - END - Detriments

		// MOD - START - Congestion
		.def("getNumCongestionInfos", &CyGlobalContext::getNumCongestionInfos, "() - Total Congestion Infos XML\\Units\\CIV4CongestionInfos.xml")
		.def("getCongestionInfo", &CyGlobalContext::getCongestionInfo, python::return_value_policy<python::reference_existing_object>(), "(CongestionID) - CvInfo for CongestionID")
		// MOD - END - Congestion
		;
}
