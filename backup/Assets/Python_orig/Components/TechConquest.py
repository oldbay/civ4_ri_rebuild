## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
##
## TechConquest by Bhruic
##

from CvPythonExtensions import *
import CvUtil
import PyHelpers
import Popup as PyPopup


## Globals

gc = CyGlobalContext()


## Tech Conquest

def onCityAcquired(argsList):
	eOldOwner, ePlayer, pOldCity, bConquest, bTrade = argsList
	if (bConquest):
		pOldTeam = gc.getTeam(gc.getPlayer(eOldOwner).getTeam())
		pNewOwner = gc.getPlayer(ePlayer)
		pNewTeam = gc.getTeam(pNewOwner.getTeam())
		lTechDiff = []
		for iLoopTech in range(gc.getNumTechInfos()):
			if ((pOldTeam.isHasTech(iLoopTech)) and (not pNewTeam.isHasTech(iLoopTech)) and (pNewOwner.canResearch(iLoopTech, False))):
				lTechDiff.append(iLoopTech)
		if (len(lTechDiff) > 0):
			iRandTech = gc.getGame().getSorenRandNum(len(lTechDiff), "TechConquest")
			eTech = lTechDiff[iRandTech]
			iTechCost = pNewTeam.getResearchCost(eTech)

			iPercent = pOldCity.getPopulation() * 10
			iPercent = gc.getGame().getSorenRandNum(iPercent, "TechConquest")
			iBaseTechPoints  = int(iTechCost / 100 * 30)
			iExtraTechPoints = int(iTechCost / 100 * iPercent)
			iTechPoints = iBaseTechPoints + iExtraTechPoints

			iCurrentTechPoints = pNewTeam.getResearchProgress(eTech)
			if ((iCurrentTechPoints + iTechPoints) > iTechCost):
				iTechPoints = iTechCost - iCurrentTechPoints

			pNewTeam.changeResearchProgress(eTech, iTechPoints, ePlayer)

			pActivePlayer = gc.getActivePlayer()
			if (pActivePlayer.getTeam() == pNewOwner.getTeam()):
				if (pActivePlayer.isHuman()):
					popup = PyPopup.PyPopup()
					popup.setBodyString("From the inhabitants of %s, you learn SOME of the secrets of %s" 
						%(pOldCity.getName(), gc.getTechInfo(eTech).getDescription()))
					popup.launch()



