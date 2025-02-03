## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005

from CvPythonExtensions import *
from CvPediaViewBuildings import CvPediaViewBuildings

# globals
gc = CyGlobalContext()
localText = CyTranslator()

class CvPediaViewGreatWorks(CvPediaViewBuildings):
	"Civilopedia Great Works View"

	def __init__(self, section, eBuilderUnitClass):
		super(CvPediaViewGreatWorks, self).__init__(section, CivilopediaViewTypes.CIVILOPEDIA_VIEW_BUILDINGS_GREAT_WORKS, "", self.filterGreatWork, eBuilderUnitClass)
		
		self.eBuilderUnitClass = eBuilderUnitClass
	
	def getTitle(self):
		eRepresentativeBuilder = gc.getUnitClassInfo(self.eBuilderUnitClass).getRepresentativeUnit()
		kRepresentativeBuilder = gc.getUnitInfo(eRepresentativeBuilder)
		return u"%s (%s)" %(localText.getText("TXT_KEY_PEDIA_VIEW_BUILDINGS_GREAT_WORKS", ()), kRepresentativeBuilder.getDescription().replace("Great ", ""))
	
	def matchesView(self, eView, eBuilderUnitClass, iViewData2):
		return self.eView == eView and self.eBuilderUnitClass == eBuilderUnitClass
	
	def getData(self):
		return (self.eBuilderUnitClass, -1)
	
	def filterGreatWork(self, info, eBuilderUnitClass):
		if not info.isGreatWork():
			return False
		
		for iMem in range(info.getNumBuilderUnitClasses()):
			if info.getBuilderUnitClasses(iMem) == eBuilderUnitClass:
				return True
		
		return False
	
	def makeGreatWorkFilter(self, eBuilderUnitClass):
		eLocal = eBuilderUnitClass
		return lambda info, iData: info.isGreatWork() and self.isBuilderClassForBuilding(eLocal, info)
	
	def isBuilderClassForBuilding(self, eBuilderUnitClass, kBuilding):
		for iMem in range(kBuilding.getNumBuilderUnitClasses()):
			if kBuilding.getBuilderUnitClasses(iMem) == eBuilderUnitClass:
				return True
		return False