#pragma once

// teamAI.h

#ifndef CIV4_TEAM_AI_H
#define CIV4_TEAM_AI_H

//#include "CvGameCoreDLL.h"
#include "CvTeam.h"
#include "FAssert.h"

class CvTeamAI : public CvTeam
{

public:

	CvTeamAI();
	virtual ~CvTeamAI();

#if (defined(__GNUC__) || defined(_USRDLL))
//#ifdef _USRDLL
	// inlined for performance reasons, only in the dll
	static CvTeamAI& getTeam(TeamTypes eTeam)
	{
		FAssertMsg(eTeam != NO_TEAM, "eTeam is not assigned a valid value");
		FAssertMsg(eTeam < MAX_TEAMS, "eTeam is not assigned a valid value");
		return m_aTeams[eTeam]; 
	}
#endif
	DllExport static CvTeamAI& getTeamNonInl(TeamTypes eTeam);

	static void initStatics();
	static void freeStatics();

	void AI_init();
	void AI_initMemory(); // K-Mod. (needs game map to be initialized first)
	void AI_uninit();
	void AI_reset(bool bConstructor);

	void AI_doTurnPre();
	void AI_doTurnPost();

	void AI_makeAssignWorkDirty();

	void AI_updateAreaStragies(bool bTargets = true);
	void AI_updateAreaTargets();

	int AI_countFinancialTrouble() const;
	int AI_countMilitaryWeight(CvArea* pArea) const;

	int AI_estimateTotalYieldRate(YieldTypes eYield) const; // K-Mod

	bool AI_deduceCitySite(const CvCity* pCity) const; // K-Mod

	bool AI_isAnyCapitalAreaAlone() const;
	bool AI_isPrimaryArea(CvArea* pArea) const;
	bool AI_hasCitiesInPrimaryArea(TeamTypes eTeam) const;
	bool AI_hasSharedPrimaryArea(TeamTypes eTeam) const; // K-Mod
	AreaAITypes AI_calculateAreaAIType(CvArea* pArea, bool bPreparingTotal = false) const;

	// MOD - START - Border Statistics Cache
	void AI_updateBorderStatistics();
	// MOD - END - Border Statistics Cache

	int AI_calculateAdjacentLandPlots(TeamTypes eTeam) const;
	int AI_calculateCapitalProximity(TeamTypes eTeam) const;
	int AI_calculatePlotWarValue(TeamTypes eTeam) const;
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      07/10/08                                jdog5000      */
/*                                                                                              */
/* General AI                                                                                   */
/************************************************************************************************/
	int AI_calculateBonusWarValue(TeamTypes eTeam) const;
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/

	bool AI_haveSeenCities(TeamTypes eTeam, bool bPrimaryAreaOnly = false, int iMinimum = 1) const; // K-Mod
	bool AI_isWarPossible() const;
	bool AI_isLandTarget(TeamTypes eTeam) const;
	bool AI_isAllyLandTarget(TeamTypes eTeam) const;
	bool AI_shareWar(TeamTypes eTeam) const;

	AttitudeTypes AI_getAttitude(TeamTypes eTeam, bool bForced = true) const;
	int AI_getAttitudeVal(TeamTypes eTeam, bool bForced = true) const;
	int AI_getMemoryCount(TeamTypes eTeam, MemoryTypes eMemory) const;

	int AI_chooseElection(const VoteSelectionData& kVoteSelectionData) const;

	// K-Mod
	int AI_warSpoilsValue(TeamTypes eTarget, WarPlanTypes eWarPlan) const;
	int AI_warCommitmentCost(TeamTypes eTarget, WarPlanTypes eWarPlan) const;
	int AI_warDiplomacyCost(TeamTypes eTarget) const;
	// K-Mod end

	//int AI_startWarVal(TeamTypes eTeam) const;
	int AI_startWarVal(TeamTypes eTarget, WarPlanTypes eWarPlan) const; // K-Mod
	int AI_endWarVal(TeamTypes eTeam) const;

    int AI_knownTechValModifier(TechTypes eTech) const; // K-Mod

	int AI_techTradeVal(TechTypes eTech, TeamTypes eTeam) const;
	DenialTypes AI_techTrade(TechTypes eTech, TeamTypes eTeam) const;

	int AI_mapTradeVal(TeamTypes eTeam) const;
	DenialTypes AI_mapTrade(TeamTypes eTeam) const;

	int AI_vassalTradeVal(TeamTypes eTeam) const;
	DenialTypes AI_vassalTrade(TeamTypes eTeam) const;

	int AI_surrenderTradeVal(TeamTypes eTeam) const;
	DenialTypes AI_surrenderTrade(TeamTypes eTeam, int iPowerMultiplier = 100) const;

	// bbai
	int AI_countMembersWithStrategy(int iStrategy) const; // K-Mod
	bool AI_isAnyMemberDoVictoryStrategy( int iVictoryStrategy ) const;
	bool AI_isAnyMemberDoVictoryStrategyLevel4() const;
	bool AI_isAnyMemberDoVictoryStrategyLevel3() const;

	int AI_getWarSuccessRating() const; // K-Mod

	int AI_getEnemyPowerPercent( bool bConsiderOthers = false ) const;
	int AI_getAirPower() const; // K-Mod
	int AI_getRivalAirPower( ) const;
	bool AI_refusePeace(TeamTypes ePeaceTeam) const; // K-Mod. (refuse peace when we need war for conquest victory.)
	bool AI_refuseWar(TeamTypes eWarTeam) const; // K-Mod. (is war an acceptable side effect for event choices, vassal deals, etc)
	bool AI_acceptSurrender( TeamTypes eSurrenderTeam ) const;
	bool AI_isOkayVassalTarget( TeamTypes eTeam ) const;

	void AI_getWarRands( int &iMaxWarRand, int &iLimitedWarRand, int &iDogpileWarRand ) const;
	void AI_getWarThresholds( int &iMaxWarThreshold, int &iLimitedWarThreshold, int &iDogpileWarThreshold ) const;
	int AI_getTotalWarOddsTimes100( ) const;
	// bbai end

	int AI_makePeaceTradeVal(TeamTypes ePeaceTeam, TeamTypes eTeam) const;
	DenialTypes AI_makePeaceTrade(TeamTypes ePeaceTeam, TeamTypes eTeam) const;

	int AI_declareWarTradeVal(TeamTypes eWarTeam, TeamTypes eTeam) const;
	DenialTypes AI_declareWarTrade(TeamTypes eWarTeam, TeamTypes eTeam, bool bConsiderPower = true) const;

	// AllTheLand AI
	float AI_openBordersPositive(TeamTypes eTeam) const;
	float AI_openBordersNegative(TeamTypes eTeam) const;
	// AllTheLand AI END
	DenialTypes AI_openBordersTrade(TeamTypes eTeam) const;

	int AI_defensivePactTradeVal(TeamTypes eTeam) const;
	DenialTypes AI_defensivePactTrade(TeamTypes eTeam) const;

	DenialTypes AI_permanentAllianceTrade(TeamTypes eTeam) const;

	TeamTypes AI_getWorstEnemy() const;
	void AI_updateWorstEnemy();

	int AI_getWarPlanStateCounter(TeamTypes eIndex) const;
	void AI_setWarPlanStateCounter(TeamTypes eIndex, int iNewValue);
	void AI_changeWarPlanStateCounter(TeamTypes eIndex, int iChange);

	int AI_getAtWarCounter(TeamTypes eIndex) const;
	void AI_setAtWarCounter(TeamTypes eIndex, int iNewValue);
	void AI_changeAtWarCounter(TeamTypes eIndex, int iChange);

	int AI_getAtPeaceCounter(TeamTypes eIndex) const;
	void AI_setAtPeaceCounter(TeamTypes eIndex, int iNewValue);
	void AI_changeAtPeaceCounter(TeamTypes eIndex, int iChange);

	int AI_getHasMetCounter(TeamTypes eIndex) const;
	void AI_setHasMetCounter(TeamTypes eIndex, int iNewValue);
	void AI_changeHasMetCounter(TeamTypes eIndex, int iChange);

	int AI_getOpenBordersCounter(TeamTypes eIndex) const;
	void AI_setOpenBordersCounter(TeamTypes eIndex, int iNewValue);
	void AI_changeOpenBordersCounter(TeamTypes eIndex, int iChange);

	int AI_getDefensivePactCounter(TeamTypes eIndex) const;
	void AI_setDefensivePactCounter(TeamTypes eIndex, int iNewValue);
	void AI_changeDefensivePactCounter(TeamTypes eIndex, int iChange);

	int AI_getShareWarCounter(TeamTypes eIndex) const;
	void AI_setShareWarCounter(TeamTypes eIndex, int iNewValue);
	void AI_changeShareWarCounter(TeamTypes eIndex, int iChange);

	int AI_getWarSuccess(TeamTypes eIndex) const;
	void AI_setWarSuccess(TeamTypes eIndex, int iNewValue);
	void AI_changeWarSuccess(TeamTypes eIndex, int iChange);

	int AI_getEnemyPeacetimeTradeValue(TeamTypes eIndex) const;
	void AI_setEnemyPeacetimeTradeValue(TeamTypes eIndex, int iNewValue);
	void AI_changeEnemyPeacetimeTradeValue(TeamTypes eIndex, int iChange);

	int AI_getEnemyPeacetimeGrantValue(TeamTypes eIndex) const;
	void AI_setEnemyPeacetimeGrantValue(TeamTypes eIndex, int iNewValue);
	void AI_changeEnemyPeacetimeGrantValue(TeamTypes eIndex, int iChange);

	// MOD - START - Border Statistics Cache
	void AI_changeAdjacentLandPlots(TeamTypes eIndex, int iChange);
	int AI_getAdjacentLandPlots(TeamTypes eIndex) const;
	void AI_setAdjacentLandPlots(TeamTypes eIndex, int iNewValue);
	// MOD - END - Border Statistics Cache

	WarPlanTypes AI_getWarPlan(TeamTypes eIndex) const;
	bool AI_isChosenWar(TeamTypes eIndex) const;
	bool AI_isSneakAttackPreparing(TeamTypes eIndex) const;
	bool AI_isSneakAttackReady(TeamTypes eIndex) const;
	bool AI_isSneakAttackReady() const; // K-Mod (any team)
	void AI_setWarPlan(TeamTypes eIndex, WarPlanTypes eNewValue, bool bWar = true);

	int AI_teamCloseness(TeamTypes eIndex, int iMaxDistance = -1) const;
	
	bool AI_performNoWarRolls(TeamTypes eTeam);
	
	int AI_getAttitudeWeight(TeamTypes eTeam) const;
	
	int AI_getLowestVictoryCountdown() const;

	int AI_getTechMonopolyValue(TechTypes eTech, TeamTypes eTeam) const;
	
	bool AI_isWaterAreaRelevant(CvArea* pArea);
	
	virtual void read(FDataStreamBase* pStream);
	virtual void write(FDataStreamBase* pStream);

	// K-Mod. Strength Memory - a very basic and rough reminder-map of how strong the enemy presence is on each plot.
public:
	int AI_getStrengthMemory(int x, int y) const;
	inline int AI_getStrengthMemory(const CvPlot* pPlot) { return AI_getStrengthMemory(pPlot->getX_INLINE(), pPlot->getY_INLINE()); }
	void AI_setStrengthMemory(int x, int y, int value);
	inline void AI_setStrengthMemory(const CvPlot* pPlot, int value) { AI_setStrengthMemory(pPlot->getX_INLINE(), pPlot->getY_INLINE(), value); }
protected:
	std::vector<int> m_aiStrengthMemory;
	void AI_updateStrengthMemory(); // exponentially dimishes memory, and clears obviously obsolete memory.
	// K-Mod end

protected:

	static CvTeamAI* m_aTeams;

	TeamTypes m_eWorstEnemy;

	int* m_aiWarPlanStateCounter;
	int* m_aiAtWarCounter;
	int* m_aiAtPeaceCounter;
	int* m_aiHasMetCounter;
	int* m_aiOpenBordersCounter;
	int* m_aiDefensivePactCounter;
	int* m_aiShareWarCounter;
	int* m_aiWarSuccess;
	int* m_aiEnemyPeacetimeTradeValue;
	int* m_aiEnemyPeacetimeGrantValue;
	// MOD - START - Border Statistics Cache
	int* m_aiAdjacentLandPlotsCache;
	// MOD - END - Border Statistics Cache

	WarPlanTypes* m_aeWarPlan;

	int AI_noTechTradeThreshold() const;
	int AI_techTradeKnownPercent() const;
	int AI_maxWarRand() const;
	int AI_maxWarNearbyPowerRatio() const;
	int AI_maxWarDistantPowerRatio() const;
	int AI_maxWarMinAdjacentLandPercent() const;
	int AI_limitedWarRand() const;
	int AI_limitedWarPowerRatio() const;
	int AI_dogpileWarRand() const;
	int AI_makePeaceRand() const;
	int AI_noWarAttitudeProb(AttitudeTypes eAttitude) const;

	void AI_doCounter();
	void AI_doWar();


	// added so under cheat mode we can call protected functions for testing
	friend class CvGameTextMgr;
	friend class CvDLLWidgetData;
};

// helper for accessing static functions
#if (defined(__GNUC__) || defined(_USRDLL))
//#ifdef _USRDLL
#define GET_TEAM CvTeamAI::getTeam
#else
#define GET_TEAM CvTeamAI::getTeamNonInl
#endif

#endif
