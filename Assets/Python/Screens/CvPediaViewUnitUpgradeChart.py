## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005

from CvPythonExtensions import *
from CvPediaView import CvPediaView

# globals
gc = CyGlobalContext()

class CvPediaViewUnitUpgradeChart(CvPediaView):
	"Civilopedia Unit Upgrade Chart View"

	def __init__(self, section, eView, szTitle, filter = None):
		modes = [
			("TXT_KEY_PEDIA_MODE_DEFAULT_UNITS", CivilopediaModeTypes.CIVILOPEDIA_MODE_GROUPED),
			("TXT_KEY_PEDIA_MODE_CIVILIZATION", CivilopediaModeTypes.CIVILOPEDIA_MODE_CIVILIZATION),
			("TXT_KEY_PEDIA_MODE_ACTIVE_PLAYER", CivilopediaModeTypes.CIVILOPEDIA_MODE_ACTIVE_PLAYER)
		]
		
		super(CvPediaViewUnitUpgradeChart, self).__init__(section, eView, szTitle, CivilopediaPageTypes.CIVILOPEDIA_PAGE_UNIT_UPGRADE_CHART, WidgetTypes.WIDGET_PEDIA_JUMP_TO_UNIT, lambda: [], modes)
