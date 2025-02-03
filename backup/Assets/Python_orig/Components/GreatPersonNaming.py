## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
##
## GreatPersonNaming by Mexico
##

from CvPythonExtensions import *
import CvUtil
import PyHelpers
import Popup as PyPopup
import RandomNameUtils


## Globals

gc = CyGlobalContext()

g_iCount = 0     # autoincrement global used as the primary key for dictionary
g_dict = dict()  # holds information transferred from here to CvGreatPersonScreen.py


## Great Person Naming

def onGreatPersonBorn(argsList):
	'Great Person Born'
	pUnit, iPlayer, pCity = argsList
	player = gc.getPlayer(iPlayer)

	# Check if we should even show the popup:
	if pUnit.isNone() or pCity.isNone():
		return

	#If Person doesn't have unique name, give him a random one
	if(len(pUnit.getNameNoDesc()) == 0):
		iCivilizationType = player.getCivilizationType()
		pUnit.setName(RandomNameUtils.getRandomCivilizationName(iCivilizationType))

	#Show fancy lil popup if *we* got the great person:
	iActivePlayer = CyGame().getActivePlayer()
	if iActivePlayer == iPlayer:
		# updated by Mexico , based on Gaurav modification
		# now pop-up screen is shown with all others pop-up, so no more cover battles
		if player.isHuman():

			global g_dict
			global g_iCount
			
			g_dict[g_iCount] = (pUnit, iPlayer, pCity)
			
			popupInfo = CyPopupInfo()
			popupInfo.setButtonPopupType(ButtonPopupTypes.BUTTONPOPUP_PYTHON_SCREEN)
			popupInfo.setData1(g_iCount)
			g_iCount += 1
			popupInfo.setText(u"showGreatPersonScreen")
			popupInfo.addPopup(iPlayer)
	# GreatPerson Mod -------- end

