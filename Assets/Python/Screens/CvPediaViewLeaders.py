## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005

from CvPythonExtensions import *
from CvPediaView import CvPediaView

# globals
gc = CyGlobalContext()

class CvPediaViewLeaders(CvPediaView):
	"Civilopedia Leaders View"

	def __init__(self, section, eView, szTitle, filter = None):
		modes = [
			("TXT_KEY_PEDIA_MODE_ALL", CivilopediaModeTypes.CIVILOPEDIA_MODE_ALL),
			("TXT_KEY_PEDIA_MODE_PLAYABLE", CivilopediaModeTypes.CIVILOPEDIA_MODE_PLAYABLE),
			("TXT_KEY_PEDIA_MODE_NONPLAYABLE", CivilopediaModeTypes.CIVILOPEDIA_MODE_NONPLAYABLE),
			("TXT_KEY_PEDIA_MODE_ACTIVE_GAME", CivilopediaModeTypes.CIVILOPEDIA_MODE_ACTIVE_GAME)
		]
		
		super(CvPediaViewLeaders, self).__init__(section, eView, szTitle, CivilopediaPageTypes.CIVILOPEDIA_PAGE_LEADER, WidgetTypes.WIDGET_PEDIA_JUMP_TO_LEADER, lambda: self.getInfos(gc.getNumLeaderHeadInfos(), gc.getLeaderHeadInfo, filter), modes)
	
	def getCurrentItems(self):
		ePlayer = gc.getGame().getActivePlayer()
		
		if (self.eMode == CivilopediaModeTypes.CIVILOPEDIA_MODE_PLAYABLE):
			currentItems = []
			for item in self.items():
				if (gc.getLeaderHeadInfo(item[1]).isPlayable()):
					currentItems.append(item)
			return currentItems
		elif (self.eMode == CivilopediaModeTypes.CIVILOPEDIA_MODE_NONPLAYABLE):
			currentItems = []
			for item in self.items():
				if (not gc.getLeaderHeadInfo(item[1]).isPlayable()):
					currentItems.append(item)
			return currentItems
		elif (self.eMode == CivilopediaModeTypes.CIVILOPEDIA_MODE_ACTIVE_GAME and ePlayer > -1):
			currentItems = []
			pTeam = gc.getTeam(gc.getPlayer(ePlayer).getTeam())
			for item in self.items():
				eLeader = item[1]
				for eLoopPlayer in range(gc.getMAX_PLAYERS()):
					pLoopPlayer = gc.getPlayer(eLoopPlayer)
					if (pLoopPlayer.isAlive() and pLoopPlayer.getLeaderType() == eLeader and (pTeam.isHasMet(eLoopPlayer) or pLoopPlayer.isBarbarian())):
						currentItems.append(item)
			return currentItems
		else:
			return self.items()
