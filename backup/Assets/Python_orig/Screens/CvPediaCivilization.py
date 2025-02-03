## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
from CvPythonExtensions import *
import CvUtil
import CvPediaPage
import ScreenInput

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

class CvPediaCivilization(CvPediaPage.CvPediaPage):
	"Civilopedia Screen for Civilizations"

	def __init__(self, section):
		super(CvPediaCivilization, self).__init__(section, CivilopediaPageTypes.CIVILOPEDIA_PAGE_CIVILIZATION, WidgetTypes.WIDGET_PEDIA_JUMP_TO_CIV, "Civilization")
		
		self.eCivilization = -1
		
		self.LEADERS_PANEL_ID = self.szWidget + "LeadersPanel"
		self.UNIQUE_IMPROVEMENTS_PANEL_ID = self.szWidget + "UniqueImprovementsPanel"
		self.UNIQUE_UNITS_PANEL_ID = self.szWidget + "UniqueUnitsPanel"
		self.UNIQUE_BUILDINGS_PANEL_ID = self.szWidget + "UniqueBuildingsPanel"
		self.DISTINCTIVE_UNITS_PANEL_ID = self.szWidget + "DistinctiveUnitsPanel"
		self.DISTINCTIVE_UNITS_LIST_ID = self.szWidget + "DistinctiveUnitsList"
		self.DISTINCTIVE_BUILDINGS_PANEL_ID = self.szWidget + "DistinctiveBuildingsPanel"
		self.DISTINCTIVE_BUILDINGS_LIST_ID = self.szWidget + "DistinctiveBuildingsList"
	
	def getData(self):
		return (self.eCivilization, -1)
	
	def setData(self, eCivilization, iPageData2):
		if (self.eCivilization != eCivilization):
			self.clear()
			self.eCivilization = eCivilization
	
	def getInfo(self):
		return gc.getCivilizationInfo(self.eCivilization)
	
	def getButton(self, info, eActivePlayer):
		return ArtFileMgr.getCivilizationArtInfo(info.getArtDefineTag()).getButton()
	
	def calculateLayout(self):
		self.calculateTitleLayout()
		self.calculateSynopsisLayout(-1, self.main.COMMON_ICON_PANEL_SIZE * 2 + self.main.COMMON_MARGIN_H)
		self.calculateThreeColumnLayout(self.SYNOPSIS_PANEL_W, 0, 0, 100, 180, 0)
		
		self.LEADERS_PANEL_X = self.COLUMN2_X
		self.LEADERS_PANEL_Y = self.PAGE_CONTENT_Y
		self.LEADERS_PANEL_W = self.COLUMN2_W
		self.LEADERS_PANEL_H = self.main.COMMON_ICON_PANEL_SIZE
		
		self.UNIQUE_IMPROVEMENTS_PANEL_X = self.COLUMN3_X
		self.UNIQUE_IMPROVEMENTS_PANEL_Y = self.PAGE_CONTENT_Y
		self.UNIQUE_IMPROVEMENTS_PANEL_W = self.COLUMN3_W
		self.UNIQUE_IMPROVEMENTS_PANEL_H = self.main.COMMON_ICON_PANEL_SIZE
		
		self.calculateThreeColumnLayout(self.SYNOPSIS_PANEL_W, 0)
		
		self.UNIQUE_UNITS_PANEL_X = self.COLUMN2_X
		self.UNIQUE_UNITS_PANEL_Y = self.LEADERS_PANEL_Y + self.LEADERS_PANEL_H + self.main.COMMON_MARGIN_H
		self.UNIQUE_UNITS_PANEL_W = self.COLUMN2_W
		self.UNIQUE_UNITS_PANEL_H = self.main.COMMON_ICON_PANEL_SIZE
		
		self.UNIQUE_BUILDINGS_PANEL_X = self.COLUMN3_X
		self.UNIQUE_BUILDINGS_PANEL_Y = self.UNIQUE_UNITS_PANEL_Y
		self.UNIQUE_BUILDINGS_PANEL_W = self.COLUMN3_W
		self.UNIQUE_BUILDINGS_PANEL_H = self.main.COMMON_ICON_PANEL_SIZE
		
		self.calculateTwoColumnLayout(0, 40, 0, 60)
		
		self.CIVILOPEDIA_PANEL_X = self.COLUMN1_X
		self.CIVILOPEDIA_PANEL_Y = self.SYNOPSIS_PANEL_Y + self.SYNOPSIS_PANEL_H + self.main.COMMON_MARGIN_H
		self.CIVILOPEDIA_PANEL_W = self.COLUMN1_W
		self.CIVILOPEDIA_PANEL_H = self.main.PAGE_AREA_Y + self.main.PAGE_AREA_H - self.CIVILOPEDIA_PANEL_Y
		
		if (self.main.SCREEN_W >= 1920 and self.main.SCREEN_H >= 1200):
			self.DISTINCTIVE_ICON_SIZE = 64
		elif (self.main.SCREEN_W >= 1600 and self.main.SCREEN_H >= 960):
			self.DISTINCTIVE_ICON_SIZE = 46
		elif (self.main.SCREEN_W >= 1280 and self.main.SCREEN_H >= 1024):
			self.DISTINCTIVE_ICON_SIZE = 46
		else:
			self.DISTINCTIVE_ICON_SIZE = 32
		
		self.DISTINCTIVE_UNITS_PANEL_X = self.COLUMN2_X
		self.DISTINCTIVE_UNITS_PANEL_Y = self.SYNOPSIS_PANEL_Y + self.SYNOPSIS_PANEL_H + self.main.COMMON_MARGIN_H
		self.DISTINCTIVE_UNITS_PANEL_W = self.COLUMN2_W
		self.DISTINCTIVE_UNITS_PANEL_H = self.main.PAGE_AREA_Y + self.main.PAGE_AREA_H - self.DISTINCTIVE_UNITS_PANEL_Y - self.main.COMMON_MARGIN_H - (self.DISTINCTIVE_ICON_SIZE * 2) - 50
		
		self.DISTINCTIVE_BUILDINGS_PANEL_X = self.COLUMN2_X
		self.DISTINCTIVE_BUILDINGS_PANEL_Y = self.DISTINCTIVE_UNITS_PANEL_Y + self.DISTINCTIVE_UNITS_PANEL_H + self.main.COMMON_MARGIN_H
		self.DISTINCTIVE_BUILDINGS_PANEL_W = self.COLUMN2_W
		self.DISTINCTIVE_BUILDINGS_PANEL_H = self.main.PAGE_AREA_Y + self.main.PAGE_AREA_H - self.DISTINCTIVE_BUILDINGS_PANEL_Y
	
	def draw(self):
		if (not self.isEmpty()):
			return
		
		screen = self.getScreen()
		info = self.getInfo()
		ePlayer = gc.getGame().getActivePlayer()
		
		self.drawTitle(screen, info, ePlayer)
		self.drawSynopsis(screen, info, ePlayer)
		self.drawLeaders(screen, info)
		self.drawUniqueImprovements(screen, info)
		self.drawUniqueUnits(screen, info, ePlayer)
		self.drawUniqueBuildings(screen, info)
		self.drawDistinctiveUnits(screen, info, ePlayer)
		self.drawDistinctiveBuildings(screen, info)
		self.drawCivilopedia(screen, info, "TXT_KEY_PEDIA_BACKGROUND")
	
	def drawLeaders(self, screen, info):
		screen.addPanel(self.record(self.LEADERS_PANEL_ID), localText.getText("TXT_KEY_CONCEPT_LEADERS", ()), "", False, True, self.LEADERS_PANEL_X, self.LEADERS_PANEL_Y, self.LEADERS_PANEL_W, self.LEADERS_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		screen.attachLabel(self.LEADERS_PANEL_ID, "", "  ")
		
		for eLeader in range(gc.getNumLeaderHeadInfos()):
			if info.isLeaders(eLeader):
				screen.attachImageButton(self.LEADERS_PANEL_ID, "", gc.getLeaderHeadInfo(eLeader).getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_LEADER, eLeader, self.eCivilization, False)
	
	def drawUniqueImprovements(self, screen, info):
		screen.addPanel(self.record(self.UNIQUE_IMPROVEMENTS_PANEL_ID), localText.getText("TXT_KEY_UNIQUE_IMPROVEMENTS", ()), "", False, True, self.UNIQUE_IMPROVEMENTS_PANEL_X, self.UNIQUE_IMPROVEMENTS_PANEL_Y, self.UNIQUE_IMPROVEMENTS_PANEL_W, self.UNIQUE_IMPROVEMENTS_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		screen.attachLabel(self.UNIQUE_IMPROVEMENTS_PANEL_ID, "", "  ")
		
		for eLoopImprovement in range(gc.getNumImprovementInfos()):
			pLoopImprovement = gc.getImprovementInfo(eLoopImprovement)
			if (pLoopImprovement.isUniqueImprovement()):
				if (info.canProduceImprovement(eLoopImprovement)):
					screen.attachImageButton(self.UNIQUE_IMPROVEMENTS_PANEL_ID, "", pLoopImprovement.getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_IMPROVEMENT, eLoopImprovement, 1, False)
	
	def drawUniqueUnits(self, screen, info, ePlayer):
		screen.addPanel(self.record(self.UNIQUE_UNITS_PANEL_ID), localText.getText("TXT_KEY_PEDIA_UNIQUE_UNITS", ()), "", False, True, self.UNIQUE_UNITS_PANEL_X, self.UNIQUE_UNITS_PANEL_Y, self.UNIQUE_UNITS_PANEL_W, self.UNIQUE_UNITS_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		screen.attachLabel(self.UNIQUE_UNITS_PANEL_ID, "", "  ")
		
		for eLoopUnitClass in range(gc.getNumUnitClassInfos()):
			eLoopUnit = info.getCivilizationUnits(eLoopUnitClass)
			if (eLoopUnit > -1):
				pLoopUnit = gc.getUnitInfo(eLoopUnit)
				if (pLoopUnit.isUniqueUnit()):
					if ePlayer != -1:
						szButton = gc.getPlayer(ePlayer).getUnitButton(eLoopUnit)
					else:
						szButton = pLoopUnit.getButton()
					screen.attachImageButton(self.UNIQUE_UNITS_PANEL_ID, "", szButton, self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_UNIT, eLoopUnit, 1, False)
	
	def drawDistinctiveUnits(self, screen, info, ePlayer):
		screen.addPanel(self.record(self.DISTINCTIVE_UNITS_PANEL_ID), localText.getText("TXT_KEY_PEDIA_DISTINCTIVE_UNITS", ()), "", False, True, self.DISTINCTIVE_UNITS_PANEL_X, self.DISTINCTIVE_UNITS_PANEL_Y, self.DISTINCTIVE_UNITS_PANEL_W, self.DISTINCTIVE_UNITS_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		#screen.attachLabel(self.DISTINCTIVE_UNITS_PANEL_ID, "", "  ")
		screen.addMultiListControlGFC(self.record(self.DISTINCTIVE_UNITS_LIST_ID), "", self.DISTINCTIVE_UNITS_PANEL_X+15, self.DISTINCTIVE_UNITS_PANEL_Y+40, self.DISTINCTIVE_UNITS_PANEL_W-20, self.DISTINCTIVE_UNITS_PANEL_H-40, 1, self.DISTINCTIVE_ICON_SIZE, self.DISTINCTIVE_ICON_SIZE, TableStyles.TABLE_STYLE_STANDARD)
		
		for eLoopUnitClass in range(gc.getNumUnitClassInfos()):
			eLoopUnit = info.getCivilizationUnits(eLoopUnitClass)
			if (eLoopUnit > -1):
				pLoopUnit = gc.getUnitInfo(eLoopUnit)
				if (not pLoopUnit.isUniqueUnit()):
					eDefaultUnit = gc.getUnitClassInfo(eLoopUnitClass).getDefaultUnitIndex()
					if (eDefaultUnit > -1 and eLoopUnit != eDefaultUnit):
						if ePlayer != -1:
							szButton = gc.getPlayer(ePlayer).getUnitButton(eLoopUnit)
						else:
							szButton = pLoopUnit.getButton()
						#screen.attachImageButton(self.DISTINCTIVE_UNITS_PANEL_ID, "", szButton, self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_UNIT, eLoopUnit, 1, False)
						screen.appendMultiListButton(self.DISTINCTIVE_UNITS_LIST_ID, szButton, 0, WidgetTypes.WIDGET_PEDIA_JUMP_TO_UNIT, eLoopUnit, -1, False)
	
	def drawUniqueBuildings(self, screen, info):
		screen.addPanel(self.record(self.UNIQUE_BUILDINGS_PANEL_ID), localText.getText("TXT_KEY_PEDIA_UNIQUE_BUILDINGS", ()), "", False, True, self.UNIQUE_BUILDINGS_PANEL_X, self.UNIQUE_BUILDINGS_PANEL_Y, self.UNIQUE_BUILDINGS_PANEL_W, self.UNIQUE_BUILDINGS_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		screen.attachLabel(self.UNIQUE_BUILDINGS_PANEL_ID, "", "  ")
		
		for eLoopBuildingClass in range(gc.getNumBuildingClassInfos()):
			eLoopBuilding = info.getCivilizationBuildings(eLoopBuildingClass)
			if (eLoopBuilding > -1):
				pLoopBuilding = gc.getBuildingInfo(eLoopBuilding)
				if (pLoopBuilding.isUniqueBuilding()):
					screen.attachImageButton(self.UNIQUE_BUILDINGS_PANEL_ID, "", pLoopBuilding.getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_BUILDING, eLoopBuilding, 1, False)
	
	def drawDistinctiveBuildings(self, screen, info):
		screen.addPanel(self.record(self.DISTINCTIVE_BUILDINGS_PANEL_ID), localText.getText("TXT_KEY_PEDIA_DISTINCTIVE_BUILDINGS", ()), "", True, True, self.DISTINCTIVE_BUILDINGS_PANEL_X, self.DISTINCTIVE_BUILDINGS_PANEL_Y, self.DISTINCTIVE_BUILDINGS_PANEL_W, self.DISTINCTIVE_BUILDINGS_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		
		screen.addMultiListControlGFC(self.record(self.DISTINCTIVE_BUILDINGS_LIST_ID), "", self.DISTINCTIVE_BUILDINGS_PANEL_X+15, self.DISTINCTIVE_BUILDINGS_PANEL_Y+40, self.DISTINCTIVE_BUILDINGS_PANEL_W-20, self.DISTINCTIVE_BUILDINGS_PANEL_H-40, 1, self.DISTINCTIVE_ICON_SIZE, self.DISTINCTIVE_ICON_SIZE, TableStyles.TABLE_STYLE_STANDARD)
		
		#screen.attachLabel(self.DISTINCTIVE_BUILDINGS_PANEL_ID, "", "  ")
		
		for eLoopBuildingClass in range(gc.getNumBuildingClassInfos()):
			eLoopBuilding = info.getCivilizationBuildings(eLoopBuildingClass)
			if (eLoopBuilding > -1):
				pLoopBuilding = gc.getBuildingInfo(eLoopBuilding)
				if (not pLoopBuilding.isUniqueBuilding()):
					eDefaultBuilding = gc.getBuildingClassInfo(eLoopBuildingClass).getDefaultBuildingIndex()
					if (eDefaultBuilding > -1 and eLoopBuilding != eDefaultBuilding):
						#screen.attachImageButton(self.DISTINCTIVE_BUILDINGS_PANEL_ID, "", gc.getBuildingInfo(eLoopBuilding).getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_BUILDING, eLoopBuilding, 1, False)
						screen.appendMultiListButton(self.DISTINCTIVE_BUILDINGS_LIST_ID, gc.getBuildingInfo(eLoopBuilding).getButton(), 0, WidgetTypes.WIDGET_PEDIA_JUMP_TO_BUILDING, eLoopBuilding, -1, False)
