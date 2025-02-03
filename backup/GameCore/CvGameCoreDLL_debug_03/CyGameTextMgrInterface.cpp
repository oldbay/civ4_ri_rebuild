#include "CvGameCoreDLL.h"
#include "CyGameTextMgr.h"
#include "CyDeal.h" //PORT NEW
#include "CyUnit.h" //PORT NEW

//
// published python interface for CyGameTextMgr
//

void CyGameTextMgrInterface()
{
    #if defined(__GNUC__)
    std::clog << "Python Extension Module - CyTextMgr\n";
    #else
    OutputDebugString("Python Extension Module - CyTextMgr\n");
    #endif

	python::class_<CyGameTextMgr>("CyGameTextMgr")
		.def("isNone", &CyGameTextMgr::isNone, "bool () - Checks to see if pointer points to a real object")

		.def("Reset", &CyGameTextMgr::Reset, "void ()")
		.def("getTimeStr", &CyGameTextMgr::getTimeStr, "wstring (int iGameTurn, bool bSave)")
		.def("getDateStr", &CyGameTextMgr::getDateStr, "wstring (int iGameTurn, bool bSave, int /*CalendarTypes*/ eCalendar, int iStartYear, int /*GameSpeedTypes*/ eSpeed)")
		.def("getInterfaceTimeStr", &CyGameTextMgr::getInterfaceTimeStr, "wstring (int /*PlayerTypes*/ iPlayer)")
		.def("getGoldStr", &CyGameTextMgr::getGoldStr, "wstring (int /*PlayerTypes*/ iPlayer)")
		.def("getResearchStr", &CyGameTextMgr::getResearchStr, "wstring (int /*PlayerTypes*/ iPlayer)")
		.def("getOOSSeeds", &CyGameTextMgr::getOOSSeeds, "wstring (int /*PlayerTypes*/ iPlayer)")
		.def("getNetStats", &CyGameTextMgr::getNetStats, "wstring (int /*PlayerTypes*/ iPlayer)")
		.def("getTechHelp", &CyGameTextMgr::getTechHelp, "wstring (int iTech, bool bCivilopediaText, bool bPlayerContext, bool bStrategyText, bool bTreeInfo, int iFromTech)")
		.def("getUnitHelp", &CyGameTextMgr::getUnitHelp, "wstring (int iUnit, bool bCivilopediaText, bool bStrategyText, bool bTechChooserText, CyCity* pCity)")
		.def("getSpecificUnitHelp", &CyGameTextMgr::getSpecificUnitHelp, "wstring (CyUnit* pUnit, bool bOneLine, bool bShort)")
		.def("getBuildingHelp", &CyGameTextMgr::getBuildingHelp, "wstring (int iBuilding, bool bCivilopediaText, bool bStrategyText, bool bTechChooserText, CyCity* pCity)")
		.def("getProjectHelp", &CyGameTextMgr::getProjectHelp, "wstring (int iProject, bool bCivilopediaText, CyCity* pCity)")
		.def("getPromotionHelp", &CyGameTextMgr::getPromotionHelp, "wstring (int iPromotion, bool bCivilopediaText)")
		// MOD - START - Aid
		.def("getAidHelp", &CyGameTextMgr::getAidHelp, "wstring (int iAid, bool bCivilopediaText)")
		// MOD - END - Aid
		// MOD - START - Detriments
		.def("getDetrimentHelp", &CyGameTextMgr::getDetrimentHelp, "wstring (int iDetriment, bool bCivilopediaText)")
		// MOD - END - Detriments
		.def("getBonusHelp", &CyGameTextMgr::getBonusHelp, "wstring (int iBonus, bool bCivilopediaText)")
		.def("getReligionHelpCity", &CyGameTextMgr::getReligionHelpCity, "wstring (int iReligion, CyCity* pCity, bool bCityScreen, bool bForceReligion, bool bForceState)")
		.def("getCorporationHelpCity", &CyGameTextMgr::getCorporationHelpCity, "wstring (int iCorporation, CyCity* pCity, bool bCityScreen, bool bForceCorporation)")
		// MOD - START - Improved Civilopedia
		//.def("getImprovementHelp", &CyGameTextMgr::getImprovementHelp, "wstring (int iImprovement, bool bCivilopediaText)")
		.def("getImprovementHelp", &CyGameTextMgr::getImprovementHelp, "wstring (int iImprovement, bool bCivilopediaEffects, bool bCivilopediaImprovements)")
		// MOD - END - Improved Civilopedia
		.def("getTerrainHelp", &CyGameTextMgr::getTerrainHelp, "wstring (int iTerrain, bool bCivilopediaText)")
		.def("getFeatureHelp", &CyGameTextMgr::getFeatureHelp, "wstring (int iFeature, bool bCivilopediaText)")
		// MOD - START - Improved Civilopedia
		.def("getRouteHelp", &CyGameTextMgr::getRouteHelp, "wstring (int iRoute, bool bCivilopediaText)")
		.def("getWorldSizeHelp", &CyGameTextMgr::getWorldSizeHelp, "wstring (int iWorld, bool bCivilopediaText)")
		.def("getHandicapHelp", &CyGameTextMgr::getHandicapHelp, "wstring (int iHandicap, bool bCivilopediaText)")
		.def("getConceptHelp", &CyGameTextMgr::getConceptHelp, "wstring (int iConcept, bool bCivilopediaText)")
		.def("getHotKeyClassHelp", &CyGameTextMgr::getHotKeyClassHelp, "wstring (int iHotKeyClass, bool bCivilopediaText)")
		// MOD - END - Improved Civilopedia
		// MOD - START - Era Effects
		.def("getEraHelp", &CyGameTextMgr::getEraHelp, "wstring (int iEra, bool bSplash, bool bCivilopediaText)")
		// MOD - END - Era Effects
		.def("parseCivicInfo", &CyGameTextMgr::parseCivicInfo, "wstring (int /*CivicTypes*/ iCivicType, bool bCivilopediaText, bool bPlayerContext, bool bSkipName)")
		.def("parseReligionInfo", &CyGameTextMgr::parseReligionInfo, "wstring (int /*ReligionTypes*/ iReligionType, bool bCivilopediaText)")
		.def("parseCorporationInfo", &CyGameTextMgr::parseCorporationInfo, "wstring (int /*CorporationTypes*/ iCorporationType, bool bCivilopediaText)")
		.def("parseCivInfos", &CyGameTextMgr::parseCivInfos, "wstring (int /*CivilizationTypes*/ iCivilization, bool bDawnOfMan)")
		.def("parseLeaderTraits", &CyGameTextMgr::parseLeaderTraits, "wstring (int /*LeaderHeadTypes*/ iLeader, int /*CivilizationTypes*/ iCivilization, bool bDawnOfMan, bool bCivilopediaText)")
		// MOD - START - Improved Civilopedia
		.def("parseTraits", &CyGameTextMgr::parseTraits, "wstring (int /*TraitTypes*/ iTrait, int /*CivilizationTypes*/ iCivilization, bool bDawnOfMan, bool bCivilopediaText)")
		// MOD - END - Improved Civilopedia
		.def("getSpecialistHelp", &CyGameTextMgr::getSpecialistHelp, "wstring (TradeData* pTradeData, int iPlayer1, int iPlayer2)")
		.def("getTradeString", &CyGameTextMgr::getTradeString, "wstring (int iSpecialist, bool bCivilopediaText)")
		.def("buildHintsList", &CyGameTextMgr::buildHintsList, "wstring ()")
		.def("getAttitudeString", &CyGameTextMgr::getAttitudeString, "wstring (int iPlayer, int iTargetPlayer)")
		.def("setConvertHelp", &CyGameTextMgr::setConvertHelp, "wstring (int iPlayer, int iReligion)")
		.def("setRevolutionHelp", &CyGameTextMgr::setRevolutionHelp, "wstring (int iPlayer)")
		.def("setVassalRevoltHelp", &CyGameTextMgr::setVassalRevoltHelp, "wstring (int iMaster, int iVassal)")
		.def("getActiveDealsString", &CyGameTextMgr::getActiveDealsString, "wstring (int iThisPlayer, int iOtherPlayer)")
		.def("getDealString", &CyGameTextMgr::getDealString, "wstring (CyDeal* pDeal, int iPlayerPerspective)")
	;
}
