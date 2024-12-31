## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
from CvPythonExtensions import *
import CvUtil
import ScreenInput
import CvScreenEnums

# globals
gc = CyGlobalContext()

class IDW:
		
	def __init__(self):
		self.iBase			= 2	## Base Value directly on Winner Plot and Loser Plot
		self.iLevelBonus		= 10	## X% Bonus for every level increase
		self.iEraBonus		= 10	## X% Bonus for every era advance
		self.iLeaderBonus		= 25	## X% Bonus for led by warlords
		self.iAngerPercent		= 50	## X% Auto Conscript Anger Length
		self.iPenalty		= gc.getInfoTypeForString("PROMOTION_PENALTY")

	def checkCulture(self, pWinner, pLoser):
		if pLoser.getUnitCombatType() == -1: return			## Non Combat Units
		if pLoser.getDomainType() != (2 or 3): return	## Land combat only
		pWinner.setHasPromotion(self.iPenalty, False)
		iGameSpeedMod = gc.getGameSpeedInfo(CyGame().getGameSpeedType()).getUnitGreatWorkPercent()

		iWinPlayer = pWinner.getOwner()
		iWinPercent = 100
		iWinPercent += (pWinner.getLevel() - 1) * self.iLevelBonus
		iWinPercent += gc.getPlayer(iWinPlayer).getCurrentEra() * self.iEraBonus
		if pWinner.getLeaderUnitType() > -1:
			iWinPercent += self.iLeaderBonus
		iWinCulture = iWinPercent * self.iBase * iGameSpeedMod / 25000	

		iLosePlayer = pLoser.getOwner()
		iLosePercent = 90
		iLosePercent += pLoser.getLevel() * self.iLevelBonus
		iLosePercent += gc.getPlayer(iLosePlayer).getCurrentEra() * self.iEraBonus
		if pLoser.getLeaderUnitType() > -1:
			iLosePercent += self.iLeaderBonus
		iLoseCulture = iLosePercent * self.iBase * iGameSpeedMod / 25000

		self.changeCulture(pWinner.plot(), iWinCulture, iWinPlayer, iLosePlayer)
		self.changeCulture(pLoser.plot(), iLoseCulture, iWinPlayer, iLosePlayer)
		self.checkCity(pLoser.plot(), pWinner, iLosePlayer)

	def checkCity(self, pPlot, pWinner, iLosePlayer):
		if pPlot.getNumVisibleEnemyDefenders(pWinner) > 1: return
		if pPlot.isCity():
			pCity = pPlot.getPlotCity()
			if pCity.isDisorder(): return
			iMaxRandom = (gc.getNumCultureLevelInfos() - 1) * 100
			if CyGame().getSorenRandNum(iMaxRandom, "Resistance Chance") >= pCity.getCultureLevel() * pPlot.calculateCulturePercent(iLosePlayer): return
			iConscript = pCity.getConscriptUnit()
			if iConscript == -1: return
			iPopulationCost = max(1, (gc.getUnitInfo(iConscript).getProductionCost() / gc.getDefineINT("CONSCRIPT_POPULATION_PER_COST")))
			iMinPopulation = max(1, gc.getDefineINT("CONSCRIPT_MIN_CITY_POPULATION"))
			if pCity.getPopulation() - iPopulationCost < iMinPopulation: return
			pCity.changePopulation(- iPopulationCost)
			pNewUnit = gc.getPlayer(iLosePlayer).initUnit(iConscript, pCity.getX(), pCity.getY(), UnitAITypes.NO_UNITAI, DirectionTypes.NO_DIRECTION)
			pNewUnit.finishMoves()
			pNewUnit.setHasPromotion(self.iPenalty, True)
			pCity.addProductionExperience(pNewUnit, True)
			iAngerTimer = gc.getDefineINT("CONSCRIPT_ANGER_DIVISOR") * gc.getGameSpeedInfo(CyGame().getGameSpeedType()).getHurryConscriptAngerPercent() * self.iAngerPercent / 10000
			pCity.changeConscriptAngerTimer(max(0, iAngerTimer))
			CyInterface().addMessage(pWinner.getOwner(),True,15,CyTranslator().getText("TXT_AUTO_DRAFT",(pNewUnit.getName(), pCity.getName(),)),'',0, pNewUnit.getButton(),gc.getInfoTypeForString("COLOR_WARNING_TEXT"),pCity.getX(), pCity.getY(), True,True)
			CyInterface().addMessage(iLosePlayer,True,15,CyTranslator().getText("TXT_AUTO_DRAFT",(pNewUnit.getName(), pCity.getName(),)),'',0, pNewUnit.getButton(),gc.getInfoTypeForString("COLOR_POSITIVE_TEXT"),pCity.getX(), pCity.getY(), True,True)								

	def changeCulture(self, pPlot, iCulture, iWinPlayer, iLosePlayer):
		for x in xrange(pPlot.getX() - 2, pPlot.getX() + 3):
			for y in xrange(pPlot.getY() - 2, pPlot.getY() + 3):
				pLoopPlot = CyMap().plot(x,y)
				iDistanceX = abs(pPlot.getX() - x)
				iDistanceY = abs(pPlot.getY() - y)
				if iDistanceX == 2 and iDistanceY == 2: continue
				iLoserCulture = pLoopPlot.getCulture(iLosePlayer)
				if iLoserCulture == 0: continue
				iNewCulture = iCulture * iLoserCulture / 100
				iRadius = max(iDistanceX, iDistanceY)
				if iRadius == 0:
					iChange = min(iNewCulture, iLoserCulture)
				elif iRadius == 1:
					iChange = min(iNewCulture/2, iLoserCulture)
				else:
					iChange = min(iNewCulture/4, iLoserCulture)
				pLoopPlot.changeCulture(iWinPlayer, iChange, True)
				pLoopPlot.changeCulture(iLosePlayer, - iChange, True)