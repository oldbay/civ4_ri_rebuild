#pragma once

#ifndef CyGameCoreUtils_h
#define CyGameCoreUtils_h

//
// Python wrapper functions for DLL 
//

//#include "CvGameCoreDLL.h"
#include "CvDepends.h"
#include "CvEnums.h"
#include "CvStructs.h"

class CyCity;
class CyPlot;
class CyUnit;

int cyIntRange(int iNum, int iLow, int iHigh);
float cyFloatRange(float fNum, float fLow, float fHigh);
int cyDxWrap(int iDX);
int cyDyWrap(int iDY);
int cyPlotDistance(int iX1, int iY1, int iX2, int iY2);
int cyStepDistance(int iX1, int iY1, int iX2, int iY2);
CyPlot* cyPlotDirection(int iX, int iY, DirectionTypes eDirection);
CyPlot* cyPlotCardinalDirection(int iX, int iY, CardinalDirectionTypes eCardDirection);
CyPlot* cysPlotCardinalDirection(int iX, int iY, CardinalDirectionTypes eCardDirection);
CyPlot* cyPlotXY(int iX, int iY, int iDX, int iDY);
CyPlot* cysPlotXY(int iX, int iY, int iDX, int iDY);
DirectionTypes cyDirectionXYFromInt(int iDX, int iDY);
DirectionTypes cyDirectionXYFromPlot(CyPlot* pFromPlot, CyPlot* pToPlot);
CyPlot* cyPlotCity(int iX, int iY, int iIndex);
int cyPlotCityXYFromInt(int iDX, int iDY);
int cyPlotCityXYFromCity(CyCity* pCity, CyPlot* pPlot);
CardinalDirectionTypes cyGetOppositeCardinalDirection(CardinalDirectionTypes eDir);
DirectionTypes cyCardinalDirectionToDirection(CardinalDirectionTypes eCard);

bool cyIsCardinalDirection(DirectionTypes eDirection);
DirectionTypes cyEstimateDirection(int iDX, int iDY);

bool cyAtWar(int /*TeamTypes*/ eTeamA, int /*TeamTypes*/ eTeamB);
bool cyIsPotentialEnemy(int /*TeamTypes*/ eOurPlayer, int /*TeamTypes*/ eTheirPlayer);

CyCity* cyGetCity(IDInfo city);
CyUnit* cyGetUnit(IDInfo unit);

bool cyIsPromotionValid(int /*PromotionTypes*/ ePromotion, int /*UnitTypes*/ eUnit, bool bLeader);
int cyGetPopulationAsset(int iPopulation);
int cyGetLandPlotsAsset(int iLandPlots);
int cyGetPopulationPower(int iPopulation);
int cyGetPopulationScore(int iPopulation);
int cyGetLandPlotsScore(int iPopulation);
int cyGetTechScore(int /*TechTypes*/ eTech);
int cyGetWonderScore(int /*BuildingClassTypes*/ eWonderClass);
int /*ImprovementTypes*/ cyFinalImprovementUpgrade(int /*ImprovementTypes*/ eImprovement);

int cyGetWorldSizeMaxConscript(int /*CivicTypes*/ eCivic);

bool cyIsReligionTech(int /*TechTypes*/ eTech);

bool cyIsTechRequiredForUnit(int /*TechTypes*/ eTech, int /*UnitTypes*/ eUnit);
bool cyIsTechRequiredForBuilding(int /*TechTypes*/ eTech, int /*BuildingTypes*/ eBuilding);
bool cyIsTechRequiredForProject(int /*TechTypes*/ eTech, int /*ProjectTypes*/ eProject);
// MOD - START - Advanced Building Prerequisites (Civic)
bool cyIsCivicRequiredForBuilding(int /*CivicTypes*/ eCivic, int /*BuildingTypes*/ eBuilding);
// MOD - END - Advanced Building Prerequisites (Civic)
bool cyIsWorldUnitClass(int /*UnitClassTypes*/ eUnitClass);
bool cyIsTeamUnitClass(int /*UnitClassTypes*/ eUnitClass);
bool cyIsNationalUnitClass(int /*UnitClassTypes*/ eUnitClass);
bool cyIsLimitedUnitClass(int /*UnitClassTypes*/ eUnitClass);
bool cyIsWorldWonderClass(int /*BuildingClassTypes*/ eBuildingClass);
bool cyIsTeamWonderClass(int /*BuildingClassTypes*/ eBuildingClass);
bool cyIsNationalWonderClass(int /*BuildingClassTypes*/ eBuildingClass);
bool cyIsLimitedWonderClass(int /*BuildingClassTypes*/ eBuildingClass);
bool cyIsWorldProject(int /*ProjectTypes*/ eProject);
bool cyIsTeamProject(int /*ProjectTypes*/ eProject);
bool cyIsLimitedProject(int /*ProjectTypes*/ eProject);
int cyGetCombatOdds(CyUnit* pAttacker, CyUnit* pDefender);
int cyGetEspionageModifier(int /*TeamTypes*/ iOurTeam, int /*TeamTypes*/ iTargetTeam);

// MOD - START - Widget Data Packing
int cyPackByteID1(int iData1);
int cyPackByteID2(int iData1, int iData2);
int cyPackByteID3(int iData1, int iData2, int iData3);
int cyPackByteID4(int iData1, int iData2, int iData3, int iData4);

int cyPackWordID1(int iData1);
int cyPackWordID2(int iData1, int iData2);

int cyGetPackedByteID1(int iPackedData);
int cyGetPackedByteID2(int iPackedData);
int cyGetPackedByteID3(int iPackedData);
int cyGetPackedByteID4(int iPackedData);

int cyGetPackedWordID1(int iPackedData);
int cyGetPackedWordID2(int iPackedData);
// MOD - END - Widget Data Packing

// MOD - START - Improved Civilopedia
void cyDoPediaLink(std::wstring szLink);
// MOD - END - Improved Civilopedia

#endif	// CyGameCoreUtils_h
