//---------------------------------------------------------------------------------------
//
//  *****************   Civilization IV   ********************
//
//  FILE:    CvGameTextMgr.cpp
//
//  PURPOSE: Interfaces with GameText XML Files to manage the paths of art files
//
//---------------------------------------------------------------------------------------
//  Copyright (c) 2004 Firaxis Games, Inc. All rights reserved.
//---------------------------------------------------------------------------------------

#include "CvGameCoreDLL.h"
#include "CvGameTextMgr.h"
#include "CvGameCoreUtils.h"
#include "CvDLLUtilityIFaceBase.h"
#include "CvDLLInterfaceIFaceBase.h"
#include "CvDLLSymbolIFaceBase.h"
#include "CvInfos.h"
#include "CvXMLLoadUtility.h"
#include "CvCity.h"
#include "CvPlayerAI.h"
#include "CvTeamAI.h"
#include "CvGameAI.h"
#include "CvSelectionGroup.h"
#include "CvMap.h"
#include "CvArea.h"
#include "CvPlot.h"
#include "CvPopupInfo.h"
#include "FProfiler.h"
#include "CyArgsList.h"
#include "CvDLLPythonIFaceBase.h"
#include "CvBugOptions.h"

int shortenID(int iId)
{
	return iId;
}

// For displaying Asserts and error messages
static char* szErrorMsg;

//----------------------------------------------------------------------------
//
//	FUNCTION:	GetInstance()
//
//	PURPOSE:	Get the instance of this class.
//
//----------------------------------------------------------------------------
CvGameTextMgr& CvGameTextMgr::GetInstance()
{
	static CvGameTextMgr gs_GameTextMgr;
	return gs_GameTextMgr;
}

//----------------------------------------------------------------------------
//
//	FUNCTION:	CvGameTextMgr()
//
//	PURPOSE:	Constructor
//
//----------------------------------------------------------------------------
CvGameTextMgr::CvGameTextMgr()
{

}

CvGameTextMgr::~CvGameTextMgr()
{
}

//----------------------------------------------------------------------------
//
//	FUNCTION:	Initialize()
//
//	PURPOSE:	Allocates memory
//
//----------------------------------------------------------------------------
void CvGameTextMgr::Initialize()
{

}

//----------------------------------------------------------------------------
//
//	FUNCTION:	DeInitialize()
//
//	PURPOSE:	Clears memory
//
//----------------------------------------------------------------------------
void CvGameTextMgr::DeInitialize()
{
	for(int i=0;i<(int)m_apbPromotion.size();i++)
	{
		delete [] m_apbPromotion[i];
	}
}

//----------------------------------------------------------------------------
//
//	FUNCTION:	Reset()
//
//	PURPOSE:	Accesses CvXMLLoadUtility to clean global text memory and
//				reload the XML files
//
//----------------------------------------------------------------------------
void CvGameTextMgr::Reset()
{
	CvXMLLoadUtility pXML;
	pXML.LoadGlobalText();
}


// Returns the current language
int CvGameTextMgr::getCurrentLanguage()
{
	return gDLL->getCurrentLanguage();
}

void CvGameTextMgr::setYearStr(CvWString& szString, int iGameTurn, bool bSave, CalendarTypes eCalendar, int iStartYear, GameSpeedTypes eSpeed)
{
	int iTurnYear = getTurnYearForGame(iGameTurn, iStartYear, eCalendar, eSpeed);

	if (iTurnYear < 0)
	{
		if (bSave)
		{
			szString = gDLL->getText("TXT_KEY_TIME_BC_SAVE", CvWString::format(L"%04d", -iTurnYear).GetCString());
		}
		else
		{
			szString = gDLL->getText("TXT_KEY_TIME_BC", -(iTurnYear));
		}
	}
	else if (iTurnYear > 0)
	{
		if (bSave)
		{
			szString = gDLL->getText("TXT_KEY_TIME_AD_SAVE", CvWString::format(L"%04d", iTurnYear).GetCString());
		}
		else
		{
			szString = gDLL->getText("TXT_KEY_TIME_AD", iTurnYear);
		}
	}
	else
	{
		if (bSave)
		{
			szString = gDLL->getText("TXT_KEY_TIME_AD_SAVE", L"0001");
		}
		else
		{
			szString = gDLL->getText("TXT_KEY_TIME_AD", 1);
		}
	}
}


void CvGameTextMgr::setDateStr(CvWString& szString, int iGameTurn, bool bSave, CalendarTypes eCalendar, int iStartYear, GameSpeedTypes eSpeed)
{
	CvWString szYearBuffer;
	CvWString szWeekBuffer;

	setYearStr(szYearBuffer, iGameTurn, bSave, eCalendar, iStartYear, eSpeed);

	switch (eCalendar)
	{
	case CALENDAR_DEFAULT:
		if (0 == (getTurnMonthForGame(iGameTurn + 1, iStartYear, eCalendar, eSpeed) - getTurnMonthForGame(iGameTurn, iStartYear, eCalendar, eSpeed)) % GC.getNumMonthInfos())
		{
			szString = szYearBuffer;
		}
		else
		{
			int iMonth = getTurnMonthForGame(iGameTurn, iStartYear, eCalendar, eSpeed);
			if (bSave)
			{
				szString = (szYearBuffer + "-" + GC.getMonthInfo((MonthTypes)(iMonth % GC.getNumMonthInfos())).getDescription());
			}
			else
			{
				szString = (GC.getMonthInfo((MonthTypes)(iMonth % GC.getNumMonthInfos())).getDescription() + CvString(", ") + szYearBuffer);
			}
		}
		break;
	case CALENDAR_YEARS:
	case CALENDAR_BI_YEARLY:
		szString = szYearBuffer;
		break;

	case CALENDAR_TURNS:
		szString = gDLL->getText("TXT_KEY_TIME_TURN", (iGameTurn + 1));
		break;

	case CALENDAR_SEASONS:
		if (bSave)
		{
			szString = (szYearBuffer + "-" + GC.getSeasonInfo((SeasonTypes)(iGameTurn % GC.getNumSeasonInfos())).getDescription());
		}
		else
		{
			szString = (GC.getSeasonInfo((SeasonTypes)(iGameTurn % GC.getNumSeasonInfos())).getDescription() + CvString(", ") + szYearBuffer);
		}
		break;

	case CALENDAR_MONTHS:
		if (bSave)
		{
			szString = (szYearBuffer + "-" + GC.getMonthInfo((MonthTypes)(iGameTurn % GC.getNumMonthInfos())).getDescription());
		}
		else
		{
			szString = (GC.getMonthInfo((MonthTypes)(iGameTurn % GC.getNumMonthInfos())).getDescription() + CvString(", ") + szYearBuffer);
		}
		break;

	case CALENDAR_WEEKS:
		szWeekBuffer = gDLL->getText("TXT_KEY_TIME_WEEK", ((iGameTurn % GC.getDefineINT("WEEKS_PER_MONTHS")) + 1));

		if (bSave)
		{
			szString = (szYearBuffer + "-" + GC.getMonthInfo((MonthTypes)((iGameTurn / GC.getDefineINT("WEEKS_PER_MONTHS")) % GC.getNumMonthInfos())).getDescription() + "-" + szWeekBuffer);
		}
		else
		{
			szString = (szWeekBuffer + ", " + GC.getMonthInfo((MonthTypes)((iGameTurn / GC.getDefineINT("WEEKS_PER_MONTHS")) % GC.getNumMonthInfos())).getDescription() + ", " + szYearBuffer);
		}
		break;

	default:
		FAssert(false);
	}
}


void CvGameTextMgr::setTimeStr(CvWString& szString, int iGameTurn, bool bSave)
{
	setDateStr(szString, iGameTurn, bSave, GC.getGameINLINE().getCalendar(), GC.getGameINLINE().getStartYear(), GC.getGameINLINE().getGameSpeedType());
}


void CvGameTextMgr::setInterfaceTime(CvWString& szString, PlayerTypes ePlayer)
{
	CvWString szTempBuffer;

	if (GET_PLAYER(ePlayer).isGoldenAge())
	{
		szString.Format(L"%c(%d) ", gDLL->getSymbolID(GOLDEN_AGE_CHAR), GET_PLAYER(ePlayer).getGoldenAgeTurns());
	}
	else
	{
		szString.clear();
	}

	setTimeStr(szTempBuffer, GC.getGameINLINE().getGameTurn(), false);
	szString += CvWString(szTempBuffer);
}


void CvGameTextMgr::setGoldStr(CvWString& szString, PlayerTypes ePlayer)
{
	if (GET_PLAYER(ePlayer).getGold() < 0)
	{
		szString.Format(L"%c: " SETCOLR L"%d" SETCOLR, GC.getCommerceInfo(COMMERCE_GOLD).getChar(), TEXT_COLOR("COLOR_NEGATIVE_TEXT"), GET_PLAYER(ePlayer).getGold());
	}
	else
	{
		szString.Format(L"%c: %d", GC.getCommerceInfo(COMMERCE_GOLD).getChar(), GET_PLAYER(ePlayer).getGold());
	}

	int iGoldRate = GET_PLAYER(ePlayer).calculateGoldRate();
	if (iGoldRate < 0)
	{
		szString += gDLL->getText("TXT_KEY_MISC_NEG_GOLD_PER_TURN", iGoldRate);
	}
	else if (iGoldRate > 0)
	{
		szString += gDLL->getText("TXT_KEY_MISC_POS_GOLD_PER_TURN", iGoldRate);
	}

	if (GET_PLAYER(ePlayer).isStrike())
	{
		szString += gDLL->getText("TXT_KEY_MISC_STRIKE");
	}
}


void CvGameTextMgr::setResearchStr(CvWString& szString, PlayerTypes ePlayer)
{
	CvWString szTempBuffer;

	szString = gDLL->getText("TXT_KEY_MISC_RESEARCH_STRING", GC.getTechInfo(GET_PLAYER(ePlayer).getCurrentResearch()).getTextKeyWide());

	if (GET_TEAM(GET_PLAYER(ePlayer).getTeam()).getTechCount(GET_PLAYER(ePlayer).getCurrentResearch()) > 0)
	{
		szTempBuffer.Format(L" %d", (GET_TEAM(GET_PLAYER(ePlayer).getTeam()).getTechCount(GET_PLAYER(ePlayer).getCurrentResearch()) + 1));
		szString+=szTempBuffer;
	}

	szTempBuffer.Format(L" (%d)", GET_PLAYER(ePlayer).getResearchTurnsLeft(GET_PLAYER(ePlayer).getCurrentResearch(), true));
	szString+=szTempBuffer;
}


void CvGameTextMgr::setOOSSeeds(CvWString& szString, PlayerTypes ePlayer)
{
	if (GET_PLAYER(ePlayer).isHuman())
	{
		int iNetID = GET_PLAYER(ePlayer).getNetID();
		if (gDLL->isConnected(iNetID))
		{
			szString = gDLL->getText("TXT_KEY_PLAYER_OOS", gDLL->GetSyncOOS(iNetID), gDLL->GetOptionsOOS(iNetID));
		}
	}
}

void CvGameTextMgr::setNetStats(CvWString& szString, PlayerTypes ePlayer)
{
	if (ePlayer != GC.getGameINLINE().getActivePlayer())
	{
		if (GET_PLAYER(ePlayer).isHuman())
		{
			if (gDLL->getInterfaceIFace()->isNetStatsVisible())
			{
				int iNetID = GET_PLAYER(ePlayer).getNetID();
				if (gDLL->isConnected(iNetID))
				{
					szString = gDLL->getText("TXT_KEY_MISC_NUM_MS", gDLL->GetLastPing(iNetID));
				}
				else
				{
					szString = gDLL->getText("TXT_KEY_MISC_DISCONNECTED");
				}
			}
		}
		else
		{
			szString = gDLL->getText("TXT_KEY_MISC_AI");
		}
	}
}


void CvGameTextMgr::setMinimizePopupHelp(CvWString& szString, const CvPopupInfo & info)
{
	CvCity* pCity;
	UnitTypes eTrainUnit;
	BuildingTypes eConstructBuilding;
	ProjectTypes eCreateProject;
	ReligionTypes eReligion;
	CivicTypes eCivic;

	switch (info.getButtonPopupType())
	{
	case BUTTONPOPUP_CHOOSEPRODUCTION:
		pCity = GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getCity(info.getData1());
		if (pCity != NULL)
		{
			eTrainUnit = NO_UNIT;
			eConstructBuilding = NO_BUILDING;
			eCreateProject = NO_PROJECT;

			switch (info.getData2())
			{
			case (ORDER_TRAIN):
				eTrainUnit = (UnitTypes)info.getData3();
				break;
			case (ORDER_CONSTRUCT):
				eConstructBuilding = (BuildingTypes)info.getData3();
				break;
			case (ORDER_CREATE):
				eCreateProject = (ProjectTypes)info.getData3();
				break;
			default:
				break;
			}

			if (eTrainUnit != NO_UNIT)
			{
				szString += gDLL->getText("TXT_KEY_MINIMIZED_CHOOSE_PRODUCTION_UNIT", GC.getUnitInfo(eTrainUnit).getTextKeyWide(), pCity->getNameKey());
			}
			else if (eConstructBuilding != NO_BUILDING)
			{
				szString += gDLL->getText("TXT_KEY_MINIMIZED_CHOOSE_PRODUCTION_BUILDING", GC.getBuildingInfo(eConstructBuilding).getTextKeyWide(), pCity->getNameKey());
			}
			else if (eCreateProject != NO_PROJECT)
			{
				szString += gDLL->getText("TXT_KEY_MINIMIZED_CHOOSE_PRODUCTION_PROJECT", GC.getProjectInfo(eCreateProject).getTextKeyWide(), pCity->getNameKey());
			}
			else
			{
				szString += gDLL->getText("TXT_KEY_MINIMIZED_CHOOSE_PRODUCTION", pCity->getNameKey());
			}
		}
		break;

	case BUTTONPOPUP_CHANGERELIGION:
		eReligion = ((ReligionTypes)(info.getData1()));
		if (eReligion != NO_RELIGION)
		{
			szString += gDLL->getText("TXT_KEY_MINIMIZED_CHANGE_RELIGION", GC.getReligionInfo(eReligion).getTextKeyWide());
		}
		break;

	case BUTTONPOPUP_CHOOSETECH:
		if (info.getData1() > 0)
		{
			szString += gDLL->getText("TXT_KEY_MINIMIZED_CHOOSE_TECH_FREE");
		}
		else
		{
			szString += gDLL->getText("TXT_KEY_MINIMIZED_CHOOSE_TECH");
		}
		break;

	case BUTTONPOPUP_CHANGECIVIC:
		eCivic = ((CivicTypes)(info.getData2()));
		if (eCivic != NO_CIVIC)
		{
			szString += gDLL->getText("TXT_KEY_MINIMIZED_CHANGE_CIVIC", GC.getCivicInfo(eCivic).getTextKeyWide());
		}
		break;
	}
}

void CvGameTextMgr::setEspionageMissionHelp(CvWStringBuffer &szBuffer, const CvUnit* pUnit)
{
	if (pUnit->isSpy())
	{
		PlayerTypes eOwner =  pUnit->plot()->getOwnerINLINE();
		if (NO_PLAYER != eOwner && GET_PLAYER(eOwner).getTeam() != pUnit->getTeam())
		{
			if (!pUnit->canEspionage(pUnit->plot()))
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_UNIT_HELP_NO_ESPIONAGE"));

				if (pUnit->hasMoved() || pUnit->isMadeAttack())
				{
					szBuffer.append(gDLL->getText("TXT_KEY_UNIT_HELP_NO_ESPIONAGE_REASON_MOVED"));
				}
				else if (!pUnit->isInvisible(GET_PLAYER(eOwner).getTeam(), false))
				{
					szBuffer.append(gDLL->getText("TXT_KEY_UNIT_HELP_NO_ESPIONAGE_REASON_VISIBLE", GET_PLAYER(eOwner).getNameKey()));
				}
			}
			else if (pUnit->getFortifyTurns() > 0)
			{
				int iModifier = -(pUnit->getFortifyTurns() * GC.getDefineINT("ESPIONAGE_EACH_TURN_UNIT_COST_DECREASE"));
				if (0 != iModifier)
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_ESPIONAGE_COST", iModifier));
				}
			}
		}
	}
}


// MOD - START - Epidemics
// Epidemic system by Mexico
void CvGameTextMgr::setEpidemicHelp(CvWStringBuffer &szBuffer, CvCity& city)
{
	BuildingTypes eLoopBuilding;
	CvCity* pLoopCity;
	CvPlot* pLoopPlot;
	FeatureTypes eFeature;
	ImprovementTypes eImprovement;

	CvWString szTempBuffer;
	int iI;
	int iTemp;
	int iSum = 0;
	int iCount = 0;

	CvHandicapInfo& kHandicap = GC.getHandicapInfo(city.getHandicapType());

	int iRandBase = GC.getDefineINT("EPIDEMIC_RAND_BASE");

	// check for immunity
	if (city.isImmune())
	{
		szBuffer.append(gDLL->getText("TXT_KEY_EPIDEMIC_PROBABILITY_IMMUNE"));
		szBuffer.append(L"\n-----------------------\n");
	}
	// check for minimum turns
	else if (city.isEpidemic() && (GC.getDefineINT("EPIDEMIC_MIN_TURNS")!= -1) && (city.getEpidemicTurns() < GC.getDefineINT("EPIDEMIC_MIN_TURNS")))
	{
		szBuffer.append(gDLL->getText("TXT_KEY_HELP_EPIDEMIC_MIN_TURNS", GC.getDefineINT("EPIDEMIC_MIN_TURNS") - city.getEpidemicTurns() +1));
		szBuffer.append(L"\n-----------------------\n");
	}

	// check for maximum turns
	if (city.isEpidemic() && (GC.getDefineINT("EPIDEMIC_MAX_TURNS")!= -1) && (city.getEpidemicTurns() >= GC.getDefineINT("EPIDEMIC_MAX_TURNS")))
	{
		szBuffer.append(gDLL->getText("TXT_KEY_HELP_EPIDEMIC_MAX_TURNS"));
		szBuffer.append(L"\n-----------------------\n");
	}

	// base spread rate
	//iTemp = GC.getDefineINT("EPIDEMIC_BASE_RATE");
	iTemp = kHandicap.getEpidemicBaseRate();
	if (iTemp != 0)
	{
		iSum = iSum + iTemp;
		iCount++;
		szTempBuffer.Format(L"%d", iTemp*100/iRandBase);
		szBuffer.append(gDLL->getText("TXT_KEY_HELP_EPIDEMIC_BASE_RATE", szTempBuffer.GetCString()));
	}

	// population spread rate
	//iTemp = GC.getDefineINT("EPIDEMIC_POPULATION_RATE") * city.getPopulation();
	iTemp = kHandicap.getEpidemicPopulationRate() * city.getPopulation();
	if (iTemp != 0)
	{
		iSum = iSum + iTemp;
		iCount++;
		szTempBuffer.Format(L"%d", iTemp*100/iRandBase);
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_HELP_EPIDEMIC_POPULATION_RATE", szTempBuffer.GetCString()));
	}

	// each unhealth point increase chance
	// we must negate this, because healthRate return negative points
	//iTemp = (-(city.healthRate(false,0))) * GC.getDefineINT("EPIDEMIC_UNHEALTH_RATE");
	iTemp = (-(city.healthRate(false,0))) * kHandicap.getEpidemicUnhealthRate();
	if (iTemp != 0)
	{
		iSum = iSum + iTemp;
		iCount++;
		szTempBuffer.Format(L"%d", iTemp*100/iRandBase);
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_HELP_EPIDEMIC_UNHEALTH_RATE", szTempBuffer.GetCString()));
	}

	// buildings/wonders modifier
	iTemp = 0;
	for (iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
	{
		eLoopBuilding = ((BuildingTypes)(GC.getCivilizationInfo(city.getCivilizationType()).getCivilizationBuildings(iI)));
		if (eLoopBuilding != NO_BUILDING)
		{
			iTemp += city.getNumActiveBuilding(eLoopBuilding) * GC.getBuildingInfo(eLoopBuilding).getEpidemicModifier();
		}
	}
	if (iTemp != 0)
	{
		iSum = iSum + iTemp;
		iCount++;
		szTempBuffer.Format(L"%d", iTemp*100/iRandBase);
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_HELP_EPIDEMIC_BUILDINGS_MODIFIER", szTempBuffer.GetCString()));
	}

	// terrain features modifiers
	iTemp = 0;
	for (iI = 0; iI < NUM_CITY_PLOTS; iI++)
	{
		pLoopPlot = city.getCityIndexPlot(iI);
		if (pLoopPlot != NULL)
		{
			eFeature = pLoopPlot->getFeatureType();
			if (eFeature != NO_FEATURE)
			{
				iTemp += GC.getFeatureInfo(eFeature).getEpidemicModifier();
			}
		}
	}
	if (iTemp != 0)
	{
		iSum = iSum + iTemp;
		iCount++;
		szTempBuffer.Format(L"%d", iTemp*100/iRandBase);
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_HELP_EPIDEMIC_TERRAIN_MODIFIER", szTempBuffer.GetCString()));
	}

	// improvement modifiers
	iTemp = 0;
	for (iI = 0; iI < NUM_CITY_PLOTS; iI++)
	{
		pLoopPlot = city.getCityIndexPlot(iI);
		if (pLoopPlot != NULL)
		{
			eImprovement = pLoopPlot->getImprovementType();
			if (eImprovement != NO_IMPROVEMENT)
			{
				iTemp += GC.getImprovementInfo(eImprovement).getEpidemicModifier();
			}
		}
	}
	if (iTemp != 0)
	{
		iSum = iSum + iTemp;
		iCount++;
		szTempBuffer.Format(L"%d", iTemp*100/iRandBase);
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_HELP_EPIDEMIC_IMPROVEMENT_MODIFIER", szTempBuffer.GetCString()));
	}

	// bonuses (resources) modifiers
	iTemp = 0;
	for (iI = 0; iI < GC.getNumBonusInfos(); iI++)
	{
		if (city.hasBonus((BonusTypes)iI))
		{
			iTemp += city.getBonusEpidemicModifier((BonusTypes)iI);
		}
	}
	if (iTemp != 0)
	{
		iSum = iSum + iTemp;
		iCount++;
		szTempBuffer.Format(L"%d", iTemp*100/iRandBase);
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_HELP_EPIDEMIC_BONUS_MODIFIER", szTempBuffer.GetCString()));
	}

	// player global modifier (ie from trait/projects/tech/civic/wonder)
	iTemp = GET_PLAYER(city.getOwnerINLINE()).getGlobalEpidemicModifier() + GET_TEAM(city.getTeam()).getGlobalEpidemicModifier();
	if (iTemp != 0)
	{
		iSum = iSum + iTemp;
		iCount++;
		szTempBuffer.Format(L"%d", iTemp*100/iRandBase);
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_HELP_EPIDEMIC_PLAYER_MODIFIER", szTempBuffer.GetCString()));
	}

	// other modifications: previous turns, trade route, num of epidemic turns etc..
	iTemp = 0;
	for (iI = 0; iI < GC.getDefineINT("MAX_TRADE_ROUTES"); iI++)
	{
		pLoopCity = city.getTradeCity(iI);
		if (pLoopCity != NULL)
		{
			if(pLoopCity->isEpidemic())
			{
				//iTemp += GC.getDefineINT("EPIDEMIC_TRADE_RATE");
				iTemp += kHandicap.getEpidemicTradeRouteRate();
			}
		}
	}
	if (iTemp != 0)
	{
		iSum = iSum + iTemp;
		iCount++;
		szTempBuffer.Format(L"%d", iTemp*100/iRandBase);
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_HELP_EPIDEMIC_TRADE_MODIFIER", szTempBuffer.GetCString()));
	}

	iTemp = 0;
	if (city.isEpidemic())
	{
		if ( city.getEpidemicTurns() <  GC.getDefineINT("EPIDEMIC_TURN_IMMUNE"))
		{
			iTemp += GC.getDefineINT("EPIDEMIC_TURN_RATE_SPREAD");
		}
		else
		{
			iTemp -= GC.getDefineINT("EPIDEMIC_TURN_RATE_IMMUNIZE");
		}
	}
	if (iTemp != 0)
	{
		iSum = iSum + iTemp;
		iCount++;
		szTempBuffer.Format(L"%d", iTemp*100/iRandBase);
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_HELP_EPIDEMIC_OTHER_MODIFIER", szTempBuffer.GetCString()));
	}

	// total value of the above modifiers
	if (iCount != 0)
	{
		szTempBuffer.Format(L"%d", iSum*100/iRandBase);
		szBuffer.append(NEWLINE);
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_HELP_EPIDEMIC_TOTAL_MODIFIERS", szTempBuffer.GetCString()));
	}

	if ((iCount != 0) && (GC.getDefineINT("EPIDEMIC_RANDOM_ROUND") == 2))
	{

		float y = (float)iSum / iRandBase;
		float iProb = 0;
		
		if (y < 0) 
		{
			iProb = 0;
		} 
		else if (y <= 0.5) 
		{
			iProb = 200 * y * y;
		} 
		else if (y <= 1) 
		{
			iProb = (1 - (2 * y * y)) * 100;
		} 
		else 
		{
			iProb = 100;
		}
		
		szTempBuffer.Format(L"%.2f", iProb);
		szBuffer.append(NEWLINE);
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_HELP_EPIDEMIC_PROBABILITY", szTempBuffer.GetCString()));
	}

	if ((iCount != 0) && (GC.getDefineINT("EPIDEMIC_RANDOM_ROUND") == 1))
	{
		szTempBuffer.Format(L"%+.1f", iSum*100.0/iRandBase);
		szBuffer.append(NEWLINE);
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_HELP_EPIDEMIC_PROBABILITY", szTempBuffer.GetCString()));		
	}
	return;
}
// MOD - END - Epidemics

void CvGameTextMgr::setAheadHelp(CvWStringBuffer &szBuffer)
{
	CvWString szTempBuffer;
	int iYear = GC.getGameINLINE().getGameTurnYear();
	int iEra = GC.getGameINLINE().getAheadOfTimeEra();
	int iTurn = GC.getGameINLINE().getGameTurn();
	int iGoalTurn = iTurn;
	int iTurnDiff = 0;
	
	if (iYear < -2000)
	{
		int iGoal = GC.getGameINLINE().getTurnYear(iGoalTurn);
		if (iGoal < -2000)
		{
			while (GC.getGameINLINE().getTurnYear(iGoalTurn) < -2000)
			{
				iGoalTurn++;
			}
			iTurnDiff = iGoalTurn - iTurn;
		}
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_WIDGET_AHEAD_OF_TIME_CLAS_AT_100"));
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_WIDGET_AHEAD_OF_TIME_CLAS_50", iTurnDiff));
	}
	else if (iYear < -1200)
	{
		int iGoal = GC.getGameINLINE().getTurnYear(iGoalTurn);
		if (iGoal < -1200)
		{
			while (GC.getGameINLINE().getTurnYear(iGoalTurn) < -1200)
			{
				iGoalTurn++;
			}
			iTurnDiff = iGoalTurn - iTurn;
		}
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_WIDGET_AHEAD_OF_TIME_CLAS_AT_50"));
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_WIDGET_AHEAD_OF_TIME_CLAS_50", iTurnDiff));
	}
	else if (iYear < 0)
	{
		int iGoal = GC.getGameINLINE().getTurnYear(iGoalTurn);
		if (iGoal < 0)
		{
			while (GC.getGameINLINE().getTurnYear(iGoalTurn) < 0)
			{
				iGoalTurn++;
			}
			iTurnDiff = iGoalTurn - iTurn;
		}
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_WIDGET_AHEAD_OF_TIME_MED_AT_100"));
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_WIDGET_AHEAD_OF_TIME_CLAS_50", iTurnDiff));
	}
	else if (iYear < 400)
	{
		int iGoal = GC.getGameINLINE().getTurnYear(iGoalTurn);
		if (iGoal < 400)
		{
			while (GC.getGameINLINE().getTurnYear(iGoalTurn) < 400)
			{
				iGoalTurn++;
			}
			iTurnDiff = iGoalTurn - iTurn;
		}
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_WIDGET_AHEAD_OF_TIME_MED_AT_50"));
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_WIDGET_AHEAD_OF_TIME_CLAS_50", iTurnDiff));
	}
	else if (iYear < 1100)
	{
		int iGoal = GC.getGameINLINE().getTurnYear(iGoalTurn);
		if (iGoal < 1100)
		{
			while (GC.getGameINLINE().getTurnYear(iGoalTurn) < 1100)
			{
				iGoalTurn++;
			}
			iTurnDiff = iGoalTurn - iTurn;
		}
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_WIDGET_AHEAD_OF_TIME_REN_AT_100"));
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_WIDGET_AHEAD_OF_TIME_CLAS_50", iTurnDiff));
	}
	else if (iYear < 1400)
	{
		int iGoal = GC.getGameINLINE().getTurnYear(iGoalTurn);
		if (iGoal < 1400)
		{
			while (GC.getGameINLINE().getTurnYear(iGoalTurn) < 1400)
			{
				iGoalTurn++;
			}
			iTurnDiff = iGoalTurn - iTurn;
		}
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_WIDGET_AHEAD_OF_TIME_REN_AT_50"));
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_WIDGET_AHEAD_OF_TIME_CLAS_50", iTurnDiff));
	}
	else if (iYear < 1600)
	{
		int iGoal = GC.getGameINLINE().getTurnYear(iGoalTurn);
		if (iGoal < 1600)
		{
			while (GC.getGameINLINE().getTurnYear(iGoalTurn) < 1600)
			{
				iGoalTurn++;
			}
			iTurnDiff = iGoalTurn - iTurn;
		}
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_WIDGET_AHEAD_OF_TIME_IND_AT_100"));
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_WIDGET_AHEAD_OF_TIME_CLAS_50", iTurnDiff));
	}
	else if (iYear < 1750)
	{
		int iGoal = GC.getGameINLINE().getTurnYear(iGoalTurn);
		if (iGoal < 1750)
		{
			while (GC.getGameINLINE().getTurnYear(iGoalTurn) < 1750)
			{
				iGoalTurn++;
			}
			iTurnDiff = iGoalTurn - iTurn;
		}
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_WIDGET_AHEAD_OF_TIME_IND_AT_50"));
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_WIDGET_AHEAD_OF_TIME_CLAS_50", iTurnDiff));
	}
	else if (iYear < 1910)
	{
		int iGoal = GC.getGameINLINE().getTurnYear(iGoalTurn);
		if (iGoal < 1910)
		{
			while (GC.getGameINLINE().getTurnYear(iGoalTurn) < 1910)
			{
				iGoalTurn++;
			}
			iTurnDiff = iGoalTurn - iTurn;
		}
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_WIDGET_AHEAD_OF_TIME_MOD_AT_100"));
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_WIDGET_AHEAD_OF_TIME_CLAS_50", iTurnDiff));
	}
	else if (iYear < 1940)
	{
		int iGoal = GC.getGameINLINE().getTurnYear(iGoalTurn);
		if (iGoal < 1940)
		{
			while (GC.getGameINLINE().getTurnYear(iGoalTurn) < 1940)
			{
				iGoalTurn++;
			}
			iTurnDiff = iGoalTurn - iTurn;
		}
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_WIDGET_AHEAD_OF_TIME_MOD_AT_50"));
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_WIDGET_AHEAD_OF_TIME_CLAS_50", iTurnDiff));
	}
	else if (iYear > 1939)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_WIDGET_AHEAD_OF_TIME_NONE"));
	}
}
// MOD - START - Congestion
void CvGameTextMgr::setCongestionContextHelp(CvWStringBuffer &szBuffer, const CvPlot* pPlot, TeamTypes eTeam, DomainTypes eDomain)
{
	FAssert(pPlot != NULL);

	const bool bIsCity = pPlot->isCity(true, eTeam);

	std::vector<CongestionTypes> aRelevantCongestionTypes;
	getRelevantCongestionTypes(aRelevantCongestionTypes, eDomain, bIsCity);

	if (aRelevantCongestionTypes.size() == 0)
	{
		return;
	}

	const unsigned short iNumUnits = pPlot->getCombatUnitDomainCount(eTeam, eDomain);
	const int iThreshold = bIsCity ? GET_TEAM(eTeam).getInsideCityCongestionThreshold() : GET_TEAM(eTeam).getOutsideCityCongestionThreshold();

	if (bIsCity)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_CONGESTION_INSIDE_CITY"));
	}
	else
	{
		szBuffer.append(gDLL->getText("TXT_KEY_CONGESTION_OUTSIDE_CITY"));
	}

	szBuffer.append(DOUBLE_SEPARATOR);
	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_CONGESTION_DOMAIN_COMBAT_UNITS", GC.getDomainInfo(eDomain).getDescription(), iNumUnits));
	szBuffer.append(NEWLINE);
	szBuffer.append(bIsCity ? gDLL->getText("TXT_KEY_CONGESTION_INSIDE_CITY_THRESHOLD", iThreshold) : gDLL->getText("TXT_KEY_CONGESTION_OUTSIDE_CITY_THRESHOLD", iThreshold));
	szBuffer.append(DOUBLE_SEPARATOR);
	szBuffer.append(NEWLINE);

	bool bFirst = true;

	for (std::vector<CongestionTypes>::const_iterator it = aRelevantCongestionTypes.begin(); it != aRelevantCongestionTypes.end(); ++it)
	{
		if (bFirst)
		{
			bFirst = false;
		}
		else
		{
			szBuffer.append(NEWLINE);
		}

		setCongestionHelp(szBuffer, *it, pPlot, eTeam, false);
	}
}


void CvGameTextMgr::setCongestionPreviousLevelHelp(CvWStringBuffer &szBuffer, const CvPlot* pPlot, TeamTypes eTeam, DomainTypes eDomain)
{
	FAssert(pPlot != NULL);

	szBuffer.append(gDLL->getText("TXT_KEY_CONGESTION_PREVIOUS_LEVEL"));
	szBuffer.append(DOUBLE_SEPARATOR);
	szBuffer.append(NEWLINE);

	CongestionTypes eCongestion = getCongestionRelativeToCurrent(pPlot, eTeam, eDomain, -1);

	if (eCongestion != NO_CONGESTION)
	{
		setCongestionHelp(szBuffer, eCongestion, pPlot, eTeam, eDomain);
	}
	else
	{
		setNoCongestionHelp(szBuffer, pPlot, eTeam, eDomain);
	}
}


void CvGameTextMgr::setCongestionCurrentLevelHelp(CvWStringBuffer &szBuffer, const CvPlot* pPlot, TeamTypes eTeam, DomainTypes eDomain)
{
	FAssert(pPlot != NULL);

	szBuffer.append(gDLL->getText("TXT_KEY_CONGESTION_CURRENT_LEVEL"));
	szBuffer.append(DOUBLE_SEPARATOR);
	szBuffer.append(NEWLINE);

	CongestionTypes eCongestion = getCongestionRelativeToCurrent(pPlot, eTeam, eDomain, 0);

	if (eCongestion != NO_CONGESTION)
	{
		setCongestionHelp(szBuffer, eCongestion, pPlot, eTeam, eDomain);
	}
	else
	{
		setBelowMinimumCongestionHelp(szBuffer, pPlot, eTeam, eDomain);
	}
}


void CvGameTextMgr::setCongestionNextLevelHelp(CvWStringBuffer &szBuffer, const CvPlot* pPlot, TeamTypes eTeam, DomainTypes eDomain)
{
	FAssert(pPlot != NULL);

	szBuffer.append(gDLL->getText("TXT_KEY_CONGESTION_NEXT_LEVEL"));
	szBuffer.append(DOUBLE_SEPARATOR);
	szBuffer.append(NEWLINE);

	CongestionTypes eCongestion = getCongestionRelativeToCurrent(pPlot, eTeam, eDomain, 1);

	if (eCongestion != NO_CONGESTION)
	{
		setCongestionHelp(szBuffer, eCongestion, pPlot, eTeam, eDomain);
	}
	else
	{
		if (getCongestionRelativeToCurrent(pPlot, eTeam, eDomain, 0) != NO_CONGESTION)
		{
			setMaxCongestionHelp(szBuffer, pPlot, eTeam, eDomain);
		}
		else
		{
			setNoCongestionHelp(szBuffer, pPlot, eTeam, eDomain);
		}
	}
}


void CvGameTextMgr::setCongestionHelp(CvWStringBuffer &szBuffer, CongestionTypes eCongestion, const CvPlot* pPlot, TeamTypes eTeam, bool bEffects)
{
	FAssert(pPlot != NULL);

	const CvCongestionInfo& kCongestion = GC.getCongestionInfo(eCongestion);

	bool bIsCity = pPlot->isCity(true, eTeam);

	int iThreshold = 0;
	if (eTeam != NO_TEAM)
	{
		iThreshold = bIsCity ? GET_TEAM(eTeam).getInsideCityCongestionThreshold() : GET_TEAM(eTeam).getOutsideCityCongestionThreshold();
	}

	if (kCongestion.getPromotionType() != NO_PROMOTION)
	{
		const CvPromotionInfo& kPromotion = GC.getPromotionInfo(kCongestion.getPromotionType());

		int iMinUnits = kCongestion.getMinUnits() + iThreshold;
		int iMaxUnits = kCongestion.getMaxUnits() == MAX_UNSIGNED_SHORT ? MAX_UNSIGNED_SHORT : kCongestion.getMaxUnits() + iThreshold;

		CvWString szTempBuffer;
		szTempBuffer.Format(SETCOLR L"%s" ENDCOLR L" (", TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), kPromotion.getDescription());
		szBuffer.append(szTempBuffer);
		if (iMinUnits == iMaxUnits)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_CONGESTION_COMBAT_UNIT_RANGE_SINGLE", iMinUnits));
		}
		else if (iMaxUnits >= MAX_UNSIGNED_SHORT)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_CONGESTION_COMBAT_UNIT_RANGE_GREATER_THAN", iMinUnits - 1));
		}
		else
		{
			szBuffer.append(gDLL->getText("TXT_KEY_CONGESTION_COMBAT_UNIT_RANGE", iMinUnits, iMaxUnits));
		}
		szBuffer.append(L")");

		if (bEffects)
		{
			setPromotionHelp(szBuffer, kCongestion.getPromotionType(), true);
		}
	}
}


void CvGameTextMgr::setNoCongestionHelp(CvWStringBuffer &szBuffer, const CvPlot* pPlot, TeamTypes eTeam, DomainTypes eDomain)
{
	bool bIsCity = pPlot->isCity(true, eTeam);

	CvWString szTempBuffer;
	szTempBuffer.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), bIsCity ? gDLL->getText("TXT_KEY_CONGESTION_NONE_INSIDE_CITY").GetCString() : gDLL->getText("TXT_KEY_CONGESTION_NONE_OUTSIDE_CITY").GetCString());
	szBuffer.append(szTempBuffer);
}


void CvGameTextMgr::setBelowMinimumCongestionHelp(CvWStringBuffer &szBuffer, const CvPlot* pPlot, TeamTypes eTeam, DomainTypes eDomain)
{
	FAssert(pPlot != NULL);

	bool bIsCity = pPlot->isCity(true, eTeam);
	std::vector<CongestionTypes> aRelevantCongestionTypes;
	getRelevantCongestionTypes(aRelevantCongestionTypes, eDomain, bIsCity);

	CvWString szTempBuffer;
	szTempBuffer.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), bIsCity ? gDLL->getText("TXT_KEY_CONGESTION_BELOW_MINIMUM_INSIDE_CITY").GetCString() : gDLL->getText("TXT_KEY_CONGESTION_BELOW_MINIMUM_OUTSIDE_CITY").GetCString());
	szBuffer.append(szTempBuffer);

	if (aRelevantCongestionTypes.size() > 0)
	{
		int iThreshold = 0;
		if (eTeam != NO_TEAM)
		{
			iThreshold = bIsCity ? GET_TEAM(eTeam).getInsideCityCongestionThreshold() : GET_TEAM(eTeam).getOutsideCityCongestionThreshold();
		}

		int iMinUnits = 0;
		int iMaxUnits = GC.getCongestionInfo(aRelevantCongestionTypes[0]).getMinUnits() + iThreshold - 1;

		szBuffer.append(L" (");
		if (iMinUnits == iMaxUnits)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_CONGESTION_COMBAT_UNIT_RANGE_SINGLE", iMinUnits));
		}
		else if (iMaxUnits >= MAX_UNSIGNED_SHORT)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_CONGESTION_COMBAT_UNIT_RANGE_GREATER_THAN", iMinUnits - 1));
		}
		else
		{
			szBuffer.append(gDLL->getText("TXT_KEY_CONGESTION_COMBAT_UNIT_RANGE", iMinUnits, iMaxUnits));
		}
		szBuffer.append(L")");
	}
}


void CvGameTextMgr::setMaxCongestionHelp(CvWStringBuffer &szBuffer, const CvPlot* pPlot, TeamTypes eTeam, DomainTypes eDomain)
{
	FAssert(pPlot != NULL);

	bool bIsCity = pPlot->isCity(true, eTeam);
	std::vector<CongestionTypes> aRelevantCongestionTypes;
	getRelevantCongestionTypes(aRelevantCongestionTypes, eDomain, bIsCity);

	CvWString szTempBuffer;
	szTempBuffer.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR("COLOR_WARNING_TEXT"), bIsCity ? gDLL->getText("TXT_KEY_CONGESTION_MAX_INSIDE_CITY").GetCString() : gDLL->getText("TXT_KEY_CONGESTION_MAX_OUTSIDE_CITY").GetCString());
	szBuffer.append(szTempBuffer);

	if (aRelevantCongestionTypes.size() > 0)
	{
		int iThreshold = 0;
		if (eTeam != NO_TEAM)
		{
			iThreshold = bIsCity ? GET_TEAM(eTeam).getInsideCityCongestionThreshold() : GET_TEAM(eTeam).getOutsideCityCongestionThreshold();
		}

		int iMinUnits = GC.getCongestionInfo(aRelevantCongestionTypes[aRelevantCongestionTypes.size() - 1]).getMinUnits() + iThreshold;

		szBuffer.append(L" (");
		szBuffer.append(gDLL->getText("TXT_KEY_CONGESTION_COMBAT_UNIT_RANGE_GREATER_THAN", iMinUnits - 1));
		szBuffer.append(L")");
	}
}
// MOD - END - Congestion


// MOD - START - Population Metrics
void CvGameTextMgr::setCityStabilityHelp(CvWStringBuffer &szBuffer, CvCity& city)
{
	CvWString szTempBuffer;

	szBuffer.append(gDLL->getText("TXT_KEY_CITY_STABILITY"));
	szBuffer.append(DOUBLE_SEPARATOR);
	szBuffer.append(NEWLINE);
	szBuffer.append(L"TODO: Display quick population breakdown");

	// TODO: Display some sort of quick metric breakdown
}
// MOD - END - Population Metrics


void CvGameTextMgr::setUnitHelp(CvWStringBuffer &szString, const CvUnit* pUnit, bool bOneLine, bool bShort)
{
	PROFILE_FUNC();

	CvWString szTempBuffer;
	BuildTypes eBuild;
	int iCurrMoves;
	int iI;
	bool bFirst;
	bool bShift = GC.shiftKey();
	bool bAlt = GC.altKey();

	szTempBuffer.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR("COLOR_UNIT_TEXT"), pUnit->getName().GetCString());
	szString.append(szTempBuffer);

	szString.append(L", ");

	if (pUnit->getDomainType() == DOMAIN_AIR)
	{
		if (pUnit->airBaseCombatStr() > 0)
		{
			if (pUnit->isFighting())
			{
				szTempBuffer.Format(L"?/%d%c, ", pUnit->airBaseCombatStr(), gDLL->getSymbolID(STRENGTH_CHAR));
			}
			else if (pUnit->isHurt())
			{
				szTempBuffer.Format(L"%.1f/%d%c, ", (((float)(pUnit->airBaseCombatStr() * pUnit->currHitPoints())) / ((float)(pUnit->maxHitPoints()))), pUnit->airBaseCombatStr(), gDLL->getSymbolID(STRENGTH_CHAR));
			}
			else
			{
				szTempBuffer.Format(L"%d%c, ", pUnit->airBaseCombatStr(), gDLL->getSymbolID(STRENGTH_CHAR));
			}
			szString.append(szTempBuffer);
		}
	}
	else
	{
		if (pUnit->canFight())
		{
			if (pUnit->isFighting())
			{
				szTempBuffer.Format(L"?/%d%c, ", pUnit->baseCombatStr(), gDLL->getSymbolID(STRENGTH_CHAR));
			}
			else if (pUnit->isHurt())
			{
				szTempBuffer.Format(L"%.1f/%d%c, ", (((float)(pUnit->baseCombatStr() * pUnit->currHitPoints())) / ((float)(pUnit->maxHitPoints()))), pUnit->baseCombatStr(), gDLL->getSymbolID(STRENGTH_CHAR));
			}
			else
			{
				szTempBuffer.Format(L"%d%c, ", pUnit->baseCombatStr(), gDLL->getSymbolID(STRENGTH_CHAR));
			}
			szString.append(szTempBuffer);
		}
	}

	iCurrMoves = ((pUnit->movesLeft() / GC.getMOVE_DENOMINATOR()) + (((pUnit->movesLeft() % GC.getMOVE_DENOMINATOR()) > 0) ? 1 : 0));
	if ((pUnit->baseMoves() == iCurrMoves) || (pUnit->getTeam() != GC.getGameINLINE().getActiveTeam()))
	{
		szTempBuffer.Format(L"%d%c", pUnit->baseMoves(), gDLL->getSymbolID(MOVES_CHAR));
	}
	else
	{
		szTempBuffer.Format(L"%d/%d%c", iCurrMoves, pUnit->baseMoves(), gDLL->getSymbolID(MOVES_CHAR));
	}
	szString.append(szTempBuffer);

	if (pUnit->airRange() > 0)
	{
		szString.append(gDLL->getText("TXT_KEY_UNIT_HELP_AIR_RANGE", pUnit->airRange()));
	}

	eBuild = pUnit->getBuildType();

	if (eBuild != NO_BUILD)
	{
		szString.append(L", ");
		szTempBuffer.Format(L"%s (%d)", GC.getBuildInfo(eBuild).getDescription(), pUnit->plot()->getBuildTurnsLeft(eBuild, 0, 0));
		szString.append(szTempBuffer);
	}

	if (pUnit->getImmobileTimer() > 0)
	{
		szString.append(L", ");
		szString.append(gDLL->getText("TXT_KEY_UNIT_HELP_IMMOBILE", pUnit->getImmobileTimer()));
	}

	/*if (!bOneLine)
	{
		if (pUnit->getUnitCombatType() != NO_UNITCOMBAT)
		{
			szTempBuffer.Format(L" (%s)", GC.getUnitCombatInfo(pUnit->getUnitCombatType()).getDescription());
			szString += szTempBuffer;
		}
	}*/

	if (GC.getGameINLINE().isDebugMode() && !bAlt && !bShift)
	{
		FAssertMsg(pUnit->AI_getUnitAIType() != NO_UNITAI, "pUnit's AI type expected to != NO_UNITAI");
		szTempBuffer.Format(L" (%s)", GC.getUnitAIInfo(pUnit->AI_getUnitAIType()).getDescription());
		szString.append(szTempBuffer);
	}

	if ((pUnit->getTeam() == GC.getGameINLINE().getActiveTeam()) || GC.getGameINLINE().isDebugMode())
	{
		if ((pUnit->getExperience() > 0) && !(pUnit->isFighting()))
		{
			szString.append(gDLL->getText("TXT_KEY_UNIT_HELP_LEVEL", pUnit->getExperience(), pUnit->experienceNeeded()));
		}
	}

	if (pUnit->getOwnerINLINE() != GC.getGameINLINE().getActivePlayer() && !pUnit->isAnimal() && !pUnit->getUnitInfo().isHiddenNationality())
	{
		szString.append(L", ");
		szTempBuffer.Format(SETCOLR L"%s" ENDCOLR, GET_PLAYER(pUnit->getOwnerINLINE()).getPlayerTextColorR(), GET_PLAYER(pUnit->getOwnerINLINE()).getPlayerTextColorG(), GET_PLAYER(pUnit->getOwnerINLINE()).getPlayerTextColorB(), GET_PLAYER(pUnit->getOwnerINLINE()).getPlayerTextColorA(), GET_PLAYER(pUnit->getOwnerINLINE()).getName());
		szString.append(szTempBuffer);
	}

	for (iI = 0; iI < GC.getNumPromotionInfos(); ++iI)
	{
		if (pUnit->isHasPromotion((PromotionTypes)iI))
		{
			szTempBuffer.Format(L"<img=%S size=16 />", GC.getPromotionInfo((PromotionTypes)iI).getButton());
			szString.append(szTempBuffer);
		}
	}
    if (bAlt && (gDLL->getChtLvl() > 0))
    {
		CvSelectionGroup* eGroup = pUnit->getGroup();
		if (eGroup != NULL)
		{
			if (pUnit->isGroupHead())
				szString.append(CvWString::format(L"\nLeading "));
			else
				szString.append(L"\n");

			szTempBuffer.Format(L"Group(%d), %d units", eGroup->getID(), eGroup->getNumUnits());
			szString.append(szTempBuffer);
		}
    }

	if (!bOneLine)
	{
		setEspionageMissionHelp(szString, pUnit);

		if (pUnit->cargoSpace() > 0)
		{
			if (pUnit->getTeam() == GC.getGameINLINE().getActiveTeam())
			{
				szTempBuffer = NEWLINE + gDLL->getText("TXT_KEY_UNIT_HELP_CARGO_SPACE", pUnit->getCargo(), pUnit->cargoSpace());
			}
			else
			{
				szTempBuffer = NEWLINE + gDLL->getText("TXT_KEY_UNIT_CARGO_SPACE", pUnit->cargoSpace());
			}
			szString.append(szTempBuffer);

			if (pUnit->specialCargo() != NO_SPECIALUNIT)
			{
				szString.append(gDLL->getText("TXT_KEY_UNIT_CARRIES", GC.getSpecialUnitInfo(pUnit->specialCargo()).getTextKeyWide()));
			}
		}

		if (pUnit->fortifyModifier() != 0)
		{
			szString.append(NEWLINE);
			szString.append(gDLL->getText("TXT_KEY_UNIT_HELP_FORTIFY_BONUS", pUnit->fortifyModifier()));
		}

		if (!bShort)
		{
			if (pUnit->nukeRange() >= 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_UNIT_CAN_NUKE"));
			}

			if (pUnit->alwaysInvisible())
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_UNIT_INVISIBLE_ALL"));
			}
			else if (pUnit->getInvisibleType() != NO_INVISIBLE)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_UNIT_INVISIBLE_MOST"));
			}

			for (iI = 0; iI < pUnit->getNumSeeInvisibleTypes(); ++iI)
			{
				if (pUnit->getSeeInvisibleType(iI) != pUnit->getInvisibleType())
				{
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_UNIT_SEE_INVISIBLE", GC.getInvisibleInfo(pUnit->getSeeInvisibleType(iI)).getTextKeyWide()));
				}
			}

			if (pUnit->canMoveImpassable())
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_UNIT_CAN_MOVE_IMPASSABLE"));
			}

			// MOD - START - Feature Damage Immunity
			szTempBuffer.Format(L"%s%s ", NEWLINE, gDLL->getText("TXT_KEY_MISC_FEATURE_DAMAGE_IMMUNITY").GetCString());

			bool bFirst = true;
			for (iI = 0; iI < GC.getNumFeatureInfos(); ++iI)
			{
				if (pUnit->isImmuneToDamageFromFeature((FeatureTypes)iI))
				{
					const CvFeatureInfo& kLoopFeature = GC.getFeatureInfo((FeatureTypes)iI);

					CvWString szFeature;
					szFeature.Format(L"<link=%s>%s</link>", CvWString(kLoopFeature.getType()).GetCString(), kLoopFeature.getDescription(1));

					setListHelp(szString, szTempBuffer, szFeature, L", ", bFirst);

					bFirst = false;
				}
			}
			// MOD - END - Feature Damage Immunity
		}

		if (pUnit->maxFirstStrikes() > 0)
		{
			if (pUnit->firstStrikes() == pUnit->maxFirstStrikes())
			{
				if (pUnit->firstStrikes() == 1)
				{
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_UNIT_ONE_FIRST_STRIKE"));
				}
				else
				{
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_UNIT_NUM_FIRST_STRIKES", pUnit->firstStrikes()));
				}
			}
			else
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_UNIT_FIRST_STRIKE_CHANCES", pUnit->firstStrikes(), pUnit->maxFirstStrikes()));
			}
		}

		if (pUnit->immuneToFirstStrikes())
		{
			szString.append(NEWLINE);
			szString.append(gDLL->getText("TXT_KEY_UNIT_IMMUNE_FIRST_STRIKES"));
		}

		if (!bShort)
		{
			if (pUnit->noDefensiveBonus())
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_UNIT_NO_DEFENSE_BONUSES"));
			}

			if (pUnit->flatMovementCost())
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_UNIT_FLAT_MOVEMENT"));
			}

			if (pUnit->ignoreTerrainCost())
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_UNIT_IGNORE_TERRAIN"));
			}

			// MOD - START - Untransportable Units
			if (pUnit->noTransport())
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_UNIT_NO_TRANSPORT"));
			}
			// MOD - END - Untransportable Units

			if (pUnit->isBlitz())
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_PROMOTION_BLITZ_TEXT"));
			}

			if (pUnit->isAmphib())
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_PROMOTION_AMPHIB_TEXT"));
			}

			if (pUnit->isRiver())
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_PROMOTION_RIVER_ATTACK_TEXT"));
			}

			if (pUnit->isEnemyRoute())
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_PROMOTION_ENEMY_ROADS_TEXT"));
			}

			if (pUnit->isAlwaysHeal())
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_PROMOTION_ALWAYS_HEAL_TEXT"));
			}

			if (pUnit->isHillsDoubleMove())
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_PROMOTION_HILLS_MOVE_TEXT"));
			}

			for (iI = 0; iI < GC.getNumTerrainInfos(); ++iI)
			{
				if (pUnit->isTerrainDoubleMove((TerrainTypes)iI))
				{
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_PROMOTION_DOUBLE_MOVE_TEXT", GC.getTerrainInfo((TerrainTypes) iI).getTextKeyWide()));
				}
			}

			for (iI = 0; iI < GC.getNumFeatureInfos(); ++iI)
			{
				if (pUnit->isFeatureDoubleMove((FeatureTypes)iI))
				{
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_PROMOTION_DOUBLE_MOVE_TEXT", GC.getFeatureInfo((FeatureTypes) iI).getTextKeyWide()));
				}
			}

			if (pUnit->getExtraVisibilityRange() != 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_PROMOTION_VISIBILITY_TEXT", pUnit->getExtraVisibilityRange()));
			}

			if (pUnit->getExtraMoveDiscount() != 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_PROMOTION_MOVE_DISCOUNT_TEXT", -(pUnit->getExtraMoveDiscount())));
			}

			if (pUnit->getExtraEnemyHeal() != 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_PROMOTION_HEALS_EXTRA_TEXT", pUnit->getExtraEnemyHeal()) + gDLL->getText("TXT_KEY_PROMOTION_ENEMY_LANDS_TEXT"));
			}

			if (pUnit->getExtraNeutralHeal() != 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_PROMOTION_HEALS_EXTRA_TEXT", pUnit->getExtraNeutralHeal()) + gDLL->getText("TXT_KEY_PROMOTION_NEUTRAL_LANDS_TEXT"));
			}

			if (pUnit->getExtraFriendlyHeal() != 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_PROMOTION_HEALS_EXTRA_TEXT", pUnit->getExtraFriendlyHeal()) + gDLL->getText("TXT_KEY_PROMOTION_FRIENDLY_LANDS_TEXT"));
			}

			if (pUnit->getSameTileHeal() != 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_PROMOTION_HEALS_SAME_TEXT", pUnit->getSameTileHeal()) + gDLL->getText("TXT_KEY_PROMOTION_DAMAGE_TURN_TEXT"));
			}

			if (pUnit->getAdjacentTileHeal() != 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_PROMOTION_HEALS_ADJACENT_TEXT", pUnit->getAdjacentTileHeal()) + gDLL->getText("TXT_KEY_PROMOTION_DAMAGE_TURN_TEXT"));
			}
		}

		if (pUnit->currInterceptionProbability() > 0)
		{
			szString.append(NEWLINE);
			szString.append(gDLL->getText("TXT_KEY_UNIT_INTERCEPT_AIRCRAFT", pUnit->currInterceptionProbability()));
		}

		if (pUnit->evasionProbability() > 0)
		{
			szString.append(NEWLINE);
			szString.append(gDLL->getText("TXT_KEY_UNIT_EVADE_INTERCEPTION", pUnit->evasionProbability()));
		}

		if (pUnit->withdrawalProbability() > 0)
		{
			if (bShort)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_UNIT_WITHDRAWL_PROBABILITY_SHORT", pUnit->withdrawalProbability()));
			}
			else
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_UNIT_WITHDRAWL_PROBABILITY", pUnit->withdrawalProbability()));
			}
		}

		if (pUnit->combatLimit() < GC.getMAX_HIT_POINTS() && pUnit->canAttack())
		{
			szString.append(NEWLINE);
			szString.append(gDLL->getText("TXT_KEY_UNIT_COMBAT_LIMIT", (100 * pUnit->combatLimit()) / GC.getMAX_HIT_POINTS()));
		}

		if (pUnit->collateralDamage() > 0)
		{
			szString.append(NEWLINE);
			if (pUnit->getExtraCollateralDamage() == 0)
			{
				int iCollateralPercentage = 100 * pUnit->getUnitInfo().getCollateralDamageLimit() / GC.getMAX_HIT_POINTS();
				int iCollateralUnits = pUnit->getUnitInfo().getCollateralDamageMaxUnits();
				if (iCollateralUnits == 1)
					szString.append(gDLL->getText("TXT_KEY_UNIT_COLLATERAL_DAMAGE_SINGLE", iCollateralPercentage));
				else
					szString.append(gDLL->getText("TXT_KEY_UNIT_COLLATERAL_DAMAGE", iCollateralPercentage, iCollateralUnits));
			}
			else
			{
				szString.append(gDLL->getText("TXT_KEY_UNIT_COLLATERAL_DAMAGE_EXTRA", pUnit->getExtraCollateralDamage()));
			}
		}

		// MOD - START - Relation Caching
		for (int iRel = 0; iRel < pUnit->getUnitInfo().getNumRelatedUnitCombats(); iRel++)
		{
			UnitCombatTypes eLoopUnitCombat = (UnitCombatTypes)pUnit->getUnitInfo().getRelatedUnitCombats(iRel);

			if (pUnit->getUnitInfo().getUnitCombatCollateralImmune(eLoopUnitCombat))
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_UNIT_COLLATERAL_IMMUNE", GC.getUnitCombatInfo(eLoopUnitCombat).getTextKeyWide()));
			}
		}
		// MOD - END - Relation Caching

		if (pUnit->getCollateralDamageProtection() > 0)
		{
			szString.append(NEWLINE);
			szString.append(gDLL->getText("TXT_KEY_PROMOTION_COLLATERAL_PROTECTION_TEXT", pUnit->getCollateralDamageProtection()));
		}

		if (pUnit->getExtraCombatPercent() != 0)
		{
			szString.append(NEWLINE);
			szString.append(gDLL->getText("TXT_KEY_PROMOTION_STRENGTH_TEXT", pUnit->getExtraCombatPercent()));
		}

		if (pUnit->cityAttackModifier() == pUnit->cityDefenseModifier())
		{
			if (pUnit->cityAttackModifier() != 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_UNIT_CITY_STRENGTH_MOD", pUnit->cityAttackModifier()));
			}
		}
		else
		{
			if (pUnit->cityAttackModifier() != 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_PROMOTION_CITY_ATTACK_TEXT", pUnit->cityAttackModifier()));
			}

			if (pUnit->cityDefenseModifier() != 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_PROMOTION_CITY_DEFENSE_TEXT", pUnit->cityDefenseModifier()));
			}
		}

		if (pUnit->animalCombatModifier() != 0)
		{
			szString.append(NEWLINE);
			szString.append(gDLL->getText("TXT_KEY_UNIT_ANIMAL_COMBAT_MOD", pUnit->animalCombatModifier()));
		}

		// MOD - START - Extra Barbarian Combat Modifier
		if (pUnit->getExtraBarbarianCombatModifier() != 0)
		{
			szString.append(NEWLINE);
			szString.append(gDLL->getText("TXT_KEY_UNIT_BARBARIAN_COMBAT_MOD", pUnit->getExtraBarbarianCombatModifier()));
		}
		// MOD - END - Extra Barbarian Combat Modifier

		if (pUnit->getDropRange() > 0)
		{
			szString.append(NEWLINE);
			szString.append(gDLL->getText("TXT_KEY_UNIT_PARADROP_RANGE", pUnit->getDropRange()));
		}

		if (pUnit->hillsAttackModifier() == pUnit->hillsDefenseModifier())
		{
			if (pUnit->hillsAttackModifier() != 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_UNIT_HILLS_STRENGTH", pUnit->hillsAttackModifier()));
			}
		}
		else
		{
			if (pUnit->hillsAttackModifier() != 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_UNIT_HILLS_ATTACK", pUnit->hillsAttackModifier()));
			}

			if (pUnit->hillsDefenseModifier() != 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_UNIT_HILLS_DEFENSE", pUnit->hillsDefenseModifier()));
			}
		}

		for (iI = 0; iI < GC.getNumTerrainInfos(); ++iI)
		{
			if (pUnit->terrainAttackModifier((TerrainTypes)iI) == pUnit->terrainDefenseModifier((TerrainTypes)iI))
			{
				if (pUnit->terrainAttackModifier((TerrainTypes)iI) != 0)
				{
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_UNIT_STRENGTH", pUnit->terrainAttackModifier((TerrainTypes)iI), GC.getTerrainInfo((TerrainTypes) iI).getTextKeyWide()));
				}
			}
			else
			{
				if (pUnit->terrainAttackModifier((TerrainTypes)iI) != 0)
				{
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_UNIT_ATTACK", pUnit->terrainAttackModifier((TerrainTypes)iI), GC.getTerrainInfo((TerrainTypes) iI).getTextKeyWide()));
				}

				if (pUnit->terrainDefenseModifier((TerrainTypes)iI) != 0)
				{
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_UNIT_DEFENSE", pUnit->terrainDefenseModifier((TerrainTypes)iI), GC.getTerrainInfo((TerrainTypes) iI).getTextKeyWide()));
				}
			}
		}

		for (iI = 0; iI < GC.getNumFeatureInfos(); ++iI)
		{
			if (pUnit->featureAttackModifier((FeatureTypes)iI) == pUnit->featureDefenseModifier((FeatureTypes)iI))
			{
				if (pUnit->featureAttackModifier((FeatureTypes)iI) != 0)
				{
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_UNIT_STRENGTH", pUnit->featureAttackModifier((FeatureTypes)iI), GC.getFeatureInfo((FeatureTypes) iI).getTextKeyWide()));
				}
			}
			else
			{
				if (pUnit->featureAttackModifier((FeatureTypes)iI) != 0)
				{
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_UNIT_ATTACK", pUnit->featureAttackModifier((FeatureTypes)iI), GC.getFeatureInfo((FeatureTypes) iI).getTextKeyWide()));
				}

				if (pUnit->featureDefenseModifier((FeatureTypes)iI) != 0)
				{
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_UNIT_DEFENSE", pUnit->featureDefenseModifier((FeatureTypes)iI), GC.getFeatureInfo((FeatureTypes) iI).getTextKeyWide()));
				}
			}
		}

		// MOD - START - Relation Caching
		for (int iRel = 0; iRel < pUnit->getUnitInfo().getNumRelatedUnitClasses(); iRel++)
		{
			UnitClassTypes eLoopUnitClass = (UnitClassTypes)pUnit->getUnitInfo().getRelatedUnitClasses(iRel);

			// MOD - START - Improved Civilopedia
			if (pUnit->getUnitInfo().getUnitClassAttackModifier(eLoopUnitClass) == GC.getUnitInfo(pUnit->getUnitType()).getUnitClassDefenseModifier(eLoopUnitClass))
			{
				if (pUnit->getUnitInfo().getUnitClassAttackModifier(eLoopUnitClass) != 0)
				{
					const CvUnitClassInfo& kLoopUnitClass = GC.getUnitClassInfo(eLoopUnitClass);
					CvWString szLinkType = (kLoopUnitClass.getRepresentativeUnit() != NO_UNIT) ? GC.getUnitInfo((UnitTypes)kLoopUnitClass.getRepresentativeUnit()).getType() : kLoopUnitClass.getType();
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_UNIT_MOD_VS_TYPE", pUnit->getUnitInfo().getUnitClassAttackModifier(eLoopUnitClass), szLinkType.GetCString(), kLoopUnitClass.getTextKeyWide()));
				}
			}
			else
			{
				const CvUnitClassInfo& kLoopUnitClass = GC.getUnitClassInfo(eLoopUnitClass);
				CvWString szLinkType = (kLoopUnitClass.getRepresentativeUnit() != NO_UNIT) ? GC.getUnitInfo((UnitTypes)kLoopUnitClass.getRepresentativeUnit()).getType() : kLoopUnitClass.getType();

				if (pUnit->getUnitInfo().getUnitClassAttackModifier(eLoopUnitClass) != 0)
				{
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_UNIT_ATTACK_MOD_VS_CLASS", pUnit->getUnitInfo().getUnitClassAttackModifier(eLoopUnitClass), szLinkType.GetCString(), kLoopUnitClass.getTextKeyWide()));
				}

				if (pUnit->getUnitInfo().getUnitClassDefenseModifier(eLoopUnitClass) != 0)
				{
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_UNIT_DEFENSE_MOD_VS_CLASS", pUnit->getUnitInfo().getUnitClassDefenseModifier(eLoopUnitClass), szLinkType.GetCString(), kLoopUnitClass.getTextKeyWide()));
				}
			}
			// MOD - END - Improved Civilopedia
		}
		
		for (int iRel = 0; iRel < pUnit->getUnitInfo().getNumRelatedUnitCombats(); iRel++)
		{
			UnitCombatTypes eLoopUnitCombat = (UnitCombatTypes)pUnit->getUnitInfo().getRelatedUnitCombats(iRel);
			
			// MOD - START - Improved Civilopedia
			// MOD - START - Unit Combat Attack Defense Mod
			if (pUnit->getUnitInfo().getUnitCombatAttackModifier(eLoopUnitCombat) == GC.getUnitInfo(pUnit->getUnitType()).getUnitCombatDefenseModifier(eLoopUnitCombat))
			{
				if (pUnit->getUnitInfo().getUnitCombatAttackModifier(eLoopUnitCombat) != 0)
				{
					const CvInfoBase& kLoopUnitCombat = GC.getUnitCombatInfo(eLoopUnitCombat);
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_UNIT_MOD_VS_TYPE", pUnit->getUnitInfo().getUnitCombatAttackModifier(eLoopUnitCombat), CvWString(kLoopUnitCombat.getType()).GetCString(), kLoopUnitCombat.getTextKeyWide()));
				}
			}
			else
			{
				if (pUnit->getUnitInfo().getUnitCombatAttackModifier(eLoopUnitCombat) != 0)
				{
					const CvInfoBase& kLoopUnitCombat = GC.getUnitCombatInfo(eLoopUnitCombat);
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_UNIT_ATTACK_MOD_VS_COMBAT", pUnit->getUnitInfo().getUnitCombatAttackModifier(eLoopUnitCombat), CvWString(kLoopUnitCombat.getType()).GetCString(), kLoopUnitCombat.getTextKeyWide()));
				}

				if (pUnit->getUnitInfo().getUnitCombatDefenseModifier(eLoopUnitCombat) != 0)
				{
					const CvInfoBase& kLoopUnitCombat = GC.getUnitCombatInfo(eLoopUnitCombat);
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_UNIT_DEFENSE_MOD_VS_COMBAT", pUnit->getUnitInfo().getUnitCombatDefenseModifier(eLoopUnitCombat), CvWString(kLoopUnitCombat.getType()).GetCString(), kLoopUnitCombat.getTextKeyWide()));
				}
			}
			// MOD - END - Unit Combat Attack Defense Mod

			if (pUnit->unitCombatModifier(eLoopUnitCombat) != 0)
			{
				const CvInfoBase& kLoopUnitCombat = GC.getUnitCombatInfo(eLoopUnitCombat);
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_UNIT_MOD_VS_TYPE", pUnit->unitCombatModifier(eLoopUnitCombat), CvWString(kLoopUnitCombat.getType()).GetCString(), kLoopUnitCombat.getTextKeyWide()));
			}
			// MOD - END - Improved Civilopedia
		}
		// MOD - END - Relation Caching

		for (iI = 0; iI < NUM_DOMAIN_TYPES; ++iI)
		{
			if (pUnit->domainModifier((DomainTypes)iI) != 0)
			{
				// MOD - START - Improved Civilopedia
				const CvDomainInfo& kLoopDomain = GC.getDomainInfo((DomainTypes)iI);
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_UNIT_MOD_VS_TYPE", pUnit->domainModifier((DomainTypes)iI), CvWString(kLoopDomain.getType()).GetCString(), kLoopDomain.getTextKeyWide()));
				// MOD - END - Improved Civilopedia
			}
		}

		szTempBuffer.clear();
		bFirst = true;

		// MOD - START - Relation Caching
		for (int iRel = 0; iRel < pUnit->getUnitInfo().getNumRelatedUnitClasses(); iRel++)
		{
			UnitClassTypes eLoopUnitClass = (UnitClassTypes)pUnit->getUnitInfo().getRelatedUnitClasses(iRel);

			if (pUnit->getUnitInfo().getTargetUnitClass(eLoopUnitClass))
			{
				if (bFirst)
				{
					bFirst = false;
				}
				else
				{
					szTempBuffer += L", ";
				}

				szTempBuffer += CvWString::format(L"<link=literal>%s</link>", GC.getUnitClassInfo(eLoopUnitClass).getDescription());
			}
		}
		// MOD - END - Relation Caching

		if (!bFirst)
		{
			szString.append(NEWLINE);
			szString.append(gDLL->getText("TXT_KEY_UNIT_TARGETS_UNIT_FIRST", szTempBuffer.GetCString()));
		}

		szTempBuffer.clear();
		bFirst = true;

		// MOD - START - Relation Caching
		for (int iRel = 0; iRel < pUnit->getUnitInfo().getNumRelatedUnitClasses(); iRel++)
		{
			UnitClassTypes eLoopUnitClass = (UnitClassTypes)pUnit->getUnitInfo().getRelatedUnitClasses(iRel);

			if (pUnit->getUnitInfo().getDefenderUnitClass(eLoopUnitClass))
			{
				if (bFirst)
				{
					bFirst = false;
				}
				else
				{
					szTempBuffer += L", ";
				}

				szTempBuffer += CvWString::format(L"<link=literal>%s</link>", GC.getUnitClassInfo(eLoopUnitClass).getDescription());
			}
		}
		// MOD - END - Relation Caching

		if (!bFirst)
		{
			szString.append(NEWLINE);
			szString.append(gDLL->getText("TXT_KEY_UNIT_DEFENDS_UNIT_FIRST", szTempBuffer.GetCString()));
		}

		szTempBuffer.clear();
		bFirst = true;

		// MOD - START - Relation Caching
		for (int iRel = 0; iRel < pUnit->getUnitInfo().getNumRelatedUnitCombats(); iRel++)
		{
			UnitCombatTypes eLoopUnitCombat = (UnitCombatTypes)pUnit->getUnitInfo().getRelatedUnitCombats(iRel);

			if (pUnit->getUnitInfo().getTargetUnitCombat(eLoopUnitCombat))
			{
				if (bFirst)
				{
					bFirst = false;
				}
				else
				{
					szTempBuffer += L", ";
				}

				szTempBuffer += CvWString::format(L"<link=literal>%s</link>", GC.getUnitCombatInfo(eLoopUnitCombat).getDescription());
			}
		}
		// MOD - END - Relation Caching

		if (!bFirst)
		{
			szString.append(NEWLINE);
			szString.append(gDLL->getText("TXT_KEY_UNIT_TARGETS_UNIT_FIRST", szTempBuffer.GetCString()));
		}

		szTempBuffer.clear();
		bFirst = true;

		// MOD - START - Relation Caching
		for (int iRel = 0; iRel < pUnit->getUnitInfo().getNumRelatedUnitCombats(); iRel++)
		{
			UnitCombatTypes eLoopUnitCombat = (UnitCombatTypes)pUnit->getUnitInfo().getRelatedUnitCombats(iRel);

			if (pUnit->getUnitInfo().getDefenderUnitCombat(eLoopUnitCombat))
			{
				if (bFirst)
				{
					bFirst = false;
				}
				else
				{
					szTempBuffer += L", ";
				}

				szTempBuffer += CvWString::format(L"<link=literal>%s</link>", GC.getUnitCombatInfo(eLoopUnitCombat).getDescription());
			}
		}
		// MOD - END - Relation Caching

		if (!bFirst)
		{
			szString.append(NEWLINE);
			szString.append(gDLL->getText("TXT_KEY_UNIT_DEFENDS_UNIT_FIRST", szTempBuffer.GetCString()));
		}

		szTempBuffer.clear();
		bFirst = true;

		// MOD - START - Relation Caching
		for (int iRel = 0; iRel < pUnit->getUnitInfo().getNumRelatedUnitClasses(); iRel++)
		{
			UnitClassTypes eLoopUnitClass = (UnitClassTypes)pUnit->getUnitInfo().getRelatedUnitClasses(iRel);

			if (pUnit->getUnitInfo().getFlankingStrikeUnitClass(eLoopUnitClass) > 0)
			{
				if (bFirst)
				{
					bFirst = false;
				}
				else
				{
					szTempBuffer += L", ";
				}

				szTempBuffer += CvWString::format(L"<link=literal>%s</link>", GC.getUnitClassInfo(eLoopUnitClass).getDescription());
			}
		}
		// MOD - END - Relation Caching

		if (!bFirst)
		{
			szString.append(NEWLINE);
			szString.append(gDLL->getText("TXT_KEY_UNIT_FLANKING_STRIKES", szTempBuffer.GetCString()));
		}

		if (pUnit->bombardRate() > 0)
		{
			if (bShort)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_UNIT_BOMBARD_RATE_SHORT", ((pUnit->bombardRate() * 100) / GC.getMAX_CITY_DEFENSE_DAMAGE())));
			}
			else
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_UNIT_BOMBARD_RATE", ((pUnit->bombardRate() * 100) / GC.getMAX_CITY_DEFENSE_DAMAGE())));
			}
		}
/************************************************************************************************/
/* DCM									   START				04/19/09            Johny Smith */
/************************************************************************************************/
		// Dale - FE: Fighters START
		if (GC.getUnitInfo(pUnit->getUnitType()).getDCMFighterEngage())
		{
			szString.append(NEWLINE);
			szString.append(gDLL->getText("TXT_KEY_IS_FIGHTER_ENGAGE"));
		}
		// Dale - FE: Fighters END
/************************************************************************************************/
/* DCM                                     END                                                  */
/************************************************************************************************/
		if (pUnit->isSpy())
		{
			szString.append(NEWLINE);
			szString.append(gDLL->getText("TXT_KEY_UNIT_IS_SPY"));
		}

		if (pUnit->getUnitInfo().isNoRevealMap())
		{
			szString.append(NEWLINE);
			szString.append(gDLL->getText("TXT_KEY_UNIT_VISIBILITY_MOVE_RANGE"));
		}

		if (!CvWString(pUnit->getUnitInfo().getHelp()).empty())
		{
			szString.append(NEWLINE);
			szString.append(pUnit->getUnitInfo().getHelp());
		}

        if (bShift && (gDLL->getChtLvl() > 0))
        {
            szTempBuffer.Format(L"\nUnitAI Type = %s.", GC.getUnitAIInfo(pUnit->AI_getUnitAIType()).getDescription());
            szString.append(szTempBuffer);
            szTempBuffer.Format(L"\nSacrifice Value = %d.", pUnit->AI_sacrificeValue(NULL));
            szString.append(szTempBuffer);
        }
	}
}


void CvGameTextMgr::setPlotListHelp(CvWStringBuffer &szString, CvPlot* pPlot, bool bOneLine, bool bShort)
{
	PROFILE_FUNC();

	int numPromotionInfos = GC.getNumPromotionInfos();

	// if cheatmode and ctrl, display grouping info instead
	if ((gDLL->getChtLvl() > 0) && GC.ctrlKey())
	{
		if (pPlot->isVisible(GC.getGameINLINE().getActiveTeam(), true))
		{
			CvWString szTempString;

			CLLNode<IDInfo>* pUnitNode = pPlot->headUnitNode();
			while(pUnitNode != NULL)
			{
				CvUnit* pHeadUnit = ::getUnit(pUnitNode->m_data);
				pUnitNode = pPlot->nextUnitNode(pUnitNode);

				// is this unit the head of a group, not cargo, and visible?
				if (pHeadUnit && pHeadUnit->isGroupHead() && !pHeadUnit->isCargo() && !pHeadUnit->isInvisible(GC.getGameINLINE().getActiveTeam(), true)) // K-Mod
				{
					// head unit name and unitai
					szString.append(CvWString::format(SETCOLR L"%s" ENDCOLR, 255,190,0,255, pHeadUnit->getName().GetCString()));
					szString.append(CvWString::format(L" (%d)", shortenID(pHeadUnit->getID())));
					getUnitAIString(szTempString, pHeadUnit->AI_getUnitAIType());
					szString.append(CvWString::format(SETCOLR L" %s " ENDCOLR, GET_PLAYER(pHeadUnit->getOwnerINLINE()).getPlayerTextColorR(), GET_PLAYER(pHeadUnit->getOwnerINLINE()).getPlayerTextColorG(), GET_PLAYER(pHeadUnit->getOwnerINLINE()).getPlayerTextColorB(), GET_PLAYER(pHeadUnit->getOwnerINLINE()).getPlayerTextColorA(), szTempString.GetCString()));

					// promotion icons
					for (int iPromotionIndex = 0; iPromotionIndex < numPromotionInfos; iPromotionIndex++)
					{
						PromotionTypes ePromotion = (PromotionTypes)iPromotionIndex;
						if (pHeadUnit->isHasPromotion(ePromotion))
						{
							szString.append(CvWString::format(L"<img=%S size=16 />", GC.getPromotionInfo(ePromotion).getButton()));
						}
					}

					// group
					CvSelectionGroup* pHeadGroup = pHeadUnit->getGroup();
					FAssertMsg(pHeadGroup != NULL, "unit has NULL group");
					if (pHeadGroup->getNumUnits() > 1)
					{
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      07/17/09                                jdog5000      */
/*                                                                                              */
/* DEBUG                                                                                        */
/************************************************************************************************/
						szString.append(CvWString::format(L"\nGroup:%d [%d units", shortenID(pHeadGroup->getID()), pHeadGroup->getNumUnits()));
						if( pHeadGroup->getCargo() > 0 )
						{
							szString.append(CvWString::format(L" + %d cargo", pHeadGroup->getCargo()));
						}
						szString.append(CvWString::format(L"]"));

						// get average damage
						int iAverageDamage = 0;
						CLLNode<IDInfo>* pUnitNode = pHeadGroup->headUnitNode();
						while (pUnitNode != NULL)
						{
							CvUnit* pLoopUnit = ::getUnit(pUnitNode->m_data);
							pUnitNode = pHeadGroup->nextUnitNode(pUnitNode);

							iAverageDamage += (pLoopUnit->getDamage() * pLoopUnit->maxHitPoints()) / 100;
						}
						iAverageDamage /= pHeadGroup->getNumUnits();
						if (iAverageDamage > 0)
						{
							szString.append(CvWString::format(L" %d%%", 100 - iAverageDamage));
						}
					}

					if (!pHeadGroup->isHuman() && pHeadGroup->isStranded()) // K-Mod
					{
						szString.append(CvWString::format(SETCOLR L"\n***STRANDED***" ENDCOLR, TEXT_COLOR("COLOR_RED")));
					}

					if( !GC.altKey() )
					{
						// mission ai
						MissionAITypes eMissionAI = pHeadGroup->AI_getMissionAIType();
						if (eMissionAI != NO_MISSIONAI)
						{
							getMissionAIString(szTempString, eMissionAI);
							szString.append(CvWString::format(SETCOLR L"\n%s" ENDCOLR, TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), szTempString.GetCString()));
						}

						// mission
						MissionTypes eMissionType = (MissionTypes) pHeadGroup->getMissionType(0);
						if (eMissionType != NO_MISSION)
						{
							getMissionTypeString(szTempString, eMissionType);
							szString.append(CvWString::format(SETCOLR L"\n%s" ENDCOLR, TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), szTempString.GetCString()));
						}

						// mission unit
						CvUnit* pMissionUnit = pHeadGroup->AI_getMissionAIUnit();
						if (pMissionUnit != NULL && (eMissionAI != NO_MISSIONAI || eMissionType != NO_MISSION))
						{
							// mission unit
							szString.append(L"\n to ");
							szString.append(CvWString::format(SETCOLR L"%s" ENDCOLR, GET_PLAYER(pMissionUnit->getOwnerINLINE()).getPlayerTextColorR(), GET_PLAYER(pMissionUnit->getOwnerINLINE()).getPlayerTextColorG(), GET_PLAYER(pMissionUnit->getOwnerINLINE()).getPlayerTextColorB(), GET_PLAYER(pMissionUnit->getOwnerINLINE()).getPlayerTextColorA(), pMissionUnit->getName().GetCString()));
							szString.append(CvWString::format(L"(%d) G:%d", shortenID(pMissionUnit->getID()), shortenID(pMissionUnit->getGroupID())));
							getUnitAIString(szTempString, pMissionUnit->AI_getUnitAIType());
							szString.append(CvWString::format(SETCOLR L" %s" ENDCOLR, GET_PLAYER(pMissionUnit->getOwnerINLINE()).getPlayerTextColorR(), GET_PLAYER(pMissionUnit->getOwnerINLINE()).getPlayerTextColorG(), GET_PLAYER(pMissionUnit->getOwnerINLINE()).getPlayerTextColorB(), GET_PLAYER(pMissionUnit->getOwnerINLINE()).getPlayerTextColorA(), szTempString.GetCString()));
						}
						
						// mission plot
						if (eMissionAI != NO_MISSIONAI || eMissionType != NO_MISSION)
						{
							// first try the plot from the missionAI
							CvPlot* pMissionPlot = pHeadGroup->AI_getMissionAIPlot();
							
							// if MissionAI does not have a plot, get one from the mission itself
							if (pMissionPlot == NULL && eMissionType != NO_MISSION)
							{
								switch (eMissionType)
								{
								case MISSION_MOVE_TO:
								case MISSION_ROUTE_TO:
									pMissionPlot =  GC.getMapINLINE().plotINLINE(pHeadGroup->getMissionData1(0), pHeadGroup->getMissionData2(0));
									break;

								case MISSION_MOVE_TO_UNIT:
									if (pMissionUnit != NULL)
									{
										pMissionPlot = pMissionUnit->plot();
									}
									break;
								}
							}

							if (pMissionPlot != NULL)
							{
								szString.append(CvWString::format(L"\n [%d,%d]", pMissionPlot->getX_INLINE(), pMissionPlot->getY_INLINE()));
								
								CvCity* pCity = pMissionPlot->getWorkingCity();
								if (pCity != NULL)
								{
									szString.append(L" (");

									if (!pMissionPlot->isCity())
									{
										DirectionTypes eDirection = estimateDirection(dxWrap(pMissionPlot->getX_INLINE() - pCity->plot()->getX_INLINE()), dyWrap(pMissionPlot->getY_INLINE() - pCity->plot()->getY_INLINE()));

										getDirectionTypeString(szTempString, eDirection);
										szString.append(CvWString::format(L"%s of ", szTempString.GetCString()));
									}

									szString.append(CvWString::format(SETCOLR L"%s" ENDCOLR L")", GET_PLAYER(pCity->getOwnerINLINE()).getPlayerTextColorR(), GET_PLAYER(pCity->getOwnerINLINE()).getPlayerTextColorG(), GET_PLAYER(pCity->getOwnerINLINE()).getPlayerTextColorB(), GET_PLAYER(pCity->getOwnerINLINE()).getPlayerTextColorA(), pCity->getName().GetCString()));
								}
								else
								{
									if (pMissionPlot != pPlot)
									{
										DirectionTypes eDirection = estimateDirection(dxWrap(pMissionPlot->getX_INLINE() - pPlot->getX_INLINE()), dyWrap(pMissionPlot->getY_INLINE() - pPlot->getY_INLINE()));

										getDirectionTypeString(szTempString, eDirection);
										szString.append(CvWString::format(L" (%s)", szTempString.GetCString()));
									}

									PlayerTypes eMissionPlotOwner = pMissionPlot->getOwnerINLINE();
									if (eMissionPlotOwner != NO_PLAYER)
									{
										szString.append(CvWString::format(L", " SETCOLR L"%s" ENDCOLR, GET_PLAYER(eMissionPlotOwner).getPlayerTextColorR(), GET_PLAYER(eMissionPlotOwner).getPlayerTextColorG(), GET_PLAYER(eMissionPlotOwner).getPlayerTextColorB(), GET_PLAYER(eMissionPlotOwner).getPlayerTextColorA(), GET_PLAYER(eMissionPlotOwner).getName()));
									}
								}
							}
						}

						// activity
						ActivityTypes eActivityType = (ActivityTypes) pHeadGroup->getActivityType();
						if (eActivityType != NO_ACTIVITY)
						{
							getActivityTypeString(szTempString, eActivityType);
							szString.append(CvWString::format(SETCOLR L"\n%s" ENDCOLR, TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), szTempString.GetCString()));
						}
					}
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/

/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      06/10/08                                jdog5000      */
/*                                                                                              */
/* DEBUG                                                                                        */
/************************************************************************************************/
					if( !GC.altKey() && !GC.shiftKey() )
					{
						// display cargo for head unit
						std::vector<CvUnit*> aCargoUnits;
						pHeadUnit->getCargoUnits(aCargoUnits);
						for (uint i = 0; i < aCargoUnits.size(); ++i)
						{
							CvUnit* pCargoUnit = aCargoUnits[i];
							if (!pCargoUnit->isInvisible(GC.getGameINLINE().getActiveTeam(), true))
							{
								// name and unitai
								szString.append(CvWString::format(SETCOLR L"\n %s" ENDCOLR, TEXT_COLOR("COLOR_ALT_HIGHLIGHT_TEXT"), pCargoUnit->getName().GetCString()));
								szString.append(CvWString::format(L"(%d)", shortenID(pCargoUnit->getID())));
								getUnitAIString(szTempString, pCargoUnit->AI_getUnitAIType());
								szString.append(CvWString::format(SETCOLR L" %s " ENDCOLR, GET_PLAYER(pCargoUnit->getOwnerINLINE()).getPlayerTextColorR(), GET_PLAYER(pCargoUnit->getOwnerINLINE()).getPlayerTextColorG(), GET_PLAYER(pCargoUnit->getOwnerINLINE()).getPlayerTextColorB(), GET_PLAYER(pCargoUnit->getOwnerINLINE()).getPlayerTextColorA(), szTempString.GetCString()));

								// promotion icons
								for (int iPromotionIndex = 0; iPromotionIndex < numPromotionInfos; iPromotionIndex++)
								{
									PromotionTypes ePromotion = (PromotionTypes)iPromotionIndex;
									if (pCargoUnit->isHasPromotion(ePromotion))
									{
										szString.append(CvWString::format(L"<img=%S size=16 />", GC.getPromotionInfo(ePromotion).getButton()));
									}
								}
							}
						}
						
						// display grouped units
						CLLNode<IDInfo>* pUnitNode3 = pPlot->headUnitNode();
						while(pUnitNode3 != NULL)
						{
							CvUnit* pUnit = ::getUnit(pUnitNode3->m_data);
							pUnitNode3 = pPlot->nextUnitNode(pUnitNode3);

							// is this unit not head, in head's group and visible?
							if (pUnit && (pUnit != pHeadUnit) && (pUnit->getGroupID() == pHeadUnit->getGroupID()) && !pUnit->isInvisible(GC.getGameINLINE().getActiveTeam(), true))
							{
								FAssertMsg(!pUnit->isCargo(), "unit is cargo but head unit is not cargo");
								// name and unitai
								szString.append(CvWString::format(SETCOLR L"\n-%s" ENDCOLR, TEXT_COLOR("COLOR_UNIT_TEXT"), pUnit->getName().GetCString()));
								szString.append(CvWString::format(L" (%d)", shortenID(pUnit->getID())));
								getUnitAIString(szTempString, pUnit->AI_getUnitAIType());
								szString.append(CvWString::format(SETCOLR L" %s " ENDCOLR, GET_PLAYER(pUnit->getOwnerINLINE()).getPlayerTextColorR(), GET_PLAYER(pUnit->getOwnerINLINE()).getPlayerTextColorG(), GET_PLAYER(pUnit->getOwnerINLINE()).getPlayerTextColorB(), GET_PLAYER(pUnit->getOwnerINLINE()).getPlayerTextColorA(), szTempString.GetCString()));

								// promotion icons
								for (int iPromotionIndex = 0; iPromotionIndex < numPromotionInfos; iPromotionIndex++)
								{
									PromotionTypes ePromotion = (PromotionTypes)iPromotionIndex;
									if (pUnit->isHasPromotion(ePromotion))
									{
										szString.append(CvWString::format(L"<img=%S size=16 />", GC.getPromotionInfo(ePromotion).getButton()));
									}
								}

								// display cargo for loop unit
								std::vector<CvUnit*> aLoopCargoUnits;
								pUnit->getCargoUnits(aLoopCargoUnits);
								for (uint i = 0; i < aLoopCargoUnits.size(); ++i)
								{
									CvUnit* pCargoUnit = aLoopCargoUnits[i];
									if (!pCargoUnit->isInvisible(GC.getGameINLINE().getActiveTeam(), true))
									{
										// name and unitai
										szString.append(CvWString::format(SETCOLR L"\n %s" ENDCOLR, TEXT_COLOR("COLOR_ALT_HIGHLIGHT_TEXT"), pCargoUnit->getName().GetCString()));
										szString.append(CvWString::format(L"(%d)", shortenID(pCargoUnit->getID())));
										getUnitAIString(szTempString, pCargoUnit->AI_getUnitAIType());
										szString.append(CvWString::format(SETCOLR L" %s " ENDCOLR, GET_PLAYER(pCargoUnit->getOwnerINLINE()).getPlayerTextColorR(), GET_PLAYER(pCargoUnit->getOwnerINLINE()).getPlayerTextColorG(), GET_PLAYER(pCargoUnit->getOwnerINLINE()).getPlayerTextColorB(), GET_PLAYER(pCargoUnit->getOwnerINLINE()).getPlayerTextColorA(), szTempString.GetCString()));

										// promotion icons
										for (int iPromotionIndex = 0; iPromotionIndex < numPromotionInfos; iPromotionIndex++)
										{
											PromotionTypes ePromotion = (PromotionTypes)iPromotionIndex;
											if (pCargoUnit->isHasPromotion(ePromotion))
											{
												szString.append(CvWString::format(L"<img=%S size=16 />", GC.getPromotionInfo(ePromotion).getButton()));
											}
										}
									}
								}
							}
						}
					}
					
					if( !GC.altKey() )
					{
						if( pPlot->getTeam() == NO_TEAM || GET_TEAM(pHeadGroup->getTeam()).isAtWar(pPlot->getTeam()) )
						{
							szString.append(NEWLINE);
							CvWString szTempBuffer;

							//AI strategies
							if (GET_PLAYER(pHeadGroup->getOwner()).AI_isDoStrategy(AI_STRATEGY_DAGGER))
							{
								szTempBuffer.Format(L"Dagger, ");
								szString.append(szTempBuffer);
							}
							if (GET_PLAYER(pHeadGroup->getOwner()).AI_isDoStrategy(AI_STRATEGY_CRUSH))
							{
								szTempBuffer.Format(L"Crush, ");
								szString.append(szTempBuffer);
							}
							if (GET_PLAYER(pHeadGroup->getOwner()).AI_isDoStrategy(AI_STRATEGY_ALERT1))
							{
								szTempBuffer.Format(L"Alert1, ");
								szString.append(szTempBuffer);
							}
							if (GET_PLAYER(pHeadGroup->getOwner()).AI_isDoStrategy(AI_STRATEGY_ALERT2))
							{
								szTempBuffer.Format(L"Alert2, ");
								szString.append(szTempBuffer);
							}
							if (GET_PLAYER(pHeadGroup->getOwner()).AI_isDoStrategy(AI_STRATEGY_TURTLE))
							{
								szTempBuffer.Format(L"Turtle, ");
								szString.append(szTempBuffer);
							}
							if (GET_PLAYER(pHeadGroup->getOwner()).AI_isDoStrategy(AI_STRATEGY_LAST_STAND))
							{
								szTempBuffer.Format(L"Last Stand, ");
								szString.append(szTempBuffer);
							}
							if (GET_PLAYER(pHeadGroup->getOwner()).AI_isDoStrategy(AI_STRATEGY_FINAL_WAR))
							{
								szTempBuffer.Format(L"FinalWar, ");
								szString.append(szTempBuffer);
							}
							if (GET_PLAYER(pHeadGroup->getOwner()).AI_isDoStrategy(AI_STRATEGY_GET_BETTER_UNITS))
							{
								szTempBuffer.Format(L"GetBetterUnits, ");
								szString.append(szTempBuffer);
							}
							if (GET_PLAYER(pHeadGroup->getOwner()).AI_isDoStrategy(AI_STRATEGY_FASTMOVERS))
							{
								szTempBuffer.Format(L"FastMovers, ");
								szString.append(szTempBuffer);
							}
							if (GET_PLAYER(pHeadGroup->getOwner()).AI_isDoStrategy(AI_STRATEGY_LAND_BLITZ))
							{
								szTempBuffer.Format(L"LandBlitz, ");
								szString.append(szTempBuffer);
							}
							if (GET_PLAYER(pHeadGroup->getOwner()).AI_isDoStrategy(AI_STRATEGY_AIR_BLITZ))
							{
								szTempBuffer.Format(L"AirBlitz, ");
								szString.append(szTempBuffer);
							}
 							if (GET_PLAYER(pHeadGroup->getOwner()).AI_isDoStrategy(AI_STRATEGY_OWABWNW))
							{
								szTempBuffer.Format(L"OWABWNW, ");
								szString.append(szTempBuffer);
							}
							if (GET_PLAYER(pHeadGroup->getOwner()).AI_isDoStrategy(AI_STRATEGY_PRODUCTION))
							{
								szTempBuffer.Format(L"Production, ");
								szString.append(szTempBuffer);
							}
							if (GET_PLAYER(pHeadGroup->getOwner()).AI_isDoStrategy(AI_STRATEGY_MISSIONARY))
							{
								szTempBuffer.Format(L"Missionary, ");
								szString.append(szTempBuffer);
							}
							if (GET_PLAYER(pHeadGroup->getOwner()).AI_isDoStrategy(AI_STRATEGY_BIG_ESPIONAGE))
							{
								szTempBuffer.Format(L"BigEspionage, ");
								szString.append(szTempBuffer);
							}
							if (GET_PLAYER(pHeadGroup->getOwner()).AI_isDoStrategy(AI_STRATEGY_ECONOMY_FOCUS)) // K-Mod
							{
								szTempBuffer.Format(L"EconomyFocus, ");
								szString.append(szTempBuffer);
							}
							if (GET_PLAYER(pHeadGroup->getOwner()).AI_isDoStrategy(AI_STRATEGY_ESPIONAGE_ECONOMY)) // K-Mod
							{
								szTempBuffer.Format(L"EspionageEconomy, ");
								szString.append(szTempBuffer);
							}

							//Area battle plans.
							if (pPlot->area()->getAreaAIType(pHeadGroup->getTeam()) == AREAAI_OFFENSIVE)
							{
								szTempBuffer.Format(L"\n Area AI = OFFENSIVE");
							}
							else if (pPlot->area()->getAreaAIType(pHeadGroup->getTeam()) == AREAAI_DEFENSIVE)
							{
								szTempBuffer.Format(L"\n Area AI = DEFENSIVE");
							}
							else if (pPlot->area()->getAreaAIType(pHeadGroup->getTeam()) == AREAAI_MASSING)
							{
								szTempBuffer.Format(L"\n Area AI = MASSING");
							}
							else if (pPlot->area()->getAreaAIType(pHeadGroup->getTeam()) == AREAAI_ASSAULT)
							{
								szTempBuffer.Format(L"\n Area AI = ASSAULT");
							}
							else if (pPlot->area()->getAreaAIType(pHeadGroup->getTeam()) == AREAAI_ASSAULT_MASSING)
							{
								szTempBuffer.Format(L"\n Area AI = ASSAULT_MASSING");
							}
							else if (pPlot->area()->getAreaAIType(pHeadGroup->getTeam()) == AREAAI_NEUTRAL)
							{
								szTempBuffer.Format(L"\n Area AI = NEUTRAL");
							}

							CvCity* pTargetCity = pPlot->area()->getTargetCity(pHeadGroup->getOwner());
							if( pTargetCity )
							{
								szString.append(CvWString::format(L"\nTarget City: %s (%d)", pTargetCity->getName().c_str(), pTargetCity->getOwner()));
							}
							else
							{
								szString.append(CvWString::format(L"\nTarget City: None"));
							}

							if( GC.shiftKey() )
							{
								CvCity* pLoopCity;
								int iLoop = 0;
								int iBestTargetValue = (pTargetCity != NULL ? GET_PLAYER(pHeadGroup->getOwner()).AI_targetCityValue(pTargetCity,false,true) : 0);
								int iTargetValue = 0;
								szString.append(CvWString::format(L"\n\nTarget City values:\n"));
								for( int iPlayer = 0; iPlayer < MAX_PLAYERS; iPlayer++ )
								{
									if( GET_TEAM(pHeadGroup->getTeam()).AI_getWarPlan(GET_PLAYER((PlayerTypes)iPlayer).getTeam()) != NO_WARPLAN )
									{
										if( pPlot->area()->getCitiesPerPlayer((PlayerTypes)iPlayer) > 0 )
										{
											for (pLoopCity = GET_PLAYER((PlayerTypes)iPlayer).firstCity(&iLoop); pLoopCity != NULL; pLoopCity = GET_PLAYER((PlayerTypes)iPlayer).nextCity(&iLoop))
											{
												if( pLoopCity->area() == pPlot->area() )
												{
													iTargetValue = GET_PLAYER(pHeadGroup->getOwner()).AI_targetCityValue(pLoopCity,false,true);

													if( (GC.getMapINLINE().calculatePathDistance(pPlot, pLoopCity->plot()) < 20))
													{
														szString.append(CvWString::format(L"\n%s : %d + rand %d", pLoopCity->getName().c_str(), iTargetValue, (pLoopCity->getPopulation() / 2)));
													}
												}
											}
										}
									}
								}
							}
						}
					}
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/

					// double space non-empty groups
					if (pHeadGroup->getNumUnits() > 1 || pHeadUnit->hasCargo())
					{
						szString.append(NEWLINE);
					}

					szString.append(NEWLINE);
				}
			}
		}

		return;
	}

	// MOD - START - Efficient Plot Summary
	// Note: Original code deleted
	if (pPlot->isVisible(GC.getGameINLINE().getActiveTeam(), true))
	{
		static const int iMaxNumUnits = 15;
		static std::vector<CvUnit *> plotUnits;
		GC.getGameINLINE().getPlotUnits(pPlot, plotUnits);
		int iNumVisibleUnits = (int)plotUnits.size();

		if (iNumVisibleUnits > 0)
		{
			// Show the center unit first
			if (pPlot->getCenterUnit())
			{
				setUnitHelp(szString, pPlot->getCenterUnit(), iNumVisibleUnits > iMaxNumUnits, true);
			}

			// Show the next set of units (up to 15)
			int iNumShown = std::min<int>(iMaxNumUnits, iNumVisibleUnits);
			for (int iI = 0; iI < iNumShown; iI++)
			{
				CvUnit* pLoopUnit = plotUnits[iI];
				if (pLoopUnit != pPlot->getCenterUnit())
				{
					szString.append(NEWLINE);
					setUnitHelp(szString, pLoopUnit, true, true);
				}
			}

			// Show the summaries last
			if (iNumVisibleUnits > iMaxNumUnits)
			{
				std::map<PlayerUnitPair, UnitSummary, PlayerUnitPair_less> plotSummary;

				for (int iI = iMaxNumUnits; iI < iNumVisibleUnits; iI++)
				{
					CvUnit* pLoopUnit = plotUnits[iI];

					if (pLoopUnit != pPlot->getCenterUnit())
					{
						UnitSummary& kUnitSummary = plotSummary[PlayerUnitPair(pLoopUnit->getOwnerINLINE(), pLoopUnit->getUnitType())];
						kUnitSummary.iNumUnits++;

						int iBase = (DOMAIN_AIR == pLoopUnit->getDomainType() ? pLoopUnit->airBaseCombatStr() : pLoopUnit->baseCombatStr());
						if (iBase > 0 && pLoopUnit->maxHitPoints() > 0)
						{
							kUnitSummary.iSumMaxStrength += 100 * iBase;
							kUnitSummary.iSumCurrentStrength += (100 * iBase * pLoopUnit->currHitPoints()) / pLoopUnit->maxHitPoints();
						}

						// TODO: Consider use of a promotion vector on each unit
						for (int iJ = 0; iJ < numPromotionInfos; iJ++)
						{
							if (pLoopUnit->isHasPromotion((PromotionTypes)iJ))
							{
								int& iNumPromotions = kUnitSummary.aPromotions[(PromotionTypes)iJ];
								iNumPromotions++;
							}
						}
					}
				}

				for (std::map<PlayerUnitPair, UnitSummary, PlayerUnitPair_less>::const_iterator it=plotSummary.begin(); it!=plotSummary.end(); it++)
				{
					PlayerTypes eLoopPlayer = (*it).first.ePlayer;
					UnitTypes eLoopUnit = (*it).first.eUnit;
					const UnitSummary& kUnitSummary = (*it).second;
					const CvUnitInfo& kLoopUnit = GC.getUnitInfo(eLoopUnit);

					szString.append(NEWLINE);
					szString.append(CvWString::format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR("COLOR_UNIT_TEXT"), kLoopUnit.getDescription()));
					szString.append(CvWString::format(L" (%d)", kUnitSummary.iNumUnits));

					if (kUnitSummary.iSumMaxStrength > 0)
					{
						int iBase = (kUnitSummary.iSumMaxStrength / kUnitSummary.iNumUnits) / 100;
						int iCurrent = (kUnitSummary.iSumCurrentStrength / kUnitSummary.iNumUnits) / 100;
						int iCurrent100 = (kUnitSummary.iSumCurrentStrength / kUnitSummary.iNumUnits) % 100;
						if (0 == iCurrent100)
						{
							if (iBase == iCurrent)
							{
								szString.append(CvWString::format(L" %d", iBase));
							}
							else
							{
								szString.append(CvWString::format(L" %d/%d", iCurrent, iBase));
							}
						}
						else
						{
							szString.append(CvWString::format(L" %d.%02d/%d", iCurrent, iCurrent100, iBase));
						}
						szString.append(CvWString::format(L"%c", gDLL->getSymbolID(STRENGTH_CHAR)));
					}

					for (std::map<PromotionTypes, int>::const_iterator it=kUnitSummary.aPromotions.begin(); it!=kUnitSummary.aPromotions.end(); it++)
					{
						PromotionTypes eLoopPromotion = (*it).first;
						int iNumPromotions = (*it).second;
						szString.append(CvWString::format(L"%d<img=%S size=16 />", iNumPromotions, GC.getPromotionInfo(eLoopPromotion).getButton()));
					}

					if (eLoopPlayer != GC.getGameINLINE().getActivePlayer() && !kLoopUnit.isAnimal() && !kLoopUnit.isHiddenNationality())
					{
						const CvPlayerAI& kLoopPlayer = GET_PLAYER(eLoopPlayer);
						szString.append(L", ");
						szString.append(CvWString::format(SETCOLR L"%s" ENDCOLR, kLoopPlayer.getPlayerTextColorR(), kLoopPlayer.getPlayerTextColorG(), kLoopPlayer.getPlayerTextColorB(), kLoopPlayer.getPlayerTextColorA(), kLoopPlayer.getName()));
					}
				}
			}
		}
	}
	// MOD - END - Efficient Plot Summary
}

/*************************************************************************************************/
/** ADVANCED COMBAT ODDS                      11/7/09                           PieceOfMind      */
/** BEGIN                                                                       v1.1             */
/*************************************************************************************************/

//Calculates the probability of a particular combat outcome
//Returns a float value (between 0 and 1)
//Written by PieceOfMind
//n_A = hits taken by attacker, n_D = hits taken by defender.
static float getCombatOddsSpecific(CvUnit* pAttacker, CvUnit* pDefender, int n_A, int n_D)
{
	int iAttackerStrength;
	int iAttackerFirepower;
	int iDefenderStrength;
	int iDefenderFirepower;
	int iDefenderOdds;
	int iAttackerOdds;
	int iStrengthFactor;
	int iDamageToAttacker;
	int iDamageToDefender;
	int iNeededRoundsAttacker;
	//int iNeededRoundsDefender;

	int AttFSnet;
	int AttFSC;
	int DefFSC;

	int iDefenderHitLimit;

	iAttackerStrength = pAttacker->currCombatStr(NULL, NULL);
	iAttackerFirepower = pAttacker->currFirepower(NULL, NULL);
	iDefenderStrength = pDefender->currCombatStr(pDefender->plot(), pAttacker);
	iDefenderFirepower = pDefender->currFirepower(pDefender->plot(), pAttacker);

	iStrengthFactor = ((iAttackerFirepower + iDefenderFirepower + 1) / 2);
	iDamageToAttacker = std::max(1,((GC.getDefineINT("COMBAT_DAMAGE") * (iDefenderFirepower + iStrengthFactor)) / (iAttackerFirepower + iStrengthFactor)));
	iDamageToDefender = std::max(1,((GC.getDefineINT("COMBAT_DAMAGE") * (iAttackerFirepower + iStrengthFactor)) / (iDefenderFirepower + iStrengthFactor)));

	iDefenderOdds = ((GC.getDefineINT("COMBAT_DIE_SIDES") * iDefenderStrength) / (iAttackerStrength + iDefenderStrength));
	iAttackerOdds = GC.getDefineINT("COMBAT_DIE_SIDES") - iDefenderOdds;

/* 	if (GC.getDefineINT("ACO_IgnoreBarbFreeWins")==0)
	{
		if (pDefender->isBarbarian())
		{
			//defender is barbarian
			if (!GET_PLAYER(pAttacker->getOwnerINLINE()).isBarbarian() && GET_PLAYER(pAttacker->getOwnerINLINE()).getWinsVsBarbs() < GC.getHandicapInfo(GET_PLAYER(pAttacker->getOwnerINLINE()).getHandicapType()).getFreeWinsVsBarbs())
			{
				//attacker is not barb and attacker player has free wins left
				//I have assumed in the following code only one of the units (attacker and defender) can be a barbarian

				iDefenderOdds = std::min((10 * GC.getDefineINT("COMBAT_DIE_SIDES")) / 100, iDefenderOdds);
				iAttackerOdds = std::max((90 * GC.getDefineINT("COMBAT_DIE_SIDES")) / 100, iAttackerOdds);
			}
		}
		else if (pAttacker->isBarbarian())
		{
			//attacker is barbarian
			if (!GET_PLAYER(pDefender->getOwnerINLINE()).isBarbarian() && GET_PLAYER(pDefender->getOwnerINLINE()).getWinsVsBarbs() < GC.getHandicapInfo(GET_PLAYER(pDefender->getOwnerINLINE()).getHandicapType()).getFreeWinsVsBarbs())
			{
				//defender is not barbarian and defender has free wins left and attacker is barbarian
				iAttackerOdds = std::min((10 * GC.getDefineINT("COMBAT_DIE_SIDES")) / 100, iAttackerOdds);
				iDefenderOdds = std::max((90 * GC.getDefineINT("COMBAT_DIE_SIDES")) / 100, iDefenderOdds);
			}
		}
	} */

	iDefenderHitLimit = pDefender->maxHitPoints() - pAttacker->combatLimit();

	//iNeededRoundsAttacker = (std::max(0, pDefender->currHitPoints() - iDefenderHitLimit) + iDamageToDefender - (((pAttacker->combatLimit())==GC.getMAX_HIT_POINTS())?1:0) ) / iDamageToDefender;
	iNeededRoundsAttacker = (pDefender->currHitPoints() - pDefender->maxHitPoints() + pAttacker->combatLimit() - (((pAttacker->combatLimit())==pDefender->maxHitPoints())?1:0))/iDamageToDefender + 1;

	int N_D = (std::max(0, pDefender->currHitPoints() - iDefenderHitLimit) + iDamageToDefender - (((pAttacker->combatLimit())==GC.getMAX_HIT_POINTS())?1:0) ) / iDamageToDefender;

	//int N_A = (pAttacker->currHitPoints() + iDamageToAttacker - 1 ) / iDamageToAttacker;  //same as next line
	int N_A = (pAttacker->currHitPoints() - 1)/iDamageToAttacker + 1;

	//int iRetreatOdds = std::max((pAttacker->withdrawalProbability()),100);
	float RetreatOdds = ((float)(std::min((pAttacker->withdrawalProbability()),100)))/100.0f ;

	AttFSnet = ( (pDefender->immuneToFirstStrikes()) ? 0 : pAttacker->firstStrikes() ) - ((pAttacker->immuneToFirstStrikes()) ? 0 : pDefender->firstStrikes());
	AttFSC = (pDefender->immuneToFirstStrikes()) ? 0 : (pAttacker->chanceFirstStrikes());
	DefFSC = (pAttacker->immuneToFirstStrikes()) ? 0 : (pDefender->chanceFirstStrikes());

	float P_A = (float)iAttackerOdds / GC.getDefineINT("COMBAT_DIE_SIDES");
	float P_D = (float)iDefenderOdds / GC.getDefineINT("COMBAT_DIE_SIDES");
	float answer = 0.0f;
	if (n_A < N_A && n_D == iNeededRoundsAttacker)   // (1) Defender dies or is taken to combat limit
	{
		float sum1 = 0.0f;
		for (int i = (-AttFSnet-AttFSC<1?1:-AttFSnet-AttFSC); i <= DefFSC - AttFSnet; i++)
		{
			for (int j = 0; j <= i; j++)
			{
				if (n_A >= j)
				{
					sum1 += (float)getBinomialCoefficient(i,j) * pow(P_A,(float)(i-j)) * getBinomialCoefficient(iNeededRoundsAttacker-1+n_A-j,iNeededRoundsAttacker-1);

				} //if
			}//for j
		}//for i
		sum1 *= pow(P_D,(float)n_A)*pow(P_A,(float)iNeededRoundsAttacker);
		answer += sum1;

		float sum2 = 0.0f;

		for (int i = (0<AttFSnet-DefFSC?AttFSnet-DefFSC:0); i <= AttFSnet + AttFSC; i++)
		{

			for (int j = 0; j <= i; j++)
			{
				if (N_D > j)
				{
					sum2 = sum2 + getBinomialCoefficient(n_A+iNeededRoundsAttacker-j-1,n_A) * (float)getBinomialCoefficient(i,j) * pow(P_A,(float)iNeededRoundsAttacker) * pow(P_D,(float)(n_A+i-j));

				}
				else if (n_A == 0)
				{
					sum2 = sum2 + (float)getBinomialCoefficient(i,j) * pow(P_A,(float)j) * pow(P_D,(float)(i-j));
				}
				else
				{
					sum2 = sum2 + 0.0f;
				}
			}//for j

		}//for i
		answer += sum2;
	}
	else if (n_D < N_D && n_A == N_A)  // (2) Attacker dies!
	{

		float sum1 = 0.0f;
		for (int i = (-AttFSnet-AttFSC<1?1:-AttFSnet-AttFSC); i <= DefFSC - AttFSnet; i++)
		{

			for (int j = 0; j <= i; j++)
			{
				if (N_A>j)
				{
					sum1 += getBinomialCoefficient(n_D+N_A-j-1,n_D) * (float)getBinomialCoefficient(i,j) * pow(P_D,(float)(N_A)) * pow(P_A,(float)(n_D+i-j));
				}
				else
				{
					if (n_D == 0)
					{
						sum1 += (float)getBinomialCoefficient(i,j) * pow(P_D,(float)(j)) * pow(P_A,(float)(i-j));
					}//if (inside if) else sum += 0
				}//if
			}//for j
		}//for i
		answer += sum1;
		float sum2 = 0.0f;
		for (int i = (0<AttFSnet-DefFSC?AttFSnet-DefFSC:0); i <= AttFSnet + AttFSC; i++)
		{
			for (int j = 0; j <= i; j++)
			{
				if (n_D >= j)
				{
					sum2 += (float)getBinomialCoefficient(i,j) * pow(P_D,(float)(i-j)) * getBinomialCoefficient(N_A-1+n_D-j,N_A-1);
				} //if
			}//for j
		}//for i
		sum2 *= pow(P_A,(float)(n_D))*pow(P_D,(float)(N_A));
		answer += sum2;
		answer = answer * (1.0f - RetreatOdds);
	}
	else if (n_A == (N_A-1) && n_D < N_D)  // (3) Attacker retreats!
	{
		float sum1 = 0.0f;
		for (int i = (AttFSnet+AttFSC>-1?1:-AttFSnet-AttFSC); i <= DefFSC - AttFSnet; i++)
		{

			for (int j = 0; j <= i; j++)
			{
				if (N_A>j)
				{
					sum1 += getBinomialCoefficient(n_D+N_A-j-1,n_D) * (float)getBinomialCoefficient(i,j) * pow(P_D,(float)(N_A)) * pow(P_A,(float)(n_D+i-j));
				}
				else
				{
					if (n_D == 0)
					{
						sum1 += (float)getBinomialCoefficient(i,j) * pow(P_D,(float)(j)) * pow(P_A,(float)(i-j));
					}//if (inside if) else sum += 0
				}//if
			}//for j
		}//for i
		answer += sum1;

		float sum2 = 0.0f;
		for (int i = (0<AttFSnet-DefFSC?AttFSnet-DefFSC:0); i <= AttFSnet + AttFSC; i++)
		{
			for (int j = 0; j <= i; j++)
			{
				if (n_D >= j)
				{
					sum2 += (float)getBinomialCoefficient(i,j) * pow(P_D,(float)(i-j)) * getBinomialCoefficient(N_A-1+n_D-j,N_A-1);
				} //if
			}//for j
		}//for i
		sum2 *= pow(P_A,(float)(n_D))*pow(P_D,(float)(N_A));
		answer += sum2;
		answer = answer * RetreatOdds;//
	}
	else
	{
		//Unexpected value.  Process should not reach here.
		FAssertMsg(false, "unexpected value in getCombatOddsSpecific");
	}

	answer = answer / ((float)(AttFSC+DefFSC+1)); // dividing by (t+w+1) as is necessary
	return answer;
}// getCombatOddsSpecific

// I had to add this function to the header file CvGameCoreUtils.h
/*************************************************************************************************/
/** ADVANCED COMBAT ODDS                      11/7/09                           PieceOfMind      */
/** END                                                                                          */
/*************************************************************************************************/

// Returns true if help was given...

// K-Mod note: this function can change the center unit on the plot. (because of a change I made)
// Also, I've made some unmarked structural changes to this function, to make it easier to read and to fix a few minor bugs.
// TODO: Verify that the merged combat modifiers make sense (in particular the sign of each modifier with and without ACO)
bool CvGameTextMgr::setCombatPlotHelp(CvWStringBuffer &szString, CvPlot* pPlot)
{
	PROFILE_FUNC();

/*************************************************************************************************/
/** ADVANCED COMBAT ODDS                      3/11/09                           PieceOfMind      */
/** BEGIN                                                                       v2.0             */
/*************************************************************************************************/
	/*
	Note that due to the large amount of extra content added to this function (setCombatPlotHelp), this should never be used in any function that needs to be called repeatedly (e.g. hundreds of times) quickly.
	It is fine for a human player mouse-over (which is what it is used for).
	*/
/* New Code */
	bool ACO_enabled = getBugOptionBOOL("ACO__Enabled", false, "ACO_ENABLED");
	bool bShift = GC.shiftKey();
	int iView = bShift ? 2 : 1;
	if (getBugOptionBOOL("ACO__SwapViews", false, "ACO_SWAP_VIEWS"))
	{
		iView = 3 - iView; //swaps 1 and 2.
	}
	CvWString szTempBuffer2;
/*************************************************************************************************/
/** ADVANCED COMBAT ODDS                      3/11/09                           PieceOfMind      */
/** END                                                                         v2.0             */
/*************************************************************************************************/

	CvWString szTempBuffer;
	CvWString szOffenseOdds;
	CvWString szDefenseOdds;
	int iModifier;

	if (gDLL->getInterfaceIFace()->getLengthSelectionList() == 0)
	{
		return false;
	}

	bool bValid = false;
	int iOdds;
	CvUnit* pAttacker = gDLL->getInterfaceIFace()->getSelectionList()->AI_getBestGroupAttacker(pPlot, false, iOdds);

	switch (gDLL->getInterfaceIFace()->getSelectionList()->getDomainType())
	{
	case DOMAIN_SEA:
		bValid = pPlot->isWater();
		break;

	case DOMAIN_AIR:
		bValid = true;
		break;

	case DOMAIN_LAND:
		if (pAttacker->canMoveAllTerrain())
		{
			bValid = true;
		}
		else
		{
			bValid = !(pPlot->isWater());
		}
		break;

	case DOMAIN_IMMOBILE:
		break;

	default:
		FAssert(false);
		break;
	}

	if (!bValid)
	{
		return false;
	}

	if (pAttacker == NULL)
		pAttacker = gDLL->getInterfaceIFace()->getSelectionList()->AI_getBestGroupAttacker(pPlot, false, iOdds, true); // bypass checks for moves and war etc.

	if (pAttacker == NULL)
		return false;

	CvUnit* pDefender = pPlot->getBestDefender(NO_PLAYER, pAttacker->getOwnerINLINE(), pAttacker, !GC.altKey());
	if (pDefender == NULL || !pDefender->canDefend(pPlot) || !pAttacker->canAttack(*pDefender))
		return false;

	// K-Mod. If the plot's center unit isn't one of our own units, then use this defender as the plot's center unit.
	// With this, the map will accurately shows who we're up against.
	if (gDLL->getInterfaceIFace()->getSelectionPlot() != pPlot)
	{
		if (pDefender->getOwnerINLINE() == GC.getGameINLINE().getActivePlayer() ||
			!pPlot->getCenterUnit() || // I don't think this is possible... but it's pretty cheap to check.
			pPlot->getCenterUnit()->getOwnerINLINE() != GC.getGameINLINE().getActivePlayer())
		{
			pPlot->setCenterUnit(pDefender);
		}
	}
	// K-Mod end

	if (pAttacker->getDomainType() != DOMAIN_AIR)
	{
		int iCombatOdds = getCombatOdds(pAttacker, pDefender);

		if (pAttacker->combatLimit() >= GC.getMAX_HIT_POINTS())
		{
			if (!ACO_enabled || getBugOptionBOOL("ACO__ForceOriginalOdds", false, "ACO_FORCE_ORIGINAL_ODDS"))
			{
				if (iCombatOdds > 999)
				{
					szTempBuffer = L"&gt; 99.9";
				}
				else if (iCombatOdds < 1)
				{
					szTempBuffer = L"&lt; 0.1";
				}
				else
				{
					szTempBuffer.Format(L"%.1f", ((float)iCombatOdds) / 10.0f);
				}

				szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_ODDS", szTempBuffer.GetCString()));
				if (ACO_enabled)
				{
					szString.append(NEWLINE);
				}
			}
		}


		int iWithdrawal = 0;

		if (pAttacker->combatLimit() < GC.getMAX_HIT_POINTS())
		{
			iWithdrawal += 100 * iCombatOdds;
		}

		iWithdrawal += std::min(100, pAttacker->withdrawalProbability()) * (1000 - iCombatOdds);

		if (iWithdrawal > 0 || pAttacker->combatLimit() < GC.getMAX_HIT_POINTS())
		{
			if (!ACO_enabled || getBugOptionBOOL("ACO__ForceOriginalOdds", false, "ACO_FORCE_ORIGINAL_ODDS"))
			{
				if (iWithdrawal > 99900)
				{
					szTempBuffer = L"&gt; 99.9";
				}
				else if (iWithdrawal < 100)
				{
					szTempBuffer = L"&lt; 0.1";
				}
				else
				{
					szTempBuffer.Format(L"%.1f", iWithdrawal / 1000.0f);
				}

				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_ODDS_RETREAT", szTempBuffer.GetCString()));
				if (ACO_enabled)
				{
					szString.append(NEWLINE);
				}
			}
		}

		//szTempBuffer.Format(L"AI odds: %d%%", iOdds);
		//szString += NEWLINE + szTempBuffer;
/*************************************************************************************************/
/** ADVANCED COMBAT ODDS                      3/11/09                           PieceOfMind      */
/** BEGIN                                                                       v2.0             */
/*************************************************************************************************/
/* New Code */
		if (ACO_enabled)
		{

			BOOL ACO_debug = false;
			//Change this to true when you need to spot errors, particular in the expected hit points calculations
			ACO_debug = getBugOptionBOOL("ACO__Debug", false, "ACO_DEBUG");

			/** phungus sart **/
//					bool bctrl; bctrl = GC.ctrlKey();
//					if (bctrl)
//					{//SWITCHAROO IS DISABLED IN V1.0.  Hopefully it will be available in the next version. At the moment is has issues when modifiers are present.
//						CvUnit* swap = pAttacker;
//						pAttacker = pDefender;
//						pDefender = swap;
//
//						CvPlot* pAttackerPlot = pAttacker->plot();
//		                  CvPlot* pDefenderPlot = pDefender->plot();
//					}
			int iAttackerExperienceModifier = 0;
			int iDefenderExperienceModifier = 0;
			for (int ePromotion = 0; ePromotion < GC.getNumPromotionInfos(); ++ePromotion)
			{
				if (pAttacker->isHasPromotion((PromotionTypes)ePromotion) && GC.getPromotionInfo((PromotionTypes)ePromotion).getExperiencePercent() != 0)
				{
					iAttackerExperienceModifier += GC.getPromotionInfo((PromotionTypes)ePromotion).getExperiencePercent();
				}
			}

			for (int ePromotion = 0; ePromotion < GC.getNumPromotionInfos(); ++ePromotion)
			{
				if (pDefender->isHasPromotion((PromotionTypes)ePromotion) && GC.getPromotionInfo((PromotionTypes)ePromotion).getExperiencePercent() != 0)
				{
					iDefenderExperienceModifier += GC.getPromotionInfo((PromotionTypes)ePromotion).getExperiencePercent();
				}
			}
			/** phungus end **/ //thanks to phungus420



			/** Many thanks to DanF5771 for some of these calculations! **/
			int iAttackerStrength  = pAttacker->currCombatStr(NULL, NULL);
			int iAttackerFirepower = pAttacker->currFirepower(NULL, NULL);
			int iDefenderStrength  = pDefender->currCombatStr(pPlot, pAttacker);
			int iDefenderFirepower = pDefender->currFirepower(pPlot, pAttacker);

			FAssert((iAttackerStrength + iDefenderStrength)*(iAttackerFirepower + iDefenderFirepower) > 0);

			int iStrengthFactor    = ((iAttackerFirepower + iDefenderFirepower + 1) / 2);
			int iDamageToAttacker  = std::max(1,((GC.getDefineINT("COMBAT_DAMAGE") * (iDefenderFirepower + iStrengthFactor)) / (iAttackerFirepower + iStrengthFactor)));
			int iDamageToDefender  = std::max(1,((GC.getDefineINT("COMBAT_DAMAGE") * (iAttackerFirepower + iStrengthFactor)) / (iDefenderFirepower + iStrengthFactor)));
			int iFlankAmount       = iDamageToAttacker;

			int iDefenderOdds = ((GC.getDefineINT("COMBAT_DIE_SIDES") * iDefenderStrength) / (iAttackerStrength + iDefenderStrength));
			int iAttackerOdds = GC.getDefineINT("COMBAT_DIE_SIDES") - iDefenderOdds;


/* 			// Barbarian related code.
			if (getBugOptionBOOL("ACO__IgnoreBarbFreeWins", false, "ACO_IGNORE_BARB_FREE_WINS"))//Are we not going to ignore barb free wins?  If not, skip this section...
			{
				if (pDefender->isBarbarian())
				{
					//defender is barbarian
					if (!GET_PLAYER(pAttacker->getOwnerINLINE()).isBarbarian() && GET_PLAYER(pAttacker->getOwnerINLINE()).getWinsVsBarbs() < GC.getHandicapInfo(GET_PLAYER(pAttacker->getOwnerINLINE()).getHandicapType()).getFreeWinsVsBarbs())
					{
						//attacker is not barb and attacker player has free wins left
						//I have assumed in the following code only one of the units (attacker and defender) can be a barbarian
						iDefenderOdds = std::min((10 * GC.getDefineINT("COMBAT_DIE_SIDES")) / 100, iDefenderOdds);
						iAttackerOdds = std::max((90 * GC.getDefineINT("COMBAT_DIE_SIDES")) / 100, iAttackerOdds);
						szTempBuffer.Format(SETCOLR L"%d\n" ENDCOLR,
							TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"),GC.getHandicapInfo(GET_PLAYER(pAttacker->getOwnerINLINE()).getHandicapType()).getFreeWinsVsBarbs()-GET_PLAYER(pAttacker->getOwnerINLINE()).getWinsVsBarbs());
						szString.append(gDLL->getText("TXT_ACO_BarbFreeWinsLeft"));
						szString.append(szTempBuffer.GetCString());
					}
				}
				else
				{
					//defender is not barbarian
					if (pAttacker->isBarbarian())
					{
						//attacker is barbarian
						if (!GET_PLAYER(pDefender->getOwnerINLINE()).isBarbarian() && GET_PLAYER(pDefender->getOwnerINLINE()).getWinsVsBarbs() < GC.getHandicapInfo(GET_PLAYER(pDefender->getOwnerINLINE()).getHandicapType()).getFreeWinsVsBarbs())
						{
							//defender is not barbarian and defender has free wins left and attacker is barbarian
							iAttackerOdds = std::min((10 * GC.getDefineINT("COMBAT_DIE_SIDES")) / 100, iAttackerOdds);
							iDefenderOdds = std::max((90 * GC.getDefineINT("COMBAT_DIE_SIDES")) / 100, iDefenderOdds);
							szTempBuffer.Format(SETCOLR L"%d\n" ENDCOLR,
								TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"),GC.getHandicapInfo(GET_PLAYER(pDefender->getOwnerINLINE()).getHandicapType()).getFreeWinsVsBarbs()-GET_PLAYER(pDefender->getOwnerINLINE()).getWinsVsBarbs());
							szString.append(gDLL->getText("TXT_ACO_BarbFreeWinsLeft"));
							szString.append(szTempBuffer.GetCString());
						}
					}
				}
			} */
			

			if (!GET_PLAYER(pAttacker->getOwnerINLINE()).isBarbarian() && GET_PLAYER(pAttacker->getOwnerINLINE()).getWinsVsBarbs() < GC.getHandicapInfo(GET_PLAYER(pAttacker->getOwnerINLINE()).getHandicapType()).getFreeWinsVsBarbs())
			{
				//attacker is not barb and attacker player has free wins left

				szTempBuffer.Format(SETCOLR L"%d\n" ENDCOLR,
					TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"),GC.getHandicapInfo(GET_PLAYER(pDefender->getOwnerINLINE()).getHandicapType()).getFreeWinsVsBarbs()-GET_PLAYER(pDefender->getOwnerINLINE()).getWinsVsBarbs());
				szString.append(gDLL->getText("TXT_ACO_BarbFreeWinsLeft"));
				szString.append(szTempBuffer.GetCString());
			}			


			//XP calculations
			int iExperience;
			int iWithdrawXP;//thanks to phungus420
			iWithdrawXP = GC.getDefineINT("EXPERIENCE_FROM_WITHDRAWL");//thanks to phungus420

			if (pAttacker->combatLimit() < 100)
			{
				iExperience        = GC.getDefineINT("EXPERIENCE_FROM_WITHDRAWL");
			}
			else
			{
				iExperience        = (pDefender->attackXPValue() * iDefenderStrength) / iAttackerStrength;
				iExperience        = range(iExperience, GC.getDefineINT("MIN_EXPERIENCE_PER_COMBAT"), GC.getDefineINT("MAX_EXPERIENCE_PER_COMBAT"));
			}

			int iDefExperienceKill;
			iDefExperienceKill = (pAttacker->defenseXPValue() * iAttackerStrength) / iDefenderStrength;
			iDefExperienceKill = range(iDefExperienceKill, GC.getDefineINT("MIN_EXPERIENCE_PER_COMBAT"), GC.getDefineINT("MAX_EXPERIENCE_PER_COMBAT"));

			int iBonusAttackerXP = (iExperience * iAttackerExperienceModifier) / 100;
			int iBonusDefenderXP = (iDefExperienceKill * iDefenderExperienceModifier) / 100;
			int iBonusWithdrawXP = (GC.getDefineINT("EXPERIENCE_FROM_WITHDRAWL") * iAttackerExperienceModifier) / 100;


			//The following code adjusts the XP for barbarian encounters.  In standard game, barb and animal xp cap is 10,5 respectively.
			/**Thanks to phungus420 for the following block of code! **/
			if(pDefender->isBarbarian())
			{
				if (pDefender->isAnimal())
				{
					//animal
					iExperience = range(iExperience,0,GC.getDefineINT("ANIMAL_MAX_XP_VALUE")-(pAttacker->getExperience()));
					if (iExperience < 0 )
					{
						iExperience = 0;
					}
					iWithdrawXP = range(iWithdrawXP,0,GC.getDefineINT("ANIMAL_MAX_XP_VALUE")-(pAttacker->getExperience()));
					if (iWithdrawXP < 0 )
					{
						iWithdrawXP = 0;
					}
					iBonusAttackerXP = range(iBonusAttackerXP,0,GC.getDefineINT("ANIMAL_MAX_XP_VALUE")-(pAttacker->getExperience() + iExperience));
					if (iBonusAttackerXP < 0 )
					{
						iBonusAttackerXP = 0;
					}
					iBonusWithdrawXP = range(iBonusWithdrawXP,0,GC.getDefineINT("ANIMAL_MAX_XP_VALUE")-(pAttacker->getExperience() + iWithdrawXP));
					if (iBonusWithdrawXP < 0 )
					{
						iBonusWithdrawXP = 0;
					}
				}
				else
				{
					//normal barbarian
					iExperience = range(iExperience,0,GC.getDefineINT("BARBARIAN_MAX_XP_VALUE")-pAttacker->getExperience());
					if (iExperience < 0 )
					{
						iExperience = 0;
					}
					iWithdrawXP = range(iWithdrawXP,0,GC.getDefineINT("BARBARIAN_MAX_XP_VALUE")-(pAttacker->getExperience()));
					if (iWithdrawXP < 0 )
					{
						iWithdrawXP = 0;
					}
					iBonusAttackerXP = range(iBonusAttackerXP,0,GC.getDefineINT("BARBARIAN_MAX_XP_VALUE")-(pAttacker->getExperience() + iExperience));
					if (iBonusAttackerXP < 0 )
					{
						iBonusAttackerXP = 0;
					}
					iBonusWithdrawXP = range(iBonusWithdrawXP,0,GC.getDefineINT("BARBARIAN_MAX_XP_VALUE")-(pAttacker->getExperience() + iWithdrawXP));
					if (iBonusWithdrawXP < 0 )
					{
						iBonusWithdrawXP = 0;
					}
				}
			}

			int iNeededRoundsAttacker = (pDefender->currHitPoints() - pDefender->maxHitPoints() + pAttacker->combatLimit() - (((pAttacker->combatLimit())==pDefender->maxHitPoints())?1:0))/iDamageToDefender + 1;
			//The extra term introduced here was to account for the incorrect way it treated units that had combatLimits.
			//A catapult that deals 25HP per round, and has a combatLimit of 75HP must deal four successful hits before it kills the warrior -not 3.  This is proved in the way CvUnit::resolvecombat works
			// The old formula (with just a plain -1 instead of a conditional -1 or 0) was incorrectly saying three.

			// int iNeededRoundsDefender = (pAttacker->currHitPoints() + iDamageToAttacker - 1 ) / iDamageToAttacker;  //this is idential to the following line
			int iNeededRoundsDefender = (pAttacker->currHitPoints() - 1)/iDamageToAttacker + 1;

			//szTempBuffer.Format(L"iNeededRoundsAttacker = %d\niNeededRoundsDefender = %d",iNeededRoundsAttacker,iNeededRoundsDefender);
			//szString.append(NEWLINE);szString.append(szTempBuffer.GetCString());
			//szTempBuffer.Format(L"pDefender->currHitPoints = %d\n-pDefender->maxHitPOints = %d\n + pAttacker->combatLimit = %d\n - 1 if\npAttackercomBatlimit equals pDefender->maxHitpoints\n=(%d == %d)\nall over iDamageToDefender = %d\n+1 = ...",
			//pDefender->currHitPoints(),pDefender->maxHitPoints(),pAttacker->combatLimit(),pAttacker->combatLimit(),pDefender->maxHitPoints(),iDamageToDefender);
			//szString.append(NEWLINE);szString.append(szTempBuffer.GetCString());

			int iDefenderHitLimit = pDefender->maxHitPoints() - pAttacker->combatLimit();

			//NOW WE CALCULATE SOME INTERESTING STUFF :)

			float E_HP_Att = 0.0f;//expected damage dealt to attacker
			float E_HP_Def = 0.0f;
			float E_HP_Att_Withdraw; //Expected hitpoints for attacker if attacker withdraws (not the same as retreat)
			float E_HP_Att_Victory; //Expected hitpoints for attacker if attacker kills defender
			int E_HP_Att_Retreat = (pAttacker->currHitPoints()) - (iNeededRoundsDefender-1)*iDamageToAttacker;//this one is predetermined easily
			float E_HP_Def_Withdraw;
			float E_HP_Def_Defeat; // if attacker dies
			//Note E_HP_Def is the same for if the attacker withdraws or dies

			float AttackerUnharmed;
			float DefenderUnharmed;

			AttackerUnharmed = getCombatOddsSpecific(pAttacker,pDefender,0,iNeededRoundsAttacker);
			DefenderUnharmed = getCombatOddsSpecific(pAttacker,pDefender,iNeededRoundsDefender,0);
			DefenderUnharmed += getCombatOddsSpecific(pAttacker,pDefender,iNeededRoundsDefender-1,0);//attacker withdraws or retreats

			float prob_bottom_Att_HP; // The probability the attacker exits combat with min HP
			float prob_bottom_Def_HP; // The probability the defender exits combat with min HP



			if (ACO_debug)
			{
				szTempBuffer.Format(L"E[HP ATTACKER]");
				//szString.append(NEWLINE);
				szString.append(szTempBuffer.GetCString());
			}
			// already covers both possibility of defender not being killed AND being killed
			for (int n_A = 0; n_A < iNeededRoundsDefender; n_A++)
			{
				//prob_attack[n_A] = getCombatOddsSpecific(pAttacker,pDefender,n_A,iNeededRoundsAttacker);
				E_HP_Att += ( (pAttacker->currHitPoints()) - n_A*iDamageToAttacker) * getCombatOddsSpecific(pAttacker,pDefender,n_A,iNeededRoundsAttacker);

				if (ACO_debug)
				{
					szTempBuffer.Format(L"+%d * %.2f%%  (Def %d) (%d:%d)",
						((pAttacker->currHitPoints()) - n_A*iDamageToAttacker),100.0f*getCombatOddsSpecific(pAttacker,pDefender,n_A,iNeededRoundsAttacker),iDefenderHitLimit,n_A,iNeededRoundsAttacker);
					szString.append(NEWLINE);
					szString.append(szTempBuffer.GetCString());
				}
			}
			E_HP_Att_Victory = E_HP_Att;//NOT YET NORMALISED
			E_HP_Att_Withdraw = E_HP_Att;//NOT YET NORMALIZED
			prob_bottom_Att_HP = getCombatOddsSpecific(pAttacker,pDefender,iNeededRoundsDefender-1,iNeededRoundsAttacker);
			if((pAttacker->withdrawalProbability()) > 0)
			{
				// if withdraw odds involved
				if (ACO_debug)
				{
					szTempBuffer.Format(L"Attacker retreat odds");
					szString.append(NEWLINE);
					szString.append(szTempBuffer.GetCString());
				}
				for (int n_D = 0; n_D < iNeededRoundsAttacker; n_D++)
				{
					E_HP_Att += ( (pAttacker->currHitPoints()) - (iNeededRoundsDefender-1)*iDamageToAttacker) * getCombatOddsSpecific(pAttacker,pDefender,iNeededRoundsDefender-1,n_D);
					prob_bottom_Att_HP += getCombatOddsSpecific(pAttacker,pDefender,iNeededRoundsDefender-1,n_D);
					if (ACO_debug)
					{
						szTempBuffer.Format(L"+%d * %.2f%%  (Def %d) (%d:%d)",
							( (pAttacker->currHitPoints()) - (iNeededRoundsDefender-1)*iDamageToAttacker),100.0f*getCombatOddsSpecific(pAttacker,pDefender,iNeededRoundsDefender-1,n_D),(pDefender->currHitPoints())-n_D*iDamageToDefender,iNeededRoundsDefender-1,n_D);
						szString.append(NEWLINE);
						szString.append(szTempBuffer.GetCString());
					}
				}
			}
			// finished with the attacker HP I think.

			if (ACO_debug)
			{
				szTempBuffer.Format(L"E[HP DEFENDER]\nOdds that attacker dies or retreats");
				szString.append(NEWLINE);
				szString.append(szTempBuffer.GetCString());
			}
			for (int n_D = 0; n_D < iNeededRoundsAttacker; n_D++)
			{
				//prob_defend[n_D] = getCombatOddsSpecific(pAttacker,pDefender,iNeededRoundsDefender,n_D);//attacker dies
				//prob_defend[n_D] += getCombatOddsSpecific(pAttacker,pDefender,iNeededRoundsDefender-1,n_D);//attacker retreats
				E_HP_Def += ( (pDefender->currHitPoints()) - n_D*iDamageToDefender) * (getCombatOddsSpecific(pAttacker,pDefender,iNeededRoundsDefender,n_D)+getCombatOddsSpecific(pAttacker,pDefender,iNeededRoundsDefender-1,n_D));
				if (ACO_debug)
				{
					szTempBuffer.Format(L"+%d * %.2f%%  (Att 0 or %d) (%d:%d)",
						( (pDefender->currHitPoints()) - n_D*iDamageToDefender),100.0f*(getCombatOddsSpecific(pAttacker,pDefender,iNeededRoundsDefender,n_D)+getCombatOddsSpecific(pAttacker,pDefender,iNeededRoundsDefender-1,n_D)),(pAttacker->currHitPoints())-(iNeededRoundsDefender-1)*iDamageToAttacker,iNeededRoundsDefender,n_D);
					szString.append(NEWLINE);
					szString.append(szTempBuffer.GetCString());
				}
			}
			prob_bottom_Def_HP = getCombatOddsSpecific(pAttacker,pDefender,iNeededRoundsDefender,iNeededRoundsAttacker-1);
			//prob_bottom_Def_HP += getCombatOddsSpecific(pAttacker,pDefender,iNeededRoundsDefender-1,iNeededRoundsAttacker-1);
			E_HP_Def_Defeat = E_HP_Def;
			E_HP_Def_Withdraw = 0.0f;

			if (pAttacker->combatLimit() < (pDefender->maxHitPoints() ))//if attacker has a combatLimit (eg. catapult)
			{
				if (pAttacker->combatLimit() == iDamageToDefender*(iNeededRoundsAttacker-1) )
				{
					//Then we have an odd situation because the last successful hit by an attacker will do 0 damage, and doing either iNeededRoundsAttacker or iNeededRoundsAttacker-1 will cause the same damage
					if (ACO_debug)
					{
						szTempBuffer.Format(L"Odds that attacker withdraws at combatLimit (abnormal)");
						szString.append(NEWLINE);
						szString.append(szTempBuffer.GetCString());
					}
					for (int n_A = 0; n_A < iNeededRoundsDefender; n_A++)
					{
						//prob_defend[iNeededRoundsAttacker-1] += getCombatOddsSpecific(pAttacker,pDefender,n_A,iNeededRoundsAttacker);//this is the defender at the combatLimit
						E_HP_Def += (float)iDefenderHitLimit * getCombatOddsSpecific(pAttacker,pDefender,n_A,iNeededRoundsAttacker);
						//should be the same as
						//E_HP_Def += ( (pDefender->currHitPoints()) - (iNeededRoundsAttacker-1)*iDamageToDefender) * getCombatOddsSpecific(pAttacker,pDefender,n_A,iNeededRoundsAttacker);
						E_HP_Def_Withdraw += (float)iDefenderHitLimit * getCombatOddsSpecific(pAttacker,pDefender,n_A,iNeededRoundsAttacker);
						prob_bottom_Def_HP += getCombatOddsSpecific(pAttacker,pDefender,n_A,iNeededRoundsAttacker);
						if (ACO_debug)
						{
							szTempBuffer.Format(L"+%d * %.2f%%  (Att %d) (%d:%d)",
								iDefenderHitLimit,100.0f*getCombatOddsSpecific(pAttacker,pDefender,n_A,iNeededRoundsAttacker),100-n_A*iDamageToAttacker,n_A,iNeededRoundsAttacker);
							szString.append(NEWLINE);
							szString.append(szTempBuffer.GetCString());
						}
					}
				}
				else // normal situation
				{
					if (ACO_debug)
					{
						szTempBuffer.Format(L"Odds that attacker withdraws at combatLimit (normal)",pAttacker->combatLimit());
						szString.append(NEWLINE);
						szString.append(szTempBuffer.GetCString());
					}

					for (int n_A = 0; n_A < iNeededRoundsDefender; n_A++)
					{

						E_HP_Def += (float)iDefenderHitLimit * getCombatOddsSpecific(pAttacker,pDefender,n_A,iNeededRoundsAttacker);
						E_HP_Def_Withdraw += (float)iDefenderHitLimit * getCombatOddsSpecific(pAttacker,pDefender,n_A,iNeededRoundsAttacker);
						prob_bottom_Def_HP += getCombatOddsSpecific(pAttacker,pDefender,n_A,iNeededRoundsAttacker);
						if (ACO_debug)
						{
							szTempBuffer.Format(L"+%d * %.2f%%  (Att %d) (%d:%d)",
								iDefenderHitLimit,100.0f*getCombatOddsSpecific(pAttacker,pDefender,n_A,iNeededRoundsAttacker),GC.getMAX_HIT_POINTS()-n_A*iDamageToAttacker,n_A,iNeededRoundsAttacker);
							szString.append(NEWLINE);
							szString.append(szTempBuffer.GetCString());
						}
					}//for
				}//else
			}
			if (ACO_debug)
			{
				szString.append(NEWLINE);
			}

			float Scaling_Factor = 1.6f;//how many pixels per 1% of odds

			float AttackerKillOdds = 0.0f;
			float PullOutOdds = 0.0f;//Withdraw odds
			float RetreatOdds = 0.0f;
			float DefenderKillOdds = 0.0f;

			float CombatRatio = ((float)(pAttacker->currCombatStr(NULL, NULL))) / ((float)(pDefender->currCombatStr(pPlot, pAttacker)));
			// THE ALL-IMPORTANT COMBATRATIO


			float AttXP = (pDefender->attackXPValue())/CombatRatio;
			float DefXP = (pAttacker->defenseXPValue())*CombatRatio;// These two values are simply for the Unrounded XP display

			// General odds
			if (pAttacker->combatLimit() == (pDefender->maxHitPoints() )) //ie. we can kill the defender... I hope this is the most general form
			{
				//float AttackerKillOdds = 0.0f;
				for (int n_A = 0; n_A < iNeededRoundsDefender; n_A++)
				{
					AttackerKillOdds += getCombatOddsSpecific(pAttacker,pDefender,n_A,iNeededRoundsAttacker);
				}//for
			}
			else
			{
				// else we cannot kill the defender (eg. catapults attacking)
				for (int n_A = 0; n_A < iNeededRoundsDefender; n_A++)
				{
					PullOutOdds += getCombatOddsSpecific(pAttacker,pDefender,n_A,iNeededRoundsAttacker);
				}//for
			}
			if ((pAttacker->withdrawalProbability()) > 0)
			{
				for (int n_D = 0; n_D < iNeededRoundsAttacker; n_D++)
				{
					RetreatOdds += getCombatOddsSpecific(pAttacker,pDefender,iNeededRoundsDefender-1,n_D);
				}//for
			}
			for (int n_D = 0; n_D < iNeededRoundsAttacker; n_D++)
			{
				DefenderKillOdds += getCombatOddsSpecific(pAttacker,pDefender,iNeededRoundsDefender,n_D);
			}//for
			//DefenderKillOdds = 1.0f - (AttackerKillOdds + RetreatOdds + PullOutOdds);//this gives slight negative numbers sometimes, I think



			if (iView & getBugOptionINT("ACO__ShowSurvivalOdds", 3, "ACO_SHOW_SURVIVAL_ODDS"))
			{
				szTempBuffer.Format(L"%.2f%%",100.0f*(AttackerKillOdds+RetreatOdds+PullOutOdds));
				szTempBuffer2.Format(L"%.2f%%", 100.0f*(RetreatOdds+PullOutOdds+DefenderKillOdds));
				szString.append(gDLL->getText("TXT_ACO_SurvivalOdds"));
				szString.append(gDLL->getText("TXT_ACO_VS", szTempBuffer.GetCString(), szTempBuffer2.GetCString()));
				szString.append(NEWLINE);
			}

			if (pAttacker->withdrawalProbability()>=100)
			{
				// a rare situation indeed
				szString.append(gDLL->getText("TXT_ACO_SurvivalGuaranteed"));
				szString.append(NEWLINE);
			}

			//CvWString szTempBuffer2; // moved to elsewhere in the code (earlier)
			//CvWString szBuffer; // duplicate

			float prob1 = 100.0f*(AttackerKillOdds + PullOutOdds);//up to win odds
			float prob2 = prob1 + 100.0f*RetreatOdds;//up to retreat odds

			float prob = 100.0f*(AttackerKillOdds+RetreatOdds+PullOutOdds);
			int pixels_left = 199;// 1 less than 200 to account for right end bar
			int pixels;
			int fullBlocks;
			int lastBlock;

			pixels = (2 * ((int)(prob1 + 0.5)))-1;  // 1% per pixel // subtracting one to account for left end bar
			fullBlocks = pixels / 10;
			lastBlock = pixels % 10;

			szString.append(L"<img=Art/ACO/green_bar_left_end.dds>");
			for (int i = 0; i < fullBlocks; ++i)
			{
				szString.append(L"<img=Art/ACO/green_bar_10.dds>");
				pixels_left -= 10;
			}
			if (lastBlock > 0)
			{
				szTempBuffer2.Format(L"<img=Art/ACO/green_bar_%d.dds>", lastBlock);
				szString.append(szTempBuffer2);
				pixels_left-= lastBlock;
			}


			pixels = 2 * ((int)(prob2 + 0.5)) - (pixels+1);//the number up to the next one...
			fullBlocks = pixels / 10;
			lastBlock = pixels % 10;
			for (int i = 0; i < fullBlocks; ++i)
			{
				szString.append(L"<img=Art/ACO/yellow_bar_10.dds>");
				pixels_left -= 10;
			}
			if (lastBlock > 0)
			{
				szTempBuffer2.Format(L"<img=Art/ACO/yellow_bar_%d.dds>", lastBlock);
				szString.append(szTempBuffer2);
				pixels_left-= lastBlock;
			}

			fullBlocks = pixels_left / 10;
			lastBlock = pixels_left % 10;
			for (int i = 0; i < fullBlocks; ++i)
			{
				szString.append(L"<img=Art/ACO/red_bar_10.dds>");
			}
			if (lastBlock > 0)
			{
				szTempBuffer2.Format(L"<img=Art/ACO/red_bar_%d.dds>", lastBlock);
				szString.append(szTempBuffer2);
			}

			szString.append(L"<img=Art/ACO/red_bar_right_end.dds> ");


			szString.append(NEWLINE);
			if (pAttacker->combatLimit() == (pDefender->maxHitPoints() ))
			{
				szTempBuffer.Format(L": " SETCOLR L"%.2f%% " L"%d" ENDCOLR,
					TEXT_COLOR("COLOR_POSITIVE_TEXT"),100.0f*AttackerKillOdds,iExperience);
				szString.append(gDLL->getText("TXT_ACO_Victory"));
				szString.append(szTempBuffer.GetCString());
				if (iAttackerExperienceModifier > 0)
				{
					szTempBuffer.Format(SETCOLR L"+%d" ENDCOLR,TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"),iBonusAttackerXP);
					szString.append(szTempBuffer.GetCString());
				}

				szString.append(gDLL->getText("TXT_KEY_COLOR_POSITIVE"));
				szString.append(gDLL->getText("TXT_ACO_XP"));
				szString.append(gDLL->getText("TXT_KEY_COLOR_REVERT"));
				szString.append("  (");
				szString.append(gDLL->getText("TXT_KEY_COLOR_POSITIVE"));
				szTempBuffer.Format(L"%.1f",
					E_HP_Att_Victory/AttackerKillOdds);
				szString.append(szTempBuffer.GetCString());
				szString.append(gDLL->getText("TXT_ACO_HP"));
				szString.append(gDLL->getText("TXT_KEY_COLOR_REVERT"));
			}
			else
			{
				szTempBuffer.Format(L": " SETCOLR L"%.2f%% " L"%d" ENDCOLR,
					TEXT_COLOR("COLOR_POSITIVE_TEXT"),100.0f*PullOutOdds,GC.getDefineINT("EXPERIENCE_FROM_WITHDRAWL"));
				//iExperience,TEXT_COLOR("COLOR_POSITIVE_TEXT"), E_HP_Att_Victory/AttackerKillOdds);
				szString.append(gDLL->getText("TXT_ACO_Withdraw"));
				szString.append(szTempBuffer.GetCString());
				if (iAttackerExperienceModifier > 0)
				{
					szTempBuffer.Format(SETCOLR L"+%d" ENDCOLR,TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"),iBonusWithdrawXP);
					szString.append(szTempBuffer.GetCString());
				}

				szString.append(gDLL->getText("TXT_KEY_COLOR_POSITIVE"));
				szString.append(gDLL->getText("TXT_ACO_XP"));
				szString.append(gDLL->getText("TXT_KEY_COLOR_REVERT"));
				szString.append("  (");
				szString.append(gDLL->getText("TXT_KEY_COLOR_POSITIVE"));
				szTempBuffer.Format(L"%.1f",E_HP_Att_Withdraw/PullOutOdds);
				szString.append(szTempBuffer.GetCString());
				szString.append(gDLL->getText("TXT_ACO_HP"));
				szString.append(gDLL->getText("TXT_KEY_COLOR_REVERT"));
				szString.append(",");
				szString.append(gDLL->getText("TXT_KEY_COLOR_NEGATIVE"));
				szTempBuffer.Format(L"%d",iDefenderHitLimit);
				szString.append(szTempBuffer.GetCString());
				szString.append(gDLL->getText("TXT_ACO_HP"));
				szString.append(gDLL->getText("TXT_KEY_COLOR_REVERT"));
			}
			szString.append(")");

			if (iDefenderOdds == 0)
			{
				szString.append(gDLL->getText("TXT_ACO_GuaranteedNoDefenderHit"));
				DefenderKillOdds = 0.0f;
			}

			if ((pAttacker->withdrawalProbability()) > 0)//if there are retreat odds
			{
				szString.append(NEWLINE);
				szTempBuffer.Format(L": " SETCOLR L"%.2f%% " ENDCOLR SETCOLR L"%d" ENDCOLR,
					TEXT_COLOR("COLOR_UNIT_TEXT"),100.0f*RetreatOdds,TEXT_COLOR("COLOR_POSITIVE_TEXT"),GC.getDefineINT("EXPERIENCE_FROM_WITHDRAWL"));
				szString.append(gDLL->getText("TXT_ACO_Retreat"));
				szString.append(szTempBuffer.GetCString());
				if (iAttackerExperienceModifier > 0)
				{
					szTempBuffer.Format(SETCOLR L"+%d" ENDCOLR,TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"),iBonusWithdrawXP);
					szString.append(szTempBuffer.GetCString());
				}
				szString.append(gDLL->getText("TXT_KEY_COLOR_POSITIVE"));
				szString.append(gDLL->getText("TXT_ACO_XP"));
				szString.append(gDLL->getText("TXT_KEY_COLOR_REVERT"));
				szString.append("  (");
				szTempBuffer.Format(SETCOLR L"%d" ENDCOLR ,
					TEXT_COLOR("COLOR_UNIT_TEXT"),E_HP_Att_Retreat);
				szString.append(szTempBuffer.GetCString());
				szString.append(gDLL->getText("TXT_ACO_HP_NEUTRAL"));
				szString.append(")");
				//szString.append(gDLL->getText("TXT_KEY_COLOR_REVERT"));
			}

			szString.append(NEWLINE);
			szTempBuffer.Format(L": " SETCOLR L"%.2f%% " L"%d" ENDCOLR,
				TEXT_COLOR("COLOR_NEGATIVE_TEXT"),100.0f*DefenderKillOdds,iDefExperienceKill);
			szString.append(gDLL->getText("TXT_ACO_Defeat"));
			szString.append(szTempBuffer.GetCString());
			if (iDefenderExperienceModifier > 0)
			{
				szTempBuffer.Format(SETCOLR L"+%d" ENDCOLR,TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"),iBonusDefenderXP);
				szString.append(szTempBuffer.GetCString());
			}
			szString.append(gDLL->getText("TXT_KEY_COLOR_NEGATIVE"));
			szString.append(gDLL->getText("TXT_ACO_XP"));
			szString.append(gDLL->getText("TXT_KEY_COLOR_REVERT"));
			szString.append("  (");
			szString.append(gDLL->getText("TXT_KEY_COLOR_NEGATIVE"));
			szTempBuffer.Format(L"%.1f",
				(iDefenderOdds != 0 ? E_HP_Def_Defeat/(RetreatOdds+DefenderKillOdds):0.0));
			szString.append(szTempBuffer.GetCString());
			szString.append(gDLL->getText("TXT_ACO_HP"));
			szString.append(gDLL->getText("TXT_KEY_COLOR_REVERT"));
			szString.append(")");


			float HP_percent_cutoff = 0.5f; // Probabilities lower than this (in percent) will not be shown individually for the HP detail section.
			if (!getBugOptionBOOL("ACO__MergeShortBars", true, "ACO_MERGE_SHORT_BARS"))
			{
				HP_percent_cutoff = 0.0f;
			}
			int first_combined_HP_Att = 0;
			int first_combined_HP_Def = 0;
			int last_combined_HP;
			float combined_HP_sum = 0.0f;
			BOOL bIsCondensed = false;



			//START ATTACKER DETAIL HP HERE
			// Individual bars for each attacker HP outcome.
			if (iView & getBugOptionINT("ACO__ShowAttackerHealthBars", 2, "ACO_SHOW_ATTACKER_HEALTH_BARS"))
			{
				for (int n_A = 0; n_A < iNeededRoundsDefender-1; n_A++)
				{
					float prob = 100.0f*getCombatOddsSpecific(pAttacker,pDefender,n_A,iNeededRoundsAttacker);
					if (prob > HP_percent_cutoff || n_A==0)
					{
						if (bIsCondensed) // then we need to print the prev ones
						{
							int pixels = (int)(Scaling_Factor*combined_HP_sum + 0.5);  // 1% per pixel
							int fullBlocks = (pixels) / 10;
							int lastBlock = (pixels) % 10;
							//if(pixels>=2) {szString.append(L"<img=Art/ACO/green_bar_left_end.dds>");}
							szString.append(NEWLINE);
							szString.append(L"<img=Art/ACO/green_bar_left_end.dds>");
							for (int iI = 0; iI < fullBlocks; ++iI)
							{
								szString.append(L"<img=Art/ACO/green_bar_10.dds>");
							}
							if (lastBlock > 0)
							{
								szTempBuffer2.Format(L"<img=Art/ACO/green_bar_%d.dds>", lastBlock);
								szString.append(szTempBuffer2);
							}
							szString.append(L"<img=Art/ACO/green_bar_right_end.dds>");
							szString.append(L" ");

							szString.append(gDLL->getText("TXT_KEY_COLOR_POSITIVE"));
							if (last_combined_HP!=first_combined_HP_Att)
							{
								szTempBuffer.Format(L"%d",last_combined_HP);
								szString.append(szTempBuffer.GetCString());
								szString.append(gDLL->getText("TXT_ACO_HP"));
								szString.append(gDLL->getText("-"));
							}

							szTempBuffer.Format(L"%d",first_combined_HP_Att);
							szString.append(szTempBuffer.GetCString());
							szString.append(gDLL->getText("TXT_ACO_HP"));
							szString.append(gDLL->getText("TXT_KEY_COLOR_REVERT"));
							szTempBuffer.Format(L" %.2f%%",combined_HP_sum);
							szString.append(szTempBuffer.GetCString());

							bIsCondensed = false;//resetting
							combined_HP_sum = 0.0f;//resetting this variable
							last_combined_HP = 0;
						}

						szString.append(NEWLINE);
						int pixels = (int)(Scaling_Factor*prob + 0.5);  // 1% per pixel
						int fullBlocks = (pixels) / 10;
						int lastBlock = (pixels) % 10;
						szString.append(L"<img=Art/ACO/green_bar_left_end.dds>");
						for (int iI = 0; iI < fullBlocks; ++iI)
						{
							szString.append(L"<img=Art/ACO/green_bar_10.dds>");
						}
						if (lastBlock > 0)
						{
							szTempBuffer2.Format(L"<img=Art/ACO/green_bar_%d.dds>", lastBlock);
							szString.append(szTempBuffer2);
						}
						szString.append(L"<img=Art/ACO/green_bar_right_end.dds>");
						szString.append(L" ");

						szString.append(gDLL->getText("TXT_KEY_COLOR_POSITIVE"));
						szTempBuffer.Format(L"%d",
							((pAttacker->currHitPoints()) - n_A*iDamageToAttacker));
						szString.append(szTempBuffer.GetCString());
						szString.append(gDLL->getText("TXT_ACO_HP"));
						szString.append(gDLL->getText("TXT_KEY_COLOR_REVERT"));
						szTempBuffer.Format(L" %.2f%%",
							prob);
						szString.append(szTempBuffer.GetCString());
					}
					else // we add to the condensed list
					{
						bIsCondensed = true;
						first_combined_HP_Att = std::max(first_combined_HP_Att,((pAttacker->currHitPoints()) - n_A*iDamageToAttacker));
						last_combined_HP = ((pAttacker->currHitPoints()) - n_A*iDamageToAttacker);
						combined_HP_sum += prob;
					}
				}

				if (bIsCondensed) // then we need to print the prev ones
				{
					szString.append(NEWLINE);
					int pixels = (int)(Scaling_Factor*combined_HP_sum + 0.5);  // 1% per pixel
					int fullBlocks = (pixels) / 10;
					int lastBlock = (pixels) % 10;

					szString.append(L"<img=Art/ACO/green_bar_left_end.dds>");
					for (int iI = 0; iI < fullBlocks; ++iI)
					{
						szString.append(L"<img=Art/ACO/green_bar_10.dds>");
					}
					if (lastBlock > 0)
					{
						szTempBuffer2.Format(L"<img=Art/ACO/green_bar_%d.dds>", lastBlock);
						szString.append(szTempBuffer2);
					}

					szString.append(L"<img=Art/ACO/green_bar_right_end.dds>");
					szString.append(L" ");

					szString.append(gDLL->getText("TXT_KEY_COLOR_POSITIVE"));
					if (last_combined_HP!=first_combined_HP_Att)
					{
						szTempBuffer.Format(L"%d",last_combined_HP);
						szString.append(szTempBuffer.GetCString());
						szString.append(gDLL->getText("TXT_ACO_HP"));
						szString.append(gDLL->getText("-"));
					}
					szTempBuffer.Format(L"%d",first_combined_HP_Att);
					szString.append(szTempBuffer.GetCString());
					szString.append(gDLL->getText("TXT_ACO_HP"));
					szString.append(gDLL->getText("TXT_KEY_COLOR_REVERT"));
					szTempBuffer.Format(L" %.2f%%",combined_HP_sum);
					szString.append(szTempBuffer.GetCString());

					bIsCondensed = false;//resetting
					combined_HP_sum = 0.0f;//resetting this variable
					last_combined_HP = 0;
				}
				// At the moment I am not allowing the lowest Attacker HP value to be condensed, as it would be confusing if it includes retreat odds
				// I may include this in the future though, but probably only if retreat odds are zero.

				float prob_victory = 100.0f*getCombatOddsSpecific(pAttacker,pDefender,iNeededRoundsDefender-1,iNeededRoundsAttacker);
				float prob_retreat = 100.0f*RetreatOdds;

				szString.append(NEWLINE);
				int green_pixels = (int)(Scaling_Factor*prob_victory + 0.5);
				int yellow_pixels = (int)(Scaling_Factor*(prob_retreat+prob_victory) + 0.5) - green_pixels;//makes the total length of the bar more accurate - more important than the length of the pieces
				green_pixels+=1;//we put an extra 2 on every one of the bar pixel counts
				if (yellow_pixels>=1)
				{
					yellow_pixels+=1;
				}
				else
				{
					green_pixels+=1;
				}
				szString.append(L"<img=Art/ACO/green_bar_left_end.dds>");
				green_pixels--;

				green_pixels--;//subtracting off the right end
				int fullBlocks = green_pixels / 10;
				int lastBlock = green_pixels % 10;
				for (int iI = 0; iI < fullBlocks; ++iI)
				{
					szString.append(L"<img=Art/ACO/green_bar_10.dds>");
				}//for
				if (lastBlock > 0)
				{
					szTempBuffer2.Format(L"<img=Art/ACO/green_bar_%d.dds>", lastBlock);
					szString.append(szTempBuffer2);
				}//if
				if (yellow_pixels>=1)// then there will at least be a right end yellow pixel
				{
					yellow_pixels--;//subtracting off right end
					fullBlocks = yellow_pixels / 10;
					lastBlock = yellow_pixels % 10;
					for (int iI = 0; iI < fullBlocks; ++iI)
					{
						szString.append(L"<img=Art/ACO/yellow_bar_10.dds>");
					}//for
					if (lastBlock > 0)
					{
						szTempBuffer2.Format(L"<img=Art/ACO/yellow_bar_%d.dds>", lastBlock);
						szString.append(szTempBuffer2);
					}
					szString.append(L"<img=Art/ACO/yellow_bar_right_end.dds>");
					//finished
				}
				else
				{
					szString.append(L"<img=Art/ACO/green_bar_right_end.dds>");
					//finished
				}//else if

				szString.append(L" ");
				szString.append(gDLL->getText("TXT_KEY_COLOR_POSITIVE"));
				szTempBuffer.Format(L"%d",((pAttacker->currHitPoints()) - (iNeededRoundsDefender-1)*iDamageToAttacker));
				szString.append(szTempBuffer.GetCString());
				szString.append(gDLL->getText("TXT_ACO_HP"));
				szString.append(gDLL->getText("TXT_KEY_COLOR_REVERT"));
				szTempBuffer.Format(L" %.2f%%",prob_victory+prob_retreat);
				szString.append(szTempBuffer.GetCString());
			}
			//END ATTACKER DETAIL HP HERE


			//START DEFENDER DETAIL HP HERE
			first_combined_HP_Def = pDefender->currHitPoints();
			if (iView & getBugOptionINT("ACO__ShowDefenderHealthBars", 2, "ACO_SHOW_DEFENDER_HEALTH_BARS"))
			{
				float prob = 0.0f;
				int def_HP;
				for (int n_D = iNeededRoundsAttacker; n_D >= 1; n_D--)//
				{
					if (pAttacker->combatLimit() >= pDefender->maxHitPoints())// a unit with a combat limit
					{
						if (n_D == iNeededRoundsAttacker)
						{
							n_D--;//we don't need to do HP for when the unit is dead.
						}
					}

					def_HP = std::max((pDefender->currHitPoints()) - n_D*iDamageToDefender,(pDefender->maxHitPoints()  - pAttacker->combatLimit()));

					if ( (pDefender->maxHitPoints() - pAttacker->combatLimit() ) == pDefender->currHitPoints() - (n_D-1)*iDamageToDefender)
					{
						// if abnormal
						if (n_D == iNeededRoundsAttacker)
						{
							n_D--;
							def_HP = (pDefender->maxHitPoints()  - pAttacker->combatLimit());
							prob += 100.0f*PullOutOdds;
							prob += 100.0f*(getCombatOddsSpecific(pAttacker,pDefender,iNeededRoundsDefender,n_D)+(getCombatOddsSpecific(pAttacker,pDefender,iNeededRoundsDefender-1,n_D)));
						}
					}
					else
					{
						//not abnormal
						if (n_D == iNeededRoundsAttacker)
						{
							prob += 100.0f*PullOutOdds;
						}
						else
						{
							prob += 100.0f*(getCombatOddsSpecific(pAttacker,pDefender,iNeededRoundsDefender,n_D)+(getCombatOddsSpecific(pAttacker,pDefender,iNeededRoundsDefender-1,n_D)));
						}
					}

					if (prob > HP_percent_cutoff || (pAttacker->combatLimit()<pDefender->maxHitPoints() && (n_D==iNeededRoundsAttacker)))
					{
						if (bIsCondensed) // then we need to print the prev ones
						{
							szString.append(NEWLINE);

							int pixels = (int)(Scaling_Factor*combined_HP_sum + 0.5);  // 1% per pixel
							int fullBlocks = (pixels) / 10;
							int lastBlock = (pixels) % 10;
							szString.append(L"<img=Art/ACO/red_bar_left_end.dds>");
							for (int iI = 0; iI < fullBlocks; ++iI)
							{
								szString.append(L"<img=Art/ACO/red_bar_10.dds>");
							}
							if (lastBlock > 0)
							{
								szTempBuffer2.Format(L"<img=Art/ACO/red_bar_%d.dds>", lastBlock);
								szString.append(szTempBuffer2);
							}
							szString.append(L"<img=Art/ACO/red_bar_right_end.dds>");
							szString.append(L" ");
							szString.append(gDLL->getText("TXT_KEY_COLOR_NEGATIVE"));
							szTempBuffer.Format(L"%dHP",first_combined_HP_Def);
							szString.append(szTempBuffer.GetCString());
							szString.append(gDLL->getText("TXT_ACO_HP"));
							if (first_combined_HP_Def!=last_combined_HP)
							{
								szString.append("-");
								szTempBuffer.Format(L"%d",last_combined_HP);
								szString.append(szTempBuffer.GetCString());
								szString.append(gDLL->getText("TXT_ACO_HP"));
							}
							szString.append(gDLL->getText("TXT_KEY_COLOR_REVERT"));
							szTempBuffer.Format(L" %.2f%%",
								combined_HP_sum);
							szString.append(szTempBuffer.GetCString());

							bIsCondensed = false;//resetting
							combined_HP_sum = 0.0f;//resetting this variable
						}

						szString.append(NEWLINE);
						int pixels = (int)(Scaling_Factor*prob + 0.5);  // 1% per pixel
						int fullBlocks = (pixels) / 10;
						int lastBlock = (pixels) % 10;
						//if(pixels>=2) // this is now guaranteed by the way we define number of pixels
						//{
						szString.append(L"<img=Art/ACO/red_bar_left_end.dds>");
						for (int iI = 0; iI < fullBlocks; ++iI)
						{
							szString.append(L"<img=Art/ACO/red_bar_10.dds>");
						}
						if (lastBlock > 0)
						{
							szTempBuffer2.Format(L"<img=Art/ACO/red_bar_%d.dds>", lastBlock);
							szString.append(szTempBuffer2);
						}
						szString.append(L"<img=Art/ACO/red_bar_right_end.dds>");
						//}
						szString.append(L" ");

						szTempBuffer.Format(SETCOLR L"%d" ENDCOLR,
							TEXT_COLOR("COLOR_NEGATIVE_TEXT"),def_HP);
						szString.append(szTempBuffer.GetCString());
						szString.append(gDLL->getText("TXT_KEY_COLOR_NEGATIVE"));
						szString.append(gDLL->getText("TXT_ACO_HP"));
						szString.append(gDLL->getText("TXT_KEY_COLOR_REVERT"));
						szTempBuffer.Format(L" %.2f%%",prob);
						szString.append(szTempBuffer.GetCString());
					}
					else
					{
						bIsCondensed = true;
						first_combined_HP_Def = (std::min(first_combined_HP_Def,def_HP));
						last_combined_HP = std::max(((pDefender->currHitPoints()) - n_D*iDamageToDefender),pDefender->maxHitPoints()-pAttacker->combatLimit());
						combined_HP_sum += prob;
					}
					prob = 0.0f;
				}//for n_D

				if (bIsCondensed && iNeededRoundsAttacker>1) // then we need to print the prev ones
					// the reason we need iNeededRoundsAttacker to be greater than 1 is that if it's equal to 1 then we end up with the defender detailed HP bar show up twice, because it will also get printed below
				{
					szString.append(NEWLINE);
					int pixels = (int)(Scaling_Factor*combined_HP_sum + 0.5);  // 1% per pixel
					int fullBlocks = (pixels) / 10;
					int lastBlock = (pixels) % 10;
					//if(pixels>=2) {szString.append(L"<img=Art/ACO/green_bar_left_end.dds>");}
					szString.append(L"<img=Art/ACO/red_bar_left_end.dds>");
					for (int iI = 0; iI < fullBlocks; ++iI)
					{
						szString.append(L"<img=Art/ACO/red_bar_10.dds>");
					}
					if (lastBlock > 0)
					{
						szTempBuffer2.Format(L"<img=Art/ACO/red_bar_%d.dds>", lastBlock);
						szString.append(szTempBuffer2);
					}
					szString.append(L"<img=Art/ACO/red_bar_right_end.dds>");
					szString.append(L" ");
					szString.append(gDLL->getText("TXT_KEY_COLOR_NEGATIVE"));
					szTempBuffer.Format(L"%d",first_combined_HP_Def);
					szString.append(szTempBuffer.GetCString());
					szString.append(gDLL->getText("TXT_ACO_HP"));
					if (first_combined_HP_Def != last_combined_HP)
					{
						szTempBuffer.Format(L"-%d",last_combined_HP);
						szString.append(szTempBuffer.GetCString());
						szString.append(gDLL->getText("TXT_ACO_HP"));
					}
					szString.append(gDLL->getText("TXT_KEY_COLOR_REVERT"));
					szTempBuffer.Format(L" %.2f%%",combined_HP_sum);
					szString.append(szTempBuffer.GetCString());

					bIsCondensed = false;//resetting
					combined_HP_sum = 0.0f;//resetting this variable
				}

				//print the unhurt value...always

				prob = 100.0f*(getCombatOddsSpecific(pAttacker,pDefender,iNeededRoundsDefender,0)+(getCombatOddsSpecific(pAttacker,pDefender,iNeededRoundsDefender-1,0)));
				int pixels = (int)(Scaling_Factor*prob + 0.5);  // 1% per pixel
				int fullBlocks = (pixels) / 10;
				int lastBlock = (pixels) % 10;

				szString.append(NEWLINE);
				szString.append(L"<img=Art/ACO/red_bar_left_end.dds>");
				for (int iI = 0; iI < fullBlocks; ++iI)
				{
					szString.append(L"<img=Art/ACO/red_bar_10.dds>");
				}
				if (lastBlock > 0)
				{
					szTempBuffer2.Format(L"<img=Art/ACO/red_bar_%d.dds>", lastBlock);
					szString.append(szTempBuffer2);
				}
				szString.append(L"<img=Art/ACO/red_bar_right_end.dds>");
				szString.append(L" ");
				szString.append(gDLL->getText("TXT_KEY_COLOR_NEGATIVE"));
				szTempBuffer.Format(L"%d",pDefender->currHitPoints());
				szString.append(szTempBuffer.GetCString());
				szString.append(gDLL->getText("TXT_ACO_HP"));
				szString.append(gDLL->getText("TXT_KEY_COLOR_REVERT"));
				szTempBuffer.Format(L" %.2f%%",prob);
				szString.append(szTempBuffer.GetCString());
			}
			//END DEFENDER DETAIL HP HERE

			szString.append(NEWLINE);

			if (iView & getBugOptionINT("ACO__ShowBasicInfo", 3, "ACO_SHOW_BASIC_INFO"))
			{
				szTempBuffer.Format(SETCOLR L"%d" ENDCOLR L", " SETCOLR L"%d " ENDCOLR,
					TEXT_COLOR("COLOR_POSITIVE_TEXT"), iDamageToDefender, TEXT_COLOR("COLOR_NEGATIVE_TEXT"), iDamageToAttacker);
				szString.append(NEWLINE);
				szString.append(szTempBuffer.GetCString());
				szString.append(gDLL->getText("TXT_ACO_HP"));
				szString.append(" ");
				szString.append(gDLL->getText("TXT_ACO_MULTIPLY"));
				szTempBuffer.Format(L" " SETCOLR L"%d" ENDCOLR L", " SETCOLR L"%d " ENDCOLR,
					TEXT_COLOR("COLOR_POSITIVE_TEXT"),iNeededRoundsAttacker,TEXT_COLOR("COLOR_NEGATIVE_TEXT"),
					iNeededRoundsDefender);
				szString.append(szTempBuffer.GetCString());
				szString.append(gDLL->getText("TXT_ACO_HitsAt"));
				szTempBuffer.Format(SETCOLR L" %.1f%%" ENDCOLR,
					TEXT_COLOR("COLOR_POSITIVE_TEXT"),float(iAttackerOdds)*100.0f / float(GC.getDefineINT("COMBAT_DIE_SIDES")));
				szString.append(szTempBuffer.GetCString());
			}
			if (!(iView & getBugOptionINT("ACO__ShowExperienceRange", 2, "ACO_SHOW_EXPERIENCE_RANGE")) || (pAttacker->combatLimit() < (pDefender->maxHitPoints() ))) //medium and high only
			{
				if (iView & getBugOptionINT("ACO__ShowBasicInfo", 3, "ACO_SHOW_BASIC_INFO"))
				{
					szTempBuffer.Format(L". R=" SETCOLR L"%.2f" ENDCOLR,
						TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"),CombatRatio);
					szString.append(szTempBuffer.GetCString());
				}
			}
			else
			{
				//we do an XP range display
				//This should hopefully now work for any max and min XP values.

				if (pAttacker->combatLimit() == (pDefender->maxHitPoints() ))
				{
					FAssert(GC.getDefineINT("MAX_EXPERIENCE_PER_COMBAT") > GC.getDefineINT("MIN_EXPERIENCE_PER_COMBAT")); //ensuring the differences is at least 1
					int size = GC.getDefineINT("MAX_EXPERIENCE_PER_COMBAT") - GC.getDefineINT("MIN_EXPERIENCE_PER_COMBAT");
					std::vector<float> CombatRatioThresholds(size);

					for (int i = 0; i < size; i++) //setup the array
					{
						CombatRatioThresholds[i] = ((float)(pDefender->attackXPValue()))/((float)(GC.getDefineINT("MAX_EXPERIENCE_PER_COMBAT")-i));
						//For standard game, this is the list created:
						//  {4/10, 4/9, 4/8,
						//   4/7, 4/6, 4/5,
						//   4/4, 4/3, 4/2}
					}
					for (int i = size-1; i >= 0; i--) // find which range we are in
					{
						//starting at i = 8, going through to i = 0
						if (CombatRatio>CombatRatioThresholds[i])
						{

							if (i== (size-1) )//highest XP value already
							{
								szString.append(NEWLINE);
								szTempBuffer.Format(L"(%.2f:%d",
									CombatRatioThresholds[i],GC.getDefineINT("MIN_EXPERIENCE_PER_COMBAT")+1);
								szString.append(szTempBuffer.GetCString());
								szString.append(gDLL->getText("TXT_ACO_XP"));
								szTempBuffer.Format(L"), (R=" SETCOLR L"%.2f" ENDCOLR L":%d",
									TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"),CombatRatio,iExperience);
								szString.append(szTempBuffer.GetCString());
								szString.append(gDLL->getText("TXT_ACO_XP"));
								szString.append(")");
							}
							else // normal situation
							{
								szString.append(NEWLINE);
								szTempBuffer.Format(L"(%.2f:%d",
									CombatRatioThresholds[i],GC.getDefineINT("MAX_EXPERIENCE_PER_COMBAT")-i);
								szString.append(szTempBuffer.GetCString());
								szString.append(gDLL->getText("TXT_ACO_XP"));
								szTempBuffer.Format(L"), (R=" SETCOLR L"%.2f" ENDCOLR L":%d",
									TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"),CombatRatio,GC.getDefineINT("MAX_EXPERIENCE_PER_COMBAT")-(i+1));
								szString.append(szTempBuffer.GetCString());
								szString.append(gDLL->getText("TXT_ACO_XP"));
								szTempBuffer.Format(L"), (>%.2f:%d",
									CombatRatioThresholds[i+1],GC.getDefineINT("MAX_EXPERIENCE_PER_COMBAT")-(i+2));
								szString.append(szTempBuffer.GetCString());
								szString.append(gDLL->getText("TXT_ACO_XP"));
								szString.append(")");
							}
							break;

						}
						else//very rare (ratio less than or equal to 0.4)
						{
							if (i==0)//maximum XP
							{
								szString.append(NEWLINE);
								szTempBuffer.Format(L"(R=" SETCOLR L"%.2f" ENDCOLR L":%d",
									TEXT_COLOR("COLOR_POSITIVE_TEXT"),CombatRatio,GC.getDefineINT("MAX_EXPERIENCE_PER_COMBAT"));
								szString.append(szTempBuffer.GetCString());

								szTempBuffer.Format(L"), (>%.2f:%d",
									CombatRatioThresholds[i],GC.getDefineINT("MAX_EXPERIENCE_PER_COMBAT")-1);
								szString.append(szTempBuffer.GetCString());
								szString.append(gDLL->getText("TXT_ACO_XP"));
								szString.append(")");
								break;
							}//if
						}// else if
					}//for
					//throw away the array
				}//if
			} // else if
			//Finished Showing XP range display


			if (iView & getBugOptionINT("ACO__ShowAverageHealth", 2, "ACO_SHOW_AVERAGE_HEALTH"))
			{
				szTempBuffer.Format(L"%.1f",E_HP_Att);
				szTempBuffer2.Format(L"%.1f",E_HP_Def);
				szString.append(gDLL->getText("TXT_ACO_AverageHP"));
				szString.append(gDLL->getText("TXT_ACO_VS", szTempBuffer.GetCString(), szTempBuffer2.GetCString()));
			}

			if (iView & getBugOptionINT("ACO__ShowUnharmedOdds", 2, "ACO_SHOW_UNHARMED_ODDS"))
			{
				szTempBuffer.Format(L"%.2f%%",100.0f*AttackerUnharmed);
				szTempBuffer2.Format(L"%.2f%%",100.0f*DefenderUnharmed);
				szString.append(gDLL->getText("TXT_ACO_Unharmed"));
				szString.append(gDLL->getText("TXT_ACO_VS", szTempBuffer.GetCString(), szTempBuffer2.GetCString()));
			}

			if (iView & getBugOptionINT("ACO__ShowUnroundedExperience", 0, "ACO_SHOW_UNROUNDED_EXPERIENCE"))
			{
				szTempBuffer.Format(L"%.2f", AttXP);
				szTempBuffer2.Format(L"%.2f", DefXP);
				szString.append(gDLL->getText("TXT_ACO_UnroundedXP"));
				szString.append(gDLL->getText("TXT_ACO_VS", szTempBuffer.GetCString(), szTempBuffer2.GetCString()));
			}

			szString.append(NEWLINE);
			if (iView & getBugOptionINT("ACO__ShowShiftInstructions", 1, "ACO_SHOW_SHIFT_INSTRUCTIONS"))
			{
				szString.append(gDLL->getText("TXT_ACO_PressSHIFT"));
				szString.append(NEWLINE);
			}

		}//if ACO_enabled
	}

	//////////

	if (ACO_enabled)
	{
		szString.append(NEWLINE);

		szTempBuffer.Format(L"%.2f",
			((pAttacker->getDomainType() == DOMAIN_AIR) ? pAttacker->airCurrCombatStrFloat(pDefender) : pAttacker->currCombatStrFloat(NULL, NULL)));

		if (pAttacker->isHurt())
		{
			szTempBuffer.append(L" (");
			szTempBuffer.append(gDLL->getText("TXT_ACO_INJURED_HP",
				pAttacker->currHitPoints(),
				pAttacker->maxHitPoints()));
			szTempBuffer.append(L")");
		}


		szTempBuffer2.Format(L"%.2f",
			pDefender->currCombatStrFloat(pPlot, pAttacker)
			);

		if (pDefender->isHurt())
		{
			szTempBuffer2.append(L" (");
			szTempBuffer2.append(gDLL->getText("TXT_ACO_INJURED_HP",
				pDefender->currHitPoints(),
				pDefender->maxHitPoints()));
			szTempBuffer2.append(L")");
		}

		szString.append(gDLL->getText("TXT_ACO_VS", szTempBuffer.GetCString(), szTempBuffer2.GetCString()));

		if (((!(pDefender->immuneToFirstStrikes())) && (pAttacker->maxFirstStrikes() > 0)) || (pAttacker->maxCombatStr(NULL,NULL)!=pAttacker->baseCombatStr()*100))
		{
			//if attacker uninjured strength is not the same as base strength (i.e. modifiers are in effect) or first strikes exist, then
			if (getBugOptionBOOL("ACO__ShowModifierLabels", false, "ACO_SHOW_MODIFIER_LABELS"))
			{
				szString.append(gDLL->getText("TXT_ACO_AttackModifiers"));
			}
		}//if
		if ((iView & getBugOptionINT("ACO__ShowAttackerInfo", 0, "ACO_SHOW_ATTACKER_INFO")))
		{
			szString.append(NEWLINE);
			setUnitHelp(szString, pAttacker, true, true);
		}



		szString.append(gDLL->getText("TXT_KEY_COLOR_POSITIVE"));

		szString.append(L' ');//XXX

		if (!(pDefender->immuneToFirstStrikes()))
		{
			if (pAttacker->maxFirstStrikes() > 0)
			{
				if (pAttacker->firstStrikes() == pAttacker->maxFirstStrikes())
				{
					if (pAttacker->firstStrikes() == 1)
					{
						szString.append(NEWLINE);
						szString.append(gDLL->getText("TXT_KEY_UNIT_ONE_FIRST_STRIKE"));
					}
					else
					{
						szString.append(NEWLINE);
						szString.append(gDLL->getText("TXT_KEY_UNIT_NUM_FIRST_STRIKES", pAttacker->firstStrikes()));
					}
				}
				else
				{
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_UNIT_FIRST_STRIKE_CHANCES", pAttacker->firstStrikes(), pAttacker->maxFirstStrikes()));
				}
			}
		}

		iModifier = pAttacker->getExtraCombatPercent();

		if (iModifier != 0)
		{
			szString.append(NEWLINE);
			szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_EXTRA_STRENGTH", iModifier));
		}


		szString.append(gDLL->getText("TXT_KEY_COLOR_REVERT"));

		szString.append(L' ');//XXX

		if (((!(pAttacker->immuneToFirstStrikes())) && (pDefender->maxFirstStrikes() > 0)) || (pDefender->maxCombatStr(pPlot,pAttacker)!=pDefender->baseCombatStr()*100))
		{
			//if attacker uninjured strength is not the same as base strength (i.e. modifiers are in effect) or first strikes exist, then
			if (getBugOptionBOOL("ACO__ShowModifierLabels", false, "ACO_SHOW_MODIFIER_LABELS"))
			{
				szString.append(gDLL->getText("TXT_ACO_DefenseModifiers"));
			}
		}//if
		if (iView & getBugOptionINT("ACO__ShowDefenderInfo", 3, "ACO_SHOW_DEFENDER_INFO"))
		{
			szString.append(NEWLINE);
			setUnitHelp(szString, pDefender, true, true);
		}


		if (iView & getBugOptionINT("ACO__ShowDefenseModifiers", 3, "ACO_SHOW_DEFENSE_MODIFIERS"))
		{
			//if defense modifiers are enabled - recommend leaving this on unless Total defense Modifier is enabled

			szString.append(gDLL->getText("TXT_KEY_COLOR_NEGATIVE"));

			szString.append(L' ');//XXX

			if (!(pAttacker->immuneToFirstStrikes()))
			{
				if (pDefender->maxFirstStrikes() > 0)
				{
					if (pDefender->firstStrikes() == pDefender->maxFirstStrikes())
					{
						if (pDefender->firstStrikes() == 1)
						{
							szString.append(NEWLINE);
							szString.append(gDLL->getText("TXT_KEY_UNIT_ONE_FIRST_STRIKE"));
						}
						else
						{
							szString.append(NEWLINE);
							szString.append(gDLL->getText("TXT_KEY_UNIT_NUM_FIRST_STRIKES", pDefender->firstStrikes()));
						}
					}
					else
					{
						szString.append(NEWLINE);
						szString.append(gDLL->getText("TXT_KEY_UNIT_FIRST_STRIKE_CHANCES", pDefender->firstStrikes(), pDefender->maxFirstStrikes()));
					}
				}
			}

			if (!(pAttacker->isRiver()))
			{
				if (pAttacker->plot()->isRiverCrossing(directionXY(pAttacker->plot(), pPlot)))
				{
					iModifier = GC.getRIVER_ATTACK_MODIFIER();

					if (iModifier != 0)
					{
						szString.append(NEWLINE);
						szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_RIVER_MOD", -(iModifier)));
					}
				}
			}

			if (!(pAttacker->isAmphib()))
			{
				if (!(pPlot->isWater()) && pAttacker->plot()->isWater())
				{
					iModifier = GC.getAMPHIB_ATTACK_MODIFIER();

					if (iModifier != 0)
					{
						szString.append(NEWLINE);
						szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_AMPHIB_MOD", -(iModifier)));
					}
				}
			}

			iModifier = pDefender->getExtraCombatPercent();

			if (iModifier != 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_EXTRA_STRENGTH", iModifier));
			}

			iModifier = pDefender->unitClassDefenseModifier(pAttacker->getUnitClassType());

			if (iModifier != 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_MOD_VS_TYPE", iModifier, GC.getUnitClassInfo(pAttacker->getUnitClassType()).getTextKeyWide()));
			}

			if (pAttacker->getUnitCombatType() != NO_UNITCOMBAT)
			{
				// MOD - START - Unit Combat Attack Defense Mod
				iModifier = pDefender->unitCombatDefenseModifier(pAttacker->getUnitCombatType());

				if (iModifier != 0)
				{
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_MOD_VS_TYPE", iModifier, GC.getUnitCombatInfo(pDefender->getUnitCombatType()).getTextKeyWide()));
				}
				// MOD - END - Unit Combat Attack Defense Mod

				iModifier = pDefender->unitCombatModifier(pAttacker->getUnitCombatType());

				if (iModifier != 0)
				{
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_MOD_VS_TYPE", iModifier, GC.getUnitCombatInfo(pAttacker->getUnitCombatType()).getTextKeyWide()));
				}
			}

			iModifier = pDefender->domainModifier(pAttacker->getDomainType());

			if (iModifier != 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_MOD_VS_TYPE", iModifier, GC.getDomainInfo(pAttacker->getDomainType()).getTextKeyWide()));
			}

			if (!(pDefender->noDefensiveBonus()))
			{
				iModifier = pPlot->defenseModifier(pDefender->getTeam(), (pAttacker != NULL) ? pAttacker->ignoreBuildingDefense() : true);

				if (iModifier != 0)
				{
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_TILE_MOD", iModifier));
				}

				// MOD - START - Defense Modifiers (Mexico)
				CvCity* pCity = pPlot->getPlotCity();
				if (pCity != NULL)
				{
					UnitCombatTypes eAttackerUnitCombatType = pAttacker->getUnitCombatType();
					UnitClassTypes  eAttackerUnitClassType = pAttacker->getUnitClassType();

					iModifier = 0;
					int iCityAdjustedModifier = 100 * (GC.getMAX_CITY_DEFENSE_DAMAGE() - pCity->getDefenseDamage()) / std::max(1,GC.getMAX_CITY_DEFENSE_DAMAGE());						

					if (eAttackerUnitCombatType != NO_UNITCOMBAT)
					{
						iModifier += pCity->getUnitCombatDefenseModifier(eAttackerUnitCombatType);
					}

					iModifier += pCity->getUnitClassDefenseModifier(eAttackerUnitClassType);
					iModifier = iModifier * iCityAdjustedModifier / 100;				

					if (iModifier != 0)
					{
						szString.append(NEWLINE);
						szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_BUILDING_MOD", iModifier));
					}
				}
				// MOD - END - Defense Modifiers (Mexico)
			}

			iModifier = pDefender->fortifyModifier();

			if (iModifier != 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_FORTIFY_MOD", iModifier));
			}

			if (pPlot->isCity(true, pDefender->getTeam()))
			{
				iModifier = pDefender->cityDefenseModifier();

				if (iModifier != 0)
				{
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_CITY_MOD", iModifier));
				}
			}

			if (pPlot->isHills())
			{
				iModifier = pDefender->hillsDefenseModifier();

				if (iModifier != 0)
				{
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_HILLS_MOD", iModifier));
				}
			}

			if (pPlot->getFeatureType() != NO_FEATURE)
			{
				iModifier = pDefender->featureDefenseModifier(pPlot->getFeatureType());

				if (iModifier != 0)
				{
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_UNIT_MOD", iModifier, GC.getFeatureInfo(pPlot->getFeatureType()).getTextKeyWide()));
				}
			}
			else
			{
				iModifier = pDefender->terrainDefenseModifier(pPlot->getTerrainType());

				if (iModifier != 0)
				{
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_UNIT_MOD", iModifier, GC.getTerrainInfo(pPlot->getTerrainType()).getTextKeyWide()));
				}
			}

			szString.append(gDLL->getText("TXT_KEY_COLOR_REVERT"));

			szString.append(L' ');//XXX

			szString.append(gDLL->getText("TXT_KEY_COLOR_POSITIVE"));

			szString.append(L' ');//XXX


			iModifier = pAttacker->unitClassAttackModifier(pDefender->getUnitClassType());

			if (iModifier != 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_MOD_VS_TYPE", -iModifier, GC.getUnitClassInfo(pDefender->getUnitClassType()).getTextKeyWide()));
			}

			if (pDefender->getUnitCombatType() != NO_UNITCOMBAT)
			{
				// MOD - START - Unit Combat Attack Defense Mod
				iModifier = pAttacker->unitCombatAttackModifier(pDefender->getUnitCombatType());

				if (iModifier != 0)
				{
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_MOD_VS_TYPE", -iModifier, GC.getUnitCombatInfo(pDefender->getUnitCombatType()).getTextKeyWide()));
				}
				// MOD - END - Unit Combat Attack Defense Mod

				iModifier = pAttacker->unitCombatModifier(pDefender->getUnitCombatType());

				if (iModifier != 0)
				{
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_MOD_VS_TYPE", -iModifier, GC.getUnitCombatInfo(pDefender->getUnitCombatType()).getTextKeyWide()));
				}
			}

			iModifier = pAttacker->domainModifier(pDefender->getDomainType());

			if (iModifier != 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_MOD_VS_TYPE", -iModifier, GC.getDomainInfo(pDefender->getDomainType()).getTextKeyWide()));
			}

			if (pPlot->isCity(true, pDefender->getTeam()))
			{
				iModifier = pAttacker->cityAttackModifier();

				if (iModifier != 0)
				{
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_CITY_MOD", -iModifier));
				}
			}

			if (pPlot->isHills())
			{
				iModifier = pAttacker->hillsAttackModifier();

				if (iModifier != 0)
				{
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_HILLS_MOD", -iModifier));
				}
			}

			if (pPlot->getFeatureType() != NO_FEATURE)
			{
				iModifier = pAttacker->featureAttackModifier(pPlot->getFeatureType());

				if (iModifier != 0)
				{
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_UNIT_MOD", -iModifier, GC.getFeatureInfo(pPlot->getFeatureType()).getTextKeyWide()));
				}
			}
			else
			{
				iModifier = pAttacker->terrainAttackModifier(pPlot->getTerrainType());

				if (iModifier != 0)
				{
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_UNIT_MOD", -iModifier, GC.getTerrainInfo(pPlot->getTerrainType()).getTextKeyWide()));
				}
			}

			iModifier = pAttacker->getKamikazePercent();
			if (iModifier != 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_COMBAT_KAMIKAZE_MOD", -iModifier));
			}

			if (pDefender->isAnimal())
			{
				//iModifier = -GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAnimalCombatModifier();
				iModifier = -GC.getHandicapInfo(GET_PLAYER(pAttacker->getOwnerINLINE()).getHandicapType()).getAnimalCombatModifier(); // K-Mod

				iModifier += pAttacker->getUnitInfo().getAnimalCombatModifier();

				if (iModifier != 0)
				{
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_UNIT_ANIMAL_COMBAT_MOD", -iModifier));
				}
			}

			if (pDefender->isBarbarian())
			{
				//iModifier = -GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getBarbarianCombatModifier();
				iModifier = -GC.getHandicapInfo(GET_PLAYER(pAttacker->getOwnerINLINE()).getHandicapType()).getBarbarianCombatModifier(); // K-Mod

				// MOD - START - Extra Barbarian Combat Modifier
				iModifier += pAttacker->getExtraBarbarianCombatModifier();
				// MOD - END - Extra Barbarian Combat Modifier

				if (iModifier != 0)
				{
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_UNIT_BARBARIAN_COMBAT_MOD", -iModifier));
				}
			}
		}//if

		szString.append(gDLL->getText("TXT_KEY_COLOR_REVERT"));

		szString.append(L' ');//XXX

		if (iView & getBugOptionINT("ACO__ShowTotalDefenseModifier", 2, "ACO_SHOW_TOTAL_DEFENSE_MODIFIER"))
		{
			//szString.append(L' ');//XXX
			if (pDefender->maxCombatStr(pPlot,pAttacker)>pDefender->baseCombatStr()*100) // modifier is positive
			{
				szTempBuffer.Format(SETCOLR L"%d%%" ENDCOLR,
					TEXT_COLOR("COLOR_NEGATIVE_TEXT"),(((pDefender->maxCombatStr(pPlot,pAttacker)))/pDefender->baseCombatStr())-100);
			}
			else   // modifier is negative
			{
				szTempBuffer.Format(SETCOLR L"%d%%" ENDCOLR,
					TEXT_COLOR("COLOR_POSITIVE_TEXT"),(100-((pDefender->baseCombatStr()*10000)/(pDefender->maxCombatStr(pPlot,pAttacker)))));
			}

			szString.append(gDLL->getText("TXT_ACO_TotalDefenseModifier"));
			szString.append(szTempBuffer.GetCString());
		}
	}//if

	/** What follows in the "else" block, is the original code **/
	else
	{
		//ACO is not enabled
/*************************************************************************************************/
/** ADVANCED COMBAT ODDS                      3/11/09                           PieceOfMind      */
/** END                                                                         v2.0             */
/*************************************************************************************************/
		szOffenseOdds.Format(L"%.2f", ((pAttacker->getDomainType() == DOMAIN_AIR) ? pAttacker->airCurrCombatStrFloat(pDefender) : pAttacker->currCombatStrFloat(NULL, NULL)));
		szDefenseOdds.Format(L"%.2f", pDefender->currCombatStrFloat(pPlot, pAttacker));
		szString.append(NEWLINE);
		szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_ODDS_VS", szOffenseOdds.GetCString(), szDefenseOdds.GetCString()));

		szString.append(L' ');//XXX

		szString.append(gDLL->getText("TXT_KEY_COLOR_POSITIVE"));

		szString.append(L' ');//XXX

		iModifier = pAttacker->getExtraCombatPercent();

		if (iModifier != 0)
		{
			szString.append(NEWLINE);
			szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_EXTRA_STRENGTH", iModifier));
		}

		iModifier = pAttacker->unitClassAttackModifier(pDefender->getUnitClassType());

		if (iModifier != 0)
		{
			szString.append(NEWLINE);
			szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_MOD_VS_TYPE", iModifier, GC.getUnitClassInfo(pDefender->getUnitClassType()).getTextKeyWide()));
		}

		if (pDefender->getUnitCombatType() != NO_UNITCOMBAT)
		{
			// MOD - START - Unit Combat Attack Defense Mod
			iModifier = pAttacker->unitCombatAttackModifier(pDefender->getUnitCombatType());

			if (iModifier != 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_MOD_VS_TYPE", iModifier, GC.getUnitCombatInfo(pDefender->getUnitCombatType()).getTextKeyWide()));
			}
			// MOD - END - Unit Combat Attack Defense Mod

			iModifier = pAttacker->unitCombatModifier(pDefender->getUnitCombatType());

			if (iModifier != 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_MOD_VS_TYPE", iModifier, GC.getUnitCombatInfo(pDefender->getUnitCombatType()).getTextKeyWide()));
			}
		}

		iModifier = pAttacker->domainModifier(pDefender->getDomainType());

		if (iModifier != 0)
		{
			szString.append(NEWLINE);
			szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_MOD_VS_TYPE", iModifier, GC.getDomainInfo(pDefender->getDomainType()).getTextKeyWide()));
		}

		if (pPlot->isCity(true, pDefender->getTeam()))
		{
			iModifier = pAttacker->cityAttackModifier();

			if (iModifier != 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_CITY_MOD", iModifier));
			}
		}

		if (pPlot->isHills())
		{
			iModifier = pAttacker->hillsAttackModifier();

			if (iModifier != 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_HILLS_MOD", iModifier));
			}
		}

		if (pPlot->getFeatureType() != NO_FEATURE)
		{
			iModifier = pAttacker->featureAttackModifier(pPlot->getFeatureType());

			if (iModifier != 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_UNIT_MOD", iModifier, GC.getFeatureInfo(pPlot->getFeatureType()).getTextKeyWide()));
			}
		}
		else
		{
			iModifier = pAttacker->terrainAttackModifier(pPlot->getTerrainType());

			if (iModifier != 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_UNIT_MOD", iModifier, GC.getTerrainInfo(pPlot->getTerrainType()).getTextKeyWide()));
			}
		}

		iModifier = pAttacker->getKamikazePercent();
		if (iModifier != 0)
		{
			szString.append(NEWLINE);
			szString.append(gDLL->getText("TXT_KEY_COMBAT_KAMIKAZE_MOD", iModifier));
		}

		if (pDefender->isAnimal())
		{
			//iModifier = -GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAnimalCombatModifier();
			iModifier = -GC.getHandicapInfo(GET_PLAYER(pAttacker->getOwnerINLINE()).getHandicapType()).getAnimalCombatModifier(); // K-Mod

			iModifier += pAttacker->getUnitInfo().getAnimalCombatModifier();

			if (iModifier != 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_UNIT_ANIMAL_COMBAT_MOD", iModifier));
			}
		}

		if (pDefender->isBarbarian())
		{
			//iModifier = -GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getBarbarianCombatModifier();
			iModifier = -GC.getHandicapInfo(GET_PLAYER(pAttacker->getOwnerINLINE()).getHandicapType()).getBarbarianCombatModifier(); // K-Mod

			// MOD - START - Extra Barbarian Combat Modifier
			iModifier += pAttacker->getExtraBarbarianCombatModifier();
			// MOD - END - Extra Barbarian Combat Modifier

			if (iModifier != 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_UNIT_BARBARIAN_COMBAT_MOD", iModifier));
			}
		}

		if (!(pDefender->immuneToFirstStrikes()))
		{
			if (pAttacker->maxFirstStrikes() > 0)
			{
				if (pAttacker->firstStrikes() == pAttacker->maxFirstStrikes())
				{
					if (pAttacker->firstStrikes() == 1)
					{
						szString.append(NEWLINE);
						szString.append(gDLL->getText("TXT_KEY_UNIT_ONE_FIRST_STRIKE"));
					}
					else
					{
						szString.append(NEWLINE);
						szString.append(gDLL->getText("TXT_KEY_UNIT_NUM_FIRST_STRIKES", pAttacker->firstStrikes()));
					}
				}
				else
				{
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_UNIT_FIRST_STRIKE_CHANCES", pAttacker->firstStrikes(), pAttacker->maxFirstStrikes()));
				}
			}
		}

		if (pAttacker->isHurt())
		{
			szString.append(NEWLINE);
			szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_HP", pAttacker->currHitPoints(), pAttacker->maxHitPoints()));
		}

		szString.append(gDLL->getText("TXT_KEY_COLOR_REVERT"));

		szString.append(L' ');//XXX

		szString.append(gDLL->getText("TXT_KEY_COLOR_NEGATIVE"));

		szString.append(L' ');//XXX

		if (!(pAttacker->isRiver()))
		{
			if (pAttacker->plot()->isRiverCrossing(directionXY(pAttacker->plot(), pPlot)))
			{
				iModifier = GC.getRIVER_ATTACK_MODIFIER();

				if (iModifier != 0)
				{
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_RIVER_MOD", -(iModifier)));
				}
			}
		}

		if (!(pAttacker->isAmphib()))
		{
			if (!(pPlot->isWater()) && pAttacker->plot()->isWater())
			{
				iModifier = GC.getAMPHIB_ATTACK_MODIFIER();

				if (iModifier != 0)
				{
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_AMPHIB_MOD", -(iModifier)));
				}
			}
		}

		iModifier = pDefender->getExtraCombatPercent();

		if (iModifier != 0)
		{
			szString.append(NEWLINE);
			szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_EXTRA_STRENGTH", iModifier));
		}

		iModifier = pDefender->unitClassDefenseModifier(pAttacker->getUnitClassType());

		if (iModifier != 0)
		{
			szString.append(NEWLINE);
			szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_MOD_VS_TYPE", iModifier, GC.getUnitClassInfo(pAttacker->getUnitClassType()).getTextKeyWide()));
		}

		if (pAttacker->getUnitCombatType() != NO_UNITCOMBAT)
		{
			// MOD - START - Unit Combat Attack Defense Mod
			iModifier = pDefender->unitCombatDefenseModifier(pAttacker->getUnitCombatType());

			if (iModifier != 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_MOD_VS_TYPE", iModifier, GC.getUnitCombatInfo(pDefender->getUnitCombatType()).getTextKeyWide()));
			}
			// MOD - END - Unit Combat Attack Defense Mod

			iModifier = pDefender->unitCombatModifier(pAttacker->getUnitCombatType());

			if (iModifier != 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_MOD_VS_TYPE", iModifier, GC.getUnitCombatInfo(pAttacker->getUnitCombatType()).getTextKeyWide()));
			}
		}

		iModifier = pDefender->domainModifier(pAttacker->getDomainType());

		if (iModifier != 0)
		{
			szString.append(NEWLINE);
			szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_MOD_VS_TYPE", iModifier, GC.getDomainInfo(pAttacker->getDomainType()).getTextKeyWide()));
		}

		if (!(pDefender->noDefensiveBonus()))
		{
			iModifier = pPlot->defenseModifier(pDefender->getTeam(), (pAttacker != NULL) ? pAttacker->ignoreBuildingDefense() : true);

			if (iModifier != 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_TILE_MOD", iModifier));
			}

			// MOD - START - Defense Modifiers (Mexico)
			CvCity* pCity = pPlot->getPlotCity();
			if (pCity != NULL)
			{
				UnitCombatTypes eAttackerUnitCombatType = pAttacker->getUnitCombatType();
				UnitClassTypes  eAttackerUnitClassType = pAttacker->getUnitClassType();

				iModifier = 0;
				int iCityAdjustedModifier = 100 * (GC.getMAX_CITY_DEFENSE_DAMAGE() - pCity->getDefenseDamage()) / std::max(1,GC.getMAX_CITY_DEFENSE_DAMAGE());					

				if (eAttackerUnitCombatType != NO_UNITCOMBAT)
				{
					iModifier += pCity->getUnitCombatDefenseModifier(eAttackerUnitCombatType);
				}

				iModifier += pCity->getUnitClassDefenseModifier(eAttackerUnitClassType);
				iModifier = iModifier * iCityAdjustedModifier / 100;		

				if (iModifier != 0)
				{
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_BUILDING_MOD", iModifier));
				}
			}
			// MOD - END - Defense Modifiers (Mexico)
		}

		iModifier = pDefender->fortifyModifier();

		if (iModifier != 0)
		{
			szString.append(NEWLINE);
			szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_FORTIFY_MOD", iModifier));
		}

		if (pPlot->isCity(true, pDefender->getTeam()))
		{
			iModifier = pDefender->cityDefenseModifier();

			if (iModifier != 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_CITY_MOD", iModifier));
			}
		}

		if (pPlot->isHills())
		{
			iModifier = pDefender->hillsDefenseModifier();

			if (iModifier != 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_HILLS_MOD", iModifier));
			}
		}

		if (pPlot->getFeatureType() != NO_FEATURE)
		{
			iModifier = pDefender->featureDefenseModifier(pPlot->getFeatureType());

			if (iModifier != 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_UNIT_MOD", iModifier, GC.getFeatureInfo(pPlot->getFeatureType()).getTextKeyWide()));
			}
		}
		else
		{
			iModifier = pDefender->terrainDefenseModifier(pPlot->getTerrainType());

			if (iModifier != 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_UNIT_MOD", iModifier, GC.getTerrainInfo(pPlot->getTerrainType()).getTextKeyWide()));
			}
		}

		if (!(pAttacker->immuneToFirstStrikes()))
		{
			if (pDefender->maxFirstStrikes() > 0)
			{
				if (pDefender->firstStrikes() == pDefender->maxFirstStrikes())
				{
					if (pDefender->firstStrikes() == 1)
					{
						szString.append(NEWLINE);
						szString.append(gDLL->getText("TXT_KEY_UNIT_ONE_FIRST_STRIKE"));
					}
					else
					{
						szString.append(NEWLINE);
						szString.append(gDLL->getText("TXT_KEY_UNIT_NUM_FIRST_STRIKES", pDefender->firstStrikes()));
					}
				}
				else
				{
					szString.append(NEWLINE);
					szString.append(gDLL->getText("TXT_KEY_UNIT_FIRST_STRIKE_CHANCES", pDefender->firstStrikes(), pDefender->maxFirstStrikes()));
				}
			}
		}

		if (pDefender->isHurt())
		{
			szString.append(NEWLINE);
			szString.append(gDLL->getText("TXT_KEY_COMBAT_PLOT_HP", pDefender->currHitPoints(), pDefender->maxHitPoints()));
		}
	}

	szString.append(gDLL->getText("TXT_KEY_COLOR_REVERT"));

	/* original code
	if ((gDLL->getChtLvl() > 0)) */
	if (GC.getGameINLINE().isDebugMode()) // BBAI: Only display this info in debug mode so game can be played with cheat code entered
	{
		szTempBuffer.Format(L"\nStack Compare Value = %d",
			gDLL->getInterfaceIFace()->getSelectionList()->AI_compareStacks(pPlot));
		szString.append(szTempBuffer);

		if( pPlot->getPlotCity() != NULL )
		{
			szTempBuffer.Format(L"\nBombard turns = %d",
				gDLL->getInterfaceIFace()->getSelectionList()->getBombardTurns(pPlot->getPlotCity()));
			szString.append(szTempBuffer);
		}

		const CvPlayerAI& kPlayer = GET_PLAYER(GC.getGameINLINE().getActivePlayer());
		int iOurStrengthDefense = kPlayer.AI_localDefenceStrength(pPlot, kPlayer.getTeam(), DOMAIN_LAND, 1, true, true, true);
		int iOurStrengthOffense = kPlayer.AI_localAttackStrength(pPlot, kPlayer.getTeam(), DOMAIN_LAND, 1, false, true, false);
		szTempBuffer.Format(L"\nPlot Strength(Ours)= d%d, o%d", iOurStrengthDefense, iOurStrengthOffense);
		szString.append(szTempBuffer);
		int iEnemyStrengthDefense = kPlayer.AI_localDefenceStrength(pPlot, NO_TEAM, DOMAIN_LAND, 1, true, true, true);
		int iEnemyStrengthOffense = kPlayer.AI_localAttackStrength(pPlot, NO_TEAM, DOMAIN_LAND, 1, false, true, false);
		szTempBuffer.Format(L"\nPlot Strength(Enemy)= d%d, o%d", iEnemyStrengthDefense, iEnemyStrengthOffense);
		szString.append(szTempBuffer);
	}

	szString.append(gDLL->getText("TXT_KEY_COLOR_REVERT"));

	return true;
}

// DO NOT REMOVE - needed for font testing - Moose
void createTestFontString(CvWStringBuffer& szString)
{
	int iI;
	szString.assign(L"!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[Ч]^_`abcdefghijklmnopqrstuvwxyz\n");
	szString.append(L"{}~\\ЯАБВГДЕЖЗИЙКЛМНОПРСТУФХЦШЩЪЫЬЭЮџЯабвгдежзийклмнопрстуфхцчшщъыьэюяїЎ«»°ЉЊЋљњћ™©®ЂЈў”‘“…’");
	for (iI=0;iI<NUM_YIELD_TYPES;++iI)
		szString.append(CvWString::format(L"%c", GC.getYieldInfo((YieldTypes) iI).getChar()));

	szString.append(L"\n");
	for (iI=0;iI<NUM_COMMERCE_TYPES;++iI)
		szString.append(CvWString::format(L"%c", GC.getCommerceInfo((CommerceTypes) iI).getChar()));
	szString.append(L"\n");
	for (iI = 0; iI < GC.getNumReligionInfos(); ++iI)
	{
		szString.append(CvWString::format(L"%c", GC.getReligionInfo((ReligionTypes) iI).getChar()));
		szString.append(CvWString::format(L"%c", GC.getReligionInfo((ReligionTypes) iI).getHolyCityChar()));
	}
	for (iI = 0; iI < GC.getNumCorporationInfos(); ++iI)
	{
		szString.append(CvWString::format(L"%c", GC.getCorporationInfo((CorporationTypes) iI).getChar()));
		szString.append(CvWString::format(L"%c", GC.getCorporationInfo((CorporationTypes) iI).getHeadquarterChar()));
	}
	szString.append(L"\n");
	for (iI = 0; iI < GC.getNumBonusInfos(); ++iI)
		szString.append(CvWString::format(L"%c", GC.getBonusInfo((BonusTypes) iI).getChar()));
	for (iI=0; iI<MAX_NUM_SYMBOLS; ++iI)
		szString.append(CvWString::format(L"%c", gDLL->getSymbolID(iI)));
}

void CvGameTextMgr::setPlotHelp(CvWStringBuffer& szString, CvPlot* pPlot)
{
	PROFILE_FUNC();

	int iI;

	CvWString szTempBuffer;
	ImprovementTypes eImprovement;
	PlayerTypes eRevealOwner;
	BonusTypes eBonus;
	bool bShift;
	bool bAlt;
	bool bCtrl;
	bool bSpace;
	bool bFound;
	int iDefenseModifier;
	int iYield;
	int iTurns;

	bShift = GC.shiftKey();
	bAlt = GC.altKey();
	bCtrl = GC.ctrlKey();
	bSpace = GC.spaceKey();


	// MOD - START - Aid and Detriments
	if (bCtrl && bSpace && gDLL->getChtLvl() > 0)
	{  
		setPlotAidAndDetrimentHelp(szString, pPlot);
	}
	// MOD - END - Aid and Detriments
	else if (bCtrl && (gDLL->getChtLvl() > 0))
	{
		if (bShift && pPlot->headUnitNode() != NULL)
		{
			return;
		}

		if (pPlot->getOwnerINLINE() != NO_PLAYER)
		{
			int iPlotDanger = GET_PLAYER(pPlot->getOwnerINLINE()).AI_getPlotDanger(pPlot, 2);
			if (iPlotDanger > 0)
			{
				szString.append(CvWString::format(L"\nPlot Danger = %d", iPlotDanger));
			}
		}

		CvCity* pPlotCity = pPlot->getPlotCity();
		if (pPlotCity != NULL)
		{
			PlayerTypes ePlayer = pPlot->getOwnerINLINE();
			CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);

			int iCityDefenders = pPlot->plotCount(PUF_canDefendGroupHead, -1, -1, ePlayer, NO_TEAM, PUF_isCityAIType);
			int iAttackGroups = pPlot->plotCount(PUF_isUnitAIType, UNITAI_ATTACK, -1, ePlayer);
			szString.append(CvWString::format(L"\nDefenders [D+A]/N ([%d + %d] / %d)", iCityDefenders, iAttackGroups, pPlotCity->AI_neededDefenders()));

			szString.append(CvWString::format(L"\nFloating Defenders H/N (%d / %d)", kPlayer.AI_getTotalFloatingDefenders(pPlotCity->area()), kPlayer.AI_getTotalFloatingDefendersNeeded(pPlotCity->area())));
			szString.append(CvWString::format(L"\nAir Defenders H/N (%d / %d)", pPlotCity->plot()->plotCount(PUF_canAirDefend, -1, -1, pPlotCity->getOwnerINLINE(), NO_TEAM, PUF_isDomainType, DOMAIN_AIR), pPlotCity->AI_neededAirDefenders()));
//			int iHostileUnits = kPlayer.AI_countNumAreaHostileUnits(pPlotCity->area());
//			if (iHostileUnits > 0)
//			{
//				szString+=CvWString::format(L"\nHostiles = %d", iHostileUnits);
//			}

			szString.append(CvWString::format(L"\nThreat C/P (%d / %d)", pPlotCity->AI_cityThreat(), kPlayer.AI_getTotalAreaCityThreat(pPlotCity->area())));

			bool bFirst = true;
			for (int iI = 0; iI < MAX_PLAYERS; ++iI)
			{
				PlayerTypes eLoopPlayer = (PlayerTypes) iI;
				CvPlayerAI& kLoopPlayer = GET_PLAYER(eLoopPlayer);
				if ((eLoopPlayer != ePlayer) && kLoopPlayer.isAlive())
				{
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      06/16/08                                jdog5000      */
/*                                                                                              */
/* DEBUG                                                                                        */
/************************************************************************************************/
/* original code
					int iCloseness = pPlotCity->AI_playerCloseness(eLoopPlayer, 7);
					if (iCloseness != 0)
					{
						if (bFirst)
						{
							bFirst = false;
						
							szString.append(CvWString::format(L"\n\nCloseness:"));
						}

						szString.append(CvWString::format(L"\n%s(7) : %d", kLoopPlayer.getName(), iCloseness));
						szString.append(CvWString::format(L" (%d, ", kPlayer.AI_playerCloseness(eLoopPlayer, 7)));
						if (kPlayer.getTeam() != kLoopPlayer.getTeam())
						{
							szString.append(CvWString::format(L"%d)", GET_TEAM(kPlayer.getTeam()).AI_teamCloseness(kLoopPlayer.getTeam(), 7)));
						}
						else
						{
							szString.append(CvWString::format(L"-)"));
						}
					}
*/
					if( pPlotCity->isCapital() )
					{
						if( true )
						{
							int iCloseness = pPlotCity->AI_playerCloseness(eLoopPlayer, DEFAULT_PLAYER_CLOSENESS);
							int iPlayerCloseness = kPlayer.AI_playerCloseness(eLoopPlayer, DEFAULT_PLAYER_CLOSENESS);

							if( GET_TEAM(kPlayer.getTeam()).isHasMet(kLoopPlayer.getTeam()) || iPlayerCloseness != 0 )
							{
								if (bFirst)
								{
									bFirst = false;
								
									szString.append(CvWString::format(L"\n\nCloseness + War: (in %d wars)", GET_TEAM(kPlayer.getTeam()).getAtWarCount(true)));
								}

								if (!bAlt)
								{
									szString.append(CvWString::format(L"\n%s(%d) : %d ", kLoopPlayer.getName(), DEFAULT_PLAYER_CLOSENESS, iCloseness));
									szString.append(CvWString::format(L" [%d, ", iPlayerCloseness));
									if (kPlayer.getTeam() != kLoopPlayer.getTeam())
									{
										szString.append(CvWString::format(L"%d]", GET_TEAM(kPlayer.getTeam()).AI_teamCloseness(kLoopPlayer.getTeam(), DEFAULT_PLAYER_CLOSENESS)));
										if( GET_TEAM(kPlayer.getTeam()).isHasMet(kLoopPlayer.getTeam()) && GET_TEAM(kPlayer.getTeam()).AI_getAttitude(kLoopPlayer.getTeam()) != ATTITUDE_FRIENDLY )
										{
											int iStartWarVal = GET_TEAM(kPlayer.getTeam()).AI_startWarVal(kLoopPlayer.getTeam(), WARPLAN_TOTAL);

											if( GET_TEAM(kPlayer.getTeam()).isAtWar(kLoopPlayer.getTeam()) )
											{
												szString.append(CvWString::format(L"\n   At War:   "));
											}
											else if( GET_TEAM(kPlayer.getTeam()).AI_getWarPlan(kLoopPlayer.getTeam()) != NO_WARPLAN )
											{
												szString.append(CvWString::format(L"\n   Plan. War:"));
											}
											else if( !GET_TEAM(kPlayer.getTeam()).canDeclareWar(kLoopPlayer.getTeam()) )
											{
												szString.append(CvWString::format(L"\n   Can't War:"));
											}
											else
											{
												szString.append(CvWString::format(L"\n   No War:   "));
											}

											if( iStartWarVal > 1200 )
											{
												szString.append(CvWString::format(SETCOLR L" %d" ENDCOLR, TEXT_COLOR("COLOR_RED"), iStartWarVal));
											}
											else if( iStartWarVal > 600 )
											{
												szString.append(CvWString::format(SETCOLR L" %d" ENDCOLR, TEXT_COLOR("COLOR_YELLOW"), iStartWarVal));
											}
											else
											{
												szString.append(CvWString::format(L" %d", iStartWarVal));
											}
											szString.append(CvWString::format(L" (%d", GET_TEAM(kPlayer.getTeam()).AI_calculatePlotWarValue(kLoopPlayer.getTeam())));
											szString.append(CvWString::format(L", %d", GET_TEAM(kPlayer.getTeam()).AI_calculateBonusWarValue(kLoopPlayer.getTeam())));
											szString.append(CvWString::format(L", %d", GET_TEAM(kPlayer.getTeam()).AI_calculateCapitalProximity(kLoopPlayer.getTeam())));
											szString.append(CvWString::format(L", %4s", GC.getAttitudeInfo(GET_TEAM(kPlayer.getTeam()).AI_getAttitude(kLoopPlayer.getTeam())).getDescription(0)));
											szString.append(CvWString::format(L", %d%%)", 100-GET_TEAM(kPlayer.getTeam()).AI_noWarAttitudeProb(GET_TEAM(kPlayer.getTeam()).AI_getAttitude(kLoopPlayer.getTeam()))));
										}
									}
									else
									{
										szString.append(CvWString::format(L"-]"));
									}
								}
								else if( GET_TEAM(kPlayer.getTeam()).isHasMet(kLoopPlayer.getTeam()) && GET_TEAM(kPlayer.getTeam()).AI_getAttitude(kLoopPlayer.getTeam()) != ATTITUDE_FRIENDLY )
								{
									szString.append(CvWString::format(L"\n%s(%d,%d) ", kLoopPlayer.getName(), iCloseness, GET_TEAM(kLoopPlayer.getTeam()).getPower(true)));

									if( GET_TEAM(kPlayer.getTeam()).AI_getWarPlan(kLoopPlayer.getTeam()) != NO_WARPLAN )
									{
										szString.append(CvWString::format(L"->war"));
									}

									szString.append(CvWString::format(L" (%d", GET_TEAM(kPlayer.getTeam()).AI_warSpoilsValue(kLoopPlayer.getTeam(), WARPLAN_TOTAL)));
									szString.append(CvWString::format(L",%d)", GET_TEAM(kPlayer.getTeam()).AI_warCommitmentCost(kLoopPlayer.getTeam(), WARPLAN_TOTAL)));

									szString.append(CvWString::format(L" (%d", GET_TEAM(kPlayer.getTeam()).AI_warSpoilsValue(kLoopPlayer.getTeam(), WARPLAN_LIMITED)));
									szString.append(CvWString::format(L",%d)", GET_TEAM(kPlayer.getTeam()).AI_warCommitmentCost(kLoopPlayer.getTeam(), WARPLAN_LIMITED)));

									szString.append(CvWString::format(L" (%d", GET_TEAM(kPlayer.getTeam()).AI_warSpoilsValue(kLoopPlayer.getTeam(), WARPLAN_DOGPILE)));
									szString.append(CvWString::format(L",%d)", GET_TEAM(kPlayer.getTeam()).AI_warCommitmentCost(kLoopPlayer.getTeam(), WARPLAN_DOGPILE)));
								}
							}
					}
					}
					else 
					{
						int iCloseness = pPlotCity->AI_playerCloseness(eLoopPlayer, DEFAULT_PLAYER_CLOSENESS);

						if (iCloseness != 0)
						{
							if (bFirst)
							{
								bFirst = false;
							
								szString.append(CvWString::format(L"\n\nCloseness:"));
							}

							szString.append(CvWString::format(L"\n%s(%d) : %d ", kLoopPlayer.getName(), DEFAULT_PLAYER_CLOSENESS, iCloseness));
							szString.append(CvWString::format(L" [%d, ", kPlayer.AI_playerCloseness(eLoopPlayer, DEFAULT_PLAYER_CLOSENESS)));
							if (kPlayer.getTeam() != kLoopPlayer.getTeam())
							{
								szString.append(CvWString::format(L"%d]", GET_TEAM(kPlayer.getTeam()).AI_teamCloseness(kLoopPlayer.getTeam(), DEFAULT_PLAYER_CLOSENESS)));
							}
							else
							{
								szString.append(CvWString::format(L"-]"));
							}
						}
					}
	/************************************************************************************************/
	/* BETTER_BTS_AI_MOD                       END                                                  */
	/************************************************************************************************/
					}
				}

				int iWorkersHave = pPlotCity->AI_getWorkersHave();
				int iWorkersNeeded = pPlotCity->AI_getWorkersNeeded();
				szString.append(CvWString::format(L"\n\nWorkers H/N (%d , %d)", iWorkersHave, iWorkersNeeded));

				int iWorkBoatsNeeded = pPlotCity->AI_neededSeaWorkers();
				szString.append(CvWString::format(L"\n\nWorkboats Needed = %d", iWorkBoatsNeeded));

				int iAreaSiteBestValue = 0;
				int iNumAreaCitySites = kPlayer.AI_getNumAreaCitySites(pPlot->getArea(), iAreaSiteBestValue);
				int iOtherSiteBestValue = 0;
				int iNumOtherCitySites = (pPlot->waterArea() == NULL) ? 0 : kPlayer.AI_getNumAdjacentAreaCitySites(pPlot->waterArea()->getID(), pPlot->getArea(), iOtherSiteBestValue);

				szString.append(CvWString::format(L"\n\nArea Sites = %d (%d)", iNumAreaCitySites, iAreaSiteBestValue));
				szString.append(CvWString::format(L"\nOther Sites = %d (%d)", iNumOtherCitySites, iOtherSiteBestValue));
			}
			else if (pPlot->getOwner() != NO_PLAYER)
			{
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      11/30/08                                jdog5000      */
/*                                                                                              */
/* Debug                (K-Mod edited)                                                          */
/************************************************************************************************/
/* original code
			for (int iI = 0; iI < GC.getNumCivicInfos(); iI++)
			{
				szString.append(CvWString::format(L"\n %s = %d", GC.getCivicInfo((CivicTypes)iI).getDescription(), GET_PLAYER(pPlot->getOwner()).AI_civicValue((CivicTypes)iI)));			
			}
*/
			if( bShift && !bAlt)
			{
				const CvPlayerAI& kPlayer = GET_PLAYER(pPlot->getOwnerINLINE());

				std::vector<int> viBonusClassRevealed(GC.getNumBonusClassInfos(), 0);
				std::vector<int> viBonusClassUnrevealed(GC.getNumBonusClassInfos(), 0);
				std::vector<int> viBonusClassHave(GC.getNumBonusClassInfos(), 0);

				for (int iI = 0; iI < GC.getNumBonusInfos(); iI++)
				{
					TechTypes eRevealTech = (TechTypes)GC.getBonusInfo((BonusTypes)iI).getTechReveal();
					BonusClassTypes eBonusClass = (BonusClassTypes)GC.getBonusInfo((BonusTypes)iI).getBonusClassType();
					if (eRevealTech != NO_TECH)
					{
						if ((GET_TEAM(pPlot->getTeam()).isHasTech(eRevealTech)))
						{
							viBonusClassRevealed[eBonusClass]++;
						}
						else
						{
							viBonusClassUnrevealed[eBonusClass]++;
						}

						if (kPlayer.getNumAvailableBonuses((BonusTypes)iI) > 0)
						{
							viBonusClassHave[eBonusClass]++;
						}
						else if (kPlayer.countOwnedBonuses((BonusTypes)iI) > 0)
						{
							viBonusClassHave[eBonusClass]++;
						}
					}
				}
				
				for (int iI = 0; iI < GC.getNumTechInfos(); iI++)
				{
					// MOD - START - Tech Path Caching
					//int iPathLength = kPlayer.findPathLength(((TechTypes)iI), false);
					int iPathLength = kPlayer.getTechPathLength((TechTypes)iI);
					// MOD - END - Tech Path Caching

					if( iPathLength <= 3 && !GET_TEAM(pPlot->getTeam()).isHasTech((TechTypes)iI) )
					{
						bool bDummy;
						szString.append(CvWString::format(L"\n%s(%d)=%8d", GC.getTechInfo((TechTypes)iI).getDescription(), iPathLength, kPlayer.AI_techValue((TechTypes)iI, 1, false, true, viBonusClassRevealed, viBonusClassUnrevealed, viBonusClassHave)));
						szString.append(CvWString::format(L" (bld:%d, ", kPlayer.AI_techBuildingValue((TechTypes)iI, true, bDummy)));
						int iObs = kPlayer.AI_obsoleteBuildingPenalty((TechTypes)iI, true);
						if (iObs != 0)
							szString.append(CvWString::format(L"obs:%d, ", -iObs));
						szString.append(CvWString::format(L"unt:%d)", kPlayer.AI_techUnitValue((TechTypes)iI, 1, bDummy)));
					}
				}
			}
			else if( bAlt && !bShift )
			{
				if( pPlot->isHasPathToEnemyCity(pPlot->getTeam()) )
				{
					szString.append(CvWString::format(L"\nCan reach an enemy city\n\n"));	
				}
				else
				{
					szString.append(CvWString::format(L"\nNo reachable enemy cities\n\n"));	
				}
				for (int iI = 0; iI < MAX_PLAYERS; ++iI)
				{
					if( GET_PLAYER((PlayerTypes)iI).isAlive() )
					{
						if( pPlot->isHasPathToPlayerCity(pPlot->getTeam(),(PlayerTypes)iI) )
						{
							szString.append(CvWString::format(SETCOLR L"Can reach %s city" ENDCOLR, TEXT_COLOR("COLOR_GREEN"), GET_PLAYER((PlayerTypes)iI).getName()));
						}
						else
						{
							szString.append(CvWString::format(SETCOLR L"Cannot reach any %s city" ENDCOLR, TEXT_COLOR("COLOR_NEGATIVE_TEXT"), GET_PLAYER((PlayerTypes)iI).getName()));
						}

						if( GET_TEAM(pPlot->getTeam()).isAtWar(GET_PLAYER((PlayerTypes)iI).getTeam()) )
						{
							szString.append(CvWString::format(L" (enemy)"));
						}
						szString.append(CvWString::format(L"\n"));
					}
					
				}
			}
			else if( bShift && bAlt )
			{
				for (int iI = 0; iI < GC.getNumCivicInfos(); iI++)
				{
					szString.append(CvWString::format(L"\n %s = %d", GC.getCivicInfo((CivicTypes)iI).getDescription(), GET_PLAYER(pPlot->getOwner()).AI_civicValue((CivicTypes)iI)));			
				}
			}
			else if( pPlot->headUnitNode() == NULL )
			{
				std::vector<UnitAITypes> vecUnitAIs;

				if( pPlot->getFeatureType() != NO_FEATURE )
				{
					szString.append(CvWString::format(L"\nDefense unit AIs:"));
					vecUnitAIs.push_back(UNITAI_CITY_DEFENSE);
					vecUnitAIs.push_back(UNITAI_COUNTER);
					vecUnitAIs.push_back(UNITAI_CITY_COUNTER);
					// MOD - START - Fortification Units
					vecUnitAIs.push_back(UNITAI_FORTIFICATION);
					// MOD - END - Fortification Units
				}
				else
				{
					szString.append(CvWString::format(L"\nAttack unit AIs:"));
					vecUnitAIs.push_back(UNITAI_ATTACK);
					vecUnitAIs.push_back(UNITAI_ATTACK_CITY);
					vecUnitAIs.push_back(UNITAI_COUNTER);
				}

				CvCity* pCloseCity = GC.getMapINLINE().findCity(pPlot->getX_INLINE(), pPlot->getY_INLINE(), pPlot->getOwner(), NO_TEAM, true);

				if( pCloseCity != NULL )
				{
					for( uint iI = 0; iI < vecUnitAIs.size(); iI++ )
					{
						CvWString szTempString;
						getUnitAIString(szTempString, vecUnitAIs[iI]);
						szString.append(CvWString::format(L"\n  %s  ", szTempString.GetCString()));
						for( int iJ = 0; iJ < GC.getNumUnitClassInfos(); iJ++ )
						{
							UnitTypes eUnit = (UnitTypes)GC.getCivilizationInfo(GET_PLAYER(pPlot->getOwner()).getCivilizationType()).getCivilizationUnits((UnitClassTypes)iJ);
							if( eUnit != NO_UNIT && pCloseCity->canTrain(eUnit) )
							{
								int iValue = GET_PLAYER(pPlot->getOwner()).AI_unitValue(eUnit, vecUnitAIs[iI], pPlot->area());
								if( iValue > 0 )
								{
									szString.append(CvWString::format(L"\n %s = %d", GC.getUnitInfo(eUnit).getDescription(), iValue));
								}
							}
						}
					}
				}
			}
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/
		}
		return;	
	}
	else if (bShift && !bAlt && (gDLL->getChtLvl() > 0))
	{
		szString.append(GC.getTerrainInfo(pPlot->getTerrainType()).getDescription());

		FAssert((0 < GC.getNumBonusInfos()) && "GC.getNumBonusInfos() is not greater than zero but an array is being allocated in CvInterface::updateHelpStrings");
		for (iI = 0; iI < GC.getNumBonusInfos(); ++iI)
		{
			if (pPlot->isPlotGroupConnectedBonus(GC.getGameINLINE().getActivePlayer(), ((BonusTypes)iI)))
			{
				szString.append(NEWLINE);
				szString.append(GC.getBonusInfo((BonusTypes)iI).getDescription());
				szString.append(CvWString::format(L" (%d)", GET_PLAYER(GC.getGameINLINE().getActivePlayer()).AI_bonusVal((BonusTypes)iI, 0, true)));
			}
		}

		if (pPlot->getPlotGroup(GC.getGameINLINE().getActivePlayer()) != NULL)
		{
			szTempBuffer.Format(L"\n(%d, %d) group: %d", pPlot->getX_INLINE(), pPlot->getY_INLINE(), pPlot->getPlotGroup(GC.getGameINLINE().getActivePlayer())->getID());
		}
		else
		{
			szTempBuffer.Format(L"\n(%d, %d) group: (-1, -1)", pPlot->getX_INLINE(), pPlot->getY_INLINE());
		}
		szString.append(szTempBuffer);

		szTempBuffer.Format(L"\nArea: %d", pPlot->getArea());
		szString.append(szTempBuffer);

/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      07/13/09                                jdog5000      */
/*                                                                                              */
/* Debug					                                                                     */
/************************************************************************************************/
		szTempBuffer.Format(L"\nLatitude: %d", pPlot->getLatitude());
		szString.append(szTempBuffer);
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/

		char tempChar = 'x';
		if(pPlot->getRiverNSDirection() == CARDINALDIRECTION_NORTH)
		{
			tempChar = 'N';
		}
		else if(pPlot->getRiverNSDirection() == CARDINALDIRECTION_SOUTH)
		{
			tempChar = 'S';
		}
		szTempBuffer.Format(L"\nNSRiverFlow: %c", tempChar);
		szString.append(szTempBuffer);

		tempChar = 'x';
		if(pPlot->getRiverWEDirection() == CARDINALDIRECTION_WEST)
		{
			tempChar = 'W';
		}
		else if(pPlot->getRiverWEDirection() == CARDINALDIRECTION_EAST)
		{
			tempChar = 'E';
		}
		szTempBuffer.Format(L"\nWERiverFlow: %c", tempChar);
		szString.append(szTempBuffer);

		if(pPlot->getRouteType() != NO_ROUTE)
		{
			szTempBuffer.Format(L"\nRoute: %s", GC.getRouteInfo(pPlot->getRouteType()).getDescription());
			szString.append(szTempBuffer);
		}

		if(pPlot->getRouteSymbol() != NULL)
		{
			szTempBuffer.Format(L"\nConnection: %i", gDLL->getRouteIFace()->getConnectionMask(pPlot->getRouteSymbol()));
			szString.append(szTempBuffer);
		}

		for (iI = 0; iI < MAX_PLAYERS; ++iI)
		{
			if (GET_PLAYER((PlayerTypes)iI).isEverAlive())
			{
				if (pPlot->getCulture((PlayerTypes)iI) > 0)
				{
					szTempBuffer.Format(L"\n%s Culture: %d", GET_PLAYER((PlayerTypes)iI).getName(), pPlot->getCulture((PlayerTypes)iI));
					szString.append(szTempBuffer);
				}
			}
		}

		PlayerTypes eActivePlayer = GC.getGameINLINE().getActivePlayer();
		int iActualFoundValue = pPlot->getFoundValue(eActivePlayer);
		int iCalcFoundValue = GET_PLAYER(eActivePlayer).AI_foundValue(pPlot->getX_INLINE(), pPlot->getY_INLINE(), -1, false);
		int iStartingFoundValue = GET_PLAYER(eActivePlayer).AI_foundValue(pPlot->getX_INLINE(), pPlot->getY_INLINE(), -1, true);

		szTempBuffer.Format(L"\nFound Value: %d, (%d, %d)", iActualFoundValue, iCalcFoundValue, iStartingFoundValue);
		szString.append(szTempBuffer);

		CvCity* pWorkingCity = pPlot->getWorkingCity();
		if (NULL != pWorkingCity)
		{
		    int iPlotIndex = pWorkingCity->getCityPlotIndex(pPlot);
            int iBuildValue = pWorkingCity->AI_getBestBuildValue(iPlotIndex);
            BuildTypes eBestBuild = pWorkingCity->AI_getBestBuild(iPlotIndex);

/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      06/25/09                                jdog5000      */
/*                                                                                              */
/* Debug                                                                                        */
/************************************************************************************************/
			szString.append(NEWLINE);

			int iFoodMultiplier, iProductionMultiplier, iCommerceMultiplier, iDesiredFoodChange;
			pWorkingCity->AI_getYieldMultipliers( iFoodMultiplier, iProductionMultiplier, iCommerceMultiplier, iDesiredFoodChange );

			szTempBuffer.Format(L"\n%s yield multipliers: ", pWorkingCity->getName().c_str() );
			szString.append(szTempBuffer);
			szTempBuffer.Format(L"\n   Food %d, Prod %d, Com %d, DesFoodChange %d", iFoodMultiplier, iProductionMultiplier, iCommerceMultiplier, iDesiredFoodChange );
			szString.append(szTempBuffer);
			szTempBuffer.Format(L"\nTarget pop: %d, (%d good tiles)", pWorkingCity->AI_getTargetPopulation(), pWorkingCity->AI_countGoodPlots() );
			szString.append(szTempBuffer);

			ImprovementTypes eImprovement = pPlot->getImprovementType();

            if (NO_BUILD != eBestBuild)
            {
				
				if( GC.getBuildInfo(eBestBuild).getImprovement() != NO_IMPROVEMENT && eImprovement != NO_IMPROVEMENT && eImprovement != GC.getBuildInfo(eBestBuild).getImprovement() )
				{
					szTempBuffer.Format(SETCOLR L"\nBest Build: %s (%d) replacing %s" ENDCOLR, TEXT_COLOR("COLOR_RED"), GC.getBuildInfo(eBestBuild).getDescription(), iBuildValue, GC.getImprovementInfo(eImprovement).getDescription());
				}
				else
				{
					szTempBuffer.Format(SETCOLR L"\nBest Build: %s (%d)" ENDCOLR, TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), GC.getBuildInfo(eBestBuild).getDescription(), iBuildValue);
				}
				
                szString.append(szTempBuffer);
			}

			szTempBuffer.Format(L"\nActual Build Values: ");
			szString.append(szTempBuffer);

			for (iI = 0; iI < GC.getNumImprovementInfos(); iI++)
			{
				if( pPlot->canHaveImprovement((ImprovementTypes)iI, pWorkingCity->getTeam()) )
				{
					int iOtherBuildValue = pWorkingCity->AI_getImprovementValue( pPlot, (ImprovementTypes)iI, iFoodMultiplier, iProductionMultiplier, iCommerceMultiplier, iDesiredFoodChange);
					int iOldValue = pWorkingCity->AI_getImprovementValue( pPlot, (ImprovementTypes)iI, iFoodMultiplier, iProductionMultiplier, iCommerceMultiplier, iDesiredFoodChange);

					szTempBuffer.Format(L"\n   %s : %d  (old %d)", GC.getImprovementInfo((ImprovementTypes)iI).getDescription(), iOtherBuildValue, iOldValue );
					szString.append(szTempBuffer);
				}
			}

			szTempBuffer.Format(L"\nStandard Build Values: ");
			szString.append(szTempBuffer);

			for (iI = 0; iI < GC.getNumImprovementInfos(); iI++)
			{
				int iOtherBuildValue = pWorkingCity->AI_getImprovementValue( pPlot, (ImprovementTypes)iI, 100, 100, 100, 0);
				if( iOtherBuildValue > 0 )
				{
					szTempBuffer.Format(L"\n   %s : %d", GC.getImprovementInfo((ImprovementTypes)iI).getDescription(), iOtherBuildValue);
					szString.append(szTempBuffer);
				}
			}

			szString.append(NEWLINE);
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/		

		}

		/*{
			szTempBuffer.Format(L"\nStack Str: land=%d(%d), sea=%d(%d), air=%d(%d)",
				pPlot->AI_sumStrength(NO_PLAYER, NO_PLAYER, DOMAIN_LAND, false, false, false),
				pPlot->AI_sumStrength(NO_PLAYER, NO_PLAYER, DOMAIN_LAND, true, false, false),
				pPlot->AI_sumStrength(NO_PLAYER, NO_PLAYER, DOMAIN_SEA, false, false, false),
				pPlot->AI_sumStrength(NO_PLAYER, NO_PLAYER, DOMAIN_SEA, true, false, false),
				pPlot->AI_sumStrength(NO_PLAYER, NO_PLAYER, DOMAIN_AIR, false, false, false),
				pPlot->AI_sumStrength(NO_PLAYER, NO_PLAYER, DOMAIN_AIR, true, false, false));
			szString.append(szTempBuffer);
		}*/

		if (pPlot->getPlotCity() != NULL)
		{
			PlayerTypes ePlayer = pPlot->getOwnerINLINE();
			CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);

			szString.append(CvWString::format(L"\n\nAI unit class weights ..."));
			for (iI = 0; iI < GC.getNumUnitClassInfos(); ++iI)
			{
				if (kPlayer.AI_getUnitClassWeight((UnitClassTypes)iI) != 0)
				{
					szString.append(CvWString::format(L"\n%s = %d", GC.getUnitClassInfo((UnitClassTypes)iI).getDescription(), kPlayer.AI_getUnitClassWeight((UnitClassTypes)iI)));
				}
			}
			szString.append(CvWString::format(L"\n\nalso unit combat type weights..."));
			for (iI = 0; iI < GC.getNumUnitCombatInfos(); ++iI)
			{
				if (kPlayer.AI_getUnitCombatWeight((UnitCombatTypes)iI) != 0)
				{
					szString.append(CvWString::format(L"\n%s = % d", GC.getUnitCombatInfo((UnitCombatTypes)iI).getDescription(), kPlayer.AI_getUnitCombatWeight((UnitCombatTypes)iI)));
				}
			}			
		}
	}
	else if (!bShift && bAlt && (gDLL->getChtLvl() > 0))
	{
	    if (pPlot->isOwned())
	    {
            szTempBuffer.Format(L"\nThis player has %d area cities", pPlot->area()->getCitiesPerPlayer(pPlot->getOwnerINLINE()));
            szString.append(szTempBuffer);
            for (int iI = 0; iI < GC.getNumReligionInfos(); ++iI)
            {
                int iNeededMissionaries = GET_PLAYER(pPlot->getOwnerINLINE()).AI_neededMissionaries(pPlot->area(), ((ReligionTypes)iI));
                if (iNeededMissionaries > 0)
                {
                    szTempBuffer.Format(L"\nNeeded %c missionaries = %d", GC.getReligionInfo((ReligionTypes)iI).getChar(), iNeededMissionaries);
                    szString.append(szTempBuffer);
                }
            }

			int iOurDefense = GET_PLAYER(pPlot->getOwnerINLINE()).AI_localDefenceStrength(pPlot, pPlot->getTeam(), DOMAIN_LAND, 0, true, false, true);
			int iEnemyOffense = GET_PLAYER(pPlot->getOwnerINLINE()).AI_localAttackStrength(pPlot, NO_TEAM);
			if (iEnemyOffense > 0)
			{
				szString.append(CvWString::format(SETCOLR L"\nDanger: %.2f (%d/%d)" ENDCOLR, TEXT_COLOR("COLOR_NEGATIVE_TEXT"), 
					(iEnemyOffense * 1.0f) / std::max(1, iOurDefense), iEnemyOffense, iOurDefense));
			}
			
			CvCity* pCity = pPlot->getPlotCity();
            if (pCity != NULL)
            {
                /* szTempBuffer.Format(L"\n\nCulture Pressure Value = %d (%d)", pCity->AI_calculateCulturePressure(), pCity->culturePressureFactor());
                szString.append(szTempBuffer); */
                
                szTempBuffer.Format(L"\nWater World Percent = %d", pCity->AI_calculateWaterWorldPercent());
                szString.append(szTempBuffer);

				CvPlayerAI& kPlayer = GET_PLAYER(pCity->getOwnerINLINE());
				/* original bts code
				int iUnitCost = kPlayer.calculateUnitCost();
				int iTotalCosts = kPlayer.calculatePreInflatedCosts();
				int iUnitCostPercentage = (iUnitCost * 100) / std::max(1, iTotalCosts);
				szString.append(CvWString::format(L"\nUnit cost percentage: %d (%d / %d)", iUnitCostPercentage, iUnitCost, iTotalCosts)); */
				// K-Mod
				int iBuildUnitProb = static_cast<CvCityAI*>(pCity)->AI_buildUnitProb();
				szString.append(CvWString::format(L"\nUnit Cost: %d (max: %d)", kPlayer.AI_unitCostPerMil(), kPlayer.AI_maxUnitCostPerMil(pCity->area(), iBuildUnitProb)));
				// K-Mod end

				szString.append(CvWString::format(L"\nUpgrade all units: %d gold", kPlayer.AI_getGoldToUpgradeAllUnits()));
				// K-Mod
				{
					int iValue = 0;
					for (int iI = 0; iI < NUM_COMMERCE_TYPES; iI++)
					{
						iValue += kPlayer.getCommerceRate((CommerceTypes)iI) * kPlayer.AI_commerceWeight((CommerceTypes)iI)/100;
					}
					int iLoop;
					for (CvCity* pLoopCity = kPlayer.firstCity(&iLoop); pLoopCity != NULL; pLoopCity = kPlayer.nextCity(&iLoop))
					{
						iValue += 2*pLoopCity->getYieldRate(YIELD_PRODUCTION);
					}
					iValue /= kPlayer.getTotalPopulation();
					szString.append(CvWString::format(L"\nAverage citizen value: %d", iValue));

					//
					szString.append(CvWString::format(L"\nBuild unit prob: %d%%", iBuildUnitProb));
					BuildingTypes eBestBuilding = static_cast<CvCityAI*>(pCity)->AI_bestBuildingThreshold(0, 0, 0, true);
					int iBestBuildingValue = (eBestBuilding == NO_BUILDING) ? 0 : pCity->AI_buildingValue(eBestBuilding, 0, 0, true);

					// Note. cf. adjustments made in AI_chooseProduction
					if (GC.getNumEraInfos() > 1)
					{
						iBestBuildingValue *= 2*(GC.getNumEraInfos()-1) - kPlayer.getCurrentEra();
						iBestBuildingValue /= GC.getNumEraInfos()-1;
					}
					{
						int iTargetCities = GC.getWorldInfo(GC.getMapINLINE().getWorldSize()).getTargetNumCities();
						int iDummy;
						if (kPlayer.AI_getNumAreaCitySites(pPlot->getArea(), iDummy) > 0 && kPlayer.getNumCities() < iTargetCities)
						{
							iBestBuildingValue *= kPlayer.getNumCities() + iTargetCities;
							iBestBuildingValue /= 2*iTargetCities;
						}
					}
					//

					iBuildUnitProb *= (250 + iBestBuildingValue);
					iBuildUnitProb /= (100 + 3 * iBestBuildingValue);
					szString.append(CvWString::format(L" (%d%%)", iBuildUnitProb));
				}
				// K-Mod end

				szString.append(CvWString::format(L"\n\nRanks:"));
				szString.append(CvWString::format(L"\nPopulation:%d", pCity->findPopulationRank()));
				
				szString.append(CvWString::format(L"\nFood:%d(%d), ", pCity->findYieldRateRank(YIELD_FOOD), pCity->findBaseYieldRateRank(YIELD_FOOD)));
				szString.append(CvWString::format(L"Prod:%d(%d), ", pCity->findYieldRateRank(YIELD_PRODUCTION), pCity->findBaseYieldRateRank(YIELD_PRODUCTION)));
				szString.append(CvWString::format(L"Commerce:%d(%d)", pCity->findYieldRateRank(YIELD_COMMERCE), pCity->findBaseYieldRateRank(YIELD_COMMERCE)));
				
				szString.append(CvWString::format(L"\nGold:%d, ", pCity->findCommerceRateRank(COMMERCE_GOLD)));
				szString.append(CvWString::format(L"Research:%d, ", pCity->findCommerceRateRank(COMMERCE_RESEARCH)));
				szString.append(CvWString::format(L"Culture:%d", pCity->findCommerceRateRank(COMMERCE_CULTURE)));
			}
            szString.append(NEWLINE);
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                         06/11/08                             jdog5000      */
/*                                                                                              */
/* Debug                                                                                        */
/************************************************************************************************/           
            //AI strategies
            if (GET_PLAYER(pPlot->getOwner()).AI_isDoStrategy(AI_STRATEGY_DAGGER))
            {
                szTempBuffer.Format(L"Dagger, ");
                szString.append(szTempBuffer);
            }
			if (GET_PLAYER(pPlot->getOwner()).AI_isDoStrategy(AI_STRATEGY_CRUSH))
            {
                szTempBuffer.Format(L"Crush, ");
                szString.append(szTempBuffer);
			}
			if (GET_PLAYER(pPlot->getOwner()).AI_isDoStrategy(AI_STRATEGY_ALERT1))
            {
                szTempBuffer.Format(L"Alert1, ");
                szString.append(szTempBuffer);
			}
			if (GET_PLAYER(pPlot->getOwner()).AI_isDoStrategy(AI_STRATEGY_ALERT2))
            {
                szTempBuffer.Format(L"Alert2, ");
                szString.append(szTempBuffer);
			}
			if (GET_PLAYER(pPlot->getOwner()).AI_isDoStrategy(AI_STRATEGY_TURTLE))
            {
                szTempBuffer.Format(L"Turtle, ");
                szString.append(szTempBuffer);
			}
			if (GET_PLAYER(pPlot->getOwner()).AI_isDoStrategy(AI_STRATEGY_LAST_STAND))
            {
                szTempBuffer.Format(L"LastStand, ");
                szString.append(szTempBuffer);
			}
			if (GET_PLAYER(pPlot->getOwner()).AI_isDoStrategy(AI_STRATEGY_FINAL_WAR))
            {
                szTempBuffer.Format(L"FinalWar, ");
                szString.append(szTempBuffer);
            }
            if (GET_PLAYER(pPlot->getOwner()).AI_isDoStrategy(AI_STRATEGY_GET_BETTER_UNITS))
            {
                szTempBuffer.Format(L"GetBetterUnits, ");
                szString.append(szTempBuffer);
            }
            if (GET_PLAYER(pPlot->getOwner()).AI_isDoStrategy(AI_STRATEGY_FASTMOVERS))
            {
                szTempBuffer.Format(L"FastMovers, ");
                szString.append(szTempBuffer);
            }
            if (GET_PLAYER(pPlot->getOwner()).AI_isDoStrategy(AI_STRATEGY_LAND_BLITZ))
            {
                szTempBuffer.Format(L"LandBlitz, ");
                szString.append(szTempBuffer);
            }
			if (GET_PLAYER(pPlot->getOwner()).AI_isDoStrategy(AI_STRATEGY_AIR_BLITZ))
            {
                szTempBuffer.Format(L"AirBlitz, ");
                szString.append(szTempBuffer);
            }                        
 			if (GET_PLAYER(pPlot->getOwner()).AI_isDoStrategy(AI_STRATEGY_OWABWNW))
            {
                szTempBuffer.Format(L"OWABWNW, ");
                szString.append(szTempBuffer);
            }
			if (GET_PLAYER(pPlot->getOwner()).AI_isDoStrategy(AI_STRATEGY_PRODUCTION))
            {
                szTempBuffer.Format(L"Production, ");
                szString.append(szTempBuffer);
            }
            if (GET_PLAYER(pPlot->getOwner()).AI_isDoStrategy(AI_STRATEGY_MISSIONARY))
            {
                szTempBuffer.Format(L"Missionary, ");
                szString.append(szTempBuffer);
            }
            if (GET_PLAYER(pPlot->getOwner()).AI_isDoStrategy(AI_STRATEGY_BIG_ESPIONAGE))
            {
                szTempBuffer.Format(L"BigEspionage, ");
                szString.append(szTempBuffer);
            }
			if (GET_PLAYER(pPlot->getOwner()).AI_isDoStrategy(AI_STRATEGY_ECONOMY_FOCUS)) // K-Mod
			{
				szTempBuffer.Format(L"EconomyFocus, ");
				szString.append(szTempBuffer);
			}
			if (GET_PLAYER(pPlot->getOwner()).AI_isDoStrategy(AI_STRATEGY_ESPIONAGE_ECONOMY)) // K-Mod
			{
				szTempBuffer.Format(L"EspionageEconomy, ");
				szString.append(szTempBuffer);
			}
                       
            //Area battle plans.
            if (pPlot->area()->getAreaAIType(pPlot->getTeam()) == AREAAI_OFFENSIVE)
            {
				szTempBuffer.Format(L"\n Area AI = OFFENSIVE");
            }
            else if (pPlot->area()->getAreaAIType(pPlot->getTeam()) == AREAAI_DEFENSIVE)
            {
				szTempBuffer.Format(L"\n Area AI = DEFENSIVE");
            }
            else if (pPlot->area()->getAreaAIType(pPlot->getTeam()) == AREAAI_MASSING)
            {
            	szTempBuffer.Format(L"\n Area AI = MASSING");
            }
            else if (pPlot->area()->getAreaAIType(pPlot->getTeam()) == AREAAI_ASSAULT)
            {
				szTempBuffer.Format(L"\n Area AI = ASSAULT");
            }
			else if (pPlot->area()->getAreaAIType(pPlot->getTeam()) == AREAAI_ASSAULT_MASSING)
            {
				szTempBuffer.Format(L"\n Area AI = ASSAULT_MASSING");
            }
            else if (pPlot->area()->getAreaAIType(pPlot->getTeam()) == AREAAI_NEUTRAL)
            {
            	szTempBuffer.Format(L"\n Area AI = NEUTRAL");
            }
            
			szString.append(szTempBuffer);
			szString.append(CvWString::format(L"\n\nNum Wars: %d + %d minor", GET_TEAM(pPlot->getTeam()).getAtWarCount(true), GET_TEAM(pPlot->getTeam()).getAtWarCount(false) - GET_TEAM(pPlot->getTeam()).getAtWarCount(true)));
			szString.append(CvWString::format(L"\nWarplans:"));
			for (int iK = 0; iK < MAX_TEAMS; ++iK)
			{
				TeamTypes eTeam = (TeamTypes)iK;

				if( GET_TEAM(eTeam).isAlive() || GET_TEAM(eTeam).isBarbarian() )
				{
					if( GET_TEAM(pPlot->getTeam()).AI_getWarPlan(eTeam) == WARPLAN_ATTACKED )
					{
						szString.append(CvWString::format(L"\n%s: ATTACKED", GET_TEAM(eTeam).getName().c_str()));
					}
					else if( GET_TEAM(pPlot->getTeam()).AI_getWarPlan(eTeam) == WARPLAN_ATTACKED_RECENT )
					{
						szString.append(CvWString::format(L"\n%s: ATTACKED_RECENT", GET_TEAM(eTeam).getName().c_str()));
					}
					else if( GET_TEAM(pPlot->getTeam()).AI_getWarPlan(eTeam) == WARPLAN_PREPARING_LIMITED )
					{
						szString.append(CvWString::format(L"\n%s: PREP_LIM", GET_TEAM(eTeam).getName().c_str()));
					}
					else if( GET_TEAM(pPlot->getTeam()).AI_getWarPlan(eTeam) == WARPLAN_PREPARING_TOTAL )
					{
						szString.append(CvWString::format(L"\n%s: PREP_TOTAL", GET_TEAM(eTeam).getName().c_str()));
					}
					else if( GET_TEAM(pPlot->getTeam()).AI_getWarPlan(eTeam) == WARPLAN_LIMITED )
					{
						szString.append(CvWString::format(L"\n%s: LIMITED", GET_TEAM(eTeam).getName().c_str()));
					}
					else if( GET_TEAM(pPlot->getTeam()).AI_getWarPlan(eTeam) == WARPLAN_TOTAL )
					{
						szString.append(CvWString::format(L"\n%s: TOTAL", GET_TEAM(eTeam).getName().c_str()));
					}
					else if( GET_TEAM(pPlot->getTeam()).AI_getWarPlan(eTeam) == WARPLAN_DOGPILE )
					{
						szString.append(CvWString::format(L"\n%s: DOGPILE", GET_TEAM(eTeam).getName().c_str()));
					}
					else if( GET_TEAM(pPlot->getTeam()).AI_getWarPlan(eTeam) == NO_WARPLAN )
					{
						if( GET_TEAM(pPlot->getTeam()).isAtWar(eTeam) ) 
						{
							szString.append(CvWString::format(SETCOLR L"\n%s: NO_WARPLAN!" ENDCOLR, TEXT_COLOR("COLOR_WARNING_TEXT"), GET_TEAM(eTeam).getName().c_str()));
						}
					}
				}

				if( GET_TEAM(pPlot->getTeam()).isMinorCiv() || GET_TEAM(pPlot->getTeam()).isBarbarian() )
				{
					if( pPlot->getTeam() != eTeam && !GET_TEAM(pPlot->getTeam()).isAtWar(eTeam) )
					{
						szString.append(CvWString::format(SETCOLR L"\n%s: minor/barb not at war!" ENDCOLR, TEXT_COLOR("COLOR_WARNING_TEXT"), GET_TEAM(eTeam).getName().c_str()));
					}
				}
			}
			
			CvCity* pTargetCity = pPlot->area()->getTargetCity(pPlot->getOwner());
			if( pTargetCity )
			{
				szString.append(CvWString::format(L"\nTarget City: %s", pTargetCity->getName().c_str()));
			}
			else
			{
				szString.append(CvWString::format(L"\nTarget City: None"));
			}
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/
	    }

		bool bFirst = true;
        for (iI = 0; iI < MAX_PLAYERS; ++iI)
        {
            PlayerTypes ePlayer = (PlayerTypes)iI;
			CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);
			
			if (kPlayer.isAlive())
            {
				int iActualFoundValue = pPlot->getFoundValue(ePlayer);
                int iCalcFoundValue = kPlayer.AI_foundValue(pPlot->getX_INLINE(), pPlot->getY_INLINE(), -1, false);
                int iStartingFoundValue = kPlayer.AI_foundValue(pPlot->getX_INLINE(), pPlot->getY_INLINE(), -1, true);
                int iBestAreaFoundValue = pPlot->area()->getBestFoundValue(ePlayer);
                int iCitySiteBestValue;
                int iNumAreaCitySites = kPlayer.AI_getNumAreaCitySites(pPlot->getArea(), iCitySiteBestValue);

				if ((iActualFoundValue > 0 || iCalcFoundValue > 0 || iStartingFoundValue > 0)
					|| ((pPlot->getOwner() == iI) && (iBestAreaFoundValue > 0)))
				{
					if (bFirst)
					{
						szString.append(CvWString::format(SETCOLR L"\nFound Values:" ENDCOLR, TEXT_COLOR("COLOR_UNIT_TEXT")));
						bFirst = false;
					}

					szString.append(NEWLINE);

					bool bIsRevealed = pPlot->isRevealed(kPlayer.getTeam(), false);
					
					szString.append(CvWString::format(SETCOLR, TEXT_COLOR(bIsRevealed ? (((iActualFoundValue > 0) && (iActualFoundValue == iBestAreaFoundValue)) ? "COLOR_UNIT_TEXT" : "COLOR_ALT_HIGHLIGHT_TEXT") : "COLOR_HIGHLIGHT_TEXT")));
					
					if (!bIsRevealed)
					{
						szString.append(CvWString::format(L"("));
					}

					szString.append(CvWString::format(L"%s: %d", kPlayer.getName(), iActualFoundValue));

					if (!bIsRevealed)
					{
						szString.append(CvWString::format(L")"));
					}

					szString.append(CvWString::format(ENDCOLR)); 

					if (iCalcFoundValue > 0 || iStartingFoundValue > 0)
					{
						szTempBuffer.Format(L" (%d,%ds)", iCalcFoundValue, iStartingFoundValue);
						szString.append(szTempBuffer);
					}

					int iDeadlockCount = kPlayer.AI_countDeadlockedBonuses(pPlot);
					if (iDeadlockCount > 0)
					{
						szTempBuffer.Format(L", " SETCOLR L"d=%d" ENDCOLR, TEXT_COLOR("COLOR_NEGATIVE_TEXT"), iDeadlockCount);
						szString.append(szTempBuffer);
					}
					
					if (kPlayer.AI_isPlotCitySite(pPlot))
					{
						szTempBuffer.Format(L", " SETCOLR L"X" ENDCOLR, TEXT_COLOR("COLOR_UNIT_TEXT"));
						szString.append(szTempBuffer);
					}

					if ((iBestAreaFoundValue > 0) || (iNumAreaCitySites > 0))
					{
						int iBestFoundValue = kPlayer.findBestFoundValue();

						szTempBuffer.Format(L"\n  Area Best = %d, Best = %d, Sites = %d", iBestAreaFoundValue, iBestFoundValue, iNumAreaCitySites);
						szString.append(szTempBuffer);
					}
                }
            }
        }
	}
	else if (bShift && bAlt && (gDLL->getChtLvl() > 0))
	{
		CvCity*	pCity = pPlot->getWorkingCity();
		if (pCity != NULL)
		{
			// some functions we want to call are not in CvCity, worse some are protected, so made us a friend
			CvCityAI* pCityAI = static_cast<CvCityAI*>(pCity);

			//bool bAvoidGrowth = pCity->AI_avoidGrowth();
			//bool bIgnoreGrowth = pCityAI->AI_ignoreGrowth();
			int iGrowthValue = pCityAI->AI_growthValuePerFood();

			// if we over the city, then do an array of all the plots
			if (pPlot->getPlotCity() != NULL)
			{
				// check avoid growth
				/* if (bAvoidGrowth || bIgnoreGrowth)
				{
					// red color
					szString.append(CvWString::format(SETCOLR, TEXT_COLOR("COLOR_NEGATIVE_TEXT")));
					
					if (bAvoidGrowth)
					{
						szString.append(CvWString::format(L"AvoidGrowth"));

						if (bIgnoreGrowth)
							szString.append(CvWString::format(L", "));
					}

					if (bIgnoreGrowth)
						szString.append(CvWString::format(L"IgnoreGrowth"));

					// end color
					szString.append(CvWString::format( ENDCOLR L"\n" ));
				} */

				// if control key is down, ignore food
				bool bIgnoreFood = GC.ctrlKey();

				// line one is: blank, 20, 9, 10, blank
				setCityPlotYieldValueString(szString, pCity, -1, bIgnoreFood, iGrowthValue);
				setCityPlotYieldValueString(szString, pCity, 20, bIgnoreFood, iGrowthValue);
				setCityPlotYieldValueString(szString, pCity, 9, bIgnoreFood, iGrowthValue);
				setCityPlotYieldValueString(szString, pCity, 10, bIgnoreFood, iGrowthValue);
				szString.append(L"\n");

				// line two is: 19, 8, 1, 2, 11
				setCityPlotYieldValueString(szString, pCity, 19, bIgnoreFood, iGrowthValue);
				setCityPlotYieldValueString(szString, pCity, 8, bIgnoreFood, iGrowthValue);
				setCityPlotYieldValueString(szString, pCity, 1, bIgnoreFood, iGrowthValue);
				setCityPlotYieldValueString(szString, pCity, 2, bIgnoreFood, iGrowthValue);
				setCityPlotYieldValueString(szString, pCity, 11, bIgnoreFood, iGrowthValue);
				szString.append(L"\n");

				// line three is: 18, 7, 0, 3, 12
				setCityPlotYieldValueString(szString, pCity, 18, bIgnoreFood, iGrowthValue);
				setCityPlotYieldValueString(szString, pCity, 7, bIgnoreFood, iGrowthValue);
				setCityPlotYieldValueString(szString, pCity, 0, bIgnoreFood, iGrowthValue);
				setCityPlotYieldValueString(szString, pCity, 3, bIgnoreFood, iGrowthValue);
				setCityPlotYieldValueString(szString, pCity, 12, bIgnoreFood, iGrowthValue);
				szString.append(L"\n");

				// line four is: 17, 6, 5, 4, 13
				setCityPlotYieldValueString(szString, pCity, 17, bIgnoreFood, iGrowthValue);
				setCityPlotYieldValueString(szString, pCity, 6, bIgnoreFood, iGrowthValue);
				setCityPlotYieldValueString(szString, pCity, 5, bIgnoreFood, iGrowthValue);
				setCityPlotYieldValueString(szString, pCity, 4, bIgnoreFood, iGrowthValue);
				setCityPlotYieldValueString(szString, pCity, 13, bIgnoreFood, iGrowthValue);
				szString.append(L"\n");

				// line five is: blank, 16, 15, 14, blank
				setCityPlotYieldValueString(szString, pCity, -1, bIgnoreFood, iGrowthValue);
				setCityPlotYieldValueString(szString, pCity, 16, bIgnoreFood, iGrowthValue);
				setCityPlotYieldValueString(szString, pCity, 15, bIgnoreFood, iGrowthValue);
				setCityPlotYieldValueString(szString, pCity, 14, bIgnoreFood, iGrowthValue);
				
				// show specialist values too
				for (int iI = 0; iI < GC.getNumSpecialistInfos(); ++iI)
				{
					int iMaxThisSpecialist = pCity->getMaxSpecialistCount((SpecialistTypes) iI);
					int iSpecialistCount = pCity->getSpecialistCount((SpecialistTypes) iI);
					bool bUsingSpecialist = (iSpecialistCount > 0);
					bool bIsDefaultSpecialist = (iI == GC.getDefineINT("DEFAULT_SPECIALIST"));
					
					// can this city have any of this specialist?
					if (iMaxThisSpecialist > 0 || bIsDefaultSpecialist)
					{
						// start color
						if (pCity->getForceSpecialistCount((SpecialistTypes) iI) > 0)
							szString.append(CvWString::format(L"\n" SETCOLR, TEXT_COLOR("COLOR_NEGATIVE_TEXT")));
						else if (bUsingSpecialist)
							szString.append(CvWString::format(L"\n" SETCOLR, TEXT_COLOR("COLOR_ALT_HIGHLIGHT_TEXT")));
						else
							szString.append(CvWString::format(L"\n" SETCOLR, TEXT_COLOR("COLOR_HIGHLIGHT_TEXT")));

						// add name
						szString.append(GC.getSpecialistInfo((SpecialistTypes) iI).getDescription());

						// end color
						szString.append(CvWString::format( ENDCOLR ));

						// add usage
						szString.append(CvWString::format(L": (%d/%d) ", iSpecialistCount, iMaxThisSpecialist));

						// add value
						int iValue = pCityAI->AI_specialistValue((SpecialistTypes)iI, bUsingSpecialist, false, iGrowthValue);
						setYieldValueString(szString, iValue, bUsingSpecialist);
					}
				}
				{
					/* int iFood = GET_PLAYER(pCity->getOwnerINLINE()).AI_averageYieldMultiplier(YIELD_FOOD);
					int iHammer = GET_PLAYER(pCity->getOwnerINLINE()).AI_averageYieldMultiplier(YIELD_PRODUCTION);
					int iCommerce = GET_PLAYER(pCity->getOwnerINLINE()).AI_averageYieldMultiplier(YIELD_COMMERCE);
					
					szString.append(CvWString::format(L"\nPlayer avg:       (f%d, h%d, c%d)", iFood, iHammer, iCommerce));
					
					iFood = pCity->AI_yieldMultiplier(YIELD_FOOD);
					iHammer = pCity->AI_yieldMultiplier(YIELD_PRODUCTION);
					iCommerce = pCity->AI_yieldMultiplier(YIELD_COMMERCE);
					
					szString.append(CvWString::format(L"\nCity yield mults: (f%d, h%d, c%d)", iFood, iHammer, iCommerce));
					
					iFood = pCityAI->AI_specialYieldMultiplier(YIELD_FOOD);
					iHammer = pCityAI->AI_specialYieldMultiplier(YIELD_PRODUCTION);
					iCommerce = pCityAI->AI_specialYieldMultiplier(YIELD_COMMERCE);
					
					szString.append(CvWString::format(L"\nCity spec mults:  (f%d, h%d, c%d)", iFood, iHammer, iCommerce)); */

					// K-Mod
					szString.append(L"\n\nPlayer avg:       (");
					for (YieldTypes i = (YieldTypes)0; i < NUM_YIELD_TYPES; i = (YieldTypes)(i+1))
						szString.append(CvWString::format(L"%s%d%c", i == 0 ? L"" : L", ", GET_PLAYER(pCity->getOwnerINLINE()).AI_averageYieldMultiplier(i), GC.getYieldInfo(i).getChar()));
					szString.append(L")");

					szString.append(L"\nCity yield mults: (");
					for (YieldTypes i = (YieldTypes)0; i < NUM_YIELD_TYPES; i = (YieldTypes)(i+1))
						szString.append(CvWString::format(L"%s%d%c", i == 0 ? L"" : L", ", pCity->AI_yieldMultiplier(i), GC.getYieldInfo(i).getChar()));
					szString.append(L")");

					szString.append(L"\nCity spec mults:  (");
					for (YieldTypes i = (YieldTypes)0; i < NUM_YIELD_TYPES; i = (YieldTypes)(i+1))
						szString.append(CvWString::format(L"%s%d%c", i == 0 ? L"" : L", ", pCityAI->AI_specialYieldMultiplier(i), GC.getYieldInfo(i).getChar()));
					szString.append(L")");

					szString.append(CvWString::format(L"\nCity weights: ("));
					for (CommerceTypes i = (CommerceTypes)0; i < NUM_COMMERCE_TYPES; i=(CommerceTypes)(i+1))
						szString.append(CvWString::format(L"%s%d%c", i == 0 ? L"" : L", ", GET_PLAYER(pCity->getOwnerINLINE()).AI_commerceWeight(i, pCity), GC.getCommerceInfo(i).getChar()));
					szString.append(L")");
					// K-Mod end

					szString.append(CvWString::format(L"\nExchange"));
					for (int iI = 0; iI < NUM_COMMERCE_TYPES; ++iI)
					{
						int iCommerce = GET_PLAYER(pCity->getOwnerINLINE()).AI_averageCommerceExchange((CommerceTypes)iI);
						szTempBuffer.Format(L", %d%c", iCommerce, GC.getCommerceInfo((CommerceTypes) iI).getChar());
						szString.append(szTempBuffer);
					}

					// BBAI
					szString.append(CvWString::format(L"\nAvg mults"));
					for (int iI = 0; iI < NUM_COMMERCE_TYPES; ++iI)
					{
						int iCommerce = GET_PLAYER(pCity->getOwnerINLINE()).AI_averageCommerceMultiplier((CommerceTypes)iI);
						szTempBuffer.Format(L", %d%c", iCommerce, GC.getCommerceInfo((CommerceTypes) iI).getChar());
						szString.append(szTempBuffer);
					}
					// BBAI end
					// K-Mod
					szString.append(CvWString::format(L"\nAvg %c pressure: %d",
						GC.getCommerceInfo(COMMERCE_CULTURE).getChar(),
						GET_PLAYER(pCity->getOwnerINLINE()).AI_averageCulturePressure()));
					// K-Mod end

					if (GET_PLAYER(pCity->getOwnerINLINE()).AI_isFinancialTrouble())
					{
						szTempBuffer.Format(L"$$$!!!");
						szString.append(szTempBuffer);
					}
				}
			}
			else
			{
				bool bWorkingPlot = pCity->isWorkingPlot(pPlot);

				if (bWorkingPlot)
					szTempBuffer.Format( SETCOLR L"%s is working" ENDCOLR, TEXT_COLOR("COLOR_ALT_HIGHLIGHT_TEXT"), pCity->getName().GetCString());
				else
					szTempBuffer.Format( SETCOLR L"%s not working" ENDCOLR, TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), pCity->getName().GetCString());
				szString.append(szTempBuffer);

				int iValue = pCityAI->AI_plotValue(pPlot, bWorkingPlot, false, false, iGrowthValue);
				int iRawValue = pCityAI->AI_plotValue(pPlot, bWorkingPlot, true, false, iGrowthValue);
				int iMagicValue = pCityAI->AI_getPlotMagicValue(pPlot, pCityAI->healthRate() == 0);

				szTempBuffer.Format(L"\nvalue = %d\nraw value = %d\nmagic value = %d", iValue, iRawValue, iMagicValue);
				szString.append(szTempBuffer);
			}
		}
	
		// calc some bonus info
		if (GC.getGameINLINE().isDebugMode())
		{
			eBonus = pPlot->getBonusType();
		}
		else
		{
			eBonus = pPlot->getBonusType(GC.getGameINLINE().getActiveTeam());
		}
		if (eBonus != NO_BONUS)
		{
			szString.append(CvWString::format(L"\n%s values:", GC.getBonusInfo(eBonus).getDescription()));
			
			for (int iPlayerIndex = 0; iPlayerIndex < MAX_PLAYERS; iPlayerIndex++)
			{
				CvPlayerAI& kLoopPlayer = GET_PLAYER((PlayerTypes) iPlayerIndex);
				if (kLoopPlayer.isAlive())
				{
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      07/11/08                                jdog5000      */
/*                                                                                              */
/* DEBUG                                                                                        */
/************************************************************************************************/
/* original code
					szString.append(CvWString::format(L"\n %s: %d", kLoopPlayer.getName(), kLoopPlayer.AI_bonusVal(eBonus)));
*/
					BonusTypes eNonObsBonus = pPlot->getNonObsoleteBonusType(kLoopPlayer.getTeam());
					if( eNonObsBonus != NO_BONUS )
					{
						szString.append(CvWString::format(L"\n %s: %d", kLoopPlayer.getName(), kLoopPlayer.AI_bonusVal(eNonObsBonus, 0, true)));
					}
					else
					{
						szString.append(CvWString::format(L"\n %s: unknown (%d)", kLoopPlayer.getName(), kLoopPlayer.AI_bonusVal(eBonus, 0, true)));
					}
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/
				}
			}
		}
	}
	else
	{
		eRevealOwner = pPlot->getRevealedOwner(GC.getGameINLINE().getActiveTeam(), true);

		if (eRevealOwner != NO_PLAYER)
		{
			if (pPlot->isActiveVisible(true))
			{
				szTempBuffer.Format(L"%d%% " SETCOLR L"%s" ENDCOLR, pPlot->calculateCulturePercent(eRevealOwner), GET_PLAYER(eRevealOwner).getPlayerTextColorR(), GET_PLAYER(eRevealOwner).getPlayerTextColorG(), GET_PLAYER(eRevealOwner).getPlayerTextColorB(), GET_PLAYER(eRevealOwner).getPlayerTextColorA(), GET_PLAYER(eRevealOwner).getCivilizationAdjective());
				szString.append(szTempBuffer);
				szString.append(NEWLINE);

				for (int iPlayer = 0; iPlayer < MAX_PLAYERS; ++iPlayer)
				{
					if (iPlayer != eRevealOwner)
					{
						CvPlayer& kPlayer = GET_PLAYER((PlayerTypes)iPlayer);
						if (kPlayer.isEverAlive() && pPlot->getCulture((PlayerTypes)iPlayer) > 0)
						{
						/**
						*** K-Mod, 29/sep/10, Karadoc
						*** Prevented display of 0% culture, to reduce the spam created by trade culture.
						***/
							/* original bts code
							szTempBuffer.Format(L"%d%% " SETCOLR L"%s" ENDCOLR, pPlot->calculateCulturePercent((PlayerTypes)iPlayer), kPlayer.getPlayerTextColorR(), kPlayer.getPlayerTextColorG(), kPlayer.getPlayerTextColorB(), kPlayer.getPlayerTextColorA(), kPlayer.getCivilizationAdjective());
							szString.append(szTempBuffer);
							szString.append(NEWLINE);
							*/
							int iCulturePercent = pPlot->calculateCulturePercent((PlayerTypes)iPlayer);
							if (iCulturePercent >= 1)
							{
								szTempBuffer.Format(L"%d%% " SETCOLR L"%s" ENDCOLR, iCulturePercent, kPlayer.getPlayerTextColorR(), kPlayer.getPlayerTextColorG(), kPlayer.getPlayerTextColorB(), kPlayer.getPlayerTextColorA(), kPlayer.getCivilizationAdjective());
								szString.append(szTempBuffer);
								szString.append(NEWLINE);
							}
						/* K-Mod end */
						}
					}
				}
				
				// Mod start IDW
				
				if (GC.getGameINLINE().isOption(GAMEOPTION_IDW))
				{
					if (pPlot->isCity())
					{
						CvCity* pCity = pPlot->getPlotCity();
						int iPopulation = pCity->getPopulation();
						if (iPopulation > (GC.getDefineINT("CONSCRIPT_MIN_CITY_POPULATION") - 1))
						{
							int iCityLevel = pCity->getCultureLevel();
							int iPlotCulture = pPlot->calculateCulturePercent(eRevealOwner);
							int iMaxRandom = (GC.getNumCultureLevelInfos() - 1) * 100;
							int iIDWChance = std::min(iCityLevel * iPlotCulture * 100 / iMaxRandom,100);
							szString.append(gDLL->getText("TXT_KEY_IDW_DRAFT_CHANCE", iIDWChance));
							szString.append(NEWLINE);
						}
						else
						{
							if (pCity->isDisorder())
							{
								szString.append(gDLL->getText("TXT_KEY_IDW_DISORDER"));
							}
							else
							{
								szString.append(gDLL->getText("TXT_KEY_IDW_NO_DRAFT", GC.getDefineINT("CONSCRIPT_MIN_CITY_POPULATION") - 1));
							}
							szString.append(NEWLINE);
						}
						
					}
				}
				// Mod end IDW
			}
			else
			{
				szTempBuffer.Format(SETCOLR L"%s" ENDCOLR, GET_PLAYER(eRevealOwner).getPlayerTextColorR(), GET_PLAYER(eRevealOwner).getPlayerTextColorG(), GET_PLAYER(eRevealOwner).getPlayerTextColorB(), GET_PLAYER(eRevealOwner).getPlayerTextColorA(), GET_PLAYER(eRevealOwner).getCivilizationDescription());
				szString.append(szTempBuffer);
				szString.append(NEWLINE);
			}
		}

		iDefenseModifier = pPlot->defenseModifier((eRevealOwner != NO_PLAYER ? GET_PLAYER(eRevealOwner).getTeam() : NO_TEAM), true, true);

		if (iDefenseModifier != 0)
		{
			szString.append(gDLL->getText("TXT_KEY_PLOT_BONUS", iDefenseModifier));
			szString.append(NEWLINE);
		}

		if (pPlot->getTerrainType() != NO_TERRAIN)
		{
			if (pPlot->isPeak())
			{
				szString.append(gDLL->getText("TXT_KEY_PLOT_PEAK"));
			}
			else
			{
				if (pPlot->isWater())
				{
					szTempBuffer.Format(SETCOLR, TEXT_COLOR("COLOR_WATER_TEXT"));
					szString.append(szTempBuffer);
				}

				if (pPlot->isHills())
				{
					szString.append(gDLL->getText("TXT_KEY_PLOT_HILLS"));
				}

				if (pPlot->getFeatureType() != NO_FEATURE)
				{
					szTempBuffer.Format(L"%s/", GC.getFeatureInfo(pPlot->getFeatureType()).getDescription());
					szString.append(szTempBuffer);
				}

				szString.append(GC.getTerrainInfo(pPlot->getTerrainType()).getDescription());

				if (pPlot->isWater())
				{
					szString.append(ENDCOLR);
				}
			}
		}

		if (pPlot->hasYield())
		{
			for (iI = 0; iI < NUM_YIELD_TYPES; ++iI)
			{
				iYield = pPlot->calculateYield(((YieldTypes)iI), true);

				if (iYield != 0)
				{
					szTempBuffer.Format(L", %d%c", iYield, GC.getYieldInfo((YieldTypes) iI).getChar());
					szString.append(szTempBuffer);
				}
			}

			int iNuclearWinterEffect = GC.getGameINLINE().getNuclearWinterFoodYieldEffect(GC.getGameINLINE().getNuclearWinterLevel());
			if (iNuclearWinterEffect > 0)
			{
				szString.append(NEWLINE);
				szString.append(gDLL->getText("TXT_KEY_PLOT_NUCLEAR_WINTER_EFFECT"));
				
				szTempBuffer.Format(L": -%d%c", iNuclearWinterEffect, GC.getYieldInfo(YIELD_FOOD).getChar());
				szString.append(szTempBuffer);
			}
		}

		if (pPlot->isFreshWater())
		{
			szString.append(NEWLINE);
			szString.append(gDLL->getText("TXT_KEY_PLOT_FRESH_WATER"));
		}

		if (pPlot->isLake())
		{
			szString.append(NEWLINE);
			szString.append(gDLL->getText("TXT_KEY_PLOT_FRESH_WATER_LAKE"));
		}

		if (pPlot->isImpassable())
		{
			szString.append(NEWLINE);
			szString.append(gDLL->getText("TXT_KEY_PLOT_IMPASSABLE"));
		}

		if (GC.getGameINLINE().isDebugMode())
		{
			eBonus = pPlot->getBonusType();
		}
		else
		{
			eBonus = pPlot->getBonusType(GC.getGameINLINE().getActiveTeam());
		}

		if (eBonus != NO_BONUS)
		{
			CvBonusInfo& kBonus = GC.getBonusInfo(eBonus);

			szTempBuffer.Format(L"%c " SETCOLR L"%s" ENDCOLR, GC.getBonusInfo(eBonus).getChar(), TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), GC.getBonusInfo(eBonus).getDescription());
			szString.append(NEWLINE);
			szString.append(szTempBuffer);

			// K-Mod. I've rearranged and edited some of the code in this section to fix some bugs.
			// (for example, obsolete bonuses shouldn't say "requires x improvement"; and neither should bonuses that are already connected with a fort.)
			if (!GET_TEAM(GC.getGameINLINE().getActiveTeam()).isBonusObsolete(eBonus))
			{
				if (kBonus.getHealth() != 0)
				{
					szTempBuffer.Format(L", +%d%c", abs(kBonus.getHealth()), ((kBonus.getHealth() > 0) ? gDLL->getSymbolID(HEALTHY_CHAR): gDLL->getSymbolID(UNHEALTHY_CHAR)));
					szString.append(szTempBuffer);
				}

				if (kBonus.getHappiness() != 0)
				{
					szTempBuffer.Format(L", +%d%c", abs(kBonus.getHappiness()), ((kBonus.getHappiness() > 0) ? gDLL->getSymbolID(HAPPY_CHAR): gDLL->getSymbolID(UNHAPPY_CHAR)));
					szString.append(szTempBuffer);
				}

				if (!(GET_TEAM(GC.getGameINLINE().getActiveTeam()).isHasTech((TechTypes)kBonus.getTechCityTrade())))
				{
					szString.append(gDLL->getText("TXT_KEY_PLOT_RESEARCH", GC.getTechInfo((TechTypes) kBonus.getTechCityTrade()).getTextKeyWide()));
				}

				ImprovementTypes ePlotImprovement = pPlot->getImprovementType();

				if (!pPlot->isCity())
				{
					CvPlayer& kActivePlayer = GET_PLAYER(GC.getGameINLINE().getActivePlayer());
					CvCivilizationInfo& kActiveCivilization = GC.getCivilizationInfo(kActivePlayer.getCivilizationType());
					if (!kActivePlayer.doesImprovementConnectBonus(pPlot->getImprovementType(), eBonus))
					{
						// K-Mod note: unfortunately, the following code assumes that there is only one improvement marked "isImprovementBonusTrade" for each bonus.
						for (iI = 0; iI < GC.getNumBuildInfos(); ++iI)
						{
							BuildTypes eBuild = (BuildTypes)iI;
							CvBuildInfo& kBuild = GC.getBuildInfo(eBuild);

							// MOD - START - Unique Improvements
							//if (kBuild.getImprovement() != NO_IMPROVEMENT)
							if (kBuild.getImprovement() != NO_IMPROVEMENT && (kActiveCivilization.canBuild(eBuild) || kActivePlayer.getBuilderCount(eBuild) > 0))
							// MOD - END - Unique Improvements
							{
								const CvImprovementInfo& kImprovement = GC.getImprovementInfo((ImprovementTypes) kBuild.getImprovement());
								if (kImprovement.isImprovementBonusTrade(eBonus))
								{
									if (pPlot->canHaveImprovement(((ImprovementTypes)(kBuild.getImprovement())), GC.getGameINLINE().getActiveTeam(), true))
									{
										if (GET_TEAM(GC.getGameINLINE().getActiveTeam()).isHasTech((TechTypes)kBuild.getTechPrereq()))
										{
											szString.append(gDLL->getText("TXT_KEY_PLOT_REQUIRES", kImprovement.getTextKeyWide()));
										}
										else if (kBonus.getTechCityTrade() != kBuild.getTechPrereq())
										{
											szString.append(gDLL->getText("TXT_KEY_PLOT_RESEARCH", GC.getTechInfo((TechTypes) kBuild.getTechPrereq()).getTextKeyWide()));
										}

										bool bFirst = true;

										for (int k = 0; k < NUM_YIELD_TYPES; k++)
										{
											int iYieldChange = kImprovement.getImprovementBonusYield(eBonus, k) + kImprovement.getYieldChange(k);
											// MOD - START - Negative Yields
											iYieldChange = std::max(iYieldChange, -pPlot->getYield((YieldTypes)k));
											// MOD - END - Negative Yields
											if (iYieldChange != 0)
											{
												if (iYieldChange > 0)
												{
													szTempBuffer.Format(L"+%d%c", iYieldChange, GC.getYieldInfo((YieldTypes)k).getChar());
												}
												else
												{
													szTempBuffer.Format(L"%d%c", iYieldChange, GC.getYieldInfo((YieldTypes)k).getChar());
												}
												setListHelp(szString, L"\n", szTempBuffer, L", ", bFirst);
												bFirst = false;
											}
										}

										if (!bFirst)
										{
											szString.append(gDLL->getText("TXT_KEY_BONUS_WITH_IMPROVEMENT", kImprovement.getTextKeyWide()));
										}

										break;
									}
								}
							}
						}
					}
					if (!(pPlot->isBonusNetwork(GC.getGameINLINE().getActiveTeam())))
					{
						szString.append(gDLL->getText("TXT_KEY_PLOT_REQUIRES_ROUTE"));
					}
				}

				// K-Mod note: I'm not sure whether or not the help should be displayed when the bonus is obsolete.
				// The fact is, none of the bonuses have help text in K-Mod (or standard bts.)
				if (!CvWString(kBonus.getHelp()).empty())
				{
					szString.append(NEWLINE);
					szString.append(kBonus.getHelp());
				}
			} // end !isBonusObsolete
		}

		eImprovement = pPlot->getRevealedImprovementType(GC.getGameINLINE().getActiveTeam(), true);

		if (eImprovement != NO_IMPROVEMENT)
		{
			const CvImprovementInfo& kImprovement = GC.getImprovementInfo(eImprovement);

			szString.append(NEWLINE);
			szString.append(kImprovement.getDescription());

			bFound = false;

			for (iI = 0; iI < NUM_YIELD_TYPES; ++iI)
			{
				if (kImprovement.getIrrigatedYieldChange(iI) != 0)
				{
					bFound = true;
					break;
				}
			}

			if (bFound)
			{
				if (pPlot->isIrrigationAvailable())
				{
					szString.append(gDLL->getText("TXT_KEY_PLOT_IRRIGATED"));
				}
				else
				{
					szString.append(gDLL->getText("TXT_KEY_PLOT_NOT_IRRIGATED"));
				}
			}

			if (kImprovement.getImprovementUpgrade() != NO_IMPROVEMENT)
			{
				if ((pPlot->getUpgradeProgress() > 0) || pPlot->isBeingWorked())
				{
					iTurns = pPlot->getUpgradeTimeLeft(eImprovement, eRevealOwner);

					szString.append(gDLL->getText("TXT_KEY_PLOT_IMP_UPGRADE", iTurns, GC.getImprovementInfo((ImprovementTypes) kImprovement.getImprovementUpgrade()).getTextKeyWide()));
				}
				else
				{
					szString.append(gDLL->getText("TXT_KEY_PLOT_WORK_TO_UPGRADE", GC.getImprovementInfo((ImprovementTypes) kImprovement.getImprovementUpgrade()).getTextKeyWide()));
				}
			}
		}

		if (pPlot->getRevealedRouteType(GC.getGameINLINE().getActiveTeam(), true) != NO_ROUTE)
		{
			szString.append(NEWLINE);
			szString.append(GC.getRouteInfo(pPlot->getRevealedRouteType(GC.getGameINLINE().getActiveTeam(), true)).getDescription());
		}

/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      07/11/08                                jdog5000      */
/*                                                                                              */
/* DEBUG                                                                                        */
/************************************************************************************************/
/* original code
		if (pPlot->getBlockadedCount(GC.getGameINLINE().getActiveTeam()) > 0)
*/
		if( GC.getGameINLINE().isDebugMode() )
		{
			bool bFirst = true;
			for (int iK = 0; iK < MAX_TEAMS; ++iK)
			{
				TeamTypes eTeam = (TeamTypes)iK;
				if( pPlot->getBlockadedCount(eTeam) > 0 && GET_TEAM(eTeam).isAlive() )
				{
					if( bFirst )
					{
						szString.append(CvWString::format(SETCOLR, TEXT_COLOR("COLOR_NEGATIVE_TEXT")));
						szString.append(NEWLINE);
						szString.append(gDLL->getText("TXT_KEY_PLOT_BLOCKADED"));
						szString.append(CvWString::format( ENDCOLR));

						szString.append(CvWString::format(L"Teams:"));
						bFirst = false;
					}
					szString.append(CvWString::format(L" %s,", GET_TEAM(eTeam).getName().c_str()));
				}
			}
		}
		else if (pPlot->getBlockadedCount(GC.getGameINLINE().getActiveTeam()) > 0)
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/
		{
			szString.append(CvWString::format(SETCOLR, TEXT_COLOR("COLOR_NEGATIVE_TEXT")));
			szString.append(NEWLINE);
			szString.append(gDLL->getText("TXT_KEY_PLOT_BLOCKADED"));
			szString.append(CvWString::format( ENDCOLR));
		}

	}

	if (pPlot->getFeatureType() != NO_FEATURE)
	{
		int iDamage = GC.getFeatureInfo(pPlot->getFeatureType()).getTurnDamage();

		if (iDamage > 0)
		{
			szString.append(CvWString::format(SETCOLR, TEXT_COLOR("COLOR_NEGATIVE_TEXT")));
			szString.append(NEWLINE);
			szString.append(gDLL->getText("TXT_KEY_PLOT_DAMAGE", iDamage));
			szString.append(CvWString::format( ENDCOLR));
		}
/*************************************************************************************************/
/* UNOFFICIAL_PATCH                       06/02/10                           LunarMongoose       */
/*                                                                                               */
/* User interface                                                                                */
/*************************************************************************************************/
		// Mongoose FeatureDamageFix
		else if (iDamage < 0)
		{
			szString.append(CvWString::format(SETCOLR, TEXT_COLOR("COLOR_POSITIVE_TEXT")));
			szString.append(NEWLINE);
			szString.append(gDLL->getText("TXT_KEY_PLOT_DAMAGE", iDamage));
			szString.append(CvWString::format( ENDCOLR));
		}
/*************************************************************************************************/
/* UNOFFICIAL_PATCH                         END                                                  */
/*************************************************************************************************/
	}
	szString.assign(CvWString::format(L"<font=1>%s</font>", szString));
}

// MOD - START - Aid and Detriments
void CvGameTextMgr::setPlotAidAndDetrimentHelp(CvWStringBuffer& szString, CvPlot* pPlot)
{
	if (!szString.isEmpty())
	{
		szString.append(L"\n");
	}

	int iNumAidInfos = GC.getNumAidInfos();
	int iNumDetrimentInfos = GC.getNumDetrimentInfos();
	bool bFirst = true;

	for (int iI = 0; iI < MAX_TEAMS; iI++)
	{
		TeamTypes eLoopTeam = (TeamTypes)iI;
		CvTeamAI& kLoopTeam = GET_TEAM(eLoopTeam);

		if (kLoopTeam.isAlive())
		{
			if ((pPlot->m_apaiTeamAidAmount != NULL) && (pPlot->m_apaiTeamAidAmount[eLoopTeam] != NULL))
			{
				for (int iJ = 0; iJ < iNumAidInfos; iJ++)
				{
					AidTypes eLoopAid = (AidTypes)iJ;
					int iAmount = pPlot->m_apaiTeamAidAmount[eLoopTeam][eLoopAid];
					if (iAmount != 0)
					{
						const CvAidInfo& kLoopAid = GC.getAidInfo(eLoopAid);
						szString.append(CvWString::format(L"%s%s %s: %d", !bFirst ? L"\n" : L"", kLoopTeam.getName().GetCString(), kLoopAid.getDescription(), iAmount));
						bFirst = false;
					}
				}
			}

			if ((pPlot->m_apaiTeamDetrimentAmount != NULL) && (pPlot->m_apaiTeamDetrimentAmount[eLoopTeam] != NULL))
			{
				for (int iJ = 0; iJ < iNumDetrimentInfos; iJ++)
				{
					DetrimentTypes eLoopDetriment = (DetrimentTypes)iJ;
					int iAmount = pPlot->m_apaiTeamDetrimentAmount[eLoopTeam][eLoopDetriment];
					if (iAmount != 0)
					{
						const CvDetrimentInfo& kLoopDetriment = GC.getDetrimentInfo(eLoopDetriment);
						szString.append(CvWString::format(L"%s%s %s: %d", !bFirst ? L"\n" : L"", kLoopTeam.getName().GetCString(), kLoopDetriment.getDescription(), iAmount));
						bFirst = false;
					}
				}
			}
		}
	}

	if (bFirst)
	{
		szString.append(L"No Aid or Detriments");
	}
}
// MOD - END - Aid and Detriments

void CvGameTextMgr::setCityPlotYieldValueString(CvWStringBuffer &szString, CvCity* pCity, int iIndex, bool bIgnoreFood, int iGrowthValue)
{
	PROFILE_FUNC();

	CvPlot* pPlot = NULL;

	if (iIndex >= 0 && iIndex < NUM_CITY_PLOTS)
		pPlot = pCity->getCityIndexPlot(iIndex);

	if (pPlot != NULL && pPlot->getWorkingCity() == pCity)
	{
		CvCityAI* pCityAI = static_cast<CvCityAI*>(pCity);
		bool bWorkingPlot = pCity->isWorkingPlot(iIndex);

		int iValue = pCityAI->AI_plotValue(pPlot, bWorkingPlot, bIgnoreFood, false, iGrowthValue);
		
		setYieldValueString(szString, iValue, /*bActive*/ bWorkingPlot);
	}
	else
		setYieldValueString(szString, 0, /*bActive*/ false, /*bMakeWhitespace*/ true);
}

void CvGameTextMgr::setYieldValueString(CvWStringBuffer &szString, int iValue, bool bActive, bool bMakeWhitespace)
{
	PROFILE_FUNC();

	static bool bUseFloats = false;
	
	if (bActive)
		szString.append(CvWString::format(SETCOLR, TEXT_COLOR("COLOR_ALT_HIGHLIGHT_TEXT")));
	else
		szString.append(CvWString::format(SETCOLR, TEXT_COLOR("COLOR_HIGHLIGHT_TEXT")));

	if (!bMakeWhitespace)
	{
		if (bUseFloats)
		{
			float fValue = ((float) iValue) / 10000;
			szString.append(CvWString::format(L"%2.3f " ENDCOLR, fValue));
		}
		else
			szString.append(CvWString::format(L"%05d  " ENDCOLR, iValue/10));
	}
	else
		szString.append(CvWString::format(L"           " ENDCOLR));
}

void CvGameTextMgr::setCityBarHelp(CvWStringBuffer &szString, CvCity* pCity)
{
	PROFILE_FUNC();

	CvWString szTempBuffer;
	bool bFirst;
	int iFoodDifference;
	int iProductionDiffNoFood;
	int iProductionDiffJustFood;
	int iRate;
	int iI;

	iFoodDifference = pCity->foodDifference();

	szString.append(pCity->getName());

	if (iFoodDifference <= 0)
	{
		szString.append(gDLL->getText("TXT_KEY_CITY_BAR_GROWTH", pCity->getFood(), pCity->growthThreshold()));
	}
	else
	{
		// MOD - START - City Growth Limits
		//szString.append(gDLL->getText("TXT_KEY_CITY_BAR_FOOD_GROWTH", iFoodDifference, pCity->getFood(), pCity->growthThreshold(), pCity->getFoodTurnsLeft()));
		int iTurnsToGrow = pCity->getFoodTurnsLeft();
		if (pCity->isGrowthAllowed())
		{
			szString.append(gDLL->getText("TXT_KEY_CITY_BAR_FOOD_GROWTH", iFoodDifference, pCity->getFood(), pCity->growthThreshold(), iTurnsToGrow));
		}
		else if (iTurnsToGrow > 1)
		{
			szString.append(gDLL->getText("TXT_KEY_CITY_BAR_FOOD_GROWTH_LIMITED_TURNS", iFoodDifference, pCity->getFood(), pCity->growthThreshold(), iTurnsToGrow));
		}
		else
		{
			szString.append(gDLL->getText("TXT_KEY_CITY_BAR_FOOD_GROWTH_LIMITED", iFoodDifference, pCity->getFood(), pCity->growthThreshold()));
		}
		// MOD - END - City Growth Limits
	}
	if (pCity->getProductionNeeded() != MAX_INT)
	{
		iProductionDiffNoFood = pCity->getCurrentProductionDifference(true, true);
		iProductionDiffJustFood = (pCity->getCurrentProductionDifference(false, true) - iProductionDiffNoFood);

		if (iProductionDiffJustFood > 0)
		{
			szString.append(gDLL->getText("TXT_KEY_CITY_BAR_FOOD_HAMMER_PRODUCTION", iProductionDiffJustFood, iProductionDiffNoFood, pCity->getProductionName(), pCity->getProduction(), pCity->getProductionNeeded(), pCity->getProductionTurnsLeft()));
		}
		else if (iProductionDiffNoFood > 0)
		{
			szString.append(gDLL->getText("TXT_KEY_CITY_BAR_HAMMER_PRODUCTION", iProductionDiffNoFood, pCity->getProductionName(), pCity->getProduction(), pCity->getProductionNeeded(), pCity->getProductionTurnsLeft()));
		}
		else
		{
			szString.append(gDLL->getText("TXT_KEY_CITY_BAR_PRODUCTION", pCity->getProductionName(), pCity->getProduction(), pCity->getProductionNeeded()));
		}
	}

	bFirst = true;

	for (iI = 0; iI < NUM_COMMERCE_TYPES; ++iI)
	{
		iRate = pCity->getCommerceRateTimes100((CommerceTypes)iI);

		if (iRate != 0)
		{
			szTempBuffer.Format(L"%d.%02d %c", iRate/100, iRate%100, GC.getCommerceInfo((CommerceTypes)iI).getChar());
			setListHelp(szString, NEWLINE, szTempBuffer, L", ", bFirst);
			bFirst = false;
		}
	}

	iRate = pCity->getGreatPeopleRate() / 100;

	if (iRate != 0)
	{
		szTempBuffer.Format(L"%d%c", iRate, gDLL->getSymbolID(GREAT_PEOPLE_CHAR));
		setListHelp(szString, NEWLINE, szTempBuffer, L", ", bFirst);
		bFirst = false;
	}

	if (!bFirst)
	{
		szString.append(gDLL->getText("TXT_KEY_PER_TURN"));
	}

	szString.append(NEWLINE);
	szString.append(gDLL->getText("INTERFACE_CITY_MAINTENANCE"));
	//int iMaintenance = pCity->getMaintenanceTimes100();
	int iMaintenance = pCity->getMaintenanceTimes100() * (100+GET_PLAYER(pCity->getOwnerINLINE()).getInflationRate()) / 100; // K-Mod
	szString.append(CvWString::format(L" -%d.%02d %c", iMaintenance/100, iMaintenance%100, GC.getCommerceInfo(COMMERCE_GOLD).getChar()));

	bFirst = true;
	for (iI = 0; iI < GC.getNumBuildingInfos(); ++iI)
	{
		if (pCity->getNumRealBuilding((BuildingTypes)iI) > 0)
		{
			setListHelp(szString, NEWLINE, GC.getBuildingInfo((BuildingTypes)iI).getDescription(), L", ", bFirst);
			bFirst = false;
		}
	}

	if (pCity->getCultureLevel() != NO_CULTURELEVEL)
	{
		szString.append(gDLL->getText("TXT_KEY_CITY_BAR_CULTURE", pCity->getCulture(pCity->getOwnerINLINE()), pCity->getCultureThreshold(), GC.getCultureLevelInfo(pCity->getCultureLevel()).getTextKeyWide()));
	}

	if (pCity->getGreatPeopleProgress() > 0)
	{
		szString.append(gDLL->getText("TXT_KEY_CITY_BAR_GREAT_PEOPLE", pCity->getGreatPeopleProgress(), GET_PLAYER(pCity->getOwnerINLINE()).greatPeopleThreshold(false)));
	}

	int iNumUnits = pCity->plot()->countNumAirUnits(GC.getGameINLINE().getActiveTeam());
	if (pCity->getAirUnitCapacity(GC.getGameINLINE().getActiveTeam()) > 0 && iNumUnits > 0)
	{
		szString.append(NEWLINE);
		szString.append(gDLL->getText("TXT_KEY_CITY_BAR_AIR_UNIT_CAPACITY", iNumUnits, pCity->getAirUnitCapacity(GC.getGameINLINE().getActiveTeam())));
	}

	szString.append(NEWLINE);

	szString.append(gDLL->getText("TXT_KEY_CITY_BAR_SELECT", pCity->getNameKey()));
	szString.append(gDLL->getText("TXT_KEY_CITY_BAR_SELECT_CTRL"));
	szString.append(gDLL->getText("TXT_KEY_CITY_BAR_SELECT_ALT"));
}


void CvGameTextMgr::parseTraits(CvWStringBuffer &szHelpString, TraitTypes eTrait, CivilizationTypes eCivilization, bool bDawnOfMan, bool bCivilopediaText)
{
	PROFILE_FUNC();

	CvWString szTempBuffer;
	BuildingTypes eLoopBuilding;
	UnitTypes eLoopUnit;
	int iLast;
	CvWString szText;

	CvTraitInfo& kTrait = GC.getTraitInfo(eTrait);

	// MOD - START - Improved Civilopedia
	if (!bCivilopediaText)
	{
		// Trait Name
		szText = kTrait.getDescription();
		if (bDawnOfMan)
		{
			szTempBuffer.Format(L"%s", szText.GetCString());
		}
		else
		{
			szTempBuffer.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR("COLOR_ALT_HIGHLIGHT_TEXT"), szText.GetCString());
		}
		szHelpString.append(szTempBuffer);
	}
	// MOD - END - Improved Civilopedia

	if (!bDawnOfMan)
	{
		if (!CvWString(kTrait.getHelp()).empty())
		{
			szHelpString.append(kTrait.getHelp());
		}

		// iHealth
		if (kTrait.getHealth() != 0)
		{
			szHelpString.append(gDLL->getText("TXT_KEY_TRAIT_HEALTH", kTrait.getHealth()));
		}

		// iHappiness
		if (kTrait.getHappiness() != 0)
		{
			szHelpString.append(gDLL->getText("TXT_KEY_TRAIT_HAPPINESS", kTrait.getHappiness()));
		}

		// MOD - START - Trait Attitude Modifier
		if (kTrait.getAttitudeModifier() != 0)
		{
			szHelpString.append(gDLL->getText("TXT_KEY_TRAIT_ATTITUDE", kTrait.getAttitudeModifier()));
		}
		// MOD - END - Trait Attitude Modifier

		// MOD - START - Epidemics
		// Epidemic system by Mexico
		if (kTrait.getGlobalEpidemicModifier() != 0)
		{
			szTempBuffer.Format(L"%+d", (kTrait.getGlobalEpidemicModifier() * 100) / GC.getDefineINT("EPIDEMIC_RAND_BASE"));
			szHelpString.append(gDLL->getText("TXT_KEY_TRAIT_GLOBAL_EPIDEMIC_MODIFIER", szTempBuffer.GetCString()));
		}
		// MOD - END - Epidemics

		// iMaxAnarchy
		if (kTrait.getMaxAnarchy() != -1)
		{
			if (kTrait.getMaxAnarchy() == 0)
			{
				szHelpString.append(gDLL->getText("TXT_KEY_TRAIT_NO_ANARCHY"));
			}
			// MOD - START - Standard Maximum Anarchy Length: this is just a comment, no changes here ATM
			// Absinthe: could directly remove the 3 turns anarchy text here, but probably better to add a default hard cap for max turns instead
			// MOD - END - Standard Maximum Anarchy Length
			else
			{
				szHelpString.append(gDLL->getText("TXT_KEY_TRAIT_MAX_ANARCHY", kTrait.getMaxAnarchy()));
			}
		}

		// iUpkeepModifier
		if (kTrait.getUpkeepModifier() != 0)
		{
			szHelpString.append(gDLL->getText("TXT_KEY_TRAIT_CIVIC_UPKEEP_MODIFIER", kTrait.getUpkeepModifier()));
		}

		// iLevelExperienceModifier
		if (kTrait.getLevelExperienceModifier() != 0)
		{
			szHelpString.append(gDLL->getText("TXT_KEY_TRAIT_CIVIC_LEVEL_MODIFIER", kTrait.getLevelExperienceModifier()));
		}

		// MOD - START - Trait Upgrade Modifier
		if (kTrait.getUpgradeDiscount() != 0)
		{
			szHelpString.append(gDLL->getText("TXT_KEY_TRAIT_UPGRADE_MODIFIER", -kTrait.getUpgradeDiscount()));
		}
		// MOD - END - Trait Upgrade Modifier

		// iGreatPeopleRateModifier
		if (kTrait.getGreatPeopleRateModifier() != 0)
		{
			szHelpString.append(gDLL->getText("TXT_KEY_TRAIT_GREAT_PEOPLE_MODIFIER", kTrait.getGreatPeopleRateModifier()));
		}

		// iGreatGeneralRateModifier
		if (kTrait.getGreatGeneralRateModifier() != 0)
		{
			szHelpString.append(gDLL->getText("TXT_KEY_TRAIT_GREAT_GENERAL_MODIFIER", kTrait.getGreatGeneralRateModifier()));
		}

		if (kTrait.getDomesticGreatGeneralRateModifier() != 0)
		{
			szHelpString.append(gDLL->getText("TXT_KEY_TRAIT_DOMESTIC_GREAT_GENERAL_MODIFIER", kTrait.getDomesticGreatGeneralRateModifier()));
		}

		// MOD - START - Trait Golden Age Modifier
		if (kTrait.getGoldenAgeModifier() != 0)
		{
			szHelpString.append(gDLL->getText("TXT_KEY_TRAIT_GOLDEN_AGE_MODIFIER", kTrait.getGoldenAgeModifier()));
		}
		// MOD - END - Trait Golden Age Modifier

		// Wonder Production Effects
		if ((kTrait.getMaxGlobalBuildingProductionModifier() != 0)
			|| (kTrait.getMaxTeamBuildingProductionModifier() != 0)
			|| (kTrait.getMaxPlayerBuildingProductionModifier() != 0))
		{
			if ((kTrait.getMaxGlobalBuildingProductionModifier() == kTrait.getMaxTeamBuildingProductionModifier())
				&& 	(kTrait.getMaxGlobalBuildingProductionModifier() == kTrait.getMaxPlayerBuildingProductionModifier()))
			{
				szHelpString.append(gDLL->getText("TXT_KEY_TRAIT_WONDER_PRODUCTION_MODIFIER", kTrait.getMaxGlobalBuildingProductionModifier()));
			}
			else
			{
				if (kTrait.getMaxGlobalBuildingProductionModifier() != 0)
				{
					szHelpString.append(gDLL->getText("TXT_KEY_TRAIT_WORLD_WONDER_PRODUCTION_MODIFIER", kTrait.getMaxGlobalBuildingProductionModifier()));
				}

				if (kTrait.getMaxTeamBuildingProductionModifier() != 0)
				{
					szHelpString.append(gDLL->getText("TXT_KEY_TRAIT_TEAM_WONDER_PRODUCTION_MODIFIER", kTrait.getMaxTeamBuildingProductionModifier()));
				}

				if (kTrait.getMaxPlayerBuildingProductionModifier() != 0)
				{
					szHelpString.append(gDLL->getText("TXT_KEY_TRAIT_NATIONAL_WONDER_PRODUCTION_MODIFIER", kTrait.getMaxPlayerBuildingProductionModifier()));
				}
			}
		}

		// MOD - START - Trait Worker Speed Modifier
		if (kTrait.getWorkerSpeedModifier() != 0)
		{
			szHelpString.append(gDLL->getText("TXT_KEY_TRAIT_WORKER_SPEED_MODIFIER", kTrait.getWorkerSpeedModifier()));
		}
		// MOD - END - Trait Worker Speed Modifier

		// ExtraYieldThresholds
		for (int iI = 0; iI < NUM_YIELD_TYPES; ++iI)
		{
			if (kTrait.getExtraYieldThreshold(iI) > 0)
			{
				szHelpString.append(gDLL->getText("TXT_KEY_TRAIT_EXTRA_YIELD_THRESHOLDS", GC.getYieldInfo((YieldTypes) iI).getChar(), kTrait.getExtraYieldThreshold(iI), GC.getYieldInfo((YieldTypes) iI).getChar()));
			}
			// Trade Yield Modifiers
			if (kTrait.getTradeYieldModifier(iI) != 0)
			{
				szHelpString.append(gDLL->getText("TXT_KEY_TRAIT_TRADE_YIELD_MODIFIERS", kTrait.getTradeYieldModifier(iI), GC.getYieldInfo((YieldTypes) iI).getChar(), "YIELD"));
			}
			// MOD - START - Trait City Yield Bonuses
			if (kTrait.getCityYieldBonus(iI) != 0)
			{
				szHelpString.append(gDLL->getText("TXT_KEY_TRAIT_CITY_YIELD_BONUSES", kTrait.getCityYieldBonus(iI), GC.getYieldInfo((YieldTypes) iI).getChar(), "YIELD"));
			}	
			// MOD - END - Trait City Yield Bonuses
		}

		// CommerceChanges
		for (int iI = 0; iI < NUM_COMMERCE_TYPES; ++iI)
		{
			if (kTrait.getCommerceChange(iI) != 0)
			{
				szHelpString.append(gDLL->getText("TXT_KEY_TRAIT_COMMERCE_CHANGES", 	kTrait.getCommerceChange(iI), GC.getCommerceInfo((CommerceTypes) iI).getChar(), "COMMERCE"));
			}

			if (kTrait.getCommerceModifier(iI) != 0)
			{
				szHelpString.append(gDLL->getText("TXT_KEY_TRAIT_COMMERCE_MODIFIERS", kTrait.getCommerceModifier(iI), GC.getCommerceInfo((CommerceTypes) iI).getChar(), "COMMERCE"));
			}
		}

		// MOD - START - Advanced Yield and Commerce Effects
		bool bPlayerContext = GC.getGameINLINE().getActivePlayer() != NO_PLAYER;
		CvWString szFirstBuffer;
		for (int iI = 0; iI < NUM_YIELD_TYPES; ++iI)
		{
			YieldTypes eLoopYield = (YieldTypes)iI;
			iLast = 0;

			//	Improvement Yields
			for (int iJ = 0; iJ < kTrait.getNumImprovementYieldChanges(); iJ++)
			{
				if (kTrait.getImprovementYieldChangeYield(iJ) == eLoopYield)
				{
					ImprovementTypes eLoopImprovement = (ImprovementTypes)kTrait.getImprovementYieldChangeImprovement(iJ);
					// MOD - START - Unique Improvements
					// TODO: Display improvements only for players in the current game who possess the trait and can produce the improvement?
					//if (!bPlayerContext || GC.getCivilizationInfo(GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getCivilizationType()).canProduceImprovement(eLoopImprovement))
					{
					// MOD - END - Unique Improvements
						int iValue = kTrait.getImprovementYieldChangeValue(iJ);
						szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_CIVIC_IMPROVEMENT_YIELD_CHANGE", iValue, GC.getYieldInfo(eLoopYield).getChar()).c_str());
						CvWString szImprovement;
						szImprovement.Format(L"<link=literal>%s</link>", GC.getImprovementInfo(eLoopImprovement).getDescription());
						setListHelp(szHelpString, szFirstBuffer, szImprovement, L", ", (iValue != iLast));
						iLast = iValue;
					}
				}
			}

			for (int iJ = 0; iJ < kTrait.getNumSpecialistYieldChanges(); iJ++)
			{
				if (kTrait.getSpecialistYieldChangeYield(iJ) == eLoopYield)
				{
					SpecialistTypes eLoopSpecialist = (SpecialistTypes)kTrait.getSpecialistYieldChangeSpecialist(iJ);
					int iValue = kTrait.getSpecialistYieldChangeValue(iJ);
					szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_CIVIC_IMPROVEMENT_YIELD_CHANGE", iValue, GC.getYieldInfo(eLoopYield).getChar()).c_str());
					CvWString szSpecialist;
					szSpecialist.Format(L"<link=literal>%s</link>", GC.getSpecialistInfo(eLoopSpecialist).getDescription());
					setListHelp(szHelpString, szFirstBuffer, szSpecialist, L", ", (iValue != iLast));
					iLast = iValue;
				}
			}
		}

		// Specialist Commerce
		for (int iI = 0; iI < NUM_COMMERCE_TYPES; ++iI)
		{
			CommerceTypes eLoopCommerce = (CommerceTypes)iI;
			iLast = 0;

			for (int iJ = 0; iJ < kTrait.getNumSpecialistCommerceChanges(); iJ++)
			{
				if (kTrait.getSpecialistCommerceChangeCommerce(iJ) == eLoopCommerce)
				{
					SpecialistTypes eLoopSpecialist = (SpecialistTypes)kTrait.getSpecialistCommerceChangeSpecialist(iJ);
					int iValue = kTrait.getSpecialistCommerceChangeValue(iJ);
					szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_CIVIC_IMPROVEMENT_YIELD_CHANGE", iValue, GC.getCommerceInfo(eLoopCommerce).getChar()).c_str());
					CvWString szSpecialist;
					szSpecialist.Format(L"<link=literal>%s</link>", GC.getSpecialistInfo(eLoopSpecialist).getDescription());
					setListHelp(szHelpString, szFirstBuffer, szSpecialist, L", ", (iValue != iLast));
					iLast = iValue;
				}
			}
		}
		// MOD - END - Advanced Yield and Commerce Effects

		// MOD - START - Trait Coastal Trade Routes
		if (kTrait.getCoastalTradeRoutes() != 0)
		{
			szHelpString.append(gDLL->getText("TXT_KEY_TRAIT_COASTAL_TRADE_ROUTES", kTrait.getCoastalTradeRoutes()));
		}
		// MOD - END - Trait Coastal Trade Routes

		// MOD - START - Trait Trade Modifier
		if (kTrait.getTradeRouteIncomeModifier() != 0)
		{
			szHelpString.append(gDLL->getText("TXT_KEY_TRAIT_TRADE_ROUTE_MODIFIER", kTrait.getTradeRouteIncomeModifier()));
		}
		if (kTrait.getForeignTradeRouteIncomeModifier() != 0)
		{
			szHelpString.append(gDLL->getText("TXT_KEY_TRAIT_FOREIGN_TRADE_ROUTE_MODIFIER", kTrait.getForeignTradeRouteIncomeModifier()));
		}
		// MOD - END - Trait Trade Modifier

		// MOD - START - Trait UnitCombat Free Experience
		// Free UnitCombat Experience
		for (int iI = 0; iI < GC.getNumUnitCombatInfos(); iI++)
		{
			if (kTrait.getUnitCombatFreeExperience(iI) != 0)
			{
				szHelpString.append(gDLL->getText("TXT_KEY_TRAIT_FREE_XP", GC.getUnitCombatInfo((UnitCombatTypes)iI).getTextKeyWide(), kTrait.getUnitCombatFreeExperience(iI)));
			}
		}
		// MOD - END - Trait UnitCombat Free Experience

		// MOD - START - Trait UnitClass enabled (no prereq Civic)
		for (int iI = 0; iI < GC.getNumUnitClassInfos(); iI++)
		{
			if (kTrait.isNoPrereqCivicUnitClass(iI))
			{
				UnitTypes eLoopUnit;
				if (GC.getGameINLINE().getActivePlayer() != NO_PLAYER)
				{
					eLoopUnit = (UnitTypes)GC.getCivilizationInfo(GC.getGameINLINE().getActiveCivilizationType()).getCivilizationUnits((UnitClassTypes)iI);
				}
				else
				{
					eLoopUnit = (UnitTypes)GC.getUnitClassInfo((UnitClassTypes)iI).getRepresentativeUnit();
				}

				if (eLoopUnit != NO_UNIT)
				{
					CvWString szLinkedName = CvWString::format(L"<link=literal>%s</link>", GC.getUnitInfo(eLoopUnit).getDescription());
					szHelpString.append(gDLL->getText("TXT_KEY_TRAIT_UNITCLASS_WITHOUT_CIVIC", szLinkedName.GetCString()));
				}
			}
		}
		// MOD - END - Trait UnitClass enabled (no prereq Civic)

		// Free Promotions
		bool bFoundPromotion = false;
		szTempBuffer.clear();
		for (int iI = 0; iI < GC.getNumPromotionInfos(); ++iI)
		{
			if (kTrait.isFreePromotion(iI))
			{
				if (bFoundPromotion)
				{
					szTempBuffer += L", ";
				}

				szTempBuffer += CvWString::format(L"<link=literal>%s</link>", GC.getPromotionInfo((PromotionTypes) iI).getDescription());
				bFoundPromotion = true;
			}
		}

		if (bFoundPromotion)
		{
			szHelpString.append(gDLL->getText("TXT_KEY_TRAIT_FREE_PROMOTIONS", szTempBuffer.GetCString()));

			for (int iJ = 0; iJ < GC.getNumUnitCombatInfos(); iJ++)
			{
				if (kTrait.isFreePromotionUnitCombat(iJ))
				{
					szTempBuffer.Format(L"\n        %c<link=literal>%s</link>", gDLL->getSymbolID(BULLET_CHAR), GC.getUnitCombatInfo((UnitCombatTypes)iJ).getDescription());
					szHelpString.append(szTempBuffer);
				}
			}

			// MOD - START - Trait UnitClass Promotions
			for (int iJ = 0; iJ < GC.getNumUnitClassInfos(); iJ++)
			{
				if (kTrait.isFreePromotionUnitClass(iJ))
				{
					szTempBuffer.Format(L"\n        %c<link=literal>%s</link>", gDLL->getSymbolID(BULLET_CHAR), GC.getUnitClassInfo((UnitClassTypes)iJ).getDescription());
					szHelpString.append(szTempBuffer);
				}
			}
			// MOD - END - Trait UnitClass Promotions
		}

		// MOD - START - Congestion
		if (kTrait.getInsideCityCongestionThresholdChange() == kTrait.getOutsideCityCongestionThresholdChange())
		{
			if (kTrait.getInsideCityCongestionThresholdChange() != 0)
			{
				szHelpString.append(gDLL->getText("TXT_KEY_TRAIT_CONGESTION_THRESHOLD_CHANGE", kTrait.getInsideCityCongestionThresholdChange()));
			}
		}
		else
		{
			if (kTrait.getInsideCityCongestionThresholdChange() != 0)
			{
				szHelpString.append(gDLL->getText("TXT_KEY_TRAIT_CONGESTION_INSIDE_CITY_THRESHOLD_CHANGE", kTrait.getInsideCityCongestionThresholdChange()));
			}

			if (kTrait.getOutsideCityCongestionThresholdChange() != 0)
			{
				szHelpString.append(gDLL->getText("TXT_KEY_TRAIT_CONGESTION_OUTSIDE_CITY_THRESHOLD_CHANGE", kTrait.getOutsideCityCongestionThresholdChange()));
			}
		}
		// MOD - END - Congestion

		// No Civic Maintenance
		for (int iI = 0; iI < GC.getNumCivicOptionInfos(); ++iI)
		{
			if (GC.getCivicOptionInfo((CivicOptionTypes) iI).getTraitNoUpkeep(eTrait))
			{
				szHelpString.append(gDLL->getText("TXT_KEY_TRAIT_NO_UPKEEP", GC.getCivicOptionInfo((CivicOptionTypes)iI).getTextKeyWide()));
			}
		}

		// Increase Building/Unit Production Speeds
		iLast = 0;
		for (int iI = 0; iI < GC.getNumSpecialUnitInfos(); ++iI)
		{
			// MOD - START - Negative Production Modifiers
			const CvSpecialUnitInfo& kLoopSpecialUnit = GC.getSpecialUnitInfo((SpecialUnitTypes)iI);
			int iModifier = kLoopSpecialUnit.getProductionTraits(eTrait);
			if (iModifier != 0)
			{
				if (iModifier == 100)
				{
					szText = gDLL->getText("TXT_KEY_TRAIT_DOUBLE_SPEED");
				}
				else
				{
					szText = gDLL->getText(iModifier >= 0 ? "TXT_KEY_TRAIT_PRODUCTION_MODIFIER_FASTER" : "TXT_KEY_TRAIT_PRODUCTION_MODIFIER_SLOWER", abs(iModifier));
				}
				setListHelp(szHelpString, szText.GetCString(), kLoopSpecialUnit.getDescription(), L", ", (iModifier != iLast));
				iLast = iModifier;
			}
			// MOD - END - Negative Production Modifiers
		}

		// Unit Classes
		iLast = 0;
		for (int iI = 0; iI < GC.getNumUnitClassInfos();++iI)
		{
			if (eCivilization == NO_CIVILIZATION)
			{
				eLoopUnit = ((UnitTypes)(GC.getUnitClassInfo((UnitClassTypes)iI).getDefaultUnitIndex()));
			}
			else
			{
				eLoopUnit = ((UnitTypes)(GC.getCivilizationInfo(eCivilization).getCivilizationUnits(iI)));
			}

			if (eLoopUnit != NO_UNIT && !isWorldUnitClass((UnitClassTypes)iI))
			{
				// MOD - START - Negative Production Modifiers
				const CvUnitInfo& kLoopUnit = GC.getUnitInfo(eLoopUnit);
				int iModifier = kLoopUnit.getProductionTraits(eTrait);
				if (iModifier != 0)
				{
					if (iModifier == 100)
					{
						szText = gDLL->getText("TXT_KEY_TRAIT_DOUBLE_SPEED");
					}
					else
					{
						szText = gDLL->getText(iModifier >= 0 ? "TXT_KEY_TRAIT_PRODUCTION_MODIFIER_FASTER" : "TXT_KEY_TRAIT_PRODUCTION_MODIFIER_SLOWER", abs(iModifier));
					}
					CvWString szUnit;
					szUnit.Format(L"<link=%s>%s</link>", CvWString(kLoopUnit.getType()).GetCString(), kLoopUnit.getDescription());
					setListHelp(szHelpString, szText.GetCString(), szUnit, L", ", (iModifier != iLast));
					iLast = iModifier;
				}
				// MOD - END - Negative Production Modifiers
			}
		}

		// SpecialBuildings
		iLast = 0;
		for (int iI = 0; iI < GC.getNumSpecialBuildingInfos(); ++iI)
		{
			// MOD - START - Negative Production Modifiers
			const CvSpecialBuildingInfo& kLoopSpecialBuilding = GC.getSpecialBuildingInfo((SpecialBuildingTypes)iI);
			int iModifier = kLoopSpecialBuilding.getProductionTraits(eTrait);
			if (iModifier != 0)
			{
				if (iModifier == 100)
				{
					szText = gDLL->getText("TXT_KEY_TRAIT_DOUBLE_SPEED");
				}
				else
				{
					szText = gDLL->getText(iModifier >= 0 ? "TXT_KEY_TRAIT_PRODUCTION_MODIFIER_FASTER" : "TXT_KEY_TRAIT_PRODUCTION_MODIFIER_SLOWER", abs(iModifier));
				}
				setListHelp(szHelpString, szText.GetCString(), kLoopSpecialBuilding.getDescription(), L", ", (iModifier != iLast));
				iLast = kLoopSpecialBuilding.getProductionTraits(eTrait);
			}
			// MOD - END - Negative Production Modifiers
		}

		// Buildings
		iLast = 0;
		for (int iI = 0; iI < GC.getNumBuildingClassInfos(); ++iI)
		{
			if (eCivilization == NO_CIVILIZATION)
			{
				eLoopBuilding = ((BuildingTypes)(GC.getBuildingClassInfo((BuildingClassTypes)iI).getDefaultBuildingIndex()));
			}
			else
			{
				eLoopBuilding = ((BuildingTypes)(GC.getCivilizationInfo(eCivilization).getCivilizationBuildings(iI)));
			}

			if (eLoopBuilding != NO_BUILDING && !isWorldWonderClass((BuildingClassTypes)iI))
			{
				// MOD - START - Negative Production Modifiers
				const CvBuildingInfo& kLoopBuilding = GC.getBuildingInfo(eLoopBuilding);
				int iModifier = kLoopBuilding.getProductionTraits(eTrait);
				if (iModifier != 0)
				{
					if (iModifier == 100)
					{
						szText = gDLL->getText("TXT_KEY_TRAIT_DOUBLE_SPEED");
					}
					else
					{
						szText = gDLL->getText(iModifier >= 0 ? "TXT_KEY_TRAIT_PRODUCTION_MODIFIER_FASTER" : "TXT_KEY_TRAIT_PRODUCTION_MODIFIER_SLOWER", abs(iModifier));
					}
					CvWString szBuilding;
					szBuilding.Format(L"<link=%s>%s</link>", CvWString(kLoopBuilding.getType()).GetCString(), kLoopBuilding.getDescription());
					setListHelp(szHelpString, szText.GetCString(), szBuilding, L", ", (iModifier != iLast));
					iLast = iModifier;
				}
				// MOD - END - Negative Production Modifiers
			}
		}

		// Buildings
		iLast = 0;
		for (int iI = 0; iI < GC.getNumBuildingClassInfos(); ++iI)
		{
			if (eCivilization == NO_CIVILIZATION)
			{
				eLoopBuilding = ((BuildingTypes)(GC.getBuildingClassInfo((BuildingClassTypes)iI).getDefaultBuildingIndex()));
			}
			else
			{
				eLoopBuilding = ((BuildingTypes)(GC.getCivilizationInfo(eCivilization).getCivilizationBuildings(iI)));
			}

			if (eLoopBuilding != NO_BUILDING && !isWorldWonderClass((BuildingClassTypes)iI))
			{
				int iHappiness = GC.getBuildingInfo(eLoopBuilding).getHappinessTraits(eTrait);
				if (iHappiness != 0)
				{
					if (iHappiness > 0)
					{
						szText = gDLL->getText("TXT_KEY_TRAIT_BUILDING_HAPPINESS", iHappiness, gDLL->getSymbolID(HAPPY_CHAR));
					}
					else
					{
						szText = gDLL->getText("TXT_KEY_TRAIT_BUILDING_HAPPINESS", -iHappiness, gDLL->getSymbolID(UNHAPPY_CHAR));
					}

					CvWString szBuilding;
					szBuilding.Format(L"<link=literal>%s</link>", GC.getBuildingInfo(eLoopBuilding).getDescription());
					setListHelp(szHelpString, szText.GetCString(), szBuilding, L", ", (iHappiness != iLast));
					iLast = iHappiness;
				}
			}
		}

	}

//	return szHelpString;
}


//
// parseLeaderTraits - SimpleCivPicker							// LOCALIZATION READY
//
void CvGameTextMgr::parseLeaderTraits(CvWStringBuffer &szHelpString, LeaderHeadTypes eLeader, CivilizationTypes eCivilization, bool bDawnOfMan, bool bCivilopediaText)
{
	PROFILE_FUNC();

	CvWString szTempBuffer;	// Formatting

	//	Build help string
	if (eLeader != NO_LEADER)
	{
		// MOD - START - Improved Civilopedia
		if (eCivilization == NO_CIVILIZATION)
		{
			int iNumCivs = 0;
			CivilizationTypes eCandidateCivilization = NO_CIVILIZATION;

			for (int iI = 0; iI < GC.getNumCivilizationInfos(); ++iI)
			{
				if (GC.getCivilizationInfo((CivilizationTypes)iI).isLeaders(eLeader))
				{
					eCandidateCivilization = (CivilizationTypes)iI;
					iNumCivs++;
				}
			}

			if (iNumCivs == 1)
			{
				eCivilization = eCandidateCivilization;
			}
		}
		// MOD - END - Improved Civilopedia

		if (!bDawnOfMan && !bCivilopediaText)
		{
			szTempBuffer.Format( SETCOLR L"%s" ENDCOLR , TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), GC.getLeaderHeadInfo(eLeader).getDescription());
			szHelpString.append(szTempBuffer);
		}

		FAssertMsg(GC.getNumTraitInfos() > 0, "GC.getNumTraitInfos() is less than or equal to zero but is expected to be larger than zero in CvSimpleCivPicker::setLeaderText");

		bool bFirst = true;
		for (int iI = 0; iI < GC.getNumTraitInfos(); ++iI)
		{
			TraitTypes eTrait = (TraitTypes)iI;
			if (GC.getLeaderHeadInfo(eLeader).hasTrait(eTrait))
			{
				if (!bFirst)
				{
					if (bDawnOfMan)
					{
						szHelpString.append(L", ");
					}
				}
				else
				{
					bFirst = false;
				}

				// MOD - START - Improved Civilopedia
				if (!bDawnOfMan)
				{
					szHelpString.append(NEWLINE);
				}
				// MOD - END - Improved Civilopedia

				parseTraits(szHelpString, eTrait, eCivilization, bDawnOfMan);
			}
		}
	}
	else
	{
		//	Random leader
		szTempBuffer.Format( SETCOLR L"%s" ENDCOLR , TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), gDLL->getText("TXT_KEY_TRAIT_PLAYER_UNKNOWN").c_str());
		szHelpString.append(szTempBuffer);
	}

//	return szHelpString;
}

//
// parseLeaderTraits - SimpleCivPicker							// LOCALIZATION READY
//
void CvGameTextMgr::parseLeaderShortTraits(CvWStringBuffer &szHelpString, LeaderHeadTypes eLeader)
{
	PROFILE_FUNC();

	int iI;

	//	Build help string
	if (eLeader != NO_LEADER)
	{
		FAssert((GC.getNumTraitInfos() > 0) &&
			"GC.getNumTraitInfos() is less than or equal to zero but is expected to be larger than zero in CvSimpleCivPicker::setLeaderText");

		bool bFirst = true;
		for (iI = 0; iI < GC.getNumTraitInfos(); ++iI)
		{
			if (GC.getLeaderHeadInfo(eLeader).hasTrait(iI))
			{
				if (!bFirst)
				{
					szHelpString.append(L"/");
				}
				szHelpString.append(gDLL->getText(GC.getTraitInfo((TraitTypes)iI).getShortDescription()));
				bFirst = false;
			}
		}
	}
	else
	{
		//	Random leader
		szHelpString.append(CvWString("???/???"));
	}

	//	return szHelpString;
}

//
// Build Civilization Info Help Text
//
void CvGameTextMgr::parseCivInfos(CvWStringBuffer &szInfoText, CivilizationTypes eCivilization, bool bDawnOfMan, bool bLinks)
{
	PROFILE_FUNC();

	CvWString szBuffer;
	CvWString szTempString;
	CvWString szText;
	CvWString szList;
	UnitTypes eDefaultUnit;
	BuildingTypes eDefaultBuilding;

	if (eCivilization == NO_CIVILIZATION)
	{
		// This is a random civ, let us know here...
		szInfoText.append(gDLL->getText("TXT_KEY_CIV_UNKNOWN"));
		return;
	}

	const CvCivilizationInfo& kCivilization = GC.getCivilizationInfo(eCivilization);

	if (!bDawnOfMan)
	{
		// Civ Name
		szBuffer.Format(SETCOLR L"%s" ENDCOLR , TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), GC.getCivilizationInfo(eCivilization).getDescription());
		szInfoText.append(szBuffer);
		
		// Free Techs
		// MOD - START - No Starting Technologies
		szList.clear();

		bool bFound = false;
		for (int iI = 0; iI < GC.getNumTechInfos(); ++iI)
		{
			if (kCivilization.isCivilizationFreeTechs(iI))
			{
				bFound = true;
				// Add Tech
				szText.Format((bLinks ? L"<link=literal>%s</link>" : L"%s"), GC.getTechInfo((TechTypes)iI).getDescription());
				szList.Format(L"%s  %c%s", NEWLINE, gDLL->getSymbolID(BULLET_CHAR), szText.GetCString());
			}
		}

		if (bFound)
		{
			szBuffer.Format(NEWLINE SETCOLR L"%s" ENDCOLR , TEXT_COLOR("COLOR_ALT_HIGHLIGHT_TEXT"), gDLL->getText("TXT_KEY_FREE_TECHS").GetCString());
			szInfoText.append(szBuffer);
			szInfoText.append(szList);
		}

		/*
		if (!bFound)
		{
			szBuffer.Format(L"%s  %s", NEWLINE, gDLL->getText("TXT_KEY_FREE_TECHS_NO").GetCString());
			szInfoText.append(szBuffer);
		}
		*/
		// MOD - END - No Starting Technologies
	}

	// Free Units
	szText = gDLL->getText("TXT_KEY_FREE_UNITS");
	if (bDawnOfMan)
	{
		szTempString.Format(L"%s: ", szText.GetCString());
	}
	else
	{
		szTempString.Format(NEWLINE SETCOLR L"%s" ENDCOLR , TEXT_COLOR("COLOR_ALT_HIGHLIGHT_TEXT"), szText.GetCString());
	}
	szInfoText.append(szTempString);

	bool bFound = false;
	for (int iI = 0; iI < GC.getNumUnitClassInfos(); ++iI)
	{
		// MOD - START - Unique Units
		/*
		eDefaultUnit = ((UnitTypes)(GC.getCivilizationInfo(eCivilization).getCivilizationUnits(iI)));
		eUniqueUnit = ((UnitTypes)(GC.getUnitClassInfo((UnitClassTypes) iI).getDefaultUnitIndex()));
		if ((eDefaultUnit != NO_UNIT) && (eUniqueUnit != NO_UNIT))
		{
			if (eDefaultUnit != eUniqueUnit)
			{
				// Add Unit
				if (bDawnOfMan)
				{
					if (bFound)
					{
						szInfoText.append(L", ");
					}
					szBuffer.Format((bLinks ? L"<link=literal>%s</link> - (<link=literal>%s</link>)" : L"%s - (%s)"),
						GC.getUnitInfo(eDefaultUnit).getDescription(),
						GC.getUnitInfo(eUniqueUnit).getDescription());
				}
				else
				{
					szBuffer.Format(L"\n  %c%s - (%s)", gDLL->getSymbolID(BULLET_CHAR),
						GC.getUnitInfo(eDefaultUnit).getDescription(),
						GC.getUnitInfo(eUniqueUnit).getDescription());
				}
				szInfoText.append(szBuffer);
				bFound = true;
			}
		}
		*/

		UnitTypes eUniqueUnit = ((UnitTypes)(kCivilization.getCivilizationUnits(iI)));
		if (eUniqueUnit != NO_UNIT)
		{
			if (GC.getUnitInfo(eUniqueUnit).isUniqueUnit())
			{
				eDefaultUnit = ((UnitTypes)(GC.getUnitClassInfo((UnitClassTypes) iI).getDefaultUnitIndex()));

				// Add Unit
				if (bDawnOfMan)
				{
					if (bFound)
					{
						szInfoText.append(L", ");
					}
				}
				else
				{
					szBuffer.Format(L"\n  %c", gDLL->getSymbolID(BULLET_CHAR));
					szInfoText.append(szBuffer);
				}

				if ((eDefaultUnit != NO_UNIT) && (eDefaultUnit != eUniqueUnit))
				{
					szBuffer.Format((bLinks ? L"<link=literal>%s</link> - (<link=literal>%s</link>)" : L"%s - (%s)"),
						GC.getUnitInfo(eUniqueUnit).getDescription(),
						GC.getUnitInfo(eDefaultUnit).getDescription());
				}
				else
				{
					szBuffer.Format((bLinks ? L"<link=literal>%s</link>" : L"%s"), GC.getUnitInfo(eUniqueUnit).getDescription());
				}

				szInfoText.append(szBuffer);
				bFound = true;
			}
		}
		// MOD - END - Unique Units
	}

	if (!bFound)
	{
		szText = gDLL->getText("TXT_KEY_FREE_UNITS_NO");
		if (bDawnOfMan)
		{
			szTempString.Format(L"%s", szText.GetCString());
		}
		else
		{
			szTempString.Format(L"%s  %s", NEWLINE, szText.GetCString());
		}
		szInfoText.append(szTempString);
		bFound = true;
	}


	// Free Buildings
	szText = gDLL->getText("TXT_KEY_UNIQUE_BUILDINGS");
	if (bDawnOfMan)
	{
		if (bFound)
		{
			szInfoText.append(NEWLINE);
		}
		szTempString.Format(L"%s: ", szText.GetCString());
	}
	else
	{
		szTempString.Format(NEWLINE SETCOLR L"%s" ENDCOLR , TEXT_COLOR("COLOR_ALT_HIGHLIGHT_TEXT"), szText.GetCString());
	}
	szInfoText.append(szTempString);

	bFound = false;
	for (int iI = 0; iI < GC.getNumBuildingClassInfos(); ++iI)
	{
		// MOD - START - Unique Buildings
		/*
		eDefaultBuilding = ((BuildingTypes)(GC.getCivilizationInfo(eCivilization).getCivilizationBuildings(iI)));
		eUniqueBuilding = ((BuildingTypes)(GC.getBuildingClassInfo((BuildingClassTypes) iI).getDefaultBuildingIndex()));
		if ((eDefaultBuilding != NO_BUILDING) && (eUniqueBuilding != NO_BUILDING))
		{
			if (eDefaultBuilding != eUniqueBuilding)
			{
				// Add Building
				if (bDawnOfMan)
				{
					if (bFound)
					{
						szInfoText.append(L", ");
					}
					szBuffer.Format((bLinks ? L"<link=literal>%s</link> - (<link=literal>%s</link>)" : L"%s - (%s)"),
						GC.getBuildingInfo(eDefaultBuilding).getDescription(),
						GC.getBuildingInfo(eUniqueBuilding).getDescription());
				}
				else
				{
					szBuffer.Format(L"\n  %c%s - (%s)", gDLL->getSymbolID(BULLET_CHAR),
						GC.getBuildingInfo(eDefaultBuilding).getDescription(),
						GC.getBuildingInfo(eUniqueBuilding).getDescription());
				}
				szInfoText.append(szBuffer);
				bFound = true;
			}
		}
		*/

		BuildingTypes eUniqueBuilding = ((BuildingTypes)(kCivilization.getCivilizationBuildings(iI)));
		if (eUniqueBuilding != NO_UNIT)
		{
			if (GC.getBuildingInfo(eUniqueBuilding).isUniqueBuilding())
			{
				eDefaultBuilding = ((BuildingTypes)(GC.getBuildingClassInfo((BuildingClassTypes) iI).getDefaultBuildingIndex()));

				// Add Building
				if (bDawnOfMan)
				{
					if (bFound)
					{
						szInfoText.append(L", ");
					}
				}
				else
				{
					szBuffer.Format(L"\n  %c", gDLL->getSymbolID(BULLET_CHAR));
					szInfoText.append(szBuffer);
				}

				if ((eDefaultBuilding != NO_UNIT) && (eDefaultBuilding != eUniqueBuilding))
				{
					szBuffer.Format((bLinks ? L"<link=literal>%s</link> - (<link=literal>%s</link>)" : L"%s - (%s)"),
						GC.getBuildingInfo(eUniqueBuilding).getDescription(),
						GC.getBuildingInfo(eDefaultBuilding).getDescription());
				}
				else
				{
					szBuffer.Format((bLinks ? L"<link=literal>%s</link>" : L"%s"), GC.getBuildingInfo(eUniqueBuilding).getDescription());
				}

				szInfoText.append(szBuffer);
				bFound = true;
			}
		}
		// MOD - END - Unique Buildings
	}
	if (!bFound)
	{
		szText = gDLL->getText("TXT_KEY_UNIQUE_BUILDINGS_NO");
		if (bDawnOfMan)
		{
			szTempString.Format(L"%s", szText.GetCString());
		}
		else
		{
			szTempString.Format(L"%s  %s", NEWLINE, szText.GetCString());
		}
		szInfoText.append(szTempString);
	}

	// MOD - START - Unique Improvements
	szText = gDLL->getText("TXT_KEY_UNIQUE_IMPROVEMENTS");
	if (bDawnOfMan)
	{
		if (bFound)
		{
			szInfoText.append(NEWLINE);
		}
		szTempString.Format(L"%s: ", szText.GetCString());
	}
	else
	{
		szTempString.Format(NEWLINE SETCOLR L"%s" ENDCOLR , TEXT_COLOR("COLOR_ALT_HIGHLIGHT_TEXT"), szText.GetCString());
	}
	szInfoText.append(szTempString);

	bFound = false;
	for (ImprovementTypes eLoopImprovement = (ImprovementTypes)0; eLoopImprovement < GC.getNumImprovementInfos(); eLoopImprovement = (ImprovementTypes)(eLoopImprovement + 1))
	{
		if (GC.getImprovementInfo(eLoopImprovement).isUniqueImprovement())
		{
			if (kCivilization.canProduceImprovement(eLoopImprovement))
			{
				// Add Building
				if (bDawnOfMan)
				{
					if (bFound)
					{
						szInfoText.append(L", ");
					}
				}
				else
				{
					szBuffer.Format(L"\n  %c", gDLL->getSymbolID(BULLET_CHAR));
					szInfoText.append(szBuffer);
				}

				szBuffer.Format((bLinks ? L"<link=literal>%s</link>" : L"%s"), GC.getImprovementInfo(eLoopImprovement).getDescription());

				szInfoText.append(szBuffer);
				bFound = true;
			}
		}
	}
	if (!bFound)
	{
		szText = gDLL->getText("TXT_KEY_UNIQUE_IMPROVEMENTS_NO");
		if (bDawnOfMan)
		{
			szTempString.Format(L"%s", szText.GetCString());
		}
		else
		{
			szTempString.Format(L"%s  %s", NEWLINE, szText.GetCString());
		}
		szInfoText.append(szTempString);
	}
	// MOD - END - Unique Improvements

//	return szInfoText;
}


void CvGameTextMgr::parseSpecialistHelp(CvWStringBuffer &szHelpString, SpecialistTypes eSpecialist, CvCity* pCity, bool bCivilopediaText)
{
	PROFILE_FUNC();

	CvWString szText;
	int aiYields[NUM_YIELD_TYPES];
	int aiCommerces[NUM_COMMERCE_TYPES];
	int iI;

	const CvSpecialistInfo& kSpecialist = GC.getSpecialistInfo(eSpecialist); // K-Mod

	if (eSpecialist != NO_SPECIALIST)
	{
		if (!bCivilopediaText)
		{
			szHelpString.append(kSpecialist.getDescription());
		}

		for (iI = 0; iI < NUM_YIELD_TYPES; ++iI)
		{
			if (GC.getGameINLINE().getActivePlayer() == NO_PLAYER)
			{
				aiYields[iI] = kSpecialist.getYieldChange(iI);
			}
			else
			{
				// MOD - START - Advanced Yield and Commerce Effects
				//aiYields[iI] = GET_PLAYER((pCity != NULL) ? pCity->getOwnerINLINE() : GC.getGameINLINE().getActivePlayer()).specialistYield(eSpecialist, ((YieldTypes)iI));
				if (pCity != NULL)
				{
					aiYields[iI] = pCity->getYieldPerSpecialist(eSpecialist, (YieldTypes)iI);
				}
				else
				{
					aiYields[iI] = GET_PLAYER(GC.getGameINLINE().getActivePlayer()).specialistYield(eSpecialist, (YieldTypes)iI);
				}
				// MOD - END - Advanced Yield and Commerce Effects
			}
		}

		setYieldChangeHelp(szHelpString, L"", L"", L"", aiYields);

		for (iI = 0; iI < NUM_COMMERCE_TYPES; ++iI)
		{
			if (GC.getGameINLINE().getActivePlayer() == NO_PLAYER)
			{
				aiCommerces[iI] = kSpecialist.getCommerceChange(iI);
			}
			else
			{
				// MOD - START - Advanced Yield and Commerce Effects
				//aiCommerces[iI] = GET_PLAYER((pCity != NULL) ? pCity->getOwnerINLINE() : GC.getGameINLINE().getActivePlayer()).specialistCommerce(((SpecialistTypes)eSpecialist), ((CommerceTypes)iI));
				if (pCity != NULL)
				{
					aiCommerces[iI] = pCity->getCommercePerSpecialist(eSpecialist, (CommerceTypes)iI);
				}
				else
				{
					aiCommerces[iI] = GET_PLAYER(GC.getGameINLINE().getActivePlayer()).specialistCommerce(eSpecialist, (CommerceTypes)iI);
				}
				// MOD - END - Advanced Yield and Commerce Effects
			}
		}

		setCommerceChangeHelp(szHelpString, L"", L"", L"", aiCommerces);

		// MOD - START - Advanced Yield and Commerce Effects
		// This is an older style that puts each item on its own line
		/*
		if (GC.getGameINLINE().getActivePlayer() != NO_PLAYER)
		{
			const CvCivilizationInfo& kCivilization = GC.getCivilizationInfo(GC.getGameINLINE().getActiveCivilizationType());

			for (int iRel = 0; iRel < kSpecialist.getNumRelatedBuildingClasses(); iRel++)
			{
				BuildingClassTypes eLoopBuildingClass = (BuildingClassTypes)kSpecialist.getRelatedBuildingClasses(iRel);
				BuildingTypes eLoopBuilding = (BuildingTypes)kCivilization.getCivilizationBuildings(eLoopBuildingClass);

				if (eLoopBuilding != NO_BUILDING)
				{
					const CvBuildingInfo& kLoopBuilding = GC.getBuildingInfo(eLoopBuilding);

					bool bStarted = false;

					int aiYieldChanges[NUM_YIELD_TYPES] = {};
					kLoopBuilding.getSpecialistYieldChangeArray(eSpecialist, aiYieldChanges);
					bStarted = setResumableYieldChangeHelp(szHelpString, L"", L"", L"", aiYieldChanges, false, true, bStarted);

					int aiCommerceChanges[NUM_COMMERCE_TYPES] = {};
					kLoopBuilding.getSpecialistCommerceChangeArray(eSpecialist, aiCommerceChanges);
					bStarted = setResumableCommerceChangeHelp(szHelpString, L"", L"", L"", aiCommerceChanges, false, true, bStarted);

					if (bStarted)
					{
						szHelpString.append(gDLL->getText("TXT_KEY_FROM_BUILDING", kLoopBuilding.getTextKeyWide()));
					}

					bStarted = false;

					int aiGlobalYieldChanges[NUM_YIELD_TYPES] = {};
					kLoopBuilding.getGlobalSpecialistYieldChangeArray(eSpecialist, aiGlobalYieldChanges);
					bStarted = setResumableYieldChangeHelp(szHelpString, L"", L"", L"", aiGlobalYieldChanges, false, true, bStarted);

					int aiGlobalCommerceChanges[NUM_COMMERCE_TYPES] = {};
					kLoopBuilding.getGlobalSpecialistCommerceChangeArray(eSpecialist, aiGlobalCommerceChanges);
					bStarted = setResumableCommerceChangeHelp(szHelpString, L"", L"", L"", aiGlobalCommerceChanges, false, true, bStarted);

					if (bStarted)
					{
						szHelpString.append(gDLL->getText("TXT_KEY_FROM_BUILDING_ALL_CITIES", kLoopBuilding.getTextKeyWide()));
					}
				}
			}
		}
		else
		{
			for (int iRel = 0; iRel < kSpecialist.getNumRelatedBuildingClasses(); iRel++)
			{
				BuildingClassTypes eLoopBuildingClass = (BuildingClassTypes)kSpecialist.getRelatedBuildingClasses(iRel);
				const CvBuildingClassInfo& kLoopBuildingClass = GC.getBuildingClassInfo(eLoopBuildingClass);

				for (int iMem = 0; iMem < kLoopBuildingClass.getNumBuildingTypes(); iMem++)
				{
					BuildingTypes eLoopBuilding = (BuildingTypes)kLoopBuildingClass.getBuildingTypes(iMem);
					const CvBuildingInfo& kLoopBuilding = GC.getBuildingInfo(eLoopBuilding);

					bool bStarted = false;

					int aiYieldChanges[NUM_YIELD_TYPES] = {};
					kLoopBuilding.getSpecialistYieldChangeArray(eSpecialist, aiYieldChanges);
					bStarted = setResumableYieldChangeHelp(szHelpString, L"", L"", L"", aiYieldChanges, false, true, bStarted);

					int aiCommerceChanges[NUM_COMMERCE_TYPES] = {};
					kLoopBuilding.getSpecialistCommerceChangeArray(eSpecialist, aiCommerceChanges);
					bStarted = setResumableCommerceChangeHelp(szHelpString, L"", L"", L"", aiCommerceChanges, false, true, bStarted);

					if (bStarted)
					{
						szHelpString.append(gDLL->getText("TXT_KEY_FROM_BUILDING", kLoopBuilding.getTextKeyWide()));
					}

					bStarted = false;

					int aiGlobalYieldChanges[NUM_YIELD_TYPES] = {};
					kLoopBuilding.getGlobalSpecialistYieldChangeArray(eSpecialist, aiGlobalYieldChanges);
					bStarted = setResumableYieldChangeHelp(szHelpString, L"", L"", L"", aiGlobalYieldChanges, false, true, bStarted);

					int aiGlobalCommerceChanges[NUM_COMMERCE_TYPES] = {};
					kLoopBuilding.getGlobalSpecialistCommerceChangeArray(eSpecialist, aiGlobalCommerceChanges);
					bStarted = setResumableCommerceChangeHelp(szHelpString, L"", L"", L"", aiGlobalCommerceChanges, false, true, bStarted);

					if (bStarted)
					{
						szHelpString.append(gDLL->getText("TXT_KEY_FROM_BUILDING_ALL_CITIES", kLoopBuilding.getTextKeyWide()));
					}
				}
			}
		}
		*/

		CvWString szFirstBuffer;
		for (int iI = 0; iI < NUM_YIELD_TYPES; ++iI)
		{
			YieldTypes eLoopYield = (YieldTypes)iI;
			int iLast = 0;

			// Trait Yield Changes
			for (int iRel = 0; iRel < kSpecialist.getNumRelatedTraits(); iRel++)
			{
				TraitTypes eLoopTrait = (TraitTypes)kSpecialist.getRelatedTraits(iRel);
				const CvTraitInfo& kLoopTrait = GC.getTraitInfo(eLoopTrait);
				int iChange = kLoopTrait.getSpecialistYieldChanges(eSpecialist, eLoopYield);
				if (iChange != 0)
				{
					szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_YIELD_CHANGE_FROM", iChange, GC.getYieldInfo(eLoopYield).getChar()).c_str());
					CvWString szTrait = gDLL->getText("TXT_KEY_MISC_TRAIT_EFFECT", kLoopTrait.getTextKeyWide());
					setListHelp(szHelpString, szFirstBuffer, szTrait, L", ", (iChange != iLast));
					iLast = iChange;
				}
			}

			// Civic Yield Changes
			for (int iRel = 0; iRel < kSpecialist.getNumRelatedCivics(); iRel++)
			{
				CivicTypes eLoopCivic = (CivicTypes)kSpecialist.getRelatedCivics(iRel);
				const CvCivicInfo& kLoopCivic = GC.getCivicInfo(eLoopCivic);
				int iChange = kLoopCivic.getSpecialistYieldChanges(eSpecialist, eLoopYield);
				if (iChange != 0)
				{
					szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_YIELD_CHANGE_FROM", iChange, GC.getYieldInfo(eLoopYield).getChar()).c_str());
					CvWString szCivic = gDLL->getText("TXT_KEY_MISC_CIVIC_EFFECT", kLoopCivic.getTextKeyWide());
					setListHelp(szHelpString, szFirstBuffer, szCivic, L", ", (iChange != iLast));
					iLast = iChange;
				}
			}

			// Building Yield Changes
			// MOD - START - Relation Caching
			if (GC.getGameINLINE().getActivePlayer() != NO_PLAYER)
			{
				const CvCivilizationInfo& kCivilization = GC.getCivilizationInfo(GC.getGameINLINE().getActiveCivilizationType());

				for (int iRel = 0; iRel < kSpecialist.getNumRelatedBuildingClasses(); iRel++)
				{
					BuildingClassTypes eLoopBuildingClass = (BuildingClassTypes)kSpecialist.getRelatedBuildingClasses(iRel);
					BuildingTypes eLoopBuilding = (BuildingTypes)kCivilization.getCivilizationBuildings(eLoopBuildingClass);

					if (eLoopBuilding != NO_BUILDING)
					{
						const CvBuildingInfo& kLoopBuilding = GC.getBuildingInfo(eLoopBuilding);

						int aiYieldChanges[NUM_YIELD_TYPES] = {};
						kLoopBuilding.getSpecialistYieldChangeArray(eSpecialist, aiYieldChanges);
						int iChange = aiYieldChanges[eLoopYield];
						if (iChange != 0)
						{
							szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_YIELD_CHANGE_FROM", iChange, GC.getYieldInfo(eLoopYield).getChar()).c_str());
							CvWString szBuilding = gDLL->getText("TXT_KEY_MISC_BUILDING_EFFECT", kLoopBuilding.getTextKeyWide());
							setListHelp(szHelpString, szFirstBuffer, szBuilding, L", ", (iChange != iLast));
							iLast = iChange;
						}

						int aiGlobalYieldChanges[NUM_YIELD_TYPES] = {};
						kLoopBuilding.getGlobalSpecialistYieldChangeArray(eSpecialist, aiGlobalYieldChanges);
						iChange = aiGlobalYieldChanges[eLoopYield];
						if (iChange != 0)
						{
							szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_YIELD_CHANGE_FROM", iChange, GC.getYieldInfo(eLoopYield).getChar()).c_str());
							CvWString szBuilding = gDLL->getText("TXT_KEY_MISC_BUILDING_EFFECT_ALL_CITIES", kLoopBuilding.getTextKeyWide());
							setListHelp(szHelpString, szFirstBuffer, szBuilding, L", ", (iChange != iLast));
							iLast = iChange;
						}
					}
				}
			}
			else
			{
				for (int iRel = 0; iRel < kSpecialist.getNumRelatedBuildingClasses(); iRel++)
				{
					BuildingClassTypes eLoopBuildingClass = (BuildingClassTypes)kSpecialist.getRelatedBuildingClasses(iRel);
					const CvBuildingClassInfo& kLoopBuildingClass = GC.getBuildingClassInfo(eLoopBuildingClass);

					for (int iMem = 0; iMem < kLoopBuildingClass.getNumBuildingTypes(); iMem++)
					{
						BuildingTypes eLoopBuilding = (BuildingTypes)kLoopBuildingClass.getBuildingTypes(iMem);
						const CvBuildingInfo& kLoopBuilding = GC.getBuildingInfo(eLoopBuilding);

						int aiYieldChanges[NUM_YIELD_TYPES] = {};
						kLoopBuilding.getSpecialistYieldChangeArray(eSpecialist, aiYieldChanges);
						int iChange = aiYieldChanges[eLoopYield];
						if (iChange != 0)
						{
							szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_YIELD_CHANGE_FROM", iChange, GC.getYieldInfo(eLoopYield).getChar()).c_str());
							CvWString szBuilding = gDLL->getText("TXT_KEY_MISC_BUILDING_EFFECT", kLoopBuilding.getTextKeyWide());
							setListHelp(szHelpString, szFirstBuffer, szBuilding, L", ", (iChange != iLast));
							iLast = iChange;
						}

						int aiGlobalYieldChanges[NUM_YIELD_TYPES] = {};
						kLoopBuilding.getGlobalSpecialistYieldChangeArray(eSpecialist, aiGlobalYieldChanges);
						iChange = aiGlobalYieldChanges[eLoopYield];
						if (iChange != 0)
						{
							szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_CIVIC_IMPROVEMENT_YIELD_CHANGE", iChange, GC.getYieldInfo(eLoopYield).getChar()).c_str());
							CvWString szBuilding = gDLL->getText("TXT_KEY_MISC_BUILDING_EFFECT_ALL_CITIES", kLoopBuilding.getTextKeyWide());
							setListHelp(szHelpString, szFirstBuffer, szBuilding, L", ", (iChange != iLast));
							iLast = iChange;
						}
					}
				}
			}
			// MOD - END - Relation Caching
		}

		for (int iI = 0; iI < NUM_COMMERCE_TYPES; ++iI)
		{
			CommerceTypes eLoopCommerce = (CommerceTypes)iI;
			int iLast = 0;

			// Trait Commerce Changes
			for (int iRel = 0; iRel < kSpecialist.getNumRelatedTraits(); iRel++)
			{
				TraitTypes eLoopTrait = (TraitTypes)kSpecialist.getRelatedTraits(iRel);
				const CvTraitInfo& kLoopTrait = GC.getTraitInfo(eLoopTrait);
				int iChange = kLoopTrait.getSpecialistCommerceChanges(eSpecialist, eLoopCommerce);
				if (iChange != 0)
				{
					szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_COMMERCE_CHANGE_FROM", iChange, GC.getCommerceInfo(eLoopCommerce).getChar()).c_str());
					CvWString szTrait = gDLL->getText("TXT_KEY_MISC_TRAIT_EFFECT", kLoopTrait.getTextKeyWide());
					setListHelp(szHelpString, szFirstBuffer, szTrait, L", ", (iChange != iLast));
					iLast = iChange;
				}
			}

			// Civic Commerce Changes
			for (int iRel = 0; iRel < kSpecialist.getNumRelatedCivics(); iRel++)
			{
				CivicTypes eLoopCivic = (CivicTypes)kSpecialist.getRelatedCivics(iRel);
				const CvCivicInfo& kLoopCivic = GC.getCivicInfo(eLoopCivic);
				int iChange = kLoopCivic.getSpecialistCommerceChanges(eSpecialist, eLoopCommerce);
				if (iChange != 0)
				{
					szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_COMMERCE_CHANGE_FROM", iChange, GC.getCommerceInfo(eLoopCommerce).getChar()).c_str());
					CvWString szCivic = gDLL->getText("TXT_KEY_MISC_CIVIC_EFFECT", kLoopCivic.getTextKeyWide());
					setListHelp(szHelpString, szFirstBuffer, szCivic, L", ", (iChange != iLast));
					iLast = iChange;
				}
			}

			// Building Commerce Changes
			if (GC.getGameINLINE().getActivePlayer() != NO_PLAYER)
			{
				const CvCivilizationInfo& kCivilization = GC.getCivilizationInfo(GC.getGameINLINE().getActiveCivilizationType());

				for (int iRel = 0; iRel < kSpecialist.getNumRelatedBuildingClasses(); iRel++)
				{
					BuildingClassTypes eLoopBuildingClass = (BuildingClassTypes)kSpecialist.getRelatedBuildingClasses(iRel);
					BuildingTypes eLoopBuilding = (BuildingTypes)kCivilization.getCivilizationBuildings(eLoopBuildingClass);

					if (eLoopBuilding != NO_BUILDING)
					{
						const CvBuildingInfo& kLoopBuilding = GC.getBuildingInfo(eLoopBuilding);

						int aiCommerceChanges[NUM_COMMERCE_TYPES] = {};
						kLoopBuilding.getSpecialistCommerceChangeArray(eSpecialist, aiCommerceChanges);
						int iChange = aiCommerceChanges[eLoopCommerce];
						if (iChange != 0)
						{
							szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_COMMERCE_CHANGE_FROM", iChange, GC.getCommerceInfo(eLoopCommerce).getChar()).c_str());
							CvWString szBuilding = gDLL->getText("TXT_KEY_MISC_BUILDING_EFFECT", kLoopBuilding.getTextKeyWide());
							setListHelp(szHelpString, szFirstBuffer, szBuilding, L", ", (iChange != iLast));
							iLast = iChange;
						}

						int aiGlobalCommerceChanges[NUM_COMMERCE_TYPES] = {};
						kLoopBuilding.getGlobalSpecialistCommerceChangeArray(eSpecialist, aiGlobalCommerceChanges);
						iChange = aiGlobalCommerceChanges[eLoopCommerce];
						if (iChange != 0)
						{
							szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_COMMERCE_CHANGE_FROM", iChange, GC.getCommerceInfo(eLoopCommerce).getChar()).c_str());
							CvWString szBuilding = gDLL->getText("TXT_KEY_MISC_BUILDING_EFFECT_ALL_CITIES", kLoopBuilding.getTextKeyWide());
							setListHelp(szHelpString, szFirstBuffer, szBuilding, L", ", (iChange != iLast));
							iLast = iChange;
						}
					}
				}
			}
			else
			{
				for (int iRel = 0; iRel < kSpecialist.getNumRelatedBuildingClasses(); iRel++)
				{
					BuildingClassTypes eLoopBuildingClass = (BuildingClassTypes)kSpecialist.getRelatedBuildingClasses(iRel);
					const CvBuildingClassInfo& kLoopBuildingClass = GC.getBuildingClassInfo(eLoopBuildingClass);

					for (int iMem = 0; iMem < kLoopBuildingClass.getNumBuildingTypes(); iMem++)
					{
						BuildingTypes eLoopBuilding = (BuildingTypes)kLoopBuildingClass.getBuildingTypes(iMem);
						const CvBuildingInfo& kLoopBuilding = GC.getBuildingInfo(eLoopBuilding);

						int aiCommerceChanges[NUM_COMMERCE_TYPES] = {};
						kLoopBuilding.getSpecialistCommerceChangeArray(eSpecialist, aiCommerceChanges);
						int iChange = aiCommerceChanges[eLoopCommerce];
						if (iChange != 0)
						{
							szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_COMMERCE_CHANGE_FROM", iChange, GC.getCommerceInfo(eLoopCommerce).getChar()).c_str());
							CvWString szBuilding = gDLL->getText("TXT_KEY_MISC_BUILDING_EFFECT", kLoopBuilding.getTextKeyWide());
							setListHelp(szHelpString, szFirstBuffer, szBuilding, L", ", (iChange != iLast));
							iLast = iChange;
						}

						int aiGlobalCommerceChanges[NUM_COMMERCE_TYPES] = {};
						kLoopBuilding.getGlobalSpecialistCommerceChangeArray(eSpecialist, aiGlobalCommerceChanges);
						iChange = aiGlobalCommerceChanges[eLoopCommerce];
						if (iChange != 0)
						{
							szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_COMMERCE_CHANGE_FROM", iChange, GC.getCommerceInfo(eLoopCommerce).getChar()).c_str());
							CvWString szBuilding = gDLL->getText("TXT_KEY_MISC_BUILDING_EFFECT_ALL_CITIES", kLoopBuilding.getTextKeyWide());
							setListHelp(szHelpString, szFirstBuffer, szBuilding, L", ", (iChange != iLast));
							iLast = iChange;
						}
					}
				}
			}
			// MOD - END - Relation Caching
		}
		// MOD - END - Advanced Yield and Commerce Effects

		if (kSpecialist.getExperience() > 0)
		{
			szHelpString.append(NEWLINE);
			szHelpString.append(gDLL->getText("TXT_KEY_SPECIALIST_EXPERIENCE", kSpecialist.getExperience()));
		}

		if (kSpecialist.getGreatPeopleRateChange() != 0)
		{
			szHelpString.append(NEWLINE);
			szHelpString.append(gDLL->getText("TXT_KEY_SPECIALIST_BIRTH_RATE", kSpecialist.getGreatPeopleRateChange()));

			// K-Mod
			if (!bCivilopediaText && gDLL->getChtLvl() > 0 && GC.ctrlKey())
			{
				szHelpString.append(NEWLINE);
				szHelpString.append(CvWString::format(L"weight: %d", GET_PLAYER((pCity != NULL) ? pCity->getOwnerINLINE() : GC.getGameINLINE().getActivePlayer()).AI_getGreatPersonWeight((UnitClassTypes)kSpecialist.getGreatPeopleUnitClass())));
			}
			// K-Mod end
		}

// BUG - Specialist Actual Effects - start
		if (pCity && (GC.altKey() || getBugOptionBOOL("MiscHover__SpecialistActualEffects", false, "BUG_MISC_SPECIALIST_HOVER_ACTUAL_EFFECTS")) && (pCity->getOwnerINLINE() == GC.getGame().getActivePlayer() || gDLL->getChtLvl() > 0))
		{
			bool bStarted = false;
			CvWString szStart;
            szStart.Format(L"\n" SETCOLR L"(%s", TEXT_COLOR("COLOR_LIGHT_GREY"), gDLL->getText("TXT_KEY_ACTUAL_EFFECTS").GetCString());

			// Yield
			int aiYields[NUM_YIELD_TYPES];
			for (int iI = 0; iI < NUM_YIELD_TYPES; ++iI)
			{
				aiYields[iI] = pCity->getAdditionalYieldBySpecialist((YieldTypes)iI, eSpecialist);
			}
			bStarted = setResumableYieldChangeHelp(szHelpString, szStart, L": ", L"", aiYields, false, false, bStarted);

			// Commerce
			int aiCommerces[NUM_COMMERCE_TYPES];
			for (int iI = 0; iI < NUM_COMMERCE_TYPES; ++iI)
			{
				aiCommerces[iI] = pCity->getAdditionalCommerceTimes100BySpecialist((CommerceTypes)iI, eSpecialist);
			}
			bStarted = setResumableCommerceTimes100ChangeHelp(szHelpString, szStart, L": ", L"", aiCommerces, false, bStarted);

			// Great People
			int iGreatPeopleRate = pCity->getAdditionalGreatPeopleRateBySpecialist(eSpecialist);
			bStarted = setResumableValueChangeHelp(szHelpString, szStart, L": ", L"", iGreatPeopleRate, gDLL->getSymbolID(GREAT_PEOPLE_CHAR), false, false, bStarted);

			if (bStarted)
				szHelpString.append(L")" ENDCOLR);
		}
// BUG - Specialist Actual Effects - end

		if (!CvWString(kSpecialist.getHelp()).empty() && !bCivilopediaText)
		{
			szHelpString.append(NEWLINE);
			szHelpString.append(kSpecialist.getHelp());
		}
	}
}

void CvGameTextMgr::parseFreeSpecialistHelp(CvWStringBuffer &szHelpString, const CvCity& kCity)
{
	PROFILE_FUNC();

	for (int iLoopSpecialist = 0; iLoopSpecialist < GC.getNumSpecialistInfos(); iLoopSpecialist++)
	{
		SpecialistTypes eSpecialist = (SpecialistTypes)iLoopSpecialist;
		int iNumSpecialists = kCity.getFreeSpecialistCount(eSpecialist);

		if (iNumSpecialists > 0)
		{
			int aiYields[NUM_YIELD_TYPES];
			int aiCommerces[NUM_COMMERCE_TYPES];

			szHelpString.append(NEWLINE);
			szHelpString.append(CvWString::format(L"%s (%d): ", GC.getSpecialistInfo(eSpecialist).getDescription(), iNumSpecialists));

			for (int iI = 0; iI < NUM_YIELD_TYPES; ++iI)
			{
				// MOD - START - Advanced Yield and Commerce Effects
				//aiYields[iI] = iNumSpecialists * GET_PLAYER(kCity.getOwnerINLINE()).specialistYield(eSpecialist, ((YieldTypes)iI));
				aiYields[iI] = iNumSpecialists * kCity.getYieldPerSpecialist(eSpecialist, ((YieldTypes)iI));
				// MOD - END - Advanced Yield and Commerce Effects
			}

			CvWStringBuffer szYield;
			setYieldChangeHelp(szYield, L"", L"", L"", aiYields, false, false);
			szHelpString.append(szYield);

			for (int iI = 0; iI < NUM_COMMERCE_TYPES; ++iI)
			{
				// MOD - START - Advanced Yield and Commerce Effects
				//aiCommerces[iI] = iNumSpecialists * GET_PLAYER(kCity.getOwnerINLINE()).specialistCommerce(eSpecialist, ((CommerceTypes)iI));
				aiCommerces[iI] = iNumSpecialists * kCity.getCommercePerSpecialist(eSpecialist, (CommerceTypes)iI);
				// MOD - END - Advanced Yield and Commerce Effects
			}

			CvWStringBuffer szCommerceString;
			setCommerceChangeHelp(szCommerceString, L"", L"", L"", aiCommerces, false, false);
			if (!szYield.isEmpty() && !szCommerceString.isEmpty())
			{
				szHelpString.append(L", ");
			}
			szHelpString.append(szCommerceString);

			if (GC.getSpecialistInfo(eSpecialist).getExperience() > 0)
			{
				if (!szYield.isEmpty() || !szCommerceString.isEmpty())
				{
					szHelpString.append(L", ");
				}
				szHelpString.append(gDLL->getText("TXT_KEY_SPECIALIST_EXPERIENCE_SHORT", iNumSpecialists * GC.getSpecialistInfo(eSpecialist).getExperience()));
			}
		}
	}
}


//
// Promotion Help
//
void CvGameTextMgr::parsePromotionHelp(CvWStringBuffer &szBuffer, PromotionTypes ePromotion, const wchar* pcNewline)
{
	PROFILE_FUNC();

	CvWString szText, szText2;
	int iI;

	if (NO_PROMOTION == ePromotion)
	{
		return;
	}

	const CvPromotionInfo& kPromotion = GC.getPromotionInfo(ePromotion);

	if (kPromotion.isBlitz())
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_BLITZ_TEXT"));
	}

	if (kPromotion.isAmphib())
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_AMPHIB_TEXT"));
	}

	if (kPromotion.isRiver())
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_RIVER_ATTACK_TEXT"));
	}

	if (kPromotion.isEnemyRoute())
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_ENEMY_ROADS_TEXT"));
	}

	if (kPromotion.isAlwaysHeal())
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_ALWAYS_HEAL_TEXT"));
	}

	if (kPromotion.isHillsDoubleMove())
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_HILLS_MOVE_TEXT"));
	}

	if (kPromotion.isImmuneToFirstStrikes())
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_IMMUNE_FIRST_STRIKES_TEXT"));
	}

	for (iI = 0; iI < GC.getNumTerrainInfos(); ++iI)
	{
		if (kPromotion.getTerrainDoubleMove(iI))
		{
			szBuffer.append(pcNewline);
			szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_DOUBLE_MOVE_TEXT", GC.getTerrainInfo((TerrainTypes) iI).getTextKeyWide()));
		}
	}

	for (iI = 0; iI < GC.getNumFeatureInfos(); ++iI)
	{
		if (kPromotion.getFeatureDoubleMove(iI))
		{
			szBuffer.append(pcNewline);
			szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_DOUBLE_MOVE_TEXT", GC.getFeatureInfo((FeatureTypes) iI).getTextKeyWide()));
		}
	}

	// MOD - START - Feature Damage Immunity
	CvWString szTempBuffer;
	szTempBuffer.Format(L"%s%s ", NEWLINE, gDLL->getText("TXT_KEY_MISC_FEATURE_DAMAGE_IMMUNITY").GetCString());

	bool bFirst = true;
	for (iI = 0; iI < GC.getNumFeatureInfos(); ++iI)
	{
		if (kPromotion.getFeatureDamageImmunity(iI))
		{
			const CvFeatureInfo& kLoopFeature = GC.getFeatureInfo((FeatureTypes)iI);

			CvWString szFeature;
			szFeature.Format(L"<link=%s>%s</link>", CvWString(kLoopFeature.getType()).GetCString(), kLoopFeature.getDescription(1));

			setListHelp(szBuffer, szTempBuffer, szFeature, L", ", bFirst);

			bFirst = false;
		}
	}
	// MOD - END - Feature Damage Immunity

	if (kPromotion.getVisibilityChange() != 0)
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_VISIBILITY_TEXT", kPromotion.getVisibilityChange()));
	}

	if (kPromotion.getMovesChange() != 0)
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_MOVE_TEXT", kPromotion.getMovesChange()));
	}

	if (kPromotion.getMoveDiscountChange() != 0)
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_MOVE_DISCOUNT_TEXT", -(kPromotion.getMoveDiscountChange())));
	}

	if (kPromotion.getAirRangeChange() != 0)
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_AIR_RANGE_TEXT", kPromotion.getAirRangeChange()));
	}

	if (kPromotion.getInterceptChange() != 0)
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_INTERCEPT_TEXT", kPromotion.getInterceptChange()));
	}

	if (kPromotion.getEvasionChange() != 0)
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_EVASION_TEXT", kPromotion.getEvasionChange()));
	}

	if (kPromotion.getWithdrawalChange() != 0)
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_WITHDRAWAL_TEXT", kPromotion.getWithdrawalChange()));
	}

	if (kPromotion.getCargoChange() != 0)
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_CARGO_TEXT", kPromotion.getCargoChange()));
	}

	if (kPromotion.getCollateralDamageChange() != 0)
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_COLLATERAL_DAMAGE_TEXT", kPromotion.getCollateralDamageChange()));
	}

	if (kPromotion.getBombardRateChange() != 0)
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_BOMBARD_TEXT", kPromotion.getBombardRateChange()));
	}

	if (kPromotion.getFirstStrikesChange() != 0)
	{
		if (kPromotion.getFirstStrikesChange() == 1)
		{
			szBuffer.append(pcNewline);
			szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_FIRST_STRIKE_TEXT", kPromotion.getFirstStrikesChange()));
		}
		else
		{
			szBuffer.append(pcNewline);
			szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_FIRST_STRIKES_TEXT", kPromotion.getFirstStrikesChange()));
		}
	}

	if (kPromotion.getChanceFirstStrikesChange() != 0)
	{
		if (kPromotion.getChanceFirstStrikesChange() == 1)
		{
			szBuffer.append(pcNewline);
			szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_FIRST_STRIKE_CHANCE_TEXT", kPromotion.getChanceFirstStrikesChange()));
		}
		else
		{
			szBuffer.append(pcNewline);
			szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_FIRST_STRIKES_CHANCE_TEXT", kPromotion.getChanceFirstStrikesChange()));
		}
	}

	if (kPromotion.getEnemyHealChange() != 0)
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_HEALS_EXTRA_TEXT", kPromotion.getEnemyHealChange()) + gDLL->getText("TXT_KEY_PROMOTION_ENEMY_LANDS_TEXT"));
	}

	if (kPromotion.getNeutralHealChange() != 0)
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_HEALS_EXTRA_TEXT", kPromotion.getNeutralHealChange()) + gDLL->getText("TXT_KEY_PROMOTION_NEUTRAL_LANDS_TEXT"));
	}

	if (kPromotion.getFriendlyHealChange() != 0)
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_HEALS_EXTRA_TEXT", kPromotion.getFriendlyHealChange()) + gDLL->getText("TXT_KEY_PROMOTION_FRIENDLY_LANDS_TEXT"));
	}

	if (kPromotion.getSameTileHealChange() != 0)
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_HEALS_SAME_TEXT", kPromotion.getSameTileHealChange()) + gDLL->getText("TXT_KEY_PROMOTION_DAMAGE_TURN_TEXT"));
	}

	if (kPromotion.getAdjacentTileHealChange() != 0)
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_HEALS_ADJACENT_TEXT", kPromotion.getAdjacentTileHealChange()) + gDLL->getText("TXT_KEY_PROMOTION_DAMAGE_TURN_TEXT"));
	}

	if (kPromotion.getCombatPercent() != 0)
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_STRENGTH_TEXT", kPromotion.getCombatPercent()));
	}

	if (kPromotion.getCityAttackPercent() != 0)
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_CITY_ATTACK_TEXT", kPromotion.getCityAttackPercent()));
	}

	if (kPromotion.getCityDefensePercent() != 0)
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_CITY_DEFENSE_TEXT", kPromotion.getCityDefensePercent()));
	}

	if (kPromotion.getHillsAttackPercent() != 0)
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_HILLS_ATTACK", kPromotion.getHillsAttackPercent()));
	}

	if (kPromotion.getHillsDefensePercent() != 0)
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_HILLS_DEFENSE_TEXT", kPromotion.getHillsDefensePercent()));
	}

	if (kPromotion.getRevoltProtection() != 0)
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_REVOLT_PROTECTION_TEXT", kPromotion.getRevoltProtection()));
	}

	if (kPromotion.getCollateralDamageProtection() != 0)
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_COLLATERAL_PROTECTION_TEXT", kPromotion.getCollateralDamageProtection()));
	}

	if (kPromotion.getPillageChange() != 0)
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_PILLAGE_CHANGE_TEXT", kPromotion.getPillageChange()));
	}

	if (kPromotion.getUpgradeDiscount() != 0)
	{
		if (100 == kPromotion.getUpgradeDiscount())
		{
			szBuffer.append(pcNewline);
			szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_UPGRADE_DISCOUNT_FREE_TEXT"));
		}
		else
		{
			szBuffer.append(pcNewline);
			szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_UPGRADE_DISCOUNT_TEXT", kPromotion.getUpgradeDiscount()));
		}
	}

	if (kPromotion.getExperiencePercent() != 0)
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_FASTER_EXPERIENCE_TEXT", kPromotion.getExperiencePercent()));
	}

	if (kPromotion.getKamikazePercent() != 0)
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_KAMIKAZE_TEXT", kPromotion.getKamikazePercent()));
	}

	for (iI = 0; iI < GC.getNumTerrainInfos(); ++iI)
	{
		if (kPromotion.getTerrainAttackPercent(iI) != 0)
		{
			szBuffer.append(pcNewline);
			szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_ATTACK_TEXT", kPromotion.getTerrainAttackPercent(iI), GC.getTerrainInfo((TerrainTypes) iI).getTextKeyWide()));
		}

		if (kPromotion.getTerrainDefensePercent(iI) != 0)
		{
			szBuffer.append(pcNewline);
			szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_DEFENSE_TEXT", kPromotion.getTerrainDefensePercent(iI), GC.getTerrainInfo((TerrainTypes) iI).getTextKeyWide()));
		}
	}

	for (iI = 0; iI < GC.getNumFeatureInfos(); ++iI)
	{
		if (kPromotion.getFeatureAttackPercent(iI) != 0)
		{
			szBuffer.append(pcNewline);
			szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_ATTACK_TEXT", kPromotion.getFeatureAttackPercent(iI), GC.getFeatureInfo((FeatureTypes) iI).getTextKeyWide()));
		}

		if (kPromotion.getFeatureDefensePercent(iI) != 0)
		{
			szBuffer.append(pcNewline);
			szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_DEFENSE_TEXT", kPromotion.getFeatureDefensePercent(iI), GC.getFeatureInfo((FeatureTypes) iI).getTextKeyWide()));
		}
	}

	// MOD - START - Extra Barbarian Combat Modifier
	if (kPromotion.getExtraBarbarianCombatModifier() != 0)
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_BARBARIAN_COMBAT_MOD", kPromotion.getExtraBarbarianCombatModifier()));
	}
	// MOD - END - Extra Barbarian Combat Modifier

	for (iI = 0; iI < GC.getNumUnitCombatInfos(); ++iI)
	{
		if (kPromotion.getUnitCombatModifierPercent(iI) != 0)
		{
			// MOD - START - Improved Civilopedia
			const CvInfoBase& kLoopUnitCombat = GC.getUnitCombatInfo((UnitCombatTypes)iI);
			szBuffer.append(pcNewline);
			szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_VERSUS_TEXT", kPromotion.getUnitCombatModifierPercent(iI), CvWString(kLoopUnitCombat.getType()).GetCString(), kLoopUnitCombat.getTextKeyWide()));
			// MOD - END - Improved Civilopedia
		}
	}

	for (iI = 0; iI < NUM_DOMAIN_TYPES; ++iI)
	{
		if (kPromotion.getDomainModifierPercent(iI) != 0)
		{
			// MOD - START - Improved Civilopedia
			const CvDomainInfo& kLoopDomain = GC.getDomainInfo((DomainTypes)iI);
			szBuffer.append(pcNewline);
			szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_VERSUS_TEXT", kPromotion.getDomainModifierPercent(iI), CvWString(kLoopDomain.getType()).GetCString(), kLoopDomain.getTextKeyWide()));
			// MOD - END - Improved Civilopedia
		}
	}

	// MOD - START - Advanced Experience Limits
	if (kPromotion.getExtraBarbarianMaxXPValue() != 0)
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_EXTRA_BARBARIAN_XP_VALUE_TEXT", kPromotion.getExtraBarbarianMaxXPValue()));
	}
	// MOD - END - Advanced Experience Limits

	if (wcslen(kPromotion.getHelp()) > 0)
	{
		szBuffer.append(pcNewline);
		szBuffer.append(kPromotion.getHelp());
	}
}

// MOD - START - Aid
//
// Aid Help
//
void CvGameTextMgr::parseAidHelp(CvWStringBuffer &szBuffer, AidTypes eAid, const wchar* pcNewline)
{
	//CvWString szText, szText2;
	//int iI;

	if (NO_AID == eAid)
	{
		return;
	}

	CvAidInfo& aid = GC.getAidInfo(eAid);
	int iRange = aid.getRange();

	if (iRange == 2)
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_AID_AFFECTS_ADJACENT"));
	}
	else if (iRange > 2)
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_AID_AFFECTS_RANGE", iRange));
	}

	if (aid.isCoastalOnly())
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_AID_COASTAL_ONLY"));
	}
}
// MOD - END - Aid

// MOD - START - Detriments
//
// Detriment Help
//
void CvGameTextMgr::parseDetrimentHelp(CvWStringBuffer &szBuffer, DetrimentTypes eDetriment, const wchar* pcNewline)
{
	//CvWString szText, szText2;
	//int iI;

	if (NO_DETRIMENT == eDetriment)
	{
		return;
	}

	const CvDetrimentInfo& kDetriment = GC.getDetrimentInfo(eDetriment);
	int iRange = kDetriment.getRange();

	if (iRange == 2)
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_DETRIMENT_AFFECTS_ADJACENT"));
	}
	else if (iRange > 2)
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_DETRIMENT_AFFECTS_RANGE", iRange));
	}

	if (kDetriment.isCoastalOnly())
	{
		szBuffer.append(pcNewline);
		szBuffer.append(gDLL->getText("TXT_KEY_DETRIMENT_COASTAL_ONLY"));
	}
}
// MOD - END - Detriments

//	Function:			parseCivicInfo()
//	Description:	Will parse the civic info help
//	Parameters:		szHelpText -- the text to put it into
//								civicInfo - what to parse
//	Returns:			nothing
void CvGameTextMgr::parseCivicInfo(CvWStringBuffer &szHelpText, CivicTypes eCivic, bool bCivilopediaText, bool bPlayerContext, bool bSkipName)
{
	PROFILE_FUNC();

	CvWString szFirstBuffer;
	bool bFound;
	bool bFirst;
	int iLast;

	if (NO_CIVIC == eCivic)
	{
		return;
	}

	CvCivicInfo& kCivic = GC.getCivicInfo(eCivic);

	szHelpText.clear();

	FAssert(GC.getGameINLINE().getActivePlayer() != NO_PLAYER || !bPlayerContext);

	if (!bSkipName)
	{
		szHelpText.append(kCivic.getDescription());
	}

	if (!bCivilopediaText)
	{
		if (!bPlayerContext || !(GET_PLAYER(GC.getGameINLINE().getActivePlayer()).canDoCivics(eCivic)))
		{
			if (!bPlayerContext || !(GET_TEAM(GC.getGameINLINE().getActiveTeam()).isHasTech((TechTypes)(kCivic.getTechPrereq()))))
			{
				if (kCivic.getTechPrereq() != NO_TECH)
				{
					szHelpText.append(NEWLINE);
					szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_REQUIRES", GC.getTechInfo((TechTypes)kCivic.getTechPrereq()).getTextKeyWide()));
				}
			}
		}
	}

	// Special Building Not Required...
	for (int iI = 0; iI < GC.getNumSpecialBuildingInfos(); ++iI)
	{
		if (kCivic.isSpecialBuildingNotRequired(iI))
		{
			// XXX "Missionaries"??? - Now in XML
			szHelpText.append(NEWLINE);
			szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_BUILD_MISSIONARIES", GC.getSpecialBuildingInfo((SpecialBuildingTypes)iI).getTextKeyWide()));
		}
	}

	// Valid Specialists...

	bFirst = true;

	for (int iI = 0; iI < GC.getNumSpecialistInfos(); ++iI)
	{
		if (kCivic.isSpecialistValid(iI))
		{
			szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_CIVIC_UNLIMTED").c_str());
			CvWString szSpecialist;
			szSpecialist.Format(L"<link=literal>%s</link>", GC.getSpecialistInfo((SpecialistTypes)iI).getDescription());
			setListHelp(szHelpText, szFirstBuffer, szSpecialist, L", ", bFirst);
			bFirst = false;
		}
	}

	//	Great People Modifier...
	if (kCivic.getGreatPeopleRateModifier() != 0)
	{
		szHelpText.append(NEWLINE);
		szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_GREAT_PEOPLE_MOD", kCivic.getGreatPeopleRateModifier()));
	}

	//	Great General Modifier...
	if (kCivic.getGreatGeneralRateModifier() != 0)
	{
		szHelpText.append(NEWLINE);
		szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_GREAT_GENERAL_MOD", kCivic.getGreatGeneralRateModifier()));
	}

	if (kCivic.getDomesticGreatGeneralRateModifier() != 0)
	{
		szHelpText.append(NEWLINE);
		szHelpText.append(gDLL->getText("TXT_KEY_DOMESTIC_GREAT_GENERAL_MODIFIER", kCivic.getDomesticGreatGeneralRateModifier()));
	}

	//	State Religion Great People Modifier...
	if (kCivic.getStateReligionGreatPeopleRateModifier() != 0)
	{
		if (bPlayerContext && (GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getStateReligion() != NO_RELIGION))
		{
			szHelpText.append(NEWLINE);
			szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_GREAT_PEOPLE_MOD_RELIGION", kCivic.getStateReligionGreatPeopleRateModifier(), GC.getReligionInfo(GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getStateReligion()).getChar()));
		}
		else
		{
			szHelpText.append(NEWLINE);
			szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_GREAT_PEOPLE_MOD_STATE_RELIGION", kCivic.getStateReligionGreatPeopleRateModifier(), gDLL->getSymbolID(RELIGION_CHAR)));
		}
	}

	//	Distance Maintenance Modifer...
	if (kCivic.getDistanceMaintenanceModifier() != 0)
	{
		if (kCivic.getDistanceMaintenanceModifier() <= -100)
		{
			szHelpText.append(NEWLINE);
			szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_DISTANCE_MAINT"));
		}
		else
		{
			szHelpText.append(NEWLINE);
			szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_DISTANCE_MAINT_MOD", kCivic.getDistanceMaintenanceModifier()));
		}
	}

	//	Num Cities Maintenance Modifer...
	if (kCivic.getNumCitiesMaintenanceModifier() != 0)
	{
		if (kCivic.getNumCitiesMaintenanceModifier() <= -100)
		{
			szHelpText.append(NEWLINE);
			szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_NO_MAINT_NUM_CITIES"));
		}
		else
		{
			szHelpText.append(NEWLINE);
			szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_NO_MAINT_NUM_CITIES_MOD", kCivic.getNumCitiesMaintenanceModifier()));
		}
	}

	//	Corporations Maintenance Modifer...
	if (kCivic.getCorporationMaintenanceModifier() != 0)
	{
		if (kCivic.getCorporationMaintenanceModifier() <= -100)
		{
			szHelpText.append(NEWLINE);
			szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_NO_MAINT_CORPORATION"));
		}
		else
		{
			szHelpText.append(NEWLINE);
			szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_NO_MAINT_CORPORATION_MOD", kCivic.getCorporationMaintenanceModifier()));
		}
	}

	//	Extra Health
	if (kCivic.getExtraHealth() != 0)
	{
		szHelpText.append(NEWLINE);
		szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_EXTRA_HEALTH", abs(kCivic.getExtraHealth()), ((kCivic.getExtraHealth() > 0) ? gDLL->getSymbolID(HEALTHY_CHAR): gDLL->getSymbolID(UNHEALTHY_CHAR))));
	}

	// MOD - START - Epidemics
	// Epidemic system by Mexico
	if (kCivic.getGlobalEpidemicModifier() != 0)
	{
		CvWString szTempBuffer;
		szTempBuffer.Format(L"%+d", (kCivic.getGlobalEpidemicModifier() * 100) / GC.getDefineINT("EPIDEMIC_RAND_BASE"));
		szHelpText.append(NEWLINE);
		szHelpText.append(gDLL->getText("TXT_KEY_GLOBAL_EPIDEMIC_MODIFIER", szTempBuffer.GetCString()));
	}
	// MOD - END - Epidemics

	//	Extra Happiness (new in K-Mod)
	if (kCivic.getExtraHappiness() != 0)
	{
		szHelpText.append(NEWLINE);
		szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_EXTRA_HEALTH", abs(kCivic.getExtraHappiness()), kCivic.getExtraHappiness() > 0 ? gDLL->getSymbolID(HAPPY_CHAR): gDLL->getSymbolID(UNHAPPY_CHAR)));
		// note: TXT_KEY_CIVIC_EXTRA_HEALTH just says "[blah] in all cities", so it's ok for happiness as well as for health.
	}

	//	Free Experience
	if (kCivic.getFreeExperience() != 0)
	{
		szHelpText.append(NEWLINE);
		szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_FREE_XP", kCivic.getFreeExperience()));
	}

	//	Worker speed modifier
	if (kCivic.getWorkerSpeedModifier() != 0)
	{
		szHelpText.append(NEWLINE);
		// MOD - START - Negative Worker Speed Modifiers
		//szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_WORKER_SPEED", kCivic.getWorkerSpeedModifier()));
		szHelpText.append(gDLL->getText(kCivic.getWorkerSpeedModifier() >= 0 ? "TXT_KEY_WORKER_SPEED_MODIFIER_FASTER" : "TXT_KEY_WORKER_SPEED_MODIFIER_SLOWER", abs(kCivic.getWorkerSpeedModifier())));
		// MOD - END - Negative Worker Speed Modifiers
	}

	//	Improvement upgrade rate modifier
	if (kCivic.getImprovementUpgradeRateModifier() != 0)
	{
		bFirst = true;

		for (int iI = 0; iI < GC.getNumImprovementInfos(); ++iI)
		{
			const CvImprovementInfo& kImprovement = GC.getImprovementInfo((ImprovementTypes)iI);

			if (kImprovement.getImprovementUpgrade() != NO_IMPROVEMENT)
			{
				// MOD - START - Unique Improvements
				if (!bPlayerContext || GC.getCivilizationInfo(GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getCivilizationType()).canProduceImprovement((ImprovementTypes)iI))
				{
				// MOD - END - Unique Improvements
					szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_CIVIC_IMPROVEMENT_UPGRADE", kCivic.getImprovementUpgradeRateModifier()).c_str());
					CvWString szImprovement;
					szImprovement.Format(L"<link=literal>%s</link>", kImprovement.getDescription());
					setListHelp(szHelpText, szFirstBuffer, szImprovement, L", ", bFirst);
					bFirst = false;
				}
			}
		}
	}

	//	Military unit production modifier
	if (kCivic.getMilitaryProductionModifier() != 0)
	{
		szHelpText.append(NEWLINE);
		szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_MILITARY_PRODUCTION", kCivic.getMilitaryProductionModifier()));
	}

	//	Free units population percent
	if ((kCivic.getBaseFreeUnits() != 0) || (kCivic.getFreeUnitsPopulationPercent() != 0))
	{
		if (bPlayerContext)
		{
			szHelpText.append(NEWLINE);
			szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_FREE_UNITS", (kCivic.getBaseFreeUnits() + ((GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getTotalPopulation() * kCivic.getFreeUnitsPopulationPercent()) / 100))));
		}
		else
		{
			szHelpText.append(NEWLINE);
			szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_UNIT_SUPPORT"));
		}
	}

	//	Free military units population percent
	if ((kCivic.getBaseFreeMilitaryUnits() != 0) || (kCivic.getFreeMilitaryUnitsPopulationPercent() != 0))
	{
		if (bPlayerContext)
		{
			szHelpText.append(NEWLINE);
			szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_FREE_MILITARY_UNITS", (kCivic.getBaseFreeMilitaryUnits() + ((GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getTotalPopulation() * kCivic.getFreeMilitaryUnitsPopulationPercent()) / 100))));
		}
		else
		{
			szHelpText.append(NEWLINE);
			szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_MILITARY_UNIT_SUPPORT"));
		}
	}

	//	Happiness per military unit
	if (kCivic.getHappyPerMilitaryUnit() != 0)
	{
		szHelpText.append(NEWLINE);
/************************************************************************************************/
/* UNOFFICIAL_PATCH                       08/28/09                            jdog5000          */
/*                                                                                              */
/* Bugfix                                                                                       */
/************************************************************************************************/
/* original bts code
		szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_UNIT_HAPPINESS", kCivic.getHappyPerMilitaryUnit(), ((kCivic.getHappyPerMilitaryUnit() > 0) ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR))));
*/
		// Use absolute value with unhappy face
		szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_UNIT_HAPPINESS", abs(kCivic.getHappyPerMilitaryUnit()), ((kCivic.getHappyPerMilitaryUnit() > 0) ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR))));
/************************************************************************************************/
/* UNOFFICIAL_PATCH                        END                                                  */
/************************************************************************************************/
	}

	//	Military units produced with food
	if (kCivic.isMilitaryFoodProduction())
	{
		szHelpText.append(NEWLINE);
		szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_MILITARY_FOOD"));
	}

	//	Conscription
	if (getWorldSizeMaxConscript(eCivic) != 0)
	{
		szHelpText.append(NEWLINE);
		szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_CONSCRIPTION", getWorldSizeMaxConscript(eCivic)));
	}

	//	Population Unhealthiness
/*
** K-Mod, 27/dec/10, karadoc
** replaced NoUnhealthyPopulation with UnhealthyPopulationModifier
*/
	/* original bts code
	if (kCivic.isNoUnhealthyPopulation())
	{
		szHelpText.append(NEWLINE);
		szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_NO_POP_UNHEALTHY"));
	}
	*/
	if (kCivic.getUnhealthyPopulationModifier() != 0)
	{
		// If the modifier is less than -100, display the old NoUnhealth. text
		// Note: this could be technically inaccurate if we combine this modifier with a positive modifier
		if (kCivic.getUnhealthyPopulationModifier() <= -100)
		{
			szHelpText.append(NEWLINE);
			szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_NO_POP_UNHEALTHY"));
		}
		else
		{
			szHelpText.append(NEWLINE);
			szHelpText.append(gDLL->getText("TXT_KEY_UNHEALTHY_POP_MODIFIER", kCivic.getUnhealthyPopulationModifier()));
		}
	}
/*
** K-Mod end
*/

	//	Building Unhealthiness
	if (kCivic.isBuildingOnlyHealthy())
	{
		szHelpText.append(NEWLINE);
		szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_NO_BUILDING_UNHEALTHY"));
	}

	//	Population Unhealthiness
	if (0 != kCivic.getExpInBorderModifier())
	{
		szHelpText.append(NEWLINE);
		szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_EXPERIENCE_IN_BORDERS", kCivic.getExpInBorderModifier()));
	}

	//	War Weariness
	if (kCivic.getWarWearinessModifier() != 0)
	{
		if (kCivic.getWarWearinessModifier() <= -100)
		{
			szHelpText.append(NEWLINE);
			szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_NO_WAR_WEARINESS"));
		}
		else
		{
			szHelpText.append(NEWLINE);
			szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_EXTRA_WAR_WEARINESS", kCivic.getWarWearinessModifier()));
		}
	}

	// MOD - START - Congestion
	if (kCivic.getInsideCityCongestionThresholdChange() == kCivic.getOutsideCityCongestionThresholdChange())
	{
		if (kCivic.getInsideCityCongestionThresholdChange() != 0)
		{
			szHelpText.append(NEWLINE);
			szHelpText.append(gDLL->getText("TXT_KEY_CONGESTION_THRESHOLD_CHANGE", kCivic.getInsideCityCongestionThresholdChange()));
		}
	}
	else
	{
		if (kCivic.getInsideCityCongestionThresholdChange() != 0)
		{
			szHelpText.append(NEWLINE);
			szHelpText.append(gDLL->getText("TXT_KEY_CONGESTION_INSIDE_CITY_THRESHOLD_CHANGE", kCivic.getInsideCityCongestionThresholdChange()));
		}

		if (kCivic.getOutsideCityCongestionThresholdChange() != 0)
		{
			szHelpText.append(NEWLINE);
			szHelpText.append(gDLL->getText("TXT_KEY_CONGESTION_OUTSIDE_CITY_THRESHOLD_CHANGE", kCivic.getOutsideCityCongestionThresholdChange()));
		}
	}
	// MOD - END - Congestion

	//	Free specialists
	if (kCivic.getFreeSpecialist() != 0)
	{
		szHelpText.append(NEWLINE);
		szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_FREE_SPECIALISTS", kCivic.getFreeSpecialist()));
	}

	//	Trade routes
	if (kCivic.getTradeRoutes() != 0)
	{
		szHelpText.append(NEWLINE);
		szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_TRADE_ROUTES", kCivic.getTradeRoutes()));
	}

	//	No Foreign Trade
	if (kCivic.isNoForeignTrade())
	{
		szHelpText.append(NEWLINE);
		szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_NO_FOREIGN_TRADE"));
	}

	//	No Corporations
	if (kCivic.isNoCorporations())
	{
		szHelpText.append(NEWLINE);
		szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_NO_CORPORATIONS"));
	}

	//	No Foreign Corporations
	if (kCivic.isNoForeignCorporations())
	{
		szHelpText.append(NEWLINE);
		szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_NO_FOREIGN_CORPORATIONS"));
	}

	//	Freedom Anger
	if (kCivic.getCivicPercentAnger() != 0)
	{
		szHelpText.append(NEWLINE);
		szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_FREEDOM_ANGER", kCivic.getTextKeyWide()));
	}

	if (!(kCivic.isStateReligion()))
	{
		bFound = false;

		for (int iI = 0; iI < GC.getNumCivicInfos(); ++iI)
		{
			if ((GC.getCivicInfo((CivicTypes) iI).getCivicOptionType() == kCivic.getCivicOptionType()) && (GC.getCivicInfo((CivicTypes) iI).isStateReligion()))
			{
				bFound = true;
			}
		}

		if (bFound)
		{
			szHelpText.append(NEWLINE);
			szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_NO_STATE_RELIGION"));
		}
	}

	if (kCivic.getStateReligionHappiness() != 0)
	{
		if  (kCivic.getStateReligionHappiness() == -1)
		{
			szHelpText.append(NEWLINE);
			szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_NO_RELIGION_HAPPINESS", abs(kCivic.getStateReligionHappiness()), (gDLL->getSymbolID(HAPPY_CHAR))));
		}
		else 
		{
			if (bPlayerContext && (GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getStateReligion() != NO_RELIGION))
			{
				szHelpText.append(NEWLINE);
				szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_STATE_RELIGION_HAPPINESS", abs(kCivic.getStateReligionHappiness()), ((kCivic.getStateReligionHappiness() > 0) ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR)), GC.getReligionInfo(GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getStateReligion()).getChar()));
			}
			else
			{
				szHelpText.append(NEWLINE);
				szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_RELIGION_HAPPINESS", abs(kCivic.getStateReligionHappiness()), ((kCivic.getStateReligionHappiness() > 0) ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR))));
			}
		}
	}

	if (kCivic.getNonStateReligionHappiness() != 0)
	{
/************************************************************************************************/
/* UNOFFICIAL_PATCH                       08/28/09                  EmperorFool & jdog5000      */
/*                                                                                              */
/* Bugfix                                                                                       */
/************************************************************************************************/
/* original bts code
		if (kCivic.isStateReligion())
		{
			szHelpText.append(NEWLINE);
			szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_NON_STATE_REL_HAPPINESS_NO_STATE"));
		}
		else
		{
			szHelpText.append(NEWLINE);
			szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_NON_STATE_REL_HAPPINESS_WITH_STATE", abs(kCivic.getNonStateReligionHappiness()), ((kCivic.getNonStateReligionHappiness() > 0) ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR))));
		}
*/
		if (!kCivic.isStateReligion())
		{
			szHelpText.append(NEWLINE);
			szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_NON_STATE_REL_HAPPINESS_NO_STATE", abs(kCivic.getNonStateReligionHappiness()), ((kCivic.getNonStateReligionHappiness() > 0) ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR))));
		}
		else
		{
			szHelpText.append(NEWLINE);
			szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_NON_STATE_REL_HAPPINESS_WITH_STATE", abs(kCivic.getNonStateReligionHappiness()), ((kCivic.getNonStateReligionHappiness() > 0) ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR))));
		}
/************************************************************************************************/
/* UNOFFICIAL_PATCH                        END                                                  */
/************************************************************************************************/
	}

	//	State Religion Unit Production Modifier
	if (kCivic.getStateReligionUnitProductionModifier() != 0)
	{
		if (bPlayerContext && (GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getStateReligion() != NO_RELIGION))
		{
			szHelpText.append(NEWLINE);
			szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_REL_TRAIN_BONUS", GC.getReligionInfo(GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getStateReligion()).getChar(), kCivic.getStateReligionUnitProductionModifier()));
		}
		else
		{
			szHelpText.append(NEWLINE);
			szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_STATE_REL_TRAIN_BONUS", kCivic.getStateReligionUnitProductionModifier()));
		}
	}

	//	State Religion Building Production Modifier
	if (kCivic.getStateReligionBuildingProductionModifier() != 0)
	{
		if (bPlayerContext && (GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getStateReligion() != NO_RELIGION))
		{
			szHelpText.append(NEWLINE);
			szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_REL_BUILDING_BONUS", GC.getReligionInfo(GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getStateReligion()).getChar(), kCivic.getStateReligionBuildingProductionModifier()));
		}
		else
		{
			szHelpText.append(NEWLINE);
			szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_STATE_REL_BUILDING_BONUS", kCivic.getStateReligionBuildingProductionModifier()));
		}
	}

	//	State Religion Free Experience
	if (kCivic.getStateReligionFreeExperience() != 0)
	{
		if (bPlayerContext && (GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getStateReligion() != NO_RELIGION))
		{
			szHelpText.append(NEWLINE);
			szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_REL_FREE_XP", kCivic.getStateReligionFreeExperience(), GC.getReligionInfo(GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getStateReligion()).getChar()));
		}
		else
		{
			szHelpText.append(NEWLINE);
			szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_STATE_REL_FREE_XP", kCivic.getStateReligionFreeExperience()));
		}
	}

	if (kCivic.isNoNonStateReligionSpread())
	{
		szHelpText.append(NEWLINE);
		szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_NO_NON_STATE_SPREAD"));
	}

	// MOD - START - Holy City Migration
	if (kCivic.isCanAcquireHolyCityWithoutStateReligion())
	{
		szHelpText.append(NEWLINE);
		szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_CAN_ACQUIRE_HOLY_CITY_WITHOUT_STATE_RELIGION"));
	}
	else if (kCivic.isCanMaintainHolyCityWithoutStateReligion())
	{
		szHelpText.append(NEWLINE);
		szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_CAN_MAINTAIN_HOLY_CITY_WITHOUT_STATE_RELIGION"));
	}
	// MOD - END - Holy City Migration

	//	Yield Modifiers
	setYieldChangeHelp(szHelpText, L"", L"", gDLL->getText("TXT_KEY_CIVIC_IN_ALL_CITIES").GetCString(), kCivic.getYieldModifierArray(), true);

	//	Capital Yield Modifiers
	setYieldChangeHelp(szHelpText, L"", L"", gDLL->getText("TXT_KEY_CIVIC_IN_CAPITAL").GetCString(), kCivic.getCapitalYieldModifierArray(), true);

	//	Trade Yield Modifiers
	setYieldChangeHelp(szHelpText, L"", L"", gDLL->getText("TXT_KEY_CIVIC_FROM_TRADE_ROUTES").GetCString(), kCivic.getTradeYieldModifierArray(), true);

	//	Commerce Modifier
	setCommerceChangeHelp(szHelpText, L"", L"", gDLL->getText("TXT_KEY_CIVIC_IN_ALL_CITIES").GetCString(), kCivic.getCommerceModifierArray(), true);

	//	Capital Commerce Modifiers
	setCommerceChangeHelp(szHelpText, L"", L"", gDLL->getText("TXT_KEY_CIVIC_IN_CAPITAL").GetCString(), kCivic.getCapitalCommerceModifierArray(), true);

	//	Specialist Commerce
	// MOD - START - Advanced Yield and Commerce Effects
	//setCommerceChangeHelp(szHelpText, L"", L"", gDLL->getText("TXT_KEY_CIVIC_PER_SPECIALIST").GetCString(), kCivic.getSpecialistExtraCommerceArray());
	setCommerceChangeHelp(szHelpText, L"", L"", gDLL->getText("TXT_KEY_CIVIC_PER_SPECIALIST").GetCString(), kCivic.getAllSpecialistCommerceArray());
	// MOD - END - Advanced Yield and Commerce Effects

	//	Largest City Happiness
	if (kCivic.getLargestCityHappiness() != 0)
	{
		szHelpText.append(NEWLINE);
/************************************************************************************************/
/* UNOFFICIAL_PATCH                       08/28/09                            jdog5000          */
/*                                                                                              */
/* Bugfix                                                                                       */
/************************************************************************************************/
/* original bts code
		szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_LARGEST_CITIES_HAPPINESS", kCivic.getLargestCityHappiness(), ((kCivic.getLargestCityHappiness() > 0) ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR)), GC.getWorldInfo(GC.getMapINLINE().getWorldSize()).getTargetNumCities()));
*/
		// Use absolute value with unhappy face
		szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_LARGEST_CITIES_HAPPINESS", abs(kCivic.getLargestCityHappiness()), ((kCivic.getLargestCityHappiness() > 0) ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR)), GC.getWorldInfo(GC.getMapINLINE().getWorldSize()).getTargetNumCities()));
/************************************************************************************************/
/* UNOFFICIAL_PATCH                        END                                                  */
/************************************************************************************************/
	}
	
	// MOD - START - Advanced Yield and Commerce Effects
	for (int iI = 0; iI < NUM_YIELD_TYPES; ++iI)
	{
		YieldTypes eLoopYield = (YieldTypes)iI;
		iLast = 0;

		//	Improvement Yields
		for (int iJ = 0; iJ < kCivic.getNumImprovementYieldChanges(); iJ++)
		{
			if (kCivic.getImprovementYieldChangeYield(iJ) == eLoopYield)
			{
				ImprovementTypes eLoopImprovement = (ImprovementTypes) kCivic.getImprovementYieldChangeImprovement(iJ);
				// MOD - START - Unique Improvements
				if (!bPlayerContext || GC.getCivilizationInfo(GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getCivilizationType()).canProduceImprovement(eLoopImprovement))
				{
				// MOD - END - Unique Improvements
					int iValue = kCivic.getImprovementYieldChangeValue(iJ);
					szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_CIVIC_IMPROVEMENT_YIELD_CHANGE", iValue, GC.getYieldInfo(eLoopYield).getChar()).c_str());
					CvWString szImprovement;
					szImprovement.Format(L"<link=literal>%s</link>", GC.getImprovementInfo(eLoopImprovement).getDescription());
					setListHelp(szHelpText, szFirstBuffer, szImprovement, L", ", (iValue != iLast));
					iLast = iValue;
				}
			}
		}

		for (int iJ = 0; iJ < kCivic.getNumSpecialistYieldChanges(); iJ++)
		{
			if (kCivic.getSpecialistYieldChangeYield(iJ) == eLoopYield)
			{
				SpecialistTypes eLoopSpecialist = (SpecialistTypes) kCivic.getSpecialistYieldChangeSpecialist(iJ);
				int iValue = kCivic.getSpecialistYieldChangeValue(iJ);
				szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_CIVIC_IMPROVEMENT_YIELD_CHANGE", iValue, GC.getYieldInfo(eLoopYield).getChar()).c_str());
				CvWString szSpecialist;
				szSpecialist.Format(L"<link=literal>%s</link>", GC.getSpecialistInfo(eLoopSpecialist).getDescription());
				setListHelp(szHelpText, szFirstBuffer, szSpecialist, L", ", (iValue != iLast));
				iLast = iValue;
			}
		}
	}

	// Specialist Commerce
	for (int iI = 0; iI < NUM_COMMERCE_TYPES; ++iI)
	{
		CommerceTypes eLoopCommerce = (CommerceTypes)iI;
		iLast = 0;

		for (int iJ = 0; iJ < kCivic.getNumSpecialistCommerceChanges(); iJ++)
		{
			if (kCivic.getSpecialistCommerceChangeCommerce(iJ) == eLoopCommerce)
			{
				SpecialistTypes eLoopSpecialist = (SpecialistTypes) kCivic.getSpecialistCommerceChangeSpecialist(iJ);
				int iValue = kCivic.getSpecialistCommerceChangeValue(iJ);
				szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_CIVIC_IMPROVEMENT_YIELD_CHANGE", iValue, GC.getCommerceInfo(eLoopCommerce).getChar()).c_str());
				CvWString szSpecialist;
				szSpecialist.Format(L"<link=literal>%s</link>", GC.getSpecialistInfo(eLoopSpecialist).getDescription());
				setListHelp(szHelpText, szFirstBuffer, szSpecialist, L", ", (iValue != iLast));
				iLast = iValue;
			}
		}
	}
	// MOD - END - Advanced Yield and Commerce Effects

	// MOD - START - Relation Caching
	//	Building Happiness
	// MOD - START - Advanced Building Prerequisites (Civic)
	for (int iRel = 0; iRel < kCivic.getNumRelatedBuildingClasses(); iRel++)
	{
		BuildingClassTypes eLoopBuildingClass = (BuildingClassTypes)kCivic.getRelatedBuildingClasses(iRel);

		if (kCivic.getBuildingHappinessChanges(eLoopBuildingClass) != 0)
		{
			if (bPlayerContext && NO_PLAYER != GC.getGameINLINE().getActivePlayer())
			{
				BuildingTypes eBuilding = (BuildingTypes)GC.getCivilizationInfo(GC.getGameINLINE().getActiveCivilizationType()).getCivilizationBuildings(eLoopBuildingClass);
				if (NO_BUILDING != eBuilding)
				{
					szHelpText.append(NEWLINE);
/************************************************************************************************/
/* UNOFFICIAL_PATCH                       08/28/09                            jdog5000          */
/*                                                                                              */
/* Bugfix                                                                                       */
/************************************************************************************************/
/* original bts code
					szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_BUILDING_HAPPINESS", kCivic.getBuildingHappinessChanges(eLoopBuildingClass), ((kCivic.getBuildingHappinessChanges(eLoopBuildingClass) > 0) ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR)), GC.getBuildingInfo(eBuilding).getTextKeyWide()));
*/
					// Use absolute value with unhappy face
					szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_BUILDING_HAPPINESS", abs(kCivic.getBuildingHappinessChanges(eLoopBuildingClass)), ((kCivic.getBuildingHappinessChanges(eLoopBuildingClass) > 0) ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR)), GC.getBuildingInfo(eBuilding).getTextKeyWide()));
/************************************************************************************************/
/* UNOFFICIAL_PATCH                        END                                                  */
/************************************************************************************************/
				}
			}
			else
			{
				szHelpText.append(NEWLINE);
/************************************************************************************************/
/* UNOFFICIAL_PATCH                       08/28/09                            jdog5000          */
/*                                                                                              */
/* Bugfix                                                                                       */
/************************************************************************************************/
/* original bts code
				szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_BUILDING_HAPPINESS", kCivic.getBuildingHappinessChanges(eLoopBuildingClass), ((kCivic.getBuildingHappinessChanges(eLoopBuildingClass) > 0) ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR)), GC.getBuildingClassInfo(eLoopBuildingClass).getTextKeyWide()));
*/
				// Use absolute value with unhappy face
				szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_BUILDING_HAPPINESS", abs(kCivic.getBuildingHappinessChanges(eLoopBuildingClass)), ((kCivic.getBuildingHappinessChanges(eLoopBuildingClass) > 0) ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR)), GC.getBuildingClassInfo(eLoopBuildingClass).getTextKeyWide()));
/************************************************************************************************/
/* UNOFFICIAL_PATCH                        END                                                  */
/************************************************************************************************/
			}
		}

		if (kCivic.getBuildingHealthChanges(eLoopBuildingClass) != 0)
		{
			if (bPlayerContext && NO_PLAYER != GC.getGameINLINE().getActivePlayer())
			{
				BuildingTypes eBuilding = (BuildingTypes)GC.getCivilizationInfo(GC.getGameINLINE().getActiveCivilizationType()).getCivilizationBuildings(eLoopBuildingClass);
				if (NO_BUILDING != eBuilding)
				{
					szHelpText.append(NEWLINE);
/************************************************************************************************/
/* UNOFFICIAL_PATCH                       08/28/09                             jdog5000         */
/*                                                                                              */
/* Bugfix                                                                                       */
/************************************************************************************************/
/* original bts code
					szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_BUILDING_HAPPINESS", kCivic.getBuildingHealthChanges(eLoopBuildingClass), ((kCivic.getBuildingHealthChanges(eLoopBuildingClass) > 0) ? gDLL->getSymbolID(HEALTHY_CHAR) : gDLL->getSymbolID(UNHEALTHY_CHAR)), GC.getBuildingInfo(eBuilding).getTextKeyWide()));
*/
					// Use absolute value with unhealthy symbol
					szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_BUILDING_HAPPINESS", abs(kCivic.getBuildingHealthChanges(eLoopBuildingClass)), ((kCivic.getBuildingHealthChanges(eLoopBuildingClass) > 0) ? gDLL->getSymbolID(HEALTHY_CHAR) : gDLL->getSymbolID(UNHEALTHY_CHAR)), GC.getBuildingInfo(eBuilding).getTextKeyWide()));
/************************************************************************************************/
/* UNOFFICIAL_PATCH                        END                                                  */
/************************************************************************************************/
				}
			}
			else
			{
				szHelpText.append(NEWLINE);
/************************************************************************************************/
/* UNOFFICIAL_PATCH                       08/28/09                             jdog5000         */
/*                                                                                              */
/* Bugfix                                                                                       */
/************************************************************************************************/
/* original bts code
				szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_BUILDING_HAPPINESS", kCivic.getBuildingHealthChanges(eLoopBuildingClass), ((kCivic.getBuildingHealthChanges(eLoopBuildingClass) > 0) ? gDLL->getSymbolID(HEALTHY_CHAR) : gDLL->getSymbolID(UNHEALTHY_CHAR)), GC.getBuildingClassInfo(eLoopBuildingClass).getTextKeyWide()));
*/
				// Use absolute value with unhealthy symbol
				szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_BUILDING_HAPPINESS", abs(kCivic.getBuildingHealthChanges(eLoopBuildingClass)), ((kCivic.getBuildingHealthChanges(eLoopBuildingClass) > 0) ? gDLL->getSymbolID(HEALTHY_CHAR) : gDLL->getSymbolID(UNHEALTHY_CHAR)), GC.getBuildingClassInfo(eLoopBuildingClass).getTextKeyWide()));
/************************************************************************************************/
/* UNOFFICIAL_PATCH                        END                                                  */
/************************************************************************************************/
			}
		}
	}
	// MOD - END - Advanced Building Prerequisites (Civic)

	//	Feature Happiness
	iLast = 0;

	for (int iI = 0; iI < GC.getNumFeatureInfos(); ++iI)
	{
		if (kCivic.getFeatureHappinessChanges(iI) != 0)
		{
/************************************************************************************************/
/* UNOFFICIAL_PATCH                       08/28/09                             jdog5000         */
/*                                                                                              */
/* Bugfix                                                                                       */
/************************************************************************************************/
/* original bts code
			szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_CIVIC_FEATURE_HAPPINESS", kCivic.getFeatureHappinessChanges(iI), ((kCivic.getFeatureHappinessChanges(iI) > 0) ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR))).c_str());
*/
			// Use absolute value with unhappy face
			szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_CIVIC_FEATURE_HAPPINESS", abs(kCivic.getFeatureHappinessChanges(iI)), ((kCivic.getFeatureHappinessChanges(iI) > 0) ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR))).c_str());
/************************************************************************************************/
/* UNOFFICIAL_PATCH                        END                                                  */
/************************************************************************************************/
			CvWString szFeature;
			szFeature.Format(L"<link=literal>%s</link>", GC.getFeatureInfo((FeatureTypes)iI).getDescription());
			setListHelp(szHelpText, szFirstBuffer, szFeature, L", ", (kCivic.getFeatureHappinessChanges(iI) != iLast));
			iLast = kCivic.getFeatureHappinessChanges(iI);
		}
	}

	//	Hurry types
	for (int iI = 0; iI < GC.getNumHurryInfos(); ++iI)
	{
		if (kCivic.isHurry(iI))
		{
			szHelpText.append(CvWString::format(L"%s%c%s", NEWLINE, gDLL->getSymbolID(BULLET_CHAR), GC.getHurryInfo((HurryTypes)iI).getDescription()));
		}
	}

	float fInflationFactor = bPlayerContext ? (float)(100 + GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getInflationRate())/100 : 1.0f; // K-Mod
	//	Gold cost per unit
	if (kCivic.getGoldPerUnit() != 0)
	{
		/* szHelpText.append(NEWLINE);
		szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_SUPPORT_COSTS", (kCivic.getGoldPerUnit() > 0), GC.getCommerceInfo(COMMERCE_GOLD).getChar())); */
		// K-Mod.
		szHelpText.append(CvWString::format(L"\n%c%+.2f%c %s", gDLL->getSymbolID(BULLET_CHAR), (float)kCivic.getGoldPerUnit()*fInflationFactor/100, GC.getCommerceInfo(COMMERCE_GOLD).getChar(),
			gDLL->getText("TXT_KEY_CIVIC_SUPPORT_COSTS").GetCString()));
		// K-Mod end
	}

	//	Gold cost per military unit
	if (kCivic.getGoldPerMilitaryUnit() != 0)
	{
		/* szHelpText.append(NEWLINE);
		szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_MILITARY_SUPPORT_COSTS", (kCivic.getGoldPerMilitaryUnit() > 0), GC.getCommerceInfo(COMMERCE_GOLD).getChar())); */
		// K-Mod
		szHelpText.append(CvWString::format(L"\n%c%+.2f%c %s", gDLL->getSymbolID(BULLET_CHAR), (float)kCivic.getGoldPerMilitaryUnit()*fInflationFactor/100, GC.getCommerceInfo(COMMERCE_GOLD).getChar(),
			gDLL->getText("TXT_KEY_CIVIC_MILITARY_SUPPORT_COSTS").GetCString()));
		// K-Mod end
	}

	// MOD - START - Relation Caching
	// MOD - START - Advanced Building Prerequisites (Civic)
	if (!bCivilopediaText)
	{
		bFirst = true;
		for (int iRel = 0; iRel < kCivic.getNumRelatedBuildingClasses(); iRel++)
		{
			BuildingClassTypes eLoopBuildingClass = (BuildingClassTypes)kCivic.getRelatedBuildingClasses(iRel);
			BuildingTypes eLoopBuilding;
			if (GC.getGameINLINE().getActivePlayer() != NO_PLAYER)
			{
				eLoopBuilding = (BuildingTypes)GC.getCivilizationInfo(GC.getGameINLINE().getActiveCivilizationType()).getCivilizationBuildings(eLoopBuildingClass);
			}
			else
			{
				eLoopBuilding = (BuildingTypes)GC.getBuildingClassInfo(eLoopBuildingClass).getRepresentativeBuilding();
			}

			if (eLoopBuilding != NO_BUILDING && GC.getBuildingInfo(eLoopBuilding).isPrereqCivic(eCivic))
			{
				szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_MISC_ENABLES").c_str());
				CvWString szBuilding;
				szBuilding.Format(L"<link=literal>%s</link>", GC.getBuildingInfo(eLoopBuilding).getDescription());
				setListHelp(szHelpText, szFirstBuffer, szBuilding, L", ", bFirst);
				bFirst = false;
			}
		}
	}
	// MOD - END - Advanced Building Prerequisites (Civic)

	// MOD - START - Wonder production cost modifier
	if (kCivic.getWonderCostModifier() != 0)
	{
		szHelpText.append(NEWLINE);
		szHelpText.append(gDLL->getText("TXT_KEY_CIVIC_WONDER_PRODUCTION_MODIFIER", kCivic.getWonderCostModifier()));
	}
	// MOD - END - Wonder production cost modifier

	// MOD - START - Unit civic prereq
	if (!bCivilopediaText)
	{
		bFirst = true;
		for (int iRel = 0; iRel < kCivic.getNumRelatedUnitClasses(); iRel++)
		{
			UnitClassTypes eLoopUnitClass = (UnitClassTypes)kCivic.getRelatedUnitClasses(iRel);
			UnitTypes eLoopUnit;
			if (GC.getGameINLINE().getActivePlayer() != NO_PLAYER)
			{
				eLoopUnit = (UnitTypes)GC.getCivilizationInfo(GC.getGameINLINE().getActiveCivilizationType()).getCivilizationUnits(eLoopUnitClass);
			}
			else
			{
				eLoopUnit = (UnitTypes)GC.getUnitClassInfo(eLoopUnitClass).getRepresentativeUnit();
			}

			if (eLoopUnit != NO_UNIT && GC.getUnitInfo(eLoopUnit).getPrereqCivic() == eCivic)
			{
				szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_MISC_ENABLES").c_str());
				CvWString szUnit;
				szUnit.Format(L"<link=literal>%s</link>", GC.getUnitInfo(eLoopUnit).getDescription());
				setListHelp(szHelpText, szFirstBuffer, szUnit, L", ", bFirst);
				bFirst = false;
			}
		}
	}
	// MOD - END - Unit civic prereq
	// MOD - END - Relation Caching

	if (!CvWString(kCivic.getHelp()).empty())
	{
		szHelpText.append(CvWString::format(L"%s%s", NEWLINE, kCivic.getHelp()).c_str());
	}
}


// MOD - START - Improved Help Context
//void CvGameTextMgr::setTechHelp(CvWStringBuffer &szBuffer, TechTypes eTech, bool bCivilopediaText, bool bPlayerContext, bool bStrategyText, bool bTreeInfo, TechTypes eFromTech)
void CvGameTextMgr::setTechHelp(CvWStringBuffer &szBuffer, TechTypes eTech, CivilizationTypes eCivilization, PlayerTypes ePlayer, bool bCivilopediaText, bool bStrategyText, bool bTreeInfo, TechTypes eFromTech)
// MOD - END - Improved Help Context
{
	PROFILE_FUNC();

	CvWString szTempBuffer;
	CvWString szFirstBuffer;
	bool bFirst;

	// show debug info if cheat level > 0 and alt down
	bool bAlt = GC.altKey();
    if (bAlt && (gDLL->getChtLvl() > 0))
    {
		szBuffer.clear();
		
		for (int iI = 0; iI < MAX_PLAYERS; ++iI)
		{
			CvPlayerAI* playerI = &GET_PLAYER((PlayerTypes)iI);
			CvTeamAI* teamI = &GET_TEAM(playerI->getTeam());
			if (playerI->isAlive())
			{
				szTempBuffer.Format(L"%s: ", playerI->getName());
		        szBuffer.append(szTempBuffer);

				TechTypes ePlayerTech = playerI->getCurrentResearch();
				if (ePlayerTech == NO_TECH)
					szTempBuffer.Format(L"-\n");
				else
					szTempBuffer.Format(L"%s (%d->%dt)(%d/%d)\n", GC.getTechInfo(ePlayerTech).getDescription(), playerI->calculateResearchRate(ePlayerTech), playerI->getResearchTurnsLeft(ePlayerTech, true), teamI->getResearchProgress(ePlayerTech), teamI->getResearchCost(ePlayerTech));

		        szBuffer.append(szTempBuffer);
			}
		}

		return;
    }


	if (NO_TECH == eTech)
	{
		return;
	}

	const CvTechInfo& kTech = GC.getTechInfo(eTech);

	//	Tech Name
	if (!bCivilopediaText && (!bTreeInfo || (NO_TECH == eFromTech)))
	{
		szTempBuffer.Format( SETCOLR L"%s" ENDCOLR , TEXT_COLOR("COLOR_TECH_TEXT"), kTech.getDescription());
		szBuffer.append(szTempBuffer);
	}

	// MOD - START - Improved Help Context
	//FAssert(GC.getGameINLINE().getActivePlayer() != NO_PLAYER || !bPlayerContext);
	FAssert(GC.getGameINLINE().isFinalInitialized() || ePlayer == NO_PLAYER);
	// MOD - END - Improved Help Context

	if (bTreeInfo && (NO_TECH != eFromTech))
	{
		// MOD - START - Improved Help Context
		//buildTechTreeString(szBuffer, eTech, bPlayerContext, eFromTech);
		buildTechTreeString(szBuffer, eTech, eCivilization, ePlayer, eFromTech);
		// MOD - END - Improved Help Context
	}

	// MOD - START - Relation Caching
	// MOD - START - Improved Help Context
	// MOD - START - Building Discontinuation
	//	Discontinued Buildings
	for (int iRel = 0; iRel < kTech.getNumRelatedBuildingClasses(); iRel++)
	{
		BuildingClassTypes eLoopBuildingClass = (BuildingClassTypes)kTech.getRelatedBuildingClasses(iRel);

		if (ePlayer == NO_PLAYER || (GET_PLAYER(ePlayer).getBuildingClassCount(eLoopBuildingClass) > 0))
		{
			BuildingTypes eLoopBuilding;

			if (eCivilization != NO_CIVILIZATION)
			{
				eLoopBuilding = (BuildingTypes)GC.getCivilizationInfo(eCivilization).getCivilizationBuildings(eLoopBuildingClass);
			}
			else
			{
				eLoopBuilding = (BuildingTypes)GC.getBuildingClassInfo(eLoopBuildingClass).getRepresentativeBuilding();
			}

			if (eLoopBuilding != NO_BUILDING)
			{
				//	Discontinued Buildings Check...
				if (GC.getBuildingInfo(eLoopBuilding).getDiscontinueTech() == eTech)
				{
					buildDiscontinueString(szBuffer, eLoopBuilding, true);
				}
			}
		}
	}

	for (int iI = 0; iI < GC.getNumSpecialBuildingInfos(); ++iI)
	{
		SpecialBuildingTypes eSpecialBuilding = (SpecialBuildingTypes)iI;
		if (GC.getSpecialBuildingInfo(eSpecialBuilding).getDiscontinueTech() == eTech)
		{
			buildDiscontinueSpecialString(szBuffer, eSpecialBuilding, true);
		}
	}
	// MOD - END - Building Discontinuation

	//	Obsolete Buildings
	for (int iRel = 0; iRel < kTech.getNumRelatedBuildingClasses(); iRel++)
	{
		BuildingClassTypes eLoopBuildingClass = (BuildingClassTypes)kTech.getRelatedBuildingClasses(iRel);

		if (ePlayer == NO_PLAYER || (GET_PLAYER(ePlayer).getBuildingClassCount(eLoopBuildingClass) > 0))
		{
			BuildingTypes eLoopBuilding;

			if (eCivilization != NO_CIVILIZATION)
			{
				eLoopBuilding = (BuildingTypes)GC.getCivilizationInfo(eCivilization).getCivilizationBuildings(eLoopBuildingClass);
			}
			else
			{
				eLoopBuilding = (BuildingTypes)GC.getBuildingClassInfo(eLoopBuildingClass).getRepresentativeBuilding();
			}

			if (eLoopBuilding != NO_BUILDING)
			{
				//	Obsolete Buildings Check...
				if (GC.getBuildingInfo(eLoopBuilding).getObsoleteTech() == eTech)
				{
					buildObsoleteString(szBuffer, eLoopBuilding, true);
				}
			}
		}
	}
	// MOD - END - Improved Help Context

	for (int iI = 0; iI < GC.getNumSpecialBuildingInfos(); ++iI)
	{
		if (GC.getSpecialBuildingInfo((SpecialBuildingTypes) iI).getObsoleteTech() == eTech)
		{
			buildObsoleteSpecialString(szBuffer, (SpecialBuildingTypes)iI, true);
		}
	}

	//	Obsolete Bonuses
	for (int iRel = 0; iRel < kTech.getNumRelatedBonuses(); iRel++)
	{
		BonusTypes eLoopBonus = (BonusTypes)kTech.getRelatedBonuses(iRel);
		if (GC.getBonusInfo(eLoopBonus).getTechObsolete() == eTech)
		{
			buildObsoleteBonusString(szBuffer, eLoopBonus, true);
		}
	}

	// MOD - START - Unit Obsolete Tech (Dom Pedro II)
	for (int iRel = 0; iRel < kTech.getNumRelatedUnitClasses(); iRel++)
	{
		UnitClassTypes eLoopUnitClass = (UnitClassTypes)kTech.getRelatedUnitClasses(iRel);
		UnitTypes eLoopUnit;

		if (eCivilization != NO_CIVILIZATION)
		{
			eLoopUnit = (UnitTypes)GC.getCivilizationInfo(eCivilization).getCivilizationUnits(eLoopUnitClass);
		}
		else
		{
			eLoopUnit = (UnitTypes)GC.getUnitClassInfo(eLoopUnitClass).getRepresentativeUnit();
		}

		if (eLoopUnit != NO_UNIT)
		{
			if (GC.getUnitInfo(eLoopUnit).getObsoleteTech() == eTech)
			{
				buildObsoleteUnitString(szBuffer, eLoopUnit, true);
			}
		}
	}
	// MOD - END - Unit Obsolete Tech (Dom Pedro II)
	// MOD - END - Relation Caching

	// MOD - START - Improved Help Context
	bool bPlayerContext = (ePlayer != NO_PLAYER && GC.getGameINLINE().getActivePlayer() == ePlayer);
	// MOD - END - Improved Help Context

	//	Route movement change...
	buildMoveString(szBuffer, eTech, true, bPlayerContext);

	//	Creates a free unit...
	buildFreeUnitString(szBuffer, eTech, true, bPlayerContext);

	//	Increases feature production...
	buildFeatureProductionString(szBuffer, eTech, true, bPlayerContext);

	//	Increases worker build rate...
	buildWorkerRateString(szBuffer, eTech, true, bPlayerContext);

	//	Trade Routed per city change...
	buildTradeRouteString(szBuffer, eTech, true, bPlayerContext);

	//	Health increase...
	buildHealthRateString(szBuffer, eTech, true, bPlayerContext);

	//	Happiness increase...
	buildHappinessRateString(szBuffer, eTech, true, bPlayerContext);

	// MOD - START - Epidemics
	// Epidemic system by Mexico
	buildEpidemicGlobalModifierString(szBuffer, eTech, true, bPlayerContext);
	// MOD - END - Epidemics

	//	Free Techs...
	buildFreeTechString(szBuffer, eTech, true, bPlayerContext);

	//	Line of Sight Bonus across water...
	buildLOSString(szBuffer, eTech, true, bPlayerContext);

	//	Centers world map...
	buildMapCenterString(szBuffer, eTech, true, bPlayerContext);

	//	Reveals World Map...
	buildMapRevealString(szBuffer, eTech, true);

	//	Enables map trading...
	buildMapTradeString(szBuffer, eTech, true, bPlayerContext);

	//	Enables tech trading...
	buildTechTradeString(szBuffer, eTech, true, bPlayerContext);

	//	Enables gold trading...
	buildGoldTradeString(szBuffer, eTech, true, bPlayerContext);

	//	Enables open borders...
	buildOpenBordersString(szBuffer, eTech, true, bPlayerContext);

	//	Enables defensive pacts...
	buildDefensivePactString(szBuffer, eTech, true, bPlayerContext);

	//	Enables permanent alliances...
	buildPermanentAllianceString(szBuffer, eTech, true, bPlayerContext);

	//	Enables bridge building...
	buildBridgeString(szBuffer, eTech, true, bPlayerContext);

	//	Can spread irrigation...
	buildIrrigationString(szBuffer, eTech, true, bPlayerContext);

	//	Ignore irrigation...
	buildIgnoreIrrigationString(szBuffer, eTech, true, bPlayerContext);

	//	Coastal work...
	buildWaterWorkString(szBuffer, eTech, true, bPlayerContext);

	//	Enables permanent alliances...
	buildVassalStateString(szBuffer, eTech, true, bPlayerContext);

	//	Build farm, irrigation, etc...
	for (int iI = 0; iI < GC.getNumBuildInfos(); ++iI)
	{
		// MOD - START - Improved Help Context
		//buildImprovementString(szBuffer, eTech, iI, true, bPlayerContext);
		buildImprovementString(szBuffer, eTech, (BuildTypes)iI, eCivilization, ePlayer, true, bTreeInfo);
		// MOD - END - Improved Help Context
	}

	//	Extra moves for certain domains...
	for (int iI = 0; iI < NUM_DOMAIN_TYPES; ++iI)
	{
		buildDomainExtraMovesString(szBuffer, eTech, iI, true, bPlayerContext);
	}

	//	K-Mod. Extra specialist commerce
	// MOD - START - Advanced Yield and Commerce Effects
	//setCommerceChangeHelp(szBuffer, L"", L"", gDLL->getText("TXT_KEY_CIVIC_PER_SPECIALIST").GetCString(), GC.getTechInfo(eTech).getSpecialistExtraCommerceArray());
	setCommerceChangeHelp(szBuffer, L"", L"", gDLL->getText("TXT_KEY_CIVIC_PER_SPECIALIST").GetCString(), GC.getTechInfo(eTech).getAllSpecialistCommerceArray());
	// MOD - END - Advanced Yield and Commerce Effects

	//	K-Mod. Global commerce modifers
	setCommerceChangeHelp(szBuffer, L"", L"", gDLL->getText("TXT_KEY_CIVIC_IN_ALL_CITIES").GetCString(), GC.getTechInfo(eTech).getCommerceModifierArray(), true);

	//	Adjusting culture, science, etc
	for (int iI = 0; iI < NUM_COMMERCE_TYPES; ++iI)
	{
		buildAdjustString(szBuffer, eTech, iI, true, bPlayerContext);
	}

	//	Enabling trade routes on water...?
	for (int iI = 0; iI < GC.getNumTerrainInfos(); ++iI)
	{
		buildTerrainTradeString(szBuffer, eTech, iI, true, bPlayerContext);
	}

	buildRiverTradeString(szBuffer, eTech, true, bPlayerContext);

	//	Special Buildings
	for (int iI = 0; iI < GC.getNumSpecialBuildingInfos(); ++iI)
	{
		buildSpecialBuildingString(szBuffer, eTech, iI, true, bPlayerContext);
	}

	//	Build farm, mine, etc...
	for (int iI = 0; iI < GC.getNumImprovementInfos(); ++iI)
	{
		buildYieldChangeString(szBuffer, eTech, iI, true, bPlayerContext);
	}

	// MOD - START - Advanced Yield and Commerce Effects
	for (int iI = 0; iI < GC.getNumImprovementInfos(); ++iI)
	{
		buildIrrigatedYieldChangeString(szBuffer, eTech, iI, true, bPlayerContext);
	}
	// MOD - END - Advanced Yield and Commerce Effects

	bFirst = true;

	for (int iI = 0; iI < GC.getNumBonusInfos(); ++iI)
	{
		bFirst = buildBonusRevealString(szBuffer, eTech, iI, bFirst, true, bPlayerContext);
	}

	bFirst = true;

	for (int iI = 0; iI < GC.getNumCivicInfos(); ++iI)
	{
		bFirst = buildCivicRevealString(szBuffer, eTech, iI, bFirst, true, bPlayerContext);
	}

	// MOD - START - Improved Help Context
	// Note: Changes made throughout
	if (!bCivilopediaText)
	{
		bFirst = true;

		// MOD - START - Relation Caching
		for (int iRel = 0; iRel < kTech.getNumRelatedUnitClasses(); iRel++)
		{
			UnitClassTypes eLoopUnitClass = (UnitClassTypes)kTech.getRelatedUnitClasses(iRel);

			if (ePlayer == NO_PLAYER || !(GET_PLAYER(ePlayer).isProductionMaxedUnitClass(eLoopUnitClass)))
			{
				UnitTypes eLoopUnit;

				if (eCivilization != NO_CIVILIZATION)
				{
					eLoopUnit = (UnitTypes)GC.getCivilizationInfo(eCivilization).getCivilizationUnits(eLoopUnitClass);
				}
				else
				{
					eLoopUnit = (UnitTypes)GC.getUnitClassInfo(eLoopUnitClass).getDefaultUnitIndex();
				}

				if (eLoopUnit != NO_UNIT)
				{
					// MOD - START - Advanced Unit Prerequisites
					// Old code deleted
					if (isTechRequiredForUnit(eTech, eLoopUnit))
					{
						if (ePlayer == NO_PLAYER || !(GET_PLAYER(ePlayer).canTrain(eLoopUnit)))
						{
							szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_TECH_CAN_TRAIN").c_str());
							szTempBuffer.Format( SETCOLR L"%s" ENDCOLR , TEXT_COLOR("COLOR_UNIT_TEXT"), GC.getUnitInfo(eLoopUnit).getDescription());
							setListHelp(szBuffer, szFirstBuffer, szTempBuffer, L", ", bFirst);
							bFirst = false;
						}
					}
					// MOD - END - Advanced Unit Prerequisites
				}
			}
		}

		bFirst = true;
		
		for (int iRel = 0; iRel < kTech.getNumRelatedBuildingClasses(); iRel++)
		{
			BuildingClassTypes eLoopBuildingClass = (BuildingClassTypes)kTech.getRelatedBuildingClasses(iRel);

			if (ePlayer == NO_PLAYER || !(GET_PLAYER(ePlayer).isProductionMaxedBuildingClass(eLoopBuildingClass)))
			{
				BuildingTypes eLoopBuilding;

				if (eCivilization != NO_CIVILIZATION)
				{
					eLoopBuilding = (BuildingTypes)GC.getCivilizationInfo(eCivilization).getCivilizationBuildings(eLoopBuildingClass);
				}
				else
				{
					eLoopBuilding = (BuildingTypes)GC.getBuildingClassInfo(eLoopBuildingClass).getDefaultBuildingIndex();
				}

				if (eLoopBuilding != NO_BUILDING)
				{
					if (ePlayer == NO_PLAYER || !(GET_PLAYER(ePlayer).canConstruct(eLoopBuilding, false, true)))
					{
						// MOD - START - Advanced Building Prerequisites (Tech)
						// Old code deleted
						const CvBuildingInfo& kLoopBuilding = GC.getBuildingInfo(eLoopBuilding);
						for (int iPre = 0; iPre < kLoopBuilding.getNumPrereqAndTechs(); iPre++)
						{
							if (kLoopBuilding.getPrereqAndTechs(iPre) == eTech)
							{
								szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_TECH_CAN_CONSTRUCT").c_str());
								szTempBuffer.Format( SETCOLR L"<link=literal>%s</link>" ENDCOLR , TEXT_COLOR("COLOR_BUILDING_TEXT"), GC.getBuildingInfo(eLoopBuilding).getDescription());
								setListHelp(szBuffer, szFirstBuffer, szTempBuffer, L", ", bFirst);
								bFirst = false;
								break;
							}
						}
						// MOD - END - Advanced Building Prerequisites (Tech)
					}
				}
			}
		}
		// MOD - END - Relation Caching

		bFirst = true;

		for (int iI = 0; iI < GC.getNumProjectInfos(); ++iI)
		{
			ProjectTypes eLoopProject = (ProjectTypes)iI;
			if (ePlayer == NO_PLAYER || !(GET_PLAYER(ePlayer).isProductionMaxedProject(eLoopProject)))
			{
				if (ePlayer == NO_PLAYER || !(GET_PLAYER(ePlayer).canCreate(eLoopProject, false, true)))
				{
					if (GC.getProjectInfo(eLoopProject).getTechPrereq() == eTech)
					{
						szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_TECH_CAN_CREATE").c_str());
						szTempBuffer.Format( SETCOLR L"%s" ENDCOLR , TEXT_COLOR("COLOR_PROJECT_TEXT"), GC.getProjectInfo(eLoopProject).getDescription());
						setListHelp(szBuffer, szFirstBuffer, szTempBuffer, L", ", bFirst);
						bFirst = false;
					}
				}
			}
		}
	}
	// MOD - END - Improved Help Context

	bFirst = true;
	for (int iI = 0; iI < GC.getNumProcessInfos(); ++iI)
	{
		bFirst = buildProcessInfoString(szBuffer, eTech, iI, bFirst, true, bPlayerContext);
	}

	bFirst = true;
	for (int iI = 0; iI < GC.getNumReligionInfos(); ++iI)
	{
		if (!bPlayerContext || !(GC.getGameINLINE().isReligionSlotTaken((ReligionTypes)iI)))
		{
			bFirst = buildFoundReligionString(szBuffer, eTech, iI, bFirst, true, bPlayerContext);
		}
	}

	bFirst = true;
	for (int iI = 0; iI < GC.getNumCorporationInfos(); ++iI)
	{
		if (!bPlayerContext || !(GC.getGameINLINE().isCorporationFounded((CorporationTypes)iI)))
		{
			bFirst = buildFoundCorporationString(szBuffer, eTech, iI, bFirst, true, bPlayerContext);
		}
	}

	bFirst = true;
	for (int iI = 0; iI < GC.getNumPromotionInfos(); ++iI)
	{
		bFirst = buildPromotionString(szBuffer, eTech, iI, bFirst, true, bPlayerContext);
	}

	// MOD - START - Congestion
	if (kTech.getInsideCityCongestionThresholdChange() == kTech.getOutsideCityCongestionThresholdChange())
	{
		if (kTech.getInsideCityCongestionThresholdChange() != 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_CONGESTION_THRESHOLD_CHANGE", kTech.getInsideCityCongestionThresholdChange()));
		}
	}
	else
	{
		buildInsideCityCongestionThresholdChangeString(szBuffer, eTech, true, bPlayerContext);
		buildOutsideCityCongestionThresholdChangeString(szBuffer, eTech, true, bPlayerContext);
	}
	// MOD - END - Congestion

	if (bTreeInfo && NO_TECH == eFromTech)
	{
		buildSingleLineTechTreeString(szBuffer, eTech, bPlayerContext);
	}

	if (!CvWString(kTech.getHelp()).empty())
	{
		szBuffer.append(CvWString::format(L"%s%s", NEWLINE, kTech.getHelp()).c_str());
	}

	// MOD - START - Improved Help Context
	// Note: Changes made throughout
	if (!bCivilopediaText)
	{
		// MOD - START - Era Effects
		EraTypes eTechEra = (EraTypes)kTech.getEra();
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_TECH_ERA", GC.getEraInfo(eTechEra).getDescription()));
		// MOD - END - Era Effects

		if (ePlayer == NO_PLAYER)
		{
			szTempBuffer.Format(L"\n%d%c", kTech.getResearchCost(), GC.getCommerceInfo(COMMERCE_RESEARCH).getChar());
			szBuffer.append(szTempBuffer);
		}
		else
		{
			const CvPlayer& kPlayer = GET_PLAYER(ePlayer);
			const CvTeamAI& kTeam = GET_TEAM(kPlayer.getTeam());

			if (kTeam.isHasTech(eTech))
			{
				szTempBuffer.Format(L"\n%d%c", kTeam.getResearchCost(eTech), GC.getCommerceInfo(COMMERCE_RESEARCH).getChar());
				szBuffer.append(szTempBuffer);
			}
			else
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_TECH_NUM_TURNS", kPlayer.getResearchTurnsLeft(eTech, (GC.ctrlKey() || !(GC.shiftKey())))));

				szTempBuffer.Format(L" (%d/%d %c)", kTeam.getResearchProgress(eTech), kTeam.getResearchCost(eTech), GC.getCommerceInfo(COMMERCE_RESEARCH).getChar());
				szBuffer.append(szTempBuffer);

				// MOD - START - Tech Transfer Commerce
				int iTechTransferMod = kPlayer.calculateTotalTechTransferCommerceRateModifier(COMMERCE_RESEARCH, eTech);
				if (iTechTransferMod != 0)
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_YIELD_TECH_TRANSFER", iTechTransferMod, GC.getCommerceInfo(COMMERCE_RESEARCH).getChar()));
				}
				// MOD - END - Tech Transfer Commerce
			}
		
		int iAheadOfTime = kTech.getAheadOfTime();
		if (iAheadOfTime != 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_YIELD_AHEAD_OF_TIME", iAheadOfTime, GC.getCommerceInfo(COMMERCE_RESEARCH).getChar()));
		}
		}
	}
	// MOD - END - Improved Help Context

	if (GC.getGameINLINE().getActivePlayer() != NO_PLAYER)
	{
		if (GET_PLAYER(GC.getGameINLINE().getActivePlayer()).canResearch(eTech))
		{
			// MOD - START - Show only player-specific unit (or default) in Tech Help
			// 04.06.2010 by Mexico
			/*
			for (iI = 0; iI < GC.getNumUnitInfos(); ++iI)
			{
				CvUnitInfo& kUnit = GC.getUnitInfo((UnitTypes)iI);

				if (kUnit.getBaseDiscover() > 0 || kUnit.getDiscoverMultiplier() > 0)
				{
					if (::getDiscoveryTech((UnitTypes)iI, GC.getGameINLINE().getActivePlayer()) == eTech)
					{
						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_TECH_GREAT_PERSON_DISCOVER", kUnit.getTextKeyWide()));
					}
				}
			}
			*/
			// TODO: Cache the discovery techs for each player so that we save on a bunch of loops in places like this
			for (int iI = 0; iI < GC.getNumUnitClassInfos(); ++iI)
			{
				UnitTypes eLoopUnit = (UnitTypes)GC.getCivilizationInfo(GC.getGameINLINE().getActiveCivilizationType()).getCivilizationUnits(iI);
				if (eLoopUnit != NO_UNIT)
				{
					const CvUnitInfo& kUnit = GC.getUnitInfo(eLoopUnit);

					if (kUnit.getBaseDiscover() > 0 || kUnit.getDiscoverMultiplier() > 0)
					{
						if (::getDiscoveryTech((UnitTypes)iI, GC.getGameINLINE().getActivePlayer()) == eTech)
						{
							szBuffer.append(NEWLINE);
							szBuffer.append(gDLL->getText("TXT_KEY_TECH_GREAT_PERSON_DISCOVER", kUnit.getTextKeyWide()));
						}
					}
				}
			}
			// MOD - END - Show only player-specific unit (or default) in Tech Help

			if (GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getCurrentEra() < kTech.getEra())
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_TECH_ERA_ADVANCE", GC.getEraInfo((EraTypes)kTech.getEra()).getTextKeyWide()));
			}
		}
	}

	if (bStrategyText)
	{
		if (!CvWString(kTech.getStrategy()).empty())
		{
			if ((GC.getGameINLINE().getActivePlayer() == NO_PLAYER) || GET_PLAYER(GC.getGameINLINE().getActivePlayer()).isOption(PLAYEROPTION_ADVISOR_HELP))
			{
				// MOD - START - Disabled Sid's Tips
				/*
				szBuffer.append(SEPARATOR);
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_SIDS_TIPS"));
				szBuffer.append(L'\"');
				szBuffer.append(kTech.getStrategy());
				szBuffer.append(L'\"');
				*/
				// MOD - END - Disabled Sid's Tips
			}
		}
	}
}


void CvGameTextMgr::setBasicUnitHelp(CvWStringBuffer &szBuffer, UnitTypes eUnit, bool bCivilopediaText)
{
	PROFILE_FUNC();

	CvWString szTempBuffer;
	bool bFirst;
	int iCount;

	if (NO_UNIT == eUnit)
	{
		return;
	}

	// MOD - START - Relation Caching
	PlayerTypes ePlayer = GC.getGameINLINE().getActivePlayer(); // TODO: Receive this as an argument to the function?
	// MOD - END - Relation Caching

	const CvUnitInfo& kUnit = GC.getUnitInfo(eUnit);
	const CvUnitClassInfo& kUnitClass = GC.getUnitClassInfo((UnitClassTypes)kUnit.getUnitClassType());

	if (!bCivilopediaText)
	{
		szBuffer.append(NEWLINE);

		if (kUnit.getDomainType() == DOMAIN_AIR)
		{
			if (kUnit.getAirCombat() > 0)
			{
				szTempBuffer.Format(L"%d%c, ", kUnit.getAirCombat(), gDLL->getSymbolID(STRENGTH_CHAR));
				szBuffer.append(szTempBuffer);
			}
		}
		else
		{
			if (kUnit.getCombat() > 0)
			{
				szTempBuffer.Format(L"%d%c, ", kUnit.getCombat(), gDLL->getSymbolID(STRENGTH_CHAR));
				szBuffer.append(szTempBuffer);
			}
		}

		szTempBuffer.Format(L"%d%c", kUnit.getMoves(), gDLL->getSymbolID(MOVES_CHAR));
		szBuffer.append(szTempBuffer);

		if (kUnit.getAirRange() > 0)
		{
			szBuffer.append(L", ");
			szBuffer.append(gDLL->getText("TXT_KEY_UNIT_AIR_RANGE", kUnit.getAirRange()));
		}
	}

	if (kUnit.isGoldenAge())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_GOLDEN_AGE"));
	}

	if (kUnit.getLeaderExperience() > 0)
	{
		if (0 == GC.getDefineINT("WARLORD_EXTRA_EXPERIENCE_PER_UNIT_PERCENT"))
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_UNIT_LEADER", kUnit.getLeaderExperience()));
		}
		else
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_UNIT_LEADER_EXPERIENCE", kUnit.getLeaderExperience()));
		}
	}

	if (NO_PROMOTION != kUnit.getLeaderPromotion())
	{
		szBuffer.append(CvWString::format(L"%s%c%s", NEWLINE, gDLL->getSymbolID(BULLET_CHAR), gDLL->getText("TXT_KEY_PROMOTION_WHEN_LEADING").GetCString()));
		parsePromotionHelp(szBuffer, (PromotionTypes)kUnit.getLeaderPromotion(), L"\n   ");
	}

	if ((kUnit.getBaseDiscover() > 0) || (kUnit.getDiscoverMultiplier() > 0))
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_DISCOVER_TECH"));
	}

	if ((kUnit.getBaseHurry() > 0) || (kUnit.getHurryMultiplier() > 0))
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_HURRY_PRODUCTION"));
	}

	if ((kUnit.getBaseTrade() > 0) || (kUnit.getTradeMultiplier() > 0))
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_TRADE_MISSION"));
	}

	if (kUnit.getGreatWorkCulture() > 0)
	{
		int iCulture = kUnit.getGreatWorkCulture();
		if (NO_GAMESPEED != GC.getGameINLINE().getGameSpeedType())
		{
			iCulture *= GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getUnitGreatWorkPercent();
			iCulture /= 100;
		}

		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_GREAT_WORK", iCulture));
	}

	if (kUnit.getEspionagePoints() > 0)
	{
		int iEspionage = kUnit.getEspionagePoints();
		if (NO_GAMESPEED != GC.getGameINLINE().getGameSpeedType())
		{
			iEspionage *= GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getUnitGreatWorkPercent();
			iEspionage /= 100;
		}

		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_ESPIONAGE_MISSION", iEspionage));
	}

	bFirst = true;

	// MOD - START - Relation Caching
	for (int iRel = 0; iRel < kUnit.getNumRelatedReligions(); iRel++)
	{
		ReligionTypes eLoopReligion = (ReligionTypes)kUnit.getRelatedReligions(iRel);

		if (kUnit.getReligionSpreads(eLoopReligion) > 0)
		{
			szTempBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_UNIT_CAN_SPREAD").c_str());
			CvWString szReligion;
			szReligion.Format(L"<link=literal>%s</link>", GC.getReligionInfo(eLoopReligion).getDescription());
			setListHelp(szBuffer, szTempBuffer, szReligion, L", ", bFirst);
			bFirst = false;
		}
	}
	// MOD - END - Relation Caching

	// MOD - START - Inquisition
	if (kUnit.isRemoveNonStateReligion())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_CAN_REMOVE_NON_STATE_RELIGIONS"));
	}
	// MOD - END - Inquisition

	bFirst = true;

	for (int iI = 0; iI < GC.getNumCorporationInfos(); ++iI)
	{
		if (kUnit.getCorporationSpreads(iI) > 0)
		{
			szTempBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_UNIT_CAN_EXPAND").c_str());
			CvWString szCorporation;
			szCorporation.Format(L"<link=literal>%s</link>", GC.getCorporationInfo((CorporationTypes) iI).getDescription());
			setListHelp(szBuffer, szTempBuffer, szCorporation, L", ", bFirst);
			bFirst = false;
		}
	}

	bFirst = true;

	for (int iI = 0; iI < GC.getNumSpecialistInfos(); ++iI)
	{
		if (kUnit.getGreatPeoples(iI))
		{
			szTempBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_UNIT_CAN_JOIN").c_str());
			CvWString szSpecialistLink = CvWString::format(L"<link=literal>%s</link>", GC.getSpecialistInfo((SpecialistTypes) iI).getDescription());
			setListHelp(szBuffer, szTempBuffer, szSpecialistLink.GetCString(), L", ", bFirst);
			bFirst = false;
		}
	}

	bFirst = true;

	// MOD - START - Relation Caching
	for (int iRel = 0; iRel < kUnit.getNumRelatedBuildingClasses(); iRel++)
	{
		BuildingClassTypes eLoopBuildingClass = (BuildingClassTypes)kUnit.getRelatedBuildingClasses(iRel);
		BuildingTypes eLoopBuilding;

		if (ePlayer != NO_PLAYER)
		{
			eLoopBuilding = ((BuildingTypes)(GC.getCivilizationInfo(GET_PLAYER(ePlayer).getCivilizationType()).getCivilizationBuildings(eLoopBuildingClass)));
		}
		else
		{
			eLoopBuilding = (BuildingTypes)GC.getBuildingClassInfo(eLoopBuildingClass).getDefaultBuildingIndex();
		}

		if (eLoopBuilding != NO_BUILDING)
		{
			if (kUnit.getBuildings(eLoopBuilding) || kUnit.getForceBuildings(eLoopBuilding))
			{
				szTempBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_UNIT_CAN_CONSTRUCT").c_str());
				CvWString szBuildingLink = CvWString::format(L"<link=literal>%s</link>", GC.getBuildingInfo(eLoopBuilding).getDescription());
				setListHelp(szBuffer, szTempBuffer, szBuildingLink.GetCString(), L", ", bFirst);
				bFirst = false;
			}
		}
	}
	// MOD - END - Relation Caching


	// MOD - START - Aid
	szTempBuffer.clear();
	bFirst = true;
	for (int iI = 0; iI < kUnitClass.getNumAidTypes(); iI++)
	{
		AidTypes eAidType = (AidTypes)kUnitClass.getAidTypes(iI);

		if (bFirst)
		{
			bFirst = false;
		}
		else
		{
			szTempBuffer += L", ";
		}

		szTempBuffer += CvWString::format(L"<link=literal>%s</link>", GC.getAidInfo(eAidType).getDescription());
	}

	if (!bFirst)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_PROVIDES_AID", szTempBuffer.GetCString()));
	}
	// MOD - END - Aid

	// MOD - START - Detriments
	szTempBuffer.clear();
	bFirst = true;
	for (int iI = 0; iI < kUnitClass.getNumDetrimentTypes(); iI++)
	{
		DetrimentTypes eDetrimentType = (DetrimentTypes)kUnitClass.getDetrimentTypes(iI);

		if (bFirst)
		{
			bFirst = false;
		}
		else
		{
			szTempBuffer += L", ";
		}

		szTempBuffer += CvWString::format(L"<link=literal>%s</link>", GC.getDetrimentInfo(eDetrimentType).getDescription());
	}

	if (!bFirst)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_INFLICTS_DETRIMENTS", szTempBuffer.GetCString()));
	}
	// MOD - END - Detriments

	if (kUnit.getCargoSpace() > 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_CARGO_SPACE", kUnit.getCargoSpace()));

		if (kUnit.getSpecialCargo() != NO_SPECIALUNIT)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_UNIT_CARRIES", GC.getSpecialUnitInfo((SpecialUnitTypes) kUnit.getSpecialCargo()).getTextKeyWide()));
		}
	}

	szTempBuffer.Format(L"%s%s ", NEWLINE, gDLL->getText("TXT_KEY_UNIT_CANNOT_ENTER").GetCString());

	bFirst = true;

	for (int iI = 0; iI < GC.getNumTerrainInfos(); ++iI)
	{
		if (kUnit.getTerrainImpassable(iI))
		{
			CvWString szTerrain;
			TechTypes eTech = (TechTypes)kUnit.getTerrainPassableTech(iI);
			if (NO_TECH == eTech)
			{
				szTerrain.Format(L"<link=literal>%s</link>", GC.getTerrainInfo((TerrainTypes)iI).getDescription());
			}
			else
			{
				szTerrain = gDLL->getText("TXT_KEY_TERRAIN_UNTIL_TECH", GC.getTerrainInfo((TerrainTypes)iI).getTextKeyWide(), GC.getTechInfo(eTech).getTextKeyWide());
			}
			setListHelp(szBuffer, szTempBuffer, szTerrain, L", ", bFirst);
			bFirst = false;
		}
	}

	for (int iI = 0; iI < GC.getNumFeatureInfos(); ++iI)
	{
		if (kUnit.getFeatureImpassable(iI))
		{
			CvWString szFeature;
			TechTypes eTech = (TechTypes)kUnit.getTerrainPassableTech(iI);
			if (NO_TECH == eTech)
			{
				szFeature.Format(L"<link=literal>%s</link>", GC.getFeatureInfo((FeatureTypes)iI).getDescription());
			}
			else
			{
				szFeature = gDLL->getText("TXT_KEY_TERRAIN_UNTIL_TECH", GC.getFeatureInfo((FeatureTypes)iI).getTextKeyWide(), GC.getTechInfo(eTech).getTextKeyWide());
			}
			setListHelp(szBuffer, szTempBuffer, szFeature, L", ", bFirst);
			bFirst = false;
		}
	}

	// MOD - START - Feature Damage Immunity
	szTempBuffer.Format(L"%s%s ", NEWLINE, gDLL->getText("TXT_KEY_MISC_FEATURE_DAMAGE_IMMUNITY").GetCString());

	bFirst = true;
	for (int iI = 0; iI < GC.getNumFeatureInfos(); ++iI)
	{
		if (kUnit.getFeatureDamageImmunity(iI))
		{
			const CvFeatureInfo& kLoopFeature = GC.getFeatureInfo((FeatureTypes)iI);

			CvWString szFeature;
			szFeature.Format(L"<link=%s>%s</link>", CvWString(kLoopFeature.getType()).GetCString(), kLoopFeature.getDescription(1));

			setListHelp(szBuffer, szTempBuffer, szFeature, L", ", bFirst);

			bFirst = false;
		}
	}
	// MOD - END - Feature Damage Immunity

	if (kUnit.isInvisible())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_INVISIBLE_ALL"));
	}
	else if (kUnit.getInvisibleType() != NO_INVISIBLE)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_INVISIBLE_MOST"));
	}

	for (int iI = 0; iI < kUnit.getNumSeeInvisibleTypes(); ++iI)
	{
		if (bCivilopediaText || (kUnit.getSeeInvisibleType(iI) != kUnit.getInvisibleType()))
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_UNIT_SEE_INVISIBLE", GC.getInvisibleInfo((InvisibleTypes) kUnit.getSeeInvisibleType(iI)).getTextKeyWide()));
		}
	}

	if (kUnit.isCanMoveImpassable())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_CAN_MOVE_IMPASSABLE"));
	}


	if (kUnit.isNoBadGoodies())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_NO_BAD_GOODIES"));
	}

	if (kUnit.isHiddenNationality())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_HIDDEN_NATIONALITY"));
	}

	if (kUnit.isAlwaysHostile())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_ALWAYS_HOSTILE"));
	}
	
	if (kUnit.isStrategic())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_STRATEGIC"));
	}	

	if (kUnit.isOnlyDefensive())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_ONLY_DEFENSIVE"));
	}

	if (kUnit.isNoCapture())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_CANNOT_CAPTURE"));
	}

	if (kUnit.isRivalTerritory())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_EXPLORE_RIVAL"));
	}

	if (kUnit.isFound())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_FOUND_CITY"));
	}

	iCount = 0;

	// MOD - START - Relation Caching
	for (int iRel = 0; iRel < kUnit.getNumRelatedBuilds(); iRel++)
	{
		BuildTypes eLoopBuild = (BuildTypes)kUnit.getRelatedBuilds(iRel);
		if (kUnit.getBuilds(eLoopBuild))
		{
			iCount++;
		}
	}

	if (iCount > ((GC.getNumBuildInfos() * 3) / 4))
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_IMPROVE_PLOTS"));
	}
	else
	{
		bFirst = true;
		
		for (int iRel = 0; iRel < kUnit.getNumRelatedBuilds(); iRel++)
		{
			BuildTypes eLoopBuild = (BuildTypes)kUnit.getRelatedBuilds(iRel);
			if (kUnit.getBuilds(eLoopBuild))
			{
				// K-Mod, 4/jan/11, karadoc: removed the space on the next line. (ie. from "%s%s ", to "%s%s")
				//szTempBuffer.Format(L"%s%s ", NEWLINE, gDLL->getText("TXT_KEY_UNIT_CAN").c_str());
				szTempBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_UNIT_CAN").c_str());
				setListHelp(szBuffer, szTempBuffer, GC.getBuildInfo(eLoopBuild).getDescription(), L", ", bFirst);
				bFirst = false;
			}
		}
	}
	// MOD - END - Relation Caching

	// MOD - START - Slavery
	if (kUnit.getBuildKillChance() > 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_BUILD_KILL", kUnit.getBuildKillChance()));
	}
	// MOD - END - Slavery
	
	if (kUnit.getNukeRange() != -1)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_CAN_NUKE"));
	}

	if (kUnit.isCounterSpy())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_EXPOSE_SPIES"));
	}

	if ((kUnit.getFirstStrikes() + kUnit.getChanceFirstStrikes()) > 0)
	{
		if (kUnit.getChanceFirstStrikes() == 0)
		{
			if (kUnit.getFirstStrikes() == 1)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_UNIT_ONE_FIRST_STRIKE"));
			}
			else
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_UNIT_NUM_FIRST_STRIKES", kUnit.getFirstStrikes()));
			}
		}
		else
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_UNIT_FIRST_STRIKE_CHANCES", kUnit.getFirstStrikes(), kUnit.getFirstStrikes() + kUnit.getChanceFirstStrikes()));
		}
	}

	if (kUnit.isFirstStrikeImmune())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_IMMUNE_FIRST_STRIKES"));
	}

	if (kUnit.isNoDefensiveBonus())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_NO_DEFENSE_BONUSES"));
	}

	if (kUnit.isFlatMovementCost())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_FLAT_MOVEMENT"));
	}

	if (kUnit.isIgnoreTerrainCost())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_IGNORE_TERRAIN"));
	}

	// MOD - START - Untransportable Units
	if (kUnit.isNoTransport())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_NO_TRANSPORT"));
	}
	// MOD - END - Untransportable Units

	if (kUnit.getInterceptionProbability() > 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_INTERCEPT_AIRCRAFT", kUnit.getInterceptionProbability()));
	}

	if (kUnit.getEvasionProbability() > 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_EVADE_INTERCEPTION", kUnit.getEvasionProbability()));
	}

	if (kUnit.getWithdrawalProbability() > 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_WITHDRAWL_PROBABILITY", kUnit.getWithdrawalProbability()));
	}

	if (kUnit.getCombatLimit() < GC.getMAX_HIT_POINTS() && kUnit.getCombat() > 0 && !kUnit.isOnlyDefensive())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_COMBAT_LIMIT", (100 * kUnit.getCombatLimit()) / GC.getMAX_HIT_POINTS()));
	}

	if (kUnit.getCollateralDamage() > 0)
	{
		szBuffer.append(NEWLINE);
		int iCollateralPercentage = 100 * kUnit.getCollateralDamageLimit() / GC.getMAX_HIT_POINTS();
		int iCollateralUnits = kUnit.getCollateralDamageMaxUnits();
		if (iCollateralUnits == 1)
			szBuffer.append(gDLL->getText("TXT_KEY_UNIT_COLLATERAL_DAMAGE_SINGLE", iCollateralPercentage));
		else
			szBuffer.append(gDLL->getText("TXT_KEY_UNIT_COLLATERAL_DAMAGE", iCollateralPercentage, iCollateralUnits));
	}

	// MOD - START - Relation Caching
	for (int iRel = 0; iRel < kUnit.getNumRelatedUnitCombats(); iRel++)
	{
		UnitCombatTypes eLoopUnitCombat = (UnitCombatTypes)kUnit.getRelatedUnitCombats(iRel);

		if (kUnit.getUnitCombatCollateralImmune(eLoopUnitCombat))
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_UNIT_COLLATERAL_IMMUNE", GC.getUnitCombatInfo(eLoopUnitCombat).getTextKeyWide()));
		}
	}
	// MOD - END - Relation Caching

	if (kUnit.getCityAttackModifier() == kUnit.getCityDefenseModifier())
	{
		if (kUnit.getCityAttackModifier() != 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_UNIT_CITY_STRENGTH_MOD", kUnit.getCityAttackModifier()));
		}
	}
	else
	{
		if (kUnit.getCityAttackModifier() != 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_UNIT_CITY_ATTACK_MOD", kUnit.getCityAttackModifier()));
		}

		if (kUnit.getCityDefenseModifier() != 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_UNIT_CITY_DEFENSE_MOD", kUnit.getCityDefenseModifier()));
		}
	}

	if (kUnit.getAnimalCombatModifier() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_ANIMAL_COMBAT_MOD", kUnit.getAnimalCombatModifier()));
	}

	if (kUnit.getDropRange() > 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_PARADROP_RANGE", kUnit.getDropRange()));
	}

	if (kUnit.getHillsDefenseModifier() == kUnit.getHillsAttackModifier())
	{
		if (kUnit.getHillsAttackModifier() != 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_UNIT_HILLS_STRENGTH", kUnit.getHillsAttackModifier()));
		}
	}
	else
	{
		if (kUnit.getHillsAttackModifier() != 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_UNIT_HILLS_ATTACK", kUnit.getHillsAttackModifier()));
		}

		if (kUnit.getHillsDefenseModifier() != 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_UNIT_HILLS_DEFENSE", kUnit.getHillsDefenseModifier()));
		}
	}

	// MOD - START - Relation Caching
	for (int iRel = 0; iRel < kUnit.getNumRelatedDomains(); iRel++)
	{
		DomainTypes eLoopDomain = (DomainTypes)kUnit.getRelatedDomains(iRel);

		if (kUnit.getDomainModifier(eLoopDomain) != 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_UNIT_MOD_VS_TYPE_NO_LINK", kUnit.getDomainModifier(eLoopDomain), GC.getDomainInfo(eLoopDomain).getTextKeyWide()));
		}
	}

	for (int iRel = 0; iRel < kUnit.getNumRelatedTerrains(); iRel++)
	{
		TerrainTypes eLoopTerrain = (TerrainTypes)kUnit.getRelatedTerrains(iRel);

		if (kUnit.getTerrainDefenseModifier(eLoopTerrain) != 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_UNIT_DEFENSE", kUnit.getTerrainDefenseModifier(eLoopTerrain), GC.getTerrainInfo(eLoopTerrain).getTextKeyWide()));
		}

		if (kUnit.getTerrainAttackModifier(eLoopTerrain) != 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_UNIT_ATTACK", kUnit.getTerrainAttackModifier(eLoopTerrain), GC.getTerrainInfo(eLoopTerrain).getTextKeyWide()));
		}
	}

	for (int iRel = 0; iRel < kUnit.getNumRelatedFeatures(); iRel++)
	{
		FeatureTypes eLoopFeature = (FeatureTypes)kUnit.getRelatedFeatures(iRel);

		if (kUnit.getFeatureDefenseModifier(eLoopFeature) != 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_UNIT_DEFENSE", kUnit.getFeatureDefenseModifier(eLoopFeature), GC.getFeatureInfo(eLoopFeature).getTextKeyWide()));
		}

		if (kUnit.getFeatureAttackModifier(eLoopFeature) != 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_UNIT_ATTACK", kUnit.getFeatureAttackModifier(eLoopFeature), GC.getFeatureInfo(eLoopFeature).getTextKeyWide()));
		}
	}

	for (int iRel = 0; iRel < kUnit.getNumRelatedUnitClasses(); iRel++)
	{
		UnitClassTypes eLoopUnitClass = (UnitClassTypes)kUnit.getRelatedUnitClasses(iRel);

		// MOD - START - Improved Civilopedia
		if (kUnit.getUnitClassAttackModifier(eLoopUnitClass) == kUnit.getUnitClassDefenseModifier(eLoopUnitClass))
		{
			if (kUnit.getUnitClassAttackModifier(eLoopUnitClass) != 0)
			{
				const CvUnitClassInfo& kLoopUnitClass = GC.getUnitClassInfo(eLoopUnitClass);
				CvWString szLinkType = (kLoopUnitClass.getRepresentativeUnit() != NO_UNIT) ? GC.getUnitInfo((UnitTypes)kLoopUnitClass.getRepresentativeUnit()).getType() : kLoopUnitClass.getType();
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_UNIT_MOD_VS_TYPE", kUnit.getUnitClassAttackModifier(eLoopUnitClass), szLinkType.GetCString(), kLoopUnitClass.getTextKeyWide()));
			}
		}
		else
		{
			const CvUnitClassInfo& kLoopUnitClass = GC.getUnitClassInfo(eLoopUnitClass);
			CvWString szLinkType = (kLoopUnitClass.getRepresentativeUnit() != NO_UNIT) ? GC.getUnitInfo((UnitTypes)kLoopUnitClass.getRepresentativeUnit()).getType() : kLoopUnitClass.getType();

			if (kUnit.getUnitClassAttackModifier(eLoopUnitClass) != 0)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_UNIT_ATTACK_MOD_VS_CLASS", kUnit.getUnitClassAttackModifier(eLoopUnitClass), szLinkType.GetCString(), kLoopUnitClass.getTextKeyWide()));
			}

			if (kUnit.getUnitClassDefenseModifier(eLoopUnitClass) != 0)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_UNIT_DEFENSE_MOD_VS_CLASS", kUnit.getUnitClassDefenseModifier(eLoopUnitClass), szLinkType.GetCString(), kLoopUnitClass.getTextKeyWide()));
			}
		}
		// MOD - END - Improved Civilopedia
	}

	for (int iRel = 0; iRel < kUnit.getNumRelatedUnitCombats(); iRel++)
	{
		UnitCombatTypes eLoopUnitCombat = (UnitCombatTypes)kUnit.getRelatedUnitCombats(iRel);

		// MOD - START - Improved Civilopedia
		// MOD - START - Unit Combat Attack Defense Mod
		if (kUnit.getUnitCombatAttackModifier(eLoopUnitCombat) == kUnit.getUnitCombatDefenseModifier(eLoopUnitCombat))
		{
			if (kUnit.getUnitCombatAttackModifier(eLoopUnitCombat) != 0)
			{
				const CvInfoBase& kLoopUnitCombat = GC.getUnitCombatInfo(eLoopUnitCombat);
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_UNIT_MOD_VS_TYPE", kUnit.getUnitCombatAttackModifier(eLoopUnitCombat), CvWString(kLoopUnitCombat.getType()).GetCString(), kLoopUnitCombat.getTextKeyWide()));
			}
		}
		else
		{
			if (kUnit.getUnitCombatAttackModifier(eLoopUnitCombat) != 0)
			{
				const CvInfoBase& kLoopUnitCombat = GC.getUnitCombatInfo(eLoopUnitCombat);
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_UNIT_ATTACK_MOD_VS_COMBAT", kUnit.getUnitCombatAttackModifier(eLoopUnitCombat), CvWString(kLoopUnitCombat.getType()).GetCString(), kLoopUnitCombat.getTextKeyWide()));
			}

			if (kUnit.getUnitCombatDefenseModifier(eLoopUnitCombat) != 0)
			{
				const CvInfoBase& kLoopUnitCombat = GC.getUnitCombatInfo(eLoopUnitCombat);
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_UNIT_DEFENSE_MOD_VS_COMBAT", kUnit.getUnitCombatDefenseModifier(eLoopUnitCombat), CvWString(kLoopUnitCombat.getType()).GetCString(), kLoopUnitCombat.getTextKeyWide()));
			}
		}
		// MOD - END - Unit Combat Attack Defense Mod

		if (kUnit.getUnitCombatModifier(eLoopUnitCombat) != 0)
		{
			const CvInfoBase& kLoopUnitCombat = GC.getUnitCombatInfo(eLoopUnitCombat);
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_UNIT_MOD_VS_TYPE", kUnit.getUnitCombatModifier(eLoopUnitCombat), CvWString(kLoopUnitCombat.getType()).GetCString(), kLoopUnitCombat.getTextKeyWide()));
		}
		// MOD - END - Improved Civilopedia
	}
	// MOD - END - Relation Caching

	szTempBuffer.clear();
	bFirst = true;

	// MOD - START - Relation Caching
	for (int iRel = 0; iRel < kUnit.getNumRelatedUnitClasses(); iRel++)
	{
		UnitClassTypes eLoopUnitClass = (UnitClassTypes)kUnit.getRelatedUnitClasses(iRel);

		if (kUnit.getTargetUnitClass(eLoopUnitClass))
		{
			if (bFirst)
			{
				bFirst = false;
			}
			else
			{
				szTempBuffer += L", ";
			}

			szTempBuffer += CvWString::format(L"<link=literal>%s</link>", GC.getUnitClassInfo(eLoopUnitClass).getDescription());
		}
	}
	// MOD - END - Relation Caching

	if (!bFirst)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_TARGETS_UNIT_FIRST", szTempBuffer.GetCString()));
	}

	szTempBuffer.clear();
	bFirst = true;

	// MOD - START - Relation Caching
	for (int iRel = 0; iRel < kUnit.getNumRelatedUnitClasses(); iRel++)
	{
		UnitClassTypes eLoopUnitClass = (UnitClassTypes)kUnit.getRelatedUnitClasses(iRel);

		if (kUnit.getDefenderUnitClass(eLoopUnitClass))
		{
			if (bFirst)
			{
				bFirst = false;
			}
			else
			{
				szTempBuffer += L", ";
			}

			szTempBuffer += CvWString::format(L"<link=literal>%s</link>", GC.getUnitClassInfo(eLoopUnitClass).getDescription());
		}
	}
	// MOD - END - Relation Caching

	if (!bFirst)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_DEFENDS_UNIT_FIRST", szTempBuffer.GetCString()));
	}

	szTempBuffer.clear();
	bFirst = true;
	// MOD - START - Relation Caching
	for (int iRel = 0; iRel < kUnit.getNumRelatedUnitCombats(); iRel++)
	{
		UnitCombatTypes eLoopUnitCombat = (UnitCombatTypes)kUnit.getRelatedUnitCombats(iRel);

		if (kUnit.getTargetUnitCombat(eLoopUnitCombat))
		{
			if (bFirst)
			{
				bFirst = false;
			}
			else
			{
				szTempBuffer += L", ";
			}

			szTempBuffer += CvWString::format(L"<link=literal>%s</link>", GC.getUnitCombatInfo(eLoopUnitCombat).getDescription());
		}
	}
	// MOD - END - Relation Caching

	if (!bFirst)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_TARGETS_UNIT_FIRST", szTempBuffer.GetCString()));
	}

	szTempBuffer.clear();
	bFirst = true;
	// MOD - START - Relation Caching
	for (int iRel = 0; iRel < kUnit.getNumRelatedUnitCombats(); iRel++)
	{
		UnitCombatTypes eLoopUnitCombat = (UnitCombatTypes)kUnit.getRelatedUnitCombats(iRel);

		if (kUnit.getDefenderUnitCombat(eLoopUnitCombat))
		{
			if (bFirst)
			{
				bFirst = false;
			}
			else
			{
				szTempBuffer += L", ";
			}

			szTempBuffer += CvWString::format(L"<link=literal>%s</link>", GC.getUnitCombatInfo(eLoopUnitCombat).getDescription());
		}
	}
	// MOD - END - Relation Caching

	if (!bFirst)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_DEFENDS_UNIT_FIRST", szTempBuffer.GetCString()));
	}

	szTempBuffer.clear();
	bFirst = true;
	// MOD - START - Relation Caching
	for (int iRel = 0; iRel < kUnit.getNumRelatedUnitClasses(); iRel++)
	{
		UnitClassTypes eLoopUnitClass = (UnitClassTypes)kUnit.getRelatedUnitClasses(iRel);

		if (kUnit.getFlankingStrikeUnitClass(eLoopUnitClass) > 0)
		{
			if (bFirst)
			{
				bFirst = false;
			}
			else
			{
				szTempBuffer += L", ";
			}

			szTempBuffer += CvWString::format(L"<link=literal>%s</link>", GC.getUnitClassInfo(eLoopUnitClass).getDescription());
		}
	}
	// MOD - END - Relation Caching

	if (!bFirst)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_FLANKING_STRIKES", szTempBuffer.GetCString()));
	}

	if (kUnit.getBombRate() > 0)
	{
		if (kUnit.isStrategic())
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_UNIT_BOMB_STRATEGIC_RATE", ((kUnit.getBombRate() * 100) / GC.getMAX_CITY_DEFENSE_DAMAGE())));
		}
		else
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_UNIT_BOMB_RATE", ((kUnit.getBombRate() * 100) / GC.getMAX_CITY_DEFENSE_DAMAGE())));
		}		
	}

	if (kUnit.getBombardRate() > 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_BOMBARD_RATE", ((kUnit.getBombardRate() * 100) / GC.getMAX_CITY_DEFENSE_DAMAGE())));
	}

	bFirst = true;
	
/************************************************************************************************/
/* DCM									   START				04/19/09            Johny Smith */
/************************************************************************************************/


	// Dale - FE: Fighters START
	if (GC.getUnitInfo(eUnit).getDCMFighterEngage())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_IS_FIGHTER_ENGAGE"));
	}
	// Dale - FE: Fighters END
/************************************************************************************************/
/* DCM                                     END                                                  */
/************************************************************************************************/	

	// MOD - START - Relation Caching
	for (int iRel = 0; iRel < kUnit.getNumRelatedPromotions(); iRel++)
	{
		PromotionTypes eLoopPromotion = (PromotionTypes)kUnit.getRelatedPromotions(iRel);

		if (kUnit.getFreePromotions(eLoopPromotion))
		{
			szTempBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_UNIT_STARTS_WITH").c_str());
			setListHelp(szBuffer, szTempBuffer, CvWString::format(L"<link=literal>%s</link>", GC.getPromotionInfo(eLoopPromotion).getDescription()), L", ", bFirst);
			bFirst = false;
		}
	}
	// MOD - END - Relation Caching

	if (kUnit.getExtraCost() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_EXTRA_COST", kUnit.getExtraCost()));
	}

	if (bCivilopediaText)
	{
		// Trait
		for (int i = 0; i < GC.getNumTraitInfos(); ++i)
		{
			if (kUnit.getProductionTraits((TraitTypes)i) != 0)
			{
				if (kUnit.getProductionTraits((TraitTypes)i) == 100)
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_DOUBLE_SPEED_TRAIT", GC.getTraitInfo((TraitTypes)i).getTextKeyWide()));
				}
				else
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_PRODUCTION_MODIFIER_TRAIT", kUnit.getProductionTraits((TraitTypes)i), GC.getTraitInfo((TraitTypes)i).getTextKeyWide()));
				}
			}
		}
	}

	// MOD - START - Unit Obsolete Tech (Dom Pedro II)
	if (((TechTypes) kUnit.getObsoleteTech()) != NO_TECH)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_OBSOLETE_WITH", GC.getTechInfo((TechTypes) kUnit.getObsoleteTech()).getTextKeyWide()));
	}
	// MOD - END - Unit Obsolete Tech (Dom Pedro II)

	if (!CvWString(kUnit.getHelp()).empty())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(kUnit.getHelp());
	}
}


void CvGameTextMgr::setUnitHelp(CvWStringBuffer &szBuffer, UnitTypes eUnit, bool bCivilopediaText, bool bStrategyText, bool bTechChooserText, CvCity* pCity)
{
	PROFILE_FUNC();

	CvWString szTempBuffer;
	PlayerTypes ePlayer;
	bool bFirst;
	int iProduction;
	int iI;

	if (NO_UNIT == eUnit)
	{
		return;
	}

	const CvUnitInfo& kUnit = GC.getUnitInfo(eUnit);
	UnitClassTypes eUnitClass = (UnitClassTypes)kUnit.getUnitClassType();
	const CvUnitClassInfo& kUnitClass = GC.getUnitClassInfo(eUnitClass);

	if (pCity != NULL)
	{
		ePlayer = pCity->getOwnerINLINE();
	}
	else
	{
		ePlayer = GC.getGameINLINE().getActivePlayer();
	}

	if (!bCivilopediaText)
	{
		szTempBuffer.Format(SETCOLR L"%s" ENDCOLR , TEXT_COLOR("COLOR_UNIT_TEXT"), kUnit.getDescription());
		szBuffer.append(szTempBuffer);

		if (kUnit.getUnitCombatType() != NO_UNITCOMBAT)
		{
			szTempBuffer.Format(L" (%s)", GC.getUnitCombatInfo((UnitCombatTypes) kUnit.getUnitCombatType()).getDescription());
			szBuffer.append(szTempBuffer);
		}
	}

	// MOD - START - Unique Units
	if (kUnit.isUniqueUnit())
	{
		for (iI  = 0; iI < GC.getNumCivilizationInfos(); ++iI)
		{
			UnitTypes eUniqueUnit = (UnitTypes)GC.getCivilizationInfo((CivilizationTypes)iI).getCivilizationUnits(eUnitClass);
			if (eUniqueUnit == eUnit)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_UNIQUE_UNIT", GC.getCivilizationInfo((CivilizationTypes)iI).getTextKeyWide()));
			}
		}
	}

	UnitTypes eDefaultUnit = (UnitTypes)GC.getUnitClassInfo(eUnitClass).getDefaultUnitIndex();
	if (NO_UNIT != eDefaultUnit && eDefaultUnit != eUnit)
	{
		if (!kUnit.isUniqueUnit())
		{
			for (iI  = 0; iI < GC.getNumCivilizationInfos(); ++iI)
			{
				UnitTypes eUniqueUnit = (UnitTypes)GC.getCivilizationInfo((CivilizationTypes)iI).getCivilizationUnits(eUnitClass);
				if (eUniqueUnit == eUnit)
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_DISTINCTIVE_UNIT", GC.getCivilizationInfo((CivilizationTypes)iI).getTextKeyWide()));
				}
			}
		}

		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_REPLACES_UNIT", GC.getUnitInfo(eDefaultUnit).getTextKeyWide()));
	}
	// MOD - END - Unique Units

	if (isWorldUnitClass(eUnitClass))
	{
		if (pCity == NULL)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_UNIT_WORLD_UNIT_ALLOWED", GC.getUnitClassInfo(eUnitClass).getMaxGlobalInstances()));
		}
		else
		{
			//szBuffer.append(gDLL->getText("TXT_KEY_UNIT_WORLD_UNIT_LEFT", (GC.getUnitClassInfo(eUnitClass).getMaxGlobalInstances() - (ePlayer != NO_PLAYER ? GC.getGameINLINE().getUnitClassCreatedCount(eUnitClass) + GET_TEAM(GET_PLAYER(ePlayer).getTeam()).getUnitClassMaking(eUnitClass) : 0))));
			// K-Mod
			int iRemaining = std::max(0, GC.getUnitClassInfo(eUnitClass).getMaxGlobalInstances() - (ePlayer != NO_PLAYER ? GC.getGameINLINE().getUnitClassCreatedCount(eUnitClass) + GET_TEAM(GET_PLAYER(ePlayer).getTeam()).getUnitClassMaking(eUnitClass) : 0));
			szBuffer.append(NEWLINE);

			if (iRemaining <= 0)
				szBuffer.append(CvWString::format(SETCOLR, TEXT_COLOR("COLOR_WARNING_TEXT")));

			szBuffer.append(gDLL->getText("TXT_KEY_UNIT_WORLD_UNIT_LEFT", iRemaining));

			if (iRemaining <= 0)
				szBuffer.append(ENDCOLR);
			// K-Mod end
		}
	}

	if (isTeamUnitClass(eUnitClass))
	{
		if (pCity == NULL)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_UNIT_TEAM_UNIT_ALLOWED", GC.getUnitClassInfo(eUnitClass).getMaxTeamInstances()));
		}
		else
		{
			//szBuffer.append(gDLL->getText("TXT_KEY_UNIT_TEAM_UNIT_LEFT", (GC.getUnitClassInfo(eUnitClass).getMaxTeamInstances() - (ePlayer != NO_PLAYER ? GET_TEAM(GET_PLAYER(ePlayer).getTeam()).getUnitClassCountPlusMaking(eUnitClass) : 0))));
			// K-Mod
			int iRemaining = std::max(0, GC.getUnitClassInfo(eUnitClass).getMaxTeamInstances() - (ePlayer != NO_PLAYER ? GET_TEAM(GET_PLAYER(ePlayer).getTeam()).getUnitClassCountPlusMaking(eUnitClass) : 0));
			szBuffer.append(NEWLINE);

			if (iRemaining <= 0)
				szBuffer.append(CvWString::format(SETCOLR, TEXT_COLOR("COLOR_WARNING_TEXT")));

			szBuffer.append(gDLL->getText("TXT_KEY_UNIT_TEAM_UNIT_LEFT", iRemaining));

			if (iRemaining <= 0)
				szBuffer.append(ENDCOLR);
			// K-Mod end
		}
	}

	if (isNationalUnitClass(eUnitClass))
	{
		if (pCity == NULL)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_UNIT_NATIONAL_UNIT_ALLOWED", GC.getUnitClassInfo(eUnitClass).getMaxPlayerInstances()));
		}
		else
		{
			//szBuffer.append(gDLL->getText("TXT_KEY_UNIT_NATIONAL_UNIT_LEFT", (GC.getUnitClassInfo(eUnitClass).getMaxPlayerInstances() - (ePlayer != NO_PLAYER ? GET_PLAYER(ePlayer).getUnitClassCountPlusMaking(eUnitClass) : 0))));
			// K-Mod
			int iRemaining = std::max(0, GC.getUnitClassInfo(eUnitClass).getMaxPlayerInstances() - (ePlayer != NO_PLAYER ? GET_PLAYER(ePlayer).getUnitClassCountPlusMaking(eUnitClass) : 0));
			szBuffer.append(NEWLINE);

			if (iRemaining <= 0)
				szBuffer.append(CvWString::format(SETCOLR, TEXT_COLOR("COLOR_WARNING_TEXT")));

			szBuffer.append(gDLL->getText("TXT_KEY_UNIT_NATIONAL_UNIT_LEFT", iRemaining));

			if (iRemaining <= 0)
				szBuffer.append(ENDCOLR);
			// K-Mod end
		}
	}

	// MOD - START - Optional Unit Cost Scaling
	if (!GC.getGameINLINE().isFinalInitialized() || !GC.getGameINLINE().isOption(GAMEOPTION_NO_UNIT_COST_SCALING))
	{
		if (0 != kUnitClass.getInstanceCostModifier())
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_UNIT_INSTANCE_COST_MOD", kUnitClass.getInstanceCostModifier()));
		}

		// MOD - START - Unit Roles
		if (kUnitClass.getRoleCostModifier() > 0 && kUnitClass.getNumRoles() > 0)
		{
			szBuffer.append(NEWLINE);
			//szTempBuffer.Format(L"%+.2f", ((float)kUnitClass.getRoleCostModifier() / 100.0f) / GC.getDefineINT("EPIDEMIC_RAND_BASE"));
			szBuffer.append(gDLL->getText("TXT_KEY_UNIT_ROLE_COST_MOD", kUnitClass.getRoleCostModifier() / 100));

			for (int iMem = 0; iMem < kUnitClass.getNumRoles(); iMem++)
			{
				UnitRoleTypes eUnitRole = (UnitRoleTypes)kUnitClass.getRoles(iMem);
				setListHelp(szBuffer, L" (", GC.getUnitRoleInfo(eUnitRole).getDescription(), L", ", iMem == 0);
			}
			szBuffer.append(L")");
		}
		// MOD - END - Unit Roles
	}
	// MOD - END - Optional Unit Cost Scaling

	setBasicUnitHelp(szBuffer, eUnit, bCivilopediaText);

	if ((pCity == NULL) || !(pCity->canTrain(eUnit)))
	{
		if (pCity != NULL)
		{
			if (GC.getGameINLINE().isNoNukes())
			{
				if (kUnit.getNukeRange() != -1)
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_UNIT_NO_NUKES"));
				}
			}
		}

		if (kUnit.getHolyCity() != NO_RELIGION)
		{
			if ((pCity == NULL) || !(pCity->isHolyCity((ReligionTypes)(kUnit.getHolyCity()))))
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_UNIT_REQUIRES_HOLY_CITY", GC.getReligionInfo((ReligionTypes)(kUnit.getHolyCity())).getChar()));
			}
		}

		bFirst = true;

		if (kUnit.getSpecialUnitType() != NO_SPECIALUNIT)
		{
			if ((pCity == NULL) || !(GC.getGameINLINE().isSpecialUnitValid((SpecialUnitTypes)(kUnit.getSpecialUnitType()))))
			{
				for (iI = 0; iI < GC.getNumProjectInfos(); ++iI)
				{
					if (GC.getProjectInfo((ProjectTypes)iI).getEveryoneSpecialUnit() == kUnit.getSpecialUnitType())
					{
						szTempBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_REQUIRES").c_str());
						CvWString szProject;
						szProject.Format(L"<link=literal>%s</link>", GC.getProjectInfo((ProjectTypes)iI).getDescription());
						setListHelp(szBuffer, szTempBuffer, szProject, gDLL->getText("TXT_KEY_OR").c_str(), bFirst);
						bFirst = false;
					}
				}
			}
		}

		if (!bFirst)
		{
			szBuffer.append(ENDCOLR);
		}

		bFirst = true;

		if (kUnit.getNukeRange() != -1)
		{
			if (NULL == pCity || !GC.getGameINLINE().isNukesValid())
			{
				for (iI = 0; iI < GC.getNumProjectInfos(); ++iI)
				{
					if (GC.getProjectInfo((ProjectTypes)iI).isAllowsNukes())
					{
						szTempBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_REQUIRES").c_str());
						CvWString szProject;
						szProject.Format(L"<link=literal>%s</link>", GC.getProjectInfo((ProjectTypes)iI).getDescription());
						setListHelp(szBuffer, szTempBuffer, szProject, gDLL->getText("TXT_KEY_OR").c_str(), bFirst);
						bFirst = false;
					}
				}

				for (iI = 0; iI < GC.getNumBuildingInfos(); ++iI)
				{
					if (GC.getBuildingInfo((BuildingTypes)iI).isAllowsNukes())
					{
						szTempBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_REQUIRES").c_str());
						CvWString szBuilding;
						szBuilding.Format(L"<link=literal>%s</link>", GC.getBuildingInfo((BuildingTypes)iI).getDescription());
						setListHelp(szBuffer, szTempBuffer, szBuilding, gDLL->getText("TXT_KEY_OR").c_str(), bFirst);
						bFirst = false;
					}
				}
			}
		}

		if (!bFirst)
		{
			szBuffer.append(ENDCOLR);
		}

		if (!bCivilopediaText)
		{
			// MOD - START - Unit civic prereq
			if (kUnit.getPrereqCivic() != NO_CIVIC)
			{
				if (pCity == NULL)
				{
					// MOD - START - Trait UnitClass enabled (no prereq Civic)
					bool bPrereqTextRemains = true;
					if (GC.getGameINLINE().getActivePlayer() != NO_PLAYER)
					{
						for (iI = 0; iI < GC.getNumTraitInfos(); iI++)
						{
							if (GET_PLAYER(ePlayer).hasTrait((TraitTypes)iI))
							{
								if (GC.getTraitInfo((TraitTypes)iI).isNoPrereqCivicUnitClass(((UnitClassTypes)(kUnit.getUnitClassType()))))
								{
									bPrereqTextRemains = false;
								}
							}
						}
					}
					if (bPrereqTextRemains)
					{
					// MOD - END - Trait UnitClass enabled (no prereq Civic)
						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_UNIT_REQUIRES_STRING", GC.getCivicInfo((CivicTypes)(kUnit.getPrereqCivic())).getDescription()));
					// MOD - START - Trait UnitClass enabled (no prereq Civic)
					}
					// MOD - END - Trait UnitClass enabled (no prereq Civic)
				}
				else if (!(GET_PLAYER(ePlayer).isCivic((CivicTypes)(kUnit.getPrereqCivic()))))
				{
					// MOD - START - Trait UnitClass enabled (no prereq Civic)
					bool bPrereqTextRemains = true;
					for (iI = 0; iI < GC.getNumTraitInfos(); iI++)
					{
						if (GET_PLAYER(ePlayer).hasTrait((TraitTypes)iI))
						{
							if (GC.getTraitInfo((TraitTypes)iI).isNoPrereqCivicUnitClass(((UnitClassTypes)(kUnit.getUnitClassType()))))
							{
								bPrereqTextRemains = false;
							}
						}
					}
					if (bPrereqTextRemains)
					{
					// MOD - END - Trait UnitClass enabled (no prereq Civic)
						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_UNIT_REQUIRES_STRING", GC.getCivicInfo((CivicTypes)(kUnit.getPrereqCivic())).getDescription()));
					// MOD - START - Trait UnitClass enabled (no prereq Civic)
					}
					// MOD - END - Trait UnitClass enabled (no prereq Civic)
				}
			}
			// MOD - END - Unit civic prereq

			// MOD - START - Advanced Unit Prerequisites
			/* removed old code - getPrereqBuilding() and <prereqBuilding> tag disabled
			if (kUnit.getPrereqBuilding() != NO_BUILDING)
			{
				if ((pCity == NULL) || (pCity->getNumBuilding((BuildingTypes)(kUnit.getPrereqBuilding())) <= 0))
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_UNIT_REQUIRES_STRING", GC.getBuildingInfo((BuildingTypes)(kUnit.getPrereqBuilding())).getTextKeyWide()));
				}
			}
			*/

			bFirst = true;

			CivilizationTypes eCivilization = NO_CIVILIZATION;
			if (ePlayer != NO_PLAYER)
			{
				eCivilization = GET_PLAYER(ePlayer).getCivilizationType();
			}

			for (iI = 0; iI < kUnit.getNumPrereqAndBuildingClasses(); iI++)
			{
				BuildingClassTypes ePreqreqBuildingClass = (BuildingClassTypes) kUnit.getPrereqAndBuildingClasses(iI);
				if (ePreqreqBuildingClass != NO_BUILDINGCLASS)
				{
					BuildingTypes ePrereqBuilding;
					if (eCivilization == NO_CIVILIZATION)
					{
						ePrereqBuilding = (BuildingTypes)GC.getBuildingClassInfo(ePreqreqBuildingClass).getDefaultBuildingIndex();
					}
					else
					{
						ePrereqBuilding = (BuildingTypes)GC.getCivilizationInfo(eCivilization).getCivilizationBuildings(ePreqreqBuildingClass);
					}
					if (ePrereqBuilding != NO_BUILDING)
					{
						if ((pCity == NULL) || (pCity->getNumBuilding(ePrereqBuilding) <= 0))
						{
							// K-Mod. Check if the player has an exemption from the building requirement.
							SpecialBuildingTypes eSpecialBuilding = (SpecialBuildingTypes)GC.getBuildingInfo(ePrereqBuilding).getSpecialBuildingType();

							if (pCity == NULL || eSpecialBuilding == NO_SPECIALBUILDING || !GET_PLAYER(pCity->getOwnerINLINE()).isSpecialBuildingNotRequired(eSpecialBuilding))
							{
							// K-Mod end
								szTempBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_REQUIRES").c_str());
								setListHelp(szBuffer, szTempBuffer, GC.getBuildingInfo(ePrereqBuilding).getDescription(), gDLL->getText("TXT_KEY_AND").c_str(), bFirst);
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

			bFirst = true;

			if (kUnit.getNumPrereqOrBuildingClasses() > 0)
			{
				bool bRequiresMissingOrBuilding = true;

				for (iI = 0; iI < kUnit.getNumPrereqOrBuildingClasses(); iI++)
				{
					BuildingClassTypes ePreqreqBuildingClass = (BuildingClassTypes)kUnit.getPrereqOrBuildingClasses(iI);
					if (ePreqreqBuildingClass != NO_BUILDINGCLASS)
					{
						BuildingTypes ePrereqBuilding;
						if (eCivilization == NO_CIVILIZATION)
						{
							ePrereqBuilding = (BuildingTypes)GC.getBuildingClassInfo(ePreqreqBuildingClass).getDefaultBuildingIndex();
						}
						else
						{
							ePrereqBuilding = (BuildingTypes)GC.getCivilizationInfo(eCivilization).getCivilizationBuildings(ePreqreqBuildingClass);
						}
						if (ePrereqBuilding != NO_BUILDING)
						{
							if ((pCity != NULL))
							{
								if ((pCity->getNumBuilding(ePrereqBuilding) > 0))
								{
									bRequiresMissingOrBuilding = false;
								}
								else
								{
									SpecialBuildingTypes eSpecialBuilding = (SpecialBuildingTypes)GC.getBuildingInfo(ePrereqBuilding).getSpecialBuildingType();

									if (eSpecialBuilding != NO_SPECIALBUILDING && GET_PLAYER(pCity->getOwnerINLINE()).isSpecialBuildingNotRequired(eSpecialBuilding))
									{
										bRequiresMissingOrBuilding = false;
									}
								}
							}
						}
					}
				}

				if (bRequiresMissingOrBuilding)
				{
					for (iI = 0; iI < kUnit.getNumPrereqOrBuildingClasses(); iI++)
					{
						BuildingClassTypes ePreqreqBuildingClass = (BuildingClassTypes)kUnit.getPrereqOrBuildingClasses(iI);
						if (ePreqreqBuildingClass != NO_BUILDINGCLASS)
						{
							BuildingTypes ePrereqBuilding;
							if (eCivilization == NO_CIVILIZATION)
							{
								ePrereqBuilding = (BuildingTypes)GC.getBuildingClassInfo(ePreqreqBuildingClass).getDefaultBuildingIndex();
							}
							else
							{
								ePrereqBuilding = (BuildingTypes)GC.getCivilizationInfo(eCivilization).getCivilizationBuildings(ePreqreqBuildingClass);
							}
							if (ePrereqBuilding != NO_BUILDING)
							{
								if ((pCity == NULL) || (pCity->getNumBuilding(ePrereqBuilding) <= 0	))
								{
									szTempBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_REQUIRES").c_str());
									setListHelp(szBuffer, szTempBuffer, GC.getBuildingInfo(ePrereqBuilding).getDescription(), gDLL->getText("TXT_KEY_OR").c_str(), bFirst);
									bFirst = false;
								}
							}
						}
					}
				}
			}

			if (!bFirst)
			{
				szBuffer.append(ENDCOLR);
			}

			/*
			if (!bTechChooserText)
			{
				if (kUnit.getPrereqAndTech() != NO_TECH)
				{
					if (GC.getGameINLINE().getActivePlayer() == NO_PLAYER || !(GET_TEAM(GET_PLAYER(ePlayer).getTeam()).isHasTech((TechTypes)(kUnit.getPrereqAndTech()))))
					{
						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_UNIT_REQUIRES_STRING", GC.getTechInfo((TechTypes)(kUnit.getPrereqAndTech())).getTextKeyWide()));
					}
				}
			}

			bFirst = true;

			for (iI = 0; iI < GC.getNUM_UNIT_AND_TECH_PREREQS(); ++iI)
			{
				if (kUnit.getPrereqAndTechs(iI) != NO_TECH)
				{
					if (bTechChooserText || GC.getGameINLINE().getActivePlayer() == NO_PLAYER || !(GET_TEAM(GET_PLAYER(ePlayer).getTeam()).isHasTech((TechTypes)(kUnit.getPrereqAndTechs(iI)))))
					{
						szTempBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_REQUIRES").c_str());
						setListHelp(szBuffer, szTempBuffer, GC.getTechInfo(((TechTypes)(kUnit.getPrereqAndTechs(iI)))).getDescription(), gDLL->getText("TXT_KEY_AND").c_str(), bFirst);
						bFirst = false;
					}
				}
			}

			if (!bFirst)
			{
				szBuffer.append(ENDCOLR);
			}
			*/

			bFirst = true;

			// Note: In the tech chooser view the unit will appear on the panel of the first
			// prerequisite tech, so we needn't display it in the help text because it's placement
			// implies the prerequisite
			for (iI = (bTechChooserText ? 1 : 0); iI < kUnit.getNumPrereqAndTechs(); ++iI)
			{
				TechTypes eTech = (TechTypes)kUnit.getPrereqAndTechs(iI);

				if (bTechChooserText || ePlayer == NO_PLAYER || !GET_TEAM(GET_PLAYER(ePlayer).getTeam()).isHasTech(eTech))
				{
					szTempBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_REQUIRES").c_str());
					setListHelp(szBuffer, szTempBuffer, GC.getTechInfo(eTech).getDescription(), gDLL->getText("TXT_KEY_AND").c_str(), bFirst);
					bFirst = false;
				}
			}

			if (!bFirst)
			{
				szBuffer.append(ENDCOLR);
			}

			/*
			if (kUnit.getPrereqAndBonus() != NO_BONUS)
			{
				if ((pCity == NULL) || !(pCity->hasBonus((BonusTypes)kUnit.getPrereqAndBonus())))
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_UNIT_REQUIRES_STRING", GC.getBonusInfo((BonusTypes)(kUnit.getPrereqAndBonus())).getTextKeyWide()));
				}
			}
			*/

			bFirst = true;

			for (iI = 0; iI < kUnit.getNumPrereqAndBonuses(); iI++)
			{
				BonusTypes eBonus = (BonusTypes)kUnit.getPrereqAndBonuses(iI);
				if ((pCity == NULL) || !pCity->hasBonus(eBonus))
				{
					szTempBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_REQUIRES").c_str());
					setListHelp(szBuffer, szTempBuffer, GC.getBonusInfo(eBonus).getDescription(), gDLL->getText("TXT_KEY_AND").c_str(), bFirst);
					bFirst = false;
				}
			}

			if (!bFirst)
			{
				szBuffer.append(ENDCOLR);
			}

			bFirst = true;

			bool bRequiresOrBonus = true;

			for (iI = 0; iI < kUnit.getNumPrereqOrBonuses(); iI++)
			{
				if ((pCity != NULL) && (pCity->hasBonus((BonusTypes)kUnit.getPrereqOrBonuses(iI))))
				{
					bRequiresOrBonus = false;
				}
			}

			if(bRequiresOrBonus)
			{
				for (iI = 0; iI < kUnit.getNumPrereqOrBonuses(); iI++)
				{
					if ((pCity == NULL) || !(pCity->hasBonus((BonusTypes)kUnit.getPrereqOrBonuses(iI))))
					{
						szTempBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_REQUIRES").c_str());
						setListHelp(szBuffer, szTempBuffer, GC.getBonusInfo((BonusTypes) kUnit.getPrereqOrBonuses(iI)).getDescription(), gDLL->getText("TXT_KEY_OR").c_str(), bFirst);
						bFirst = false;
					}
				}
			}
			// MOD - END - Advanced Unit Prerequisites

			if (!bFirst)
			{
				szBuffer.append(ENDCOLR);
			}
		}
	}

	if (!bCivilopediaText && GC.getGameINLINE().getActivePlayer() != NO_PLAYER)
	{
		if (pCity == NULL)
		{
			szTempBuffer.Format(L"%s%d%c", NEWLINE, GET_PLAYER(ePlayer).getProductionNeeded(eUnit), GC.getYieldInfo(YIELD_PRODUCTION).getChar());
			szBuffer.append(szTempBuffer);
		}
		else
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_UNIT_TURNS", pCity->getProductionTurnsLeft(eUnit, ((GC.ctrlKey() || !(GC.shiftKey())) ? 0 : pCity->getOrderQueueLength())), pCity->getProductionNeeded(eUnit), GC.getYieldInfo(YIELD_PRODUCTION).getChar()));

			iProduction = pCity->getUnitProduction(eUnit);

			if (iProduction > 0)
			{
				szTempBuffer.Format(L" - %d/%d%c", iProduction, pCity->getProductionNeeded(eUnit), GC.getYieldInfo(YIELD_PRODUCTION).getChar());
				szBuffer.append(szTempBuffer);
			}
			else
			{
				szTempBuffer.Format(L" - %d%c", pCity->getProductionNeeded(eUnit), GC.getYieldInfo(YIELD_PRODUCTION).getChar());
				szBuffer.append(szTempBuffer);
			}
		}

		// MOD - START - Unit Roles
		long iCostScalingModifier = GET_PLAYER(ePlayer).getProductionCostScalingModifier(eUnit) / 100;
		if (iCostScalingModifier != 0)
		{
			szTempBuffer.Format(L" (+%d%%)", iCostScalingModifier);
			szBuffer.append(szTempBuffer);
		}
		// MOD - END - Unit Roles
	}

	// MOD - START - Relation Caching
	for (int iRel = 0; iRel < kUnit.getNumRelatedBonuses(); iRel++)
	{
		BonusTypes eLoopBonus = (BonusTypes)kUnit.getRelatedBonuses(iRel);

		if (kUnit.getBonusProductionModifier(eLoopBonus) != 0)
		{
			if (pCity != NULL)
			{
				if (pCity->hasBonus(eLoopBonus))
				{
					szBuffer.append(gDLL->getText("TXT_KEY_COLOR_POSITIVE"));
				}
				else
				{
					szBuffer.append(gDLL->getText("TXT_KEY_COLOR_NEGATIVE"));
				}
			}
			if (!bCivilopediaText)
			{
				szBuffer.append(L" (");
			}
			else
			{
				szTempBuffer.Format(L"%s%c", NEWLINE, gDLL->getSymbolID(BULLET_CHAR), szTempBuffer.GetCString());
				szBuffer.append(szTempBuffer);
			}
			if (kUnit.getBonusProductionModifier(eLoopBonus) == 100)
			{
				szBuffer.append(gDLL->getText("TXT_KEY_UNIT_DOUBLE_SPEED", GC.getBonusInfo(eLoopBonus).getTextKeyWide()));
			}
			else
			{
				szBuffer.append(gDLL->getText("TXT_KEY_UNIT_BUILDS_FASTER", kUnit.getBonusProductionModifier(eLoopBonus), GC.getBonusInfo(eLoopBonus).getTextKeyWide()));
			}
			if (!bCivilopediaText)
			{
				szBuffer.append(L")");
			}
			if (pCity != NULL)
			{
				szBuffer.append(gDLL->getText("TXT_KEY_COLOR_REVERT"));
			}
		}
	}
	// MOD - END - Relation Caching

	if (bStrategyText)
	{
		if (!CvWString(kUnit.getStrategy()).empty())
		{
			if ((ePlayer == NO_PLAYER) || GET_PLAYER(ePlayer).isOption(PLAYEROPTION_ADVISOR_HELP))
			{
				// MOD - START - Disabled Sid's Tips
				/*
				szBuffer.append(SEPARATOR);
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_SIDS_TIPS"));
				szBuffer.append(L'\"');
				szBuffer.append(kUnit.getStrategy());
				szBuffer.append(L'\"');
				*/
				// MOD - END - Disabled Sid's Tips
			}
		}
	}

	if (bCivilopediaText)
	{
		if (eDefaultUnit == eUnit)
		{
			// MOD - START - Relation Caching
			for (int iMem = 0; iMem < kUnitClass.getNumUnitTypes(); iMem++)
			{
				UnitTypes eLoopUnit = (UnitTypes)kUnitClass.getUnitTypes(iMem);
				if (eLoopUnit != eUnit)
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_REPLACED_BY_UNIT", GC.getUnitInfo(eLoopUnit).getTextKeyWide()));
				}
			}
			// MOD - END - Relation Caching
		}
	}

	if (pCity != NULL)
	{
		if ((gDLL->getChtLvl() > 0) && GC.ctrlKey())
		{
			szBuffer.append(NEWLINE);
			for (int iUnitAI = 0; iUnitAI < NUM_UNITAI_TYPES; iUnitAI++)
			{
				int iTempValue = GET_PLAYER(pCity->getOwner()).AI_unitValue(eUnit, (UnitAITypes)iUnitAI, pCity->area());
				if (iTempValue != 0)
				{
					CvWString szTempString;
					getUnitAIString(szTempString, (UnitAITypes)iUnitAI);
					szBuffer.append(CvWString::format(L"(%s : %d) ", szTempString.GetCString(), iTempValue));
				}
			}
		}
	}
}


// BUG - Building Actual Effects - start
/*
 * Calls new function below without displaying actual effects.
 */
void CvGameTextMgr::setBuildingHelp(CvWStringBuffer &szBuffer, BuildingTypes eBuilding, bool bCivilopediaText, bool bStrategyText, bool bTechChooserText, CvCity* pCity)
{
	setBuildingHelpActual(szBuffer, eBuilding, bCivilopediaText, bStrategyText, bTechChooserText, pCity, false);
}

/*
 * Adds the actual effects of adding a building to the city.
 */
void CvGameTextMgr::setBuildingNetEffectsHelp(CvWStringBuffer &szBuffer, BuildingTypes eBuilding, CvCity* pCity)
{
	if (NULL != pCity)
	{
		bool bStarted = false;
		CvWString szStart;
        szStart.Format(L"\n" SETCOLR L"(%s", TEXT_COLOR("COLOR_LIGHT_GREY"), gDLL->getText("TXT_KEY_ACTUAL_EFFECTS").GetCString());

		// Defense
		int iDefense = pCity->getAdditionalDefenseByBuilding(eBuilding);
		bStarted = setResumableValueChangeHelp(szBuffer, szStart, L": ", L"", iDefense, gDLL->getSymbolID(DEFENSE_CHAR), true, false, bStarted);
		
		// Happiness
		int iGood = 0;
		int iBad = 0;
		pCity->getAdditionalHappinessByBuilding(eBuilding, iGood, iBad);
		//int iAngryPop = pCity->getAdditionalAngryPopuplation(iGood, iBad);
		bStarted = setResumableGoodBadChangeHelp(szBuffer, szStart, L": ", L"", iGood, gDLL->getSymbolID(HAPPY_CHAR), iBad, gDLL->getSymbolID(UNHAPPY_CHAR), false, false, bStarted);
		//bStarted = setResumableValueChangeHelp(szBuffer, szStart, L": ", L"", iAngryPop, gDLL->getSymbolID(ANGRY_POP_CHAR), false, bNewLine, bStarted);

		// Health
		iGood = 0;
		iBad = 0;
		pCity->getAdditionalHealthByBuilding(eBuilding, iGood, iBad);
		//int iSpoiledFood = pCity->getAdditionalSpoiledFood(iGood, iBad);
		//int iStarvation = pCity->getAdditionalStarvation(iSpoiledFood);
		bStarted = setResumableGoodBadChangeHelp(szBuffer, szStart, L": ", L"", iGood, gDLL->getSymbolID(HEALTHY_CHAR), iBad, gDLL->getSymbolID(UNHEALTHY_CHAR), false, false, bStarted);
		//bStarted = setResumableValueChangeHelp(szBuffer, szStart, L": ", L"", iSpoiledFood, gDLL->getSymbolID(EATEN_FOOD_CHAR), false, false, bStarted);
		//bStarted = setResumableValueChangeHelp(szBuffer, szStart, L": ", L"", iStarvation, gDLL->getSymbolID(BAD_FOOD_CHAR), false, false, bStarted);

		// MOD - START - Epidemics
		// Epidemics
		iGood = 0;
		iBad = 0;
		int iEpidemicModifier = pCity->getAdditionalEpidemicModifierByBuilding(eBuilding, iGood, iBad);
		bStarted = setResumableValueTimes10ChangeHelp(szBuffer, szStart, L": ", L"", iEpidemicModifier, gDLL->getSymbolID(EPIDEMIC_START_CHAR), true, false, bStarted);
		//bStarted = setResumableValueTimes100ChangeHelp(szBuffer, szStart, L": ", L"", iBad, gDLL->getSymbolID(EPIDEMIC_START_CHAR), false, bStarted);
		// MOD - END - Epidemics

		// Yield
		int aiYields[NUM_YIELD_TYPES];
		for (int iI = 0; iI < NUM_YIELD_TYPES; ++iI)
		{
			aiYields[iI] = pCity->getAdditionalYieldByBuilding((YieldTypes)iI, eBuilding);
		}
		bStarted = setResumableYieldChangeHelp(szBuffer, szStart, L": ", L"", aiYields, false, false, bStarted);
		
		// Commerce
		int aiCommerces[NUM_COMMERCE_TYPES];
		for (int iI = 0; iI < NUM_COMMERCE_TYPES; ++iI)
		{
			aiCommerces[iI] = pCity->getAdditionalCommerceTimes100ByBuilding((CommerceTypes)iI, eBuilding);
		}
		// Maintenance - add to gold
		aiCommerces[COMMERCE_GOLD] += pCity->getSavedMaintenanceTimes100ByBuilding(eBuilding);
		bStarted = setResumableCommerceTimes100ChangeHelp(szBuffer, szStart, L": ", L"", aiCommerces, false, bStarted);

		// Great People
		int iGreatPeopleRate = pCity->getAdditionalGreatPeopleRateByBuilding(eBuilding);
		bStarted = setResumableValueChangeHelp(szBuffer, szStart, L": ", L"", iGreatPeopleRate, gDLL->getSymbolID(GREAT_PEOPLE_CHAR), false, false, bStarted);

		if (bStarted)
			szBuffer.append(L")" ENDCOLR);
	}
}

/*
 * Adds option to display actual effects.
 */
//void CvGameTextMgr::setBuildingHelp(CvWStringBuffer &szBuffer, BuildingTypes eBuilding, bool bCivilopediaText, bool bStrategyText, bool bTechChooserText, CvCity* pCity)
void CvGameTextMgr::setBuildingHelpActual(CvWStringBuffer &szBuffer, BuildingTypes eBuilding, bool bCivilopediaText, bool bStrategyText, bool bTechChooserText, CvCity* pCity, bool bActual)
// BUG - Building Actual Effects - end
{
	PROFILE_FUNC();

	CvWString szFirstBuffer;
	CvWString szTempBuffer;
	BuildingTypes eLoopBuilding;
	UnitTypes eGreatPeopleUnit;
	PlayerTypes ePlayer;
	bool bFirst;
	int iProduction;
	int iLast;

	if (NO_BUILDING == eBuilding)
	{
		return;
	}

	CvBuildingInfo& kBuilding = GC.getBuildingInfo(eBuilding);
	BuildingClassTypes eBuildingClass = (BuildingClassTypes)kBuilding.getBuildingClassType();


	if (pCity != NULL)
	{
		ePlayer = pCity->getOwnerINLINE();
	}
	else
	{
		ePlayer = GC.getGameINLINE().getActivePlayer();
	}

	if (!bCivilopediaText)
	{
		szTempBuffer.Format( SETCOLR L"<link=literal>%s</link>" ENDCOLR , TEXT_COLOR("COLOR_BUILDING_TEXT"), kBuilding.getDescription());
		szBuffer.append(szTempBuffer);

		int iHappiness;
/*
** K-Mod, 30/dec/10, karadoc
** changed so that conditional happiness is not double-reported. (such as happiness from state-religion buildings, or culture slider)
*/
		/* original bts code
		if (NULL != pCity)
		{
			iHappiness = pCity->getBuildingHappiness(eBuilding);
		}
		else
		{
			iHappiness = kBuilding.getHappiness();
		}*/
		iHappiness = kBuilding.getHappiness();
		if (pCity)
		{
			// special modifiers (eg. events). These modifiers don't get their own line of text, so they need to be included here.
			iHappiness += pCity->getBuildingHappyChange(eBuildingClass);
			iHappiness += GET_PLAYER(ePlayer).getExtraBuildingHappiness(eBuilding);
			// 'Extra building happiness' includes happiness from several sources, including events, civics, traits, and boosts from other buildings.
			// My aim here is to only include in the total what isn't already in the list of bonuses below. As far as I know the only thing that would
			// be double-reported is the civic happiness. So I'll subtract that.
			for (int i = 0; i < GC.getNumCivicInfos(); i++)
			{
				if (GET_PLAYER(ePlayer).isCivic((CivicTypes)i))
				{
					iHappiness -= GC.getCivicInfo((CivicTypes)i).getBuildingHappinessChanges(eBuildingClass);
				}
			}
		}
/*
** K-Mod end
*/

		if (iHappiness != 0)
		{
			szTempBuffer.Format(L", +%d%c", abs(iHappiness), ((iHappiness > 0) ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR)));
			szBuffer.append(szTempBuffer);
		}

		int iHealth;
/*
** K-Mod, 30/dec/10, karadoc
** changed so that conditional healthiness is not counted. (such as healthiness from public transport with environmentalism)
*/
		/* original bts code
		if (NULL != pCity)
		{
			iHealth = pCity->getBuildingGoodHealth(eBuilding);
		}
		else
		{
			iHealth = kBuilding.getHealth();
			if (ePlayer != NO_PLAYER)
			{
				if (eBuilding == GC.getCivilizationInfo(GET_PLAYER(ePlayer).getCivilizationType()).getCivilizationBuildings(kBuilding.getBuildingClassType()))
				{
					iHealth += GET_PLAYER(ePlayer).getExtraBuildingHealth(eBuilding);
				}
			}
		}
		if (iHealth != 0)
		{
			szTempBuffer.Format(L", +%d%c", abs(iHealth), ((iHealth > 0) ? gDLL->getSymbolID(HEALTHY_CHAR): gDLL->getSymbolID(UNHEALTHY_CHAR)));
			szBuffer.append(szTempBuffer);
		}

		iHealth = 0;
		if (NULL != pCity)
		{
			iHealth = pCity->getBuildingBadHealth(eBuilding);
		}*/
		iHealth = kBuilding.getHealth();
		if (pCity)
		{
			// special modifiers (eg. events). These modifiers don't get their own line of text, so they need to be included here.
			iHealth += pCity->getBuildingHealthChange(eBuildingClass);
			iHealth += GET_PLAYER(ePlayer).getExtraBuildingHealth(eBuilding);
			// We need to subtract any civic bonuses from 'extra building health', so as not to double-report. (see comments for the happiness section.)
			for (int i = 0; i < GC.getNumCivicInfos(); i++)
			{
				if (GET_PLAYER(ePlayer).isCivic((CivicTypes)i))
				{
					iHealth -= GC.getCivicInfo((CivicTypes)i).getBuildingHealthChanges(eBuildingClass);
				}
			}
		}
/*
** K-Mod end
*/
		if (iHealth != 0)
		{
			szTempBuffer.Format(L", +%d%c", abs(iHealth), ((iHealth > 0) ? gDLL->getSymbolID(HEALTHY_CHAR): gDLL->getSymbolID(UNHEALTHY_CHAR)));
			szBuffer.append(szTempBuffer);
		}

		int aiYields[NUM_YIELD_TYPES];
		for (int iI = 0; iI < NUM_YIELD_TYPES; ++iI)
		{
			aiYields[iI] = kBuilding.getYieldChange(iI);

			if (NULL != pCity)
			{
				aiYields[iI] += pCity->getBuildingYieldChange(eBuildingClass, (YieldTypes)iI);
			}
			// MOD - START - Advanced Yield and Commerce Effects
			if (ePlayer != NO_PLAYER)
			{
				aiYields[iI] += GET_PLAYER(ePlayer).getBuildingYieldChange(eBuildingClass, (YieldTypes)iI);
			}
			// MOD - END - Advanced Yield and Commerce Effects
		}
		setYieldChangeHelp(szBuffer, L", ", L"", L"", aiYields, false, false);

		int aiCommerces[NUM_COMMERCE_TYPES];
		for (int iI = 0; iI < NUM_COMMERCE_TYPES; ++iI)
		{
			if ((NULL != pCity) && (pCity->getNumBuilding(eBuilding) > 0))
			{
				aiCommerces[iI] = pCity->getBuildingCommerceByBuilding((CommerceTypes)iI, eBuilding);
			}
			else
			{
				aiCommerces[iI] = kBuilding.getCommerceChange(iI);
				aiCommerces[iI] += kBuilding.getObsoleteSafeCommerceChange(iI);
				// MOD - START - Advanced Yield and Commerce Effects
				if (ePlayer != NO_PLAYER)
				{
					aiCommerces[iI] += GET_PLAYER(ePlayer).getBuildingCommerceChange(eBuildingClass, (CommerceTypes)iI);
				}
				// MOD - END - Advanced Yield and Commerce Effects
/*
** K-Mod, 30/dec/10, karadoc
** added relgious building bonus info
*/
				if (ePlayer != NO_PLAYER &&
					kBuilding.getReligionType() != NO_RELIGION &&
					kBuilding.getReligionType() == GET_PLAYER(ePlayer).getStateReligion())
				{
					aiCommerces[iI] += GET_PLAYER(ePlayer).getStateReligionBuildingCommerce((CommerceTypes)iI);
				}
/*
** K-Mod end
*/
			}
		}
		setCommerceChangeHelp(szBuffer, L", ", L"", L"", aiCommerces, false, false);

		setYieldChangeHelp(szBuffer, L", ", L"", L"", kBuilding.getYieldModifierArray(), true, bCivilopediaText);
		setCommerceChangeHelp(szBuffer, L", ", L"", L"", kBuilding.getCommerceModifierArray(), true, bCivilopediaText);

		if (kBuilding.getGreatPeopleRateChange() != 0)
		{
			szTempBuffer.Format(L", %s%d%c", ((kBuilding.getGreatPeopleRateChange() > 0) ? "+" : ""), kBuilding.getGreatPeopleRateChange(), gDLL->getSymbolID(GREAT_PEOPLE_CHAR));
			szBuffer.append(szTempBuffer);

			if (kBuilding.getGreatPeopleUnitClass() != NO_UNITCLASS)
			{
				if (ePlayer != NO_PLAYER)
				{
					eGreatPeopleUnit = ((UnitTypes)(GC.getCivilizationInfo(GET_PLAYER(ePlayer).getCivilizationType()).getCivilizationUnits(kBuilding.getGreatPeopleUnitClass())));
				}
				else
				{
					eGreatPeopleUnit = (UnitTypes)GC.getUnitClassInfo((UnitClassTypes)kBuilding.getGreatPeopleUnitClass()).getDefaultUnitIndex();
				}

				if (eGreatPeopleUnit != NO_UNIT)
				{
					szTempBuffer.Format(L" (%s)", GC.getUnitInfo(eGreatPeopleUnit).getDescription());
					szBuffer.append(szTempBuffer);
				}
			}
		}
	}

	// MOD - START - Congestion
	if (kBuilding.getInsideCityCongestionThresholdChange() == kBuilding.getOutsideCityCongestionThresholdChange())
	{
		if (kBuilding.getInsideCityCongestionThresholdChange() != 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_CONGESTION_THRESHOLD_CHANGE", kBuilding.getInsideCityCongestionThresholdChange()));
		}
	}
	else
	{
		if (kBuilding.getInsideCityCongestionThresholdChange() != 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_CONGESTION_INSIDE_CITY_THRESHOLD_CHANGE", kBuilding.getInsideCityCongestionThresholdChange()));
		}

		if (kBuilding.getOutsideCityCongestionThresholdChange() != 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_CONGESTION_OUTSIDE_CITY_THRESHOLD_CHANGE", kBuilding.getOutsideCityCongestionThresholdChange()));
		}
	}
	// MOD - END - Congestion

	// MOD - START - Unique Buildings
	if (kBuilding.isUniqueBuilding())
	{
		for (int iI  = 0; iI < GC.getNumCivilizationInfos(); ++iI)
		{
			BuildingTypes eUniqueBuilding = (BuildingTypes)GC.getCivilizationInfo((CivilizationTypes)iI).getCivilizationBuildings(eBuildingClass);
			if (eUniqueBuilding == eBuilding)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_UNIQUE_BUILDING", GC.getCivilizationInfo((CivilizationTypes)iI).getTextKeyWide()));
			}
		}
	}

	BuildingTypes eDefaultBuilding = (BuildingTypes)GC.getBuildingClassInfo(eBuildingClass).getDefaultBuildingIndex();
	if (NO_BUILDING != eDefaultBuilding && eDefaultBuilding != eBuilding)
	{
		if (!kBuilding.isUniqueBuilding())
		{
			for (int iI  = 0; iI < GC.getNumCivilizationInfos(); ++iI)
			{
				BuildingTypes eUniqueBuilding = (BuildingTypes)GC.getCivilizationInfo((CivilizationTypes)iI).getCivilizationBuildings(eBuildingClass);
				if (eUniqueBuilding == eBuilding)
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_DISTINCTIVE_BUILDING", GC.getCivilizationInfo((CivilizationTypes)iI).getTextKeyWide()));
				}
			}
		}

		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_REPLACES_UNIT", GC.getBuildingInfo(eDefaultBuilding).getTextKeyWide()));
	}
	// MOD - END - Unique Buildings

	// MOD - START - Building Timer
	if ( kBuilding.getTimer() > 0)
	{
		if (pCity == NULL || ePlayer == NO_PLAYER)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_TIMER_PEDIA", kBuilding.getTimer()));
		}
		else
		{
			if (kBuilding.getTimer() == GC.getGameINLINE().getGameTurn() + pCity->getBuildingOriginalTurn(eBuilding))
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_TIMER_THIS", kBuilding.getTimer()));
			}
			else
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_TIMER", kBuilding.getTimer(), (kBuilding.getTimer()) - GC.getGameINLINE().getGameTurn() + pCity->getBuildingOriginalTurn(eBuilding)));
			}
		}
	}
	// MOD - END - Building Timer

	if (bCivilopediaText)
	{
		setYieldChangeHelp(szBuffer, L"", L"", L"", kBuilding.getYieldModifierArray(), true, bCivilopediaText);

		setCommerceChangeHelp(szBuffer, L"", L"", L"", kBuilding.getCommerceModifierArray(), true, bCivilopediaText);
	}
	else
	{
		if (isWorldWonderClass((BuildingClassTypes)(kBuilding.getBuildingClassType())))
		{
			if (pCity == NULL || ePlayer == NO_PLAYER)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_WORLD_WONDER_ALLOWED", GC.getBuildingClassInfo((BuildingClassTypes) kBuilding.getBuildingClassType()).getMaxGlobalInstances()));
			}
			else
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_WORLD_WONDER_LEFT", (GC.getBuildingClassInfo((BuildingClassTypes) kBuilding.getBuildingClassType()).getMaxGlobalInstances() - GC.getGameINLINE().getBuildingClassCreatedCount((BuildingClassTypes)(kBuilding.getBuildingClassType())) - GET_TEAM(GET_PLAYER(ePlayer).getTeam()).getBuildingClassMaking((BuildingClassTypes)(kBuilding.getBuildingClassType())))));
			}
		}

		if (isTeamWonderClass((BuildingClassTypes)(kBuilding.getBuildingClassType())))
		{
			if (pCity == NULL || ePlayer == NO_PLAYER)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_TEAM_WONDER_ALLOWED", GC.getBuildingClassInfo((BuildingClassTypes) kBuilding.getBuildingClassType()).getMaxTeamInstances()));
			}
			else
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_TEAM_WONDER_LEFT", (GC.getBuildingClassInfo((BuildingClassTypes) kBuilding.getBuildingClassType()).getMaxTeamInstances() - GET_TEAM(GET_PLAYER(ePlayer).getTeam()).getBuildingClassCountPlusMaking((BuildingClassTypes)(kBuilding.getBuildingClassType())))));
			}
		}

		if (isNationalWonderClass((BuildingClassTypes)(kBuilding.getBuildingClassType())))
		{
			if (pCity == NULL || ePlayer == NO_PLAYER)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_NATIONAL_WONDER_ALLOWED", GC.getBuildingClassInfo((BuildingClassTypes) kBuilding.getBuildingClassType()).getMaxPlayerInstances()));
			}
			else
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_NATIONAL_WONDER_LEFT", (GC.getBuildingClassInfo((BuildingClassTypes) kBuilding.getBuildingClassType()).getMaxPlayerInstances() - GET_PLAYER(ePlayer).getBuildingClassCountPlusMaking((BuildingClassTypes)(kBuilding.getBuildingClassType())))));
			}
		}
	}

	if (kBuilding.getGlobalReligionCommerce() != NO_RELIGION)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_GOLD_PER_GLOBAL", (GC.getWorldInfo(GC.getMapINLINE().getWorldSize()).getTargetNumCities() * 6)));
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_RELIGION_PERCENTAGE_WITH", GC.getReligionInfo((ReligionTypes) kBuilding.getGlobalReligionCommerce()).getChar()));
	
	}

	if (NO_CORPORATION != kBuilding.getFoundsCorporation())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_FOUNDS_CORPORATION", GC.getCorporationInfo((CorporationTypes)kBuilding.getFoundsCorporation()).getTextKeyWide()));
	}

	if (kBuilding.getGlobalCorporationCommerce() != NO_CORPORATION)
	{
		szFirstBuffer = gDLL->getText("TXT_KEY_BUILDING_PER_CITY_WITH", GC.getCorporationInfo((CorporationTypes) kBuilding.getGlobalCorporationCommerce()).getChar());
		setCommerceChangeHelp(szBuffer, L"", L"", szFirstBuffer, GC.getCorporationInfo((CorporationTypes) kBuilding.getGlobalCorporationCommerce()).getHeadquarterCommerceArray());
	}

	// MOD - START - Fresh Water Aquifers
	if (kBuilding.isFreshWaterSource())
	{
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_PROVIDES_FRESH_WATER"));
	}
	// MOD - END - Fresh Water Aquifers

	if (kBuilding.isRiver())
	{
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_REQUIRES_RIVER"));
	}
	
	if (kBuilding.isWater())
	{
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_REQUIRES_WATER"));
	}	

	if (kBuilding.getNoBonus() != NO_BONUS)
	{
		CvBonusInfo& kNoBonus = GC.getBonusInfo((BonusTypes) kBuilding.getNoBonus());
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_DISABLES", kNoBonus.getTextKeyWide(), kNoBonus.getChar()));
	}

	if (kBuilding.getFreeBonus() != NO_BONUS)
	{
		// MOD - START - Improved Civilopedia
		const CvBonusInfo& kFreeBonus = GC.getBonusInfo((BonusTypes) kBuilding.getFreeBonus());

		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_PROVIDES", GC.getGameINLINE().getNumFreeBonuses(eBuilding), CvWString(kFreeBonus.getType()).GetCString(), kFreeBonus.getTextKeyWide(), kFreeBonus.getChar()));

		if (kFreeBonus.getHealth() != 0)
		{
			szTempBuffer.Format(L", +%d%c", abs(kFreeBonus.getHealth()), ((kFreeBonus.getHealth() > 0) ? gDLL->getSymbolID(HEALTHY_CHAR): gDLL->getSymbolID(UNHEALTHY_CHAR)));
			szBuffer.append(szTempBuffer);
		}

		if (kFreeBonus.getHappiness() != 0)
		{
			szTempBuffer.Format(L", +%d%c", abs(kFreeBonus.getHappiness()), ((kFreeBonus.getHappiness() > 0) ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR)));
			szBuffer.append(szTempBuffer);
		}
		// MOD - END - Improved Civilopedia
	}

	// MOD - START - Bonus Converters
	if (kBuilding.isBonusConverter())
	{
		CvWStringBuffer szBonusList;
		CvWString szBonusItem;
		bFirst = true;

		// Bonus Consumption
		for (int iConsumedBonusIndex = 0; iConsumedBonusIndex < kBuilding.getNumConsumedBonuses(); iConsumedBonusIndex++)
		{
			BonusTypes eConsumedBonus = (BonusTypes)kBuilding.getConsumedBonusType(iConsumedBonusIndex);
			int iConsumedAmount = kBuilding.getConsumedBonusAmount(iConsumedBonusIndex);

			if (iConsumedAmount > 1)
			{
				szBonusItem.Format(L"%d %s", iConsumedAmount, GC.getBonusInfo(eConsumedBonus).getDescription());
			}
			else
			{
				szBonusItem.Format(L"%s", GC.getBonusInfo(eConsumedBonus).getDescription());
			}

			szTempBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_CONSUMES").c_str());

			setListHelp(szBonusList, szTempBuffer, szBonusItem, gDLL->getText("TXT_KEY_AND").c_str(), bFirst);
			bFirst = false;
		}

		if (!bFirst)
		{
			szBuffer.append(szBonusList);
		}

		// Bonus Production
		szBonusList.clear();
		bFirst = true;

		for (int iProducedBonusIndex = 0; iProducedBonusIndex < kBuilding.getNumProducedBonuses(); iProducedBonusIndex++)
		{
			BonusTypes eProducedBonus = (BonusTypes)kBuilding.getProducedBonusType(iProducedBonusIndex);
			int iProducedAmount = kBuilding.getProducedBonusAmount(iProducedBonusIndex);

			if (iProducedAmount > 1)
			{
				szBonusItem.Format(L"%d %s", iProducedAmount, GC.getBonusInfo(eProducedBonus).getDescription());
			}
			else
			{
				szBonusItem.Format(L"%s", GC.getBonusInfo(eProducedBonus).getDescription());
			}

			szTempBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_PRODUCES").c_str());

			setListHelp(szBonusList, szTempBuffer, szBonusItem, gDLL->getText("TXT_KEY_AND").c_str(), bFirst);
			bFirst = false;
		}

		if (!bFirst)
		{
			szBuffer.append(szBonusList);
		}
	}
	// MOD - END - Bonus Converters

	if (kBuilding.getFreeBuildingClass() != NO_BUILDINGCLASS)
	{
		BuildingTypes eFreeBuilding;
		if (ePlayer != NO_PLAYER)
		{
			eFreeBuilding = ((BuildingTypes)(GC.getCivilizationInfo(GET_PLAYER(ePlayer).getCivilizationType()).getCivilizationBuildings(kBuilding.getFreeBuildingClass())));
		}
		else
		{
			eFreeBuilding = (BuildingTypes)GC.getBuildingClassInfo((BuildingClassTypes)kBuilding.getFreeBuildingClass()).getDefaultBuildingIndex();
		}

		if (NO_BUILDING != eFreeBuilding)
		{
			// MOD - START - Improved Civilopedia
			const CvBuildingInfo& kFreeBuilding = GC.getBuildingInfo(eFreeBuilding);
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_FREE_IN_CITY", CvWString(kFreeBuilding.getType()).GetCString(), kFreeBuilding.getTextKeyWide()));
			// MOD - END - Improved Civilopedia
		}
	}

	if (kBuilding.getFreePromotion() != NO_PROMOTION)
	{
		// MOD - START - Improved Civilopedia
		const CvPromotionInfo& kFreePromotion = GC.getPromotionInfo((PromotionTypes)kBuilding.getFreePromotion());
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_FREE_PROMOTION", CvWString(kFreePromotion.getType()).GetCString(), kFreePromotion.getTextKeyWide()));
		// MOD - END - Improved Civilopedia
	}

	if (kBuilding.getCivicOption() != NO_CIVICOPTION)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_ENABLES_CIVICS", GC.getCivicOptionInfo((CivicOptionTypes)(kBuilding.getCivicOption())).getTextKeyWide()));
	}

	if (kBuilding.isPower())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_PROVIDES_POWER"));

		/* original bts code
		if (kBuilding.isDirtyPower() && (GC.getDefineINT("DIRTY_POWER_HEALTH_CHANGE") != 0))
		{
			szTempBuffer.Format(L" (+%d%c)", abs(GC.getDefineINT("DIRTY_POWER_HEALTH_CHANGE")), ((GC.getDefineINT("DIRTY_POWER_HEALTH_CHANGE") > 0) ? gDLL->getSymbolID(HEALTHY_CHAR): gDLL->getSymbolID(UNHEALTHY_CHAR)));
			szBuffer.append(szTempBuffer);
		} */
		// K-Mod. Also include base health change from power.
		int iPowerHealth = GC.getDefineINT("POWER_HEALTH_CHANGE") + (kBuilding.isDirtyPower() ? GC.getDefineINT("DIRTY_POWER_HEALTH_CHANGE") : 0);
		if (iPowerHealth)
		{
			szTempBuffer.Format(L" (+%d%c)", abs(iPowerHealth), iPowerHealth > 0 ? gDLL->getSymbolID(HEALTHY_CHAR): gDLL->getSymbolID(UNHEALTHY_CHAR));
			szBuffer.append(szTempBuffer);
		}
		// K-Mod end
	}

	if (kBuilding.isAreaCleanPower())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_PROVIDES_AREA_CLEAN_POWER"));
	}

	if (kBuilding.isAreaBorderObstacle())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_BORDER_OBSTACLE"));
	}

	for (int iI = 0; iI < GC.getNumVoteSourceInfos(); ++iI)
	{
		if (kBuilding.getVoteSourceType() == (VoteSourceTypes)iI)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_DIPLO_VOTE", GC.getVoteSourceInfo((VoteSourceTypes)iI).getTextKeyWide()));
		}
	}

	if (kBuilding.isForceTeamVoteEligible())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_ELECTION_ELIGIBILITY"));
	}

	if (kBuilding.isCapital())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_CAPITAL"));
	}

	if (kBuilding.isGovernmentCenter())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_REDUCES_MAINTENANCE"));
	}

	if (kBuilding.isGoldenAge())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_GOLDEN_AGE"));
	}

	if (kBuilding.isAllowsNukes())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_PROJECT_ENABLES_NUKES"));
	}

	if (kBuilding.isMapCentering())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_CENTERS_MAP"));
	}

	if (kBuilding.isNoUnhappiness())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_NO_UNHAPPY"));
	}
/*
** K-Mod, 27/dec/10, karadoc
** replaced NoUnhealthyPopulation with UnhealthyPopulationModifier
*/
	/* original bts code
	if (kBuilding.isNoUnhealthyPopulation())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_NO_UNHEALTHY_POP"));
	}
	*/
	if (kBuilding.getUnhealthyPopulationModifier() != 0)
	{
		// If the modifier is less than -100, display the old NoUnhealth. text
		// Note: this could be techinically inaccurate if we combine this modifier with a positive modifier
		if (kBuilding.getUnhealthyPopulationModifier() <= -100)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_CIVIC_NO_POP_UNHEALTHY"));
		}
		else
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_UNHEALTHY_POP_MODIFIER", kBuilding.getUnhealthyPopulationModifier()));
		}
	}
/*
** K-Mod end
*/
	if (kBuilding.isBuildingOnlyHealthy())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_NO_UNHEALTHY_BUILDINGS"));
	}

	if (kBuilding.getGreatPeopleRateModifier() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_BIRTH_RATE_MOD", kBuilding.getGreatPeopleRateModifier()));
	}

	if (kBuilding.getGreatGeneralRateModifier() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_GENERAL_RATE_MOD", kBuilding.getGreatGeneralRateModifier()));
	}

	if (kBuilding.getDomesticGreatGeneralRateModifier() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_DOMESTIC_GREAT_GENERAL_MODIFIER", kBuilding.getDomesticGreatGeneralRateModifier()));
	}

	if (kBuilding.getGlobalGreatPeopleRateModifier() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_BIRTH_RATE_MOD_ALL_CITIES", kBuilding.getGlobalGreatPeopleRateModifier()));
	}

	if (kBuilding.getAnarchyModifier() != 0)
	{
		if (-100 == kBuilding.getAnarchyModifier())
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_NO_ANARCHY"));
		}
		else
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_ANARCHY_MOD", kBuilding.getAnarchyModifier()));

			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_ANARCHY_TIMER_MOD", kBuilding.getAnarchyModifier()));
		}
	}

	if (kBuilding.getGoldenAgeModifier() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_GOLDENAGE_MOD", kBuilding.getGoldenAgeModifier()));
	}

	if (kBuilding.getGlobalHurryModifier() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_HURRY_MOD", kBuilding.getGlobalHurryModifier()));
	}

	if (kBuilding.getFreeExperience() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_FREE_XP_UNITS", kBuilding.getFreeExperience()));
	}

	if (kBuilding.getGlobalFreeExperience() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_FREE_XP_ALL_CITIES", kBuilding.getGlobalFreeExperience()));
	}

	if (kBuilding.getFoodKept() > 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_STORES_FOOD", kBuilding.getFoodKept()));
	}

	if (kBuilding.getAirlift() > 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_AIRLIFT", kBuilding.getAirlift()));
	}

	if (kBuilding.getAirModifier() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_AIR_DAMAGE_MOD", kBuilding.getAirModifier()));
	}

	if (kBuilding.getAirUnitCapacity() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_AIR_UNIT_CAPACITY", kBuilding.getAirUnitCapacity()));
	}

	if (kBuilding.getNukeModifier() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_NUKE_DAMAGE_MOD", kBuilding.getNukeModifier()));
	}

	if (kBuilding.getNukeExplosionRand() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_NUKE_EXPLOSION_CHANCE"));
	}

	if (kBuilding.getFreeSpecialist() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_FREE_SPECIALISTS", kBuilding.getFreeSpecialist()));
	}

	if (kBuilding.getAreaFreeSpecialist() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_FREE_SPECIALISTS_CONT", kBuilding.getAreaFreeSpecialist()));
	}

	if (kBuilding.getGlobalFreeSpecialist() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_FREE_SPECIALISTS_ALL_CITIES", kBuilding.getGlobalFreeSpecialist()));
	}

	if (kBuilding.getMaintenanceModifier() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_MAINT_MOD", kBuilding.getMaintenanceModifier()));
	}

	if (kBuilding.getHurryAngerModifier() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_HURRY_ANGER_MOD", kBuilding.getHurryAngerModifier()));
	}

	if (kBuilding.getWarWearinessModifier() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_WAR_WEAR_MOD", kBuilding.getWarWearinessModifier()));
	}

	if (kBuilding.getGlobalWarWearinessModifier() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_WAR_WEAR_MOD_ALL_CITIES", kBuilding.getGlobalWarWearinessModifier()));
	}

	if (kBuilding.getEnemyWarWearinessModifier() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_ENEMY_WAR_WEAR", kBuilding.getEnemyWarWearinessModifier()));
	}

	// MOD - START - Unlimited Conscription
	if (kBuilding.isGlobalUnlimitedConscript())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_UNLIMITED_CONSCRIPTION_ALL_CITIES", kBuilding.getWarConscript()));
	}
	// MOD - END - Unlimited Conscription

	// MOD - START - Wartime Conscription
	if (kBuilding.getWarConscript() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_WARTIME_CONSCRIPTION", kBuilding.getWarConscript()));
	}

	if (kBuilding.getGlobalWarConscript() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_WARTIME_CONSCRIPTION_ALL_CITIES", kBuilding.getGlobalWarConscript()));
	}
	// MOD - END - Wartime Conscription

	if (kBuilding.getHealRateChange() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_HEAL_MOD", kBuilding.getHealRateChange()));
	}

	if (kBuilding.getAreaHealth() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_HEALTH_CHANGE_CONT", abs(kBuilding.getAreaHealth()), ((kBuilding.getAreaHealth() > 0) ? gDLL->getSymbolID(HEALTHY_CHAR): gDLL->getSymbolID(UNHEALTHY_CHAR))));
	}

	if (kBuilding.getGlobalHealth() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_HEALTH_CHANGE_ALL_CITIES", abs(kBuilding.getGlobalHealth()), ((kBuilding.getGlobalHealth() > 0) ? gDLL->getSymbolID(HEALTHY_CHAR): gDLL->getSymbolID(UNHEALTHY_CHAR))));
	}
/************************************************************************************************/
/* UNOFFICIAL_PATCH                       08/28/09                             jdog5000         */
/*                                                                                              */
/* Bugfix                                                                                       */
/************************************************************************************************/
/* original bts code
	if (kBuilding.getAreaHappiness() > 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_HAPPY_CHANGE_CONT", kBuilding.getAreaHappiness(), ((kBuilding.getAreaHappiness() > 0) ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR))));
	}

	if (kBuilding.getGlobalHappiness() > 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_HAPPY_CHANGE_ALL_CITIES", kBuilding.getGlobalHappiness(), ((kBuilding.getGlobalHappiness() > 0) ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR))));
	}

	if (kBuilding.getStateReligionHappiness() > 0)
	{
		if (kBuilding.getReligionType() != NO_RELIGION)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_RELIGION_HAPPINESS", kBuilding.getStateReligionHappiness(), ((kBuilding.getStateReligionHappiness() > 0) ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR)), GC.getReligionInfo((ReligionTypes)(kBuilding.getReligionType())).getChar()));
		}
	}
*/
	// Use absolute value with unhappy face
	if (kBuilding.getAreaHappiness() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_HAPPY_CHANGE_CONT", abs(kBuilding.getAreaHappiness()), ((kBuilding.getAreaHappiness() > 0) ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR))));
	}

	if (kBuilding.getGlobalHappiness() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_HAPPY_CHANGE_ALL_CITIES", abs(kBuilding.getGlobalHappiness()), ((kBuilding.getGlobalHappiness() > 0) ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR))));
	}

	if (kBuilding.getStateReligionHappiness() != 0)
	{
		if (kBuilding.getReligionType() != NO_RELIGION)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_RELIGION_HAPPINESS", abs(kBuilding.getStateReligionHappiness()), ((kBuilding.getStateReligionHappiness() > 0) ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR)), GC.getReligionInfo((ReligionTypes)(kBuilding.getReligionType())).getChar()));
		}
	}
/************************************************************************************************/
/* UNOFFICIAL_PATCH                        END                                                  */
/************************************************************************************************/

	// MOD - START - Epidemics
	// Epidemic system by Mexico
	if (kBuilding.getEpidemicModifier() != 0)
	{
		szTempBuffer.Format(L"%+d", (kBuilding.getEpidemicModifier() * 100) / GC.getDefineINT("EPIDEMIC_RAND_BASE"));
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_CITY_EPIDEMIC_MODIFIER", szTempBuffer.GetCString()));
	}

	if (kBuilding.getGlobalEpidemicModifier() != 0)
	{
		szTempBuffer.Format(L"%+d", (kBuilding.getGlobalEpidemicModifier() * 100) / GC.getDefineINT("EPIDEMIC_RAND_BASE"));
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_GLOBAL_EPIDEMIC_MODIFIER", szTempBuffer.GetCString()));
	}
	// MOD - END - Epidemics

	if (kBuilding.getWorkerSpeedModifier() != 0)
	{
		szBuffer.append(NEWLINE);
		// MOD - START - Negative Worker Speed Modifiers
		//szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_WORKER_MOD", kBuilding.getWorkerSpeedModifier()));
		szBuffer.append(gDLL->getText(kBuilding.getWorkerSpeedModifier() >= 0 ? "TXT_KEY_WORKER_SPEED_MODIFIER_FASTER" : "TXT_KEY_WORKER_SPEED_MODIFIER_SLOWER", abs(kBuilding.getWorkerSpeedModifier())));
		// MOD - END - Negative Worker Speed Modifiers
	}

	if (kBuilding.getMilitaryProductionModifier() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_MILITARY_MOD", kBuilding.getMilitaryProductionModifier()));
	}

	if (kBuilding.getSpaceProductionModifier() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_SPACESHIP_MOD", kBuilding.getSpaceProductionModifier()));
	}

	if (kBuilding.getGlobalSpaceProductionModifier() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_SPACESHIP_MOD_ALL_CITIES", kBuilding.getGlobalSpaceProductionModifier()));
	}

	if (kBuilding.getTradeRoutes() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_TRADE_ROUTES", kBuilding.getTradeRoutes()));
	}

	if (kBuilding.getCoastalTradeRoutes() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_COASTAL_TRADE_ROUTES", kBuilding.getCoastalTradeRoutes()));
	}

	if (kBuilding.getGlobalTradeRoutes() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_TRADE_ROUTES_ALL_CITIES", kBuilding.getGlobalTradeRoutes()));
	}

	if (kBuilding.getTradeRouteModifier() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_TRADE_ROUTE_MOD", kBuilding.getTradeRouteModifier()));
	}

	if (kBuilding.getForeignTradeRouteModifier() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_FOREIGN_TRADE_ROUTE_MOD", kBuilding.getForeignTradeRouteModifier()));
	}

	if (kBuilding.getGlobalPopulationChange() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_GLOBAL_POP", kBuilding.getGlobalPopulationChange()));
	}

	if (kBuilding.getFreeTechs() != 0)
	{
		if (kBuilding.getFreeTechs() == 1)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_FREE_TECH"));
		}
		else
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_FREE_TECHS", kBuilding.getFreeTechs()));
		}
	}

	// MOD - START - Obsolete Defense
	/*
	if (kBuilding.getDefenseModifier() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_DEFENSE_MOD", kBuilding.getDefenseModifier()));
	}

	if (kBuilding.getBombardDefenseModifier() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_BOMBARD_DEFENSE_MOD", -kBuilding.getBombardDefenseModifier()));
	}
	*/
	int defenseModifier = kBuilding.getDefenseModifier() + kBuilding.getObsoleteSafeDefenseModifier();
	if (defenseModifier != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_DEFENSE_MOD", defenseModifier));
	}
	
	if (kBuilding.getNaturalDefenseModifier() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_NATURAL_DEFENSE_MOD", kBuilding.getNaturalDefenseModifier()));
	}	

	int bombardDefenseModifier = kBuilding.getBombardDefenseModifier() + kBuilding.getObsoleteSafeBombardDefenseModifier();
	if (bombardDefenseModifier != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_BOMBARD_DEFENSE_MOD", -bombardDefenseModifier));
	}
	// MOD - END - Obsolete Defense

	if (kBuilding.getAllCityDefenseModifier() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_DEFENSE_MOD_ALL_CITIES", kBuilding.getAllCityDefenseModifier()));
	}

	// MOD - START - Defense Modifiers (Mexico)
	// MOD - START - Relation Caching
	for (int iRel = 0; iRel < kBuilding.getNumRelatedUnitCombats(); iRel++)
	{
		UnitCombatTypes eLoopUnitCombat = (UnitCombatTypes)kBuilding.getRelatedUnitCombats(iRel);

		if (kBuilding.getUnitCombatDefenseModifier(eLoopUnitCombat) != 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_COMBAT_DEFENSE", kBuilding.getUnitCombatDefenseModifier(eLoopUnitCombat), GC.getUnitCombatInfo(eLoopUnitCombat).getTextKeyWide()));
		}
	}

	for (int iRel = 0; iRel < kBuilding.getNumRelatedUnitClasses(); iRel++)
	{
		UnitClassTypes eLoopUnitClass = (UnitClassTypes)kBuilding.getRelatedUnitClasses(iRel);

		if (kBuilding.getUnitClassDefenseModifier(eLoopUnitClass) != 0)
		{
			// Question: If this is defense against some other civilization's units of this class,
			//           why would we use our own civ's unit for the description?
			//           Wouldn't the generic unit class description be more appropriate here?
			UnitTypes eLoopUnit;

			if (ePlayer != NO_PLAYER)
			{
				eLoopUnit = (UnitTypes)GC.getCivilizationInfo(GET_PLAYER(ePlayer).getCivilizationType()).getCivilizationUnits(eLoopUnitClass);
			}
			else
			{
				eLoopUnit = (UnitTypes)GC.getUnitClassInfo(eLoopUnitClass).getDefaultUnitIndex();
			}

			if (eLoopUnit != NO_UNIT)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_CLASS_DEFENSE", kBuilding.getUnitClassDefenseModifier(eLoopUnitClass), GC.getUnitInfo(eLoopUnit).getDescription(), GC.getUnitClassInfo(eLoopUnitClass).getTextKeyWide()));
			}
		}
	}
	// MOD - END - Relation Caching
	// MOD - END - Defense Modifiers (Mexico)

	if (kBuilding.getEspionageDefenseModifier() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_ESPIONAGE_DEFENSE_MOD", kBuilding.getEspionageDefenseModifier()));
/************************************************************************************************/
/* UNOFFICIAL_PATCH                       12/07/09                      Afforess & jdog5000     */
/*                                                                                              */
/* Bugfix                                                                                       */
/************************************************************************************************/
/* original BTS code
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_EXPOSE_SPIES"));
*/		
		if (kBuilding.getEspionageDefenseModifier() > 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_UNIT_EXPOSE_SPIES"));
		}
/************************************************************************************************/
/* UNOFFICIAL_PATCH                        END                                                  */
/************************************************************************************************/
	}

	setYieldChangeHelp(szBuffer, gDLL->getText("TXT_KEY_BUILDING_WATER_PLOTS").c_str(), L": ", L"", kBuilding.getSeaPlotYieldChangeArray());

	setYieldChangeHelp(szBuffer, gDLL->getText("TXT_KEY_BUILDING_RIVER_PLOTS").c_str(), L": ", L"", kBuilding.getRiverPlotYieldChangeArray());

	setYieldChangeHelp(szBuffer, gDLL->getText("TXT_KEY_BUILDING_WATER_PLOTS_ALL_CITIES").c_str(), L": ", L"", kBuilding.getGlobalSeaPlotYieldChangeArray());

	setYieldChangeHelp(szBuffer, L"", L"", gDLL->getText("TXT_KEY_BUILDING_WITH_POWER").c_str(), kBuilding.getPowerYieldModifierArray(), true);

	setYieldChangeHelp(szBuffer, L"", L"", gDLL->getText("TXT_KEY_BUILDING_ALL_CITIES_THIS_CONTINENT").c_str(), kBuilding.getAreaYieldModifierArray(), true);

	setYieldChangeHelp(szBuffer, L"", L"", gDLL->getText("TXT_KEY_BUILDING_ALL_CITIES").c_str(), kBuilding.getGlobalYieldModifierArray(), true);

	setCommerceChangeHelp(szBuffer, L"", L"", gDLL->getText("TXT_KEY_BUILDING_ALL_CITIES").c_str(), kBuilding.getGlobalCommerceModifierArray(), true);

	// MOD - START - Advanced Yield and Commerce Effects
	//setCommerceChangeHelp(szBuffer, L"", L"", gDLL->getText("TXT_KEY_BUILDING_PER_SPECIALIST_ALL_CITIES").c_str(), kBuilding.getSpecialistExtraCommerceArray());
	// MOD - END - Advanced Yield and Commerce Effects

/*
** K-Mod, 30/dec/10, karadoc
** Changed to always say state religion, rather than the particular religion that happens to be the current state religion.
*/
	/* original bts code
	if (ePlayer != NO_PLAYER && GET_PLAYER(ePlayer).getStateReligion() != NO_RELIGION)
	{
		szTempBuffer = gDLL->getText("TXT_KEY_BUILDING_FROM_ALL_REL_BUILDINGS", GC.getReligionInfo(GET_PLAYER(ePlayer).getStateReligion()).getChar());
	}
	else
	{
		szTempBuffer = gDLL->getText("TXT_KEY_BUILDING_STATE_REL_BUILDINGS");
	}*/
	szTempBuffer = gDLL->getText("TXT_KEY_BUILDING_STATE_REL_BUILDINGS");
/*
** K-Mod end
*/
	setCommerceChangeHelp(szBuffer, L"", L"", szTempBuffer, kBuilding.getStateReligionCommerceArray());

	for (int iI = 0; iI < NUM_COMMERCE_TYPES; ++iI)
	{
		if (kBuilding.getCommerceHappiness(iI) != 0)
		{
			szBuffer.append(NEWLINE);
/************************************************************************************************/
/* UNOFFICIAL_PATCH                       08/28/09                             jdog5000         */
/*                                                                                              */
/* Bugfix                                                                                       */
/************************************************************************************************/
/* original bts code
			szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_PER_LEVEL", ((kBuilding.getCommerceHappiness(iI) > 0) ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR)), (100 / kBuilding.getCommerceHappiness(iI)), GC.getCommerceInfo((CommerceTypes)iI).getChar()));
*/
			// Use absolute value with unhappy face
			szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_PER_LEVEL", ((kBuilding.getCommerceHappiness(iI) > 0) ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR)), abs(100 / kBuilding.getCommerceHappiness(iI)), GC.getCommerceInfo((CommerceTypes)iI).getChar()));
/************************************************************************************************/
/* UNOFFICIAL_PATCH                        END                                                  */
/************************************************************************************************/
		}

		if (kBuilding.isCommerceFlexible(iI) != 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_ADJUST_COMM_RATE", GC.getCommerceInfo((CommerceTypes) iI).getChar()));
		}
	}

	// MOD - START - Relation Caching
	// MOD - START - Advanced Yield and Commerce Effects
	/*
	for (int iI = 0; iI < GC.getNumSpecialistInfos(); ++iI)
	{
		szFirstBuffer = gDLL->getText("TXT_KEY_BUILDING_FROM_IN_ALL_CITIES", GC.getSpecialistInfo((SpecialistTypes) iI).getTextKeyWide());
		setYieldChangeHelp(szBuffer, L"", L"", szFirstBuffer, kBuilding.getSpecialistYieldChangeArray(iI));
	}

	for (int iI = 0; iI < GC.getNumBonusInfos(); ++iI)
	{
		szFirstBuffer = gDLL->getText("TXT_KEY_BUILDING_WITH_BONUS", GC.getBonusInfo((BonusTypes) iI).getTextKeyWide());
		setYieldChangeHelp(szBuffer, L"", L"", szFirstBuffer, kBuilding.getBonusYieldModifierArray(iI), true);
	}
	*/

	setCommerceChangeHelp(szBuffer, L"", L"", gDLL->getText("TXT_KEY_BUILDING_PER_SPECIALIST_ALL_CITIES").c_str(), kBuilding.getAllSpecialistCommerceArray());

	for (int iRel = 0; iRel < kBuilding.getNumRelatedSpecialists(); iRel++)
	{
		SpecialistTypes eLoopSpecialist = (SpecialistTypes)kBuilding.getRelatedSpecialists(iRel);

		szFirstBuffer = gDLL->getText("TXT_KEY_BUILDING_FROM_SOURCE_ALL_CITIES", GC.getSpecialistInfo(eLoopSpecialist).getTextKeyWide());
		int aiYieldChanges[NUM_YIELD_TYPES] = {};
		kBuilding.getGlobalSpecialistYieldChangeArray(eLoopSpecialist, aiYieldChanges);
		setYieldChangeHelp(szBuffer, L"", L"", szFirstBuffer, aiYieldChanges);

		int aiCommerceChanges[NUM_COMMERCE_TYPES] = {};
		kBuilding.getGlobalSpecialistCommerceChangeArray(eLoopSpecialist, aiCommerceChanges);
		setCommerceChangeHelp(szBuffer, L"", L"", szFirstBuffer, aiCommerceChanges);
	}

	if ((kBuilding.getNumGlobalBuildingYieldChanges() < 20) && (kBuilding.getNumGlobalBuildingCommerceChanges() < 20))
	{	
		for (int iRel = 0; iRel < kBuilding.getNumRelatedBuildingClasses(); iRel++)
		{
			BuildingClassTypes eLoopBuildingClass = (BuildingClassTypes)kBuilding.getRelatedBuildingClasses(iRel);

			szFirstBuffer = gDLL->getText("TXT_KEY_BUILDING_FROM_SOURCE_ALL_CITIES", GC.getBuildingClassInfo(eLoopBuildingClass).getTextKeyWide());
			int aiYieldChanges[NUM_YIELD_TYPES] = {};
			kBuilding.getGlobalBuildingYieldChangeArray(eLoopBuildingClass, aiYieldChanges);
			setYieldChangeHelp(szBuffer, L"", L"", szFirstBuffer, aiYieldChanges);

			int aiCommerceChanges[NUM_COMMERCE_TYPES] = {};
			kBuilding.getGlobalBuildingCommerceChangeArray(eLoopBuildingClass, aiCommerceChanges);
			setCommerceChangeHelp(szBuffer, L"", L"", szFirstBuffer, aiCommerceChanges);
		}
	}

	for (int iRel = 0; iRel < kBuilding.getNumRelatedImprovements(); iRel++)
	{
		ImprovementTypes eLoopImprovement = (ImprovementTypes)kBuilding.getRelatedImprovements(iRel);

		// MOD - START - Unique Improvements
		if (ePlayer == NO_PLAYER || GC.getCivilizationInfo(GET_PLAYER(ePlayer).getCivilizationType()).canProduceImprovement(eLoopImprovement))
		{
		// MOD - END - Unique Improvements
			szFirstBuffer = gDLL->getText("TXT_KEY_BUILDING_FROM_SOURCE_ALL_CITIES", GC.getImprovementInfo(eLoopImprovement).getTextKeyWide());
			int aiYieldChanges[NUM_YIELD_TYPES] = {};
			kBuilding.getGlobalImprovementYieldChangeArray(eLoopImprovement, aiYieldChanges);
			setYieldChangeHelp(szBuffer, L"", L"", szFirstBuffer, aiYieldChanges);
		}
	}

	for (int iRel = 0; iRel < kBuilding.getNumRelatedSpecialists(); iRel++)
	{
		SpecialistTypes eLoopSpecialist = (SpecialistTypes)kBuilding.getRelatedSpecialists(iRel);

		szFirstBuffer = gDLL->getText("TXT_KEY_BUILDING_FROM_SOURCE", GC.getSpecialistInfo(eLoopSpecialist).getTextKeyWide());
		int aiYieldChanges[NUM_YIELD_TYPES] = {};
		kBuilding.getSpecialistYieldChangeArray(eLoopSpecialist, aiYieldChanges);
		setYieldChangeHelp(szBuffer, L"", L"", szFirstBuffer, aiYieldChanges);

		int aiCommerceChanges[NUM_COMMERCE_TYPES] = {};
		kBuilding.getSpecialistCommerceChangeArray(eLoopSpecialist, aiCommerceChanges);
		setCommerceChangeHelp(szBuffer, L"", L"", szFirstBuffer, aiCommerceChanges);
	}

	for (int iRel = 0; iRel < kBuilding.getNumRelatedBuildingClasses(); iRel++)
	{
		BuildingClassTypes eLoopBuildingClass = (BuildingClassTypes)kBuilding.getRelatedBuildingClasses(iRel);

		szFirstBuffer = gDLL->getText("TXT_KEY_BUILDING_FROM_SOURCE", GC.getBuildingClassInfo(eLoopBuildingClass).getTextKeyWide());
		int aiYieldChanges[NUM_YIELD_TYPES] = {};
		kBuilding.getBuildingYieldChangeArray(eLoopBuildingClass, aiYieldChanges);
		setYieldChangeHelp(szBuffer, L"", L"", szFirstBuffer, aiYieldChanges);

		int aiCommerceChanges[NUM_COMMERCE_TYPES] = {};
		kBuilding.getBuildingCommerceChangeArray(eLoopBuildingClass, aiCommerceChanges);
		setCommerceChangeHelp(szBuffer, L"", L"", szFirstBuffer, aiCommerceChanges);
	}

	for (int iRel = 0; iRel < kBuilding.getNumRelatedImprovements(); iRel++)
	{
		ImprovementTypes eLoopImprovement = (ImprovementTypes)kBuilding.getRelatedImprovements(iRel);

		// MOD - START - Unique Improvements
		if (ePlayer == NO_PLAYER || GC.getCivilizationInfo(GET_PLAYER(ePlayer).getCivilizationType()).canProduceImprovement(eLoopImprovement))
		{
		// MOD - END - Unique Improvements
			szFirstBuffer = gDLL->getText("TXT_KEY_BUILDING_FROM_SOURCE", GC.getImprovementInfo(eLoopImprovement).getTextKeyWide());
			int aiYieldChanges[NUM_YIELD_TYPES] = {};
			kBuilding.getImprovementYieldChangeArray(eLoopImprovement, aiYieldChanges);
			setYieldChangeHelp(szBuffer, L"", L"", szFirstBuffer, aiYieldChanges);
		}
	}

	for (int iRel = 0; iRel < kBuilding.getNumRelatedBonuses(); iRel++)
	{
		BonusTypes eLoopBonus = (BonusTypes)kBuilding.getRelatedBonuses(iRel);

		szFirstBuffer = gDLL->getText("TXT_KEY_BUILDING_WITH_BONUS", GC.getBonusInfo(eLoopBonus).getTextKeyWide());
		int aiYieldModifiers[NUM_YIELD_TYPES] = {};
		kBuilding.getBonusYieldModifierArray(eLoopBonus, aiYieldModifiers);
		setYieldChangeHelp(szBuffer, L"", L"", szFirstBuffer, aiYieldModifiers, true);
	}
	// MOD - END - Advanced Yield and Commerce Effects

	for (int iRel = 0; iRel < kBuilding.getNumRelatedReligions(); iRel++)
	{
		ReligionTypes eLoopReligion = (ReligionTypes)kBuilding.getRelatedReligions(iRel);

		if (kBuilding.getReligionChange(eLoopReligion) > 0)
		{
			szTempBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_BUILDING_SPREADS_RELIGION", GC.getReligionInfo(eLoopReligion).getChar()).c_str());
			szBuffer.append(szTempBuffer);
		}
	}

	for (int iRel = 0; iRel < kBuilding.getNumRelatedSpecialists(); iRel++)
	{
		SpecialistTypes eLoopSpecialist = (SpecialistTypes)kBuilding.getRelatedSpecialists(iRel);

		if (kBuilding.getSpecialistCount(eLoopSpecialist) > 0)
		{
			if (kBuilding.getSpecialistCount(eLoopSpecialist) == 1)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_TURN_CITIZEN_INTO", GC.getSpecialistInfo(eLoopSpecialist).getTextKeyWide()));
			}
			else
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_TURN_CITIZENS_INTO", kBuilding.getSpecialistCount(eLoopSpecialist), GC.getSpecialistInfo(eLoopSpecialist).getTextKeyWide()));
			}
		}

		if (kBuilding.getFreeSpecialistCount(eLoopSpecialist) > 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_FREE_SPECIALIST", kBuilding.getFreeSpecialistCount(eLoopSpecialist), GC.getSpecialistInfo(eLoopSpecialist).getTextKeyWide()));
		}
	}

	iLast = 0;

	for (int iRel = 0; iRel < kBuilding.getNumRelatedImprovements(); iRel++)
	{
		ImprovementTypes eLoopImprovement = (ImprovementTypes)kBuilding.getRelatedImprovements(iRel);

		if (kBuilding.getImprovementFreeSpecialist(eLoopImprovement) > 0)
		{
			// MOD - START - Unique Improvements
			if (ePlayer == NO_PLAYER || GC.getCivilizationInfo(GET_PLAYER(ePlayer).getCivilizationType()).canProduceImprovement(eLoopImprovement))
			{
			// MOD - END - Unique Improvements
				szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_BUILDING_IMPROVEMENT_FREE_SPECIALISTS", kBuilding.getImprovementFreeSpecialist(eLoopImprovement)).GetCString() );
				szTempBuffer.Format(L"<link=literal>%s</link>", GC.getImprovementInfo(eLoopImprovement).getDescription());
				setListHelp(szBuffer, szFirstBuffer, szTempBuffer, L", ", (kBuilding.getImprovementFreeSpecialist(eLoopImprovement) != iLast));
				iLast = kBuilding.getImprovementFreeSpecialist(eLoopImprovement);
			}
		}
	}

	iLast = 0;

	for (int iRel = 0; iRel < kBuilding.getNumRelatedBonuses(); iRel++)
	{
		BonusTypes eLoopBonus = (BonusTypes)kBuilding.getRelatedBonuses(iRel);

		if (kBuilding.getBonusHealthChanges(eLoopBonus) != 0)
		{
			szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_BUILDING_HEALTH_HAPPINESS_CHANGE", abs(kBuilding.getBonusHealthChanges(eLoopBonus)), ((kBuilding.getBonusHealthChanges(eLoopBonus) > 0) ? gDLL->getSymbolID(HEALTHY_CHAR): gDLL->getSymbolID(UNHEALTHY_CHAR))).c_str());
			szTempBuffer.Format(L"<link=literal>%s</link>", GC.getBonusInfo(eLoopBonus).getDescription());
			setListHelp(szBuffer, szFirstBuffer, szTempBuffer, L", ", (kBuilding.getBonusHealthChanges(eLoopBonus) != iLast));
			iLast = kBuilding.getBonusHealthChanges(eLoopBonus);
		}
	}

	iLast = 0;

	for (int iRel = 0; iRel < kBuilding.getNumRelatedCivics(); iRel++)
	{
		CivicTypes eLoopCivic = (CivicTypes)kBuilding.getRelatedCivics(iRel);

		int iChange = GC.getCivicInfo(eLoopCivic).getBuildingHealthChanges(kBuilding.getBuildingClassType());
		if (0 != iChange)
		{
			szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_BUILDING_CIVIC_HEALTH_HAPPINESS_CHANGE", abs(iChange), ((iChange > 0) ? gDLL->getSymbolID(HEALTHY_CHAR): gDLL->getSymbolID(UNHEALTHY_CHAR))).c_str());
			szTempBuffer.Format(L"<link=literal>%s</link>", GC.getCivicInfo(eLoopCivic).getDescription());
			setListHelp(szBuffer, szFirstBuffer, szTempBuffer, L", ", (iChange != iLast));
			iLast = iChange;
		}
	}

	iLast = 0;

	for (int iRel = 0; iRel < kBuilding.getNumRelatedBonuses(); iRel++)
	{
		BonusTypes eLoopBonus = (BonusTypes)kBuilding.getRelatedBonuses(iRel);

		if (kBuilding.getBonusHappinessChanges(eLoopBonus) != 0)
		{
			szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_BUILDING_HEALTH_HAPPINESS_CHANGE", abs(kBuilding.getBonusHappinessChanges(eLoopBonus)), ((kBuilding.getBonusHappinessChanges(eLoopBonus) > 0) ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR))).c_str());
			szTempBuffer.Format(L"<link=literal>%s</link>", GC.getBonusInfo(eLoopBonus).getDescription());
			setListHelp(szBuffer, szFirstBuffer, szTempBuffer, L", ", (kBuilding.getBonusHappinessChanges(eLoopBonus) != iLast));
			iLast = kBuilding.getBonusHappinessChanges(eLoopBonus);
		}
	}

	iLast = 0;

	for (int iRel = 0; iRel < kBuilding.getNumRelatedCivics(); iRel++)
	{
		CivicTypes eLoopCivic = (CivicTypes)kBuilding.getRelatedCivics(iRel);

		int iChange = GC.getCivicInfo(eLoopCivic).getBuildingHappinessChanges(kBuilding.getBuildingClassType());
		if (0 != iChange)
		{
			szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_BUILDING_CIVIC_HEALTH_HAPPINESS_CHANGE", abs(iChange), ((iChange > 0) ? gDLL->getSymbolID(HAPPY_CHAR): gDLL->getSymbolID(UNHAPPY_CHAR))).c_str());
			szTempBuffer.Format(L"<link=literal>%s</link>", GC.getCivicInfo(eLoopCivic).getDescription());
			setListHelp(szBuffer, szFirstBuffer, szTempBuffer, L", ", (iChange != iLast));
			iLast = iChange;
		}
	}

	for (int iRel = 0; iRel < kBuilding.getNumRelatedUnitCombats(); iRel++)
	{
		UnitCombatTypes eLoopUnitCombat = (UnitCombatTypes)kBuilding.getRelatedUnitCombats(iRel);

		if (kBuilding.getUnitCombatFreeExperience(eLoopUnitCombat) != 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_FREE_XP", GC.getUnitCombatInfo(eLoopUnitCombat).getTextKeyWide(), kBuilding.getUnitCombatFreeExperience(eLoopUnitCombat)));
		}
	}
	// MOD - END - Relation Caching

	for (int iI = 0; iI < NUM_DOMAIN_TYPES; ++iI)
	{
		if (kBuilding.getDomainFreeExperience(iI) != 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_FREE_XP", GC.getDomainInfo((DomainTypes)iI).getTextKeyWide(), kBuilding.getDomainFreeExperience(iI)));
		}
	}

	for (int iI = 0; iI < NUM_DOMAIN_TYPES; ++iI)
	{
		if (kBuilding.getDomainProductionModifier(iI) != 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_BUILDS_FASTER_DOMAIN", GC.getDomainInfo((DomainTypes)iI).getTextKeyWide(), kBuilding.getDomainProductionModifier(iI)));
		}
	}

	// MOD - START - Relation Caching
	// MOD - START - Building Heal Rate
	for (int iI = 0; iI < NUM_DOMAIN_TYPES; iI++)
	{
		if (kBuilding.getDomainHealRateChanges(iI) != 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_DOMAIN_HEAL_MOD", GC.getDomainInfo((DomainTypes)iI).getTextKeyWide(), kBuilding.getDomainHealRateChanges(iI)));
		}
	}

	for (int iRel = 0; iRel < kBuilding.getNumRelatedUnitCombats(); iRel++)
	{
		UnitCombatTypes eLoopUnitCombat = (UnitCombatTypes)kBuilding.getRelatedUnitCombats(iRel);

		if (kBuilding.getUnitCombatHealRateChanges(eLoopUnitCombat) != 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_UNIT_COMBAT_HEAL_MOD", GC.getUnitCombatInfo(eLoopUnitCombat).getTextKeyWide(), kBuilding.getUnitCombatHealRateChanges(eLoopUnitCombat)));
		}
	}

	for (int iRel = 0; iRel < kBuilding.getNumRelatedUnitClasses(); iRel++)
	{
		UnitClassTypes eLoopUnitClass = (UnitClassTypes)kBuilding.getRelatedUnitClasses(iRel);

		if (kBuilding.getUnitClassHealRateChanges(eLoopUnitClass) != 0)
		{
			UnitTypes eLoopUnit;
			if (ePlayer != NO_PLAYER)
			{
				eLoopUnit = (UnitTypes)GC.getCivilizationInfo(GET_PLAYER(ePlayer).getCivilizationType()).getCivilizationUnits(eLoopUnitClass);
			}
			else
			{
				eLoopUnit = (UnitTypes)GC.getUnitClassInfo(eLoopUnitClass).getDefaultUnitIndex();
			}

			if (eLoopUnit != NO_UNIT)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_UNIT_CLASS_HEAL_MOD", GC.getUnitInfo(eLoopUnit).getTextKeyWide(), kBuilding.getUnitClassHealRateChanges(eLoopUnitClass)));
			}
		}
	}
	// MOD - END - Building Heal Rate

	// MOD - START - Show only player-specific unit (or default) in Building Help (04.06.2010 by Mexico)
	// MOD - START - Advanced Building Prerequisites
	bool bFirstAnd = true;
	bool bFirstOr = true;

	CvWString szAndBuffer;
	CvWString szOrBuffer;

	for (int iRel = 0; iRel < kBuilding.getNumRelatedUnitClasses(); iRel++)
	{
		UnitClassTypes eLoopUnitClass = (UnitClassTypes)kBuilding.getRelatedUnitClasses(iRel);
		UnitTypes eLoopUnit;

		if (ePlayer == NO_PLAYER)
		{
			eLoopUnit = ((UnitTypes)(GC.getUnitClassInfo(eLoopUnitClass).getDefaultUnitIndex()));
		}
		else
		{
			eLoopUnit = (UnitTypes)GC.getCivilizationInfo(GC.getGameINLINE().getActiveCivilizationType()).getCivilizationUnits(eLoopUnitClass);
		}

		if (eLoopUnit != NO_UNIT)
		{
			CvUnitInfo& kLoopUnit = GC.getUnitInfo(eLoopUnit);

			/* old code disabled
			if (kLoopUnit.getPrereqBuilding() == eBuilding)
			{
				szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_BUILDING_REQUIRED_TO_TRAIN").c_str());
				szTempBuffer.Format( SETCOLR L"<link=literal>%s</link>" ENDCOLR , TEXT_COLOR("COLOR_UNIT_TEXT"), kLoopUnit.getDescription());
				setListHelp(szBuffer, szFirstBuffer, szTempBuffer, L", ", bFirst);
				bFirst = false;
			}
			*/
			for (int iPre = 0; iPre < kLoopUnit.getNumPrereqAndBuildingClasses(); iPre++)
			{
				BuildingClassTypes eLoopPrereqBuildingClass = (BuildingClassTypes)kLoopUnit.getPrereqAndBuildingClasses(iPre);
				if (eLoopPrereqBuildingClass == eBuildingClass)
				{
					szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_BUILDING_REQUIRED_TO_TRAIN").c_str());
					szTempBuffer.Format(SETCOLR L"<link=literal>%s</link>" ENDCOLR , TEXT_COLOR("COLOR_UNIT_TEXT"), kLoopUnit.getDescription());
					setListHelp(szAndBuffer, szFirstBuffer, szTempBuffer, L", ", bFirstAnd);
					bFirstAnd = false;
					break;
				}
			}

			for (int iPre = 0; iPre < kLoopUnit.getNumPrereqOrBuildingClasses(); iPre++)
			{
				BuildingClassTypes eLoopPrereqBuildingClass = (BuildingClassTypes)kLoopUnit.getPrereqOrBuildingClasses(iPre);
				if (eLoopPrereqBuildingClass == eBuildingClass)
				{
					szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_BUILDING_OR_REQUIRED_TO_TRAIN").c_str());
					szTempBuffer.Format(SETCOLR L"<link=literal>%s</link>" ENDCOLR , TEXT_COLOR("COLOR_UNIT_TEXT"), kLoopUnit.getDescription());
					setListHelp(szOrBuffer, szFirstBuffer, szTempBuffer, L", ", bFirstOr);
					bFirstOr = false;
					break;
				}
			}
		}
	}
	szBuffer.append(szAndBuffer);
	szBuffer.append(szOrBuffer);
	// MOD - END - Advanced Building Prerequisites

	bFirst = true;

	for (int iRel = 0; iRel < kBuilding.getNumRelatedUnitClasses(); iRel++)
	{
		UnitClassTypes eLoopUnitClass = (UnitClassTypes)kBuilding.getRelatedUnitClasses(iRel);
		UnitTypes eLoopUnit;

		if (ePlayer == NO_PLAYER)
		{
			eLoopUnit = ((UnitTypes)(GC.getUnitClassInfo(eLoopUnitClass).getDefaultUnitIndex()));
		}
		else
		{
			eLoopUnit = (UnitTypes)GC.getCivilizationInfo(GC.getGameINLINE().getActiveCivilizationType()).getCivilizationUnits(eLoopUnitClass);
		}

		if (eLoopUnit != NO_UNIT)
		{
			if (GC.getUnitInfo(eLoopUnit).getBuildings(eBuilding) || GC.getUnitInfo(eLoopUnit).getForceBuildings(eBuilding))
			{
				szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_UNIT_REQUIRED_TO_BUILD").c_str());
				szTempBuffer.Format( SETCOLR L"<link=literal>%s</link>" ENDCOLR , TEXT_COLOR("COLOR_UNIT_TEXT"), GC.getUnitInfo(eLoopUnit).getDescription());
				setListHelp(szBuffer, szFirstBuffer, szTempBuffer, L", ", bFirst);
				bFirst = false;
			}
		}
	}
	// MOD - END - Show only player-specific unit (or default) in Building Help

	iLast = 0;

	for (int iRel = 0; iRel < kBuilding.getNumRelatedBuildingClasses(); iRel++)
	{
		BuildingClassTypes eLoopBuildingClass = (BuildingClassTypes)kBuilding.getRelatedBuildingClasses(iRel);

		if (kBuilding.getBuildingHappinessChanges(eLoopBuildingClass) != 0)
		{
/************************************************************************************************/
/* UNOFFICIAL_PATCH                       08/28/09                             jdog5000         */
/*                                                                                              */
/* Bugfix                                                                                       */
/************************************************************************************************/
/* original bts code
			szTempBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_BUILDING_HAPPINESS_CHANGE", kBuilding.getBuildingHappinessChanges(iI),
				((kBuilding.getBuildingHappinessChanges(iI) > 0) ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR))).c_str());
*/
			// Use absolute value with unhappy face
			szTempBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_BUILDING_HAPPINESS_CHANGE", abs(kBuilding.getBuildingHappinessChanges(eLoopBuildingClass)),
				((kBuilding.getBuildingHappinessChanges(eLoopBuildingClass) > 0) ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR))).c_str());
/************************************************************************************************/
/* UNOFFICIAL_PATCH                        END                                                  */
/************************************************************************************************/
			CvWString szBuilding;
			if (NO_PLAYER != ePlayer)
			{
				BuildingTypes ePlayerBuilding = ((BuildingTypes)(GC.getCivilizationInfo(GET_PLAYER(ePlayer).getCivilizationType()).getCivilizationBuildings(eLoopBuildingClass)));
				if (NO_BUILDING != ePlayerBuilding)
				{
					szBuilding.Format(L"<link=literal>%s</link>", GC.getBuildingClassInfo(eLoopBuildingClass).getDescription());
				}
			}
			else
			{
				szBuilding.Format(L"<link=literal>%s</link>", GC.getBuildingClassInfo(eLoopBuildingClass).getDescription());
			}
			setListHelp(szBuffer, szTempBuffer, szBuilding, L", ", (kBuilding.getBuildingHappinessChanges(eLoopBuildingClass) != iLast));
			iLast = kBuilding.getBuildingHappinessChanges(eLoopBuildingClass);
		}
	}
	// MOD - END - Relation Caching

	if (kBuilding.getPowerBonus() != NO_BONUS)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_PROVIDES_POWER_WITH", GC.getBonusInfo((BonusTypes)kBuilding.getPowerBonus()).getTextKeyWide()));

		/* original bts code
		if (kBuilding.isDirtyPower() && (GC.getDefineINT("DIRTY_POWER_HEALTH_CHANGE") != 0))
		{
			szTempBuffer.Format(L" (+%d%c)", abs(GC.getDefineINT("DIRTY_POWER_HEALTH_CHANGE")), ((GC.getDefineINT("DIRTY_POWER_HEALTH_CHANGE") > 0) ? gDLL->getSymbolID(HEALTHY_CHAR): gDLL->getSymbolID(UNHEALTHY_CHAR)));
			szBuffer.append(szTempBuffer);
		} */
		// K-Mod. Also include base health change from power.
		int iPowerHealth = GC.getDefineINT("POWER_HEALTH_CHANGE") + (kBuilding.isDirtyPower() ? GC.getDefineINT("DIRTY_POWER_HEALTH_CHANGE") : 0);
		if (iPowerHealth)
		{
			szTempBuffer.Format(L" (+%d%c)", abs(iPowerHealth), iPowerHealth > 0 ? gDLL->getSymbolID(HEALTHY_CHAR): gDLL->getSymbolID(UNHEALTHY_CHAR));
			szBuffer.append(szTempBuffer);
		}
		// K-Mod end
	}

	bFirst = true;

	// MOD - START - Relation Caching
	for (int iRel = 0; iRel < kBuilding.getNumRelatedBuildingClasses(); iRel++)
	{
		BuildingClassTypes eLoopBuildingClass = (BuildingClassTypes)kBuilding.getRelatedBuildingClasses(iRel);

		if (ePlayer != NO_PLAYER)
		{
			eLoopBuilding = ((BuildingTypes)(GC.getCivilizationInfo(GET_PLAYER(ePlayer).getCivilizationType()).getCivilizationBuildings(eLoopBuildingClass)));
		}
		else
		{
			eLoopBuilding = (BuildingTypes)GC.getBuildingClassInfo(eLoopBuildingClass).getDefaultBuildingIndex();
		}

		if (eLoopBuilding != NO_BUILDING)
		{
			if (GC.getBuildingInfo(eLoopBuilding).isBuildingClassNeededInCity(kBuilding.getBuildingClassType()))
			{
				if ((pCity == NULL) || pCity->canConstruct(eLoopBuilding, false, true))
				{
					szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_BUILDING_REQUIRED_TO_BUILD").c_str());
					szTempBuffer.Format(SETCOLR L"<link=literal>%s</link>" ENDCOLR, TEXT_COLOR("COLOR_BUILDING_TEXT"), GC.getBuildingInfo(eLoopBuilding).getDescription());
					setListHelp(szBuffer, szFirstBuffer, szTempBuffer, L", ", bFirst);
					bFirst = false;
				}
			}
		}
	}
	// MOD - END - Relation Caching

	if (bCivilopediaText)
	{
		// Trait
		for (int i = 0; i < GC.getNumTraitInfos(); ++i)
		{
			if (kBuilding.getProductionTraits((TraitTypes)i) != 0)
			{
				if (kBuilding.getProductionTraits((TraitTypes)i) == 100)
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_DOUBLE_SPEED_TRAIT", GC.getTraitInfo((TraitTypes)i).getTextKeyWide()));
				}
				else
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_PRODUCTION_MODIFIER_TRAIT", kBuilding.getProductionTraits((TraitTypes)i), GC.getTraitInfo((TraitTypes)i).getTextKeyWide()));
				}
			}
		}

		for (int i = 0; i < GC.getNumTraitInfos(); ++i)
		{
			if (kBuilding.getHappinessTraits((TraitTypes)i) != 0)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_HAPPINESS_TRAIT", kBuilding.getHappinessTraits((TraitTypes)i), GC.getTraitInfo((TraitTypes)i).getTextKeyWide()));
			}
		}
	}

	if (bCivilopediaText)
	{
		if (kBuilding.getGreatPeopleUnitClass() != NO_UNITCLASS)
		{
			if (ePlayer != NO_PLAYER)
			{
				eGreatPeopleUnit = ((UnitTypes)(GC.getCivilizationInfo(GET_PLAYER(ePlayer).getCivilizationType()).getCivilizationUnits(kBuilding.getGreatPeopleUnitClass())));
			}
			else
			{
				eGreatPeopleUnit = (UnitTypes)GC.getUnitClassInfo((UnitClassTypes)kBuilding.getGreatPeopleUnitClass()).getDefaultUnitIndex();
			}

			if (eGreatPeopleUnit!= NO_UNIT)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_LIKELY_TO_GENERATE", GC.getUnitInfo(eGreatPeopleUnit).getTextKeyWide()));
			}
		}

		if (kBuilding.getFreeStartEra() != NO_ERA)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_FREE_START_ERA", GC.getEraInfo((EraTypes)kBuilding.getFreeStartEra()).getTextKeyWide()));
		}
	}

	if (!CvWString(kBuilding.getHelp()).empty())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(kBuilding.getHelp());
	}

	buildBuildingRequiresString(szBuffer, eBuilding, bCivilopediaText, bTechChooserText, pCity);

	if (pCity != NULL)
	{
		if (!(GC.getBuildingClassInfo((BuildingClassTypes)(kBuilding.getBuildingClassType())).isNoLimit()))
		{
			if (isWorldWonderClass((BuildingClassTypes)(kBuilding.getBuildingClassType())))
			{
				if (pCity->isWorldWondersMaxed())
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_WORLD_WONDERS_PER_CITY", GC.getDefineINT("MAX_WORLD_WONDERS_PER_CITY")));
				}
			}
			else if (isTeamWonderClass((BuildingClassTypes)(kBuilding.getBuildingClassType())))
			{
				if (pCity->isTeamWondersMaxed())
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_TEAM_WONDERS_PER_CITY", GC.getDefineINT("MAX_TEAM_WONDERS_PER_CITY")));
				}
			}
			else if (isNationalWonderClass((BuildingClassTypes)(kBuilding.getBuildingClassType())))
			{
				if (pCity->isNationalWondersMaxed())
				{
					int iMaxNumWonders = (GC.getGameINLINE().isOption(GAMEOPTION_ONE_CITY_CHALLENGE) && GET_PLAYER(pCity->getOwnerINLINE()).isHuman()) ? GC.getDefineINT("MAX_NATIONAL_WONDERS_PER_CITY_FOR_OCC") : GC.getDefineINT("MAX_NATIONAL_WONDERS_PER_CITY");
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_NATIONAL_WONDERS_PER_CITY", iMaxNumWonders));
				}
			}
			else
			{
				if (pCity->isBuildingsMaxed())
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_NUM_PER_CITY", GC.getDefineINT("MAX_BUILDINGS_PER_CITY")));
				}
			}
		}
	}

	if ((pCity == NULL) || pCity->getNumRealBuilding(eBuilding) < GC.getCITY_MAX_NUM_BUILDINGS())
	{
		// MOD - START - Building Replacement
		// MOD - START - Relation Caching
		for (int iRel = 0; iRel < kBuilding.getNumRelatedBuildingClasses(); iRel++)
		{
			BuildingClassTypes eLoopBuildingClass = (BuildingClassTypes)kBuilding.getRelatedBuildingClasses(iRel);

			if (ePlayer != NO_PLAYER)
			{
				eLoopBuilding = ((BuildingTypes)(GC.getCivilizationInfo(GET_PLAYER(ePlayer).getCivilizationType()).getCivilizationBuildings(eLoopBuildingClass)));
			}
			else
			{
				eLoopBuilding = (BuildingTypes)GC.getBuildingClassInfo(eLoopBuildingClass).getDefaultBuildingIndex();
			}

			if (eLoopBuilding != NO_BUILDING)
			{
				const CvBuildingInfo& kLoopBuilding = GC.getBuildingInfo(eLoopBuilding);

				if (kLoopBuilding.getObsoleteBuildingClass() == eBuildingClass)
				{
					// MOD - START - Improved Civilopedia
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_OBSOLETES", CvWString(kLoopBuilding.getType()).GetCString(), kLoopBuilding.getTextKeyWide()));
					// MOD - END - Improved Civilopedia
				}
			}
		}
		// MOD - END - Relation Caching
		// MOD - END - Building Replacement

		if (!bCivilopediaText)
		{
			if (pCity == NULL)
			{
				if (kBuilding.getProductionCost() > 0)
				{
					szTempBuffer.Format(L"\n%d%c", (ePlayer != NO_PLAYER ? GET_PLAYER(ePlayer).getProductionNeeded(eBuilding) : kBuilding.getProductionCost()), GC.getYieldInfo(YIELD_PRODUCTION).getChar());
					szBuffer.append(szTempBuffer);
				}
			}
			else
			{
		// BUG - Building Actual Effects (edited and moved by K-Mod) - start
				if (bActual && (GC.altKey() || getBugOptionBOOL("MiscHover__BuildingActualEffects", false, "BUG_BUILDING_HOVER_ACTUAL_EFFECTS")) && (pCity->getOwnerINLINE() == GC.getGame().getActivePlayer() || gDLL->getChtLvl() > 0))
				{
					setBuildingNetEffectsHelp(szBuffer, eBuilding, pCity);
				}
		// BUG - Building Actual Effects - end

				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_NUM_TURNS", pCity->getProductionTurnsLeft(eBuilding, ((GC.ctrlKey() || !(GC.shiftKey())) ? 0 : pCity->getOrderQueueLength()))));

				iProduction = pCity->getBuildingProduction(eBuilding);
				
				int iProductionNeeded = pCity->getProductionNeeded(eBuilding);
				if (iProduction > 0)
				{
					szTempBuffer.Format(L" - %d/%d%c", iProduction, iProductionNeeded, GC.getYieldInfo(YIELD_PRODUCTION).getChar());
					szBuffer.append(szTempBuffer);
				}
				else
				{
					szTempBuffer.Format(L" - %d%c", iProductionNeeded, GC.getYieldInfo(YIELD_PRODUCTION).getChar());
					szBuffer.append(szTempBuffer);
				}
			}
		}

		// MOD - START - Relation Caching
		for (int iRel = 0; iRel < kBuilding.getNumRelatedBonuses(); iRel++)
		{
			BonusTypes eLoopBonus = (BonusTypes)kBuilding.getRelatedBonuses(iRel);

			if (kBuilding.getBonusProductionModifier(eLoopBonus) != 0)
			{
				if (pCity != NULL)
				{
					if (pCity->hasBonus(eLoopBonus))
					{
						szBuffer.append(gDLL->getText("TXT_KEY_COLOR_POSITIVE"));
					}
					else
					{
						szBuffer.append(gDLL->getText("TXT_KEY_COLOR_NEGATIVE"));
					}
				}
				if (!bCivilopediaText)
				{
					szBuffer.append(L" (");
				}
				else
				{
					szTempBuffer.Format(L"\n%c", gDLL->getSymbolID(BULLET_CHAR), szTempBuffer.GetCString());
					szBuffer.append(szTempBuffer);
				}
				if (kBuilding.getBonusProductionModifier(eLoopBonus) == 100)
				{
					szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_DOUBLE_SPEED_WITH", GC.getBonusInfo(eLoopBonus).getTextKeyWide()));
				}
				else
				{
					szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_BUILDS_FASTER_WITH", kBuilding.getBonusProductionModifier(eLoopBonus), GC.getBonusInfo(eLoopBonus).getTextKeyWide()));
				}
				if (!bCivilopediaText)
				{
					szBuffer.append(L')');
				}
				if (pCity != NULL)
				{
					szBuffer.append(gDLL->getText("TXT_KEY_COLOR_REVERT"));
				}
			}
		}
		// MOD - END - Relation Caching

		// MOD - START - Building Discontinuation
		if (kBuilding.getDiscontinueTech() != NO_TECH)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_DISCONTINUED_WITH", GC.getTechInfo((TechTypes)kBuilding.getDiscontinueTech()).getTextKeyWide()));
		}
		
		if (kBuilding.getSpecialBuildingType() != NO_SPECIALBUILDING)
		{
			TechTypes eDiscontinueTech = (TechTypes)GC.getSpecialBuildingInfo((SpecialBuildingTypes)kBuilding.getSpecialBuildingType()).getDiscontinueTech();

			if (eDiscontinueTech != NO_TECH)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_DISCONTINUED_WITH", GC.getTechInfo(eDiscontinueTech).getTextKeyWide()));
			}
		}
		// MOD - END - Building Discontinuation

		// MOD - START - Building Replacement
		CivilizationTypes eCivilization = NO_CIVILIZATION;
		if (ePlayer != NO_PLAYER)
		{
			eCivilization = GET_PLAYER(ePlayer).getCivilizationType();
		}

		BuildingClassTypes eObsoleteBuildingClass = (BuildingClassTypes)GC.getBuildingInfo(eBuilding).getObsoleteBuildingClass();
		if (eObsoleteBuildingClass != NO_BUILDINGCLASS)
		{
			BuildingTypes eObsoleteBuilding;

			if (eCivilization == NO_CIVILIZATION)
			{
				eObsoleteBuilding = ((BuildingTypes)(GC.getBuildingClassInfo(eObsoleteBuildingClass).getDefaultBuildingIndex()));
			}
			else
			{
				eObsoleteBuilding = ((BuildingTypes)(GC.getCivilizationInfo(eCivilization).getCivilizationBuildings(eObsoleteBuildingClass)));
			}

			if (eObsoleteBuilding != NO_BUILDING)
			{
				// MOD - START - Improved Civilopedia
				const CvBuildingInfo& kObsoleteWithBuilding = GC.getBuildingInfo(eObsoleteBuilding);
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_OBSOLETE_WITH", CvWString(kObsoleteWithBuilding.getType()).GetCString(), kObsoleteWithBuilding.getTextKeyWide()));
				// MOD - END - Improved Civilopedia

				// MOD - START - Obsolete Defense
				//if (kBuilding.getDefenseModifier() != 0 || kBuilding.getBombardDefenseModifier() != 0)
				if (kBuilding.getObsoleteSafeDefenseModifier() != 0 || kBuilding.getObsoleteSafeBombardDefenseModifier() != 0)
				// MOD - END - Obsolete Defense
				{
					szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_OBSOLETE_EXCEPT"));
				}
			}
		}
		// MOD - END - Building Replacement

		if (kBuilding.getObsoleteTech() != NO_TECH)
		{
			// MOD - START - Improved Civilopedia
			const CvTechInfo& kObsoleteWithTech = GC.getTechInfo((TechTypes) kBuilding.getObsoleteTech());
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_OBSOLETE_WITH", CvWString(kObsoleteWithTech.getType()).GetCString(), kObsoleteWithTech.getTextKeyWide()));
			// MOD - END - Improved Civilopedia

			// MOD - START - Obsolete Defense
			//if (kBuilding.getDefenseModifier() != 0 || kBuilding.getBombardDefenseModifier() != 0)
			if (kBuilding.getObsoleteSafeDefenseModifier() != 0 || kBuilding.getObsoleteSafeBombardDefenseModifier() != 0)
			// MOD - END - Obsolete Defense
			{
				szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_OBSOLETE_EXCEPT"));
			}
		}

		if (kBuilding.getSpecialBuildingType() != NO_SPECIALBUILDING)
		{
			// MOD - START - Improved Civilopedia
			const CvSpecialBuildingInfo& kSpecialBuilding = GC.getSpecialBuildingInfo((SpecialBuildingTypes) kBuilding.getSpecialBuildingType());

			if (kSpecialBuilding.getObsoleteTech() != NO_TECH)
			{
				const CvTechInfo& kObsoleteWithTech = GC.getTechInfo((TechTypes) kSpecialBuilding.getObsoleteTech());
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_OBSOLETE_WITH", CvWString(kObsoleteWithTech.getType()).GetCString(), kObsoleteWithTech.getTextKeyWide()));
			}
			
			if (kSpecialBuilding.isExclusive())
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_EXCLUSIVE", kSpecialBuilding.getTextKeyWide()));	
			}
			// MOD - END - Improved Civilopedia
		}
	}

	// K-Mod. Moved from inside that }, above.
	if (pCity && gDLL->getChtLvl() > 0 && GC.ctrlKey())
	{
		int iBuildingValue = pCity->AI_buildingValue(eBuilding, 0, 0, true);
		szBuffer.append(CvWString::format(L"\nAI Building Value = %d", iBuildingValue));
	}
	//

	if (bStrategyText)
	{
		if (!CvWString(kBuilding.getStrategy()).empty())
		{
			if ((ePlayer == NO_PLAYER) || GET_PLAYER(ePlayer).isOption(PLAYEROPTION_ADVISOR_HELP))
			{
				// MOD - START - Disabled Sid's Tips
				// TODO: Hard-wire a game option for now?
				/*
				szBuffer.append(SEPARATOR);
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_SIDS_TIPS"));
				szBuffer.append(L'\"');
				szBuffer.append(kBuilding.getStrategy());
				szBuffer.append(L'\"');
				*/
				// MOD - END - Disabled Sid's Tips
			}
		}
	}

	if (bCivilopediaText)
	{
		if (eDefaultBuilding == eBuilding)
		{
			// TODO: Improve performance
			for (int iI = 0; iI < GC.getNumBuildingInfos(); ++iI)
			{
				if (iI != eBuilding)
				{
					if (eBuildingClass == GC.getBuildingInfo((BuildingTypes)iI).getBuildingClassType())
					{
						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_REPLACED_BY_BUILDING", GC.getBuildingInfo((BuildingTypes)iI).getTextKeyWide()));
					}
				}
			}
		}
	}

}

void CvGameTextMgr::buildBuildingRequiresString(CvWStringBuffer& szBuffer, BuildingTypes eBuilding, bool bCivilopediaText, bool bTechChooserText, const CvCity* pCity)
{
	// K-mod note. I've made a couple of style adjustments throughout this function to make it easier for me to read & maintain.

	CvBuildingInfo& kBuilding = GC.getBuildingInfo(eBuilding);

	PlayerTypes ePlayer = pCity ? pCity->getOwnerINLINE() : GC.getGameINLINE().getActivePlayer();

	// MOD - START - Building Activation
	if (pCity != NULL)
	{
		if (pCity->getNumActiveBuilding(eBuilding) > 0)
		{
			// The building is active; no need to show requirements text
			return;
		}
		else if (pCity->getNumBuilding(eBuilding) > 0)
		{
			if (pCity->getNumNonObsoleteBuilding(eBuilding) > 0)
			{
				// Indicate that the building is disabled or inactive and explain why
				if (pCity->isDisabledBuilding(eBuilding))
				{
					CvWString szTempBuffer;
					szTempBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_BUILDING_DISABLED").c_str());
					szBuffer.append(szTempBuffer);
				}
				else
				{
					CvWString szTempBuffer;
					szTempBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_BUILDING_INACTIVE").c_str());
					szBuffer.append(szTempBuffer);
				}

				// MOD - START - Holy City Migration
				if (kBuilding.getHolyCity() != NO_RELIGION)
				{
					if (!pCity->isHolyCity((ReligionTypes)(kBuilding.getHolyCity())))
					{
						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_REQUIRES_HOLY_CITY", GC.getReligionInfo((ReligionTypes) kBuilding.getHolyCity()).getChar()));
					}
				}
				// MOD - END - Holy City Migration

				// MOD - START - Advanced Building Prerequisites (Power)
				if (kBuilding.isInactiveWithoutPower())
				{
					if (!pCity->isPower())
					{
						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_REQUIRES_POWER"));
					}
				}
				// MOD - END - Advanced Building Prerequisites (Power)

				// MOD - START - Advanced Building Prerequisites (Religion)
				if (kBuilding.isInactiveWithoutPrereqStateReligion() && ePlayer != NO_PLAYER && kBuilding.getStateReligion() != NO_RELIGION)
				{
					ReligionTypes eReligion = GET_PLAYER(ePlayer).getStateReligion();
					if (eReligion != kBuilding.getStateReligion())
					{
						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_REQUIRES_STRING", GC.getReligionInfo((ReligionTypes)kBuilding.getStateReligion()).getTextKeyWide()));
					}
				}
				// MOD - END - Advanced Building Prerequisites (Religion)

				// MOD - START - Advanced Building Prerequisites (Civic)
				if (kBuilding.isInactiveWithoutCivicPrereqs() && ePlayer != NO_PLAYER)
				{
					for (int iPrereqCivicIndex = 0; iPrereqCivicIndex < kBuilding.getNumPrereqAndCivics(); iPrereqCivicIndex++)
					{
						CivicTypes ePrereqAndCivic = (CivicTypes)kBuilding.getPrereqAndCivics(iPrereqCivicIndex);
						if (!GET_PLAYER(ePlayer).isCivic(ePrereqAndCivic))
						{
							szBuffer.append(NEWLINE);
							szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_REQUIRES_STRING", GC.getCivicInfo(ePrereqAndCivic).getTextKeyWide()));
						}
					}
				}
				// MOD - END - Advanced Building Prerequisites (Civic)
			}
			else
			{
				// Indicate that the building is obsolete
				CvWString szTempBuffer;
				szTempBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_BUILDING_OBSOLETE").c_str());
				szBuffer.append(szTempBuffer);
			}

			return;
		}
	}
	// MOD - END - Building Activation

	if (NULL == pCity || !pCity->canConstruct(eBuilding))
	{
		if (kBuilding.getHolyCity() != NO_RELIGION)
		{
			if (pCity == NULL || !pCity->isHolyCity((ReligionTypes)(kBuilding.getHolyCity())))
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_ACTION_ONLY_HOLY_CONSTRUCT", GC.getReligionInfo((ReligionTypes) kBuilding.getHolyCity()).getChar()));
			}
		}

		bool bFirst = true;

		if (kBuilding.getSpecialBuildingType() != NO_SPECIALBUILDING)
		{
			if ((pCity == NULL) || !(GC.getGameINLINE().isSpecialBuildingValid((SpecialBuildingTypes)(kBuilding.getSpecialBuildingType()))))
			{
				for (int iI = 0; iI < GC.getNumProjectInfos(); ++iI)
				{
					if (GC.getProjectInfo((ProjectTypes)iI).getEveryoneSpecialBuilding() == kBuilding.getSpecialBuildingType())
					{
						CvWString szTempBuffer;
						szTempBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_REQUIRES").c_str());
						CvWString szProject;
						szProject.Format(L"<link=literal>%s</link>", GC.getProjectInfo((ProjectTypes)iI).getDescription());
						setListHelp(szBuffer, szTempBuffer, szProject, gDLL->getText("TXT_KEY_OR").c_str(), bFirst);
						bFirst = false;
					}
				}
			}
		}

		if (!bFirst)
		{
			szBuffer.append(ENDCOLR);
		}

		if (kBuilding.getSpecialBuildingType() != NO_SPECIALBUILDING)
		{
			if ((pCity == NULL) || !(GC.getGameINLINE().isSpecialBuildingValid((SpecialBuildingTypes)(kBuilding.getSpecialBuildingType()))))
			{
				TechTypes eTech = (TechTypes)GC.getSpecialBuildingInfo((SpecialBuildingTypes)kBuilding.getSpecialBuildingType()).getTechPrereqAnyone();
				if (NO_TECH != eTech)
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_REQUIRES_TECH_ANYONE", GC.getTechInfo(eTech).getTextKeyWide()));
				}
			}
		}

		// MOD - START - Relation Caching
		for (int iRel = 0; iRel < kBuilding.getNumRelatedBuildingClasses(); iRel++)
		{
			BuildingClassTypes eLoopBuildingClass = (BuildingClassTypes)kBuilding.getRelatedBuildingClasses(iRel);

/*************************************************************************************************/
/* UNOFFICIAL_PATCH                       06/10/10                           EmperorFool         */
/*                                                                                               */
/* Bugfix                                                                                        */
/*************************************************************************************************/
			// EF: show "Requires Hospital" if "Requires Hospital (x/5)" requirement has been met
			bool bShowedPrereq = false;

			// K-Mod note: I've rearranged the conditions in the following block. Originally it was something like this:
			//if (ePlayer == NO_PLAYER && kBuilding.getPrereqNumOfBuildingClass(eLoopBuildingClass) > 0)
			//else if (ePlayer != NO_PLAYER && GET_PLAYER(ePlayer).getBuildingClassPrereqBuilding(eBuilding, eLoopBuildingClass) > 0)
			//

			// K-Mod. Check that we can actually build this class of building. (Previously this was checked in every single block below.)
			BuildingTypes eLoopBuilding = (BuildingTypes)(ePlayer == NO_PLAYER
				? GC.getBuildingClassInfo(eLoopBuildingClass).getDefaultBuildingIndex()
				: GC.getCivilizationInfo(GET_PLAYER(ePlayer).getCivilizationType()).getCivilizationBuildings(eLoopBuildingClass));
			if (eLoopBuilding == NO_BUILDING)
				continue;
			// K-Mod end

			if (kBuilding.getPrereqNumOfBuildingClass(eLoopBuildingClass) > 0)
			{
				if (ePlayer == NO_PLAYER)
				{
					CvWString szTempBuffer;
					szTempBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_BUILDING_REQUIRES_NUM_SPECIAL_BUILDINGS_NO_CITY", GC.getBuildingInfo(eLoopBuilding).getTextKeyWide(), kBuilding.getPrereqNumOfBuildingClass(eLoopBuildingClass)).c_str());

					szBuffer.append(szTempBuffer);
					bShowedPrereq = true;
				}
				else
				{
					// K-Mod. In the city screen, include in the prereqs calculation the number of eBuilding under construction.
					// But in the civilopedia, don't even include the number we have already built!
					const CvPlayer& kPlayer = GET_PLAYER(ePlayer);
					int iNeeded = kPlayer.getBuildingClassPrereqBuilding(eBuilding, eLoopBuildingClass, bCivilopediaText ? -kPlayer.getBuildingClassCount((BuildingClassTypes)kBuilding.getBuildingClassType()) : kPlayer.getBuildingClassMaking((BuildingClassTypes)kBuilding.getBuildingClassType()));
					FAssert(iNeeded > 0);
					//if ((pCity == NULL) || (GET_PLAYER(ePlayer).getBuildingClassCount(eLoopBuildingClass) < GET_PLAYER(ePlayer).getBuildingClassPrereqBuilding(eBuilding, eLoopBuildingClass)))
					// K-Mod end

					CvWString szTempBuffer;
					if (pCity != NULL)
					{
						//szTempBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_BUILDING_REQUIRES_NUM_SPECIAL_BUILDINGS", GC.getBuildingInfo(eLoopBuilding).getTextKeyWide(), GET_PLAYER(ePlayer).getBuildingClassCount(eLoopBuildingClass), GET_PLAYER(ePlayer).getBuildingClassPrereqBuilding(eBuilding, eLoopBuildingClass)).c_str());
						szTempBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_BUILDING_REQUIRES_NUM_SPECIAL_BUILDINGS", GC.getBuildingInfo(eLoopBuilding).getTextKeyWide(), kPlayer.getBuildingClassCount(eLoopBuildingClass), iNeeded).c_str()); // K-Mod
					}
					else
					{
						//szTempBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_BUILDING_REQUIRES_NUM_SPECIAL_BUILDINGS_NO_CITY", GC.getBuildingInfo(eLoopBuilding).getTextKeyWide(), kPlayer.getBuildingClassPrereqBuilding(eBuilding, eLoopBuildingClass).c_str());
						szTempBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_BUILDING_REQUIRES_NUM_SPECIAL_BUILDINGS_NO_CITY", GC.getBuildingInfo(eLoopBuilding).getTextKeyWide(), iNeeded).c_str()); // K-Mod
					}

					szBuffer.append(szTempBuffer);
					bShowedPrereq = true;
				}
			}
			
			if (!bShowedPrereq && kBuilding.isBuildingClassNeededInCity(eLoopBuildingClass))
			{
				if ((pCity == NULL) || (pCity->getNumBuilding(eLoopBuilding) <= 0))
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_REQUIRES_STRING", GC.getBuildingInfo(eLoopBuilding).getTextKeyWide()));
				}
			}
/*************************************************************************************************/
/* UNOFFICIAL_PATCH                         END                                                  */
/*************************************************************************************************/
		}
		// MOD - END - Relation Caching

		// MOD - START - Advanced Building Prerequisites (Power)
		if (kBuilding.isPrereqPower())
		{
			if (NULL == pCity || !pCity->isPower())
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_REQUIRES_POWER"));
			}
		}
		// MOD - END - Advanced Building Prerequisites (Power)

		if (kBuilding.isPrereqNoReligion())
		{
			if (NULL == pCity || NO_PLAYER == ePlayer || NO_RELIGION != GET_PLAYER(ePlayer).getStateReligion())
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_REQUIRES_NO_RELIGION"));
			}
		}

		if (kBuilding.isStateReligion())
		{
			if (NULL == pCity || NO_PLAYER == ePlayer || NO_RELIGION == GET_PLAYER(ePlayer).getStateReligion() || !pCity->isHasReligion(GET_PLAYER(ePlayer).getStateReligion()))
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_REQUIRES_STATE_RELIGION"));
			}
		}	

		// MOD - START - Building State Religion Prerequisite
		if (kBuilding.getStateReligion() != NO_RELIGION)
		{
			if ((NO_PLAYER == ePlayer) || (GET_PLAYER(ePlayer).getStateReligion() != kBuilding.getStateReligion()))
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_REQUIRES_SPECIFIC_STATE_RELIGION", GC.getReligionInfo((ReligionTypes)kBuilding.getStateReligion()).getTextKeyWide()));
			}
		}
		// MOD - END - Building State Religion Prerequisite

		if (kBuilding.getNumCitiesPrereq() > 0)
		{
			if (NO_PLAYER == ePlayer || GET_PLAYER(ePlayer).getNumCities() < kBuilding.getNumCitiesPrereq())
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_REQUIRES_NUM_CITIES", kBuilding.getNumCitiesPrereq()));
			}
		}

		if (kBuilding.getUnitLevelPrereq() > 0)
		{
			if (NO_PLAYER == ePlayer || GET_PLAYER(ePlayer).getHighestUnitLevel() < kBuilding.getUnitLevelPrereq())
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_REQUIRES_UNIT_LEVEL", kBuilding.getUnitLevelPrereq()));
			}
		}

		if (kBuilding.getMinLatitude() > 0)
		{
			if (NULL == pCity || pCity->plot()->getLatitude() < kBuilding.getMinLatitude())
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_MIN_LATITUDE", kBuilding.getMinLatitude()));
			}
		}

		if (kBuilding.getMaxLatitude() < 90)
		{
			if (NULL == pCity || pCity->plot()->getLatitude() > kBuilding.getMaxLatitude())
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_MAX_LATITUDE", kBuilding.getMaxLatitude()));
			}
		}

		if (pCity != NULL)
		{
			if (GC.getGameINLINE().isNoNukes())
			{
				if (kBuilding.isAllowsNukes())
				{
					for (int iI = 0; iI < GC.getNumUnitInfos(); ++iI)
					{
						if (GC.getUnitInfo((UnitTypes)iI).getNukeRange() != -1)
						{
							szBuffer.append(NEWLINE);
							szBuffer.append(gDLL->getText("TXT_KEY_PROJECT_NO_NUKES"));
							break;
						}
					}
				}
			}
		}

		if (bCivilopediaText)
		{
			if (kBuilding.getVictoryPrereq() != NO_VICTORY)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_REQUIRES_VICTORY", GC.getVictoryInfo((VictoryTypes)(kBuilding.getVictoryPrereq())).getTextKeyWide()));
			}

			if (kBuilding.getMaxStartEra() != NO_ERA)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_MAX_START_ERA", GC.getEraInfo((EraTypes)kBuilding.getMaxStartEra()).getTextKeyWide()));
			}

			if (kBuilding.getNumTeamsPrereq() > 0)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_REQUIRES_NUM_TEAMS", kBuilding.getNumTeamsPrereq()));
			}
		}
		else
		{
			// MOD - START - Advanced Building Prerequisites (Tech)
			bool bFirst = true;

			// Note: In the tech chooser view the building will appear on the panel of the first
			// prerequisite tech, so we needn't display it in the help text because it's placement
			// implies the prerequisite
			for (int iPre = (bTechChooserText ? 1 : 0); iPre < kBuilding.getNumPrereqAndTechs(); iPre++)
			{
				TechTypes ePrereqAndTech = (TechTypes)kBuilding.getPrereqAndTechs(iPre);
				if (ePlayer == NO_PLAYER || !(GET_TEAM(GET_PLAYER(ePlayer).getTeam()).isHasTech(ePrereqAndTech)))
				{
					const CvTechInfo& kPrereqAndTech = GC.getTechInfo(ePrereqAndTech);
					CvWString szFirstBuffer;
					CvWString szItemBuffer;
					szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_REQUIRES").c_str());
					szItemBuffer.Format(L"<link=%s>%s</link>", CvWString(kPrereqAndTech.getType()).GetCString(), kPrereqAndTech.getDescription());
					setListHelp(szBuffer, szFirstBuffer, szItemBuffer, gDLL->getText("TXT_KEY_AND").c_str(), bFirst);
					bFirst = false;
				}
			}

			if (!bFirst)
			{
				szBuffer.append(ENDCOLR);
			}

			/*
			if (!bTechChooserText)
			{
				if (kBuilding.getPrereqAndTech() != NO_TECH)
				{
					if (ePlayer == NO_PLAYER || !(GET_TEAM(GET_PLAYER(ePlayer).getTeam()).isHasTech((TechTypes)(kBuilding.getPrereqAndTech()))))
					{
						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_REQUIRES_STRING", GC.getTechInfo((TechTypes)(kBuilding.getPrereqAndTech())).getTextKeyWide()));
					}
				}
			}

			bool bFirst = true;

			for (int iI = 0; iI < GC.getNUM_BUILDING_AND_TECH_PREREQS(); ++iI)
			{
				if (kBuilding.getPrereqAndTechs(iI) != NO_TECH)
				{
					if (bTechChooserText || ePlayer == NO_PLAYER || !(GET_TEAM(GET_PLAYER(ePlayer).getTeam()).isHasTech((TechTypes)(kBuilding.getPrereqAndTechs(iI)))))
					{
						CvWString szTempBuffer;
						szTempBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_REQUIRES").c_str());
						setListHelp(szBuffer, szTempBuffer, GC.getTechInfo(((TechTypes)(kBuilding.getPrereqAndTechs(iI)))).getDescription(), gDLL->getText("TXT_KEY_AND").c_str(), bFirst);
						bFirst = false;
					}
				}
			}

			if (!bFirst)
			{
				szBuffer.append(ENDCOLR);
			}
			*/
			// MOD - END - Advanced Building Prerequisites (Tech)

			// MOD - START - Advanced Building Prerequisites (Civic)
			bFirst = true;
			CvWStringBuffer szCivicList;

			for (int iPrereqCivicIndex = 0; iPrereqCivicIndex < kBuilding.getNumPrereqAndCivics(); iPrereqCivicIndex++)
			{
				CivicTypes ePrereqCivic = (CivicTypes)kBuilding.getPrereqAndCivics(iPrereqCivicIndex);
				if ((pCity == NULL) || !(GET_PLAYER(ePlayer).isCivic(ePrereqCivic)))
				{
					CvWString szTempBuffer;
					szTempBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_REQUIRES").c_str());
					setListHelp(szCivicList, szTempBuffer, GC.getCivicInfo(ePrereqCivic).getDescription(), L", ", bFirst);
					bFirst = false;
				}
			}

			if (!bFirst)
			{
				szCivicList.append(ENDCOLR);
				szBuffer.append(szCivicList);
			}
			// MOD - END - Advanced Building Prerequisites (Civic)

			// MOD - START - Bonus Converters
			/*
			if (kBuilding.getPrereqAndBonus() != NO_BONUS)
			{
				if ((pCity == NULL) || !(pCity->hasBonus((BonusTypes)kBuilding.getPrereqAndBonus())))
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_UNIT_REQUIRES_STRING", GC.getBonusInfo((BonusTypes)kBuilding.getPrereqAndBonus()).getTextKeyWide()));
				}
			}
			*/
			// MOD - END - Bonus Converters

			CvWStringBuffer szBonusList;
			bFirst = true;

			// MOD - START - Bonus Converters
			// MOD - START - Advanced Building Prerequisites (Bonus)
			for (int iPre = 0; iPre < kBuilding.getNumPrereqAndBonuses(); iPre++)
			{
				if ((pCity == NULL) || !(pCity->hasBonus((BonusTypes)kBuilding.getPrereqAndBonuses(iPre))))
				{
					CvWString szTempBuffer;
					szTempBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_REQUIRES").c_str());
					setListHelp(szBonusList, szTempBuffer, GC.getBonusInfo((BonusTypes)kBuilding.getPrereqAndBonuses(iPre)).getDescription(), gDLL->getText("TXT_KEY_AND").c_str(), bFirst);
					bFirst = false;
				}
			}
			// MOD - END - Advanced Building Prerequisites (Bonus)

			if (kBuilding.isBonusConverter())
			{
				CvWString szBonusItem;

				if (pCity == NULL)
				{
					for (int iConsumedBonusIndex = 0; iConsumedBonusIndex < kBuilding.getNumConsumedBonuses(); iConsumedBonusIndex++)
					{
						BonusTypes eConsumedBonus = (BonusTypes)kBuilding.getConsumedBonusType(iConsumedBonusIndex);
						int iConsumedAmount = kBuilding.getConsumedBonusAmount(iConsumedBonusIndex);

						if (iConsumedAmount > 1)
						{
							szBonusItem.Format(L"%d %s", iConsumedAmount, GC.getBonusInfo(eConsumedBonus).getDescription());
						}
						else
						{
							szBonusItem.Format(L"%s", GC.getBonusInfo(eConsumedBonus).getDescription());
						}

						CvWString szTempBuffer;
						szTempBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_REQUIRES").c_str());
						setListHelp(szBonusList, szTempBuffer, szBonusItem, gDLL->getText("TXT_KEY_AND").c_str(), bFirst);
						bFirst = false;
					}
				}
				else
				{
					CvPlotGroup* pPlotGroup = pCity->plot()->getOwnerPlotGroup();
					if (pPlotGroup != NULL)
					{
						for (int iConsumedBonusIndex = 0; iConsumedBonusIndex < kBuilding.getNumConsumedBonuses(); iConsumedBonusIndex++)
						{
							BonusTypes eConsumedBonus = (BonusTypes)kBuilding.getConsumedBonusType(iConsumedBonusIndex);
							int iConsumedAmount = kBuilding.getConsumedBonusAmount(iConsumedBonusIndex);

							// Get the number of bonuses available in this city
							int iNumBonus = pCity->getNumBonuses(eConsumedBonus);	// This will be zero if the city has a NoBonus

							// Subtract the number of bonuses already consumed by any instances of this
							// building anywhere in the entire plot group
							int iNumAvailableBonus = iNumBonus - pPlotGroup->getNumBonusConverterConsumedBonusesByIndex(eBuilding, iConsumedBonusIndex);	// Could be negative

							if (iConsumedAmount > iNumAvailableBonus)
							{
								if (iConsumedAmount > 1)
								{
									szBonusItem.Format(L"%d %s", iConsumedAmount, GC.getBonusInfo(eConsumedBonus).getDescription());
								}
								else
								{
									szBonusItem.Format(L"%s", GC.getBonusInfo(eConsumedBonus).getDescription());
								}

								CvWString szTempBuffer;
								szTempBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_REQUIRES").c_str());
								setListHelp(szBonusList, szTempBuffer, szBonusItem, gDLL->getText("TXT_KEY_AND").c_str(), bFirst);
								bFirst = false;

								if ((iNumAvailableBonus >= 0) && (iNumAvailableBonus != iNumBonus))
								{
									szTempBuffer.Format(L" (%d %s)", iNumAvailableBonus, gDLL->getText("TXT_KEY_AVAILABLE").c_str());
									szBonusList.append(szTempBuffer);
								}
							}
						}
					}
				}
			}

			if (!bFirst)
			{
				szBonusList.append(ENDCOLR);
				szBuffer.append(szBonusList);
			}

			szBonusList.clear();
			bFirst = true;
			// MOD - END - Bonus Converters

			// MOD - START - Advanced Building Prerequisites (Bonus)
			for (int iPre = 0; iPre < kBuilding.getNumPrereqOrBonuses(); iPre++)
			{
				if ((pCity == NULL) || !(pCity->hasBonus((BonusTypes)kBuilding.getPrereqOrBonuses(iPre))))
				{
					CvWString szTempBuffer;
					szTempBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_REQUIRES").c_str());
					setListHelp(szBonusList, szTempBuffer, GC.getBonusInfo((BonusTypes)kBuilding.getPrereqOrBonuses(iPre)).getDescription(), gDLL->getText("TXT_KEY_OR").c_str(), bFirst);
					bFirst = false;
				}
				else if (NULL != pCity)
				{
					bFirst = true;
					break;
				}
			}

			if (!bFirst)
			{
				szBonusList.append(ENDCOLR);
				szBuffer.append(szBonusList);
			}
			// MOD - END - Advanced Building Prerequisites (Bonus)

			if (NO_CORPORATION != kBuilding.getFoundsCorporation())
			{
				bFirst = true;
				szBonusList.clear();
				for (int iI = 0; iI < GC.getNUM_CORPORATION_PREREQ_BONUSES(); ++iI)
				{
					BonusTypes eBonus = (BonusTypes)GC.getCorporationInfo((CorporationTypes)kBuilding.getFoundsCorporation()).getPrereqBonus(iI);
					if (NO_BONUS != eBonus)
					{
						if ((pCity == NULL) || !(pCity->hasBonus(eBonus)))
						{
							CvWString szTempBuffer, szFirstBuffer;
							szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_REQUIRES").c_str());
							szTempBuffer.Format(L"<link=literal>%s</link>", GC.getBonusInfo(eBonus).getDescription());
							setListHelp(szBonusList, szFirstBuffer, szTempBuffer, gDLL->getText("TXT_KEY_OR"), bFirst);
							bFirst = false;
						}
						else if (NULL != pCity)
						{
							bFirst = true;
							break;
						}
					}
				}

				if (!bFirst)
				{
					szBonusList.append(ENDCOLR);
					szBuffer.append(szBonusList);
				}
			}
		}
	}
}


void CvGameTextMgr::setProjectHelp(CvWStringBuffer &szBuffer, ProjectTypes eProject, bool bCivilopediaText, CvCity* pCity)
{
	PROFILE_FUNC();
	
	CvWString szTempBuffer;
	CvWString szFirstBuffer;
	PlayerTypes ePlayer;
	bool bFirst;
	int iProduction;
	int iI;

	if (NO_PROJECT == eProject)
	{
		return;
	}

	CvProjectInfo& kProject = GC.getProjectInfo(eProject);

	if (pCity != NULL)
	{
		ePlayer = pCity->getOwnerINLINE();
	}
	else
	{
		ePlayer = GC.getGameINLINE().getActivePlayer();
	}

	if (!bCivilopediaText)
	{
		szTempBuffer.Format( SETCOLR L"<link=literal>%s</link>" ENDCOLR , TEXT_COLOR("COLOR_PROJECT_TEXT"), kProject.getDescription());
		szBuffer.append(szTempBuffer);

		if (isWorldProject(eProject))
		{
			if (pCity == NULL)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_PROJECT_WORLD_NUM_ALLOWED", kProject.getMaxGlobalInstances()));
			}
			else
			{
				szBuffer.append(gDLL->getText("TXT_KEY_PROJECT_WORLD_NUM_LEFT", (kProject.getMaxGlobalInstances() - GC.getGameINLINE().getProjectCreatedCount(eProject) - GET_TEAM(GET_PLAYER(ePlayer).getTeam()).getProjectMaking(eProject))));
			}
		}

		if (isTeamProject(eProject))
		{
			if (pCity == NULL)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_PROJECT_TEAM_NUM_ALLOWED", kProject.getMaxTeamInstances()));
			}
			else
			{
                szBuffer.append(gDLL->getText("TXT_KEY_PROJECT_TEAM_NUM_LEFT", (kProject.getMaxTeamInstances() - GET_TEAM(GET_PLAYER(ePlayer).getTeam()).getProjectCount(eProject) - GET_TEAM(GET_PLAYER(ePlayer).getTeam()).getProjectMaking(eProject))));
			}
		}
	}

	if (kProject.getNukeInterception() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_PROJECT_CHANCE_INTERCEPT_NUKES", kProject.getNukeInterception()));
	}

	if (kProject.getTechShare() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_PROJECT_TECH_SHARE", kProject.getTechShare()));
	}

	if (kProject.isAllowsNukes())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_PROJECT_ENABLES_NUKES"));
	}

	// MOD - START - Epidemics
	// Epidemic system by Mexico
	if (kProject.getGlobalEpidemicModifier() != 0)
	{
		szTempBuffer.Format(L"%+d", (kProject.getGlobalEpidemicModifier() * 100) / GC.getDefineINT("EPIDEMIC_RAND_BASE"));
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_GLOBAL_EPIDEMIC_MODIFIER", szTempBuffer.GetCString()));
	}
	// MOD - END - Epidemics

	if (kProject.getEveryoneSpecialUnit() != NO_SPECIALUNIT)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_PROJECT_ENABLES_SPECIAL", GC.getSpecialUnitInfo((SpecialUnitTypes)(kProject.getEveryoneSpecialUnit())).getTextKeyWide()));
	}

	if (kProject.getEveryoneSpecialBuilding() != NO_SPECIALBUILDING)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_PROJECT_ENABLES_SPECIAL", GC.getSpecialBuildingInfo((SpecialBuildingTypes)(kProject.getEveryoneSpecialBuilding())).getTextKeyWide()));
	}

	for (iI = 0; iI < GC.getNumVictoryInfos(); ++iI)
	{
		if (kProject.getVictoryThreshold(iI) > 0)
		{
			if (kProject.getVictoryThreshold(iI) == kProject.getVictoryMinThreshold(iI))
			{
				szTempBuffer.Format(L"%d", kProject.getVictoryThreshold(iI));
			}
			else
			{
				szTempBuffer.Format(L"%d-%d", kProject.getVictoryMinThreshold(iI), kProject.getVictoryThreshold(iI));
			}

			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_PROJECT_REQUIRED_FOR_VICTORY", szTempBuffer.GetCString(), GC.getVictoryInfo((VictoryTypes)iI).getTextKeyWide()));
		}
	}

	bFirst = true;

	for (iI = 0; iI < GC.getNumProjectInfos(); ++iI)
	{
		if (GC.getProjectInfo((ProjectTypes)iI).getAnyoneProjectPrereq() == eProject)
		{
			szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_PROJECT_REQUIRED_TO_CREATE_ANYONE").c_str());
			szTempBuffer.Format(SETCOLR L"<link=literal>%s</link>" ENDCOLR, TEXT_COLOR("COLOR_PROJECT_TEXT"), GC.getProjectInfo((ProjectTypes)iI).getDescription());
			setListHelp(szBuffer, szFirstBuffer, szTempBuffer, L", ", bFirst);
			bFirst = false;
		}
	}

	bFirst = true;

	for (iI = 0; iI < GC.getNumProjectInfos(); ++iI)
	{
		if (GC.getProjectInfo((ProjectTypes)iI).getProjectsNeeded(eProject) > 0)
		{
			if ((pCity == NULL) || pCity->canCreate(((ProjectTypes)iI), false, true))
			{
				szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_PROJECT_REQUIRED_TO_CREATE").c_str());
				szTempBuffer.Format(SETCOLR L"<link=literal>%s</link>" ENDCOLR, TEXT_COLOR("COLOR_PROJECT_TEXT"), GC.getProjectInfo((ProjectTypes)iI).getDescription());
				setListHelp(szBuffer, szFirstBuffer, szTempBuffer, L", ", bFirst);
				bFirst = false;
			}
		}
	}

	if ((pCity == NULL) || !(pCity->canCreate(eProject)))
	{
		if (pCity != NULL)
		{
			if (GC.getGameINLINE().isNoNukes())
			{
				if (kProject.isAllowsNukes())
				{
					for (iI = 0; iI < GC.getNumUnitInfos(); ++iI)
					{
						if (GC.getUnitInfo((UnitTypes)iI).getNukeRange() != -1)
						{
							szBuffer.append(NEWLINE);
							szBuffer.append(gDLL->getText("TXT_KEY_PROJECT_NO_NUKES"));
							break;
						}
					}
				}
			}
		}

		if (kProject.getAnyoneProjectPrereq() != NO_PROJECT)
		{
			if ((pCity == NULL) || (GC.getGameINLINE().getProjectCreatedCount((ProjectTypes)(kProject.getAnyoneProjectPrereq())) == 0))
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_PROJECT_REQUIRES_ANYONE", GC.getProjectInfo((ProjectTypes)kProject.getAnyoneProjectPrereq()).getTextKeyWide()));
			}
		}

		for (iI = 0; iI < GC.getNumProjectInfos(); ++iI)
		{
			if (kProject.getProjectsNeeded(iI) > 0)
			{
				if ((pCity == NULL) || (GET_TEAM(GET_PLAYER(ePlayer).getTeam()).getProjectCount((ProjectTypes)iI) < kProject.getProjectsNeeded(iI)))
				{
					if (pCity != NULL)
					{
						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_PROJECT_REQUIRES", GC.getProjectInfo((ProjectTypes)iI).getTextKeyWide(), GET_TEAM(GET_PLAYER(ePlayer).getTeam()).getProjectCount((ProjectTypes)iI), kProject.getProjectsNeeded(iI)));
					}
					else
					{
						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_PROJECT_REQUIRES_NO_CITY", GC.getProjectInfo((ProjectTypes)iI).getTextKeyWide(), kProject.getProjectsNeeded(iI)));
					}
				}
			}
		}

		if (bCivilopediaText)
		{
			if (kProject.getVictoryPrereq() != NO_VICTORY)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_PROJECT_REQUIRES_STRING_VICTORY", GC.getVictoryInfo((VictoryTypes)(kProject.getVictoryPrereq())).getTextKeyWide()));
			}
		}
	}

	if (!bCivilopediaText)
	{
		if (pCity == NULL)
		{
			if (ePlayer != NO_PLAYER)
			{
				szTempBuffer.Format(L"\n%d%c", GET_PLAYER(ePlayer).getProductionNeeded(eProject), GC.getYieldInfo(YIELD_PRODUCTION).getChar());
			}
			else
			{
				szTempBuffer.Format(L"\n%d%c", kProject.getProductionCost(), GC.getYieldInfo(YIELD_PRODUCTION).getChar());
			}
			szBuffer.append(szTempBuffer);
		}
		else
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_PROJECT_NUM_TURNS", pCity->getProductionTurnsLeft(eProject, ((GC.ctrlKey() || !(GC.shiftKey())) ? 0 : pCity->getOrderQueueLength()))));

			iProduction = pCity->getProjectProduction(eProject);

			if (iProduction > 0)
			{
				szTempBuffer.Format(L" - %d/%d%c", iProduction, pCity->getProductionNeeded(eProject), GC.getYieldInfo(YIELD_PRODUCTION).getChar());
			}
			else
			{
				szTempBuffer.Format(L" - %d%c", pCity->getProductionNeeded(eProject), GC.getYieldInfo(YIELD_PRODUCTION).getChar());
			}
			szBuffer.append(szTempBuffer);
		}
	}

	for (iI = 0; iI < GC.getNumBonusInfos(); ++iI)
	{
		if (kProject.getBonusProductionModifier(iI) != 0)
		{
			if (pCity != NULL)
			{
				if (pCity->hasBonus((BonusTypes)iI))
				{
					szBuffer.append(gDLL->getText("TXT_KEY_COLOR_POSITIVE"));
				}
				else
				{
					szBuffer.append(gDLL->getText("TXT_KEY_COLOR_NEGATIVE"));
				}
			}
			if (!bCivilopediaText)
			{
				szBuffer.append(L" (");
			}
			else
			{
				szTempBuffer.Format(L"%s%c", NEWLINE, gDLL->getSymbolID(BULLET_CHAR), szTempBuffer.GetCString());
				szBuffer.append(szTempBuffer);
			}
			if (kProject.getBonusProductionModifier(iI) == 100)
			{
				szBuffer.append(gDLL->getText("TXT_KEY_PROJECT_DOUBLE_SPEED_WITH", GC.getBonusInfo((BonusTypes)iI).getTextKeyWide()));
			}
			else
			{
				szBuffer.append(gDLL->getText("TXT_KEY_PROJECT_BUILDS_FASTER_WITH", kProject.getBonusProductionModifier(iI), GC.getBonusInfo((BonusTypes)iI).getTextKeyWide()));
			}
			if (!bCivilopediaText)
			{
				szBuffer.append(L')');
			}
			if (pCity != NULL)
			{
				szBuffer.append(gDLL->getText("TXT_KEY_COLOR_REVERT"));
			}
		}
	}

	// K-Mod
	if (pCity && gDLL->getChtLvl() > 0 && GC.ctrlKey())
	{
		int iValue = pCity->AI_projectValue(eProject);
		szBuffer.append(CvWString::format(L"\nProject Value (base) = %d", iValue));

		ProjectTypes eBestProject = ((CvCityAI*)pCity)->AI_bestProject(&iValue);
		if (eBestProject == eProject)
		{
			szBuffer.append(CvWString::format(SETCOLR L"\n(Best project value (scaled) = %d)" ENDCOLR, TEXT_COLOR("COLOR_LIGHT_GREY"), iValue));
		}
	}
	//
}


void CvGameTextMgr::setProcessHelp(CvWStringBuffer &szBuffer, ProcessTypes eProcess)
{
	int iI;

	szBuffer.append(GC.getProcessInfo(eProcess).getDescription());

	for (iI = 0; iI < NUM_COMMERCE_TYPES; ++iI)
	{
		if (GC.getProcessInfo(eProcess).getProductionToCommerceModifier(iI) != 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_PROCESS_CONVERTS", GC.getProcessInfo(eProcess).getProductionToCommerceModifier(iI), GC.getYieldInfo(YIELD_PRODUCTION).getChar(), GC.getCommerceInfo((CommerceTypes) iI).getChar()));
		}
	}
}

void CvGameTextMgr::setBadHealthHelp(CvWStringBuffer &szBuffer, CvCity& city)
{
	CvPlot* pLoopPlot;
	FeatureTypes eFeature;
	int iHealth;
	int iI;

	if (city.badHealth() > 0)
	{
		iHealth = -(city.getFreshWaterBadHealth());
		if (iHealth > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_FROM_FRESH_WATER", iHealth));
			szBuffer.append(NEWLINE);
		}

		iHealth = -(city.getFeatureBadHealth());
		if (iHealth > 0)
		{
			eFeature = NO_FEATURE;

			for (iI = 0; iI < NUM_CITY_PLOTS; ++iI)
			{
				pLoopPlot = plotCity(city.getX_INLINE(), city.getY_INLINE(), iI);

				if (pLoopPlot != NULL)
				{
					if (pLoopPlot->getFeatureType() != NO_FEATURE)
					{
						if (GC.getFeatureInfo(pLoopPlot->getFeatureType()).getHealthPercent() < 0)
						{
							if (eFeature == NO_FEATURE)
							{
								eFeature = pLoopPlot->getFeatureType();
							}
							else if (eFeature != pLoopPlot->getFeatureType())
							{
								eFeature = NO_FEATURE;
								break;
							}
						}
					}
				}
			}

			szBuffer.append(gDLL->getText("TXT_KEY_MISC_FEAT_HEALTH", iHealth, ((eFeature == NO_FEATURE) ? L"TXT_KEY_MISC_FEATURES" : GC.getFeatureInfo(eFeature).getTextKeyWide())));
			szBuffer.append(NEWLINE);
		}

		iHealth = city.getEspionageHealthCounter();
		if (iHealth > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_HEALTH_FROM_ESPIONAGE", iHealth));
			szBuffer.append(NEWLINE);
		}

		iHealth = -(city.getPowerBadHealth());
		if (iHealth > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_HEALTH_FROM_POWER", iHealth));
			szBuffer.append(NEWLINE);
		}

		iHealth = -(city.getBonusBadHealth());
		if (iHealth > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_HEALTH_FROM_BONUSES", iHealth));
			szBuffer.append(NEWLINE);
		}

		iHealth = -(city.totalBadBuildingHealth());
		if (iHealth > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_HEALTH_FROM_BUILDINGS", iHealth));
			szBuffer.append(NEWLINE);
		}

		iHealth = -(GET_PLAYER(city.getOwnerINLINE()).getExtraHealth());
		if (iHealth > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_HEALTH_FROM_CIV", iHealth));
			szBuffer.append(NEWLINE);
		}

		iHealth = -city.getExtraHealth();
		if (iHealth > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_UNHEALTH_EXTRA", iHealth));
			szBuffer.append(NEWLINE);
		}

		iHealth = -(GC.getHandicapInfo(city.getHandicapType()).getHealthBonus());
		if (iHealth > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_HEALTH_FROM_HANDICAP", iHealth));
			szBuffer.append(NEWLINE);
		}

		iHealth = city.unhealthyPopulation();
		if (iHealth > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_HEALTH_FROM_POP", iHealth));
/*
** K-Mod, 29/dec/10, karadoc
** added modifier text
*/
			if (city.getUnhealthyPopulationModifier() != 0)
			{
				wchar szTempBuffer[1024];

				swprintf(szTempBuffer, 1024, L" (%+d%%)", city.getUnhealthyPopulationModifier());
				szBuffer.append(szTempBuffer);
			}
/*
** K-Mod end
*/
			szBuffer.append(NEWLINE);
		}

		// MOD - START - Era Effects
		iHealth = GET_PLAYER(city.getOwnerINLINE()).getEraUnhealthiness();
		if (iHealth < 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_ERA_UNHEALTHINESS", -iHealth));
			szBuffer.append(NEWLINE);
		}
		// MOD - END - Era Effects

		// MOD - START - Better Nukes
		iHealth = GC.getGameINLINE().getNuclearWinterUnhealthiness();
		if (iHealth > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_NUCLEAR_WINTER_UNHEALTHINESS", iHealth));
			szBuffer.append(NEWLINE);
		}
		// MOD - END - Better Nukes

		szBuffer.append(L"-----------------------\n");

		szBuffer.append(gDLL->getText("TXT_KEY_MISC_TOTAL_UNHEALTHY", city.badHealth()));
	}
}

void CvGameTextMgr::setGoodHealthHelp(CvWStringBuffer &szBuffer, CvCity& city)
{
	CvPlot* pLoopPlot;
	FeatureTypes eFeature;
	int iHealth;
	int iI;

	if (city.goodHealth() > 0)
	{
		iHealth = city.getFreshWaterGoodHealth();
		if (iHealth > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_HEALTH_FROM_FRESH_WATER", iHealth));
			szBuffer.append(NEWLINE);
		}

		iHealth = city.getFeatureGoodHealth();
		if (iHealth > 0)
		{
			eFeature = NO_FEATURE;

			for (iI = 0; iI < NUM_CITY_PLOTS; ++iI)
			{
				pLoopPlot = plotCity(city.getX_INLINE(), city.getY_INLINE(), iI);

				if (pLoopPlot != NULL)
				{
					if (pLoopPlot->getFeatureType() != NO_FEATURE)
					{
						if (GC.getFeatureInfo(pLoopPlot->getFeatureType()).getHealthPercent() > 0)
						{
							if (eFeature == NO_FEATURE)
							{
								eFeature = pLoopPlot->getFeatureType();
							}
							else if (eFeature != pLoopPlot->getFeatureType())
							{
								eFeature = NO_FEATURE;
								break;
							}
						}
					}
				}
			}

			szBuffer.append(gDLL->getText("TXT_KEY_MISC_FEAT_GOOD_HEALTH", iHealth, ((eFeature == NO_FEATURE) ? L"TXT_KEY_MISC_FEATURES" : GC.getFeatureInfo(eFeature).getTextKeyWide())));
			szBuffer.append(NEWLINE);
		}

		iHealth = city.getPowerGoodHealth();
		if (iHealth > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_GOOD_HEALTH_FROM_POWER", iHealth));
			szBuffer.append(NEWLINE);
		}

		iHealth = city.getBonusGoodHealth();
		if (iHealth > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_GOOD_HEALTH_FROM_BONUSES", iHealth));
			szBuffer.append(NEWLINE);
		}

		iHealth = city.totalGoodBuildingHealth();
		if (iHealth > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_GOOD_HEALTH_FROM_BUILDINGS", iHealth));
			szBuffer.append(NEWLINE);
		}

		iHealth = GET_PLAYER(city.getOwnerINLINE()).getExtraHealth();
		if (iHealth > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_GOOD_HEALTH_FROM_CIV", iHealth));
			szBuffer.append(NEWLINE);
		}

		iHealth = city.getExtraHealth();
		if (iHealth > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_HEALTH_EXTRA", iHealth));
			szBuffer.append(NEWLINE);
		}

		iHealth = GC.getHandicapInfo(city.getHandicapType()).getHealthBonus();
		if (iHealth > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_GOOD_HEALTH_FROM_HANDICAP", iHealth));
			szBuffer.append(NEWLINE);
		}

		// MOD - START - Era Effects
		iHealth = GET_PLAYER(city.getOwnerINLINE()).getEraHealthiness();
		if (iHealth > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_ERA_HEALTHINESS", iHealth));
			szBuffer.append(NEWLINE);
		}
		// MOD - END - Era Effects

		szBuffer.append(L"-----------------------\n");

		szBuffer.append(gDLL->getText("TXT_KEY_MISC_TOTAL_HEALTHY", city.goodHealth()));
	}
}

// BUG - Building Additional Health - start
bool CvGameTextMgr::setBuildingAdditionalHealthHelp(CvWStringBuffer &szBuffer, const CvCity& city, const CvWString& szStart, bool bStarted)
{
	CvWString szLabel;
	CvCivilizationInfo& kCivilization = GC.getCivilizationInfo(GET_PLAYER(city.getOwnerINLINE()).getCivilizationType());

	for (int iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
	{
		BuildingTypes eBuilding = (BuildingTypes)kCivilization.getCivilizationBuildings((BuildingClassTypes)iI);

		if (eBuilding != NO_BUILDING && city.canConstruct(eBuilding, false, true, false))
		{
			int iGood = 0, iBad = 0;
			int iChange = city.getAdditionalHealthByBuilding(eBuilding, iGood, iBad);

			if (iGood != 0 || iBad != 0)
			{
				int iSpoiledFood = city.getAdditionalSpoiledFood(iGood, iBad);
				int iStarvation = city.getAdditionalStarvation(iSpoiledFood);

				if (!bStarted)
				{
					szBuffer.append(szStart);
					bStarted = true;
				}

				szLabel.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR("COLOR_BUILDING_TEXT"), GC.getBuildingInfo(eBuilding).getDescription());
				bool bStartedLine = setResumableGoodBadChangeHelp(szBuffer, szLabel, L": ", L"", iGood, gDLL->getSymbolID(HEALTHY_CHAR), iBad, gDLL->getSymbolID(UNHEALTHY_CHAR), false, true);
				setResumableValueChangeHelp(szBuffer, szLabel, L": ", L"", iSpoiledFood, gDLL->getSymbolID(EATEN_FOOD_CHAR), false, true, bStartedLine);
				setResumableValueChangeHelp(szBuffer, szLabel, L": ", L"", iStarvation, gDLL->getSymbolID(BAD_FOOD_CHAR), false, true, bStartedLine);
			}
		}
	}

	return bStarted;
}
// BUG - Building Additional Health - end


void CvGameTextMgr::setAngerHelp(CvWStringBuffer &szBuffer, CvCity& city)
{
	int iOldAngerPercent;
	int iNewAngerPercent;
	int iOldAnger;
	int iNewAnger;
	int iAnger;
	int iI;

	if (city.isOccupation())
	{
		szBuffer.append(gDLL->getText("TXT_KEY_ANGER_RESISTANCE"));
	}
	else if (GET_PLAYER(city.getOwnerINLINE()).isAnarchy())
	{
		szBuffer.append(gDLL->getText("TXT_KEY_ANGER_ANARCHY"));
	}
	else if (city.unhappyLevel() > 0)
	{
		iOldAngerPercent = 0;
		iNewAngerPercent = 0;

		iOldAnger = 0;
		iNewAnger = 0;

/*
** K-Mod, 5/jan/11, karadoc
** all anger perecent bits were like this:
iNewAnger += (((iNewAngerPercent * city.getPopulation()) / GC.getPERCENT_ANGER_DIVISOR()) - ((iOldAngerPercent * city.getPopulation()) / GC.getDefineINT("PERCENT_ANGER_DIVISOR")));
** I've changed it to use GC.getPERCENT_ANGER_DIVISOR() for both parts.
*/
		// XXX decomp these???
		iNewAngerPercent += city.getOvercrowdingPercentAnger();
		iNewAnger += (((iNewAngerPercent * city.getPopulation()) / GC.getPERCENT_ANGER_DIVISOR()) - ((iOldAngerPercent * city.getPopulation()) / GC.getPERCENT_ANGER_DIVISOR()));
		iAnger = ((iNewAnger - iOldAnger) + std::min(0, iOldAnger));
		if (iAnger > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_ANGER_OVERCROWDING", iAnger));
			szBuffer.append(NEWLINE);
		}
		iOldAngerPercent = iNewAngerPercent;
		iOldAnger = iNewAnger;

		iNewAngerPercent += city.getNoMilitaryPercentAnger();
		iNewAnger += (((iNewAngerPercent * city.getPopulation()) / GC.getPERCENT_ANGER_DIVISOR()) - ((iOldAngerPercent * city.getPopulation()) / GC.getPERCENT_ANGER_DIVISOR()));
		iAnger = ((iNewAnger - iOldAnger) + std::min(0, iOldAnger));
		if (iAnger > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_ANGER_MILITARY_PROTECTION", iAnger));
			szBuffer.append(NEWLINE);
		}
		iOldAngerPercent = iNewAngerPercent;
		iOldAnger = iNewAnger;

		iNewAngerPercent += city.getCulturePercentAnger();
		iNewAnger += (((iNewAngerPercent * city.getPopulation()) / GC.getPERCENT_ANGER_DIVISOR()) - ((iOldAngerPercent * city.getPopulation()) / GC.getPERCENT_ANGER_DIVISOR()));
		iAnger = ((iNewAnger - iOldAnger) + std::min(0, iOldAnger));
		if (iAnger > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_ANGER_OCCUPIED", iAnger));
			szBuffer.append(NEWLINE);
		}
		iOldAngerPercent = iNewAngerPercent;
		iOldAnger = iNewAnger;

		iNewAngerPercent += city.getReligionPercentAnger();
		iNewAnger += (((iNewAngerPercent * city.getPopulation()) / GC.getPERCENT_ANGER_DIVISOR()) - ((iOldAngerPercent * city.getPopulation()) / GC.getPERCENT_ANGER_DIVISOR()));
		iAnger = ((iNewAnger - iOldAnger) + std::min(0, iOldAnger));
		if (iAnger > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_ANGER_RELIGION_FIGHT", iAnger));
			szBuffer.append(NEWLINE);
		}
		iOldAngerPercent = iNewAngerPercent;
		iOldAnger = iNewAnger;

		iNewAngerPercent += city.getHurryPercentAnger();
		iNewAnger += (((iNewAngerPercent * city.getPopulation()) / GC.getPERCENT_ANGER_DIVISOR()) - ((iOldAngerPercent * city.getPopulation()) / GC.getPERCENT_ANGER_DIVISOR()));
		iAnger = ((iNewAnger - iOldAnger) + std::min(0, iOldAnger));
		if (iAnger > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_ANGER_OPPRESSION", iAnger));
			szBuffer.append(NEWLINE);
		}
		iOldAngerPercent = iNewAngerPercent;
		iOldAnger = iNewAnger;

		iNewAngerPercent += city.getConscriptPercentAnger();
		iNewAnger += (((iNewAngerPercent * city.getPopulation()) / GC.getPERCENT_ANGER_DIVISOR()) - ((iOldAngerPercent * city.getPopulation()) / GC.getPERCENT_ANGER_DIVISOR()));
		iAnger = ((iNewAnger - iOldAnger) + std::min(0, iOldAnger));
		if (iAnger > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_ANGER_DRAFT", iAnger));
			szBuffer.append(NEWLINE);
		}
		iOldAngerPercent = iNewAngerPercent;
		iOldAnger = iNewAnger;

		iNewAngerPercent += city.getDefyResolutionPercentAnger();
		iNewAnger += (((iNewAngerPercent * city.getPopulation()) / GC.getPERCENT_ANGER_DIVISOR()) - ((iOldAngerPercent * city.getPopulation()) / GC.getPERCENT_ANGER_DIVISOR()));
		iAnger = ((iNewAnger - iOldAnger) + std::min(0, iOldAnger));
		if (iAnger > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_ANGER_DEFY_RESOLUTION", iAnger));
			szBuffer.append(NEWLINE);
		}
		iOldAngerPercent = iNewAngerPercent;
		iOldAnger = iNewAnger;

		iNewAngerPercent += city.getWarWearinessPercentAnger();
		iNewAnger += (((iNewAngerPercent * city.getPopulation()) / GC.getPERCENT_ANGER_DIVISOR()) - ((iOldAngerPercent * city.getPopulation()) / GC.getPERCENT_ANGER_DIVISOR()));
		iAnger = ((iNewAnger - iOldAnger) + std::min(0, iOldAnger));
		if (iAnger > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_ANGER_WAR_WEAR", iAnger));
			szBuffer.append(NEWLINE);
		}
		iOldAngerPercent = iNewAngerPercent;
		iOldAnger = iNewAnger;
/*
** K-Mod, 30/dec/10, karadoc
** Global warming unhappiness
*/
		// MOD - START - Stock Global Warming
		/*
		// when I say 'percent' I mean 1/100. Unfortunately, people who made the rest of the game meant something else...
		// so I have to multiply my GwPercentAnger by 10 to make it fit in.
		iNewAngerPercent += std::max(0, GET_PLAYER(city.getOwnerINLINE()).getGwPercentAnger()*10);
		iNewAnger += (((iNewAngerPercent * city.getPopulation()) / GC.getPERCENT_ANGER_DIVISOR()) - ((iOldAngerPercent * city.getPopulation()) / GC.getPERCENT_ANGER_DIVISOR()));
		iAnger = ((iNewAnger - iOldAnger) + std::min(0, iOldAnger));
		if (iAnger > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_ANGER_GLOBAL_WARMING", iAnger));
			szBuffer.append(NEWLINE);
		}
		iOldAngerPercent = iNewAngerPercent;
		iOldAnger = iNewAnger;
		*/
		// MOD - END - Stock Global Warming
/*
** K-Mod end
*/

		iNewAnger += std::max(0, city.getVassalUnhappiness());
		iAnger = ((iNewAnger - iOldAnger) + std::min(0, iOldAnger));
		if (iAnger > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_UNHAPPY_VASSAL", iAnger));
			szBuffer.append(NEWLINE);
		}
		iOldAnger = iNewAnger;

		iNewAnger += std::max(0, city.getEspionageHappinessCounter());
		iAnger = ((iNewAnger - iOldAnger) + std::min(0, iOldAnger));
		if (iAnger > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_ANGER_ESPIONAGE", iAnger));
			szBuffer.append(NEWLINE);
		}
		iOldAnger = iNewAnger;

		for (iI = 0; iI < GC.getNumCivicInfos(); ++iI)
		{
			iNewAngerPercent += GET_PLAYER(city.getOwnerINLINE()).getCivicPercentAnger((CivicTypes)iI);
			iNewAnger += (((iNewAngerPercent * city.getPopulation()) / GC.getPERCENT_ANGER_DIVISOR()) - ((iOldAngerPercent * city.getPopulation()) / GC.getPERCENT_ANGER_DIVISOR()));
			iAnger = ((iNewAnger - iOldAnger) + std::min(0, iOldAnger));
			if (iAnger > 0)
			{
				szBuffer.append(gDLL->getText("TXT_KEY_ANGER_DEMAND_CIVIC", iAnger, GC.getCivicInfo((CivicTypes) iI).getTextKeyWide()));
				szBuffer.append(NEWLINE);
			}
			iOldAngerPercent = iNewAngerPercent;
			iOldAnger = iNewAnger;
		}

		iNewAnger -= std::min(0, city.getLargestCityHappiness());
		iAnger = ((iNewAnger - iOldAnger) + std::min(0, iOldAnger));
		if (iAnger > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_ANGER_BIG_CITY", iAnger));
			szBuffer.append(NEWLINE);
		}
		iOldAnger = iNewAnger;

		iNewAnger -= std::min(0, city.getMilitaryHappiness());
		iAnger = ((iNewAnger - iOldAnger) + std::min(0, iOldAnger));
		if (iAnger > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_ANGER_MILITARY_PRESENCE", iAnger));
			szBuffer.append(NEWLINE);
		}
		iOldAnger = iNewAnger;

		iNewAnger -= std::min(0, city.getCurrentStateReligionHappiness());
		iAnger = ((iNewAnger - iOldAnger) + std::min(0, iOldAnger));
		if (iAnger > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_ANGER_STATE_RELIGION", iAnger));
			szBuffer.append(NEWLINE);
		}
		iOldAnger = iNewAnger;

		iNewAnger -= std::min(0, (city.getBuildingBadHappiness() + city.getExtraBuildingBadHappiness()));
		iAnger = ((iNewAnger - iOldAnger) + std::min(0, iOldAnger));
		if (iAnger > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_ANGER_BUILDINGS", iAnger));
			szBuffer.append(NEWLINE);
		}
		iOldAnger = iNewAnger;

		iNewAnger -= std::min(0, city.getFeatureBadHappiness());
		iAnger = ((iNewAnger - iOldAnger) + std::min(0, iOldAnger));
		if (iAnger > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_ANGER_FEATURES", iAnger));
			szBuffer.append(NEWLINE);
		}
		iOldAnger = iNewAnger;

		iNewAnger -= std::min(0, city.getBonusBadHappiness());
		iAnger = ((iNewAnger - iOldAnger) + std::min(0, iOldAnger));
		if (iAnger > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_ANGER_BONUS", iAnger));
			szBuffer.append(NEWLINE);
		}
		iOldAnger = iNewAnger;

		iNewAnger -= std::min(0, city.getReligionBadHappiness());
		iAnger = ((iNewAnger - iOldAnger) + std::min(0, iOldAnger));
		if (iAnger > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_ANGER_RELIGIOUS_FREEDOM", iAnger));
			szBuffer.append(NEWLINE);
		}
		iOldAnger = iNewAnger;

		iNewAnger -= std::min(0, city.getCommerceHappiness());
		iAnger = ((iNewAnger - iOldAnger) + std::min(0, iOldAnger));
		if (iAnger > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_ANGER_BAD_ENTERTAINMENT", iAnger));
			szBuffer.append(NEWLINE);
		}
		iOldAnger = iNewAnger;

		iNewAnger -= std::min(0, city.area()->getBuildingHappiness(city.getOwnerINLINE()));
		iAnger = ((iNewAnger - iOldAnger) + std::min(0, iOldAnger));
		if (iAnger > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_ANGER_BUILDINGS", iAnger));
			szBuffer.append(NEWLINE);
		}
		iOldAnger = iNewAnger;

		iNewAnger -= std::min(0, GET_PLAYER(city.getOwnerINLINE()).getBuildingHappiness());
		iAnger = ((iNewAnger - iOldAnger) + std::min(0, iOldAnger));
		if (iAnger > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_ANGER_BUILDINGS", iAnger));
			szBuffer.append(NEWLINE);
		}
		iOldAnger = iNewAnger;

		iNewAnger -= std::min(0, (city.getExtraHappiness() + GET_PLAYER(city.getOwnerINLINE()).getExtraHappiness()));
		iAnger = ((iNewAnger - iOldAnger) + std::min(0, iOldAnger));
		if (iAnger > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_ANGER_ARGH", iAnger));
			szBuffer.append(NEWLINE);
		}
		iOldAnger = iNewAnger;

		iNewAnger -= std::min(0, GC.getHandicapInfo(city.getHandicapType()).getHappyBonus());
		iAnger = ((iNewAnger - iOldAnger) + std::min(0, iOldAnger));
		if (iAnger > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_ANGER_HANDICAP", iAnger));
			szBuffer.append(NEWLINE);
		}
		iOldAnger = iNewAnger;

		// MOD - START - Era Effects
		iNewAnger -= std::min(0, GET_PLAYER(city.getOwnerINLINE()).getEraUnhappiness());
		iAnger = ((iNewAnger - iOldAnger) + std::min(0, iOldAnger));
		if (iAnger > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_ERA_ANGER", iAnger));
			szBuffer.append(NEWLINE);
		}
		iOldAnger = iNewAnger;
		// MOD - END - Era Effects

		szBuffer.append(L"-----------------------\n");

		szBuffer.append(gDLL->getText("TXT_KEY_ANGER_TOTAL_UNHAPPY", iOldAnger));

		FAssert(iOldAnger == city.unhappyLevel());
	}
}


void CvGameTextMgr::setHappyHelp(CvWStringBuffer &szBuffer, CvCity& city)
{
	int iHappy;
	int iTotalHappy = 0;

	if (city.isOccupation() || GET_PLAYER(city.getOwnerINLINE()).isAnarchy())
	{
		return;
	}
	if (city.happyLevel() > 0)
	{
		iHappy = city.getLargestCityHappiness();
		if (iHappy > 0)
		{
			iTotalHappy += iHappy;
			szBuffer.append(gDLL->getText("TXT_KEY_HAPPY_BIG_CITY", iHappy));
			szBuffer.append(NEWLINE);
		}

		iHappy = city.getMilitaryHappiness();
		if (iHappy > 0)
		{
			iTotalHappy += iHappy;
			szBuffer.append(gDLL->getText("TXT_KEY_HAPPY_MILITARY_PRESENCE", iHappy));
			szBuffer.append(NEWLINE);
		}

		iHappy = city.getVassalHappiness();
		if (iHappy > 0)
		{
			iTotalHappy += iHappy;
			szBuffer.append(gDLL->getText("TXT_KEY_HAPPY_VASSAL", iHappy));
			szBuffer.append(NEWLINE);
		}

		iHappy = city.getCurrentStateReligionHappiness();
		if (iHappy > 0)
		{
			iTotalHappy += iHappy;
			szBuffer.append(gDLL->getText("TXT_KEY_HAPPY_STATE_RELIGION", iHappy));
			szBuffer.append(NEWLINE);
		}

		iHappy = (city.getBuildingGoodHappiness() + city.getExtraBuildingGoodHappiness());
		if (iHappy > 0)
		{
			iTotalHappy += iHappy;
			szBuffer.append(gDLL->getText("TXT_KEY_HAPPY_BUILDINGS", iHappy));
			szBuffer.append(NEWLINE);
		}

		iHappy = city.getFeatureGoodHappiness();
		if (iHappy > 0)
		{
			iTotalHappy += iHappy;
			szBuffer.append(gDLL->getText("TXT_KEY_HAPPY_FEATURES", iHappy));
			szBuffer.append(NEWLINE);
		}

		iHappy = city.getBonusGoodHappiness();
		if (iHappy > 0)
		{
			iTotalHappy += iHappy;
			szBuffer.append(gDLL->getText("TXT_KEY_HAPPY_BONUS", iHappy));
			szBuffer.append(NEWLINE);
		}

		iHappy = city.getReligionGoodHappiness();
		if (iHappy > 0)
		{
			iTotalHappy += iHappy;
			szBuffer.append(gDLL->getText("TXT_KEY_HAPPY_RELIGIOUS_FREEDOM", iHappy));
			szBuffer.append(NEWLINE);
		}

		iHappy = city.getCommerceHappiness();
		if (iHappy > 0)
		{
			iTotalHappy += iHappy;
			szBuffer.append(gDLL->getText("TXT_KEY_HAPPY_ENTERTAINMENT", iHappy));
			szBuffer.append(NEWLINE);
		}

		iHappy = city.area()->getBuildingHappiness(city.getOwnerINLINE());
		if (iHappy > 0)
		{
			iTotalHappy += iHappy;
			szBuffer.append(gDLL->getText("TXT_KEY_HAPPY_BUILDINGS", iHappy));
			szBuffer.append(NEWLINE);
		}

		iHappy = GET_PLAYER(city.getOwnerINLINE()).getBuildingHappiness();
		if (iHappy > 0)
		{
			iTotalHappy += iHappy;
			szBuffer.append(gDLL->getText("TXT_KEY_HAPPY_BUILDINGS", iHappy));
			szBuffer.append(NEWLINE);
		}

		iHappy = (city.getExtraHappiness() + GET_PLAYER(city.getOwnerINLINE()).getExtraHappiness());
		if (iHappy > 0)
		{
			iTotalHappy += iHappy;
			szBuffer.append(gDLL->getText("TXT_KEY_HAPPY_YEAH", iHappy));
			szBuffer.append(NEWLINE);
		}

		if (city.getHappinessTimer() > 0)
		{
			iHappy = GC.getDefineINT("TEMP_HAPPY");
			iTotalHappy += iHappy;
			szBuffer.append(gDLL->getText("TXT_KEY_HAPPY_TEMP", iHappy, city.getHappinessTimer()));
			szBuffer.append(NEWLINE);
		}

		iHappy = GC.getHandicapInfo(city.getHandicapType()).getHappyBonus();
		if (iHappy > 0)
		{
			iTotalHappy += iHappy;
			szBuffer.append(gDLL->getText("TXT_KEY_HAPPY_HANDICAP", iHappy));
			szBuffer.append(NEWLINE);
		}

		// MOD - START - Era Effects
		iHappy = GET_PLAYER(city.getOwnerINLINE()).getEraHappiness();
		if (iHappy > 0)
		{
			iTotalHappy += iHappy;
			szBuffer.append(gDLL->getText("TXT_KEY_ERA_HAPPINESS", iHappy));
			szBuffer.append(NEWLINE);
		}
		// MOD - END - Era Effects

		szBuffer.append(L"-----------------------\n");

		szBuffer.append(gDLL->getText("TXT_KEY_HAPPY_TOTAL_HAPPY", iTotalHappy));

		FAssert(iTotalHappy == city.happyLevel())
	}
}

// BUG - Building Additional Happiness - start
bool CvGameTextMgr::setBuildingAdditionalHappinessHelp(CvWStringBuffer &szBuffer, const CvCity& city, const CvWString& szStart, bool bStarted)
{
	CvWString szLabel;
	CvCivilizationInfo& kCivilization = GC.getCivilizationInfo(GET_PLAYER(city.getOwnerINLINE()).getCivilizationType());

	for (int iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
	{
		BuildingTypes eBuilding = (BuildingTypes)kCivilization.getCivilizationBuildings((BuildingClassTypes)iI);

		if (eBuilding != NO_BUILDING && city.canConstruct(eBuilding, false, true, false))
		{
			int iGood = 0, iBad = 0;
			int iChange = city.getAdditionalHappinessByBuilding(eBuilding, iGood, iBad);

			if (iGood != 0 || iBad != 0)
			{
				int iAngryPop = city.getAdditionalAngryPopuplation(iGood, iBad);

				if (!bStarted)
				{
					szBuffer.append(szStart);
					bStarted = true;
				}

				szLabel.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR("COLOR_BUILDING_TEXT"), GC.getBuildingInfo(eBuilding).getDescription());
				bool bStartedLine = setResumableGoodBadChangeHelp(szBuffer, szLabel, L": ", L"", iGood, gDLL->getSymbolID(HAPPY_CHAR), iBad, gDLL->getSymbolID(UNHAPPY_CHAR), false, true);
				setResumableValueChangeHelp(szBuffer, szLabel, L": ", L"", iAngryPop, gDLL->getSymbolID(ANGRY_POP_CHAR), false, true, bStartedLine);
			}
		}
	}

	return bStarted;
}
// BUG - Building Additional Happiness - end


/* replaced by BUG
void CvGameTextMgr::setYieldChangeHelp(CvWStringBuffer &szBuffer, const CvWString& szStart, const CvWString& szSpace, const CvWString& szEnd, const int* piYieldChange, bool bPercent, bool bNewLine)
{
	PROFILE_FUNC();

	CvWString szTempBuffer;
	bool bStarted;
	int iI;

	bStarted = false;

	for (iI = 0; iI < NUM_YIELD_TYPES; ++iI)
	{
		if (piYieldChange[iI] != 0)
		{
			if (!bStarted)
			{
				if (bNewLine)
				{
					szTempBuffer.Format(L"\n%c", gDLL->getSymbolID(BULLET_CHAR));
				}
				szTempBuffer += CvWString::format(L"%s%s%s%d%s%c",
					szStart.GetCString(),
					szSpace.GetCString(),
					piYieldChange[iI] > 0 ? L"+" : L"",
					piYieldChange[iI],
					bPercent ? L"%" : L"",
					GC.getYieldInfo((YieldTypes)iI).getChar());
			}
			else
			{
				szTempBuffer.Format(L", %s%d%s%c",
					piYieldChange[iI] > 0 ? L"+" : L"",
					piYieldChange[iI],
					bPercent ? L"%" : L"",
					GC.getYieldInfo((YieldTypes)iI).getChar());
			}
			szBuffer.append(szTempBuffer);

			bStarted = true;
		}
	}

	if (bStarted)
	{
		szBuffer.append(szEnd);
	}
}

void CvGameTextMgr::setCommerceChangeHelp(CvWStringBuffer &szBuffer, const CvWString& szStart, const CvWString& szSpace, const CvWString& szEnd, const int* piCommerceChange, bool bPercent, bool bNewLine)
{
	CvWString szTempBuffer;
	bool bStarted;
	int iI;

	bStarted = false;

	for (iI = 0; iI < NUM_COMMERCE_TYPES; ++iI)
	{
		if (piCommerceChange[iI] != 0)
		{
			if (!bStarted)
			{
				if (bNewLine)
				{
					szTempBuffer.Format(L"\n%c", gDLL->getSymbolID(BULLET_CHAR));
				}
				szTempBuffer += CvWString::format(L"%s%s%s%d%s%c", szStart.GetCString(), szSpace.GetCString(), ((piCommerceChange[iI] > 0) ? L"+" : L""), piCommerceChange[iI], ((bPercent) ? L"%" : L""), GC.getCommerceInfo((CommerceTypes) iI).getChar());
			}
			else
			{
				szTempBuffer.Format(L", %s%d%s%c", ((piCommerceChange[iI] > 0) ? L"+" : L""), piCommerceChange[iI], ((bPercent) ? L"%" : L""), GC.getCommerceInfo((CommerceTypes) iI).getChar());
			}
			szBuffer.append(szTempBuffer);

			bStarted = true;
		}
	}

	if (bStarted)
	{
		szBuffer.append(szEnd);
	}
} */

// BUG - Resumable Value Change Help - start
void CvGameTextMgr::setYieldChangeHelp(CvWStringBuffer &szBuffer, const CvWString& szStart, const CvWString& szSpace, const CvWString& szEnd, const int* piYieldChange, bool bPercent, bool bNewLine)
{
	setResumableYieldChangeHelp(szBuffer, szStart, szSpace, szEnd, piYieldChange, bPercent, bNewLine);
}

/*
 * Adds the ability to pass in and get back the value of bStarted so that
 * it can be used with other setResumable<xx>ChangeHelp() calls on a single line.
 */
bool CvGameTextMgr::setResumableYieldChangeHelp(CvWStringBuffer &szBuffer, const CvWString& szStart, const CvWString& szSpace, const CvWString& szEnd, const int* piYieldChange, bool bPercent, bool bNewLine, bool bStarted)
{
	CvWString szTempBuffer;
//	bool bStarted;
	int iI;

//	bStarted = false;

	for (iI = 0; iI < NUM_YIELD_TYPES; ++iI)
	{
		if (piYieldChange[iI] != 0)
		{
			if (!bStarted)
			{
				if (bNewLine)
				{
					szTempBuffer.Format(L"\n%c", gDLL->getSymbolID(BULLET_CHAR));
				}
				szTempBuffer += CvWString::format(L"%s%s%s%d%s%c",
					szStart.GetCString(),
					szSpace.GetCString(),
					piYieldChange[iI] > 0 ? L"+" : L"",
					piYieldChange[iI],
					bPercent ? L"%" : L"",
					GC.getYieldInfo((YieldTypes)iI).getChar());
			}
			else
			{
				szTempBuffer.Format(L", %s%d%s%c",
					piYieldChange[iI] > 0 ? L"+" : L"",
					piYieldChange[iI],
					bPercent ? L"%" : L"",
					GC.getYieldInfo((YieldTypes)iI).getChar());
			}
			szBuffer.append(szTempBuffer);

			bStarted = true;
		}
	}

	if (bStarted)
	{
		szBuffer.append(szEnd);
	}

// added
	return bStarted;
}

void CvGameTextMgr::setCommerceChangeHelp(CvWStringBuffer &szBuffer, const CvWString& szStart, const CvWString& szSpace, const CvWString& szEnd, const int* piCommerceChange, bool bPercent, bool bNewLine)
{
	setResumableCommerceChangeHelp(szBuffer, szStart, szSpace, szEnd, piCommerceChange, bPercent, bNewLine);
}

/*
 * Adds the ability to pass in and get back the value of bStarted so that
 * it can be used with other setResumable<xx>ChangeHelp() calls on a single line.
 */
bool CvGameTextMgr::setResumableCommerceChangeHelp(CvWStringBuffer &szBuffer, const CvWString& szStart, const CvWString& szSpace, const CvWString& szEnd, const int* piCommerceChange, bool bPercent, bool bNewLine, bool bStarted)
{
	CvWString szTempBuffer;
//	bool bStarted;
	int iI;

//	bStarted = false;

	for (iI = 0; iI < NUM_COMMERCE_TYPES; ++iI)
	{
		if (piCommerceChange[iI] != 0)
		{
			if (!bStarted)
			{
				if (bNewLine)
				{
					szTempBuffer.Format(L"\n%c", gDLL->getSymbolID(BULLET_CHAR));
				}
				szTempBuffer += CvWString::format(L"%s%s%s%d%s%c", szStart.GetCString(), szSpace.GetCString(), ((piCommerceChange[iI] > 0) ? L"+" : L""), piCommerceChange[iI], ((bPercent) ? L"%" : L""), GC.getCommerceInfo((CommerceTypes) iI).getChar());
			}
			else
			{
				szTempBuffer.Format(L", %s%d%s%c", ((piCommerceChange[iI] > 0) ? L"+" : L""), piCommerceChange[iI], ((bPercent) ? L"%" : L""), GC.getCommerceInfo((CommerceTypes) iI).getChar());
			}
			szBuffer.append(szTempBuffer);

			bStarted = true;
		}
	}

	if (bStarted)
	{
		szBuffer.append(szEnd);
	}

// added
	return bStarted;
}

/*
 * Displays float values by dividing each value by 100.
 */
void CvGameTextMgr::setCommerceTimes100ChangeHelp(CvWStringBuffer &szBuffer, const CvWString& szStart, const CvWString& szSpace, const CvWString& szEnd, const int* piCommerceChange, bool bNewLine, bool bStarted)
{
	setResumableCommerceTimes100ChangeHelp(szBuffer, szStart, szSpace, szEnd, piCommerceChange, bNewLine);
}

/*
 * Adds the ability to pass in and get back the value of bStarted so that
 * it can be used with other setResumable<xx>ChangeHelp() calls on a single line.
 */
bool CvGameTextMgr::setResumableCommerceTimes100ChangeHelp(CvWStringBuffer &szBuffer, const CvWString& szStart, const CvWString& szSpace, const CvWString& szEnd, const int* piCommerceChange, bool bNewLine, bool bStarted)
{
	CvWString szTempBuffer;

	for (int iI = 0; iI < NUM_COMMERCE_TYPES; ++iI)
	{
		int iChange = piCommerceChange[iI];
		if (iChange != 0)
		{
			if (!bStarted)
			{
				if (bNewLine)
				{
					szTempBuffer.Format(L"\n%c", gDLL->getSymbolID(BULLET_CHAR));
				}
				szTempBuffer += CvWString::format(L"%s%s", szStart.GetCString(), szSpace.GetCString());
				bStarted = true;
			}
			else
			{
				szTempBuffer.Format(L", ");
			}
			szBuffer.append(szTempBuffer);

			if (iChange % 100 == 0)
			{
				szTempBuffer.Format(L"%+d%c", iChange / 100, GC.getCommerceInfo((CommerceTypes) iI).getChar());
			}
			else
			{
				if (iChange >= 0)
				{
					szBuffer.append(L"+");
				}
				else
				{
					iChange = - iChange;
					szBuffer.append(L"-");
				}
				szTempBuffer.Format(L"%d.%02d%c", iChange / 100, iChange % 100, GC.getCommerceInfo((CommerceTypes) iI).getChar());
			}
			szBuffer.append(szTempBuffer);
		}
	}

	if (bStarted)
	{
		szBuffer.append(szEnd);
	}

	return bStarted;
}

/*
 * Adds the ability to pass in and get back the value of bStarted so that
 * it can be used with other setResumable<xx>ChangeHelp() calls on a single line.
 */
bool CvGameTextMgr::setResumableGoodBadChangeHelp(CvWStringBuffer &szBuffer, const CvWString& szStart, const CvWString& szSpace, const CvWString& szEnd, int iGood, int iGoodSymbol, int iBad, int iBadSymbol, bool bPercent, bool bNewLine, bool bStarted)
{
	bStarted = setResumableValueChangeHelp(szBuffer, szStart, szSpace, szEnd, iGood, iGoodSymbol, bPercent, bNewLine, bStarted);
	bStarted = setResumableValueChangeHelp(szBuffer, szStart, szSpace, szEnd, iBad, iBadSymbol, bPercent, bNewLine, bStarted);

	return bStarted;
}

/*
 * Adds the ability to pass in and get back the value of bStarted so that
 * it can be used with other setResumable<xx>ChangeHelp() calls on a single line.
 */
bool CvGameTextMgr::setResumableValueChangeHelp(CvWStringBuffer &szBuffer, const CvWString& szStart, const CvWString& szSpace, const CvWString& szEnd, int iValue, int iSymbol, bool bPercent, bool bNewLine, bool bStarted)
{
	CvWString szTempBuffer;

	if (iValue != 0)
	{
		if (!bStarted)
		{
			if (bNewLine)
			{
				szTempBuffer.Format(L"\n%c", gDLL->getSymbolID(BULLET_CHAR));
			}
			szTempBuffer += CvWString::format(L"%s%s", szStart.GetCString(), szSpace.GetCString());
		}
		else
		{
			szTempBuffer = L", ";
		}
		szBuffer.append(szTempBuffer);

		szTempBuffer.Format(L"%+d%s%c", iValue, bPercent ? L"%" : L"", iSymbol);
		szBuffer.append(szTempBuffer);

		bStarted = true;
	}

	return bStarted;
}

/*
 * Adds the ability to pass in and get back the value of bStarted so that
 * it can be used with other setResumable<xx>ChangeHelp() calls on a single line.
 */
bool CvGameTextMgr::setResumableValueTimes100ChangeHelp(CvWStringBuffer &szBuffer, const CvWString& szStart, const CvWString& szSpace, const CvWString& szEnd, int iValue, int iSymbol, bool bNewLine, bool bStarted)
{
	CvWString szTempBuffer;

	if (iValue != 0)
	{
		if (!bStarted)
		{
			if (bNewLine)
			{
				szTempBuffer.Format(L"\n%c", gDLL->getSymbolID(BULLET_CHAR));
			}
			szTempBuffer += CvWString::format(L"%s%s", szStart.GetCString(), szSpace.GetCString());
		}
		else
		{
			szTempBuffer = L", ";
		}
		szBuffer.append(szTempBuffer);

		if (iValue % 100 == 0)
		{
			szTempBuffer.Format(L"%+d%c", iValue / 100, iSymbol);
		}
		else
		{
			if (iValue >= 0)
			{
				szBuffer.append(L"+");
			}
			else
			{
				iValue = - iValue;
				szBuffer.append(L"-");
			}
			szTempBuffer.Format(L"%d.%02d%c", iValue / 100, iValue % 100, iSymbol);
		}
		szBuffer.append(szTempBuffer);

		bStarted = true;
	}

	return bStarted;
}

bool CvGameTextMgr::setResumableValueTimes10ChangeHelp(CvWStringBuffer &szBuffer, const CvWString& szStart, const CvWString& szSpace, const CvWString& szEnd, int iValue, int iSymbol, bool bPercent, bool bNewLine, bool bStarted)
{
	CvWString szTempBuffer;

	if (iValue != 0)
	{
		if (!bStarted)
		{
			if (bNewLine)
			{
				szTempBuffer.Format(L"\n%c", gDLL->getSymbolID(BULLET_CHAR));
			}
			szTempBuffer += CvWString::format(L"%s%s", szStart.GetCString(), szSpace.GetCString());
		}
		else
		{
			szTempBuffer = L", ";
		}
		szBuffer.append(szTempBuffer);

		if (iValue % 10 == 0)
		{
			szTempBuffer.Format(L"%+d%s%c", iValue / 10, bPercent ? L"%" : L"", iSymbol);
		}
		else
		{
			if (iValue >= 0)
			{
				szBuffer.append(L"+");
			}
			else
			{
				iValue = - iValue;
				szBuffer.append(L"-");
			}
			szTempBuffer.Format(L"%d.%2d%s%c", iValue / 10, (iValue % 10) * 10, bPercent ? L"%" : L"", iSymbol);
		}
		szBuffer.append(szTempBuffer);

		bStarted = true;
	}

	return bStarted;
}
// BUG - Resumable Value Change Help - end


// This function has been effectly rewritten for K-Mod. (there were a lot of things to change.)
void CvGameTextMgr::setBonusHelp(CvWStringBuffer &szBuffer, BonusTypes eBonus, bool bCivilopediaText)
{
	if (NO_BONUS == eBonus)
	{
		return;
	}

	CvBonusInfo& kBonus = GC.getBonusInfo(eBonus);

	int iHappiness = kBonus.getHappiness();
	int iHealth = kBonus.getHealth();

	if (bCivilopediaText)
	{
		// K-Mod. for the civilopedia text, display the basic bonuses as individual bullet points.
		// (they are displayed beside the name of the bonus when outside of the civilopedia.)
		if (iHappiness != 0)
		{
			szBuffer.append(CvWString::format(L"\n%c+%d%c", gDLL->getSymbolID(BULLET_CHAR), abs(iHappiness), iHappiness > 0 ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR)));
		}
		if (iHealth != 0)
		{
			szBuffer.append(CvWString::format(L"\n%c+%d%c", gDLL->getSymbolID(BULLET_CHAR), abs(iHealth), iHealth > 0 ? gDLL->getSymbolID(HEALTHY_CHAR) : gDLL->getSymbolID(UNHEALTHY_CHAR)));
		}
	}
	else
	{
		szBuffer.append(CvWString::format( SETCOLR L"%s" ENDCOLR , TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), kBonus.getDescription()));

		if (NO_PLAYER != GC.getGameINLINE().getActivePlayer())
		{
			CvPlayer& kActivePlayer = GET_PLAYER(GC.getGameINLINE().getActivePlayer());

			// K-Mod. Bonuses now display "(Obsolete)" instead of "(player has 0)" when the bonus is obsolete.
			if (GET_TEAM(kActivePlayer.getTeam()).isBonusObsolete(eBonus))
			{
				szBuffer.append(gDLL->getText("TXT_KEY_BONUS_OBSOLETE"));
			}
			else
			{
				// display the basic bonuses next to the name of the bonus
				if (iHappiness != 0)
				{
					szBuffer.append(CvWString::format(L", +%d%c", abs(iHappiness), iHappiness > 0 ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR)));
				}
				if (iHealth != 0)
				{
					szBuffer.append(CvWString::format(L", +%d%c", abs(iHealth), iHealth > 0 ? gDLL->getSymbolID(HEALTHY_CHAR) : gDLL->getSymbolID(UNHEALTHY_CHAR)));
				}

				szBuffer.append(gDLL->getText("TXT_KEY_BONUS_AVAILABLE_PLAYER", kActivePlayer.getNumAvailableBonuses(eBonus), kActivePlayer.getNameKey()));

				for (int iCorp = 0; iCorp < GC.getNumCorporationInfos(); ++iCorp)
				{
					bool bCorpBonus = false;
					if (kActivePlayer.getHasCorporationCount((CorporationTypes)iCorp) > 0)
					{
						for (int i = 0; !bCorpBonus && i < GC.getNUM_CORPORATION_PREREQ_BONUSES(); ++i)
						{
							bCorpBonus = eBonus == GC.getCorporationInfo((CorporationTypes)iCorp).getPrereqBonus(i);
						}
					}

					if (bCorpBonus)
						szBuffer.append(GC.getCorporationInfo((CorporationTypes)iCorp).getChar());
				}
			}
		}

		setYieldChangeHelp(szBuffer, L"", L"", gDLL->getText("TXT_KEY_BONUS_ON_PLOT"), kBonus.getYieldChangeArray());

		if (kBonus.getTechReveal() != NO_TECH)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_BONUS_REVEALED_BY", GC.getTechInfo((TechTypes)kBonus.getTechReveal()).getTextKeyWide()));
		}
	}

	// K-Mod. Only display the perks of the bonus if it is not already obsolete
	if (bCivilopediaText || GC.getGameINLINE().getActiveTeam() == NO_TEAM || !GET_TEAM(GC.getGameINLINE().getActiveTeam()).isBonusObsolete(eBonus))
	{
		CivilizationTypes eCivilization = GC.getGameINLINE().getActiveCivilizationType();

		// MOD - START - Relation Caching
		for (int iRel = 0; iRel < kBonus.getNumRelatedBuildingClasses(); iRel++)
		{
			BuildingClassTypes eLoopBuildingClass = (BuildingClassTypes)kBonus.getRelatedBuildingClasses(iRel);
			BuildingTypes eLoopBuilding;
			if (eCivilization == NO_CIVILIZATION)
			{
				eLoopBuilding = ((BuildingTypes)(GC.getBuildingClassInfo(eLoopBuildingClass).getDefaultBuildingIndex()));
			}
			else
			{
				eLoopBuilding = ((BuildingTypes)(GC.getCivilizationInfo(eCivilization).getCivilizationBuildings(eLoopBuildingClass)));
			}

			if (eLoopBuilding != NO_BUILDING)
			{
				CvBuildingInfo& kBuilding = GC.getBuildingInfo(eLoopBuilding);
				if (kBuilding.getBonusHappinessChanges(eBonus) != 0)
				{
					szBuffer.append(CvWString::format(L"\n%s", gDLL->getText("TXT_KEY_BUILDING_CIVIC_HEALTH_HAPPINESS_CHANGE", abs(kBuilding.getBonusHappinessChanges(eBonus)),
						kBuilding.getBonusHappinessChanges(eBonus) > 0 ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR)).c_str()));
					szBuffer.append(CvWString::format(L"<link=literal>%s</link>", kBuilding.getDescription()));
				}

				if (kBuilding.getBonusHealthChanges(eBonus) != 0)
				{
					szBuffer.append(CvWString::format(L"\n%s", gDLL->getText("TXT_KEY_BUILDING_CIVIC_HEALTH_HAPPINESS_CHANGE", abs(kBuilding.getBonusHealthChanges(eBonus)),
						kBuilding.getBonusHealthChanges(eBonus) > 0 ? gDLL->getSymbolID(HEALTHY_CHAR) : gDLL->getSymbolID(UNHEALTHY_CHAR)).c_str()));
					szBuffer.append(CvWString::format(L"<link=literal>%s</link>", kBuilding.getDescription()));
				}
			}
		}
		// MOD - END - Relation Caching

		// MOD - START - Religion Affected Bonuses
		CvWString szTempBuffer;
		CvWString szBonusHealthiness;
		CvWString szBonusHappiness;

		szBonusHealthiness.clear();
		szBonusHappiness.clear();

		for (int iI = 0; iI < GC.getNumReligionInfos(); iI++)
		{
			ReligionTypes eReligion = (ReligionTypes)iI;

			if (kBonus.isNoReligionHealthiness(eReligion))
			{
				if (szBonusHealthiness.empty())
				{
					szTempBuffer.Format(L"%c%s", GC.getReligionInfo(eReligion).getChar(), GC.getReligionInfo(eReligion).getDescription());
					szBonusHealthiness.append(szTempBuffer);
				}
				else
				{
					szTempBuffer.Format(L", %c%s", GC.getReligionInfo(eReligion).getChar(), GC.getReligionInfo(eReligion).getDescription());
					szBonusHealthiness.append(szTempBuffer);
				}
			}

			if (kBonus.isNoReligionHappiness(eReligion))
			{
				if (szBonusHappiness.empty())
				{
					szTempBuffer.Format(L"%c%s", GC.getReligionInfo(eReligion).getChar(), GC.getReligionInfo(eReligion).getDescription());
					szBonusHappiness.append(szTempBuffer);
				}
				else
				{
					szTempBuffer.Format(L", %c%s", GC.getReligionInfo(eReligion).getChar(), GC.getReligionInfo(eReligion).getDescription());
					szBonusHappiness.append(szTempBuffer);
				}
			}
		}

		if (!szBonusHealthiness.empty())
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_RELIGION_NO_HEALTH_FROM_BONUS", szBonusHealthiness.GetCString()));
		}

		if (!szBonusHappiness.empty())
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_RELIGION_NO_HAPPY_FROM_BONUS", szBonusHappiness.GetCString()));
		}
		// MOD - END - Religion Affected Bonuses

		// MOD - START - Epidemics
		// Epidemic system by Mexico
		if (kBonus.getEpidemicModifier() != 0)
		{
			szTempBuffer.Format(L"%+d", (kBonus.getEpidemicModifier() * 100) / GC.getDefineINT("EPIDEMIC_RAND_BASE"));
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_BONUS_EPIDEMIC_MODIFIER", szTempBuffer.GetCString()));

			/*
			if (eImprovement != NO_IMPROVEMENT)
			{
				szBuffer.append(gDLL->getText("TXT_KEY_BONUS_WITH_IMPROVEMENT", GC.getImprovementInfo(eImprovement).getTextKeyWide()));
			}
			*/
		}
		// MOD - END - Epidemics

		if (!CvWString(kBonus.getHelp()).empty())
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(kBonus.getHelp());
		}
	}
}


void CvGameTextMgr::setReligionHelp(CvWStringBuffer &szBuffer, ReligionTypes eReligion, bool bCivilopedia)
{
	UnitTypes eFreeUnit;

	if (NO_RELIGION == eReligion)
	{
		return;
	}
	CvReligionInfo& religion = GC.getReligionInfo(eReligion);

	if (!bCivilopedia)
	{
		szBuffer.append(CvWString::format(SETCOLR L"%s" ENDCOLR , TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), religion.getDescription()));
	}

	setCommerceChangeHelp(szBuffer, gDLL->getText("TXT_KEY_RELIGION_HOLY_CITY").c_str(), L": ", L"", religion.getHolyCityCommerceArray());
	setCommerceChangeHelp(szBuffer, gDLL->getText("TXT_KEY_RELIGION_ALL_CITIES").c_str(), L": ", L"", religion.getStateReligionCommerceArray());

	if (!bCivilopedia)
	{
		if (religion.getTechPrereq() != NO_TECH)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_RELIGION_FOUNDED_FIRST", GC.getTechInfo((TechTypes)religion.getTechPrereq()).getTextKeyWide()));
		}
	}

	if (religion.getFreeUnitClass() != NO_UNITCLASS)
	{
		if (GC.getGameINLINE().getActivePlayer() != NO_PLAYER)
		{
			eFreeUnit = ((UnitTypes)(GC.getCivilizationInfo(GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getCivilizationType()).getCivilizationUnits(religion.getFreeUnitClass())));
		}
		else
		{
			eFreeUnit = (UnitTypes)GC.getUnitClassInfo((UnitClassTypes)religion.getFreeUnitClass()).getDefaultUnitIndex();
		}

		if (eFreeUnit != NO_UNIT)
		{
			if (religion.getNumFreeUnits() > 1)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_RELIGION_FOUNDER_RECEIVES_NUM", GC.getUnitInfo(eFreeUnit).getTextKeyWide(), religion.getNumFreeUnits()));
			}
			else if (religion.getNumFreeUnits() > 0)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_RELIGION_FOUNDER_RECEIVES", GC.getUnitInfo(eFreeUnit).getTextKeyWide()));
			}
		}
	}

	// MOD - START - Religion Affected Bonuses
	CvWString szTempBuffer;
	CvWString szBonusHealthiness;
	CvWString szBonusHappiness;

	szBonusHealthiness.clear();
	szBonusHappiness.clear();

	for (int iI = 0; iI < GC.getNumBonusInfos(); iI++)
	{
		BonusTypes eBonus = (BonusTypes)iI;

		if (GC.getBonusInfo(eBonus).isNoReligionHealthiness(eReligion))
		{
			if (szBonusHealthiness.empty())
			{
				szTempBuffer.Format(L"%c%s", GC.getBonusInfo(eBonus).getChar(), GC.getBonusInfo(eBonus).getDescription());
				szBonusHealthiness.append(szTempBuffer);
			}
			else
			{
				szTempBuffer.Format(L", %c%s", GC.getBonusInfo(eBonus).getChar(), GC.getBonusInfo(eBonus).getDescription());
				szBonusHealthiness.append(szTempBuffer);
			}
		}

		if (GC.getBonusInfo(eBonus).isNoReligionHappiness(eReligion))
		{
			if (szBonusHappiness.empty())
			{
				szTempBuffer.Format(L"%c%s", GC.getBonusInfo(eBonus).getChar(), GC.getBonusInfo(eBonus).getDescription());
				szBonusHappiness.append(szTempBuffer);
			}
			else
			{
				szTempBuffer.Format(L", %c%s", GC.getBonusInfo(eBonus).getChar(), GC.getBonusInfo(eBonus).getDescription());
				szBonusHappiness.append(szTempBuffer);
			}
		}
	}

	if (!szBonusHealthiness.empty())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BONUS_NO_HEALTH_WITH_RELIGION", szBonusHealthiness.GetCString()));
	}

	if (!szBonusHappiness.empty())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_BONUS_NO_HAPPY_WITH_RELIGION", szBonusHappiness.GetCString()));
	}
	// MOD - END - Religion Affected Bonuses
}

void CvGameTextMgr::setReligionHelpCity(CvWStringBuffer &szBuffer, ReligionTypes eReligion, CvCity *pCity, bool bCityScreen, bool bForceReligion, bool bForceState, bool bNoStateReligion)
{
	int i;
	CvWString szTempBuffer;
	bool bHandled = false;
	int iCommerce;
	int iHappiness;
	int iProductionModifier;
	int iFreeExperience;
	int iGreatPeopleRateModifier;

	if (pCity == NULL)
	{
		return;
	}

	ReligionTypes eStateReligion = (bNoStateReligion ? NO_RELIGION : GET_PLAYER(pCity->getOwnerINLINE()).getStateReligion());

	if (bCityScreen)
	{
		szBuffer.append(CvWString::format(SETCOLR L"%s" ENDCOLR , TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), GC.getReligionInfo(eReligion).getDescription()));
		szBuffer.append(NEWLINE);

		if (!(GC.getGameINLINE().isReligionFounded(eReligion)) && !GC.getGameINLINE().isOption(GAMEOPTION_PICK_RELIGION))
		{
			if (GC.getReligionInfo(eReligion).getTechPrereq() != NO_TECH)
			{
				szBuffer.append(gDLL->getText("TXT_KEY_RELIGION_FOUNDED_FIRST", GC.getTechInfo((TechTypes)(GC.getReligionInfo(eReligion).getTechPrereq())).getTextKeyWide()));
			}
		}
	}

	// K-Mod
	if (GC.getGameINLINE().isReligionFounded(eReligion) && pCity && gDLL->getChtLvl() > 0 && GC.ctrlKey())
	{
		szBuffer.append(CvWString::format(L"grip: %d", pCity->getReligionGrip(eReligion)));
		szBuffer.append(NEWLINE);
	}
	// K-Mod end

	if (!bForceReligion)
	{
		if (!(pCity->isHasReligion(eReligion)))
		{
			return;
		}
	}

	if (eStateReligion == eReligion || eStateReligion == NO_RELIGION || bForceState)
	{
		for (i = 0; i < NUM_COMMERCE_TYPES; i++)
		{
			iCommerce = GC.getReligionInfo(eReligion).getStateReligionCommerce((CommerceTypes)i);

			if (pCity->isHolyCity(eReligion))
			{
				iCommerce += GC.getReligionInfo(eReligion).getHolyCityCommerce((CommerceTypes)i);
			}

			if (iCommerce != 0)
			{
				if (bHandled)
				{
					szBuffer.append(L", ");
				}

				szTempBuffer.Format(L"%s%d%c", iCommerce > 0 ? "+" : "", iCommerce, GC.getCommerceInfo((CommerceTypes)i).getChar());
				szBuffer.append(szTempBuffer);
				bHandled = true;
			}
		}
	}

	if (eStateReligion == eReligion || bForceState)
	{
		iHappiness = (pCity->getStateReligionHappiness(eReligion) + GET_PLAYER(pCity->getOwnerINLINE()).getStateReligionHappiness());

		if (iHappiness != 0)
		{
			if (bHandled)
			{
				szBuffer.append(L", ");
			}

/************************************************************************************************/
/* UNOFFICIAL_PATCH                       08/28/09                             jdog5000         */
/*                                                                                              */
/* Bugfix                                                                                       */
/************************************************************************************************/
/* original bts code
			szTempBuffer.Format(L"%d%c", iHappiness, ((iHappiness > 0) ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR)));
*/
			// Use absolute value with unhappy face
			szTempBuffer.Format(L"%d%c", abs(iHappiness), ((iHappiness > 0) ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR)));
/************************************************************************************************/
/* UNOFFICIAL_PATCH                        END                                                  */
/************************************************************************************************/
			szBuffer.append(szTempBuffer);
			bHandled = true;
		}

		iProductionModifier = GET_PLAYER(pCity->getOwnerINLINE()).getStateReligionBuildingProductionModifier();
		if (iProductionModifier != 0)
		{
			if (bHandled)
			{
				szBuffer.append(L", ");
			}

			szBuffer.append(gDLL->getText("TXT_KEY_RELIGION_BUILDING_PROD_MOD", iProductionModifier));
			bHandled = true;
		}

		iProductionModifier = GET_PLAYER(pCity->getOwnerINLINE()).getStateReligionUnitProductionModifier();
		if (iProductionModifier != 0)
		{
			if (bHandled)
			{
				szBuffer.append(L", ");
			}

			szBuffer.append(gDLL->getText("TXT_KEY_RELIGION_UNIT_PROD_MOD", iProductionModifier));
			bHandled = true;
		}

		iFreeExperience = GET_PLAYER(pCity->getOwnerINLINE()).getStateReligionFreeExperience();
		if (iFreeExperience != 0)
		{
			if (bHandled)
			{
				szBuffer.append(L", ");
			}

			szBuffer.append(gDLL->getText("TXT_KEY_RELIGION_FREE_XP", iFreeExperience));
			bHandled = true;
		}

		iGreatPeopleRateModifier = GET_PLAYER(pCity->getOwnerINLINE()).getStateReligionGreatPeopleRateModifier();
		if (iGreatPeopleRateModifier != 0)
		{
			if (bHandled)
			{
				szBuffer.append(L", ");
			}

			szBuffer.append(gDLL->getText("TXT_KEY_RELIGION_BIRTH_RATE_MOD", iGreatPeopleRateModifier));
			bHandled = true;
		}
	}

	// MOD - START - Holy City Migration
	const PlayerTypes eOwner = pCity->getOwnerINLINE();
	const CvPlayer& kOwner = GET_PLAYER(eOwner);

	if (pCity->isHolyCity(eReligion))
	{
		if (bHandled)
		{
			szBuffer.append(NEWLINE);
		}

		szBuffer.append(gDLL->getText("TXT_KEY_RELIGION_HOLY_CITY_INFLUENCE", kOwner.getHolyCityInfluence(eReligion)));

		if (!kOwner.canMaintainHolyCity(eReligion) && GC.getGameINLINE().getNumHolyCityCandidates(eReligion, eOwner) > 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_RELIGION_HOLY_CITY_TURNS_BEFORE_LOSS", kOwner.getHolyCityLossTurnsRemaining(eReligion)));
		}
	}
	else if (kOwner.canAcquireHolyCity(eReligion) && pCity->isHolyCityCandidate(eReligion) && GC.getGameINLINE().getHolyCity(eReligion) == NULL)
	{
		if (bHandled)
		{
			szBuffer.append(NEWLINE);
		}
		
		szBuffer.append(gDLL->getText("TXT_KEY_RELIGION_HOLY_CITY_INFLUENCE", kOwner.getHolyCityInfluence(eReligion)));
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_RELIGION_HOLY_CITY_CANDIDATE", GC.getGameINLINE().getNumHolyCityCandidates(eReligion, eOwner)));
	}
	// MOD - END - Holy City Migration
}

void CvGameTextMgr::setCorporationHelp(CvWStringBuffer &szBuffer, CorporationTypes eCorporation, bool bCivilopedia)
{
	UnitTypes eFreeUnit;
	CvWString szTempBuffer;

	if (NO_CORPORATION == eCorporation)
	{
		return;
	}
	CvCorporationInfo& kCorporation = GC.getCorporationInfo(eCorporation);

	if (!bCivilopedia)
	{
		szBuffer.append(CvWString::format(SETCOLR L"%s" ENDCOLR , TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), kCorporation.getDescription()));
	}

	szTempBuffer.clear();
	for (int iI = 0; iI < NUM_YIELD_TYPES; ++iI)
	{
		int iYieldProduced = GC.getCorporationInfo(eCorporation).getYieldProduced((YieldTypes)iI);
		if (NO_PLAYER != GC.getGameINLINE().getActivePlayer())
		{
			iYieldProduced *= GC.getWorldInfo(GC.getMapINLINE().getWorldSize()).getCorporationMaintenancePercent();
			iYieldProduced /= 100;
		}

		if (iYieldProduced != 0)
		{
			if (!szTempBuffer.empty())
			{
				szTempBuffer += L", ";
			}

			if (iYieldProduced % 100 == 0)
			{
				szTempBuffer += CvWString::format(L"%s%d%c",
					iYieldProduced > 0 ? L"+" : L"",
					iYieldProduced / 100,
					GC.getYieldInfo((YieldTypes)iI).getChar());
			}
			else
			{
				szTempBuffer += CvWString::format(L"%s%.2f%c",
					iYieldProduced > 0 ? L"+" : L"",
					0.01f * abs(iYieldProduced),
					GC.getYieldInfo((YieldTypes)iI).getChar());
			}
		}
	}

	if (!szTempBuffer.empty())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_CORPORATION_ALL_CITIES", szTempBuffer.GetCString()));
	}

	szTempBuffer.clear();
	for (int iI = 0; iI < NUM_COMMERCE_TYPES; ++iI)
	{
		int iCommerceProduced = GC.getCorporationInfo(eCorporation).getCommerceProduced((CommerceTypes)iI);
		if (NO_PLAYER != GC.getGameINLINE().getActivePlayer())
		{
			iCommerceProduced *= GC.getWorldInfo(GC.getMapINLINE().getWorldSize()).getCorporationMaintenancePercent();
			iCommerceProduced /= 100;
		}
		if (iCommerceProduced != 0)
		{
			if (!szTempBuffer.empty())
			{
				szTempBuffer += L", ";
			}

			if (iCommerceProduced % 100 == 0)
			{
				szTempBuffer += CvWString::format(L"%s%d%c",
					iCommerceProduced > 0 ? L"+" : L"",
					iCommerceProduced / 100,
					GC.getCommerceInfo((CommerceTypes)iI).getChar());
			}
			else
			{
				szTempBuffer += CvWString::format(L"%s%.2f%c",
					iCommerceProduced > 0 ? L"+" : L"",
					0.01f * abs(iCommerceProduced),
					GC.getCommerceInfo((CommerceTypes)iI).getChar());
			}

		}
	}

	if (!szTempBuffer.empty())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_CORPORATION_ALL_CITIES", szTempBuffer.GetCString()));
	}

	if (!bCivilopedia)
	{
		if (kCorporation.getTechPrereq() != NO_TECH)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_CORPORATION_FOUNDED_FIRST", GC.getTechInfo((TechTypes)kCorporation.getTechPrereq()).getTextKeyWide()));
		}
	}

	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_CORPORATION_BONUS_REQUIRED"));
	bool bFirst = true;
	for (int i = 0; i < GC.getNUM_CORPORATION_PREREQ_BONUSES(); ++i)
	{
		if (NO_BONUS != kCorporation.getPrereqBonus(i))
		{
			if (bFirst)
			{
				bFirst = false;
			}
			else
			{
				szBuffer.append(L", ");
			}

			szBuffer.append(CvWString::format(L"%c", GC.getBonusInfo((BonusTypes)kCorporation.getPrereqBonus(i)).getChar()));
		}
	}

	if (kCorporation.getBonusProduced() != NO_BONUS)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_CORPORATION_BONUS_PRODUCED", GC.getBonusInfo((BonusTypes)kCorporation.getBonusProduced()).getChar()));
	}

	if (kCorporation.getFreeUnitClass() != NO_UNITCLASS)
	{
		if (GC.getGameINLINE().getActivePlayer() != NO_PLAYER)
		{
			eFreeUnit = ((UnitTypes)(GC.getCivilizationInfo(GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getCivilizationType()).getCivilizationUnits(kCorporation.getFreeUnitClass())));
		}
		else
		{
			eFreeUnit = (UnitTypes)GC.getUnitClassInfo((UnitClassTypes)kCorporation.getFreeUnitClass()).getDefaultUnitIndex();
		}

		if (eFreeUnit != NO_UNIT)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_RELIGION_FOUNDER_RECEIVES", GC.getUnitInfo(eFreeUnit).getTextKeyWide()));
		}
	}

	std::vector<CorporationTypes> aCompetingCorps;
	bFirst = true;
	for (int iCorporation = 0; iCorporation < GC.getNumCorporationInfos(); ++iCorporation)
	{
		if (iCorporation != eCorporation)
		{
			bool bCompeting = false;

			CvCorporationInfo& kLoopCorporation = GC.getCorporationInfo((CorporationTypes)iCorporation);
			for (int i = 0; i < GC.getNUM_CORPORATION_PREREQ_BONUSES(); ++i)
			{
				if (kCorporation.getPrereqBonus(i) != NO_BONUS)
				{
					for (int j = 0; j < GC.getNUM_CORPORATION_PREREQ_BONUSES(); ++j)
					{
						if (kLoopCorporation.getPrereqBonus(j) == kCorporation.getPrereqBonus(i))
						{
							bCompeting = true;
							break;
						}
					}
				}

				if (bCompeting)
				{
					break;
				}
			}

			if (bCompeting)
			{
				CvWString szTemp = CvWString::format(L"<link=literal>%s</link>", kLoopCorporation.getDescription());
				setListHelp(szBuffer, gDLL->getText("TXT_KEY_CORPORATION_COMPETES").c_str(), szTemp.GetCString(), L", ", bFirst);
				bFirst = false;
			}
		}
	}
}

void CvGameTextMgr::setCorporationHelpCity(CvWStringBuffer &szBuffer, CorporationTypes eCorporation, CvCity *pCity, bool bCityScreen, bool bForceCorporation)
{
	if (pCity == NULL)
	{
		return;
	}

	CvCorporationInfo& kCorporation = GC.getCorporationInfo(eCorporation);

	if (bCityScreen)
	{
		szBuffer.append(CvWString::format(SETCOLR L"%s" ENDCOLR , TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), kCorporation.getDescription()));
		szBuffer.append(NEWLINE);

		if (!(GC.getGameINLINE().isCorporationFounded(eCorporation)))
		{
			if (GC.getCorporationInfo(eCorporation).getTechPrereq() != NO_TECH)
			{
				szBuffer.append(gDLL->getText("TXT_KEY_CORPORATION_FOUNDED_FIRST", GC.getTechInfo((TechTypes)(kCorporation.getTechPrereq())).getTextKeyWide()));
			}
		}
	}

	if (!bForceCorporation)
	{
		if (!(pCity->isHasCorporation(eCorporation)))
		{
			return;
		}
	}

	int iNumResources = 0;
	for (int i = 0; i < GC.getNUM_CORPORATION_PREREQ_BONUSES(); ++i)
	{
		BonusTypes eBonus = (BonusTypes)kCorporation.getPrereqBonus(i);
		if (NO_BONUS != eBonus)
		{
			iNumResources += pCity->getNumBonuses(eBonus);
		}
	}

	bool bActive = (pCity->isActiveCorporation(eCorporation) || (bForceCorporation && iNumResources > 0));

	bool bHandled = false;
	for (int i = 0; i < NUM_YIELD_TYPES; ++i)
	{
		int iYield = 0;

		if (bActive)
		{
			iYield += (kCorporation.getYieldProduced(i) * iNumResources * GC.getWorldInfo(GC.getMapINLINE().getWorldSize()).getCorporationMaintenancePercent()) / 100;
		}

		if (iYield != 0)
		{
			if (bHandled)
			{
				szBuffer.append(L", ");
			}

			CvWString szTempBuffer;
			szTempBuffer.Format(L"%s%d%c", iYield > 0 ? "+" : "", (iYield + 99) / 100, GC.getYieldInfo((YieldTypes)i).getChar());
			szBuffer.append(szTempBuffer);
			bHandled = true;
		}
	}

	bHandled = false;
	for (int i = 0; i < NUM_COMMERCE_TYPES; ++i)
	{
		int iCommerce = 0;

		if (bActive)
		{
			iCommerce += (kCorporation.getCommerceProduced(i) * iNumResources * GC.getWorldInfo(GC.getMapINLINE().getWorldSize()).getCorporationMaintenancePercent()) / 100;
		}

		if (iCommerce != 0)
		{
			if (bHandled)
			{
				szBuffer.append(L", ");
			}

			CvWString szTempBuffer;
			szTempBuffer.Format(L"%s%d%c", iCommerce > 0 ? "+" : "", (iCommerce + 99) / 100, GC.getCommerceInfo((CommerceTypes)i).getChar());
			szBuffer.append(szTempBuffer);
			bHandled = true;
		}
	}

	int iMaintenance = 0;

	if (bActive)
	{
		iMaintenance += pCity->calculateCorporationMaintenanceTimes100(eCorporation);
		iMaintenance *= 100 + pCity->getMaintenanceModifier();
		iMaintenance /= 100;
		iMaintenance = (100+GET_PLAYER(pCity->getOwnerINLINE()).getInflationRate())*iMaintenance / 100; // K-Mod
	}

	if (0 != iMaintenance)
	{
		if (bHandled)
		{
			szBuffer.append(L", ");
		}

		CvWString szTempBuffer;
		szTempBuffer.Format(L"%d%c", -iMaintenance / 100, GC.getCommerceInfo(COMMERCE_GOLD).getChar());
		szBuffer.append(szTempBuffer);
		bHandled = true;
	}

	if (bCityScreen)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_CORPORATION_BONUS_REQUIRED"));
		bool bFirst = true;
		for (int i = 0; i < GC.getNUM_CORPORATION_PREREQ_BONUSES(); ++i)
		{
			if (NO_BONUS != kCorporation.getPrereqBonus(i))
			{
				if (bFirst)
				{
					bFirst = false;
				}
				else
				{
					szBuffer.append(L", ");
				}

				szBuffer.append(CvWString::format(L"%c", GC.getBonusInfo((BonusTypes)kCorporation.getPrereqBonus(i)).getChar()));
			}
		}

		if (bActive)
		{
			if (kCorporation.getBonusProduced() != NO_BONUS)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_CORPORATION_BONUS_PRODUCED", GC.getBonusInfo((BonusTypes)kCorporation.getBonusProduced()).getChar()));
			}
		}
	}
	else
	{
		if (kCorporation.getBonusProduced() != NO_BONUS)
		{
			if (bActive)
			{
				if (bHandled)
				{
					szBuffer.append(L", ");
				}

				szBuffer.append(CvWString::format(L"%c", GC.getBonusInfo((BonusTypes)kCorporation.getBonusProduced()).getChar()));
			}
		}
	}
}

// MOD - START - Building Discontinuation
void CvGameTextMgr::buildDiscontinueString(CvWStringBuffer &szBuffer, BuildingTypes eBuilding, bool bList, bool bPlayerContext)
{
	CvWString szTempBuffer;

	if (bList)
	{
		szBuffer.append(NEWLINE);
	}
	szBuffer.append(gDLL->getText("TXT_KEY_TECH_DISCONTINUES", GC.getBuildingInfo(eBuilding).getTextKeyWide()));
}

void CvGameTextMgr::buildDiscontinueSpecialString(CvWStringBuffer &szBuffer, SpecialBuildingTypes eSpecialBuilding, bool bList, bool bPlayerContext)
{
	CvWString szTempBuffer;

	if (bList)
	{
		szBuffer.append(NEWLINE);
	}
	szBuffer.append(gDLL->getText("TXT_KEY_TECH_DISCONTINUES_NO_LINK", GC.getSpecialBuildingInfo(eSpecialBuilding).getTextKeyWide()));
}
// MOD - END - Building Discontinuation

void CvGameTextMgr::buildObsoleteString(CvWStringBuffer &szBuffer, BuildingTypes eBuilding, bool bList, bool bPlayerContext)
{
	CvWString szTempBuffer;

	if (bList)
	{
		szBuffer.append(NEWLINE);
	}
	szBuffer.append(gDLL->getText("TXT_KEY_TECH_OBSOLETES", GC.getBuildingInfo(eBuilding).getTextKeyWide()));
}

void CvGameTextMgr::buildObsoleteBonusString(CvWStringBuffer &szBuffer, BonusTypes eBonus, bool bList, bool bPlayerContext)
{
	CvWString szTempBuffer;

	if (bList)
	{
		szBuffer.append(NEWLINE);
	}
	szBuffer.append(gDLL->getText("TXT_KEY_TECH_OBSOLETES", GC.getBonusInfo(eBonus).getTextKeyWide()));
}

// MOD - START - Unit Obsolete Tech (Dom Pedro II)
void CvGameTextMgr::buildObsoleteUnitString(CvWStringBuffer &szBuffer, UnitTypes eUnit, bool bList, bool bPlayerContext)
{
    CvWString szTempBuffer;

    if (bList)
    {
		szBuffer.append(NEWLINE);
    }
    szBuffer.append(gDLL->getText("TXT_KEY_TECH_OBSOLETES", GC.getUnitInfo(eUnit).getTextKeyWide()));
}
// MOD - END - Unit Obsolete Tech (Dom Pedro II)

void CvGameTextMgr::buildObsoleteSpecialString(CvWStringBuffer &szBuffer, SpecialBuildingTypes eSpecialBuilding, bool bList, bool bPlayerContext)
{
	CvWString szTempBuffer;

	if (bList)
	{
		szBuffer.append(NEWLINE);
	}
	szBuffer.append(gDLL->getText("TXT_KEY_TECH_OBSOLETES_NO_LINK", GC.getSpecialBuildingInfo(eSpecialBuilding).getTextKeyWide()));
}

void CvGameTextMgr::buildMoveString(CvWStringBuffer &szBuffer, TechTypes eTech, bool bList, bool bPlayerContext)
{
	int iI;
	int iMoveDiff;

	for (iI = 0; iI < GC.getNumRouteInfos(); ++iI)
	{
		iMoveDiff = ((GC.getMOVE_DENOMINATOR() / std::max(1, (GC.getRouteInfo((RouteTypes) iI).getMovementCost() + ((bPlayerContext) ? GET_TEAM(GC.getGameINLINE().getActiveTeam()).getRouteChange((RouteTypes)iI) : 0)))) - (GC.getMOVE_DENOMINATOR() / std::max(1, (GC.getRouteInfo((RouteTypes) iI).getMovementCost() + ((bPlayerContext) ? GET_TEAM(GC.getGameINLINE().getActiveTeam()).getRouteChange((RouteTypes)iI) : 0) + GC.getRouteInfo((RouteTypes) iI).getTechMovementChange(eTech)))));

		if (iMoveDiff != 0)
		{
			if (bList)
			{
				szBuffer.append(NEWLINE);
			}
			szBuffer.append(gDLL->getText("TXT_KEY_UNIT_MOVEMENT", -(iMoveDiff), GC.getRouteInfo((RouteTypes) iI).getTextKeyWide()));
			bList = true;
		}
	}
}

void CvGameTextMgr::buildFreeUnitString(CvWStringBuffer &szBuffer, TechTypes eTech, bool bList, bool bPlayerContext)
{
	UnitTypes eFreeUnit = NO_UNIT;
	if (GC.getGameINLINE().getActivePlayer() != NO_PLAYER)
	{
		eFreeUnit = GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getTechFreeUnit(eTech);
	}
	else
	{
		if (GC.getTechInfo(eTech).getFirstFreeUnitClass() != NO_UNITCLASS)
		{
			eFreeUnit = (UnitTypes)GC.getUnitClassInfo((UnitClassTypes)GC.getTechInfo(eTech).getFirstFreeUnitClass()).getDefaultUnitIndex();
		}
	}

	if (eFreeUnit != NO_UNIT)
	{
		if (!bPlayerContext || (GC.getGameINLINE().countKnownTechNumTeams(eTech) == 0))
		{
			if (bList)
			{
				szBuffer.append(NEWLINE);
			}
			szBuffer.append(gDLL->getText("TXT_KEY_TECH_FIRST_RECEIVES", GC.getUnitInfo(eFreeUnit).getTextKeyWide()));
		}
	}
}

void CvGameTextMgr::buildFeatureProductionString(CvWStringBuffer &szBuffer, TechTypes eTech, bool bList, bool bPlayerContext)
{
	if (GC.getTechInfo(eTech).getFeatureProductionModifier() != 0)
	{
		if (bList)
		{
			szBuffer.append(NEWLINE);
		}
		szBuffer.append(gDLL->getText("TXT_KEY_FEATURE_PRODUCTION_MODIFIER", GC.getTechInfo(eTech).getFeatureProductionModifier()));
	}
}

void CvGameTextMgr::buildWorkerRateString(CvWStringBuffer &szBuffer, TechTypes eTech, bool bList, bool bPlayerContext)
{
	// MOD - START - Negative Worker Speed Modifiers
	int iModifier = GC.getTechInfo(eTech).getWorkerSpeedModifier();
	if (iModifier != 0)
	{
		if (bList)
		{
			szBuffer.append(NEWLINE);
		}
		szBuffer.append(gDLL->getText(iModifier >= 0 ? "TXT_KEY_WORKER_SPEED_MODIFIER_FASTER" : "TXT_KEY_WORKER_SPEED_MODIFIER_SLOWER", abs(iModifier)));
	}
	// MOD - END - Negative Worker Speed Modifiers
}

void CvGameTextMgr::buildTradeRouteString(CvWStringBuffer &szBuffer, TechTypes eTech, bool bList, bool bPlayerContext)
{
	if (GC.getTechInfo(eTech).getTradeRoutes() != 0)
	{
		if (bList)
		{
			szBuffer.append(NEWLINE);
		}
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_TRADE_ROUTES", GC.getTechInfo(eTech).getTradeRoutes()));
	}
}

// MOD - START - Congestion
void CvGameTextMgr::buildInsideCityCongestionThresholdChangeString(CvWStringBuffer& szBuffer, TechTypes eTech, bool bList, bool bPlayerContext)
{
	if (GC.getTechInfo(eTech).getInsideCityCongestionThresholdChange() != 0)
	{
		if (bList)
		{
			szBuffer.append(NEWLINE);
		}
		szBuffer.append(gDLL->getText("TXT_KEY_CONGESTION_INSIDE_CITY_THRESHOLD_CHANGE", GC.getTechInfo(eTech).getInsideCityCongestionThresholdChange()));
	}
}

void CvGameTextMgr::buildOutsideCityCongestionThresholdChangeString(CvWStringBuffer& szBuffer, TechTypes eTech, bool bList, bool bPlayerContext)
{
	if (GC.getTechInfo(eTech).getOutsideCityCongestionThresholdChange() != 0)
	{
		if (bList)
		{
			szBuffer.append(NEWLINE);
		}
		szBuffer.append(gDLL->getText("TXT_KEY_CONGESTION_OUTSIDE_CITY_THRESHOLD_CHANGE", GC.getTechInfo(eTech).getOutsideCityCongestionThresholdChange()));
	}
}
// MOD - END - Congestion

void CvGameTextMgr::buildHealthRateString(CvWStringBuffer &szBuffer, TechTypes eTech, bool bList, bool bPlayerContext)
{
	if (GC.getTechInfo(eTech).getHealth() != 0)
	{
		if (bList)
		{
			szBuffer.append(NEWLINE);
		}
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HEALTH_ALL_CITIES", abs(GC.getTechInfo(eTech).getHealth()), ((GC.getTechInfo(eTech).getHealth() > 0) ? gDLL->getSymbolID(HEALTHY_CHAR): gDLL->getSymbolID(UNHEALTHY_CHAR))));
	}
}

// MOD - START - Epidemics
void CvGameTextMgr::buildEpidemicGlobalModifierString(CvWStringBuffer &szBuffer, TechTypes eTech, bool bList, bool bPlayerContext)
{
	if (GC.getTechInfo(eTech).getGlobalEpidemicModifier() != 0)
	{
		if (bList)
		{
			szBuffer.append(NEWLINE);
		}
		
		CvWString szTempBuffer;
		szTempBuffer.Format(L"%+d", (GC.getTechInfo(eTech).getGlobalEpidemicModifier() * 100) / GC.getDefineINT("EPIDEMIC_RAND_BASE"));
		szBuffer.append(gDLL->getText("TXT_KEY_GLOBAL_EPIDEMIC_MODIFIER", szTempBuffer.GetCString()));
	}
}
// MOD - END - Epidemics

void CvGameTextMgr::buildHappinessRateString(CvWStringBuffer &szBuffer, TechTypes eTech, bool bList, bool bPlayerContext)
{
	if (GC.getTechInfo(eTech).getHappiness() != 0)
	{
		if (bList)
		{
			szBuffer.append(NEWLINE);
		}
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HAPPINESS_ALL_CITIES", abs(GC.getTechInfo(eTech).getHappiness()), ((GC.getTechInfo(eTech).getHappiness() > 0) ? gDLL->getSymbolID(HAPPY_CHAR): gDLL->getSymbolID(UNHAPPY_CHAR))));
	}
}

void CvGameTextMgr::buildFreeTechString(CvWStringBuffer &szBuffer, TechTypes eTech, bool bList, bool bPlayerContext)
{
	if (GC.getTechInfo(eTech).getFirstFreeTechs() > 0)
	{
		if (!bPlayerContext || (GC.getGameINLINE().countKnownTechNumTeams(eTech) == 0))
		{
			if (bList)
			{
				szBuffer.append(NEWLINE);
			}

			if (GC.getTechInfo(eTech).getFirstFreeTechs() == 1)
			{
				szBuffer.append(gDLL->getText("TXT_KEY_TECH_FIRST_FREE_TECH"));
			}
			else
			{
				szBuffer.append(gDLL->getText("TXT_KEY_TECH_FIRST_FREE_TECHS", GC.getTechInfo(eTech).getFirstFreeTechs()));
			}
		}
	}
}

void CvGameTextMgr::buildLOSString(CvWStringBuffer &szBuffer, TechTypes eTech, bool bList, bool bPlayerContext)
{
	if (GC.getTechInfo(eTech).isExtraWaterSeeFrom() && (!bPlayerContext || !(GET_TEAM(GC.getGameINLINE().getActiveTeam()).isExtraWaterSeeFrom())))
	{
		if (bList)
		{
			szBuffer.append(NEWLINE);
		}
		szBuffer.append(gDLL->getText("TXT_KEY_UNIT_EXTRA_SIGHT"));
	}
}

void CvGameTextMgr::buildMapCenterString(CvWStringBuffer &szBuffer, TechTypes eTech, bool bList, bool bPlayerContext)
{
	if (GC.getTechInfo(eTech).isMapCentering() && (!bPlayerContext || !(GET_TEAM(GC.getGameINLINE().getActiveTeam()).isMapCentering())))
	{
		if (bList)
		{
			szBuffer.append(NEWLINE);
		}
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_CENTERS_MAP"));
	}
}

void CvGameTextMgr::buildMapRevealString(CvWStringBuffer &szBuffer, TechTypes eTech, bool bList)
{
	if (GC.getTechInfo(eTech).isMapVisible())
	{
		if (bList)
		{
			szBuffer.append(NEWLINE);
		}
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_REVEALS_MAP"));
	}
}

void CvGameTextMgr::buildMapTradeString(CvWStringBuffer &szBuffer, TechTypes eTech, bool bList, bool bPlayerContext)
{
	if (GC.getTechInfo(eTech).isMapTrading() && (!bPlayerContext || !(GET_TEAM(GC.getGameINLINE().getActiveTeam()).isMapTrading())))
	{
		if (bList)
		{
			szBuffer.append(NEWLINE);
		}
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_ENABLES_MAP_TRADING"));
	}
}

void CvGameTextMgr::buildTechTradeString(CvWStringBuffer &szBuffer, TechTypes eTech, bool bList, bool bPlayerContext)
{
// MOD - START - Disable Tech Trading Info When Not Applicable
//	if (GC.getTechInfo(eTech).isTechTrading() && (!bPlayerContext || !(GET_TEAM(GC.getGameINLINE().getActiveTeam()).isTechTrading())))
	if (!(GC.getGameINLINE().isOption(GAMEOPTION_NO_TECH_TRADING)) && GC.getTechInfo(eTech).isTechTrading() && (!bPlayerContext || !(GET_TEAM(GC.getGameINLINE().getActiveTeam()).isTechTrading())))
// MOD - END - Disable Tech Trading Info When Not Applicable
	{
		if (bList)
		{
			szBuffer.append(NEWLINE);
		}
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_ENABLES_TECH_TRADING"));
	}
}

void CvGameTextMgr::buildGoldTradeString(CvWStringBuffer &szBuffer, TechTypes eTech, bool bList, bool bPlayerContext)
{
	if (GC.getTechInfo(eTech).isGoldTrading() && (!bPlayerContext || !(GET_TEAM(GC.getGameINLINE().getActiveTeam()).isGoldTrading())))
	{
		if (bList)
		{
			szBuffer.append(NEWLINE);
		}
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_ENABLES_GOLD_TRADING"));
	}
}

void CvGameTextMgr::buildOpenBordersString(CvWStringBuffer &szBuffer, TechTypes eTech, bool bList, bool bPlayerContext)
{
	if (GC.getTechInfo(eTech).isOpenBordersTrading() && (!bPlayerContext || !(GET_TEAM(GC.getGameINLINE().getActiveTeam()).isOpenBordersTrading())))
	{
		if (bList)
		{
			szBuffer.append(NEWLINE);
		}
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_ENABLES_OPEN_BORDERS"));
	}
}

void CvGameTextMgr::buildDefensivePactString(CvWStringBuffer &szBuffer, TechTypes eTech, bool bList, bool bPlayerContext)
{
	if (GC.getTechInfo(eTech).isDefensivePactTrading() && (!bPlayerContext || (!(GET_TEAM(GC.getGameINLINE().getActiveTeam()).isDefensivePactTrading()) && GC.getGameINLINE().isOption(GAMEOPTION_ALLIANCES))))
	{
		if (bList)
		{
			szBuffer.append(NEWLINE);
		}
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_ENABLES_DEFENSIVE_PACTS"));
	}
}

void CvGameTextMgr::buildPermanentAllianceString(CvWStringBuffer &szBuffer, TechTypes eTech, bool bList, bool bPlayerContext)
{
	if (GC.getTechInfo(eTech).isPermanentAllianceTrading() && (!bPlayerContext || (!(GET_TEAM(GC.getGameINLINE().getActiveTeam()).isPermanentAllianceTrading()) && GC.getGameINLINE().isOption(GAMEOPTION_PERMANENT_ALLIANCES))))
	{
		if (bList)
		{
			szBuffer.append(NEWLINE);
		}
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_ENABLES_PERM_ALLIANCES"));
	}
}

void CvGameTextMgr::buildVassalStateString(CvWStringBuffer &szBuffer, TechTypes eTech, bool bList, bool bPlayerContext)
{
	if (GC.getTechInfo(eTech).isVassalStateTrading() && (!bPlayerContext || (!(GET_TEAM(GC.getGameINLINE().getActiveTeam()).isVassalStateTrading()) && GC.getGameINLINE().isOption(GAMEOPTION_NO_VASSAL_STATES))))
	{
		if (bList)
		{
			szBuffer.append(NEWLINE);
		}
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_ENABLES_VASSAL_STATES"));
	}
}

void CvGameTextMgr::buildBridgeString(CvWStringBuffer &szBuffer, TechTypes eTech, bool bList, bool bPlayerContext)
{
	if (GC.getTechInfo(eTech).isBridgeBuilding() && (!bPlayerContext || !(GET_TEAM(GC.getGameINLINE().getActiveTeam()).isBridgeBuilding())))
	{
		if (bList)
		{
			szBuffer.append(NEWLINE);
		}
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_ENABLES_BRIDGE_BUILDING"));
	}
}

void CvGameTextMgr::buildIrrigationString(CvWStringBuffer &szBuffer, TechTypes eTech, bool bList, bool bPlayerContext)
{
	if (GC.getTechInfo(eTech).isIrrigation() && (!bPlayerContext || !(GET_TEAM(GC.getGameINLINE().getActiveTeam()).isIrrigation())))
	{
		if (bList)
		{
			szBuffer.append(NEWLINE);
		}
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_SPREAD_IRRIGATION"));
	}
}

void CvGameTextMgr::buildIgnoreIrrigationString(CvWStringBuffer &szBuffer, TechTypes eTech, bool bList, bool bPlayerContext)
{
	if (GC.getTechInfo(eTech).isIgnoreIrrigation() && (!bPlayerContext || !(GET_TEAM(GC.getGameINLINE().getActiveTeam()).isIgnoreIrrigation())))
	{
		if (bList)
		{
			szBuffer.append(NEWLINE);
		}
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_IRRIGATION_ANYWHERE"));
	}
}

void CvGameTextMgr::buildWaterWorkString(CvWStringBuffer &szBuffer, TechTypes eTech, bool bList, bool bPlayerContext)
{
	if (GC.getTechInfo(eTech).isWaterWork() && (!bPlayerContext || !(GET_TEAM(GC.getGameINLINE().getActiveTeam()).isWaterWork())))
	{
		if (bList)
		{
			szBuffer.append(NEWLINE);
		}
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_WATER_WORK"));
	}
}

// MOD - START - Improved Help Context
//void CvGameTextMgr::buildImprovementString(CvWStringBuffer &szBuffer, TechTypes eTech, int iBuild, bool bList, bool bPlayerContext, bool bTreeInfo)
void CvGameTextMgr::buildImprovementString(CvWStringBuffer& szBuffer, TechTypes eTech, BuildTypes eBuild, CivilizationTypes eCivilization, PlayerTypes ePlayer, bool bList, bool bTreeInfo)
{
	const CvBuildInfo& kBuild = GC.getBuildInfo(eBuild);

	bool bTechFound = false;

	// MOD - START - Unique Improvements
	if (eCivilization == NO_CIVILIZATION || GC.getCivilizationInfo(eCivilization).canBuild(eBuild))
	{
	// MOD - END - Unique Improvements
		if (kBuild.getTechPrereq() == NO_TECH)
		{
			for (int iJ = 0; iJ < GC.getNumFeatureInfos(); iJ++)
			{
				if (kBuild.getFeatureTech(iJ) == eTech)
				{
					bTechFound = true;
					break;
				}
			}
		}
		else
		{
			if (kBuild.getTechPrereq() == eTech)
			{
				bTechFound = true;
			}
		}
	}

	if (bTechFound)
	{
		if (bList)
		{
			szBuffer.append(NEWLINE);
		}

		szBuffer.append(gDLL->getText("TXT_KEY_MISC_CAN_BUILD_IMPROVEMENT", kBuild.getTextKeyWide()));
	}
}
// MOD - END - Improved Help Context

void CvGameTextMgr::buildDomainExtraMovesString(CvWStringBuffer &szBuffer, TechTypes eTech, int iDomainType, bool bList, bool bPlayerContext)
{
	if (GC.getTechInfo(eTech).getDomainExtraMoves(iDomainType) != 0)
	{
		if (bList)
		{
			szBuffer.append(NEWLINE);
		}

		szBuffer.append(gDLL->getText("TXT_KEY_MISC_EXTRA_MOVES", GC.getTechInfo(eTech).getDomainExtraMoves(iDomainType), GC.getDomainInfo((DomainTypes)iDomainType).getTextKeyWide()));
	}
}

void CvGameTextMgr::buildAdjustString(CvWStringBuffer &szBuffer, TechTypes eTech, int iCommerceType, bool bList, bool bPlayerContext)
{
	if (GC.getTechInfo(eTech).isCommerceFlexible(iCommerceType) && (!bPlayerContext || !(GET_TEAM(GC.getGameINLINE().getActiveTeam()).isCommerceFlexible((CommerceTypes)iCommerceType))))
	{
		if (bList)
		{
			szBuffer.append(NEWLINE);
		}
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_ADJUST_COMMERCE_RATE", GC.getCommerceInfo((CommerceTypes) iCommerceType).getChar()));
	}
}

void CvGameTextMgr::buildTerrainTradeString(CvWStringBuffer &szBuffer, TechTypes eTech, int iTerrainType, bool bList, bool bPlayerContext)
{
	if (GC.getTechInfo(eTech).isTerrainTrade(iTerrainType) && (!bPlayerContext || !(GET_TEAM(GC.getGameINLINE().getActiveTeam()).isTerrainTrade((TerrainTypes)iTerrainType))))
	{
		if (bList)
		{
			szBuffer.append(NEWLINE);
		}
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_ENABLES_ON_TERRAIN", gDLL->getSymbolID(TRADE_CHAR), GC.getTerrainInfo((TerrainTypes) iTerrainType).getTextKeyWide()));
	}
}

void CvGameTextMgr::buildRiverTradeString(CvWStringBuffer &szBuffer, TechTypes eTech, bool bList, bool bPlayerContext)
{
	if (GC.getTechInfo(eTech).isRiverTrade() && (!bPlayerContext || !(GET_TEAM(GC.getGameINLINE().getActiveTeam()).isRiverTrade())))
	{
		if (bList)
		{
			szBuffer.append(NEWLINE);
		}
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_ENABLES_ON_TERRAIN", gDLL->getSymbolID(TRADE_CHAR), gDLL->getText("TXT_KEY_MISC_RIVERS").GetCString()));
	}
}

void CvGameTextMgr::buildSpecialBuildingString(CvWStringBuffer &szBuffer, TechTypes eTech, int iBuildingType, bool bList, bool bPlayerContext)
{
	if (GC.getSpecialBuildingInfo((SpecialBuildingTypes)iBuildingType).getTechPrereq() == eTech)
	{
		if (bList)
		{
			szBuffer.append(NEWLINE);
		}
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_CAN_CONSTRUCT_BUILDING", GC.getSpecialBuildingInfo((SpecialBuildingTypes) iBuildingType).getTextKeyWide()));
	}

	if (GC.getSpecialBuildingInfo((SpecialBuildingTypes)iBuildingType).getTechPrereqAnyone() == eTech)
	{
		if (bList)
		{
			szBuffer.append(NEWLINE);
		}
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_CAN_CONSTRUCT_BUILDING_ANYONE", GC.getSpecialBuildingInfo((SpecialBuildingTypes) iBuildingType).getTextKeyWide()));
	}
}

void CvGameTextMgr::buildYieldChangeString(CvWStringBuffer &szBuffer, TechTypes eTech, int iImprovementType, bool bList, bool bPlayerContext)
{
	CvImprovementInfo& kImprovement = GC.getImprovementInfo((ImprovementTypes)iImprovementType);

	CvWString szTempBuffer;
	if (bList)
	{
		szTempBuffer.Format(L"<link=%s>%s</link>", CvWString(kImprovement.getType()).GetCString(), kImprovement.getDescription());
	}
	else
	{
		szTempBuffer.Format(L"%c<link=%s>%s</link>", gDLL->getSymbolID(BULLET_CHAR), CvWString(kImprovement.getType()).GetCString(), kImprovement.getDescription());
	}

	setYieldChangeHelp(szBuffer, szTempBuffer, L": ", L"", kImprovement.getTechYieldChangesArray(eTech), false, bList);
}

// MOD - START - Advanced Yield and Commerce Effects
void CvGameTextMgr::buildIrrigatedYieldChangeString( CvWStringBuffer& szBuffer, TechTypes eTech, int iImprovementType, bool bList, bool bPlayerContext )
{
	CvImprovementInfo& kImprovement = GC.getImprovementInfo((ImprovementTypes)iImprovementType);

	CvWString szTempBuffer;
	if (bList)
	{
		szTempBuffer.Format(L"<link=%s>%s</link>", CvWString(kImprovement.getType()).GetCString(), kImprovement.getDescription());
	}
	else
	{
		szTempBuffer.Format(L"%c<link=%s>%s</link>", gDLL->getSymbolID(BULLET_CHAR), CvWString(kImprovement.getType()).GetCString(), kImprovement.getDescription());
	}

	setYieldChangeHelp(szBuffer, szTempBuffer, L": ", gDLL->getText("TXT_KEY_MISC_WITH_IRRIGATION_QUALIFIER").c_str(), kImprovement.getTechIrrigatedYieldChangeArray(eTech), false, bList);
}
// MOD - END - Advanced Yield and Commerce Effects

bool CvGameTextMgr::buildBonusRevealString(CvWStringBuffer &szBuffer, TechTypes eTech, int iBonusType, bool bFirst, bool bList, bool bPlayerContext)
{
	CvWString szTempBuffer;

	if (GC.getBonusInfo((BonusTypes) iBonusType).getTechReveal() == eTech)
	{
		if (bList && bFirst)
		{
			szBuffer.append(NEWLINE);
		}
		szTempBuffer.Format( SETCOLR L"<link=literal>%s</link>" ENDCOLR , TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), GC.getBonusInfo((BonusTypes) iBonusType).getDescription());
		setListHelp(szBuffer, gDLL->getText("TXT_KEY_MISC_REVEALS").c_str(), szTempBuffer, L", ", bFirst);
		bFirst = false;
	}
	return bFirst;
}

bool CvGameTextMgr::buildCivicRevealString(CvWStringBuffer &szBuffer, TechTypes eTech, int iCivicType, bool bFirst, bool bList, bool bPlayerContext)
{
	CvWString szTempBuffer;

	if (GC.getCivicInfo((CivicTypes) iCivicType).getTechPrereq() == eTech)
	{
		if (bList && bFirst)
		{
			szBuffer.append(NEWLINE);
		}
		szTempBuffer.Format( SETCOLR L"<link=literal>%s</link>" ENDCOLR , TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), GC.getCivicInfo((CivicTypes) iCivicType).getDescription());
		setListHelp(szBuffer, gDLL->getText("TXT_KEY_MISC_ENABLES").c_str(), szTempBuffer, L", ", bFirst);
		bFirst = false;
	}
	return bFirst;
}

bool CvGameTextMgr::buildProcessInfoString(CvWStringBuffer &szBuffer, TechTypes eTech, int iProcessType, bool bFirst, bool bList, bool bPlayerContext)
{
	CvWString szTempBuffer;

	if (GC.getProcessInfo((ProcessTypes) iProcessType).getTechPrereq() == eTech)
	{
		if (bList && bFirst)
		{
			szBuffer.append(NEWLINE);
		}
		szTempBuffer.Format( SETCOLR L"<link=literal>%s</link>" ENDCOLR , TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), GC.getProcessInfo((ProcessTypes) iProcessType).getDescription());
		setListHelp(szBuffer, gDLL->getText("TXT_KEY_MISC_CAN_BUILD").c_str(), szTempBuffer, L", ", bFirst);
		bFirst = false;
	}
	return bFirst;
}

bool CvGameTextMgr::buildFoundReligionString(CvWStringBuffer &szBuffer, TechTypes eTech, int iReligionType, bool bFirst, bool bList, bool bPlayerContext)
{
	CvWString szTempBuffer;

	if (GC.getReligionInfo((ReligionTypes) iReligionType).getTechPrereq() == eTech)
	{
		if (!bPlayerContext || (GC.getGameINLINE().countKnownTechNumTeams(eTech) == 0))
		{
			if (bList && bFirst)
			{
				szBuffer.append(NEWLINE);
			}

			if (GC.getGameINLINE().isOption(GAMEOPTION_PICK_RELIGION))
			{
				szTempBuffer = gDLL->getText("TXT_KEY_RELIGION_UNKNOWN");
			}
			else
			{
				szTempBuffer.Format( SETCOLR L"<link=literal>%s</link>" ENDCOLR , TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), GC.getReligionInfo((ReligionTypes) iReligionType).getDescription());
			}
			setListHelp(szBuffer, gDLL->getText("TXT_KEY_MISC_FIRST_DISCOVER_FOUNDS").c_str(), szTempBuffer, L", ", bFirst);
			bFirst = false;
		}
	}
	return bFirst;
}

bool CvGameTextMgr::buildFoundCorporationString(CvWStringBuffer &szBuffer, TechTypes eTech, int iCorporationType, bool bFirst, bool bList, bool bPlayerContext)
{
	CvWString szTempBuffer;

	if (GC.getCorporationInfo((CorporationTypes) iCorporationType).getTechPrereq() == eTech)
	{
		if (!bPlayerContext || (GC.getGameINLINE().countKnownTechNumTeams(eTech) == 0))
		{
			if (bList && bFirst)
			{
				szBuffer.append(NEWLINE);
			}
			szTempBuffer.Format( SETCOLR L"<link=literal>%s</link>" ENDCOLR , TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), GC.getCorporationInfo((CorporationTypes) iCorporationType).getDescription());
			setListHelp(szBuffer, gDLL->getText("TXT_KEY_MISC_FIRST_DISCOVER_INCORPORATES").c_str(), szTempBuffer, L", ", bFirst);
			bFirst = false;
		}
	}
	return bFirst;
}

bool CvGameTextMgr::buildPromotionString(CvWStringBuffer &szBuffer, TechTypes eTech, int iPromotionType, bool bFirst, bool bList, bool bPlayerContext)
{
	CvWString szTempBuffer;

	if (GC.getPromotionInfo((PromotionTypes) iPromotionType).getTechPrereq() == eTech)
	{
		if (bList && bFirst)
		{
			szBuffer.append(NEWLINE);
		}
		szTempBuffer.Format( SETCOLR L"<link=literal>%s</link>" ENDCOLR , TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), GC.getPromotionInfo((PromotionTypes) iPromotionType).getDescription());
		setListHelp(szBuffer, gDLL->getText("TXT_KEY_MISC_ENABLES").c_str(), szTempBuffer, L", ", bFirst);
		bFirst = false;
	}
	return bFirst;
}

// Displays a list of derived technologies - no distinction between AND/OR prerequisites
void CvGameTextMgr::buildSingleLineTechTreeString(CvWStringBuffer &szBuffer, TechTypes eTech, bool bPlayerContext)
{
	CvWString szTempBuffer;	// Formatting

	if (NO_TECH == eTech)
	{
		// you need to specify a tech of origin for this method to do anything
		return;
	}

	bool bFirst = true;
	for (int iI = 0; iI < GC.getNumTechInfos(); ++iI)
	{
		bool bTechAlreadyAccessible = false;
		if (bPlayerContext)
		{
			bTechAlreadyAccessible = (GET_TEAM(GC.getGameINLINE().getActiveTeam()).isHasTech((TechTypes)iI) || GET_PLAYER(GC.getGameINLINE().getActivePlayer()).canResearch((TechTypes)iI));
		}
		if (!bTechAlreadyAccessible)
		{
			bool bTechFound = false;

			if (!bTechFound)
			{
				for (int iJ = 0; iJ < GC.getNUM_OR_TECH_PREREQS(); iJ++)
				{
					if (GC.getTechInfo((TechTypes) iI).getPrereqOrTechs(iJ) == eTech)
					{
						bTechFound = true;
						break;
					}
				}
			}

			if (!bTechFound)
			{
				for (int iJ = 0; iJ < GC.getNUM_AND_TECH_PREREQS(); iJ++)
				{
					if (GC.getTechInfo((TechTypes) iI).getPrereqAndTechs(iJ) == eTech)
					{
						bTechFound = true;
						break;
					}
				}
			}

			if (bTechFound)
			{
				szTempBuffer.Format( SETCOLR L"<link=literal>%s</link>" ENDCOLR , TEXT_COLOR("COLOR_TECH_TEXT"), GC.getTechInfo((TechTypes) iI).getDescription());
				setListHelp(szBuffer, gDLL->getText("TXT_KEY_MISC_LEADS_TO").c_str(), szTempBuffer, L", ", bFirst);
				bFirst = false;
			}
		}
	}
}

// Information about other prerequisite technologies to eTech besides eFromTech
// MOD - START - Improved Help Context
//void CvGameTextMgr::buildTechTreeString(CvWStringBuffer &szBuffer, TechTypes eTech, bool bPlayerContext, TechTypes eFromTech)
void CvGameTextMgr::buildTechTreeString(CvWStringBuffer &szBuffer, TechTypes eTech, CivilizationTypes eCivilization, PlayerTypes ePlayer, TechTypes eFromTech)
// MOD - END - Improved Help Context
{
	CvWString szTempBuffer;	// Formatting

	if (NO_TECH == eTech || NO_TECH == eFromTech)
	{
		return;
	}

	szTempBuffer.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR("COLOR_TECH_TEXT"), GC.getTechInfo(eTech).getDescription());
	szBuffer.append(szTempBuffer);

	// Loop through OR prerequisites to make list
	CvWString szOtherOrTechs;
	int nOtherOrTechs = 0;
	bool bOrTechFound = false;
	for (int iJ = 0; iJ < GC.getNUM_OR_TECH_PREREQS(); iJ++)
	{
		TechTypes eTestTech = (TechTypes)GC.getTechInfo(eTech).getPrereqOrTechs(iJ);
		if (eTestTech >= 0)
		{
			bool bTechAlreadyResearched = false;
			// MOD - START - Improved Help Context
			/*
			if (bPlayerContext)
			{
				bTechAlreadyResearched = GET_TEAM(GC.getGameINLINE().getActiveTeam()).isHasTech(eTestTech);
			}
			*/
			if (ePlayer != NO_PLAYER)
			{
				bTechAlreadyResearched = GET_TEAM(GET_PLAYER(ePlayer).getTeam()).isHasTech(eTestTech);
			}
			// MOD - END - Improved Help Context
			if (!bTechAlreadyResearched)
			{
				if (eTestTech == eFromTech)
				{
					bOrTechFound = true;
				}
				else
				{
					szTempBuffer.Format( SETCOLR L"%s" ENDCOLR , TEXT_COLOR("COLOR_TECH_TEXT"), GC.getTechInfo(eTestTech).getDescription());
					setListHelp(szOtherOrTechs, L"", szTempBuffer, gDLL->getText("TXT_KEY_OR").c_str(), 0 == nOtherOrTechs);
					nOtherOrTechs++;
				}
			}
		}
	}

	// Loop through AND prerequisites to make list
	CvWString szOtherAndTechs;
	int nOtherAndTechs = 0;
	bool bAndTechFound = false;
	for (int iJ = 0; iJ < GC.getNUM_AND_TECH_PREREQS(); iJ++)
	{
		TechTypes eTestTech = (TechTypes)GC.getTechInfo(eTech).getPrereqAndTechs(iJ);
		if (eTestTech >= 0)
		{
			bool bTechAlreadyResearched = false;
			// MOD - START - Improved Help Context
			/*
			if (bPlayerContext)
			{
				bTechAlreadyResearched = GET_TEAM(GC.getGameINLINE().getActiveTeam()).isHasTech(eTestTech);
			}
			*/
			if (ePlayer != NO_PLAYER)
			{
				bTechAlreadyResearched = GET_TEAM(GET_PLAYER(ePlayer).getTeam()).isHasTech(eTestTech);
			}
			// MOD - END - Improved Help Context
			if (!bTechAlreadyResearched)
			{
				if (eTestTech == eFromTech)
				{
					bAndTechFound = true;
				}
				else
				{
					szTempBuffer.Format( SETCOLR L"%s" ENDCOLR , TEXT_COLOR("COLOR_TECH_TEXT"), GC.getTechInfo(eTestTech).getDescription());
					setListHelp(szOtherAndTechs, L"", szTempBuffer, L", ", 0 == nOtherAndTechs);
					nOtherAndTechs++;
				}
			}
		}
	}

	if (bOrTechFound || bAndTechFound)
	{
		if (nOtherAndTechs > 0 || nOtherOrTechs > 0)
		{
			szBuffer.append(L' ');

			if (nOtherAndTechs > 0)
			{
				szBuffer.append(gDLL->getText("TXT_KEY_WITH_SPACE"));
				szBuffer.append(szOtherAndTechs);
			}

			if (nOtherOrTechs > 0)
			{
				if (bAndTechFound)
				{
					if (nOtherAndTechs > 0)
					{
						szBuffer.append(gDLL->getText("TXT_KEY_AND_SPACE"));
					}
					else
					{
						szBuffer.append(gDLL->getText("TXT_KEY_WITH_SPACE"));
					}
					szBuffer.append(szOtherOrTechs);
				}
				else if (bOrTechFound)
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_MISC_ALTERNATIVELY_DERIVED", GC.getTechInfo(eTech).getTextKeyWide(), szOtherOrTechs.GetCString()));
				}
			}
		}
	}
}

void CvGameTextMgr::setPromotionHelp(CvWStringBuffer &szBuffer, PromotionTypes ePromotion, bool bCivilopediaText)
{
	if (!bCivilopediaText)
	{
		CvWString szTempBuffer;

		if (NO_PROMOTION == ePromotion)
		{
			return;
		}
		CvPromotionInfo& promo = GC.getPromotionInfo(ePromotion);

		szTempBuffer.Format( SETCOLR L"%s" ENDCOLR , TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), promo.getDescription());
		szBuffer.append(szTempBuffer);
	}

	parsePromotionHelp(szBuffer, ePromotion);
}

// MOD - START - Aid
void CvGameTextMgr::setAidHelp(CvWStringBuffer &szBuffer, AidTypes eAid, bool bCivilopediaText)
{
	if (!bCivilopediaText)
	{
		CvWString szTempBuffer;	// Formatting

		if (NO_AID == eAid)
		{
			return;
		}
		CvAidInfo& aid = GC.getAidInfo(eAid);

		szTempBuffer.Format( SETCOLR L"%s" ENDCOLR , TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), aid.getDescription());
		szBuffer.append(szTempBuffer);
	}

	parseAidHelp(szBuffer, eAid);
}
// MOD - END - Aid

// MOD - START - Detriments
void CvGameTextMgr::setDetrimentHelp(CvWStringBuffer &szBuffer, DetrimentTypes eDetriment, bool bCivilopediaText)
{
	if (!bCivilopediaText)
	{
		CvWString szTempBuffer;	// Formatting

		if (NO_DETRIMENT == eDetriment)
		{
			return;
		}
		CvDetrimentInfo& detriment = GC.getDetrimentInfo(eDetriment);

		szTempBuffer.Format( SETCOLR L"%s" ENDCOLR , TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), detriment.getDescription());
		szBuffer.append(szTempBuffer);
	}

	parseDetrimentHelp(szBuffer, eDetriment);
}
// MOD - END - Detriments

void CvGameTextMgr::setUnitCombatHelp(CvWStringBuffer &szBuffer, UnitCombatTypes eUnitCombat)
{
	szBuffer.append(GC.getUnitCombatInfo(eUnitCombat).getDescription());
}

// MOD - START - Improved Civilopedia
// NOTE: Changes made throughout
void CvGameTextMgr::setImprovementHelp(CvWStringBuffer &szBuffer, ImprovementTypes eImprovement, bool bCivilopediaEffects, bool bCivilopediaImprovements)
{
	CvWString szTempBuffer;
	CvWString szFirstBuffer;
	int iTurns;

	if (NO_IMPROVEMENT == eImprovement)
	{
		return;
	}

	bool bCivilopediaText = bCivilopediaEffects || bCivilopediaImprovements;

	CvImprovementInfo& kImprovement = GC.getImprovementInfo(eImprovement);
	if (!bCivilopediaText)
	{
		szTempBuffer.Format( SETCOLR L"%s" ENDCOLR, TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), kImprovement.getDescription());
		szBuffer.append(szTempBuffer);

		setYieldChangeHelp(szBuffer, L", ", L"", L"", kImprovement.getYieldChangeArray(), false, false);
	}

	// MOD - START - Unique Improvements
	if (!bCivilopediaImprovements)
	{
		if (kImprovement.isUniqueImprovement())
		{
			for (int iI  = 0; iI < GC.getNumCivilizationInfos(); iI++)
			{
				const CvCivilizationInfo& kCivilization = GC.getCivilizationInfo((CivilizationTypes)iI);
				if (kCivilization.canProduceImprovement(eImprovement))
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_UNIQUE_IMPROVEMENT_FOR", kCivilization.getTextKeyWide()));
				}
			}
		}
	}
	// MOD - END - Unique Improvements

	if (!bCivilopediaEffects)
	{
		setYieldChangeHelp(szBuffer, L"", L"", gDLL->getText("TXT_KEY_MISC_WITH_IRRIGATION").c_str(), kImprovement.getIrrigatedYieldChangeArray());
		// MOD - START - Advanced Yield and Commerce Effects
		// MOD - START - Relation Caching
		for (int iRel = 0; iRel < kImprovement.getNumRelatedTechs(); iRel++)
		{
			TechTypes eLoopTech = (TechTypes)kImprovement.getRelatedTechs(iRel);
			const CvTechInfo& kLoopTech = GC.getTechInfo(eLoopTech);
			setYieldChangeHelp(szBuffer, L"", L"", gDLL->getText("TXT_KEY_MISC_WITH_IRRIGATION_WITH_TECH", CvWString(kLoopTech.getType()).GetCString(), GC.getTechInfo(eLoopTech).getTextKeyWide()).c_str(), kImprovement.getTechIrrigatedYieldChangeArray(eLoopTech));
		}
		// MOD - END - Relation Caching
		// MOD - END - Advanced Yield and Commerce Effects
		setYieldChangeHelp(szBuffer, L"", L"", gDLL->getText("TXT_KEY_MISC_ON_HILLS").c_str(), kImprovement.getHillsYieldChangeArray());
		setYieldChangeHelp(szBuffer, L"", L"", gDLL->getText("TXT_KEY_MISC_ALONG_RIVER").c_str(), kImprovement.getRiverSideYieldChangeArray());
		
		// MOD - START - Advanced Yield and Commerce Effects
		// MOD - START - Relation Caching
		CvWString szFirstBuffer;
		for (YieldTypes eLoopYield = (YieldTypes)0; eLoopYield < NUM_YIELD_TYPES; eLoopYield = (YieldTypes)(eLoopYield+1))
		{
			int iLast = 0;

			// Tech Yield Changes
			for (int iRel = 0; iRel < kImprovement.getNumRelatedTechs(); iRel++)
			{
				TechTypes eLoopTech = (TechTypes)kImprovement.getRelatedTechs(iRel);
				int iChange = kImprovement.getTechYieldChanges(eLoopTech, eLoopYield);
				if (iChange != 0)
				{
					const CvTechInfo& kLoopTech = GC.getTechInfo(eLoopTech);
					szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_YIELD_CHANGE_FROM", iChange, GC.getYieldInfo(eLoopYield).getChar()).c_str());
					CvWString szTech = gDLL->getText("TXT_KEY_MISC_TECH_EFFECT", kLoopTech.getTextKeyWide());
					setListHelp(szBuffer, szFirstBuffer, szTech, L", ", (iChange != iLast));
					iLast = iChange;
				}
			}

			// Trait Yield Changes
			for (int iRel = 0; iRel < kImprovement.getNumRelatedTraits(); iRel++)
			{
				TraitTypes eLoopTrait = (TraitTypes)kImprovement.getRelatedTraits(iRel);
				const CvTraitInfo& kLoopTrait = GC.getTraitInfo(eLoopTrait);
				int iChange = kLoopTrait.getImprovementYieldChanges(eImprovement, eLoopYield);
				if (iChange != 0)
				{
					szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_YIELD_CHANGE_FROM", iChange, GC.getYieldInfo(eLoopYield).getChar()).c_str());
					CvWString szTrait = gDLL->getText("TXT_KEY_MISC_TRAIT_EFFECT", kLoopTrait.getTextKeyWide());
					setListHelp(szBuffer, szFirstBuffer, szTrait, L", ", (iChange != iLast));
					iLast = iChange;
				}
			}

			// Civic Yield Changes
			for (int iRel = 0; iRel < kImprovement.getNumRelatedCivics(); iRel++)
			{
				CivicTypes eLoopCivic = (CivicTypes)kImprovement.getRelatedCivics(iRel);
				const CvCivicInfo& kLoopCivic = GC.getCivicInfo(eLoopCivic);
				int iChange = kLoopCivic.getImprovementYieldChanges(eImprovement, eLoopYield);
				if (iChange != 0)
				{
					szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_YIELD_CHANGE_FROM", iChange, GC.getYieldInfo(eLoopYield).getChar()).c_str());
					CvWString szCivic = gDLL->getText("TXT_KEY_MISC_CIVIC_EFFECT", kLoopCivic.getTextKeyWide());
					setListHelp(szBuffer, szFirstBuffer, szCivic, L", ", (iChange != iLast));
					iLast = iChange;
				}
			}

			// Building Yield Changes
			if (GC.getGameINLINE().getActivePlayer() != NO_PLAYER)
			{
				const CvCivilizationInfo& kCivilization = GC.getCivilizationInfo(GC.getGameINLINE().getActiveCivilizationType());

				for (int iRel = 0; iRel < kImprovement.getNumRelatedBuildingClasses(); iRel++)
				{
					BuildingClassTypes eLoopBuildingClass = (BuildingClassTypes)kImprovement.getRelatedBuildingClasses(iRel);
					BuildingTypes eLoopBuilding = (BuildingTypes)kCivilization.getCivilizationBuildings(eLoopBuildingClass);

					if (eLoopBuilding != NO_BUILDING)
					{
						const CvBuildingInfo& kLoopBuilding = GC.getBuildingInfo(eLoopBuilding);

						int aiYieldChanges[NUM_YIELD_TYPES] = {};
						kLoopBuilding.getImprovementYieldChangeArray(eImprovement, aiYieldChanges);
						int iChange = aiYieldChanges[eLoopYield];
						if (iChange != 0)
						{
							szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_YIELD_CHANGE_FROM", iChange, GC.getYieldInfo(eLoopYield).getChar()).c_str());
							CvWString szBuilding = gDLL->getText("TXT_KEY_MISC_BUILDING_EFFECT", kLoopBuilding.getTextKeyWide());
							setListHelp(szBuffer, szFirstBuffer, szBuilding, L", ", (iChange != iLast));
							iLast = iChange;
						}

						int aiGlobalYieldChanges[NUM_YIELD_TYPES] = {};
						kLoopBuilding.getGlobalImprovementYieldChangeArray(eImprovement, aiGlobalYieldChanges);
						iChange = aiGlobalYieldChanges[eLoopYield];
						if (iChange != 0)
						{
							szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_YIELD_CHANGE_FROM", iChange, GC.getYieldInfo(eLoopYield).getChar()).c_str());
							CvWString szBuilding = gDLL->getText("TXT_KEY_MISC_BUILDING_EFFECT_ALL_CITIES", kLoopBuilding.getTextKeyWide());
							setListHelp(szBuffer, szFirstBuffer, szBuilding, L", ", (iChange != iLast));
							iLast = iChange;
						}
					}
				}
			}
			else
			{
				for (int iRel = 0; iRel < kImprovement.getNumRelatedBuildingClasses(); iRel++)
				{
					BuildingClassTypes eLoopBuildingClass = (BuildingClassTypes)kImprovement.getRelatedBuildingClasses(iRel);
					const CvBuildingClassInfo& kLoopBuildingClass = GC.getBuildingClassInfo(eLoopBuildingClass);

					for (int iMem = 0; iMem < kLoopBuildingClass.getNumBuildingTypes(); iMem++)
					{
						BuildingTypes eLoopBuilding = (BuildingTypes)kLoopBuildingClass.getBuildingTypes(iMem);
						const CvBuildingInfo& kLoopBuilding = GC.getBuildingInfo(eLoopBuilding);

						int aiYieldChanges[NUM_YIELD_TYPES] = {};
						kLoopBuilding.getImprovementYieldChangeArray(eImprovement, aiYieldChanges);
						int iChange = aiYieldChanges[eLoopYield];
						if (iChange != 0)
						{
							szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_YIELD_CHANGE_FROM", iChange, GC.getYieldInfo(eLoopYield).getChar()).c_str());
							CvWString szBuilding = gDLL->getText("TXT_KEY_MISC_BUILDING_EFFECT", kLoopBuilding.getTextKeyWide());
							setListHelp(szBuffer, szFirstBuffer, szBuilding, L", ", (iChange != iLast));
							iLast = iChange;
						}

						int aiGlobalYieldChanges[NUM_YIELD_TYPES] = {};
						kLoopBuilding.getGlobalImprovementYieldChangeArray(eImprovement, aiGlobalYieldChanges);
						iChange = aiGlobalYieldChanges[eLoopYield];
						if (iChange != 0)
						{
							szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_YIELD_CHANGE_FROM", iChange, GC.getYieldInfo(eLoopYield).getChar()).c_str());
							CvWString szBuilding = gDLL->getText("TXT_KEY_MISC_BUILDING_EFFECT_ALL_CITIES", kLoopBuilding.getTextKeyWide());
							setListHelp(szBuffer, szFirstBuffer, szBuilding, L", ", (iChange != iLast));
							iLast = iChange;
						}
					}
				}
			}

			// Routes
			for (RouteTypes eLoopRoute = (RouteTypes)0; eLoopRoute < GC.getNumRouteInfos(); eLoopRoute = (RouteTypes)(eLoopRoute+1))
			{
				int iChange = kImprovement.getRouteYieldChanges(eLoopRoute, eLoopYield);
				if (iChange != 0)
				{
					const CvRouteInfo& kLoopRoute = GC.getRouteInfo(eLoopRoute);
					szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_YIELD_CHANGE_FROM", iChange, GC.getYieldInfo(eLoopYield).getChar()).c_str());
					CvWString szRoute = gDLL->getText("TXT_KEY_MISC_ROUTE_EFFECT", kLoopRoute.getTextKeyWide());
					setListHelp(szBuffer, szFirstBuffer, szRoute, L", ", (iChange != iLast));
					iLast = iChange;
				}
			}
		}
		// MOD - END - Relation Caching
		// MOD - END - Advanced Yield and Commerce Effects
	}

	if (!bCivilopediaImprovements)
	{
		if (kImprovement.isRequiresRiverSide())
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_IMPROVEMENT_REQUIRES_RIVER"));
		}
		if (kImprovement.isCarriesIrrigation())
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_IMPROVEMENT_CARRIES_IRRIGATION"));
		}
		if (bCivilopediaText)
		{
			if (kImprovement.isNoFreshWater())
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_IMPROVEMENT_NO_BUILD_FRESH_WATER"));
			}
			if (kImprovement.isWater())
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_IMPROVEMENT_BUILD_ONLY_WATER"));
			}
			if (kImprovement.isRequiresFlatlands())
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_IMPROVEMENT_ONLY_BUILD_FLATLANDS"));
			}
		}

		if (kImprovement.getImprovementUpgrade() != NO_IMPROVEMENT)
		{
			// MOD - START - Improved Civilopedia
			const CvImprovementInfo& kImprovementUpgrade = GC.getImprovementInfo((ImprovementTypes) kImprovement.getImprovementUpgrade());

			iTurns = GC.getGameINLINE().getImprovementUpgradeTime(eImprovement);

			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_IMPROVEMENT_EVOLVES", CvWString(kImprovementUpgrade.getType()).GetCString(), kImprovementUpgrade.getTextKeyWide(), iTurns));
			// MOD - END - Improved Civilopedia
		}

		int iLast = -1;
		for (int iBonus = 0; iBonus < GC.getNumBonusInfos(); iBonus++)
		{
			int iRand = kImprovement.getImprovementBonusDiscoverRand(iBonus);
			if (iRand > 0)
			{
				szFirstBuffer.Format(L"%s%s", NEWLINE, gDLL->getText("TXT_KEY_IMPROVEMENT_CHANCE_DISCOVER").c_str());
				szTempBuffer.Format(L"%c", GC.getBonusInfo((BonusTypes) iBonus).getChar());
				setListHelp(szBuffer, szFirstBuffer, szTempBuffer, L", ", iRand != iLast);
				iLast = iRand;
			}
		}

		if (0 != kImprovement.getDefenseModifier())
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_IMPROVEMENT_DEFENSE_MODIFIER", kImprovement.getDefenseModifier()));
		}

		if (0 != kImprovement.getHappiness())
		{
			szBuffer.append(NEWLINE);
	/************************************************************************************************/
	/* UNOFFICIAL_PATCH                       08/28/09                             jdog5000         */
	/*                                                                                              */
	/* Bugfix                                                                                       */
	/************************************************************************************************/
	/* original bts code
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_ICON_CHANGE_NEARBY_CITIES", info.getHappiness(), (info.getHappiness() > 0 ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR))));
	*/
			// Use absolute value with unhappy face
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_ICON_CHANGE_NEARBY_CITIES", abs(kImprovement.getHappiness()), (kImprovement.getHappiness() > 0 ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR))));
	/************************************************************************************************/
	/* UNOFFICIAL_PATCH                        END                                                  */
	/************************************************************************************************/
		}

		if (kImprovement.isActsAsCity())
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_IMPROVEMENT_DEFENSE_MODIFIER_EXTRA"));
		}

		if (kImprovement.getFeatureGrowthProbability() > 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_IMPROVEMENT_MORE_GROWTH"));
		}
		else if (kImprovement.getFeatureGrowthProbability() < 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_IMPROVEMENT_LESS_GROWTH"));
		}

		// MOD - START - Aid
		szTempBuffer.clear();
		bool bFirst = true;
		for (int iI = 0; iI < kImprovement.getNumAidTypes(); iI++)
		{
			AidTypes eAidType = (AidTypes)kImprovement.getAidTypes(iI);

			if (bFirst)
			{
				bFirst = false;
			}
			else
			{
				szTempBuffer += L", ";
			}

			szTempBuffer += CvWString::format(L"<link=literal>%s</link>", GC.getAidInfo(eAidType).getDescription());
		}

		if (!bFirst)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_PROVIDES_AID", szTempBuffer.GetCString()));
		}
		// MOD - END - Aid

		// MOD - START - Epidemics
		// Epidemic system by Mexico
		if (kImprovement.getEpidemicModifier() != 0)
		{
			szTempBuffer.Format(L"%+d", (kImprovement.getEpidemicModifier() * 100) / GC.getDefineINT("EPIDEMIC_RAND_BASE"));
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_IMPROVEMENT_EPIDEMIC_MODIFIER", szTempBuffer.GetCString()));
		}
		// MOD - END - Epidemics

		if (bCivilopediaText)
		{
			if (kImprovement.getPillageGold() > 0)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_IMPROVEMENT_PILLAGE_YIELDS", kImprovement.getPillageGold()));
			}
		}
		
		if (!CvWString(kImprovement.getHelp()).empty())
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(kImprovement.getHelp());
			}
	}
}
// MOD - END - Improved Civilopedia


void CvGameTextMgr::getDealString(CvWStringBuffer& szBuffer, CvDeal& deal, PlayerTypes ePlayerPerspective)
{
	PlayerTypes ePlayer1 = deal.getFirstPlayer();
	PlayerTypes ePlayer2 = deal.getSecondPlayer();

	const CLinkList<TradeData>* pListPlayer1 = deal.getFirstTrades();
	const CLinkList<TradeData>* pListPlayer2 = deal.getSecondTrades();

	getDealString(szBuffer, ePlayer1, ePlayer2, pListPlayer1,  pListPlayer2, ePlayerPerspective);
}

void CvGameTextMgr::getDealString(CvWStringBuffer& szBuffer, PlayerTypes ePlayer1, PlayerTypes ePlayer2, const CLinkList<TradeData>* pListPlayer1, const CLinkList<TradeData>* pListPlayer2, PlayerTypes ePlayerPerspective)
{
	if (NO_PLAYER == ePlayer1 || NO_PLAYER == ePlayer2)
	{
		FAssertMsg(false, "Deal needs two parties");
		return;
	}

	CvWStringBuffer szDealOne;
	if (NULL != pListPlayer1 && pListPlayer1->getLength() > 0)
	{
		CLLNode<TradeData>* pTradeNode;
		bool bFirst = true;
		for (pTradeNode = pListPlayer1->head(); pTradeNode; pTradeNode = pListPlayer1->next(pTradeNode))
		{
			CvWStringBuffer szTrade;
			getTradeString(szTrade, pTradeNode->m_data, ePlayer1, ePlayer2);
			setListHelp(szDealOne, L"", szTrade.getCString(), L", ", bFirst);
			bFirst = false;
		}
	}

	CvWStringBuffer szDealTwo;
	if (NULL != pListPlayer2 && pListPlayer2->getLength() > 0)
	{
		CLLNode<TradeData>* pTradeNode;
		bool bFirst = true;
		for (pTradeNode = pListPlayer2->head(); pTradeNode; pTradeNode = pListPlayer2->next(pTradeNode))
		{
			CvWStringBuffer szTrade;
			getTradeString(szTrade, pTradeNode->m_data, ePlayer2, ePlayer1);
			setListHelp(szDealTwo, L"", szTrade.getCString(), L", ", bFirst);
			bFirst = false;
		}
	}

	if (!szDealOne.isEmpty())
	{
		if (!szDealTwo.isEmpty())
		{
			if (ePlayerPerspective == ePlayer1)
			{
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_OUR_DEAL", szDealOne.getCString(), GET_PLAYER(ePlayer2).getNameKey(), szDealTwo.getCString()));
			}
			else if (ePlayerPerspective == ePlayer2)
			{
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_OUR_DEAL", szDealTwo.getCString(), GET_PLAYER(ePlayer1).getNameKey(), szDealOne.getCString()));
			}
			else
			{
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_DEAL", GET_PLAYER(ePlayer1).getNameKey(), szDealOne.getCString(), GET_PLAYER(ePlayer2).getNameKey(), szDealTwo.getCString()));
			}
		}
		else
		{
			if (ePlayerPerspective == ePlayer1)
			{
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_DEAL_ONESIDED_OURS", szDealOne.getCString(), GET_PLAYER(ePlayer2).getNameKey()));
			}
			else if (ePlayerPerspective == ePlayer2)
			{
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_DEAL_ONESIDED_THEIRS", szDealOne.getCString(), GET_PLAYER(ePlayer1).getNameKey()));
			}
			else
			{
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_DEAL_ONESIDED", GET_PLAYER(ePlayer1).getNameKey(), szDealOne.getCString(), GET_PLAYER(ePlayer2).getNameKey()));
			}
		}
	}
	else if (!szDealTwo.isEmpty())
	{
		if (ePlayerPerspective == ePlayer1)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_DEAL_ONESIDED_THEIRS", szDealTwo.getCString(), GET_PLAYER(ePlayer2).getNameKey()));
		}
		else if (ePlayerPerspective == ePlayer2)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_DEAL_ONESIDED_OURS", szDealTwo.getCString(), GET_PLAYER(ePlayer1).getNameKey()));
		}
		else
		{
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_DEAL_ONESIDED", GET_PLAYER(ePlayer2).getNameKey(), szDealTwo.getCString(), GET_PLAYER(ePlayer1).getNameKey()));
		}
	}
}

void CvGameTextMgr::getWarplanString(CvWStringBuffer& szString, WarPlanTypes eWarPlan)
{
	switch (eWarPlan)
	{
		case WARPLAN_ATTACKED_RECENT: szString.assign(L"new defensive war"); break;
		case WARPLAN_ATTACKED: szString.assign(L"defensive war"); break;
		case WARPLAN_PREPARING_LIMITED: szString.assign(L"preparing limited war"); break;
		case WARPLAN_PREPARING_TOTAL: szString.assign(L"preparing total war"); break;
		case WARPLAN_LIMITED: szString.assign(L"limited war"); break;
		case WARPLAN_TOTAL: szString.assign(L"total war"); break;
		case WARPLAN_DOGPILE: szString.assign(L"dogpile war"); break;
		case NO_WARPLAN: szString.assign(L"no warplan"); break;
		default:  szString.assign(L"unknown warplan"); break;
	}
}

void CvGameTextMgr::getAttitudeString(CvWStringBuffer& szBuffer, PlayerTypes ePlayer,
	PlayerTypes eTargetPlayer, /* advc.sha: */ bool bConstCache)
{
	CvGame const& g = GC.getGame();
	CvWString szTempBuffer;
	CvWStringBuffer szBreakdown; // advc.sha

	CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);
	TeamTypes eTeam = (TeamTypes) kPlayer.getTeam();
	CvTeamAI& kTeam = GET_TEAM(eTeam);

	// K-Mod
	// TODO: Consider re-instating human-to-human attitude display because its used to compute open borders commerce
	if (kPlayer.isHuman())
		return;
	
	// advc.sha:
	bool bSHowHiddenAttitude = (GC.getDefineINT("SHOW_HIDDEN_ATTITUDE") == 1 || g.isDebugMode());
	bool bObscurePersonality = (g.isOption(GAMEOPTION_RANDOM_PERSONALITIES) && !g.isDebugMode());
	
	szBuffer.append(NEWLINE);
	// K-Mod end

	// (K-Mod note: vassal information has been moved from here to a new function)

	// Attitude breakdown
	int iTotal = 0; // advc.sha
	for (int iPass = 0; iPass < 2; iPass++)
	{
		int iAttitudeChange = kPlayer.AI_getCloseBordersAttitude(eTargetPlayer);
		if ((iPass == 0) ? (iAttitudeChange > 0) : (iAttitudeChange < 0))
		{
			szTempBuffer.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR((iAttitudeChange > 0) ? "COLOR_POSITIVE_TEXT" : "COLOR_NEGATIVE_TEXT"), gDLL->getText("TXT_KEY_MISC_ATTITUDE_LAND_TARGET", iAttitudeChange).GetCString());
			szBreakdown.append(NEWLINE);
			szBreakdown.append(szTempBuffer);
			iTotal += iAttitudeChange;
		}
		// <advc.sha>
		iAttitudeChange = kPlayer.AI_getWarAttitude(eTargetPlayer,
				kPlayer.AI_getAttitudeVal(eTargetPlayer) - iAttitudeChange);
		// </advc.sha>
		if ((iPass == 0) ? (iAttitudeChange > 0) : (iAttitudeChange < 0))
		{
			szTempBuffer.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR((iAttitudeChange > 0) ? "COLOR_POSITIVE_TEXT" : "COLOR_NEGATIVE_TEXT"), gDLL->getText("TXT_KEY_MISC_ATTITUDE_WAR", iAttitudeChange).GetCString());
			szBreakdown.append(NEWLINE);
			szBreakdown.append(szTempBuffer);
			iTotal += iAttitudeChange;
		}

		iAttitudeChange = kPlayer.AI_getPeaceAttitude(eTargetPlayer);
		if ((iPass == 0) ? (iAttitudeChange > 0) : (iAttitudeChange < 0))
		{
			szTempBuffer.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR((iAttitudeChange > 0) ? "COLOR_POSITIVE_TEXT" : "COLOR_NEGATIVE_TEXT"), gDLL->getText("TXT_KEY_MISC_ATTITUDE_PEACE", iAttitudeChange).GetCString());
			szBreakdown.append(NEWLINE);
			szBreakdown.append(szTempBuffer);
			iTotal += iAttitudeChange;
		}

		iAttitudeChange = kPlayer.AI_getSameReligionAttitude(eTargetPlayer);
		if ((iPass == 0) ? (iAttitudeChange > 0) : (iAttitudeChange < 0))
		{
			szTempBuffer.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR((iAttitudeChange > 0) ? "COLOR_POSITIVE_TEXT" : "COLOR_NEGATIVE_TEXT"), gDLL->getText("TXT_KEY_MISC_ATTITUDE_SAME_RELIGION", iAttitudeChange).GetCString());
			szBreakdown.append(NEWLINE);
			szBreakdown.append(szTempBuffer);
			iTotal += iAttitudeChange;
		}

		iAttitudeChange = kPlayer.AI_getDifferentReligionAttitude(eTargetPlayer);
		if ((iPass == 0) ? (iAttitudeChange > 0) : (iAttitudeChange < 0))
		{
			szTempBuffer.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR((iAttitudeChange > 0) ? "COLOR_POSITIVE_TEXT" : "COLOR_NEGATIVE_TEXT"), gDLL->getText("TXT_KEY_MISC_ATTITUDE_DIFFERENT_RELIGION", iAttitudeChange).GetCString());
			szBreakdown.append(NEWLINE);
			szBreakdown.append(szTempBuffer);
			iTotal += iAttitudeChange;
		}
		
		iAttitudeChange = kPlayer.AI_getCommunistAttitude(eTargetPlayer);
		if ((iPass == 0) ? (iAttitudeChange > 0) : (iAttitudeChange < 0))
		{
			if (kPlayer.isCommunist())
			{
				szTempBuffer.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR((iAttitudeChange > 0) ? "COLOR_POSITIVE_TEXT" : "COLOR_NEGATIVE_TEXT"), gDLL->getText("TXT_KEY_MISC_ATTITUDE_DIFFERENT_IDEOLOGY_USCOM", iAttitudeChange).GetCString());
				szBreakdown.append(NEWLINE);
				szBreakdown.append(szTempBuffer);
				iTotal += iAttitudeChange;
			}
			else
			{
				szTempBuffer.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR((iAttitudeChange > 0) ? "COLOR_POSITIVE_TEXT" : "COLOR_NEGATIVE_TEXT"), gDLL->getText("TXT_KEY_MISC_ATTITUDE_DIFFERENT_IDEOLOGY_THEMCOM", iAttitudeChange).GetCString());
				szBreakdown.append(NEWLINE);
				szBreakdown.append(szTempBuffer);
				iTotal += iAttitudeChange;
			}				
		}		

		iAttitudeChange = kPlayer.AI_getBonusTradeAttitude(eTargetPlayer);
		if ((iPass == 0) ? (iAttitudeChange > 0) : (iAttitudeChange < 0))
		{
			szTempBuffer.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR((iAttitudeChange > 0) ? "COLOR_POSITIVE_TEXT" : "COLOR_NEGATIVE_TEXT"), gDLL->getText("TXT_KEY_MISC_ATTITUDE_BONUS_TRADE", iAttitudeChange).GetCString());
			szBreakdown.append(NEWLINE);
			szBreakdown.append(szTempBuffer);
			iTotal += iAttitudeChange;
		}

		iAttitudeChange = kPlayer.AI_getOpenBordersAttitude(eTargetPlayer);
		if ((iPass == 0) ? (iAttitudeChange > 0) : (iAttitudeChange < 0))
		{
			szTempBuffer.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR((iAttitudeChange > 0) ? "COLOR_POSITIVE_TEXT" : "COLOR_NEGATIVE_TEXT"), gDLL->getText("TXT_KEY_MISC_ATTITUDE_OPEN_BORDERS", iAttitudeChange).GetCString());
			szBreakdown.append(NEWLINE);
			szBreakdown.append(szTempBuffer);
			iTotal += iAttitudeChange;
		}

		iAttitudeChange = kPlayer.AI_getDefensivePactAttitude(eTargetPlayer);
		if ((iPass == 0) ? (iAttitudeChange > 0) : (iAttitudeChange < 0))
		{
			szTempBuffer.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR((iAttitudeChange > 0) ? "COLOR_POSITIVE_TEXT" : "COLOR_NEGATIVE_TEXT"), gDLL->getText("TXT_KEY_MISC_ATTITUDE_DEFENSIVE_PACT", iAttitudeChange).GetCString());
			szBreakdown.append(NEWLINE);
			szBreakdown.append(szTempBuffer);
			iTotal += iAttitudeChange;
		}

		iAttitudeChange = kPlayer.AI_getRivalDefensivePactAttitude(eTargetPlayer);
		if ((iPass == 0) ? (iAttitudeChange > 0) : (iAttitudeChange < 0))
		{
			szTempBuffer.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR((iAttitudeChange > 0) ? "COLOR_POSITIVE_TEXT" : "COLOR_NEGATIVE_TEXT"), gDLL->getText("TXT_KEY_MISC_ATTITUDE_RIVAL_DEFENSIVE_PACT", iAttitudeChange).GetCString());
			szBreakdown.append(NEWLINE);
			szBreakdown.append(szTempBuffer);
			iTotal += iAttitudeChange;
		}

		iAttitudeChange = kPlayer.AI_getRivalVassalAttitude(eTargetPlayer);
		if ((iPass == 0) ? (iAttitudeChange > 0) : (iAttitudeChange < 0))
		{
			szTempBuffer.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR((iAttitudeChange > 0) ? "COLOR_POSITIVE_TEXT" : "COLOR_NEGATIVE_TEXT"), gDLL->getText("TXT_KEY_MISC_ATTITUDE_RIVAL_VASSAL", iAttitudeChange).GetCString());
			szBreakdown.append(NEWLINE);
			szBreakdown.append(szTempBuffer);
			iTotal += iAttitudeChange;
		}

		iAttitudeChange = kPlayer.AI_getShareWarAttitude(eTargetPlayer);
		if ((iPass == 0) ? (iAttitudeChange > 0) : (iAttitudeChange < 0))
		{
			szTempBuffer.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR((iAttitudeChange > 0) ? "COLOR_POSITIVE_TEXT" : "COLOR_NEGATIVE_TEXT"), gDLL->getText("TXT_KEY_MISC_ATTITUDE_SHARE_WAR", iAttitudeChange).GetCString());
			szBreakdown.append(NEWLINE);
			szBreakdown.append(szTempBuffer);
			iTotal += iAttitudeChange;
		}

		iAttitudeChange = kPlayer.AI_getFavoriteCivicAttitude(eTargetPlayer);
		if ((iPass == 0) ? (iAttitudeChange > 0) : (iAttitudeChange < 0))
		{
			szTempBuffer.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR((iAttitudeChange > 0) ? "COLOR_POSITIVE_TEXT" : "COLOR_NEGATIVE_TEXT"), gDLL->getText("TXT_KEY_MISC_ATTITUDE_FAVORITE_CIVIC", iAttitudeChange).GetCString());
			szBreakdown.append(NEWLINE);
			szBreakdown.append(szTempBuffer);
			iTotal += iAttitudeChange;
		}

		// MOD - START - Favorite Religion
		iAttitudeChange = kPlayer.AI_getFavoriteReligionAttitude(eTargetPlayer);
		if ((iPass == 0) ? (iAttitudeChange > 0) : (iAttitudeChange < 0))
		{
			szTempBuffer.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR((iAttitudeChange > 0) ? "COLOR_POSITIVE_TEXT" : "COLOR_NEGATIVE_TEXT"), gDLL->getText("TXT_KEY_MISC_ATTITUDE_FAVORITE_RELIGION", iAttitudeChange).GetCString());
			szBuffer.append(NEWLINE);
			szBuffer.append(szTempBuffer);
			iTotal += iAttitudeChange;
		}
		// MOD - END - Favorite Religion

		iAttitudeChange = kPlayer.AI_getTradeAttitude(eTargetPlayer);
		if ((iPass == 0) ? (iAttitudeChange > 0) : (iAttitudeChange < 0))
		{
			szTempBuffer.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR((iAttitudeChange > 0) ? "COLOR_POSITIVE_TEXT" : "COLOR_NEGATIVE_TEXT"), gDLL->getText("TXT_KEY_MISC_ATTITUDE_TRADE", iAttitudeChange).GetCString());
			szBreakdown.append(NEWLINE);
			szBreakdown.append(szTempBuffer);
			iTotal += iAttitudeChange;
		}

		iAttitudeChange = kPlayer.AI_getRivalTradeAttitude(eTargetPlayer);
		if ((iPass == 0) ? (iAttitudeChange > 0) : (iAttitudeChange < 0))
		{
			szTempBuffer.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR((iAttitudeChange > 0) ? "COLOR_POSITIVE_TEXT" : "COLOR_NEGATIVE_TEXT"), gDLL->getText("TXT_KEY_MISC_ATTITUDE_RIVAL_TRADE", iAttitudeChange).GetCString());
			szBreakdown.append(NEWLINE);
			szBreakdown.append(szTempBuffer);
			iTotal += iAttitudeChange;
		}
		// advc.130r: Commented out
		iAttitudeChange = GET_PLAYER(ePlayer).AI_getColonyAttitude(eTargetPlayer);
		if ((iPass == 0) ? (iAttitudeChange > 0) : (iAttitudeChange < 0)) {
			szTempBuffer.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR((iAttitudeChange > 0) ? "COLOR_POSITIVE_TEXT" : "COLOR_NEGATIVE_TEXT"), gDLL->getText("TXT_KEY_MISC_ATTITUDE_FREEDOM", iAttitudeChange).GetCString());
			szBreakdown.append(NEWLINE);
			szBreakdown.append(szTempBuffer);
			iTotal += iAttitudeChange;
		}

		iAttitudeChange = GET_PLAYER(ePlayer).AI_getAttitudeExtra(eTargetPlayer);
		if ((iPass == 0) ? (iAttitudeChange > 0) : (iAttitudeChange < 0))
		{
			szTempBuffer.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR((iAttitudeChange > 0) ? "COLOR_POSITIVE_TEXT" : "COLOR_NEGATIVE_TEXT"), gDLL->getText(((iAttitudeChange > 0) ? "TXT_KEY_MISC_ATTITUDE_EXTRA_GOOD" : "TXT_KEY_MISC_ATTITUDE_EXTRA_BAD"), iAttitudeChange).GetCString());
			szBreakdown.append(NEWLINE);
			szBreakdown.append(szTempBuffer);
			iTotal += iAttitudeChange;
		} 
		// <advc.sha>
		if(bSHowHiddenAttitude) // BEGIN: Show Hidden Attitude Mod 01/22/2010
		{
			iAttitudeChange = kPlayer.AI_getTeamSizeAttitude(eTargetPlayer);
			if((iPass == 0) ? (iAttitudeChange > 0) : (iAttitudeChange < 0))
			{
				szTempBuffer.Format(SETCOLR L"%s" ENDCOLR,
						TEXT_COLOR((iAttitudeChange > 0) ? "COLOR_POSITIVE_TEXT" :
						"COLOR_NEGATIVE_TEXT"),
						gDLL->getText("TXT_KEY_MISC_ATTITUDE_TEAM_SIZE",
						iAttitudeChange).GetCString());
				szBreakdown.append(NEWLINE);
				szBreakdown.append(szTempBuffer);
				iTotal += iAttitudeChange;
			}
			if (!bObscurePersonality) // advc: Moved this check from CvPlayerAI
			{
				iAttitudeChange = kPlayer.AI_getRankDifferenceAttitude(eTargetPlayer);
				CvWString szRankText = gDLL->getText(
						g.getPlayerRank(ePlayer) < g.getPlayerRank(eTargetPlayer) ?
						"TXT_KEY_MISC_ATTITUDE_BETTER_RANK" :
						"TXT_KEY_MISC_ATTITUDE_WORSE_RANK", iAttitudeChange);
				if(iAttitudeChange > 0 && iPass == 0)
				{
					szTempBuffer.Format(SETCOLR L"%s" ENDCOLR,
							TEXT_COLOR("COLOR_POSITIVE_TEXT"), szRankText.GetCString());
					szBreakdown.append(NEWLINE);
					szBreakdown.append(szTempBuffer);
					iTotal += iAttitudeChange;
				}
				if(iAttitudeChange < 0 && iPass != 0)
				{
					szTempBuffer.Format(SETCOLR L"%s" ENDCOLR,
							TEXT_COLOR("COLOR_NEGATIVE_TEXT"), szRankText.GetCString());
					szBreakdown.append(NEWLINE);
					szBreakdown.append(szTempBuffer);
					iTotal += iAttitudeChange;
				}
				iAttitudeChange = kPlayer.AI_getFirstImpressionAttitude(eTargetPlayer);
				if((iPass == 0) ? (iAttitudeChange > 0) : (iAttitudeChange < 0))
				{
					szTempBuffer.Format(SETCOLR L"%s" ENDCOLR,
							TEXT_COLOR((iAttitudeChange > 0) ? "COLOR_POSITIVE_TEXT" :
							"COLOR_NEGATIVE_TEXT"),
							gDLL->getText("TXT_KEY_MISC_ATTITUDE_FIRST_IMPRESSION",
							iAttitudeChange).GetCString());
					szBreakdown.append(NEWLINE);
					szBreakdown.append(szTempBuffer);
					iTotal += iAttitudeChange;
				}
			}
			// Lost-war attitude: disabled
			/*iAttitudeChange = kPlayer.AI_getLostWarAttitude(eTargetPlayer);
			if((iPass == 0) ? (iAttitudeChange > 0) : (iAttitudeChange < 0)) {
				szTempBuffer.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR((iAttitudeChange > 0) ? "COLOR_POSITIVE_TEXT" : "COLOR_NEGATIVE_TEXT"), gDLL->getText("TXT_KEY_MISC_ATTITUDE_LOST_WAR", iAttitudeChange).GetCString());
				szBreakdown.append(NEWLINE);
				szBreakdown.append(szTempBuffer);
				iTotal += iAttitudeChange;
			}*/ // END: Show Hidden Attitude Mod
		} // </advc.sha>

		static MemoryTypes aeObscureMemoryTypes[] =
		{
			MEMORY_DENIED_STOP_TRADING, MEMORY_DENIED_JOIN_WAR,
			MEMORY_ACCEPTED_CIVIC, MEMORY_DENIED_CIVIC,
			MEMORY_ACCEPTED_RELIGION, MEMORY_DENIED_RELIGION,
			MEMORY_REJECTED_DEMAND, MEMORY_ACCEPT_DEMAND,
			MEMORY_REFUSED_HELP, MEMORY_GIVE_HELP,
			MEMORY_SPY_CAUGHT, MEMORY_ACCEPTED_STOP_TRADING,
			MEMORY_NUKED_FRIEND, MEMORY_DECLARED_WAR_ON_FRIEND,
		};
		static int const iNumObscureMemoryTypes = sizeof(aeObscureMemoryTypes) / sizeof(MemoryTypes);
		// </advc.004q>
		for (int iI = 0; iI < NUM_MEMORY_TYPES; ++iI)
		{
			iAttitudeChange = kPlayer.AI_getMemoryAttitude(eTargetPlayer, (MemoryTypes)iI);
			if ((iPass == 0) ? (iAttitudeChange > 0) : (iAttitudeChange < 0))
			{
				/*  <advc.004q> Cap attitude change at 1 for memory types that
					can give away a leader's hidden personality. */
				int iAbsAttitudeChange = std::abs(iAttitudeChange);
				if (iAbsAttitudeChange > 1 && bObscurePersonality &&
					kPlayer.AI_getMemoryCount(eTargetPlayer, (MemoryTypes)iI) <= 2)
				{
					for (int j = 0; j < iNumObscureMemoryTypes; j++)
					{
						if (iI == aeObscureMemoryTypes[j])
						{
							iAbsAttitudeChange = 1;
							break;
						}
					}
					iAttitudeChange = (iAttitudeChange > 0 ? iAbsAttitudeChange : -iAbsAttitudeChange);
				} // </advc.004q>
				szTempBuffer.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR(
						(iAttitudeChange > 0) ? "COLOR_POSITIVE_TEXT" : "COLOR_NEGATIVE_TEXT"),
						gDLL->getText("TXT_KEY_MISC_ATTITUDE_MEMORY", iAttitudeChange,
						GC.getMemoryInfo((MemoryTypes)iI).getDescription()).GetCString());
				szBreakdown.append(NEWLINE);
				szBreakdown.append(szTempBuffer);
				iTotal += iAttitudeChange;
			}
		}
		
		// MOD - START - Trait Attitude Modifier
		iAttitudeChange = 0;
        for (int iI=0; iI < GC.getNumTraitInfos(); iI++)
		{
			if (GET_PLAYER(eTargetPlayer).hasTrait((TraitTypes)iI))
			{
				iAttitudeChange += GC.getTraitInfo((TraitTypes)iI).getAttitudeModifier();
			}
		}
		if ((iPass == 0) ? (iAttitudeChange > 0) : (iAttitudeChange < 0))
		{
			szTempBuffer.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR((iAttitudeChange > 0) ? "COLOR_POSITIVE_TEXT" : "COLOR_NEGATIVE_TEXT"), gDLL->getText((iAttitudeChange > 0) ? "TXT_KEY_MISC_ATTITUDE_TRAIT" : "TXT_KEY_MISC_ATTITUDE_TRAIT_NEG", iAttitudeChange).GetCString());
			szBreakdown.append(NEWLINE);
			szBreakdown.append(szTempBuffer);
			iTotal += iAttitudeChange;
		}
		// MOD - END - Trait Attitude Modifier
		
	}
	// <advc.004q>
	int iTotalCached = kPlayer.AI_getAttitudeVal(eTargetPlayer, false);
	if (!bConstCache && bSHowHiddenAttitude && !bObscurePersonality &&
		iTotal != iTotalCached && !g.isNetworkMultiPlayer())
	{
		FAssertMsg(iTotal == iTotalCached, "Attitude cache out of date "
				"(OK after loading a save created prior to v0.95 or Alt+Z)");
		kPlayer.AI_updateAttitudeCache(eTargetPlayer);
		// Try again, this time without recursion. szBuffer hasn't been changed yet.
		getAttitudeString(szBuffer, ePlayer, eTargetPlayer, true);
		return;
	}
	// Attitude string - append this to szBuffer before appending the breakdown
	szBuffer.append(NEWLINE);
	//szBuffer.append(gDLL->getText("TXT_KEY_ATTITUDE_TOWARDS", GC.getAttitudeInfo(kPlayer.AI_getAttitude(eTargetPlayer)).getTextKeyWide(), GET_PLAYER(eTargetPlayer).getNameKey()));
	// Replacing the BtS line above
	wchar const* szAttitude = GC.getAttitudeInfo(
			kPlayer.AI_getAttitude(eTargetPlayer)).getTextKeyWide();
	wchar const* szTargetName = GET_PLAYER(eTargetPlayer).getNameKey();
	AttitudeTypes eAttitudeCached = kPlayer.AI_getAttitudeFromValue(iTotal);
	if(eAttitudeCached == ATTITUDE_CAUTIOUS)
	{
		if(iTotal == 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_ZERO_ATTITUDE_TOWARDS",
					szAttitude, szTargetName));
		}
		else if(iTotal > 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_POSITIVE_NEUTRAL_ATTITUDE_TOWARDS",
				szAttitude, iTotal, szTargetName));
		}
		else szBuffer.append(gDLL->getText("TXT_KEY_NEGATIVE_NEUTRAL_ATTITUDE_TOWARDS",
				szAttitude, iTotal, szTargetName));
	}
	else if(eAttitudeCached > ATTITUDE_CAUTIOUS)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_GOOD_ATTITUDE_TOWARDS",
				szAttitude, iTotal, szTargetName));
	}
	else szBuffer.append(gDLL->getText("TXT_KEY_BAD_ATTITUDE_TOWARDS",
			szAttitude, iTotal, szTargetName));

	szBuffer.append(szBreakdown);
	// </advc.004q>
	/*if (NO_PLAYER != eTargetPlayer) {
		int iWarWeariness = GET_PLAYER(eTargetPlayer).getModifiedWarWearinessPercentAnger(GET_TEAM(GET_PLAYER(eTargetPlayer).getTeam()).getWarWeariness(eTeam) * std::max(0, 100 + kTeam.getEnemyWarWearinessModifier()));
		if (iWarWeariness / 10000 > 0) {
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_WAR_WEAR_HELP", iWarWeariness / 10000));
		}
	}*/ // K-Mod, I've moved this to a new function
}

// K-Mod
void CvGameTextMgr::getVassalInfoString(CvWStringBuffer& szBuffer, PlayerTypes ePlayer)
{
	FAssert(ePlayer != NO_PLAYER);

	const CvTeam& kTeam = GET_TEAM(GET_PLAYER(ePlayer).getTeam());
	//CvTeam& kTargetTeam = GET_TEAM(GET_PLAYER(eTargetPlayer).getTeam());

	for (TeamTypes i = (TeamTypes)0; i < MAX_TEAMS; i=(TeamTypes)(i+1))
	{
		const CvTeam& kLoopTeam = GET_TEAM(i);
		if (kLoopTeam.isAlive())
		{
			//if (kTargetTeam.isHasMet(i))

			if (kTeam.isVassal(i))
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(CvWString::format(SETCOLR, TEXT_COLOR("COLOR_LIGHT_GREY")));
				szBuffer.append(gDLL->getText("TXT_KEY_ATTITUDE_VASSAL_OF", kLoopTeam.getName().GetCString()));
				setVassalRevoltHelp(szBuffer, i, kTeam.getID());
				szBuffer.append(ENDCOLR);
			}
			else if (kLoopTeam.isVassal(kTeam.getID()))
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(CvWString::format(SETCOLR, TEXT_COLOR("COLOR_LIGHT_GREY")));
				szBuffer.append(gDLL->getText("TXT_KEY_ATTITUDE_MASTER_OF", kLoopTeam.getName().GetCString()));
				szBuffer.append(ENDCOLR);
			}
		}
	}
}

void CvGameTextMgr::getWarWearinessString(CvWStringBuffer& szBuffer, PlayerTypes ePlayer, PlayerTypes eTargetPlayer) const
{
	FAssert(ePlayer != NO_PLAYER);
	// Show ePlayer's war weariness towards eTargetPlayer.
	// (note: this is the reverse of what was shown in the original code.)
	// War weariness should be shown in it natural units - it's a percentage of population
	const CvPlayer& kPlayer = GET_PLAYER(ePlayer);

	int iWarWeariness = 0;
	if (eTargetPlayer == NO_PLAYER || eTargetPlayer == ePlayer)
	{
		// If eTargetPlayer == NO_PLAYER, show ePlayer's total war weariness?
		// There are a couple of problems with displaying the total war weariness: information leak, out-of-date information...
		// lets do it only for the active player.
		if (GC.getGameINLINE().getActivePlayer() == ePlayer)
			iWarWeariness = kPlayer.getWarWearinessPercentAnger();
	}
	else
	{
		const CvPlayer& kTargetPlayer = GET_PLAYER(eTargetPlayer);
		if (atWar(kPlayer.getTeam(), kTargetPlayer.getTeam()) && (GC.getGameINLINE().isDebugMode() || GET_PLAYER(GC.getGameINLINE().getActivePlayer()).canSeeDemographics(ePlayer)))
		{
			iWarWeariness = kPlayer.getModifiedWarWearinessPercentAnger(GET_TEAM(kPlayer.getTeam()).getWarWeariness(kTargetPlayer.getTeam(), true)/100);
		}
	}

	iWarWeariness *= 100;
	iWarWeariness /= GC.getPERCENT_ANGER_DIVISOR();

	if (iWarWeariness != 0)
		szBuffer.append(CvWString::format(L"\n%s: %d%%", gDLL->getText("TXT_KEY_WAR_WEAR_HELP").GetCString(), iWarWeariness));
}
// K-Mod end

void CvGameTextMgr::getEspionageString(CvWStringBuffer& szBuffer, PlayerTypes ePlayer, PlayerTypes eTargetPlayer)
{
	FAssertMsg(false, "obsolete function. (getEspionageString)"); // K-Mod
	if (!GC.getGameINLINE().isOption(GAMEOPTION_NO_ESPIONAGE))
	{
		CvPlayer& kPlayer = GET_PLAYER(ePlayer);
		TeamTypes eTeam = (TeamTypes) kPlayer.getTeam();
		CvTeam& kTeam = GET_TEAM(eTeam);
		CvPlayer& kTargetPlayer = GET_PLAYER(eTargetPlayer);

		szBuffer.append(gDLL->getText("TXT_KEY_ESPIONAGE_AGAINST_PLAYER", kTargetPlayer.getNameKey(), kTeam.getEspionagePointsAgainstTeam(kTargetPlayer.getTeam()), GET_TEAM(kTargetPlayer.getTeam()).getEspionagePointsAgainstTeam(kPlayer.getTeam())));
	}
}

void CvGameTextMgr::getTradeString(CvWStringBuffer& szBuffer, const TradeData& tradeData, PlayerTypes ePlayer1, PlayerTypes ePlayer2)
{
	switch (tradeData.m_eItemType)
	{
	case TRADE_GOLD:
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_GOLD", tradeData.m_iData));
		break;
	case TRADE_GOLD_PER_TURN:
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_GOLD_PER_TURN", tradeData.m_iData));
		break;
	case TRADE_MAPS:
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_WORLD_MAP"));
		break;
	case TRADE_SURRENDER:
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_CAPITULATE"));
		break;
	case TRADE_VASSAL:
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_VASSAL"));
		break;
	case TRADE_OPEN_BORDERS:
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_OPEN_BORDERS"));
		break;
	case TRADE_DEFENSIVE_PACT:
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_DEFENSIVE_PACT"));
		break;
	case TRADE_PERMANENT_ALLIANCE:
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_PERMANENT_ALLIANCE"));
		break;
	case TRADE_PEACE_TREATY:
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_PEACE_TREATY", GC.getDefineINT("PEACE_TREATY_LENGTH")));
		break;
	case TRADE_TECHNOLOGIES:
		szBuffer.assign(CvWString::format(L"%s", GC.getTechInfo((TechTypes)tradeData.m_iData).getDescription()));
		break;
	case TRADE_RESOURCES:
		szBuffer.assign(CvWString::format(L"%s", GC.getBonusInfo((BonusTypes)tradeData.m_iData).getDescription()));
		break;
	case TRADE_CITIES:
		szBuffer.assign(CvWString::format(L"%s", GET_PLAYER(ePlayer1).getCity(tradeData.m_iData)->getName().GetCString()));
		break;
	case TRADE_PEACE:
	case TRADE_WAR:
	case TRADE_EMBARGO:
		szBuffer.assign(CvWString::format(L"%s", GET_TEAM((TeamTypes)tradeData.m_iData).getName().GetCString()));
		break;
	case TRADE_CIVIC:
		szBuffer.assign(CvWString::format(L"%s", GC.getCivicInfo((CivicTypes)tradeData.m_iData).getDescription()));
		break;
	case TRADE_RELIGION:
		szBuffer.assign(CvWString::format(L"%s", GC.getReligionInfo((ReligionTypes)tradeData.m_iData).getDescription()));
		break;
	default:
		FAssert(false);
		break;
	}
}

void CvGameTextMgr::setFeatureHelp(CvWStringBuffer &szBuffer, FeatureTypes eFeature, bool bCivilopediaText)
{
	if (NO_FEATURE == eFeature)
	{
		return;
	}
	CvFeatureInfo& feature = GC.getFeatureInfo(eFeature);

	int aiYields[NUM_YIELD_TYPES];
	if (!bCivilopediaText)
	{
		szBuffer.append(feature.getDescription());

		for (int iI = 0; iI < NUM_YIELD_TYPES; ++iI)
		{
			aiYields[iI] = feature.getYieldChange(iI);
		}
		setYieldChangeHelp(szBuffer, L"", L"", L"", aiYields);
	}
	for (int iI = 0; iI < NUM_YIELD_TYPES; ++iI)
	{
		aiYields[iI] = feature.getRiverYieldChange(iI);
	}
	setYieldChangeHelp(szBuffer, L"", L"", gDLL->getText("TXT_KEY_TERRAIN_NEXT_TO_RIVER"), aiYields);

	for (int iI = 0; iI < NUM_YIELD_TYPES; ++iI)
	{
		aiYields[iI] = feature.getHillsYieldChange(iI);
	}
	setYieldChangeHelp(szBuffer, L"", L"", gDLL->getText("TXT_KEY_TERRAIN_ON_HILLS"), aiYields);

	if (feature.getMovementCost() != 1)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_TERRAIN_MOVEMENT_COST", feature.getMovementCost()));
	}

	CvWString szHealth;
	szHealth.Format(L"%.2f", 0.01f * abs(feature.getHealthPercent()));
	if (feature.getHealthPercent() > 0)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_FEATURE_GOOD_HEALTH", szHealth.GetCString()));
	}
	else if (feature.getHealthPercent() < 0)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_FEATURE_BAD_HEALTH", szHealth.GetCString()));
	}

	// MOD - START - Epidemics
	// Epidemic system by Mexico
	if (feature.getEpidemicModifier() != 0)
	{
		CvWString szTempBuffer;
		szTempBuffer.Format(L"%+d", (feature.getEpidemicModifier() * 100) / GC.getDefineINT("EPIDEMIC_RAND_BASE"));
		szBuffer.append(gDLL->getText("TXT_KEY_FEATURE_EPIDEMIC", szTempBuffer.GetCString()));
	}
	// MOD - END - Epidemics

	if (feature.getDefenseModifier() != 0)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_TERRAIN_DEFENSE_MODIFIER", feature.getDefenseModifier()));
	}

/*************************************************************************************************/
/* UNOFFICIAL_PATCH                       06/02/10                           LunarMongoose       */
/*                                                                                               */
/* Bugfix                                                                                        */
/*************************************************************************************************/
	// Mongoose FeatureDamageFix
	if (feature.getTurnDamage() > 0)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_TERRAIN_TURN_DAMAGE", feature.getTurnDamage()));
	}
	else if (feature.getTurnDamage() < 0)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_TERRAIN_TURN_HEALING", -feature.getTurnDamage()));
	}
/*************************************************************************************************/
/* UNOFFICIAL_PATCH                         END                                                  */
/*************************************************************************************************/

	if (feature.isAddsFreshWater())
	{
		szBuffer.append(gDLL->getText("TXT_KEY_FEATURE_ADDS_FRESH_WATER"));
	}

	if (feature.isImpassable())
	{
		szBuffer.append(gDLL->getText("TXT_KEY_TERRAIN_IMPASSABLE"));
	}

	if (feature.isNoCity())
	{
		szBuffer.append(gDLL->getText("TXT_KEY_TERRAIN_NO_CITIES"));
	}

	if (feature.isNoImprovement())
	{
		szBuffer.append(gDLL->getText("TXT_KEY_FEATURE_NO_IMPROVEMENT"));
	}

}


void CvGameTextMgr::setTerrainHelp(CvWStringBuffer &szBuffer, TerrainTypes eTerrain, bool bCivilopediaText)
{
	if (NO_TERRAIN == eTerrain)
	{
		return;
	}
	CvTerrainInfo& terrain = GC.getTerrainInfo(eTerrain);

	int aiYields[NUM_YIELD_TYPES];
	if (!bCivilopediaText)
	{
		szBuffer.append(terrain.getDescription());

		for (int iI = 0; iI < NUM_YIELD_TYPES; ++iI)
		{
			aiYields[iI] = terrain.getYield(iI);
		}
		setYieldChangeHelp(szBuffer, L"", L"", L"", aiYields);
	}
	for (int iI = 0; iI < NUM_YIELD_TYPES; ++iI)
	{
		aiYields[iI] = terrain.getRiverYieldChange(iI);
	}
	setYieldChangeHelp(szBuffer, L"", L"", gDLL->getText("TXT_KEY_TERRAIN_NEXT_TO_RIVER"), aiYields);

	for (int iI = 0; iI < NUM_YIELD_TYPES; ++iI)
	{
		aiYields[iI] = terrain.getHillsYieldChange(iI);
	}
	setYieldChangeHelp(szBuffer, L"", L"", gDLL->getText("TXT_KEY_TERRAIN_ON_HILLS"), aiYields);

	if (terrain.getMovementCost() != 1)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_TERRAIN_MOVEMENT_COST", terrain.getMovementCost()));
	}

	if (terrain.getBuildModifier() != 0)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_TERRAIN_BUILD_MODIFIER", terrain.getBuildModifier()));
	}

	if (terrain.getDefenseModifier() != 0)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_TERRAIN_DEFENSE_MODIFIER", terrain.getDefenseModifier()));
	}

	if (terrain.isImpassable())
	{
		szBuffer.append(gDLL->getText("TXT_KEY_TERRAIN_IMPASSABLE"));
	}
	if (!terrain.isFound())
	{
		szBuffer.append(gDLL->getText("TXT_KEY_TERRAIN_NO_CITIES"));
		bool bFirst = true;
		if (terrain.isFoundCoast())
		{
			szBuffer.append(gDLL->getText("TXT_KEY_TERRAIN_COASTAL_CITIES"));
			bFirst = false;
		}
		if (!bFirst)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_OR"));
		}
		if (terrain.isFoundFreshWater())
		{
			szBuffer.append(gDLL->getText("TXT_KEY_TERRAIN_FRESH_WATER_CITIES"));
			bFirst = false;
		}
	}
}

// MOD - START - Improved Civilopedia
void CvGameTextMgr::setRouteHelp(CvWStringBuffer &szBuffer, RouteTypes eRoute, bool bCivilopediaText)
{
	if (NO_ROUTE == eRoute)
	{
		return;
	}

	CvRouteInfo& kRoute = GC.getRouteInfo(eRoute);
	CvWString szTemp;

	if (!bCivilopediaText)
	{
		szTemp.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), kRoute.getDescription());
		szBuffer.append(szTemp);
	}

	//int iMovementCost = kRoute.getMovementCost() + GET_TEAM(pHeadSelectedUnit->getTeam()).getRouteChange(eRoute);
	int iMovementCost = kRoute.getMovementCost();
	int iFlatMovementCost = kRoute.getFlatMovementCost();
	int iMoves;
	int iFlatMoves;

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
}

void CvGameTextMgr::setWorldSizeHelp(CvWStringBuffer &szBuffer, WorldSizeTypes eWorldSize, bool bCivilopediaText)
{
	if (NO_WORLDSIZE == eWorldSize)
	{
		return;
	}

	CvWorldInfo& kWorldSize = GC.getWorldInfo(eWorldSize);
	CvWString szTemp;

	if (!bCivilopediaText)
	{
		szTemp.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), kWorldSize.getDescription());
		szBuffer.append(szTemp);
		szBuffer.append(L" (");
		szBuffer.append(gDLL->getText("TXT_KEY_WORLD_SIZE_DIMENSIONS", kWorldSize.getGridWidthInPlots(), kWorldSize.getGridHeightInPlots()));
		szBuffer.append(L")");
	}

	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_RESEARCH_PERCENT", kWorldSize.getResearchPercent()));

	// MOD - START - Per City Research Cost
	if (!GC.getGameINLINE().isFinalInitialized() || !GC.getGameINLINE().isOption(GAMEOPTION_NO_PER_CITY_RESEARCH_COST))
	{
		if (kWorldSize.getPerCityResearchCostModifier() != 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_PER_CITY_RESEARCH_COST_MODIFIER", kWorldSize.getPerCityResearchCostModifier()));
		}
	}
	// MOD - END - Per City Research Cost

	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_TRADE_PROFIT_PERCENT", kWorldSize.getTradeProfitPercent()));

	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_DISTANCE_MAINT_PERCENT", kWorldSize.getDistanceMaintenancePercent()));

	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_NUM_CITIES_MAINT_PERCENT", kWorldSize.getNumCitiesMaintenancePercent()));

	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_COLONY_MAINT_PERCENT", kWorldSize.getColonyMaintenancePercent()));

	if (kWorldSize.getWarWearinessModifier() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_WAR_WEARINESS_MOD", kWorldSize.getWarWearinessModifier()));
	}
}

void CvGameTextMgr::setHandicapHelp(CvWStringBuffer &szBuffer, HandicapTypes eHandicap, bool bCivilopediaText)
{
	if (NO_HANDICAP == eHandicap)
	{
		return;
	}

	CvHandicapInfo& kHandicap = GC.getHandicapInfo(eHandicap);
	CvWString szTemp;

	if (!bCivilopediaText)
	{
		szTemp.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), kHandicap.getDescription());
		szBuffer.append(szTemp);

		if (kHandicap.getHappyBonus() != 0)
		{
			szTemp.Format(L", +%d%c", abs(kHandicap.getHappyBonus()), (kHandicap.getHappyBonus() > 0) ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR));
			szBuffer.append(szTemp);
		}

		if (kHandicap.getHealthBonus() != 0)
		{
			szTemp.Format(L", +%d%c", abs(kHandicap.getHealthBonus()), (kHandicap.getHealthBonus() > 0) ? gDLL->getSymbolID(HEALTHY_CHAR) : gDLL->getSymbolID(UNHEALTHY_CHAR));
			szBuffer.append(szTemp);
		}
	}

	// MOD - START - Epidemics
	const int iRandBase = GC.getDefineINT("EPIDEMIC_RAND_BASE");

	szTemp.Format(L"%d", (kHandicap.getEpidemicBaseRate() * 100) / iRandBase);
	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_HANDICAP_EPIDEMIC_BASE_RATE", szTemp.GetCString()));

	szTemp.Format(L"%d", (kHandicap.getEpidemicPopulationRate() * 100) / iRandBase);
	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_HANDICAP_EPIDEMIC_POPULATION_RATE", szTemp.GetCString()));

	szTemp.Format(L"%d", (kHandicap.getEpidemicUnhealthRate() * 100) / iRandBase);
	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_HANDICAP_EPIDEMIC_UNHEALTH_RATE", szTemp.GetCString()));

	szTemp.Format(L"%d", (kHandicap.getEpidemicTradeRouteRate() * 100) / iRandBase);
	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_HANDICAP_EPIDEMIC_TRADE_ROUTE_RATE", szTemp.GetCString()));
	// MOD - END - Epidemics

	// Starting Gold

	if (kHandicap.getFreeUnits() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_STARTING_FREE_UNITS", kHandicap.getFreeUnits()));
	}

	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_UNIT_COST_PERCENT", kHandicap.getUnitCostPercent()));

	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_RESEARCH_PERCENT", kHandicap.getResearchPercent()));

	// MOD - START - Per City Research Cost
	if (!GC.getGameINLINE().isFinalInitialized() || !GC.getGameINLINE().isOption(GAMEOPTION_NO_PER_CITY_RESEARCH_COST))
	{
		if (kHandicap.getPerCityResearchCostModifier() != 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_PER_CITY_RESEARCH_COST_MODIFIER", kHandicap.getPerCityResearchCostModifier()));
		}
	}
	// MOD - END - Per City Research Cost

	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_DISTANCE_MAINT_PERCENT", kHandicap.getDistanceMaintenancePercent()));

	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_NUM_CITIES_MAINT_PERCENT", kHandicap.getNumCitiesMaintenancePercent()));

	// NOTE: Disabled by K-Mod
	/*
	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_MAX_NUM_CITIES_MAINT", kHandicap.getMaxNumCitiesMaintenance()));
	*/

	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_COLONY_MAINT_PERCENT", kHandicap.getColonyMaintenancePercent()));

	// MaxColonyMaintenance
	// CorporationMaintenancePercent

	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_CIVIC_UPKEEP_PERCENT", kHandicap.getCivicUpkeepPercent()));

	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_INFLATION_PERCENT", kHandicap.getInflationPercent()));

	// AttitudeChange
	// NoTechTradeModifier
	// TechTradeKnownModifier

	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_UNOWNED_TILES_PER_GAME_ANIMAL", kHandicap.getUnownedTilesPerGameAnimal()));

	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_UNOWNED_TILES_PER_BARBARIAN_UNIT", kHandicap.getUnownedTilesPerBarbarianUnit()));

	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_UNOWNED_WATER_TILES_PER_BARBARIAN_UNIT", kHandicap.getUnownedWaterTilesPerBarbarianUnit()));

	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_UNOWNED_TILES_PER_BARBARIAN_CITY", kHandicap.getUnownedTilesPerBarbarianUnit()));

	// BarbarianCreationTurnsElapsed
	// BarbarianCityCreationTurnsElapsed
	// BarbarianCityCreationProb
	// AnimalBonus
	// BarbarianBonus

	// AIAnimalBonus
	// AIBarbarianBonus
	// StartingDefenseUnits
	// StartingWorkerUnits
	// StartingExploreUnits
	// AIStartingUnitMultiplier
	// AIStartingDefenseUnits
	// AIStartingWorkerUnits
	// AIStartingExploreUnits
	// BarbarianDefenders
	// AIDeclareWarProb
	// AIWorkRateModifier
	// AIGrowthPercent

	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_AI_TRAIN_PERCENT", kHandicap.getAITrainPercent()));

	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_AI_WORLD_TRAIN_PERCENT", kHandicap.getAIWorldTrainPercent()));

	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_AI_CONSTRUCT_PERCENT", kHandicap.getAIConstructPercent()));

	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_AI_WORLD_CONSTRUCT_PERCENT", kHandicap.getAIWorldConstructPercent()));

	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_AI_CREATE_PERCENT", kHandicap.getAICreatePercent()));

	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_AI_WORLD_CREATE_PERCENT", kHandicap.getAIWorldCreatePercent()));

	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_AI_CIVIC_UPKEEP_PERCENT", kHandicap.getAICivicUpkeepPercent()));

	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_AI_UNIT_COST_PERCENT", kHandicap.getAIUnitCostPercent()));

	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_AI_UNIT_SUPPLY_PERCENT", kHandicap.getAIUnitSupplyPercent()));

	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_AI_UNIT_UPGRADE_PERCENT", kHandicap.getAIUnitUpgradePercent()));

	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_AI_INFLATION_PERCENT", kHandicap.getAIInflationPercent()));

	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_AI_WAR_WEARINESS_MOD", kHandicap.getAIWarWearinessPercent()));

	if (kHandicap.getAIPerEraModifier() != 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_HANDICAP_AI_ERA_MOD", kHandicap.getAIPerEraModifier()));
	}
}

void CvGameTextMgr::setConceptHelp(CvWStringBuffer &szBuffer, ConceptTypes eConcept, bool bCivilopediaText)
{
	if (eConcept == NO_CONCEPT)
	{
		return;
	}

	const CvInfoBase& kConcept = GC.getConceptInfo(eConcept);
	CvWString szTemp;

	if (!bCivilopediaText)
	{
		szTemp.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), kConcept.getDescription());
		szBuffer.append(szTemp);
	}
}


void CvGameTextMgr::setHotKeyClassHelp(CvWStringBuffer &szBuffer, HotKeyClassTypes eHotKeyClass, bool bCivilopediaText)
{
	if (eHotKeyClass == NO_HOTKEYCLASS)
	{
		return;
	}

	const CvHotKeyClassInfo& kHotKeyClass = GC.getHotKeyClassInfo(eHotKeyClass);
	CvWString szTemp;

	if (!bCivilopediaText)
	{
		szTemp.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), kHotKeyClass.getDescription());
		szBuffer.append(szTemp);
	}
}
// MOD - END - Improved Civilopedia

// MOD - START - Era Effects
void CvGameTextMgr::setEraHelp(CvWStringBuffer &szBuffer, EraTypes eEra, bool bSplash, bool bCivilopediaText)
{
	if (NO_ERA == eEra)
	{
		return;
	}

	CvEraInfo& kEra = GC.getEraInfo(eEra);
	CvWString szTemp;

	if (!bSplash && !bCivilopediaText)
	{
		szTemp.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), kEra.getDescription());
		szBuffer.append(szTemp);
	}

	if (kEra.getDiscoverHappiness() != 0)
	{
		szBuffer.append(NEWLINE);
		if (bSplash)
		{
			if (kEra.getDiscoverHappiness() > 0)
			{
				szBuffer.append(gDLL->getText("TXT_KEY_ERA_SPLASH_HAPPINESS", kEra.getDiscoverHappiness()));
			}
			else
			{
				szBuffer.append(gDLL->getText("TXT_KEY_ERA_SPLASH_ANGER", std::abs(kEra.getDiscoverHappiness())));
			}
		}
		else
		{
			szBuffer.append(gDLL->getText("TXT_KEY_ERA_CHANGE", abs(kEra.getDiscoverHappiness()), (kEra.getDiscoverHappiness() > 0) ? gDLL->getSymbolID(HAPPY_CHAR) : gDLL->getSymbolID(UNHAPPY_CHAR)));
		}
	}

	if (kEra.getDiscoverHealth() != 0)
	{
		szBuffer.append(NEWLINE);
		if (bSplash)
		{
			if (kEra.getDiscoverHealth() > 0)
			{
				szBuffer.append(gDLL->getText("TXT_KEY_ERA_SPLASH_HEALTHINESS", kEra.getDiscoverHealth()));
			}
			else
			{
				szBuffer.append(gDLL->getText("TXT_KEY_ERA_SPLASH_UNHEALTHINESS", std::abs(kEra.getDiscoverHealth())));
			}
		}
		else
		{
			szBuffer.append(gDLL->getText("TXT_KEY_ERA_CHANGE", abs(kEra.getDiscoverHealth()), (kEra.getDiscoverHealth() > 0) ? gDLL->getSymbolID(HEALTHY_CHAR) : gDLL->getSymbolID(UNHEALTHY_CHAR)));
		}
	}
}
// MOD - END - Era Effects

void CvGameTextMgr::buildFinanceInflationString(CvWStringBuffer& szBuffer, PlayerTypes ePlayer)
{
	if (NO_PLAYER == ePlayer)
	{
		return;
	}
	CvPlayer& kPlayer = GET_PLAYER(ePlayer);

	int iInflationRate = kPlayer.getInflationRate();
	//if (iInflationRate != 0)
	{
		int iPreInflation = kPlayer.calculatePreInflatedCosts();
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_FINANCE_ADVISOR_INFLATION", iPreInflation, iInflationRate, iInflationRate, iPreInflation, (iPreInflation * iInflationRate) / 100));
	}
}

void CvGameTextMgr::buildFinanceUnitCostString(CvWStringBuffer& szBuffer, PlayerTypes ePlayer)
{
	if (NO_PLAYER == ePlayer)
	{
		return;
	}
	CvPlayer& player = GET_PLAYER(ePlayer);

	int iFreeUnits = 0;
	int iFreeMilitaryUnits = 0;
	int iUnits = player.getNumUnits();
	int iMilitaryUnits = player.getNumMilitaryUnits();
	int iPaidUnits = iUnits;
	int iPaidMilitaryUnits = iMilitaryUnits;
	int iMilitaryCost = 0;
	int iUnitCost = 0; // was called "base unit cost"
	int iExtraCost = 0;
	int iCost = player.calculateUnitCost(iFreeUnits, iFreeMilitaryUnits, iPaidUnits, iPaidMilitaryUnits, iUnitCost, iMilitaryCost, iExtraCost);
	int iHandicap = iCost-iUnitCost-iMilitaryCost-iExtraCost;

	// K-Mod include inflation
	iCost = ROUND_DIVIDE(iCost*(100+player.getInflationRate()), 100);
	iUnitCost = ROUND_DIVIDE(iUnitCost*(100+player.getInflationRate()), 100);
	iMilitaryCost = ROUND_DIVIDE(iMilitaryCost*(100+player.getInflationRate()), 100);
	iHandicap = ROUND_DIVIDE(iHandicap*(100+player.getInflationRate()), 100);
	// K-Mod end

	szBuffer.append(NEWLINE);
	//szBuffer.append(gDLL->getText("TXT_KEY_FINANCE_ADVISOR_UNIT_COST", iPaidUnits, iFreeUnits, iBaseUnitCost));
	szBuffer.append(gDLL->getText("TXT_KEY_FINANCE_ADVISOR_UNIT_COST", iUnits, iFreeUnits, iUnitCost)); // K-Mod

	//if (iPaidMilitaryUnits != 0)
	if (iMilitaryCost != 0) // K-Mod
	{
		//szBuffer.append(gDLL->getText("TXT_KEY_FINANCE_ADVISOR_UNIT_COST_2", iPaidMilitaryUnits, iFreeMilitaryUnits, iMilitaryCost));
		szBuffer.append(gDLL->getText("TXT_KEY_FINANCE_ADVISOR_UNIT_COST_2", iMilitaryUnits, iFreeMilitaryUnits, iMilitaryCost)); // K-Mod
	}
	if (iExtraCost != 0)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_FINANCE_ADVISOR_UNIT_COST_3", iExtraCost));
	}
	if (iHandicap != 0)
	{
		FAssertMsg(false, "not all unit costs were accounted for"); // K-Mod (handicap modifier are now rolled into the other costs)
		szBuffer.append(gDLL->getText("TXT_KEY_FINANCE_ADVISOR_HANDICAP_COST", iHandicap));
		szBuffer.append(CvWString::format(L" (%+d%%)", GC.getHandicapInfo(player.getHandicapType()).getUnitCostPercent()-100)); // K-Mod
	}
	szBuffer.append(gDLL->getText("TXT_KEY_FINANCE_ADVISOR_UNIT_COST_4", iCost));
}

void CvGameTextMgr::buildFinanceAwaySupplyString(CvWStringBuffer& szBuffer, PlayerTypes ePlayer)
{
	if (NO_PLAYER == ePlayer)
	{
		return;
	}
	CvPlayer& player = GET_PLAYER(ePlayer);

	int iPaidUnits = 0;
	int iBaseCost = 0;
	int iCost = player.calculateUnitSupply(iPaidUnits, iBaseCost);
	int iHandicap = iCost - iBaseCost;

	// K-Mod include inflation
	iCost = ROUND_DIVIDE(iCost*(100+player.getInflationRate()), 100);
	iBaseCost = ROUND_DIVIDE(iBaseCost*(100+player.getInflationRate()), 100);
	iHandicap = ROUND_DIVIDE(iHandicap*(100+player.getInflationRate()), 100);
	// K-Mod end

	CvWString szHandicap;
	if (iHandicap != 0)
	{
		FAssertMsg(false, "not all supply costs were accounted for"); // K-Mod (handicap modifier are now rolled into the other costs)
		szHandicap = gDLL->getText("TXT_KEY_FINANCE_ADVISOR_HANDICAP_COST", iHandicap);
	}

	szBuffer.append(NEWLINE);
	//szBuffer.append(gDLL->getText("TXT_KEY_FINANCE_ADVISOR_SUPPLY_COST", iPaidUnits, GC.getDefineINT("INITIAL_FREE_OUTSIDE_UNITS"), iBaseCost, szHandicap.GetCString(), iCost));
	szBuffer.append(gDLL->getText("TXT_KEY_FINANCE_ADVISOR_SUPPLY_COST", player.getNumOutsideUnits(), player.getNumOutsideUnits() - iPaidUnits, iBaseCost, szHandicap.GetCString(), iCost)); // K-Mod
}

void CvGameTextMgr::buildFinanceCityMaintString(CvWStringBuffer& szBuffer, PlayerTypes ePlayer)
{
	if (NO_PLAYER == ePlayer)
	{
		return;
	}
	int iLoop;
	int iDistanceMaint = 0;
	int iColonyMaint = 0;
	int iCorporationMaint = 0;

	CvPlayer& player = GET_PLAYER(ePlayer);
	for (CvCity* pLoopCity = player.firstCity(&iLoop); pLoopCity != NULL; pLoopCity = player.nextCity(&iLoop))
	{
/**
*** K-Mod, 06/sep/10, Karadoc
*** Bug fix
**/

		/* Old BTS code
		iDistanceMaint += (pLoopCity->calculateDistanceMaintenanceTimes100() * std::max(0, (pLoopCity->getMaintenanceModifier() + 100))) / 100;
		iColonyMaint += (pLoopCity->calculateColonyMaintenanceTimes100() * std::max(0, (pLoopCity->getMaintenanceModifier() + 100))) / 100;
		iCorporationMaint += (pLoopCity->calculateCorporationMaintenanceTimes100() * std::max(0, (pLoopCity->getMaintenanceModifier() + 100))) / 100;
		*/
		if (!pLoopCity->isDisorder() && !pLoopCity->isWeLoveTheKingDay() && (pLoopCity->getPopulation() > 0))
		{
			iDistanceMaint += (pLoopCity->calculateDistanceMaintenanceTimes100() * std::max(0, (pLoopCity->getMaintenanceModifier() + 100))+50) / 100;
			iColonyMaint += (pLoopCity->calculateColonyMaintenanceTimes100() * std::max(0, (pLoopCity->getMaintenanceModifier() + 100))+50) / 100;
			iCorporationMaint += (pLoopCity->calculateCorporationMaintenanceTimes100() * std::max(0, (pLoopCity->getMaintenanceModifier() + 100))+50) / 100;
		}
/**
*** K-Mod end
**/
	}

	// K-Mod. Changed to include the effects of inflation.
	int iInflationFactor = player.getInflationRate()+100;

	iDistanceMaint = ROUND_DIVIDE(iDistanceMaint * iInflationFactor, 10000);
	iColonyMaint = ROUND_DIVIDE(iColonyMaint * iInflationFactor, 10000);
	iCorporationMaint = ROUND_DIVIDE(iCorporationMaint * iInflationFactor, 10000); // Note: currently, calculateCorporationMaintenanceTimes100 includes the inverse of this factor.

	int iNumCityMaint = ROUND_DIVIDE(player.getTotalMaintenance() * iInflationFactor, 100) - iDistanceMaint - iColonyMaint - iCorporationMaint;

	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_FINANCE_ADVISOR_CITY_MAINT_COST", iDistanceMaint, iNumCityMaint, iColonyMaint, iCorporationMaint, player.getTotalMaintenance()*iInflationFactor/100));
}

void CvGameTextMgr::buildFinanceCivicUpkeepString(CvWStringBuffer& szBuffer, PlayerTypes ePlayer)
{
	if (NO_PLAYER == ePlayer)
	{
		return;
	}
	CvPlayer& player = GET_PLAYER(ePlayer);
	CvWString szCivicOptionCosts;
	for (int iI = 0; iI < GC.getNumCivicOptionInfos(); ++iI)
	{
		CivicTypes eCivic = player.getCivics((CivicOptionTypes)iI);
		if (NO_CIVIC != eCivic)
		{
			CvWString szTemp;
			//szTemp.Format(L"%d%c: %s", player.getSingleCivicUpkeep(eCivic), GC.getCommerceInfo(COMMERCE_GOLD).getChar(),  GC.getCivicInfo(eCivic).getDescription());
			szTemp.Format(L"%d%c: %s", ROUND_DIVIDE(player.getSingleCivicUpkeep(eCivic)*(100+player.getInflationRate()),100), GC.getCommerceInfo(COMMERCE_GOLD).getChar(),  GC.getCivicInfo(eCivic).getDescription()); // K-Mod
			szCivicOptionCosts += NEWLINE + szTemp;
		}
	}

	szBuffer.append(NEWLINE);
	//szBuffer.append(gDLL->getText("TXT_KEY_FINANCE_ADVISOR_CIVIC_UPKEEP_COST", szCivicOptionCosts.GetCString(), player.getCivicUpkeep()));
	szBuffer.append(gDLL->getText("TXT_KEY_FINANCE_ADVISOR_CIVIC_UPKEEP_COST", szCivicOptionCosts.GetCString(), ROUND_DIVIDE(player.getCivicUpkeep()*(100+player.getInflationRate()),100))); // K-Mod
}

void CvGameTextMgr::buildFinanceForeignIncomeString(CvWStringBuffer& szBuffer, PlayerTypes ePlayer)
{
	if (NO_PLAYER == ePlayer)
	{
		return;
	}
	CvPlayer& player = GET_PLAYER(ePlayer);

	CvWString szPlayerIncome;
	for (int iI = 0; iI < MAX_PLAYERS; ++iI)
	{
		CvPlayer& otherPlayer = GET_PLAYER((PlayerTypes)iI);
		if (otherPlayer.isAlive() && player.getGoldPerTurnByPlayer((PlayerTypes)iI) != 0)
		{
			CvWString szTemp;
			szTemp.Format(L"%d%c: %s", player.getGoldPerTurnByPlayer((PlayerTypes)iI), GC.getCommerceInfo(COMMERCE_GOLD).getChar(), otherPlayer.getCivilizationShortDescription());
			szPlayerIncome += NEWLINE + szTemp;
		}
	}
	if (!szPlayerIncome.empty())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_FINANCE_ADVISOR_FOREIGN_INCOME", szPlayerIncome.GetCString(), player.getGoldPerTurn()));
	}
}

// BUG - Building Additional Yield - start
bool CvGameTextMgr::setBuildingAdditionalYieldHelp(CvWStringBuffer &szBuffer, const CvCity& city, YieldTypes eIndex, const CvWString& szStart, bool bStarted)
{
	CvYieldInfo& info = GC.getYieldInfo(eIndex);
	CvWString szLabel;
	CvCivilizationInfo& kCivilization = GC.getCivilizationInfo(GET_PLAYER(city.getOwnerINLINE()).getCivilizationType());

	for (int iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
	{
		BuildingTypes eBuilding = (BuildingTypes)kCivilization.getCivilizationBuildings((BuildingClassTypes)iI);

		if (eBuilding != NO_BUILDING && city.canConstruct(eBuilding, false, true, false))
		{
			int iChange = city.getAdditionalYieldByBuilding(eIndex, eBuilding);

			if (iChange != 0)
			{
				if (!bStarted)
				{
					szBuffer.append(szStart);
					bStarted = true;
				}

				szLabel.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR("COLOR_BUILDING_TEXT"), GC.getBuildingInfo(eBuilding).getDescription());
				setResumableValueChangeHelp(szBuffer, szLabel, L": ", L"", iChange, info.getChar(), false, true);
			}
		}
	}

	return bStarted;
}
// BUG - Building Additional Yield - end

// BUG - Building Additional Commerce - start
bool CvGameTextMgr::setBuildingAdditionalCommerceHelp(CvWStringBuffer &szBuffer, const CvCity& city, CommerceTypes eIndex, const CvWString& szStart, bool bStarted)
{
	CvCommerceInfo& info = GC.getCommerceInfo(eIndex);
	CvWString szLabel;
	CvCivilizationInfo& kCivilization = GC.getCivilizationInfo(GET_PLAYER(city.getOwnerINLINE()).getCivilizationType());

	for (int iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
	{
		BuildingTypes eBuilding = (BuildingTypes)kCivilization.getCivilizationBuildings((BuildingClassTypes)iI);

		if (eBuilding != NO_BUILDING && city.canConstruct(eBuilding, false, true, false))
		{
			int iChange = city.getAdditionalCommerceTimes100ByBuilding(eIndex, eBuilding);

			if (iChange != 0)
			{
				if (!bStarted)
				{
					szBuffer.append(szStart);
					bStarted = true;
				}

				szLabel.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR("COLOR_BUILDING_TEXT"), GC.getBuildingInfo(eBuilding).getDescription());
				setResumableValueTimes100ChangeHelp(szBuffer, szLabel, L": ", L"", iChange, info.getChar(), true);
			}
		}
	}

	return bStarted;
}
// BUG - Building Additional Commerce - end

// BUG - Building Saved Maintenance - start
bool CvGameTextMgr::setBuildingSavedMaintenanceHelp(CvWStringBuffer &szBuffer, const CvCity& city, const CvWString& szStart, bool bStarted)
{
	CvCommerceInfo& info = GC.getCommerceInfo(COMMERCE_GOLD);
	CvWString szLabel;
	CvCivilizationInfo& kCivilization = GC.getCivilizationInfo(GET_PLAYER(city.getOwnerINLINE()).getCivilizationType());

	for (int iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
	{
		BuildingTypes eBuilding = (BuildingTypes)kCivilization.getCivilizationBuildings((BuildingClassTypes)iI);

		if (eBuilding != NO_BUILDING && city.canConstruct(eBuilding, false, true, false))
		{
			int iChange = city.getSavedMaintenanceTimes100ByBuilding(eBuilding);
			
			if (iChange != 0)
			{
				if (!bStarted)
				{
					szBuffer.append(szStart);
					bStarted = true;
				}

				szLabel.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR("COLOR_BUILDING_TEXT"), GC.getBuildingInfo(eBuilding).getDescription());
				setResumableValueTimes100ChangeHelp(szBuffer, szLabel, L": ", L"", iChange, info.getChar(), true);
			}
		}
	}

	return bStarted;
}
// BUG - Building Saved Maintenance - end

void CvGameTextMgr::setProductionHelp(CvWStringBuffer &szBuffer, CvCity& city)
{
	FAssertMsg(NO_PLAYER != city.getOwnerINLINE(), "City must have an owner");

	bool bIsProcess = city.isProductionProcess();
	int iPastOverflow = (bIsProcess ? 0 : city.getOverflowProduction());
	if (iPastOverflow != 0)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_PROD_OVERFLOW", iPastOverflow));
		szBuffer.append(NEWLINE);
	}

	int iFromChops = (city.isProductionProcess() ? 0 : city.getFeatureProduction());
	if (iFromChops != 0)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_PROD_CHOPS", iFromChops));
		szBuffer.append(NEWLINE);
	}

	//if (city.getCurrentProductionDifference(false, true) == 0)
// BUG - Building Additional Production - start
	bool bBuildingAdditionalYield = getBugOptionBOOL("MiscHover__BuildingAdditionalProduction", false, "BUG_BUILDING_ADDITIONAL_PRODUCTION_HOVER");
	if (city.getCurrentProductionDifference(false, true) == 0 && !bBuildingAdditionalYield)
// BUG - Building Additional Production - end
	{
		return;
	}

	setYieldHelp(szBuffer, city, YIELD_PRODUCTION);

	int iBaseProduction = city.getBaseYieldRate(YIELD_PRODUCTION) + iPastOverflow + iFromChops;
	int iBaseModifier = city.getBaseYieldRateModifier(YIELD_PRODUCTION);

	UnitTypes eUnit = city.getProductionUnit();
	if (NO_UNIT != eUnit)
	{
		CvUnitInfo& unit = GC.getUnitInfo(eUnit);

		// Domain
		int iDomainMod = city.getDomainProductionModifier((DomainTypes)unit.getDomainType());
		if (0 != iDomainMod)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_PROD_DOMAIN", iDomainMod, GC.getDomainInfo((DomainTypes)unit.getDomainType()).getTextKeyWide()));
			szBuffer.append(NEWLINE);
			iBaseModifier += iDomainMod;
		}

		// Military
		if (unit.isMilitaryProduction())
		{
			int iMilitaryMod = city.getMilitaryProductionModifier() + GET_PLAYER(city.getOwnerINLINE()).getMilitaryProductionModifier();
			if (0 != iMilitaryMod)
			{
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_PROD_MILITARY", iMilitaryMod));
				szBuffer.append(NEWLINE);
				iBaseModifier += iMilitaryMod;
			}
		}

		// Bonus
		for (int i = 0; i < GC.getNumBonusInfos(); i++)
		{
			if (city.hasBonus((BonusTypes)i))
			{
				int iBonusMod = unit.getBonusProductionModifier(i);
				if (0 != iBonusMod)
				{
					szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_PROD_MOD_BONUS", iBonusMod, unit.getTextKeyWide(), GC.getBonusInfo((BonusTypes)i).getTextKeyWide()));
					szBuffer.append(NEWLINE);
					iBaseModifier += iBonusMod;
				}
			}
		}

		// Trait
		for (int i = 0; i < GC.getNumTraitInfos(); i++)
		{
			if (city.hasTrait((TraitTypes)i))
			{
				int iTraitMod = unit.getProductionTraits(i);

				if (unit.getSpecialUnitType() != NO_SPECIALUNIT)
				{
					iTraitMod += GC.getSpecialUnitInfo((SpecialUnitTypes) unit.getSpecialUnitType()).getProductionTraits(i);
				}
				if (0 != iTraitMod)
				{
					szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_PROD_TRAIT", iTraitMod, unit.getTextKeyWide(), GC.getTraitInfo((TraitTypes)i).getTextKeyWide()));
					szBuffer.append(NEWLINE);
					iBaseModifier += iTraitMod;
				}
			}
		}

		// Religion
		if (NO_PLAYER != city.getOwnerINLINE() && NO_RELIGION != GET_PLAYER(city.getOwnerINLINE()).getStateReligion())
		{
			if (city.isHasReligion(GET_PLAYER(city.getOwnerINLINE()).getStateReligion()))
			{
				int iReligionMod = GET_PLAYER(city.getOwnerINLINE()).getStateReligionUnitProductionModifier();
				if (0 != iReligionMod)
				{
					szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_PROD_RELIGION", iReligionMod, GC.getReligionInfo(GET_PLAYER(city.getOwnerINLINE()).getStateReligion()).getTextKeyWide()));
					szBuffer.append(NEWLINE);
					iBaseModifier += iReligionMod;
				}
			}
		}
	}

	BuildingTypes eBuilding = city.getProductionBuilding();
	if (NO_BUILDING != eBuilding)
	{
		CvBuildingInfo& building = GC.getBuildingInfo(eBuilding);

		// Bonus
		for (int i = 0; i < GC.getNumBonusInfos(); i++)
		{
			if (city.hasBonus((BonusTypes)i))
			{
				int iBonusMod = building.getBonusProductionModifier(i);
				if (0 != iBonusMod)
				{
					szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_PROD_MOD_BONUS", iBonusMod, building.getTextKeyWide(), GC.getBonusInfo((BonusTypes)i).getTextKeyWide()));
					szBuffer.append(NEWLINE);
					iBaseModifier += iBonusMod;
				}
			}
		}

		// Trait
		for (int i = 0; i < GC.getNumTraitInfos(); i++)
		{
			if (city.hasTrait((TraitTypes)i))
			{
				int iTraitMod = building.getProductionTraits(i);

				if (building.getSpecialBuildingType() != NO_SPECIALBUILDING)
				{
					iTraitMod += GC.getSpecialBuildingInfo((SpecialBuildingTypes) building.getSpecialBuildingType()).getProductionTraits(i);
				}
				if (0 != iTraitMod)
				{
					szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_PROD_TRAIT", iTraitMod, building.getTextKeyWide(), GC.getTraitInfo((TraitTypes)i).getTextKeyWide()));
					szBuffer.append(NEWLINE);
					iBaseModifier += iTraitMod;
				}
			}
		}

		// Wonder
		if (isWorldWonderClass((BuildingClassTypes)(GC.getBuildingInfo(eBuilding).getBuildingClassType())) && NO_PLAYER != city.getOwnerINLINE())
		{
			int iWonderMod = GET_PLAYER(city.getOwnerINLINE()).getMaxGlobalBuildingProductionModifier();
			if (0 != iWonderMod)
			{
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_PROD_WONDER", iWonderMod));
				szBuffer.append(NEWLINE);
				iBaseModifier += iWonderMod;
			}
		}

		// Team Wonder
		if (isTeamWonderClass((BuildingClassTypes)(GC.getBuildingInfo(eBuilding).getBuildingClassType())) && NO_PLAYER != city.getOwnerINLINE())
		{
			int iWonderMod = GET_PLAYER(city.getOwnerINLINE()).getMaxTeamBuildingProductionModifier();
			if (0 != iWonderMod)
			{
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_PROD_TEAM_WONDER", iWonderMod));
				szBuffer.append(NEWLINE);
				iBaseModifier += iWonderMod;
			}
		}

		// National Wonder
		if (isNationalWonderClass((BuildingClassTypes)(GC.getBuildingInfo(eBuilding).getBuildingClassType())) && NO_PLAYER != city.getOwnerINLINE())
		{
			int iWonderMod = GET_PLAYER(city.getOwnerINLINE()).getMaxPlayerBuildingProductionModifier();
			if (0 != iWonderMod)
			{
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_PROD_NATIONAL_WONDER", iWonderMod));
				szBuffer.append(NEWLINE);
				iBaseModifier += iWonderMod;
			}
		}

		// Religion
		if (NO_PLAYER != city.getOwnerINLINE() && NO_RELIGION != GET_PLAYER(city.getOwnerINLINE()).getStateReligion())
		{
			if (city.isHasReligion(GET_PLAYER(city.getOwnerINLINE()).getStateReligion()))
			{
				int iReligionMod = GET_PLAYER(city.getOwnerINLINE()).getStateReligionBuildingProductionModifier();
				if (0 != iReligionMod)
				{
					szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_PROD_RELIGION", iReligionMod, GC.getReligionInfo(GET_PLAYER(city.getOwnerINLINE()).getStateReligion()).getTextKeyWide()));
					szBuffer.append(NEWLINE);
					iBaseModifier += iReligionMod;
				}
			}
		}
	}

	ProjectTypes eProject = city.getProductionProject();
	if (NO_PROJECT != eProject)
	{
		CvProjectInfo& project = GC.getProjectInfo(eProject);

		// Spaceship
		if (project.isSpaceship())
		{
			int iSpaceshipMod = city.getSpaceProductionModifier();
			if (NO_PLAYER != city.getOwnerINLINE())
			{
				iSpaceshipMod += GET_PLAYER(city.getOwnerINLINE()).getSpaceProductionModifier();
			}
			if (0 != iSpaceshipMod)
			{
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_PROD_SPACESHIP", iSpaceshipMod));
				szBuffer.append(NEWLINE);
				iBaseModifier += iSpaceshipMod;
			}
		}

		// Bonus
		for (int i = 0; i < GC.getNumBonusInfos(); i++)
		{
			if (city.hasBonus((BonusTypes)i))
			{
				int iBonusMod = project.getBonusProductionModifier(i);
				if (0 != iBonusMod)
				{
					szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_PROD_MOD_BONUS", iBonusMod, project.getTextKeyWide(), GC.getBonusInfo((BonusTypes)i).getTextKeyWide()));
					szBuffer.append(NEWLINE);
					iBaseModifier += iBonusMod;
				}
			}
		}
	}

	int iFoodProduction = (city.isFoodProduction() ? std::max(0, (city.getYieldRate(YIELD_FOOD) - city.foodConsumption(true))) : 0);
	if (iFoodProduction > 0)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_PROD_FOOD", iFoodProduction, iFoodProduction));
		szBuffer.append(NEWLINE);
	}

	int iModProduction = iFoodProduction + (iBaseModifier * iBaseProduction) / 100;

	FAssertMsg(iModProduction == city.getCurrentProductionDifference(false, !bIsProcess), "Modified Production does not match actual value");

	szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_PROD_FINAL_YIELD", iModProduction));
	//szBuffer.append(NEWLINE);

// BUG - Building Additional Production - start
	if (bBuildingAdditionalYield && city.getOwnerINLINE() == GC.getGame().getActivePlayer())
	{
		setBuildingAdditionalYieldHelp(szBuffer, city, YIELD_PRODUCTION, DOUBLE_SEPARATOR);
	}
// BUG - Building Additional Production - end
}


void CvGameTextMgr::parsePlayerTraits(CvWStringBuffer &szBuffer, PlayerTypes ePlayer)
{
	bool bFirst = true;

	for (int iTrait = 0; iTrait < GC.getNumTraitInfos(); ++iTrait)
	{
		if (GET_PLAYER(ePlayer).hasTrait((TraitTypes)iTrait))
		{
			if (bFirst)
			{
				szBuffer.append(L" (");
				bFirst = false;
			}
			else
			{
				szBuffer.append(L", ");
			}
			szBuffer.append(GC.getTraitInfo((TraitTypes)iTrait).getDescription());
		}
	}

	if (!bFirst)
	{
		szBuffer.append(L")");
	}
}

// K-Mod. I've rewritten most of this function.
void CvGameTextMgr::parseLeaderHeadHelp(CvWStringBuffer &szBuffer, PlayerTypes eThisPlayer, PlayerTypes eOtherPlayer)
{
	if (eThisPlayer == NO_PLAYER)
		return;

	const CvPlayerAI& kPlayer = GET_PLAYER(eThisPlayer);

	szBuffer.append(CvWString::format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), kPlayer.getName()));

	parsePlayerTraits(szBuffer, eThisPlayer);

	// Some debug info: found-site traits, and AI flavours
	if (gDLL->getChtLvl() > 0 && GC.altKey())
	{
		szBuffer.append(CvWString::format(SETCOLR SEPARATOR NEWLINE, TEXT_COLOR("COLOR_LIGHT_GREY")));
		CvPlayerAI::CvFoundSettings kFoundSet(kPlayer, false);

		bool bFirst = true;

#define trait_info(x) do { \
	if (kFoundSet.b##x) \
	{ \
        szBuffer.append(CvWString::format(L"%s" L#x, bFirst? L"" : L", ")); \
		bFirst = false; \
	} \
} while (0)

		trait_info(Ambitious);
		trait_info(Defensive);
		trait_info(EasyCulture);
		trait_info(Expansive);
		trait_info(Financial);
		trait_info(Seafaring);

#undef trait_info

#define flavour_info(x) do { \
	if (kPlayer.AI_getFlavorValue(FLAVOR_##x)) \
	{ \
        szBuffer.append(CvWString::format(L"%s" L#x L"=%d", bFirst? L"" : L", ", kPlayer.AI_getFlavorValue(FLAVOR_##x))); \
		bFirst = false; \
	} \
} while (0)

		flavour_info(MILITARY);
		flavour_info(RELIGION);
		flavour_info(PRODUCTION);
		flavour_info(GOLD);
		flavour_info(SCIENCE);
		flavour_info(CULTURE);
		flavour_info(GROWTH);

#undef flavour_info

		szBuffer.append(SEPARATOR ENDCOLR);
	}

	//szBuffer.append(L"\n");

	if (eOtherPlayer != NO_PLAYER)
	{
		CvTeam& kThisTeam = GET_TEAM(kPlayer.getTeam());
		TeamTypes eOtherTeam = GET_PLAYER(eOtherPlayer).getTeam();

		if (kThisTeam.isHasMet(eOtherTeam)) // K-Mod. Allow the "other relations string" to display even if eOtherPlayer == eThisPlayer. It's useful info.
		{
			getVassalInfoString(szBuffer, eThisPlayer); // K-Mod
			//getEspionageString(szBuffer, eThisPlayer, eOtherPlayer); // disabled by K-Mod. (The player should not be told exactly how many espionage points everyone has.)

			if (eOtherPlayer != eThisPlayer)
			{
				getAttitudeString(szBuffer, eThisPlayer, eOtherPlayer);
				getActiveDealsString(szBuffer, eThisPlayer, eOtherPlayer);
			}
			getWarWearinessString(szBuffer, eThisPlayer, eOtherPlayer);

			getOtherRelationsString(szBuffer, eThisPlayer, eOtherPlayer);
		}
	}
	else
		getWarWearinessString(szBuffer, eThisPlayer, NO_PLAYER); // total war weariness
}


void CvGameTextMgr::parseLeaderLineHelp(CvWStringBuffer &szBuffer, PlayerTypes eThisPlayer, PlayerTypes eOtherPlayer)
{
	if (NO_PLAYER == eThisPlayer || NO_PLAYER == eOtherPlayer)
	{
		return;
	}
	CvTeam& thisTeam = GET_TEAM(GET_PLAYER(eThisPlayer).getTeam());
	CvTeam& otherTeam = GET_TEAM(GET_PLAYER(eOtherPlayer).getTeam());

	if (thisTeam.getID() == otherTeam.getID())
	{
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_PERMANENT_ALLIANCE"));
		szBuffer.append(NEWLINE);
	}
	else if (thisTeam.isAtWar(otherTeam.getID()))
	{
		szBuffer.append(gDLL->getText("TXT_KEY_CONCEPT_WAR"));
		szBuffer.append(NEWLINE);
	}
	else
	{
		if (thisTeam.isDefensivePact(otherTeam.getID()))
		{
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_DEFENSIVE_PACT"));
			szBuffer.append(NEWLINE);
		}
		if (thisTeam.isOpenBorders(otherTeam.getID()))
		{
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_OPEN_BORDERS"));
			szBuffer.append(NEWLINE);
		}
		if (thisTeam.isVassal(otherTeam.getID()))
		{
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_VASSAL"));
			szBuffer.append(NEWLINE);
		}
	}
}


void CvGameTextMgr::getActiveDealsString(CvWStringBuffer &szBuffer, PlayerTypes eThisPlayer, PlayerTypes eOtherPlayer)
{
	int iIndex;
	CvDeal* pDeal = GC.getGameINLINE().firstDeal(&iIndex);
	while (NULL != pDeal)
	{
		if ((pDeal->getFirstPlayer() == eThisPlayer && pDeal->getSecondPlayer() == eOtherPlayer)
			|| (pDeal->getFirstPlayer() == eOtherPlayer && pDeal->getSecondPlayer() == eThisPlayer))
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(CvWString::format(L"%c", gDLL->getSymbolID(BULLET_CHAR)));
			getDealString(szBuffer, *pDeal, eThisPlayer);
		}
		pDeal = GC.getGameINLINE().nextDeal(&iIndex);
	}
}

void CvGameTextMgr::getOtherRelationsString(CvWStringBuffer& szString, PlayerTypes eThisPlayer, PlayerTypes eOtherPlayer)
{
	if (eThisPlayer == NO_PLAYER || eOtherPlayer == NO_PLAYER)
	{
		return;
	}
	CvPlayer& kThisPlayer = GET_PLAYER(eThisPlayer);
	CvPlayer& kOtherPlayer = GET_PLAYER(eOtherPlayer);

/************************************************************************************************/
/* BETTER_BTS_AI                         11/08/08                                jdog5000       */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
/* original code
	for (int iTeam = 0; iTeam < MAX_CIV_TEAMS; ++iTeam)
	{
		CvTeamAI& kTeam = GET_TEAM((TeamTypes) iTeam);
		if (kTeam.isAlive() && !kTeam.isMinorCiv() && iTeam != kThisPlayer.getTeam() && iTeam != kOtherPlayer.getTeam())
		{
			if (kTeam.isHasMet(kOtherPlayer.getTeam()))
			{
				if (::atWar((TeamTypes) iTeam, kThisPlayer.getTeam()))
				{
					szString.append(NEWLINE);
					szString.append(gDLL->getText(L"TXT_KEY_AT_WAR_WITH", kTeam.getName().GetCString()));
				}

				if (!kTeam.isHuman() && kTeam.AI_getWorstEnemy() == kThisPlayer.getTeam())
				{
					szString.append(NEWLINE);
					szString.append(gDLL->getText(L"TXT_KEY_WORST_ENEMY_OF", kTeam.getName().GetCString()));
				}
			}
		}
	}
*/
	// Put all war, worst enemy strings on one line
	CvWStringBuffer szWarWithString;
	CvWStringBuffer szWorstEnemyString;
	bool bFirst = true;
	bool bFirst2 = true;
	for (int iTeam = 0; iTeam < MAX_CIV_TEAMS; ++iTeam)
	{
		CvTeamAI& kTeam = GET_TEAM((TeamTypes) iTeam);
		//if (kTeam.isAlive() && !kTeam.isMinorCiv() && iTeam != kThisPlayer.getTeam() && iTeam != kOtherPlayer.getTeam())
		if (kTeam.isAlive() && !kTeam.isMinorCiv() && iTeam != kThisPlayer.getTeam()) // K-Mod. (show "at war" even for the civ selected.)
		{
/************************************************************************************************/
/* UNOFFICIAL_PATCH                       09/28/09                  EmperorFool & jdog5000      */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
/* original bts code
			if (kTeam.isHasMet(kOtherPlayer.getTeam()))
*/
			if (kTeam.isHasMet(kOtherPlayer.getTeam()) && kTeam.isHasMet(kThisPlayer.getTeam()) && kTeam.isHasMet(GC.getGameINLINE().getActiveTeam()))
/************************************************************************************************/
/* UNOFFICIAL_PATCH                        END                                                  */
/************************************************************************************************/
			{
				if (::atWar((TeamTypes) iTeam, kThisPlayer.getTeam()))
				{
					setListHelp(szWarWithString, L"", kTeam.getName().GetCString(), L", ", bFirst);
					bFirst = false;
				}

				if ( !kTeam.isHuman() && kTeam.AI_getWorstEnemy() == kThisPlayer.getTeam() )
				{
					setListHelp(szWorstEnemyString, L"", kTeam.getName().GetCString(), L", ", bFirst2);
					bFirst2 = false;
				}
			}
		}
	}

	if( !szWorstEnemyString.isEmpty() )
	{
		CvWString szTempBuffer;

		szTempBuffer.assign(gDLL->getText(L"TXT_KEY_WORST_ENEMY_OF", szWorstEnemyString.getCString()));

		szString.append(NEWLINE);
		szString.append(szTempBuffer);
	}
	if( !szWarWithString.isEmpty() )
	{
		CvWString szTempBuffer;

		szTempBuffer.assign(gDLL->getText(L"TXT_KEY_AT_WAR_WITH", szWarWithString.getCString()));

		szString.append(NEWLINE);
		szString.append(szTempBuffer);
	}

/************************************************************************************************/
/* BETTER_BTS_AI                          END                                                   */
/************************************************************************************************/
}

void CvGameTextMgr::buildHintsList(CvWStringBuffer& szBuffer)
{
	for (int i = 0; i < GC.getNumHints(); i++)
	{
		szBuffer.append(CvWString::format(L"%c%s", gDLL->getSymbolID(BULLET_CHAR), GC.getHints(i).getText()));
		szBuffer.append(NEWLINE);
		szBuffer.append(NEWLINE);
	}
}

void CvGameTextMgr::setCommerceHelp(CvWStringBuffer &szBuffer, CvCity& city, CommerceTypes eCommerceType)
{
// BUG - Building Additional Commerce - start
	bool bBuildingAdditionalCommerce = getBugOptionBOOL("MiscHover__BuildingAdditionalCommerce", false, "BUG_BUILDING_ADDITIONAL_COMMERCE_HOVER");
	if (NO_COMMERCE == eCommerceType || (0 == city.getCommerceRateTimes100(eCommerceType) && !bBuildingAdditionalCommerce))
// BUG - Building Additional Commerce - end
	{
		return;
	}
	CvCommerceInfo& info = GC.getCommerceInfo(eCommerceType);

	if (NO_PLAYER == city.getOwnerINLINE())
	{
		return;
	}
	CvPlayer& owner = GET_PLAYER(city.getOwnerINLINE());

	setYieldHelp(szBuffer, city, YIELD_COMMERCE);

	// Slider
	int iBaseCommerceRate = city.getCommerceFromPercent(eCommerceType, (city.getYieldRate(YIELD_COMMERCE) * 100) + city.getDecimalTrade());
	int iBaseCommerceDecimal = (city.getYieldRate(YIELD_COMMERCE) * 100) + city.getDecimalTrade();
	CvWString szRate = CvWString::format(L"%d.%02d", iBaseCommerceRate/100, iBaseCommerceRate%100);
	CvWString szDecimal = CvWString::format(L"%d.%02d", iBaseCommerceDecimal/100, iBaseCommerceDecimal%100);
	szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_SLIDER_PERCENT_FLOAT", owner.getCommercePercent(eCommerceType), szDecimal.GetCString(), szRate.GetCString(), info.getChar()));
	szBuffer.append(NEWLINE);

// BUG - Base Commerce - start
	bool bNeedSubtotal = false;
// BUG - Base Commerce - end

	// MOD - START - Advanced Yield and Commerce Effects
	//int iSpecialistCommerce = city.getSpecialistCommerce(eCommerceType) + (city.getSpecialistPopulation() + city.getNumGreatPeople()) * owner.getSpecialistExtraCommerce(eCommerceType);
	int iSpecialistCommerce = city.getSpecialistCommerce(eCommerceType);
	// MOD - END - Advanced Yield and Commerce Effects
	if (0 != iSpecialistCommerce)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_SPECIALIST_COMMERCE", iSpecialistCommerce, info.getChar(), L"TXT_KEY_CONCEPT_SPECIALISTS"));
		szBuffer.append(NEWLINE);
		iBaseCommerceRate += 100 * iSpecialistCommerce;
// BUG - Base Commerce - start
		bNeedSubtotal = true;
// BUG - Base Commerce - end
	}

	int iReligionCommerce = city.getReligionCommerce(eCommerceType);
	if (0 != iReligionCommerce)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_RELIGION_COMMERCE", iReligionCommerce, info.getChar()));
		szBuffer.append(NEWLINE);
		iBaseCommerceRate += 100 * iReligionCommerce;
// BUG - Base Commerce - start
		bNeedSubtotal = true;
// BUG - Base Commerce - end
	}

	int iCorporationCommerce = city.getCorporationCommerce(eCommerceType);
	if (0 != iCorporationCommerce)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_CORPORATION_COMMERCE", iCorporationCommerce, info.getChar()));
		szBuffer.append(NEWLINE);
		iBaseCommerceRate += 100 * iCorporationCommerce;
// BUG - Base Commerce - start
		bNeedSubtotal = true;
// BUG - Base Commerce - end
	}

	int iBuildingCommerce = city.getBuildingCommerce(eCommerceType);
	if (0 != iBuildingCommerce)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_BUILDING_COMMERCE", iBuildingCommerce, info.getChar()));
		szBuffer.append(NEWLINE);
		iBaseCommerceRate += 100 * iBuildingCommerce;
// BUG - Base Commerce - start
		bNeedSubtotal = true;
// BUG - Base Commerce - end
	}

	int iFreeCityCommerce = owner.getFreeCityCommerce(eCommerceType);
	if (0 != iFreeCityCommerce)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_FREE_CITY_COMMERCE", iFreeCityCommerce, info.getChar()));
		szBuffer.append(NEWLINE);
		iBaseCommerceRate += 100 * iFreeCityCommerce;
// BUG - Base Commerce - start
		bNeedSubtotal = true;
// BUG - Base Commerce - end
	}

	// MOD - START - Open Borders Commerce
	int iOpenBordersFreeCityCommerce = owner.getTotalOpenBordersFreeCityCommerce(eCommerceType);
	if (0 != iOpenBordersFreeCityCommerce)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_OPEN_BORDERS_FREE_CITY_COMMERCE", iOpenBordersFreeCityCommerce, info.getChar()));
		szBuffer.append(NEWLINE);
		iBaseCommerceRate += 100 * iOpenBordersFreeCityCommerce;
// BUG - Base Commerce - start
		bNeedSubtotal = true;
// BUG - Base Commerce - end
	}
	// MOD - END - Open Borders Commerce

	// MOD - START - Tech Transfer Commerce
	/*
	int iTechTransferFreeCityCommerce = owner.getTotalTechTransferFreeCityCommerce(eCommerceType);
	if (0 != iTechTransferFreeCityCommerce)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_TECH_TRANSFER_FREE_CITY_COMMERCE", iTechTransferFreeCityCommerce, info.getChar()));
		szBuffer.append(NEWLINE);
		iBaseCommerceRate += 100 * iTechTransferFreeCityCommerce;
// BUG - Base Commerce - start
		bNeedSubtotal = true;
// BUG - Base Commerce - end
	}
	*/
	// MOD - END - Tech Transfer Commerce

// BUG - Base Commerce - start
	if (bNeedSubtotal && city.getCommerceRateModifier(eCommerceType) != 0 && getBugOptionBOOL("MiscHover__BaseCommerce", false, "BUG_CITY_SCREEN_BASE_COMMERCE_HOVER"))
	{
		CvWString szYield = CvWString::format(L"%d.%02d", iBaseCommerceRate/100, iBaseCommerceRate%100);
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_COMMERCE_SUBTOTAL_YIELD_FLOAT", info.getTextKeyWide(), szYield.GetCString(), info.getChar()));
		szBuffer.append(NEWLINE);
	}
// BUG - Base Commerce - end

	FAssertMsg(city.getBaseCommerceRateTimes100(eCommerceType) == iBaseCommerceRate, "Base Commerce rate does not agree with actual value");
	
	int iModifier = 100;

	// Buildings
	int iBuildingMod = 0;
	for (int i = 0; i < GC.getNumBuildingInfos(); i++)
	{
		CvBuildingInfo& infoBuilding = GC.getBuildingInfo((BuildingTypes)i);
		// MOD - START - Building Activation
		/*
		if (city.getNumBuilding((BuildingTypes)i) > 0 && !GET_TEAM(city.getTeam()).isObsoleteBuilding((BuildingTypes)i))
		{
			for (int iLoop = 0; iLoop < city.getNumBuilding((BuildingTypes)i); iLoop++)
			{
				iBuildingMod += infoBuilding.getCommerceModifier(eCommerceType);
			}
		}
		*/

		if (city.getNumActiveBuilding((BuildingTypes)i) > 0)
		{
			for (int iLoop = 0; iLoop < city.getNumActiveBuilding((BuildingTypes)i); iLoop++)
			{
				iBuildingMod += infoBuilding.getCommerceModifier(eCommerceType);
			}
		}
		// MOD - END - Building Activation

		for (int j = 0; j < MAX_PLAYERS; j++)
		{
			if (GET_PLAYER((PlayerTypes)j).isAlive())
			{
				if (GET_PLAYER((PlayerTypes)j).getTeam() == owner.getTeam())
				{
					int iLoop;
					for (CvCity* pLoopCity = GET_PLAYER((PlayerTypes)j).firstCity(&iLoop); pLoopCity != NULL; pLoopCity = GET_PLAYER((PlayerTypes)j).nextCity(&iLoop))
					{
						// MOD - START - Building Activation
						/*
						if (pLoopCity->getNumBuilding((BuildingTypes)i) > 0 && !GET_TEAM(pLoopCity->getTeam()).isObsoleteBuilding((BuildingTypes)i))
						{
							for (int iLoop = 0; iLoop < pLoopCity->getNumBuilding((BuildingTypes)i); iLoop++)
							{
								iBuildingMod += infoBuilding.getGlobalCommerceModifier(eCommerceType);
							}
						}
						*/

						if (pLoopCity->getNumActiveBuilding((BuildingTypes)i) > 0)
						{
							for (int iLoop = 0; iLoop < pLoopCity->getNumActiveBuilding((BuildingTypes)i); iLoop++)
							{
								iBuildingMod += infoBuilding.getGlobalCommerceModifier(eCommerceType);
							}
						}
						// MOD - END - Building Activation
					}
				}
			}
		}
	}
	if (0 != iBuildingMod)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_YIELD_BUILDINGS", iBuildingMod, info.getChar()));
		szBuffer.append(NEWLINE);
		iModifier += iBuildingMod;
	}


	// Trait
	for (int i = 0; i < GC.getNumTraitInfos(); i++)
	{
		if (city.hasTrait((TraitTypes)i))
		{
			CvTraitInfo& trait = GC.getTraitInfo((TraitTypes)i);
			int iTraitMod = trait.getCommerceModifier(eCommerceType);
			if (0 != iTraitMod)
			{
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_COMMERCE_TRAIT", iTraitMod, info.getChar(), trait.getTextKeyWide()));
				szBuffer.append(NEWLINE);
				iModifier += iTraitMod;
			}
		}
	}

	// Capital
	int iCapitalMod = city.isCapital() ? owner.getCapitalCommerceRateModifier(eCommerceType) : 0;
	if (iCapitalMod != 0)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_YIELD_CAPITAL", iCapitalMod, info.getChar()));
		szBuffer.append(NEWLINE);
		iModifier += iCapitalMod;
	}


	// Civics
	int iCivicMod = 0;
	for (int i = 0; i < GC.getNumCivicOptionInfos(); i++)
	{
		if (NO_CIVIC != owner.getCivics((CivicOptionTypes)i))
		{
			iCivicMod += GC.getCivicInfo(owner.getCivics((CivicOptionTypes)i)).getCommerceModifier(eCommerceType);
		}
	}
	if (0 != iCivicMod)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_YIELD_CIVICS", iCivicMod, info.getChar()));
		szBuffer.append(NEWLINE);
		iModifier += iCivicMod;
	}

	// Techs (K-Mod)
	/* Rather than counting just tech modifiers. We might as well just have an generic case which supports other sources too.
	int iTechMod = 0;
	for (TechTypes i = (TechTypes)0; i < GC.getNumTechInfos(); i=(TechTypes)(i+1))
	{
		if (GET_TEAM(owner.getTeam()).isHasTech(i))
			iTechMod += GC.getTechInfo(i).getCommerceModifier(eCommerceType);
	}
	if (iTechMod != 0)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_MODIFIER_FROM_CIV", iTechMod, info.getChar()));
		szBuffer.append(NEWLINE);
		iModifier += iCivicMod;
	} */

	// Left over modifiers!
	int iUnaccountedModifiers = city.getTotalCommerceRateModifier(eCommerceType)-iModifier;
	if (iUnaccountedModifiers != 0)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_MODIFIER_FROM_CIV", iUnaccountedModifiers, info.getChar()));
		szBuffer.append(NEWLINE);
		iModifier += iUnaccountedModifiers;
	}
	// K-Mod end

	// MOD - START - Open Borders Commerce
	int iOpenBordersMod = owner.getTotalOpenBordersCommerceRateModifier(eCommerceType);
	if (iOpenBordersMod != 0)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_YIELD_OPEN_BORDERS", iOpenBordersMod, info.getChar()));
		szBuffer.append(NEWLINE);
		iModifier += iOpenBordersMod;
	}
	// MOD - END - Open Borders Commerce

	// MOD - START - Tech Transfer Commerce
	/*
	int iTechTransferMod = owner.getTotalTechTransferCommerceRateModifier(eCommerceType);
	if (iTechTransferMod != 0)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_YIELD_TECH_TRANSFER", iTechTransferMod, info.getChar()));
		szBuffer.append(NEWLINE);
		iModifier += iTechTransferMod;
	}
	*/
	// MOD - END - Tech Transfer Commerce

	int iModYield = (iModifier * iBaseCommerceRate) / 100;

	int iProductionToCommerce = city.getProductionToCommerceModifier(eCommerceType) * city.getYieldRate(YIELD_PRODUCTION);
	if (0 != iProductionToCommerce)
	{
		if (iProductionToCommerce%100 == 0)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_PRODUCTION_TO_COMMERCE", iProductionToCommerce/100, info.getChar()));
			szBuffer.append(NEWLINE);
		}
		else
		{
			szRate = CvWString::format(L"+%d.%02d", iProductionToCommerce/100, iProductionToCommerce%100);
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_PRODUCTION_TO_COMMERCE_FLOAT", szRate.GetCString(), info.getChar()));
			szBuffer.append(NEWLINE);
		}
		iModYield += iProductionToCommerce;
	}

	if (eCommerceType == COMMERCE_CULTURE && GC.getGameINLINE().isOption(GAMEOPTION_NO_ESPIONAGE))
	{
		int iEspionageToCommerce = city.getCommerceRateTimes100(COMMERCE_CULTURE) - iModYield;
		if (0 != iEspionageToCommerce)
		{
			if (iEspionageToCommerce%100 == 0)
			{
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_COMMERCE_TO_COMMERCE", iEspionageToCommerce/100, info.getChar(), GC.getCommerceInfo(COMMERCE_ESPIONAGE).getChar()));
				szBuffer.append(NEWLINE);
			}
			else
			{
				szRate = CvWString::format(L"+%d.%02d", iEspionageToCommerce/100, iEspionageToCommerce%100);
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_COMMERCE_TO_COMMERCE_FLOAT", szRate.GetCString(), info.getChar(), GC.getCommerceInfo(COMMERCE_ESPIONAGE).getChar()));
				szBuffer.append(NEWLINE);
			}
			iModYield += iEspionageToCommerce;
		}
	}

	FAssertMsg(iModYield == city.getCommerceRateTimes100(eCommerceType), "Commerce yield does not match actual value");

	CvWString szYield = CvWString::format(L"%d.%02d", iModYield/100, iModYield%100);
	szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_COMMERCE_FINAL_YIELD_FLOAT", info.getTextKeyWide(), szYield.GetCString(), info.getChar()));
	//szBuffer.append(NEWLINE);

// BUG - Building Additional Commerce - start
	if (bBuildingAdditionalCommerce && city.getOwnerINLINE() == GC.getGame().getActivePlayer())
	{
		setBuildingAdditionalCommerceHelp(szBuffer, city, eCommerceType, DOUBLE_SEPARATOR);
	}
// BUG - Building Additional Commerce - end
}


void CvGameTextMgr::setYieldHelp(CvWStringBuffer &szBuffer, CvCity& city, YieldTypes eYieldType)
{
	FAssertMsg(NO_PLAYER != city.getOwnerINLINE(), "City must have an owner");

	if (NO_YIELD == eYieldType)
	{
		return;
	}
	CvYieldInfo& info = GC.getYieldInfo(eYieldType);

	if (NO_PLAYER == city.getOwnerINLINE())
	{
		return;
	}
	CvPlayer& owner = GET_PLAYER(city.getOwnerINLINE());

	if (YIELD_COMMERCE == eYieldType)
	{
		int iBaseCommerceDecimal = (city.getYieldRate(YIELD_COMMERCE) * 100) + city.getDecimalTrade();
		CvWString szDecimal = CvWString::format(L"%d.%02d", iBaseCommerceDecimal/100, iBaseCommerceDecimal%100);
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_BASE_COMMERCE", info.getTextKeyWide(), szDecimal.GetCString(), info.getChar()));
		szBuffer.append(NEWLINE);		
	}
	else
	{
		int iBaseProduction = city.getBaseYieldRate(eYieldType);
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_BASE_YIELD", info.getTextKeyWide(), iBaseProduction, info.getChar()));
		szBuffer.append(NEWLINE);
	}

	int iBaseModifier = 100;

	// Buildings
	int iBuildingMod = 0;
	for (int i = 0; i < GC.getNumBuildingInfos(); i++)
	{
		CvBuildingInfo& infoBuilding = GC.getBuildingInfo((BuildingTypes)i);
		// MOD - START - Building Replacement
		//if (city.getNumBuilding((BuildingTypes)i) > 0 && !GET_TEAM(city.getTeam()).isObsoleteBuilding((BuildingTypes)i))
		if (city.getNumActiveBuilding((BuildingTypes)i))
		// MOD - END - Building Replacement
		{
			for (int iLoop = 0; iLoop < city.getNumBuilding((BuildingTypes)i); iLoop++)
			{
				iBuildingMod += infoBuilding.getYieldModifier(eYieldType);
			}
		}
		for (int j = 0; j < MAX_PLAYERS; j++)
		{
			if (GET_PLAYER((PlayerTypes)j).isAlive())
			{
				if (GET_PLAYER((PlayerTypes)j).getTeam() == owner.getTeam())
				{
					int iLoop;
					for (CvCity* pLoopCity = GET_PLAYER((PlayerTypes)j).firstCity(&iLoop); pLoopCity != NULL; pLoopCity = GET_PLAYER((PlayerTypes)j).nextCity(&iLoop))
					{
						// MOD - START - Building Replacement
						//if (pLoopCity->getNumBuilding((BuildingTypes)i) > 0 && !GET_TEAM(pLoopCity->getTeam()).isObsoleteBuilding((BuildingTypes)i))
						if (pLoopCity->getNumActiveBuilding((BuildingTypes)i) > 0)
						// MOD - END - Building Replacement
						// TODO: Consider changing all these loops to use getNumActiveBuilding
						{
							for (int iLoop = 0; iLoop < pLoopCity->getNumBuilding((BuildingTypes)i); iLoop++)
							{
								iBuildingMod += infoBuilding.getGlobalYieldModifier(eYieldType);
							}
						}
					}
				}
			}
		}
	}
	if (NULL != city.area())
	{
		iBuildingMod += city.area()->getYieldRateModifier(city.getOwnerINLINE(), eYieldType);
	}
	if (0 != iBuildingMod)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_YIELD_BUILDINGS", iBuildingMod, info.getChar()));
		szBuffer.append(NEWLINE);
		iBaseModifier += iBuildingMod;
	}

	// Power
	if (city.isPower())
	{
		int iPowerMod = city.getPowerYieldRateModifier(eYieldType);
		if (0 != iPowerMod)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_YIELD_POWER", iPowerMod, info.getChar()));
			szBuffer.append(NEWLINE);
			iBaseModifier += iPowerMod;
		}
	}

	// Resources
	int iBonusMod = city.getBonusYieldRateModifier(eYieldType);
	if (0 != iBonusMod)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_YIELD_BONUS", iBonusMod, info.getChar()));
		szBuffer.append(NEWLINE);
		iBaseModifier += iBonusMod;
	}

	// Capital
	if (city.isCapital())
	{
		int iCapitalMod = owner.getCapitalYieldRateModifier(eYieldType);
		if (0 != iCapitalMod)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_YIELD_CAPITAL", iCapitalMod, info.getChar()));
			szBuffer.append(NEWLINE);
			iBaseModifier += iCapitalMod;
		}
	}

	// Civics
	int iCivicMod = 0;
	for (int i = 0; i < GC.getNumCivicOptionInfos(); i++)
	{
		if (NO_CIVIC != owner.getCivics((CivicOptionTypes)i))
		{
			iCivicMod += GC.getCivicInfo(owner.getCivics((CivicOptionTypes)i)).getYieldModifier(eYieldType);
		}
	}
	if (0 != iCivicMod)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_YIELD_CIVICS", iCivicMod, info.getChar()));
		szBuffer.append(NEWLINE);
		iBaseModifier += iCivicMod;
	}
}

void CvGameTextMgr::setConvertHelp(CvWStringBuffer& szBuffer, PlayerTypes ePlayer, ReligionTypes eReligion)
{
	CvWString szReligion = L"TXT_KEY_MISC_NO_STATE_RELIGION";

	if (eReligion != NO_RELIGION)
	{
		szReligion = GC.getReligionInfo(eReligion).getTextKeyWide();
	}

	szBuffer.assign(gDLL->getText("TXT_KEY_MISC_CANNOT_CONVERT_TO", szReligion.GetCString()));

	if (GET_PLAYER(ePlayer).isAnarchy())
	{
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_WHILE_IN_ANARCHY"));
	}
	else if (GET_PLAYER(ePlayer).getStateReligion() == eReligion)
	{
		szBuffer.append(L". ");
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_ALREADY_STATE_REL"));
	}
	else if (GET_PLAYER(ePlayer).getConversionTimer() > 0)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_ANOTHER_REVOLUTION_RECENTLY"));
		szBuffer.append(L". ");
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_WAIT_MORE_TURNS", GET_PLAYER(ePlayer).getConversionTimer()));
	}
}

void CvGameTextMgr::setRevolutionHelp(CvWStringBuffer& szBuffer, PlayerTypes ePlayer)
{
	szBuffer.assign(gDLL->getText("TXT_KEY_MISC_CANNOT_CHANGE_CIVICS"));

	if (GET_PLAYER(ePlayer).isAnarchy())
	{
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_WHILE_IN_ANARCHY"));
	}
	else if (GET_PLAYER(ePlayer).getRevolutionTimer() > 0)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_ANOTHER_REVOLUTION_RECENTLY"));
		szBuffer.append(L" : ");
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_WAIT_MORE_TURNS", GET_PLAYER(ePlayer).getRevolutionTimer()));
	}
}

void CvGameTextMgr::setVassalRevoltHelp(CvWStringBuffer& szBuffer, TeamTypes eMaster, TeamTypes eVassal)
{
	if (NO_TEAM == eMaster || NO_TEAM == eVassal)
	{
		return;
	}

	if (!GET_TEAM(eVassal).isCapitulated())
	{
		return;
	}

	if (GET_TEAM(eMaster).isParent(eVassal))
	{
		return;
	}

	CvTeam& kMaster = GET_TEAM(eMaster);
	CvTeam& kVassal = GET_TEAM(eVassal);

	int iMasterLand = kMaster.getTotalLand(false);
	int iVassalLand = kVassal.getTotalLand(false);
	if (iMasterLand > 0 && GC.getDefineINT("FREE_VASSAL_LAND_PERCENT") >= 0)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_VASSAL_LAND_STATS", (iVassalLand * 100) / iMasterLand, GC.getDefineINT("FREE_VASSAL_LAND_PERCENT")));
	}

	int iMasterPop = kMaster.getTotalPopulation(false);
	int iVassalPop = kVassal.getTotalPopulation(false);
	if (iMasterPop > 0 && GC.getDefineINT("FREE_VASSAL_POPULATION_PERCENT") >= 0)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_VASSAL_POPULATION_STATS", (iVassalPop * 100) / iMasterPop, GC.getDefineINT("FREE_VASSAL_POPULATION_PERCENT")));
	}

	if (GC.getDefineINT("VASSAL_REVOLT_OWN_LOSSES_FACTOR") > 0 && kVassal.getVassalPower() > 0)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_VASSAL_AREA_LOSS", (iVassalLand * 100) / kVassal.getVassalPower(), GC.getDefineINT("VASSAL_REVOLT_OWN_LOSSES_FACTOR")));
	}

	if (GC.getDefineINT("VASSAL_REVOLT_MASTER_LOSSES_FACTOR") > 0 && kVassal.getMasterPower() > 0)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_MASTER_AREA_LOSS", (iMasterLand * 100) / kVassal.getMasterPower(), GC.getDefineINT("VASSAL_REVOLT_MASTER_LOSSES_FACTOR")));
	}
}

void CvGameTextMgr::parseGreatPeopleHelp(CvWStringBuffer &szBuffer, CvCity& city)
{
	int iTotalGreatPeopleUnitProgress;
	int iI;

	if (NO_PLAYER == city.getOwnerINLINE())
	{
		return;
	}
	CvPlayer& owner = GET_PLAYER(city.getOwnerINLINE());

	szBuffer.assign(gDLL->getText("TXT_KEY_MISC_GREAT_PERSON", city.getGreatPeopleProgress(), owner.greatPeopleThreshold(false)));

	if (city.getGreatPeopleRate() > 0)
	{
		int iGPPLeft = owner.greatPeopleThreshold(false) - city.getGreatPeopleProgress();

		if (iGPPLeft > 0)
		{
			int iTurnsLeft = iGPPLeft * 100 / city.getGreatPeopleRate();

			if (iTurnsLeft * city.getGreatPeopleRate() / 100 <  iGPPLeft)
			{
				iTurnsLeft++;
			}

			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("INTERFACE_CITY_TURNS", std::max(1, iTurnsLeft)));
		}
	}

	iTotalGreatPeopleUnitProgress = 0;

	for (iI = 0; iI < GC.getNumUnitInfos(); ++iI)
	{
		iTotalGreatPeopleUnitProgress += city.getGreatPeopleUnitProgress((UnitTypes)iI);
	}

	if (iTotalGreatPeopleUnitProgress > 0)
	{
		szBuffer.append(SEPARATOR);
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_PROB"));

		std::vector< std::pair<UnitTypes, int> > aUnitProgress;
		int iTotalTruncated = 0;
		for (iI = 0; iI < GC.getNumUnitInfos(); ++iI)
		{
			int iProgress = ((city.getGreatPeopleUnitProgress((UnitTypes)iI) * 100) / iTotalGreatPeopleUnitProgress);
			if (iProgress > 0)
			{
				iTotalTruncated += iProgress;
				aUnitProgress.push_back(std::make_pair((UnitTypes)iI, iProgress));
			}
		}

		if (iTotalTruncated < 100 && aUnitProgress.size() > 0)
		{
			aUnitProgress[0].second += 100 - iTotalTruncated;
		}

		for (iI = 0; iI < (int)aUnitProgress.size(); ++iI)
		{
			szBuffer.append(CvWString::format(L"%s%s - %d%%", NEWLINE, GC.getUnitInfo(aUnitProgress[iI].first).getDescription(), aUnitProgress[iI].second));
		}
	}

	//if (city.getGreatPeopleRate() == 0)
// BUG - Building Additional Great People - start
	bool bBuildingAdditionalGreatPeople = getBugOptionBOOL("MiscHover__BuildingAdditionalGreatPeople", false, "BUG_BUILDING_ADDITIONAL_GREAT_PEOPLE_HOVER");
	if (city.getGreatPeopleRate() == 0 && !bBuildingAdditionalGreatPeople)
// BUG - Building Additional Great People - end
	{
		return;
	}

	szBuffer.append(SEPARATOR);
	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_GREATPEOPLE_BASE_RATE", city.getBaseGreatPeopleRate()));
	szBuffer.append(NEWLINE);

	int iModifier = 100;

	// Buildings
	int iBuildingMod = 0;
	for (int i = 0; i < GC.getNumBuildingInfos(); i++)
	{
		CvBuildingInfo& infoBuilding = GC.getBuildingInfo((BuildingTypes)i);
		// MOD - START - Building Replacement
		//if (city.getNumBuilding((BuildingTypes)i) > 0 && !GET_TEAM(city.getTeam()).isObsoleteBuilding((BuildingTypes)i))
		if (city.getNumActiveBuilding((BuildingTypes)i) > 0)
		// MOD - END - Building Replacement
		{
			for (int iLoop = 0; iLoop < city.getNumBuilding((BuildingTypes)i); iLoop++)
			{
				iBuildingMod += infoBuilding.getGreatPeopleRateModifier();
			}
		}
		for (int j = 0; j < MAX_PLAYERS; j++)
		{
			if (GET_PLAYER((PlayerTypes)j).isAlive())
			{
				if (GET_PLAYER((PlayerTypes)j).getTeam() == owner.getTeam())
				{
					int iLoop;
					for (CvCity* pLoopCity = GET_PLAYER((PlayerTypes)j).firstCity(&iLoop); pLoopCity != NULL; pLoopCity = GET_PLAYER((PlayerTypes)j).nextCity(&iLoop))
					{
						// MOD - START - Building Replacement
						if (pLoopCity->getNumActiveBuilding((BuildingTypes)i) > 0)
						// MOD - END - Building Replacement
						{
							for (int iLoop = 0; iLoop < pLoopCity->getNumBuilding((BuildingTypes)i); iLoop++)
							{
								iBuildingMod += infoBuilding.getGlobalGreatPeopleRateModifier();
							}
						}
					}
				}
			}
		}
	}
	if (0 != iBuildingMod)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_GREATPEOPLE_BUILDINGS", iBuildingMod));
		szBuffer.append(NEWLINE);
		iModifier += iBuildingMod;
	}

	// Civics
	int iCivicMod = 0;
	for (int i = 0; i < GC.getNumCivicOptionInfos(); i++)
	{
		if (NO_CIVIC != owner.getCivics((CivicOptionTypes)i))
		{
			iCivicMod += GC.getCivicInfo(owner.getCivics((CivicOptionTypes)i)).getGreatPeopleRateModifier();
			if (owner.getStateReligion() != NO_RELIGION && city.isHasReligion(owner.getStateReligion()))
			{
				iCivicMod += GC.getCivicInfo(owner.getCivics((CivicOptionTypes)i)).getStateReligionGreatPeopleRateModifier();
			}
		}
	}
	if (0 != iCivicMod)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_GREATPEOPLE_CIVICS", iCivicMod));
		szBuffer.append(NEWLINE);
		iModifier += iCivicMod;
	}

	// Trait
	for (int i = 0; i < GC.getNumTraitInfos(); i++)
	{
		if (city.hasTrait((TraitTypes)i))
		{
			CvTraitInfo& trait = GC.getTraitInfo((TraitTypes)i);
			int iTraitMod = trait.getGreatPeopleRateModifier();
			if (0 != iTraitMod)
			{
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_GREATPEOPLE_TRAIT", iTraitMod, trait.getTextKeyWide()));
				szBuffer.append(NEWLINE);
				iModifier += iTraitMod;
			}
		}
	}

	if (owner.isGoldenAge())
	{
		int iGoldenAgeMod = GC.getDefineINT("GOLDEN_AGE_GREAT_PEOPLE_MODIFIER");

		if (0 != iGoldenAgeMod)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_GREATPEOPLE_GOLDEN_AGE", iGoldenAgeMod));
			szBuffer.append(NEWLINE);
			iModifier += iGoldenAgeMod;
		}
	}

	int fModGreatPeople = (iModifier * city.getBaseGreatPeopleRate());

	FAssertMsg(fModGreatPeople == city.getGreatPeopleRate(), "Great person rate does not match actual value");

	CvWString szGP = CvWString::format(L"%d.%02d", fModGreatPeople/100, fModGreatPeople%100);
	szBuffer.append(gDLL->getText("TXT_KEY_MISC_HELP_GREATPEOPLE_FINAL", szGP.GetCString()));

	//szBuffer.append(CvWString::format(L"%f",gDLL->getText("TXT_KEY_MISC_HELP_GREATPEOPLE_FINAL", fModGreatPeople));
	//szBuffer.append(NEWLINE);
// BUG - Building Additional Great People - start
	if (bBuildingAdditionalGreatPeople && city.getOwnerINLINE() == GC.getGame().getActivePlayer())
	{
		setBuildingAdditionalGreatPeopleHelp(szBuffer, city, DOUBLE_SEPARATOR);
	}
// BUG - Building Additional Great People - end
}

// BUG - Building Additional Great People - start
bool CvGameTextMgr::setBuildingAdditionalGreatPeopleHelp(CvWStringBuffer &szBuffer, const CvCity& city, const CvWString& szStart, bool bStarted)
{
	CvWString szLabel;
	CvCivilizationInfo& kCivilization = GC.getCivilizationInfo(GET_PLAYER(city.getOwnerINLINE()).getCivilizationType());

	for (int iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
	{
		BuildingTypes eBuilding = (BuildingTypes)kCivilization.getCivilizationBuildings((BuildingClassTypes)iI);

		if (eBuilding != NO_BUILDING && city.canConstruct(eBuilding, false, true, false))
		{
			int iChange = city.getAdditionalGreatPeopleRateByBuilding(eBuilding);
			
			if (iChange != 0)
			{
				if (!bStarted)
				{
					szBuffer.append(szStart);
					bStarted = true;
				}

				szLabel.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR("COLOR_BUILDING_TEXT"), GC.getBuildingInfo(eBuilding).getDescription());
				setResumableValueChangeHelp(szBuffer, szLabel, L": ", L"", iChange, gDLL->getSymbolID(GREAT_PEOPLE_CHAR), false, true);
			}
		}
	}

	return bStarted;
}
// BUG - Building Additional Great People - end


void CvGameTextMgr::parseGreatGeneralHelp(CvWStringBuffer &szBuffer, CvPlayer& kPlayer)
{
	szBuffer.assign(gDLL->getText("TXT_KEY_MISC_GREAT_GENERAL", kPlayer.getCombatExperience(), kPlayer.greatPeopleThreshold(true)));
}


//------------------------------------------------------------------------------------------------

void CvGameTextMgr::buildCityBillboardIconString( CvWStringBuffer& szBuffer, CvCity* pCity)
{
	szBuffer.clear();

	// government center icon
	if (pCity->isGovernmentCenter() && !(pCity->isCapital()))
	{
		szBuffer.append(CvWString::format(L"%c", gDLL->getSymbolID(SILVER_STAR_CHAR)));
	}

	// happiness, healthiness, superlative icons
	if (pCity->canBeSelected())
	{
		if (pCity->angryPopulation() > 0)
		{
			szBuffer.append(CvWString::format(L"%c", gDLL->getSymbolID(UNHAPPY_CHAR)));
		}

		if (pCity->healthRate() < 0)
		{
			szBuffer.append(CvWString::format(L"%c", gDLL->getSymbolID(UNHEALTHY_CHAR)));
		}
		
		if (pCity->getEpidemicProbability() > 0)
		{
			szBuffer.append(CvWString::format(L"%c", gDLL->getSymbolID(EPIDEMIC_START_CHAR)));
		}

		if (gDLL->getGraphicOption(GRAPHICOPTION_CITY_DETAIL))
		{
			if (GET_PLAYER(pCity->getOwnerINLINE()).getNumCities() > 2)
			{
				if (pCity->findYieldRateRank(YIELD_PRODUCTION) == 1)
				{
					szBuffer.append(CvWString::format(L"%c", GC.getYieldInfo(YIELD_PRODUCTION).getChar()));
				}
				if (pCity->findCommerceRateRank(COMMERCE_GOLD) == 1)
				{
					szBuffer.append(CvWString::format(L"%c", GC.getCommerceInfo(COMMERCE_GOLD).getChar()));
				}
				if (pCity->findCommerceRateRank(COMMERCE_RESEARCH) == 1)
				{
					szBuffer.append(CvWString::format(L"%c", GC.getCommerceInfo(COMMERCE_RESEARCH).getChar()));
				}
			}
		}

		if (pCity->isConnectedToCapital())
		{
			if (GET_PLAYER(pCity->getOwnerINLINE()).countNumCitiesConnectedToCapital() > 1)
			{
				szBuffer.append(CvWString::format(L"%c", gDLL->getSymbolID(TRADE_CHAR)));
			}
		}
	}

	// religion icons
	for (int iI = 0; iI < GC.getNumReligionInfos(); ++iI)
	{
		if (pCity->isHasReligion((ReligionTypes)iI))
		{
			if (pCity->isHolyCity((ReligionTypes)iI))
			{
				szBuffer.append(CvWString::format(L"%c", GC.getReligionInfo((ReligionTypes) iI).getHolyCityChar()));
			}
			else
			{
				szBuffer.append(CvWString::format(L"%c", GC.getReligionInfo((ReligionTypes) iI).getChar()));
			}
		}
	}

	// corporation icons
	for (int iI = 0; iI < GC.getNumCorporationInfos(); ++iI)
	{
		if (pCity->isHeadquarters((CorporationTypes)iI))
		{
			if (pCity->isHasCorporation((CorporationTypes)iI))
			{
				szBuffer.append(CvWString::format(L"%c", GC.getCorporationInfo((CorporationTypes) iI).getHeadquarterChar()));
			}
		}
		else
		{
			if (pCity->isActiveCorporation((CorporationTypes)iI))
			{
				szBuffer.append(CvWString::format(L"%c", GC.getCorporationInfo((CorporationTypes) iI).getChar()));
			}
		}
	}

	if (pCity->getTeam() == GC.getGameINLINE().getActiveTeam())
	{
		if (pCity->isPower())
		{
			szBuffer.append(CvWString::format(L"%c", gDLL->getSymbolID(POWER_CHAR)));
		}
	}

	BuildingTypes eRiot = (BuildingTypes)GC.getInfoTypeForString("BUILDING_UNREST");
	int iSeparatism = pCity->getCitySeparatism();

	if (iSeparatism < -100)
	{
		szBuffer.append(CvWString::format(L"%c", gDLL->getSymbolID(SEPARATISM_GREEN_CHAR)));
	}

	if ((iSeparatism > 0) && (iSeparatism < 10) && (pCity->getNumRealBuilding(eRiot) < 1))
	{
		szBuffer.append(CvWString::format(L"%c", gDLL->getSymbolID(SEPARATISM_YELLOW_CHAR)));
	}
	
	if ((iSeparatism >= 10) && (pCity->getNumRealBuilding(eRiot) < 1))
	{
		szBuffer.append(CvWString::format(L"%c", gDLL->getSymbolID(SEPARATISM_ORANGE_CHAR)));
	}

	if (pCity->getNumRealBuilding(eRiot) > 0)
	{
		szBuffer.append(CvWString::format(L"%c", gDLL->getSymbolID(SEPARATISM_CHAR)));
	}

	// XXX out this in bottom bar???
	if (pCity->isOccupation())
	{
		szBuffer.append(CvWString::format(L" (%c:%d)", gDLL->getSymbolID(OCCUPATION_CHAR), pCity->getOccupationTimer()));
	}

	// defense icon and text
	//if (pCity->getTeam() != GC.getGameINLINE().getActiveTeam())
	{
		if (pCity->isVisible(GC.getGameINLINE().getActiveTeam(), true))
		{
			int iDefenseModifier = pCity->getDefenseModifier(GC.getGameINLINE().selectionListIgnoreBuildingDefense());

			if (iDefenseModifier != 0)
			{
				szBuffer.append(CvWString::format(L" %c:%s%d%%", gDLL->getSymbolID(DEFENSE_CHAR), ((iDefenseModifier > 0) ? "+" : ""), iDefenseModifier));
			}
		}
	}
}

void CvGameTextMgr::buildCityBillboardCityNameString( CvWStringBuffer& szBuffer, CvCity* pCity)
{
	szBuffer.assign(pCity->getName());

	if (pCity->canBeSelected())
	{
		if (gDLL->getGraphicOption(GRAPHICOPTION_CITY_DETAIL))
		{
			if (pCity->foodDifference() > 0)
			{
				int iTurns = pCity->getFoodTurnsLeft();

				// MOD - START - City Growth Limits
				//if ((iTurns > 1) || !(pCity->AI_isEmphasizeAvoidGrowth()))
				if ((iTurns > 1) || pCity->isGrowthAllowed())
				// MOD - END - City Growth Limits
				{
					if (iTurns < MAX_INT)
					{
						szBuffer.append(CvWString::format(L" (%d)", iTurns));
					}
				}
			}
		}
	}
}

void CvGameTextMgr::buildCityBillboardProductionString( CvWStringBuffer& szBuffer, CvCity* pCity)
{
	if (pCity->getOrderQueueLength() > 0)
	{
		szBuffer.assign(pCity->getProductionName());

		if (gDLL->getGraphicOption(GRAPHICOPTION_CITY_DETAIL))
		{
			int iTurns = pCity->getProductionTurnsLeft();

			if (iTurns < MAX_INT)
			{
				szBuffer.append(CvWString::format(L" (%d)", iTurns));
			}
		}
	}
	else
	{
		szBuffer.clear();
	}
}


void CvGameTextMgr::buildCityBillboardCitySizeString( CvWStringBuffer& szBuffer, CvCity* pCity, const NiColorA& kColor)
{
#define CAPARAMS(c) (int)((c).r * 255.0f), (int)((c).g * 255.0f), (int)((c).b * 255.0f), (int)((c).a * 255.0f)
	szBuffer.assign(CvWString::format(SETCOLR L"%d" ENDCOLR, CAPARAMS(kColor), pCity->getPopulation()));
#undef CAPARAMS
}

void CvGameTextMgr::getCityBillboardFoodbarColors(CvCity* pCity, std::vector<NiColorA>& aColors)
{
	aColors.resize(NUM_INFOBAR_TYPES);
	aColors[INFOBAR_STORED] = GC.getColorInfo((ColorTypes)(GC.getYieldInfo(YIELD_FOOD).getColorType())).getColor();
	aColors[INFOBAR_RATE] = aColors[INFOBAR_STORED];
	aColors[INFOBAR_RATE].a = 0.5f;
	aColors[INFOBAR_RATE_EXTRA] = GC.getColorInfo((ColorTypes)GC.getInfoTypeForString("COLOR_NEGATIVE_RATE")).getColor();
	aColors[INFOBAR_EMPTY] = GC.getColorInfo((ColorTypes)GC.getInfoTypeForString("COLOR_EMPTY")).getColor();
}

void CvGameTextMgr::getCityBillboardProductionbarColors(CvCity* pCity, std::vector<NiColorA>& aColors)
{
	aColors.resize(NUM_INFOBAR_TYPES);
	aColors[INFOBAR_STORED] = GC.getColorInfo((ColorTypes)(GC.getYieldInfo(YIELD_PRODUCTION).getColorType())).getColor();
	aColors[INFOBAR_RATE] = aColors[INFOBAR_STORED];
	aColors[INFOBAR_RATE].a = 0.5f;
	aColors[INFOBAR_RATE_EXTRA] = GC.getColorInfo((ColorTypes)(GC.getYieldInfo(YIELD_FOOD).getColorType())).getColor();
	aColors[INFOBAR_RATE_EXTRA].a = 0.5f;
	aColors[INFOBAR_EMPTY] = GC.getColorInfo((ColorTypes)GC.getInfoTypeForString("COLOR_EMPTY")).getColor();
}


void CvGameTextMgr::setScoreHelp(CvWStringBuffer &szString, PlayerTypes ePlayer)
{
	if (NO_PLAYER != ePlayer)
	{
		CvPlayer& player = GET_PLAYER(ePlayer);

		// MOD - START - GameCore Scoring
		/*
		int iPop = player.getPopScore();
		int iMaxPop = GC.getGameINLINE().getMaxPopulation();
		int iPopScore = 0;
		if (iMaxPop > 0)
		{
			iPopScore = (GC.getDefineINT("SCORE_POPULATION_FACTOR") * iPop) / iMaxPop;
		}
		int iLand = player.getLandScore();
		int iMaxLand = GC.getGameINLINE().getMaxLand();
		int iLandScore = 0;
		if (iMaxLand > 0)
		{
			iLandScore = (GC.getDefineINT("SCORE_LAND_FACTOR") * iLand) / iMaxLand;
		}
		int iTech = player.getTechScore();
		int iMaxTech = GC.getGameINLINE().getMaxTech();
		*/
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      02/24/10                                jdog5000      */
/*                                                                                              */
/* Bugfix                                                                                       */
/************************************************************************************************/
		/*
		int iTechScore = 0;
		if( iMaxTech > 0 )
		{
			iTechScore = (GC.getDefineINT("SCORE_TECH_FACTOR") * iTech) / iMaxTech;
		}
		int iWonders = player.getWondersScore();
		int iMaxWonders = GC.getGameINLINE().getMaxWonders();
		int iWondersScore = 0;
		if( iMaxWonders > 0 )
		{
			iWondersScore = (GC.getDefineINT("SCORE_WONDER_FACTOR") * iWonders) / iMaxWonders;
		}
		*/
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/
		/*
		int iTotalScore = iPopScore + iLandScore + iTechScore + iWondersScore;
		int iVictoryScore = player.calculateScore(true, true);
		if (iTotalScore == player.calculateScore())
		{
			szString.append(gDLL->getText("TXT_KEY_SCORE_BREAKDOWN", iPopScore, iPop, iMaxPop, iLandScore, iLand, iMaxLand, iTechScore, iTech, iMaxTech, iWondersScore, iWonders, iMaxWonders, iTotalScore, iVictoryScore));
		}
		*/

		int iPopulation = player.getPopScore();
		int iInitPopulation = GC.getGameINLINE().getInitPopulation();
		int iMaxPopulation = GC.getGameINLINE().getMaxPopulation();
		int iPopulationScore = player.getScoreComponent(iPopulation, iInitPopulation, iMaxPopulation, GC.getDefineINT("SCORE_POPULATION_FACTOR"), true, false, false);

		int iLand = player.getLandScore();
		int iInitLand = GC.getGameINLINE().getInitLand();
		int iMaxLand = GC.getGameINLINE().getMaxLand();
		int iLandScore = player.getScoreComponent(iLand, iInitLand, iMaxLand, GC.getDefineINT("SCORE_LAND_FACTOR"), true, false, false);

		int iTech = player.getTechScore();
		int iInitTech = GC.getGameINLINE().getInitTech();
		int iMaxTech = GC.getGameINLINE().getMaxTech();
		int iTechScore = player.getScoreComponent(iTech, iInitTech, iMaxTech, GC.getDefineINT("SCORE_TECH_FACTOR"), true, false, false);

		int iWonders = player.getWondersScore();
		int iInitWonders = GC.getGameINLINE().getInitWonders();
		int iMaxWonders = GC.getGameINLINE().getMaxWonders();
		int iWondersScore = player.getScoreComponent(iWonders, iInitWonders, iMaxWonders, GC.getDefineINT("SCORE_WONDER_FACTOR"), false, false, false);

		int iTotalScore = player.calculateScore();
		int iVictoryScore = player.calculateScore(true, true);

		// MOD - START - Handicap Score Modifier
		int iHandicapModifier = player.getHandicapScoreModifier();
		if (iHandicapModifier == 0)
		{
			szString.append(gDLL->getText("TXT_KEY_SCORE_BREAKDOWN", iPopulationScore, iPopulation, iMaxPopulation, iLandScore, iLand, iMaxLand, iTechScore, iTech, iMaxTech, iWondersScore, iWonders, iMaxWonders, iTotalScore, iVictoryScore));
		}
		else if (iHandicapModifier > 0)
		{
			szString.append(gDLL->getText("TXT_KEY_SCORE_BREAKDOWN_WITH_POSITIVE_MODIFIERS", iPopulationScore, iPopulation, iMaxPopulation, iLandScore, iLand, iMaxLand, iTechScore, iTech, iMaxTech, iWondersScore, iWonders, iMaxWonders, iHandicapModifier, iTotalScore, iVictoryScore));
		}
		else if (iHandicapModifier < 0)
		{
			szString.append(gDLL->getText("TXT_KEY_SCORE_BREAKDOWN_WITH_NEGATIVE_MODIFIERS", iPopulationScore, iPopulation, iMaxPopulation, iLandScore, iLand, iMaxLand, iTechScore, iTech, iMaxTech, iWondersScore, iWonders, iMaxWonders, iHandicapModifier, iTotalScore, iVictoryScore));
		}
		// MOD - END - Handicap Score Modifier
		// MOD - END - GameCore Scoring
	}
}

void CvGameTextMgr::setEventHelp(CvWStringBuffer& szBuffer, EventTypes eEvent, int iEventTriggeredId, PlayerTypes ePlayer)
{
	if (NO_EVENT == eEvent || NO_PLAYER == ePlayer)
	{
		return;
	}

	CvEventInfo& kEvent = GC.getEventInfo(eEvent);
	CvPlayer& kActivePlayer = GET_PLAYER(ePlayer);
	EventTriggeredData* pTriggeredData = kActivePlayer.getEventTriggered(iEventTriggeredId);

	if (NULL == pTriggeredData)
	{
		return;
	}

	CvCity* pCity = kActivePlayer.getCity(pTriggeredData->m_iCityId);
	CvCity* pOtherPlayerCity = NULL;
	CvPlot* pPlot = GC.getMapINLINE().plot(pTriggeredData->m_iPlotX, pTriggeredData->m_iPlotY);
	CvUnit* pUnit = kActivePlayer.getUnit(pTriggeredData->m_iUnitId);

	if (NO_PLAYER != pTriggeredData->m_eOtherPlayer)
	{
		pOtherPlayerCity = GET_PLAYER(pTriggeredData->m_eOtherPlayer).getCity(pTriggeredData->m_iOtherPlayerCityId);
	}

	CvWString szCity = gDLL->getText("TXT_KEY_EVENT_THE_CITY");
	if (NULL != pCity && kEvent.isCityEffect())
	{
		szCity = pCity->getNameKey();
	}
	else if (NULL != pOtherPlayerCity && kEvent.isOtherPlayerCityEffect())
	{
		szCity = pOtherPlayerCity->getNameKey();
	}

	CvWString szUnit = gDLL->getText("TXT_KEY_EVENT_THE_UNIT");
	if (NULL != pUnit)
	{
		szUnit = pUnit->getNameKey();
	}

	CvWString szReligion = gDLL->getText("TXT_KEY_EVENT_THE_RELIGION");
	if (NO_RELIGION != pTriggeredData->m_eReligion)
	{
		szReligion = GC.getReligionInfo(pTriggeredData->m_eReligion).getTextKeyWide();
	}

	eventGoldHelp(szBuffer, eEvent, ePlayer, pTriggeredData->m_eOtherPlayer);

	eventTechHelp(szBuffer, eEvent, kActivePlayer.getBestEventTech(eEvent, pTriggeredData->m_eOtherPlayer), ePlayer, pTriggeredData->m_eOtherPlayer);

	if (NO_PLAYER != pTriggeredData->m_eOtherPlayer && NO_BONUS != kEvent.getBonusGift())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_EVENT_GIFT_BONUS_TO_PLAYER", GC.getBonusInfo((BonusTypes)kEvent.getBonusGift()).getTextKeyWide(), GET_PLAYER(pTriggeredData->m_eOtherPlayer).getNameKey()));
	}

	if (kEvent.getHappy() != 0)
	{
		if (NO_PLAYER != pTriggeredData->m_eOtherPlayer)
		{
			if (kEvent.getHappy() > 0)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_EVENT_HAPPY_FROM_PLAYER", kEvent.getHappy(), kEvent.getHappy(), GET_PLAYER(pTriggeredData->m_eOtherPlayer).getNameKey()));
			}
			else
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_EVENT_HAPPY_TO_PLAYER", -kEvent.getHappy(), -kEvent.getHappy(), GET_PLAYER(pTriggeredData->m_eOtherPlayer).getNameKey()));
			}
		}
		else
		{
			if (kEvent.getHappy() > 0)
			{
				if (kEvent.isCityEffect() || kEvent.isOtherPlayerCityEffect())
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_EVENT_HAPPY_CITY", kEvent.getHappy(), szCity.GetCString()));
				}
				else
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_EVENT_HAPPY", kEvent.getHappy()));
				}
			}
			else
			{
				if (kEvent.isCityEffect() || kEvent.isOtherPlayerCityEffect())
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_EVENT_UNHAPPY_CITY", -kEvent.getHappy(), szCity.GetCString()));
				}
				else
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_EVENT_UNHAPPY", -kEvent.getHappy()));
				}
			}
		}
	}

	if (kEvent.getHealth() != 0)
	{
		if (NO_PLAYER != pTriggeredData->m_eOtherPlayer)
		{
			if (kEvent.getHealth() > 0)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_EVENT_HEALTH_FROM_PLAYER", kEvent.getHealth(), kEvent.getHealth(), GET_PLAYER(pTriggeredData->m_eOtherPlayer).getNameKey()));
			}
			else
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_EVENT_HEALTH_TO_PLAYER", -kEvent.getHealth(), -kEvent.getHealth(), GET_PLAYER(pTriggeredData->m_eOtherPlayer).getNameKey()));
			}
		}
		else
		{
			if (kEvent.getHealth() > 0)
			{
				if (kEvent.isCityEffect() || kEvent.isOtherPlayerCityEffect())
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_EVENT_HEALTH_CITY", kEvent.getHealth(), szCity.GetCString()));
				}
				else
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_EVENT_HEALTH", kEvent.getHealth()));
				}
			}
			else
			{
				if (kEvent.isCityEffect() || kEvent.isOtherPlayerCityEffect())
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_EVENT_UNHEALTH", -kEvent.getHealth(), szCity.GetCString()));
				}
				else
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_EVENT_UNHEALTH_CITY", -kEvent.getHealth()));
				}
			}
		}
	}

	if (kEvent.getHurryAnger() != 0)
	{
		if (kEvent.isCityEffect() || kEvent.isOtherPlayerCityEffect())
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_EVENT_HURRY_ANGER_CITY", kEvent.getHurryAnger(), szCity.GetCString()));
		}
		else
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_EVENT_HURRY_ANGER", kEvent.getHurryAnger()));
		}
	}

	if (kEvent.getHappyTurns() > 0)
	{
		if (kEvent.isCityEffect() || kEvent.isOtherPlayerCityEffect())
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_EVENT_TEMP_HAPPY_CITY", GC.getDefineINT("TEMP_HAPPY"), kEvent.getHappyTurns(), szCity.GetCString()));
		}
		else
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_EVENT_TEMP_HAPPY", GC.getDefineINT("TEMP_HAPPY"), kEvent.getHappyTurns()));
		}
	}

	if (kEvent.getFood() != 0)
	{
		// MOD - START - Game Speed Event Scaling
		int iFood = kActivePlayer.getEventFood(eEvent);

		if (kEvent.isCityEffect() || kEvent.isOtherPlayerCityEffect())
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_EVENT_FOOD_CITY", iFood, szCity.GetCString()));
		}
		else
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_EVENT_FOOD", iFood));
		}
		// MOD - END - Game Speed Event Scaling
	}

	if (kEvent.getFoodPercent() != 0)
	{
		// MOD - START - Game Speed Event Scaling
		int iFoodPercent = kActivePlayer.getEventFoodPercent(eEvent);

		if (kEvent.isCityEffect() || kEvent.isOtherPlayerCityEffect())
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_EVENT_FOOD_PERCENT_CITY", iFoodPercent, szCity.GetCString()));
		}
		else
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_EVENT_FOOD_PERCENT", iFoodPercent));
		}
		// MOD - END - Game Speed Event Scaling
	}

	if (kEvent.getRevoltTurns() > 0)
	{
		if (kEvent.isCityEffect() || kEvent.isOtherPlayerCityEffect())
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_EVENT_REVOLT_TURNS", kEvent.getRevoltTurns(), szCity.GetCString()));
		}
	}

	if (0 != kEvent.getSpaceProductionModifier())
	{
		if (kEvent.isCityEffect() || kEvent.isOtherPlayerCityEffect())
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_EVENT_SPACE_PRODUCTION_CITY", kEvent.getSpaceProductionModifier(), szCity.GetCString()));
		}
		else
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_SPACESHIP_MOD_ALL_CITIES", kEvent.getSpaceProductionModifier()));
		}
	}

	if (kEvent.getMaxPillage() > 0)
	{
		if (kEvent.isCityEffect() || kEvent.isOtherPlayerCityEffect())
		{
			if (kEvent.getMaxPillage() == kEvent.getMinPillage())
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_EVENT_PILLAGE_CITY", kEvent.getMinPillage(), szCity.GetCString()));
			}
			else
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_EVENT_PILLAGE_RANGE_CITY", kEvent.getMinPillage(), kEvent.getMaxPillage(), szCity.GetCString()));
			}
		}
		else
		{
			if (kEvent.getMaxPillage() == kEvent.getMinPillage())
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_EVENT_PILLAGE", kEvent.getMinPillage()));
			}
			else
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_EVENT_PILLAGE_RANGE", kEvent.getMinPillage(), kEvent.getMaxPillage()));
			}
		}
	}

	for (int i = 0; i < GC.getNumSpecialistInfos(); ++i)
	{
		if (kEvent.getFreeSpecialistCount(i) > 0)
		{
			if (kEvent.isCityEffect() || kEvent.isOtherPlayerCityEffect())
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_EVENT_FREE_SPECIALIST", kEvent.getFreeSpecialistCount(i), GC.getSpecialistInfo((SpecialistTypes)i).getTextKeyWide(), szCity.GetCString()));
			}
		}
	}

	if (kEvent.getPopulationChange() != 0)
	{
		if (kEvent.isCityEffect() || kEvent.isOtherPlayerCityEffect())
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_EVENT_POPULATION_CHANGE_CITY", kEvent.getPopulationChange(), szCity.GetCString()));
		}
		else
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_EVENT_POPULATION_CHANGE", kEvent.getPopulationChange()));
		}
	}

	if (kEvent.getCulture() != 0)
	{
		if (kEvent.isCityEffect() || kEvent.isOtherPlayerCityEffect())
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_EVENT_CULTURE_CITY", kEvent.getCulture(), szCity.GetCString()));
		}
		else
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_EVENT_CULTURE", kEvent.getCulture()));
		}
	}

	if (kEvent.getUnitClass() != NO_UNITCLASS)
	{
		CivilizationTypes eCiv = kActivePlayer.getCivilizationType();
		if (NO_CIVILIZATION != eCiv)
		{
			UnitTypes eUnit = (UnitTypes)GC.getCivilizationInfo(eCiv).getCivilizationUnits(kEvent.getUnitClass());
			if (eUnit != NO_UNIT)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_EVENT_BONUS_UNIT", kEvent.getNumUnits(), GC.getUnitInfo(eUnit).getTextKeyWide()));
			}
		}
	}

	if (kEvent.getBuildingClass() != NO_BUILDINGCLASS)
	{
		CivilizationTypes eCiv = kActivePlayer.getCivilizationType();
		if (NO_CIVILIZATION != eCiv)
		{
			BuildingTypes eBuilding = (BuildingTypes)GC.getCivilizationInfo(eCiv).getCivilizationBuildings(kEvent.getBuildingClass());
			if (eBuilding != NO_BUILDING)
			{
				if (kEvent.getBuildingChange() > 0)
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_EVENT_BONUS_BUILDING", GC.getBuildingInfo(eBuilding).getTextKeyWide()));
				}
				else if (kEvent.getBuildingChange() < 0)
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_EVENT_REMOVE_BUILDING", GC.getBuildingInfo(eBuilding).getTextKeyWide()));
				}
			}
		}
	}

	if (kEvent.getNumBuildingYieldChanges() > 0)
	{
		CvWStringBuffer szYield;
		for (int iBuildingClass = 0; iBuildingClass < GC.getNumBuildingClassInfos(); ++iBuildingClass)
		{
			CivilizationTypes eCiv = kActivePlayer.getCivilizationType();
			if (NO_CIVILIZATION != eCiv)
			{
				BuildingTypes eBuilding = (BuildingTypes)GC.getCivilizationInfo(eCiv).getCivilizationBuildings(iBuildingClass);
				if (eBuilding != NO_BUILDING)
				{
					int aiYields[NUM_YIELD_TYPES];
					for (int iYield = 0; iYield < NUM_YIELD_TYPES; ++iYield)
					{
						aiYields[iYield] = kEvent.getBuildingYieldChange(iBuildingClass, iYield);
					}

					szYield.clear();
					setYieldChangeHelp(szYield, L"", L"", L"", aiYields, false, false);
					if (!szYield.isEmpty())
					{
						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_EVENT_YIELD_CHANGE_BUILDING", GC.getBuildingInfo(eBuilding).getTextKeyWide(), szYield.getCString()));
					}
				}
			}
		}
	}

	if (kEvent.getNumBuildingCommerceChanges() > 0)
	{
		CvWStringBuffer szCommerce;
		for (int iBuildingClass = 0; iBuildingClass < GC.getNumBuildingClassInfos(); ++iBuildingClass)
		{
			CivilizationTypes eCiv = kActivePlayer.getCivilizationType();
			if (NO_CIVILIZATION != eCiv)
			{
				BuildingTypes eBuilding = (BuildingTypes)GC.getCivilizationInfo(eCiv).getCivilizationBuildings(iBuildingClass);
				if (eBuilding != NO_BUILDING)
				{
					int aiCommerces[NUM_COMMERCE_TYPES];
					for (int iCommerce = 0; iCommerce < NUM_COMMERCE_TYPES; ++iCommerce)
					{
						aiCommerces[iCommerce] = kEvent.getBuildingCommerceChange(iBuildingClass, iCommerce);
					}

					szCommerce.clear();
					setCommerceChangeHelp(szCommerce, L"", L"", L"", aiCommerces, false, false);
					if (!szCommerce.isEmpty())
					{
						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_EVENT_YIELD_CHANGE_BUILDING", GC.getBuildingInfo(eBuilding).getTextKeyWide(), szCommerce.getCString()));
					}
				}
			}
		}
	}

	if (kEvent.getNumBuildingHappyChanges() > 0)
	{
		for (int iBuildingClass = 0; iBuildingClass < GC.getNumBuildingClassInfos(); ++iBuildingClass)
		{
			CivilizationTypes eCiv = kActivePlayer.getCivilizationType();
			if (NO_CIVILIZATION != eCiv)
			{
				BuildingTypes eBuilding = (BuildingTypes)GC.getCivilizationInfo(eCiv).getCivilizationBuildings(iBuildingClass);
				if (eBuilding != NO_BUILDING)
				{
					int iHappy = kEvent.getBuildingHappyChange(iBuildingClass);
					if (iHappy > 0)
					{
						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_EVENT_HAPPY_BUILDING", GC.getBuildingInfo(eBuilding).getTextKeyWide(), iHappy, gDLL->getSymbolID(HAPPY_CHAR)));
					}
					else if (iHappy < 0)
					{
						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_EVENT_HAPPY_BUILDING", GC.getBuildingInfo(eBuilding).getTextKeyWide(), -iHappy, gDLL->getSymbolID(UNHAPPY_CHAR)));
					}
				}
			}
		}
	}

	if (kEvent.getNumBuildingHealthChanges() > 0)
	{
		for (int iBuildingClass = 0; iBuildingClass < GC.getNumBuildingClassInfos(); ++iBuildingClass)
		{
			CivilizationTypes eCiv = kActivePlayer.getCivilizationType();
			if (NO_CIVILIZATION != eCiv)
			{
				BuildingTypes eBuilding = (BuildingTypes)GC.getCivilizationInfo(eCiv).getCivilizationBuildings(iBuildingClass);
				if (eBuilding != NO_BUILDING)
				{
					int iHealth = kEvent.getBuildingHealthChange(iBuildingClass);
					if (iHealth > 0)
					{
						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_EVENT_HAPPY_BUILDING", GC.getBuildingInfo(eBuilding).getTextKeyWide(), iHealth, gDLL->getSymbolID(HEALTHY_CHAR)));
					}
					else if (iHealth < 0)
					{
						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_EVENT_HAPPY_BUILDING", GC.getBuildingInfo(eBuilding).getTextKeyWide(), -iHealth, gDLL->getSymbolID(UNHEALTHY_CHAR)));
					}
				}
			}
		}
	}

	if (kEvent.getFeatureChange() > 0)
	{
		if (kEvent.getFeature() != NO_FEATURE)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_EVENT_FEATURE_GROWTH", GC.getFeatureInfo((FeatureTypes)kEvent.getFeature()).getTextKeyWide()));
		}
	}
	else if (kEvent.getFeatureChange() < 0)
	{
		if (NULL != pPlot && NO_FEATURE != pPlot->getFeatureType())
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_EVENT_FEATURE_REMOVE", GC.getFeatureInfo(pPlot->getFeatureType()).getTextKeyWide()));
		}
	}

	if (kEvent.getImprovementChange() > 0)
	{
		if (kEvent.getImprovement() != NO_IMPROVEMENT)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_EVENT_IMPROVEMENT_GROWTH", GC.getImprovementInfo((ImprovementTypes)kEvent.getImprovement()).getTextKeyWide()));
		}
	}
	else if (kEvent.getImprovementChange() < 0)
	{
		if (NULL != pPlot && NO_IMPROVEMENT != pPlot->getImprovementType())
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_EVENT_IMPROVEMENT_REMOVE", GC.getImprovementInfo(pPlot->getImprovementType()).getTextKeyWide()));
		}
	}

	if (kEvent.getBonusChange() > 0)
	{
		if (kEvent.getBonus() != NO_BONUS)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_EVENT_BONUS_GROWTH", GC.getBonusInfo((BonusTypes)kEvent.getBonus()).getTextKeyWide()));
		}
	}
	else if (kEvent.getBonusChange() < 0)
	{
		if (NULL != pPlot && NO_BONUS != pPlot->getBonusType())
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_EVENT_BONUS_REMOVE", GC.getBonusInfo(pPlot->getBonusType()).getTextKeyWide()));
		}
	}

	if (kEvent.getRouteChange() > 0)
	{
		if (kEvent.getRoute() != NO_ROUTE)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_EVENT_ROUTE_GROWTH", GC.getRouteInfo((RouteTypes)kEvent.getRoute()).getTextKeyWide()));
		}
	}
	else if (kEvent.getRouteChange() < 0)
	{
		if (NULL != pPlot && NO_ROUTE != pPlot->getRouteType())
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_EVENT_ROUTE_REMOVE", GC.getRouteInfo(pPlot->getRouteType()).getTextKeyWide()));
		}
	}

	int aiYields[NUM_YIELD_TYPES];
	for (int i = 0; i < NUM_YIELD_TYPES; ++i)
	{
		aiYields[i] = kEvent.getPlotExtraYield(i);
	}

	CvWStringBuffer szYield;
	setYieldChangeHelp(szYield, L"", L"", L"", aiYields, false, false);
	if (!szYield.isEmpty())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_EVENT_YIELD_CHANGE_PLOT", szYield.getCString()));
	}

	if (NO_BONUS != kEvent.getBonusRevealed())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_EVENT_BONUS_REVEALED", GC.getBonusInfo((BonusTypes)kEvent.getBonusRevealed()).getTextKeyWide()));
	}

	if (0 != kEvent.getUnitExperience())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_EVENT_UNIT_EXPERIENCE", kEvent.getUnitExperience(), szUnit.GetCString()));
	}

	if (0 != kEvent.isDisbandUnit())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_EVENT_UNIT_DISBAND", szUnit.GetCString()));
	}

	if (NO_PROMOTION != kEvent.getUnitPromotion())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_EVENT_UNIT_PROMOTION", szUnit.GetCString(), GC.getPromotionInfo((PromotionTypes)kEvent.getUnitPromotion()).getTextKeyWide()));
	}

	for (int i = 0; i < GC.getNumUnitCombatInfos(); ++i)
	{
		if (NO_PROMOTION != kEvent.getUnitCombatPromotion(i))
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_EVENT_UNIT_COMBAT_PROMOTION", GC.getUnitCombatInfo((UnitCombatTypes)i).getTextKeyWide(), GC.getPromotionInfo((PromotionTypes)kEvent.getUnitCombatPromotion(i)).getTextKeyWide()));
		}
	}

	for (int i = 0; i < GC.getNumUnitClassInfos(); ++i)
	{
		if (NO_PROMOTION != kEvent.getUnitClassPromotion(i))
		{
			UnitTypes ePromotedUnit = ((UnitTypes)(GC.getCivilizationInfo(kActivePlayer.getCivilizationType()).getCivilizationUnits(i)));
			if (NO_UNIT != ePromotedUnit)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_EVENT_UNIT_CLASS_PROMOTION", GC.getUnitInfo(ePromotedUnit).getTextKeyWide(), GC.getPromotionInfo((PromotionTypes)kEvent.getUnitClassPromotion(i)).getTextKeyWide()));
			}
		}
	}

	if (kEvent.getConvertOwnCities() > 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_EVENT_CONVERT_OWN_CITIES", kEvent.getConvertOwnCities(), szReligion.GetCString()));
	}

	if (kEvent.getConvertOtherCities() > 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_EVENT_CONVERT_OTHER_CITIES", kEvent.getConvertOtherCities(), szReligion.GetCString()));
	}

	if (NO_PLAYER != pTriggeredData->m_eOtherPlayer)
	{
		if (kEvent.getAttitudeModifier() > 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_EVENT_ATTITUDE_GOOD", kEvent.getAttitudeModifier(), GET_PLAYER(pTriggeredData->m_eOtherPlayer).getNameKey()));
		}
		else if (kEvent.getAttitudeModifier() < 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_EVENT_ATTITUDE_BAD", kEvent.getAttitudeModifier(), GET_PLAYER(pTriggeredData->m_eOtherPlayer).getNameKey()));
		}
	}

	if (NO_PLAYER != pTriggeredData->m_eOtherPlayer)
	{
		TeamTypes eWorstEnemy = GET_TEAM(GET_PLAYER(pTriggeredData->m_eOtherPlayer).getTeam()).AI_getWorstEnemy();
		if (NO_TEAM != eWorstEnemy)
		{
			if (kEvent.getTheirEnemyAttitudeModifier() > 0)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_EVENT_ATTITUDE_GOOD", kEvent.getTheirEnemyAttitudeModifier(), GET_TEAM(eWorstEnemy).getName().GetCString()));
			}
			else if (kEvent.getTheirEnemyAttitudeModifier() < 0)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_EVENT_ATTITUDE_BAD", kEvent.getTheirEnemyAttitudeModifier(), GET_TEAM(eWorstEnemy).getName().GetCString()));
			}
		}
	}

	if (NO_PLAYER != pTriggeredData->m_eOtherPlayer)
	{
		if (kEvent.getEspionagePoints() > 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_EVENT_ESPIONAGE_POINTS", kEvent.getEspionagePoints(), GET_PLAYER(pTriggeredData->m_eOtherPlayer).getNameKey()));
		}
		else if (kEvent.getEspionagePoints() < 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_EVENT_ESPIONAGE_COST", -kEvent.getEspionagePoints()));
		}
	}

	if (kEvent.isGoldenAge())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_EVENT_GOLDEN_AGE"));
	}

	if (0 != kEvent.getFreeUnitSupport())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_EVENT_FREE_UNIT_SUPPORT", kEvent.getFreeUnitSupport()));
	}

	if (0 != kEvent.getInflationModifier())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_EVENT_INFLATION_MODIFIER", kEvent.getInflationModifier()));
	}

	if (kEvent.isDeclareWar())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_EVENT_DECLARE_WAR", GET_PLAYER(pTriggeredData->m_eOtherPlayer).getCivilizationAdjectiveKey()));
	}

	if (kEvent.getUnitImmobileTurns() > 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_EVENT_IMMOBILE_UNIT", kEvent.getUnitImmobileTurns(), szUnit.GetCString()));
	}

	if (!CvWString(kEvent.getPythonHelp()).empty())
	{
		CvWString szHelp;
		CyArgsList argsList;
		argsList.add(eEvent);
		argsList.add(gDLL->getPythonIFace()->makePythonObject(pTriggeredData));

		gDLL->getPythonIFace()->callFunction(PYRandomEventModule, kEvent.getPythonHelp(), argsList.makeFunctionArgs(), &szHelp);

		szBuffer.append(NEWLINE);
		szBuffer.append(szHelp);
	}

	CvWStringBuffer szTemp;
	for (int i = 0; i < GC.getNumEventInfos(); ++i)
	{
		if (0 == kEvent.getAdditionalEventTime(i))
		{
			if (kEvent.getAdditionalEventChance(i) > 0)
			{
				if (GET_PLAYER(GC.getGameINLINE().getActivePlayer()).canDoEvent((EventTypes)i, *pTriggeredData))
				{
					szTemp.clear();
					setEventHelp(szTemp, (EventTypes)i, iEventTriggeredId, ePlayer);

					if (!szTemp.isEmpty())
					{
						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_EVENT_ADDITIONAL_CHANCE", kEvent.getAdditionalEventChance(i), L""));
						szBuffer.append(NEWLINE);
						szBuffer.append(szTemp);
					}
				}
			}
		}
		else
		{
			szTemp.clear();
			setEventHelp(szTemp, (EventTypes)i, iEventTriggeredId, ePlayer);

			if (!szTemp.isEmpty())
			{
				CvWString szDelay = gDLL->getText("TXT_KEY_EVENT_DELAY_TURNS", (GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getGrowthPercent() * kEvent.getAdditionalEventTime((EventTypes)i)) / 100);

				if (kEvent.getAdditionalEventChance(i) > 0)
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_EVENT_ADDITIONAL_CHANCE", kEvent.getAdditionalEventChance(i), szDelay.GetCString()));
				}
				else
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_EVENT_DELAY", szDelay.GetCString()));
				}

				szBuffer.append(NEWLINE);
				szBuffer.append(szTemp);
			}
		}
	}

	if (NO_TECH != kEvent.getPrereqTech())
	{
		if (!GET_TEAM(kActivePlayer.getTeam()).isHasTech((TechTypes)kEvent.getPrereqTech()))
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_REQUIRES_STRING", GC.getTechInfo((TechTypes)(kEvent.getPrereqTech())).getTextKeyWide()));
		}
	}

	bool done = false;
	while(!done)
	{
		done = true;
		if(!szBuffer.isEmpty())
		{
			const wchar* wideChar = szBuffer.getCString();
			if(wideChar[0] == L'\n')
			{
				CvWString tempString(&wideChar[1]);
				szBuffer.clear();
				szBuffer.append(tempString);
				done = false;
			}
		}
	}
}

void CvGameTextMgr::eventTechHelp(CvWStringBuffer& szBuffer, EventTypes eEvent, TechTypes eTech, PlayerTypes eActivePlayer, PlayerTypes eOtherPlayer)
{
	CvEventInfo& kEvent = GC.getEventInfo(eEvent);

	if (eTech != NO_TECH)
	{
		if (100 == kEvent.getTechPercent())
		{
			if (NO_PLAYER != eOtherPlayer)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_EVENT_TECH_GAINED_FROM_PLAYER", GC.getTechInfo(eTech).getTextKeyWide(), GET_PLAYER(eOtherPlayer).getNameKey()));
			}
			else
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_EVENT_TECH_GAINED", GC.getTechInfo(eTech).getTextKeyWide()));
			}
		}
		else if (0 != kEvent.getTechPercent())
		{
			CvTeam& kTeam = GET_TEAM(GET_PLAYER(eActivePlayer).getTeam());
			int iBeakers = (kTeam.getResearchCost(eTech) * kEvent.getTechPercent()) / 100;
			if (kEvent.getTechPercent() > 0)
			{
				iBeakers = std::min(kTeam.getResearchLeft(eTech), iBeakers);
			}
			else if (kEvent.getTechPercent() < 0)
			{
				iBeakers = std::max(kTeam.getResearchLeft(eTech) - kTeam.getResearchCost(eTech), iBeakers);
			}

			if (NO_PLAYER != eOtherPlayer)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_EVENT_TECH_GAINED_FROM_PLAYER_PERCENT", iBeakers, GC.getTechInfo(eTech).getTextKeyWide(), GET_PLAYER(eOtherPlayer).getNameKey()));
			}
			else
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_EVENT_TECH_GAINED_PERCENT", iBeakers, GC.getTechInfo(eTech).getTextKeyWide()));
			}
		}
	}
}

void CvGameTextMgr::eventGoldHelp(CvWStringBuffer& szBuffer, EventTypes eEvent, PlayerTypes ePlayer, PlayerTypes eOtherPlayer)
{
	CvEventInfo& kEvent = GC.getEventInfo(eEvent);
	CvPlayer& kPlayer = GET_PLAYER(ePlayer);

	int iGold1 = kPlayer.getEventCost(eEvent, eOtherPlayer, false);
	int iGold2 = kPlayer.getEventCost(eEvent, eOtherPlayer, true);

	if (0 != iGold1 || 0 != iGold2)
	{
		if (iGold1 == iGold2)
		{
			if (NO_PLAYER != eOtherPlayer && kEvent.isGoldToPlayer())
			{
				if (iGold1 > 0)
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_EVENT_GOLD_FROM_PLAYER", iGold1, GET_PLAYER(eOtherPlayer).getNameKey()));
				}
				else
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_EVENT_GOLD_TO_PLAYER", -iGold1, GET_PLAYER(eOtherPlayer).getNameKey()));
				}
			}
			else
			{
				if (iGold1 > 0)
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_EVENT_GOLD_GAINED", iGold1));
				}
				else
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_EVENT_GOLD_LOST", -iGold1));
				}
			}
		}
		else
		{
			if (NO_PLAYER != eOtherPlayer && kEvent.isGoldToPlayer())
			{
				if (iGold1 > 0)
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_EVENT_GOLD_RANGE_FROM_PLAYER", iGold1, iGold2, GET_PLAYER(eOtherPlayer).getNameKey()));
				}
				else
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_EVENT_GOLD_RANGE_TO_PLAYER", -iGold1, -iGold2, GET_PLAYER(eOtherPlayer).getNameKey()));
				}
			}
			else
			{
				if (iGold1 > 0)
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_EVENT_GOLD_RANGE_GAINED", iGold1, iGold2));
				}
				else
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_EVENT_GOLD_RANGE_LOST", -iGold1, iGold2));
				}
			}
		}
	}
}

void CvGameTextMgr::setTradeRouteHelp(CvWStringBuffer &szBuffer, int iRoute, CvCity* pCity)
{
	if (NULL != pCity)
	{
		CvCity* pOtherCity = pCity->getTradeCity(iRoute);

		if (NULL != pOtherCity)
		{
			szBuffer.append(pOtherCity->getName());

			int iProfit = pCity->getBaseTradeProfit(pOtherCity);

			szBuffer.append(NEWLINE);
			CvWString szBaseProfit;
			szBaseProfit.Format(L"%d.%02d", iProfit/100, iProfit%100);
			szBuffer.append(gDLL->getText("TXT_KEY_TRADE_ROUTE_HELP_BASE", szBaseProfit.GetCString()));

			PlayerTypes pOwner = pCity->getOwner();

			int iModifier = 100;
			int iNewMod;

			for (int iBuilding = 0; iBuilding < GC.getNumBuildingInfos(); ++iBuilding)
			{
				if (pCity->getNumActiveBuilding((BuildingTypes)iBuilding) > 0)
				{
					iNewMod = pCity->getNumActiveBuilding((BuildingTypes)iBuilding) * GC.getBuildingInfo((BuildingTypes)iBuilding).getTradeRouteModifier();
					if (0 != iNewMod)
					{
						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_TRADE_ROUTE_MOD_BUILDING", GC.getBuildingInfo((BuildingTypes)iBuilding).getTextKeyWide(), iNewMod));
						iModifier += iNewMod;
					}
				}
			}

			iNewMod = pCity->getPopulationTradeModifier();
			if (0 != iNewMod)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_TRADE_ROUTE_MOD_POPULATION", iNewMod));
				iModifier += iNewMod;
			}

			if (pCity->isConnectedToCapital())
			{
				iNewMod = GC.getDefineINT("CAPITAL_TRADE_MODIFIER");
				if (0 != iNewMod)
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_TRADE_ROUTE_MOD_CAPITAL", iNewMod));
					iModifier += iNewMod;
				}
			}

			if (NULL != pOtherCity)
			{
				if (pCity->area() != pOtherCity->area())
				{
					iNewMod = GC.getDefineINT("OVERSEAS_TRADE_MODIFIER");
					if (0 != iNewMod)
					{
						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_TRADE_ROUTE_MOD_OVERSEAS", iNewMod));
						iModifier += iNewMod;
					}
				}

				if (pCity->getTeam() != pOtherCity->getTeam())
				{
					iNewMod = pCity->getForeignTradeRouteModifier();
					if (0 != iNewMod)
					{
						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_TRADE_ROUTE_MOD_FOREIGN", iNewMod));
						iModifier += iNewMod;
					}

					iNewMod = 0;
					int iI;
					for (iI = 0; iI < GC.getNumTraitInfos(); iI++)
					{
						if (GET_PLAYER(pOwner).hasTrait((TraitTypes)iI))
						{
							CvTraitInfo& kTrait = GC.getTraitInfo((TraitTypes)iI);
							if (kTrait.getForeignTradeRouteIncomeModifier() != 0)
							{
								iNewMod += kTrait.getForeignTradeRouteIncomeModifier();
							}
					}
					}
					if (0 != iNewMod)
					{
						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_TRADE_ROUTE_MOD_FOREIGN_TRAITS", iNewMod));
						iModifier += iNewMod;					
					}

					iNewMod = pCity->getPeaceTradeModifier(pOtherCity->getTeam());
					if (0 != iNewMod)
					{
						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_TRADE_ROUTE_MOD_PEACE", iNewMod));
						iModifier += iNewMod;
					}
				}
			}

			FAssert(pCity->totalTradeModifier(pOtherCity) == iModifier);

			iProfit *= iModifier;
			iProfit /= 100;
			FAssert(iProfit == pCity->calculateTradeProfitTimes100(pOtherCity));

			szBuffer.append(SEPARATOR);

			szBuffer.append(NEWLINE);
			CvWString szProfit;
			szProfit.Format(L"%d.%02d", iProfit / 100, iProfit % 100);
			szBuffer.append(gDLL->getText("TXT_KEY_TRADE_ROUTE_TOTAL_FRACTIONAL", szProfit.GetCString()));
		}
	}
}

void CvGameTextMgr::setEspionageCostHelp(CvWStringBuffer &szBuffer, EspionageMissionTypes eMission, PlayerTypes eTargetPlayer, const CvPlot* pPlot, int iExtraData, const CvUnit* pSpyUnit)
{
	CvPlayer& kPlayer = GET_PLAYER(GC.getGameINLINE().getActivePlayer());
	CvEspionageMissionInfo& kMission = GC.getEspionageMissionInfo(eMission);

	//szBuffer.assign(kMission.getDescription());

	int iMissionCost = kPlayer.getEspionageMissionBaseCost(eMission, eTargetPlayer, pPlot, iExtraData, pSpyUnit);
	iMissionCost *= GET_TEAM(kPlayer.getTeam()).getNumMembers(); // K-Mod

	if (kMission.isDestroyImprovement())
	{
		if (NULL != pPlot && NO_IMPROVEMENT != pPlot->getImprovementType())
		{
			szBuffer.append(gDLL->getText("TXT_KEY_ESPIONAGE_HELP_DESTROY_IMPROVEMENT", GC.getImprovementInfo(pPlot->getImprovementType()).getTextKeyWide()));
			szBuffer.append(NEWLINE);
		}
	}

	if (kMission.getDestroyBuildingCostFactor() > 0)
	{
		BuildingTypes eTargetBuilding = (BuildingTypes)iExtraData;

		if (NULL != pPlot)
		{
			CvCity* pCity = pPlot->getPlotCity();

			if (NULL != pCity)
			{
				szBuffer.append(gDLL->getText("TXT_KEY_ESPIONAGE_HELP_DESTROY_IMPROVEMENT", GC.getBuildingInfo(eTargetBuilding).getTextKeyWide()));
				szBuffer.append(NEWLINE);
			}
		}
	}

	if (kMission.getDestroyProjectCostFactor() > 0)
	{
		ProjectTypes eTargetProject = (ProjectTypes)iExtraData;

		if (NULL != pPlot)
		{
			CvCity* pCity = pPlot->getPlotCity();

			if (NULL != pCity)
			{
				szBuffer.append(gDLL->getText("TXT_KEY_ESPIONAGE_HELP_DESTROY_IMPROVEMENT", GC.getProjectInfo(eTargetProject).getTextKeyWide()));
				szBuffer.append(NEWLINE);
			}
		}
	}

	if (kMission.getDestroyProductionCostFactor() > 0)
	{
		if (NULL != pPlot)
		{
			CvCity* pCity = pPlot->getPlotCity();

			if (NULL != pCity)
			{
				szBuffer.append(gDLL->getText("TXT_KEY_ESPIONAGE_HELP_DESTROY_PRODUCTION", pCity->getProduction()));
				szBuffer.append(NEWLINE);
			}
		}
	}

	if (kMission.getDestroyUnitCostFactor() > 0)
	{
		if (NO_PLAYER != eTargetPlayer)
		{
			int iTargetUnitID = iExtraData;

			CvUnit* pUnit = GET_PLAYER(eTargetPlayer).getUnit(iTargetUnitID);

			if (NULL != pUnit)
			{
				szBuffer.append(gDLL->getText("TXT_KEY_ESPIONAGE_HELP_DESTROY_UNIT", pUnit->getNameKey()));
				szBuffer.append(NEWLINE);
			}
		}
	}

	if (kMission.getBuyUnitCostFactor() > 0)
	{
		if (NO_PLAYER != eTargetPlayer)
		{
			int iTargetUnitID = iExtraData;

			CvUnit* pUnit = GET_PLAYER(eTargetPlayer).getUnit(iTargetUnitID);

			if (NULL != pUnit)
			{
				szBuffer.append(gDLL->getText("TXT_KEY_ESPIONAGE_HELP_BRIBE", pUnit->getNameKey()));
				szBuffer.append(NEWLINE);
			}
		}
	}

	if (kMission.getBuyCityCostFactor() > 0)
	{
		if (NULL != pPlot)
		{
			CvCity* pCity = pPlot->getPlotCity();

			if (NULL != pCity)
			{
				szBuffer.append(gDLL->getText("TXT_KEY_ESPIONAGE_HELP_BRIBE", pCity->getNameKey()));
				szBuffer.append(NEWLINE);
			}
		}
	}

	if (kMission.getCityInsertCultureCostFactor() > 0)
	{
		if (NULL != pPlot)
		{
			CvCity* pCity = pPlot->getPlotCity();

			if (NULL != pCity && pPlot->getCulture(GC.getGameINLINE().getActivePlayer()) > 0)
			{
				int iCultureAmount = kMission.getCityInsertCultureAmountFactor() *  pCity->countTotalCultureTimes100();
				iCultureAmount /= 10000;
				iCultureAmount = std::max(1, iCultureAmount);

				szBuffer.append(gDLL->getText("TXT_KEY_ESPIONAGE_HELP_INSERT_CULTURE", pCity->getNameKey(), iCultureAmount, kMission.getCityInsertCultureAmountFactor()));
				szBuffer.append(NEWLINE);
			}
		}
	}

	if (kMission.getCityPoisonWaterCounter() > 0)
	{
		if (NULL != pPlot)
		{
			CvCity* pCity = pPlot->getPlotCity();

			if (NULL != pCity)
			{
				szBuffer.append(gDLL->getText("TXT_KEY_ESPIONAGE_HELP_POISON", kMission.getCityPoisonWaterCounter(), gDLL->getSymbolID(UNHEALTHY_CHAR), pCity->getNameKey(), kMission.getCityPoisonWaterCounter()));
				szBuffer.append(NEWLINE);
			}
		}
	}

	if (kMission.getCityUnhappinessCounter() > 0)
	{
		if (NULL != pPlot)
		{
			CvCity* pCity = pPlot->getPlotCity();

			if (NULL != pCity)
			{
				szBuffer.append(gDLL->getText("TXT_KEY_ESPIONAGE_HELP_POISON", kMission.getCityUnhappinessCounter(), gDLL->getSymbolID(UNHAPPY_CHAR), pCity->getNameKey(), kMission.getCityUnhappinessCounter()));
				szBuffer.append(NEWLINE);
			}
		}
	}

	if (kMission.getCityRevoltCounter() > 0)
	{
		if (NULL != pPlot)
		{
			CvCity* pCity = pPlot->getPlotCity();

			if (NULL != pCity)
			{
				szBuffer.append(gDLL->getText("TXT_KEY_ESPIONAGE_HELP_REVOLT", pCity->getNameKey(), kMission.getCityRevoltCounter()));
				szBuffer.append(NEWLINE);
			}
		}
	}

	if (kMission.getStealTreasuryTypes() > 0)
	{
		if (NO_PLAYER != eTargetPlayer)
		{
			// K-Mod
			//int iNumTotalGold = (GET_PLAYER(eTargetPlayer).getGold() * kMission.getStealTreasuryTypes()) / 100;
			int iNumTotalGold = 0;

			if (NULL != pPlot)
			{
				CvCity* pCity = pPlot->getPlotCity();

				if (NULL != pCity)
				{
					/* iNumTotalGold *= pCity->getPopulation();
					iNumTotalGold /= std::max(1, GET_PLAYER(eTargetPlayer).getTotalPopulation());*/
					// K-Mod					
					iNumTotalGold = kPlayer.getEspionageGoldQuantity(eMission, eTargetPlayer, pCity);
				}
			}

			szBuffer.append(gDLL->getText("TXT_KEY_ESPIONAGE_HELP_STEAL_TREASURY", iNumTotalGold, GET_PLAYER(eTargetPlayer).getCivilizationAdjectiveKey()));
			szBuffer.append(NEWLINE);
		}
	}

	if (kMission.getBuyTechCostFactor() > 0)
	{
		szBuffer.append(gDLL->getText("TXT_KEY_ESPIONAGE_HELP_STEAL_TECH", GC.getTechInfo((TechTypes)iExtraData).getTextKeyWide()));
		szBuffer.append(NEWLINE);
	}

	if (kMission.getSwitchCivicCostFactor() > 0)
	{
		if (NO_PLAYER != eTargetPlayer)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_ESPIONAGE_HELP_SWITCH_CIVIC", GET_PLAYER(eTargetPlayer).getNameKey(), GC.getCivicInfo((CivicTypes)iExtraData).getTextKeyWide()));
			szBuffer.append(NEWLINE);
		}
	}

	if (kMission.getSwitchReligionCostFactor() > 0)
	{
		if (NO_PLAYER != eTargetPlayer)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_ESPIONAGE_HELP_SWITCH_CIVIC", GET_PLAYER(eTargetPlayer).getNameKey(), GC.getReligionInfo((ReligionTypes)iExtraData).getTextKeyWide()));
			szBuffer.append(NEWLINE);
		}
	}

	if (kMission.getPlayerAnarchyCounter() > 0)
	{
		if (NO_PLAYER != eTargetPlayer)
		{
			int iTurns = (kMission.getPlayerAnarchyCounter() * GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getAnarchyPercent()) / 100;
			szBuffer.append(gDLL->getText("TXT_KEY_ESPIONAGE_HELP_ANARCHY", GET_PLAYER(eTargetPlayer).getNameKey(), iTurns));
			szBuffer.append(NEWLINE);
		}
	}

	if (kMission.getCounterespionageNumTurns() > 0 && kMission.getCounterespionageMod() > 0)
	{
		if (NO_PLAYER != eTargetPlayer)
		{
			int iTurns = (kMission.getCounterespionageNumTurns() * GC.getGameSpeedInfo(GC.getGameINLINE().getGameSpeedType()).getResearchPercent()) / 100;

			szBuffer.append(gDLL->getText("TXT_KEY_ESPIONAGE_HELP_COUNTERESPIONAGE", kMission.getCounterespionageMod(), GET_PLAYER(eTargetPlayer).getCivilizationAdjectiveKey(), iTurns));
			szBuffer.append(NEWLINE);
		}
	}

	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_ESPIONAGE_BASE_COST", iMissionCost));

	if (kPlayer.getEspionageMissionCost(eMission, eTargetPlayer, pPlot, iExtraData, pSpyUnit) > 0)
	{
		int iModifier = 100;
		int iTempModifier = 0;
		CvCity* pCity = NULL;
		if (NULL != pPlot)
		{
			pCity = pPlot->getPlotCity();
		}

		if (pCity != NULL && GC.getEspionageMissionInfo(eMission).isTargetsCity())
		{
			// City Population
			iTempModifier = (GC.getDefineINT("ESPIONAGE_CITY_POP_EACH_MOD") * (pCity->getPopulation() - 1));
			if (0 != iTempModifier)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_ESPIONAGE_POPULATION_MOD", iTempModifier));
				iModifier *= 100 + iTempModifier;
				iModifier /= 100;
			}

			// Trade Route
			if (pCity->isTradeRoute(kPlayer.getID()))
			{
				iTempModifier = GC.getDefineINT("ESPIONAGE_CITY_TRADE_ROUTE_MOD");
				if (0 != iTempModifier)
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_ESPIONAGE_TRADE_ROUTE_MOD", iTempModifier));
					iModifier *= 100 + iTempModifier;
					iModifier /= 100;
				}
			}

			ReligionTypes eReligion = kPlayer.getStateReligion();
			if (NO_RELIGION != eReligion)
			{
				iTempModifier = 0;

				if (pCity->isHasReligion(eReligion))
				{
					if (GET_PLAYER(eTargetPlayer).getStateReligion() != eReligion)
					{
						iTempModifier += GC.getDefineINT("ESPIONAGE_CITY_RELIGION_STATE_MOD");
					}

					if (kPlayer.hasHolyCity(eReligion))
					{
						iTempModifier += GC.getDefineINT("ESPIONAGE_CITY_HOLY_CITY_MOD");
					}
				}

				if (0 != iTempModifier)
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_ESPIONAGE_RELIGION_MOD", iTempModifier));
					iModifier *= 100 + iTempModifier;
					iModifier /= 100;
				}
			}

			// City's culture affects cost
			// MOD - START - Stock Culture
			// TODO: Decide on culture style
			/* original bts code */
			iTempModifier = - (pCity->getCultureTimes100(kPlayer.getID()) * GC.getDefineINT("ESPIONAGE_CULTURE_MULTIPLIER_MOD")) / std::max(1, pCity->getCultureTimes100(eTargetPlayer) + pCity->getCultureTimes100(kPlayer.getID()));
			if (0 != iTempModifier)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_ESPIONAGE_CULTURE_MOD", iTempModifier));
				iModifier *= 100 + iTempModifier;
				iModifier /= 100;
			} /**/ // (moved and changed by K-Mod)
			// MOD - END - Stock Culture

			iTempModifier = pCity->getEspionageDefenseModifier();
			if (0 != iTempModifier)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_ESPIONAGE_DEFENSE_MOD", iTempModifier));
				iModifier *= 100 + iTempModifier;
				iModifier /= 100;
			}
		}

		if (pPlot != NULL)
		{
			// MOD - START - Stock Culture
			// TODO: Decide on whether or not to use this
			/*
			// K-Mod. Culture Mod. (Based on plot culture rather than city culture.)
			if (eMission == NO_ESPIONAGEMISSION || GC.getEspionageMissionInfo(eMission).isSelectPlot() || GC.getEspionageMissionInfo(eMission).isTargetsCity())
			{
				iTempModifier = - (pPlot->getCulture(kPlayer.getID()) * GC.getDefineINT("ESPIONAGE_CULTURE_MULTIPLIER_MOD")) / std::max(1, pPlot->getCulture(eTargetPlayer) + pPlot->getCulture(kPlayer.getID()));
				if (0 != iTempModifier)
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_ESPIONAGE_CULTURE_MOD", iTempModifier));
					iModifier *= 100 + iTempModifier;
					iModifier /= 100;
				}
			}
			// K-Mod end
			*/
			// MOD - END - Stock Culture

			// Distance mod
			int iDistance = GC.getMap().maxPlotDistance();

			CvCity* pOurCapital = kPlayer.getCapitalCity();
			if (NULL != pOurCapital)
			{
				if (kMission.isSelectPlot() || kMission.isTargetsCity())
				{
					iDistance = plotDistance(pOurCapital->getX_INLINE(), pOurCapital->getY_INLINE(), pPlot->getX_INLINE(), pPlot->getY_INLINE());
				}
				else
				{
					CvCity* pTheirCapital = GET_PLAYER(eTargetPlayer).getCapitalCity();
					if (NULL != pTheirCapital)
					{
						iDistance = plotDistance(pOurCapital->getX_INLINE(), pOurCapital->getY_INLINE(), pTheirCapital->getX_INLINE(), pTheirCapital->getY_INLINE());
					}
				}
			}

			iTempModifier = (iDistance + GC.getMapINLINE().maxPlotDistance()) * GC.getDefineINT("ESPIONAGE_DISTANCE_MULTIPLIER_MOD") / GC.getMapINLINE().maxPlotDistance() - 100;
			if (0 != iTempModifier)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_ESPIONAGE_DISTANCE_MOD", iTempModifier));
				iModifier *= 100 + iTempModifier;
				iModifier /= 100;
			}
		}

		// Spy presence mission cost alteration
		if (NULL != pSpyUnit)
		{
			iTempModifier = -(pSpyUnit->getFortifyTurns() * GC.getDefineINT("ESPIONAGE_EACH_TURN_UNIT_COST_DECREASE"));
			if (0 != iTempModifier)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_ESPIONAGE_SPY_STATIONARY_MOD", iTempModifier));
				iModifier *= 100 + iTempModifier;
				iModifier /= 100;
			}
		}

		// My points VS. Your points to mod cost
		iTempModifier = ::getEspionageModifier(kPlayer.getTeam(), GET_PLAYER(eTargetPlayer).getTeam()) - 100;
		if (0 != iTempModifier)
		{
			szBuffer.append(SEPARATOR);
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_ESPIONAGE_EP_RATIO_MOD", iTempModifier));
			iModifier *= 100 + iTempModifier;
			iModifier /= 100;
		}

		// Counterespionage Mission Mod
		CvTeam& kTargetTeam = GET_TEAM(GET_PLAYER(eTargetPlayer).getTeam());
		if (kTargetTeam.getCounterespionageModAgainstTeam(kPlayer.getTeam()) > 0)
		{
			//iTempModifier = kTargetTeam.getCounterespionageModAgainstTeam(kPlayer.getTeam()) - 100;
			// K-Mod
			iTempModifier = std::max(-100, kTargetTeam.getCounterespionageModAgainstTeam(kPlayer.getTeam()));
			if (0 != iTempModifier)
			{
				szBuffer.append(SEPARATOR);
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_ESPIONAGE_COUNTERESPIONAGE_MOD", iTempModifier));
				iModifier *= 100 + iTempModifier;
				iModifier /= 100;
			}
		}

		FAssert(iModifier == kPlayer.getEspionageMissionCostModifier(eMission, eTargetPlayer, pPlot, iExtraData, pSpyUnit));

		iMissionCost *= iModifier;
		iMissionCost /= 100;

		FAssert(iMissionCost == kPlayer.getEspionageMissionCost(eMission, eTargetPlayer, pPlot, iExtraData, pSpyUnit));

		szBuffer.append(SEPARATOR);

		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_ESPIONAGE_COST_TOTAL", iMissionCost));


		if (NULL != pSpyUnit)
		{
			int iInterceptChance = (pSpyUnit->getSpyInterceptPercent(GET_PLAYER(eTargetPlayer).getTeam(), true) * (100 + kMission.getDifficultyMod())) / 100;

			szBuffer.append(NEWLINE);
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_ESPIONAGE_CHANCE_OF_SUCCESS", std::min(100, std::max(0, 100 - iInterceptChance))));
		}
	}
}

void CvGameTextMgr::getTradeScreenTitleIcon(CvString& szButton, CvWidgetDataStruct& widgetData, PlayerTypes ePlayer)
{
	szButton.clear();

	ReligionTypes eReligion = GET_PLAYER(ePlayer).getStateReligion();
	if (eReligion != NO_RELIGION)
	{
		szButton = GC.getReligionInfo(eReligion).getButton();
		widgetData.m_eWidgetType = WIDGET_HELP_RELIGION;
		widgetData.m_iData1 = eReligion;
		widgetData.m_iData2 = -1;
		widgetData.m_bOption = false;
	}
}

void CvGameTextMgr::getTradeScreenIcons(std::vector< std::pair<CvString, CvWidgetDataStruct> >& aIconInfos, PlayerTypes ePlayer)
{
	aIconInfos.clear();
	for (int i = 0; i < GC.getNumCivicOptionInfos(); i++)
	{
		CivicTypes eCivic = GET_PLAYER(ePlayer).getCivics((CivicOptionTypes)i);
		CvWidgetDataStruct widgetData;
		widgetData.m_eWidgetType = WIDGET_PEDIA_JUMP_TO_CIVIC;
		widgetData.m_iData1 = eCivic;
		widgetData.m_iData2 = -1;
		widgetData.m_bOption = false;
		aIconInfos.push_back(std::make_pair(GC.getCivicInfo(eCivic).getButton(), widgetData));
	}

}

void CvGameTextMgr::getTradeScreenHeader(CvWString& szHeader, PlayerTypes ePlayer, PlayerTypes eOtherPlayer, bool bAttitude)
{
	CvPlayer& kPlayer = GET_PLAYER(ePlayer);
	szHeader.Format(L"%s - %s", CvWString(kPlayer.getName()).GetCString(), CvWString(kPlayer.getCivilizationShortDescription()).GetCString());
	if (bAttitude)
	{
		szHeader += CvWString::format(L" (%s)", GC.getAttitudeInfo(kPlayer.AI_getAttitude(eOtherPlayer)).getDescription());
	}
}

void CvGameTextMgr::getGlobeLayerName(GlobeLayerTypes eType, int iOption, CvWString& strName)
{
	switch (eType)
	{
	case GLOBE_LAYER_STRATEGY:
		switch(iOption)
		{
		case 0:
			strName = gDLL->getText("TXT_KEY_GLOBELAYER_STRATEGY_VIEW");
			break;
		case 1:
			strName = gDLL->getText("TXT_KEY_GLOBELAYER_STRATEGY_NEW_LINE");
			break;
		case 2:
			strName = gDLL->getText("TXT_KEY_GLOBELAYER_STRATEGY_NEW_SIGN");
			break;
		case 3:
			strName = gDLL->getText("TXT_KEY_GLOBELAYER_STRATEGY_DELETE");
			break;
		case 4:
			strName = gDLL->getText("TXT_KEY_GLOBELAYER_STRATEGY_DELETE_LINES");
			break;
		}
		break;
	case GLOBE_LAYER_UNIT:
		switch(iOption)
		{
		case SHOW_ALL_MILITARY:
			strName = gDLL->getText("TXT_KEY_GLOBELAYER_UNITS_ALLMILITARY");
			break;
		case SHOW_TEAM_MILITARY:
			strName = gDLL->getText("TXT_KEY_GLOBELAYER_UNITS_TEAMMILITARY");
			break;
		case SHOW_ENEMIES_IN_TERRITORY:
			strName = gDLL->getText("TXT_KEY_GLOBELAYER_UNITS_ENEMY_TERRITORY_MILITARY");
			break;
		case SHOW_ENEMIES:
			strName = gDLL->getText("TXT_KEY_GLOBELAYER_UNITS_ENEMYMILITARY");
			break;
		case SHOW_PLAYER_DOMESTICS:
			strName = gDLL->getText("TXT_KEY_GLOBELAYER_UNITS_DOMESTICS");
			break;
		}
		break;
	case GLOBE_LAYER_RESOURCE:
		switch(iOption)
		{
		case SHOW_ALL_RESOURCES:
			strName = gDLL->getText("TXT_KEY_GLOBELAYER_RESOURCES_EVERYTHING");
			break;
		case SHOW_STRATEGIC_RESOURCES:
			// MOD - START - Bonus Category for Globe View
			strName = gDLL->getText("TXT_KEY_GLOBELAYER_RESOURCES_STRATEGIC");
			// MOD - END - Bonus Category for Globe View
			//strName = gDLL->getText("TXT_KEY_GLOBELAYER_RESOURCES_GENERAL");
			break;
		case SHOW_HAPPY_RESOURCES:
			strName = gDLL->getText("TXT_KEY_GLOBELAYER_RESOURCES_LUXURIES");
			break;
		case SHOW_HEALTH_RESOURCES:
			strName = gDLL->getText("TXT_KEY_GLOBELAYER_RESOURCES_FOOD");
			break;
		}
		break;
	case GLOBE_LAYER_RELIGION:
		strName = GC.getReligionInfo((ReligionTypes) iOption).getDescription();
		break;
	case GLOBE_LAYER_CULTURE:
	case GLOBE_LAYER_TRADE:
		// these have no sub-options
		strName.clear();
		break;
	}
}

void CvGameTextMgr::getPlotHelp(CvPlot* pMouseOverPlot, CvCity* pCity, CvPlot* pFlagPlot, bool bAlt, CvWStringBuffer& strHelp)
{
	if (gDLL->getInterfaceIFace()->isCityScreenUp())
	{
		if (pMouseOverPlot != NULL)
		{
			CvCity* pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();
			if (pHeadSelectedCity != NULL)
			{
				if (pMouseOverPlot->getWorkingCity() == pHeadSelectedCity)
				{
					if (pMouseOverPlot->isRevealed(GC.getGameINLINE().getActiveTeam(), true))
					{
						setPlotHelp(strHelp, pMouseOverPlot);
					}
				}
			}
		}
	}
	else
	{
		if (pCity != NULL)
		{
			setCityBarHelp(strHelp, pCity);
		}
		else if (pFlagPlot != NULL)
		{
			setPlotListHelp(strHelp, pFlagPlot, false, true);
		}

		if (strHelp.isEmpty())
		{
			if (pMouseOverPlot != NULL)
			{
				//if ((pMouseOverPlot == gDLL->getInterfaceIFace()->getGotoPlot()) || bAlt)
				if (pMouseOverPlot == gDLL->getInterfaceIFace()->getGotoPlot() || (bAlt && gDLL->getChtLvl() == 0)) // K-Mod. (alt does something else in cheat mode)
				{
					if (pMouseOverPlot->isActiveVisible(true))
					{
						setCombatPlotHelp(strHelp, pMouseOverPlot);
					}
				}
			}
		}

		if (strHelp.isEmpty())
		{
			if (pMouseOverPlot != NULL)
			{
				if (pMouseOverPlot->isRevealed(GC.getGameINLINE().getActiveTeam(), true))
				{
					if (pMouseOverPlot->isActiveVisible(true))
					{
						setPlotListHelp(strHelp, pMouseOverPlot, true, false);

						if (!strHelp.isEmpty())
						{
							strHelp.append(L"\n");
						}
					}

					setPlotHelp(strHelp, pMouseOverPlot);
				}
			}
		}

		InterfaceModeTypes eInterfaceMode = gDLL->getInterfaceIFace()->getInterfaceMode();
		if (eInterfaceMode != INTERFACEMODE_SELECTION)
		{
			CvWString szTempBuffer;
			szTempBuffer.Format(SETCOLR L"%s" ENDCOLR NEWLINE, TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), GC.getInterfaceModeInfo(eInterfaceMode).getDescription());

			switch (eInterfaceMode)
			{
			case INTERFACEMODE_REBASE:
				getRebasePlotHelp(pMouseOverPlot, szTempBuffer);
				break;

			case INTERFACEMODE_NUKE:
				getNukePlotHelp(pMouseOverPlot, szTempBuffer);
				break;
			// MOD - START - Advanced Automations
/************************************************************************************************/
/* Afforess	                  Start		 09/18/10                                               */
/*                                                                                              */
/* Advanced Automations                                                                         */
/************************************************************************************************/
			case INTERFACEMODE_SHADOW_UNIT:
				{
					CvUnit* pHeadSelectedUnit = gDLL->getInterfaceIFace()->getHeadSelectedUnit();
					if (pHeadSelectedUnit != NULL && pMouseOverPlot != NULL)
					{
						if (pHeadSelectedUnit->getGroup()->canDoInterfaceModeAt(eInterfaceMode, pMouseOverPlot))
						{
							// TODO: Give the player some indication of what they are doing
						}
						else
						{
							CvUnit* pShadowUnit = pMouseOverPlot->getCenterUnit();
							if (pShadowUnit != NULL)
							{
								strHelp.clear();

								bool bFirst = true;

								if (pShadowUnit->baseMoves() > pHeadSelectedUnit->baseMoves())
								{
									if (!bFirst)
										strHelp.append(L"\n");
									strHelp.append(gDLL->getText("TXT_KEY_CAN_NOT_AUTOMATE_PROTECT_NOT_FAST_ENOUGH", pShadowUnit->getNameKey(), pHeadSelectedUnit->getNameKey(), pShadowUnit->baseMoves(), pShadowUnit->getNameKey()));
									bFirst = false;
								}

								if (pShadowUnit->getTeam() != pHeadSelectedUnit->getTeam())
								{
									if (!bFirst)
										strHelp.append(L"\n");
									strHelp.append(gDLL->getText("TXT_KEY_CAN_NOT_AUTOMATE_PROTECT_DIFFERENT_TEAM", pShadowUnit->getNameKey()));
									bFirst = false;
								}

								if (pShadowUnit == pHeadSelectedUnit)
								{
									if (!bFirst)
										strHelp.append(L"\n");
									strHelp.append(gDLL->getText("TXT_KEY_CAN_NOT_AUTOMATE_PROTECT_YOURSELF"));
									bFirst = false;
								}
							}
						}
					}
				}
				break;
/************************************************************************************************/
/* Afforess	                     END                                                            */
/************************************************************************************************/
			// MOD - END - Advanced Automations
			default:
				break;
			}

			szTempBuffer += strHelp.getCString();
			strHelp.assign(szTempBuffer);
		}
	}
}

void CvGameTextMgr::getRebasePlotHelp(CvPlot* pPlot, CvWString& strHelp)
{
	if (NULL != pPlot)
	{
		CvUnit* pHeadSelectedUnit = gDLL->getInterfaceIFace()->getHeadSelectedUnit();
		if (NULL != pHeadSelectedUnit)
		{
			if (pPlot->isFriendlyCity(*pHeadSelectedUnit, true))
			{
				CvCity* pCity = pPlot->getPlotCity();
				if (NULL != pCity)
				{
					int iNumUnits = pCity->plot()->countNumAirUnits(GC.getGameINLINE().getActiveTeam());
					bool bFull = (iNumUnits >= pCity->getAirUnitCapacity(GC.getGameINLINE().getActiveTeam()));

					if (bFull)
					{
						strHelp += CvWString::format(SETCOLR, TEXT_COLOR("COLOR_WARNING_TEXT"));
					}

					strHelp +=  NEWLINE + gDLL->getText("TXT_KEY_CITY_BAR_AIR_UNIT_CAPACITY", iNumUnits, pCity->getAirUnitCapacity(GC.getGameINLINE().getActiveTeam()));

					if (bFull)
					{
						strHelp += ENDCOLR;
					}

					strHelp += NEWLINE;
				}
			}
		}
	}
}

void CvGameTextMgr::getNukePlotHelp(CvPlot* pPlot, CvWString& strHelp)
{
	if (NULL != pPlot)
	{
		CvUnit* pHeadSelectedUnit = gDLL->getInterfaceIFace()->getHeadSelectedUnit();

		if (NULL != pHeadSelectedUnit)
		{
			for (int iI = 0; iI < MAX_TEAMS; iI++)
			{
				if (pHeadSelectedUnit->isNukeVictim(pPlot, ((TeamTypes)iI)))
				{
					if (!pHeadSelectedUnit->isEnemy((TeamTypes)iI))
					{
						strHelp +=  NEWLINE + gDLL->getText("TXT_KEY_CANT_NUKE_FRIENDS");
						break;
					}
				}
			}
		}
	}
}

void CvGameTextMgr::getInterfaceCenterText(CvWString& strText)
{
	strText.clear();
	if (!gDLL->getInterfaceIFace()->isCityScreenUp())
	{
		if (GC.getGameINLINE().getWinner() != NO_TEAM)
		{
			strText = gDLL->getText("TXT_KEY_MISC_WINS_VICTORY", GET_TEAM(GC.getGameINLINE().getWinner()).getName().GetCString(), GC.getVictoryInfo(GC.getGameINLINE().getVictory()).getTextKeyWide());
		}
		else if (!(GET_PLAYER(GC.getGameINLINE().getActivePlayer()).isAlive()))
		{
			strText = gDLL->getText("TXT_KEY_MISC_DEFEAT");
		}
	}
}

void CvGameTextMgr::getTurnTimerText(CvWString& strText)
{
	strText.clear();
	if (gDLL->getInterfaceIFace()->getShowInterface() == INTERFACE_SHOW || gDLL->getInterfaceIFace()->getShowInterface() == INTERFACE_ADVANCED_START)
	{
		if (GC.getGameINLINE().isMPOption(MPOPTION_TURN_TIMER))
		{
			// Get number of turn slices remaining until end-of-turn
			int iTurnSlicesRemaining = GC.getGameINLINE().getTurnSlicesRemaining();

			if (iTurnSlicesRemaining > 0)
			{
				// Get number of seconds remaining
				int iTurnSecondsRemaining = ((int)floorf((float)(iTurnSlicesRemaining-1) * ((float)gDLL->getMillisecsPerTurn()/1000.0f)) + 1);
				int iTurnMinutesRemaining = (int)(iTurnSecondsRemaining/60);
				iTurnSecondsRemaining = (iTurnSecondsRemaining%60);
				int iTurnHoursRemaining = (int)(iTurnMinutesRemaining/60);
				iTurnMinutesRemaining = (iTurnMinutesRemaining%60);

				// Display time remaining
				CvWString szTempBuffer;
				szTempBuffer.Format(L"%d:%02d:%02d", iTurnHoursRemaining, iTurnMinutesRemaining, iTurnSecondsRemaining);
				strText += szTempBuffer;
			}
			else
			{
				// Flash zeroes
				if (iTurnSlicesRemaining % 2 == 0)
				{
					// Display 0
					strText+=L"0:00";
				}
			}
		}

		if (GC.getGameINLINE().getGameState() == GAMESTATE_ON)
		{
			int iMinVictoryTurns = MAX_INT;
			for (int i = 0; i < GC.getNumVictoryInfos(); ++i)
			{
				TeamTypes eActiveTeam = GC.getGameINLINE().getActiveTeam();
				if (NO_TEAM != eActiveTeam)
				{
					int iCountdown = GET_TEAM(eActiveTeam).getVictoryCountdown((VictoryTypes)i);
					if (iCountdown > 0 && iCountdown < iMinVictoryTurns)
					{
						iMinVictoryTurns = iCountdown;
					}
				}
			}

			if (GC.getGameINLINE().isOption(GAMEOPTION_ADVANCED_START) && !GC.getGameINLINE().isOption(GAMEOPTION_ALWAYS_WAR) && GC.getGameINLINE().getElapsedGameTurns() <= GC.getDefineINT("PEACE_TREATY_LENGTH"))
			{
				if (!strText.empty())
				{
					strText += L" -- ";
				}

				strText += gDLL->getText("TXT_KEY_MISC_ADVANCED_START_PEACE_REMAINING", GC.getDefineINT("PEACE_TREATY_LENGTH") - GC.getGameINLINE().getElapsedGameTurns());
			}
			else if (iMinVictoryTurns < MAX_INT)
			{
				if (!strText.empty())
				{
					strText += L" -- ";
				}

				strText += gDLL->getText("TXT_KEY_MISC_TURNS_LEFT_TO_VICTORY", iMinVictoryTurns);
			}
			else if (GC.getGameINLINE().getMaxTurns() > 0)
			{
				if ((GC.getGameINLINE().getElapsedGameTurns() >= (GC.getGameINLINE().getMaxTurns() - 100)) && (GC.getGameINLINE().getElapsedGameTurns() < GC.getGameINLINE().getMaxTurns()))
				{
					if (!strText.empty())
					{
						strText += L" -- ";
					}

					strText += gDLL->getText("TXT_KEY_MISC_TURNS_LEFT", (GC.getGameINLINE().getMaxTurns() - GC.getGameINLINE().getElapsedGameTurns()));
				}
			}
		}
	}
}


void CvGameTextMgr::getFontSymbols(std::vector< std::vector<wchar> >& aacSymbols, std::vector<int>& aiMaxNumRows)
{
	aacSymbols.push_back(std::vector<wchar>());
	aiMaxNumRows.push_back(1);
	for (int iI = 0; iI < NUM_YIELD_TYPES; iI++)
	{
		aacSymbols[aacSymbols.size() - 1].push_back((wchar) GC.getYieldInfo((YieldTypes) iI).getChar());
	}

	aacSymbols.push_back(std::vector<wchar>());
	aiMaxNumRows.push_back(2);
	for (int iI = 0; iI < NUM_COMMERCE_TYPES; iI++)
	{
		aacSymbols[aacSymbols.size() - 1].push_back((wchar) GC.getCommerceInfo((CommerceTypes) iI).getChar());
	}

	aacSymbols.push_back(std::vector<wchar>());
	aiMaxNumRows.push_back(2);
	for (int iI = 0; iI < GC.getNumReligionInfos(); iI++)
	{
		aacSymbols[aacSymbols.size() - 1].push_back((wchar) GC.getReligionInfo((ReligionTypes) iI).getChar());
		aacSymbols[aacSymbols.size() - 1].push_back((wchar) GC.getReligionInfo((ReligionTypes) iI).getHolyCityChar());
	}
	for (int iI = 0; iI < GC.getNumCorporationInfos(); iI++)
	{
		aacSymbols[aacSymbols.size() - 1].push_back((wchar) GC.getCorporationInfo((CorporationTypes) iI).getChar());
		aacSymbols[aacSymbols.size() - 1].push_back((wchar) GC.getCorporationInfo((CorporationTypes) iI).getHeadquarterChar());
	}

	aacSymbols.push_back(std::vector<wchar>());
	aiMaxNumRows.push_back(3);
	for (int iI = 0; iI < GC.getNumBonusInfos(); iI++)
	{
		aacSymbols[aacSymbols.size() - 1].push_back((wchar) GC.getBonusInfo((BonusTypes) iI).getChar());
	}

	aacSymbols.push_back(std::vector<wchar>());
	aiMaxNumRows.push_back(3);
	for (int iI = 0; iI < MAX_NUM_SYMBOLS; iI++)
	{
		aacSymbols[aacSymbols.size() - 1].push_back((wchar) gDLL->getSymbolID(iI));
	}
}

void CvGameTextMgr::assignFontIds(int iFirstSymbolCode, int iPadAmount)
{
	int iCurSymbolID = iFirstSymbolCode;

	// set yield symbols
	for (int i = 0; i < NUM_YIELD_TYPES; i++)
	{
		GC.getYieldInfo((YieldTypes) i).setChar(iCurSymbolID);
		++iCurSymbolID;
	}

	do
	{
		++iCurSymbolID;
	} while (iCurSymbolID % iPadAmount != 0);

	// set commerce symbols
	for (int i=0;i<GC.getNUM_COMMERCE_TYPES();i++)
	{
		GC.getCommerceInfo((CommerceTypes) i).setChar(iCurSymbolID);
		++iCurSymbolID;
	}

	do
	{
		++iCurSymbolID;
	} while (iCurSymbolID % iPadAmount != 0);

	if (NUM_COMMERCE_TYPES < iPadAmount)
	{
		do
		{
			++iCurSymbolID;
		} while (iCurSymbolID % iPadAmount != 0);
	}

	for (int i = 0; i < GC.getNumReligionInfos(); i++)
	{
		GC.getReligionInfo((ReligionTypes) i).setChar(iCurSymbolID);
		++iCurSymbolID;
		GC.getReligionInfo((ReligionTypes) i).setHolyCityChar(iCurSymbolID);
		++iCurSymbolID;
	}
	for (int i = 0; i < GC.getNumCorporationInfos(); i++)
	{
		GC.getCorporationInfo((CorporationTypes) i).setChar(iCurSymbolID);
		++iCurSymbolID;
		GC.getCorporationInfo((CorporationTypes) i).setHeadquarterChar(iCurSymbolID);
		++iCurSymbolID;
	}

	do
	{
		++iCurSymbolID;
	} while (iCurSymbolID % iPadAmount != 0);

	if (2 * (GC.getNumReligionInfos() + GC.getNumCorporationInfos()) < iPadAmount)
	{
		do
		{
			++iCurSymbolID;
		} while (iCurSymbolID % iPadAmount != 0);
	}

// modified Sephi
// Symbol loading adjusted to WoC.
	int iSavePosition=iCurSymbolID;

	// set bonus symbols
	int bonusBaseID = iCurSymbolID;
	++iCurSymbolID;
	for (int i = 0; i < GC.getNumBonusInfos(); i++)
	{
		int bonusID = bonusBaseID + GC.getBonusInfo((BonusTypes) i).getArtInfo()->getFontButtonIndex();
		GC.getBonusInfo((BonusTypes) i).setChar(bonusID);
		++iCurSymbolID;
	}

	iCurSymbolID=iSavePosition+125;
	iCurSymbolID-=MAX_NUM_SYMBOLS;
	do
	{
		--iCurSymbolID;
	} while (iCurSymbolID % iPadAmount != 0);

// modified Sephi



	// set extra symbols
	for (int i=0; i < MAX_NUM_SYMBOLS; i++)
	{
		gDLL->setSymbolID(i, iCurSymbolID);
		++iCurSymbolID;
	}
}

void CvGameTextMgr::getCityDataForAS(std::vector<CvWBData>& mapCityList, std::vector<CvWBData>& mapBuildingList, std::vector<CvWBData>& mapAutomateList)
{
	CvPlayer& kActivePlayer = GET_PLAYER(GC.getGameINLINE().getActivePlayer());

	CvWString szHelp;
	int iCost = kActivePlayer.getAdvancedStartCityCost(true);
	if (iCost > 0)
	{
		szHelp = gDLL->getText("TXT_KEY_CITY");
		szHelp += gDLL->getText("TXT_KEY_AS_UNREMOVABLE");
		mapCityList.push_back(CvWBData(0, szHelp, ARTFILEMGR.getInterfaceArtInfo("INTERFACE_BUTTONS_CITYSELECTION")->getPath()));
	}

	iCost = kActivePlayer.getAdvancedStartPopCost(true);
	if (iCost > 0)
	{
		szHelp = gDLL->getText("TXT_KEY_WB_AS_POPULATION");
		mapCityList.push_back(CvWBData(1, szHelp, ARTFILEMGR.getInterfaceArtInfo("INTERFACE_ANGRYCITIZEN_TEXTURE")->getPath()));
	}

	iCost = kActivePlayer.getAdvancedStartCultureCost(true);
	if (iCost > 0)
	{
		szHelp = gDLL->getText("TXT_KEY_ADVISOR_CULTURE");
		szHelp += gDLL->getText("TXT_KEY_AS_UNREMOVABLE");
		mapCityList.push_back(CvWBData(2, szHelp, ARTFILEMGR.getInterfaceArtInfo("CULTURE_BUTTON")->getPath()));
	}

	CvWStringBuffer szBuffer;
	for(int i = 0; i < GC.getNumBuildingClassInfos(); i++)
	{
		BuildingTypes eBuilding = (BuildingTypes) GC.getCivilizationInfo(kActivePlayer.getCivilizationType()).getCivilizationBuildings(i);

		if (eBuilding != NO_BUILDING)
		{
			if (GC.getBuildingInfo(eBuilding).getFreeStartEra() == NO_ERA || GC.getGameINLINE().getStartEra() < GC.getBuildingInfo(eBuilding).getFreeStartEra())
			{
				// Building cost -1 denotes unit which may not be purchased
				iCost = kActivePlayer.getAdvancedStartBuildingCost(eBuilding, true);
				if (iCost > 0)
				{
					szBuffer.clear();
					setBuildingHelp(szBuffer, eBuilding);
					mapBuildingList.push_back(CvWBData(eBuilding, szBuffer.getCString(), GC.getBuildingInfo(eBuilding).getButton()));
				}
			}
		}
	}

	szHelp = gDLL->getText("TXT_KEY_ACTION_AUTOMATE_BUILD");
	mapAutomateList.push_back(CvWBData(0, szHelp, ARTFILEMGR.getInterfaceArtInfo("INTERFACE_AUTOMATE")->getPath()));
}

void CvGameTextMgr::getUnitDataForAS(std::vector<CvWBData>& mapUnitList)
{
	CvPlayer& kActivePlayer = GET_PLAYER(GC.getGameINLINE().getActivePlayer());

	CvWStringBuffer szBuffer;
	for(int i = 0; i < GC.getNumUnitClassInfos(); i++)
	{
		UnitTypes eUnit = (UnitTypes) GC.getCivilizationInfo(kActivePlayer.getCivilizationType()).getCivilizationUnits(i);
		if (eUnit != NO_UNIT)
		{
			// Unit cost -1 denotes unit which may not be purchased
			int iCost = kActivePlayer.getAdvancedStartUnitCost(eUnit, true);
			if (iCost > 0)
			{
				szBuffer.clear();
				setUnitHelp(szBuffer, eUnit);

				int iMaxUnitsPerCity = GC.getDefineINT("ADVANCED_START_MAX_UNITS_PER_CITY");
				if (iMaxUnitsPerCity >= 0 && GC.getUnitInfo(eUnit).isMilitarySupport())
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_WB_AS_MAX_UNITS_PER_CITY", iMaxUnitsPerCity));
				}
				mapUnitList.push_back(CvWBData(eUnit, szBuffer.getCString(), kActivePlayer.getUnitButton(eUnit)));
			}
		}
	}
}

void CvGameTextMgr::getImprovementDataForAS(std::vector<CvWBData>& mapImprovementList, std::vector<CvWBData>& mapRouteList)
{
	CvPlayer& kActivePlayer = GET_PLAYER(GC.getGameINLINE().getActivePlayer());

	for (int i = 0; i < GC.getNumRouteInfos(); i++)
	{
		RouteTypes eRoute = (RouteTypes) i;
		if (eRoute != NO_ROUTE)
		{
			// Route cost -1 denotes route which may not be purchased
			int iCost = kActivePlayer.getAdvancedStartRouteCost(eRoute, true);
			if (iCost > 0)
			{
				mapRouteList.push_back(CvWBData(eRoute, GC.getRouteInfo(eRoute).getDescription(), GC.getRouteInfo(eRoute).getButton()));
			}
		}
	}

	CvWStringBuffer szBuffer;
	for (int i = 0; i < GC.getNumImprovementInfos(); i++)
	{
		ImprovementTypes eImprovement = (ImprovementTypes) i;
		if (eImprovement != NO_IMPROVEMENT)
		{
			// Improvement cost -1 denotes Improvement which may not be purchased
			int iCost = kActivePlayer.getAdvancedStartImprovementCost(eImprovement, true);
			if (iCost > 0)
			{
				szBuffer.clear();
				setImprovementHelp(szBuffer, eImprovement);
				mapImprovementList.push_back(CvWBData(eImprovement, szBuffer.getCString(), GC.getImprovementInfo(eImprovement).getButton()));
			}
		}
	}
}

void CvGameTextMgr::getVisibilityDataForAS(std::vector<CvWBData>& mapVisibilityList)
{
	// Unit cost -1 denotes unit which may not be purchased
	int iCost = GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getAdvancedStartVisibilityCost(true);
	if (iCost > 0)
	{
		CvWString szHelp = gDLL->getText("TXT_KEY_WB_AS_VISIBILITY");
		szHelp += gDLL->getText("TXT_KEY_AS_UNREMOVABLE", iCost);
		mapVisibilityList.push_back(CvWBData(0, szHelp, ARTFILEMGR.getInterfaceArtInfo("INTERFACE_TECH_LOS")->getPath()));
	}
}

void CvGameTextMgr::getTechDataForAS(std::vector<CvWBData>& mapTechList)
{
	mapTechList.push_back(CvWBData(0, gDLL->getText("TXT_KEY_WB_AS_TECH"), ARTFILEMGR.getInterfaceArtInfo("INTERFACE_BTN_TECH")->getPath()));
}

void CvGameTextMgr::getUnitDataForWB(std::vector<CvWBData>& mapUnitData)
{
	CvWStringBuffer szBuffer;
	for (int i = 0; i < GC.getNumUnitInfos(); i++)
	{
		szBuffer.clear();
		setUnitHelp(szBuffer, (UnitTypes)i);
		mapUnitData.push_back(CvWBData(i, szBuffer.getCString(), GC.getUnitInfo((UnitTypes)i).getButton()));
	}
}

void CvGameTextMgr::getBuildingDataForWB(bool bStickyButton, std::vector<CvWBData>& mapBuildingData)
{
	int iCount = 0;
	if (!bStickyButton)
	{
		mapBuildingData.push_back(CvWBData(iCount++, GC.getMissionInfo(MISSION_FOUND).getDescription(), GC.getMissionInfo(MISSION_FOUND).getButton()));
	}

	CvWStringBuffer szBuffer;
	for (int i=0; i < GC.getNumBuildingInfos(); i++)
	{
		szBuffer.clear();
		setBuildingHelp(szBuffer, (BuildingTypes)i);
		mapBuildingData.push_back(CvWBData(iCount++, szBuffer.getCString(), GC.getBuildingInfo((BuildingTypes)i).getButton()));
	}
}

void CvGameTextMgr::getTerrainDataForWB(std::vector<CvWBData>& mapTerrainData, std::vector<CvWBData>& mapFeatureData, std::vector<CvWBData>& mapPlotData, std::vector<CvWBData>& mapRouteData)
{
	CvWStringBuffer szBuffer;

	for (int i = 0; i < GC.getNumTerrainInfos(); i++)
	{
		if (!GC.getTerrainInfo((TerrainTypes)i).isGraphicalOnly())
		{
			szBuffer.clear();
			setTerrainHelp(szBuffer, (TerrainTypes)i);
			mapTerrainData.push_back(CvWBData(i, szBuffer.getCString(), GC.getTerrainInfo((TerrainTypes)i).getButton()));
		}
	}

	for (int i = 0; i < GC.getNumFeatureInfos(); i++)
	{
		for (int k = 0; k < GC.getFeatureInfo((FeatureTypes)i).getArtInfo()->getNumVarieties(); k++)
		{
			szBuffer.clear();
			setFeatureHelp(szBuffer, (FeatureTypes)i);
			mapFeatureData.push_back(CvWBData(i + GC.getNumFeatureInfos() * k, szBuffer.getCString(), GC.getFeatureInfo((FeatureTypes)i).getArtInfo()->getVariety(k).getVarietyButton()));
		}
	}

	mapPlotData.push_back(CvWBData(0, gDLL->getText("TXT_KEY_WB_PLOT_TYPE_MOUNTAIN"), ARTFILEMGR.getInterfaceArtInfo("WORLDBUILDER_PLOT_TYPE_MOUNTAIN")->getPath()));
	mapPlotData.push_back(CvWBData(1, gDLL->getText("TXT_KEY_WB_PLOT_TYPE_HILL"), ARTFILEMGR.getInterfaceArtInfo("WORLDBUILDER_PLOT_TYPE_HILL")->getPath()));
	mapPlotData.push_back(CvWBData(2, gDLL->getText("TXT_KEY_WB_PLOT_TYPE_PLAINS"), ARTFILEMGR.getInterfaceArtInfo("WORLDBUILDER_PLOT_TYPE_PLAINS")->getPath()));
	mapPlotData.push_back(CvWBData(3, gDLL->getText("TXT_KEY_WB_PLOT_TYPE_OCEAN"), ARTFILEMGR.getInterfaceArtInfo("WORLDBUILDER_PLOT_TYPE_OCEAN")->getPath()));

	for (int i = 0; i < GC.getNumRouteInfos(); i++)
	{
		mapRouteData.push_back(CvWBData(i, GC.getRouteInfo((RouteTypes)i).getDescription(), GC.getRouteInfo((RouteTypes)i).getButton()));
	}
	mapRouteData.push_back(CvWBData(GC.getNumRouteInfos(), gDLL->getText("TXT_KEY_WB_RIVER_PLACEMENT"), ARTFILEMGR.getInterfaceArtInfo("WORLDBUILDER_RIVER_PLACEMENT")->getPath()));
}

void CvGameTextMgr::getTerritoryDataForWB(std::vector<CvWBData>& mapTerritoryData)
{
	for (int i = 0; i <= MAX_CIV_PLAYERS; i++)
	{
		CvWString szName = gDLL->getText("TXT_KEY_MAIN_MENU_NONE");
		CvString szButton = GC.getCivilizationInfo(GET_PLAYER(BARBARIAN_PLAYER).getCivilizationType()).getButton();

		if (GET_PLAYER((PlayerTypes) i).isEverAlive())
		{
			szName = GET_PLAYER((PlayerTypes)i).getName();
			szButton = GC.getCivilizationInfo(GET_PLAYER((PlayerTypes)i).getCivilizationType()).getButton();
		}
		mapTerritoryData.push_back(CvWBData(i, szName, szButton));
	}
}


void CvGameTextMgr::getTechDataForWB(std::vector<CvWBData>& mapTechData)
{
	CvWStringBuffer szBuffer;
	for (int i=0; i < GC.getNumTechInfos(); i++)
	{
		szBuffer.clear();
		setTechHelp(szBuffer, (TechTypes) i);
		mapTechData.push_back(CvWBData(i, szBuffer.getCString(), GC.getTechInfo((TechTypes) i).getButton()));
	}
}

void CvGameTextMgr::getPromotionDataForWB(std::vector<CvWBData>& mapPromotionData)
{
	CvWStringBuffer szBuffer;
	for (int i=0; i < GC.getNumPromotionInfos(); i++)
	{
		szBuffer.clear();
		setPromotionHelp(szBuffer, (PromotionTypes) i, false);
		mapPromotionData.push_back(CvWBData(i, szBuffer.getCString(), GC.getPromotionInfo((PromotionTypes) i).getButton()));
	}
}

void CvGameTextMgr::getBonusDataForWB(std::vector<CvWBData>& mapBonusData)
{
	CvWStringBuffer szBuffer;
	for (int i=0; i < GC.getNumBonusInfos(); i++)
	{
		szBuffer.clear();
		setBonusHelp(szBuffer, (BonusTypes)i);
		mapBonusData.push_back(CvWBData(i, szBuffer.getCString(), GC.getBonusInfo((BonusTypes) i).getButton()));
	}
}

void CvGameTextMgr::getImprovementDataForWB(std::vector<CvWBData>& mapImprovementData)
{
	CvWStringBuffer szBuffer;
	for (int i=0; i < GC.getNumImprovementInfos(); i++)
	{
		CvImprovementInfo& kInfo = GC.getImprovementInfo((ImprovementTypes) i);
		if (!kInfo.isGraphicalOnly())
		{
			szBuffer.clear();
			setImprovementHelp(szBuffer, (ImprovementTypes) i);
			mapImprovementData.push_back(CvWBData(i, szBuffer.getCString(), kInfo.getButton()));
		}
	}
}

void CvGameTextMgr::getReligionDataForWB(bool bHolyCity, std::vector<CvWBData>& mapReligionData)
{
	for (int i = 0; i < GC.getNumReligionInfos(); ++i)
	{
		CvReligionInfo& kInfo = GC.getReligionInfo((ReligionTypes) i);
		CvWString strDescription = kInfo.getDescription();
		if (bHolyCity)
		{
			strDescription = gDLL->getText("TXT_KEY_WB_HOLYCITY", strDescription.GetCString());
		}
		mapReligionData.push_back(CvWBData(i, strDescription, kInfo.getButton()));
	}
}


void CvGameTextMgr::getCorporationDataForWB(bool bHeadquarters, std::vector<CvWBData>& mapCorporationData)
{
	for (int i = 0; i < GC.getNumCorporationInfos(); ++i)
	{
		CvCorporationInfo& kInfo = GC.getCorporationInfo((CorporationTypes) i);
		CvWString strDescription = kInfo.getDescription();
		if (bHeadquarters)
		{
			strDescription = gDLL->getText("TXT_KEY_CORPORATION_HEADQUARTERS", strDescription.GetCString());
		}
		mapCorporationData.push_back(CvWBData(i, strDescription, kInfo.getButton()));
	}
}
