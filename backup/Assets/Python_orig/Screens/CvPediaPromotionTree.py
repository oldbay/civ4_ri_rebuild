## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
from CvPythonExtensions import *
from CvPediaPage import CvPediaPage
from PyPromotionGraph import PyPromotionGraph

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

class CvPediaPromotionTree(CvPediaPage):
	"Civilopedia Screen for Promotion Tree"
	
	def __init__(self, section):
		super(CvPediaPromotionTree, self).__init__(section, CivilopediaPageTypes.CIVILOPEDIA_PAGE_PROMOTION_TREE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_PROMOTION_TREE, "PromotionTree")
		
		self.PROMOTION_GRAPH_ID = self.szWidget + "Graph"
		
		self.graph = PyPromotionGraph(self, self.PROMOTION_GRAPH_ID)
	
	def calculateLayout(self):
		self.PROMOTION_GRAPH_X = self.main.OUTER_PAGE_AREA_X
		self.PROMOTION_GRAPH_Y = self.main.OUTER_PAGE_AREA_Y - 8
		self.PROMOTION_GRAPH_W = self.main.OUTER_PAGE_AREA_W + 7
		self.PROMOTION_GRAPH_H = self.main.OUTER_PAGE_AREA_H - 14
		
		self.graph.setDimensions(self.PROMOTION_GRAPH_X, self.PROMOTION_GRAPH_Y, self.PROMOTION_GRAPH_W, self.PROMOTION_GRAPH_H)
		self.graph.calculateLayout()
	
	def draw(self):
		self.graph.draw()
	
	def clear(self):
		super(CvPediaPromotionTree, self).clear()
		
		self.graph.clear()
