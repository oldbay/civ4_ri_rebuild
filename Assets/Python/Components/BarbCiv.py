## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
from CvPythonExtensions import *
import CvUtil
import ScreenInput
import CvScreenEnums
import PyHelpers	
from math import sqrt	

# globals
gc = CyGlobalContext()
PyPlayer = PyHelpers.PyPlayer

class BarbCiv:
		
	def __init__(self):
		self.iRagingMultiplier	= 2	## Increased Chance for Raging Barbs
		self.iTechPercent		= 25	## 25% Progress Per Team with Tech
		self.iExtraBuildings		= 2	## Extra Buildings Per City, increased with Era
		self.iNumDefender		= 2	## Number of Free Defenders per City
		self.iNumWorker		= 0	## Number of Free Workers per City
		self.iNumSettler		= 0	## Number of Free Settlers per City
		self.iPopulationBoost	= 0	## Number of Extra Population per City
		self.iCapitalBonus		= 1	## Number of Extra XXX in Capital
		self.iFreeCulture		= 100 ## Amount of free culture per city the new civ gets
		self.iModifier			= 0.25 ## percentage modifier for barb settle chance 
		self.iWorker		= gc.getInfoTypeForString("UNITCLASS_WORKER")
		self.iSettler		= gc.getInfoTypeForString("UNITCLASS_SETTLER")
		self.cityRadius = 15 ## for new barbarian settles, this is the maximum distance from the capital for other cities to join in
		self.newWorldChange = 1000 ## The chance to settle in the New World (and elsewhere once all civs on a continent know Shipyards) will be divided by this number
		self.newWorldMax = 10
		self.LOG_BARB_SETTLE = True ## popup alert when new civ is formed from barb cities
		self.LOG_DEBUG = True  ## enable debug log and functions
		self.SPAWN_DERIVATIVE = True ## allow derivative civs to spawn as settled barbarians
		self.SETTLE_REMOTE = False ## allow barbarians to settle on landmasses without player cities 

	def checkBarb(self):
		lCiv = []
		lCity = []
		iBarbPlayer = gc.getBARBARIAN_PLAYER()
		pBarbPlayer = gc.getPlayer(iBarbPlayer)
		iTargetNumber = gc.getWorldInfo(gc.getMap().getWorldSize()).getDefaultPlayers()
		iCurrentNumber = CyGame().countCivPlayersAlive()
		iHighNumber = CyGame().countCivPlayersEverAlive()
		iLength = (CyGame().getEstimateEndTurn() / 7)								## Don't settle down until 1/7 of the game length has passed. 
	
#		iChance = 0
#		iPopulation = 0
#		iCities = 0
#		areas = []
#		sortList = []
		popChance = 0
		
		if (CyGame().getGameTurn() > iLength) and (iHighNumber < iTargetNumber * 2):
			barbPlayer = PyPlayer( gc.getBARBARIAN_PLAYER() )
			if( barbPlayer.getNumCities() >= 1 ) :
				
				barbCityList = barbPlayer.getCityList()
				if self.SETTLE_REMOTE == False:
					barbCityRemove = []
					for i in range(0,len(barbCityList)) :
						ix = barbCityList[i].getX()
						iy = barbCityList[i].getY()
						if barbCityList[i].GetCy().area().getNumCities() == barbCityList[i].GetCy().area().getCitiesPerPlayer(iBarbPlayer):
							barbCityRemove.append(barbCityList[i])
							continue 
							# Check if there are other civs' units right next to barbarian city
						bUnderSiege = False 
						for jX in range (ix-1, ix+1):
							for jX in range (iy-1, ix-1):
								if gc.getMap().isPlot(jX, jX):
									pPlot = gc.getMap().plot(jX, jX)
									if (not pPlot.isCity()):
										if (pPlot.isFlatlands() or pPlot.isHills()):
											iNumUnits = pPlot.getNumUnits()
											if (iNumUnits > 0):
												for iUnit in range(iNumUnits):
													pUnit = pPlot.getUnit(iUnit)
													if (pUnit.getOwner() != iBarbPlayer) and (pUnit.canAttack()):
														bUnderSiege = True
														break
							else: 
								continue
							break
						if bUnderSiege == True:
							barbCityRemove.append(barbCityList[i])
							continue 
						# a rare but important case of a big city falling to barbarians; shouldn't settle immediately	
						if (barbCityList[i].getNumBuilding(gc.getInfoTypeForString("BUILDING_SUBJUGATION")) > 0):
							barbCityRemove.append(barbCityList[i])
							continue 
					for j in range(0,len(barbCityRemove)):		
						barbCityList.remove(barbCityRemove[j])
						
				# ith entry of Tree contains list of cities close to ith entry of city list
				barbCityTree = list()

				for i in range(0,len(barbCityList)) :

					ix = barbCityList[i].getX()
					iy = barbCityList[i].getY()
					closeCityList = list()
					closeCityList.append(barbCityList[i])

					# Build a list of cities close to this city
					for j in range(0,len(barbCityList)) :
						if( not barbCityList[j] in closeCityList ) :
							jx = barbCityList[j].getX()
							jy = barbCityList[j].getY()
							# I think this will catch cities close over edge of map ...
							if( plotDistance(ix,iy,jx,jy) < self.cityRadius ) :
								closeCityList.append(barbCityList[j])

					barbCityTree.append(closeCityList)
					
				maxListPop = 0
				maxListIdx = 0
				for i,cityList in enumerate(barbCityTree) :
					pop = 0
					for j in range(0,len(cityList)) :
						pop += cityList[j].getPopulation()
					if( pop > maxListPop ) :
						maxListPop = pop
						maxListIdx = i
		   

				if( self.LOG_DEBUG ) :
					print "Found %d barbarian cities"%(len(barbCityList))

				pop = maxListPop
				# Odds modifier from config file
				mod = self.iModifier
				# If lots of barb cities, increase odds
				numCitiesMod = int(sqrt(len(barbCityList)))
				# Change by gamespeed
				gsm = (661.0)/(CyGame().getMaxTurns() + 1.0)
				# Odds of barb city settling down go up by era
				eraMod = CyGame().getCurrentEra() - CyGame().getStartEra()/2.0
				if( eraMod < .5 ) :
					eraMod = .5
				if maxListPop > 0:
					if( self.LOG_DEBUG ) : print("  Potential barb capital, " + (barbCityTree[maxListIdx][0].GetCy().getName()))
	
				bNewWorld = False 
				iCountAll = 0
				iCountRen = 0

				# If all civs on the landmass know Shipyards, we're likely in the New World	
				if maxListPop > 0:
					for iPlayer in xrange(gc.getMAX_PLAYERS()): 
						pPlayer = gc.getPlayer(iPlayer)
						if pPlayer.isAlive():
							if (barbCityTree[maxListIdx][0].GetCy().area().getCitiesPerPlayer(iPlayer) > 0):
								if (iPlayer != gc.getBARBARIAN_PLAYER()):
									iCountAll += 1
									if (gc.getTeam(pPlayer.getTeam()).isHasTech(gc.getInfoTypeForString("TECH_SHIPYARDS"))):
										iCountRen += 1
					if ((iCountAll > 0) and (iCountAll == iCountRen)):
						bNewWorld = True

				popChance = int(mod*numCitiesMod*gsm*eraMod*1.5*(pop*pop))

				if( self.LOG_DEBUG ) : print("  BC - Barb odds: %d pop, "%(popChance))
				
				if (bNewWorld): 
					popChance /= self.newWorldChange
					popChance = min(popChance, self.newWorldMax) 

	#			for i in range(CyMap().getIndexAfterLastArea()) :
	#				area = CyMap().getArea(i)
	#				if area.isNone() : continue
	#				if area.isWater() : continue
	#				if area.getCitiesPerPlayer(iBarbPlayer) == 0 : continue
	#				areas.append(area)
	#
	#			if areas == []: 
	#				iChance = 0
	#			else :
	#				sortList = [ [area.getID(), area.getCitiesPerPlayer(iBarbPlayer), area.getPopulationPerPlayer(iBarbPlayer)] for area in areas ]
	#				sortList.sort(cmp=lambda x,y: cmp(x[2], y[2]))
	#				sortList.sort(cmp=lambda x,y: cmp(x[1], y[1]))
	#				sortList.reverse()
	#				
	#				(pCity, iter) = pBarbPlayer.firstCity(false)
	#				while(pCity):
	#					if pCity.area().getID() == sortList[0][0]:
	#						iCities += 1
	#						iPopulation += pCity.getPopulation()
	#					(pCity, iter) = pBarbPlayer.nextCity(iter, False)
	#
	#				iChance = ((iCities + (2 * (iTargetNumber - iCurrentNumber))) * iPopulation)*((iTargetNumber * 2) - iHighNumber) - 10
				
				if CyGame().isOption(GameOptionTypes.GAMEOPTION_RAGING_BARBARIANS):
					popChance *= self.iRagingMultiplier
				if CyGame().isOption(GameOptionTypes.GAMEOPTION_NO_BARBARIANS):
					popChance = 0
					
		#		if iChance != 0:
		#			print ("Game turn " + str(CyGame().getGameTurn()) + ", BarbChance " + str(iChance) + "#")
		#		if iChance < 0:
		#			iChance = 0
				if( popChance > (CyGame().getSorenRandNum(999,'BarbarianCiv: Check barbs'))) :
					#if( self.LOG_DEBUG ) : CyInterface().addImmediateMessage("Attempting to reincarnate civ, giving them %d cities"%(len(barbCityTree[maxListIdx])),"")
					if( self.LOG_DEBUG ) : print("  BC - Population barb roll succeeded, with %d cities, %d pop"%(len(barbCityTree[maxListIdx]),maxListPop))
					if( self.LOG_DEBUG ) : print("  BC - Current modifiers are: mod %f, ncm %d, gsm %f"%(mod,numCitiesMod,gsm))
		#			self.createBarbCiv( barbCityTree[maxListIdx] )
					if len(barbCityTree[maxListIdx]) > 0:				
		#		if (CyGame().getSorenRandNum(100, "Chosen Plot")+1) < iChance:

						iNewPlayer = self.getNewID()
						if iNewPlayer == -1: return
						pNewPlayer = gc.getPlayer(iNewPlayer)
						iNewCiv = -1
						for iCiv in xrange(gc.getNumCivilizationInfos()):
							if gc.getCivilizationInfo(iCiv).isPlayable() or ((gc.getCivilizationInfo(iCiv).isDerivative()) and (self.SPAWN_DERIVATIVE == True)): 
								if CyGame().isCivEverActive(iCiv): continue
								lCiv.append(iCiv)
						if len(lCiv) == 0:
							for iCiv in xrange(gc.getNumCivilizationInfos()):
								if gc.getCivilizationInfo(iCiv).isPlayable(): 
									pCiv = gc.getPlayer(iCiv)
									if pCiv.isEverAlive():
										(civCity, iter) = pCiv.firstCity(False)
										while (civCity): 
											if civCity.getLiberationPlayer(False)!=-1:
												lCity.append(civCity)
											(civCity, iter) = pCiv.nextCity(iter, False)
										if len(lCity) > 0:
											pCity = lCity[0]
											iPotentialCiv = pCity.getLiberationPlayer(False)
											lCiv.append(iPotentialCiv)
										del lCity[:]
						if len(lCiv) > 0:
							iNewCiv = lCiv[CyGame().getSorenRandNum(len(lCiv), "Chose Barbarian Civ")]
						if iNewCiv == -1:
							for iCiv in xrange(gc.getNumCivilizationInfos()):
								if CyGame().isCivEverActive(iCiv): continue
								if gc.getCivilizationInfo(iCiv).isPlayable():
									lCiv.append(iCiv)
							if len(lCiv) > 0:
								iNewCiv = lCiv[CyGame().getSorenRandNum(len(lCiv), "Chose Barbarian Civ")]
						if iNewCiv == -1: return
						del lCiv[:]

						iNewLeader = -1
						lLeader = []
						for iLeader in xrange(gc.getNumLeaderHeadInfos()):
							if CyGame().isLeaderEverActive(iLeader): continue
							if iLeader == gc.getDefineINT("BARBARIAN_LEADER"): continue
							if not CyGame().isOption(GameOptionTypes.GAMEOPTION_LEAD_ANY_CIV):
								if not gc.getCivilizationInfo(iNewCiv).isLeaders(iLeader): continue
							lLeader.append(iLeader)
						if len(lLeader) > 0:
							iNewLeader = lLeader[CyGame().getSorenRandNum(len(lLeader), "Chose Barbarian Civ Leader")]
						if iNewLeader == -1: return

						CyGame().addPlayer(iNewPlayer, iNewLeader, iNewCiv)
						self.spawnBarbCities(iNewPlayer, pBarbPlayer, barbCityTree[maxListIdx])
						print ("Barbarians settle under " + pNewPlayer.getName() + " forming " + gc.getCivilizationInfo(pNewPlayer.getCivilizationType()).getDescription())

	def spawnBarbCities(self, iPlayer, pBarbPlayer, cityList):
		iBarbPlayer = gc.getBARBARIAN_PLAYER()
		pPlayer = gc.getPlayer(iPlayer)
		iTeam = pPlayer.getTeam()
		pTeam = gc.getTeam(iTeam)
#		areas = []
#		sortList = []

#		for i in range(CyMap().getIndexAfterLastArea()) :
#			area = CyMap().getArea(i)
#			if area.isNone() : continue
#			if area.isWater() : continue
#			if area.getCitiesPerPlayer(iBarbPlayer) == 0 : continue
#			areas.append(area)
#
#		if areas == []: return 
#		else :
#			sortList = [ [area.getID(), area.getCitiesPerPlayer(iBarbPlayer), area.getPopulationPerPlayer(iBarbPlayer)] for area in areas ]
#			sortList.sort(cmp=lambda x,y: cmp(x[2], y[2]))
#			sortList.sort(cmp=lambda x,y: cmp(x[1], y[1]))
#			sortList.reverse()
#
#		(pCity, iter) = pBarbPlayer.firstCity(false)
		for i,giftCity in enumerate(cityList[0:]) :
#		while(pCity):
#			if pCity.area().getID() == sortList[0][0]:
			name = pPlayer.getNewCityName()
			giftCity.GetCy().setName(name,true)
			pCityPlot = giftCity.GetCy().plot()
			pPlayer.acquireCity( giftCity.GetCy(), False, True )
			pCity1 = pCityPlot.getPlotCity()
			if pPlayer.getNumCities() == 1:
				pCity1.setNumRealBuilding(gc.getInfoTypeForString("BUILDING_PALACE"), 1)
				pHomeArea = pCity1.area()
#		(pCity, iter) = pBarbPlayer.nextCity(iter, False)

		iStartGold = pPlayer.getNumCities()*100
		pPlayer.changeGold(iStartGold)

		iNumCities = pPlayer.getNumCities()
		print ("NumCities")
		print (iNumCities)
		
		(pUnit, iter) = pBarbPlayer.firstUnit(false)
		while(pUnit):
			if not pUnit.isAnimal(): 
				pUnitPlot = pUnit.plot()
				if pUnitPlot.isVisible(iTeam,false):
					iUnitType = gc.getCivilizationInfo(pPlayer.getCivilizationType()).getCivilizationUnits(pUnit.getUnitClassType())
					if iUnitType == -1:
						iUnitType = pUnit.getUnitType()
					print ("Unit type chosen as ", iUnitType, "translating to ", pUnit.getName())
					
					iCityIter = 0
					minDistanceIter = 0
					minDistance = 100
					for iCityIter in xrange(iNumCities):
						pCity = pPlayer.getCity(iCityIter)
						pCityIterPlot = pCity.plot()
						iDistanceIter = CyMap().calculatePathDistance(pCityIterPlot,pUnitPlot)
						if (iDistanceIter < minDistance):
							minDistanceIter = iCityIter
							minDistance = iDistanceIter
					iCityIter = minDistanceIter
					pCity = pPlayer.getCity(minDistanceIter)
					pNewUnitPlot = pCity.plot()
					
					pNewUnit = pPlayer.initUnit(iUnitType, pNewUnitPlot.getX(), pNewUnitPlot.getY(), UnitAITypes.NO_UNITAI, DirectionTypes.NO_DIRECTION)
					print ("Spawned new unit",  pNewUnit.getName(), "in city ", pCity.getName())
					pNewUnit.convert(pUnit)
					pUnit.kill(false,iPlayer)

			(pUnit, iter) = pBarbPlayer.nextUnit(iter, false)
		CyInterface().addImmediateMessage(CyTranslator().getText("TXT_NEW_BARBIE", (pPlayer.getName(),)), "")

		## Grant Techs ##
		for iTech in xrange(gc.getNumTechInfos()):
			if pTeam.isHasTech(iTech): continue
			if pPlayer.canEverResearch(iTech):
				iCost = pTeam.getResearchCost(iTech)
				iHas = 0
				iHasnot = 0
				if gc.getTeam(gc.getBARBARIAN_TEAM()).isHasTech(iTech):
					pTeam.changeResearchProgress(iTech, iCost - pTeam.getResearchProgress(iTech), iPlayer)
					continue
				for iTeamX in xrange(gc.getMAX_CIV_TEAMS()):
					pTeamX = gc.getTeam(iTeamX)
					if pTeamX.isAlive():
						if pTeamX.countNumCitiesByArea(pHomeArea) > 0:
							if pTeamX.isHasTech(iTech):
								iHas += 1
							else:
								iHasnot += 1
				if (iHas >= iHasnot) and (iHas > 0):
					iChange = iCost - pTeam.getResearchProgress(iTech)
					pTeam.changeResearchProgress(iTech, iChange, iPlayer)
	
		## Grant Culture, Buildings and Units ##
		(pCity, iter) = pPlayer.firstCity(False)
		while(pCity):
			iCapitalBonus = max(0, (pCity.isCapital() * self.iCapitalBonus))
			iCulture1 = 0 
			if pCity.countTotalCultureTimes100() > 0:
				iLosingPlayer = pCity.findHighestCulture()
				iOldCulture = pCity.getCulture(iLosingPlayer)
				pLosingPlayer = gc.getPlayer(iLosingPlayer)
				
				iCulture2 = 0
				iCulture2 += iOldCulture
				iCulture2 *= -0.75
				iCulture2 = int(iCulture2)
				
				iCulture1 += iOldCulture
				iCulture1 *= 0.75
				iCulture1 = int(iCulture1)
				
				iLoser = pLosingPlayer.getID()
				pCity.changeCulture(iLoser, iCulture2, True)
			iGaining = pPlayer.getID()
			iCulture1 += self.iFreeCulture
			pCity.changeCulture(iGaining, iCulture1, True)
			
			pCity.setNumRealBuilding(gc.getInfoTypeForString("BUILDING_ENTHUSIASM"), 1)
			if pCity.getNumBuilding(gc.getInfoTypeForString("BUILDING_PALACE")) > 0:
				if pTeam.isHasTech(gc.getInfoTypeForString("TECH_BRONZE_WORKING")):
					pCity.setNumRealBuilding(gc.getInfoTypeForString("BUILDING_LOCAL_BRONZE"), 1)
				if pTeam.isHasTech(gc.getInfoTypeForString("TECH_IRON_WORKING")):
					pCity.setNumRealBuilding(gc.getInfoTypeForString("BUILDING_LOCAL_IRON"), 1)
				if pTeam.isHasTech(gc.getInfoTypeForString("TECH_HORSEBACK_RIDING")):
					pCity.setNumRealBuilding(gc.getInfoTypeForString("BUILDING_LOCAL_HORSES"), 1)					
				if pTeam.isHasTech(gc.getInfoTypeForString("TECH_COAL_MINING")):
					pCity.setNumRealBuilding(gc.getInfoTypeForString("BUILDING_LOCAL_COAL"), 1)		
				if pTeam.isHasTech(gc.getInfoTypeForString("TECH_ALCHEMY_ASTROLOGY")):
					pCity.setNumRealBuilding(gc.getInfoTypeForString("BUILDING_LOCAL_SULFUR"), 1)						
					
			for i in xrange(pPlayer.getCurrentEra() + self.iExtraBuildings + iCapitalBonus):
				RandBuilding = []
				for iBuildingClass in xrange(gc.getNumBuildingClassInfos()):
					if isLimitedWonderClass(iBuildingClass): continue
					iBuilding = gc.getCivilizationInfo(pPlayer.getCivilizationType()).getCivilizationBuildings(iBuildingClass)
					if iBuilding == -1: continue
					if pCity.canConstruct(iBuilding, True, True, False):
						RandBuilding.append(iBuilding)
				if len(RandBuilding) > 0:
					pCity.pushOrder(OrderTypes.ORDER_CONSTRUCT, RandBuilding[CyGame().getSorenRandNum(len(RandBuilding), "Free Building")] , -1, False, False, False, True)
					pCity.popOrder(0, True, False)
				else:
					break

			iWorker = gc.getCivilizationInfo(pPlayer.getCivilizationType()).getCivilizationUnits(self.iWorker)
			if iWorker > -1:
				for i in xrange(self.iNumWorker + iCapitalBonus):
					pNewUnit = pPlayer.initUnit(iWorker, pCity.getX(), pCity.getY(), UnitAITypes.NO_UNITAI, DirectionTypes.NO_DIRECTION)
			iSettler = gc.getCivilizationInfo(pPlayer.getCivilizationType()).getCivilizationUnits(self.iSettler)
			if iSettler > -1:
				for i in xrange(self.iNumSettler + iCapitalBonus):
					pNewUnit = pPlayer.initUnit(iSettler, pCity.getX(), pCity.getY(), UnitAITypes.NO_UNITAI, DirectionTypes.NO_DIRECTION)
			iBestDefender = UnitTypes.NO_UNIT
			for unitID in range(0,gc.getNumUnitInfos()) :
				if( pCity.canTrain(unitID,False,False) and gc.getUnitInfo(unitID).getDomainType() == DomainTypes.DOMAIN_LAND ): 
					unitClass = gc.getUnitInfo(unitID).getUnitClassType()
					if ( gc.getUnitInfo(unitID).getDefaultUnitAIType() == UnitAITypes.UNITAI_CITY_DEFENSE ):
						if( (iBestDefender == UnitTypes.NO_UNIT) or gc.getUnitInfo(unitID).getCombat() >= gc.getUnitInfo(iBestDefender).getCombat() ) :
							if( unitID == gc.getCivilizationInfo( pPlayer.getCivilizationType() ).getCivilizationUnits(unitClass) ) :
								iBestDefender = unitID
			for i in xrange((self.iNumDefender / 2) + iCapitalBonus):
				if( not iBestDefender == UnitTypes.NO_UNIT ) : pNewUnit = pPlayer.initUnit(iBestDefender, pCity.getX(), pCity.getY(), UnitAITypes.UNITAI_CITY_DEFENSE, DirectionTypes.NO_DIRECTION)
				
			iBestAttacker = UnitTypes.NO_UNIT
			for unitID in range(0,gc.getNumUnitInfos()) :
				if( pCity.canTrain(unitID,False,False) and gc.getUnitInfo(unitID).getDomainType() == DomainTypes.DOMAIN_LAND ): 
					unitClass = gc.getUnitInfo(unitID).getUnitClassType()
					if ( gc.getUnitInfo(unitID).getDefaultUnitAIType() == UnitAITypes.UNITAI_ATTACK_CITY ):
						if( (iBestAttacker == UnitTypes.NO_UNIT) or gc.getUnitInfo(unitID).getCombat() >= gc.getUnitInfo(iBestAttacker).getCombat() ) :
							if( unitID == gc.getCivilizationInfo( pPlayer.getCivilizationType() ).getCivilizationUnits(unitClass) ) :
								iBestAttacker = unitID
			for i in xrange((self.iNumDefender / 2) + iCapitalBonus):
				if( not iBestAttacker == UnitTypes.NO_UNIT ) : pNewUnit = pPlayer.initUnit(iBestAttacker, pCity.getX(), pCity.getY(), UnitAITypes.UNITAI_ATTACK_CITY, DirectionTypes.NO_DIRECTION)
				
			pCity.changePopulation(max(0, (self.iPopulationBoost + iCapitalBonus)))
			(pCity, iter) = pPlayer.nextCity(iter, False)
			
		(pUnit, iter) = pBarbPlayer.firstUnit(false)
		while(pUnit):
			pUnit.setImmobileTimer(2)
			(pUnit, iter) = pBarbPlayer.nextUnit(iter, False)

	def getNewID(self):
		for iPlayerX in xrange(gc.getMAX_CIV_PLAYERS()):
			pPlayerX = gc.getPlayer(iPlayerX)
			if pPlayerX.isAlive(): continue
			if pPlayerX.isEverAlive(): continue
			else:
				return iPlayerX
		return -1