## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
##
## ReligionConquest (new religion mod component) by Mexico
##
## added 04.NOV.2006 by Mexico
## request by TR team
## 
## v 0.1 - New islam spread on conquered cities: if conqueror have Islam as state religion, this is automaticaly spreaded on newly conquered cities (until Industrial era)


from CvPythonExtensions import *
import CvUtil
import PyHelpers
import Popup as PyPopup


## Globals

gc = CyGlobalContext()


## Religion Conquest

def onCityAcquired(argsList):
	iPreviousOwner,iNewOwner,pCity,bConquest,bTrade = argsList
	
	pPlayer = gc.getPlayer(iNewOwner)
	iIslam = gc.getInfoTypeForString("RELIGION_ISLAM")
	
	if (pPlayer.getStateReligion() == iIslam):
		iEra = pPlayer.getCurrentEra()
		if(iEra < gc.getInfoTypeForString("ERA_INDUSTRIAL")):
			pCity.setHasReligion(iIslam,true,true,false)

