## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
from CvPythonExtensions import *
import CvUtil
import CvPediaPage

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

class CvPediaTerrain(CvPediaPage.CvPediaPage):
	"Civilopedia Screen for Terrain"
	
	def __init__(self, section):
		super(CvPediaTerrain, self).__init__(section, CivilopediaPageTypes.CIVILOPEDIA_PAGE_TERRAIN, WidgetTypes.WIDGET_PEDIA_JUMP_TO_TERRAIN, "Terrain")
		
		self.eTerrain = -1
	
	def getData(self):
		return (self.eTerrain, -1)
	
	def setData(self, eTerrain, iPageData2):
		if (self.eTerrain != eTerrain):
			self.clear()
			self.eTerrain = eTerrain
	
	def getInfo(self):
		return gc.getTerrainInfo(self.eTerrain)
	
	def calculateLayout(self):
		self.calculateTitleLayout()
		self.calculateSynopsisLayout(self.main.PAGE_AREA_W, self.main.COMMON_ICON_PANEL_SIZE * 2 + self.main.COMMON_MARGIN_H)
		self.calculateTwoColumnLayout(0, 50, 0, 50)
		
		self.SPECIAL_PANEL_X = self.COLUMN1_X
		self.SPECIAL_PANEL_Y = self.SYNOPSIS_PANEL_Y + self.SYNOPSIS_PANEL_H + self.main.COMMON_MARGIN_H
		self.SPECIAL_PANEL_W = self.COLUMN1_W
		self.SPECIAL_PANEL_H = self.main.PAGE_AREA_Y + self.main.PAGE_AREA_H - self.SPECIAL_PANEL_Y
		
		self.CIVILOPEDIA_PANEL_X = self.COLUMN2_X
		self.CIVILOPEDIA_PANEL_Y = self.SPECIAL_PANEL_Y
		self.CIVILOPEDIA_PANEL_W = self.COLUMN2_W
		self.CIVILOPEDIA_PANEL_H = self.main.PAGE_AREA_Y + self.main.PAGE_AREA_H - self.CIVILOPEDIA_PANEL_Y
	
	def draw(self):
		if (not self.isEmpty()):
			return
		
		screen = self.getScreen()
		info = self.getInfo()
		ePlayer = gc.getGame().getActivePlayer()
		
		self.drawTitle(screen, info, ePlayer)
		self.drawSynopsis(screen, info, ePlayer)
		self.drawSpecial(screen, CyGameTextMgr().getTerrainHelp(self.eTerrain, True)[1:])
		self.drawCivilopedia(screen, info, "TXT_KEY_PEDIA_DESCRIPTION")
	
	def drawStats(self, screen, info, ePlayer):
		screen.addListBoxGFC(self.record(self.STATS_ID), "", self.STATS_X, self.STATS_Y, self.STATS_W, self.STATS_H, TableStyles.TABLE_STYLE_EMPTY)
		screen.enableSelect(self.STATS_ID, False)
		
		for k in range(YieldTypes.NUM_YIELD_TYPES):
			iYield = info.getYield(k)
			if (iYield != 0):
				szYield = (u"%s: %i %c" % (gc.getYieldInfo(k).getDescription().upper(), iYield, gc.getYieldInfo(k).getChar()))
				screen.appendListBoxStringNoUpdate(self.STATS_ID, u"<font=3>" + szYield + u"</font>", WidgetTypes.WIDGET_GENERAL, 0, 0, CvUtil.FONT_LEFT_JUSTIFY)
		
		screen.updateListBox(self.STATS_ID)
