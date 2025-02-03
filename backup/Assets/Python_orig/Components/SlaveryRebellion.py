# Sid Meier's Civilization 4
# Copyright Firaxis Games 2005
#
#
# Slavery Rebellion component by Mexico
# ver 0.1 
# ver 0.2 (24.05.2006) - resolved bug when rebel unit is spawned on wrong place (ice/peak/ocean)
# ver 0.2a (25.05.2006) - just only small update for plot validity checking (using API fuctions rather then directly checking for value)
# ver 0.2b (25.05.2006) - ehm...typo error fixing in API  calling (i'm so tired..), reduced chance of rebellion to 7% (was 10%)
#ver 0.2c (28.05.2006) - cities smaller than  pop 3 do not fall to rebellion,chance reduced to 5%
#ver 0.2d (17.06.2006) - solved bug, when message about rebellion was showed for bad player, also solved bug, which generate python
#                                         exception when player 0 was destroyed
#ver 0.3 (30.11.2006) - execution moved to onCityDoTurn event, barbarian civ no more revolting,  reduced slavery rebelion possibility to 3%, use TXT_KEY_..... for message
#ver 0.4 (5.12.2006) - used new API function CyUnit::setIgnoreObstacle(bool), so rebel units, still barbarian, ignore great wall
#ver 0.5 (10.12.2006) - possibility to slavery revolt increased with classical democracy civic
#ver 0.5a (11.12.2006) - added peasant revolt possibility (under serfdom civic) 2% possibility
#ver 0.6 (march 2007) - lowered base chance to 2,5% (1,5%), used new unique revolting  units by Houman
#ver 0.6a (6.3.2007) - some cleanup by Mexico (removed unused functions and lists)
# wroted as part of Realism mod

from CvPythonExtensions import *
import CvUtil
import PyHelpers
import Popup as PyPopup
import BugUtil


## Globals

localText = CyTranslator()
gc = CyGlobalContext()
PyInfo = PyHelpers.PyInfo
PyPlayer = PyHelpers.PyPlayer
gDistance = []


## Slavery Rebellion
def initDistanceLookup():
	global gDistance
	gDistance = [[0] * 11 for i in range(11)]
	for iX in range(11):
		for iY in range(11):
			if iX in [0, 1] and iY in [0, 1]:
				gDistance[iX][iY] = 1
			else:
				gDistance[iX][iY] = int((iX**2 + iY**2)**0.5 + (0.5**2 + 0.5**2)**0.5)


def onCityDoTurn(argsList):
	pCity, iPlayer = argsList
	
	if gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_REBELLIONS):
		return

	pPlayer = gc.getPlayer(iPlayer)
	# We don't want barbarians rebelling against barbarians
	if (pPlayer.isBarbarian()):
		return

	# We can only spawn rebels if we are running Slavery or Serfdom
	bSlavery = pPlayer.isCivic(gc.getInfoTypeForString("CIVIC_SLAVERY"))
	bSerfdom = pPlayer.isCivic(gc.getInfoTypeForString("CIVIC_SERFDOM"))
	if (not bSlavery and not bSerfdom):
		return

	# If we're running serfdom, rebellions only happen in cities with manors
	if (bSerfdom):
		iManorClass = CvUtil.findInfoTypeNum(gc.getBuildingClassInfo, gc.getNumBuildingClassInfos(), 'BUILDINGCLASS_MANOR')
		iManor = gc.getCivilizationInfo(gc.getActivePlayer().getCivilizationType()).getCivilizationBuildings(iManorClass)	
		if (not pCity.isHasBuilding(iManor)):
			return

	# Find the barbarian player
	iBarbPlayer = gc.getBARBARIAN_PLAYER()
	if (iBarbPlayer <= 0):
		return

	pBarbPlayer = gc.getPlayer(iBarbPlayer)

	# Determine the maximum number of units that would rebel in this city
	iCityPopulation = pCity.getPopulation()
	if (iCityPopulation < 3):
		iMaxRebels = 1
	else:
		iMaxRebels = iCityPopulation

	if (iMaxRebels == 0):
		return

	# Roll the dice to see if we have a rebellion on our hands
	if (bSlavery):
		iMarketClass = CvUtil.findInfoTypeNum(gc.getBuildingClassInfo, gc.getNumBuildingClassInfos(), 'BUILDINGCLASS_SLAVE_MARKET')
		iMarket = gc.getCivilizationInfo(gc.getActivePlayer().getCivilizationType()).getCivilizationBuildings(iMarketClass)	
		iGladiatorClass = CvUtil.findInfoTypeNum(gc.getBuildingClassInfo, gc.getNumBuildingClassInfos(), 'BUILDINGCLASS_GLADIATOR_SCHOOL')
		iGladiator = gc.getCivilizationInfo(gc.getActivePlayer().getCivilizationType()).getCivilizationBuildings(iGladiatorClass)			
		if (pCity.isHasBuilding(iMarket)):
			iRebellionChance = 30 # Out of 1000 (1.0% chance with slavery and slave market)
		else:
			iRebellionChance = 15 # Out of 1000 (0.5% chance without slave market)
		if (pCity.isHasBuilding(iGladiator)):
			iRebellionChance *= 2
	else:
		iRebellionChance = 15 # Out of 1000 (0.5% chance with serfdom and manor)

	iRand = gc.getGame().getSorenRandNum(3000, "Rebellion possibility")
	if (iRand > iRebellionChance):
		# Sorry kids, no rebellion today
		return

	# Create a list of surrounding plots
	iMaxRadius = 3
	iMinRadius = 2
	surroundingPlots = []

	global gDistance
	if (len(gDistance) == 0):
		initDistanceLookup()

	pMap = gc.getMap()

	iCityPlotX = pCity.getX()
	iCityPlotY = pCity.getY()
	pCityPlot = pMap.plot(iCityPlotX, iCityPlotY)

	iRange = (2 * iMaxRadius) + 1

	for iRangeX in range(iRange):
		for iRangeY in range(iRange):
			iPlotX = iRangeX - iMaxRadius + iCityPlotX
			iPlotY = iRangeY - iMaxRadius + iCityPlotY

			iDistanceX = abs(iRangeX - iMaxRadius)
			iDistanceY = abs(iRangeY - iMaxRadius)
			iDistance = gDistance[iDistanceX][iDistanceY]

			# Find valid plots of appropriate distance
			if (pMap.isPlot(iPlotX, iPlotY) and (iMaxRadius >= iDistance) and (iDistance >= iMinRadius)):
				pPlot = pMap.plot(iPlotX, iPlotY)
				# Make sure they have same owner
				if (pPlot.getOwner() == iPlayer):
					# Make sure you can reach the revolt target
					if (pPlot.getArea() == pCityPlot.getArea()):
						# Make sure it's not a city
						if (not pPlot.isCity()):
							# Only land and hill plots are valid for spawning rebels
							if (pPlot.isFlatlands() or pPlot.isHills()):
								# Don't spawn on tiles with non-barbarian units already on them or on tiles with no reachable cities (usually small islands in the city radius)
								iNumUnits = pPlot.getNumUnits()
								bIsPlotFree = True
								if (iNumUnits > 0):
									for iUnit in range(iNumUnits):
										pUnit = pPlot.getUnit(iUnit)
										if (pUnit.getOwner() != iBarbPlayer):
											bIsPlotFree = False
											break
								if pPlot.area().getNumCities() == 0:
									bIsPlotFree = False
								for iNeighbor in range(gc.getMAX_PLAYERS()):
									pNeighbor = gc.getPlayer(iNeighbor)
									if not pNeighbor.isNone():
										if pNeighbor.isAlive():
											if not (iPlayer == iNeighbor):
												if pPlot.isAdjacentPlayer(iNeighbor, true):
													bIsPlotFree = False
													break
								# If the plot is free then add it to the list
								if bIsPlotFree:
									surroundingPlots.append([iPlotX, iPlotY])

	# If there are no unoccupied tiles around the city then we don't spawn any rebels
	if (len(surroundingPlots) == 0):
		BugUtil.debug("No free land around %s" %(pCity.getName()))
		return

	# Log the rebellion
	szCityName = pCity.getName()
	BugUtil.debug("Slavery/peasant rebellion in %s for player %d" %(szCityName,iPlayer))

	# Spawn units
	if (iMaxRebels > 1):
		iNumRebels = 1 + gc.getGame().getSorenRandNum(iMaxRebels, "Number of extra units spawned by rebellion")
	else:
		iNumRebels = 1
		
	iRandPos = gc.getGame().getSorenRandNum(len(surroundingPlots), "Rebellion spawning plot")
	(iSpawnPlotX, iSpawnPlotY) = surroundingPlots[iRandPos]
		
	iUnitAI = UnitAITypes.UNITAI_ATTACK_CITY_LEMMING	
	
	# Determine rebellion type
	if (bSlavery):
		szRebellionMessage = localText.getText("TXT_KEY_SLAVERY_REBELLION_MESSAGE", (szCityName,))
	else:
		szRebellionMessage = localText.getText("TXT_KEY_PEASANT_REBELLION_MESSAGE", (szCityName,))
	
	for iNum in range(iNumRebels):
		if (bSlavery):
			iUnitType = selectSlaveRebel(pPlayer)
		else:
			iUnitType = selectPeasantRebel(pPlayer)	
		# Spawn the rebel
		pRebellionUnit = pBarbPlayer.initUnit(iUnitType, iSpawnPlotX, iSpawnPlotY, iUnitAI, DirectionTypes.NO_DIRECTION)

		# The unit should not be able to move for a turn
		pRebellionUnit.setImmobileTimer(2)

		# The unit can ignore the great wall
		pRebellionUnit.setIgnoreObstacle(true)

		# Log the unit
		BugUtil.debug("Spawned %s rebel unit near %s" %(pRebellionUnit.getName(), szCityName))
		
		# Report the rebellion
	CyInterface().addMessage(iPlayer, true, gc.getEVENT_MESSAGE_TIME(), szRebellionMessage, "AS2D_REVOLTSTART", InterfaceMessageTypes.MESSAGE_TYPE_INFO, gc.getUnitInfo(iUnitType).getButton(), gc.getInfoTypeForString("COLOR_RED"), iSpawnPlotX, iSpawnPlotY, true, true)
		
		# Determine unit AI
		#iRandAttackCityAI = gc.getGame().getSorenRandNum(100, "Chance of UNITAI_ATTACK_CITY rebellion unit")
		#if (iRandAttackCityAI > 10):
		#	iUnitAI = UnitAITypes.UNITAI_ATTACK_CITY # 90% chance of attack city AI
		#else:
		#	iUnitAI = UnitAITypes.UNITAI_PILLAGE		


def onCombatResult(argsList):
	'Combat Result'
	pWinningUnit, pLosingUnit = argsList

	iWinner = pWinningUnit.getOwner()
	pWinner = gc.getPlayer(iWinner)
	iLoser = pLosingUnit.getOwner()
	pLoser = gc.getPlayer(iLoser)
	pLoserCiv = pLoser.getCivilizationType()

	# Winner must be running slavery
	if pWinner.isCivic(gc.getInfoTypeForString("CIVIC_SLAVERY")):
		# Losing unit must be a land unit
		if (pLosingUnit.getDomainType() == gc.getInfoTypeForString("DOMAIN_LAND")):
			# Losing unit must not be barbarian
			#if (not pLosingUnit.isBarbarian()):
			# Losing unit must be capturable
			if (not pLosingUnit.isNoCapture()):
				# Losing unit must not be captured normally
				if (pLosingUnit.getCaptureUnitType(pWinner.getCivilizationType()) == UnitTypes.NO_UNIT):
					# 1/5 chance of capturing a slave normally
					iRand = gc.getGame().getSorenRandNum(100, "Capturing slave possibility")
					if pWinner.getBuildingClassCount(CvUtil.findInfoTypeNum(gc.getBuildingClassInfo, gc.getNumBuildingClassInfos(), 'BUILDINGCLASS_COLOSSEUM1')) > 0:
						iChance = 60
					else:
						iChance = 80
					if (iRand > iChance):
						if pWinner.getBuildingClassCount(CvUtil.findInfoTypeNum(gc.getBuildingClassInfo, gc.getNumBuildingClassInfos(), 'BUILDINGCLASS_COLOSSEUM1')) > 0:
							iSlaveCount = pWinner.getSlaveAfrican() + pWinner.getSlaveAsian() + pWinner.getSlaveEuropean() + pWinner.getSlaveIndian() + pWinner.getSlaveMid()	+ pWinner.getSlaveNewworld() + pWinner.getSlaveSlavic()
							if (((iSlaveCount % 10) == 0) and (iSlaveCount > 0)):
								iAfr = pWinner.getSlaveAfrican()
								iTotal = iAfr + pWinner.getSlaveAsian() + pWinner.getSlaveEuropean() + pWinner.getSlaveIndian() + pWinner.getSlaveMid() + pWinner.getSlaveNewworld() + pWinner.getSlaveSlavic()
								(loopCity, iter) = pWinner.firstCity(False)
								while(loopCity):
									if loopCity.getNumActiveBuilding(gc.getInfoTypeForString("BUILDING_COLOSSEUM1")):
										szMessage = localText.getText("TXT_KEY_GLADIATOR_MESSAGE", ("", ))
										CyInterface().addMessage(iWinner, true, gc.getEVENT_MESSAGE_TIME(), szMessage, "", InterfaceMessageTypes.MESSAGE_TYPE_INFO, gc.getUnitInfo(gc.getInfoTypeForString("UNIT_GLADIATOR")).getButton(), gc.getInfoTypeForString("COLOR_GREEN"), loopCity.getX(), loopCity.getY(), true, true)
										iGladiatorRand = gc.getGame().getSorenRandNum(iTotal, "Gladiator skin colour")	
										if (iGladiatorRand <= iAfr):
											pWinner.initUnit(gc.getInfoTypeForString("UNIT_GLADIATOR_AFRICAN"), loopCity.getX(), loopCity.getY(), UnitAITypes.NO_UNITAI, DirectionTypes.NO_DIRECTION)
										else:	
											pWinner.initUnit(gc.getInfoTypeForString("UNIT_GLADIATOR"), loopCity.getX(), loopCity.getY(), UnitAITypes.NO_UNITAI, DirectionTypes.NO_DIRECTION)
									(loopCity, iter) = pWinner.nextCity(iter, False)	
						if (pLoser.isBarbarian()):
							iLoserType = pLosingUnit.getUnitType()
							if (iLoserType == gc.getInfoTypeForString("UNIT_MODERN_SLAVE_AFR") or iLoserType == gc.getInfoTypeForString("UNIT_SLAVE_INDUSTRIAL_AFR") or iLoserType == gc.getInfoTypeForString("UNIT_SLAVE_MUSKETMAN_AFRICA") or iLoserType == gc.getInfoTypeForString("UNIT_MEDIEVAL_UPRISING_SLAVES_AFR") or iLoserType == gc.getInfoTypeForString("UNIT_ANCIENT_UPRISING_SLAVES_AFR")):
								iSlaveUnitType = gc.getInfoTypeForString("UNIT_SLAVE_AFRICAN")
								pWinner.incrementSlaveAfrican()
							elif (iLoserType == gc.getInfoTypeForString("UNIT_MODERN_SLAVE_ASI") or iLoserType == gc.getInfoTypeForString("UNIT_SLAVE_INDUSTRIAL_ASI") or iLoserType == gc.getInfoTypeForString("UNIT_SLAVE_MUSKETMAN_ASIA") or iLoserType == gc.getInfoTypeForString("UNIT_MEDIEVAL_UPRISING_SLAVES_ASI") or iLoserType == gc.getInfoTypeForString("UNIT_ANCIENT_UPRISING_SLAVES_ASI")):	
								iSlaveUnitType = gc.getInfoTypeForString("UNIT_SLAVE_ASIAN")
								pWinner.incrementSlaveAsian()
							elif (iLoserType == gc.getInfoTypeForString("UNIT_SLAVE_INDUSTRIAL_IND") or iLoserType == gc.getInfoTypeForString("UNIT_SLAVE_MUSKETMAN_INDIA") or iLoserType == gc.getInfoTypeForString("UNIT_MEDIEVAL_UPRISING_SLAVES_IND") or iLoserType == gc.getInfoTypeForString("UNIT_ANCIENT_UPRISING_SLAVES_IND")):
								iSlaveUnitType = gc.getInfoTypeForString("UNIT_SLAVE_INDIA")	
								pWinner.incrementSlaveIndian()
							elif (iLoserType == gc.getInfoTypeForString("UNIT_MODERN_SLAVE") or iLoserType == gc.getInfoTypeForString("UNIT_SLAVE_INDUSTRIAL") or iLoserType == gc.getInfoTypeForString("UNIT_SLAVE_MUSKETMAN") or iLoserType == gc.getInfoTypeForString("UNIT_MEDIEVAL_UPRISING_SLAVES") or iLoserType == gc.getInfoTypeForString("UNIT_ANCIENT_UPRISING_SLAVES")):
								iSlaveUnitType = gc.getInfoTypeForString("UNIT_SLAVE")
								pWinner.incrementSlaveMid()
							elif (iLoserType == gc.getInfoTypeForString("UNIT_SLAVE_INDUSTRIAL_NEW") or iLoserType == gc.getInfoTypeForString("UNIT_SLAVE_MUSKETMAN_NEWWORLD") or iLoserType == gc.getInfoTypeForString("UNIT_MEDIEVAL_UPRISING_SLAVES_NEW") or iLoserType == gc.getInfoTypeForString("UNIT_ANCIENT_UPRISING_SLAVES_NEW")):	
								iSlaveUnitType = gc.getInfoTypeForString("UNIT_SLAVE_NEWWORLD")	
								pWinner.incrementSlaveNewworld()
							elif (iLoserType == gc.getInfoTypeForString("UNIT_SLAVE_INDUSTRIAL_SLA") or iLoserType == gc.getInfoTypeForString("UNIT_SLAVE_MUSKETMAN_SLAVIC") or iLoserType == gc.getInfoTypeForString("UNIT_MEDIEVAL_UPRISING_SLAVES_SLA") or iLoserType == gc.getInfoTypeForString("UNIT_ANCIENT_UPRISING_SLAVES_SLA")):
								iSlaveUnitType = gc.getInfoTypeForString("UNIT_SLAVE_SLAVIC")	
								pWinner.incrementSlaveSlavic()	
							else:
								iSlaveUnitType = gc.getInfoTypeForString("UNIT_SLAVE_EUROPEAN")	
								pWinner.incrementSlaveEuropean()
						else:			
							if gc.getCivilizationInfo(pLoserCiv).getArtStyleType() == 7:
								iSlaveUnitType = gc.getInfoTypeForString("UNIT_SLAVE_AFRICAN")
								pWinner.incrementSlaveAfrican()
							elif gc.getCivilizationInfo(pLoserCiv).getArtStyleType() == 0 or gc.getCivilizationInfo(pLoserCiv).getArtStyleType() == 5 or gc.getCivilizationInfo(pLoserCiv).getArtStyleType() == 6 or gc.getCivilizationInfo(pLoserCiv).getArtStyleType() == 8 or gc.getCivilizationInfo(pLoserCiv).getArtStyleType() == 14:
								iSlaveUnitType = gc.getInfoTypeForString("UNIT_SLAVE_EUROPEAN")	
								pWinner.incrementSlaveEuropean()
							elif gc.getCivilizationInfo(pLoserCiv).getArtStyleType() == 1 or gc.getCivilizationInfo(pLoserCiv).getArtStyleType() == 13:
								iSlaveUnitType = gc.getInfoTypeForString("UNIT_SLAVE_ASIAN")
								pWinner.incrementSlaveAsian()
							elif gc.getCivilizationInfo(pLoserCiv).getArtStyleType() == 2 or gc.getCivilizationInfo(pLoserCiv).getArtStyleType() == 3 or gc.getCivilizationInfo(pLoserCiv).getArtStyleType() == 12:
								iSlaveUnitType = gc.getInfoTypeForString("UNIT_SLAVE_NEWWORLD")	
								pWinner.incrementSlaveNewworld()
							elif gc.getCivilizationInfo(pLoserCiv).getArtStyleType() == 9:
								iSlaveUnitType = gc.getInfoTypeForString("UNIT_SLAVE_SLAVIC")	
								pWinner.incrementSlaveSlavic()							
							elif gc.getCivilizationInfo(pLoserCiv).getArtStyleType() == 11:
								iSlaveUnitType = gc.getInfoTypeForString("UNIT_SLAVE_INDIA")	
								pWinner.incrementSlaveIndian()
							else: 
								iSlaveUnitType = gc.getInfoTypeForString("UNIT_SLAVE")
								pWinner.incrementSlaveMid()
						szMessage = localText.getText("TXT_KEY_SLAVE_CAPTURED_MESSAGE", ("", ))
						CyInterface().addMessage(iWinner, true, gc.getEVENT_MESSAGE_TIME(), szMessage, "", InterfaceMessageTypes.MESSAGE_TYPE_INFO, gc.getUnitInfo(iSlaveUnitType).getButton(), gc.getInfoTypeForString("COLOR_GREEN"), pWinningUnit.getX(), pWinningUnit.getY(), true, true)
						pWinner.initUnit(iSlaveUnitType, pWinningUnit.getX(), pWinningUnit.getY(), UnitAITypes.NO_UNITAI, DirectionTypes.NO_DIRECTION)

def onEndPlayerTurn(argsList):
	'Called at the beginning of a players turn'
	iGameTurn, iPlayer = argsList
	pPlayer = gc.getPlayer(iPlayer)

	if not pPlayer.isCivic(gc.getInfoTypeForString("CIVIC_SLAVERY")):
		if pPlayer.isAnarchy() == False:
			# Change slaves to workers for human players
			# Change slaves to revolting slaves for barbarians
			# 50% possibility
			# Added by Mexico 11.12.2006

			unitList = PyPlayer(iPlayer).getUnitList()
			iSlaveClass = gc.getInfoTypeForString("UNITCLASS_SLAVE")

			if (pPlayer.isBarbarian()):
				iReplacementUnitType = selectSlaveRebel(pPlayer)
				iReleaseChance = 50
			else:
				iReplacementUnitType = selectWorker(pPlayer)
				iReleaseChance = 250

			# Make sure the unit type is valid (this civ may not have a unit of the desired unit class)
			if (iReplacementUnitType != UnitTypes.NO_UNIT):
				for pUnit in unitList:
					if (pUnit.getUnitClassType() == iSlaveClass):
						iRand = gc.getGame().getSorenRandNum(iReleaseChance, "Releasing slave")
						if (iRand < 50):
							# Report the new unit
							szMessage = localText.getText("TXT_KEY_SLAVE_FREE_MESSAGE", ("",))
							CyInterface().addMessage(iPlayer, false, gc.getEVENT_MESSAGE_TIME(), szMessage, "AS2D_REVOLTEND", InterfaceMessageTypes.MESSAGE_TYPE_INFO, gc.getUnitInfo(iReplacementUnitType).getButton(), gc.getInfoTypeForString("COLOR_GREEN"), pUnit.getX(), pUnit.getY(), true, true)

							# Spawn the new unit
							if (pPlayer.isBarbarian()):
								pPlayer.initUnit(iReplacementUnitType, pUnit.getX(), pUnit.getY(), UnitAITypes.UNITAI_ATTACK,DirectionTypes.NO_DIRECTION)
							elif (pPlayer.isHuman()):
								pPlayer.initUnit(iReplacementUnitType, pUnit.getX(), pUnit.getY(), UnitAITypes.NO_UNITAI,DirectionTypes.NO_DIRECTION)
							else:
								pPlayer.initUnit(iReplacementUnitType, pUnit.getX(), pUnit.getY(), UnitAITypes.UNITAI_WORKER,DirectionTypes.NO_DIRECTION)

						# Kill the old unit
						pUnit.kill(False, 0)

def selectSlaveRebel(pPlayer):
	AncientTech = gc.getInfoTypeForString("TECH_WEAPON_SMITHING")
	MedievalTech = gc.getInfoTypeForString("TECH_PEASANT_SERVITUDE")
	RenaissanceTech = gc.getInfoTypeForString("TECH_FLINTLOCK_MUSKET")
	RenaissanceTech2 = gc.getInfoTypeForString("TECH_ADMINISTRATION")	
	IndustrialTech = gc.getInfoTypeForString("TECH_MASS_CONSCRIPTION")
	ModernTech = gc.getInfoTypeForString("TECH_GLOBALIZATION")
	ModernTech2 = gc.getInfoTypeForString("TECH_ROCKET_PROJECTILE")		
	iAfr = pPlayer.getSlaveAfrican()
	iAsi = pPlayer.getSlaveAsian()
	iEur = pPlayer.getSlaveEuropean()
	iInd = pPlayer.getSlaveIndian()
	iMid = pPlayer.getSlaveMid()		
	iNew = pPlayer.getSlaveNewworld()
	iSla = pPlayer.getSlaveSlavic()
	iTotal = iAfr + iAsi + iEur + iInd + iMid + iNew + iSla
	if (pPlayer.isBarbarian()):
		if gc.getTeam(pPlayer.getTeam()).isHasTech(IndustrialTech):
			iBestUnit = gc.getInfoTypeForString("UNIT_CONSCRIPT_BARBARIAN")
		elif gc.getTeam(pPlayer.getTeam()).isHasTech(RenaissanceTech) and gc.getTeam(pPlayer.getTeam()).isHasTech(RenaissanceTech2):
			iBestUnit = gc.getInfoTypeForString("UNIT_IRREGULARS_BARBARIAN")
		elif gc.getTeam(pPlayer.getTeam()).isHasTech(MedievalTech):
			iBestUnit = gc.getInfoTypeForString("UNIT_LEVY_BARBARIAN")
		elif gc.getTeam(pPlayer.getTeam()).isHasTech(AncientTech):
			iBestUnit = gc.getInfoTypeForString("UNIT_HYAPIST_BARBARIAN")			
		else:
			iBestUnit = gc.getInfoTypeForString("UNIT_WARRIOR_BARBARIAN")
		return iBestUnit
	elif iTotal == 0:
		# If we haven't captured any foreign slaves yet, revolters will be from our own civ 
		pCiv = pPlayer.getCivilizationType()
		if gc.getCivilizationInfo(pCiv).getArtStyleType() == 7:
			iSlaveUnitType = 1
			# African
		elif gc.getCivilizationInfo(pCiv).getArtStyleType() == 0 or gc.getCivilizationInfo(pCiv).getArtStyleType() == 5 or gc.getCivilizationInfo(pCiv).getArtStyleType() == 6 or gc.getCivilizationInfo(pCiv).getArtStyleType() == 8 or gc.getCivilizationInfo(pCiv).getArtStyleType() == 14:
			iSlaveUnitType = 3
			# European
		elif gc.getCivilizationInfo(pCiv).getArtStyleType() == 1 or gc.getCivilizationInfo(pCiv).getArtStyleType() == 13:
			iSlaveUnitType = 2
			# Asian
		elif gc.getCivilizationInfo(pCiv).getArtStyleType() == 2 or gc.getCivilizationInfo(pCiv).getArtStyleType() == 3 or gc.getCivilizationInfo(pCiv).getArtStyleType() == 12:
			iSlaveUnitType = 6
			# Newworld
		elif gc.getCivilizationInfo(pCiv).getArtStyleType() == 9:
			iSlaveUnitType = 7		
			# Slavic
		elif gc.getCivilizationInfo(pCiv).getArtStyleType() == 11:
			iSlaveUnitType = 4
			# India
		else: 
			iSlaveUnitType = 5
			# Mideast
			
		if gc.getTeam(pPlayer.getTeam()).isHasTech(ModernTech) and gc.getTeam(pPlayer.getTeam()).isHasTech(ModernTech2):
			if iSlaveUnitType == 1:
				return gc.getInfoTypeForString("UNIT_MODERN_SLAVE_AFR")
			elif iSlaveUnitType == 2:
				return gc.getInfoTypeForString("UNIT_MODERN_SLAVE_ASI")
			elif iSlaveUnitType == 3:
				return gc.getInfoTypeForString("UNIT_MODERN_SLAVE_EUR")
			elif iSlaveUnitType == 4:
				return gc.getInfoTypeForString("UNIT_MODERN_SLAVE")
			elif iSlaveUnitType == 5:
				return gc.getInfoTypeForString("UNIT_MODERN_SLAVE")	
			elif iSlaveUnitType == 6:
				return gc.getInfoTypeForString("UNIT_MODERN_SLAVE")
			else:
				return gc.getInfoTypeForString("UNIT_MODERN_SLAVE_EUR")			
		elif gc.getTeam(pPlayer.getTeam()).isHasTech(IndustrialTech):
			if iSlaveUnitType == 1:
				return gc.getInfoTypeForString("UNIT_SLAVE_INDUSTRIAL_AFR")
			elif iSlaveUnitType == 2:
				return gc.getInfoTypeForString("UNIT_SLAVE_INDUSTRIAL_ASI")
			elif iSlaveUnitType == 3:
				return gc.getInfoTypeForString("UNIT_SLAVE_INDUSTRIAL_EUR")
			elif iSlaveUnitType == 4:
				return gc.getInfoTypeForString("UNIT_SLAVE_INDUSTRIAL_IND")
			elif iSlaveUnitType == 5:
				return gc.getInfoTypeForString("UNIT_SLAVE_INDUSTRIAL")	
			elif iSlaveUnitType == 6:
				return gc.getInfoTypeForString("UNIT_SLAVE_INDUSTRIAL_NEW")
			else:
				return gc.getInfoTypeForString("UNIT_SLAVE_INDUSTRIAL_SLA")	
		elif gc.getTeam(pPlayer.getTeam()).isHasTech(RenaissanceTech) and gc.getTeam(pPlayer.getTeam()).isHasTech(RenaissanceTech2):
			if iSlaveUnitType == 1:
				return gc.getInfoTypeForString("UNIT_SLAVE_MUSKETMAN_AFRICA")
			elif iSlaveUnitType == 2:
				return gc.getInfoTypeForString("UNIT_SLAVE_MUSKETMAN_ASIA")
			elif iSlaveUnitType == 3:
				return gc.getInfoTypeForString("UNIT_SLAVE_MUSKETMAN_EUROPE")
			elif iSlaveUnitType == 4:
				return gc.getInfoTypeForString("UNIT_SLAVE_MUSKETMAN_INDIA")
			elif iSlaveUnitType == 5:
				return gc.getInfoTypeForString("UNIT_SLAVE_MUSKETMAN")	
			elif iSlaveUnitType == 6:
				return gc.getInfoTypeForString("UNIT_SLAVE_MUSKETMAN_NEWWORLD")
			else:
				return gc.getInfoTypeForString("UNIT_SLAVE_MUSKETMAN_SLAVIC")	
		elif gc.getTeam(pPlayer.getTeam()).isHasTech(MedievalTech):
			if iSlaveUnitType == 1:
				return gc.getInfoTypeForString("UNIT_MEDIEVAL_UPRISING_SLAVES_AFR")
			elif iSlaveUnitType == 2:
				return gc.getInfoTypeForString("UNIT_MEDIEVAL_UPRISING_SLAVES_ASI")
			elif iSlaveUnitType == 3:
				return gc.getInfoTypeForString("UNIT_MEDIEVAL_UPRISING_SLAVES_EUR")
			elif iSlaveUnitType == 4:
				return gc.getInfoTypeForString("UNIT_MEDIEVAL_UPRISING_SLAVES_IND")
			elif iSlaveUnitType == 5:
				return gc.getInfoTypeForString("UNIT_MEDIEVAL_UPRISING_SLAVES")	
			elif iSlaveUnitType == 6:
				return gc.getInfoTypeForString("UNIT_MEDIEVAL_UPRISING_SLAVES_NEW")
			else:
				return gc.getInfoTypeForString("UNIT_MEDIEVAL_UPRISING_SLAVES_SLA")	
		elif gc.getTeam(pPlayer.getTeam()).isHasTech(AncientTech):
			if iSlaveUnitType == 1:
				return gc.getInfoTypeForString("UNIT_ANCIENT_UPRISING_SLAVES_AFR")
			elif iSlaveUnitType == 2:
				return gc.getInfoTypeForString("UNIT_ANCIENT_UPRISING_SLAVES_ASI")
			elif iSlaveUnitType == 3:
				return gc.getInfoTypeForString("UNIT_ANCIENT_UPRISING_SLAVES_EUR")
			elif iSlaveUnitType == 4:
				return gc.getInfoTypeForString("UNIT_ANCIENT_UPRISING_SLAVES_IND")
			elif iSlaveUnitType == 5:
				return gc.getInfoTypeForString("UNIT_ANCIENT_UPRISING_SLAVES")	
			elif iSlaveUnitType == 6:
				return gc.getInfoTypeForString("UNIT_ANCIENT_UPRISING_SLAVES_NEW")
			else:
				return gc.getInfoTypeForString("UNIT_ANCIENT_UPRISING_SLAVES_SLA")					
		else:
			if iSlaveUnitType == 1:
				return gc.getInfoTypeForString("UNIT_PRIMITIVE_UPRISING_SLAVES_AFR")
			elif iSlaveUnitType == 2:
				return gc.getInfoTypeForString("UNIT_PRIMITIVE_UPRISING_SLAVES_ASI")
			elif iSlaveUnitType == 3:
				return gc.getInfoTypeForString("UNIT_PRIMITIVE_UPRISING_SLAVES_EUR")
			elif iSlaveUnitType == 4:
				return gc.getInfoTypeForString("UNIT_PRIMITIVE_UPRISING_SLAVES_IND")
			elif iSlaveUnitType == 5:
				return gc.getInfoTypeForString("UNIT_PRIMITIVE_UPRISING_SLAVES")	
			elif iSlaveUnitType == 6:
				return gc.getInfoTypeForString("UNIT_PRIMITIVE_UPRISING_SLAVES_NEW")
			else:
				return gc.getInfoTypeForString("UNIT_PRIMITIVE_UPRISING_SLAVES_SLA")			
	else:
		iSlaveRand = gc.getGame().getSorenRandNum(iTotal, "Slave culture") + 1
		if gc.getTeam(pPlayer.getTeam()).isHasTech(ModernTech) and gc.getTeam(pPlayer.getTeam()).isHasTech(ModernTech2):
			if iSlaveRand <= iAfr:
				return gc.getInfoTypeForString("UNIT_MODERN_SLAVE_AFR")
			elif iSlaveRand <= iAfr + iAsi:
				return gc.getInfoTypeForString("UNIT_MODERN_SLAVE_ASI")
			elif iSlaveRand <= iAfr + iAsi + iEur:
				return gc.getInfoTypeForString("UNIT_MODERN_SLAVE_EUR")
			elif iSlaveRand <= iAfr + iAsi + iEur + iInd:
				return gc.getInfoTypeForString("UNIT_MODERN_SLAVE")
			elif iSlaveRand <= iAfr + iAsi + iEur + iInd + iMid:
				return gc.getInfoTypeForString("UNIT_MODERN_SLAVE")	
			elif iSlaveRand <= iAfr + iAsi + iEur + iInd + iMid + iNew:
				return gc.getInfoTypeForString("UNIT_MODERN_SLAVE")
			else:
				return gc.getInfoTypeForString("UNIT_MODERN_SLAVE_EUR")			
		elif gc.getTeam(pPlayer.getTeam()).isHasTech(IndustrialTech):
			if iSlaveRand <= iAfr:
				return gc.getInfoTypeForString("UNIT_SLAVE_INDUSTRIAL_AFR")
			elif iSlaveRand <= iAfr + iAsi:
				return gc.getInfoTypeForString("UNIT_SLAVE_INDUSTRIAL_ASI")
			elif iSlaveRand <= iAfr + iAsi + iEur:
				return gc.getInfoTypeForString("UNIT_SLAVE_INDUSTRIAL_EUR")
			elif iSlaveRand <= iAfr + iAsi + iEur + iInd:
				return gc.getInfoTypeForString("UNIT_SLAVE_INDUSTRIAL_IND")
			elif iSlaveRand <= iAfr + iAsi + iEur + iInd + iMid:
				return gc.getInfoTypeForString("UNIT_SLAVE_INDUSTRIAL")	
			elif iSlaveRand <= iAfr + iAsi + iEur + iInd + iMid + iNew:
				return gc.getInfoTypeForString("UNIT_SLAVE_INDUSTRIAL_NEW")
			else:
				return gc.getInfoTypeForString("UNIT_SLAVE_INDUSTRIAL_SLA")	
		elif gc.getTeam(pPlayer.getTeam()).isHasTech(RenaissanceTech) and gc.getTeam(pPlayer.getTeam()).isHasTech(RenaissanceTech2):
			if iSlaveRand <= iAfr:
				return gc.getInfoTypeForString("UNIT_SLAVE_MUSKETMAN_AFRICA")
			elif iSlaveRand <= iAfr + iAsi:
				return gc.getInfoTypeForString("UNIT_SLAVE_MUSKETMAN_ASIA")
			elif iSlaveRand <= iAfr + iAsi + iEur:
				return gc.getInfoTypeForString("UNIT_SLAVE_MUSKETMAN_EUROPE")
			elif iSlaveRand <= iAfr + iAsi + iEur + iInd:
				return gc.getInfoTypeForString("UNIT_SLAVE_MUSKETMAN_INDIA")
			elif iSlaveRand <= iAfr + iAsi + iEur + iInd + iMid:
				return gc.getInfoTypeForString("UNIT_SLAVE_MUSKETMAN")	
			elif iSlaveRand <= iAfr + iAsi + iEur + iInd + iMid + iNew:
				return gc.getInfoTypeForString("UNIT_SLAVE_MUSKETMAN_NEWWORLD")
			else:
				return gc.getInfoTypeForString("UNIT_SLAVE_MUSKETMAN_SLAVIC")	
		elif gc.getTeam(pPlayer.getTeam()).isHasTech(MedievalTech):
			if iSlaveRand <= iAfr:
				return gc.getInfoTypeForString("UNIT_MEDIEVAL_UPRISING_SLAVES_AFR")
			elif iSlaveRand <= iAfr + iAsi:
				return gc.getInfoTypeForString("UNIT_MEDIEVAL_UPRISING_SLAVES_ASI")
			elif iSlaveRand <= iAfr + iAsi + iEur:
				return gc.getInfoTypeForString("UNIT_MEDIEVAL_UPRISING_SLAVES_EUR")
			elif iSlaveRand <= iAfr + iAsi + iEur + iInd:
				return gc.getInfoTypeForString("UNIT_MEDIEVAL_UPRISING_SLAVES_IND")
			elif iSlaveRand <= iAfr + iAsi + iEur + iInd + iMid:
				return gc.getInfoTypeForString("UNIT_MEDIEVAL_UPRISING_SLAVES")	
			elif iSlaveRand <= iAfr + iAsi + iEur + iInd + iMid + iNew:
				return gc.getInfoTypeForString("UNIT_MEDIEVAL_UPRISING_SLAVES_NEW")
			else:
				return gc.getInfoTypeForString("UNIT_MEDIEVAL_UPRISING_SLAVES_SLA")		
		elif gc.getTeam(pPlayer.getTeam()).isHasTech(AncientTech):
			if iSlaveRand <= iAfr:
				return gc.getInfoTypeForString("UNIT_ANCIENT_UPRISING_SLAVES_AFR")
			elif iSlaveRand <= iAfr + iAsi:
				return gc.getInfoTypeForString("UNIT_ANCIENT_UPRISING_SLAVES_ASI")
			elif iSlaveRand <= iAfr + iAsi + iEur:
				return gc.getInfoTypeForString("UNIT_ANCIENT_UPRISING_SLAVES_EUR")
			elif iSlaveRand <= iAfr + iAsi + iEur + iInd:
				return gc.getInfoTypeForString("UNIT_ANCIENT_UPRISING_SLAVES_IND")
			elif iSlaveRand <= iAfr + iAsi + iEur + iInd + iMid:
				return gc.getInfoTypeForString("UNIT_ANCIENT_UPRISING_SLAVES")	
			elif iSlaveRand <= iAfr + iAsi + iEur + iInd + iMid + iNew:
				return gc.getInfoTypeForString("UNIT_ANCIENT_UPRISING_SLAVES_NEW")
			else:
				return gc.getInfoTypeForString("UNIT_ANCIENT_UPRISING_SLAVES_SLA")				
		else:
			if iSlaveRand <= iAfr:
				return gc.getInfoTypeForString("UNIT_PRIMITIVE_UPRISING_SLAVES_AFR")
			elif iSlaveRand <= iAfr + iAsi:
				return gc.getInfoTypeForString("UNIT_PRIMITIVE_UPRISING_SLAVES_ASI")
			elif iSlaveRand <= iAfr + iAsi + iEur:
				return gc.getInfoTypeForString("UNIT_PRIMITIVE_UPRISING_SLAVES_EUR")
			elif iSlaveRand <= iAfr + iAsi + iEur + iInd:
				return gc.getInfoTypeForString("UNIT_PRIMITIVE_UPRISING_SLAVES_IND")
			elif iSlaveRand <= iAfr + iAsi + iEur + iInd + iMid:
				return gc.getInfoTypeForString("UNIT_PRIMITIVE_UPRISING_SLAVES")	
			elif iSlaveRand <= iAfr + iAsi + iEur + iInd + iMid + iNew:
				return gc.getInfoTypeForString("UNIT_PRIMITIVE_UPRISING_SLAVES_NEW")
			else:
				return gc.getInfoTypeForString("UNIT_PRIMITIVE_UPRISING_SLAVES_SLA")

def selectPeasantRebel(pPlayer):
	RenaissanceTech = gc.getInfoTypeForString("TECH_FLINTLOCK_MUSKET")
	RenaissanceTech2 = gc.getInfoTypeForString("TECH_ADMINISTRATION")	
	IndustrialTech = gc.getInfoTypeForString("TECH_MASS_CONSCRIPTION")
	ModernTech = gc.getInfoTypeForString("TECH_GLOBALIZATION")
	ModernTech2 = gc.getInfoTypeForString("TECH_ROCKET_PROJECTILE")	
	if gc.getTeam(pPlayer.getTeam()).isHasTech(ModernTech) and gc.getTeam(pPlayer.getTeam()).isHasTech(ModernTech2):
		iPeasantRebel = gc.getInfoTypeForString("UNITCLASS_PARAMILITARY")
	elif gc.getTeam(pPlayer.getTeam()).isHasTech(IndustrialTech):	
		iPeasantRebel = gc.getInfoTypeForString("UNITCLASS_CONSCRIPT")
	elif gc.getTeam(pPlayer.getTeam()).isHasTech(RenaissanceTech) and gc.getTeam(pPlayer.getTeam()).isHasTech(RenaissanceTech2):
		iPeasantRebel = gc.getInfoTypeForString("UNITCLASS_IRREGULARS")
	else:
		iPeasantRebel = gc.getInfoTypeForString("UNITCLASS_LEVY")
	return gc.getCivilizationInfo(pPlayer.getCivilizationType()).getCivilizationUnits(iPeasantRebel)


def selectWorker(pPlayer):
	iUnitClass = gc.getInfoTypeForString("UNITCLASS_WORKER")
	return gc.getCivilizationInfo(pPlayer.getCivilizationType()).getCivilizationUnits(iUnitClass)
