## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005

from CvPythonExtensions import *
import CvUtil
import CvScreenEnums

"""
class Rect(object):
	"Rectangle Object"
	
	def __init__(self, iX, iY, iWidth, iHeight):
		self.iTop = iX
		self.iRight = iX + iWidth
		self.iBottom = iY + iHeight
		self.iLeft = iY
		self.iWidth = iWidth
		self.iHeight = iHeight
	
	def margin(self, iTop, iRight = None, iBottom = None, iLeft = None):
		if (iRight == None):
			iRight = iTop
		
		if (iBottom == None):
			iBottom = iTop
		
		if (iLeft == None):
			iLeft = iRight
		
		return Rect(self.iTop + iTop, self.iLeft + iLeft, self.iWidth - iLeft + iRight, self.iHeight - iTop + iBottom)
	
	def splitX(self, iSeparation = 0, iCount = 2):
		iWidth = int((self.iWidth - iSeparation * (iCount - 1)) / iCount)
		iStride = iWidth + iSeparation
		return [Rect(self.iX + iNum * iStride, self.iY, iWidth, self.iHeight) for iNum in range(iCount)]
	
	def splitY(self, iSeparation = 0, iCount = 2):
		iHeight = int((self.iWidth - iSeparation * (iCount - 1)) / iCount)
		iStride = iHeight + iSeparation
		return [Rect(self.iX + iNum * iStride, self.iY, iWidth, self.iHeight) for iNum in range(iCount)]
"""

class CvPediaScreen(object):
	"Civilopedia Base"
	
	def __init__(self):
		self.widgets = set()
	
	def update(self, fDelta):
		return
	
	"""
	def getSortedList( self, numInfos, getInfo ):
		' returned a list of infos sorted alphabetically '
		
		# count the items we are going to display
		iNumNonGraphical = 0
		for i in range(numInfos):
			if (not getInfo(i).isGraphicalOnly()):
				iNumNonGraphical += 1
				
		infoList = [(0,0,0)] * iNumNonGraphical
		j = 0
		for i in range( numInfos ):
			if (not getInfo(i).isGraphicalOnly()):
				infoList[j] = (getInfo(i).getDescription(), i, getInfo(i).getButton())
				j += 1
				
		infoList.sort()
		
		return infoList
	"""
	
	"""
	def getViewItems(self, numInfos, getInfo, predicate, iData):
		items = []
		
		for i in range(numInfos):
			info = getInfo(i)
			if (predicate(info, iData)):
				items.append((info.getDescription(), i, info.getButton()))
		
		items.sort()
		
		return items
	"""
	
	def getInfos(self, numInfos, getInfo, filter = None, iFilterData = -1):
		items = []
		
		for eItem in range(numInfos):
			info = getInfo(eItem)
			if (not info.isGraphicalOnly()):
				if (filter == None or filter(info, iFilterData)):
					items.append((info.getDescription(), eItem, info.getButton()))
		
		return items
	
	def getClassifiedInfos(self, numClassInfos, getClassInfo, getInfo, predicate, filter = None, iFilterData = -1):
		items = []
		
		for eClass in range(numClassInfos):
			classInfo = getClassInfo(eClass)
			if (not classInfo.isGraphicalOnly()):
				eItem = predicate(classInfo)
				if (eItem > -1):
					info = getInfo(eItem)
					if (not info.isGraphicalOnly()):
						if (filter == None or filter(info, iFilterData)):
							items.append((info.getDescription(), eItem, info.getButton()))
		
		items.sort()
		
		return items
	
	def getScreen(self):
		return CyGInterfaceScreen("PediaScreen", CvScreenEnums.PEDIA_SCREEN)

	def record(self, szWidgetID):
		self.widgets.add(szWidgetID)
		return szWidgetID

	def clear(self):
		screen = self.getScreen()
		for szWidgetID in self.widgets:
			screen.deleteWidget(szWidgetID)
		self.widgets.clear()
	
	def isEmpty(self):
		return len(self.widgets) == 0
