## Sid Meier's Civilization 4
## 
## Directed Acyclic Graph
##
## Derived from the UnitUpgradesPediaMod by Vovan
## Automatic layout algorithm by Progor
##

import string

from CvPythonExtensions import *

import CvUtil

# BUG - Mac Support - start
import BugUtil
BugUtil.fixSets(globals())
# BUG - Mac Support - end

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()


################################### BEGIN CLASS DEFINITIONS ###########################################
#Don't change below unless you know what you're doing

class Node:
	"This node holds all necessary information for a single element"
	
	def __init__(self):
		self.x = 999
		self.y = 999
		self.connectsTo = set()
		self.connectsFrom = set()
		self.seen = False
		
	def __repr__(self):
		return "Node<x: %i, y: %i, to: %s, from: %s, seen: %s>"%(self.x, self.y, self.connectsTo, self.connectsFrom, self.seen)

class MGraph:
	"This graph is a collection of element nodes with multiple access methods for fast topological sorting"
	
	def __init__(self):
		self.graph = {}
		self.matrix = []
		self.depth = 0
		self.width = 0
	
	def __repr__(self):
		return "MGraph<depth: %i, width: %i, graph: %s, matrix: %s>"%(self.depth, self.width, self.graph, self.matrix)

class PyDirectedAcyclicGraph(object):
	"Directed Acyclic Graph"
	
	def __init__(self, container, szGraphID):
		self.container = container
		
		self.mGraphs = []
		
		self.widgets = set()
		
		self.GRAPH_ID = szGraphID
		self.GRAPH_ARROW_ID = self.GRAPH_ID + "_A"
		self.GRAPH_LINE_ID = self.GRAPH_ID + "_L"
		self.GRAPH_BUTTON_ID = self.GRAPH_ID + "_B"
		
		#The exception list allow you to completely hide a unit from the upgrade graph.
		#This is useful for mods that use an unreachable unit to keep others from expiring.
		#unitExceptionList = [UNIT_UNREACHABLE]
		self.exceptionList = []
		
		#The split lists let you split a particular unit into several different graphs, so it doesn't hold several
		#upgrade paths together that would look better separate.  For instance, you might have a Ranger unit that
		#upgrades from both a Scout and Warrior unit, but otherwise Scouting and Fighting units are in their own
		#trees.  In this case, you would place the Ranger in the SplitIncoming list, to get the program to split up
		#the incoming upgrades to Ranger so that Ranger shows up in both trees without holding them together.
		#unitSplitIncoming = [UNIT_RANGER, UNIT_STYGIAN_GUARD]
		self.splitIncoming = []
		
		#SplitOutgoing is similar, but for a unit that holds together two trees by what it upgrades to.  For instance,
		#you might have a commoner unit that upgrades to Scout, Warrior, and Worker, while otherwise those units have
		#their own trees.  Using the SplitOutgoing list will just put a commoner at the start of each of the 3 trees
		#unitSplitOutgoing = [UNIT_DWARVEN_SOLDIER]
		self.splitOutgoing = []
	
	def cacheArtInfo(self):
		self.LINE_ARROW = ArtFileMgr.getInterfaceArtInfo("LINE_ARROW").getPath()
		self.LINE_TLBR = ArtFileMgr.getInterfaceArtInfo("LINE_TLBR").getPath()
		self.LINE_BLTR = ArtFileMgr.getInterfaceArtInfo("LINE_BLTR").getPath()
		self.LINE_STRAIT = ArtFileMgr.getInterfaceArtInfo("LINE_STRAIT").getPath()
	
	def setDimensions(self, iX, iY, iW, iH):
		self.GRAPH_X = iX
		self.GRAPH_Y = iY
		self.GRAPH_W = iW
		self.GRAPH_H = iH
	
	def calculateLayout(self):
		self.build()
		
		# Margin is the distance from the edge and the space between graphs
		self.MARGIN_X = 20
		self.MARGIN_Y = 20
		
		# Spacing refers to the space between icons
		self.SPACING_X = 40
		self.SPACING_Y = 5
		
		# Default is 64, but if your graph is too large to fit in the Pedia window, specify a smaller button size here.
		self.ICON_SIZE = 64
	
	################## Virtual methods to be overwritten by subclasses ##################
	
	def getNumberOfElements(self):
		return 0
	
	def translateElement(self, eID):
		"Performs any translation necessary for the given element id"
		return eID
	
	def getElementType(self, eID):
		"Returns the type of the element with the specified id"
		return ""

	def addGraphEdges(self, graph):
		return

	def drawElement(self, screen, element, xPos, yPos):
		return
	
	def elementToString(self, unit):
		return str(unit)
	
	################## Graph Construction ##################
	
	def addEdge(self, graph, elementFrom, elementTo):
		# Check if element numbers are valid
		if (elementFrom >= 0 and graph.has_key(elementFrom) and elementTo >= 0 and graph.has_key(elementTo)):
			graph[elementFrom].connectsTo.add(elementTo)
			graph[elementTo].connectsFrom.add(elementFrom)
			#CvUtil.pyPrint(self.elementToString(elementFrom) + " connects to " + self.elementToString(elementTo) + ".")
	
	def getMedianY(self, mGraph, elementSet):
		"Returns the average Y position of the elements in elementSet"
		
		if (len(elementSet) == 0):
			return -1
		sum = 0.0
		num = 0.0
		for element in elementSet:
			sum += mGraph.graph[element].y
			num += 1
		return sum/num
	
	def swap(self, mGraph, x, yA, yB):
		"Swaps two elements in a given row"
		
		elementA = mGraph.matrix[x][yA]
		elementB = mGraph.matrix[x][yB]
		if (elementA != "E"):
			mGraph.graph[elementA].y = yB
		if (elementB != "E"):
			mGraph.graph[elementB].y = yA
		mGraph.matrix[x][yA] = elementB
		mGraph.matrix[x][yB] = elementA
		return
	
	def build(self):
		"Goes through all the elements and adds paths to the graph. The MGraph data structure is complete by the end of this function."
		
		self.mGraphs = []
		
		self.mGraphs.append(MGraph())
		graph = self.mGraphs[0].graph
		
		for k in range(self.getNumberOfElements()):
			element = self.translateElement(k)
			if (element == -1):
				continue
			if (self.getElementType(element) not in self.exceptionList):
				graph[element] = Node()
		
		self.addGraphEdges(graph)
		#CvUtil.pyPrint("1:\n" + str(graph))
		
		#remove elements that don't connect to or from anything
		for element in graph.keys():
			if (len(graph[element].connectsTo) == 0 and len(graph[element].connectsFrom) == 0):
				del(graph[element])
		#CvUtil.pyPrint("2:\n" + str(graph))
		
## Platy Fix ##
		if len(graph) == 0: return
## Platy Fix ##		
		#split the graph into several disconnected graphs, filling out the rest of the data structure as we go
		mGraphIndex = 0
		while (len(self.mGraphs) > mGraphIndex):
			mGraph = self.mGraphs[mGraphIndex]
			self.mGraphs.append(MGraph())
			newMGraph = self.mGraphs[mGraphIndex + 1]
			
			#Pick a "random" element and mark it as order 0, then make all its successors higher and predecessors lower
			#We can fix that to the range (0..depth) in a moment, and we've already marked everything that's connected
			unit = mGraph.graph.iterkeys().next()
			mGraph.graph[unit].x = 0
			map = {}
			map[0] = set([unit])
## Platy Fix ##
			bDone = False
			while not bDone:
				bDone = True
				for level in xrange(min(map.keys()), max(map.keys())+1):
					for unit in map[level].copy():
						node = mGraph.graph[unit]
						if node.x != level: continue
						for u in node.connectsTo:
							nodeB = mGraph.graph[u]
							nodeB.x = level + 1
							if (not map.has_key(nodeB.x)):
								map[nodeB.x] = set()
							if not u in map[nodeB.x]:
								map[nodeB.x].add(u)
								bDone = False
				for level in xrange (max(map.keys()), min(map.keys()) - 1, -1):
					for unit in map[level].copy():
						node = mGraph.graph[unit]
						if node.x != level: continue
						for u in node.connectsFrom:
							nodeB = mGraph.graph[u]
							nodeB.x = level - 1
							if (not map.has_key(nodeB.x)):
								map[nodeB.x] = set()
							if not u in map[nodeB.x]:
								map[nodeB.x].add(u)
								bDone = False
## Platy Fix ##
			highOrder = max(map.keys())
			lowOrder = min(map.keys())
			map = 0
			mGraph.depth = highOrder - lowOrder + 1
						
			#Now we can move anything that isn't marked with an order to the next MGraph
			#if there's nothing to move, we're done after this iteration
			for (element, node) in mGraph.graph.items():
				if (node.x == 999):
					newMGraph.graph[element] = node
					del(mGraph.graph[element])
				else:
					node.x -= lowOrder
					
			if (len(newMGraph.graph) == 0):
				del(self.mGraphs[mGraphIndex + 1])

			mGraphIndex += 1
		
		#CvUtil.pyPrint("3:\n" + str(self.mGraphs))
			
		for mGraph in self.mGraphs:
			#remove links that would otherwise have to jump 
			for (element, node) in mGraph.graph.iteritems():
				for u in node.connectsTo.copy():
					if (not mGraph.graph.has_key(u)):
						node.connectsTo.remove(u)
				for u in node.connectsFrom.copy():
					if (not mGraph.graph.has_key(u)):
						node.connectsFrom.remove(u)
			

			nextDummy = -1
			#For any upgrade path that crosses more than one level, insert dummy nodes in between
			for (elementA, nodeA) in mGraph.graph.items():
				for elementB in nodeA.connectsTo.copy():
					nodeB = mGraph.graph[elementB]
					if (nodeB.x - nodeA.x > 1):
## Platy Fix ##
						if elementB in nodeA.connectsTo:
							nodeA.connectsTo.remove(elementB)
						if elementA in nodeB.connectsFrom:
							nodeB.connectsFrom.remove(elementA)
## Platy Fix ##
						n = nodeA.x + 1
						nodeA1 = nodeA # original node A
# Begin Promotions Graph fix for Warlords by Gaurav
						while (n < nodeB.x):
							nodeA.connectsTo.add(nextDummy)
							mGraph.graph[nextDummy] = Node()
							nodeA = mGraph.graph[nextDummy]
							if n == nodeA1.x + 1:
								nodeA.connectsFrom.add(elementA)
							else:
								nodeA.connectsFrom.add(nextDummy + 1)
# End Promotions Graph fix for Warlords by Gaurav
							nodeA.x = n
							n += 1
							nextDummy -= 1
						nodeA.connectsTo.add(elementB)
						nodeB.connectsFrom.add(nextDummy + 1)
						

			#Now we can build the matrix from the order data
			#make sure the matrix is <depth> deep
			while(len(mGraph.matrix) < mGraph.depth):
				mGraph.matrix.append([])
			
			#fill out node.y and the matrix
			for (element, node) in mGraph.graph.iteritems():
				node.y = len(mGraph.matrix[node.x])
				mGraph.matrix[node.x].append(element)
				if (node.y >= mGraph.width):
					mGraph.width = node.y + 1

			#make all rows of the matrix the same width
			for row in mGraph.matrix:
				row.extend(["E"] * (mGraph.width - len(row)))
			
			#finally, do the Sugiyama algorithm: iteratively step through layer by layer, swapping
			#two elements in layer i, if they cause fewer crosses or give a shorter line length from
			#layer i-1, then work back from the other end.  Repeat until no changes are made
			
			doneA = False
			iterlimit = 10
			while (not doneA and iterlimit > 0):
				doneA = True
				iterlimit -= 1
				for dir in [1, -1]:
					start = 1
					end = mGraph.depth
					if (dir == -1):
						start = mGraph.depth - 2
						end = -1
					for x in range(start, end, dir):
						doneB = False
						while (not doneB):
							doneB = True
							for y in range(mGraph.width - 1, 0, -1):
								medA = -1.0
								medB = -1.0
								elementA = mGraph.matrix[x][y-1]
								elementB = mGraph.matrix[x][y]
								nodeA = 0
								nodeB = 0
								setA = 0
								setB = 0
								if (elementA != "E"):
									nodeA = mGraph.graph[elementA]
									setA = nodeA.connectsFrom
									if (dir == -1):
										setA = nodeA.connectsTo
									medA = self.getMedianY(mGraph, setA)
								if (elementB != "E"):
									nodeB = mGraph.graph[elementB]
									setB = nodeB.connectsFrom
									if (dir == -1):
										setB = nodeB.connectsTo
									medB = self.getMedianY(mGraph, setB)

								if (medA < 0 and medB < 0):
									continue
								if (medA > -1 and medB > -1):
									if (medA > medB):
										self.swap(mGraph, x, y-1, y)
										doneB = False
								if (medA == -1 and medB < y):
									self.swap(mGraph, x, y-1, y)
									doneB = False
								if (medB == -1 and medA >= y):
									self.swap(mGraph, x, y-1, y)
									doneB = False
							if (doneB == False):
								doneA = False
						doneB = False
						while (not doneB):
							doneB = True
							for y in range(1, mGraph.width):
								elementA = mGraph.matrix[x][y-1]
								elementB = mGraph.matrix[x][y]
								if (elementA == "E" or elementB == "E"):
									continue
								nodeA = mGraph.graph[elementA]
								nodeB = mGraph.graph[elementB]
								setA = nodeA.connectsFrom
								setB = nodeB.connectsFrom
								if (dir == -1):
									setA = nodeA.connectsTo
									setB = nodeB.connectsTo
								crosses = 0
								crossesFlipped = 0
								for a in setA:
									yA = mGraph.graph[a].y
									for b in setB:
										yB = mGraph.graph[b].y
										if (yB < yA):
											crosses += 1
										elif (yB > yA):
											crossesFlipped += 1
								if (crossesFlipped < crosses):
									self.swap(mGraph, x, y-1, y)
									doneB = False
							if (doneB == False):
								doneA = False
						
						#this is a fix for median float->int conversions throwing off the list
						if (mGraph.matrix[x][-1] == "E"):
							sum = 0.0
							num = 0.0
							for y in range(mGraph.width - 1):
								element = mGraph.matrix[x][y]
								if (element != "E"):
									node = mGraph.graph[element]
									seto = node.connectsFrom
									if (dir == -1):
										seto = node.connectsTo
									sum += y - self.getMedianY(mGraph, seto)
									num += 1
							if (num > 0 and sum / num < -0.5):
								for y in range(mGraph.width - 1, 0, -1):
									element = mGraph.matrix[x][y-1]
									mGraph.matrix[x][y] = element
									if (element != "E"):
										mGraph.graph[element].y = y
								mGraph.matrix[x][0] = "E"
		
		#CvUtil.pyPrint("4:\n" + str(self.mGraphs))
		
		#one final step: sort the graphs with the biggest one at top
		done = False
		while (done == False):
			done = True
			for i in range(1, len(self.mGraphs)):
				if (len(self.mGraphs[i-1].graph) < len(self.mGraphs[i].graph)):
					done = False
					temp = self.mGraphs[i]
					self.mGraphs[i] = self.mGraphs[i-1]
					self.mGraphs[i-1] = temp
		return
	
	################## Drawing ###################
	
	def getPosition(self, x, y, verticalOffset):
		xPos = self.MARGIN_X + x * (self.ICON_SIZE + self.SPACING_X)
		yPos = self.MARGIN_Y + y * (self.ICON_SIZE + self.SPACING_Y) + verticalOffset
		return (xPos, yPos)
	
	def draw(self):
		self.cacheArtInfo()
		screen = self.getScreen()
		
		if (not self.GRAPH_ID in self.widgets):
			screen.addScrollPanel(self.record(self.GRAPH_ID), u"", self.GRAPH_X, self.GRAPH_Y, self.GRAPH_W, self.GRAPH_H, PanelStyles.PANEL_STYLE_STANDARD)
			screen.setActivation(self.GRAPH_ID, ActivationTypes.ACTIVATE_MIMICPARENT)
		
		offset = 0
		for mGraph in self.mGraphs:
			#draw arrows first so they'll go under the buttons if there is overlap
			self.drawGraphArrows(screen, mGraph, offset)
			self.drawGraphElements(screen, mGraph, offset)
			offset = self.getPosition(0, mGraph.width, offset)[1]
	
	def drawGraphElements(self, screen, mGraph, offset):
		for x in range(mGraph.depth):
			for y in range (mGraph.width):
				element = mGraph.matrix[x][y]
				(xPos, yPos) = self.getPosition(x, y, offset)
				if element != "E" and element > -1:
					self.drawElement(screen, element, xPos, yPos)
	
	def drawGraphArrows(self, screen, mGraph, offset):
		matrix = mGraph.matrix
		for x in range(len(matrix) - 1, -1, -1):
			for y in range(len(matrix[x])):
				element = matrix[x][y]
				if element != "E":
					self.drawElementArrows(screen, mGraph, offset, element)
	
	def drawElementArrows(self, screen, mGraph, offset, element):
		toNode = mGraph.graph[element]
		for fromElement in toNode.connectsFrom:
			fromNode = mGraph.graph[fromElement]
			posFrom = self.getPosition(fromNode.x, fromNode.y, offset)
			posTo = self.getPosition(toNode.x, toNode.y, offset)
			self.drawArrow(screen, posFrom, posTo, fromElement < 0, element < 0)
	
	def drawArrow(self, screen, posFrom, posTo, dummyFrom, dummyTo):
		if (dummyFrom):
			xFrom = posFrom[0] + self.ICON_SIZE / 2
		else:
			xFrom = posFrom[0] + self.ICON_SIZE
		if (dummyTo):
			xTo = posTo[0] + self.ICON_SIZE / 2
		else:
			xTo = posTo[0] - 8
		yFrom = posFrom[1] + (self.ICON_SIZE / 2)
		yTo = posTo[1] + (self.ICON_SIZE / 2)
		
		if (yFrom == yTo):
			screen.addDDSGFCAt(self.record(self.getNextWidgetID(self.GRAPH_LINE_ID)), self.GRAPH_ID, self.LINE_STRAIT, xFrom, yFrom - 3, xTo - xFrom, 8, WidgetTypes.WIDGET_GENERAL, -1, -1, False)
		else:
			xDiff = float(xTo - xFrom)
			yDiff = float(yTo - yFrom)

			iterations = int(max(xDiff, abs(yDiff)) / 80) + 1
			if (abs(xDiff/yDiff) >= 2 or abs(xDiff/yDiff) < 0.5):
				iterations = int(max(xDiff, abs(yDiff)) / 160) + 1

			line = self.LINE_TLBR
			if (yDiff < 0):
				line = self.LINE_BLTR
			for i in range(iterations):
				xF = int((xDiff / iterations) * max(i-0.1, 0)) + xFrom
				yF = int((yDiff / iterations) * max(i-0.1, 0)) + yFrom
				xT = int((xDiff / iterations) * (i + 1)) + xFrom
				yT = int((yDiff / iterations) * (i + 1)) + yFrom
				if (yT < yF):
					temp = yT
					yT = yF
					yF = temp
				screen.addDDSGFCAt(self.record(self.getNextWidgetID(self.GRAPH_LINE_ID)), self.GRAPH_ID, line, xF, yF, xT-xF, yT-yF, WidgetTypes.WIDGET_GENERAL, -1, -1, False)
		
		if (dummyTo == False):
			screen.addDDSGFCAt(self.record(self.getNextWidgetID(self.GRAPH_ARROW_ID)), self.GRAPH_ID, self.LINE_ARROW, xTo, yTo - 6, 12, 12, WidgetTypes.WIDGET_GENERAL, -1, -1, False)
	
	####################### Supporting Methods #######################
	
	def getScreen(self):
		return self.container.getScreen()
	
	def record(self, szWidgetID):
		self.widgets.add(szWidgetID)
		return szWidgetID
	
	def clear(self):
		screen = self.getScreen()
		for szWidgetID in self.widgets:
			screen.deleteWidget(szWidgetID)
		self.widgets.clear()
	
	def getNextWidgetID(self, szPrefix):
		return szPrefix + str(len(self.widgets))
	
	def clearElements(self):
		screen = self.getScreen()
		bFound = False
		for szWidgetID in self.widgets:
			if (szWidgetID != self.GRAPH_ID):
				screen.deleteWidget(szWidgetID)
			else:
				bFound = True
		self.widgets.clear()
		if (bFound):
			self.widgets.add(self.GRAPH_ID)
	
	def isEmptyOfElements(self):
		return len(self.widgets) <= 1
	
	def isEmpty(self):
		return len(self.widgets) == 0
