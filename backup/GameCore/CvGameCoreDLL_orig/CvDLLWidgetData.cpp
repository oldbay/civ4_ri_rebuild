#include "CvGameCoreDLL.h"
#include "CvGameCoreUtils.h"
#include "CvCity.h"
#include "CvUnit.h"
#include "CvGlobals.h"
#include "CvGameAI.h"
#include "CvMap.h"
#include "CvPlot.h"
#include "CvTeamAI.h"
#include "CvPlayerAI.h"
#include "CvGameCoreUtils.h"
#include "CyArgsList.h"
#include "CvGameTextMgr.h"
#include "CvDLLInterfaceIFaceBase.h"
#include "CvDLLPythonIFaceBase.h"
#include "CvDLLEngineIFaceBase.h"
#include "CvEventReporter.h"
#include "CvDLLWidgetData.h"
#include "CvPopupInfo.h"
#include "FProfiler.h"
#include "CvMessageControl.h"
#include "CvBugOptions.h"

CvDLLWidgetData* CvDLLWidgetData::m_pInst = NULL;

CvDLLWidgetData& CvDLLWidgetData::getInstance()
{
	if (m_pInst == NULL)
	{
		m_pInst = new CvDLLWidgetData;
	}
	return *m_pInst;
}

void CvDLLWidgetData::freeInstance()
{
	delete m_pInst;
	m_pInst = NULL;
}

void CvDLLWidgetData::parseHelp(CvWStringBuffer &szBuffer, CvWidgetDataStruct &widgetDataStruct)
{
	switch (widgetDataStruct.m_eWidgetType)
	{
	case WIDGET_PLOT_LIST:
		parsePlotListHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PLOT_LIST_SHIFT:
		szBuffer.assign(gDLL->getText("TXT_KEY_MISC_CTRL_SHIFT", (GC.getDefineINT("MAX_PLOT_LIST_SIZE") - 1)));
		break;

	case WIDGET_CITY_SCROLL:
		break;

	case WIDGET_LIBERATE_CITY:
		parseLiberateCityHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_CITY_NAME:
		parseCityNameHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_UNIT_NAME:
		szBuffer.append(gDLL->getText("TXT_KEY_CHANGE_NAME"));
		break;

	case WIDGET_CREATE_GROUP:
		szBuffer.append(gDLL->getText("TXT_KEY_WIDGET_CREATE_GROUP"));
		break;

	case WIDGET_DELETE_GROUP:
		szBuffer.append(gDLL->getText("TXT_KEY_WIDGET_DELETE_GROUP"));
		break;

	case WIDGET_TRAIN:
		parseTrainHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_CONSTRUCT:
		parseConstructHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_CREATE:
		parseCreateHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_MAINTAIN:
		parseMaintainHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HURRY:
		parseHurryHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_MENU_ICON:
		szBuffer.append(gDLL->getText("TXT_KEY_MAIN_MENU"));

	case WIDGET_CONSCRIPT:
		parseConscriptHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_ACTION:
		parseActionHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_CITIZEN:
		parseCitizenHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_FREE_CITIZEN:
		parseFreeCitizenHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_DISABLED_CITIZEN:
		parseDisabledCitizenHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_ANGRY_CITIZEN:
		parseAngryCitizenHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_CHANGE_SPECIALIST:
		parseChangeSpecialistHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_RESEARCH:
		parseResearchHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_TECH_TREE:
		parseTechTreeHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_CHANGE_PERCENT:
		parseChangePercentHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_CITY_TAB:
		{
			CvWString szTemp;
            szTemp.Format(L"%s", GC.getCityTabInfo((CityTabTypes)widgetDataStruct.m_iData1).getDescription());
			szBuffer.assign(szTemp);
		}
		break;

	case WIDGET_CONTACT_CIV:
		parseContactCivHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_SCORE_BREAKDOWN:
		parseScoreHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_ZOOM_CITY:
		szBuffer.append(gDLL->getText("TXT_KEY_ZOOM_CITY_HELP"));
		break;

	case WIDGET_END_TURN:
		szBuffer.append(gDLL->getText("TXT_KEY_WIDGET_END_TURN"));
		break;
		
	case WIDGET_LAUNCH_VICTORY:
		szBuffer.append(gDLL->getText("TXT_KEY_WIDGET_LAUNCH_VICTORY"));
		break;

	case WIDGET_AUTOMATE_CITIZENS:
		parseAutomateCitizensHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_AUTOMATE_PRODUCTION:
		parseAutomateProductionHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_EMPHASIZE:
		parseEmphasizeHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_TRADE_ITEM:
		parseTradeItem(widgetDataStruct, szBuffer);
		break;

	case WIDGET_UNIT_MODEL:
		parseUnitModelHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_FLAG:
		parseFlagHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_MAINTENANCE:
		parseMaintenanceHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_RELIGION:
		parseReligionHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_RELIGION_CITY:
		parseReligionHelpCity(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_CORPORATION_CITY:
		parseCorporationHelpCity(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_NATIONALITY:
		parseNationalityHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_DEFENSE:
		break;

	case WIDGET_HELP_HEALTH:
		parseHealthHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_HAPPINESS:
		parseHappinessHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_POPULATION:
		parsePopulationHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_PRODUCTION:
		parseProductionHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_CULTURE:
		parseCultureHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_GREAT_PEOPLE:
		parseGreatPeopleHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_GREAT_GENERAL:
		parseGreatGeneralHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_SELECTED:
		parseSelectedHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_BUILDING:
		parseBuildingHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_TRADE_ROUTE_CITY:
		parseTradeRouteCityHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_ESPIONAGE_COST:
		parseEspionageCostHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_TECH_ENTRY:
		parseTechEntryHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_TECH_PREPREQ:
		parseTechPrereqHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_OBSOLETE:
		parseObsoleteHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_OBSOLETE_BONUS:
		parseObsoleteBonusString(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_OBSOLETE_SPECIAL:
		parseObsoleteSpecialHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_MOVE_BONUS:
		parseMoveHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_FREE_UNIT:
		parseFreeUnitHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_FEATURE_PRODUCTION:
		parseFeatureProductionHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_WORKER_RATE:
		parseWorkerRateHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_TRADE_ROUTES:
		parseTradeRouteHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_HEALTH_RATE:
		parseHealthRateHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_HAPPINESS_RATE:
		parseHappinessRateHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_FREE_TECH:
		parseFreeTechHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_LOS_BONUS:
		parseLOSHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_MAP_CENTER:
		parseMapCenterHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_MAP_REVEAL:
		parseMapRevealHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_MAP_TRADE:
		parseMapTradeHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_TECH_TRADE:
		parseTechTradeHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_GOLD_TRADE:
		parseGoldTradeHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_OPEN_BORDERS:
		parseOpenBordersHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_DEFENSIVE_PACT:
		parseDefensivePactHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_PERMANENT_ALLIANCE:
		parsePermanentAllianceHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_VASSAL_STATE:
		parseVassalStateHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_BUILD_BRIDGE:
		parseBuildBridgeHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_IRRIGATION:
		parseIrrigationHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_IGNORE_IRRIGATION:
		parseIgnoreIrrigationHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_WATER_WORK:
		parseWaterWorkHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_IMPROVEMENT:
		parseBuildHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_DOMAIN_EXTRA_MOVES:
		parseDomainExtraMovesHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_ADJUST:
		parseAdjustHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_TERRAIN_TRADE:
		parseTerrainTradeHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_SPECIAL_BUILDING:
		parseSpecialBuildingHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_YIELD_CHANGE:
		parseYieldChangeHelp(widgetDataStruct, szBuffer);
		break;

	// MOD - START - Advanced Yield and Commerce Effects
	case WIDGET_HELP_IRRIGATED_YIELD_CHANGE:
		parseIrrigatedYieldChangeHelp(widgetDataStruct, szBuffer);
		break;
	// MOD - END - Advanced Yield and Commerce Effects

	case WIDGET_HELP_BONUS_REVEAL:
		parseBonusRevealHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_CIVIC_REVEAL:
		parseCivicRevealHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_PROCESS_INFO:
		parseProcessInfoHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_FOUND_RELIGION:
		parseFoundReligionHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_FOUND_CORPORATION:
		parseFoundCorporationHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_FINANCE_NUM_UNITS:
		parseFinanceNumUnits(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_FINANCE_UNIT_COST:
		parseFinanceUnitCost(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_FINANCE_AWAY_SUPPLY:
		parseFinanceAwaySupply(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_FINANCE_CITY_MAINT:
		parseFinanceCityMaint(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_FINANCE_CIVIC_UPKEEP:
		parseFinanceCivicUpkeep(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_FINANCE_FOREIGN_INCOME:
		parseFinanceForeignIncome(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_FINANCE_INFLATED_COSTS:
		parseFinanceInflatedCosts(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_FINANCE_GROSS_INCOME:
		parseFinanceGrossIncome(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_FINANCE_NET_GOLD:
		parseFinanceNetGold(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_FINANCE_GOLD_RESERVE:
		parseFinanceGoldReserve(widgetDataStruct, szBuffer);
		break;

	case WIDGET_CLOSE_SCREEN:
		//parseCloseScreenHelp(szBuffer);
		break;

	case WIDGET_DEAL_KILL:
		parseKillDealHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_MINIMAP_HIGHLIGHT:
		break;

	case WIDGET_PRODUCTION_MOD_HELP:
		parseProductionModHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_LEADERHEAD:
		parseLeaderheadHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_LEADER_LINE:
		parseLeaderLineHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_COMMERCE_MOD_HELP:
		parseCommerceModHelp(widgetDataStruct, szBuffer);
		break;

	// MOD - START - Epidemics
	// Epidemic system by Mexico
	case WIDGET_HELP_EPIDEMIC_PROBABILITY:
		parseEpidemicHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_EPIDEMIC_PROBABILITY_GLOBAL_MODIFIER:
		parseEpidemicGlobalModifierHelp(widgetDataStruct, szBuffer);
		break;
	// MOD - END - Epidemics

	case WIDGET_HELP_AHEAD:
		parseAhead(widgetDataStruct, szBuffer);
		break;

	// MOD - START - Congestion
	case WIDGET_CONGESTION_CONTEXT:
		parseCongestionContextHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_CONGESTION_PREVIOUS_LEVEL:
		parseCongestionPreviousLevelHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_CONGESTION_CURRENT_LEVEL:
		parseCongestionCurrentLevelHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_CONGESTION_NEXT_LEVEL:
		parseCongestionNextLevelHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_CONGESTION_BAR:
		parseCongestionBarHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_CONGESTION_INSIDE_CITY_THRESHOLD_CHANGE:
		parseCongestionInsideCityThresholdChangeHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_CONGESTION_OUTSIDE_CITY_THRESHOLD_CHANGE:
		parseCongestionOutsideCityThresholdChangeHelp(widgetDataStruct, szBuffer);
		break;
	// MOD - END - Congestion

	// MOD - START - Population Metrics
	case WIDGET_HELP_CITY_STABILITY:
		parseCityStabilityHelp(widgetDataStruct, szBuffer);
		break;
	// MOD - END - Population Metrics

	// MOD - START - Improved Civilopedia
	case WIDGET_PEDIA_MAIN:
		break;

	case WIDGET_PEDIA_SECTION:
		break;

	case WIDGET_PEDIA_VIEW:
		break;

	case WIDGET_PEDIA_JUMP_TO_TECH:
		parseTechEntryHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_JUMP_TO_REQUIRED_TECH:
		parseTechTreePrereq(widgetDataStruct, szBuffer, false);
		break;

	case WIDGET_PEDIA_JUMP_TO_DERIVED_TECH:
		parseTechTreePrereq(widgetDataStruct, szBuffer, true);
		break;

	case WIDGET_PEDIA_JUMP_TO_UNIT:
		parseUnitHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_JUMP_TO_BUILDING:
		parseBuildingHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_BACK:
		// parsePediaBack(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_FORWARD:
		// parsePediaForward(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_JUMP_TO_BONUS:
		parseBonusHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_JUMP_TO_PROMOTION:
	case WIDGET_HELP_PROMOTION:
		parsePromotionHelp(widgetDataStruct, szBuffer);
		break;

	// MOD - START - Aid
	case WIDGET_PEDIA_JUMP_TO_AID:
		parseAidHelp(widgetDataStruct, szBuffer);
		break;
	// MOD - END - Aid

	// MOD - START - Detriments
	case WIDGET_PEDIA_JUMP_TO_DETRIMENT:
		parseDetrimentHelp(widgetDataStruct, szBuffer);
		break;
	// MOD - END - Detriments

	case WIDGET_CHOOSE_EVENT:
		parseEventHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_JUMP_TO_UNIT_COMBAT:
		parseUnitCombatHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_JUMP_TO_IMPROVEMENT:
		parseImprovementHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_JUMP_TO_CIVIC:
		parseCivicHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_JUMP_TO_CIV:
		parseCivilizationHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_JUMP_TO_LEADER:
		parseLeaderHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_JUMP_TO_TRAIT:
		parseTraitHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_JUMP_TO_SPECIALIST:
		if (widgetDataStruct.m_iData1 != NO_SPECIALIST && widgetDataStruct.m_iData2 != 0)
		{
			CvWString szTemp;
			szTemp.Format(L"%s", GC.getSpecialistInfo((SpecialistTypes)widgetDataStruct.m_iData1).getDescription());
			szBuffer.assign(szTemp);
		}
		break;

	case WIDGET_PEDIA_JUMP_TO_PROJECT:
		parseProjectHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_JUMP_TO_RELIGION:
		parseReligionHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_JUMP_TO_CORPORATION:
		parseCorporationHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_JUMP_TO_TERRAIN:
		parseTerrainHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_JUMP_TO_FEATURE:
		parseFeatureHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_JUMP_TO_ROUTE:
		parseRouteHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_JUMP_TO_WORLD:
		parseWorldSizeHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_JUMP_TO_HANDICAP:
		parseHandicapHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_JUMP_TO_ERA:
		parseEraHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_JUMP_TO_CONCEPT:
		parseConceptHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_JUMP_TO_HOTKEY_CLASS:
		parseHotKeyClassHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_DESCRIPTION:
		parseDescriptionHelp(widgetDataStruct, szBuffer, false);
		break;

	case WIDGET_PEDIA_DESCRIPTION_NO_HELP:
		//parseDescriptionHelp(widgetDataStruct, szBuffer, true);
		break;
	// MOD - END - Improved Civilopedia
	// K-Mod. Environmental advisor widgets.
	case WIDGET_HELP_POLLUTION_OFFSETS:
		parsePollutionOffsetsHelp(widgetDataStruct, szBuffer);
		break;
	case WIDGET_HELP_POLLUTION_SOURCE:
		parsePollutionHelp(widgetDataStruct, szBuffer);
		break;
	case WIDGET_HELP_SUSTAINABILITY_THRESHOLD:
		szBuffer.assign(gDLL->getText("TXT_KEY_SUSTAINABILITY_THRESHOLD_HELP"));
		break;
	case WIDGET_HELP_GW_RELATIVE_CONTRIBUTION:
		szBuffer.assign(gDLL->getText("TXT_KEY_GW_RELATIVE_CONTRIBUTION_HELP"));
		break;
	case WIDGET_HELP_GW_INDEX:
		szBuffer.assign(gDLL->getText("TXT_KEY_GW_INDEX_HELP"));
		break;
	case WIDGET_HELP_GW_UNHAPPY:
		szBuffer.assign(gDLL->getText("TXT_KEY_GW_UNHAPPY_HELP"));
		break;
	// K-Mod. Extra specialist commerce
	case WIDGET_HELP_GLOBAL_COMMERCE_MODIFIER:
		GAMETEXT.setCommerceChangeHelp(szBuffer, L"", L"", gDLL->getText("TXT_KEY_CIVIC_IN_ALL_CITIES").GetCString(), GC.getTechInfo((TechTypes)(widgetDataStruct.m_iData1)).getCommerceModifierArray(), true, false);
		break;
	// MOD - START - Advanced Yield and Commerce Effects
	/*
	case WIDGET_HELP_EXTRA_SPECIALIST_COMMERCE:
		GAMETEXT.setCommerceChangeHelp(szBuffer, L"", L"", gDLL->getText("TXT_KEY_CIVIC_PER_SPECIALIST").GetCString(), GC.getTechInfo((TechTypes)(widgetDataStruct.m_iData1)).getSpecialistExtraCommerceArray(), false, false);
		break;
	*/
	case WIDGET_HELP_EXTRA_SPECIALIST_COMMERCE:
		GAMETEXT.setCommerceChangeHelp(szBuffer, L"", L"", gDLL->getText("TXT_KEY_CIVIC_PER_SPECIALIST").GetCString(), GC.getTechInfo((TechTypes)(widgetDataStruct.m_iData1)).getAllSpecialistCommerceArray(), false, false);
		break;
	// MOD - END - Advanced Yield and Commerce Effects
	//K-Mod end
	}
}

// Protected Functions...
bool CvDLLWidgetData::executeAction( CvWidgetDataStruct &widgetData )
{
	bool bHandled = false;			//	Right now general bHandled = false;  We can specific case this to true later.  Game will run with this = false;

	switch (widgetData.m_eWidgetType)
	{

	case WIDGET_PLOT_LIST:
		doPlotList(widgetData);
		break;

	case WIDGET_PLOT_LIST_SHIFT:
		gDLL->getInterfaceIFace()->changePlotListColumn(widgetData.m_iData1 * ((GC.ctrlKey()) ? (GC.getDefineINT("MAX_PLOT_LIST_SIZE") - 1) : 1));
		break;

	case WIDGET_CITY_SCROLL:
		if ( widgetData.m_iData1 > 0 )
		{
			GC.getGameINLINE().doControl(CONTROL_NEXTCITY);
		}
		else
		{
			GC.getGameINLINE().doControl(CONTROL_PREVCITY);
		}
		break;

	case WIDGET_LIBERATE_CITY:
		doLiberateCity();
		break;

	case WIDGET_CITY_NAME:
		doRenameCity();
		break;

	case WIDGET_UNIT_NAME:
		doRenameUnit();
		break;

	case WIDGET_CREATE_GROUP:
		doCreateGroup();
		break;

	case WIDGET_DELETE_GROUP:
		doDeleteGroup();
		break;

	case WIDGET_TRAIN:
		doTrain(widgetData);
		break;

	case WIDGET_CONSTRUCT:
		doConstruct(widgetData);
		break;

	case WIDGET_CREATE:
		doCreate(widgetData);
		break;

	case WIDGET_MAINTAIN:
		doMaintain(widgetData);
		break;

	case WIDGET_HURRY:
		doHurry(widgetData);
		break;

	case WIDGET_MENU_ICON:
		doMenu();

	case WIDGET_CONSCRIPT:
		doConscript();
		break;

	case WIDGET_ACTION:
		doAction(widgetData);
		break;

	case WIDGET_CITIZEN:
		break;

	case WIDGET_FREE_CITIZEN:
		break;

	case WIDGET_DISABLED_CITIZEN:
		break;

	case WIDGET_ANGRY_CITIZEN:
		break;

	case WIDGET_CHANGE_SPECIALIST:
		doChangeSpecialist(widgetData);
		break;

	case WIDGET_RESEARCH:
	case WIDGET_TECH_TREE:
		doResearch(widgetData);
		break;

	case WIDGET_CHANGE_PERCENT:
		doChangePercent(widgetData);
		break;

	case WIDGET_CITY_TAB:
		doCityTab(widgetData);
		break;

	case WIDGET_CONTACT_CIV:
		doContactCiv(widgetData);
		break;

	case WIDGET_END_TURN:
		GC.getGameINLINE().doControl(CONTROL_FORCEENDTURN);
		break;

	case WIDGET_LAUNCH_VICTORY:
		doLaunch(widgetData);
		break;

	case WIDGET_CONVERT:
		doConvert(widgetData);
		break;

	case WIDGET_REVOLUTION:
		// handled in Python
		break;

	case WIDGET_AUTOMATE_CITIZENS:
		doAutomateCitizens();
		break;

	case WIDGET_AUTOMATE_PRODUCTION:
		doAutomateProduction();
		break;

	case WIDGET_EMPHASIZE:
		doEmphasize(widgetData);
		break;

	case WIDGET_DIPLOMACY_RESPONSE:
		// CLEANUP -- PD
//		GC.getDiplomacyScreen().handleClick(m_pWidget);
		break;

	case WIDGET_TRADE_ITEM:
		break;

	case WIDGET_UNIT_MODEL:
		doUnitModel();
		break;

	case WIDGET_FLAG:
		doFlag();
		break;

	case WIDGET_HELP_SELECTED:
		doSelected(widgetData);
		break;

	case WIDGET_TURN_EVENT:
		doGotoTurnEvent(widgetData);
		break;

	case WIDGET_FOREIGN_ADVISOR:
		doForeignAdvisor(widgetData);
		break;

	case WIDGET_DEAL_KILL:
		doDealKill(widgetData);
		break;

	case WIDGET_MINIMAP_HIGHLIGHT:
		doRefreshMilitaryAdvisor(widgetData);
		break;

	case WIDGET_CHOOSE_EVENT:
		break;

	case WIDGET_ZOOM_CITY:
		break;

	// MOD - START - Population Advisor
	case WIDGET_HELP_CITY_STABILITY:
		doPopulationAdvisor();
		break;
	// MOD - END - Population Advisor

	// MOD - START - Population Advisor
	case WIDGET_POPULATION_ADVISOR:
		doPopulationAdvisor(widgetData);
		break;
	// MOD - END - Population Advisor

	// MOD - START - Improved Civilopedia
	case WIDGET_PEDIA_BACK:
		doPediaBack();
		break;

	case WIDGET_PEDIA_FORWARD:
		doPediaForward();
		break;

	case WIDGET_PEDIA_CLEAR_SEARCH_FILTER:
		doPediaClearSearchFilter();
		break;

	case WIDGET_PEDIA_TOGGLE_PLAYABLE_FILTER:
		doPediaTogglePlayableFilter();
		break;

	case WIDGET_PEDIA_MAIN:
		doPediaAction(NO_CIVILOPEDIA_SECTION);
		break;

	case WIDGET_PEDIA_SECTION:
		doPediaAction((CivilopediaSectionTypes)widgetData.m_iData1);
		break;

	case WIDGET_PEDIA_VIEW:
		doPediaAction((CivilopediaViewTypes)widgetData.m_iData1, widgetData.m_iData2);
		break;

	case WIDGET_PEDIA_JUMP_TO_TECH:
	case WIDGET_PEDIA_JUMP_TO_REQUIRED_TECH:
	case WIDGET_PEDIA_JUMP_TO_DERIVED_TECH:
		doPediaPage(CIVILOPEDIA_PAGE_TECHNOLOGY, widgetData);
		break;

	case WIDGET_PEDIA_JUMP_TO_UNIT:
		doPediaPage(CIVILOPEDIA_PAGE_UNIT, widgetData);
		break;

	case WIDGET_PEDIA_JUMP_TO_UNIT_COMPARISON:
		doPediaPage(CIVILOPEDIA_PAGE_UNIT_COMPARISON, widgetData);
		break;

	case WIDGET_PEDIA_JUMP_TO_UNIT_COMBAT:
		doPediaPage(CIVILOPEDIA_PAGE_UNIT_COMBAT, widgetData);
		break;

	case WIDGET_PEDIA_JUMP_TO_UNIT_UPGRADE_CHART:
		doPediaPage(CIVILOPEDIA_PAGE_UNIT_UPGRADE_CHART, widgetData);
		break;

	case WIDGET_PEDIA_JUMP_TO_PROMOTION:
		doPediaPage(CIVILOPEDIA_PAGE_PROMOTION, widgetData);
		break;

	case WIDGET_PEDIA_JUMP_TO_PROMOTION_TREE:
		doPediaPage(CIVILOPEDIA_PAGE_PROMOTION_TREE, widgetData);
		break;

	// MOD - START - Aid
	case WIDGET_PEDIA_JUMP_TO_AID:
		doPediaPage(CIVILOPEDIA_PAGE_AID, widgetData);
		break;
	// MOD - END - Aid

	// MOD - START - Detriments
	case WIDGET_PEDIA_JUMP_TO_DETRIMENT:
		doPediaPage(CIVILOPEDIA_PAGE_DETRIMENT, widgetData);
		break;
	// MOD - END - Detriments

	case WIDGET_PEDIA_JUMP_TO_BUILDING:
		doPediaPage(CIVILOPEDIA_PAGE_BUILDING, widgetData);
		break;

	case WIDGET_PEDIA_JUMP_TO_BUILDING_COMPARISON:
		doPediaPage(CIVILOPEDIA_PAGE_BUILDING_COMPARISON, widgetData);
		break;

	case WIDGET_PEDIA_JUMP_TO_BUILDING_CLASS:
		doPediaPage(CIVILOPEDIA_PAGE_BUILDING_CLASS, widgetData);
		break;

	case WIDGET_PEDIA_JUMP_TO_BUILDING_UPGRADE_CHART:
		doPediaPage(CIVILOPEDIA_PAGE_BUILDING_UPGRADE_CHART, widgetData);
		break;

	case WIDGET_PEDIA_JUMP_TO_PROJECT:
		doPediaPage(CIVILOPEDIA_PAGE_PROJECT, widgetData);
		break;

	case WIDGET_PEDIA_JUMP_TO_CIV:
		doPediaPage(CIVILOPEDIA_PAGE_CIVILIZATION, widgetData);
		break;

	case WIDGET_PEDIA_JUMP_TO_LEADER:
		doPediaPage(CIVILOPEDIA_PAGE_LEADER, widgetData);
		break;

	case WIDGET_PEDIA_JUMP_TO_TRAIT:
		doPediaPage(CIVILOPEDIA_PAGE_TRAIT, widgetData);
		break;

	case WIDGET_PEDIA_JUMP_TO_CIVIC:
		doPediaPage(CIVILOPEDIA_PAGE_CIVIC, widgetData);
		break;

	case WIDGET_PEDIA_JUMP_TO_RELIGION:
		doPediaPage(CIVILOPEDIA_PAGE_RELIGION, widgetData);
		break;

	case WIDGET_PEDIA_JUMP_TO_CORPORATION:
		doPediaPage(CIVILOPEDIA_PAGE_CORPORATION, widgetData);
		break;

	case WIDGET_PEDIA_JUMP_TO_SPECIALIST:
		doPediaPage(CIVILOPEDIA_PAGE_SPECIALIST, widgetData);
		break;

	case WIDGET_PEDIA_JUMP_TO_ERA:
		doPediaPage(CIVILOPEDIA_PAGE_ERA, widgetData);
		break;

	case WIDGET_PEDIA_JUMP_TO_TERRAIN:
		doPediaPage(CIVILOPEDIA_PAGE_TERRAIN, widgetData);
		break;

	case WIDGET_PEDIA_JUMP_TO_FEATURE:
		doPediaPage(CIVILOPEDIA_PAGE_FEATURE, widgetData);
		break;

	case WIDGET_PEDIA_JUMP_TO_BONUS:
		doPediaPage(CIVILOPEDIA_PAGE_BONUS, widgetData);
		break;

	case WIDGET_PEDIA_JUMP_TO_IMPROVEMENT:
		doPediaPage(CIVILOPEDIA_PAGE_IMPROVEMENT, widgetData);
		break;

	case WIDGET_PEDIA_JUMP_TO_WORLD:
		doPediaPage(CIVILOPEDIA_PAGE_WORLD, widgetData);
		break;

	case WIDGET_PEDIA_JUMP_TO_ROUTE:
		doPediaPage(CIVILOPEDIA_PAGE_ROUTE, widgetData);
		break;

	case WIDGET_PEDIA_JUMP_TO_HANDICAP:
		doPediaPage(CIVILOPEDIA_PAGE_HANDICAP, widgetData);
		break;

	case WIDGET_PEDIA_JUMP_TO_INTRODUCTION:
		doPediaPage(CIVILOPEDIA_PAGE_INTRODUCTION, widgetData);
		break;

	case WIDGET_PEDIA_JUMP_TO_CONCEPT:
		doPediaPage(CIVILOPEDIA_PAGE_CONCEPT, widgetData);
		break;

	case WIDGET_PEDIA_JUMP_TO_HINTS:
		doPediaPage(CIVILOPEDIA_PAGE_HINTS, widgetData);
		break;

	case WIDGET_PEDIA_JUMP_TO_HOTKEY_CLASS:
		doPediaPage(CIVILOPEDIA_PAGE_HOTKEY_CLASS, widgetData);
		break;

	case WIDGET_PEDIA_DESCRIPTION:
	case WIDGET_PEDIA_DESCRIPTION_NO_HELP:
		//doPediaDescription(widgetData);
		break;
	// MOD - END - Improved Civilopedia

	case WIDGET_HELP_TECH_PREPREQ:
	case WIDGET_HELP_OBSOLETE:
	case WIDGET_HELP_OBSOLETE_BONUS:
	case WIDGET_HELP_OBSOLETE_SPECIAL:
	case WIDGET_HELP_MOVE_BONUS:
	case WIDGET_HELP_FREE_UNIT:
	case WIDGET_HELP_FEATURE_PRODUCTION:
	case WIDGET_HELP_WORKER_RATE:
	case WIDGET_HELP_TRADE_ROUTES:
	case WIDGET_HELP_HEALTH_RATE:
	case WIDGET_HELP_HAPPINESS_RATE:
	case WIDGET_HELP_FREE_TECH:
	case WIDGET_HELP_LOS_BONUS:
	case WIDGET_HELP_MAP_CENTER:
	case WIDGET_HELP_MAP_REVEAL:
	case WIDGET_HELP_MAP_TRADE:
	case WIDGET_HELP_TECH_TRADE:
	case WIDGET_HELP_GOLD_TRADE:
	case WIDGET_HELP_OPEN_BORDERS:
	case WIDGET_HELP_DEFENSIVE_PACT:
	case WIDGET_HELP_PERMANENT_ALLIANCE:
	case WIDGET_HELP_VASSAL_STATE:
	case WIDGET_HELP_BUILD_BRIDGE:
	case WIDGET_HELP_IRRIGATION:
	case WIDGET_HELP_IGNORE_IRRIGATION:
	case WIDGET_HELP_WATER_WORK:
	case WIDGET_HELP_IMPROVEMENT:
	case WIDGET_HELP_DOMAIN_EXTRA_MOVES:
	case WIDGET_HELP_ADJUST:
	case WIDGET_HELP_TERRAIN_TRADE:
	case WIDGET_HELP_SPECIAL_BUILDING:
	case WIDGET_HELP_YIELD_CHANGE:
	// MOD - START - Advanced Yield and Commerce Effects
	case WIDGET_HELP_IRRIGATED_YIELD_CHANGE:
	// MOD - END - Advanced Yield and Commerce Effects
	case WIDGET_HELP_BONUS_REVEAL:
	case WIDGET_HELP_CIVIC_REVEAL:
	case WIDGET_HELP_PROCESS_INFO:
	case WIDGET_HELP_FINANCE_NUM_UNITS:
	case WIDGET_HELP_FINANCE_UNIT_COST:
	case WIDGET_HELP_FINANCE_AWAY_SUPPLY:
	case WIDGET_HELP_FINANCE_CITY_MAINT:
	case WIDGET_HELP_FINANCE_CIVIC_UPKEEP:
	case WIDGET_HELP_FINANCE_FOREIGN_INCOME:
	case WIDGET_HELP_FINANCE_INFLATED_COSTS:
	case WIDGET_HELP_FINANCE_GROSS_INCOME:
	case WIDGET_HELP_FINANCE_NET_GOLD:
	case WIDGET_HELP_FINANCE_GOLD_RESERVE:
	case WIDGET_HELP_RELIGION_CITY:
	case WIDGET_HELP_CORPORATION_CITY:
	case WIDGET_HELP_PROMOTION:
	case WIDGET_LEADERHEAD:
	case WIDGET_LEADER_LINE:
	case WIDGET_CLOSE_SCREEN:
	case WIDGET_SCORE_BREAKDOWN:
		//	Nothing on clicked
		break;
	}

	return bHandled;
}

//	right clicking action
bool CvDLLWidgetData::executeAltAction( CvWidgetDataStruct &widgetData )
{
	// MOD - START - Improved Civilopedia
	bool bHandled = true;
	switch (widgetData.m_eWidgetType)
	{
	// K-Mod
	case WIDGET_CHANGE_PERCENT:
		doChangePercentAlt(widgetData);
		break;
	// K-Mod end
	case WIDGET_HELP_TECH_ENTRY:
	case WIDGET_HELP_TECH_PREPREQ:
	case WIDGET_RESEARCH:
	case WIDGET_TECH_TREE:
		doPediaPage(CIVILOPEDIA_PAGE_TECHNOLOGY, widgetData);
		break;
	case WIDGET_TRAIN:
		doPediaAction(CIVILOPEDIA_PAGE_UNIT, GC.getCivilizationInfo(GC.getGameINLINE().getActiveCivilizationType()).getCivilizationUnits(widgetData.m_iData1));
		break;
	case WIDGET_PEDIA_JUMP_TO_UNIT:
	case WIDGET_HELP_FREE_UNIT:
		doPediaPage(CIVILOPEDIA_PAGE_UNIT, widgetData);
		break;
	case WIDGET_PEDIA_JUMP_TO_PROMOTION:
		doPediaPage(CIVILOPEDIA_PAGE_PROMOTION, widgetData);
		break;
	// MOD - START - Aid
	case WIDGET_PEDIA_JUMP_TO_AID:
		doPediaPage(CIVILOPEDIA_PAGE_AID, widgetData);
		break;
	// MOD - END - Aid
	// MOD - START - Detriments
	case WIDGET_PEDIA_JUMP_TO_DETRIMENT:
		doPediaPage(CIVILOPEDIA_PAGE_DETRIMENT, widgetData);
		break;
	// MOD - END - Detriments
	case WIDGET_CONSTRUCT:
		doPediaAction(CIVILOPEDIA_PAGE_BUILDING, GC.getCivilizationInfo(GC.getGameINLINE().getActiveCivilizationType()).getCivilizationBuildings(widgetData.m_iData1));
		break;
	case WIDGET_PEDIA_JUMP_TO_BUILDING:
		doPediaPage(CIVILOPEDIA_PAGE_BUILDING, widgetData);
		break;
	case WIDGET_CREATE:
		doPediaPage(CIVILOPEDIA_PAGE_PROJECT, widgetData);
		break;
	case WIDGET_PEDIA_JUMP_TO_PROJECT:
		doPediaPage(CIVILOPEDIA_PAGE_PROJECT, widgetData);
		break;
	case WIDGET_HELP_FOUND_RELIGION:
		doPediaAction(CIVILOPEDIA_PAGE_RELIGION, widgetData.m_iData2);
		break;
	case WIDGET_PEDIA_JUMP_TO_RELIGION:
		doPediaPage(CIVILOPEDIA_PAGE_RELIGION, widgetData);
		break;
	case WIDGET_HELP_FOUND_CORPORATION:
		doPediaAction(CIVILOPEDIA_PAGE_CORPORATION, widgetData.m_iData2);
		break;
	case WIDGET_PEDIA_JUMP_TO_CORPORATION:
		doPediaPage(CIVILOPEDIA_PAGE_CORPORATION, widgetData);
		break;
	case WIDGET_HELP_OBSOLETE:
		doPediaPage(CIVILOPEDIA_PAGE_BUILDING, widgetData);
		break;
	case WIDGET_HELP_IMPROVEMENT:
		{
			ImprovementTypes eImprovement = NO_IMPROVEMENT;
			BuildTypes eBuild = (BuildTypes)widgetData.m_iData2;
			if (NO_BUILD != eBuild)
			{
				eImprovement = (ImprovementTypes)GC.getBuildInfo(eBuild).getImprovement();
			}
			if (NO_IMPROVEMENT != eImprovement)
			{
				doPediaAction(CIVILOPEDIA_PAGE_IMPROVEMENT, eImprovement);
			}
		}
		break;
	case WIDGET_HELP_YIELD_CHANGE:
	// MOD - START - Advanced Yield and Commerce Effects
	case WIDGET_HELP_IRRIGATED_YIELD_CHANGE:
	// MOD - END - Advanced Yield and Commerce Effects
		doPediaAction(CIVILOPEDIA_PAGE_IMPROVEMENT, widgetData.m_iData2);
		break;
	case WIDGET_HELP_BONUS_REVEAL:
	case WIDGET_HELP_OBSOLETE_BONUS:
		doPediaAction(CIVILOPEDIA_PAGE_BONUS, widgetData.m_iData2);
		break;
	case WIDGET_CITIZEN:
	case WIDGET_FREE_CITIZEN:
	case WIDGET_DISABLED_CITIZEN:
		doPediaPage(CIVILOPEDIA_PAGE_SPECIALIST, widgetData);
		break;
	case WIDGET_HELP_CIVIC_REVEAL:
		doPediaAction(CIVILOPEDIA_PAGE_CIVIC, widgetData.m_iData2);
		break;
	case WIDGET_LEADERHEAD:
		doContactCiv(widgetData);
		break;

	default:
		bHandled = false;
		break;
	}
	// MOD - END - Improved Civilopedia

	return (bHandled);
}

bool CvDLLWidgetData::isLink(const CvWidgetDataStruct &widgetDataStruct) const
{
	bool bLink = false;
	switch (widgetDataStruct.m_eWidgetType)
	{
	case WIDGET_PEDIA_JUMP_TO_TECH:
	case WIDGET_PEDIA_JUMP_TO_REQUIRED_TECH:
	case WIDGET_PEDIA_JUMP_TO_DERIVED_TECH:
	case WIDGET_PEDIA_JUMP_TO_BUILDING:
	case WIDGET_PEDIA_JUMP_TO_UNIT:
	case WIDGET_PEDIA_JUMP_TO_UNIT_COMBAT:
	case WIDGET_PEDIA_JUMP_TO_PROMOTION:
	// MOD - START - Aid
	case WIDGET_PEDIA_JUMP_TO_AID:
	// MOD - END - Aid
	// MOD - START - Detriments
	case WIDGET_PEDIA_JUMP_TO_DETRIMENT:
	// MOD - END - Detriments
	case WIDGET_PEDIA_JUMP_TO_BONUS:
	case WIDGET_PEDIA_JUMP_TO_IMPROVEMENT:
	case WIDGET_PEDIA_JUMP_TO_CIVIC:
	case WIDGET_PEDIA_JUMP_TO_CIV:
	case WIDGET_PEDIA_JUMP_TO_LEADER:
	case WIDGET_PEDIA_JUMP_TO_SPECIALIST:
	case WIDGET_PEDIA_JUMP_TO_PROJECT:
	case WIDGET_PEDIA_JUMP_TO_RELIGION:
	case WIDGET_PEDIA_JUMP_TO_CORPORATION:
	case WIDGET_PEDIA_JUMP_TO_TERRAIN:
	case WIDGET_PEDIA_JUMP_TO_FEATURE:
	case WIDGET_PEDIA_FORWARD:
	case WIDGET_PEDIA_BACK:
	case WIDGET_PEDIA_MAIN:
	case WIDGET_TURN_EVENT:
	case WIDGET_FOREIGN_ADVISOR:
	case WIDGET_PEDIA_DESCRIPTION:
	case WIDGET_PEDIA_DESCRIPTION_NO_HELP:
	case WIDGET_MINIMAP_HIGHLIGHT:
		bLink = (widgetDataStruct.m_iData1 >= 0);
		break;
	case WIDGET_DEAL_KILL:
		{
			CvDeal* pDeal = GC.getGameINLINE().getDeal(widgetDataStruct.m_iData1);
			bLink = (NULL != pDeal && pDeal->isCancelable(GC.getGameINLINE().getActivePlayer()));
		}
		break;
	case WIDGET_CONVERT:
		bLink = (0 != widgetDataStruct.m_iData2);
		break;
	case WIDGET_GENERAL:
	case WIDGET_REVOLUTION:
		bLink = (1 == widgetDataStruct.m_iData1);
		break;
	}
	return (bLink);
}


void CvDLLWidgetData::doPlotList(CvWidgetDataStruct &widgetDataStruct)
{
	PROFILE_FUNC();

	CvUnit* pUnit;
	bool bWasCityScreenUp;

	int iUnitIndex = widgetDataStruct.m_iData1 + gDLL->getInterfaceIFace()->getPlotListColumn() - gDLL->getInterfaceIFace()->getPlotListOffset();

	CvPlot *selectionPlot = gDLL->getInterfaceIFace()->getSelectionPlot();
	pUnit = gDLL->getInterfaceIFace()->getInterfacePlotUnit(selectionPlot, iUnitIndex);

	if (pUnit != NULL)
	{
		if (pUnit->getOwnerINLINE() == GC.getGameINLINE().getActivePlayer())
		{
			bWasCityScreenUp = gDLL->getInterfaceIFace()->isCityScreenUp();

			//gDLL->getInterfaceIFace()->selectGroup(pUnit, gDLL->shiftKey(), gDLL->ctrlKey(), gDLL->altKey());
			gDLL->getInterfaceIFace()->selectGroup(pUnit, GC.shiftKey(), GC.ctrlKey() || GC.altKey(), GC.altKey()); // K-Mod

			if (bWasCityScreenUp)
			{
				gDLL->getInterfaceIFace()->lookAtSelectionPlot();
			}
		}
	}
}


void CvDLLWidgetData::doLiberateCity()
{
	GC.getGameINLINE().selectedCitiesGameNetMessage(GAMEMESSAGE_DO_TASK, TASK_LIBERATE, 0);

	gDLL->getInterfaceIFace()->clearSelectedCities();
}


void CvDLLWidgetData::doRenameCity()
{
	CvCity* pHeadSelectedCity;

	pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity != NULL)
	{
		if (pHeadSelectedCity->getOwnerINLINE() == GC.getGameINLINE().getActivePlayer())
		{
			CvEventReporter::getInstance().cityRename(pHeadSelectedCity);
		}
	}
}


void CvDLLWidgetData::doRenameUnit()
{
	CvUnit* pHeadSelectedUnit;

	pHeadSelectedUnit = gDLL->getInterfaceIFace()->getHeadSelectedUnit();

	if (pHeadSelectedUnit != NULL)
	{
		if (pHeadSelectedUnit->getOwnerINLINE() == GC.getGameINLINE().getActivePlayer())
		{
			CvEventReporter::getInstance().unitRename(pHeadSelectedUnit);
		}
	}
}


void CvDLLWidgetData::doCreateGroup()
{
	GC.getGameINLINE().selectionListGameNetMessage(GAMEMESSAGE_JOIN_GROUP);
}


void CvDLLWidgetData::doDeleteGroup()
{
	GC.getGameINLINE().selectionListGameNetMessage(GAMEMESSAGE_JOIN_GROUP, -1, -1, -1, 0, false, true);
}


void CvDLLWidgetData::doTrain(CvWidgetDataStruct &widgetDataStruct)
{
	UnitTypes eUnit;

	eUnit = ((UnitTypes)(GC.getCivilizationInfo(GC.getGameINLINE().getActiveCivilizationType()).getCivilizationUnits(widgetDataStruct.m_iData1)));

	if (widgetDataStruct.m_iData2 != FFreeList::INVALID_INDEX)
	{
		//CvMessageControl::getInstance().sendPushOrder(widgetDataStruct.m_iData2, ORDER_TRAIN, eUnit, false, false, false);
		CvMessageControl::getInstance().sendPushOrder(widgetDataStruct.m_iData2, ORDER_TRAIN, eUnit, false, true, 0); // K-Mod
	}
	else
	{
		GC.getGameINLINE().selectedCitiesGameNetMessage(GAMEMESSAGE_PUSH_ORDER, ORDER_TRAIN, eUnit, -1, false, GC.altKey(), GC.shiftKey(), GC.ctrlKey());
	}

	gDLL->getInterfaceIFace()->setCityTabSelectionRow(CITYTAB_UNITS);
}


void CvDLLWidgetData::doConstruct(CvWidgetDataStruct &widgetDataStruct)
{
	BuildingTypes eBuilding;

	eBuilding = ((BuildingTypes)(GC.getCivilizationInfo(GC.getGameINLINE().getActiveCivilizationType()).getCivilizationBuildings(widgetDataStruct.m_iData1)));

	if (widgetDataStruct.m_iData2 != FFreeList::INVALID_INDEX)
	{
		//CvMessageControl::getInstance().sendPushOrder(widgetDataStruct.m_iData2, ORDER_CONSTRUCT, eBuilding, false, false, false);
		CvMessageControl::getInstance().sendPushOrder(widgetDataStruct.m_iData2, ORDER_CONSTRUCT, eBuilding, false, true, 0); // K-Mod
	}
	else
	{
		GC.getGameINLINE().selectedCitiesGameNetMessage(GAMEMESSAGE_PUSH_ORDER, ORDER_CONSTRUCT, eBuilding, -1, false, GC.altKey(), GC.shiftKey(), GC.ctrlKey());
	}

	if (isLimitedWonderClass((BuildingClassTypes)(widgetDataStruct.m_iData1)))
	{
		gDLL->getInterfaceIFace()->setCityTabSelectionRow(CITYTAB_WONDERS);
	}
	else
	{
		gDLL->getInterfaceIFace()->setCityTabSelectionRow(CITYTAB_BUILDINGS);
	}
}


void CvDLLWidgetData::doCreate(CvWidgetDataStruct &widgetDataStruct)
{
	if (widgetDataStruct.m_iData2 != FFreeList::INVALID_INDEX)
	{
		//CvMessageControl::getInstance().sendPushOrder(widgetDataStruct.m_iData2, ORDER_CREATE, widgetDataStruct.m_iData1, false, false, false);
		CvMessageControl::getInstance().sendPushOrder(widgetDataStruct.m_iData2, ORDER_CREATE, widgetDataStruct.m_iData1, false, true, 0); // K-Mod
	}
	else
	{
		GC.getGameINLINE().selectedCitiesGameNetMessage(GAMEMESSAGE_PUSH_ORDER, ORDER_CREATE, widgetDataStruct.m_iData1, -1, false, GC.altKey(), GC.shiftKey(), GC.ctrlKey());
	}

	gDLL->getInterfaceIFace()->setCityTabSelectionRow(CITYTAB_WONDERS);
}


void CvDLLWidgetData::doMaintain(CvWidgetDataStruct &widgetDataStruct)
{
	if (widgetDataStruct.m_iData2 != FFreeList::INVALID_INDEX)
	{
		//CvMessageControl::getInstance().sendPushOrder(widgetDataStruct.m_iData2, ORDER_MAINTAIN, widgetDataStruct.m_iData1, false, false, false);
		CvMessageControl::getInstance().sendPushOrder(widgetDataStruct.m_iData2, ORDER_MAINTAIN, widgetDataStruct.m_iData1, false, true, 0); // K-Mod
	}
	else
	{
		GC.getGameINLINE().selectedCitiesGameNetMessage(GAMEMESSAGE_PUSH_ORDER, ORDER_MAINTAIN, widgetDataStruct.m_iData1, -1, false, GC.altKey(), GC.shiftKey(), GC.ctrlKey());
	}

	gDLL->getInterfaceIFace()->setCityTabSelectionRow(CITYTAB_WONDERS);
}


void CvDLLWidgetData::doHurry(CvWidgetDataStruct &widgetDataStruct)
{
	GC.getGameINLINE().selectedCitiesGameNetMessage(GAMEMESSAGE_DO_TASK, TASK_HURRY, widgetDataStruct.m_iData1);
}


void CvDLLWidgetData::doConscript()
{
	GC.getGameINLINE().selectedCitiesGameNetMessage(GAMEMESSAGE_DO_TASK, TASK_CONSCRIPT);
}


void CvDLLWidgetData::doAction(CvWidgetDataStruct &widgetDataStruct)
{
	GC.getGameINLINE().handleAction(widgetDataStruct.m_iData1);
}


void CvDLLWidgetData::doChangeSpecialist(CvWidgetDataStruct &widgetDataStruct)
{
	GC.getGameINLINE().selectedCitiesGameNetMessage(GAMEMESSAGE_DO_TASK, TASK_CHANGE_SPECIALIST, widgetDataStruct.m_iData1, widgetDataStruct.m_iData2);
}


void CvDLLWidgetData::doResearch(CvWidgetDataStruct &widgetDataStruct)
{
	// K-Mod
	/* original bts code
	bool bShift;

	bShift = GC.shiftKey();

	if (!bShift)
	{
		if ((GetKeyState(VK_LSHIFT) & 0x8000) || (GetKeyState(VK_RSHIFT) & 0x8000))
		{
			bShift = true;
		}
	} */
	bool bShift = GC.shiftKey();

/************************************************************************************************/
/* UNOFFICIAL_PATCH                       12/07/09                             EmperorFool      */
/*                                                                                              */
/* Bugfix                                                                                       */
/************************************************************************************************/
	// Free Tech Popup Fix
	if (widgetDataStruct.m_iData2 > 0)
	{
		CvPlayer& kPlayer = GET_PLAYER(GC.getGameINLINE().getActivePlayer());

		if (!kPlayer.isChoosingFreeTech())
		{
			gDLL->getInterfaceIFace()->addHumanMessage(GC.getGameINLINE().getActivePlayer(), true, GC.getEVENT_MESSAGE_TIME(), gDLL->getText("TXT_KEY_CHEATERS_NEVER_PROSPER"), NULL, MESSAGE_TYPE_MAJOR_EVENT);
			FAssertMsg(false, "doResearch called for free tech when !isChoosingFreeTech()");
			return;
		}
		else
		{
			// K-Mod
			//kPlayer.setChoosingFreeTech(false);
			kPlayer.changeChoosingFreeTechCount(-1);
		}
	}
/************************************************************************************************/
/* UNOFFICIAL_PATCH                        END                                                  */
/************************************************************************************************/

	CvMessageControl::getInstance().sendResearch(((TechTypes)widgetDataStruct.m_iData1), widgetDataStruct.m_iData2, bShift);
}


void CvDLLWidgetData::doChangePercent(CvWidgetDataStruct &widgetDataStruct)
{
	CvMessageControl::getInstance().sendPercentChange(((CommerceTypes)widgetDataStruct.m_iData1), widgetDataStruct.m_iData2);
}

// K-Mod. Right click on "change percent" buttons will set them to min / max.
void CvDLLWidgetData::doChangePercentAlt(CvWidgetDataStruct &widgetDataStruct)
{
	CvMessageControl::getInstance().sendPercentChange((CommerceTypes)widgetDataStruct.m_iData1, widgetDataStruct.m_iData2 * 100);
}
// K-Mod end

void CvDLLWidgetData::doCityTab(CvWidgetDataStruct &widgetDataStruct)
{
	gDLL->getInterfaceIFace()->setCityTabSelectionRow((CityTabTypes)widgetDataStruct.m_iData1);
}

void CvDLLWidgetData::doContactCiv(CvWidgetDataStruct &widgetDataStruct)
{
	if (gDLL->isDiplomacy() || gDLL->isMPDiplomacyScreenUp())
	{
		return;
	}

	//	Do not execute this if we are trying to contact ourselves...
	if (GC.getGameINLINE().getActivePlayer() == widgetDataStruct.m_iData1)
	{
		if (!gDLL->getInterfaceIFace()->isFocusedWidget())
		{
			gDLL->getInterfaceIFace()->toggleScoresMinimized();
		}

		return;
	}

/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      01/11/09                                jdog5000      */
/*                                                                                              */
/* Player Interface                                                                             */
/************************************************************************************************/
	if (GC.shiftKey() && !GC.altKey())
	{
		if (GET_PLAYER((PlayerTypes)widgetDataStruct.m_iData1).isHuman())
		{
			if (widgetDataStruct.m_iData1 != GC.getGameINLINE().getActivePlayer())
			{
				gDLL->getInterfaceIFace()->showTurnLog((ChatTargetTypes)widgetDataStruct.m_iData1);
			}
		}
		return;
	}

	if (GC.altKey())
	{
		TeamTypes eWidgetTeam = GET_PLAYER((PlayerTypes)widgetDataStruct.m_iData1).getTeam(); // K-Mod
		if( GC.shiftKey() )
		{
			// Warning: use of this is not multiplayer compatible
			// K-Mod, since this it isn't MP compatible, I'm going to disable it for the time being.
			if (false && !GC.getGameINLINE().isGameMultiPlayer())
			{
				if (GET_TEAM(GC.getGameINLINE().getActiveTeam()).canDeclareWar(eWidgetTeam))
				{
					if( GET_TEAM(GC.getGameINLINE().getActiveTeam()).AI_getWarPlan(eWidgetTeam) == WARPLAN_PREPARING_TOTAL) 
					{
						GET_TEAM(GC.getGameINLINE().getActiveTeam()).AI_setWarPlan(eWidgetTeam, NO_WARPLAN);
					}
					else
					{
						GET_TEAM(GC.getGameINLINE().getActiveTeam()).AI_setWarPlan(eWidgetTeam, WARPLAN_PREPARING_TOTAL);
					}
					gDLL->getInterfaceIFace()->setDirty(Score_DIRTY_BIT, true);
				}
			}
			// K-Mod end
		}
		else
		{
			if (GET_TEAM(GC.getGameINLINE().getActiveTeam()).canDeclareWar(eWidgetTeam))
			{
				/* original bts code
				CvMessageControl::getInstance().sendChangeWar(GET_PLAYER((PlayerTypes)widgetDataStruct.m_iData1).getTeam(), true); */
				// K-Mod. Give us a confirmation popup...
				CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_DECLAREWARMOVE);
				if (NULL != pInfo)
				{
					pInfo->setData1(eWidgetTeam);
					pInfo->setOption1(false); // shift key
					pInfo->setFlags(1); // don't do the "move" part of the declare-war-move.
					gDLL->getInterfaceIFace()->addPopup(pInfo);
				}
				// K-Mod end
			}
			else if (GET_TEAM(eWidgetTeam).isVassal(GC.getGameINLINE().getActiveTeam()))
			{
				CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_VASSAL_DEMAND_TRIBUTE, widgetDataStruct.m_iData1);
				if (pInfo)
				{
					gDLL->getInterfaceIFace()->addPopup(pInfo, GC.getGameINLINE().getActivePlayer(), true);
				}
			}
		}
		return;
	}
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/

	GET_PLAYER(GC.getGameINLINE().getActivePlayer()).contact((PlayerTypes)widgetDataStruct.m_iData1);
}

void CvDLLWidgetData::doConvert(CvWidgetDataStruct &widgetDataStruct)
{
	if (widgetDataStruct.m_iData2 != 0)
	{
		CvMessageControl::getInstance().sendConvert((ReligionTypes)(widgetDataStruct.m_iData1));
	}
}

void CvDLLWidgetData::doAutomateCitizens()
{
	CvCity* pHeadSelectedCity;

	pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity != NULL)
	{
		GC.getGameINLINE().selectedCitiesGameNetMessage(GAMEMESSAGE_DO_TASK, TASK_SET_AUTOMATED_CITIZENS, -1, -1, !(pHeadSelectedCity->isCitizensAutomated()));
	}
}

void CvDLLWidgetData::doAutomateProduction()
{
	CvCity* pHeadSelectedCity;

	pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity != NULL)
	{
		GC.getGameINLINE().selectedCitiesGameNetMessage(GAMEMESSAGE_DO_TASK, TASK_SET_AUTOMATED_PRODUCTION, -1, -1, !pHeadSelectedCity->isProductionAutomated(), GC.altKey(), GC.shiftKey(), GC.ctrlKey());
	}
}

void CvDLLWidgetData::doEmphasize(CvWidgetDataStruct &widgetDataStruct)
{
	CvCity* pHeadSelectedCity;

	pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity != NULL)
	{
		GC.getGameINLINE().selectedCitiesGameNetMessage(GAMEMESSAGE_DO_TASK, TASK_SET_EMPHASIZE, widgetDataStruct.m_iData1, -1, !(pHeadSelectedCity->AI_isEmphasize((EmphasizeTypes)(widgetDataStruct.m_iData1))));
	}
}

void CvDLLWidgetData::doUnitModel()
{
	if (gDLL->getInterfaceIFace()->isFocused())
	{
		//	Do NOT execute if a screen is up...
		return;
	}

	gDLL->getInterfaceIFace()->lookAtSelectionPlot();
}


void CvDLLWidgetData::doFlag()
{
	GC.getGameINLINE().doControl(CONTROL_SELECTCAPITAL);
}

void CvDLLWidgetData::doSelected(CvWidgetDataStruct &widgetDataStruct)
{
	CvCity* pHeadSelectedCity;

	pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity != NULL)
	{
		//GC.getGameINLINE().selectedCitiesGameNetMessage(GAMEMESSAGE_POP_ORDER, widgetDataStruct.m_iData1);
		GC.getGameINLINE().selectedCitiesGameNetMessage(GAMEMESSAGE_POP_ORDER, widgetDataStruct.m_iData1, -1, -1, false, GC.altKey(), GC.shiftKey(), GC.ctrlKey()); // K-Mod
	}
}

void CvDLLWidgetData::doGotoTurnEvent(CvWidgetDataStruct &widgetDataStruct)
{
	CvPlot* pPlot = GC.getMapINLINE().plotINLINE(widgetDataStruct.m_iData1, widgetDataStruct.m_iData2);

	if (NULL != pPlot && !gDLL->getEngineIFace()->isCameraLocked())
	{
		if (pPlot->isRevealed(GC.getGameINLINE().getActiveTeam(), false))
		{
			gDLL->getEngineIFace()->cameraLookAt(pPlot->getPoint());
		}
	}
}

void CvDLLWidgetData::doMenu( void )
{
	if (!gDLL->isGameInitializing())
	{
		CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_MAIN_MENU);
		if (NULL != pInfo)
		{
			gDLL->getInterfaceIFace()->addPopup(pInfo, NO_PLAYER, true);
		}
	}
}

void CvDLLWidgetData::doLaunch(CvWidgetDataStruct &widgetDataStruct)
{
	if (GET_TEAM(GC.getGameINLINE().getActiveTeam()).canLaunch((VictoryTypes)widgetDataStruct.m_iData1) && GC.getGameINLINE().testVictory((VictoryTypes)widgetDataStruct.m_iData1, GC.getGameINLINE().getActiveTeam()))
	{
		CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_LAUNCH, widgetDataStruct.m_iData1);
		if (NULL != pInfo)
		{
			gDLL->getInterfaceIFace()->addPopup(pInfo);
		}
	}
}

void CvDLLWidgetData::doForeignAdvisor(CvWidgetDataStruct &widgetDataStruct)
{
	CyArgsList argsList;
	argsList.add(widgetDataStruct.m_iData1);
	gDLL->getPythonIFace()->callFunction(PYScreensModule, "showForeignAdvisorScreen", argsList.makeFunctionArgs());
}

// MOD - START - Population Advisor
void CvDLLWidgetData::doPopulationAdvisor()
{
	CvCity* pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity != NULL)
	{
		doPopulationAdvisor(pHeadSelectedCity->getOwnerINLINE(), pHeadSelectedCity->getID());
	}
	else
	{
		doPopulationAdvisor(GC.getGameINLINE().getActivePlayer(), -1);
	}
}

void CvDLLWidgetData::doPopulationAdvisor(CvWidgetDataStruct &widgetDataStruct)
{
	CyArgsList argsList;
	argsList.add(widgetDataStruct.m_iData1);
	argsList.add(widgetDataStruct.m_iData2);
	gDLL->getPythonIFace()->callFunction(PYScreensModule, "showPopulationAdvisor", argsList.makeFunctionArgs());	
}

void CvDLLWidgetData::doPopulationAdvisor(PlayerTypes ePlayer, int iCityID)
{
	CyArgsList argsList;
	argsList.add(ePlayer);
	argsList.add(iCityID);
	gDLL->getPythonIFace()->callFunction(PYScreensModule, "showPopulationAdvisor", argsList.makeFunctionArgs());	
}
// MOD - END - Population Advisor

// MOD - START - Improved Civilopedia
void CvDLLWidgetData::doPediaBack()
{
	gDLL->getPythonIFace()->callFunction(PYScreensModule, "pediaBack");	
}

void CvDLLWidgetData::doPediaForward()
{
	gDLL->getPythonIFace()->callFunction(PYScreensModule, "pediaForward");	
}

void CvDLLWidgetData::doPediaClearSearchFilter()
{
	gDLL->getPythonIFace()->callFunction(PYScreensModule, "pediaClearSearchFilter");	
}

void CvDLLWidgetData::doPediaTogglePlayableFilter()
{
	gDLL->getPythonIFace()->callFunction(PYScreensModule, "pediaTogglePlayableFilter");	
}

void CvDLLWidgetData::doPediaView(CivilopediaViewTypes eView, CvWidgetDataStruct widgetData)
{
	doPediaAction(eView, widgetData.m_iData1, widgetData.m_iData2);
}

void CvDLLWidgetData::doPediaPage(CivilopediaPageTypes ePage, CvWidgetDataStruct widgetData)
{
	doPediaAction(ePage, widgetData.m_iData1, widgetData.m_iData2);
}

void CvDLLWidgetData::doPediaDescription(CvWidgetDataStruct &widgetDataStruct)
{
	CyArgsList argsList;
	argsList.add(widgetDataStruct.m_iData1);
	argsList.add(widgetDataStruct.m_iData2);
	gDLL->getPythonIFace()->callFunction(PYScreensModule, "pediaShowHistorical", argsList.makeFunctionArgs());
}
// MOD - END - Improved Civilopedia

//
//	HELP PARSING FUNCTIONS
//

void CvDLLWidgetData::parsePlotListHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	PROFILE_FUNC();

	CvUnit* pUnit;

	int iUnitIndex = widgetDataStruct.m_iData1 + gDLL->getInterfaceIFace()->getPlotListColumn() - gDLL->getInterfaceIFace()->getPlotListOffset();

	CvPlot *selectionPlot = gDLL->getInterfaceIFace()->getSelectionPlot();
	pUnit = gDLL->getInterfaceIFace()->getInterfacePlotUnit(selectionPlot, iUnitIndex);

	if (pUnit != NULL)
	{
		GAMETEXT.setUnitHelp(szBuffer, pUnit);

		if (pUnit->plot()->plotCount(PUF_isUnitType, pUnit->getUnitType(), -1, pUnit->getOwnerINLINE()) > 1)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_CTRL_SELECT", GC.getUnitInfo(pUnit->getUnitType()).getTextKeyWide()));
		}

		if (pUnit->plot()->plotCount(NULL, -1, -1, pUnit->getOwnerINLINE()) > 1)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_ALT_SELECT"));
		}
	}
}


void CvDLLWidgetData::parseLiberateCityHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity;
	CvWString szTempBuffer;

	pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity != NULL)
	{
		PlayerTypes ePlayer = pHeadSelectedCity->getLiberationPlayer(false);
		if (NO_PLAYER != ePlayer)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_LIBERATE_CITY_HELP", pHeadSelectedCity->getNameKey(), GET_PLAYER(ePlayer).getNameKey()));
		}
	}
}

void CvDLLWidgetData::parseCityNameHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity;
	CvWString szTempBuffer;

	pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity != NULL)
	{
		szBuffer.append(pHeadSelectedCity->getName());

		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_CITY_POPULATION", pHeadSelectedCity->getRealPopulation()));

		GAMETEXT.setTimeStr(szTempBuffer, pHeadSelectedCity->getGameTurnFounded(), false);
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_CITY_FOUNDED", szTempBuffer.GetCString()));

		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_CHANGE_NAME"));
	}
}



void CvDLLWidgetData::parseTrainHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity;
	UnitTypes eUnit;

	if (widgetDataStruct.m_iData2 != FFreeList::INVALID_INDEX)
	{
		pHeadSelectedCity = GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getCity(widgetDataStruct.m_iData2);
	}
	else
	{
		pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();
	}

	if (pHeadSelectedCity != NULL)
	{
		eUnit = (UnitTypes)GC.getCivilizationInfo(pHeadSelectedCity->getCivilizationType()).getCivilizationUnits(widgetDataStruct.m_iData1);

		GAMETEXT.setUnitHelp(szBuffer, eUnit, false, widgetDataStruct.m_bOption, false, pHeadSelectedCity);
	}
}


void CvDLLWidgetData::parseConstructHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity;
	BuildingTypes eBuilding;

	if (widgetDataStruct.m_iData2 != FFreeList::INVALID_INDEX)
	{
		pHeadSelectedCity = GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getCity(widgetDataStruct.m_iData2);
	}
	else
	{
		pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();
	}

	if (pHeadSelectedCity != NULL)
	{
		eBuilding = (BuildingTypes)GC.getCivilizationInfo(pHeadSelectedCity->getCivilizationType()).getCivilizationBuildings(widgetDataStruct.m_iData1);

		//GAMETEXT.setBuildingHelp(szBuffer, eBuilding, false, widgetDataStruct.m_bOption, false, pHeadSelectedCity);
// BUG - Building Actual Effects - start
		GAMETEXT.setBuildingHelpActual(szBuffer, eBuilding, false, widgetDataStruct.m_bOption, false, pHeadSelectedCity);
// BUG - Building Actual Effects - end
	}
}


void CvDLLWidgetData::parseCreateHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity;

	if (widgetDataStruct.m_iData2 != FFreeList::INVALID_INDEX)
	{
		pHeadSelectedCity = GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getCity(widgetDataStruct.m_iData2);
	}
	else
	{
		pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();
	}

	GAMETEXT.setProjectHelp(szBuffer, ((ProjectTypes)widgetDataStruct.m_iData1), false, pHeadSelectedCity);
}


void CvDLLWidgetData::parseMaintainHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.setProcessHelp(szBuffer, ((ProcessTypes)(widgetDataStruct.m_iData1)));
}


void CvDLLWidgetData::parseHurryHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity;
	CvWString szTempBuffer;
	bool bFirst;
	int iHurryGold;
	int iHurryPopulation;
	int iHurryAngerLength;
	int iI;

	pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity != NULL)
	{
		szBuffer.assign(gDLL->getText("TXT_KEY_MISC_HURRY_PROD", pHeadSelectedCity->getProductionNameKey()));

		iHurryGold = pHeadSelectedCity->hurryGold((HurryTypes)(widgetDataStruct.m_iData1));

		if (iHurryGold > 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_HURRY_GOLD", iHurryGold));
		}

		iHurryPopulation = pHeadSelectedCity->hurryPopulation((HurryTypes)(widgetDataStruct.m_iData1));

		if (iHurryPopulation > 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_HURRY_POP", iHurryPopulation));

			if (iHurryPopulation > pHeadSelectedCity->maxHurryPopulation())
			{
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_MAX_POP_HURRY", pHeadSelectedCity->maxHurryPopulation()));
			}
		}

		iHurryAngerLength = pHeadSelectedCity->hurryAngerLength((HurryTypes)(widgetDataStruct.m_iData1));

		if (iHurryAngerLength > 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_ANGER_TURNS", GC.getDefineINT("HURRY_POP_ANGER"), (iHurryAngerLength + pHeadSelectedCity->getHurryAngerTimer())));
		}

		if (!(pHeadSelectedCity->isProductionUnit()) && !(pHeadSelectedCity->isProductionBuilding()))
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_UNIT_BUILDING_HURRY"));
		}

		if (!(GET_PLAYER(pHeadSelectedCity->getOwnerINLINE()).canHurry((HurryTypes)(widgetDataStruct.m_iData1))))
		{
			bFirst = true;

			for (iI = 0; iI < GC.getNumCivicInfos(); iI++)
			{
				if (GC.getCivicInfo((CivicTypes)iI).isHurry(widgetDataStruct.m_iData1))
				{
					szTempBuffer = NEWLINE + gDLL->getText("TXT_KEY_REQUIRES");
					setListHelp(szBuffer, szTempBuffer, GC.getCivicInfo((CivicTypes)iI).getDescription(), gDLL->getText("TXT_KEY_OR").c_str(), bFirst);
					bFirst = false;
				}
			}		

			if (!bFirst)
			{
				szBuffer.append(ENDCOLR);
			}
		}
	}
}


void CvDLLWidgetData::parseConscriptHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity;
	CvWString szTempBuffer;
	int iConscriptPopulation;
	int iConscriptAngerLength;
	int iMinCityPopulation;
	int iMinCulturePercent;
	int iI;
	bool bFirst;

	pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity != NULL)
	{
		if (pHeadSelectedCity->getConscriptUnit() != NO_UNIT)
		{
			CvWString szTemp;
			szTemp.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR("COLOR_UNIT_TEXT"), GC.getUnitInfo(pHeadSelectedCity->getConscriptUnit()).getDescription());
			szBuffer.assign(szTemp);

			iConscriptPopulation = pHeadSelectedCity->getConscriptPopulation();

			if (iConscriptPopulation > 0)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_HURRY_POP", iConscriptPopulation));
			}

			iConscriptAngerLength = pHeadSelectedCity->flatConscriptAngerLength();

			if (iConscriptAngerLength > 0)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_ANGER_TURNS", GC.getDefineINT("CONSCRIPT_POP_ANGER"), (iConscriptAngerLength + pHeadSelectedCity->getConscriptAngerTimer())));
			}

			iMinCityPopulation = pHeadSelectedCity->conscriptMinCityPopulation();

			if (pHeadSelectedCity->getPopulation() < iMinCityPopulation)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_MIN_CITY_POP", iMinCityPopulation));
			}

			iMinCulturePercent = GC.getDefineINT("CONSCRIPT_MIN_CULTURE_PERCENT");

			if (pHeadSelectedCity->plot()->calculateTeamCulturePercent(pHeadSelectedCity->getTeam()) < iMinCulturePercent)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_MIN_CULTURE_PERCENT", iMinCulturePercent));
			}

			if (GET_PLAYER(pHeadSelectedCity->getOwnerINLINE()).getMaxConscript() == 0)
			{
				bFirst = true;

				for (iI = 0; iI < GC.getNumCivicInfos(); iI++)
				{
					if (getWorldSizeMaxConscript((CivicTypes)iI) > 0)
					{
						szTempBuffer = NEWLINE + gDLL->getText("TXT_KEY_REQUIRES");
						setListHelp(szBuffer, szTempBuffer, GC.getCivicInfo((CivicTypes)iI).getDescription(), gDLL->getText("TXT_KEY_OR").c_str(), bFirst);
						bFirst = false;
					}
				}		

				if (!bFirst)
				{
					szBuffer.append(ENDCOLR);
				}
			}
		}
	}
}


void CvDLLWidgetData::parseActionHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CLLNode<IDInfo>* pSelectedUnitNode;
	CvCity* pMissionCity;
	CvCity* pCity;
	CvUnit* pHeadSelectedUnit;
	CvUnit* pSelectedUnit;
	CvPlot* pMissionPlot;
	CvPlot* pLoopPlot;
	CvWString szTempBuffer;
	CvWString szFirstBuffer;
	ReligionTypes eReligion;
	BuildingTypes eBuilding;
	PlayerTypes eGiftPlayer;
	TechTypes eTech;
	bool bAlt;
	bool bShift;
	bool bValid;
	int iUnitConsume;
	int iUnitDiff;
	int iProduction;
	int iMovementCost;
	int iFlatMovementCost;
	int iMoves;
	int iFlatMoves;
	int iNowWorkRate;
	int iThenWorkRate;
	int iTurns;
	int iPrice;
	int iLow;
	int iHigh;
	int iLast;
	int iRange;
	int iDX, iDY;
	int iI;

	bAlt = GC.altKey();
	bShift = GC.shiftKey();

	CvWString szTemp;
	szTemp.Format(SETCOLR L"%s" ENDCOLR , TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), GC.getActionInfo(widgetDataStruct.m_iData1).getHotKeyDescription().c_str());
	szBuffer.assign(szTemp);

	pHeadSelectedUnit = gDLL->getInterfaceIFace()->getHeadSelectedUnit();

	if (pHeadSelectedUnit != NULL)
	{
		if (GC.getActionInfo(widgetDataStruct.m_iData1).getMissionType() != NO_MISSION)
		{
			if (bShift && gDLL->getInterfaceIFace()->mirrorsSelectionGroup())
			{
				pMissionPlot = pHeadSelectedUnit->getGroup()->lastMissionPlot();
			}
			else
			{
				pMissionPlot = pHeadSelectedUnit->plot();
			}

			pMissionCity = pMissionPlot->getPlotCity();

			if (GC.getActionInfo(widgetDataStruct.m_iData1).getMissionType() == MISSION_HEAL)
			{
				iTurns = 0;

				pSelectedUnitNode = gDLL->getInterfaceIFace()->headSelectionListNode();

				while (pSelectedUnitNode != NULL)
				{
					pSelectedUnit = ::getUnit(pSelectedUnitNode->m_data);
					iTurns = std::max(iTurns, pSelectedUnit->healTurns(pMissionPlot));

					pSelectedUnitNode = gDLL->getInterfaceIFace()->nextSelectionListNode(pSelectedUnitNode);
				}

				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_TURN_OR_TURNS", iTurns));
			}
			else if (GC.getActionInfo(widgetDataStruct.m_iData1).getMissionType() == MISSION_PILLAGE)
			{
				if (pMissionPlot->getImprovementType() != NO_IMPROVEMENT)
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_ACTION_DESTROY_IMP", GC.getImprovementInfo(pMissionPlot->getImprovementType()).getTextKeyWide()));
				}
				else if (pMissionPlot->getRouteType() != NO_ROUTE)
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_ACTION_DESTROY_IMP", GC.getRouteInfo(pMissionPlot->getRouteType()).getTextKeyWide()));
				}
			}
			else if (GC.getActionInfo(widgetDataStruct.m_iData1).getMissionType() == MISSION_PLUNDER)
			{
				pMissionPlot = pHeadSelectedUnit->plot();

				if (pMissionPlot->getTeam() == pHeadSelectedUnit->getTeam())
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_ACTION_PLUNDER_IN_BORDERS"));
				}
			}
			else if (GC.getActionInfo(widgetDataStruct.m_iData1).getMissionType() == MISSION_SABOTAGE)
			{
				pSelectedUnitNode = gDLL->getInterfaceIFace()->headSelectionListNode();

				while (pSelectedUnitNode != NULL)
				{
					pSelectedUnit = ::getUnit(pSelectedUnitNode->m_data);

					if (pSelectedUnit->canSabotage(pMissionPlot, true)) // XXX if queuing up this action, use the current plot along the goto...
					{
						iPrice = pSelectedUnit->sabotageCost(pMissionPlot);
						if (iPrice > 0)
						{
							szTempBuffer.Format(L"%d %c", iPrice, GC.getCommerceInfo(COMMERCE_GOLD).getChar());
							szBuffer.append(NEWLINE);
							szBuffer.append(szTempBuffer);
						}

						iLow = pSelectedUnit->sabotageProb(pMissionPlot, PROBABILITY_LOW);
						iHigh = pSelectedUnit->sabotageProb(pMissionPlot, PROBABILITY_HIGH);

						if (iLow == iHigh)
						{
							szBuffer.append(NEWLINE);
							szBuffer.append(gDLL->getText("TXT_KEY_ACTION_PROBABILITY", iHigh));
						}
						else
						{
							szBuffer.append(NEWLINE);
							szBuffer.append(gDLL->getText("TXT_KEY_ACTION_PROBABILITY_RANGE", iLow, iHigh));
						}
						break;
					}

					pSelectedUnitNode = gDLL->getInterfaceIFace()->nextSelectionListNode(pSelectedUnitNode);
				}
			}
			else if (GC.getActionInfo(widgetDataStruct.m_iData1).getMissionType() == MISSION_DESTROY)
			{
				pSelectedUnitNode = gDLL->getInterfaceIFace()->headSelectionListNode();

				while (pSelectedUnitNode != NULL)
				{
					pSelectedUnit = ::getUnit(pSelectedUnitNode->m_data);

					if (pSelectedUnit->canDestroy(pMissionPlot, true)) // XXX if queuing up this action, use the current plot along the goto...
					{
						iPrice = pSelectedUnit->destroyCost(pMissionPlot);
						if (iPrice > 0)
						{
							szTempBuffer.Format(L"%d %c", iPrice, GC.getCommerceInfo(COMMERCE_GOLD).getChar());
							szBuffer.append(NEWLINE);
							szBuffer.append(szTempBuffer);
						}

						iLow = pSelectedUnit->destroyProb(pMissionPlot, PROBABILITY_LOW);
						iHigh = pSelectedUnit->destroyProb(pMissionPlot, PROBABILITY_HIGH);

						if (iLow == iHigh)
						{
							szBuffer.append(NEWLINE);
							szBuffer.append(gDLL->getText("TXT_KEY_ACTION_PROBABILITY", iHigh));
						}
						else
						{
							szBuffer.append(NEWLINE);
							szBuffer.append(gDLL->getText("TXT_KEY_ACTION_PROBABILITY_RANGE", iLow, iHigh));
						}
						break;
					}

					pSelectedUnitNode = gDLL->getInterfaceIFace()->nextSelectionListNode(pSelectedUnitNode);
				}
			}
			else if (GC.getActionInfo(widgetDataStruct.m_iData1).getMissionType() == MISSION_STEAL_PLANS)
			{
				pSelectedUnitNode = gDLL->getInterfaceIFace()->headSelectionListNode();

				while (pSelectedUnitNode != NULL)
				{
					pSelectedUnit = ::getUnit(pSelectedUnitNode->m_data);

					if (pSelectedUnit->canStealPlans(pMissionPlot, true)) // XXX if queuing up this action, use the current plot along the goto...
					{
						iPrice = pSelectedUnit->stealPlansCost(pMissionPlot);
						if (iPrice > 0)
						{
							szTempBuffer.Format(L"%d %c", iPrice, GC.getCommerceInfo(COMMERCE_GOLD).getChar());
							szBuffer.append(NEWLINE);
							szBuffer.append(szTempBuffer);
						}

						iLow = pSelectedUnit->stealPlansProb(pMissionPlot, PROBABILITY_LOW);
						iHigh = pSelectedUnit->stealPlansProb(pMissionPlot, PROBABILITY_HIGH);

						if (iLow == iHigh)
						{
							szBuffer.append(NEWLINE);
							szBuffer.append(gDLL->getText("TXT_KEY_ACTION_PROBABILITY", iHigh));
						}
						else
						{
							szBuffer.append(NEWLINE);
							szBuffer.append(gDLL->getText("TXT_KEY_ACTION_PROBABILITY_RANGE", iLow, iHigh));
						}
						break;
					}

					pSelectedUnitNode = gDLL->getInterfaceIFace()->nextSelectionListNode(pSelectedUnitNode);
				}
			}
			else if (GC.getActionInfo(widgetDataStruct.m_iData1).getMissionType() == MISSION_FOUND)
			{
				if (!(GET_PLAYER(pHeadSelectedUnit->getOwnerINLINE()).canFound(pMissionPlot->getX_INLINE(), pMissionPlot->getY_INLINE())))
				{
					bValid = true;

					iRange = GC.getMIN_CITY_RANGE();

					for (iDX = -(iRange); iDX <= iRange; iDX++)
					{
						for (iDY = -(iRange); iDY <= iRange; iDY++)
						{
							pLoopPlot	= plotXY(pMissionPlot->getX_INLINE(), pMissionPlot->getY_INLINE(), iDX, iDY);

							if (pLoopPlot != NULL)
							{
								if (pLoopPlot->isCity())
								{
									bValid = false;
								}
							}
						}
					}

					if (!bValid)
					{
						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_ACTION_CANNOT_FOUND", GC.getMIN_CITY_RANGE()));
					}
				}
			}
			else if (GC.getActionInfo(widgetDataStruct.m_iData1).getMissionType() == MISSION_SPREAD)
			{
				eReligion = ((ReligionTypes)(GC.getActionInfo(widgetDataStruct.m_iData1).getMissionData()));

				if (pMissionCity != NULL)
				{
					if (pMissionCity->getTeam() != pHeadSelectedUnit->getTeam()) // XXX still true???
					{
						if (GET_PLAYER(pMissionCity->getOwnerINLINE()).isNoNonStateReligionSpread())
						{
							if (eReligion != GET_PLAYER(pMissionCity->getOwnerINLINE()).getStateReligion())
							{
								szBuffer.append(NEWLINE);
								szBuffer.append(gDLL->getText("TXT_KEY_ACTION_CANNOT_SPREAD_NON_STATE_RELIGION"));
							}
						}
					}

					szBuffer.append(NEWLINE);
					GAMETEXT.setReligionHelpCity(szBuffer, eReligion, pMissionCity, false, true);
				}
			}
			else if (GC.getActionInfo(widgetDataStruct.m_iData1).getMissionType() == MISSION_SPREAD_CORPORATION)
			{
				CorporationTypes eCorporation = ((CorporationTypes)(GC.getActionInfo(widgetDataStruct.m_iData1).getMissionData()));

				if (pMissionCity != NULL)
				{
					szBuffer.append(NEWLINE);
					GAMETEXT.setCorporationHelpCity(szBuffer, eCorporation, pMissionCity, false, true);

					for (int iCorp = 0; iCorp < GC.getNumCorporationInfos(); ++iCorp)
					{
						if (eCorporation != iCorp)
						{
							if (pMissionCity->isHasCorporation((CorporationTypes)iCorp))
							{
								if (GC.getGameINLINE().isCompetingCorporation(eCorporation, (CorporationTypes)iCorp))
								{
									szBuffer.append(NEWLINE);
									szBuffer.append(gDLL->getText("TXT_KEY_ACTION_WILL_ELIMINATE_CORPORATION", GC.getCorporationInfo((CorporationTypes)iCorp).getTextKeyWide()));
								}
							}
						}
					}

					szTempBuffer.Format(L"%s%d %c", NEWLINE, pHeadSelectedUnit->spreadCorporationCost(eCorporation, pMissionCity), GC.getCommerceInfo(COMMERCE_GOLD).getChar());
					szBuffer.append(szTempBuffer);

					if (!pHeadSelectedUnit->canSpreadCorporation(pMissionPlot, eCorporation))
					{
						if (!GET_PLAYER(pMissionCity->getOwnerINLINE()).isActiveCorporation(eCorporation))
						{
							szBuffer.append(NEWLINE);
							szBuffer.append(gDLL->getText("TXT_KEY_ACTION_CORPORATION_NOT_ACTIVE", GC.getCorporationInfo(eCorporation).getTextKeyWide(), GET_PLAYER(pMissionCity->getOwnerINLINE()).getCivilizationAdjective()));
						}

						CorporationTypes eCompetition = NO_CORPORATION;
						for (int iCorporation = 0; iCorporation < GC.getNumCorporationInfos(); ++iCorporation)
						{
							if (pMissionCity->isHeadquarters((CorporationTypes)iCorporation))
							{
								if (GC.getGameINLINE().isCompetingCorporation((CorporationTypes)iCorporation, eCorporation))
								{
									eCompetition = (CorporationTypes)iCorporation;
									break;
								}
							}
						}

						if (NO_CORPORATION != eCompetition)
						{
							szBuffer.append(NEWLINE);
							szBuffer.append(gDLL->getText("TXT_KEY_ACTION_CORPORATION_COMPETING_HEADQUARTERS", GC.getCorporationInfo(eCorporation).getTextKeyWide(), GC.getCorporationInfo(eCompetition).getTextKeyWide()));
						}

						CvWStringBuffer szBonusList;
						bool bValid = false;
						bool bFirst = true;
						for (int i = 0; i < GC.getNUM_CORPORATION_PREREQ_BONUSES(); ++i)
						{
							BonusTypes eBonus = (BonusTypes)GC.getCorporationInfo(eCorporation).getPrereqBonus(i);
							if (NO_BONUS != eBonus)
							{
								if (!bFirst)
								{
									szBonusList.append(L", ");
								}
								else
								{
									bFirst = false;
								}
								szBonusList.append(GC.getBonusInfo(eBonus).getDescription());

								if (pMissionCity->hasBonus(eBonus))
								{
									bValid = true;
									break;
								}
							}
						}

						if (!bValid)
						{
							szBuffer.append(NEWLINE);
							szBuffer.append(gDLL->getText("TXT_KEY_ACTION_CORPORATION_NO_RESOURCES", pMissionCity->getNameKey(), szBonusList.getCString()));
						}
					}
				}
			}
			else if (GC.getActionInfo(widgetDataStruct.m_iData1).getMissionType() == MISSION_JOIN)
			{
				GAMETEXT.parseSpecialistHelp(szBuffer, ((SpecialistTypes)(GC.getActionInfo(widgetDataStruct.m_iData1).getMissionData())), pMissionCity, true);
			}
			else if (GC.getActionInfo(widgetDataStruct.m_iData1).getMissionType() == MISSION_CONSTRUCT)
			{
				eBuilding = ((BuildingTypes)(GC.getActionInfo(widgetDataStruct.m_iData1).getMissionData()));

				if (pMissionCity != NULL)
				{
					if (!pHeadSelectedUnit->getUnitInfo().getForceBuildings(eBuilding)  && !pMissionCity->canConstruct(eBuilding, false, false, true))
					{
						if (!(GC.getGameINLINE().isBuildingClassMaxedOut((BuildingClassTypes)(GC.getBuildingInfo(eBuilding).getBuildingClassType()))))
						{
							GAMETEXT.buildBuildingRequiresString(szBuffer, ((BuildingTypes)(GC.getActionInfo(widgetDataStruct.m_iData1).getMissionData())), false, false, pMissionCity);
						}
					}
					else
					{
						szBuffer.append(NEWLINE);
						//GAMETEXT.setBuildingHelp(szBuffer, ((BuildingTypes)(GC.getActionInfo(widgetDataStruct.m_iData1).getMissionData())), false, false, false, pMissionCity);
// BUG - Building Actual Effects - start
						GAMETEXT.setBuildingHelpActual(szBuffer, ((BuildingTypes)(GC.getActionInfo(widgetDataStruct.m_iData1).getMissionData())), false, false, false, pMissionCity);
// BUG - Building Actual Effects - end
					}
				}
			}
			else if (GC.getActionInfo(widgetDataStruct.m_iData1).getMissionType() == MISSION_DISCOVER)
			{
				pSelectedUnitNode = gDLL->getInterfaceIFace()->headSelectionListNode();

				while (pSelectedUnitNode != NULL)
				{
					pSelectedUnit = ::getUnit(pSelectedUnitNode->m_data);

					if (pSelectedUnit->canDiscover(pMissionPlot))
					{
						eTech = pSelectedUnit->getDiscoveryTech();
	
						if (pSelectedUnit->getDiscoverResearch(eTech) >= GET_TEAM(pSelectedUnit->getTeam()).getResearchLeft(eTech))
						{
							szTempBuffer.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR("COLOR_TECH_TEXT"), GC.getTechInfo(eTech).getDescription());
							szBuffer.append(NEWLINE);
							szBuffer.append(szTempBuffer);
						}
						else
						{
							szBuffer.append(NEWLINE);
							szBuffer.append(gDLL->getText("TXT_KEY_ACTION_EXTRA_RESEARCH", pSelectedUnit->getDiscoverResearch(eTech), GC.getTechInfo(eTech).getTextKeyWide()));
						}
						break;
					}

					pSelectedUnitNode = gDLL->getInterfaceIFace()->nextSelectionListNode(pSelectedUnitNode);
				}
			}
			else if (GC.getActionInfo(widgetDataStruct.m_iData1).getMissionType() == MISSION_HURRY)
			{
				if (pMissionCity != NULL)
				{
					if (!(pMissionCity->isProductionBuilding()))
					{
						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_ACTION_BUILDING_HURRY"));
					}
					else
					{
						pSelectedUnitNode = gDLL->getInterfaceIFace()->headSelectionListNode();

						while (pSelectedUnitNode != NULL)
						{
							pSelectedUnit = ::getUnit(pSelectedUnitNode->m_data);

							if (pSelectedUnit->canHurry(pMissionPlot, true))
							{
								const wchar* pcKey = NULL;
								if (NO_PROJECT != pMissionCity->getProductionProject())
								{
									pcKey = GC.getProjectInfo(pMissionCity->getProductionProject()).getTextKeyWide();
								}
								else if (NO_BUILDING != pMissionCity->getProductionBuilding())
								{
									pcKey = GC.getBuildingInfo(pMissionCity->getProductionBuilding()).getTextKeyWide();
								}
								else if (NO_UNIT != pMissionCity->getProductionUnit())
								{
									pcKey = GC.getUnitInfo(pMissionCity->getProductionUnit()).getTextKeyWide();
								}
								if (NULL != pcKey && pSelectedUnit->getHurryProduction(pMissionPlot) >= pMissionCity->productionLeft())
								{
									szBuffer.append(NEWLINE);
									szBuffer.append(gDLL->getText("TXT_KEY_ACTION_FINISH_CONSTRUCTION", pcKey));
								}
								else
								{
									szBuffer.append(NEWLINE);
									szBuffer.append(gDLL->getText("TXT_KEY_ACTION_EXTRA_CONSTRUCTION", pSelectedUnit->getHurryProduction(pMissionPlot), pcKey));
								}
								break;
							}

							pSelectedUnitNode = gDLL->getInterfaceIFace()->nextSelectionListNode(pSelectedUnitNode);
						}
					}
				}
			}
			// MOD - START - Advanced Automations
/************************************************************************************************/
/* Afforess	                  Start		 06/21/10                                               */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
			else if (GC.getActionInfo(widgetDataStruct.m_iData1).getMissionType() == MISSION_ESPIONAGE_SLEEP)
			{
				if (pMissionCity != NULL)
				{
					pSelectedUnitNode = gDLL->getInterfaceIFace()->headSelectionListNode();

					while (pSelectedUnitNode != NULL)
					{
						pSelectedUnit = ::getUnit(pSelectedUnitNode->m_data);

						if (pSelectedUnit->canEspionage(pMissionPlot, false) || pSelectedUnit->canEspionage(pMissionPlot, true))
						{
							szBuffer.append(NEWLINE);
							szBuffer.append(gDLL->getText("TXT_KEY_MISSION_ESPIONAGE_SLEEP_HELP"));
						}
						break;
						pSelectedUnitNode = gDLL->getInterfaceIFace()->nextSelectionListNode(pSelectedUnitNode);
					}
				}
			}
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/
			// MOD - END - Advanced Automations
			else if (GC.getActionInfo(widgetDataStruct.m_iData1).getMissionType() == MISSION_TRADE)
			{
				if (pMissionCity != NULL)
				{
					if (pMissionCity->getOwnerINLINE() == pHeadSelectedUnit->getOwnerINLINE())
					{
						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_ACTION_TRADE_MISSION_FOREIGN"));
					}
					else
					{
						pSelectedUnitNode = gDLL->getInterfaceIFace()->headSelectionListNode();

						while (pSelectedUnitNode != NULL)
						{
							pSelectedUnit = ::getUnit(pSelectedUnitNode->m_data);

							if (pSelectedUnit->canTrade(pMissionPlot, true))
							{
								szTempBuffer.Format(L"%s+%d%c", NEWLINE, pSelectedUnit->getTradeGold(pMissionPlot), GC.getCommerceInfo(COMMERCE_GOLD).getChar());
								szBuffer.append(szTempBuffer);
								break;
							}

							pSelectedUnitNode = gDLL->getInterfaceIFace()->nextSelectionListNode(pSelectedUnitNode);
						}
					}
				}
			}
			else if (GC.getActionInfo(widgetDataStruct.m_iData1).getMissionType() == MISSION_GREAT_WORK)
			{
				pSelectedUnitNode = gDLL->getInterfaceIFace()->headSelectionListNode();

				while (pSelectedUnitNode != NULL)
				{
					pSelectedUnit = ::getUnit(pSelectedUnitNode->m_data);

					if (pSelectedUnit->canGreatWork(pMissionPlot))
					{
						szTempBuffer.Format(L"%s+%d%c", NEWLINE, pSelectedUnit->getGreatWorkCulture(pMissionPlot), GC.getCommerceInfo(COMMERCE_CULTURE).getChar());
						szBuffer.append(szTempBuffer);
						break;
					}

					pSelectedUnitNode = gDLL->getInterfaceIFace()->nextSelectionListNode(pSelectedUnitNode);
				}
			}
			else if (GC.getActionInfo(widgetDataStruct.m_iData1).getMissionType() == MISSION_INFILTRATE)
			{
				if (pMissionCity != NULL)
				{
					if (pMissionCity->getOwnerINLINE() == pHeadSelectedUnit->getOwnerINLINE())
					{
						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_ACTION_INFILTRATE_MISSION_FOREIGN"));
					}
					else
					{
						pSelectedUnitNode = gDLL->getInterfaceIFace()->headSelectionListNode();

						while (pSelectedUnitNode != NULL)
						{
							pSelectedUnit = ::getUnit(pSelectedUnitNode->m_data);

							if (pSelectedUnit->canEspionage(pMissionPlot))
							{
								szTempBuffer.Format(L"%s+%d%c", NEWLINE, pSelectedUnit->getEspionagePoints(pMissionPlot), GC.getCommerceInfo(COMMERCE_ESPIONAGE).getChar());
								szBuffer.append(szTempBuffer);
								break;
							}

							pSelectedUnitNode = gDLL->getInterfaceIFace()->nextSelectionListNode(pSelectedUnitNode);
						}
					}
				}
			}
			else if (GC.getActionInfo(widgetDataStruct.m_iData1).getMissionType() == MISSION_GOLDEN_AGE)
			{
				iUnitConsume = GET_PLAYER(pHeadSelectedUnit->getOwnerINLINE()).unitsRequiredForGoldenAge();
				iUnitDiff = (iUnitConsume - GET_PLAYER(pHeadSelectedUnit->getOwnerINLINE()).unitsGoldenAgeReady());

				if (iUnitDiff > 0)
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_ACTION_MORE_GREAT_PEOPLE", iUnitDiff));
				}

				if (iUnitConsume > 1)
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_ACTION_CONSUME_GREAT_PEOPLE", iUnitConsume));
				}
			}
			else if (GC.getActionInfo(widgetDataStruct.m_iData1).getMissionType() == MISSION_LEAD)
			{
				if (pHeadSelectedUnit->getUnitInfo().getLeaderExperience() > 0)
				{
					int iNumUnits = pHeadSelectedUnit->canGiveExperience(pHeadSelectedUnit->plot());
					if (iNumUnits > 0)
					{
						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_ACTION_LEAD_TROOPS", pHeadSelectedUnit->getStackExperienceToGive(iNumUnits)));
					}
				}
				if (pHeadSelectedUnit->getUnitInfo().getLeaderPromotion() != NO_PROMOTION)
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_WHEN_LEADING"));
					GAMETEXT.parsePromotionHelp(szBuffer, (PromotionTypes)pHeadSelectedUnit->getUnitInfo().getLeaderPromotion(), L"\n   ");
				}
			}
			else if (GC.getActionInfo(widgetDataStruct.m_iData1).getMissionType() == MISSION_ESPIONAGE)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_ACTION_ESPIONAGE_MISSION"));

				GAMETEXT.setEspionageMissionHelp(szBuffer, pHeadSelectedUnit);
			}
			else if (GC.getActionInfo(widgetDataStruct.m_iData1).getMissionType() == MISSION_BUILD)
			{
				BuildTypes eBuild = ((BuildTypes)(GC.getActionInfo(widgetDataStruct.m_iData1).getMissionData()));
				FAssert(eBuild != NO_BUILD);
				ImprovementTypes eImprovement = ((ImprovementTypes)(GC.getBuildInfo(eBuild).getImprovement()));
				RouteTypes eRoute = ((RouteTypes)(GC.getBuildInfo(eBuild).getRoute()));
				RouteTypes eOldRoute = pMissionPlot->getRouteType();
				BonusTypes eBonus = pMissionPlot->getBonusType(pHeadSelectedUnit->getTeam());

				// MOD - START - Advanced Yield and Commerce Effects
				CvCity* pWorkingCity = pMissionPlot->getWorkingCity();
				PlayerTypes eBuildOwner = pHeadSelectedUnit->getOwnerINLINE();

				if (pWorkingCity != NULL)
				{
					if (pWorkingCity->getTeam() != pHeadSelectedUnit->getTeam())
					{
						pWorkingCity = NULL; // Someone else's city that isn't on our team
					}
					else
					{
						eBuildOwner = pWorkingCity->getOwnerINLINE();
					}
				}

				const CvPlayerAI& kBuildOwner = GET_PLAYER(eBuildOwner);
				const CvTeamAI& kBuildTeam = GET_TEAM(kBuildOwner.getTeam());
				// MOD - END - Advanced Yield and Commerce Effects

				for (iI = 0; iI < NUM_YIELD_TYPES; iI++)
				{
					int iYield = 0;

					if (eImprovement != NO_IMPROVEMENT)
					{
						// MOD - START - Advanced Yield and Commerce Effects
						/*
						iYield += pMissionPlot->calculateImprovementYieldChange(eImprovement, ((YieldTypes)iI), pHeadSelectedUnit->getOwnerINLINE());
						if (pMissionPlot->getImprovementType() != NO_IMPROVEMENT)
						{
							iYield -= pMissionPlot->calculateImprovementYieldChange(pMissionPlot->getImprovementType(), ((YieldTypes)iI), pHeadSelectedUnit->getOwnerINLINE());
						}
						*/

						iYield += pMissionPlot->calculateImprovementYieldChange(eImprovement, ((YieldTypes)iI), eBuildOwner, false, false, pWorkingCity);

						if (pMissionPlot->getImprovementType() != NO_IMPROVEMENT)
						{
							iYield -= pMissionPlot->calculateImprovementYieldChange(pMissionPlot->getImprovementType(), ((YieldTypes)iI), eBuildOwner, false, false, pWorkingCity);
						}
						// MOD - END - Advanced Yield and Commerce Effects
					}

					if (NO_FEATURE != pMissionPlot->getFeatureType())
					{
						if (GC.getBuildInfo(eBuild).isFeatureRemove(pMissionPlot->getFeatureType()))
						{
							iYield -= GC.getFeatureInfo(pMissionPlot->getFeatureType()).getYieldChange(iI);
						}
					}

					// MOD - START - Negative Yields
					iYield = std::max(iYield, -pMissionPlot->getYield((YieldTypes)iI));
					// MOD - END - Negative Yields

					if (iYield != 0)
					{
						szTempBuffer.Format(L", %s%d%c", ((iYield > 0) ? L"+" : L""), iYield, GC.getYieldInfo((YieldTypes) iI).getChar());
						szBuffer.append(szTempBuffer);
					}
				}

				if (NO_IMPROVEMENT != eImprovement)
				{
					int iHappy = GC.getImprovementInfo(eImprovement).getHappiness();

					if (iHappy != 0)
					{
						szTempBuffer.Format(L", +%d%c", abs(iHappy), (iHappy > 0 ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR)));
						szBuffer.append(szTempBuffer);
					}
				}

				bValid = false;

				pSelectedUnitNode = gDLL->getInterfaceIFace()->headSelectionListNode();

				while (pSelectedUnitNode != NULL)
				{
					pSelectedUnit = ::getUnit(pSelectedUnitNode->m_data);

					if (pSelectedUnit->canBuild(pMissionPlot, eBuild))
					{
						bValid = true;
						break;
					}

					pSelectedUnitNode = gDLL->getInterfaceIFace()->nextSelectionListNode(pSelectedUnitNode);
				}

				if (!bValid)
				{
					if (eImprovement != NO_IMPROVEMENT)
					{
						if (pMissionPlot->getTeam() != pHeadSelectedUnit->getTeam())
						{
							if (GC.getImprovementInfo(eImprovement).isOutsideBorders())
							{
								if (pMissionPlot->getTeam() != NO_TEAM)
								{
									szBuffer.append(NEWLINE);
									szBuffer.append(gDLL->getText("TXT_KEY_ACTION_NEEDS_OUT_RIVAL_CULTURE_BORDER"));
								}
							}
							else
							{
								szBuffer.append(NEWLINE);
								szBuffer.append(gDLL->getText("TXT_KEY_ACTION_NEEDS_CULTURE_BORDER"));
							}
						}

						if ((eBonus == NO_BONUS) || !(GC.getImprovementInfo(eImprovement).isImprovementBonusTrade(eBonus)))
						{
							if (!(GET_TEAM(pHeadSelectedUnit->getTeam()).isIrrigation()) && !(GET_TEAM(pHeadSelectedUnit->getTeam()).isIgnoreIrrigation()))
							{
								if (GC.getImprovementInfo(eImprovement).isRequiresIrrigation() && !(pMissionPlot->isIrrigationAvailable()))
								{
									for (iI = 0; iI < GC.getNumTechInfos(); iI++)
									{
										if (GC.getTechInfo((TechTypes)iI).isIrrigation())
										{
											szBuffer.append(NEWLINE);
											szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_REQUIRES_STRING", GC.getTechInfo((TechTypes)iI).getTextKeyWide()));
											break;
										}
									}
								}
							}
						}
					}

					if (!(GET_TEAM(pHeadSelectedUnit->getTeam()).isHasTech((TechTypes)GC.getBuildInfo(eBuild).getTechPrereq())))
					{
						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_REQUIRES_STRING", GC.getTechInfo((TechTypes) GC.getBuildInfo(eBuild).getTechPrereq()).getTextKeyWide()));
					}

					if (eRoute != NO_ROUTE)
					{
						if (GC.getRouteInfo(eRoute).getPrereqBonus() != NO_BONUS)
						{
							if (!(pMissionPlot->isAdjacentPlotGroupConnectedBonus(pHeadSelectedUnit->getOwnerINLINE(), ((BonusTypes)(GC.getRouteInfo(eRoute).getPrereqBonus())))))
							{
								szBuffer.append(NEWLINE);
								szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_REQUIRES_STRING", GC.getBonusInfo((BonusTypes) GC.getRouteInfo(eRoute).getPrereqBonus()).getTextKeyWide()));
							}
						}

						bool bFoundValid = true;
						std::vector<BonusTypes> aeOrBonuses;
						for (int i = 0; i < GC.getNUM_ROUTE_PREREQ_OR_BONUSES(); ++i)
						{
							if (NO_BONUS != GC.getRouteInfo(eRoute).getPrereqOrBonus(i))
							{
								aeOrBonuses.push_back((BonusTypes)GC.getRouteInfo(eRoute).getPrereqOrBonus(i));
								bFoundValid = false;

								if (pMissionPlot->isAdjacentPlotGroupConnectedBonus(pHeadSelectedUnit->getOwnerINLINE(), ((BonusTypes)(GC.getRouteInfo(eRoute).getPrereqOrBonus(i)))))
								{
									bFoundValid = true;
									break;
								}
							}
						}

						if (!bFoundValid)
						{
							bool bFirst = true;
							for (std::vector<BonusTypes>::iterator it = aeOrBonuses.begin(); it != aeOrBonuses.end(); ++it)
							{
								szFirstBuffer = NEWLINE + gDLL->getText("TXT_KEY_BUILDING_REQUIRES_LIST");
								szTempBuffer.Format( SETCOLR L"<link=literal>%s</link>" ENDCOLR , TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), GC.getBonusInfo(*it).getDescription());
								setListHelp(szBuffer, szFirstBuffer.GetCString(), szTempBuffer, gDLL->getText("TXT_KEY_OR").c_str(), bFirst);
								bFirst = false;
							}
						}
					}

					if (pMissionPlot->getFeatureType() != NO_FEATURE)
					{
						if (!(GET_TEAM(pHeadSelectedUnit->getTeam()).isHasTech((TechTypes)GC.getBuildInfo(eBuild).getFeatureTech(pMissionPlot->getFeatureType()))))
						{
							szBuffer.append(NEWLINE);
							szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_REQUIRES_STRING", GC.getTechInfo((TechTypes) GC.getBuildInfo(eBuild).getFeatureTech(pMissionPlot->getFeatureType())).getTextKeyWide()));
						}
					}
				}

				if (eImprovement != NO_IMPROVEMENT)
				{
					if (pMissionPlot->getImprovementType() != NO_IMPROVEMENT)
					{
						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_ACTION_WILL_DESTROY_IMP", GC.getImprovementInfo(pMissionPlot->getImprovementType()).getTextKeyWide()));
					}
				}

				if (GC.getBuildInfo(eBuild).isKill())
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_ACTION_CONSUME_UNIT"));
				}

				if (pMissionPlot->getFeatureType() != NO_FEATURE)
				{
					if (GC.getBuildInfo(eBuild).isFeatureRemove(pMissionPlot->getFeatureType()))
					{
						iProduction = pMissionPlot->getFeatureProduction(eBuild, pHeadSelectedUnit->getTeam(), &pCity);

						if (iProduction > 0)
						{
							szBuffer.append(NEWLINE);
							szBuffer.append(gDLL->getText("TXT_KEY_ACTION_CHANGE_PRODUCTION", iProduction, pCity->getNameKey()));
						}

						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_ACTION_REMOVE_FEATURE", GC.getFeatureInfo(pMissionPlot->getFeatureType()).getTextKeyWide()));

/*************************************************************************************************/
/* UNOFFICIAL_PATCH                       06/10/10                           EmperorFool         */
/*                                                                                               */
/* Bugfix                                                                                        */
/*************************************************************************************************/
						if (eImprovement == NO_IMPROVEMENT && pMissionPlot->getImprovementType() != NO_IMPROVEMENT && GC.getImprovementInfo(pMissionPlot->getImprovementType()).getFeatureMakesValid(pMissionPlot->getFeatureType()))
						{
							szBuffer.append(NEWLINE);
							szBuffer.append(gDLL->getText("TXT_KEY_ACTION_WILL_DESTROY_IMP", GC.getImprovementInfo(pMissionPlot->getImprovementType()).getTextKeyWide()));
						}
/*************************************************************************************************/
/* UNOFFICIAL_PATCH                         END                                                  */
/*************************************************************************************************/
					}

				}

				if (eImprovement != NO_IMPROVEMENT)
				{
					CvImprovementInfo& kImprovement = GC.getImprovementInfo(eImprovement);

					if (eBonus != NO_BONUS)
					{
						//if (!GET_TEAM(pHeadSelectedUnit->getTeam()).isBonusObsolete(eBonus))
						if (GET_PLAYER(pHeadSelectedUnit->getOwnerINLINE()).doesImprovementConnectBonus(eImprovement, eBonus)) // K-Mod
						{
							//if (kImprovement.isImprovementBonusTrade(eBonus))
							{
								szBuffer.append(NEWLINE);
								szBuffer.append(gDLL->getText("TXT_KEY_ACTION_PROVIDES_BONUS", GC.getBonusInfo(eBonus).getTextKeyWide()));

								if (GC.getBonusInfo(eBonus).getHealth() != 0)
								{
									szTempBuffer.Format(L" (+%d%c)", abs(GC.getBonusInfo(eBonus).getHealth()), ((GC.getBonusInfo(eBonus).getHealth() > 0) ? gDLL->getSymbolID(HEALTHY_CHAR) : gDLL->getSymbolID(UNHEALTHY_CHAR)));
									szBuffer.append(szTempBuffer);
								}

								if (GC.getBonusInfo(eBonus).getHappiness() != 0)
								{
									szTempBuffer.Format(L" (+%d%c)", abs(GC.getBonusInfo(eBonus).getHappiness()), ((GC.getBonusInfo(eBonus).getHappiness() > 0) ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR)));
									szBuffer.append(szTempBuffer);
								}
							}
						}
					}
					else
					{
						iLast = 0;

						FAssert((0 < GC.getNumBonusInfos()) && "GC.getNumBonusInfos() is not greater than zero but an array is being allocated in CvDLLWidgetData::parseActionHelp");
						for (iI = 0; iI < GC.getNumBonusInfos(); iI++)
						{
							if (GET_TEAM(pHeadSelectedUnit->getTeam()).isHasTech((TechTypes)(GC.getBonusInfo((BonusTypes) iI).getTechReveal())))
							{
								if (kImprovement.getImprovementBonusDiscoverRand(iI) > 0)
								{
									szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_ACTION_CHANCE_DISCOVER").c_str());
									szTempBuffer.Format(L"%c", GC.getBonusInfo((BonusTypes) iI).getChar());
									setListHelp(szBuffer, szFirstBuffer, szTempBuffer, L", ", (kImprovement.getImprovementBonusDiscoverRand(iI) != iLast));
									iLast = kImprovement.getImprovementBonusDiscoverRand(iI);
								}
							}
						}
					}

					if (!(pMissionPlot->isIrrigationAvailable()))
					{
						GAMETEXT.setYieldChangeHelp(szBuffer, gDLL->getText("TXT_KEY_ACTION_IRRIGATED").c_str(), L": ", L"", kImprovement.getIrrigatedYieldChangeArray());
					}

					// MOD - START - Improved Build Help
					// MOD - START - Relation Caching
					for (int iRel = 0; iRel < kImprovement.getNumRelatedTechs(); iRel++)
					{
						TechTypes eLoopTech = (TechTypes)kImprovement.getRelatedTechs(iRel);
						if (!kBuildTeam.isHasTech(eLoopTech))
						{
							GAMETEXT.setYieldChangeHelp(szBuffer, GC.getTechInfo(eLoopTech).getDescription(), L": ", L"", kImprovement.getTechYieldChangesArray(eLoopTech));
						}
					}

					for (int iRel = 0; iRel < kImprovement.getNumRelatedCivics(); iRel++)
					{
						CivicTypes eLoopCivic = (CivicTypes)kImprovement.getRelatedCivics(iRel);
						if (!kBuildOwner.isCivic(eLoopCivic))
						{
							const CvCivicInfo& kLoopCivic = GC.getCivicInfo(eLoopCivic);
							int aiYieldChanges[NUM_YIELD_TYPES] = {};
							kLoopCivic.getImprovementYieldChangeArray(eImprovement, aiYieldChanges);
							GAMETEXT.setYieldChangeHelp(szBuffer, GC.getCivicInfo(eLoopCivic).getDescription(), L": ", L"", aiYieldChanges);
						}
					}
					// MOD - END - Relation Caching
					// MOD - END - Improved Build Help

					if (eRoute == NO_ROUTE)
					{
						for (iI = 0; iI < GC.getNumRouteInfos(); iI++)
						{
							// MOD - START - Complex Routes
							RouteTypes eLoopRoute = (RouteTypes)iI;
							if (eOldRoute != eLoopRoute)
							{
								int aiYieldChanges[NUM_YIELD_TYPES] = {};

								// Add the effect of the candidate route
								for (int iYield = 0; iYield < NUM_YIELD_TYPES; iYield++)
								{
									aiYieldChanges[iYield] += kImprovement.getRouteYieldChanges(eLoopRoute, iYield);
								}

								// Subtract the effect of the existing route
								if (eOldRoute != NO_ROUTE)
								{
									for (int iYield = 0; iYield < NUM_YIELD_TYPES; iYield++)
									{
										aiYieldChanges[iYield] -= kImprovement.getRouteYieldChanges(eOldRoute, iYield);
									}
								}

								// Determine whether the candidate route would provide a positive benefit in at least one yield type
								bool bProvidesBenefit = false;

								for (int iYield = 0; iYield < NUM_YIELD_TYPES; iYield++)
								{
									if (aiYieldChanges[iYield] > 0)
									{
										bProvidesBenefit = true;
										break;
									}
								}

								// If the route provides a benefit over the existing route, show the yield changes
								if (bProvidesBenefit)
								{
									GAMETEXT.setYieldChangeHelp(szBuffer, GC.getRouteInfo(eLoopRoute).getDescription(), L": ", L"", aiYieldChanges);
								}
							}
							// MOD - END - Complex Routes
						}
					}

					// MOD - START - Advanced Yield and Commerce Effects
					// MOD - START - Relation Caching
					for (int iRel = 0; iRel < kImprovement.getNumRelatedBuildingClasses(); iRel++)
					{
						BuildingClassTypes eLoopBuildingClass = (BuildingClassTypes)kImprovement.getRelatedBuildingClasses(iRel);
						if (GC.getGameINLINE().isBuildingClassMaxedOut(eLoopBuildingClass) || kBuildTeam.isBuildingClassMaxedOut(eLoopBuildingClass) || kBuildOwner.isBuildingClassMaxedOut(eLoopBuildingClass))
						{
							continue;
						}

						BuildingTypes eLoopBuilding = (BuildingTypes)(GC.getCivilizationInfo(kBuildOwner.getCivilizationType()).getCivilizationBuildings(eLoopBuildingClass));
						if (eLoopBuilding == NO_BUILDING)
						{
							continue;
						}

						if (kBuildTeam.isDiscontinuedBuilding(eLoopBuilding))
						{
							continue;
						}

						bool bPotentialBuilding = false;
						if (pWorkingCity == NULL)
						{
							if (!kBuildTeam.isObsoleteBuilding(eLoopBuilding))
							{
								bPotentialBuilding = true;
							}
						}
						else
						{
							if (!pWorkingCity->isObsoleteBuilding(eLoopBuilding))
							{
								if (pWorkingCity->getNumActiveBuilding(eLoopBuilding) == 0)
								{
									bPotentialBuilding = true;
								}
							}
						}

						if (bPotentialBuilding)
						{
							const CvBuildingInfo& kLoopBuilding = GC.getBuildingInfo(eLoopBuilding);
							const CvBuildingClassInfo& kLoopBuildingClass = GC.getBuildingClassInfo(eLoopBuildingClass);

							if (kLoopBuilding.getNumImprovementYieldChanges() > 0 || kLoopBuilding.getNumGlobalImprovementYieldChanges() > 0)
							{
								int aiYieldChanges[NUM_YIELD_TYPES] = {};

								kLoopBuilding.getImprovementYieldChangeArray(eImprovement, aiYieldChanges);
								kLoopBuilding.getGlobalImprovementYieldChangeArray(eImprovement, aiYieldChanges);

								// MOD - START - Building Replacement
								if (pWorkingCity != NULL)
								{
									for (int iRep = 0; iRep < kLoopBuildingClass.getNumReplacedBuildings(); iRep++)
									{
										BuildingTypes eReplacedBuilding = (BuildingTypes)kLoopBuildingClass.getReplacedBuildings(iRep);
										int iNumBuildings = pWorkingCity->getNumActiveBuilding(eReplacedBuilding);
										if (iNumBuildings > 0)
										{
											const CvBuildingInfo& kReplacedBuilding = GC.getBuildingInfo(eReplacedBuilding);

											for (int iMem = 0; iMem < kReplacedBuilding.getNumImprovementYieldChanges(); iMem++)
											{
												if (kReplacedBuilding.getImprovementYieldChangeImprovement(iMem) == eImprovement)
												{
													aiYieldChanges[kReplacedBuilding.getImprovementYieldChangeYield(iMem)] -= kReplacedBuilding.getImprovementYieldChangeValue(iMem) * iNumBuildings;
												}
											}

											for (int iMem = 0; iMem < kReplacedBuilding.getNumGlobalImprovementYieldChanges(); iMem++)
											{
												if (kReplacedBuilding.getGlobalImprovementYieldChangeImprovement(iMem) == eImprovement)
												{
													aiYieldChanges[kReplacedBuilding.getGlobalImprovementYieldChangeYield(iMem)] -= kReplacedBuilding.getGlobalImprovementYieldChangeValue(iMem) * iNumBuildings;
												}
											}
										}
									}
								}
								// MOD - END - Building Replacement

								GAMETEXT.setYieldChangeHelp(szBuffer, kLoopBuilding.getDescription(), L": ", L"", aiYieldChanges);
							}
						}
					}
					// MOD - END - Relation Caching
					// MOD - END - Advanced Yield and Commerce Effects

					if (kImprovement.getDefenseModifier() != 0)
					{
						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_ACTION_DEFENSE_MODIFIER", kImprovement.getDefenseModifier()));
					}

					if (kImprovement.getImprovementUpgrade() != NO_IMPROVEMENT)
					{
						// MOD - START - Improved Civilopedia
						const CvImprovementInfo& kImprovementUpgrade = GC.getImprovementInfo((ImprovementTypes) kImprovement.getImprovementUpgrade());

						iTurns = pMissionPlot->getUpgradeTimeLeft(eImprovement, pHeadSelectedUnit->getOwnerINLINE());

						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_ACTION_BECOMES_IMP", CvWString(kImprovementUpgrade.getType()).GetCString(), kImprovementUpgrade.getTextKeyWide(), iTurns));
						// MOD - END - Improved Civilopedia
					}
				}

				if (eRoute != NO_ROUTE)
				{
					ImprovementTypes eActualImprovement = eImprovement;

					if (eActualImprovement == NO_IMPROVEMENT)
					{
						eActualImprovement = pMissionPlot->getImprovementType();
					}

					if (eActualImprovement != NO_IMPROVEMENT)
					{
						RouteTypes eOldRoute = pMissionPlot->getRouteType();
						const CvImprovementInfo& kImprovement = GC.getImprovementInfo(eActualImprovement);

						// MOD - START - Complex Routes
						//GAMETEXT.setYieldChangeHelp(szBuffer, GC.getImprovementInfo(eActualImprovement).getDescription(), L": ", L"", GC.getImprovementInfo(eActualImprovement).getRouteYieldChangesArray(eRoute));
						int aiYieldChanges[NUM_YIELD_TYPES] = {};

						// Add the effect of the new route
						for (int iYield = 0; iYield < NUM_YIELD_TYPES; iYield++)
						{
							aiYieldChanges[iYield] += kImprovement.getRouteYieldChanges(eRoute, iYield);
						}

						// Subtract the effect of the old route
						if (eOldRoute != NO_ROUTE && eOldRoute != eRoute)
						{
							for (int iYield = 0; iYield < NUM_YIELD_TYPES; iYield++)
							{
								aiYieldChanges[iYield] -= kImprovement.getRouteYieldChanges(eOldRoute, iYield);
							}
						}

						GAMETEXT.setYieldChangeHelp(szBuffer, kImprovement.getDescription(), L": ", L"", aiYieldChanges);
						// MOD - END - Complex Routes
					}

					iMovementCost = GC.getRouteInfo(eRoute).getMovementCost() + GET_TEAM(pHeadSelectedUnit->getTeam()).getRouteChange(eRoute);
					iFlatMovementCost = GC.getRouteInfo(eRoute).getFlatMovementCost();

					if (iMovementCost > 0)
					{
						iMoves = (GC.getMOVE_DENOMINATOR() / iMovementCost);

						if ((iMoves * iMovementCost) < GC.getMOVE_DENOMINATOR())
						{
							iMoves++;
						}
					}
					else
					{
						iMoves = GC.getMOVE_DENOMINATOR();
					}

					if (iFlatMovementCost > 0)
					{
						iFlatMoves = (GC.getMOVE_DENOMINATOR() / iFlatMovementCost);

						if ((iFlatMoves * iFlatMovementCost) < GC.getMOVE_DENOMINATOR())
						{
							iFlatMoves++;
						}
					}
					else
					{
						iFlatMoves = GC.getMOVE_DENOMINATOR();
					}

					if ((iMoves > 1) || (iFlatMoves > 1))
					{
						if (iMoves >= iFlatMoves)
						{
							szBuffer.append(NEWLINE);
							szBuffer.append(gDLL->getText("TXT_KEY_ACTION_MOVEMENT_COST", iMoves));
						}
						else
						{
							szBuffer.append(NEWLINE);
							szBuffer.append(gDLL->getText("TXT_KEY_ACTION_FLAT_MOVEMENT_COST", iFlatMoves));
						}
					}

					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_ACTION_CONNECTS_RESOURCES"));
				}

				iNowWorkRate = 0;
				iThenWorkRate = 0;

				pSelectedUnitNode = gDLL->getInterfaceIFace()->headSelectionListNode();

				if (NULL != pHeadSelectedUnit)
				{
					if (GET_PLAYER(pHeadSelectedUnit->getOwnerINLINE()).getBuildCost(pMissionPlot, eBuild) > 0)
					{
						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_BUILD_COST", GET_PLAYER(pHeadSelectedUnit->getOwnerINLINE()).getBuildCost(pMissionPlot, eBuild)));
					}
				}

				while (pSelectedUnitNode != NULL)
				{
					pSelectedUnit = ::getUnit(pSelectedUnitNode->m_data);

					if (pSelectedUnit->getBuildType() != eBuild)
					{
						iNowWorkRate += pSelectedUnit->workRate(false);
						iThenWorkRate += pSelectedUnit->workRate(true);
					}

					pSelectedUnitNode = gDLL->getInterfaceIFace()->nextSelectionListNode(pSelectedUnitNode);
				}

				iTurns = pMissionPlot->getBuildTurnsLeft(eBuild, iNowWorkRate, iThenWorkRate);


				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_ACTION_NUM_TURNS", iTurns));

				if (!CvWString(GC.getBuildInfo(eBuild).getHelp()).empty())
				{
					szBuffer.append(CvWString::format(L"%s%s", NEWLINE, GC.getBuildInfo(eBuild).getHelp()).c_str());
				}
			}

			if (!CvWString(GC.getMissionInfo((MissionTypes)(GC.getActionInfo(widgetDataStruct.m_iData1).getMissionType())).getHelp()).empty())
			{
				szBuffer.append(CvWString::format(L"%s%s", NEWLINE, GC.getMissionInfo((MissionTypes)(GC.getActionInfo(widgetDataStruct.m_iData1).getMissionType())).getHelp()).c_str());
			}
		}

		if (GC.getActionInfo(widgetDataStruct.m_iData1).getCommandType() != NO_COMMAND)
		{
			if (GC.getActionInfo(widgetDataStruct.m_iData1).getCommandType() == COMMAND_PROMOTION)
			{
				GAMETEXT.parsePromotionHelp(szBuffer, ((PromotionTypes)(GC.getActionInfo(widgetDataStruct.m_iData1).getCommandData())));
			}
			else if (GC.getActionInfo(widgetDataStruct.m_iData1).getCommandType() == COMMAND_UPGRADE)
			{
				GAMETEXT.setBasicUnitHelp(szBuffer, ((UnitTypes)(GC.getActionInfo(widgetDataStruct.m_iData1).getCommandData())));

				if (bAlt && GC.getCommandInfo((CommandTypes)(GC.getActionInfo(widgetDataStruct.m_iData1).getCommandType())).getAll())
				{
					iPrice = GET_PLAYER(pHeadSelectedUnit->getOwnerINLINE()).upgradeAllPrice(((UnitTypes)(GC.getActionInfo(widgetDataStruct.m_iData1).getCommandData())), pHeadSelectedUnit->getUnitType());
				}
				else
				{
					iPrice = 0;

					pSelectedUnitNode = gDLL->getInterfaceIFace()->headSelectionListNode();

					while (pSelectedUnitNode != NULL)
					{
						pSelectedUnit = ::getUnit(pSelectedUnitNode->m_data);

						if (pSelectedUnit->canUpgrade(((UnitTypes)(GC.getActionInfo(widgetDataStruct.m_iData1).getCommandData())), true))
						{
							iPrice += pSelectedUnit->upgradePrice((UnitTypes)(GC.getActionInfo(widgetDataStruct.m_iData1).getCommandData()));
						}

						pSelectedUnitNode = gDLL->getInterfaceIFace()->nextSelectionListNode(pSelectedUnitNode);
					}
				}

				szTempBuffer.Format(L"%s%d %c", NEWLINE, iPrice, GC.getCommerceInfo(COMMERCE_GOLD).getChar());
				szBuffer.append(szTempBuffer);
			}
			else if (GC.getActionInfo(widgetDataStruct.m_iData1).getCommandType() == COMMAND_GIFT)
			{
				eGiftPlayer = pHeadSelectedUnit->plot()->getOwnerINLINE();

				if (eGiftPlayer != NO_PLAYER)
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_ACTION_GOES_TO_CIV"));

					szTempBuffer.Format(SETCOLR L"%s" ENDCOLR, GET_PLAYER(eGiftPlayer).getPlayerTextColorR(), GET_PLAYER(eGiftPlayer).getPlayerTextColorG(), GET_PLAYER(eGiftPlayer).getPlayerTextColorB(), GET_PLAYER(eGiftPlayer).getPlayerTextColorA(), GET_PLAYER(eGiftPlayer).getCivilizationShortDescription());
					szBuffer.append(szTempBuffer);

					pSelectedUnitNode = gDLL->getInterfaceIFace()->headSelectionListNode();

					while (pSelectedUnitNode != NULL)
					{
						pSelectedUnit = ::getUnit(pSelectedUnitNode->m_data);

						if (!(GET_PLAYER(eGiftPlayer).AI_acceptUnit(pSelectedUnit)))
						{
							szBuffer.append(NEWLINE);
							szBuffer.append(gDLL->getText("TXT_KEY_REFUSE_GIFT", GET_PLAYER(eGiftPlayer).getNameKey()));
							break;
						}

						pSelectedUnitNode = gDLL->getInterfaceIFace()->nextSelectionListNode(pSelectedUnitNode);
					}
				}
			}

			if (GC.getCommandInfo((CommandTypes)(GC.getActionInfo(widgetDataStruct.m_iData1).getCommandType())).getAll())
			{
				szBuffer.append(gDLL->getText("TXT_KEY_ACTION_ALL_UNITS"));
			}

			if (!CvWString(GC.getCommandInfo((CommandTypes)(GC.getActionInfo(widgetDataStruct.m_iData1).getCommandType())).getHelp()).empty())
			{
				szBuffer.append(CvWString::format(L"%s%s", NEWLINE, GC.getCommandInfo((CommandTypes)(GC.getActionInfo(widgetDataStruct.m_iData1).getCommandType())).getHelp()).c_str());
			}
		}

		if (GC.getActionInfo(widgetDataStruct.m_iData1).getAutomateType() != NO_AUTOMATE)
		{
			if (!CvWString(GC.getAutomateInfo((ControlTypes)(GC.getActionInfo(widgetDataStruct.m_iData1).getAutomateType())).getHelp()).empty())
			{
				szBuffer.append(CvWString::format(L"%s%s", NEWLINE, GC.getAutomateInfo((ControlTypes)(GC.getActionInfo(widgetDataStruct.m_iData1).getAutomateType())).getHelp()).c_str());
			}
		}
	}

	if (GC.getActionInfo(widgetDataStruct.m_iData1).getControlType() != NO_CONTROL)
	{
		if (!CvWString(GC.getControlInfo((ControlTypes)(GC.getActionInfo(widgetDataStruct.m_iData1).getControlType())).getHelp()).empty())
		{
			szBuffer.append(CvWString::format(L"%s%s", NEWLINE, GC.getControlInfo((ControlTypes)(GC.getActionInfo(widgetDataStruct.m_iData1).getControlType())).getHelp()).c_str());
		}
	}

	if (GC.getActionInfo(widgetDataStruct.m_iData1).getInterfaceModeType() != NO_INTERFACEMODE)
	{
		if (!CvWString(GC.getInterfaceModeInfo((InterfaceModeTypes)(GC.getActionInfo(widgetDataStruct.m_iData1).getInterfaceModeType())).getHelp()).empty())
		{
			szBuffer.append(CvWString::format(L"%s%s", NEWLINE, GC.getInterfaceModeInfo((InterfaceModeTypes)(GC.getActionInfo(widgetDataStruct.m_iData1).getInterfaceModeType())).getHelp()).c_str());
		}
	}
}


void CvDLLWidgetData::parseCitizenHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity;
	int iCount;
	int iI;

	pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity != NULL)
	{
		if (widgetDataStruct.m_iData1 != NO_SPECIALIST)
		{
			GAMETEXT.parseSpecialistHelp(szBuffer, ((SpecialistTypes)(widgetDataStruct.m_iData1)), pHeadSelectedCity);

			if (widgetDataStruct.m_iData2 != -1)
			{
				iCount = 0;

				for (iI = 0; iI < GC.getNumSpecialistInfos(); iI++)
				{
					if (iI < widgetDataStruct.m_iData1)
					{
						iCount += pHeadSelectedCity->getSpecialistCount((SpecialistTypes)iI);
					}
					else if (iI == widgetDataStruct.m_iData1)
					{
						iCount += widgetDataStruct.m_iData2;
					}
				}

				if (iCount < pHeadSelectedCity->totalFreeSpecialists())
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_MISC_FREE_SPECIALIST"));
				}
			}
		}
	}
}


void CvDLLWidgetData::parseFreeCitizenHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity != NULL)
	{
		SpecialistTypes eSpecialist = (SpecialistTypes)widgetDataStruct.m_iData1;
		if (NO_SPECIALIST != eSpecialist)
		{
			GAMETEXT.parseSpecialistHelp(szBuffer, eSpecialist, pHeadSelectedCity);
		}
		if (widgetDataStruct.m_iData2 != -1)
		{
			szBuffer.append(SEPARATOR);
			GAMETEXT.parseFreeSpecialistHelp(szBuffer, *pHeadSelectedCity);
		}
	}
}


void CvDLLWidgetData::parseDisabledCitizenHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity;
	CvWString szTempBuffer;
	BuildingTypes eLoopBuilding;
	bool bFirst;
	int iI;

	pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity != NULL)
	{
		if (widgetDataStruct.m_iData1 != NO_SPECIALIST)
		{
			GAMETEXT.parseSpecialistHelp(szBuffer, ((SpecialistTypes)(widgetDataStruct.m_iData1)), pHeadSelectedCity);

			if (!(pHeadSelectedCity->isSpecialistValid(((SpecialistTypes)(widgetDataStruct.m_iData1)), 1)))
			{
				bFirst = true;

				for (iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
				{
					eLoopBuilding = (BuildingTypes)GC.getCivilizationInfo(GC.getGameINLINE().getActiveCivilizationType()).getCivilizationBuildings(iI);

					if (eLoopBuilding != NO_BUILDING)
					{
						if (GC.getBuildingInfo(eLoopBuilding).getSpecialistCount(widgetDataStruct.m_iData1) > 0)
						{
							if ((pHeadSelectedCity->getNumBuilding(eLoopBuilding) <= 0) && !isLimitedWonderClass((BuildingClassTypes)iI))
							{
								if ((GC.getBuildingInfo(eLoopBuilding).getSpecialBuildingType() == NO_SPECIALBUILDING) || pHeadSelectedCity->canConstruct(eLoopBuilding))
								{
									szTempBuffer = NEWLINE + gDLL->getText("TXT_KEY_REQUIRES");
									setListHelp(szBuffer, szTempBuffer, GC.getBuildingInfo(eLoopBuilding).getDescription(), gDLL->getText("TXT_KEY_OR").c_str(), bFirst);
									bFirst = false;
								}
							}
						}
					}
				}

				if (!bFirst)
				{
					szBuffer.append(ENDCOLR);
				}
			}
		}
	}
}


void CvDLLWidgetData::parseAngryCitizenHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity;

	pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity != NULL)
	{
		szBuffer.assign(gDLL->getText("TXT_KEY_MISC_ANGRY_CITIZEN"));
		szBuffer.append(NEWLINE);

		GAMETEXT.setAngerHelp(szBuffer, *pHeadSelectedCity);
	}
}


void CvDLLWidgetData::parseChangeSpecialistHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity;

	pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity != NULL)
	{
		if (widgetDataStruct.m_iData2 > 0)
		{
			GAMETEXT.parseSpecialistHelp(szBuffer, ((SpecialistTypes)(widgetDataStruct.m_iData1)), pHeadSelectedCity);

			if (widgetDataStruct.m_iData1 != GC.getDefineINT("DEFAULT_SPECIALIST"))
			{
				if (!(GET_PLAYER(pHeadSelectedCity->getOwnerINLINE()).isSpecialistValid((SpecialistTypes)(widgetDataStruct.m_iData1))))
				{
					if (pHeadSelectedCity->getMaxSpecialistCount((SpecialistTypes)(widgetDataStruct.m_iData1)) > 0)
					{
						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_MISC_MAX_SPECIALISTS", pHeadSelectedCity->getMaxSpecialistCount((SpecialistTypes)(widgetDataStruct.m_iData1))));
					}
				}
			}
		}
		else
		{
			szBuffer.assign(gDLL->getText("TXT_KEY_MISC_REMOVE_SPECIALIST", GC.getSpecialistInfo((SpecialistTypes) widgetDataStruct.m_iData1).getTextKeyWide()));

			if (pHeadSelectedCity->getForceSpecialistCount((SpecialistTypes)(widgetDataStruct.m_iData1)) > 0)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_FORCED_SPECIALIST", pHeadSelectedCity->getForceSpecialistCount((SpecialistTypes)(widgetDataStruct.m_iData1))));
			}
		}
	}
}


void CvDLLWidgetData::parseResearchHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	// MOD - START - Improved Help Context
	/*
	TechTypes eTech;

	eTech = ((TechTypes)(widgetDataStruct.m_iData1));

	if (eTech == NO_TECH)
	{
		//	No Technology
		if (GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getCurrentResearch() != NO_TECH)
		{
			CvGameAI& game = GC.getGameINLINE();
			CvPlayer& activePlayer = GET_PLAYER(game.getActivePlayer());
			szBuffer.assign(gDLL->getText("TXT_KEY_MISC_CHANGE_RESEARCH"));
			szBuffer.append(NEWLINE);
			GAMETEXT.setTechHelp(szBuffer, activePlayer.getCurrentResearch(), false, true, false, false);
		}
	}
	else
	{
		GAMETEXT.setTechHelp(szBuffer, eTech, false, true, widgetDataStruct.m_bOption, false);
	}
	*/
	FAssert(GC.getGameINLINE().getActivePlayer() != NO_PLAYER);

	TechTypes eTech = ((TechTypes)(widgetDataStruct.m_iData1));
	PlayerTypes ePlayer = GC.getGameINLINE().getActivePlayer();
	const CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);
	CivilizationTypes eCivilization = kPlayer.getCivilizationType();

	if (eTech == NO_TECH)
	{
		// No technology specified - use current research
		eTech = kPlayer.getCurrentResearch();

		if (eTech != NO_TECH)
		{
			szBuffer.assign(gDLL->getText("TXT_KEY_MISC_CHANGE_RESEARCH"));
			szBuffer.append(NEWLINE);
			GAMETEXT.setTechHelp(szBuffer, eTech, eCivilization, ePlayer, false, false, false);
		}
		// MOD - END - Improved Help Context
	}
	else
	{
		GAMETEXT.setTechHelp(szBuffer, eTech, eCivilization, ePlayer, false, widgetDataStruct.m_bOption, false);
	}
}


void CvDLLWidgetData::parseTechTreeHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	// MOD - START - Improved Help Context
	//GAMETEXT.setTechHelp(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)), false, false, false, true);
	CivilizationTypes eCivilization = (CivilizationTypes)getPackedWordID1(widgetDataStruct.m_iData2);
	PlayerTypes ePlayer = (PlayerTypes)getPackedWordID2(widgetDataStruct.m_iData2);

	GAMETEXT.setTechHelp(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)), eCivilization, ePlayer, false, false, true);
	// MOD - END - Improved Help Context
}


void CvDLLWidgetData::parseChangePercentHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 > 0)
	{
		szBuffer.assign(gDLL->getText("TXT_KEY_MISC_INCREASE_RATE", GC.getCommerceInfo((CommerceTypes) widgetDataStruct.m_iData1).getTextKeyWide(), widgetDataStruct.m_iData2));
	}
	else
	{
		szBuffer.assign(gDLL->getText("TXT_KEY_MISC_DECREASE_RATE", GC.getCommerceInfo((CommerceTypes) widgetDataStruct.m_iData1).getTextKeyWide(), -(widgetDataStruct.m_iData2)));
	}
}


void CvDLLWidgetData::parseContactCivHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	// do not execute if player is out of range
	PlayerTypes ePlayer = (PlayerTypes) widgetDataStruct.m_iData1;
	if (ePlayer >= MAX_PLAYERS)
	{
		return;
	}

	// do not execute if player is not a real civ
	CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);
	if (kPlayer.getCivilizationType() == NO_CIVILIZATION)
	{
		return;
	}

	// make sure its empty to start
	szBuffer.clear();

	TeamTypes eTeam = (TeamTypes) kPlayer.getTeam();
	CvTeamAI& kTeam = GET_TEAM(eTeam);

	PlayerTypes eActivePlayer = GC.getGameINLINE().getActivePlayer();
	TeamTypes eActiveTeam = (TeamTypes) GET_PLAYER(eActivePlayer).getTeam();
	CvTeamAI& kActiveTeam = GET_TEAM(eActiveTeam);

	// if alt down and cheat on, show extra info
	if (GC.altKey() && gDLL->getChtLvl() > 0)
	{
		// K-Mod. I've moved the code from here into its own function, just to get it out of the way.
		parseScoreboardCheatText(widgetDataStruct, szBuffer);
		// K-Mod end
	}

	//	Show score info instead if we are trying to contact ourselves...
	if ( eActivePlayer == ePlayer || (GC.ctrlKey() && gDLL->getChtLvl() > 0) )
	{
		parseScoreHelp(widgetDataStruct, szBuffer);
		return;
	}

	/* original bts code
	szBuffer.append(gDLL->getText("TXT_KEY_MISC_CONTACT_LEADER", kPlayer.getNameKey(), kPlayer.getCivilizationShortDescription()));
	szBuffer.append(NEWLINE);
	GAMETEXT.parsePlayerTraits(szBuffer, ePlayer); */

	if (!kActiveTeam.isHasMet(eTeam))
	{
		// K-Mod. If we haven't met the player yet - don't say "contact". Because we can't actually contact them!
		szBuffer.append(CvWString::format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), kPlayer.getName()));
		// K-Mod end
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HAVENT_MET_CIV"));
	}
	// K-Mod
	else
	{
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_CONTACT_LEADER", kPlayer.getNameKey(), kPlayer.getCivilizationShortDescription()));
		szBuffer.append(NEWLINE);
		GAMETEXT.parsePlayerTraits(szBuffer, ePlayer);
	}
	// K-Mod end

	if (kActiveTeam.isHasMet(eTeam) || GC.getGameINLINE().isDebugMode())
	{
		/* original bts code
		if (!kPlayer.isHuman())
		{
			if (!kPlayer.AI_isWillingToTalk(eActivePlayer))
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_REFUSES_TO_TALK"));
			}

			if (!((GC.altKey() || GC.ctrlKey()) && gDLL->getChtLvl() > 0))
			{
				szBuffer.append(NEWLINE);
				GAMETEXT.getAttitudeString(szBuffer, ePlayer, eActivePlayer);

				szBuffer.append(NEWLINE);
				GAMETEXT.getEspionageString(szBuffer, ((PlayerTypes)widgetDataStruct.m_iData1), eActivePlayer);
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_CTRL_TRADE"));
			}
		}
		else
		{
			szBuffer.append(NEWLINE);
			GAMETEXT.getEspionageString(szBuffer, ((PlayerTypes)widgetDataStruct.m_iData1), eActivePlayer);
		} */
		// K-Mod
		if (!kPlayer.AI_isWillingToTalk(eActivePlayer))
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_REFUSES_TO_TALK"));
		}
		if (!((GC.altKey() || GC.ctrlKey()) && gDLL->getChtLvl() > 0))
		{
			GAMETEXT.getAttitudeString(szBuffer, ePlayer, eActivePlayer);
			GAMETEXT.getWarWearinessString(szBuffer, ePlayer, eActivePlayer); // K-Mod

			if (!kPlayer.isHuman() && kPlayer.AI_isWillingToTalk(eActivePlayer))
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_CTRL_TRADE"));
			}
		}
		// K-Mod end

		if (eTeam != eActiveTeam )
		{
			// Show which civs this player is at war with
			CvWStringBuffer szWarWithString;
			CvWStringBuffer szWorstEnemyString;
			bool bFirst = true;
			bool bFirst2 = true;
			for (int iTeam = 0; iTeam < MAX_CIV_TEAMS; ++iTeam)
			{
				CvTeamAI& kTeam = GET_TEAM((TeamTypes) iTeam);
				//if (kTeam.isAlive() && !kTeam.isMinorCiv() && iTeam != eActiveTeam && iTeam != GET_PLAYER(ePlayer).getTeam())
				if (kTeam.isAlive() && !kTeam.isMinorCiv() && iTeam != GET_PLAYER(ePlayer).getTeam()) // K-Mod. show "at war" for the active player if appropriate
				{
					if (kActiveTeam.isHasMet(kTeam.getID()))
					{
						if (::atWar((TeamTypes) iTeam, GET_PLAYER(ePlayer).getTeam()))
						{
							setListHelp(szWarWithString, L"", kTeam.getName().GetCString(), L", ", bFirst);
							bFirst = false;
						}

						//if (kTeam.AI_getWorstEnemy() == GET_PLAYER(ePlayer).getTeam())
						if (!kTeam.isHuman() && kTeam.AI_getWorstEnemy() == GET_PLAYER(ePlayer).getTeam()) // K-Mod
						{
							setListHelp(szWorstEnemyString, L"", kTeam.getName().GetCString(), L", ", bFirst2);
							bFirst2 = false;
						}
					}
				}
			}

			if( !szWorstEnemyString.isEmpty() )
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText(L"TXT_KEY_WORST_ENEMY_OF", szWorstEnemyString.getCString()));
			}
			if( !szWarWithString.isEmpty() )
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText(L"TXT_KEY_AT_WAR_WITH", szWarWithString.getCString()));
			}

			if( !(kActiveTeam.isAtWar(eTeam)))
			{
				if (kActiveTeam.canDeclareWar(eTeam))
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_MISC_ALT_DECLARE_WAR"));
				}
				else
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_MISC_CANNOT_DECLARE_WAR"));
				}

				// K-Mod. The BBAI war plan control currently is not implemented for multiplayer, and it is only relevant for team games.
				if (!GC.getGameINLINE().isGameMultiPlayer() && kActiveTeam.getAliveCount() > 1)
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_MISC_SHIFT_ALT_PREPARE_WAR"));
				}
			}
		}
	}

	if (kPlayer.isHuman())
	{
//		szBuffer += "\n(<SHIFT> to Send Chat Message)";
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_SHIFT_SEND_CHAT"));
	}
}



// K-Mod. The cheat mode text associated with parseContactCivHelp.
void CvDLLWidgetData::parseScoreboardCheatText(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	PlayerTypes ePlayer = (PlayerTypes) widgetDataStruct.m_iData1;
	FAssert(ePlayer != NO_PLAYER);

	const CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);

	TeamTypes eTeam = (TeamTypes) kPlayer.getTeam();
	const CvTeamAI& kTeam = GET_TEAM(eTeam);

	PlayerTypes eActivePlayer = GC.getGameINLINE().getActivePlayer();

	// Show tech percent adjust
	szBuffer.append(CvWString::format(SETCOLR L"TechPercent: %d%%, CurResMod: %d%%" ENDCOLR, TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), kTeam.getBestKnownTechScorePercent(), kPlayer.calculateResearchModifier(kPlayer.getCurrentResearch()) ));
	szBuffer.append(NEWLINE);
	szBuffer.append(NEWLINE);

	szBuffer.append("Power");
	szBuffer.append(NEWLINE);
	// show everyones power for the active player
	if (eActivePlayer == ePlayer)
	{
		for (int iI = 0; iI < MAX_PLAYERS; iI++)
		{
			CvPlayer& kLoopPlayer = GET_PLAYER((PlayerTypes)iI);
			if (kLoopPlayer.isAlive())
			{
				CvTeam& kLoopTeam = GET_TEAM((TeamTypes) kLoopPlayer.getTeam());

				szBuffer.append(CvWString::format(SETCOLR L"%s (team%d): %d (%d def)" ENDCOLR, TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"),
					kLoopPlayer.getName(), kLoopPlayer.getTeam(), kLoopPlayer.getPower(), kLoopTeam.getDefensivePower()));
				
				// if in financial trouble, show that
				if (kLoopPlayer.AI_isFinancialTrouble())
				{
					szBuffer.append(CvWString::format(SETCOLR L" ($$$)" ENDCOLR, TEXT_COLOR("COLOR_NEGATIVE_TEXT")));
				}

				szBuffer.append(NEWLINE);
			}
		}
	}
	// only should this one power if not active player
	else
	{
		szBuffer.append(CvWString::format(SETCOLR L"%d (%d) power" ENDCOLR, TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), kPlayer.getPower(), kTeam.getPower(true)));
		
		// if in financial trouble, show that
		if (kPlayer.AI_isFinancialTrouble())
		{
			szBuffer.append(CvWString::format(SETCOLR L" ($$$)" ENDCOLR, TEXT_COLOR("COLOR_NEGATIVE_TEXT")));
		}

		szBuffer.append(NEWLINE);
	}

	// Strategies
	CvWString szTempBuffer;
	szTempBuffer.Format(L"");

	//kPlayer.AI_getStrategyHash();
	if (kPlayer.AI_isDoStrategy(AI_STRATEGY_DAGGER))
    {
        szTempBuffer.Format(L"Dagger, ");
        szBuffer.append(szTempBuffer);
    }
	if (kPlayer.AI_isDoStrategy(AI_STRATEGY_CRUSH))
    {
        szTempBuffer.Format(L"Crush, ");
        szBuffer.append(szTempBuffer);
	}
	if (kPlayer.AI_isDoStrategy(AI_STRATEGY_ALERT1))
    {
        szTempBuffer.Format(L"Alert1, ");
        szBuffer.append(szTempBuffer);
	}
	if (kPlayer.AI_isDoStrategy(AI_STRATEGY_ALERT2))
    {
        szTempBuffer.Format(L"Alert2, ");
        szBuffer.append(szTempBuffer);
	}
	if (kPlayer.AI_isDoStrategy(AI_STRATEGY_TURTLE))
    {
        szTempBuffer.Format(L"Turtle, ");
        szBuffer.append(szTempBuffer);
	}
	if (kPlayer.AI_isDoStrategy(AI_STRATEGY_LAST_STAND))
    {
        szTempBuffer.Format(L"LastStand, ");
        szBuffer.append(szTempBuffer);
	}
	if (kPlayer.AI_isDoStrategy(AI_STRATEGY_FINAL_WAR))
    {
        szTempBuffer.Format(L"FinalWar, ");
        szBuffer.append(szTempBuffer);
    }
    if (kPlayer.AI_isDoStrategy(AI_STRATEGY_GET_BETTER_UNITS))
    {
        szTempBuffer.Format(L"GetBetterUnits, ");
        szBuffer.append(szTempBuffer);
    }
	if (kPlayer.AI_isDoStrategy(AI_STRATEGY_PRODUCTION))
    {
        szTempBuffer.Format(L"Production, ");
        szBuffer.append(szTempBuffer);
    }
	if (kPlayer.AI_isDoStrategy(AI_STRATEGY_MISSIONARY))
    {
        szTempBuffer.Format(L"Missionary, ");
        szBuffer.append(szTempBuffer);
    }
	if (kPlayer.AI_isDoStrategy(AI_STRATEGY_BIG_ESPIONAGE))
    {
        szTempBuffer.Format(L"BigEspionage, ");
        szBuffer.append(szTempBuffer);
    }
	if (kPlayer.AI_isDoStrategy(AI_STRATEGY_ECONOMY_FOCUS)) // K-Mod
	{
		szTempBuffer.Format(L"EconomyFocus, ");
		szBuffer.append(szTempBuffer);
	}
	if (kPlayer.AI_isDoStrategy(AI_STRATEGY_ESPIONAGE_ECONOMY)) // K-Mod
	{
		szTempBuffer.Format(L"EspionageEconomy, ");
		szBuffer.append(szTempBuffer);
	}

	szBuffer.append(NEWLINE);
	szTempBuffer.Format(L"Vic Strats: ");
    szBuffer.append(szTempBuffer);

	szTempBuffer.Format(L"");
	// Victory strategies

	kPlayer.AI_getVictoryStrategyHash();
	if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_CULTURE1))
    {
        szTempBuffer.Format(L"Culture1, ");
        szBuffer.append(szTempBuffer);
    }
    if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_CULTURE2))
    {
        szTempBuffer.Format(L"Culture2, ");
        szBuffer.append(szTempBuffer);
    }
    if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_CULTURE3))
    {
        szTempBuffer.Format(L"Culture3, ");
        szBuffer.append(szTempBuffer);
    }
	if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_CULTURE4))
    {
        szTempBuffer.Format(L"Culture4, ");
        szBuffer.append(szTempBuffer);
    }

	if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_SPACE1))
    {
        szTempBuffer.Format(L"Space1, ");
        szBuffer.append(szTempBuffer);
    }
    if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_SPACE2))
    {
        szTempBuffer.Format(L"Space2, ");
        szBuffer.append(szTempBuffer);
    }
    if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_SPACE3))
    {
        szTempBuffer.Format(L"Space3, ");
        szBuffer.append(szTempBuffer);
    }
	if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_SPACE4))
    {
        szTempBuffer.Format(L"Space4, ");
        szBuffer.append(szTempBuffer);
    }

	if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_CONQUEST1))
    {
        szTempBuffer.Format(L"Conq1, ");
        szBuffer.append(szTempBuffer);
    }
    if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_CONQUEST2))
    {
        szTempBuffer.Format(L"Conq2, ");
        szBuffer.append(szTempBuffer);
    }
	if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_CONQUEST3))
    {
        szTempBuffer.Format(L"Conq3, ");
        szBuffer.append(szTempBuffer);
    }
	if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_CONQUEST4))
    {
        szTempBuffer.Format(L"Conq4, ");
        szBuffer.append(szTempBuffer);
    }

	if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_DOMINATION1))
    {
        szTempBuffer.Format(L"Dom1, ");
        szBuffer.append(szTempBuffer);
    }
    if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_DOMINATION2))
    {
        szTempBuffer.Format(L"Dom2, ");
        szBuffer.append(szTempBuffer);
    }
	if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_DOMINATION3))
    {
        szTempBuffer.Format(L"Dom3, ");
        szBuffer.append(szTempBuffer);
    }
	if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_DOMINATION4))
    {
        szTempBuffer.Format(L"Dom4, ");
        szBuffer.append(szTempBuffer);
    }

	if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_DIPLOMACY1))
    {
        szTempBuffer.Format(L"Diplo1, ");
        szBuffer.append(szTempBuffer);
    }
    if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_DIPLOMACY2))
    {
        szTempBuffer.Format(L"Diplo2, ");
        szBuffer.append(szTempBuffer);
    }
	if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_DIPLOMACY3))
    {
        szTempBuffer.Format(L"Diplo3, ");
        szBuffer.append(szTempBuffer);
    }
	if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_DIPLOMACY4))
    {
        szTempBuffer.Format(L"Diplo4, ");
        szBuffer.append(szTempBuffer);
    }

	// List the top 3 culture cities (by culture value weight).
	//if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_CULTURE1))
	{
		szBuffer.append(CvWString::format(L"\n\nTop %c cities by weight:", GC.getCommerceInfo(COMMERCE_CULTURE).getChar()));
		int iLegendaryCulture = GC.getGame().getCultureThreshold((CultureLevelTypes)(GC.getNumCultureLevelInfos() - 1));
		std::vector<std::pair<int,int> > city_list; // (weight, city id)

		int iLoop;
		for (CvCity* pLoopCity = kPlayer.firstCity(&iLoop); pLoopCity != NULL; pLoopCity = kPlayer.nextCity(&iLoop))
			city_list.push_back(std::make_pair(kPlayer.AI_commerceWeight(COMMERCE_CULTURE, pLoopCity), pLoopCity->getID()));

		int iListCities = std::min((int)city_list.size(), 3);
		std::partial_sort(city_list.begin(), city_list.begin()+iListCities, city_list.end(), std::greater<std::pair<int,int> >());

		int iGoldCommercePercent = kPlayer.AI_estimateBreakEvenGoldPercent();

		for (int i = 0; i < iListCities; i++)
		{
			CvCity* pLoopCity = kPlayer.getCity(city_list[i].second);
			int iEstimatedRate = pLoopCity->getCommerceRate(COMMERCE_CULTURE);
			iEstimatedRate += (100 - iGoldCommercePercent - kPlayer.getCommercePercent(COMMERCE_CULTURE)) * pLoopCity->getYieldRate(YIELD_COMMERCE) * pLoopCity->getTotalCommerceRateModifier(COMMERCE_CULTURE) / 10000;
			int iCountdown = (iLegendaryCulture - pLoopCity->getCulture(kPlayer.getID())) / std::max(1, iEstimatedRate);

			szBuffer.append(CvWString::format(L"\n  %s:\t%d%%, %d turns", pLoopCity->getName().GetCString(), city_list[i].first, iCountdown));
		}
		szBuffer.append(CvWString::format(L"\n(assuming %d%% gold)", iGoldCommercePercent));
	}

	// skip a line
	szBuffer.append(NEWLINE);
	szBuffer.append(NEWLINE);

	// show peace values
	bool bHadAny = false;
	bool bFirst = true;
	for (int iTeamIndex = 0; iTeamIndex < MAX_TEAMS; iTeamIndex++)
	{
		TeamTypes eLoopTeam = (TeamTypes) iTeamIndex;
		CvTeamAI& kLoopTeam = GET_TEAM(eLoopTeam);
		if (eLoopTeam != eTeam && kLoopTeam.isAlive() && !kLoopTeam.isBarbarian() && !kLoopTeam.isMinorCiv())
		{
			if (kTeam.isAtWar(eLoopTeam))
			{
				if (bFirst)
				{
					szBuffer.append(CvWString::format(SETCOLR L"Current War:\n" ENDCOLR, TEXT_COLOR("COLOR_UNIT_TEXT")));
					bFirst = false;
				}

				bHadAny = true;
				
				WarPlanTypes eWarPlan = kTeam.AI_getWarPlan(eLoopTeam);
				CvWStringBuffer szWarplan;
				GAMETEXT.getWarplanString(szWarplan, eWarPlan);

				int iOtherValue = kTeam.AI_endWarVal(eLoopTeam);
				int iTheirValue = kLoopTeam.AI_endWarVal(eTeam);
				
				szBuffer.append( CvWString::format(SETCOLR L" %s " ENDCOLR SETCOLR L"(%d, %d)" ENDCOLR SETCOLR L" with %s " ENDCOLR  SETCOLR L"(%d, %d)\n" ENDCOLR, 
					TEXT_COLOR((iOtherValue < iTheirValue) ? "COLOR_POSITIVE_TEXT" : "COLOR_NEGATIVE_TEXT"),
					szWarplan.getCString(),
					TEXT_COLOR((iOtherValue < iTheirValue) ? "COLOR_POSITIVE_TEXT" : "COLOR_NEGATIVE_TEXT"),
					iOtherValue, kTeam.AI_getWarSuccess(eLoopTeam),
					TEXT_COLOR((iOtherValue < iTheirValue) ? "COLOR_POSITIVE_TEXT" : "COLOR_NEGATIVE_TEXT"),
					kLoopTeam.getName().GetCString(),
					TEXT_COLOR((iTheirValue < iOtherValue) ? "COLOR_POSITIVE_TEXT" : "COLOR_NEGATIVE_TEXT"),
					iTheirValue, kLoopTeam.AI_getWarSuccess(eTeam)) );
			}
		}
	}
	
	// double space if had any war
	if( kTeam.getAnyWarPlanCount(true) > 0 )
	{
		int iEnemyPowerPercent = kTeam.AI_getEnemyPowerPercent();
		szBuffer.append(CvWString::format(SETCOLR L"\nEnemy Power Percent: %d" ENDCOLR, TEXT_COLOR((iEnemyPowerPercent < 100) ? "COLOR_POSITIVE_TEXT" : "COLOR_NEGATIVE_TEXT"), iEnemyPowerPercent));
		
	}
	if (bHadAny)
	{
		int iWarSuccessRating = kTeam.AI_getWarSuccessRating();
		szBuffer.append(CvWString::format(SETCOLR L"\nWar Success Ratio: %d" ENDCOLR, TEXT_COLOR((iWarSuccessRating > 0) ? "COLOR_POSITIVE_TEXT" : "COLOR_NEGATIVE_TEXT"), iWarSuccessRating));
	}
	if (bHadAny || kTeam.getAnyWarPlanCount(true) > 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(NEWLINE);
	}
	
	// show warplan values
	bHadAny = false;
	bFirst = true;
	for (int iTeamIndex = 0; iTeamIndex < MAX_TEAMS; iTeamIndex++)
	{
		TeamTypes eLoopTeam = (TeamTypes) iTeamIndex;
		CvTeamAI& kLoopTeam = GET_TEAM(eLoopTeam);
		if (eLoopTeam != eTeam && kLoopTeam.isAlive() && !kLoopTeam.isBarbarian())
		{
			WarPlanTypes eWarPlan = kTeam.AI_getWarPlan(eLoopTeam);
			if (!kTeam.isAtWar(eLoopTeam) && eWarPlan != NO_WARPLAN)
			{
				if (bFirst)
				{
					szBuffer.append(CvWString::format(SETCOLR L"Imminent War:\n" ENDCOLR, TEXT_COLOR("COLOR_UNIT_TEXT")));
					bFirst = false;
				}

				bHadAny = true;

				CvWStringBuffer szWarplan;
				GAMETEXT.getWarplanString(szWarplan, eWarPlan);
				szBuffer.append(CvWString::format(SETCOLR L" %s (%d) with %s\n" ENDCOLR, TEXT_COLOR("COLOR_NEGATIVE_TEXT"),
					szWarplan.getCString(), 
					kTeam.AI_startWarVal(eLoopTeam, eWarPlan), // K-Mod
					kLoopTeam.getName().GetCString()));

			}
		}
	}
	
	// double space if had any war plan
	if (bHadAny)
	{
		szBuffer.append(NEWLINE);
	}
	
	// calculate war percentages
	float fOverallWarPercentage = 0;
	bool bAggressive = GC.getGameINLINE().isOption(GAMEOPTION_AGGRESSIVE_AI);
	
	bool bIsAnyCapitalAreaAlone = kTeam.AI_isAnyCapitalAreaAlone();

	int iFinancialTroubleCount = 0;
	int iDaggerCount = 0;
	int iGetBetterUnitsCount = 0;
	for (int iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() == eTeam)
			{
				if ( GET_PLAYER((PlayerTypes)iI).AI_isDoStrategy(AI_STRATEGY_DAGGER)
					|| GET_PLAYER((PlayerTypes)iI).AI_isDoVictoryStrategy(AI_VICTORY_CONQUEST3)
					|| GET_PLAYER((PlayerTypes)iI).AI_isDoVictoryStrategy(AI_VICTORY_DOMINATION4) )
				{
					iDaggerCount++;
					bAggressive = true;
				}

				if (GET_PLAYER((PlayerTypes)iI).AI_isDoStrategy(AI_STRATEGY_GET_BETTER_UNITS))
				{
					iGetBetterUnitsCount++;
				}
				
				if (GET_PLAYER((PlayerTypes)iI).AI_isFinancialTrouble())
				{
					iFinancialTroubleCount++;
				}
			}
		}
	}

	// calculate unit spending for use in iTotalWarRandThreshold
	int iNumMembers = kTeam.getNumMembers();

	// if random in this range is 0, we go to war of this type (so lower numbers are higher probablity)
	// average of everyone on our team
	int iTotalWarRand;
    int iLimitedWarRand;
    int iDogpileWarRand;
	kTeam.AI_getWarRands( iTotalWarRand, iLimitedWarRand, iDogpileWarRand );

	int iTotalWarThreshold;
	int iLimitedWarThreshold;
	int iDogpileWarThreshold;
	kTeam.AI_getWarThresholds( iTotalWarThreshold, iLimitedWarThreshold, iDogpileWarThreshold );

	// we oppose war if half the non-dagger teammates in financial trouble
	bool bFinancesOpposeWar = false;
	if ((iFinancialTroubleCount - iDaggerCount) >= std::max(1, kTeam.getNumMembers() / 2 ))
	{
		// this can be overridden by by the pro-war booleans
		bFinancesOpposeWar = true;
	}

	// if agressive, we may start a war to get money
	bool bFinancesProTotalWar = false;
	bool bFinancesProLimitedWar = false;
	bool bFinancesProDogpileWar = false;
	if (iFinancialTroubleCount > 0)
	{
		// do we like all out wars?
		if (iDaggerCount > 0 || iTotalWarRand < 100)
		{
			bFinancesProTotalWar = true;
		}

		// do we like limited wars?
		if (iLimitedWarRand < 100)
		{
			bFinancesProLimitedWar = true;
		}
		
		// do we like dogpile wars?
		if (iDogpileWarRand < 100)
		{
			bFinancesProDogpileWar = true;
		}
	}
	bool bFinancialProWar = (bFinancesProTotalWar || bFinancesProLimitedWar || bFinancesProDogpileWar);
	
	// overall war check (quite frequently true)
	if ((iGetBetterUnitsCount - iDaggerCount) * 3 < iNumMembers * 2)
	{
		if (bFinancialProWar || !bFinancesOpposeWar)
		{
			fOverallWarPercentage = (float)std::min(100, GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAIDeclareWarProb());
		}
	}

	// team power (if agressive, we use higher value)
	int iTeamPower = kTeam.getPower(true);
	if (bAggressive && kTeam.getAnyWarPlanCount(true) == 0)
	{
		iTeamPower *= 4;
		iTeamPower /= 3;
	}

	iTeamPower *= (100 - kTeam.AI_getEnemyPowerPercent());
	iTeamPower /= 100;
	
	// we will put the values into an array, then sort it for display
	int iBestPossibleMaxWarPass = MAX_INT;
	struct CvStartWarInfo
	{
		int		iStartWarValue;
		int		iNoWarAttitudeProb;
		int		iPossibleMaxWarPass;
		bool	bPossibleLimitedWar;
		bool	bPossibleDogpileWar;
		bool    bEnoughDogpilePower;

		bool	bValid;
		bool	bIsLandTarget;
		bool	bIsVictory4;
		bool	bIsAnyCapitalAreaAlone;
		bool	bAdjacentCheckPassed;
		bool	bIsMaxWarNearbyPowerRatio;
		bool	bIsMaxWarDistantPowerRatio;
	} aStartWarInfo[MAX_TEAMS];
	
	// first calculate all the values and put into array
	for (int iTeamIndex = 0; iTeamIndex < MAX_TEAMS; iTeamIndex++)
	{
		aStartWarInfo[iTeamIndex].bValid = false;
		
		TeamTypes eLoopTeam = (TeamTypes) iTeamIndex;
		CvTeamAI& kLoopTeam = GET_TEAM(eLoopTeam);
		if (eLoopTeam != eTeam && kLoopTeam.isAlive() && !kLoopTeam.isBarbarian())
		{
			WarPlanTypes eWarPlan = kTeam.AI_getWarPlan(eLoopTeam);
			if (!kTeam.isAtWar(eLoopTeam) && (eWarPlan == NO_WARPLAN))
			{
				if (kTeam.canEventuallyDeclareWar(eLoopTeam) && kTeam.isHasMet(eLoopTeam))
				{
					if( GET_TEAM(eLoopTeam).isAVassal() && !kTeam.AI_isOkayVassalTarget(eLoopTeam) )
					{
						continue;
					}

					aStartWarInfo[iTeamIndex].bValid = true;
					
					int iLoopTeamPower = kLoopTeam.getDefensivePower();
					bool bIsLandTarget = kTeam.AI_isLandTarget(eLoopTeam);
					aStartWarInfo[iTeamIndex].bIsLandTarget = bIsLandTarget;

					bool bIsVictory4 = GET_TEAM(eLoopTeam).AI_isAnyMemberDoVictoryStrategyLevel4();
					aStartWarInfo[iTeamIndex].bIsVictory4 = bIsVictory4;

					//int iNoWarAttitudeProb = kTeam.AI_noWarAttitudeProb(kTeam.AI_getAttitude(eLoopTeam));
					int iNoWarAttitudeProb = std::max(kTeam.AI_noWarAttitudeProb(kTeam.AI_getAttitude(eLoopTeam)), kTeam.AI_noWarAttitudeProb(kTeam.AI_getAttitude(GET_TEAM(eLoopTeam).getMasterTeam())));
					aStartWarInfo[iTeamIndex].iNoWarAttitudeProb = iNoWarAttitudeProb;

					// total war
					aStartWarInfo[iTeamIndex].iPossibleMaxWarPass = MAX_INT;
					if (iNoWarAttitudeProb < 100 && (bFinancesProTotalWar || !bFinancesOpposeWar))
					{
						int iNoWarChance = range(iNoWarAttitudeProb - (bAggressive ? 10 : 0) - (bFinancesProTotalWar ? 10 : 0) + (20*iGetBetterUnitsCount)/iNumMembers, 0, 100);
						if (iNoWarChance < 100)
						{
							bool bIsMaxWarNearbyPowerRatio = (iLoopTeamPower < ((iTeamPower * kTeam.AI_maxWarNearbyPowerRatio()) / 100));
							bool bIsMaxWarDistantPowerRatio = (iLoopTeamPower < ((iTeamPower * kTeam.AI_maxWarDistantPowerRatio()) / 100));
							aStartWarInfo[iTeamIndex].bIsMaxWarNearbyPowerRatio = bIsMaxWarNearbyPowerRatio;
							aStartWarInfo[iTeamIndex].bIsMaxWarDistantPowerRatio = bIsMaxWarDistantPowerRatio;

							bool bAdjacentCheckPassed = true;
							int iMaxWarMinAdjacentPercent = kTeam.AI_maxWarMinAdjacentLandPercent();
							if (iMaxWarMinAdjacentPercent > 0)
							{
								int iMinAdjacentPlots = ((kTeam.getTotalLand() * iMaxWarMinAdjacentPercent) / 100);
								if (iMinAdjacentPlots > 0)
								{
									bAdjacentCheckPassed = (kTeam.AI_calculateAdjacentLandPlots(eLoopTeam) >= iMinAdjacentPlots);
								}
							}
							aStartWarInfo[iTeamIndex].bAdjacentCheckPassed = bAdjacentCheckPassed;

							// check to see which max war pass, if any is valid for this loop team
							int iPossibleMaxWarPass = MAX_INT;
							if (bIsMaxWarNearbyPowerRatio && (bAdjacentCheckPassed || bIsVictory4))
							{
								iPossibleMaxWarPass = 0;
							}
							else if (bIsMaxWarNearbyPowerRatio && (bIsLandTarget || bIsAnyCapitalAreaAlone || bIsVictory4))
							{
								iPossibleMaxWarPass = 1;
							}
							else if (bIsMaxWarDistantPowerRatio)
							{
								iPossibleMaxWarPass = 2;
							}
							aStartWarInfo[iTeamIndex].iPossibleMaxWarPass = iPossibleMaxWarPass;
							
							// if this team is valid on a lower pass, then it is the best pass
							if (iPossibleMaxWarPass < iBestPossibleMaxWarPass)
							{
								iBestPossibleMaxWarPass = iPossibleMaxWarPass;
							}
						}
					}

					// limited war
					aStartWarInfo[iTeamIndex].bPossibleLimitedWar = false;
					if (iNoWarAttitudeProb < 100 && (bFinancesProLimitedWar || !bFinancesOpposeWar))
					{
						int iNoWarChance = std::max(0, iNoWarAttitudeProb + 10 - (bAggressive ? 10 : 0) - (bFinancesProLimitedWar ? 10 : 0));
						if (iNoWarChance < 100)
						{
							bool bIsLimitedPowerRatio = (iLoopTeamPower < ((iTeamPower * kTeam.AI_limitedWarPowerRatio()) / 100));
							bool bIsAnyLoopTeamCapitalAreaAlone = kLoopTeam.AI_isAnyCapitalAreaAlone();
							
							if (bIsLimitedPowerRatio && (bIsLandTarget || (bIsAnyCapitalAreaAlone && bIsAnyLoopTeamCapitalAreaAlone)))
							{
								aStartWarInfo[iTeamIndex].bPossibleLimitedWar = true;
							}
						}
					}

					// dogpile war
					aStartWarInfo[iTeamIndex].bPossibleDogpileWar = false;
					aStartWarInfo[iTeamIndex].bEnoughDogpilePower = false;
					if (iNoWarAttitudeProb < 100 && (bFinancesProDogpileWar || !bFinancesOpposeWar) && kTeam.canDeclareWar(eLoopTeam))
					{
						int iNoWarChance = std::max(0, iNoWarAttitudeProb + 20 - (bAggressive ? 10 : 0) - (bFinancesProDogpileWar ? 10 : 0));
						if (iNoWarChance < 100)
						{
							int iDogpilePower = iTeamPower;
							for (int iTeamIndex2 = 0; iTeamIndex2 < MAX_CIV_TEAMS; iTeamIndex2++)
							{
								TeamTypes eDogpileLoopTeam = (TeamTypes) iTeamIndex2;
								CvTeamAI& kDogpileLoopTeam = GET_TEAM(eDogpileLoopTeam);
								if (kDogpileLoopTeam.isAlive())
								{
									if (eDogpileLoopTeam != eLoopTeam)
									{
										if (atWar(eDogpileLoopTeam, eLoopTeam))
										{
											iDogpilePower += kDogpileLoopTeam.getPower(false);
										}
									}
								}
							}

							bool bIsDogpilePowerRatio = (((iLoopTeamPower * 3) / 2) < iDogpilePower);
							aStartWarInfo[iTeamIndex].bPossibleDogpileWar = true;

							if (bIsDogpilePowerRatio)
							{
								aStartWarInfo[iTeamIndex].bEnoughDogpilePower = true;
							}
						}
					}
					
					// if this team can have any war, calculate the start war value
					aStartWarInfo[iTeamIndex].iStartWarValue = 0;
					if (aStartWarInfo[iTeamIndex].iPossibleMaxWarPass < MAX_INT || aStartWarInfo[iTeamIndex].bPossibleLimitedWar || aStartWarInfo[iTeamIndex].bPossibleDogpileWar)
					{
						aStartWarInfo[iTeamIndex].iStartWarValue = kTeam.AI_startWarVal(eLoopTeam, WARPLAN_TOTAL); // K-Mod
					}
				}
			}
		}
	}

	if( bFinancesOpposeWar )
	{
		szBuffer.append(CvWString::format(SETCOLR L"## Finances oppose war%s%s%s\n" ENDCOLR, TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"),
			bFinancesProTotalWar ? L", pro Total" : L"",
			bFinancesProLimitedWar ? L", pro Limited" : L"",
			bFinancesProDogpileWar ? L", pro Dogpile" : L""));
		szBuffer.append(NEWLINE);
	}

	// display total war items, sorting the list
	bHadAny = false;
	bFirst = true;
	int iBestValue;
	int iLastValue = MAX_INT;
	do
	{
		// find the highest value item left to do
		iBestValue = 0;
		for (int iTeamIndex = 0; iTeamIndex < MAX_TEAMS; iTeamIndex++)
		{
			if (aStartWarInfo[iTeamIndex].bValid && aStartWarInfo[iTeamIndex].iPossibleMaxWarPass < MAX_INT)
			{
				if (aStartWarInfo[iTeamIndex].iStartWarValue > iBestValue && aStartWarInfo[iTeamIndex].iStartWarValue < iLastValue)
				{
					iBestValue = aStartWarInfo[iTeamIndex].iStartWarValue;
				}
			}
		}
		
		// did we find one?
		if (iBestValue > 0)
		{
			// setup for next loop
			iLastValue = iBestValue;

			// now display every team that has that value
			for (int iTeamIndex = 0; iTeamIndex < MAX_TEAMS; iTeamIndex++)
			{
				if (aStartWarInfo[iTeamIndex].bValid && aStartWarInfo[iTeamIndex].iStartWarValue == iBestValue)
				{
					CvTeamAI& kLoopTeam = GET_TEAM((TeamTypes) iTeamIndex);

					if (bFirst)
					{
						float fMaxWarPercentage = ((fOverallWarPercentage * (iTotalWarThreshold + 1)) / iTotalWarRand);
						szBuffer.append(CvWString::format(SETCOLR L"%.2f%% [%d/%d] Total War:\n" ENDCOLR, TEXT_COLOR("COLOR_UNIT_TEXT"), fMaxWarPercentage, (iTotalWarThreshold + 1), iTotalWarRand));
						bFirst = false;
					}

					bHadAny = true;
					
					int iNoWarChance = std::max(0, aStartWarInfo[iTeamIndex].iNoWarAttitudeProb - (bAggressive ? 10 : 0) - (bFinancesProTotalWar ? 10 : 0));
					int iTeamWarPercentage = (100 - iNoWarChance);
					
					if (aStartWarInfo[iTeamIndex].iPossibleMaxWarPass <= iBestPossibleMaxWarPass)
					{
						szBuffer.append(CvWString::format(SETCOLR L" %d%% %s%s war (%d) with %s\n" ENDCOLR, TEXT_COLOR("COLOR_ALT_HIGHLIGHT_TEXT"),
							iTeamWarPercentage,
							(aStartWarInfo[iTeamIndex].bIsVictory4) ? L"**" : L"",
							(aStartWarInfo[iTeamIndex].bIsLandTarget) ? L"land" : L"sea",
							aStartWarInfo[iTeamIndex].iStartWarValue, 
							kLoopTeam.getName().GetCString()));
					}
					else
					{
						szBuffer.append(CvWString::format(SETCOLR L" (%d%% %s%s war (%d) with %s [%s%s])\n" ENDCOLR, TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"),
							iTeamWarPercentage,
							(aStartWarInfo[iTeamIndex].bIsVictory4) ? L"**" : L"",
							(aStartWarInfo[iTeamIndex].bIsLandTarget) ? L"land" : L"sea",
							aStartWarInfo[iTeamIndex].iStartWarValue, 
							kLoopTeam.getName().GetCString(),
							(iBestPossibleMaxWarPass == 0) ? ((aStartWarInfo[iTeamIndex].bIsMaxWarNearbyPowerRatio) ? L"not adjacent" : L"low power") : L"",
							(iBestPossibleMaxWarPass == 1) ? ((aStartWarInfo[iTeamIndex].bIsMaxWarNearbyPowerRatio) ? L"not land" : L"low power") : L""));
					}
				}
			}
		}
	}
	while (iBestValue > 0);

	// double space if had any war
	if (bHadAny)
	{
		szBuffer.append(NEWLINE);
	}
	
	// display limited war items, sorting the list
	bHadAny = false;
	bFirst = true;
	iLastValue = MAX_INT;
	do
	{
		// find the highest value item left to do
		iBestValue = 0;
		for (int iTeamIndex = 0; iTeamIndex < MAX_TEAMS; iTeamIndex++)
		{
			if (aStartWarInfo[iTeamIndex].bValid && aStartWarInfo[iTeamIndex].bPossibleLimitedWar)
			{
				if (aStartWarInfo[iTeamIndex].iStartWarValue > iBestValue && aStartWarInfo[iTeamIndex].iStartWarValue < iLastValue)
				{
					iBestValue = aStartWarInfo[iTeamIndex].iStartWarValue;
				}
			}
		}
		
		// did we find one?
		if (iBestValue > 0)
		{
			// setup for next loop
			iLastValue = iBestValue;

			// now display every team that has that value
			for (int iTeamIndex = 0; iTeamIndex < MAX_TEAMS; iTeamIndex++)
			{
				if (aStartWarInfo[iTeamIndex].bValid && aStartWarInfo[iTeamIndex].iStartWarValue == iBestValue)
				{
					if (bFirst)
					{
						float fLimitedWarPercentage = (fOverallWarPercentage * (iLimitedWarThreshold + 1)) / iLimitedWarRand;
						szBuffer.append(CvWString::format(SETCOLR L"%.2f%% Limited War:\n" ENDCOLR, TEXT_COLOR("COLOR_UNIT_TEXT"), fLimitedWarPercentage));
						bFirst = false;
					}

					bHadAny = true;
					
					int iNoWarChance = std::max(0, aStartWarInfo[iTeamIndex].iNoWarAttitudeProb + 10 - (bAggressive ? 10 : 0) - (bFinancesProLimitedWar ? 10 : 0));
					int iTeamWarPercentage = (100 - iNoWarChance);
					
					szBuffer.append(CvWString::format(SETCOLR L" %d%% %s%s war (%d) with %s\n" ENDCOLR, TEXT_COLOR("COLOR_ALT_HIGHLIGHT_TEXT"),
						iTeamWarPercentage,
						(aStartWarInfo[iTeamIndex].bIsVictory4) ? L"**" : L"",
						(aStartWarInfo[iTeamIndex].bIsLandTarget) ? L"land" : L"sea",
						aStartWarInfo[iTeamIndex].iStartWarValue, 
						GET_TEAM((TeamTypes) iTeamIndex).getName().GetCString()));
				}
			}
		}
	}
	while (iBestValue > 0);

	// double space if had any war
	if (bHadAny)
	{
		szBuffer.append(NEWLINE);
	}
	
	// display dogpile war items, sorting the list
	bHadAny = false;
	bFirst = true;
	iLastValue = MAX_INT;
	do
	{
		// find the highest value item left to do
		iBestValue = 0;
		for (int iTeamIndex = 0; iTeamIndex < MAX_TEAMS; iTeamIndex++)
		{
			if (aStartWarInfo[iTeamIndex].bValid && aStartWarInfo[iTeamIndex].bPossibleDogpileWar)
			{
				if (aStartWarInfo[iTeamIndex].iStartWarValue > iBestValue && aStartWarInfo[iTeamIndex].iStartWarValue < iLastValue)
				{
					iBestValue = aStartWarInfo[iTeamIndex].iStartWarValue;
				}
			}
		}
		
		// did we find one?
		if (iBestValue > 0)
		{
			// setup for next loop
			iLastValue = iBestValue;

			// now display every team that has that value
			for (int iTeamIndex = 0; iTeamIndex < MAX_TEAMS; iTeamIndex++)
			{
				if (aStartWarInfo[iTeamIndex].bValid && aStartWarInfo[iTeamIndex].iStartWarValue == iBestValue)
				{
					if (bFirst)
					{
						float fDogpileWarPercentage = (fOverallWarPercentage * (iDogpileWarThreshold + 1)) / iDogpileWarRand;
						szBuffer.append(CvWString::format(SETCOLR L"%.2f%% Dogpile War:\n" ENDCOLR, TEXT_COLOR("COLOR_UNIT_TEXT"), fDogpileWarPercentage));
						bFirst = false;
					}

					bHadAny = true;
					
					int iNoWarChance = std::max(0, aStartWarInfo[iTeamIndex].iNoWarAttitudeProb + 20 - (bAggressive ? 10 : 0) - (bFinancesProDogpileWar ? 10 : 0));
					int iTeamWarPercentage = (100 - iNoWarChance);
					
					if( aStartWarInfo[iTeamIndex].bEnoughDogpilePower )
					{
						if( (aStartWarInfo[iTeamIndex].bIsLandTarget) || (aStartWarInfo[iTeamIndex].bIsVictory4) )
						{
							szBuffer.append(CvWString::format(SETCOLR L" %d%% %s%s war (%d) with %s\n" ENDCOLR, TEXT_COLOR("COLOR_ALT_HIGHLIGHT_TEXT"),
								iTeamWarPercentage,
								(aStartWarInfo[iTeamIndex].bIsVictory4) ? L"**" : L"",
								L"land",
								aStartWarInfo[iTeamIndex].iStartWarValue, 
								GET_TEAM((TeamTypes) iTeamIndex).getName().GetCString()));
						}
						else
						{
							szBuffer.append(CvWString::format(SETCOLR L" %d%% %s%s war (%d) with %s\n" ENDCOLR, TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"),
								iTeamWarPercentage,
								(aStartWarInfo[iTeamIndex].bIsVictory4) ? L"**" : L"",
								L"sea",
								aStartWarInfo[iTeamIndex].iStartWarValue, 
								GET_TEAM((TeamTypes) iTeamIndex).getName().GetCString()));
						}
					}
					else
					{
						szBuffer.append(CvWString::format(SETCOLR L" Lack power for %s%s war (%d) with %s\n" ENDCOLR, TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"),
							(aStartWarInfo[iTeamIndex].bIsVictory4) ? L"**" : L"",
							(aStartWarInfo[iTeamIndex].bIsLandTarget) ? L"land" : L"sea",
							aStartWarInfo[iTeamIndex].iStartWarValue, 
							GET_TEAM((TeamTypes) iTeamIndex).getName().GetCString()));
					}
				}
			}
		}
	}
	while (iBestValue > 0);

	// double space if had any war
	if (bHadAny)
	{
		szBuffer.append(NEWLINE);
	}

	if (GC.shiftKey())
	{
		return;
	}
}
// K-Mod end

void CvDLLWidgetData::parseConvertHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 != 0)
	{
		if (widgetDataStruct.m_iData1 == NO_RELIGION)
		{
//			szBuffer.Format(L"No State %c", gDLL->getSymbolID(RELIGION_CHAR));
			szBuffer.assign(gDLL->getText("TXT_KEY_MISC_NO_STATE_REL"));
		}
		else
		{
//			szBuffer.Format(L"Convert to %s", GC.getReligionInfo((ReligionTypes) widgetDataStruct.m_iData1).getDescription());
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_CONVERT_TO_REL", GC.getReligionInfo((ReligionTypes) widgetDataStruct.m_iData1).getTextKeyWide()));
		}
	}
	else
	{
		GAMETEXT.setConvertHelp(szBuffer, GC.getGameINLINE().getActivePlayer(), (ReligionTypes)widgetDataStruct.m_iData1);
	}
}


void CvDLLWidgetData::parseRevolutionHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData1 != 0)
	{
//		szBuffer = "Change Civics";
		szBuffer.assign(gDLL->getText("TXT_KEY_MISC_CHANGE_CIVICS"));
	}
	else
	{
		GAMETEXT.setRevolutionHelp(szBuffer, GC.getGameINLINE().getActivePlayer());
	}
}

/*void CvDLLWidgetData::parsePopupQueue(CvWidgetDataStruct &widgetDataStruct, CvWString &szBuffer)
{
	PopupEventTypes eEvent;

	if ( m_pPopup || m_pScreen )
	{
		if ( m_pPopup )
		{
			eEvent = m_pPopup->getPopupType();
		}
		else if ( m_pScreen )
		{
			eEvent = m_pScreen->getPopupType();
		}

		switch (eEvent)
		{
			case POPUPEVENT_NONE:
//				szBuffer = "***NO MOUSEOVER TEXT***.  Click to activate popup.";
				szBuffer = gDLL->getText("TXT_KEY_MISC_NO_MOUSEOVER_TEXT");
				break;

			case POPUPEVENT_PRODUCTION:
//				szBuffer = "Production Complete";
				szBuffer = gDLL->getText("TXT_KEY_MISC_PRODUCTION_COMPLETE");
				break;

			case POPUPEVENT_TECHNOLOGY:
//				szBuffer = "Technology Research Complete";
				szBuffer = gDLL->getText("TXT_KEY_MISC_TECH_RESEARCH_COMPLETE");
				break;

			case POPUPEVENT_RELIGION:
//				szBuffer = "New Religion Discovered";
				szBuffer = gDLL->getText("TXT_KEY_MISC_NEW_REL_DISCOVERED");
				break;

			case POPUPEVENT_WARNING:
//				szBuffer = "Warning!!!";
				szBuffer = gDLL->getText("TXT_KEY_MISC_WARNING");
				break;

			case POPUPEVENT_CIVIC:
//				szBuffer = "New Civic Accessible";
				szBuffer = gDLL->getText("TXT_KEY_MISC_NEW_CIVIC_ACCESSIBLE");
				break;
		}
	}
}*/

void CvDLLWidgetData::parseAutomateCitizensHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity;

	pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity != NULL)
	{
		if (pHeadSelectedCity->isCitizensAutomated())
		{
//			szBuffer = "Turn Off Citizen Automation";
			szBuffer.assign(gDLL->getText("TXT_KEY_MISC_OFF_CITIZEN_AUTO"));
		}
		else
		{
//			szBuffer = "Turn On Citizen Automation";
			szBuffer.assign(gDLL->getText("TXT_KEY_MISC_ON_CITIZEN_AUTO"));
		}
	}
}

void CvDLLWidgetData::parseAutomateProductionHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity;

	pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity != NULL)
	{
		if (pHeadSelectedCity->isProductionAutomated())
		{
//			szBuffer = "Turn Off Production Automation";
			szBuffer.assign(gDLL->getText("TXT_KEY_MISC_OFF_PROD_AUTO"));
		}
		else
		{
//			szBuffer = "Turn On Production Automation";
			szBuffer.assign(gDLL->getText("TXT_KEY_MISC_ON_PROD_AUTO"));
		}
	}
}

void CvDLLWidgetData::parseEmphasizeHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity;

	szBuffer.clear();

	pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity != NULL)
	{
		if (pHeadSelectedCity->AI_isEmphasize((EmphasizeTypes)widgetDataStruct.m_iData1))
		{
//			szBuffer += "Turn Off ";
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_TURN_OFF"));
		}
		else
		{
//			szBuffer += "Turn On ";
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_TURN_ON"));
		}
	}

	szBuffer.append(GC.getEmphasizeInfo((EmphasizeTypes)widgetDataStruct.m_iData1).getDescription());

	// MOD - START - Epidemics
	if (GC.getEmphasizeInfo((EmphasizeTypes)widgetDataStruct.m_iData1).isAvoidEpidemics())
	{
		if (pHeadSelectedCity != NULL)
		{
			CvWString szTempBuffer;

			int iLimit = GET_PLAYER(pHeadSelectedCity->getOwnerINLINE()).getAcceptableEpidemicPercent();
			szTempBuffer.Format(SETCOLR L"Limit: %d%% %c" ENDCOLR, TEXT_COLOR("COLOR_LIGHT_GREY"), iLimit, gDLL->getSymbolID(EPIDEMIC_START_CHAR));
			szBuffer.append(NEWLINE);
			szBuffer.append(szTempBuffer);
		}
	}
	// MOD - END - Epidemics
}


void CvDLLWidgetData::parseTradeItem(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvWString szTempBuffer;
	TradeData item;
	PlayerTypes eWhoFrom = NO_PLAYER;
	PlayerTypes eWhoTo = NO_PLAYER;
	DenialTypes eDenial;
	PlayerTypes eWhoDenies;

	szBuffer.clear();

	if (widgetDataStruct.m_bOption)
	{
		if ( gDLL->isDiplomacy())
		{
			eWhoFrom = (PlayerTypes) gDLL->getDiplomacyPlayer();
		}
		else if (gDLL->isMPDiplomacyScreenUp())
		{
			eWhoFrom = (PlayerTypes) gDLL->getMPDiplomacyPlayer();
		}
		eWhoTo = GC.getGameINLINE().getActivePlayer();
	}
	else
	{
		eWhoFrom = GC.getGameINLINE().getActivePlayer();
		if ( gDLL->isDiplomacy() )
		{
			eWhoTo = (PlayerTypes) gDLL->getDiplomacyPlayer();
		}
		else if (gDLL->isMPDiplomacyScreenUp())
		{
			eWhoTo = (PlayerTypes) gDLL->getMPDiplomacyPlayer();
		}
	}

	eWhoDenies = eWhoFrom;

	if ((eWhoFrom != NO_PLAYER) && (eWhoTo != NO_PLAYER))
	{
		//	Data1 is the heading
		switch (widgetDataStruct.m_iData1)
		{
		case TRADE_TECHNOLOGIES:
			GAMETEXT.setTechHelp(szBuffer, ((TechTypes)widgetDataStruct.m_iData2));
			eWhoDenies = (widgetDataStruct.m_bOption ? eWhoFrom : eWhoTo);
			break;
		case TRADE_RESOURCES:
			GAMETEXT.setBonusHelp(szBuffer, ((BonusTypes)widgetDataStruct.m_iData2));
			eWhoDenies = (widgetDataStruct.m_bOption ? eWhoFrom : eWhoTo);
			break;
		case TRADE_CITIES:
			szBuffer.assign(gDLL->getText("TXT_KEY_TRADE_CITIES"));
			eWhoDenies = (widgetDataStruct.m_bOption ? eWhoFrom : eWhoTo);
			break;
		case TRADE_PEACE:
			szBuffer.append(gDLL->getText("TXT_KEY_TRADE_MAKE_PEACE", GET_TEAM(GET_PLAYER(eWhoFrom).getTeam()).getName().GetCString(), GET_TEAM((TeamTypes)widgetDataStruct.m_iData2).getName().GetCString()));
			break;
		case TRADE_WAR:
			szBuffer.append(gDLL->getText("TXT_KEY_TRADE_MAKE_WAR", GET_TEAM(GET_PLAYER(eWhoFrom).getTeam()).getName().GetCString(), GET_TEAM((TeamTypes)widgetDataStruct.m_iData2).getName().GetCString()));
			break;
		case TRADE_EMBARGO:
			szBuffer.append(gDLL->getText("TXT_KEY_TRADE_STOP_TRADING", GET_TEAM(GET_PLAYER(eWhoFrom).getTeam()).getName().GetCString(), GET_TEAM((TeamTypes)widgetDataStruct.m_iData2).getName().GetCString()));
			break;
		case TRADE_CIVIC:
			szBuffer.append(gDLL->getText("TXT_KEY_TRADE_ADOPT_CIVIC", GC.getCivicInfo((CivicTypes)widgetDataStruct.m_iData2).getDescription()));
			break;
		case TRADE_RELIGION:
			szBuffer.append(gDLL->getText("TXT_KEY_TRADE_CONVERT_RELIGION", GC.getReligionInfo((ReligionTypes)widgetDataStruct.m_iData2).getDescription()));
			break;
		case TRADE_GOLD:
			szBuffer.append(gDLL->getText("TXT_KEY_TRADE_GOLD"));
			break;
		case TRADE_GOLD_PER_TURN:
			szBuffer.append(gDLL->getText("TXT_KEY_TRADE_GOLD_PER_TURN"));
			break;
		case TRADE_MAPS:
			szBuffer.append(gDLL->getText("TXT_KEY_TRADE_MAPS"));
			break;
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      12/07/09                                jdog5000      */
/*                                                                                              */
/* Diplomacy                                                                                    */
/************************************************************************************************/
		case TRADE_SURRENDER:
			szBuffer.append(gDLL->getText("TXT_KEY_TRADE_CAPITULATE"));
			eWhoDenies = (widgetDataStruct.m_bOption ? eWhoFrom : NO_PLAYER);
			break;
		case TRADE_VASSAL:
			szBuffer.append(gDLL->getText("TXT_KEY_TRADE_VASSAL"));
			eWhoDenies = (widgetDataStruct.m_bOption ? eWhoFrom : NO_PLAYER);
			break;
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/
		case TRADE_OPEN_BORDERS:
			szBuffer.append(gDLL->getText("TXT_KEY_TRADE_OPEN_BORDERS"));
			break;
		case TRADE_DEFENSIVE_PACT:
			szBuffer.append(gDLL->getText("TXT_KEY_TRADE_DEFENSIVE_PACT"));
			break;
		case TRADE_PERMANENT_ALLIANCE:
			szBuffer.append(gDLL->getText("TXT_KEY_TRADE_PERMANENT_ALLIANCE"));
			break;
		case TRADE_PEACE_TREATY:
			szBuffer.append(gDLL->getText("TXT_KEY_TRADE_PEACE_TREATY", GC.getDefineINT("PEACE_TREATY_LENGTH")));
			break;
		}

		setTradeItem(&item, ((TradeableItems)(widgetDataStruct.m_iData1)), widgetDataStruct.m_iData2);

		eDenial = GET_PLAYER(eWhoFrom).getTradeDenial(eWhoTo, item);

		if (eDenial != NO_DENIAL)
		{
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      12/07/09                                jdog5000      */
/*                                                                                              */
/* Diplomacy                                                                                    */
/************************************************************************************************/
			if( eWhoDenies == NO_PLAYER )
			{
				switch(eDenial)
				{
				case DENIAL_POWER_US:
					eDenial = DENIAL_POWER_YOU;
					break;
				case DENIAL_POWER_YOU:
					eDenial = DENIAL_POWER_US;
					break;
				case DENIAL_WAR_NOT_POSSIBLE_US:
					eDenial = DENIAL_WAR_NOT_POSSIBLE_YOU;
					break;
				case DENIAL_WAR_NOT_POSSIBLE_YOU:
					eDenial = DENIAL_WAR_NOT_POSSIBLE_US;
					break;
				case DENIAL_PEACE_NOT_POSSIBLE_US:
					eDenial = DENIAL_PEACE_NOT_POSSIBLE_YOU;
					break;
				case DENIAL_PEACE_NOT_POSSIBLE_YOU:
					eDenial = DENIAL_PEACE_NOT_POSSIBLE_US;
					break;
				default :
					break;
				}
				szTempBuffer.Format(L"%s: " SETCOLR L"%s" ENDCOLR, GET_PLAYER(eWhoTo).getName(), TEXT_COLOR("COLOR_WARNING_TEXT"), GC.getDenialInfo(eDenial).getDescription());
			}
			else
			{
				szTempBuffer.Format(L"%s: " SETCOLR L"%s" ENDCOLR, GET_PLAYER(eWhoDenies).getName(), TEXT_COLOR("COLOR_WARNING_TEXT"), GC.getDenialInfo(eDenial).getDescription());
			}
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/
			szBuffer.append(NEWLINE);
			szBuffer.append(szTempBuffer);
		}
	}
}


void CvDLLWidgetData::parseUnitModelHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvUnit* pHeadSelectedUnit;

	pHeadSelectedUnit = gDLL->getInterfaceIFace()->getHeadSelectedUnit();

	if (pHeadSelectedUnit != NULL)
	{
		GAMETEXT.setUnitHelp(szBuffer, pHeadSelectedUnit);
	}
}


void CvDLLWidgetData::parseFlagHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvWString szTempBuffer;

	szTempBuffer.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), GC.getCivilizationInfo(GC.getGameINLINE().getActiveCivilizationType()).getDescription());
	szBuffer.append(szTempBuffer);
	szBuffer.append(NEWLINE);

	GAMETEXT.parseLeaderTraits(szBuffer, GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getLeaderType(), GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getCivilizationType());
}


void CvDLLWidgetData::parseMaintenanceHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity;
	wchar szTempBuffer[1024];
	int iMaintenanceValue;

	pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity != NULL)
	{
		if (pHeadSelectedCity->isWeLoveTheKingDay())
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_WE_LOVE_KING_MAINT"));
		}
		else
		{
			int iInflationFactor = 100 + GET_PLAYER(pHeadSelectedCity->getOwnerINLINE()).getInflationRate(); // K-Mod

			//		szBuffer = "Maintenance represents the total cost of governing this city.\n";
			szBuffer.assign(gDLL->getText("TXT_KEY_MISC_MAINT_INFO"));
			szBuffer.append(NEWLINE);

			//iMaintenanceValue = pHeadSelectedCity->calculateDistanceMaintenanceTimes100();
			iMaintenanceValue = pHeadSelectedCity->calculateDistanceMaintenanceTimes100()*iInflationFactor/100; // K-Mod
			if (iMaintenanceValue != 0)
			{
				//			swprintf(szTempBuffer, "\n%s%d%c: %s", ((iMaintenanceValue > 0) ?  "+" : ""), iMaintenanceValue, GC.getCommerceInfo(COMMERCE_GOLD).getChar(), ((GET_PLAYER(pHeadSelectedCity->getOwnerINLINE()).getNumGovernmentCenters() > 0) ? "Distance from Palace" : "No Palace Penalty"));
				CvWString szMaint = CvWString::format(L"%d.%02d", iMaintenanceValue/100, iMaintenanceValue%100);
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_NUM_MAINT_FLOAT", szMaint.GetCString()) + ((GET_PLAYER(pHeadSelectedCity->getOwnerINLINE()).getNumGovernmentCenters() > 0) ? gDLL->getText("TXT_KEY_MISC_DISTANCE_FROM_PALACE") : gDLL->getText("TXT_KEY_MISC_NO_PALACE_PENALTY")));
			}

			//iMaintenanceValue = pHeadSelectedCity->calculateNumCitiesMaintenanceTimes100();
			iMaintenanceValue = pHeadSelectedCity->calculateNumCitiesMaintenanceTimes100()*iInflationFactor/100; // K-Mod
			if (iMaintenanceValue != 0)
			{
				//			swprintf(szTempBuffer, "\n%s%d%c: Number of Cities", ((iMaintenanceValue > 0) ? "+" : ""), iMaintenanceValue, GC.getCommerceInfo(COMMERCE_GOLD).getChar());
				CvWString szMaint = CvWString::format(L"%d.%02d", iMaintenanceValue/100, iMaintenanceValue%100);
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_NUM_CITIES_FLOAT", szMaint.GetCString()));
			}

			//iMaintenanceValue = pHeadSelectedCity->calculateColonyMaintenanceTimes100();
			iMaintenanceValue = pHeadSelectedCity->calculateColonyMaintenanceTimes100()*iInflationFactor/100; // K-Mod
			if (iMaintenanceValue != 0)
			{
				CvWString szMaint = CvWString::format(L"%d.%02d", iMaintenanceValue/100, iMaintenanceValue%100);
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_COLONY_MAINT_FLOAT", szMaint.GetCString()));
			}

			//iMaintenanceValue = pHeadSelectedCity->calculateCorporationMaintenanceTimes100();
			iMaintenanceValue = pHeadSelectedCity->calculateCorporationMaintenanceTimes100()*iInflationFactor/100; // K-Mod
			if (iMaintenanceValue != 0)
			{
				CvWString szMaint = CvWString::format(L"%d.%02d", iMaintenanceValue/100, iMaintenanceValue%100);
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_CORPORATION_MAINT_FLOAT", szMaint.GetCString()));
			}

			szBuffer.append(SEPARATOR);

			//		swprintf(szTempBuffer, "\n%d%c Total Maintenance", pHeadSelectedCity->getMaintenance(), GC.getCommerceInfo(COMMERCE_GOLD).getChar());
			/* CvWString szMaint = CvWString::format(L"%d.%02d", pHeadSelectedCity->getMaintenanceTimes100()/100, pHeadSelectedCity->getMaintenanceTimes100()%100);
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_TOTAL_MAINT_FLOAT", szMaint.GetCString())); */
			// K-Mod
			iMaintenanceValue = pHeadSelectedCity->getMaintenanceTimes100()*iInflationFactor/100;

			CvWString szMaint = CvWString::format(L"%d.%02d", iMaintenanceValue/100, iMaintenanceValue%100);
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_TOTAL_MAINT_FLOAT", szMaint.GetCString()));
			//

			iMaintenanceValue = pHeadSelectedCity->getMaintenanceModifier();

			if (iMaintenanceValue != 0)
			{
				swprintf(szTempBuffer, L" (%s%d%%)", ((iMaintenanceValue > 0) ? L"+" : L""), iMaintenanceValue);
				szBuffer.append(szTempBuffer);
			}

// BUG - Building Saved Maintenance - start
			if (pHeadSelectedCity->getOwnerINLINE() == GC.getGame().getActivePlayer() && getBugOptionBOOL("MiscHover__BuildingSavedMaintenance", false, "BUG_BUILDING_SAVED_MAINTENANCE_HOVER"))
			{
				GAMETEXT.setBuildingSavedMaintenanceHelp(szBuffer, *pHeadSelectedCity, DOUBLE_SEPARATOR);
			}
// BUG - Building Saved Maintenance - end
		}
	}
}


void CvDLLWidgetData::parseHealthHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();
	if (NULL != pHeadSelectedCity)
	{
		GAMETEXT.setBadHealthHelp(szBuffer, *pHeadSelectedCity);
		szBuffer.append(L"\n=====================\n");
		GAMETEXT.setGoodHealthHelp(szBuffer, *pHeadSelectedCity);

// BUG - Building Additional Health - start
		if (pHeadSelectedCity->getOwnerINLINE() == GC.getGame().getActivePlayer() && getBugOptionBOOL("MiscHover__BuildingAdditionalHealth", false, "BUG_BUILDING_ADDITIONAL_HEALTH_HOVER"))
		{
			GAMETEXT.setBuildingAdditionalHealthHelp(szBuffer, *pHeadSelectedCity, DOUBLE_SEPARATOR);
		}
// BUG - Building Additional Health - end
	}
}


void CvDLLWidgetData::parseNationalityHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	wchar szTempBuffer[1024];
	CvCity* pHeadSelectedCity;
	PlayerTypes eCulturalOwner;
	int iCulturePercent;
	int iCityStrength;
	int iGarrison;
	int iI;

	szBuffer.assign(gDLL->getText("TXT_KEY_MISC_CITY_NATIONALITY"));

	pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity != NULL)
	{
		for (iI = 0; iI < MAX_PLAYERS; iI++)
		{
			if (GET_PLAYER((PlayerTypes)iI).isEverAlive())
			{
				iCulturePercent = pHeadSelectedCity->plot()->calculateCulturePercent((PlayerTypes)iI);

				if (iCulturePercent > 0)
				{
					swprintf(szTempBuffer, L"\n%d%% " SETCOLR L"%s" ENDCOLR, iCulturePercent, GET_PLAYER((PlayerTypes)iI).getPlayerTextColorR(), GET_PLAYER((PlayerTypes)iI).getPlayerTextColorG(), GET_PLAYER((PlayerTypes)iI).getPlayerTextColorB(), GET_PLAYER((PlayerTypes)iI).getPlayerTextColorA(), GET_PLAYER((PlayerTypes)iI).getCivilizationAdjective());
					szBuffer.append(szTempBuffer);
				}
			}
		}

		eCulturalOwner = pHeadSelectedCity->plot()->calculateCulturalOwner();

		if (eCulturalOwner != NO_PLAYER)
		{
			if (GET_PLAYER(eCulturalOwner).getTeam() != pHeadSelectedCity->getTeam())
			{
				iCityStrength = pHeadSelectedCity->cultureStrength(eCulturalOwner);
				iGarrison = pHeadSelectedCity->cultureGarrison(eCulturalOwner);

				if (iCityStrength > iGarrison)
				{
					//swprintf(szTempBuffer, L"%.2f", std::max(0.0f, (1.0f - (((float)iGarrison) / ((float)iCityStrength)))) * ((float)(std::min(100.0f, ((float)pHeadSelectedCity->getRevoltTestProbability())))));
					// TODO: Decide on a culture style and make sure this matches
/*
** K-Mod, 6/jan/11, karadoc
** changed text to reflect changes made in CvPlot.doCulture().
*/
/* for reference, here are the revolt tests
int iLeftOdds = GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getVictoryDelayPercent();
if (GC.getGameINLINE().getSorenRandNum(iLeftOdds, "Revolt #1") < pCity->getRevoltTestProbability())
if ((GC.getGameINLINE().getSorenRandNum(iCityStrength+2*iGarrison, "Revolt #2") > 3*iGarrison) || pCity->isBarbarian())
*/
					// The probability is thus
					float fRevoltProb = (float)(iCityStrength-iGarrison)/(iCityStrength+2*iGarrison) * (float)pHeadSelectedCity->getRevoltTestProbability() / GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getVictoryDelayPercent();
					/* original bts code
					swprintf(szTempBuffer, L"%.2f", std::max(0.0f, (1.0f - (((float)iGarrison) / ((float)iCityStrength)))) * ((float)(std::min(100.0f, ((float)pHeadSelectedCity->getRevoltTestProbability())))));*/
					swprintf(szTempBuffer, L"%.2f", 100*fRevoltProb);
/*
** K-Mod end
*/
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_MISC_CHANCE_OF_REVOLT", szTempBuffer));
				}
			}
		}
	}
}


void CvDLLWidgetData::parseHappinessHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity;

	pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity != NULL)
	{
		GAMETEXT.setAngerHelp(szBuffer, *pHeadSelectedCity);
		szBuffer.append(L"\n=====================\n");
		GAMETEXT.setHappyHelp(szBuffer, *pHeadSelectedCity);

// BUG - Building Additional Happiness - start
		if (pHeadSelectedCity->getOwnerINLINE() == GC.getGame().getActivePlayer() && getBugOptionBOOL("MiscHover__BuildingAdditionalHappiness", false, "BUG_BUILDING_ADDITIONAL_HAPPINESS_HOVER"))
		{
			GAMETEXT.setBuildingAdditionalHappinessHelp(szBuffer, *pHeadSelectedCity, DOUBLE_SEPARATOR);
		}
// BUG - Building Additional Happiness - end
	}
}


void CvDLLWidgetData::parsePopulationHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity;

	pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity != NULL)
	{
		szBuffer.assign(gDLL->getText("TXT_KEY_MISC_FOOD_THRESHOLD", pHeadSelectedCity->getFood(), pHeadSelectedCity->growthThreshold()));
	}
}


void CvDLLWidgetData::parseProductionHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity;

	pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity != NULL)
	{
		if (pHeadSelectedCity->getProductionNeeded() != MAX_INT)
		{
			CvWString szTemp;
			szTemp.Format(L"%s: %d/%d %c", pHeadSelectedCity->getProductionName(), pHeadSelectedCity->getProduction(), pHeadSelectedCity->getProductionNeeded(), GC.getYieldInfo(YIELD_PRODUCTION).getChar());
			szBuffer.assign(szTemp);
		}
	}
}


void CvDLLWidgetData::parseCultureHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity;

	pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity != NULL)
	{
		int iCultureTimes100 = pHeadSelectedCity->getCultureTimes100(pHeadSelectedCity->getOwnerINLINE());
		if (iCultureTimes100%100 == 0)
		{
			szBuffer.assign(gDLL->getText("TXT_KEY_MISC_CULTURE", iCultureTimes100/100, pHeadSelectedCity->getCultureThreshold()));
		}
		else
		{
			CvWString szCulture = CvWString::format(L"%d.%02d", iCultureTimes100/100, iCultureTimes100%100);
			szBuffer.assign(gDLL->getText("TXT_KEY_MISC_CULTURE_FLOAT", szCulture.GetCString(), pHeadSelectedCity->getCultureThreshold()));
		}

		int iCultureRateTimes100 = pHeadSelectedCity->getCommerceRateTimes100(COMMERCE_CULTURE);
		if (iCultureRateTimes100 > 0)
		{
			int iCultureLeftTimes100 = 100 * pHeadSelectedCity->getCultureThreshold() - iCultureTimes100;

			if (iCultureLeftTimes100 > 0)
			{
				int iTurnsLeft = (iCultureLeftTimes100  + iCultureRateTimes100 - 1) / iCultureRateTimes100;

				szBuffer.append(L' ');
				szBuffer.append(gDLL->getText("INTERFACE_CITY_TURNS", std::max(1, iTurnsLeft)));
			}
		}


		szBuffer.append(L"\n=====================\n");
		GAMETEXT.setCommerceHelp(szBuffer, *pHeadSelectedCity, COMMERCE_CULTURE);
	}
}


void CvDLLWidgetData::parseGreatPeopleHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();
	if (NULL != pHeadSelectedCity)
	{
		GAMETEXT.parseGreatPeopleHelp(szBuffer, *pHeadSelectedCity);
	}
}


void CvDLLWidgetData::parseGreatGeneralHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (NO_PLAYER != GC.getGame().getActivePlayer())
	{
		GAMETEXT.parseGreatGeneralHelp(szBuffer, GET_PLAYER(GC.getGame().getActivePlayer()));
	}
}


void CvDLLWidgetData::parseSelectedHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity;
	CvUnit* pHeadSelectedUnit;
	OrderData* pOrder;

	pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();
	pHeadSelectedUnit = gDLL->getInterfaceIFace()->getHeadSelectedUnit();

	if (pHeadSelectedCity != NULL)
	{
		pOrder = pHeadSelectedCity->getOrderFromQueue(widgetDataStruct.m_iData1);

		if (pOrder != NULL)
		{
			switch (pOrder->eOrderType)
			{
			case ORDER_TRAIN:
				GAMETEXT.setUnitHelp(szBuffer, ((UnitTypes)(pOrder->iData1)), false, false, false, pHeadSelectedCity);
				break;

			case ORDER_CONSTRUCT:
				//GAMETEXT.setBuildingHelp(szBuffer, ((BuildingTypes)(pOrder->iData1)), false, false, false, pHeadSelectedCity);
// BUG - Building Actual Effects - start
				GAMETEXT.setBuildingHelpActual(szBuffer, ((BuildingTypes)(pOrder->iData1)), false, false, false, pHeadSelectedCity);
// BUG - Building Actual Effects - end
				break;

			case ORDER_CREATE:
				GAMETEXT.setProjectHelp(szBuffer, ((ProjectTypes)(pOrder->iData1)), false, pHeadSelectedCity);
				break;

			case ORDER_MAINTAIN:
				GAMETEXT.setProcessHelp(szBuffer, ((ProcessTypes)(pOrder->iData1)));
				break;

			default:
				FAssertMsg(false, "eOrderType did not match valid options");
				break;
			}
		}
	}
}


void CvDLLWidgetData::parseTradeRouteCityHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 != 0)
	{
		GAMETEXT.setTradeRouteHelp(szBuffer, widgetDataStruct.m_iData1, gDLL->getInterfaceIFace()->getHeadSelectedCity());
	}
}

void CvDLLWidgetData::parseEspionageCostHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvUnit* pUnit = gDLL->getInterfaceIFace()->getHeadSelectedUnit();
	if (NULL != pUnit)
	{
		CvPlot* pPlot = pUnit->plot();
		if (NULL != pPlot)
		{
			GAMETEXT.setEspionageCostHelp(szBuffer, (EspionageMissionTypes)widgetDataStruct.m_iData1, pPlot->getOwnerINLINE(), pPlot, widgetDataStruct.m_iData2, pUnit);
		}
	}
}

void CvDLLWidgetData::parseBuildingHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 != 0)
	{
		GAMETEXT.setBuildingHelp(szBuffer, ((BuildingTypes)(widgetDataStruct.m_iData1)), false, false, widgetDataStruct.m_bOption, gDLL->getInterfaceIFace()->getHeadSelectedCity());
	}
}

void CvDLLWidgetData::parseProjectHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 != 0)
	{
		GAMETEXT.setProjectHelp(szBuffer, ((ProjectTypes)widgetDataStruct.m_iData1), false, gDLL->getInterfaceIFace()->getHeadSelectedCity());
	}
}


void CvDLLWidgetData::parseTerrainHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 != 0)
	{
		GAMETEXT.setTerrainHelp(szBuffer, (TerrainTypes)widgetDataStruct.m_iData1);
	}
}


void CvDLLWidgetData::parseFeatureHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 != 0)
	{
		GAMETEXT.setFeatureHelp(szBuffer, (FeatureTypes)widgetDataStruct.m_iData1);
	}
}


// MOD - START - Improved Civilopedia
void CvDLLWidgetData::parseRouteHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.setRouteHelp(szBuffer, (RouteTypes)widgetDataStruct.m_iData1);
}


void CvDLLWidgetData::parseWorldSizeHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.setWorldSizeHelp(szBuffer, (WorldSizeTypes)widgetDataStruct.m_iData1);
}


void CvDLLWidgetData::parseHandicapHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.setHandicapHelp(szBuffer, (HandicapTypes)widgetDataStruct.m_iData1);
}


void CvDLLWidgetData::parseConceptHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.setConceptHelp(szBuffer, (ConceptTypes)widgetDataStruct.m_iData1);
}


void CvDLLWidgetData::parseHotKeyClassHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.setHotKeyClassHelp(szBuffer, (HotKeyClassTypes)widgetDataStruct.m_iData1);
}
// MOD - END - Improved Civilopedia


// MOD - START - Era Effects
void CvDLLWidgetData::parseEraHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.setEraHelp(szBuffer, (EraTypes)widgetDataStruct.m_iData1);
}
// MOD - END - Era Effects


void CvDLLWidgetData::parseTechEntryHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 != 0)
	{
		GAMETEXT.setTechHelp(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
	}
}


void CvDLLWidgetData::parseTechPrereqHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
//	szBuffer.Format(L"%cThis technology requires the knowledge of %s", gDLL->getSymbolID(BULLET_CHAR), GC.getTechInfo((TechTypes) widgetDataStruct.m_iData1).getDescription());
	szBuffer.assign(gDLL->getText("TXT_KEY_MISC_TECH_REQUIRES_KNOWLEDGE_OF", GC.getTechInfo((TechTypes) widgetDataStruct.m_iData1).getTextKeyWide()));
}

void CvDLLWidgetData::parseTechTreePrereq(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer, bool bTreeInfo)
{
	// MOD - START - Improved Help Context
	//GAMETEXT.setTechHelp(szBuffer, (TechTypes)widgetDataStruct.m_iData1, false, false, false, bTreeInfo, (TechTypes)widgetDataStruct.m_iData2);
	GAMETEXT.setTechHelp(szBuffer, (TechTypes)widgetDataStruct.m_iData1, NO_CIVILIZATION, NO_PLAYER, false, false, bTreeInfo, (TechTypes)widgetDataStruct.m_iData2);
	// MOD - END - Improved Help Context
}


void CvDLLWidgetData::parseObsoleteHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildObsoleteString(szBuffer, ((BuildingTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseObsoleteBonusString(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildObsoleteBonusString(szBuffer, ((BonusTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseObsoleteSpecialHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildObsoleteSpecialString(szBuffer, ((SpecialBuildingTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseMoveHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildMoveString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseFreeUnitHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildFreeUnitString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData2)));
}

void CvDLLWidgetData::parseFeatureProductionHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildFeatureProductionString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseWorkerRateHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildWorkerRateString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseTradeRouteHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildTradeRouteString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseHealthRateHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildHealthRateString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseHappinessRateHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildHappinessRateString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseFreeTechHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildFreeTechString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseLOSHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildLOSString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseMapCenterHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildMapCenterString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseMapRevealHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildMapRevealString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseMapTradeHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildMapTradeString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseTechTradeHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildTechTradeString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseGoldTradeHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildGoldTradeString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseOpenBordersHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildOpenBordersString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseDefensivePactHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildDefensivePactString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parsePermanentAllianceHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildPermanentAllianceString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseVassalStateHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildVassalStateString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseBuildBridgeHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildBridgeString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseIrrigationHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildIrrigationString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseIgnoreIrrigationHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildIgnoreIrrigationString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseWaterWorkHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildWaterWorkString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseBuildHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildImprovementString(szBuffer, (TechTypes)widgetDataStruct.m_iData1, (BuildTypes)widgetDataStruct.m_iData2);
}

void CvDLLWidgetData::parseDomainExtraMovesHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildDomainExtraMovesString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)), widgetDataStruct.m_iData2);
}

void CvDLLWidgetData::parseAdjustHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildAdjustString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)), widgetDataStruct.m_iData2);
}

void CvDLLWidgetData::parseTerrainTradeHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 < GC.getNumTerrainInfos())
	{
		GAMETEXT.buildTerrainTradeString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)), widgetDataStruct.m_iData2);
	}
	else if (widgetDataStruct.m_iData2 == GC.getNumTerrainInfos())
	{
		GAMETEXT.buildRiverTradeString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
	}
}

void CvDLLWidgetData::parseSpecialBuildingHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildSpecialBuildingString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)), widgetDataStruct.m_iData2);
}

void CvDLLWidgetData::parseYieldChangeHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildYieldChangeString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)), widgetDataStruct.m_iData2, false);
}

// MOD - START - Advanced Yield and Commerce Effects
void CvDLLWidgetData::parseIrrigatedYieldChangeHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildIrrigatedYieldChangeString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)), widgetDataStruct.m_iData2, false);
}
// MOD - END - Advanced Yield and Commerce Effects

void CvDLLWidgetData::parseBonusRevealHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildBonusRevealString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)), widgetDataStruct.m_iData2, true);
}

void CvDLLWidgetData::parseCivicRevealHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildCivicRevealString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)), widgetDataStruct.m_iData2, true);
}

void CvDLLWidgetData::parseProcessInfoHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildProcessInfoString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)), widgetDataStruct.m_iData2, true);
}

void CvDLLWidgetData::parseFoundReligionHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildFoundReligionString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)), widgetDataStruct.m_iData2, true);
}

void CvDLLWidgetData::parseFoundCorporationHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildFoundCorporationString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)), widgetDataStruct.m_iData2, true);
}

void CvDLLWidgetData::parseFinanceNumUnits(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
//	szBuffer = "Number of units you are currently supporting";
	szBuffer.assign(gDLL->getText("TXT_KEY_ECON_NUM_UNITS_SUPPORTING"));
}

void CvDLLWidgetData::parseFinanceUnitCost(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
//	szBuffer = "The amount of money spent on unit upkeep";
	szBuffer.assign(gDLL->getText("TXT_KEY_ECON_MONEY_SPENT_UPKEEP"));
	if (widgetDataStruct.m_iData2 > 0)
	{
		GAMETEXT.buildFinanceUnitCostString(szBuffer, (PlayerTypes)widgetDataStruct.m_iData1);
	}
}

void CvDLLWidgetData::parseFinanceAwaySupply(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
//	szBuffer = "The amount of money spent on units in enemy territory";
	szBuffer.assign(gDLL->getText("TXT_KEY_ECON_AMOUNT_MONEY_UNITS_ENEMY_TERRITORY"));
	if (widgetDataStruct.m_iData2 > 0)
	{
		GAMETEXT.buildFinanceAwaySupplyString(szBuffer, (PlayerTypes)widgetDataStruct.m_iData1);
	}
}

void CvDLLWidgetData::parseFinanceCityMaint(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
//	szBuffer = "The amount of money spent doing city maintenance";
	szBuffer.assign(gDLL->getText("TXT_KEY_ECON_AMOUNT_MONEY_CITY_MAINT"));
	if (widgetDataStruct.m_iData2 > 0)
	{
		GAMETEXT.buildFinanceCityMaintString(szBuffer, (PlayerTypes)widgetDataStruct.m_iData1);
	}
}

void CvDLLWidgetData::parseFinanceCivicUpkeep(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
//	szBuffer = "The amount of money spent on Civics";
	szBuffer.assign(gDLL->getText("TXT_KEY_ECON_AMOUNT_MONEY_CIVIC_UPKEEP"));
	if (widgetDataStruct.m_iData2 > 0)
	{
		GAMETEXT.buildFinanceCivicUpkeepString(szBuffer, (PlayerTypes)widgetDataStruct.m_iData1);
	}
}

void CvDLLWidgetData::parseFinanceForeignIncome(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	szBuffer.assign(gDLL->getText("TXT_KEY_ECON_AMOUNT_MONEY_FOREIGN"));
	if (widgetDataStruct.m_iData2 > 0)
	{
		GAMETEXT.buildFinanceForeignIncomeString(szBuffer, (PlayerTypes)widgetDataStruct.m_iData1);
	}
}

void CvDLLWidgetData::parseFinanceInflatedCosts(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	szBuffer.assign(gDLL->getText("TXT_KEY_ECON_AMOUNT_MONEY_AFTER_INFLATION"));
	if (widgetDataStruct.m_iData2 > 0)
	{
		GAMETEXT.buildFinanceInflationString(szBuffer, (PlayerTypes)widgetDataStruct.m_iData1);
	}
}

void CvDLLWidgetData::parseFinanceGrossIncome(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
//	szBuffer = "Your gross income";
	szBuffer.assign(gDLL->getText("TXT_KEY_ECON_GROSS_INCOME"));
}

void CvDLLWidgetData::parseFinanceNetGold(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
//	szBuffer = "Net Gold per turn";
	szBuffer.assign(gDLL->getText("TXT_KEY_ECON_NET_GOLD"));
}

void CvDLLWidgetData::parseFinanceGoldReserve(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
//	szBuffer = "Amount of money in your gold reserves";
	szBuffer.assign(gDLL->getText("TXT_KEY_ECON_GOLD_RESERVE"));
}

void CvDLLWidgetData::parseUnitHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 != 0)
	{
		GAMETEXT.setUnitHelp(szBuffer, (UnitTypes)widgetDataStruct.m_iData1, false, false, widgetDataStruct.m_bOption, gDLL->getInterfaceIFace()->getHeadSelectedCity());
	}
}

void CvDLLWidgetData::parsePediaBack(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
//	szBuffer = "Back";
	szBuffer.assign(gDLL->getText("TXT_KEY_MISC_PEDIA_BACK"));
}

void CvDLLWidgetData::parsePediaForward(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
//	szBuffer = "Forward";
	szBuffer.assign(gDLL->getText("TXT_KEY_MISC_PEDIA_FORWARD"));
}

void CvDLLWidgetData::parseBonusHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 != 0)
	{
		GAMETEXT.setBonusHelp(szBuffer, (BonusTypes)widgetDataStruct.m_iData1);
	}
}

void CvDLLWidgetData::parseReligionHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 != 0)
	{
		GAMETEXT.setReligionHelp(szBuffer, (ReligionTypes)widgetDataStruct.m_iData1);
	}
}

void CvDLLWidgetData::parseReligionHelpCity( CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer )
{
	GAMETEXT.setReligionHelpCity(szBuffer, (ReligionTypes)widgetDataStruct.m_iData1, gDLL->getInterfaceIFace()->getHeadSelectedCity(), true);
}

void CvDLLWidgetData::parseCorporationHelpCity( CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer )
{
	GAMETEXT.setCorporationHelpCity(szBuffer, (CorporationTypes)widgetDataStruct.m_iData1, gDLL->getInterfaceIFace()->getHeadSelectedCity(), true);
}

void CvDLLWidgetData::parseCorporationHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 != 0)
	{
		GAMETEXT.setCorporationHelp(szBuffer, (CorporationTypes)widgetDataStruct.m_iData1);
	}
}

void CvDLLWidgetData::parsePromotionHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 != 0)
	{
		GAMETEXT.setPromotionHelp(szBuffer, (PromotionTypes)widgetDataStruct.m_iData1);
	}
}

// MOD - START - Aid
void CvDLLWidgetData::parseAidHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 != 0)
	{
		GAMETEXT.setAidHelp(szBuffer, (AidTypes)widgetDataStruct.m_iData1);
	}
}
// MOD - END - Aid

// MOD - START - Detriments
void CvDLLWidgetData::parseDetrimentHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 != 0)
	{
		GAMETEXT.setDetrimentHelp(szBuffer, (DetrimentTypes)widgetDataStruct.m_iData1);
	}
}
// MOD - END - Detriments

void CvDLLWidgetData::parseEventHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.setEventHelp(szBuffer, (EventTypes)widgetDataStruct.m_iData1, widgetDataStruct.m_iData2, GC.getGameINLINE().getActivePlayer());
}

void CvDLLWidgetData::parseUnitCombatHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 != 0)
	{
		GAMETEXT.setUnitCombatHelp(szBuffer, (UnitCombatTypes)widgetDataStruct.m_iData1);
	}
}

void CvDLLWidgetData::parseImprovementHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 != 0)
	{
		GAMETEXT.setImprovementHelp(szBuffer, (ImprovementTypes)widgetDataStruct.m_iData1);
	}
}

void CvDLLWidgetData::parseCivicHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 != 0)
	{
		GAMETEXT.parseCivicInfo(szBuffer, (CivicTypes)widgetDataStruct.m_iData1);
	}
}

void CvDLLWidgetData::parseCivilizationHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 != 0)
	{
		GAMETEXT.parseCivInfos(szBuffer, (CivilizationTypes)widgetDataStruct.m_iData1);
	}
}

void CvDLLWidgetData::parseLeaderHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	// MOD - START - Improved Civilopedia
	/*
	if (widgetDataStruct.m_iData2 != -1)
	{
		GAMETEXT.parseLeaderTraits(szBuffer, (LeaderHeadTypes)widgetDataStruct.m_iData1, (CivilizationTypes)widgetDataStruct.m_iData2);
	}
	*/
	// MOD - END - Improved Civilopedia
	GAMETEXT.parseLeaderTraits(szBuffer, (LeaderHeadTypes)widgetDataStruct.m_iData1, (CivilizationTypes)widgetDataStruct.m_iData2);
}

// MOD - START - Improved Civilopedia
void CvDLLWidgetData::parseTraitHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.parseTraits(szBuffer, (TraitTypes)widgetDataStruct.m_iData1, (CivilizationTypes)widgetDataStruct.m_iData2);
}
// MOD - END - Improved Civilopedia

void CvDLLWidgetData::parseCloseScreenHelp(CvWStringBuffer& szBuffer)
{
	szBuffer.assign(gDLL->getText("TXT_KEY_MISC_CLOSE_SCREEN"));
}

void CvDLLWidgetData::parseDescriptionHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer, bool bMinimal)
{
	CivilopediaPageTypes eType = (CivilopediaPageTypes)widgetDataStruct.m_iData1;
	switch (eType)
	{
	case CIVILOPEDIA_PAGE_TECHNOLOGY:
		{
			TechTypes eTech = (TechTypes)widgetDataStruct.m_iData2;
			if (NO_TECH != eTech)
			{
				szBuffer.assign(bMinimal ? GC.getTechInfo(eTech).getDescription() : gDLL->getText("TXT_KEY_MISC_HISTORICAL_INFO", GC.getTechInfo(eTech).getTextKeyWide()));
			}
		}
		break;
	case CIVILOPEDIA_PAGE_UNIT:
		{
			UnitTypes eUnit = (UnitTypes)widgetDataStruct.m_iData2;
			if (NO_UNIT != eUnit)
			{
				szBuffer.assign(bMinimal ? GC.getUnitInfo(eUnit).getDescription() : gDLL->getText("TXT_KEY_MISC_HISTORICAL_INFO", GC.getUnitInfo(eUnit).getTextKeyWide()));
			}
		}
		break;
	case CIVILOPEDIA_PAGE_BUILDING:
	case CIVILOPEDIA_PAGE_WONDER:
		{
			BuildingTypes eBuilding = (BuildingTypes)widgetDataStruct.m_iData2;
			if (NO_BUILDING != eBuilding)
			{
				szBuffer.assign(bMinimal ? GC.getBuildingInfo(eBuilding).getDescription() : gDLL->getText("TXT_KEY_MISC_HISTORICAL_INFO", GC.getBuildingInfo(eBuilding).getTextKeyWide()));
			}
		}
		break;
	case CIVILOPEDIA_PAGE_BONUS:
		{
			BonusTypes eBonus = (BonusTypes)widgetDataStruct.m_iData2;
			if (NO_BONUS != eBonus)
			{
				szBuffer.assign(bMinimal ? GC.getBonusInfo(eBonus).getDescription() : gDLL->getText("TXT_KEY_MISC_HISTORICAL_INFO", GC.getBonusInfo(eBonus).getTextKeyWide()));
			}
		}
		break;
	case CIVILOPEDIA_PAGE_IMPROVEMENT:
		{
			ImprovementTypes eImprovement = (ImprovementTypes)widgetDataStruct.m_iData2;
			if (NO_IMPROVEMENT != eImprovement)
			{
				szBuffer.assign(bMinimal ? GC.getImprovementInfo(eImprovement).getDescription() : gDLL->getText("TXT_KEY_MISC_HISTORICAL_INFO", GC.getImprovementInfo(eImprovement).getTextKeyWide()));
			}
		}
		break;
	case CIVILOPEDIA_PAGE_UNIT_COMBAT:
		{
			UnitCombatTypes eGroup = (UnitCombatTypes)widgetDataStruct.m_iData2;
			if (NO_UNITCOMBAT != eGroup)
			{
				szBuffer.assign(bMinimal ? GC.getUnitCombatInfo(eGroup).getDescription() : gDLL->getText("TXT_KEY_MISC_HISTORICAL_INFO", GC.getUnitCombatInfo(eGroup).getTextKeyWide()));
			}
		}
		break;
	case CIVILOPEDIA_PAGE_PROMOTION:
		{
			PromotionTypes ePromo = (PromotionTypes)widgetDataStruct.m_iData2;
			if (NO_PROMOTION != ePromo)
			{
				szBuffer.assign(bMinimal ? GC.getPromotionInfo(ePromo).getDescription() : gDLL->getText("TXT_KEY_MISC_HISTORICAL_INFO", GC.getPromotionInfo(ePromo).getTextKeyWide()));
			}
		}
		break;
	case CIVILOPEDIA_PAGE_CIVILIZATION:
		{
			CivilizationTypes eCiv = (CivilizationTypes)widgetDataStruct.m_iData2;
			if (NO_CIVILIZATION != eCiv)
			{
				szBuffer.assign(bMinimal ? GC.getCivilizationInfo(eCiv).getDescription() : gDLL->getText("TXT_KEY_MISC_HISTORICAL_INFO", GC.getCivilizationInfo(eCiv).getTextKeyWide()));
			}
		}
		break;
	case CIVILOPEDIA_PAGE_LEADER:
		{
			LeaderHeadTypes eLeader = (LeaderHeadTypes)widgetDataStruct.m_iData2;
			if (NO_LEADER != eLeader)
			{
				szBuffer.assign(bMinimal ? GC.getLeaderHeadInfo(eLeader).getDescription() : gDLL->getText("TXT_KEY_MISC_HISTORICAL_INFO", GC.getLeaderHeadInfo(eLeader).getTextKeyWide()));
			}
		}
		break;
	case CIVILOPEDIA_PAGE_RELIGION:
		{
			ReligionTypes eReligion = (ReligionTypes)widgetDataStruct.m_iData2;
			if (NO_RELIGION != eReligion)
			{
				szBuffer.assign(bMinimal ? GC.getReligionInfo(eReligion).getDescription() : gDLL->getText("TXT_KEY_MISC_HISTORICAL_INFO", GC.getReligionInfo(eReligion).getTextKeyWide()));
			}
		}
		break;
	case CIVILOPEDIA_PAGE_CORPORATION:
		{
			CorporationTypes eCorporation = (CorporationTypes)widgetDataStruct.m_iData2;
			if (NO_CORPORATION != eCorporation)
			{
				szBuffer.assign(bMinimal ? GC.getCorporationInfo(eCorporation).getDescription() : gDLL->getText("TXT_KEY_MISC_HISTORICAL_INFO", GC.getCorporationInfo(eCorporation).getTextKeyWide()));
			}
		}
		break;
	case CIVILOPEDIA_PAGE_CIVIC:
		{
			CivicTypes eCivic = (CivicTypes)widgetDataStruct.m_iData2;
			if (NO_CIVIC != eCivic)
			{
				szBuffer.assign(bMinimal ? GC.getCivicInfo(eCivic).getDescription() : gDLL->getText("TXT_KEY_MISC_HISTORICAL_INFO", GC.getCivicInfo(eCivic).getTextKeyWide()));
			}
		}
		break;
	case CIVILOPEDIA_PAGE_PROJECT:
		{
			ProjectTypes eProject = (ProjectTypes)widgetDataStruct.m_iData2;
			if (NO_PROJECT != eProject)
			{
				szBuffer.assign(bMinimal ? GC.getProjectInfo(eProject).getDescription() : gDLL->getText("TXT_KEY_MISC_HISTORICAL_INFO", GC.getProjectInfo(eProject).getTextKeyWide()));
			}
		}
		break;
	case CIVILOPEDIA_PAGE_CONCEPT:
		{
			ConceptTypes eConcept = (ConceptTypes)widgetDataStruct.m_iData2;
			if (NO_CONCEPT != eConcept)
			{
				szBuffer.assign(GC.getConceptInfo(eConcept).getDescription());
			}
		}
		break;
	/*
	case CIVILOPEDIA_PAGE_CONCEPT_NEW:
		{
			NewConceptTypes eConcept = (NewConceptTypes)widgetDataStruct.m_iData2;
			if (NO_NEW_CONCEPT != eConcept)
			{
				szBuffer.assign(GC.getNewConceptInfo(eConcept).getDescription());
			}
		}
		break;
	*/
	case CIVILOPEDIA_PAGE_SPECIALIST:
		{
			SpecialistTypes eSpecialist = (SpecialistTypes)widgetDataStruct.m_iData2;
			if (NO_SPECIALIST != eSpecialist)
			{
				szBuffer.assign(bMinimal ? GC.getSpecialistInfo(eSpecialist).getDescription() : gDLL->getText("TXT_KEY_MISC_HISTORICAL_INFO", GC.getSpecialistInfo(eSpecialist).getTextKeyWide()));
			}
		}
		break;
	default:
		break;
	}
}

void CvDLLWidgetData::parseKillDealHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
//	szBuffer = "Click to cancel";
	CvWString szTemp;
	szTemp = szBuffer.getCString();
	CvDeal* pDeal = GC.getGameINLINE().getDeal(widgetDataStruct.m_iData1);
	if (NULL != pDeal)
	{
		if (pDeal->isCancelable(GC.getGameINLINE().getActivePlayer(), &szTemp))
		{
			szTemp = gDLL->getText("TXT_KEY_MISC_CLICK_TO_CANCEL");
		}
	}

	szBuffer.assign(szTemp);
}


void CvDLLWidgetData::doDealKill(CvWidgetDataStruct &widgetDataStruct)
{
	CvDeal* pDeal = GC.getGameINLINE().getDeal(widgetDataStruct.m_iData1);
	if (pDeal != NULL)
	{
		if (!pDeal->isCancelable(GC.getGameINLINE().getActivePlayer()))
		{
			CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_TEXT);
			if (NULL != pInfo)
			{
				pInfo->setText(gDLL->getText("TXT_KEY_POPUP_CANNOT_CANCEL_DEAL"));
				gDLL->getInterfaceIFace()->addPopup(pInfo, GC.getGameINLINE().getActivePlayer(), true);
			}
		}
		else
		{
			CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_DEAL_CANCELED);
			if (NULL != pInfo)
			{
				pInfo->setData1(pDeal->getID());
				pInfo->setOption1(false);
				gDLL->getInterfaceIFace()->addPopup(pInfo, GC.getGameINLINE().getActivePlayer(), true);
			}
		}
	}
}


void CvDLLWidgetData::doRefreshMilitaryAdvisor(CvWidgetDataStruct &widgetDataStruct)
{
	CyArgsList argsList;
	argsList.add(widgetDataStruct.m_iData1);
	argsList.add(widgetDataStruct.m_iData2);
	gDLL->getPythonIFace()->callFunction(PYScreensModule, "refreshMilitaryAdvisor", argsList.makeFunctionArgs());
}

void CvDLLWidgetData::parseProductionModHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();
	if (NULL != pCity)
	{
		GAMETEXT.setProductionHelp(szBuffer, *pCity);
	}
}

void CvDLLWidgetData::parseLeaderheadHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.parseLeaderHeadHelp(szBuffer, (PlayerTypes)widgetDataStruct.m_iData1, (PlayerTypes)widgetDataStruct.m_iData2);
}

void CvDLLWidgetData::parseLeaderLineHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.parseLeaderLineHelp(szBuffer, (PlayerTypes)widgetDataStruct.m_iData1, (PlayerTypes)widgetDataStruct.m_iData2);
}

void CvDLLWidgetData::parseCommerceModHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();
	if (NULL != pCity)
	{
		GAMETEXT.setCommerceHelp(szBuffer, *pCity, (CommerceTypes)widgetDataStruct.m_iData1);
	}
}

void CvDLLWidgetData::parseScoreHelp(CvWidgetDataStruct& widgetDataStruct, CvWStringBuffer& szBuffer)
{
	GAMETEXT.setScoreHelp(szBuffer, (PlayerTypes)widgetDataStruct.m_iData1);
}

// MOD - START - Epidemics
// Epidemic system by Mexico
void CvDLLWidgetData::parseEpidemicHelp(CvWidgetDataStruct& widgetDataStruct, CvWStringBuffer& szBuffer)
{
	CvCity* pHeadSelectedCity;

	pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity != NULL)
	{
		GAMETEXT.setEpidemicHelp(szBuffer, *pHeadSelectedCity);
	}
}

void CvDLLWidgetData::parseEpidemicGlobalModifierHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildEpidemicGlobalModifierString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}
// MOD - END - Epidemics

void CvDLLWidgetData::parseAhead(CvWidgetDataStruct& widgetDataStruct, CvWStringBuffer& szBuffer)
{
	GAMETEXT.setAheadHelp(szBuffer);
}

// MOD - START - Congestion
void CvDLLWidgetData::parseCongestionContextHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	const CvUnit* pHeadSelectedUnit = gDLL->getInterfaceIFace()->getHeadSelectedUnit();
	if (pHeadSelectedUnit != NULL)
	{
		GAMETEXT.setCongestionContextHelp(szBuffer, pHeadSelectedUnit->plot(), pHeadSelectedUnit->getTeam(), pHeadSelectedUnit->getDomainType());
		return;
	}
}

void CvDLLWidgetData::parseCongestionPreviousLevelHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	const CvUnit* pHeadSelectedUnit = gDLL->getInterfaceIFace()->getHeadSelectedUnit();
	if (pHeadSelectedUnit != NULL)
	{
		GAMETEXT.setCongestionPreviousLevelHelp(szBuffer, pHeadSelectedUnit->plot(), pHeadSelectedUnit->getTeam(), pHeadSelectedUnit->getDomainType());
		return;
	}
}

void CvDLLWidgetData::parseCongestionCurrentLevelHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	const CvUnit* pHeadSelectedUnit = gDLL->getInterfaceIFace()->getHeadSelectedUnit();
	if (pHeadSelectedUnit != NULL)
	{
		GAMETEXT.setCongestionCurrentLevelHelp(szBuffer, pHeadSelectedUnit->plot(), pHeadSelectedUnit->getTeam(), pHeadSelectedUnit->getDomainType());
		return;
	}
}

void CvDLLWidgetData::parseCongestionNextLevelHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	const CvUnit* pHeadSelectedUnit = gDLL->getInterfaceIFace()->getHeadSelectedUnit();
	if (pHeadSelectedUnit != NULL)
	{
		GAMETEXT.setCongestionNextLevelHelp(szBuffer, pHeadSelectedUnit->plot(), pHeadSelectedUnit->getTeam(), pHeadSelectedUnit->getDomainType());
		return;
	}
}

void CvDLLWidgetData::parseCongestionBarHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	const CongestionTypes eCongestion = (CongestionTypes)widgetDataStruct.m_iData1;

	const CvUnit* pHeadSelectedUnit = gDLL->getInterfaceIFace()->getHeadSelectedUnit();
	if (pHeadSelectedUnit != NULL)
	{
		if (eCongestion != NO_CONGESTION)
		{
			GAMETEXT.setCongestionHelp(szBuffer, eCongestion, pHeadSelectedUnit->plot(), pHeadSelectedUnit->getTeam());
		}
		else
		{
			GAMETEXT.setBelowMinimumCongestionHelp(szBuffer, pHeadSelectedUnit->plot(), pHeadSelectedUnit->getTeam(), pHeadSelectedUnit->getDomainType());
		}
		return;
	}
}

void CvDLLWidgetData::parseCongestionInsideCityThresholdChangeHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildInsideCityCongestionThresholdChangeString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseCongestionOutsideCityThresholdChangeHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildOutsideCityCongestionThresholdChangeString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}
// MOD - END - Congestion

// MOD - START - Population Metrics
void CvDLLWidgetData::parseCityStabilityHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity;

	pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity != NULL)
	{
		GAMETEXT.setCityStabilityHelp(szBuffer, *pHeadSelectedCity);
	}
}
// MOD - END - Population Metrics


/*
** K-Mod, 5/jan/11, karadoc
** Environmental advisor mouse-over text
*/
void CvDLLWidgetData::parsePollutionOffsetsHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	szBuffer.append(gDLL->getText("TXT_KEY_POLLUTION_OFFSETS_HELP"));

	for (int iI = 0; iI < GC.getNumFeatureInfos(); ++iI)
	{
		int iWarmingDefence = GC.getFeatureInfo((FeatureTypes)iI).getWarmingDefense();

		if (iWarmingDefence != 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_OFFSET_PER_FEATURE", -iWarmingDefence, GC.getFeatureInfo((FeatureTypes)iI).getTextKeyWide()));
		}
	}
}

void CvDLLWidgetData::parsePollutionHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	int iFlags = (int)widgetDataStruct.m_iData1;

	szBuffer.append(gDLL->getText("TXT_KEY_POLLUTION")+":");

	if (iFlags & POLLUTION_POPULATION)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_POLLUTION_FROM_POPULATION", GC.getDefineINT("GLOBAL_WARMING_POPULATION_WEIGHT")));
	}
	if (iFlags & POLLUTION_BUILDINGS)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_POLLUTION_FROM_BUILDINGS", GC.getDefineINT("GLOBAL_WARMING_BUILDING_WEIGHT")));
	}
	if (iFlags & POLLUTION_BONUSES)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_POLLUTION_FROM_BONUSES", GC.getDefineINT("GLOBAL_WARMING_BONUS_WEIGHT")));
	}
	if (iFlags & POLLUTION_POWER)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_POLLUTION_FROM_POWER", GC.getDefineINT("GLOBAL_WARMING_POWER_WEIGHT")));
	}
}
/*
** K-Mod end
*/