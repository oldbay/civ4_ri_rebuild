## Sid Meier's Civilization 4
## 
## Building Upgrade Graph
##

from CvPythonExtensions import *
from PyPlayerDirectedAcyclicGraph import PyPlayerDirectedAcyclicGraph

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()

class PyBuildingUpgradeGraph(PyPlayerDirectedAcyclicGraph):
	"Building Upgrade Graph"
	
	def __init__(self, container, szWidget):
		super(PyBuildingUpgradeGraph, self).__init__(container, szWidget)
	
	def getNumberOfElements(self):
		return gc.getNumBuildingClassInfos()
	
	def translateElement(self, eBuildingClass):
		"Performs any translation necessary for the given element id"
		if self.getPlayer() > -1:
			return gc.getCivilizationInfo(gc.getPlayer(self.getPlayer()).getCivilizationType()).getCivilizationBuildings(eBuildingClass)
		elif self.getCivilization() > -1:
			return gc.getCivilizationInfo(self.getCivilization()).getCivilizationBuildings(eBuildingClass)
		else:
			return gc.getBuildingClassInfo(eBuildingClass).getDefaultBuildingIndex()
	
	def getElementType(self, eBuilding):
		"Returns the type of the element with the specified id"
		return gc.getBuildingInfo(eBuilding).getType()
	
	def addGraphEdges(self, graph):
		for eBuildingA in graph.iterkeys():
			kBuildingA = gc.getBuildingInfo(eBuildingA)
			for eLoopBuildingClass in range(gc.getNumBuildingClassInfos()):
				if kBuildingA.getObsoleteBuildingClass() == eLoopBuildingClass:
					eBuildingB = self.translateElement(eLoopBuildingClass)
					self.addEdge(graph, eBuildingA, eBuildingB)
	
	def drawElement(self, screen, eBuilding, xPos, yPos):
		screen.setImageButtonAt(self.record(self.GRAPH_BUTTON_ID + str(eBuilding)), self.GRAPH_ID, gc.getBuildingInfo(eBuilding).getButton(), xPos, yPos, self.ICON_SIZE, self.ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_BUILDING, eBuilding, 1)
	
	def elementToString(self, eBuilding):
		return gc.getBuildingInfo(eBuilding).getDescription() + ":%d"%(eBuilding, )
