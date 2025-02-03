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

class CvPediaSpecialist(CvPediaPage.CvPediaPage):
	"Civilopedia Screen for Specialists"

	def __init__(self, section):
		super(CvPediaSpecialist, self).__init__(section, CivilopediaPageTypes.CIVILOPEDIA_PAGE_SPECIALIST, WidgetTypes.WIDGET_PEDIA_JUMP_TO_SPECIALIST, "Specialist")
		
		self.eSpecialist = -1
		
		self.YIELDS_PANEL_ID = self.szWidget + "YieldsPanel"
		self.YIELDS_TEXT_ID = self.szWidget + "YieldsText"
	
	def getData(self):
		return (self.eSpecialist, -1)
	
	def setData(self, eSpecialist, iPageData2):
		if (self.eSpecialist != eSpecialist):
			self.clear()
			self.eSpecialist = eSpecialist
	
	def getInfo(self):
		return gc.getSpecialistInfo(self.eSpecialist)
	
	def calculateLayout(self):
		self.calculateTitleLayout()
		self.calculateSynopsisLayout(-1, self.main.COMMON_ICON_PANEL_SIZE * 2 + self.main.COMMON_MARGIN_H)
		self.calculateTwoColumnLayout(self.SYNOPSIS_PANEL_W, 0)
		
		self.YIELDS_PANEL_X = self.COLUMN2_X
		self.YIELDS_PANEL_Y = self.PAGE_CONTENT_Y
		self.YIELDS_PANEL_W = self.COLUMN2_W
		self.YIELDS_PANEL_H = self.SYNOPSIS_PANEL_H
		
		self.CIVILOPEDIA_PANEL_X = self.main.PAGE_AREA_X
		self.CIVILOPEDIA_PANEL_Y = self.SYNOPSIS_PANEL_Y + self.SYNOPSIS_PANEL_H + self.main.COMMON_MARGIN_H
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
		self.drawYields(screen)
		self.drawCivilopedia(screen, info, "TXT_KEY_PEDIA_BACKGROUND")
	
	def drawYields(self, screen):
		szSpecialText = CyGameTextMgr().getSpecialistHelp(self.eSpecialist, True)[1:]
		screen.addPanel(self.record(self.YIELDS_PANEL_ID), localText.getText("TXT_KEY_PEDIA_YIELDS", ()), "", True, False, self.YIELDS_PANEL_X, self.YIELDS_PANEL_Y, self.YIELDS_PANEL_W, self.YIELDS_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		screen.addMultilineText(self.record(self.YIELDS_TEXT_ID), szSpecialText, self.YIELDS_PANEL_X+5, self.YIELDS_PANEL_Y+30, self.YIELDS_PANEL_W-10, self.YIELDS_PANEL_H-35, WidgetTypes.WIDGET_GENERAL, -1, -1, CvUtil.FONT_LEFT_JUSTIFY)

	"""
	def placeLinks(self, bRedraw):

		screen = self.top.getScreen()
		
		if bRedraw:
			screen.clearListBoxGFC(self.top.LIST_ID)		

		# sort Improvements alphabetically
		listSorted=[(0,0)]*gc.getNumSpecialistInfos()
		for j in range(gc.getNumSpecialistInfos()):
			listSorted[j] = (gc.getSpecialistInfo(j).getDescription(), j)
		listSorted.sort()	

		i = 0
		iSelected = 0			
		for iI in range(gc.getNumSpecialistInfos()):
			if (not gc.getSpecialistInfo(listSorted[iI][1]).isGraphicalOnly()):
				if bRedraw:
					screen.appendListBoxString(self.top.LIST_ID, listSorted[iI][0], WidgetTypes.WIDGET_PEDIA_JUMP_TO_SPECIALIST, listSorted[iI][1], 0, CvUtil.FONT_LEFT_JUSTIFY )
				if listSorted[iI][1] == self.eSpecialist:
					iSelected = i
				i += 1

		screen.setSelectedListBoxStringGFC(self.top.LIST_ID, iSelected)

	# Will handle the input for this screen...
	def handleInput (self, inputClass):
		return 0
	"""


