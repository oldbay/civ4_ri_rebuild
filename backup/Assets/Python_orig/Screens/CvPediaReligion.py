## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
from CvPythonExtensions import *
import CvUtil
import CvPediaPage
import ScreenInput
import CvScreenEnums
import string

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

class CvPediaReligion(CvPediaPage.CvPediaPage):
	"Civilopedia Screen for Religions"

	def __init__(self, section):
		super(CvPediaReligion, self).__init__(section, CivilopediaPageTypes.CIVILOPEDIA_PAGE_RELIGION, WidgetTypes.WIDGET_PEDIA_JUMP_TO_RELIGION, "Religion")
		
		self.eReligion = -1
		
		self.REQUIRES_PANEL_ID = self.szWidget + "RequiresPanel"
		self.UNIT_PANEL_ID = self.szWidget + "UnitsPanel"
		self.BUILDING_PANEL_ID = self.szWidget + "BuildingsPanel"
	
	def getData(self):
		return (self.eReligion, -1)
	
	def setData(self, eReligion, iPageData2):
		if (self.eReligion != eReligion):
			self.clear()
			self.eReligion = eReligion
	
	def getInfo(self):
		return gc.getReligionInfo(self.eReligion)
	
	def calculateLayout(self):
		self.calculateTitleLayout()
		self.calculateSynopsisLayout()
		self.calculateTwoColumnLayout(self.SYNOPSIS_PANEL_W, 0)
		
		self.REQUIRES_PANEL_X = self.COLUMN1_X
		self.REQUIRES_PANEL_Y = self.SYNOPSIS_PANEL_Y + self.SYNOPSIS_PANEL_H + self.main.COMMON_MARGIN_H
		self.REQUIRES_PANEL_W = self.COLUMN1_W
		self.REQUIRES_PANEL_H = self.main.COMMON_ICON_PANEL_SIZE
		
		self.EFFECTS_PANEL_X = self.COLUMN2_X
		self.EFFECTS_PANEL_Y = self.PAGE_CONTENT_Y
		self.EFFECTS_PANEL_W = self.COLUMN2_W
		self.EFFECTS_PANEL_H = self.SYNOPSIS_PANEL_H + self.REQUIRES_PANEL_H + self.main.COMMON_MARGIN_H
		
		self.calculateTwoColumnLayout(0, 50, 0, 50)
		
		self.BUILDING_PANEL_X = self.COLUMN1_X
		self.BUILDING_PANEL_Y = self.REQUIRES_PANEL_Y + self.REQUIRES_PANEL_H + self.main.COMMON_MARGIN_H
		self.BUILDING_PANEL_W = self.COLUMN1_W
		self.BUILDING_PANEL_H = self.main.COMMON_ICON_PANEL_SIZE
		
		self.UNIT_PANEL_X = self.COLUMN2_X
		self.UNIT_PANEL_Y = self.BUILDING_PANEL_Y
		self.UNIT_PANEL_W = self.COLUMN2_W
		self.UNIT_PANEL_H = self.main.COMMON_ICON_PANEL_SIZE
		
		self.CIVILOPEDIA_PANEL_X = self.main.PAGE_AREA_X
		self.CIVILOPEDIA_PANEL_Y = self.BUILDING_PANEL_Y + self.BUILDING_PANEL_H + self.main.COMMON_MARGIN_H
		self.CIVILOPEDIA_PANEL_W = self.main.PAGE_AREA_W
		self.CIVILOPEDIA_PANEL_H = self.main.PAGE_AREA_Y + self.main.PAGE_AREA_H - self.CIVILOPEDIA_PANEL_Y
	
	def draw(self):
		if (not self.isEmpty()):
			return
		
		screen = self.getScreen()
		info = self.getInfo()
		ePlayer = gc.getGame().getActivePlayer()
		
		self.drawTitle(screen, info, ePlayer)
		self.drawSynopsis(screen, info, ePlayer)
		self.drawRequires(screen, info)
		self.drawEffects(screen, CyGameTextMgr().parseReligionInfo(self.eReligion, True)[1:])
		self.drawUnits(screen, info, ePlayer)
		self.drawBuildings(screen, info, ePlayer)
		self.drawCivilopedia(screen, info, "TXT_KEY_PEDIA_BACKGROUND")
	
	def drawRequires(self, screen, info):
		screen.addPanel(self.record(self.REQUIRES_PANEL_ID), localText.getText("TXT_KEY_PEDIA_REQUIRES", ()), "", False, True, self.REQUIRES_PANEL_X, self.REQUIRES_PANEL_Y, self.REQUIRES_PANEL_W, self.REQUIRES_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		screen.attachLabel(self.REQUIRES_PANEL_ID, "", "  ")
		
		eTech = info.getTechPrereq()
		if (eTech > -1):
			screen.attachImageButton(self.REQUIRES_PANEL_ID, "", gc.getTechInfo(eTech).getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_TECH, eTech, 1, False)

	def drawUnits(self, screen, info, ePlayer):
		screen.addPanel(self.record(self.UNIT_PANEL_ID), localText.getText("TXT_KEY_PEDIA_UNITS_ENABLED", ()), "", false, true, self.UNIT_PANEL_X, self.UNIT_PANEL_Y, self.UNIT_PANEL_W, self.UNIT_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		
		screen.attachLabel(self.UNIT_PANEL_ID, "", "  ")
		
		if (ePlayer != -1):
			pCivilization = gc.getCivilizationInfo(gc.getPlayer(ePlayer).getCivilizationType())
		else:
			pCivilization = None
		
		for iRel in range(info.getNumRelatedUnitClasses()):
			eLoopUnitClass = info.getRelatedUnitClasses(iRel)
			
			if (pCivilization != None):
				eLoopUnit = pCivilization.getCivilizationUnits(eLoopUnitClass)
			else:
				eLoopUnit = gc.getUnitClassInfo(eLoopUnitClass).getRepresentativeUnit()
			
			if (eLoopUnit != -1):
				pLoopUnit = gc.getUnitInfo(eLoopUnit)
				if (pLoopUnit.getPrereqReligion() == self.eReligion):
					if ePlayer != -1:
						szButton = gc.getPlayer(ePlayer).getUnitButton(eLoopUnit)
					else:
						szButton = pLoopUnit.getButton()
					screen.attachImageButton(self.UNIT_PANEL_ID, "", szButton, self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_UNIT, eLoopUnit, 1, False)

	def drawBuildings(self, screen, info, ePlayer):
		screen.addPanel(self.record(self.BUILDING_PANEL_ID), localText.getText("TXT_KEY_PEDIA_BUILDINGS_ENABLED", ()), "", false, true, self.BUILDING_PANEL_X, self.BUILDING_PANEL_Y, self.BUILDING_PANEL_W, self.BUILDING_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		
		screen.attachLabel(self.BUILDING_PANEL_ID, "", "  ")
		
		if (ePlayer != -1):
			pCivilization = gc.getCivilizationInfo(gc.getPlayer(ePlayer).getCivilizationType())
		else:
			pCivilization = None
		
		for iRel in range(info.getNumRelatedBuildingClasses()):
			eLoopBuildingClass = info.getRelatedBuildingClasses(iRel)
			
			if (pCivilization != None):
				eLoopBuilding = pCivilization.getCivilizationBuildings(eLoopBuildingClass)
			else:
				eLoopBuilding = gc.getBuildingClassInfo(eLoopBuildingClass).getRepresentativeBuilding()
			
			if (eLoopBuilding != -1):
				pLoopBuilding = gc.getBuildingInfo(eLoopBuilding)
				if (pLoopBuilding.getPrereqReligion() == self.eReligion) or (pLoopBuilding.getHolyCity() == self.eReligion):
					screen.attachImageButton(self.BUILDING_PANEL_ID, "", pLoopBuilding.getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_BUILDING, eLoopBuilding, 1, False)
	
	"""
	def placeLinks(self, bRedraw):

		screen = self.top.getScreen()
        
		if bRedraw:
			screen.clearListBoxGFC(self.top.LIST_ID)
		
		# sort Improvements alphabetically
		listSorted=[(0,0)]*gc.getNumReligionInfos()
		for j in range(gc.getNumReligionInfos()):
			listSorted[j] = (gc.getReligionInfo(j).getDescription(), j)
		listSorted.sort()	
			
		iSelected = 0
		i = 0
		for iI in range(gc.getNumReligionInfos()):
			if (not gc.getReligionInfo(listSorted[iI][1]).isGraphicalOnly()):
				if bRedraw:
					screen.appendListBoxString( self.top.LIST_ID, listSorted[iI][0], WidgetTypes.WIDGET_PEDIA_JUMP_TO_RELIGION, listSorted[iI][1], 0, CvUtil.FONT_LEFT_JUSTIFY )
				if listSorted[iI][1] == self.eReligion:
					iSelected = i
				i += 1

		screen.setSelectedListBoxStringGFC(self.top.LIST_ID, iSelected)
		
	# Will handle the input for this screen...
	def handleInput (self, inputClass):
		return 0
	"""


