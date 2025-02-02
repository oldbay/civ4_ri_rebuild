#pragma once

// player.h

#ifndef CIV4_PLAYER_H
#define CIV4_PLAYER_H

//#include "CvGameCoreDLL.h"
#include "CvCityAI.h"
#include "CvUnitAI.h"
#include "CvSelectionGroupAI.h"
#include "CvPlotGroup.h"
#include "LinkedList.h"
#include "CvTalkingHeadMessage.h"
#include "FFreeListTrashArray.h"

class CvDiploParameters;
class CvPopupInfo;
class CvEventTriggerInfo;
class CvPlayerRecord; // K-Mod

typedef std::list<CvTalkingHeadMessage> CvMessageQueue;
typedef std::list<CvPopupInfo*> CvPopupQueue;
typedef std::list<CvDiploParameters*> CvDiploQueue;
#if defined(__GNUC__)
typedef std::unordered_map<int, int> CvTurnScoreMap;
typedef std::unordered_map<EventTypes, EventTriggeredData> CvEventMap;
#else
typedef stdext::hash_map<int, int> CvTurnScoreMap;
typedef stdext::hash_map<EventTypes, EventTriggeredData> CvEventMap;
#endif
typedef std::vector< std::pair<UnitCombatTypes, PromotionTypes> > UnitCombatPromotionArray;
typedef std::vector< std::pair<UnitClassTypes, PromotionTypes> > UnitClassPromotionArray;
typedef std::vector< std::pair<CivilizationTypes, LeaderHeadTypes> > CivLeaderArray;

class CvPlayer
{
public:
	CvPlayer();
	virtual ~CvPlayer();

	DllExport void init(PlayerTypes eID);
	DllExport void setupGraphical();
	DllExport void reset(PlayerTypes eID = NO_PLAYER, bool bConstructorCall = false);

/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      12/30/08                                jdog5000      */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	void initInGame(PlayerTypes eID);
	void resetPlotAndCityData( );
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/

/************************************************************************************************/
/* CHANGE_PLAYER                          12/30/08                                jdog5000      */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	void clearTraitBonuses();
	void addTraitBonuses();
	void changePersonalityType();
	void resetCivTypeEffects();
	void changeLeader( LeaderHeadTypes eNewLeader );
	void changeCiv( CivilizationTypes eNewCiv );
	void setIsHuman( bool bNewValue );
/************************************************************************************************/
/* CHANGE_PLAYER                           END                                                  */
/************************************************************************************************/

protected:

	void uninit();

public:

	void initFreeState();
	void initFreeUnits();
	void addFreeUnitAI(UnitAITypes eUnitAI, int iCount);
	void addFreeUnit(UnitTypes eUnit, UnitAITypes eUnitAI = NO_UNITAI);

	int startingPlotRange() const;																																									// Exposed to Python
	bool startingPlotWithinRange(CvPlot* pPlot, PlayerTypes ePlayer, int iRange, int iPass) const;									// Exposed to Python
	int startingPlotDistanceFactor(CvPlot* pPlot, PlayerTypes ePlayer, int iRange) const;
	int findStartingArea() const;
	CvPlot* findStartingPlot(bool bRandomize = false);																																									// Exposed to Python

	CvPlotGroup* initPlotGroup(CvPlot* pPlot);													

	// MOD - START - Captured City Highest Culture Art (DPII 2008-07-28)
	/*
	CvCity* initCity(int iX, int iY, bool bBumpUnits, bool bUpdatePlotGroups);																																// Exposed to Python
	*/
	CvCity* initCity(int iX, int iY, bool bBumpunits, bool bUpdatePlotGroups, PlayerTypes eHighestCulturePlayer);																																// Exposed to Python
	// MOD - END - Captured City Highest Culture Art (DPII 2008-07-28)
	void acquireCity(CvCity* pCity, bool bConquest, bool bTrade, bool bUpdatePlotGroups);																							// Exposed to Python
	void killCities();																																												// Exposed to Python
	CvWString getNewCityName() const;																																								// Exposed to Python
	void getCivilizationCityName(CvWString& szBuffer, CivilizationTypes eCivilization) const;
	bool isCityNameValid(CvWString& szName, bool bTestDestroyed = true) const;

	CvUnit* initUnit(UnitTypes eUnit, int iX, int iY, UnitAITypes eUnitAI = NO_UNITAI, DirectionTypes eFacingDirection = NO_DIRECTION);							// Exposed to Python
	void setFlagDecal(CvWString const& szFlagDecal, bool bUpdate); // advc.127c
	void disbandUnit(bool bAnnounce);																																					// Exposed to Python
	void killUnits();																																													// Exposed to Python

	CvSelectionGroup* cycleSelectionGroups(CvUnit* pUnit, bool bForward, bool bWorkers, bool* pbWrap);

	bool hasTrait(TraitTypes eTrait) const;																																			// Exposed to Python						
/************************************************************************************************/
/* AI_AUTO_PLAY_MOD                       07/09/08                                jdog5000      */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	void setHumanDisabled( bool newVal );
	bool isHumanDisabled( );
/************************************************************************************************/
/* AI_AUTO_PLAY_MOD                        END                                                  */
/************************************************************************************************/
	DllExport bool isHuman() const;																																							// Exposed to Python						
	DllExport void updateHuman();
	DllExport bool isBarbarian() const;																																					// Exposed to Python						

	// K-Mod note: I've changed getName, getCivilizationDescription, and getCivilizationShortDescription to only give accurate information if the active player has met this player.
	// The "key" versions of those functions are unchanged. This is important because getNameKey and so on are used to create messages for the replay.
	DllExport const wchar* getName(uint uiForm = 0) const;																											// Exposed to Python
	const wchar* getReplayName(uint uiForm = 0) const; // K-Mod. Player name to be used in replay
/*************************************************************************************************/
/** REVOLUTION_MOD                         01/15/08                                jdog5000      */
/**                                                                                              */
/** Used for dynamic civ names                                                                   */
/*************************************************************************************************/
	void setName(std::wstring szNewValue);															// Exposed to Python																// Exposed to Python
	void setCivName(std::wstring szNewDesc, std::wstring szNewShort, std::wstring szNewAdj);																														// Exposed to Python
/*************************************************************************************************/
/** REVOLUTION_MOD                          END                                                  */
/*************************************************************************************************/
	DllExport const wchar* getNameKey() const;																																	// Exposed to Python
	DllExport const wchar* getCivilizationDescription(uint uiForm = 0) const;																		// Exposed to Python
	const wchar* getCivilizationDescriptionKey() const;																								// Exposed to Python
	const wchar* getCivilizationShortDescription(uint uiForm = 0) const;															// Exposed to Python 
	const wchar* getCivilizationShortDescriptionKey() const;																					// Exposed to Python 
	const wchar* getCivilizationAdjective(uint uiForm = 0) const;																			// Exposed to Python
	const wchar* getCivilizationAdjectiveKey() const;																									// Exposed to Python
	DllExport CvWString getFlagDecal() const;																																		// Exposed to Python
	DllExport bool isWhiteFlag() const;																																					// Exposed to Python
	DllExport void setWhiteFlag( bool bNewValue );
	const wchar* getStateReligionName(uint uiForm = 0) const;																					// Exposed to Python
	const wchar* getStateReligionKey() const;																													// Exposed to Python
	const CvWString getBestAttackUnitName(uint uiForm = 0) const;																								// Exposed to Python
	const CvWString getWorstEnemyName() const;																																	// Exposed to Python
	const wchar* getBestAttackUnitKey() const;																																	// Exposed to Python
	DllExport ArtStyleTypes getArtStyleType() const;																														// Exposed to Python
	const TCHAR* getUnitButton(UnitTypes eUnit) const;																														// Exposed to Python

	void doTurn();
	void doTurnUnits();

	void verifyCivics();

	void updatePlotGroups();

	void updateYield();
	void updateMaintenance();
	void updatePowerHealth();
	// MOD - START - Advanced Building Prerequisites (Power)
	void updatePowerBuildings();
	// MOD - END - Advanced Building Prerequisites (Power)
	void updateExtraBuildingHappiness();
	void updateExtraBuildingHealth();
	void updateFeatureHappiness();
	void updateReligionHappiness();
	// MOD - START - Advanced Building Prerequisites (Religion)
	void updateReligionAffectedBuildings();
	// MOD - END - Advanced Building Prerequisites (Religion)
	// MOD - START - Religion Affected Bonuses
	void updateReligionAffectedBonuses();
	// MOD - END - Religion Affected Bonuses
	// MOD - START - Advanced Yield and Commerce Effects
	//void updateExtraSpecialistYield();
	void updateSpecialistYield();
	void applyBuildingYieldChange(BuildingClassTypes eBuildingClass, YieldTypes eYield, int iChange);
	// MOD - END - Advanced Yield and Commerce Effects
	void updateCommerce(CommerceTypes eCommerce);
	void updateCommerce();
	void updateBuildingCommerce();
	void updateReligionCommerce();
	void updateCorporation();
	void updateCityPlotYield();
	void updateCitySight(bool bIncrement, bool bUpdatePlotGroups);
	void updateTradeRoutes();
	void updatePlunder(int iChange, bool bUpdatePlotGroups);

	// MOD - START - Open Borders Commerce
	void updateOpenBordersCommerce();
	// MOD - END - Open Borders Commerce

	// MOD - START - Tech Transfer Commerce
	//void updateTechTransferCommerce();
	// MOD - END - Tech Transfer Commerce

	// MOD - START - Detriments
	void applyDetriments(TeamTypes eAffectedTeam);
	void removeDetriments(TeamTypes eAffectedTeam);
	// MOD - END - Detriments

	void updateTimers();

	bool hasReadyUnit(bool bAny = false) const;
	bool hasAutoUnit() const;
	DllExport bool hasBusyUnit() const;

	// K-Mod
	bool isChoosingFreeTech() const { return m_iChoosingFreeTechCount > 0; }
	void changeChoosingFreeTechCount(int iChange) { m_iChoosingFreeTechCount += iChange; }
	// K-Mod end

	void chooseTech(int iDiscover = 0, CvWString szText = "", bool bFront = false);				// Exposed to Python

	int calculateScore(bool bFinal = false, bool bVictory = false) const;

	int findBestFoundValue() const;																																				// Exposed to Python

	int upgradeAllPrice(UnitTypes eUpgradeUnit, UnitTypes eFromUnit);

	// note: bbai added bIncludeTraining to the following two functions.
	int countReligionSpreadUnits(CvArea* pArea, ReligionTypes eReligion, bool bIncludeTraining = false) const;														// Exposed to Python
	// MOD - START - Inquisition
	int countRemoveNonStateReligionUnits(CvArea* pArea, ReligionTypes eReligion, bool bIncludeTraining = false) const;
	// MOD - END - Inquisition
	int countCorporationSpreadUnits(CvArea* pArea, CorporationTypes eCorporation, bool bIncludeTraining = false) const;														// Exposed to Python

	int countNumCoastalCities() const;																																		// Exposed to Python
	int countNumCoastalCitiesByArea(CvArea* pArea) const;																									// Exposed to Python
	int countTotalCulture() const;																																				// Exposed to Python
	int countOwnedBonuses(BonusTypes eBonus) const;																												// Exposed to Python
	// MOD - START - Sea Improvements
	//int countUnimprovedBonuses(CvArea* pArea, CvPlot* pFromPlot = NULL) const;														// Exposed to Python
	int countUnimprovedBonuses(CvArea* pArea, CvPlot* pFromPlot = NULL, bool bIncludeCityRadius = true) const;														// Exposed to Python
	// MOD - START - Sea Improvements
	int countCityFeatures(FeatureTypes eFeature) const;																										// Exposed to Python
	int countNumBuildings(BuildingTypes eBuilding) const;																									// Exposed to Python
	int countNumCitiesConnectedToCapital() const;																								// Exposed to Python
	/* int countPotentialForeignTradeCities(CvArea* pIgnoreArea = NULL) const;
	int countPotentialForeignTradeCitiesConnected() const; */ // K-Mod: These functions were used exclusively for AI.  I've moved them to CvPlayerAI.
	bool doesImprovementConnectBonus(ImprovementTypes eImprovement, BonusTypes eBonus, bool bCheckTechnology = true) const; // K-Mod

	DllExport bool canContact(PlayerTypes ePlayer) const;																									// Exposed to Python
	bool canContactAndTalk(PlayerTypes ePlayer) const; // K-Mod. this checks willingness to talk on both sides
	void contact(PlayerTypes ePlayer);																															// Exposed to Python
	DllExport void handleDiploEvent(DiploEventTypes eDiploEvent, PlayerTypes ePlayer, int iData1, int iData2);
	bool canTradeWith(PlayerTypes eWhoTo) const;																													// Exposed to Python
	bool canReceiveTradeCity() const;
	DllExport bool canTradeItem(PlayerTypes eWhoTo, TradeData item, bool bTestDenial = false) const;			// Exposed to Python
	DllExport DenialTypes getTradeDenial(PlayerTypes eWhoTo, TradeData item) const;												// Exposed to Python
	bool canTradeNetworkWith(PlayerTypes ePlayer) const;																									// Exposed to Python
	int getNumAvailableBonuses(BonusTypes eBonus) const;																									// Exposed to Python
	int getNumTradeableBonuses(BonusTypes eBonus) const;																				// Exposed to Python
	int getNumTradeBonusImports(PlayerTypes ePlayer) const;																								// Exposed to Python
	bool hasBonus(BonusTypes eBonus) const;									// Exposed to Python
	// MOD - START - Bonus Knowledge
	bool hasBonusKnowledge(BonusTypes eBonus) const;
	// MOD - END - Bonus Knowledge

	// MOD - START - Religion Affected Bonuses
	bool isReligionNoBonusHealthiness(BonusTypes eBonus) const;   // Exposed to Python
	void setReligionNoBonusHealthiness(BonusTypes eBonus, bool bNewValue);   // Exposed to Python

	bool isReligionNoBonusHappiness(BonusTypes eBonus) const;   // Exposed to Python
	void setReligionNoBonusHappiness(BonusTypes eBonus, bool bNewValue);   // Exposed to Python
	// MOD - END - Religion Affected Bonuses

	bool isTradingWithTeam(TeamTypes eTeam, bool bIncludeCancelable) const;
	bool canStopTradingWithTeam(TeamTypes eTeam, bool bContinueNotTrading = false) const;																										// Exposed to Python
	void stopTradingWithTeam(TeamTypes eTeam);																											// Exposed to Python
	void killAllDeals();																																						// Exposed to Python

	void findNewCapital();																																					// Exposed to Python 
	int getNumGovernmentCenters() const;																												// Exposed to Python 

	bool canRaze(CvCity* pCity) const;																													// Exposed to Python 
	void raze(CvCity* pCity);																																				// Exposed to Python  
	void disband(CvCity* pCity);																																		// Exposed to Python

	bool canReceiveGoody(CvPlot* pPlot, GoodyTypes eGoody, CvUnit* pUnit) const;													// Exposed to Python
	void receiveGoody(CvPlot* pPlot, GoodyTypes eGoody, CvUnit* pUnit);															// Exposed to Python
	void doGoody(CvPlot* pPlot, CvUnit* pUnit);																											// Exposed to Python

	DllExport bool canFound(int iX, int iY, bool bTestVisible = false) const;															// Exposed to Python			
	void found(int iX, int iY);																																			// Exposed to Python			

	bool canTrain(UnitTypes eUnit, bool bContinue = false, bool bTestVisible = false, bool bIgnoreCost = false) const;										// Exposed to Python
	// MOD - START - Advanced Building Prerequisites (Civic)
	//bool canConstruct(BuildingTypes eBuilding, bool bContinue = false, bool bTestVisible = false, bool bIgnoreCost = false, bool bIgnoreTech = false) const; // Exposed to Python, K-Mod added bIgnoreTech
	bool canConstruct(BuildingTypes eBuilding, bool bContinue = false, bool bTestVisible = false, bool bIgnoreCost = false, bool bIgnoreTech = false, bool bIgnoreCivic = false) const; // Exposed to Python, K-Mod added bIgnoreTech
	// MOD - END - Advanced Building Prerequisites (Civic)
	bool canCreate(ProjectTypes eProject, bool bContinue = false, bool bTestVisible = false) const;							// Exposed to Python
	bool canMaintain(ProcessTypes eProcess, bool bContinue = false) const;																			// Exposed to Python
	bool isProductionMaxedUnitClass(UnitClassTypes eUnitClass) const;																						// Exposed to Python
	bool isProductionMaxedBuildingClass(BuildingClassTypes eBuildingClass, bool bAcquireCity = false) const;		// Exposed to Python
	bool isProductionMaxedProject(ProjectTypes eProject) const;																									// Exposed to Python

	int getProductionNeeded(UnitTypes eUnit) const;																										// Exposed to Python
	int getProductionNeeded(BuildingTypes eBuilding) const;																						// Exposed to Python
	int getProductionNeeded(ProjectTypes eProject) const;																							// Exposed to Python
	int getProductionModifier(UnitTypes eUnit) const;
	// MOD - START - Unit Roles
	long getProductionCostScalingModifier(UnitTypes eUnit) const;
	// MOD - END - Unit Roles
	int getProductionModifier(BuildingTypes eBuilding) const;
	int getProductionModifier(ProjectTypes eProject) const;

	int getBuildingClassPrereqBuilding(BuildingTypes eBuilding, BuildingClassTypes ePrereqBuildingClass, int iExtra = 0) const;	// Exposed to Python
	void removeBuildingClass(BuildingClassTypes eBuildingClass);																		// Exposed to Python
	void processBuilding(BuildingTypes eBuilding, int iChange, CvArea* pArea);

	int getBuildCost(const CvPlot* pPlot, BuildTypes eBuild) const;
	// MOD - START - Player Build Caching
	void updateBuildCache();
	void updateBuildCache(BuildTypes eBuild);
	bool canBuild(BuildTypes eBuild) const;
	// MOD - END - Player Build Caching
	bool canBuild(const CvPlot* pPlot, BuildTypes eBuild, bool bTestEra = false, bool bTestVisible = false) const;	// Exposed to Python
	// MOD - START - Unique Improvements
	bool canProduceImprovement(ImprovementTypes eImprovement) const; // Exposed to Python
	// MOD - END - Unique Improvements
	RouteTypes getBestRoute(const CvPlot* pPlot = NULL) const;																						// Exposed to Python
	int getImprovementUpgradeRate() const;																													// Exposed to Python

	int calculateTotalYield(YieldTypes eYield) const;																											// Exposed to Python
	int calculateTotalExports(YieldTypes eYield) const;																										// Exposed to Python
	int calculateTotalImports(YieldTypes eYield) const;																										// Exposed to Python

	int calculateTotalCityHappiness() const;																															// Exposed to Python
	int calculateTotalCityUnhappiness() const;																														// Exposed to Python

	int calculateTotalCityHealthiness() const;																														// Exposed to Python
	int calculateTotalCityUnhealthiness() const;																													// Exposed to Python

	int calculatePollution(int iTypes = POLLUTION_ALL) const; // K-Mod, Exposed to Python
	int getGwPercentAnger() const; // K-Mod, Exposed to Python
	void setGwPercentAnger(int iNewValue); // K-Mod

	int getUnitCostMultiplier() const; // K-Mod
	//int calculateUnitCost(int& iFreeUnits, int& iFreeMilitaryUnits, int& iPaidUnits, int& iPaidMilitaryUnits, int& iBaseUnitCost, int& iMilitaryCost, int& iExtraCost) const;
	int calculateUnitCost(int& iFreeUnits, int& iFreeMilitaryUnits, int& iPaidUnits, int& iPaidMilitaryUnits, int& iUnitCost, int& iMilitaryCost, int& iExtraCost) const; // (K-Mod changed iBaseUnitCost to iUnitCost)
	int calculateUnitCost() const;																																				// Exposed to Python
	int calculateUnitSupply(int& iPaidUnits, int& iBaseSupplyCost) const;																	// Exposed to Python
	int calculateUnitSupply() const;																																			// Exposed to Python
	int calculatePreInflatedCosts() const;																																// Exposed to Python
	//int calculateInflationRate() const; // (was exposed to python. Use getInflationRate instead.)
	void updateInflationRate(); // K-Mod
	int getInflationRate() const { return m_iInflationRate; } // K-Mod, exposed to Python.
	int calculateInflatedCosts() const;																																		// Exposed to Python

	//int calculateBaseNetGold() const; // disabled by K-Mod
	//int calculateBaseNetResearch(TechTypes eTech = NO_TECH) const;   // disabled by K-Mod (was exposed to Python)
	int calculateResearchModifier(TechTypes eTech) const;   // Exposed to Python
	int calculateGoldRate() const;																																				// Exposed to Python
	int calculateResearchRate(TechTypes eTech = NO_TECH) const;																						// Exposed to Python
	int calculateTotalCommerce() const;

	bool isResearch() const;																																							// Exposed to Python
	bool canEverResearch(TechTypes eTech) const;																								// Exposed to Python
	// K-Mod, added "bFree" argument
	//DllExport bool canResearch(TechTypes eTech, bool bTrade = false) const;																// Exposed to Python
	bool canResearch(TechTypes eTech, bool bTrade = false, bool bFree = false) const;																// Exposed to Python
	TechTypes getCurrentResearch() const;																												// Exposed to Python
	bool isCurrentResearchRepeat() const;																																	// Exposed to Python
	// MOD - START - Tech Transfer
	int getCurrentResearchTechTransferCount();																																	// Exposed to Python
	bool isCurrentResearchTechTransfer();																																	// Exposed to Python
	// MOD - END - Tech Transfer
	bool isNoResearchAvailable() const;																																		// Exposed to Python
	int getResearchTurnsLeft(TechTypes eTech, bool bOverflow) const;														// Exposed to Python
	bool canSeeResearch(PlayerTypes ePlayer) const; // K-Mod, Exposed to Python
	bool canSeeDemographics(PlayerTypes ePlayer) const; // K-Mod, Exposed to Python

	bool isCivic(CivicTypes eCivic) const;																																// Exposed to Python
	bool canDoCivics(CivicTypes eCivic) const;																														// Exposed to Python
	bool canRevolution(CivicTypes* paeNewCivics) const;																					// Exposed to Python
	void revolution(CivicTypes* paeNewCivics, bool bForce = false);												// Exposed to Python
	int getCivicPercentAnger(CivicTypes eCivic, bool bIgnore = false) const;																										// Exposed to Python

	bool canDoReligion(ReligionTypes eReligion) const;																										// Exposed to Python
	bool canChangeReligion() const;																																				// Exposed to Python
	bool canConvert(ReligionTypes eReligion) const;																							// Exposed to Python
	/*	f1rpo: bForce added, DllExport removed
		(also from the other civc/ religion functions; the EXE doesn't call them). */
	void convert(ReligionTypes eReligion, bool bForce = false);																								// Exposed to Python
	bool hasHolyCity(ReligionTypes eReligion) const;																											// Exposed to Python
	// MOD - START - Holy City Migration
	bool hasBestHolyCityCandidate(ReligionTypes eReligion) const; // Exposed to Python
	// MOD - END - Holy City Migration
	int countHolyCities() const;																																					// Exposed to Python
	void foundReligion(ReligionTypes eReligion, ReligionTypes eSlotReligion, bool bAward);	

	bool hasHeadquarters(CorporationTypes eCorporation) const;																											// Exposed to Python
	int countHeadquarters() const;																																					// Exposed to Python
	//int countCorporations(CorporationTypes eCorporation) const;	// Exposed to Python
	int countCorporations(CorporationTypes eCorporation, CvArea* pArea = 0) const; // K-Mod, exposed to Python
	void foundCorporation(CorporationTypes eCorporation);																										// Exposed to Python

	DllExport int getCivicAnarchyLength(CivicTypes* paeNewCivics) const;																	// Exposed to Python
	DllExport int getReligionAnarchyLength() const;																												// Exposed to Python

	int unitsRequiredForGoldenAge() const;																											// Exposed to Python
	int unitsGoldenAgeCapable() const;																																		// Exposed to Python
	int unitsGoldenAgeReady() const;																														// Exposed to Python
	void killGoldenAgeUnits(CvUnit* pUnitAlive);

	int greatPeopleThreshold(bool bMilitary = false) const;																														// Exposed to Python
	int specialistYield(SpecialistTypes eSpecialist, YieldTypes eYield) const;														// Exposed to Python
	int specialistCommerce(SpecialistTypes eSpecialist, CommerceTypes eCommerce) const;										// Exposed to Python

	CvPlot* getStartingPlot() const;																																			// Exposed to Python
	void setStartingPlot(CvPlot* pNewValue, bool bUpdateStartDist);												// Exposed to Python

	int getTotalPopulation() const;																															// Exposed to Python
	int getAveragePopulation() const;																																			// Exposed to Python
	void changeTotalPopulation(int iChange);
	long getRealPopulation() const;																																				// Exposed to Python
	int getReligionPopulation(ReligionTypes eReligion) const;

	int getTotalLand() const;																																							// Exposed to Python
	void changeTotalLand(int iChange);

	int getTotalLandScored() const;																																				// Exposed to Python
	void changeTotalLandScored(int iChange);

	int getGold() const;																																				// Exposed to Python
	DllExport void setGold(int iNewValue);																													// Exposed to Python
	DllExport void changeGold(int iChange);																													// Exposed to Python

	int getGoldPerTurn() const;																																						// Exposed to Python

	int getAdvancedStartPoints() const;																																				// Exposed to Python
	void setAdvancedStartPoints(int iNewValue);																													// Exposed to Python
	void changeAdvancedStartPoints(int iChange);																													// Exposed to Python

	int getEspionageSpending(TeamTypes eAgainstTeam) const;																								// Exposed to Python
	bool canDoEspionageMission(EspionageMissionTypes eMission, PlayerTypes eTargetPlayer, const CvPlot* pPlot, int iExtraData, const CvUnit* pUnit) const;		// Exposed to Python
	int getEspionageMissionBaseCost(EspionageMissionTypes eMission, PlayerTypes eTargetPlayer, const CvPlot* pPlot, int iExtraData, const CvUnit* pSpyUnit) const;
	int getEspionageMissionCost(EspionageMissionTypes eMission, PlayerTypes eTargetPlayer, const CvPlot* pPlot = NULL, int iExtraData = -1, const CvUnit* pSpyUnit = NULL) const;		// Exposed to Python
	int getEspionageMissionCostModifier(EspionageMissionTypes eMission, PlayerTypes eTargetPlayer, const CvPlot* pPlot = NULL, int iExtraData = -1, const CvUnit* pSpyUnit = NULL) const;
	bool doEspionageMission(EspionageMissionTypes eMission, PlayerTypes eTargetPlayer, CvPlot* pPlot, int iExtraData, CvUnit* pUnit);

	int getEspionageSpendingWeightAgainstTeam(TeamTypes eIndex) const;																							// Exposed to Python
	void setEspionageSpendingWeightAgainstTeam(TeamTypes eIndex, int iValue);																		// Exposed to Python
	void changeEspionageSpendingWeightAgainstTeam(TeamTypes eIndex, int iChange);																// Exposed to Python

	bool canStealTech(PlayerTypes eTarget, TechTypes eTech) const;
	bool canForceCivics(PlayerTypes eTarget, CivicTypes eCivic) const;
	bool canForceReligion(PlayerTypes eTarget, ReligionTypes eReligion) const;
	bool canSpyDestroyUnit(PlayerTypes eTarget, CvUnit& kUnit) const;
	bool canSpyBribeUnit(PlayerTypes eTarget, CvUnit& kUnit) const;
	bool canSpyDestroyBuilding(PlayerTypes eTarget, BuildingTypes eBuilding) const;
	bool canSpyDestroyProject(PlayerTypes eTarget, ProjectTypes eProject) const;
	int getEspionageGoldQuantity(EspionageMissionTypes eMission, PlayerTypes eTargetPlayer, const CvCity* pCity) const; // K-Mod

	void doAdvancedStartAction(AdvancedStartActionTypes eAction, int iX, int iY, int iData, bool bAdd);
	int getAdvancedStartUnitCost(UnitTypes eUnit, bool bAdd, CvPlot* pPlot = NULL) const;																													// Exposed to Python 
	int getAdvancedStartCityCost(bool bAdd, CvPlot* pPlot = NULL) const;																													// Exposed to Python 
	int getAdvancedStartPopCost(bool bAdd, CvCity* pCity = NULL) const;																													// Exposed to Python 
	int getAdvancedStartCultureCost(bool bAdd, CvCity* pCity = NULL) const;																													// Exposed to Python 
	int getAdvancedStartBuildingCost(BuildingTypes eBuilding, bool bAdd, CvCity* pCity = NULL) const;																													// Exposed to Python 
	int getAdvancedStartImprovementCost(ImprovementTypes eImprovement, bool bAdd, CvPlot* pPlot = NULL) const;																													// Exposed to Python 
	int getAdvancedStartRouteCost(RouteTypes eRoute, bool bAdd, CvPlot* pPlot = NULL) const;																													// Exposed to Python 
	int getAdvancedStartTechCost(TechTypes eTech, bool bAdd) const;																													// Exposed to Python 
	int getAdvancedStartVisibilityCost(bool bAdd, CvPlot* pPlot = NULL) const;																													// Exposed to Python 

	int getGoldenAgeTurns() const;																															// Exposed to Python  
	bool isGoldenAge() const;																																		// Exposed to Python 
	void changeGoldenAgeTurns(int iChange);																													// Exposed to Python 
	int getGoldenAgeLength() const;

	int getNumUnitGoldenAges() const;																																			// Exposed to Python 
	void changeNumUnitGoldenAges(int iChange);																											// Exposed to Python 

	int getAnarchyTurns() const;																																					// Exposed to Python
	bool isAnarchy() const;																																			// Exposed to Python
	void changeAnarchyTurns(int iChange);																														// Exposed to Python

	int getStrikeTurns() const;																																						// Exposed to Python
	void changeStrikeTurns(int iChange);

	int getMaxAnarchyTurns() const;																																				// Exposed to Python 
	void updateMaxAnarchyTurns();

	int getAnarchyModifier() const;																																				// Exposed to Python 
	void changeAnarchyModifier(int iChange);

	int getGoldenAgeModifier() const;																																				// Exposed to Python 
	void changeGoldenAgeModifier(int iChange);

	int getHurryModifier() const;																																					// Exposed to Python
	void changeHurryModifier(int iChange);

	void createGreatPeople(UnitTypes eGreatPersonUnit, bool bIncrementThreshold, bool bIncrementExperience, int iX, int iY);

	int getGreatPeopleCreated() const;																																		// Exposed to Python
	void incrementGreatPeopleCreated();

	int getGreatGeneralsCreated() const;																																		// Exposed to Python
	void incrementGreatGeneralsCreated();

	int getGreatPeopleThresholdModifier() const;																													// Exposed to Python
	void changeGreatPeopleThresholdModifier(int iChange);										

	int getGreatGeneralsThresholdModifier() const;																													// Exposed to Python
	void changeGreatGeneralsThresholdModifier(int iChange);										

	int getGreatPeopleRateModifier() const;																																// Exposed to Python
	void changeGreatPeopleRateModifier(int iChange);

	int getGreatGeneralRateModifier() const;																																// Exposed to Python
	void changeGreatGeneralRateModifier(int iChange);

	int getDomesticGreatGeneralRateModifier() const;																																// Exposed to Python
	void changeDomesticGreatGeneralRateModifier(int iChange);

	int getStateReligionGreatPeopleRateModifier() const;																									// Exposed to Python
	void changeStateReligionGreatPeopleRateModifier(int iChange);

	int getMaxGlobalBuildingProductionModifier() const;																										// Exposed to Python
	void changeMaxGlobalBuildingProductionModifier(int iChange);

	int getMaxTeamBuildingProductionModifier() const;																											// Exposed to Python 
	void changeMaxTeamBuildingProductionModifier(int iChange);

	int getMaxPlayerBuildingProductionModifier() const;																										// Exposed to Python
	void changeMaxPlayerBuildingProductionModifier(int iChange);

	// MOD - START - Epidemics
	// Epidemic system by Mexico
	int getAcceptableEpidemicPercent() const; // Exposed to Python
	void setAcceptableEpidemicPercent(int iNewValue); // Exposed to Python
	void changeAcceptableEpidemicPercent(int iNewValue); // Exposed to Python

	int getGlobalEpidemicModifier() const;
	void setGlobalEpidemicModifier(int iNewValue);
	void changeGlobalEpidemicModifier(int iChange);
	// MOD - END - Epidemics

	int getFreeExperience() const;																																				// Exposed to Python
	void changeFreeExperience(int iChange);

	// MOD - START - Player UnitCombat Free Experience
	int getUnitCombatFreeExperience(UnitCombatTypes eIndex) const;																					// Exposed to Python
	void changeUnitCombatFreeExperience(UnitCombatTypes eIndex, int iChange);
	// MOD - END - Player UnitCombat Free Experience

	// MOD - START - Trait Trade Modifier
	int getTradeRouteIncomeModifier() const;
	void changeTradeRouteIncomeModifier(int iChange);
	int getForeignTradeRouteIncomeModifier() const;
	void changeForeignTradeRouteIncomeModifier(int iChange);
	// MOD - END - Trait Trade Modifier

	// MOD - START - Trait Upgrade Modifier
	int getUpgradeDiscount() const;
	void changeUpgradeDiscount(int iChange);
	// MOD - END - Trait Upgrade Modifier

	// MOD - START - Congestion
	int getInsideCityCongestionThreshold() const;
	int getOutsideCityCongestionThreshold() const;
	void changeInsideCityCongestionThreshold(int iChange);
	void changeOutsideCityCongestionThreshold(int iChange);
	// MOD - END - Congestion

	// MOD - START - Better Nukes
	int getNukesUsed() const;
	void changeNukesUsed(int iChange);
	// MOD - END - Better Nukes

	// MOD - START - RI Revolution	
	int getRevModifier() const;
	void changeRevModifier(int iChange);
	void calculateRevModifier();
	int getRevModifierScore() const;
	void changeRevModifierScore(int iChange);
	void calculateRevModifierScore();
	int getRevModifierPower() const;
	void changeRevModifierPower(int iChange);
	void calculateRevModifierPower();
	int getRevModifierEndOfHistory() const;
	void changeRevModifierEndOfHistory(int iChange);
	void calculateRevModifierEndOfHistory();
	int getRevModifierWarExhaustion() const;
	void changeRevModifierWarExhaustion(int iChange);
	void calculateRevModifierWarExhaustion();
	// MOD - END - RI Revolution

	int getSlaveAfrican() const;
	void incrementSlaveAfrican();
	int getSlaveAsian() const;
	void incrementSlaveAsian();
	int getSlaveEuropean() const;
	void incrementSlaveEuropean();
	int getSlaveIndian() const;
	void incrementSlaveIndian();
	int getSlaveMid() const;
	void incrementSlaveMid();
	int getSlaveNewworld() const;
	void incrementSlaveNewworld();
	int getSlaveSlavic() const;
	void incrementSlaveSlavic();

	int getFeatureProductionModifier() const;																															// Exposed to Python
	void changeFeatureProductionModifier(int iChange);

	int getWorkerSpeedModifier() const;																																		// Exposed to Python
	void changeWorkerSpeedModifier(int iChange);

	int getImprovementUpgradeRateModifier() const;																									// Exposed to Python
	void changeImprovementUpgradeRateModifier(int iChange);

	int getMilitaryProductionModifier() const;																											// Exposed to Python
	void changeMilitaryProductionModifier(int iChange);

	int getSpaceProductionModifier() const;																																// Exposed to Python  
	void changeSpaceProductionModifier(int iChange);

	int getCityDefenseModifier() const;																																		// Exposed to Python
	void changeCityDefenseModifier(int iChange);

	int getNumNukeUnits() const;																																					// Exposed to Python
	void changeNumNukeUnits(int iChange);

	int getNumOutsideUnits() const;																																				// Exposed to Python
	void changeNumOutsideUnits(int iChange);

	int getBaseFreeUnits() const;																																					// Exposed to Python
	void changeBaseFreeUnits(int iChange);

	int getBaseFreeMilitaryUnits() const;																																	// Exposed to Python
	void changeBaseFreeMilitaryUnits(int iChange);

	int getFreeUnitsPopulationPercent() const;																														// Exposed to Python
	void changeFreeUnitsPopulationPercent(int iChange);

	int getFreeMilitaryUnitsPopulationPercent() const;																										// Exposed to Python
	void changeFreeMilitaryUnitsPopulationPercent(int iChange);											

	int getTypicalUnitValue(UnitAITypes eUnitAI, DomainTypes eDomain = NO_DOMAIN) const; // K-Mod

	// K-Mod note: GoldPerUnit and GoldPerMilitaryUnit are now in units of 1/100 gold.
	int getGoldPerUnit() const;																																								// Exposed to Python
	void changeGoldPerUnit(int iChange);															

	int getGoldPerMilitaryUnit() const;																																				// Exposed to Python
	void changeGoldPerMilitaryUnit(int iChange);

	int getExtraUnitCost() const;																																							// Exposed to Python 
	void changeExtraUnitCost(int iChange);

	int getNumMilitaryUnits() const;																																					// Exposed to Python
	void changeNumMilitaryUnits(int iChange);													

	int getHappyPerMilitaryUnit() const;																																			// Exposed to Python
	void changeHappyPerMilitaryUnit(int iChange);												

	int getMilitaryFoodProductionCount() const;														
	bool isMilitaryFoodProduction() const;																																		// Exposed to Python
	void changeMilitaryFoodProductionCount(int iChange);

	int getHighestUnitLevel() const;																																					// Exposed to Python
	void setHighestUnitLevel(int iNewValue);

	int getConscriptCount() const;																																						// Exposed to Python
	void setConscriptCount(int iNewValue);																															// Exposed to Python
	void changeConscriptCount(int iChange);																															// Exposed to Python

	int getMaxConscript() const;																																		// Exposed to Python
	void changeMaxConscript(int iChange);														

	int getOverflowResearch() const;																																// Exposed to Python
	void setOverflowResearch(int iNewValue);																														// Exposed to Python
	void changeOverflowResearch(int iChange);																														// Exposed to Python

/*
** K-Mod, 27/dec/10, karadoc
** replaced NoUnhealthyPopulation with UnhealthyPopulationModifier
*/
	/* original bts code
	int getNoUnhealthyPopulationCount() const;
	bool isNoUnhealthyPopulation() const;																																			// Exposed to Python
	void changeNoUnhealthyPopulationCount(int iChange); */
	int getUnhealthyPopulationModifier() const; // Exposed to Python
	void changeUnhealthyPopulationModifier(int iChange);
/*
** K-Mod end
*/

	int getExpInBorderModifier() const;
	void changeExpInBorderModifier(int iChange);

	int getBuildingOnlyHealthyCount() const;
	bool isBuildingOnlyHealthy() const;																																				// Exposed to Python
	void changeBuildingOnlyHealthyCount(int iChange);

	int getDistanceMaintenanceModifier() const;																																// Exposed to Python
	void changeDistanceMaintenanceModifier(int iChange);

	int getNumCitiesMaintenanceModifier() const;																															// Exposed to Python
	void changeNumCitiesMaintenanceModifier(int iChange);

	int getCorporationMaintenanceModifier() const;																															// Exposed to Python
	void changeCorporationMaintenanceModifier(int iChange);

	int getTotalMaintenance() const;																																					// Exposed to Python
	void changeTotalMaintenance(int iChange);

	int getUpkeepModifier() const;																																						// Exposed to Python
	void changeUpkeepModifier(int iChange);

	int getLevelExperienceModifier() const;																																						// Exposed to Python
	void changeLevelExperienceModifier(int iChange);

	int getExtraHealth() const;																																			// Exposed to Python
	void changeExtraHealth(int iChange);

	int getBuildingGoodHealth() const;																																				// Exposed to Python
	void changeBuildingGoodHealth(int iChange);

	int getBuildingBadHealth() const;																																					// Exposed to Python
	void changeBuildingBadHealth(int iChange);

	int getExtraHappiness() const;																																						// Exposed to Python
	void changeExtraHappiness(int iChange);

	int getBuildingHappiness() const;																																					// Exposed to Python
	void changeBuildingHappiness(int iChange);

	int getLargestCityHappiness() const;																																			// Exposed to Python
	void changeLargestCityHappiness(int iChange);

	int getWarWearinessPercentAnger() const;																																	// Exposed to Python 
	void updateWarWearinessPercentAnger();
	int getModifiedWarWearinessPercentAnger(int iWarWearinessPercentAnger) const;

	int getWarWearinessModifier() const;																																			// Exposed to Python
	void changeWarWearinessModifier(int iChange);

	// MOD - START - Unlimited Conscription
	int getUnlimitedConscriptCount() const;
	bool isUnlimitedConscript() const; // Exposed to Python
	void changeUnlimitedConscriptCount(int iChange);
	// MOD - END - Unlimited Conscription

	// MOD - START - Wartime Conscription
	int getWarConscript() const; // Exposed to Python
	void changeWarConscript(int iChange);
	// MOD - END - Wartime Conscription

	int getFreeSpecialist() const;																																						// Exposed to Python
	void changeFreeSpecialist(int iChange);

	int getNoForeignTradeCount() const;
	bool isNoForeignTrade() const;																																						// Exposed to Python
	void changeNoForeignTradeCount(int iChange);

	int getNoCorporationsCount() const;
	bool isNoCorporations() const;																																						// Exposed to Python
	void changeNoCorporationsCount(int iChange);

	int getNoForeignCorporationsCount() const;
	bool isNoForeignCorporations() const;																																						// Exposed to Python
	void changeNoForeignCorporationsCount(int iChange);

	int getCoastalTradeRoutes() const;																																				// Exposed to Python
	void changeCoastalTradeRoutes(int iChange);																													// Exposed to Python

	int getTradeRoutes() const;																																								// Exposed to Python
	void changeTradeRoutes(int iChange);																																// Exposed to Python

	int getRevolutionTimer() const;																																	// Exposed to Python
	void setRevolutionTimer(int iNewValue);
	void changeRevolutionTimer(int iChange);

	int getConversionTimer() const;																																						// Exposed to Python
	void setConversionTimer(int iNewValue);
	void changeConversionTimer(int iChange);

	int getStateReligionCount() const;
	bool isStateReligion() const;																																							// Exposed to Python
	void changeStateReligionCount(int iChange);

	int getNoNonStateReligionSpreadCount() const;
	bool isNoNonStateReligionSpread() const;																												// Exposed to Python
	void changeNoNonStateReligionSpreadCount(int iChange);

	// MOD - START - Holy City Migration
	int getCanAcquireHolyCityWithoutStateReligionCount() const;
	bool canAcquireHolyCityWithoutStateReligion() const; // Exposed to Python
	void changeCanAcquireHolyCityWithoutStateReligionCount(int iChange);

	int getCanMaintainHolyCityWithoutStateReligionCount() const;
	bool canMaintainHolyCityWithoutStateReligion() const; // Exposed to Python
	void changeCanMaintainHolyCityWithoutStateReligionCount(int iChange);
	// MOD - END - Holy City Migration

	int getStateReligionHappiness() const;																													// Exposed to Python
	void changeStateReligionHappiness(int iChange);

	int getNonStateReligionHappiness() const;																																	// Exposed to Python
	void changeNonStateReligionHappiness(int iChange);

	int getStateReligionUnitProductionModifier() const;																												// Exposed to Python 
	void changeStateReligionUnitProductionModifier(int iChange);

	int getStateReligionBuildingProductionModifier() const;																										// Exposed to Python
	void changeStateReligionBuildingProductionModifier(int iChange);																		// Exposed to Python

	int getStateReligionFreeExperience() const;																																// Exposed to Python
	void changeStateReligionFreeExperience(int iChange);

	DllExport CvCity* getCapitalCity() const;																																	// Exposed to Python
	void setCapitalCity(CvCity* pNewCapitalCity);

	int getCitiesLost() const;																																								// Exposed to Python
	void changeCitiesLost(int iChange);

	int getWinsVsBarbs() const;																																								// Exposed to Python
	void changeWinsVsBarbs(int iChange);

	int getAssets() const;																																					// Exposed to Python
	void changeAssets(int iChange);																																			// Exposed to Python  

	int getPower() const;																																						// Exposed to Python
	void changePower(int iChange);

	int getPopScore(bool bCheckVassal = true) const;																																				// Exposed to Python
	void changePopScore(int iChange);																																		// Exposed to Python  
	int getLandScore(bool bCheckVassal = true) const;																																				// Exposed to Python
	void changeLandScore(int iChange);																																	// Exposed to Python  
	int getTechScore() const;																																				// Exposed to Python
	void changeTechScore(int iChange);																																	// Exposed to Python  
	int getWondersScore() const;																																		// Exposed to Python
	void changeWondersScore(int iChange);	// Exposed to Python  
	// MOD - START - Handicap Score Modifier
	int getHandicapScoreModifier() const;																																		// Exposed to Python
	void setHandicapScoreModifier(int iModifier);	// Exposed to Python
	// MOD - END - Handicap Score Modifier

	int getCombatExperience() const; 	// Exposed to Python  
	void setCombatExperience(int iExperience);   // Exposed to Python
	void changeCombatExperience(int iChange);   // Exposed to Python

	bool isConnected() const;
	DllExport int getNetID() const;
	DllExport void setNetID(int iNetID);
	void sendReminder();

	uint getStartTime() const;
	DllExport void setStartTime(uint uiStartTime);
	uint getTotalTimePlayed() const;																																// Exposed to Python			  
																																																			
	bool isMinorCiv() const;	
	bool isCommunist() const;
																																								// Exposed to Python			
																																																			
	DllExport bool isAlive() const;																																						// Exposed to Python			
	bool isEverAlive() const;																																				// Exposed to Python			
	void setAlive(bool bNewValue);
	void verifyAlive();

	DllExport bool isTurnActive() const;																			
	DllExport void setTurnActive(bool bNewValue, bool bDoTurn = true);
	void onTurnLogging() const; // K-Mod

	bool isAutoMoves() const;
	void setAutoMoves(bool bNewValue);
	DllExport void setTurnActiveForPbem(bool bActive);

	DllExport bool isPbemNewTurn() const;
	DllExport void setPbemNewTurn(bool bNew);

	bool isEndTurn() const;
	DllExport void setEndTurn(bool bNewValue);

	DllExport bool isTurnDone() const;

	bool isExtendedGame() const;																																			// Exposed to Python					
	void makeExtendedGame();																													
																																															
	bool isFoundedFirstCity() const;																																	// Exposed to Python					
	void setFoundedFirstCity(bool bNewValue);																										
																																															
	bool isStrike() const;																																	// Exposed to Python					
	void setStrike(bool bNewValue);																															

	// MOD - START - City Messages
	bool isCityMessagesDirty() const;
	void setCityMessagesDirty(bool bNewValue);

	void updateCityMessages();
	// MOD - END - City Messages

	DllExport PlayerTypes getID() const;																												// Exposed to Python					
																																															
	DllExport HandicapTypes getHandicapType() const;																									// Exposed to Python					
																																															
	DllExport CivilizationTypes getCivilizationType() const;																					// Exposed to Python					
																																															
	DllExport LeaderHeadTypes getLeaderType() const;																									// Exposed to Python					
																																															
	LeaderHeadTypes getPersonalityType() const;																												// Exposed to Python									
	void setPersonalityType(LeaderHeadTypes eNewValue);																					// Exposed to Python									
																																																				
	DllExport EraTypes getCurrentEra() const;																										// Exposed to Python									
	void setCurrentEra(EraTypes eNewValue);																											
																																															
	// MOD - START - Era Effects
	int getEraHappiness() const;
	int getEraUnhappiness() const;
	int getEraHealthiness() const;
	int getEraUnhealthiness() const;
	// MOD - END - Era Effects

	ReligionTypes getLastStateReligion() const;																												
	ReligionTypes getStateReligion() const;																									// Exposed to Python					
	void setLastStateReligion(ReligionTypes eNewValue);																					// Exposed to Python					
																																															
	PlayerTypes getParent() const;
	void setParent(PlayerTypes eParent);

	DllExport TeamTypes getTeam() const;																												// Exposed to Python					
	void setTeam(TeamTypes eTeam);		
	void updateTeamType();
																																																							
	DllExport PlayerColorTypes getPlayerColor() const;																								// Exposed to Python									
	DllExport int getPlayerTextColorR() const;																												// Exposed to Python								
	DllExport int getPlayerTextColorG() const;																												// Exposed to Python									
	DllExport int getPlayerTextColorB() const;																												// Exposed to Python									
	int getPlayerTextColorA() const;																												// Exposed to Python									
																																									
	int getSeaPlotYield(YieldTypes eIndex) const;																											// Exposed to Python
	void changeSeaPlotYield(YieldTypes eIndex, int iChange);

	int getYieldRateModifier(YieldTypes eIndex) const;																								// Exposed to Python
	void changeYieldRateModifier(YieldTypes eIndex, int iChange);

	int getCapitalYieldRateModifier(YieldTypes eIndex) const;																					// Exposed to Python
	void changeCapitalYieldRateModifier(YieldTypes eIndex, int iChange);

	int getExtraYieldThreshold(YieldTypes eIndex) const;																							// Exposed to Python
	void updateExtraYieldThreshold(YieldTypes eIndex);

	int getTradeYieldModifier(YieldTypes eIndex) const;																								// Exposed to Python
	void changeTradeYieldModifier(YieldTypes eIndex, int iChange);

	int getFreeCityCommerce(CommerceTypes eIndex) const;																							// Exposed to Python
	void changeFreeCityCommerce(CommerceTypes eIndex, int iChange);

	// MOD - START - Open Borders Commerce
	int getOpenBordersCommerceBorderCount(CommerceTypes eIndex) const;																							// Exposed to Python

	int getBaseOpenBordersFreeCityCommerce(CommerceTypes eIndex) const;																							// Exposed to Python
	void changeBaseOpenBordersFreeCityCommerce(CommerceTypes eIndex, int iChange);

	int getAdditionalOpenBordersFreeCityCommerce(CommerceTypes eIndex) const;																							// Exposed to Python
	void changeAdditionalOpenBordersFreeCityCommerce(CommerceTypes eIndex, int iChange);

	int getTotalOpenBordersFreeCityCommerce(CommerceTypes eIndex) const;																							// Exposed to Python
	void setTotalOpenBordersFreeCityCommerce(CommerceTypes eIndex, int iValue);

	void updateTotalOpenBordersFreeCityCommerce(CommerceTypes eIndex);
	void updateTotalOpenBordersFreeCityCommerce();
	// MOD - END - Open Borders Commerce

	// MOD - START - Tech Transfer Commerce
	/*
	int getBaseTechTransferFreeCityCommerce(CommerceTypes eIndex) const;																							// Exposed to Python
	void changeBaseTechTransferFreeCityCommerce(CommerceTypes eIndex, int iChange);

	int getAdditionalTechTransferFreeCityCommerce(CommerceTypes eIndex) const;																							// Exposed to Python
	void changeAdditionalTechTransferFreeCityCommerce(CommerceTypes eIndex, int iChange);

	int getTotalTechTransferFreeCityCommerce(CommerceTypes eIndex) const;																							// Exposed to Python
	void setTotalTechTransferFreeCityCommerce(CommerceTypes eIndex, int iValue);

	void updateTotalTechTransferFreeCityCommerce(CommerceTypes eIndex);
	void updateTotalTechTransferFreeCityCommerce();
	*/
	// MOD - END - Tech Transfer Commerce

	int getCommercePercent(CommerceTypes eIndex) const;																								// Exposed to Python
	/* void setCommercePercent(CommerceTypes eIndex, int iNewValue); // Exposed to Python
	DllExport void changeCommercePercent(CommerceTypes eIndex, int iChange); */ // Exposed to Python
	// K-Mod. these functions now return false if the value is not changed.
	bool setCommercePercent(CommerceTypes eIndex, int iNewValue, bool bForce = false); // Exposed to Python
	bool changeCommercePercent(CommerceTypes eIndex, int iChange); // Exposed to Python
	// K-Mod end

	int getCommerceRate(CommerceTypes eIndex) const;																									// Exposed to Python
	void changeCommerceRate(CommerceTypes eIndex, int iChange);

	int getCommerceRateModifier(CommerceTypes eIndex) const;																					// Exposed to Python
	void changeCommerceRateModifier(CommerceTypes eIndex, int iChange);

	int getCapitalCommerceRateModifier(CommerceTypes eIndex) const;																		// Exposed to Python
	void changeCapitalCommerceRateModifier(CommerceTypes eIndex, int iChange);

	// MOD - START - Open Borders Commerce
	int getBaseOpenBordersCommerceRateModifier(CommerceTypes eIndex) const;																							// Exposed to Python
	void changeBaseOpenBordersCommerceRateModifier(CommerceTypes eIndex, int iChange);

	int getAdditionalOpenBordersCommerceRateModifier(CommerceTypes eIndex) const;																							// Exposed to Python
	void changeAdditionalOpenBordersCommerceRateModifier(CommerceTypes eIndex, int iChange);

	int getTotalOpenBordersCommerceRateModifier(CommerceTypes eIndex) const;																							// Exposed to Python
	void setTotalOpenBordersCommerceRateModifier(CommerceTypes eIndex, int iValue);

	void updateTotalOpenBordersCommerceRateModifier(CommerceTypes eIndex);
	void updateTotalOpenBordersCommerceRateModifiers();
	// MOD - END - Open Borders Commerce

	// MOD - START - Tech Transfer Commerce
	int getBaseTechTransferCommerceRateModifier(CommerceTypes eIndex) const;																							// Exposed to Python
	void changeBaseTechTransferCommerceRateModifier(CommerceTypes eIndex, int iChange);

	int getAdditionalTechTransferCommerceRateModifier(CommerceTypes eIndex) const;																							// Exposed to Python
	void changeAdditionalTechTransferCommerceRateModifier(CommerceTypes eIndex, int iChange);

	//int getTotalTechTransferCommerceRateModifier(CommerceTypes eIndex) const;																							// Exposed to Python
	//void setTotalTechTransferCommerceRateModifier(CommerceTypes eIndex, int iValue);

	int calculateTotalTechTransferCommerceRateModifier(CommerceTypes eIndex, TechTypes eTech) const;
	//void updateTotalTechTransferCommerceRateModifier(CommerceTypes eIndex);
	//void updateTotalTechTransferCommerceRateModifiers();
	// MOD - END - Tech Transfer Commerce

	int getStateReligionBuildingCommerce(CommerceTypes eIndex) const;																	// Exposed to Python
	void changeStateReligionBuildingCommerce(CommerceTypes eIndex, int iChange);

	// MOD - START - Advanced Yield and Commerce Effects
	//int getSpecialistExtraCommerce(CommerceTypes eIndex) const;																				// Exposed to Python
	//void changeSpecialistExtraCommerce(CommerceTypes eIndex, int iChange);

	int getAllSpecialistCommerceChange(CommerceTypes eCommerce) const;																				// Exposed to Python
	void changeAllSpecialistCommerceChange(CommerceTypes eCommerce, int iChange);

	int getSpecialistCommerceChange(SpecialistTypes eSpecialist, CommerceTypes eCommerce) const;																				// Exposed to Python
	void changeSpecialistCommerceChange(SpecialistTypes eSpecialist, CommerceTypes eCommerce, int iChange);

	void updateSpecialistCommerce(CommerceTypes eCommerce);
	// MOD - END - Advanced Yield and Commerce Effects

	int getCommerceFlexibleCount(CommerceTypes eIndex) const;
	bool isCommerceFlexible(CommerceTypes eIndex) const;																							// Exposed to Python
	void changeCommerceFlexibleCount(CommerceTypes eIndex, int iChange);

	int getGoldPerTurnByPlayer(PlayerTypes eIndex) const;																							// Exposed to Python
	void changeGoldPerTurnByPlayer(PlayerTypes eIndex, int iChange);

	bool isFeatAccomplished(FeatTypes eIndex) const;																									// Exposed to Python
	void setFeatAccomplished(FeatTypes eIndex, bool bNewValue);																	// Exposed to Python

	DllExport bool isOption(PlayerOptionTypes eIndex) const;																		// Exposed to Python
	DllExport void setOption(PlayerOptionTypes eIndex, bool bNewValue);													// Exposed to Python

	bool isLoyalMember(VoteSourceTypes eVoteSource) const;																		// Exposed to Python
	void setLoyalMember(VoteSourceTypes eVoteSource, bool bNewValue);													// Exposed to Python

	bool isPlayable() const;
	void setPlayable(bool bNewValue);

	int getBonusExport(BonusTypes eIndex) const;																											// Exposed to Python
	void changeBonusExport(BonusTypes eIndex, int iChange);

	int getBonusImport(BonusTypes eIndex) const;																											// Exposed to Python
	void changeBonusImport(BonusTypes eIndex, int iChange);

	int getImprovementCount(ImprovementTypes eIndex) const;																						// Exposed to Python
	void changeImprovementCount(ImprovementTypes eIndex, int iChange);

	int getFreeBuildingCount(BuildingTypes eIndex) const;
	bool isBuildingFree(BuildingTypes eIndex) const;																									// Exposed to Python
	void changeFreeBuildingCount(BuildingTypes eIndex, int iChange);

	int getExtraBuildingHappiness(BuildingTypes eIndex) const;																				// Exposed to Python
	void changeExtraBuildingHappiness(BuildingTypes eIndex, int iChange);
	int getExtraBuildingHealth(BuildingTypes eIndex) const;																				// Exposed to Python
	void changeExtraBuildingHealth(BuildingTypes eIndex, int iChange);

	int getFeatureHappiness(FeatureTypes eIndex) const;																								// Exposed to Python
	void changeFeatureHappiness(FeatureTypes eIndex, int iChange);

	// MOD - START - Unit Roles
	int getUnitRoleCostModifier(UnitRoleTypes eIndex) const; // Exposed to Python
	void changeUnitRoleCostModifier(UnitRoleTypes eIndex, int iChange);

	int getUnitRoleCount(UnitRoleTypes eIndex) const; // Exposed to Python
	void changeUnitRoleCount(UnitRoleTypes eIndex, int iChange);
	int getUnitRoleMaking(UnitRoleTypes eIndex) const; // Exposed to Python
	void changeUnitRoleMaking(UnitRoleTypes eIndex, int iChange);
	int getUnitRoleCountPlusMaking(UnitRoleTypes eIndex) const; // Exposed to Python
	// MOD - END - Unit Roles

	int getUnitClassCount(UnitClassTypes eIndex) const;																								// Exposed to Python
	bool isUnitClassMaxedOut(UnitClassTypes eIndex, int iExtra = 0) const;														// Exposed to Python
	void changeUnitClassCount(UnitClassTypes eIndex, int iChange);
	int getUnitClassMaking(UnitClassTypes eIndex) const;																							// Exposed to Python
	void changeUnitClassMaking(UnitClassTypes eIndex, int iChange);
	int getUnitClassCountPlusMaking(UnitClassTypes eIndex) const;																			// Exposed to Python

	// MOD - START - Player Build Caching
	int getBuilderCount(BuildTypes eBuild) const; // Exposed to Python
	void changeBuilderCount(BuildTypes eBuild, int iChange);
	// MOD - END - Player Build Caching

	int getBuildingClassCount(BuildingClassTypes eIndex) const;																				// Exposed to Python
	bool isBuildingClassMaxedOut(BuildingClassTypes eIndex, int iExtra = 0) const;										// Exposed to Python
	void changeBuildingClassCount(BuildingClassTypes eIndex, int iChange);
	int getBuildingClassMaking(BuildingClassTypes eIndex) const;																			// Exposed to Python 
	void changeBuildingClassMaking(BuildingClassTypes eIndex, int iChange);
	int getBuildingClassCountPlusMaking(BuildingClassTypes eIndex) const;															// Exposed to Python

	int getHurryCount(HurryTypes eIndex) const;																												// Exposed to Python
	bool canHurry(HurryTypes eIndex) const;																									// Exposed to Python
	bool canPopRush() const;
	void changeHurryCount(HurryTypes eIndex, int iChange);

	int getSpecialBuildingNotRequiredCount(SpecialBuildingTypes eIndex) const;												// Exposed to Python
	bool isSpecialBuildingNotRequired(SpecialBuildingTypes eIndex) const;															// Exposed to Python
	void changeSpecialBuildingNotRequiredCount(SpecialBuildingTypes eIndex, int iChange);

	int getHasCivicOptionCount(CivicOptionTypes eIndex) const;
	bool isHasCivicOption(CivicOptionTypes eIndex) const;																							// Exposed to Python
	void changeHasCivicOptionCount(CivicOptionTypes eIndex, int iChange);

	int getNoCivicUpkeepCount(CivicOptionTypes eIndex) const;
	bool isNoCivicUpkeep(CivicOptionTypes eIndex) const;																							// Exposed to Python
	void changeNoCivicUpkeepCount(CivicOptionTypes eIndex, int iChange);

	int getHasReligionCount(ReligionTypes eIndex) const;																							// Exposed to Python
	int countTotalHasReligion() const;																																// Exposed to Python
	int findHighestHasReligionCount() const;																													// Exposed to Python
	void changeHasReligionCount(ReligionTypes eIndex, int iChange);

	int getHasCorporationCount(CorporationTypes eIndex) const;																							// Exposed to Python
	int countTotalHasCorporation() const;																																// Exposed to Python
	void changeHasCorporationCount(CorporationTypes eIndex, int iChange);
	bool isActiveCorporation(CorporationTypes eIndex) const;

	// MOD - START - Holy City Migration
	bool canAcquireHolyCity(ReligionTypes eReligion) const; // Exposed to Python
	bool canMaintainHolyCity(ReligionTypes eReligion) const; // Exposed to Python

	int getHolyCityInfluence(ReligionTypes eReligion) const;
	void setHolyCityInfluence(ReligionTypes eReligion, int iNewValue);
	void changeHolyCityInfluence(ReligionTypes eReligion, int iChange);
	void updateHolyCityInfluence();
	void updateHolyCityInfluence(ReligionTypes eReligion);

	int getHolyCityInfluenceGrowthRate(ReligionTypes eReligion) const;
	int getHolyCityInfluenceDecayRate(ReligionTypes eReligion) const;
	int getHolyCityLossTurnsRemaining(ReligionTypes eReligion) const;

	CvCity* getHolyCityCandidate(ReligionTypes eReligion) const;
	void setHolyCityCandidate(ReligionTypes eReligion, CvCity* pNewValue);
	void updateHolyCityCandidate();
	void updateHolyCityCandidate(ReligionTypes eReligion, CvCity* pIgnoreCity = NULL);
	// MOD - END - Holy City Migration

	int getUpkeepCount(UpkeepTypes eIndex) const;																											// Exposed to Python
	void changeUpkeepCount(UpkeepTypes eIndex, int iChange);

	int getSpecialistValidCount(SpecialistTypes eIndex) const;
	bool isSpecialistValid(SpecialistTypes eIndex) const;																		// Exposed to Python					
	void changeSpecialistValidCount(SpecialistTypes eIndex, int iChange);

	bool isResearchingTech(TechTypes eIndex) const;																					// Exposed to Python					
	void setResearchingTech(TechTypes eIndex, bool bNewValue);

	CivicTypes getCivics(CivicOptionTypes eIndex) const;																		// Exposed to Python					
	int getSingleCivicUpkeep(CivicTypes eCivic, bool bIgnoreAnarchy = false) const;										// Exposed to Python					
	int getCivicUpkeep(CivicTypes* paeCivics = NULL, bool bIgnoreAnarchy = false) const;							// Exposed to Python					
	void setCivics(CivicOptionTypes eIndex, CivicTypes eNewValue);															// Exposed to Python					

	// MOD - START - Advanced Yield and Commerce Effects
	/*
	int getSpecialistExtraYield(SpecialistTypes eIndex1, YieldTypes eIndex2) const;										// Exposed to Python
	void changeSpecialistExtraYield(SpecialistTypes eIndex1, YieldTypes eIndex2, int iChange);
	*/
	int getSpecialistYieldChange(SpecialistTypes eSpecialist, YieldTypes eYield) const;										// Exposed to Python
	void changeSpecialistYieldChange(SpecialistTypes eSpecialist, YieldTypes eYield, int iChange);
	// MOD - END - Advanced Yield and Commerce Effects

	int getImprovementYieldChange(ImprovementTypes eIndex1, YieldTypes eIndex2) const;								// Exposed to Python
	void changeImprovementYieldChange(ImprovementTypes eIndex1, YieldTypes eIndex2, int iChange);

	// MOD - START - Advanced Yield and Commerce Effects
	int getBuildingYieldChange(BuildingClassTypes eBuildingClass, YieldTypes eYield) const;
	void setBuildingYieldChange(BuildingClassTypes eBuildingClass, YieldTypes eYield, int iChange);
	void changeBuildingYieldChange(BuildingClassTypes eBuildingClass, YieldTypes eYield, int iChange);
	int getBuildingCommerceChange(BuildingClassTypes eBuildingClass, CommerceTypes eCommerce) const;
	void setBuildingCommerceChange(BuildingClassTypes eBuildingClass, CommerceTypes eCommerce, int iChange);
	void changeBuildingCommerceChange(BuildingClassTypes eBuildingClass, CommerceTypes eCommerce, int iChange);
	// MOD - END - Advanced Yield and Commerce Effects

	//void updateGroupCycle(CvUnit* pUnit);
	void updateGroupCycle(CvSelectionGroup* pGroup); // K-Mod
	void removeGroupCycle(int iID);
	void refreshGroupCycleList(); // K-Mod
	CLLNode<int>* deleteGroupCycleNode(CLLNode<int>* pNode);
	CLLNode<int>* nextGroupCycleNode(CLLNode<int>* pNode) const;
	CLLNode<int>* previousGroupCycleNode(CLLNode<int>* pNode) const;
	CLLNode<int>* headGroupCycleNode() const;
	CLLNode<int>* tailGroupCycleNode() const;

	// MOD - START - Tech Path Caching
	//int findPathLength(TechTypes eTech, bool bCost = true) const; // Exposed to Python
	int getTechPathLength(TechTypes eTech) const;
	int getTechPathCost(TechTypes eTech) const;
	// MOD - END - Tech Path Caching
	int getQueuePosition(TechTypes eTech) const;																											// Exposed to Python
	void clearResearchQueue();																												// Exposed to Python
	bool pushResearch(TechTypes eTech, bool bClear = false);													// Exposed to Python
	void popResearch(TechTypes eTech);																													// Exposed to Python
	int getLengthResearchQueue() const;																																// Exposed to Python
	CLLNode<TechTypes>* nextResearchQueueNode(CLLNode<TechTypes>* pNode) const;
	CLLNode<TechTypes>* headResearchQueueNode() const;
	CLLNode<TechTypes>* tailResearchQueueNode() const;

	void addCityName(const CvWString& szName);																									// Exposed to Python
	int getNumCityNames() const;																																// Exposed to Python
	CvWString getCityName(int iIndex) const;																										// Exposed to Python
	CLLNode<CvWString>* nextCityNameNode(CLLNode<CvWString>* pNode) const;
	CLLNode<CvWString>* headCityNameNode() const;
	// MOD - START - City Name Preservation
	void setPlotCityName(CvPlot* pPlot, CvWString szCityName);
	void erasePlotCityName(CvPlot* pPlot);
	CvWString getPlotCityName(CvPlot* pPlot) const;
	// MOD - END - City Name Preservation

	// plot groups iteration
	CvPlotGroup* firstPlotGroup(int *pIterIdx, bool bRev=false) const;
	CvPlotGroup* nextPlotGroup(int *pIterIdx, bool bRev=false) const;
	int getNumPlotGroups() const;
	CvPlotGroup* getPlotGroup(int iID) const;
	CvPlotGroup* addPlotGroup();
	void deletePlotGroup(int iID);

	// city iteration
	DllExport CvCity* firstCity(int *pIterIdx, bool bRev=false) const;																// Exposed to Python					
	DllExport CvCity* nextCity(int *pIterIdx, bool bRev=false) const;																	// Exposed to Python					
	DllExport int getNumCities() const;																																// Exposed to Python					
	DllExport CvCity* getCity(int iID) const;																													// Exposed to Python					
	CvCity* addCity();																																					
	void deleteCity(int iID);																																		
																																															
	// unit iteration																																						
	DllExport CvUnit* firstUnit(int *pIterIdx, bool bRev=false) const;																// Exposed to Python					
	DllExport CvUnit* nextUnit(int *pIterIdx, bool bRev=false) const;																	// Exposed to Python					
	DllExport int getNumUnits() const;																																// Exposed to Python					
	CvUnit* getUnit(int iID) const;																													// Exposed to Python					
	CvUnit* addUnit();																																					
	void deleteUnit(int iID);
																																															
	// selection groups iteration																																
	CvSelectionGroup* firstSelectionGroup(int *pIterIdx, bool bRev=false) const;						// Exposed to Python					
	CvSelectionGroup* nextSelectionGroup(int *pIterIdx, bool bRev=false) const;							// Exposed to Python					
	int getNumSelectionGroups() const;																																// Exposed to Python
	CvSelectionGroup* getSelectionGroup(int iID) const;																								// Exposed to Python
	CvSelectionGroup* addSelectionGroup();
	void deleteSelectionGroup(int iID);

	// pending triggers iteration																																
	EventTriggeredData* firstEventTriggered(int *pIterIdx, bool bRev=false) const;
	EventTriggeredData* nextEventTriggered(int *pIterIdx, bool bRev=false) const;
	int getNumEventsTriggered() const;
	EventTriggeredData* getEventTriggered(int iID) const;   // Exposed to Python
	EventTriggeredData* addEventTriggered();
	void deleteEventTriggered(int iID);
	EventTriggeredData* initTriggeredData(EventTriggerTypes eEventTrigger, bool bFire = false, int iCityId = -1, int iPlotX = INVALID_PLOT_COORD, int iPlotY = INVALID_PLOT_COORD, PlayerTypes eOtherPlayer = NO_PLAYER, int iOtherPlayerCityId = -1, ReligionTypes eReligion = NO_RELIGION, CorporationTypes eCorporation = NO_CORPORATION, int iUnitId = -1, BuildingTypes eBuilding = NO_BUILDING);   // Exposed to Python
	int getEventTriggerWeight(EventTriggerTypes eTrigger) const;    // Exposed to python

	DllExport void addMessage(const CvTalkingHeadMessage& message);
	void showMissedMessages();
	void clearMessages();
	DllExport const CvMessageQueue& getGameMessages() const;
	void expireMessages();
	DllExport void addPopup(CvPopupInfo* pInfo, bool bFront = false);
	void clearPopups();
	DllExport CvPopupInfo* popFrontPopup();
	DllExport const CvPopupQueue& getPopups() const;
	DllExport void addDiplomacy(CvDiploParameters* pDiplo);
	void clearDiplomacy();
	DllExport const CvDiploQueue& getDiplomacy() const;
	DllExport CvDiploParameters* popFrontDiplomacy();
	DllExport void showSpaceShip();
	DllExport void clearSpaceShipPopups();

	int getScoreHistory(int iTurn) const;																								// Exposed to Python
	void updateScoreHistory(int iTurn, int iBestScore);

	int getEconomyHistory(int iTurn) const;																							// Exposed to Python
	void updateEconomyHistory(int iTurn, int iBestEconomy);
	int getIndustryHistory(int iTurn) const;																						// Exposed to Python
	void updateIndustryHistory(int iTurn, int iBestIndustry);
	int getAgricultureHistory(int iTurn) const;																					// Exposed to Python
	void updateAgricultureHistory(int iTurn, int iBestAgriculture);
	int getPowerHistory(int iTurn) const;																								// Exposed to Python
	void updatePowerHistory(int iTurn, int iBestPower);
	int getCultureHistory(int iTurn) const;																							// Exposed to Python
	void updateCultureHistory(int iTurn, int iBestCulture);
	int getEspionageHistory(int iTurn) const;																							// Exposed to Python
	void updateEspionageHistory(int iTurn, int iBestEspionage);

	const CvPlayerRecord* getPlayerRecord() const; // K-Mod

	// Script data needs to be a narrow string for pickling in Python
	std::string getScriptData() const;																									// Exposed to Python
	void setScriptData(std::string szNewValue);																					// Exposed to Python

	DllExport const CvString getPbemEmailAddress() const;
	DllExport void setPbemEmailAddress(const char* szAddress);
	DllExport const CvString getSmtpHost() const;
	DllExport void setSmtpHost(const char* szHost);

	// MOD - START - Population Metrics
	void updateMetricTurnValues();
	// MOD - END - Population Metrics

	const EventTriggeredData* getEventOccured(EventTypes eEvent) const;			// Exposed to python
	bool isTriggerFired(EventTriggerTypes eEventTrigger) const;
	void setEventOccured(EventTypes eEvent, const EventTriggeredData& kEventTriggered, bool bOthers = true);
	void resetEventOccured(EventTypes eEvent, bool bAnnounce = true);													// Exposed to Python
	void setTriggerFired(const EventTriggeredData& kTriggeredData, bool bOthers = true, bool bAnnounce = true);	
	void resetTriggerFired(EventTriggerTypes eEventTrigger);
	void trigger(EventTriggerTypes eEventTrigger);													// Exposed to Python
	void trigger(const EventTriggeredData& kData);
	void applyEvent(EventTypes eEvent, int iTriggeredId, bool bUpdateTrigger = true);
	bool canDoEvent(EventTypes eEvent, const EventTriggeredData& kTriggeredData) const;
	TechTypes getBestEventTech(EventTypes eEvent, PlayerTypes eOtherPlayer) const;
	int getEventCost(EventTypes eEvent, PlayerTypes eOtherPlayer, bool bRandom) const;
	bool canTrigger(EventTriggerTypes eTrigger, PlayerTypes ePlayer, ReligionTypes eReligion) const;
	const EventTriggeredData* getEventCountdown(EventTypes eEvent) const;
	void setEventCountdown(EventTypes eEvent, const EventTriggeredData& kEventTriggered);
	void resetEventCountdown(EventTypes eEvent);
	// MOD - START - Game Speed Event Scaling
	int getEventFood(EventTypes eEvent) const;
	int getEventFoodPercent(EventTypes eEvent) const;
	// MOD - END - Game Speed Event Scaling

	bool isFreePromotion(UnitCombatTypes eUnitCombat, PromotionTypes ePromotion) const;
	void setFreePromotion(UnitCombatTypes eUnitCombat, PromotionTypes ePromotion, bool bFree);
	bool isFreePromotion(UnitClassTypes eUnitCombat, PromotionTypes ePromotion) const;
	void setFreePromotion(UnitClassTypes eUnitCombat, PromotionTypes ePromotion, bool bFree);

	PlayerVoteTypes getVote(int iId) const;
	void setVote(int iId, PlayerVoteTypes ePlayerVote);

	int getUnitExtraCost(UnitClassTypes eUnitClass) const;
	void setUnitExtraCost(UnitClassTypes eUnitClass, int iCost);

	bool splitEmpire(int iAreaId);
	bool canSplitEmpire() const;
	bool canSplitArea(int iAreaId) const;
	PlayerTypes getSplitEmpirePlayer(int iAreaId) const;
	bool getSplitEmpireLeaders(CivLeaderArray& aLeaders) const;

	void launch(VictoryTypes victoryType);

	bool hasShrine(ReligionTypes eReligion);
	int getVotes(VoteTypes eVote, VoteSourceTypes eVoteSource) const;   // Exposed to Python
	void processVoteSourceBonus(VoteSourceTypes eVoteSource, bool bActive);
	bool canDoResolution(VoteSourceTypes eVoteSource, const VoteSelectionSubData& kData) const;
	bool canDefyResolution(VoteSourceTypes eVoteSource, const VoteSelectionSubData& kData) const;
	void setDefiedResolution(VoteSourceTypes eVoteSource, const VoteSelectionSubData& kData);
	void setEndorsedResolution(VoteSourceTypes eVoteSource, const VoteSelectionSubData& kData);
	bool isFullMember(VoteSourceTypes eVoteSource) const;    // Exposed to Python
	bool isVotingMember(VoteSourceTypes eVoteSource) const;    // Exposed to Python

	void invalidatePopulationRankCache();
	void invalidateYieldRankCache(YieldTypes eYield = NO_YIELD);
	void invalidateCommerceRankCache(CommerceTypes eCommerce = NO_COMMERCE);

	PlayerTypes pickConqueredCityOwner(const CvCity& kCity) const;
	bool canHaveTradeRoutesWith(PlayerTypes ePlayer) const;

	void forcePeace(PlayerTypes ePlayer);    // exposed to Python

	bool canSpiesEnterBorders(PlayerTypes ePlayer) const;
	int getNewCityProductionValue() const;

	int getGrowthThreshold(int iPopulation) const;

	void verifyUnitStacksValid();
	UnitTypes getTechFreeUnit(TechTypes eTech) const;

	DllExport void buildTradeTable(PlayerTypes eOtherPlayer, CLinkList<TradeData>& ourList) const;
	DllExport bool getHeadingTradeString(PlayerTypes eOtherPlayer, TradeableItems eItem, CvWString& szString, CvString& szIcon) const;
	DllExport bool getItemTradeString(PlayerTypes eOtherPlayer, bool bOffer, bool bShowingCurrent, const TradeData& zTradeData, CvWString& szString, CvString& szIcon) const;
	DllExport void updateTradeList(PlayerTypes eOtherPlayer, CLinkList<TradeData>& ourInventory, const CLinkList<TradeData>& ourOffer, const CLinkList<TradeData>& theirOffer) const;
	void markTradeOffers(CLinkList<TradeData>& ourInventory, const CLinkList<TradeData>& ourOffer) const; // K-Mod
	DllExport int getIntroMusicScriptId(PlayerTypes eForPlayer) const;
	DllExport int getMusicScriptId(PlayerTypes eForPlayer) const;
	DllExport void getGlobeLayerColors(GlobeLayerTypes eGlobeLayerType, int iOption, std::vector<NiColorA>& aColors, std::vector<CvPlotIndicatorData>& aIndicators) const;
	DllExport void cheat(bool bCtrl, bool bAlt, bool bShift);

	DllExport const CvArtInfoUnit* getUnitArtInfo(UnitTypes eUnit, int iMeshGroup = 0) const;
	DllExport bool hasSpaceshipArrived() const;

	// MOD - START - GameCore Scoring (poyuzhe)
	int getScoreComponent(int iRawScore, int iInitial, int iMax, int iFactor, bool bExponential, bool bFinal, bool bVictory) const;
	// MOD - END - GameCore Scoring (poyuzhe)

	// K-Mod note: Adding new virtual functions to this list seems to cause unpredictable behaviour during the initialization of the game.
	// So beware!
	virtual void AI_init() = 0;
	virtual void AI_reset(bool bConstructor) = 0;
	virtual void AI_doTurnPre() = 0;
	virtual void AI_doTurnPost() = 0;
	virtual void AI_doTurnUnitsPre() = 0;
	virtual void AI_doTurnUnitsPost() = 0;
	//virtual void AI_updateFoundValues(bool bStartingLoc = false) const = 0;
	virtual void AI_updateFoundValues(bool bStartingLoc = false) = 0; // K-Mod. (Can I fix the const-correctness without breaking compatibility? No problems so far...)
	virtual void AI_unitUpdate() = 0;
	virtual void AI_makeAssignWorkDirty() = 0;
	virtual void AI_assignWorkingPlots() = 0;
	virtual void AI_updateAssignWork() = 0;
	virtual void AI_makeProductionDirty() = 0;
	virtual void AI_conquerCity(CvCity* pCity) = 0;
	//virtual int AI_foundValue(int iX, int iY, int iMinUnitRange = -1, bool bStartingLoc = false) const = 0; // Exposed to Python
	virtual short AI_foundValue(int iX, int iY, int iMinUnitRange = -1, bool bStartingLoc = false) const = 0; // Exposed to Python. K-Mod changed return value from int to short
	virtual bool AI_isCommercePlot(CvPlot* pPlot) const = 0;
	virtual int AI_getPlotDanger(CvPlot* pPlot, int iRange = -1, bool bTestMoves = true) const = 0;
	virtual bool AI_isFinancialTrouble() const = 0;																											// Exposed to Python
	virtual TechTypes AI_bestTech(int iMaxPathLength = 1, bool bIgnoreCost = false, bool bAsync = false, TechTypes eIgnoreTech = NO_TECH, AdvisorTypes eIgnoreAdvisor = NO_ADVISOR) const = 0;
	virtual void AI_chooseFreeTech() = 0;
	virtual void AI_chooseResearch() = 0;
	virtual bool AI_isWillingToTalk(PlayerTypes ePlayer) const = 0; // Exposed to Python
	virtual bool AI_demandRebukedSneak(PlayerTypes ePlayer) const = 0;
	virtual bool AI_demandRebukedWar(PlayerTypes ePlayer) const = 0;																		// Exposed to Python
	virtual AttitudeTypes AI_getAttitude(PlayerTypes ePlayer, bool bForced = true) const = 0;																// Exposed to Python
	virtual PlayerVoteTypes AI_diploVote(const VoteSelectionSubData& kVoteData, VoteSourceTypes eVoteSource, bool bPropose) = 0;
	virtual int AI_dealVal(PlayerTypes ePlayer, const CLinkList<TradeData>* pList, bool bIgnoreAnnual = false, int iExtra = 0) const = 0;
	virtual bool AI_considerOffer(PlayerTypes ePlayer, const CLinkList<TradeData>* pTheirList, const CLinkList<TradeData>* pOurList, int iChange = 1) const = 0;
	virtual bool AI_counterPropose(PlayerTypes ePlayer, const CLinkList<TradeData>* pTheirList, const CLinkList<TradeData>* pOurList, CLinkList<TradeData>* pTheirInventory, CLinkList<TradeData>* pOurInventory, CLinkList<TradeData>* pTheirCounter, CLinkList<TradeData>* pOurCounter) const = 0;
	//virtual int AI_bonusVal(BonusTypes eBonus, int iChange = 0) const = 0;
	virtual int AI_bonusVal(BonusTypes eBonus, int iChange, bool bAssumeEnabled = false) const = 0; // K-Mod added bAssumeEnabled
	virtual int AI_bonusTradeVal(BonusTypes eBonus, PlayerTypes ePlayer, int iChange = 0) const = 0;
	virtual DenialTypes AI_bonusTrade(BonusTypes eBonus, PlayerTypes ePlayer) const = 0;
	virtual int AI_cityTradeVal(CvCity* pCity) const = 0;
	virtual DenialTypes AI_cityTrade(CvCity* pCity, PlayerTypes ePlayer) const = 0;
	virtual DenialTypes AI_stopTradingTrade(TeamTypes eTradeTeam, PlayerTypes ePlayer) const = 0;
	virtual DenialTypes AI_civicTrade(CivicTypes eCivic, PlayerTypes ePlayer) const = 0;
	virtual DenialTypes AI_religionTrade(ReligionTypes eReligion, PlayerTypes ePlayer) const = 0;
	virtual int AI_unitValue(UnitTypes eUnit, UnitAITypes eUnitAI, CvArea* pArea) const = 0;						// Exposed to Python
	virtual int AI_totalUnitAIs(UnitAITypes eUnitAI) const = 0;																					// Exposed to Python
	virtual int AI_totalAreaUnitAIs(CvArea* pArea, UnitAITypes eUnitAI) const = 0;											// Exposed to Python
	// MOD - START - Multiple City Water Areas
	//virtual int AI_totalWaterAreaUnitAIs(CvArea* pArea, UnitAITypes eUnitAI) const = 0;									// Exposed to Python
	virtual int AI_totalWaterAreaUnitAIs(CvArea* pArea, UnitAITypes eUnitAI, CvCity* pIgnoreCity = NULL) const = 0;									// Exposed to Python
	// MOD - END - Multiple City Water Areas
	virtual int AI_plotTargetMissionAIs(CvPlot* pPlot, MissionAITypes eMissionAI, CvSelectionGroup* pSkipSelectionGroup = NULL, int iRange = 0) const = 0;
	virtual int AI_unitTargetMissionAIs(CvUnit* pUnit, MissionAITypes eMissionAI, CvSelectionGroup* pSkipSelectionGroup = NULL) const = 0;
	virtual int AI_civicValue(CivicTypes eCivic) const = 0;   // Exposed to Python
	virtual int AI_getNumAIUnits(UnitAITypes eIndex) const = 0;																					// Exposed to Python
	virtual void AI_changePeacetimeTradeValue(PlayerTypes eIndex, int iChange) = 0;
	virtual void AI_changePeacetimeGrantValue(PlayerTypes eIndex, int iChange) = 0;
	virtual int AI_getAttitudeExtra(PlayerTypes eIndex) const = 0;																			// Exposed to Python
	virtual void AI_setAttitudeExtra(PlayerTypes eIndex, int iNewValue) = 0;											// Exposed to Python
	virtual void AI_changeAttitudeExtra(PlayerTypes eIndex, int iChange) = 0;											// Exposed to Python
	virtual void AI_setFirstContact(PlayerTypes eIndex, bool bNewValue) = 0;
	virtual int AI_getMemoryCount(PlayerTypes eIndex1, MemoryTypes eIndex2) const = 0;
	virtual void AI_changeMemoryCount(PlayerTypes eIndex1, MemoryTypes eIndex2, int iChange) = 0;
	virtual void AI_doCommerce() = 0;
	virtual EventTypes AI_chooseEvent(int iTriggeredId) const = 0;
	virtual void AI_launch(VictoryTypes eVictory) = 0;
	virtual void AI_doAdvancedStart(bool bNoExit = false) = 0;
	virtual void AI_updateBonusValue() = 0;
	virtual void AI_updateBonusValue(BonusTypes eBonus) = 0;
	virtual ReligionTypes AI_chooseReligion() = 0;
	virtual int AI_getExtraGoldTarget() const = 0;
	virtual void AI_setExtraGoldTarget(int iNewValue) = 0;
	virtual int AI_maxGoldPerTurnTrade(PlayerTypes ePlayer) const = 0;
	virtual int AI_maxGoldTrade(PlayerTypes ePlayer) const = 0;

protected:

	int m_iStartingX;
	int m_iStartingY;
	int m_iTotalPopulation;
	int m_iTotalLand;
	int m_iTotalLandScored;
	int m_iGold;
	int m_iGoldPerTurn;
	int m_iAdvancedStartPoints;
	int m_iGoldenAgeTurns;
	int m_iNumUnitGoldenAges;
	int m_iStrikeTurns;
	int m_iAnarchyTurns;
	int m_iMaxAnarchyTurns;
	int m_iAnarchyModifier;
	int m_iGoldenAgeModifier;
	int m_iGlobalHurryModifier;
	int m_iGreatPeopleCreated;
	int m_iGreatGeneralsCreated;
	int m_iGreatPeopleThresholdModifier;
	int m_iGreatGeneralsThresholdModifier;
	int m_iGreatPeopleRateModifier;
	int m_iGreatGeneralRateModifier;
	int m_iDomesticGreatGeneralRateModifier;
	int m_iStateReligionGreatPeopleRateModifier;
	int m_iMaxGlobalBuildingProductionModifier;
	int m_iMaxTeamBuildingProductionModifier;
	int m_iMaxPlayerBuildingProductionModifier;
	// MOD - START - Epidemics
	// Epidemic system by Mexico
	int m_iAcceptableEpidemicPercent;
	int m_iGlobalEpidemicModifier;
	// MOD - END - Epidemics
	int m_iFreeExperience;
	int m_iFeatureProductionModifier;
	int m_iWorkerSpeedModifier;
	int m_iImprovementUpgradeRateModifier;
	int m_iMilitaryProductionModifier;
	int m_iSpaceProductionModifier;
	int m_iCityDefenseModifier;
	int m_iNumNukeUnits;
	int m_iNumOutsideUnits;
	int m_iBaseFreeUnits;
	int m_iBaseFreeMilitaryUnits;
	int m_iFreeUnitsPopulationPercent;
	int m_iFreeMilitaryUnitsPopulationPercent;
	int m_iGoldPerUnit;
	int m_iGoldPerMilitaryUnit;
	int m_iExtraUnitCost;
	int m_iNumMilitaryUnits;
	int m_iHappyPerMilitaryUnit;
	int m_iMilitaryFoodProductionCount;
	int m_iConscriptCount;
	int m_iMaxConscript;
	int m_iHighestUnitLevel;
	int m_iOverflowResearch;
	//int m_iNoUnhealthyPopulationCount;
	int m_iUnhealthyPopulationModifier; // K-Mod
	int m_iExpInBorderModifier;
	int m_iBuildingOnlyHealthyCount;
	int m_iDistanceMaintenanceModifier;
	int m_iNumCitiesMaintenanceModifier;
	int m_iCorporationMaintenanceModifier;
	long m_iTotalMaintenance;
	int m_iUpkeepModifier;
	int m_iLevelExperienceModifier;
	int m_iExtraHealth;
	int m_iBuildingGoodHealth;
	int m_iBuildingBadHealth;
	int m_iExtraHappiness;
	int m_iBuildingHappiness;
	int m_iLargestCityHappiness;
	int m_iWarWearinessPercentAnger;
	int m_iWarWearinessModifier;
	// MOD - START - Unlimited Conscription
	int m_iUnlimitedConscriptCount;
	// MOD - END - Unlimited Conscription
	// MOD - START - Wartime Conscription
	int m_iWarConscript;
	// MOD - END - Wartime Conscription
	int m_iGwPercentAnger; // K-Mod
	int m_iFreeSpecialist;
	int m_iNoForeignTradeCount;
	int m_iNoCorporationsCount;
	int m_iNoForeignCorporationsCount;
	int m_iCoastalTradeRoutes;
	int m_iTradeRoutes;
	int m_iRevolutionTimer;
	int m_iConversionTimer;
	int m_iStateReligionCount;
	int m_iNoNonStateReligionSpreadCount;
	// MOD - START - Holy City Migration
	int m_iCanAcquireHolyCityWithoutStateReligionCount;
	int m_iCanMaintainHolyCityWithoutStateReligionCount;
	// MOD - END - Holy City Migration
	int m_iStateReligionHappiness;
	int m_iNonStateReligionHappiness;
	int m_iStateReligionUnitProductionModifier;
	int m_iStateReligionBuildingProductionModifier;
	int m_iStateReligionFreeExperience;
	int m_iCapitalCityID;
	int m_iCitiesLost;
	int m_iWinsVsBarbs;
	int m_iAssets;
	int m_iPower;
	int m_iPopulationScore;
	int m_iLandScore;
	int m_iTechScore;
	int m_iWondersScore;
	// MOD - START - Handicap Score Modifier
	int m_iHandicapScoreModifier;
	// MOD - END - Handicap Score Modifier
	int m_iCombatExperience;
	int m_iPopRushHurryCount;
	int m_iInflationModifier;
	int m_iInflationRate; // K-Mod
	// MOD - START - Trait Trade Modifier
	int m_iTradeRouteIncomeModifier;
	int m_iForeignTradeRouteIncomeModifier;
	// MOD - END - Trait Trade Modifier
	// MOD - START - Trait Upgrade Modifier
	int m_iUpgradeDiscount;
	// MOD - END - Trait Upgrade Modifier
	// MOD - START - Congestion
	int m_iInsideCityCongestionThreshold;
	int m_iOutsideCityCongestionThreshold;
	// MOD - END - Congestion

	// MOD - START - Better Nukes
	int m_iNukesUsed;
	// MOD - END - Better Nukes

	uint m_uiStartTime;  // XXX save these?

	bool m_bAlive;
	bool m_bEverAlive;
	bool m_bTurnActive;
	bool m_bAutoMoves;
	bool m_bEndTurn;
	bool m_bPbemNewTurn;
	bool m_bExtendedGame;
	bool m_bFoundedFirstCity;
	bool m_bStrike;
	// MOD - START - City Messages
	bool m_bCityMessagesDirty;
	// MOD - END - City Messages
	bool m_bHuman;

/************************************************************************************************/
/* AI_AUTO_PLAY_MOD                        09/01/07                            MRGENIE          */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	bool m_bDisableHuman;				// Set to true to disable isHuman() check
/************************************************************************************************/
/* AI_AUTO_PLAY_MOD                        END                                                  */
/************************************************************************************************/

	int m_iChoosingFreeTechCount; // K-Mod (based on the 'Unofficial Patch'

	/*************************************************************************************************/
/** REVOLUTION_MOD                         01/01/08                                jdog5000      */
/**                                                                                              */
/**                                                                                              */
/*************************************************************************************************/

	// Used for DynamicCivNames
	CvWString m_szName;
	CvWString m_szCivDesc;
	CvWString m_szCivShort;
	CvWString m_szCivAdj;
	
	int m_iRevModifier;
	int m_iRevModifierScore;
	int m_iRevModifierPower;
	int m_iRevModifierEoH;
	int m_iRevModifierWE;
	
/*************************************************************************************************/
/** REVOLUTION_MOD                          END                                                  */
/*************************************************************************************************/

	int m_iSlaveAfrican; 
	int m_iSlaveAsian; 
	int m_iSlaveEuropean; 
	int m_iSlaveIndian; 
	int m_iSlaveMid;
	int m_iSlaveNewworld;
	int m_iSlaveSlavic;
	
	PlayerTypes m_eID;
	LeaderHeadTypes m_ePersonalityType;
	EraTypes m_eCurrentEra;
	ReligionTypes m_eLastStateReligion;
	PlayerTypes m_eParent;
	TeamTypes m_eTeamType;

	int* m_aiSeaPlotYield;
	int* m_aiYieldRateModifier;
	int* m_aiCapitalYieldRateModifier;
	int* m_aiExtraYieldThreshold;
	int* m_aiTradeYieldModifier;
	int* m_aiFreeCityCommerce;
	// MOD - START - Open Borders Commerce
	int* m_aiBaseOpenBordersFreeCityCommerce;
	int* m_aiAdditionalOpenBordersFreeCityCommerce;
	int* m_aiTotalOpenBordersFreeCityCommerce;
	// MOD - END - Open Borders Commerce
	// MOD - START - Tech Transfer Commerce
	/*
	int* m_aiBaseTechTransferFreeCityCommerce;
	int* m_aiAdditionalTechTransferFreeCityCommerce;
	int* m_aiTotalTechTransferFreeCityCommerce;
	*/
	// MOD - END - Tech Transfer Commerce
	int* m_aiCommercePercent;
	int* m_aiCommerceRate;
	int* m_aiCommerceRateModifier;
	int* m_aiCapitalCommerceRateModifier;
	// MOD - START - Open Borders Commerce
	int* m_aiBaseOpenBordersCommerceRateModifier;
	int* m_aiAdditionalOpenBordersCommerceRateModifier;
	int* m_aiTotalOpenBordersCommerceRateModifier;
	// MOD - END - Open Borders Commerce
	// MOD - START - Tech Transfer Commerce
	int* m_aiBaseTechTransferCommerceRateModifier;
	int* m_aiAdditionalTechTransferCommerceRateModifier;
	//int* m_aiTotalTechTransferCommerceRateModifier;
	// MOD - END - Tech Transfer Commerce
	int* m_aiStateReligionBuildingCommerce;
	// MOD - START - Advanced Yield and Commerce Effects
	//int* m_aiSpecialistExtraCommerce;
	int* m_aiAllSpecialistCommerceChanges;
	// MOD - END - Advanced Yield and Commerce Effects
	int* m_aiCommerceFlexibleCount;
	int* m_aiGoldPerTurnByPlayer;
	int* m_aiEspionageSpendingWeightAgainstTeam;

	bool* m_abFeatAccomplished;
	bool* m_abOptions;

	CvString m_szScriptData;

	int* m_paiBonusExport;
	int* m_paiBonusImport;
	int* m_paiImprovementCount;
	int* m_paiFreeBuildingCount;
	int* m_paiExtraBuildingHappiness;
	int* m_paiExtraBuildingHealth;
	// MOD - START - Advanced Yield and Commerce Effects
	//int** m_paiExtraBuildingYield;
	//int** m_paiExtraBuildingCommerce;
	// MOD - END - Advanced Yield and Commerce Effects
	int* m_paiFeatureHappiness;
	// MOD - START - Unit Roles
	int* m_paiUnitRoleCostModifier;
	int* m_paiUnitRoleCount;
	int* m_paiUnitRoleMaking;
	// MOD - END - Unit Roles
	int* m_paiUnitClassCount;
	int* m_paiUnitClassMaking;
	// MOD - START - Player Build Caching
	int* m_paiBuilderCount;
	// MOD - END - Player Build Caching
	int* m_paiBuildingClassCount;
	int* m_paiBuildingClassMaking;
	int* m_paiHurryCount;
	int* m_paiSpecialBuildingNotRequiredCount;
	int* m_paiHasCivicOptionCount;
	int* m_paiNoCivicUpkeepCount;
	int* m_paiHasReligionCount;
	int* m_paiHasCorporationCount;
	// MOD - START - Holy City Migration
	int* m_paiHolyCityInfluence;
	int* m_paiHolyCityCandidate;
	// MOD - END - Holy City Migration
	int* m_paiUpkeepCount;
	int* m_paiSpecialistValidCount;
	// MOD - START - Player UnitCombat Free Experience
	int* m_paiUnitCombatFreeExperience;
	// MOD - END - Player UnitCombat Free Experience

	// MOD - START - Religion Affected Bonuses
	bool* m_pabReligionNoBonusHealthiness;
	bool* m_pabReligionNoBonusHappiness;
	// MOD - END - Religion Affected Bonuses
	bool* m_pabResearchingTech;
	bool* m_pabLoyalMember;
	// MOD - START - Player Build Caching
	bool* m_pabBuildCache;
	// MOD - END - Player Build Caching

	std::vector<EventTriggerTypes> m_triggersFired;

	CivicTypes* m_paeCivics;

	int** m_ppaaiSpecialistExtraYield;
	// MOD - START - Advanced Yield and Commerce Effects
	int** m_ppaaiSpecialistExtraCommerce;
	// MOD - END - Advanced Yield and Commerce Effects
	int** m_ppaaiImprovementYieldChange;

	// MOD - START - Advanced Yield and Commerce Effects
	std::vector<BuildingYieldChange> m_aBuildingYieldChange;
	std::vector<BuildingCommerceChange> m_aBuildingCommerceChange;
	// MOD - END - Advanced Yield and Commerce Effects

	CLinkList<int> m_groupCycle;

	CLinkList<TechTypes> m_researchQueue;

	CLinkList<CvWString> m_cityNames;

	// MOD - START - City Name Preservation
	std::map<XYCoords, CvWString> m_plotCityNames;
	// MOD - END - City Name Preservation

	FFreeListTrashArray<CvPlotGroup> m_plotGroups;

	FFreeListTrashArray<CvCityAI> m_cities;

	FFreeListTrashArray<CvUnitAI> m_units;

	FFreeListTrashArray<CvSelectionGroupAI> m_selectionGroups;

	FFreeListTrashArray<EventTriggeredData> m_eventsTriggered;
	CvEventMap m_mapEventsOccured;
	CvEventMap m_mapEventCountdown;
	UnitCombatPromotionArray m_aFreeUnitCombatPromotions;
	UnitClassPromotionArray m_aFreeUnitClassPromotions;

	std::vector< std::pair<int, PlayerVoteTypes> > m_aVote;
	std::vector< std::pair<UnitClassTypes, int> > m_aUnitExtraCosts;

	CvMessageQueue m_listGameMessages; 
	CvPopupQueue m_listPopups;
	CvDiploQueue m_listDiplomacy; 

	CvTurnScoreMap m_mapScoreHistory;
	CvTurnScoreMap m_mapEconomyHistory;
	CvTurnScoreMap m_mapIndustryHistory;
	CvTurnScoreMap m_mapAgricultureHistory;
	CvTurnScoreMap m_mapPowerHistory;
	CvTurnScoreMap m_mapCultureHistory;
	CvTurnScoreMap m_mapEspionageHistory;

	void doGold();
	void doResearch();
	void doEspionagePoints();
	void doWarnings();
	void doEvents();

	bool checkExpireEvent(EventTypes eEvent, const EventTriggeredData& kTriggeredData) const;
	void expireEvent(EventTypes eEvent, const EventTriggeredData& kTriggeredData, bool bFail);
	bool isValidTriggerReligion(const CvEventTriggerInfo& kTrigger, CvCity* pCity, ReligionTypes eReligion) const;
	bool isValidTriggerCorporation(const CvEventTriggerInfo& kTrigger, CvCity* pCity, CorporationTypes eCorporation) const;
	CvCity* pickTriggerCity(EventTriggerTypes eTrigger) const;
	CvUnit* pickTriggerUnit(EventTriggerTypes eTrigger, CvPlot* pPlot, bool bPickPlot) const;
	bool isValidEventTech(TechTypes eTech, EventTypes eEvent, PlayerTypes eOtherPlayer) const;

	void verifyGoldCommercePercent();

	void processCivics(CivicTypes eCivic, int iChange);

	// for serialization
	virtual void read(FDataStreamBase* pStream);
	virtual void write(FDataStreamBase* pStream);
	
	void doUpdateCacheOnTurn();
	int getResearchTurnsLeftTimes100(TechTypes eTech, bool bOverflow) const;

	void getTradeLayerColors(std::vector<NiColorA>& aColors, std::vector<CvPlotIndicatorData>& aIndicators) const;  // used by Globeview trade layer
	void getUnitLayerColors(GlobeLayerUnitOptionTypes eOption, std::vector<NiColorA>& aColors, std::vector<CvPlotIndicatorData>& aIndicators) const;  // used by Globeview unit layer
	void getResourceLayerColors(GlobeLayerResourceOptionTypes eOption, std::vector<NiColorA>& aColors, std::vector<CvPlotIndicatorData>& aIndicators) const;  // used by Globeview resource layer
	void getReligionLayerColors(ReligionTypes eSelectedReligion, std::vector<NiColorA>& aColors, std::vector<CvPlotIndicatorData>& aIndicators) const;  // used by Globeview religion layer
	void getCultureLayerColors(std::vector<NiColorA>& aColors, std::vector<CvPlotIndicatorData>& aIndicators) const;  // used by Globeview culture layer
};

#endif
