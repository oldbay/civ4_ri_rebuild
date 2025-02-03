## Sid Meier's Civilization 4
## 
## Unit Upgrade Graph
##

from CvPythonExtensions import *
from PyDirectedAcyclicGraph import PyDirectedAcyclicGraph


# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()

class PyPlayerDirectedAcyclicGraph(PyDirectedAcyclicGraph):
	"Player Directed Acyclic Graph"
	
	def __init__(self, container, szWidget):
		super(PyPlayerDirectedAcyclicGraph, self).__init__(container, szWidget)
		
		self.ePlayer = -1
		self.eCivilization = -1
	
	def setCivilization(self, eCivilization):
		if (eCivilization != -1 and eCivilization == gc.getGame().getActiveCivilizationType()):
			ePlayer = gc.getGame().getActivePlayer()
			if (ePlayer != self.getPlayer() or eCivilization != self.getCivilization()):
				self.ePlayer = ePlayer
				self.eCivilization = eCivilization
				if (not self.isEmpty()):
					self.redraw()
		elif (eCivilization != self.getCivilization() or self.getPlayer() != -1):
			self.ePlayer = -1
			self.eCivilization = eCivilization
			if (not self.isEmpty()):
				self.redraw()
	
	def getCivilization(self):
		return self.eCivilization
	
	def setPlayer(self, ePlayer):
		if (ePlayer != -1):
			eCivilization = gc.getPlayer(ePlayer).getCivilizationType()
			if (ePlayer != self.getPlayer() or eCivilization != self.getCivilization()):
				self.ePlayer = ePlayer
				self.eCivilization = eCivilization
				if (not self.isEmpty()):
					self.redraw()
		elif (self.getPlayer() != -1 or self.getCivilization() != -1):
			self.ePlayer = -1
			self.eCivilization = -1
			if (not self.isEmpty()):
				self.redraw()
	
	def getPlayer(self):
		return self.ePlayer
	
	def redraw(self):
		self.clearElements()
		self.calculateLayout()
		self.draw()
