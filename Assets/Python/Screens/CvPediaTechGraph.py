## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
from CvPythonExtensions import *
import CvUtil
import CvPediaPage
import PyTechGraph

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

class CvPediaTechGraph(CvPediaPage.CvPediaPage):
	"Civilopedia Screen for Tech Graph"
	
	def __init__(self, section):
		super(CvPediaTechGraph, self).__init__(section, CivilopediaPageTypes.CIVILOPEDIA_PAGE_TECHNOLOGY_GRAPH, WidgetTypes.WIDGET_GENERAL, "TechGraph")
		
		self.graph = PyTechGraph.PyTechGraph(self, self.szWidget)
		
		self.eMode = CivilopediaModeTypes.CIVILOPEDIA_MODE_ALL
		self.eCivilization = 0
	
	def update(self, fDelta):
		return
	
	def setMode(self, eMode):
		ePlayer = gc.getGame().getActivePlayer()
		
		if (eMode == CivilopediaModeTypes.CIVILOPEDIA_MODE_CIVILIZATION):
			self.eMode = eMode
			self.graph.setCivilization(self.eCivilization)
		elif (eMode == CivilopediaModeTypes.CIVILOPEDIA_MODE_ACTIVE_PLAYER and ePlayer > -1):
			self.eMode = eMode
			self.graph.setPlayer(ePlayer)
		else:
			self.eMode = CivilopediaModeTypes.CIVILOPEDIA_MODE_ALL
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
		self.TECH_GRAPH_X = self.main.OUTER_PAGE_AREA_X
		self.TECH_GRAPH_Y = self.main.OUTER_PAGE_AREA_Y - 8
		self.TECH_GRAPH_W = self.main.OUTER_PAGE_AREA_W + 7
		self.TECH_GRAPH_H = self.main.OUTER_PAGE_AREA_H - 14
		
		self.graph.calculateLayout()
	
	def draw(self):
		self.graph.draw()
	
	def clear(self):
		super(CvPediaTechGraph, self).clear()
		
		self.graph.clear()
