## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
import CvUtil
from CvPythonExtensions import *

from CvPediaSection import CvPediaSection
from CvPediaView import CvPediaView
from CvPediaViewBuildings import CvPediaViewBuildings
from CvPediaViewGreatWorks import CvPediaViewGreatWorks
from CvPediaViewBuildingUpgradeChart import CvPediaViewBuildingUpgradeChart
from CvPediaBuilding import CvPediaBuilding
#from CvPediaBuildingClass import CvPediaBuildingClass
from CvPediaProject import CvPediaProject
from CvPediaBuildingUpgradeChart import CvPediaBuildingUpgradeChart
import Revolutions

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

class CvPediaSectionBuildings(CvPediaSection):
	"Civilopedia Section for Buildings"

	def __init__(self, main):
		super(CvPediaSectionBuildings, self).__init__(main, CivilopediaSectionTypes.CIVILOPEDIA_SECTION_BUILDINGS, "TXT_KEY_PEDIA_SECTION_BUILDINGS", "Buildings")
		
		self.views = [
			CvPediaViewBuildings(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_BUILDINGS_ALL, "TXT_KEY_PEDIA_VIEW_BUILDINGS_ALL", self.isCommonBuilding),
			CvPediaViewBuildingUpgradeChart(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_BUILDINGS_UPGRADE_CHART, "TXT_KEY_PEDIA_VIEW_BUILDINGS_UPGRADE_CHART"),
			#(u"All", CivilopediaViewTypes.CIVILOPEDIA_VIEW_BUILDINGS_ALL, -1, CivilopediaPageTypes.CIVILOPEDIA_PAGE_BUILDING, WidgetTypes.WIDGET_PEDIA_JUMP_TO_BUILDING, self.getViewItems(gc.getNumBuildingInfos(), gc.getBuildingInfo), -1),
			#(CivilopediaViewTypes.CIVILOPEDIA_VIEW_BUILDINGS_CIVILIZATION, -1, u"Buildings By Civilization"),
			#(CivilopediaViewTypes.CIVILOPEDIA_VIEW_BUILDINGS_COMPARISON, -1, u"Comparison"),
			#CvPediaView(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_BUILDINGS_CLASSES, u"Classes", CivilopediaPageTypes.CIVILOPEDIA_PAGE_BUILDING_CLASS, WidgetTypes.WIDGET_PEDIA_JUMP_TO_BUILDING_CLASS, self.getInfos(gc.getNumBuildingClassInfos(), gc.getBuildingClassInfo)),
			CvPediaViewBuildings(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_BUILDINGS_NATIONAL_WONDERS, "TXT_KEY_PEDIA_VIEW_BUILDINGS_NATIONAL_WONDERS", self.isNationalWonder),
			CvPediaViewBuildings(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_BUILDINGS_GREAT_WONDERS, "TXT_KEY_PEDIA_VIEW_BUILDINGS_GREAT_WONDERS", self.isWorldWonder),
			CvPediaViewBuildings(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_BUILDINGS_RELIGIOUS, "TXT_KEY_PEDIA_VIEW_BUILDINGS_SEPARATISM", self.isSeparatism),
			CvPediaView(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_BUILDINGS_PROJECTS, "TXT_KEY_PEDIA_VIEW_BUILDINGS_PROJECTS", CivilopediaPageTypes.CIVILOPEDIA_PAGE_PROJECT, WidgetTypes.WIDGET_PEDIA_JUMP_TO_PROJECT, lambda: self.getInfos(gc.getNumProjectInfos(), gc.getProjectInfo)),
			#CvPediaView(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_BUILDINGS_RELIGIOUS, "TXT_KEY_PEDIA_VIEW_BUILDINGS_RELIGIOUS")
		]
		
		for eBuilderUnitClass in self.getGreatWorkUnitClasses():
			self.views.append(CvPediaViewGreatWorks(self, eBuilderUnitClass))
		
		self.pages = {
			CivilopediaPageTypes.CIVILOPEDIA_PAGE_BUILDING : CvPediaBuilding(self),
			#CivilopediaPageTypes.CIVILOPEDIA_PAGE_BUILDING_CLASS : CvPediaBuildingClass(self),
			CivilopediaPageTypes.CIVILOPEDIA_PAGE_PROJECT : CvPediaProject(self),
			CivilopediaPageTypes.CIVILOPEDIA_PAGE_BUILDING_UPGRADE_CHART : CvPediaBuildingUpgradeChart(self)
		}
		
		self.setDefaults()
	
	def isCommonBuilding(self, info, iData):
		return not isWorldWonderClass(info.getBuildingClassType()) and not isNationalWonderClass(info.getBuildingClassType()) and not info.isGreatWork() and not info.isMilitaryDoctrine() and not info.isMilitaryTradition() and not self.isSeparatism(info,iData)
	
	def isGreatWork(self, info, iData):
		return info.isGreatWork()
	
	def isWorldWonder(self, info, iData):
		return isWorldWonderClass(info.getBuildingClassType()) and not info.isMilitaryDoctrine() and not info.isGreatWork()
		
	def isNationalWonder(self, info, iData):
		return isNationalWonderClass(info.getBuildingClassType()) and not info.isMilitaryTradition() and not info.isGreatWork()

	def isSeparatism(self, info, iData):
		return (info.getSeparatism() != 0) and (info.getProductionCost() == -1)
		
	def getGreatWorkUnitClasses(self):
		builders = set()
		for (szItemTitle, eBuilding, szButton) in self.getInfos(gc.getNumBuildingInfos(), gc.getBuildingInfo, self.isGreatWork):
			kBuilding = gc.getBuildingInfo(eBuilding)
			for iRel in range(kBuilding.getNumBuilderUnitClasses()):
				eUnitClass = kBuilding.getBuilderUnitClasses(iRel)
				builders.add(eUnitClass)
		return sorted(builders)