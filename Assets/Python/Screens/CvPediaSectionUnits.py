## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
from CvPythonExtensions import *

from CvPediaSection import CvPediaSection
from CvPediaView import CvPediaView
from CvPediaViewUnits import CvPediaViewUnits
from CvPediaViewUnitUpgradeChart import CvPediaViewUnitUpgradeChart
from CvPediaUnit import CvPediaUnit
#from CvPediaUnitClass import CvPediaUnitClass
from CvPediaUnitCombat import CvPediaUnitCombat
from CvPediaUnitUpgradeChart import CvPediaUnitUpgradeChart

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

class CvPediaSectionUnits(CvPediaSection):
	"Civilopedia Section for Units"

	def __init__(self, main):
		super(CvPediaSectionUnits, self).__init__(main, CivilopediaSectionTypes.CIVILOPEDIA_SECTION_UNITS, "TXT_KEY_PEDIA_SECTION_UNITS", "Units")
		
		self.views = [
			CvPediaViewUnits(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_UNITS_ALL, "TXT_KEY_PEDIA_VIEW_UNITS_ALL"),
			#(u"Comparison", CivilopediaViewTypes.CIVILOPEDIA_VIEW_UNITS_COMPARISON, -1, CivilopediaPageTypes.CIVILOPEDIA_PAGE_UNIT),
			CvPediaView(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_UNITS_CATEGORIES, "TXT_KEY_PEDIA_VIEW_UNITS_CATEGORIES", CivilopediaPageTypes.CIVILOPEDIA_PAGE_UNIT_COMBAT, WidgetTypes.WIDGET_PEDIA_JUMP_TO_UNIT_COMBAT, lambda: self.getInfos(gc.getNumUnitCombatInfos(), gc.getUnitCombatInfo)),
			CvPediaViewUnitUpgradeChart(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_UNITS_UPGRADE_CHART, "TXT_KEY_PEDIA_VIEW_UNITS_UPGRADE_CHART")
		]
		
		self.pages = {
			CivilopediaPageTypes.CIVILOPEDIA_PAGE_UNIT : CvPediaUnit(self),
			CivilopediaPageTypes.CIVILOPEDIA_PAGE_UNIT_COMBAT : CvPediaUnitCombat(self),
			CivilopediaPageTypes.CIVILOPEDIA_PAGE_UNIT_UPGRADE_CHART : CvPediaUnitUpgradeChart(self)
		}
		
		self.setDefaults()
