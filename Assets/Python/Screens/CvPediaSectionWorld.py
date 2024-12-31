## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
from CvPythonExtensions import *
import string
import CvUtil
import ScreenInput
import CvScreenEnums

from CvPediaSection import CvPediaSection
from CvPediaView import CvPediaView
from CvPediaViewImprovements import CvPediaViewImprovements
from CvPediaTerrain import CvPediaTerrain
from CvPediaFeature import CvPediaFeature
from CvPediaBonus import CvPediaBonus
from CvPediaImprovement import CvPediaImprovement
from CvPediaWorld import CvPediaWorld
from CvPediaRoute import CvPediaRoute
from CvPediaHandicap import CvPediaHandicap

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

class CvPediaSectionWorld(CvPediaSection):
	"Civilopedia Section for World"

	def __init__(self, main):
		super(CvPediaSectionWorld, self).__init__(main, CivilopediaSectionTypes.CIVILOPEDIA_SECTION_WORLD, "TXT_KEY_PEDIA_SECTION_WORLD", "World")
		
		self.views = [
			CvPediaView(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_WORLD_BASE_TERRAIN, "TXT_KEY_PEDIA_VIEW_WORLD_BASE_TERRAIN", CivilopediaPageTypes.CIVILOPEDIA_PAGE_TERRAIN, WidgetTypes.WIDGET_PEDIA_JUMP_TO_TERRAIN, lambda: self.getInfos(gc.getNumTerrainInfos(), gc.getTerrainInfo)),
			CvPediaView(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_WORLD_TERRAIN_FEATURES, "TXT_KEY_PEDIA_VIEW_WORLD_TERRAIN_FEATURES", CivilopediaPageTypes.CIVILOPEDIA_PAGE_FEATURE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_FEATURE, lambda: self.getInfos(gc.getNumFeatureInfos(), gc.getFeatureInfo)),
			CvPediaView(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_WORLD_RESOURCES, "TXT_KEY_PEDIA_VIEW_WORLD_RESOURCES", CivilopediaPageTypes.CIVILOPEDIA_PAGE_BONUS, WidgetTypes.WIDGET_PEDIA_JUMP_TO_BONUS, lambda: self.getInfos(gc.getNumBonusInfos(), gc.getBonusInfo)),
			CvPediaViewImprovements(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_WORLD_IMPROVEMENTS, "TXT_KEY_PEDIA_VIEW_WORLD_IMPROVEMENTS"),
			CvPediaView(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_WORLD_MAP_SIZES, "TXT_KEY_PEDIA_VIEW_WORLD_MAP_SIZES", CivilopediaPageTypes.CIVILOPEDIA_PAGE_WORLD, WidgetTypes.WIDGET_PEDIA_JUMP_TO_WORLD, lambda: self.getInfos(gc.getNumWorldInfos(), gc.getWorldInfo), [], []),
			CvPediaView(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_WORLD_ROUTES, "TXT_KEY_PEDIA_VIEW_WORLD_ROUTES", CivilopediaPageTypes.CIVILOPEDIA_PAGE_ROUTE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_ROUTE, lambda: self.getInfos(gc.getNumRouteInfos(), gc.getRouteInfo), [], []),
			CvPediaView(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_WORLD_DIFFICULTY_LEVELS, "TXT_KEY_PEDIA_VIEW_DIFFICULTY_LEVELS", CivilopediaPageTypes.CIVILOPEDIA_PAGE_HANDICAP, WidgetTypes.WIDGET_PEDIA_JUMP_TO_HANDICAP, lambda: self.getInfos(gc.getNumHandicapInfos(), gc.getHandicapInfo), [], []),
		]
		
		self.pages = {
			CivilopediaPageTypes.CIVILOPEDIA_PAGE_TERRAIN : CvPediaTerrain(self),
			CivilopediaPageTypes.CIVILOPEDIA_PAGE_FEATURE : CvPediaFeature(self),
			CivilopediaPageTypes.CIVILOPEDIA_PAGE_BONUS : CvPediaBonus(self),
			CivilopediaPageTypes.CIVILOPEDIA_PAGE_IMPROVEMENT : CvPediaImprovement(self),
			CivilopediaPageTypes.CIVILOPEDIA_PAGE_WORLD : CvPediaWorld(self),
			CivilopediaPageTypes.CIVILOPEDIA_PAGE_ROUTE : CvPediaRoute(self),
			CivilopediaPageTypes.CIVILOPEDIA_PAGE_HANDICAP : CvPediaHandicap(self)
		}
		
		self.setDefaults()
	