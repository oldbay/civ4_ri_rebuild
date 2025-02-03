## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
from CvPythonExtensions import *
import CvUtil
import CvPediaPage

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

class CvPediaWorld(CvPediaPage.CvPediaPage):
	"Civilopedia Screen for World Sizes"

	def __init__(self, section):
		super(CvPediaWorld, self).__init__(section, CivilopediaPageTypes.CIVILOPEDIA_PAGE_WORLD, WidgetTypes.WIDGET_PEDIA_JUMP_TO_WORLD, "World")
		
		self.eWorld = -1
		
		self.DIMENSIONS_PANEL_ID = self.szWidget + "DimensionsPanel"
		self.DIMENSIONS_TEXT_ID = self.szWidget + "DimensionsText"
	
	def getData(self):
		return (self.eWorld, -1)
	
	def setData(self, eWorld, iPageData2):
		if (self.eWorld != eWorld):
			self.clear()
			self.eWorld = eWorld
	
	def getInfo(self):
		return gc.getWorldInfo(self.eWorld)
	
	def calculateLayout(self):
		self.calculateTitleLayout()
		
		iMaxGridWidth = 0
		iMaxGridHeight = 0
		
		for eWorld in range(gc.getNumWorldInfos()):
			kWorld = gc.getWorldInfo(eWorld)
			iMaxGridWidth = max(iMaxGridWidth, kWorld.getGridWidthInPlots())
			iMaxGridHeight = max(iMaxGridHeight, kWorld.getGridHeightInPlots())
		
		if (self.main.SCREEN_W >= 1600 and self.main.SCREEN_H >= 1024):
			iDimensionScale = 5
		elif (self.main.SCREEN_W >= 1280):
			iDimensionScale = 4
		else:
			iDimensionScale = 3
		
		self.DIMENSIONS_AREA_X = self.main.PAGE_AREA_X
		self.DIMENSIONS_AREA_Y = self.PAGE_CONTENT_Y
		self.DIMENSIONS_AREA_W = iMaxGridWidth * iDimensionScale
		self.DIMENSIONS_AREA_H = iMaxGridHeight * iDimensionScale
		
		info = self.getInfo()
		
		self.DIMENSIONS_PANEL_X = self.DIMENSIONS_AREA_X
		self.DIMENSIONS_PANEL_Y = self.DIMENSIONS_AREA_Y
		self.DIMENSIONS_PANEL_W = info.getGridWidthInPlots() * iDimensionScale
		self.DIMENSIONS_PANEL_H = info.getGridHeightInPlots() * iDimensionScale
		
		self.DIMENSIONS_TEXT_X = self.DIMENSIONS_PANEL_X + self.DIMENSIONS_PANEL_W / 2
		self.DIMENSIONS_TEXT_Y = self.DIMENSIONS_PANEL_Y + self.DIMENSIONS_PANEL_H / 2 - 12
		
		self.calculateTwoColumnLayout(self.DIMENSIONS_AREA_W, 0, 0, 100)
		
		self.CIVILOPEDIA_PANEL_X = self.COLUMN2_X
		self.CIVILOPEDIA_PANEL_Y = self.PAGE_CONTENT_Y
		self.CIVILOPEDIA_PANEL_W = self.COLUMN2_W
		self.CIVILOPEDIA_PANEL_H = self.DIMENSIONS_AREA_H
		
		self.EFFECTS_PANEL_X = self.main.PAGE_AREA_X
		self.EFFECTS_PANEL_Y = self.DIMENSIONS_AREA_Y + self.DIMENSIONS_AREA_H + self.main.COMMON_MARGIN_H
		self.EFFECTS_PANEL_W = self.main.PAGE_AREA_W
		self.EFFECTS_PANEL_H = self.main.PAGE_AREA_Y + self.main.PAGE_AREA_H - self.EFFECTS_PANEL_Y
	
	def draw(self):
		if (not self.isEmpty()):
			return
		
		screen = self.getScreen()
		info = self.getInfo()
		ePlayer = gc.getGame().getActivePlayer()
		
		self.drawTitle(screen, info, ePlayer)
		self.drawDimensions(screen, info)
		#self.drawCivilopedia(screen, info, "TXT_KEY_PEDIA_DESCRIPTION")
		self.drawEffects(screen, CyGameTextMgr().getWorldSizeHelp(self.eWorld, True)[1:])
	
	def drawDimensions(self, screen, info):
		screen.addPanel(self.record(self.DIMENSIONS_PANEL_ID), "", "", False, True, self.DIMENSIONS_PANEL_X, self.DIMENSIONS_PANEL_Y, self.DIMENSIONS_PANEL_W, self.DIMENSIONS_PANEL_H, PanelStyles.PANEL_STYLE_FLAT)
		
		szDimensions = u"<font=4b>" + localText.getText("TXT_KEY_WORLD_SIZE_DIMENSIONS", (info.getGridWidthInPlots(), info.getGridHeightInPlots())) + u"</font>"
		screen.setLabel(self.record(self.DIMENSIONS_TEXT_ID), "", szDimensions, CvUtil.FONT_CENTER_JUSTIFY, self.DIMENSIONS_TEXT_X, self.DIMENSIONS_TEXT_Y, 0, FontTypes.TITLE_FONT, WidgetTypes.WIDGET_GENERAL, -1, -1)
	