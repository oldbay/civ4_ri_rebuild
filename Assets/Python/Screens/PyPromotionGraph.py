## Sid Meier's Civilization 4
## 
## Promotion Graph
##

from CvPythonExtensions import *
from PyDirectedAcyclicGraph import PyDirectedAcyclicGraph

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()

class PyPromotionGraph(PyDirectedAcyclicGraph):
	"Promotion Graph"

	def __init__(self, container, szWidget):
		super(PyPromotionGraph, self).__init__(container, szWidget)
	
	def calculateLayout(self):
		super(PyPromotionGraph, self).calculateLayout()
		
		# Spacing refers to the space between icons
		self.SPACING_X = 75
	
	def getNumberOfElements(self):
		return gc.getNumPromotionInfos()
	
	def getElementType(self, ePromotion):
		"Returns the type of the elements with the specified id"
		return gc.getPromotionInfo(ePromotion).getType()
	
	def addGraphEdges(self, graph):
		for ePromotionA in graph.iterkeys():
			ePromotionD = gc.getPromotionInfo(ePromotionA).getPrereqPromotion()
			self.addEdge(graph, ePromotionD, ePromotionA)
			ePromotionB = gc.getPromotionInfo(ePromotionA).getPrereqOrPromotion1()
			self.addEdge(graph, ePromotionB, ePromotionA)
			ePromotionC = gc.getPromotionInfo(ePromotionA).getPrereqOrPromotion2()
			self.addEdge(graph, ePromotionC, ePromotionA)
			# K-Mod, extra prereq
			ePromotionE = gc.getPromotionInfo(ePromotionA).getPrereqOrPromotion3()
			self.addEdge(graph, ePromotionE, ePromotionA)
	
	def drawElement(self, screen, ePromotion, xPos, yPos):
		screen.setImageButtonAt(self.record(self.GRAPH_BUTTON_ID + str(ePromotion)), self.GRAPH_ID, gc.getPromotionInfo(ePromotion).getButton(), xPos, yPos, self.ICON_SIZE, self.ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_PROMOTION, ePromotion, 1)
	
	def elementToString(self, ePromotion):
		return gc.getPromotionInfo(ePromotion).getDescription() + ":%d"%(ePromotion, )
