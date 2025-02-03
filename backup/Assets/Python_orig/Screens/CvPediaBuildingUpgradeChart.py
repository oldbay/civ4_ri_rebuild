## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
from CvPythonExtensions import *
from CvPediaPage import CvPediaPage
from PyBuildingUpgradeGraph import PyBuildingUpgradeGraph

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

class CvPediaBuildingUpgradeChart(CvPediaPage):
	"Civilopedia Screen for Building Upgrade Chart"
	
	def __init__(self, section):
		super(CvPediaBuildingUpgradeChart, self).__init__(section, CivilopediaPageTypes.CIVILOPEDIA_PAGE_BUILDING_UPGRADE_CHART, WidgetTypes.WIDGET_PEDIA_JUMP_TO_BUILDING_UPGRADE_CHART, "BuildingUpgradeChart")
		
		self.UPGRADE_GRAPH_ID = self.szWidget + "Graph"
		
		self.graph = PyBuildingUpgradeGraph(self, self.UPGRADE_GRAPH_ID)
		
		self.eMode = CivilopediaModeTypes.CIVILOPEDIA_MODE_GROUPED
		self.eCivilization = 0
	
	def setMode(self, eMode):
		ePlayer = gc.getGame().getActivePlayer()
		
		if (eMode == CivilopediaModeTypes.CIVILOPEDIA_MODE_CIVILIZATION):
			self.eMode = eMode
			self.graph.setCivilization(self.eCivilization)
		elif (eMode == CivilopediaModeTypes.CIVILOPEDIA_MODE_ACTIVE_PLAYER and ePlayer > -1):
			self.eMode = eMode
			self.graph.setPlayer(ePlayer)
		else:
			self.eMode = CivilopediaModeTypes.CIVILOPEDIA_MODE_GROUPED
			self.graph.setCivilization(-1)
	
	def setCivilization(self, eCivilization):
		if (self.eCivilization == eCivilization):
			return
		
		self.eCivilization = eCivilization
		
		self.graph.setCivilization(eCivilization)
	
	def updatePlayerContext(self):
		ePlayer = gc.getGame().getActivePlayer()
		if (ePlayer == -1):
			# Check the validity of the current mode
			self.setMode(self.eMode)
		else:
			self.setMode(CivilopediaModeTypes.CIVILOPEDIA_MODE_ACTIVE_PLAYER)
	
	def calculateLayout(self):
		self.UPGRADE_GRAPH_X = self.main.OUTER_PAGE_AREA_X
		self.UPGRADE_GRAPH_Y = self.main.OUTER_PAGE_AREA_Y - 8
		self.UPGRADE_GRAPH_W = self.main.OUTER_PAGE_AREA_W + 7
		self.UPGRADE_GRAPH_H = self.main.OUTER_PAGE_AREA_H - 14
		
		self.graph.setDimensions(self.UPGRADE_GRAPH_X, self.UPGRADE_GRAPH_Y, self.UPGRADE_GRAPH_W, self.UPGRADE_GRAPH_H)
		self.graph.calculateLayout()
	
	def draw(self):
		self.graph.draw()
	
	def clear(self):
		super(CvPediaBuildingUpgradeChart, self).clear()
		
		self.graph.clear()
