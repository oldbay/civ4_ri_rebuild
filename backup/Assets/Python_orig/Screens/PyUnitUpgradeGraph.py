## Sid Meier's Civilization 4
## 
## Unit Upgrade Graph
##

from CvPythonExtensions import *
from PyPlayerDirectedAcyclicGraph import PyPlayerDirectedAcyclicGraph

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()

class PyUnitUpgradeGraph(PyPlayerDirectedAcyclicGraph):
	"Unit Upgrade Graph"
	
	def __init__(self, container, szWidget):
		super(PyUnitUpgradeGraph, self).__init__(container, szWidget)
	
	def getNumberOfElements(self):
		return gc.getNumUnitClassInfos()
	
	def translateElement(self, eUnitClass):
		"Performs any translation necessary for the given element id"
		if self.getPlayer() > -1:
			return gc.getCivilizationInfo(gc.getPlayer(self.getPlayer()).getCivilizationType()).getCivilizationUnits(eUnitClass)
		elif self.getCivilization() > -1:
			return gc.getCivilizationInfo(self.getCivilization()).getCivilizationUnits(eUnitClass)
		else:
			return gc.getUnitClassInfo(eUnitClass).getDefaultUnitIndex()
	
	def getElementType(self, eUnit):
		"Returns the type of the element with the specified id"
		return gc.getUnitInfo(eUnit).getType()
	
	def addGraphEdges(self, graph):
		for eUnitA in graph.iterkeys():
			kUnitA = gc.getUnitInfo(eUnitA)
			for eLoopUnitClass in range(gc.getNumUnitClassInfos()):
				if kUnitA.getUpgradeUnitClass(eLoopUnitClass):
					eUnitB = self.translateElement(eLoopUnitClass)
					self.addEdge(graph, eUnitA, eUnitB)
	
	def drawElement(self, screen, eUnit, xPos, yPos):
		if self.getCivilization() > -1:
			iArtstyle = gc.getArtStyleTypes(gc.getCivilizationInfo(self.getCivilization()).getArtStyleType())
			screen.setImageButtonAt(self.record(self.GRAPH_BUTTON_ID + str(eUnit)), self.GRAPH_ID, gc.getUnitInfo(eUnit).getStyledButton(self.getCivilization()), xPos, yPos, self.ICON_SIZE, self.ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_UNIT, eUnit, 1)	
		else:
			screen.setImageButtonAt(self.record(self.GRAPH_BUTTON_ID + str(eUnit)), self.GRAPH_ID, gc.getUnitInfo(eUnit).getButton(), xPos, yPos, self.ICON_SIZE, self.ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_UNIT, eUnit, 1)
	
	def elementToString(self, eUnit):
		return gc.getUnitInfo(eUnit).getDescription() + ":%d"%(eUnit, )
