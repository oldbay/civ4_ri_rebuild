## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
from CvPythonExtensions import *
import CvUtil
import CvPediaPage
## Revolutions ##
import Revolutions
## Revolutions ##

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

class CvPediaCivic(CvPediaPage.CvPediaPage):
	"Civilopedia Screen for Civics"
	
	def __init__(self, section):
		super(CvPediaCivic, self).__init__(section, CivilopediaPageTypes.CIVILOPEDIA_PAGE_CIVIC, WidgetTypes.WIDGET_PEDIA_JUMP_TO_CIVIC, "Civic")
		
		self.eCivic = -1
		
		self.REQUIRES_PANEL_ID = self.szWidget + "RequiresPanel"
		self.UNIT_PANEL_ID = self.szWidget + "UnitsPanel"
		self.BUILDING_PANEL_ID = self.szWidget + "BuildingsPanel"
	
	def getData(self):
		return (self.eCivic, -1)
	
	def setData(self, eCivic, iPageData2):
		if (self.eCivic != eCivic):
			self.clear()
			self.eCivic = eCivic
	
	def getInfo(self):
		return gc.getCivicInfo(self.eCivic)
	
	def calculateLayout(self):
		self.calculateTitleLayout()
		self.calculateTwoColumnLayout(0, 50, 0, 50)
		self.calculateSynopsisLayout(self.COLUMN1_W)
		
		self.REQUIRES_PANEL_X = self.COLUMN1_X
		self.REQUIRES_PANEL_Y = self.SYNOPSIS_PANEL_Y + self.SYNOPSIS_PANEL_H + self.main.COMMON_MARGIN_H
		self.REQUIRES_PANEL_W = self.COLUMN1_W
		self.REQUIRES_PANEL_H = self.main.COMMON_ICON_PANEL_SIZE
		
		self.EFFECTS_PANEL_X = self.COLUMN2_X
		self.EFFECTS_PANEL_Y = self.PAGE_CONTENT_Y
		self.EFFECTS_PANEL_W = self.COLUMN2_W
		self.EFFECTS_PANEL_H = self.SYNOPSIS_PANEL_H + self.REQUIRES_PANEL_H + self.main.COMMON_MARGIN_H
		
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
		self.drawEffects(screen, CyGameTextMgr().parseCivicInfo(self.eCivic, True, False, True)[1:])
		self.drawUnits(screen, info, ePlayer)
		self.drawBuildings(screen, info, ePlayer)
		self.drawCivilopedia(screen, info, "TXT_KEY_PEDIA_BACKGROUND")
	
	def drawStats(self, screen, info, ePlayer):
		screen.addListBoxGFC(self.record(self.STATS_ID), "", self.STATS_X, self.STATS_Y, self.STATS_W, self.STATS_H, TableStyles.TABLE_STYLE_EMPTY)
		screen.enableSelect(self.STATS_ID, False)
		
		# Civic Category
		eCivicOptionType = info.getCivicOptionType()
		if (eCivicOptionType != -1):
			screen.appendListBoxString(self.STATS_ID, u"<font=3>" + gc.getCivicOptionInfo(eCivicOptionType).getDescription().upper() + u"</font>", WidgetTypes.WIDGET_GENERAL, 0, 0, CvUtil.FONT_LEFT_JUSTIFY)
#			screen.setText(self.top.getNextWidgetName(), "Background", gc.getCivicOptionInfo(eCivicOptionType).getDescription().upper(), CvUtil.FONT_LEFT_JUSTIFY, self.X_STATS_PANE, self.Y_STATS_PANE-35, 0, FontTypes.GAME_FONT, WidgetTypes.WIDGET_GENERAL, -1, -1)
		
		# Upkeep
		pUpkeepInfo = gc.getUpkeepInfo(info.getUpkeep())
		if (pUpkeepInfo):
			screen.appendListBoxString(self.STATS_ID, u"<font=3>" + pUpkeepInfo.getDescription().upper() + u"</font>", WidgetTypes.WIDGET_GENERAL, 0, 0, CvUtil.FONT_LEFT_JUSTIFY)
#			screen.setText(self.top.getNextWidgetName(), "Background", pUpkeepInfo.getDescription().upper(), CvUtil.FONT_LEFT_JUSTIFY, self.X_STATS_PANE, self.Y_STATS_PANE, 0, FontTypes.TITLE_FONT, WidgetTypes.WIDGET_GENERAL, -1, -1 )
	
## Revolutions ##		
		szSpecialText = u"" 
		for i in Revolutions.Revolutions().Civics:
			if gc.getCivicInfo(self.eCivic).getType() == i[0]:
				szSpecialText += u"%s%+d %s</font>" %(CyTranslator().getText("", ()), i[1], CyTranslator().getText("TXT_KEY_REVOLUTION_CIVIC_HELP", ()))
				screen.appendListBoxString(self.STATS_ID, u"<font=3>" + szSpecialText.upper() + u"</font>", WidgetTypes.WIDGET_GENERAL, 0, 0, CvUtil.FONT_LEFT_JUSTIFY)
## Revolutions ##
	def drawRequires(self, screen, info):
		screen.addPanel(self.record(self.REQUIRES_PANEL_ID), localText.getText("TXT_KEY_PEDIA_REQUIRES", ()), "", False, True, self.REQUIRES_PANEL_X, self.REQUIRES_PANEL_Y, self.REQUIRES_PANEL_W, self.REQUIRES_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		screen.enableSelect(self.REQUIRES_PANEL_ID, False)
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
				if (pLoopUnit.getPrereqCivic() == self.eCivic):
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
				for iPre in range(pLoopBuilding.getNumPrereqAndCivics()):
					if (pLoopBuilding.getPrereqAndCivics(iPre) == self.eCivic):
						screen.attachImageButton(self.BUILDING_PANEL_ID, "", pLoopBuilding.getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_BUILDING, eLoopBuilding, 1, False)
						break
