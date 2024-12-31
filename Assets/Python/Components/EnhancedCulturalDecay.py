## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
##
## CulturalDecay by Bhruic
## EnhancedCulturalDecay by The Lopez
## Adapted for Total Realism by Mexico
## Converted to BUG module by Mexico
##

from CvPythonExtensions import *
import BugUtil
import BugCore
import PyHelpers
import DataStorage

## Globals
gc = CyGlobalContext()
data = DataStorage.DataStorage()
PyGame = PyHelpers.PyGame()
opt = BugCore.game.EnhancedCulturalDecay
gDistance = []

## Internal functions
def init():
	global gDistance
	gDistance = [[0] * 11 for i in range(11)]
	for iX in range(11):
		for iY in range(11):
			if iX in [0, 1] and iY in [0, 1]:
				gDistance[iX][iY] = 1
			else:
				gDistance[iX][iY] = int((iX**2 + iY**2)**0.5 + (0.5**2 + 0.5**2)**0.5)

def getRadius(pCity):
	iRadius = 0
	pGame = gc.getGame()

	for iCultureLevelInfo in range(gc.getNumCultureLevelInfos()):
		pCultureLevelInfo = gc.getCultureLevelInfo(iCultureLevelInfo)			
		if (pCity.getCulture(pCity.getOwner()) < pCultureLevelInfo.getSpeedThreshold(pGame.getGameSpeedType())):
			iRadius = iCultureLevelInfo - 1
			break
			
	if (iRadius == 0):
		iRadius = iCultureLevelInfo

	return iRadius

def doCultureAdd(pCity, iRadius):
	global gDistance
	iCulture = pCity.getCommerceRate(gc.getInfoTypeForString('COMMERCE_CULTURE'))
	iCityX = pCity.getX()
	iCityY = pCity.getY()
	iRange = (2 * iRadius) + 1
	iCityId = pCity.getID()

	pMap = CyMap()

	for iRangeX in range(iRange):
		for iRangeY in range(iRange):
			iX = iRangeX - iRadius + iCityX
			iY = iRangeY - iRadius + iCityY

			iDistanceX = abs(iRangeX - iRadius)
			iDistanceY = abs(iRangeY - iRadius)
			
			if(pMap.isPlot(iX, iY) and iRadius >= gDistance[iDistanceX][iDistanceY]):
				objPlot = pMap.plot(iX, iY)
			
				iCultureChange = ((iRadius - gDistance[iDistanceX][iDistanceY]) * 20 + iCulture + 1)
				iCurrentCulture = data.getSingleVal('CulturalDecay', iCityId, objPlot)
				
				if (iCurrentCulture > -1):
					iCultureChange += iCurrentCulture
				
				data.setSingleVal('CulturalDecay', iCityId, iCultureChange, objPlot)

def decayCulture():
	pMap = CyMap()
	iTotalPlots = pMap.numPlots()
	
	playerList = PyGame.getCivPlayerList()

	for i in range(iTotalPlots):
		objPlot = pMap.plotByIndex(i)
		
		for objPlayer in playerList:
			iPlayerId = objPlayer.getID()
			iCulture = objPlot.getCulture(iPlayerId)
			iNumCultureCities = objPlot.getNumCultureRangeCities(iPlayerId)
		
			if(iCulture > 0 and iNumCultureCities == 0):
				if objPlot.isCity():
					iCultureDecay = 0
				else:
					iCultureDecay = max(((int)(iCulture * opt.getCultureDecayAmount())), 1)

				objPlot.changeCulture(iPlayerId, -iCultureDecay, False)

def doCultureShock(pCity, iRadius):
	iCityX = pCity.getX()
	iCityY = pCity.getY()
	iRange = (2 * iRadius) + 1
	iCityId = pCity.getID()
	iCityOwnerId = pCity.getOwner()
	pGame = gc.getGame()
	pMap = CyMap()
	global gDistance

	for iRangeX in range(iRange):
		for iRangeY in range(iRange):
			iX = iRangeX - iRadius + iCityX
			iY = iRangeY - iRadius + iCityY
			iDistanceX = abs(iRangeX - iRadius)
			iDistanceY = abs(iRangeY - iRadius)

			if (pMap.isPlot(iX, iY) and iRadius >= gDistance[iDistanceX][iDistanceY]):
				objPlot = pMap.plot(iX, iY)
				iCulture = objPlot.getCulture(iCityOwnerId)
				iCityCulture = data.getSingleVal('CulturalDecay', iCityId, objPlot)
				if (iCityCulture > -1):
					iRandNum = pGame.getSorenRandNum(11, "CulturalDecay")
					fRandNum = float(iRandNum) / 100 + 0.85
					iCultureChange = int(iCityCulture * fRandNum)
				else:
					iCultureChange = 0

				if (iCultureChange > iCulture):
					iCultureChange = iCulture

				if (iCultureChange > 0):
					objPlot.changeCulture(iCityOwnerId, -iCultureChange, False)

				data.setSingleVal('CulturalDecay', iCityId, 0, objPlot)
				
## Event handlers
def onCityBuilt(argsList):
	pCity = argsList[0]
	doCultureAdd(pCity, getRadius(pCity))
	
def onCityDoTurn(argsList):
	pCity = argsList[0]
	doCultureAdd(pCity, getRadius(pCity))		

def onCityLost(argsList):
	if(opt.isEnableCultureShock()):
		pCity = argsList[0]
		doCultureShock(pCity, getRadius(pCity))

def onEndGameTurn(argsList):
	if (opt.isEnableCultureDecay()):
		decayCulture()
