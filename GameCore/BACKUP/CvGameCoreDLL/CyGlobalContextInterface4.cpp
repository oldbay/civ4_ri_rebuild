//
// published python interface for CyGlobalContext
// Author - Mustafa Thamer
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


void CyGlobalContextPythonInterface4(python::class_<CyGlobalContext>& x)
{
	OutputDebugString("Python Extension Module - CyGlobalContextPythonInterface1\n");

	x
		.def("getNumTechInfos", &CyGlobalContext::getNumTechInfos, "() - Total Technology Infos XML\\Technologies\\CIV4TechInfos.xml")
		.def("getTechInfo", &CyGlobalContext::getTechInfo, python::return_value_policy<python::reference_existing_object>(), "(TechID) - CvInfo for TechID")

		.def("getNumSpecialBuildingInfos", &CyGlobalContext::getNumSpecialBuildingInfos, "() - Total Special Building Infos")
		.def("getSpecialBuildingInfo", &CyGlobalContext::getSpecialBuildingInfo, python::return_value_policy<python::reference_existing_object>(), "(SpecialBuildingID) - CvInfo for SpecialBuildingID")

		.def("getNumReligionInfos", &CyGlobalContext::getNumReligionInfos, "() - Total Religion Infos XML\\GameInfo\\CIV4ReligionInfos.xml")
		.def("getReligionInfo", &CyGlobalContext::getReligionInfo, python::return_value_policy<python::reference_existing_object>(), "(ReligionID) - CvInfo for ReligionID")

		.def("getNumCorporationInfos", &CyGlobalContext::getNumCorporationInfos, "() - Total Religion Infos XML\\GameInfo\\CIV4CorporationInfos.xml")
		.def("getCorporationInfo", &CyGlobalContext::getCorporationInfo, python::return_value_policy<python::reference_existing_object>(), "(CorporationID) - CvInfo for CorporationID")

		.def("getNumVictoryInfos", &CyGlobalContext::getNumVictoryInfos, "() - Total Victory Infos XML\\GameInfo\\CIV4VictoryInfos.xml")
		.def("getVictoryInfo", &CyGlobalContext::getVictoryInfo, python::return_value_policy<python::reference_existing_object>(), "(VictoryID) - CvInfo for VictoryID")

		.def("getNumSpecialistInfos", &CyGlobalContext::getNumSpecialistInfos, "() - Total Specialist Infos XML\\Units\\CIV4SpecialistInfos.xml")
		.def("getSpecialistInfo", &CyGlobalContext::getSpecialistInfo, python::return_value_policy<python::reference_existing_object>(), "(SpecialistID) - CvInfo for SpecialistID")

		.def("getNumCivicOptionInfos", &CyGlobalContext::getNumCivicOptionInfos, "() - Total Civic Infos XML\\Misc\\CIV4CivicOptionInfos.xml")
		.def("getCivicOptionInfo", &CyGlobalContext::getCivicOptionInfo, python::return_value_policy<python::reference_existing_object>(), "(CivicID) - CvInfo for CivicID")

		.def("getNumCivicInfos", &CyGlobalContext::getNumCivicInfos, "() - Total Civic Infos XML\\Misc\\CIV4CivicInfos.xml")
		.def("getCivicInfo", &CyGlobalContext::getCivicInfo, python::return_value_policy<python::reference_existing_object>(), "(CivicID) - CvInfo for CivicID")

		.def("getNumDiplomacyInfos", &CyGlobalContext::getNumDiplomacyInfos, "() - Total diplomacy Infos XML\\GameInfo\\CIV4DiplomacyInfos.xml")
		.def("getDiplomacyInfo", &CyGlobalContext::getDiplomacyInfo, python::return_value_policy<python::reference_existing_object>(), "(DiplomacyID) - CvInfo for DiplomacyID")

		.def("getNumProjectInfos", &CyGlobalContext::getNumProjectInfos, "() - Total Project Infos XML\\GameInfo\\CIV4ProjectInfos.xml")
		.def("getProjectInfo", &CyGlobalContext::getProjectInfo, python::return_value_policy<python::reference_existing_object>(), "(ProjectID) - CvInfo for ProjectID")

		.def("getNumVoteInfos", &CyGlobalContext::getNumVoteInfos, "() - Total VoteInfos")
		.def("getVoteInfo", &CyGlobalContext::getVoteInfo, python::return_value_policy<python::reference_existing_object>(), "(VoteID) - CvInfo for VoteID")

		.def("getNumProcessInfos", &CyGlobalContext::getNumProcessInfos, "() - Total ProcessInfos")
		.def("getProcessInfo", &CyGlobalContext::getProcessInfo, python::return_value_policy<python::reference_existing_object>(), "(ProcessID) - CvInfo for ProcessID")

		.def("getNumEmphasizeInfos", &CyGlobalContext::getNumEmphasizeInfos, "() - Total EmphasizeInfos")
		.def("getEmphasizeInfo", &CyGlobalContext::getEmphasizeInfo, python::return_value_policy<python::reference_existing_object>(), "(EmphasizeID) - CvInfo for EmphasizeID")

		.def("getHurryInfo", &CyGlobalContext::getHurryInfo, python::return_value_policy<python::reference_existing_object>(), "(HurryID) - CvInfo for HurryID")

		.def("getUnitAIInfo", &CyGlobalContext::getUnitAIInfo, python::return_value_policy<python::reference_existing_object>(), "UnitAIInfo (int id)")

		.def("getColorInfo", &CyGlobalContext::getColorInfo, python::return_value_policy<python::reference_existing_object>(), "ColorInfo (int id)")

		.def("getInfoTypeForString", &CyGlobalContext::getInfoTypeForString, "int (string) - returns the info index with the matching type string")
		.def("getTypesEnum", &CyGlobalContext::getTypesEnum, "int (string) - returns the type enum from a type string")

		.def("getNumPlayerColorInfos", &CyGlobalContext::getNumPlayerColorInfos, "int () - Returns number of PlayerColorInfos")
		.def("getPlayerColorInfo", &CyGlobalContext::getPlayerColorInfo, python::return_value_policy<python::reference_existing_object>(), "PlayerColorInfo (int id)")

		.def("getNumQuestInfos", &CyGlobalContext::getNumQuestInfos, "int () - Returns number of QuestInfos")
		.def("getQuestInfo", &CyGlobalContext::getQuestInfo, python::return_value_policy<python::reference_existing_object>(), "QuestInfo () - Returns info object")

		.def("getNumTutorialInfos", &CyGlobalContext::getNumTutorialInfos, "int () - Returns number of TutorialInfos")
		.def("getTutorialInfo", &CyGlobalContext::getTutorialInfo, python::return_value_policy<python::reference_existing_object>(), "TutorialInfo () - Returns info object")

		// MOD - START - Core Game Events
		.def("getNumGameEventInfos", &CyGlobalContext::getNumGameEventInfos, "int () - Returns number of GameEventInfos")
		.def("getGameEventInfo", &CyGlobalContext::getGameEventInfo, python::return_value_policy<python::reference_existing_object>(), "CvGameEventInfo (int GameEventID) - Returns info object")
		// MOD - END - Core Game Events

		// MOD - START - Population Metrics
		.def("getNumContributionInfos", &CyGlobalContext::getNumContributionInfos, "int () - Returns number of ContributionInfos")
		.def("getContributionInfo", &CyGlobalContext::getContributionInfo, python::return_value_policy<python::reference_existing_object>(), "CvInfoBase (int ContributionID) - Returns info object")

		.def("getNumSocialStabilityInfos", &CyGlobalContext::getNumSocialStabilityInfos, "int () - Returns number of SocialStabilityInfos")
		.def("getSocialStabilityInfo", &CyGlobalContext::getSocialStabilityInfo, python::return_value_policy<python::reference_existing_object>(), "CvSocialStabilityInfo (int SocialStabilityID) - Returns info object")

		.def("getNumMetricClassInfos", &CyGlobalContext::getNumMetricClassInfos, "int () - Returns number of MetricClassInfos")
		.def("getMetricClassInfo", &CyGlobalContext::getMetricClassInfo, python::return_value_policy<python::reference_existing_object>(), "CvMetricClassInfo (int MetricClassID) - Returns info object")

		.def("getNumMetricInfos", &CyGlobalContext::getNumMetricInfos, "int () - Returns number of MetricInfos")
		.def("getMetricInfo", &CyGlobalContext::getMetricInfo, python::return_value_policy<python::reference_existing_object>(), "CvMetricInfo (int MetricID) - Returns info object")

		.def("getNumMetricFactorInfos", &CyGlobalContext::getNumMetricFactorInfos, "int () - Returns number of MetricFactorInfos")
		.def("getMetricFactorInfo", &CyGlobalContext::getMetricFactorInfo, python::return_value_policy<python::reference_existing_object>(), "CvMetricFactorInfo (int MetricFactorID) - Returns info object")
		// MOD - END - Population Metrics

		.def("getNumEventTriggerInfos", &CyGlobalContext::getNumEventTriggerInfos, "int () - Returns number of EventTriggerInfos")
		.def("getEventTriggerInfo", &CyGlobalContext::getEventTriggerInfo, python::return_value_policy<python::reference_existing_object>(), "EventTriggerInfo () - Returns info object")

		.def("getNumEventInfos", &CyGlobalContext::getNumEventInfos, "int () - Returns number of EventInfos")
		.def("getEventInfo", &CyGlobalContext::getEventInfo, python::return_value_policy<python::reference_existing_object>(), "EventInfo () - Returns info object")

		.def("getNumEspionageMissionInfos", &CyGlobalContext::getNumEspionageMissionInfos, "int () - Returns number of EspionageMissionInfos")
		.def("getEspionageMissionInfo", &CyGlobalContext::getEspionageMissionInfo, python::return_value_policy<python::reference_existing_object>(), "EspionageMissionInfo () - Returns info object")

		.def("getNumHints", &CyGlobalContext::getNumHints, "int () - Returns number of Hints")
		.def("getHints", &CyGlobalContext::getHints, python::return_value_policy<python::reference_existing_object>(), "Hints () - Returns info object")

		// MOD - START - Improved Civilopedia
		.def("getNumHotKeyClassInfos", &CyGlobalContext::getNumHotKeyClassInfos, "int () - Returns number of Hotkey Classes")
		.def("getHotKeyClassInfo", &CyGlobalContext::getHotKeyClassInfo, python::return_value_policy<python::reference_existing_object>(), "HotKeyClassInfo () - Returns info object")
		// MOD - END - Improved Civilopedia

		.def("getNumMainMenus", &CyGlobalContext::getNumMainMenus, "int () - Returns number")
		.def("getMainMenus", &CyGlobalContext::getMainMenus, python::return_value_policy<python::reference_existing_object>(), "MainMenus () - Returns info object")

		.def("getNumVoteSourceInfos", &CyGlobalContext::getNumVoteSourceInfos, "int ()")
		.def("getVoteSourceInfo", &CyGlobalContext::getVoteSourceInfo, python::return_value_policy<python::reference_existing_object>(), "Returns info object")

		.def("getNumVoteSourceInfos", &CyGlobalContext::getNumVoteSourceInfos, "int ()")
		.def("getVoteSourceInfo", &CyGlobalContext::getVoteSourceInfo, python::return_value_policy<python::reference_existing_object>(), "Returns info object")

		// ArtInfos
		.def("getNumInterfaceArtInfos", &CyGlobalContext::getNumInterfaceArtInfos, "() - Total InterfaceArtnology Infos XML\\InterfaceArtnologies\\CIV4InterfaceArtInfos.xml")
		.def("getInterfaceArtInfo", &CyGlobalContext::getInterfaceArtInfo, python::return_value_policy<python::reference_existing_object>(), "(InterfaceArtID) - CvArtInfo for InterfaceArtID")

		.def("getNumMovieArtInfos", &CyGlobalContext::getNumMovieArtInfos, "() - Total MovieArt Infos XML\\MovieArtInfos\\CIV4ArtDefines.xml")
		.def("getMovieArtInfo", &CyGlobalContext::getMovieArtInfo, python::return_value_policy<python::reference_existing_object>(), "(MovieArtID) - CvArtInfo for MovieArtID")

		.def("getNumMiscArtInfos", &CyGlobalContext::getNumMiscArtInfos, "() - Total MiscArtnology Infos XML\\MiscArt\\CIV4MiscArtInfos.xml")
		.def("getMiscArtInfo", &CyGlobalContext::getMiscArtInfo, python::return_value_policy<python::reference_existing_object>(), "(MiscArtID) - CvArtInfo for MiscArtID")

		.def("getNumUnitArtInfos", &CyGlobalContext::getNumUnitArtInfos, "() - Total UnitArtnology Infos XML\\UnitArt\\CIV4UnitArtInfos.xml")
		.def("getUnitArtInfo", &CyGlobalContext::getUnitArtInfo, python::return_value_policy<python::reference_existing_object>(), "(UnitID) - CvArtInfo for UnitID")

		.def("getNumBuildingArtInfos", &CyGlobalContext::getNumBuildingArtInfos, "int () - Returns number of BuildingArtInfos")
		.def("getBuildingArtInfo", &CyGlobalContext::getBuildingArtInfo, python::return_value_policy<python::reference_existing_object>(), "(BuildingID) - CvArtInfo for BuildingID")

		.def("getNumCivilizationArtInfos", &CyGlobalContext::getNumCivilizationArtInfos, "int () - Returns number of CivilizationArtInfos")
		.def("getCivilizationArtInfo", &CyGlobalContext::getCivilizationArtInfo, python::return_value_policy<python::reference_existing_object>(), "(CivilizationID) - CvArtInfo for CivilizationID")

		.def("getNumLeaderheadArtInfos", &CyGlobalContext::getNumLeaderheadArtInfos, "int () - Returns number of LeaderHeadArtInfos")
		.def("getLeaderheadArtInfo", &CyGlobalContext::getLeaderheadArtInfo, python::return_value_policy<python::reference_existing_object>(), "(LeaderheadID) - CvArtInfo for LeaderheadID")

		.def("getNumBonusArtInfos", &CyGlobalContext::getNumBonusArtInfos, "int () - Returns number of BonusArtInfos")
		.def("getBonusArtInfo", &CyGlobalContext::getBonusArtInfo, python::return_value_policy<python::reference_existing_object>(), "BonusArtInfo () - Returns info object")

		.def("getNumImprovementArtInfos", &CyGlobalContext::getNumImprovementArtInfos, "int () - Returns number of ImprovementArtInfos")
		.def("getImprovementArtInfo", &CyGlobalContext::getImprovementArtInfo, python::return_value_policy<python::reference_existing_object>(), "ImprovementArtInfo () - Returns info object")

		.def("getNumTerrainArtInfos", &CyGlobalContext::getNumTerrainArtInfos, "int () - Returns number of TerrainArtInfos")
		.def("getTerrainArtInfo", &CyGlobalContext::getTerrainArtInfo, python::return_value_policy<python::reference_existing_object>(), "TerrainArtInfo () - Returns info object")

		.def("getNumFeatureArtInfos", &CyGlobalContext::getNumFeatureArtInfos, "int () - Returns number of FeatureArtInfos")
		.def("getFeatureArtInfo", &CyGlobalContext::getFeatureArtInfo, python::return_value_policy<python::reference_existing_object>(), "FeatureArtInfo () - Returns info object")

		// Types
		.def("getNumEntityEventTypes", &CyGlobalContext::getNumEntityEventTypes, "int () - Returns number of EntityEventTypes")
		.def("getEntityEventType", &CyGlobalContext::getEntityEventTypes, "string () - Returns enum string")

		.def("getNumAnimationOperatorTypes", &CyGlobalContext::getNumAnimationOperatorTypes, "int () - Returns number of AnimationOperatorTypes")
		.def("getAnimationOperatorTypes", &CyGlobalContext::getAnimationOperatorTypes, "string () - Returns enum string")

		.def("getFunctionTypes", &CyGlobalContext::getFunctionTypes, "string () - Returns enum string")

		.def("getNumArtStyleTypes", &CyGlobalContext::getNumArtStyleTypes, "int () - Returns number of ArtStyleTypes")
		.def("getArtStyleTypes", &CyGlobalContext::getArtStyleTypes, "string () - Returns enum string")

		.def("getNumFlavorTypes", &CyGlobalContext::getNumFlavorTypes, "int () - Returns number of FlavorTypes")
		.def("getFlavorTypes", &CyGlobalContext::getFlavorTypes, "string () - Returns enum string")

		.def("getNumUnitArtStyleTypeInfos", &CyGlobalContext::getNumUnitArtStyleTypeInfos, "int () - Returns number of UnitArtStyleTypes")
		.def("getUnitArtStyleTypeInfo", &CyGlobalContext::getUnitArtStyleTypeInfo, python::return_value_policy<python::reference_existing_object>(), "(UnitArtStyleTypeID) - CvInfo for UnitArtStyleTypeID")

		.def("getNumCitySizeTypes", &CyGlobalContext::getNumCitySizeTypes, "int () - Returns number of CitySizeTypes")
		.def("getCitySizeTypes", &CyGlobalContext::getCitySizeTypes, "string () - Returns enum string")

		.def("getContactTypes", &CyGlobalContext::getContactTypes, "string () - Returns enum string")

		.def("getDiplomacyPowerTypes", &CyGlobalContext::getDiplomacyPowerTypes, "string () - Returns enum string")

		// MOD - START - Asset Files
		.def("findAssetFile", &CyGlobalContext::findAssetFile, "bool (string) - Returns a bool indicating whether the asset exists or not")
		// MOD - END - Asset Files

		// MOD - START - Graphical Paging
		.def("setGraphicOptionGraphicalPaging", &CyGlobalContext::setGraphicOptionGraphicalPaging, "(bool) - Sets the Graphical Paging graphics option")
		.def("getGraphicOption", &CyGlobalContext::getGraphicOption, "bool () - Gets graphics options, including the extended option for graphical paging")
		// MOD - END - Graphical Paging
		;
}
