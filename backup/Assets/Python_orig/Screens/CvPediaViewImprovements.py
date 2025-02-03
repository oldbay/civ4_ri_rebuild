## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005

from CvPythonExtensions import *
from CvPediaView import CvPediaView

# globals
gc = CyGlobalContext()

class CvPediaViewImprovements(CvPediaView):
	"Civilopedia Improvements View"
	
	def __init__(self, section, eView, szTitle, filter = None):
		modes = [
			("TXT_KEY_PEDIA_MODE_ALL", CivilopediaModeTypes.CIVILOPEDIA_MODE_ALL),
			("TXT_KEY_PEDIA_MODE_CIVILIZATION", CivilopediaModeTypes.CIVILOPEDIA_MODE_CIVILIZATION),
			("TXT_KEY_PEDIA_MODE_ACTIVE_GAME", CivilopediaModeTypes.CIVILOPEDIA_MODE_ACTIVE_GAME),
			("TXT_KEY_PEDIA_MODE_ACTIVE_PLAYER", CivilopediaModeTypes.CIVILOPEDIA_MODE_ACTIVE_PLAYER)
		]
		
		super(CvPediaViewImprovements, self).__init__(section, eView, szTitle, CivilopediaPageTypes.CIVILOPEDIA_PAGE_IMPROVEMENT, WidgetTypes.WIDGET_PEDIA_JUMP_TO_IMPROVEMENT, lambda: self.getInfos(gc.getNumImprovementInfos(), gc.getImprovementInfo, filter), modes)
	
	def getCurrentItems(self):
		ePlayer = gc.getGame().getActivePlayer()
		
		if (self.eMode == CivilopediaModeTypes.CIVILOPEDIA_MODE_CIVILIZATION):
			currentItems = []
			pCiv = gc.getCivilizationInfo(self.eCivilization)
			for item in self.items():
				eImprovement = item[1]
				if (pCiv.canProduceImprovement(eImprovement)):
					currentItems.append(item)
			return currentItems
		elif (self.eMode == CivilopediaModeTypes.CIVILOPEDIA_MODE_ACTIVE_GAME and ePlayer > -1):
			currentItems = []
			pTeam = gc.getTeam(gc.getPlayer(ePlayer).getTeam())
			pCiv = gc.getCivilizationInfo(gc.getPlayer(ePlayer).getCivilizationType())
			for item in self.items():
				eImprovement = item[1]
				if (pCiv.canProduceImprovement(eImprovement)):
					currentItems.append(item) # Quick check for current player
				else:
					for eLoopPlayer in range(gc.getMAX_PLAYERS()):
						pLoopPlayer = gc.getPlayer(eLoopPlayer)
						if ((pLoopPlayer.isAlive() and (pTeam.isHasMet(eLoopPlayer) or pLoopPlayer.isBarbarian())) and gc.getCivilizationInfo(pLoopPlayer.getCivilizationType()).canProduceImprovement(eImprovement)):
							currentItems.append(item)
							break
			return currentItems
		elif (self.eMode == CivilopediaModeTypes.CIVILOPEDIA_MODE_ACTIVE_PLAYER and ePlayer > -1):
			currentItems = []
			pCiv = gc.getCivilizationInfo(gc.getPlayer(ePlayer).getCivilizationType())
			for item in self.items():
				eImprovement = item[1]
				if (pCiv.canProduceImprovement(eImprovement)):
					currentItems.append(item)
			return currentItems
		else:
			return self.items()
