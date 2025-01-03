#pragma once

//  $Header:
//------------------------------------------------------------------------------------------------
//
//  FILE:    CvGameTextMgr.h
//
//  AUTHOR:  Jesse Smith  --  10/2004
//
//  PURPOSE: Group of functions to manage CIV Game Text
//
//------------------------------------------------------------------------------------------------
//  Copyright (c) 2004 Firaxis Games, Inc. All rights reserved.
//------------------------------------------------------------------------------------------------
#ifndef CIV4_GAME_TEXT_MGR_H
#define CIV4_GAME_TEXT_MGR_H

#include "CvInfos.h"
//#include "CvEnums.h"

#if not defined(__GNUC__)
#pragma warning( disable: 4251 )	// needs to have dll-interface to be used by clients of class
#endif

class CvCity;
class CvDeal;
class CvPopupInfo;
class CvPlayer;

//
// Class:		CvGameTextMgr
// Purpose:		Manages Game Text...
class CvGameTextMgr
{
	friend class CvGlobals;
public:
	// singleton accessor
	DllExport static CvGameTextMgr& GetInstance();

	CvGameTextMgr();
	virtual ~CvGameTextMgr();

	DllExport void Initialize();
	DllExport void DeInitialize();
	DllExport void Reset();

	int getCurrentLanguage();

	DllExport void setTimeStr(CvWString& szString, int iGameTurn, bool bSave);
	void setYearStr(CvWString& szString, int iGameTurn, bool bSave, CalendarTypes eCalendar, int iStartYear, GameSpeedTypes eSpeed);
	void setDateStr(CvWString& szString, int iGameTurn, bool bSave, CalendarTypes eCalendar, int iStartYear, GameSpeedTypes eSpeed);
	void setInterfaceTime(CvWString& szString, PlayerTypes ePlayer);
	void setGoldStr(CvWString& szString, PlayerTypes ePlayer);
	void setResearchStr(CvWString& szString, PlayerTypes ePlayer);
	void setOOSSeeds(CvWString& szString, PlayerTypes ePlayer);
	void setNetStats(CvWString& szString, PlayerTypes ePlayer);
	DllExport void setMinimizePopupHelp(CvWString& szString, const CvPopupInfo & info);

	DllExport void setUnitHelp(CvWStringBuffer &szString, const CvUnit* pUnit, bool bOneLine = false, bool bShort = false);
	void setPlotListHelp(CvWStringBuffer &szString, CvPlot* pPlot, bool bOneLine, bool bShort);
	bool setCombatPlotHelp(CvWStringBuffer &szString, CvPlot* pPlot);
	void setPlotHelp(CvWStringBuffer &szString, CvPlot* pPlot);
	void setCityBarHelp(CvWStringBuffer &szString, CvCity* pCity);
	void setScoreHelp(CvWStringBuffer &szString, PlayerTypes ePlayer);

	// MOD - START - Improved Civilopedia
	//DllExport void parseTraits(CvWStringBuffer &szHelpString, TraitTypes eTrait, CivilizationTypes eCivilization = NO_CIVILIZATION, bool bDawnOfMan = false);
	void parseTraits(CvWStringBuffer &szHelpString, TraitTypes eTrait, CivilizationTypes eCivilization = NO_CIVILIZATION, bool bDawnOfMan = false, bool bCivilopediaText = false);
	// MOD - END - Improved Civilopedia
	DllExport void parseLeaderTraits(CvWStringBuffer &szInfoText, LeaderHeadTypes eLeader = NO_LEADER, CivilizationTypes eCivilization = NO_CIVILIZATION, bool bDawnOfMan = false, bool bCivilopediaText = false);
	DllExport void parseLeaderShortTraits(CvWStringBuffer &szInfoText, LeaderHeadTypes eLeader);
	DllExport void parseCivInfos(CvWStringBuffer &szHelpString, CivilizationTypes eCivilization, bool bDawnOfMan = false, bool bLinks = true);
	void parseSpecialistHelp(CvWStringBuffer &szHelpString, SpecialistTypes eSpecialist, CvCity* pCity, bool bCivilopediaText = false);
	void parseFreeSpecialistHelp(CvWStringBuffer &szHelpString, const CvCity& kCity);
	void parsePromotionHelp(CvWStringBuffer &szBuffer, PromotionTypes ePromotion, const wchar* pcNewline = NEWLINE);
	// MOD - START - Aid
	void parseAidHelp(CvWStringBuffer &szBuffer, AidTypes eAid, const wchar* pcNewline = NEWLINE);
	// MOD - END - Aid
	// MOD - START - Detriments
	void parseDetrimentHelp(CvWStringBuffer &szBuffer, DetrimentTypes eDetriment, const wchar* pcNewline = NEWLINE);
	// MOD - END - Detriments
	void parseCivicInfo(CvWStringBuffer &szBuffer, CivicTypes eCivic, bool bCivilopediaText = false, bool bPlayerContext = false, bool bSkipName = false);
	void parsePlayerTraits(CvWStringBuffer &szBuffer, PlayerTypes ePlayer);
	void parseLeaderHeadHelp(CvWStringBuffer &szBuffer, PlayerTypes eThisPlayer, PlayerTypes eOtherPlayer);
	void parseLeaderLineHelp(CvWStringBuffer &szBuffer, PlayerTypes eThisPlayer, PlayerTypes eOtherPlayer);
	void parseGreatPeopleHelp(CvWStringBuffer &szBuffer, CvCity& city);
// BUG - Building Additional Great People - start
	bool setBuildingAdditionalGreatPeopleHelp(CvWStringBuffer &szBuffer, const CvCity& city, const CvWString& szStart, bool bStarted = false);
// BUG - Building Additional Great People - end
	void parseGreatGeneralHelp(CvWStringBuffer &szBuffer, CvPlayer& kPlayer);

	// MOD - START - Improved Help Context
	//DllExport void setTechHelp(CvWStringBuffer &szBuffer, TechTypes eTech, bool bCivilopediaText = false, bool bPlayerContext = false, bool bStrategyText = false, bool bTreeInfo = false, TechTypes eFromTech = NO_TECH);
	void setTechHelp(CvWStringBuffer &szBuffer, TechTypes eTech, CivilizationTypes eCivilization = NO_CIVILIZATION, PlayerTypes ePlayer = NO_PLAYER, bool bCivilopediaText = false, bool bStrategyText = false, bool bTreeInfo = false, TechTypes eFromTech = NO_TECH);
	// MOD - END - Improved Help Context
	void setBasicUnitHelp(CvWStringBuffer &szBuffer, UnitTypes eUnit, bool bCivilopediaText = false);
	void setUnitHelp(CvWStringBuffer &szBuffer, UnitTypes eUnit, bool bCivilopediaText = false, bool bStrategyText = false, bool bTechChooserText = false, CvCity* pCity = NULL);
	void setBuildingHelp(CvWStringBuffer &szBuffer, BuildingTypes eBuilding, bool bCivilopediaText = false, bool bStrategyText = false, bool bTechChooserText = false, CvCity* pCity = NULL);
// BUG - Building Additional Happiness - start
	void setBuildingNetEffectsHelp(CvWStringBuffer &szBuffer, BuildingTypes eBuilding, CvCity* pCity);
	void setBuildingHelpActual(CvWStringBuffer &szBuffer, BuildingTypes eBuilding, bool bCivilopediaText = false, bool bStrategyText = false, bool bTechChooserText = false, CvCity* pCity = NULL, bool bActual = true);
// BUG - Building Additional Happiness - end
	void setProjectHelp(CvWStringBuffer &szBuffer, ProjectTypes eProject, bool bCivilopediaText = false, CvCity* pCity = NULL);
	void setProcessHelp(CvWStringBuffer &szBuffer, ProcessTypes eProcess);
	void setGoodHealthHelp(CvWStringBuffer &szBuffer, CvCity& city);
	void setBadHealthHelp(CvWStringBuffer &szBuffer, CvCity& city);
// BUG - Building Additional Health - start
	bool setBuildingAdditionalHealthHelp(CvWStringBuffer &szBuffer, const CvCity& city, const CvWString& szStart, bool bStarted = false);
// BUG - Building Additional Health - end
	void setAngerHelp(CvWStringBuffer &szBuffer, CvCity& city);
	void setHappyHelp(CvWStringBuffer &szBuffer, CvCity& city);
// BUG - Building Additional Happiness - start
	bool setBuildingAdditionalHappinessHelp(CvWStringBuffer &szBuffer, const CvCity& city, const CvWString& szStart, bool bStarted = false);
// BUG - Building Additional Happiness - end
	void setYieldChangeHelp(CvWStringBuffer &szBuffer, const CvWString& szStart, const CvWString& szSpace, const CvWString& szEnd, const int* piYieldChange, bool bPercent = false, bool bNewLine = true);
	void setCommerceChangeHelp(CvWStringBuffer &szBuffer, const CvWString& szStart, const CvWString& szSpace, const CvWString& szEnd, const int* piCommerceChange, bool bPercent = false, bool bNewLine = true);
// BUG - Resumable Value Change Help - start
	void setCommerceTimes100ChangeHelp(CvWStringBuffer &szBuffer, const CvWString& szStart, const CvWString& szSpace, const CvWString& szEnd, const int* piCommerceChange, bool bNewLine = false, bool bStarted = false);
	bool setResumableYieldChangeHelp(CvWStringBuffer &szBuffer, const CvWString& szStart, const CvWString& szSpace, const CvWString& szEnd, const int* piYieldChange, bool bPercent = false, bool bNewLine = true, bool bStarted = false);
	bool setResumableCommerceChangeHelp(CvWStringBuffer &szBuffer, const CvWString& szStart, const CvWString& szSpace, const CvWString& szEnd, const int* piCommerceChange, bool bPercent = false, bool bNewLine = true, bool bStarted = false);
	bool setResumableCommerceTimes100ChangeHelp(CvWStringBuffer &szBuffer, const CvWString& szStart, const CvWString& szSpace, const CvWString& szEnd, const int* piCommerceChange, bool bNewLine, bool bStarted = false);
	bool setResumableGoodBadChangeHelp(CvWStringBuffer &szBuffer, const CvWString& szStart, const CvWString& szSpace, const CvWString& szEnd, int iGood, int iGoodSymbol, int iBad, int iBadSymbol, bool bPercent = false, bool bNewLine = false, bool bStarted = false);
	bool setResumableValueChangeHelp(CvWStringBuffer &szBuffer, const CvWString& szStart, const CvWString& szSpace, const CvWString& szEnd, int iValue, int iSymbol, bool bPercent = false, bool bNewLine = false, bool bStarted = false);
	bool setResumableValueTimes100ChangeHelp(CvWStringBuffer &szBuffer, const CvWString& szStart, const CvWString& szSpace, const CvWString& szEnd, int iValue, int iSymbol, bool bNewLine = false, bool bStarted = false);
	bool setResumableValueTimes10ChangeHelp(CvWStringBuffer &szBuffer, const CvWString& szStart, const CvWString& szSpace, const CvWString& szEnd, int iValue, int iSymbol, bool bPercent = false, bool bNewLine = false, bool bStarted = false);
// BUG - Resumable Value Change Help - end
	void setBonusHelp(CvWStringBuffer &szBuffer, BonusTypes eBonus, bool bCivilopediaText = false);
	void setReligionHelp(CvWStringBuffer &szBuffer, ReligionTypes eReligion, bool bCivilopedia = false);
	void setReligionHelpCity(CvWStringBuffer &szBuffer, ReligionTypes eReligion, CvCity *pCity, bool bCityScreen = false, bool bForceReligion = false, bool bForceState = false, bool bNoStateReligion = false);
	void setCorporationHelp(CvWStringBuffer &szBuffer, CorporationTypes eCorporation, bool bCivilopedia = false);
	void setCorporationHelpCity(CvWStringBuffer &szBuffer, CorporationTypes eCorporation, CvCity *pCity, bool bCityScreen = false, bool bForceCorporation = false);
	void setPromotionHelp(CvWStringBuffer &szBuffer, PromotionTypes ePromotion, bool bCivilopediaText = false);
	// MOD - START - Aid
	void setAidHelp(CvWStringBuffer &szBuffer, AidTypes eAid, bool bCivilopediaText = false);
	// MOD - END - Aid
	// MOD - START - Detriments
	void setDetrimentHelp(CvWStringBuffer &szBuffer, DetrimentTypes eDetriment, bool bCivilopediaText = false);
	// MOD - END - Detriments
	void setUnitCombatHelp(CvWStringBuffer &szBuffer, UnitCombatTypes eUnitCombat);
	// MOD - START - Improved Civilopedia
	//DllExport void setImprovementHelp(CvWStringBuffer &szBuffer, ImprovementTypes eImprovement, bool bCivilopediaText = false);
	void setImprovementHelp(CvWStringBuffer &szBuffer, ImprovementTypes eImprovement, bool bCivilopediaEffects = false, bool bCivilopediaImprovements = false);
	// MOD - END - Improved Civilopedia
	void setTerrainHelp(CvWStringBuffer &szBuffer, TerrainTypes eTerrain, bool bCivilopediaText = false);
	void setFeatureHelp(CvWStringBuffer &szBuffer, FeatureTypes eFeature, bool bCivilopediaText = false);
	// MOD - START - Improved Civilopedia
	void setRouteHelp(CvWStringBuffer &szBuffer, RouteTypes eRoute, bool bCivilopediaText = false);
	void setWorldSizeHelp(CvWStringBuffer &szBuffer, WorldSizeTypes eWorldSize, bool bCivilopediaText = false);
	void setHandicapHelp(CvWStringBuffer &szBuffer, HandicapTypes eHandicap, bool bCivilopediaText = false);
	void setConceptHelp(CvWStringBuffer &szBuffer, ConceptTypes eConcept, bool bCivilopediaText = false);
	void setHotKeyClassHelp(CvWStringBuffer &szBuffer, HotKeyClassTypes eHotKeyClass, bool bCivilopediaText = false);
	// MOD - END - Improved Civilopedia
	// MOD - START - Era Effects
	void setEraHelp(CvWStringBuffer &szBuffer, EraTypes eEra, bool bSplash = false, bool bCivilopediaText = false);
	// MOD - END - Era Effects
// BUG - Building Additional info - start
	bool setBuildingAdditionalYieldHelp(CvWStringBuffer &szBuffer, const CvCity& city, YieldTypes eIndex, const CvWString& szStart, bool bStarted = false);
	bool setBuildingAdditionalCommerceHelp(CvWStringBuffer &szBuffer, const CvCity& city, CommerceTypes eIndex, const CvWString& szStart, bool bStarted = false);
	bool setBuildingSavedMaintenanceHelp(CvWStringBuffer &szBuffer, const CvCity& city, const CvWString& szStart, bool bStarted = false);
// BUG - Building Additional info - end
	void setProductionHelp(CvWStringBuffer &szBuffer, CvCity& city);
	void setCommerceHelp(CvWStringBuffer &szBuffer, CvCity& city, CommerceTypes eCommerceType);
	void setYieldHelp(CvWStringBuffer &szBuffer, CvCity& city, YieldTypes eYieldType);
	void setConvertHelp(CvWStringBuffer& szBuffer, PlayerTypes ePlayer, ReligionTypes eReligion);
	void setRevolutionHelp(CvWStringBuffer& szBuffer, PlayerTypes ePlayer);
	void setVassalRevoltHelp(CvWStringBuffer& szBuffer, TeamTypes eMaster, TeamTypes eVassal);
	void setEventHelp(CvWStringBuffer& szBuffer, EventTypes eEvent, int iEventTriggeredId, PlayerTypes ePlayer);
	void setTradeRouteHelp(CvWStringBuffer &szBuffer, int iRoute, CvCity* pCity);
	void setEspionageCostHelp(CvWStringBuffer &szBuffer, EspionageMissionTypes eMission, PlayerTypes eTargetPlayer, const CvPlot* pPlot, int iExtraData, const CvUnit* pSpyUnit);
	void setEspionageMissionHelp(CvWStringBuffer &szBuffer, const CvUnit* pUnit);
	// MOD - START - Epidemics
	// Epidemic system by Mexico
	void setEpidemicHelp(CvWStringBuffer &szBuffer, CvCity& city);
	// MOD - END - Epidemics
	void setAheadHelp(CvWStringBuffer &szBuffer);
	// MOD - START - Congestion
	void setCongestionContextHelp(CvWStringBuffer &szBuffer, const CvPlot* pPlot, TeamTypes eTeam, DomainTypes eDomain = DOMAIN_LAND);
	void setCongestionPreviousLevelHelp(CvWStringBuffer &szBuffer, const CvPlot* pPlot, TeamTypes eTeam, DomainTypes eDomain = DOMAIN_LAND);
	void setCongestionCurrentLevelHelp(CvWStringBuffer &szBuffer, const CvPlot* pPlot, TeamTypes eTeam, DomainTypes eDomain = DOMAIN_LAND);
	void setCongestionNextLevelHelp(CvWStringBuffer &szBuffer, const CvPlot* pPlot, TeamTypes eTeam, DomainTypes eDomain = DOMAIN_LAND);
	void setCongestionHelp(CvWStringBuffer &szBuffer, CongestionTypes eCongestion, const CvPlot* pPlot, TeamTypes eTeam = NO_TEAM, bool bEffects = true);
	void setNoCongestionHelp(CvWStringBuffer &szBuffer, const CvPlot* pPlot, TeamTypes eTeam, DomainTypes eDomain = DOMAIN_LAND);
	void setBelowMinimumCongestionHelp(CvWStringBuffer &szBuffer, const CvPlot* pPlot, TeamTypes eTeam, DomainTypes eDomain = DOMAIN_LAND);
	void setMaxCongestionHelp(CvWStringBuffer &szBuffer, const CvPlot* pPlot, TeamTypes eTeam, DomainTypes eDomain = DOMAIN_LAND);
	// MOD - END - Congestion
	// MOD - START - Population Metrics
	void setCityStabilityHelp(CvWStringBuffer &szBuffer, CvCity& city);
	// MOD - END - Population Metrics

	// MOD - START - Building Discontinuation
	void buildDiscontinueString(CvWStringBuffer& szBuffer, BuildingTypes eBuilding, bool bList = false, bool bPlayerContext = false);
	void buildDiscontinueSpecialString(CvWStringBuffer &szBuffer, SpecialBuildingTypes eSpecialBuilding, bool bList = false, bool bPlayerContext = false);
	// MOD - END - Building Discontinuation
	void buildObsoleteString( CvWStringBuffer& szBuffer, BuildingTypes eBuilding, bool bList = false, bool bPlayerContext = false );
	void buildObsoleteBonusString( CvWStringBuffer& szBuffer, BonusTypes eBonus, bool bList = false, bool bPlayerContext = false);
    // MOD - START - Unit Obsolete Tech (Dom Pedro II)
    void buildObsoleteUnitString( CvWStringBuffer& szBuffer, UnitTypes eUnit, bool bList = false, bool bPlayerContext = false);
    // MOD - END - Unit Obsolete Tech (Dom Pedro II)
	void buildObsoleteSpecialString( CvWStringBuffer& szBuffer, SpecialBuildingTypes eSpecialBuilding, bool bList = false, bool bPlayerContext = false );
	void buildMoveString( CvWStringBuffer& szBuffer, TechTypes eTech, bool bList = false, bool bPlayerContext = false );
	void buildFreeUnitString( CvWStringBuffer& szBuffer, TechTypes eTech, bool bList = false, bool bPlayerContext = false );
	void buildFeatureProductionString( CvWStringBuffer& szBuffer, TechTypes eTech, bool bList = false, bool bPlayerContext = false );
	void buildWorkerRateString( CvWStringBuffer& szBuffer, TechTypes eTech, bool bList = false, bool bPlayerContext = false );
	void buildTradeRouteString( CvWStringBuffer& szBuffer, TechTypes eTech, bool bList = false, bool bPlayerContext = false );
	// MOD - START - Congestion
	void buildInsideCityCongestionThresholdChangeString(CvWStringBuffer& szBuffer, TechTypes eTech, bool bList = false, bool bPlayerContext = false);
	void buildOutsideCityCongestionThresholdChangeString(CvWStringBuffer& szBuffer, TechTypes eTech, bool bList = false, bool bPlayerContext = false);
	// MOD - END - Congestion
	void buildHealthRateString( CvWStringBuffer& szBuffer, TechTypes eTech, bool bList = false, bool bPlayerContext = false );
	// MOD - START - Epidemics
	void buildEpidemicGlobalModifierString(CvWStringBuffer& szBuffer, TechTypes eTech, bool bList = false, bool bPlayerContext = false);
	// MOD - END - Epidemics
	void buildHappinessRateString( CvWStringBuffer& szBuffer, TechTypes eTech, bool bList = false, bool bPlayerContext = false );
	void buildFreeTechString( CvWStringBuffer& szBuffer, TechTypes eTech, bool bList = false, bool bPlayerContext = false );
	void buildLOSString( CvWStringBuffer& szBuffer, TechTypes eTech, bool bList = false, bool bPlayerContext = false );
	void buildMapCenterString( CvWStringBuffer& szBuffer, TechTypes eTech, bool bList = false, bool bPlayerContext = false );
	void buildMapRevealString( CvWStringBuffer& szBuffer, TechTypes eTech, bool bList = false );
	void buildMapTradeString( CvWStringBuffer& szBuffer, TechTypes eTech, bool bList = false, bool bPlayerContext = false );
	void buildTechTradeString( CvWStringBuffer& szBuffer, TechTypes eTech, bool bList = false, bool bPlayerContext = false );
	void buildGoldTradeString( CvWStringBuffer& szBuffer, TechTypes eTech, bool bList = false, bool bPlayerContext = false );
	void buildOpenBordersString( CvWStringBuffer& szBuffer, TechTypes eTech, bool bList = false, bool bPlayerContext = false );
	void buildDefensivePactString( CvWStringBuffer& szBuffer, TechTypes eTech, bool bList = false, bool bPlayerContext = false );
	void buildPermanentAllianceString( CvWStringBuffer& szBuffer, TechTypes eTech, bool bList = false, bool bPlayerContext = false );
	void buildVassalStateString( CvWStringBuffer& szBuffer, TechTypes eTech, bool bList = false, bool bPlayerContext = false );
	void buildBridgeString( CvWStringBuffer& szBuffer, TechTypes eTech, bool bList = false, bool bPlayerContext = false );
	void buildIrrigationString( CvWStringBuffer& szBuffer, TechTypes eTech, bool bList = false, bool bPlayerContext = false );
	void buildIgnoreIrrigationString( CvWStringBuffer& szBuffer, TechTypes eTech, bool bList = false, bool bPlayerContext = false );
	void buildWaterWorkString( CvWStringBuffer &szBuffer, TechTypes eTech, bool bList = false, bool bPlayerContext = false );
	// MOD - START - Improved Help Context
	//DllExport void buildImprovementString( CvWStringBuffer& szBuffer, TechTypes eTech, int iBuild, bool bList = false, bool bPlayerContext = false, bool bTreeInfo = false );
	void buildImprovementString( CvWStringBuffer& szBuffer, TechTypes eTech, BuildTypes eBuild, CivilizationTypes eCivilization = NO_CIVILIZATION, PlayerTypes ePlayer = NO_PLAYER, bool bList = false, bool bTreeInfo = false );
	// MOD - END - Improved Help Context
	void buildDomainExtraMovesString( CvWStringBuffer& szBuffer, TechTypes eTech, int iCommerceType, bool bList = false, bool bPlayerContext = false );
	void buildAdjustString( CvWStringBuffer& szBuffer, TechTypes eTech, int iCommerceType, bool bList = false, bool bPlayerContext = false );
	void buildTerrainTradeString( CvWStringBuffer& szBuffer, TechTypes eTech, int iTerrainType, bool bList = false, bool bPlayerContext = false );
	void buildRiverTradeString( CvWStringBuffer& szBuffer, TechTypes eTech, bool bList = false, bool bPlayerContext = false );
	void buildSpecialBuildingString( CvWStringBuffer& szBuffer, TechTypes eTech, int iBuildingType, bool bList = false, bool bPlayerContext = false );
	void buildYieldChangeString( CvWStringBuffer& szBuffer, TechTypes eTech, int iImprovementType, bool bList = false, bool bPlayerContext = false );
	// MOD - START - Advanced Yield and Commerce Effects
	void buildIrrigatedYieldChangeString( CvWStringBuffer& szBuffer, TechTypes eTech, int iImprovementType, bool bList = false, bool bPlayerContext = false );
	// MOD - END - Advanced Yield and Commerce Effects
	bool buildBonusRevealString( CvWStringBuffer& szBuffer, TechTypes eTech, int iBonusType, bool bFirst, bool bList = false, bool bPlayerContext = false );
	bool buildCivicRevealString( CvWStringBuffer& szBuffer, TechTypes eTech, int iCivicType, bool bFirst, bool bList = false, bool bPlayerContext = false );
	bool buildProcessInfoString( CvWStringBuffer& szBuffer, TechTypes eTech, int iProcessType, bool bFirst, bool bList = false, bool bPlayerContext = false );
	bool buildFoundReligionString( CvWStringBuffer& szBuffer, TechTypes eTech, int iReligionType, bool bFirst, bool bList = false, bool bPlayerContext = false );
	bool buildFoundCorporationString( CvWStringBuffer& szBuffer, TechTypes eTech, int iCorporationType, bool bFirst, bool bList = false, bool bPlayerContext = false );
	bool buildPromotionString( CvWStringBuffer& szBuffer, TechTypes eTech, int iPromotionType, bool bFirst, bool bList = false, bool bPlayerContext = false );
	void buildHintsList(CvWStringBuffer& szBuffer);
	void buildBuildingRequiresString(CvWStringBuffer& szBuffer, BuildingTypes eBuilding, bool bCivilopediaText, bool bTechChooserText, const CvCity* pCity);

	DllExport void buildCityBillboardIconString( CvWStringBuffer& szBuffer, CvCity* pCity);
	DllExport void buildCityBillboardCityNameString( CvWStringBuffer& szBuffer, CvCity* pCity);
	DllExport void buildCityBillboardProductionString( CvWStringBuffer& szBuffer, CvCity* pCity);
	DllExport void buildCityBillboardCitySizeString( CvWStringBuffer& szBuffer, CvCity* pCity, const NiColorA& kColor);
	DllExport void getCityBillboardFoodbarColors(CvCity* pCity, std::vector<NiColorA>& aColors);
	DllExport void getCityBillboardProductionbarColors(CvCity* pCity, std::vector<NiColorA>& aColors);

	void buildSingleLineTechTreeString(CvWStringBuffer &szBuffer, TechTypes eTech, bool bPlayerContext);
	// MOD - START - Improved Help Context
	//DllExport void buildTechTreeString(CvWStringBuffer &szBuffer, TechTypes eTech, bool bPlayerContext, TechTypes eFromTech);
	void buildTechTreeString(CvWStringBuffer &szBuffer, TechTypes eTech, CivilizationTypes eCivilization, PlayerTypes ePlayer, TechTypes eFromTech);
	// MOD - END - Improved Help Context

	void getWarplanString(CvWStringBuffer& szString, WarPlanTypes eWarPlan);
	void getAttitudeString(CvWStringBuffer& szBuffer, PlayerTypes ePlayer, PlayerTypes eTargetPlayer,
			bool bConstCache = false); // advc.sha
	void getVassalInfoString(CvWStringBuffer& szBuffer, PlayerTypes ePlayer); // K-Mod
	void getWarWearinessString(CvWStringBuffer& szBuffer, PlayerTypes ePlayer, PlayerTypes eTargetPlayer) const; // K-Mod
	void getEspionageString(CvWStringBuffer& szBuffer, PlayerTypes ePlayer, PlayerTypes eTargetPlayer);
	void getTradeString(CvWStringBuffer& szBuffer, const TradeData& tradeData, PlayerTypes ePlayer1, PlayerTypes ePlayer2);
	void getDealString(CvWStringBuffer& szString, CvDeal& deal, PlayerTypes ePlayerPerspective = NO_PLAYER);
	void getDealString(CvWStringBuffer& szBuffer, PlayerTypes ePlayer1, PlayerTypes ePlayer2, const CLinkList<TradeData>* pListPlayer1, const CLinkList<TradeData>* pListPlayer2, PlayerTypes ePlayerPerspective = NO_PLAYER);
	void getActiveDealsString(CvWStringBuffer& szString, PlayerTypes eThisPlayer, PlayerTypes eOtherPlayer);
	void getOtherRelationsString(CvWStringBuffer& szString, PlayerTypes eThisPlayer, PlayerTypes eOtherPlayer);

	void buildFinanceInflationString(CvWStringBuffer& szDetails, PlayerTypes ePlayer);
	void buildFinanceUnitCostString(CvWStringBuffer& szDetails, PlayerTypes ePlayer);
	void buildFinanceAwaySupplyString(CvWStringBuffer& szDetails, PlayerTypes ePlayer);
	void buildFinanceCityMaintString(CvWStringBuffer& szDetails, PlayerTypes ePlayer);
	void buildFinanceCivicUpkeepString(CvWStringBuffer& szDetails, PlayerTypes ePlayer);
	void buildFinanceForeignIncomeString(CvWStringBuffer& szDetails, PlayerTypes ePlayer);

	DllExport void getTradeScreenTitleIcon(CvString& szButton, CvWidgetDataStruct& widgetData, PlayerTypes ePlayer);
	DllExport void getTradeScreenIcons(std::vector< std::pair<CvString, CvWidgetDataStruct> >& aIconInfos, PlayerTypes ePlayer);
	DllExport void getTradeScreenHeader(CvWString& szHeader, PlayerTypes ePlayer, PlayerTypes eOtherPlayer, bool bAttitude);

	DllExport void getGlobeLayerName(GlobeLayerTypes eType, int iOption, CvWString& strName);

	DllExport void getPlotHelp(CvPlot* pMouseOverPlot, CvCity* pCity, CvPlot* pFlagPlot, bool bAlt, CvWStringBuffer& strHelp);
	void getRebasePlotHelp(CvPlot* pPlot, CvWString& strHelp);
	void getNukePlotHelp(CvPlot* pPlot, CvWString& strHelp);
	DllExport void getInterfaceCenterText(CvWString& strText);
	DllExport void getTurnTimerText(CvWString& strText);
	DllExport void getFontSymbols(std::vector< std::vector<wchar> >& aacSymbols, std::vector<int>& aiMaxNumRows);
	DllExport void assignFontIds(int iFirstSymbolCode, int iPadAmount);

	DllExport void getCityDataForAS(std::vector<CvWBData>& mapCityList, std::vector<CvWBData>& mapBuildingList, std::vector<CvWBData>& mapAutomateList);
	DllExport void getUnitDataForAS(std::vector<CvWBData>& mapUnitList);
	DllExport void getImprovementDataForAS(std::vector<CvWBData>& mapImprovementList, std::vector<CvWBData>& mapRouteList);
	DllExport void getVisibilityDataForAS(std::vector<CvWBData>& mapVisibilityList);
	DllExport void getTechDataForAS(std::vector<CvWBData>& mapTechList);

	DllExport void getUnitDataForWB(std::vector<CvWBData>& mapUnitData);
	DllExport void getBuildingDataForWB(bool bStickyButton, std::vector<CvWBData>& mapBuildingData);
	DllExport void getTerrainDataForWB(std::vector<CvWBData>& mapTerrainData, std::vector<CvWBData>& mapFeatureData, std::vector<CvWBData>& mapPlotData, std::vector<CvWBData>& mapRouteData);
	DllExport void getTerritoryDataForWB(std::vector<CvWBData>& mapTerritoryData);

	DllExport void getTechDataForWB(std::vector<CvWBData>& mapTechData);
	DllExport void getPromotionDataForWB(std::vector<CvWBData>& mapPromotionData);
	DllExport void getBonusDataForWB(std::vector<CvWBData>& mapBonusData);
	DllExport void getImprovementDataForWB(std::vector<CvWBData>& mapImprovementData);
	DllExport void getReligionDataForWB(bool bHolyCity, std::vector<CvWBData>& mapReligionData);
	DllExport void getCorporationDataForWB(bool bHeadquarters, std::vector<CvWBData>& mapCorporationData);

private:
	void eventTechHelp(CvWStringBuffer& szBuffer, EventTypes eEvent, TechTypes eTech, PlayerTypes ePlayer, PlayerTypes eOtherPlayer);
	void eventGoldHelp(CvWStringBuffer& szBuffer, EventTypes eEvent, PlayerTypes ePlayer, PlayerTypes eOtherPlayer);

	std::vector<int*> m_apbPromotion;

	// MOD - START - Aid and Detriments
	void setPlotAidAndDetrimentHelp(CvWStringBuffer& szString, CvPlot* pPlot);
	// MOD - END - Aid and Detriments

	//void setCityPlotYieldValueString(CvWStringBuffer &szString, CvCity* pCity, int iIndex, bool bAvoidGrowth, bool bIgnoreGrowth, bool bIgnoreFood = false);
	void setCityPlotYieldValueString(CvWStringBuffer &szString, CvCity* pCity, int iIndex, bool bIgnoreFood, int iGrowthValue);
	void setYieldValueString(CvWStringBuffer &szString, int iValue, bool bActive = false, bool bMakeWhitespace = false);
};

// Singleton Accessor
#define GAMETEXT CvGameTextMgr::GetInstance()

#endif
