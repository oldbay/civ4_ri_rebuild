#pragma once

// city.h

#ifndef CIV4_CITY_H
#define CIV4_CITY_H

#include "CvDLLEntity.h"
#include "LinkedList.h"

class CvPlot;
class CvPlotGroup;
class CvArea;
class CvGenericBuilding;
class CvArtInfoBuilding;

/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      02/24/10                            EmperorFool       */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
// From BUG
void addGoodOrBad(int iValue, int& iGood, int& iBad);
void subtractGoodOrBad(int iValue, int& iGood, int& iBad);
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/

class CvCity : public CvDLLEntity
{

public:
	CvCity();
	virtual ~CvCity();

	// MOD - START - Captured City Highest Culture Art (DPII 2008-07-28)
	/*
	void init(int iID, PlayerTypes eOwner, int iX, int iY, bool bBumpUnits, bool bUpdatePlotGroups);
	*/
	void init(int iID, PlayerTypes eOwner, PlayerTypes eHighestCulturePlayer, int iX, int iY, bool bBumpUnits, bool bUpdatePlotGroups);
	// MOD - END - Captured City Highest Culture Art (DPII 2008-07-28)
	void uninit();
	void reset(int iID = 0, PlayerTypes eOwner = NO_PLAYER, int iX = 0, int iY = 0, bool bConstructorCall = false);
	void setupGraphical();

	void kill(bool bUpdatePlotGroups);																								// Exposed to Python

	void doTurn();

	bool isCitySelected();
	DllExport bool canBeSelected() const;
	DllExport void updateSelectedCity(bool bTestProduction);

	void updateYield();

	void updateVisibility();

	void createGreatPeople(UnitTypes eGreatPersonUnit, bool bIncrementThreshold, bool bIncrementExperience);		// Exposed to Python

	void doTask(TaskTypes eTask, int iData1 = -1, int iData2 = -1, bool bOption = false, bool bAlt = false, bool bShift = false, bool bCtrl = false);		// Exposed to Python

	void chooseProduction(UnitTypes eTrainUnit = NO_UNIT, BuildingTypes eConstructBuilding = NO_BUILDING, ProjectTypes eCreateProject = NO_PROJECT, bool bFinish = false, bool bFront = false);		// Exposed to Python

	int getCityPlotIndex(const CvPlot* pPlot) const;				// Exposed to Python 
	CvPlot* getCityIndexPlot(int iIndex) const;															// Exposed to Python

	bool canWork(CvPlot* pPlot) const;																			// Exposed to Python
	void verifyWorkingPlot(int iIndex);
	void verifyWorkingPlots();
	void clearWorkingOverride(int iIndex);														// Exposed to Python
	int countNumImprovedPlots(ImprovementTypes eImprovement = NO_IMPROVEMENT, bool bPotential = false) const;																			// Exposed to Python
	int countNumWaterPlots() const;																					// Exposed to Python
	int countNumRiverPlots() const;																					// Exposed to Python

	int findPopulationRank() const;																					// Exposed to Python
	int findBaseYieldRateRank(YieldTypes eYield) const;											// Exposed to Python
	int findYieldRateRank(YieldTypes eYield) const;								// Exposed to Python					
	int findCommerceRateRank(CommerceTypes eCommerce) const;			// Exposed to Python					

	UnitTypes allUpgradesAvailable(UnitTypes eUnit, int iUpgradeCount = 0) const;						// Exposed to Python
	bool isWorldWondersMaxed() const;																							// Exposed to Python
	bool isTeamWondersMaxed() const;																							// Exposed to Python
	bool isNationalWondersMaxed() const;																					// Exposed to Python
	bool isBuildingsMaxed() const;																								// Exposed to Python

	// MOD - START - Building Replacement
	bool isObsoleteBuilding(BuildingTypes eIndex) const;						// Exposed to Python
	// MOD - END - Building Replacement

	// MOD - START - Building Disablement
	bool isDisabledBuilding(BuildingTypes eIndex, bool bIgnorePower = false, bool bIgnoreReligion = false, bool bIgnoreCivic = false) const;						// Exposed to Python
	// MOD - END - Building Disablement

	bool canTrain(UnitTypes eUnit, bool bContinue = false, bool bTestVisible = false, bool bIgnoreCost = false, bool bIgnoreUpgrades = false) const;					// Exposed to Python 
	bool canTrain(UnitCombatTypes eUnitCombat) const;
	// MOD - START - Advanced Building Prerequisites (Civic)
	//bool canConstruct(BuildingTypes eBuilding, bool bContinue = false, bool bTestVisible = false, bool bIgnoreCost = false, bool bIgnoreTech = false) const; // Exposed to Python, K-Mod added bIgnoreTech
	bool canConstruct(BuildingTypes eBuilding, bool bContinue = false, bool bTestVisible = false, bool bIgnoreCost = false, bool bIgnoreTech = false, bool bIgnoreCivic = false) const; // Exposed to Python, K-Mod added bIgnoreTech
	// MOD - END - Advanced Building Prerequisites (Civic)
	bool canCreate(ProjectTypes eProject, bool bContinue = false, bool bTestVisible = false) const;		// Exposed to Python 
	bool canMaintain(ProcessTypes eProcess, bool bContinue = false) const;														// Exposed to Python  
	bool canJoin() const;																													// Exposed to Python

	int getFoodTurnsLeft() const;																				// Exposed to Python
	bool isProduction() const;																					// Exposed to Python
	bool isProductionLimited() const;																							// Exposed to Python
	bool isProductionUnit() const;																								// Exposed to Python
	bool isProductionBuilding() const;																						// Exposed to Python
	bool isProductionProject() const;																							// Exposed to Python
	bool isProductionProcess() const;																		// Exposed to Python

	bool canContinueProduction(OrderData order);														// Exposed to Python
	int getProductionExperience(UnitTypes eUnit = NO_UNIT) const;									// Exposed to Python
	void addProductionExperience(CvUnit* pUnit, bool bConscript = false);		// Exposed to Python

	UnitTypes getProductionUnit() const;																// Exposed to Python
	UnitAITypes getProductionUnitAI() const;																			// Exposed to Python
	BuildingTypes getProductionBuilding() const;												// Exposed to Python
	ProjectTypes getProductionProject() const;													// Exposed to Python
	ProcessTypes getProductionProcess() const;													// Exposed to Python
	const wchar* getProductionName() const;															// Exposed to Python
	const wchar* getProductionNameKey() const;													// Exposed to Python
	int getGeneralProductionTurnsLeft() const;										// Exposed to Python

	bool isFoodProduction() const;																								// Exposed to Python
	bool isFoodProduction(UnitTypes eUnit) const;																	// Exposed to Python
	int getFirstUnitOrder(UnitTypes eUnit) const;																	// Exposed to Python
	int getFirstBuildingOrder(BuildingTypes eBuilding) const;											// Exposed to Python
	int getFirstProjectOrder(ProjectTypes eProject) const;												// Exposed to Python
	int getNumTrainUnitAI(UnitAITypes eUnitAI) const;															// Exposed to Python

	int getProduction() const;																						// Exposed to Python
	int getProductionNeeded() const;																						// Exposed to Python
	int getProductionNeeded(UnitTypes eUnit) const;
	int getProductionNeeded(BuildingTypes eBuilding) const;
	int getProductionNeeded(ProjectTypes eProject) const;		
	int getProductionTurnsLeft() const;																	// Exposed to Python 
	int getProductionTurnsLeft(UnitTypes eUnit, int iNum) const;					// Exposed to Python
	int getProductionTurnsLeft(BuildingTypes eBuilding, int iNum) const;	// Exposed to Python
	int getProductionTurnsLeft(ProjectTypes eProject, int iNum) const;		// Exposed to Python
	int getProductionTurnsLeft(int iProductionNeeded, int iProduction, int iFirstProductionDifference, int iProductionDifference) const;
	void setProduction(int iNewValue);																			// Exposed to Python
	void changeProduction(int iChange);																			// Exposed to Python

	int getProductionModifier() const;																						// Exposed to Python
	int getProductionModifier(UnitTypes eUnit) const;															// Exposed to Python
	int getProductionModifier(BuildingTypes eBuilding) const;											// Exposed to Python
	int getProductionModifier(ProjectTypes eProject) const;												// Exposed to Python

	int getOverflowProductionDifference(int iProductionNeeded, int iProduction, int iProductionModifier, int iDiff, int iModifiedProduction) const;
	int getProductionDifference(int iProductionNeeded, int iProduction, int iProductionModifier, bool bFoodProduction, bool bOverflow) const;
	int getCurrentProductionDifference(bool bIgnoreFood, bool bOverflow) const;				// Exposed to Python
	int getExtraProductionDifference(int iExtra) const;																					// Exposed to Python

	bool canHurry(HurryTypes eHurry, bool bTestVisible = false) const;		// Exposed to Python
	void hurry(HurryTypes eHurry);																						// Exposed to Python

	UnitTypes getConscriptUnit() const;																// Exposed to Python
	CvUnit* initConscriptedUnit();
	int getConscriptPopulation() const;																// Exposed to Python
	int conscriptMinCityPopulation() const;																			// Exposed to Python
	int flatConscriptAngerLength() const;																				// Exposed to Python
	// MOD - START - Wartime Conscription
	//bool canConscript() const;																				// Exposed to Python
	bool canConscript(bool bWarConscript = false) const; // Exposed to Python
	// MOD - END - Wartime Conscription
	void conscript();																											// Exposed to Python

	int getBonusHealth(BonusTypes eBonus) const;																// Exposed to Python - getBonusHealth
	int getBonusHappiness(BonusTypes eBonus) const;															// Exposed to Python - getBonusHappiness
	// MOD - START - Epidemics
	// Epidemic system by Mexico
	int getBonusEpidemicModifier(BonusTypes eBonus) const;
	// MOD - END - Epidemics
	int getBonusPower(BonusTypes eBonus, bool bDirty) const;										// Exposed to Python 
	int getBonusYieldRateModifier(YieldTypes eIndex, BonusTypes eBonus) const;	// Exposed to Python 

	void processBonus(BonusTypes eBonus, int iChange);
	// MOD - START - Religion Affected Bonuses
	void processBonusHealthiness(BonusTypes eBonus, int iChange);
	void processBonusHappiness(BonusTypes eBonus, int iChange);
	// MOD - END - Religion Affected Bonuses
	// MOD - START - Building Activation
	//void processBuilding(BuildingTypes eBuilding, int iChange, bool bObsolete = false);
	void processBuildingExistence(BuildingTypes eBuilding, int iChange);
	void processBuildingActivation(BuildingTypes eBuilding, int iChange);
	// MOD - END - Building Activation
	void processProcess(ProcessTypes eProcess, int iChange);
	void processSpecialist(SpecialistTypes eSpecialist, int iChange);

	HandicapTypes getHandicapType() const;												// Exposed to Python
	CivilizationTypes getCivilizationType() const;								// Exposed to Python
	LeaderHeadTypes getPersonalityType() const;															// Exposed to Python
	DllExport ArtStyleTypes getArtStyleType() const;														// Exposed to Python
	// MOD - START - Captured City Highest Culture Art (DPII 2008-07-28)
	void setArtStyleType(ArtStyleTypes eArtStyle);
	// MOD - END - Captured City Highest Culture Art (DPII 2008-07-28)
	CitySizeTypes getCitySizeType() const;												// Exposed to Python
	DllExport const CvArtInfoBuilding* getBuildingArtInfo(BuildingTypes eBuilding) const;
	DllExport float getBuildingVisibilityPriority(BuildingTypes eBuilding) const;

	bool hasTrait(TraitTypes eTrait) const;																	// Exposed to Python
	bool isBarbarian() const;																								// Exposed to Python
	bool isHuman() const;																										// Exposed to Python
	DllExport bool isVisible(TeamTypes eTeam, bool bDebug) const;						// Exposed to Python

	bool isCapital() const;																				// Exposed to Python
	bool isCoastal(int iMinWaterSize) const;																									// Exposed to Python
	bool isDisorder() const;																			// Exposed to Python				 
	bool isHolyCity(ReligionTypes eIndex) const;									// Exposed to Python
	bool isHolyCity() const;																			// Exposed to Python
	// MOD - START - Holy City Migration
	int getHolyCityCandidateValue(ReligionTypes eReligion) const;
	bool isHolyCityCandidate(ReligionTypes eReligion) const; // Exposed to Python
	bool isHolyCityCandidate() const; // Exposed to Python
	// MOD - END - Holy City Migration
	bool isHeadquarters(CorporationTypes eIndex) const;									// Exposed to Python				
	bool isHeadquarters() const;																			// Exposed to Python				
	void setHeadquarters(CorporationTypes eIndex);

	int getOvercrowdingPercentAnger(int iExtra = 0) const;									// Exposed to Python
	int getNoMilitaryPercentAnger() const;																	// Exposed to Python 
	int getCulturePercentAnger() const;																			// Exposed to Python
	int getReligionPercentAnger() const;																		// Exposed to Python
	int getHurryPercentAnger(int iExtra = 0) const;																				// Exposed to Python
	int getConscriptPercentAnger(int iExtra = 0) const;																		// Exposed to Python
	int getDefyResolutionPercentAnger(int iExtra = 0) const;
	int getWarWearinessPercentAnger() const;																// Exposed to Python
	int getLargestCityHappiness() const;																		// Exposed to Python
	int getVassalHappiness() const;																		// Exposed to Python
	int getVassalUnhappiness() const;																		// Exposed to Python
	// MOD - START - Building Additional Happiness
	//int unhappyLevel(int iExtra = 0) const;																	// Exposed to Python
	int unhappyLevel(int iExtra = 0, bool bIgnoreNoUnhappiness = false) const;																	// Exposed to Python
	// MOD - END - Building Additional Happiness
	int happyLevel() const;																				// Exposed to Python				
	int angryPopulation(int iExtra = 0) const;										// Exposed to Python

	int visiblePopulation() const;
	int totalFreeSpecialists() const;															// Exposed to Python				 
	int extraPopulation() const;																						// Exposed to Python
	int extraSpecialists() const;																						// Exposed to Python
	int extraFreeSpecialists() const;																				// Exposed to Python

	int unhealthyPopulation(bool bNoAngry = false, int iExtra = 0) const;	// Exposed to Python
	int totalGoodBuildingHealth() const;																		// Exposed to Python
	// MOD - START - Building Additional Healthiness
	//int totalBadBuildingHealth() const;														// Exposed to Python
	int totalBadBuildingHealth(bool bIgnoreBuildingOnlyHealthy = false) const;														// Exposed to Python
	// MOD - END - Building Additional Healthiness
	int goodHealth() const;																				// Exposed to Python
	int badHealth(bool bNoAngry = false, int iExtra = 0) const;		// Exposed to Python
	int healthRate(bool bNoAngry = false, int iExtra = 0) const;	// Exposed to Python
	int foodConsumption(bool bNoAngry = false, int iExtra = 0) const;				// Exposed to Python
	int foodDifference(bool bBottom = true, bool bIgnoreProduction = false) const; // Exposed to Python, K-Mod added bIgnoreProduction
	int growthThreshold() const;																	// Exposed to Python

	int productionLeft() const;																							// Exposed to Python
	int hurryCost(bool bExtra) const;																				// Exposed to Python
	int getHurryCostModifier(bool bIgnoreNew = false) const;
	int hurryGold(HurryTypes eHurry) const;												// Exposed to Python
	int hurryPopulation(HurryTypes eHurry) const;									// Exposed to Python
	int hurryProduction(HurryTypes eHurry) const;														// Exposed to Python
	int flatHurryAngerLength() const;																				// Exposed to Python
	int hurryAngerLength(HurryTypes eHurry) const;													// Exposed to Python
	int maxHurryPopulation() const;																					// Exposed to Python

	int cultureDistance(int iDX, int iDY) const;														// Exposed to Python
	int cultureStrength(PlayerTypes ePlayer) const;								// Exposed to Python					 
	int cultureGarrison(PlayerTypes ePlayer) const;								// Exposed to Python					 
	int culturePressureFactor() const; // K-Mod
																																		
	int getNumBuilding(BuildingTypes eIndex) const;									// Exposed to Python					
	// MOD - START - Bonus Converters
	int getNumNonObsoleteBuilding(BuildingTypes eIndex) const;						// Exposed to Python
	// MOD - END - Bonus Converters
	int getNumActiveBuilding(BuildingTypes eIndex) const;						// Exposed to Python
	// MOD - START - Building Activation
	int calculateNumActiveBuilding(BuildingTypes eBuilding, bool bIgnorePower = false, bool bIgnoreReligion = false, bool bIgnoreCivic = false) const;
	void changeNumActiveBuilding(BuildingTypes eBuilding, int iChange);
	void updateNumActiveBuilding(BuildingTypes eBuilding);
	void updateNumActiveBuilding(std::vector<BuildingTypes>& aeBuildings);
	// MOD - END - Building Activation
	// MOD - START - City Messages
	int getNumActiveBuildingChange(BuildingTypes eBuilding) const;
	void changeNumActiveBuildingChange(BuildingTypes eBuilding, int iChange);
	void setNumActiveBuildingChange(BuildingTypes eBuilding, int iNewValue);
	// MOD - END - City Messages
	bool hasActiveWorldWonder() const;																			// Exposed to Python
/************************************************************************************************/
/* UNOFFICIAL_PATCH                       03/04/10                     Mongoose & jdog5000      */
/*                                                                                              */
/* Bugfix                                                                                       */
/************************************************************************************************/
	int getNumActiveWorldWonders() const;
/************************************************************************************************/
/* UNOFFICIAL_PATCH                        END                                                  */
/************************************************************************************************/

	int getReligionCount() const;																						// Exposed to Python  
	int getCorporationCount() const;																						// Exposed to Python  

	DllExport int getID() const;																			// Exposed to Python
	int getIndex() const;
	DllExport IDInfo getIDInfo() const;
	void setID(int iID);

	DllExport int getX() const;																			// Exposed to Python
#ifdef _USRDLL
	inline int getX_INLINE() const
	{
		return m_iX;
	}
#endif
	DllExport int getY() const;																			// Exposed to Python
#ifdef _USRDLL
	inline int getY_INLINE() const
	{
		return m_iY;
	}
#endif	
	bool at(int iX, int iY) const;																				// Exposed to Python
	bool at(CvPlot* pPlot) const;																					// Exposed to Python - atPlot
	DllExport CvPlot* plot() const;																	// Exposed to Python
	CvPlotGroup* plotGroup(PlayerTypes ePlayer) const;
	bool isConnectedTo(CvCity* pCity) const;															// Exposed to Python
	bool isConnectedToCapital(PlayerTypes ePlayer = NO_PLAYER) const;			// Exposed to Python
	int getArea() const;
	CvArea* area() const;																						// Exposed to Python
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      01/02/09                                jdog5000      */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
	CvArea* waterArea(bool bNoImpassable = false) const;																			// Exposed to Python
	// MOD - START - Multiple City Water Areas
	//CvArea* secondWaterArea(bool bNoImpassable = false) const;
	void waterAreas(CvArea* (&aWaterAreas)[MAX_CITY_WATER_AREAS], int &iNumWaterAreas, bool bNoImpassable = false, TeamTypes eRelevantTeam = NO_TEAM) const;
	// MOD - END - Multiple City Water Areas
	CvArea* sharedWaterArea(CvCity* pCity) const;
	bool isBlockaded() const;
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/

	CvPlot* getRallyPlot() const;																// Exposed to Python
	void setRallyPlot(CvPlot* pPlot);

	int getGameTurnFounded() const;																				// Exposed to Python
	void setGameTurnFounded(int iNewValue);

	int getGameTurnAcquired() const;																			// Exposed to Python
	void setGameTurnAcquired(int iNewValue);

	int getPopulation() const;														// Exposed to Python
	void setPopulation(int iNewValue);										// Exposed to Python
	void changePopulation(int iChange);										// Exposed to Python

	long getRealPopulation() const;																	// Exposed to Python

	int getHighestPopulation() const;																			// Exposed to Python 
	void setHighestPopulation(int iNewValue);

	int getWorkingPopulation() const;																			// Exposed to Python
	void changeWorkingPopulation(int iChange);														

	int getSpecialistPopulation() const;																	// Exposed to Python
	void changeSpecialistPopulation(int iChange);													

	int getNumGreatPeople() const;																				// Exposed to Python
	void changeNumGreatPeople(int iChange);															

	int getBaseGreatPeopleRate() const;																		// Exposed to Python
	long getGreatPeopleRate() const;																				// Exposed to Python
	int getTotalGreatPeopleRateModifier() const;													// Exposed to Python
	void changeBaseGreatPeopleRate(int iChange);										// Exposed to Python

	int getGreatPeopleRateModifier() const;																// Exposed to Python
	void changeGreatPeopleRateModifier(int iChange);

// BUG - Building Additional Great People - start
	int getAdditionalGreatPeopleRateByBuilding(BuildingTypes eBuilding) const;
	int getAdditionalBaseGreatPeopleRateByBuilding(BuildingTypes eBuilding) const;
	int getAdditionalGreatPeopleRateModifierByBuilding(BuildingTypes eBuilding) const;
// BUG - Building Additional Great People - end

// BUG - Specialist Additional Great People - start
	int getAdditionalGreatPeopleRateBySpecialist(SpecialistTypes eSpecialist, int iChange = 1) const;
	int getAdditionalBaseGreatPeopleRateBySpecialist(SpecialistTypes eSpecialist, int iChange = 1) const;
// BUG - Specialist Additional Great People - end

	int getGreatPeopleProgress() const;													// Exposed to Python
	void changeGreatPeopleProgress(int iChange);										// Exposed to Python

	int getNumWorldWonders() const;																				// Exposed to Python
	void changeNumWorldWonders(int iChange);

	int getNumTeamWonders() const;																				// Exposed to Python
	void changeNumTeamWonders(int iChange);

	int getNumNationalWonders() const;																		// Exposed to Python
	void changeNumNationalWonders(int iChange);

	int getNumBuildings() const;																					// Exposed to Python
	void changeNumBuildings(int iChange);

	int getGovernmentCenterCount() const;																	
	bool isGovernmentCenter() const;														// Exposed to Python
	void changeGovernmentCenterCount(int iChange);		

// BUG - Building Saved Maintenance - start
	int getSavedMaintenanceTimes100ByBuilding(BuildingTypes eBuilding) const;
// BUG - Building Saved Maintenance - end

	int getMaintenance() const;																	// Exposed to Python
	int getMaintenanceTimes100() const;																	// Exposed to Python
	void updateMaintenance();
	int calculateMaintenanceDistance() const; // K-Mod
	int calculateDistanceMaintenance() const;										// Exposed to Python
	int calculateNumCitiesMaintenance() const;									// Exposed to Python
	int calculateColonyMaintenance() const;									// Exposed to Python
	int calculateCorporationMaintenance() const;									// Exposed to Python
	int calculateDistanceMaintenanceTimes100() const;										// Exposed to Python
	int calculateNumCitiesMaintenanceTimes100() const;									// Exposed to Python
	int calculateColonyMaintenanceTimes100() const;									// Exposed to Python
	int calculateCorporationMaintenanceTimes100(CorporationTypes eCorporation) const;									// Exposed to Python
	int calculateCorporationMaintenanceTimes100() const;									// Exposed to Python
	int calculateBaseMaintenanceTimes100() const;
	int getMaintenanceModifier() const;													// Exposed to Python
	void changeMaintenanceModifier(int iChange);													

	int getWarWearinessModifier() const;																	// Exposed to Python
	void changeWarWearinessModifier(int iChange);													

	// MOD - START - Wartime Conscription
	int getWarConscript() const;
	void changeWarConscript(int iChange);
	// MOD - END - Wartime Conscription

	int getHurryAngerModifier() const;																	// Exposed to Python
	void changeHurryAngerModifier(int iChange);													

	int getHealRate() const;																							// Exposed to Python
	void changeHealRate(int iChange);

	int getEspionageHealthCounter() const;														// Exposed to Python
	void changeEspionageHealthCounter(int iChange);													// Exposed to Python

	int getEspionageHappinessCounter() const;														// Exposed to Python
	void changeEspionageHappinessCounter(int iChange);													// Exposed to Python

	int getFreshWaterGoodHealth() const;																	// Exposed to Python
	int getFreshWaterBadHealth() const;													// Exposed to Python
	void updateFreshWaterHealth();

	int getFeatureGoodHealth() const;																			// Exposed to Python
	int getFeatureBadHealth() const;														// Exposed to Python
	void updateFeatureHealth();

// BUG - Actual Effects - start
	int getAdditionalAngryPopuplation(int iGood, int iBad) const;
	int getAdditionalSpoiledFood(int iGood, int iBad) const;
	int getAdditionalStarvation(int iSpoiledFood) const;
// BUG - Actual Effects - end

	int getBuildingGoodHealth() const;																		// Exposed to Python
	int getBuildingBadHealth() const;																			// Exposed to Python
	int getBuildingHealth(BuildingTypes eBuilding) const;									// Exposed to Python
	int getBuildingGoodHealth(BuildingTypes eBuilding) const;
	int getBuildingBadHealth(BuildingTypes eBuilding) const;
	void changeBuildingGoodHealth(int iChange);
	void changeBuildingBadHealth(int iChange);

	int getPowerGoodHealth() const;																				// Exposed to Python 
	int getPowerBadHealth() const;															// Exposed to Python 
	void updatePowerHealth();

	// MOD - START - Advanced Building Prerequisites (Power)
	void updatePowerBuildings();
	// MOD - END - Advanced Building Prerequisites (Power)

	int getBonusGoodHealth() const;																				// Exposed to Python  
	int getBonusBadHealth() const;															// Exposed to Python 
	void changeBonusGoodHealth(int iChange);
	void changeBonusBadHealth(int iChange);

	int getMilitaryHappiness() const;																			// Exposed to Python
	int getMilitaryHappinessUnits() const;																// Exposed to Python
	void changeMilitaryHappinessUnits(int iChange);

	int getBuildingGoodHappiness() const;																	// Exposed to Python 
	int getBuildingBadHappiness() const;																	// Exposed to Python 
	int getBuildingHappiness(BuildingTypes eBuilding) const;							// Exposed to Python
	void changeBuildingGoodHappiness(int iChange);
	void changeBuildingBadHappiness(int iChange);

	int getExtraBuildingGoodHappiness() const;														// Exposed to Python
	int getExtraBuildingBadHappiness() const;															// Exposed to Python
	void updateExtraBuildingHappiness();

/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      02/24/10                            EmperorFool       */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
// From BUG
	int getAdditionalHappinessByBuilding(BuildingTypes eBuilding, int& iGood, int& iBad) const;
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/

	int getExtraBuildingGoodHealth() const;														// Exposed to Python
	int getExtraBuildingBadHealth() const;															// Exposed to Python
	void updateExtraBuildingHealth();

/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      02/24/10                            EmperorFool       */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
// From BUG
	int getAdditionalHealthByBuilding(BuildingTypes eBuilding, int& iGood, int& iBad) const;
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/
	// MOD - START - Epidemics
	int getAdditionalEpidemicModifierByBuilding(BuildingTypes eBuilding, int& iGood, int& iBad) const;
	// MOD - END - Epidemics

	int getFeatureGoodHappiness() const;																	// Exposed to Python
	int getFeatureBadHappiness() const;																		// Exposed to Python
	void updateFeatureHappiness();

	int getBonusGoodHappiness() const;																		// Exposed to Python  
	int getBonusBadHappiness() const;																			// Exposed to Python  
	void changeBonusGoodHappiness(int iChange);
	void changeBonusBadHappiness(int iChange);

	int getReligionGoodHappiness() const;																	// Exposed to Python
	int getReligionBadHappiness() const;																	// Exposed to Python
	int getReligionHappiness(ReligionTypes eReligion) const;							// Exposed to Python
	void updateReligionHappiness();

	int getExtraHappiness() const;																				// Exposed to Python
	void changeExtraHappiness(int iChange);													// Exposed to Python

	int getExtraHealth() const;																				// Exposed to Python
	void changeExtraHealth(int iChange);													// Exposed to Python

	int getHurryAngerTimer() const;																				// Exposed to Python
	void changeHurryAngerTimer(int iChange);												// Exposed to Python

	int getConscriptAngerTimer() const;																		// Exposed to Python
	void changeConscriptAngerTimer(int iChange);										// Exposed to Python

	int getDefyResolutionAngerTimer() const;																		// Exposed to Python
	void changeDefyResolutionAngerTimer(int iChange);										// Exposed to Python
	int flatDefyResolutionAngerLength() const;																				// Exposed to Python

	int getHappinessTimer() const;																				// Exposed to Python
	void changeHappinessTimer(int iChange);												// Exposed to Python

	int getNoUnhappinessCount() const;
	bool isNoUnhappiness() const;																					// Exposed to Python
	void changeNoUnhappinessCount(int iChange);

/*
** K-Mod, 27/dec/10, karadoc
** replaced NoUnhealthyPopulation with UnhealthyPopulationModifier
*/
	/* original bts code
	int getNoUnhealthyPopulationCount() const;
	bool isNoUnhealthyPopulation() const;																	// Exposed to Python
	void changeNoUnhealthyPopulationCount(int iChange); */
	int getUnhealthyPopulationModifier() const; // Exposed to Python
	void changeUnhealthyPopulationModifier(int iChange);
/*
** K-Mod end
*/

	int getBuildingOnlyHealthyCount() const;
	bool isBuildingOnlyHealthy() const;																		// Exposed to Python
	void changeBuildingOnlyHealthyCount(int iChange);

	int getFood() const;																				// Exposed to Python
	void setFood(int iNewValue);																		// Exposed to Python
	void changeFood(int iChange);																		// Exposed to Python

	int getFoodKept() const;																							// Exposed to Python
	void setFoodKept(int iNewValue);
	void changeFoodKept(int iChange);

	int getMaxFoodKeptPercent() const;																		// Exposed to Python
	void changeMaxFoodKeptPercent(int iChange);

	int getOverflowProduction() const;																		// Exposed to Python
	void setOverflowProduction(int iNewValue);											// Exposed to Python
	void changeOverflowProduction(int iChange, int iProductionModifier);

	int getFeatureProduction()const;																		// Exposed to Python
	void setFeatureProduction(int iNewValue);											// Exposed to Python
	void changeFeatureProduction(int iChange);

	int getMilitaryProductionModifier() const;														// Exposed to Python
	void changeMilitaryProductionModifier(int iChange);												

	int getSpaceProductionModifier() const;																// Exposed to Python
	void changeSpaceProductionModifier(int iChange);

	int getExtraTradeRoutes() const;											// Exposed to Python
	void changeExtraTradeRoutes(int iChange);									// Exposed to Python

	int getTradeRouteModifier() const;											// Exposed to Python
	void changeTradeRouteModifier(int iChange);

	int getForeignTradeRouteModifier() const;									// Exposed to Python
	void changeForeignTradeRouteModifier(int iChange);

/***
**** K-Mod, 26/sep/10, Karadoc
**** Trade culture calculation
***/
	int getTradeCultureRateTimes100(int iLevel) const;							// Exposed to Python
/*** end */

	int getBuildingDefense() const;												// Exposed to Python
	void changeBuildingDefense(int iChange);
	int getBuildingNaturalDefense() const;										
	void changeBuildingNaturalDefense(int iChange);	
// BUG - Building Additional Defense - start
	int getAdditionalDefenseByBuilding(BuildingTypes eBuilding) const;
// BUG - Building Additional Defense - start

	int getBuildingBombardDefense() const;										// Exposed to Python
	void changeBuildingBombardDefense(int iChange);

	int getFreeExperience() const;												// Exposed to Python
	void changeFreeExperience(int iChange);															

	int getCurrAirlift() const;													// Exposed to Python
	void setCurrAirlift(int iNewValue);
	void changeCurrAirlift(int iChange);

	int getMaxAirlift() const;													// Exposed to Python
	void changeMaxAirlift(int iChange);

	int getAirModifier() const;													// Exposed to Python
	void changeAirModifier(int iChange);

	int getAirUnitCapacity(TeamTypes eTeam) const;								// Exposed to Python
	void changeAirUnitCapacity(int iChange);									// Exposed to Python

	int getNukeModifier() const;												// Exposed to Python
	void changeNukeModifier(int iChange);

	int getFreeSpecialist() const;												// Exposed to Python  
	void changeFreeSpecialist(int iChange);

	int getPowerCount() const;
	bool isPower() const;														// Exposed to Python
	bool isAreaCleanPower() const;												// Exposed to Python
	int getDirtyPowerCount() const;
	bool isDirtyPower() const;													// Exposed to Python
	void changePowerCount(int iChange, bool bDirty);

	bool isAreaBorderObstacle() const;											// Exposed to Python

	int getDefenseDamage() const;												// Exposed to Python
	void changeDefenseDamage(int iChange);										// Exposed to Python
	void changeDefenseModifier(int iChange);									// Exposed to Python

	int getLastDefenseDamage() const;											// Exposed to Python
	void setLastDefenseDamage(int iNewValue);

	bool isBombardable(const CvUnit* pUnit) const;								// Exposed to Python
	int getNaturalDefense() const;												// Exposed to Python
	int getTotalDefense(bool bIgnoreBuilding) const;							// Exposed to Python
	int getDefenseModifier(bool bIgnoreBuilding) const;							// Exposed to Python

	int getOccupationTimer() const;												// Exposed to Python
	bool isOccupation() const;													// Exposed to Python 
	void setOccupationTimer(int iNewValue);										// Exposed to Python
	void changeOccupationTimer(int iChange);									// Exposed to Python

	int getCultureUpdateTimer() const;											// Exposed to Python
	void setCultureUpdateTimer(int iNewValue);
	void changeCultureUpdateTimer(int iChange);									// Exposed to Python

	int getCitySizeBoost() const;
	void setCitySizeBoost(int iBoost);

	bool isNeverLost() const;													// Exposed to Python
	void setNeverLost(bool bNewValue);											// Exposed to Python

	bool isBombarded() const;													// Exposed to Python
	void setBombarded(bool bNewValue);											// Exposed to Python

	bool isDrafted() const;														// Exposed to Python
	void setDrafted(bool bNewValue);											// Exposed to Python

	bool isAirliftTargeted() const;												// Exposed to Python
	void setAirliftTargeted(bool bNewValue);									// Exposed to Python

	bool isWeLoveTheKingDay() const;											// Exposed to Python 
	void setWeLoveTheKingDay(bool bNewValue);

	bool isCitizensAutomated() const;											// Exposed to Python 
	void setCitizensAutomated(bool bNewValue);									// Exposed to Python 

	bool isProductionAutomated() const;											// Exposed to Python
	void setProductionAutomated(bool bNewValue, bool bClear);					// Exposed to Python 

	/* allows you to programatically specify a cities walls rather than having them be generated automagically */
	DllExport bool isWallOverride() const; 
	void setWallOverride(bool bOverride);

	DllExport bool isInfoDirty() const;
	DllExport void setInfoDirty(bool bNewValue);

	DllExport bool isLayoutDirty() const;
	DllExport void setLayoutDirty(bool bNewValue);

	bool isPlundered() const;													// Exposed to Python
	void setPlundered(bool bNewValue);											// Exposed to Python

	DllExport PlayerTypes getOwner() const;										// Exposed to Python
#ifdef _USRDLL
	inline PlayerTypes getOwnerINLINE() const
	{
		return m_eOwner;
	}
#endif
	DllExport TeamTypes getTeam() const;										// Exposed to Python

	PlayerTypes getPreviousOwner() const;										// Exposed to Python
	void setPreviousOwner(PlayerTypes eNewValue);

	PlayerTypes getOriginalOwner() const;										// Exposed to Python
	void setOriginalOwner(PlayerTypes eNewValue);

	CultureLevelTypes getCultureLevel() const;									// Exposed to Python
	int getCultureThreshold() const;											// Exposed to Python
	static int getCultureThreshold(CultureLevelTypes eLevel);
	void setCultureLevel(CultureLevelTypes eNewValue, bool bUpdatePlotGroups);
	void updateCultureLevel(bool bUpdatePlotGroups);

	int getSeaPlotYield(YieldTypes eIndex) const;								// Exposed to Python
	void changeSeaPlotYield(YieldTypes eIndex, int iChange);

	int getRiverPlotYield(YieldTypes eIndex) const;								// Exposed to Python
	void changeRiverPlotYield(YieldTypes eIndex, int iChange);

// BUG - Building Additional Yield - start
	int getAdditionalYieldByBuilding(YieldTypes eIndex, BuildingTypes eBuilding) const;
	int getAdditionalBaseYieldRateByBuilding(YieldTypes eIndex, BuildingTypes eBuilding) const;
	int getAdditionalYieldRateModifierByBuilding(YieldTypes eIndex, BuildingTypes eBuilding) const;
// BUG - Building Additional Yield - end

// BUG - Specialist Additional Yield - start
	int getAdditionalYieldBySpecialist(YieldTypes eIndex, SpecialistTypes eSpecialist, int iChange = 1) const;
	int getAdditionalBaseYieldRateBySpecialist(YieldTypes eIndex, SpecialistTypes eSpecialist, int iChange = 1) const;
// BUG - Specialist Additional Yield - end

	// MOD - START - Negative Yield Acceptance
	int getActualBaseYieldRate(YieldTypes eIndex) const;															// Exposed to Python
	void setActualBaseYieldRate(YieldTypes eIndex, int iNewValue);
	void changeActualBaseYieldRate(YieldTypes eIndex, int iChange);
	// MOD - END - Negative Yield Acceptance
	int getBaseYieldRate(YieldTypes eIndex) const;								// Exposed to Python
	int getBaseYieldRateModifier(YieldTypes eIndex, int iExtra = 0) const;		// Exposed to Python
	int getYieldRate(YieldTypes eIndex) const;									// Exposed to Python
	void setBaseYieldRate(YieldTypes eIndex, int iNewValue);					// Exposed to Python
	// MOD - START - Negative Yield Acceptance
	//void changeBaseYieldRate(YieldTypes eIndex, int iChange);												// Exposed to Python
	// MOD - END - Negative Yield Acceptance

	int getYieldRateModifier(YieldTypes eIndex) const;							// Exposed to Python
	void changeYieldRateModifier(YieldTypes eIndex, int iChange);

	int getPowerYieldRateModifier(YieldTypes eIndex) const;						// Exposed to Python 
	void changePowerYieldRateModifier(YieldTypes eIndex, int iChange);

	int getBonusYieldRateModifier(YieldTypes eIndex) const;						// Exposed to Python 
	void changeBonusYieldRateModifier(YieldTypes eIndex, int iChange);

	int getTradeYield(YieldTypes eIndex) const;									// Exposed to Python
	int totalTradeModifier(CvCity* pOtherCity = NULL) const;					// Exposed to Python
	int getPopulationTradeModifier() const;
	int getPeaceTradeModifier(TeamTypes eTeam) const;
	int getBaseTradeProfit(CvCity* pCity) const;
	int calculateTradeProfitTimes100(CvCity* pCity) const;															// Exposed to Python
	int calculateTradeProfit(CvCity* pCity) const;								// Exposed to Python
	int calculateTradeYield(YieldTypes eIndex, int iTradeProfit) const;			// Exposed to Python
	void setTradeYield(YieldTypes eIndex, int iNewValue);

	// MOD - START - Advanced Yield and Commerce Effects
	/*
	int getExtraSpecialistYield(YieldTypes eIndex) const;						// Exposed to Python
	int getExtraSpecialistYield(YieldTypes eIndex, SpecialistTypes eSpecialist) const;// Exposed to Python
	void updateExtraSpecialistYield(YieldTypes eYield);
	void updateExtraSpecialistYield();
	*/

	int getYieldPerSpecialist(SpecialistTypes eSpecialist, YieldTypes eYield) const;
	int getSpecialistYield(YieldTypes eYield) const;						// Exposed to Python
	int getSpecialistYield(SpecialistTypes eSpecialist, YieldTypes eYield) const;// Exposed to Python
	void updateSpecialistYield(YieldTypes eYield);
	void updateSpecialistYield();

	int getSpecialistYieldChange(SpecialistTypes eSpecialist, YieldTypes eYield) const;
	void changeSpecialistYieldChange(SpecialistTypes eSpecialist, YieldTypes eYield, int iChange);
	void setSpecialistYieldChange(SpecialistTypes eSpecialist, YieldTypes eYield, int iChange);

	int getImprovementYieldChange(ImprovementTypes eImprovement, YieldTypes eYield) const;
	void changeImprovementYieldChange(ImprovementTypes eImprovement, YieldTypes eYield, int iChange);
	void setImprovementYieldChange(ImprovementTypes eImprovement, YieldTypes eYield, int iChange);

	/*
	int getLocalBuildingYield(ImprovementTypes eImprovement, YieldTypes eYield) const;
	void changeLocalBuildingYield(ImprovementTypes eImprovement, YieldTypes eYield, int iChange);

	int getLocalBuildingCommerce(BuildingClassTypes eBuildingClass, CommerceTypes eCommerce) const;
	void changeLocalBuildingCommerce(BuildingClassTypes eBuildingClass, CommerceTypes eCommerce, int iChange);
	*/
	// MOD - END - Advanced Yield and Commerce Effects

	int getCommerceRate(CommerceTypes eIndex) const;									// Exposed to Python
	int getCommerceRateTimes100(CommerceTypes eIndex) const;									// Exposed to Python
	int getCommerceFromPercent(CommerceTypes eIndex, int iYieldRate) const;			// Exposed to Python
	int getBaseCommerceRate(CommerceTypes eIndex) const;												// Exposed to Python
	int getDecimalTrade() const;
	int getBaseCommerceRateTimes100(CommerceTypes eIndex) const;												// Exposed to Python
	int getTotalCommerceRateModifier(CommerceTypes eIndex) const;								// Exposed to Python
	void updateCommerce(CommerceTypes eIndex);
	void updateCommerce();

	int getProductionToCommerceModifier(CommerceTypes eIndex) const;						// Exposed to Python
	void changeProductionToCommerceModifier(CommerceTypes eIndex, int iChange);

	int getBuildingCommerce(CommerceTypes eIndex) const;																				// Exposed to Python
	int getBuildingCommerceByBuilding(CommerceTypes eIndex, BuildingTypes eBuilding) const;			// Exposed to Python
	void updateBuildingCommerce();
// BUG - Building Additional Commerce - start
	int getAdditionalCommerceByBuilding(CommerceTypes eIndex, BuildingTypes eBuilding) const;
	int getAdditionalCommerceTimes100ByBuilding(CommerceTypes eIndex, BuildingTypes eBuilding) const;
	int getAdditionalBaseCommerceRateByBuilding(CommerceTypes eIndex, BuildingTypes eBuilding) const;
	int getAdditionalBaseCommerceRateByBuildingImpl(CommerceTypes eIndex, BuildingTypes eBuilding) const;
	int getAdditionalCommerceRateModifierByBuilding(CommerceTypes eIndex, BuildingTypes eBuilding) const;
	int getAdditionalCommerceRateModifierByBuildingImpl(CommerceTypes eIndex, BuildingTypes eBuilding) const;
// BUG - Building Additional Commerce - end

	// MOD - START - Advanced Yield and Commerce Effects
	//int getSpecialistCommerce(CommerceTypes eIndex) const;											// Exposed to Python
	//void changeSpecialistCommerce(CommerceTypes eIndex, int iChange);			// Exposed to Python
	int getCommercePerSpecialist(SpecialistTypes eSpecialist, CommerceTypes eCommerce) const;
	int getSpecialistCommerce(CommerceTypes eIndex) const; // Exposed to Python
	int getSpecialistCommerce(SpecialistTypes eSpecialist, CommerceTypes eCommerce) const;
	void updateSpecialistCommerce(CommerceTypes eCommerce);
	void updateSpecialistCommerce();

	int getSpecialistCommerceChange(SpecialistTypes eSpecialist, CommerceTypes eCommerce) const;
	void changeSpecialistCommerceChange(SpecialistTypes eSpecialist, CommerceTypes eCommerce, int iChange);
	void setSpecialistCommerceChange(SpecialistTypes eSpecialist, CommerceTypes eCommerce, int iChange);
	// MOD - END - Advanced Yield and Commerce Effects

// BUG - Specialist Additional Commerce - start
	int getAdditionalCommerceBySpecialist(CommerceTypes eIndex, SpecialistTypes eSpecialist, int iChange = 1) const;				// Exposed to Python
	int getAdditionalCommerceTimes100BySpecialist(CommerceTypes eIndex, SpecialistTypes eSpecialist, int iChange = 1) const;		// Exposed to Python
	int getAdditionalBaseCommerceRateBySpecialist(CommerceTypes eIndex, SpecialistTypes eSpecialist, int iChange = 1) const;		// Exposed to Python
	int getAdditionalBaseCommerceRateBySpecialistImpl(CommerceTypes eIndex, SpecialistTypes eSpecialist, int iChange = 1) const;
// BUG - Specialist Additional Commerce - end

	int getReligionCommerce(CommerceTypes eIndex) const;																				// Exposed to Python
	int getReligionCommerceByReligion(CommerceTypes eIndex, ReligionTypes eReligion) const;			// Exposed to Python
	void updateReligionCommerce(CommerceTypes eIndex);
	void updateReligionCommerce();

	void setCorporationYield(YieldTypes eIndex, int iNewValue);
	int getCorporationCommerce(CommerceTypes eIndex) const;																				// Exposed to Python
	int getCorporationCommerceByCorporation(CommerceTypes eIndex, CorporationTypes eCorporation) const;			// Exposed to Python
	int getCorporationYield(YieldTypes eIndex) const;																				// Exposed to Python
	int getCorporationYieldByCorporation(YieldTypes eIndex, CorporationTypes eCorporation) const;			// Exposed to Python
	void updateCorporation();
	void updateCorporationCommerce(CommerceTypes eIndex);
	void updateCorporationYield(YieldTypes eIndex);
	void updateCorporationBonus();

	int getCommerceRateModifier(CommerceTypes eIndex) const;										// Exposed to Python
	void changeCommerceRateModifier(CommerceTypes eIndex, int iChange);

	int getCommerceHappinessPer(CommerceTypes eIndex) const;										// Exposed to Python
	int getCommerceHappinessByType(CommerceTypes eIndex) const;									// Exposed to Python
	int getCommerceHappiness() const;																						// Exposed to Python
	void changeCommerceHappinessPer(CommerceTypes eIndex, int iChange);

	int getDomainFreeExperience(DomainTypes eIndex) const;											// Exposed to Python
	void changeDomainFreeExperience(DomainTypes eIndex, int iChange);

	int getDomainProductionModifier(DomainTypes eIndex) const;									// Exposed to Python
	void changeDomainProductionModifier(DomainTypes eIndex, int iChange);

	int getCulture(PlayerTypes eIndex) const;													// Exposed to Python
	int getCultureTimes100(PlayerTypes eIndex) const;													// Exposed to Python
	int countTotalCultureTimes100() const;																							// Exposed to Python
	PlayerTypes findHighestCulture() const;																			// Exposed to Python
	bool canCultureFlip(PlayerTypes eToPlayer) const; // K-Mod
	int calculateCulturePercent(PlayerTypes eIndex) const;											// Exposed to Python
	int calculateTeamCulturePercent(TeamTypes eIndex) const;										// Exposed to Python
	void setCulture(PlayerTypes eIndex, int iNewValue, bool bPlots, bool bUpdatePlotGroups);			// Exposed to Python
	void setCultureTimes100(PlayerTypes eIndex, int iNewValue, bool bPlots, bool bUpdatePlotGroups);			// Exposed to Python
	void changeCulture(PlayerTypes eIndex, int iChange, bool bPlots, bool bUpdatePlotGroups);		// Exposed to Python
	void changeCultureTimes100(PlayerTypes eIndex, int iChange, bool bPlots, bool bUpdatePlotGroups);		// Exposed to Python

	int getNumRevolts(PlayerTypes eIndex) const;
	void changeNumRevolts(PlayerTypes eIndex, int iChange);
	int getRevoltTestProbability() const;

	bool isTradeRoute(PlayerTypes eIndex) const;																	// Exposed to Python
	void setTradeRoute(PlayerTypes eIndex, bool bNewValue);

	bool isEverOwned(PlayerTypes eIndex) const;																		// Exposed to Python
	void setEverOwned(PlayerTypes eIndex, bool bNewValue);

	DllExport bool isRevealed(TeamTypes eIndex, bool bDebug) const;								// Exposed to Python
	void setRevealed(TeamTypes eIndex, bool bNewValue);											// Exposed to Python

	bool getEspionageVisibility(TeamTypes eTeam) const;								// Exposed to Python
	void setEspionageVisibility(TeamTypes eTeam, bool bVisible, bool bUpdatePlotGroups);
	void updateEspionageVisibility(bool bUpdatePlotGroups);

	DllExport const CvWString getName(uint uiForm = 0) const;								// Exposed to Python
	DllExport const wchar* getNameKey() const;															// Exposed to Python
	void setName(const wchar* szNewValue, bool bFound = false);		// Exposed to Python
	void doFoundMessage();

	// MOD - START - City Messages
	void updateMessages();
	// MOD - END - City Messages

	// Script data needs to be a narrow string for pickling in Python
	std::string getScriptData() const;																						// Exposed to Python
	void setScriptData(std::string szNewValue);															// Exposed to Python

	int getFreeBonus(BonusTypes eIndex) const;																		// Exposed to Python
	void changeFreeBonus(BonusTypes eIndex, int iChange);																		// Exposed to Python
	// MOD - START - PlotGroup Bonus Efficiency
	void changeFreeBonus(std::vector<CvBonusAmountChange>& aBonusChanges);
	// MOD - END - PlotGroup Bonus Efficiency
	// MOD - START - Obsolete City Free Bonus
	void updateFreeBonusDistribution(std::vector<BonusTypes>& aBonuses, bool bAdd);
	// MOD - END - Obsolete City Free Bonus

	int getNumBonuses(BonusTypes eIndex) const;																		// Exposed to Python
	bool hasBonus(BonusTypes eIndex) const;															// Exposed to Python
	void changeNumBonuses(BonusTypes eIndex, int iChange);

	int getNumCorpProducedBonuses(BonusTypes eIndex) const;
	bool isCorporationBonus(BonusTypes eBonus) const;
	bool isActiveCorporation(CorporationTypes eCorporation) const;

	int getBuildingProduction(BuildingTypes eIndex) const;							// Exposed to Python
	void setBuildingProduction(BuildingTypes eIndex, int iNewValue);				// Exposed to Python
	void changeBuildingProduction(BuildingTypes eIndex, int iChange);				// Exposed to Python

	int getBuildingProductionTime(BuildingTypes eIndex) const;										// Exposed to Python
	void setBuildingProductionTime(BuildingTypes eIndex, int iNewValue);		// Exposed to Python
	void changeBuildingProductionTime(BuildingTypes eIndex, int iChange);		// Exposed to Python

	int getProjectProduction(ProjectTypes eIndex) const;								// Exposed to Python
	void setProjectProduction(ProjectTypes eIndex, int iNewValue);					// Exposed to Python
	void changeProjectProduction(ProjectTypes eIndex, int iChange);					// Exposed to Python

	int getBuildingOriginalOwner(BuildingTypes eIndex) const;											// Exposed to Python
	int getBuildingOriginalTime(BuildingTypes eIndex) const;											// Exposed to Python

	// MOD - START - Building Replacement
	int getBuildingReplacementCount(BuildingTypes eIndex) const;				// Exposed to Python
	bool isBuildingReplaced(BuildingTypes eIndex) const;						// Exposed to Python
	void changeBuildingReplacementCount(BuildingTypes eIndex, int iChange);		// Exposed to Python
	// MOD - END - Building Replacement
	// MOD - START - Building Disablement
	int getBuildingDisablementCount(BuildingTypes eIndex) const;				// Exposed to Python
	void changeBuildingDisablementCount(BuildingTypes eIndex, int iChange);		// Exposed to Python
	// MOD - END - Building Disablement
	int getUnitProduction(UnitTypes eIndex) const;											// Exposed to Python
	void setUnitProduction(UnitTypes eIndex, int iNewValue);								// Exposed to Python
	void changeUnitProduction(UnitTypes eIndex, int iChange);								// Exposed to Python

	int getUnitProductionTime(UnitTypes eIndex) const;														// Exposed to Python
	void setUnitProductionTime(UnitTypes eIndex, int iNewValue);						// Exposed to Python
	void changeUnitProductionTime(UnitTypes eIndex, int iChange);						// Exposed to Python

	int getGreatPeopleUnitRate(UnitTypes eIndex) const;														// Exposed to Python
	void setGreatPeopleUnitRate(UnitTypes eIndex, int iNewValue);
	void changeGreatPeopleUnitRate(UnitTypes eIndex, int iChange);

	int getGreatPeopleUnitProgress(UnitTypes eIndex) const;							// Exposed to Python
	void setGreatPeopleUnitProgress(UnitTypes eIndex, int iNewValue);				// Exposed to Python
	void changeGreatPeopleUnitProgress(UnitTypes eIndex, int iChange);			// Exposed to Python

	int getSpecialistCount(SpecialistTypes eIndex) const;								// Exposed to Python
	void setSpecialistCount(SpecialistTypes eIndex, int iNewValue);
	void changeSpecialistCount(SpecialistTypes eIndex, int iChange);
	void alterSpecialistCount(SpecialistTypes eIndex, int iChange);					// Exposed to Python

	int getMaxSpecialistCount(SpecialistTypes eIndex) const;						// Exposed to Python
	bool isSpecialistValid(SpecialistTypes eIndex, int iExtra = 0) const;					// Exposed to Python
	void changeMaxSpecialistCount(SpecialistTypes eIndex, int iChange);

	int getForceSpecialistCount(SpecialistTypes eIndex) const;					// Exposed to Python
	bool isSpecialistForced() const;																							// Exposed to Python
	void setForceSpecialistCount(SpecialistTypes eIndex, int iNewValue);		// Exposed to Python
	void changeForceSpecialistCount(SpecialistTypes eIndex, int iChange);		// Exposed to Python

	int getFreeSpecialistCount(SpecialistTypes eIndex) const;					// Exposed to Python
	void setFreeSpecialistCount(SpecialistTypes eIndex, int iNewValue);			// Exposed to Python
	void changeFreeSpecialistCount(SpecialistTypes eIndex, int iChange);		// Exposed to Python
	int getAddedFreeSpecialistCount(SpecialistTypes eIndex) const;		// Exposed to Python

	int getImprovementFreeSpecialists(ImprovementTypes eIndex) const;			// Exposed to Python
	void changeImprovementFreeSpecialists(ImprovementTypes eIndex, int iChange);		// Exposed to Python

	int getReligionInfluence(ReligionTypes eIndex) const;													// Exposed to Python
	void changeReligionInfluence(ReligionTypes eIndex, int iChange);				// Exposed to Python

	int getCurrentStateReligionHappiness() const;																	// Exposed to Python
	int getStateReligionHappiness(ReligionTypes eIndex) const;										// Exposed to Python
	void changeStateReligionHappiness(ReligionTypes eIndex, int iChange);		// Exposed to Python

	int getUnitCombatFreeExperience(UnitCombatTypes eIndex) const;								// Exposed to Python
	void changeUnitCombatFreeExperience(UnitCombatTypes eIndex, int iChange);

	int getFreePromotionCount(PromotionTypes eIndex) const;												// Exposed to Python
	bool isFreePromotion(PromotionTypes eIndex) const;														// Exposed to Python
	void changeFreePromotionCount(PromotionTypes eIndex, int iChange);

	// MOD - START - Defense Modifiers
	int getUnitCombatDefenseModifier(UnitCombatTypes eIndex) const;
	void changeUnitCombatDefenseModifier(UnitCombatTypes eIndex, int iChange);

	int getUnitClassDefenseModifier(UnitClassTypes eIndex) const;
	void changeUnitClassDefenseModifier(UnitClassTypes eIndex, int iChange);
	// MOD - END - Defense Modifiers

	// MOD - START - Building Heal Rate
	int getDomainHealRate(DomainTypes eIndex) const;
	void changeDomainHealRate(DomainTypes eIndex, int iChange);

	int getUnitClassHealRate(UnitClassTypes eIndex) const;
	void changeUnitClassHealRate(UnitClassTypes eIndex, int iChange);

	int getUnitCombatHealRate(UnitCombatTypes eIndex) const;
	void changeUnitCombatHealRate(UnitCombatTypes eIndex, int iChange);
	// MOD - END - Building Heal Rate

	int getSpecialistFreeExperience() const;								// Exposed to Python
	void changeSpecialistFreeExperience(int iChange);

	int getEspionageDefenseModifier() const;										// Exposed to Python
	void changeEspionageDefenseModifier(int iChange);

	// MOD - START - Fresh Water Aquifers
	int getFreshWaterSourceCount() const;
	bool isFreshWaterSource() const;
	void changeFreshWaterSourceCount(int iChange);
	// MOD - END - Fresh Water Aquifers

	// MOD - START - Epidemics
	// Epidemic system by Mexico
	int getEpidemicTurns() const;
	bool isEpidemic() const;
	bool isImmune() const;
	void setEpidemicTurns(int iNewValue);
	void changeEpidemicTurns(int iChange);

	int getEpidemicProbabilityPercent(int iExtra = 0) const;
	int getEpidemicProbability(int iExtra = 0) const;
	// MOD - END - Epidemics

	// MOD - START - City Growth Limits
	bool isGrowthAllowed() const; // Exposed to Python
	// MOD - END - City Growth Limits

	bool isWorkingPlot(int iIndex) const;													// Exposed to Python
	bool isWorkingPlot(const CvPlot* pPlot) const;													// Exposed to Python
	void setWorkingPlot(int iIndex, bool bNewValue);
	void setWorkingPlot(CvPlot* pPlot, bool bNewValue);
	void alterWorkingPlot(int iIndex);																			// Exposed to Python

	int getNumRealBuilding(BuildingTypes eIndex) const;														// Exposed to Python
	void setNumRealBuilding(BuildingTypes eIndex, int iNewValue);		// Exposed to Python
	void setNumRealBuildingTimed(BuildingTypes eIndex, int iNewValue, bool bFirst, PlayerTypes eOriginalOwner, int iOriginalTime);

	bool isValidBuildingLocation(BuildingTypes eIndex) const;

	int getNumFreeBuilding(BuildingTypes eIndex) const;															// Exposed to Python
	void setNumFreeBuilding(BuildingTypes eIndex, int iNewValue);

	// MOD - START - Bonus Converters
	// TODO: Track the last time a building was disabled and use that as criteria for which converters get disabled
	void updateBonusConverters();
	void updateBonusConvertersConsumingBonus(BonusTypes eBonus);
	bool canConstructBonusConverter(BuildingTypes eBuilding) const;

	void setBonusConverterIsPresent(BuildingTypes eBuilding, bool bNewValue);
	void insertAtEndBonusConverters(BuildingTypes eBuilding);
	void deleteBonusConverterNode(CLLNode<BuildingTypes>* pNode);
	CLLNode<BuildingTypes>* nextBonusConverterNode(CLLNode<BuildingTypes>* pNode) const;
	CLLNode<BuildingTypes>* headBonusConverterNode() const;
	// MOD - END - Bonus Converters

	bool isHasReligion(ReligionTypes eIndex) const;
	void setHasReligion(ReligionTypes eIndex, bool bNewValue, bool bAnnounce, bool bArrows = true);
	int getReligionGrip(ReligionTypes eReligion) const; // K-Mod

	bool isHasCorporation(CorporationTypes eIndex) const;
	void setHasCorporation(CorporationTypes eIndex, bool bNewValue, bool bAnnounce, bool bArrows = true);

	CvCity* getTradeCity(int iIndex) const;																				// Exposed to Python
	int getTradeRoutes() const;																										// Exposed to Python
	void clearTradeRoutes();
	void updateTradeRoutes();

	void clearOrderQueue();																														// Exposed to Python
	//void pushOrder(OrderTypes eOrder, int iData1, int iData2, bool bSave, bool bPop, bool bAppend, bool bForce = false);		// Exposed to Python
	void pushOrder(OrderTypes eOrder, int iData1, int iData2 = -1, bool bSave = false, bool bPop = false, int iPosition = 0, bool bForce = false); // K-Mod. (the old version is still exposed to Python)
	void popOrder(int iNum, bool bFinish = false, bool bChoose = false);		// Exposed to Python
	void startHeadOrder();
	void stopHeadOrder();
	int getOrderQueueLength();																		// Exposed to Python
	OrderData* getOrderFromQueue(int iIndex);											// Exposed to Python
	CLLNode<OrderData>* nextOrderQueueNode(CLLNode<OrderData>* pNode) const;
	CLLNode<OrderData>* headOrderQueueNode() const;
	DllExport int getNumOrdersQueued() const;
	DllExport OrderData getOrderData(int iIndex) const;

	// fill the kVisible array with buildings that you want shown in city, as well as the number of generics
	// This function is called whenever CvCity::setLayoutDirty() is called
	DllExport void getVisibleBuildings(std::list<BuildingTypes>& kVisible, int& iNumGenerics);
	
	// Fill the kEffectNames array with references to effects in the CIV4EffectInfos.xml to have a
	// city play a given set of effects. This is called whenever the interface updates the city billboard
	// or when the zoom level changes
	DllExport void getVisibleEffects(ZoomLevelTypes eCurrentZoom, std::vector<const TCHAR*>& kEffectNames);


	// Billboard appearance controls
	DllExport void getCityBillboardSizeIconColors(NiColorA& kDotColor, NiColorA& kTextColor) const;
	DllExport const TCHAR* getCityBillboardProductionIcon() const;
	DllExport bool getFoodBarPercentages(std::vector<float>& afPercentages) const;
	DllExport bool getProductionBarPercentages(std::vector<float>& afPercentages) const;
	DllExport NiColorA getBarBackgroundColor() const;
	DllExport bool isStarCity() const;

	// Exposed to Python
	void setWallOverridePoints(const std::vector< std::pair<float, float> >& kPoints); /* points are given in world space ... i.e. PlotXToPointX, etc */
	DllExport const std::vector< std::pair<float, float> >& getWallOverridePoints() const;

	// MOD - START - Core Game Events
	void applyGameEvent(GameEventTypes eGameEvent, int iGameEventContext = -1);
	// MOD - END - Core Game Events

	// MOD - START - Population Metrics
	//SocialStabilityTypes getSocialStability() const;

	bool canApplyMetricFactor(MetricFactorTypes eFactor) const;
	void applyMetricFactor(MetricFactorTypes eFactor);

	int getMetricValue(MetricTypes eMetric) const; // Exposed to Python
	void setMetricValue(MetricTypes eMetric, int iValue); // Exposed to Python
	void changeMetricValue(MetricTypes eMetric, int iChange); // Exposed to Python

	void updateMetricTurnValues();
	int getMetricTurnValue(MetricTypes eMetric, int iTurn) const; // Exposed to Python
protected:
	void setMetricTurnValue(MetricTypes eMetric, int iTurn, int iValue);
	void changeMetricTurnValue(MetricTypes eMetric, int iTurn, int iChange);
public:
	int getMetricMinValue(MetricTypes eMetric) const;
	int getMetricMaxValue(MetricTypes eMetric) const;
	// MOD - END - Population Metrics

	int getTriggerValue(EventTriggerTypes eTrigger) const;
	bool canApplyEvent(EventTypes eEvent, const EventTriggeredData& kTriggeredData) const;
	void applyEvent(EventTypes eEvent, const EventTriggeredData& kTriggeredData, bool bClear);
	bool isEventOccured(EventTypes eEvent) const;
	void setEventOccured(EventTypes eEvent, bool bOccured);

	// MOD - START - Advanced Yield and Commerce Effects
	int getYieldPerBuilding(BuildingTypes eBuilding, YieldTypes eYield) const; // Exposed to Python
	void applyBuildingYieldChange(BuildingClassTypes eBuildingClass, YieldTypes eYield, int iChange);
	// MOD - END - Advanced Yield and Commerce Effects
	int getBuildingYieldChange(BuildingClassTypes eBuildingClass, YieldTypes eYield) const;           // Exposed to Python
	void setBuildingYieldChange(BuildingClassTypes eBuildingClass, YieldTypes eYield, int iChange);          // Exposed to Python
	void changeBuildingYieldChange(BuildingClassTypes eBuildingClass, YieldTypes eYield, int iChange);
	int getBuildingCommerceChange(BuildingClassTypes eBuildingClass, CommerceTypes eCommerce) const;           // Exposed to Python
	void setBuildingCommerceChange(BuildingClassTypes eBuildingClass, CommerceTypes eCommerce, int iChange);          // Exposed to Python
	void changeBuildingCommerceChange(BuildingClassTypes eBuildingClass, CommerceTypes eCommerce, int iChange);
	int getBuildingHappyChange(BuildingClassTypes eBuildingClass) const;           // Exposed to Python
	void setBuildingHappyChange(BuildingClassTypes eBuildingClass, int iChange);          // Exposed to Python
	int getBuildingHealthChange(BuildingClassTypes eBuildingClass) const;           // Exposed to Python
	void setBuildingHealthChange(BuildingClassTypes eBuildingClass, int iChange);          // Exposed to Python

	PlayerTypes getLiberationPlayer(bool bConquest) const;   // Exposed to Python
	void liberate(bool bConquest);    // Exposed to Python

	void changeNoBonusCount(BonusTypes eBonus, int iChange);   // Exposed to Python
	int getNoBonusCount(BonusTypes eBonus) const;
	bool isNoBonus(BonusTypes eBonus) const;   // Exposed to Python

	bool isAutoRaze() const;

	DllExport int getMusicScriptId() const;
	DllExport int getSoundscapeScriptId() const;
	DllExport void cheat(bool bCtrl, bool bAlt, bool bShift);

	DllExport void getBuildQueue(std::vector<std::string>& astrQueue) const;

	void read(FDataStreamBase* pStream);
	void write(FDataStreamBase* pStream);

	virtual void AI_init() = 0;
	virtual void AI_reset() = 0;
	virtual void AI_doTurn() = 0;
	virtual void AI_assignWorkingPlots() = 0;
	virtual void AI_updateAssignWork() = 0;
	//virtual bool AI_avoidGrowth() = 0; // disabled by K-Mod (was exposed to python)
	//virtual int AI_specialistValue(SpecialistTypes eSpecialist, bool bAvoidGrowth, bool bRemove) const = 0;
	virtual int AI_specialistValue(SpecialistTypes eSpecialist, bool bRemove, bool bIgnoreFood, int iGrowthValue) const = 0; // K-Mod
	virtual int AI_permanentSpecialistValue(SpecialistTypes eSpecialist) const = 0; // K-Mod
	// MOD - START - Advanced Yield and Commerce Effects
	virtual int AI_getPotentialSpecialistEstimate(SpecialistTypes eSpecialist) const = 0;
	// MOD - END - Advanced Yield and Commerce Effects
	virtual void AI_chooseProduction() = 0;
	virtual UnitTypes AI_bestUnit(bool bAsync = false, AdvisorTypes eIgnoreAdvisor = NO_ADVISOR, UnitAITypes* peBestUnitAI = NULL) = 0;
	// MOD - START - Free Units May Consume Food
	//virtual UnitTypes AI_bestUnitAI(UnitAITypes eUnitAI, bool bAsync = false, AdvisorTypes eIgnoreAdvisor = NO_ADVISOR) = 0;
	virtual UnitTypes AI_bestUnitAI(UnitAITypes eUnitAI, bool bAsync = false, AdvisorTypes eIgnoreAdvisor = NO_ADVISOR, bool bIgnoreUnitFoodConsumption = false) = 0;
	// MOD - END - Free Units May Consume Food

	virtual BuildingTypes AI_bestBuilding(int iFocusFlags = 0, int iMaxTurns = MAX_INT, bool bAsync = false, AdvisorTypes eIgnoreAdvisor = NO_ADVISOR) = 0;
	// MOD - START - Advanced Yield and Commerce Effects
	//virtual int AI_buildingValue(BuildingTypes eBuilding, int iFocusFlags = 0) const = 0;
	//virtual int AI_buildingValue(BuildingTypes eBuilding, int iFocusFlags = 0, int iThreshold = 0, bool bConstCache = false, bool bAllowRecursion = true) const = 0; // K-Mod
	// TODO: Change bAllowRecursion to iMaxRecursion and then track iRecursion
	virtual int AI_buildingValue(BuildingTypes eBuilding, int iFocusFlags = 0, int iThreshold = 0, bool bConstCache = false, bool bAllowRecursion = true, CivicTypes eCivic = NO_CIVIC) const = 0; // K-Mod
	// MOD - END - Advanced Yield and Commerce Effects
	virtual int AI_projectValue(ProjectTypes eProject) = 0;
	virtual int AI_neededSeaWorkers() = 0;
	virtual bool AI_isDefended(int iExtra = 0) = 0;
/********************************************************************************/
/**		BETTER_BTS_AI_MOD							9/19/08		jdog5000		*/
/**																				*/
/**		Air AI																	*/
/********************************************************************************/
/* original BTS code
	virtual bool AI_isAirDefended(int iExtra = 0) = 0;
*/
	virtual bool AI_isAirDefended(bool bCountLand = 0, int iExtra = 0) = 0;
/********************************************************************************/
/**		BETTER_BTS_AI_MOD						END								*/
/********************************************************************************/
	virtual bool AI_isDanger() const = 0;
	virtual int AI_neededDefenders() = 0;
	virtual int AI_neededAirDefenders() = 0;
	virtual int AI_minDefenders() = 0;
	virtual bool AI_isEmphasizeAvoidGrowth() const = 0;
	// MOD - START - City Growth Limits
	virtual bool AI_isEmphasizeAvoidUnhappy() const = 0;
	virtual bool AI_isEmphasizeAvoidUnhealth() const = 0;
	virtual bool AI_isEmphasizeAvoidEpidemics() const = 0;
	// MOD - END - City Growth Limits
	virtual bool AI_isAssignWorkDirty() const = 0;
	virtual CvCity* AI_getRouteToCity() const = 0;
	virtual void AI_setAssignWorkDirty(bool bNewValue) = 0;
	virtual bool AI_isChooseProductionDirty() const = 0;
	virtual void AI_setChooseProductionDirty(bool bNewValue) = 0;
	virtual bool AI_isEmphasize(EmphasizeTypes eIndex) const = 0;											// Exposed to Python
	virtual void AI_setEmphasize(EmphasizeTypes eIndex, bool bNewValue) = 0;
	virtual int AI_getBestBuildValue(int iIndex) = 0;
	// K-Mod
	virtual int AI_getTargetPopulation() const = 0;
	virtual int AI_countGoodPlots() const = 0;
	virtual void AI_getYieldMultipliers( int &iFoodMultiplier, int &iProductionMultiplier, int &iCommerceMultiplier, int &iDesiredFoodChange ) const = 0;
	virtual int AI_getImprovementValue(CvPlot* pPlot, ImprovementTypes eImprovement, int iFoodPriority, int iProductionPriority, int iCommercePriority, int iDesiredFoodChange, int iClearFeatureValue = 0, bool bEmphasizeIrrigation = false, BuildTypes* peBestBuild = 0) const = 0;
	// K-Mod end
	virtual int AI_totalBestBuildValue(CvArea* pArea) = 0;
	virtual int AI_countBestBuilds(CvArea* pArea) const = 0;													// Exposed to Python
	virtual BuildTypes AI_getBestBuild(int iIndex) const = 0;
	virtual void AI_updateBestBuild() = 0;
	virtual int AI_cityValue() const = 0;
	virtual int AI_clearFeatureValue(int iIndex) = 0;

	//virtual int AI_calculateCulturePressure(bool bGreatWork = false) const = 0; // disabled by K-Mod
	virtual int AI_calculateWaterWorldPercent() = 0;
	virtual int AI_countNumBonuses(BonusTypes eBonus, bool bIncludeOurs, bool bIncludeNeutral, int iOtherCultureThreshold, bool bLand = true, bool bWater = true) = 0;
	virtual int AI_yieldMultiplier(YieldTypes eYield) const = 0;
	virtual int AI_getCultureWeight() const = 0; // K-Mod
	virtual void AI_setCultureWeight(int iWeight) = 0; // K-Mod
	virtual int AI_playerCloseness(PlayerTypes eIndex, int iMaxDistance = 7) = 0;
	virtual int AI_highestTeamCloseness(TeamTypes eTeam) = 0; // K-Mod
	virtual int AI_cityThreat(bool bDangerPercent = false) = 0;
	virtual BuildingTypes AI_bestAdvancedStartBuilding(int iPass) = 0;
	
	virtual int AI_getWorkersHave() = 0;
	virtual int AI_getWorkersNeeded() = 0;
	virtual void AI_changeWorkersHave(int iChange) = 0;
	// MOD - START - Sea Improvements
	virtual int AI_getSeaWorkersHave() = 0;
	virtual int AI_getSeaWorkersNeeded() = 0;
	virtual void AI_changeSeaWorkersHave(int iChange) = 0;
	// MOD - END - Sea Improvements

	bool hasShrine(ReligionTypes eReligion) const;
	void processVoteSourceBonus(VoteSourceTypes eVoteSource, bool bActive);

	void invalidatePopulationRankCache();
	void invalidateYieldRankCache(YieldTypes eYield = NO_YIELD);
	void invalidateCommerceRankCache(CommerceTypes eCommerce = NO_COMMERCE);

	int getBestYieldAvailable(YieldTypes eYield) const;

protected:

	int m_iID;
	int m_iX;
	int m_iY;
	int m_iRallyX;
	int m_iRallyY;
	int m_iGameTurnFounded;
	int m_iGameTurnAcquired;
	int m_iPopulation;
	int m_iHighestPopulation;
	int m_iWorkingPopulation;
	int m_iSpecialistPopulation;
	int m_iNumGreatPeople;
	int m_iBaseGreatPeopleRate;
	int m_iGreatPeopleRateModifier;
	long m_iGreatPeopleProgress;
	int m_iNumWorldWonders;
	int m_iNumTeamWonders;
	int m_iNumNationalWonders;
	int m_iNumBuildings;
	int m_iGovernmentCenterCount;
	int m_iMaintenance;
	int m_iMaintenanceModifier;
	int m_iWarWearinessModifier;
	// MOD - START - Wartime Conscription
	int m_iWarConscript;
	// MOD - END - Wartime Conscription
	int m_iHurryAngerModifier;
	int m_iHealRate;
	int m_iEspionageHealthCounter;
	int m_iEspionageHappinessCounter;
	int m_iFreshWaterGoodHealth;
	int m_iFreshWaterBadHealth;
	int m_iFeatureGoodHealth;
	int m_iFeatureBadHealth;
	int m_iBuildingGoodHealth;
	int m_iBuildingBadHealth;
	int m_iPowerGoodHealth;
	int m_iPowerBadHealth;
	int m_iBonusGoodHealth;
	int m_iBonusBadHealth;
	int m_iHurryAngerTimer;
	int m_iConscriptAngerTimer;
	int m_iDefyResolutionAngerTimer;
	int m_iHappinessTimer;
	int m_iMilitaryHappinessUnits;
	int m_iBuildingGoodHappiness;
	int m_iBuildingBadHappiness;
	int m_iExtraBuildingGoodHappiness;
	int m_iExtraBuildingBadHappiness;
	int m_iExtraBuildingGoodHealth;
	int m_iExtraBuildingBadHealth;
	int m_iFeatureGoodHappiness;
	int m_iFeatureBadHappiness;
	int m_iBonusGoodHappiness;
	int m_iBonusBadHappiness;
	int m_iReligionGoodHappiness;
	int m_iReligionBadHappiness;
	int m_iExtraHappiness;
	int m_iExtraHealth;
	int m_iNoUnhappinessCount;
	//int m_iNoUnhealthyPopulationCount;
	int m_iUnhealthyPopulationModifier; // K-Mod
	int m_iBuildingOnlyHealthyCount;
	int m_iFood;
	int m_iFoodKept;
	int m_iMaxFoodKeptPercent;
	int m_iOverflowProduction;
	int m_iFeatureProduction;
	int m_iMilitaryProductionModifier;
	int m_iSpaceProductionModifier;
	int m_iExtraTradeRoutes;
	int m_iTradeRouteModifier;
	int m_iForeignTradeRouteModifier;
	int m_iBuildingDefense;
	int m_iBuildingNaturalDefense;	
	int m_iBuildingBombardDefense;
	int m_iFreeExperience;
	int m_iCurrAirlift;
	int m_iMaxAirlift;
	int m_iAirModifier;
	int m_iAirUnitCapacity;
	int m_iNukeModifier;
	int m_iFreeSpecialist;
	int m_iPowerCount;
	int m_iDirtyPowerCount;
	int m_iDefenseDamage;
	int m_iLastDefenseDamage;
	int m_iOccupationTimer;
	int m_iCultureUpdateTimer;
	int m_iCitySizeBoost;
	int m_iSpecialistFreeExperience;
	int m_iEspionageDefenseModifier;
	// MOD - START - Fresh Water Aquifers
	int m_iFreshWaterSourceCount;
	// MOD - END - Fresh Water Aquifers
	// MOD - START - Epidemics
	// Epidemic system by Mexico
	int m_iEpidemicTurns;
	// MOD - END - Epidemics
	int m_aDecimalTradeRouteCommerce;

	bool m_bNeverLost;
	bool m_bBombarded;
	bool m_bDrafted;
	bool m_bAirliftTargeted;
	bool m_bWeLoveTheKingDay;
	bool m_bCitizensAutomated;
	bool m_bProductionAutomated;
	bool m_bWallOverride;
	bool m_bInfoDirty;
	bool m_bLayoutDirty;
	bool m_bPlundered;

	PlayerTypes m_eOwner;
	PlayerTypes m_ePreviousOwner;
	PlayerTypes m_eOriginalOwner;
	CultureLevelTypes m_eCultureLevel;
	// MOD - START - Captured City Highest Culture Art (DPII 2008-07-28)
	ArtStyleTypes m_eArtStyleType;
	// MOD - END - Captured City Highest Culture Art (DPII 2008-07-28)

	int* m_aiSeaPlotYield;
	int* m_aiRiverPlotYield;
	// MOD - START - Negative Yield Acceptance
	int* m_aiActualBaseYieldRate;
	// MOD - END - Negative Yield Acceptance
	int* m_aiBaseYieldRate;
	int* m_aiYieldRateModifier;
	int* m_aiPowerYieldRateModifier;
	int* m_aiBonusYieldRateModifier;
	int* m_aiTradeYield;
	int* m_aiCorporationYield;
	// MOD - START - Advanced Yield and Commerce Effects
	//int* m_aiExtraSpecialistYield;
	int* m_aiSpecialistYield;
	// MOD - END - Advanced Yield and Commerce Effects
	int* m_aiCommerceRate;
	int* m_aiProductionToCommerceModifier;
	int* m_aiBuildingCommerce;
	int* m_aiSpecialistCommerce;
	int* m_aiReligionCommerce;
	int* m_aiCorporationCommerce;
	int* m_aiCommerceRateModifier;
	int* m_aiCommerceHappinessPer;
	int* m_aiDomainFreeExperience;
	int* m_aiDomainProductionModifier;
	int* m_aiCulture;
	int* m_aiNumRevolts;

	bool* m_abEverOwned;
	bool* m_abTradeRoute;
	bool* m_abRevealed;
	bool* m_abEspionageVisibility;

	CvWString m_szName;
	CvString m_szScriptData;

	int* m_paiNoBonus;
	int* m_paiFreeBonus;
	int* m_paiNumBonuses;
	int* m_paiNumCorpProducedBonuses;
	int* m_paiProjectProduction;
	int* m_paiBuildingProduction;
	int* m_paiBuildingProductionTime;
	int* m_paiBuildingOriginalOwner;
	int* m_paiBuildingOriginalTime;
	// MOD - START - Building Timer
	int* m_paiBuildingOriginalTurn;
	// MOD - END - Building Timer
	// MOD - START - Building Replacement
	int* m_paiBuildingReplacementCount;
	// MOD - END - Building Replacement
	// MOD - START - Building Disablement
	int* m_paiBuildingDisablementCount;
	// MOD - END - Building Disablement
	int* m_paiUnitProduction;
	int* m_paiUnitProductionTime;
	int* m_paiGreatPeopleUnitRate;
	int* m_paiGreatPeopleUnitProgress;
	int* m_paiSpecialistCount;
	int* m_paiMaxSpecialistCount;
	int* m_paiForceSpecialistCount;
	int* m_paiFreeSpecialistCount;
	int* m_paiImprovementFreeSpecialists;
	int* m_paiReligionInfluence;
	int* m_paiStateReligionHappiness;
	int* m_paiUnitCombatFreeExperience;
	int* m_paiFreePromotionCount;

	// MOD - START - Defense Modifiers
	int* m_paiUnitCombatDefenseModifier;
	int* m_paiUnitClassDefenseModifier;
	// MOD - END - Defense Modifiers

	// MOD - START - Building Heal Rate
	int* m_piDomainHealRate;
	int* m_piUnitClassHealRate;
	int* m_piUnitCombatHealRate;
	// MOD - END - Building Heal Rate

	int* m_paiNumRealBuilding;
	int* m_paiNumFreeBuilding;
	// MOD - START - Building Activation
	int* m_paiNumActiveBuilding;
	// MOD - END - Building Activation
	// MOD - START - City Messages
	int* m_paiNumActiveBuildingChange;
	// MOD - END - City Messages
	// MOD - START - Population Metrics
	int* m_paiMetricValues;
	// MOD - END - Population Metrics

	bool* m_pabWorkingPlot;
	bool* m_pabHasReligion;
	bool* m_pabHasCorporation;

	IDInfo* m_paTradeCities;

	mutable CLinkList<OrderData> m_orderQueue;

	// MOD - START - Bonus Converters
	CLinkList<BuildingTypes> m_bonusConverters;
	// MOD - END - Bonus Converters

	std::vector< std::pair < float, float> > m_kWallOverridePoints;

	// MOD - START - Population Metrics
	TurnValueMap<MetricTypes> m_kMetricTurnValues;
	// MOD - END - Population Metrics

	std::vector<EventTypes> m_aEventsOccured;
	// MOD - START - Advanced Yield and Commerce Effects
	std::vector<SpecialistYieldChange> m_aSpecialistYieldChange;
	std::vector<SpecialistCommerceChange> m_aSpecialistCommerceChange;
	std::vector<ImprovementYieldChange> m_aImprovementYieldChange;
	// MOD - END - Advanced Yield and Commerce Effects
	std::vector<BuildingYieldChange> m_aBuildingYieldChange;
	std::vector<BuildingCommerceChange> m_aBuildingCommerceChange;
	BuildingChangeArray m_aBuildingHappyChange;
	BuildingChangeArray m_aBuildingHealthChange;

	// CACHE: cache frequently used values
	mutable int	m_iPopulationRank;
	mutable bool m_bPopulationRankValid;
	int*	m_aiBaseYieldRank;
	bool*	m_abBaseYieldRankValid;
	int*	m_aiYieldRank;
	bool*	m_abYieldRankValid;
	int*	m_aiCommerceRank;
	bool*	m_abCommerceRankValid;

	void doGrowth();
	void doCulture();
	//void doPlotCulture(bool bUpdate, PlayerTypes ePlayer, int iCultureRate, bool bCityCulture = true);
	// K-Mod. I've made this function public so that I can use it for the "insert culture" espionage mission. (I've also changed the functionality of it quite a bit.)
public:
	void doPlotCultureTimes100(bool bUpdate, PlayerTypes ePlayer, int iCultureRateTimes100, bool bCityCulture);
	// MOD - START - Building Timer
	int getBuildingOriginalTurn(BuildingTypes eIndex) const;
	void doBuildingTimed();
	// MOD - END - Building Timer
	// MOD - Revolutions
	int getCitySeparatism() const;
	void changeCitySeparatism(int iChange);
	int getCitySeparatismTemp() const;
	void changeCitySeparatismTemp(int iChange);
	int getCitySeparatismUnit() const;
	void changeCitySeparatismUnit(int iChange);
	// MOD - Revolutions - END
protected:
	// K-Mod end
	bool doCheckProduction();
	void doProduction(bool bAllowNoProduction);
	void doDecay();
	void doReligion();
	void doGreatPeople();
	void doMeltdown();
	// MOD - START - Epidemics
	// Epidemic system by Mexico
	void doEpidemic();
	void doEndEpidemic();
	// MOD - END - Epidemics
	// MOD - Revolutions
	int m_iCitySeparatism;
	int m_iCitySeparatismTemp;
	int m_iCitySeparatismUnit;
	// MOD - Revolutions - END
	int getExtraProductionDifference(int iExtra, UnitTypes eUnit) const;
	int getExtraProductionDifference(int iExtra, BuildingTypes eBuilding) const;
	int getExtraProductionDifference(int iExtra, ProjectTypes eProject) const;
	int getExtraProductionDifference(int iExtra, int iModifier) const;
	int getHurryCostModifier(UnitTypes eUnit, bool bIgnoreNew) const;
	int getHurryCostModifier(BuildingTypes eBuilding, bool bIgnoreNew) const;
	int getHurryCostModifier(int iBaseModifier, int iProduction, bool bIgnoreNew) const;
	int getHurryCost(bool bExtra, UnitTypes eUnit, bool bIgnoreNew) const;
	int getHurryCost(bool bExtra, BuildingTypes eBuilding, bool bIgnoreNew) const;
	int getHurryCost(bool bExtra, int iProductionLeft, int iHurryModifier, int iModifier) const;
	int getHurryPopulation(HurryTypes eHurry, int iHurryCost) const;
	int getHurryGold(HurryTypes eHurry, int iHurryCost) const;
	bool canHurryUnit(HurryTypes eHurry, UnitTypes eUnit, bool bIgnoreNew) const;
	bool canHurryBuilding(HurryTypes eHurry, BuildingTypes eBuilding, bool bIgnoreNew) const;

	virtual bool AI_addBestCitizen(bool bWorkers, bool bSpecialists, int* piBestPlot = NULL, SpecialistTypes* peBestSpecialist = NULL) = 0;
	virtual bool AI_removeWorstCitizen(SpecialistTypes eIgnoreSpecialist = NO_SPECIALIST) = 0;
};

#endif
