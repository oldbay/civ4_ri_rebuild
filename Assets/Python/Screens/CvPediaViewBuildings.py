## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005

from CvPythonExtensions import *
from CvPediaView import CvPediaView

# globals
gc = CyGlobalContext()

class CvPediaViewBuildings(CvPediaView):
	"Civilopedia Buildings View"

	def __init__(self, section, eView, szTitle, filter = None, iFilterData = -1):
		modes = [
			("TXT_KEY_PEDIA_MODE_ALL", CivilopediaModeTypes.CIVILOPEDIA_MODE_ALL),
			("TXT_KEY_PEDIA_MODE_GROUPED", CivilopediaModeTypes.CIVILOPEDIA_MODE_GROUPED),
			("TXT_KEY_PEDIA_MODE_CIVILIZATION", CivilopediaModeTypes.CIVILOPEDIA_MODE_CIVILIZATION),
			("TXT_KEY_PEDIA_MODE_ACTIVE_GAME", CivilopediaModeTypes.CIVILOPEDIA_MODE_ACTIVE_GAME),
			("TXT_KEY_PEDIA_MODE_ACTIVE_PLAYER", CivilopediaModeTypes.CIVILOPEDIA_MODE_ACTIVE_PLAYER)
		]
		
		super(CvPediaViewBuildings, self).__init__(section, eView, szTitle, CivilopediaPageTypes.CIVILOPEDIA_PAGE_BUILDING, WidgetTypes.WIDGET_PEDIA_JUMP_TO_BUILDING, lambda: self.getInfos(gc.getNumBuildingInfos(), gc.getBuildingInfo, filter, iFilterData), modes)
		
		self.groupedItems = lambda: self.getClassifiedInfos(gc.getNumBuildingClassInfos(), gc.getBuildingClassInfo, gc.getBuildingInfo, lambda classInfo: classInfo.getRepresentativeBuilding(), filter, iFilterData)
	
	def getCurrentItems(self):
		ePlayer = gc.getGame().getActivePlayer()
		
		if (self.eMode == CivilopediaModeTypes.CIVILOPEDIA_MODE_GROUPED):
			return self.groupedItems()
		elif (self.eMode == CivilopediaModeTypes.CIVILOPEDIA_MODE_CIVILIZATION):
			currentItems = []
			pCiv = gc.getCivilizationInfo(self.eCivilization)
			for item in self.items():
				eBuilding = item[1]
				if (pCiv.getCivilizationBuildings(gc.getBuildingInfo(eBuilding).getBuildingClassType()) == eBuilding):
					currentItems.append(item)
			return currentItems
		elif (self.eMode == CivilopediaModeTypes.CIVILOPEDIA_MODE_ACTIVE_GAME and ePlayer > -1):
			currentItems = []
			pTeam = gc.getTeam(gc.getPlayer(ePlayer).getTeam())
			pCiv = gc.getCivilizationInfo(gc.getPlayer(ePlayer).getCivilizationType())
			for item in self.items():
				eBuilding = item[1]
				eBuildingClass = gc.getBuildingInfo(eBuilding).getBuildingClassType()
				if (pCiv.getCivilizationBuildings(eBuildingClass) == eBuilding):
					currentItems.append(item) # Quick check for current player
				else:
					for eLoopPlayer in range(gc.getMAX_PLAYERS()):
						pLoopPlayer = gc.getPlayer(eLoopPlayer)
						if (((pLoopPlayer.isAlive() and pTeam.isHasMet(eLoopPlayer)) or pLoopPlayer.isBarbarian()) and gc.getCivilizationInfo(pLoopPlayer.getCivilizationType()).getCivilizationBuildings(eBuildingClass) == eBuilding):
							currentItems.append(item)
							break
			return currentItems
		elif (self.eMode == CivilopediaModeTypes.CIVILOPEDIA_MODE_ACTIVE_PLAYER and ePlayer > -1):
			currentItems = []
			pCiv = gc.getCivilizationInfo(gc.getPlayer(ePlayer).getCivilizationType())
			for item in self.items():
				eBuilding = item[1]
				if (pCiv.getCivilizationBuildings(gc.getBuildingInfo(eBuilding).getBuildingClassType()) == eBuilding):
					currentItems.append(item)
			return currentItems
		else:
			return self.items()
