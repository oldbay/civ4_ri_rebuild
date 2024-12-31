## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
from CvPythonExtensions import *
import CvUtil
import CvPediaPage
import ScreenInput
import CvScreenEnums

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

class CvPediaProject(CvPediaPage.CvPediaPage):
	"Civilopedia Screen for Projects"
	
	def __init__(self, section):
		super(CvPediaProject, self).__init__(section, CivilopediaPageTypes.CIVILOPEDIA_PAGE_PROJECT, WidgetTypes.WIDGET_PEDIA_JUMP_TO_PROJECT, "Project")
		
		self.eProject = -1
		
		self.REQUIRES_PANEL_ID = self.szWidget + "RequiresPanel"
	
	def getData(self):
		return (self.eProject, -1)
	
	def setData(self, eProject, iPageData2):
		if (self.eProject != eProject):
			self.clear()
			self.eProject = eProject
	
	def getInfo(self):
		return gc.getProjectInfo(self.eProject)
	
	def calculateLayout(self):
		self.calculateTitleLayout()
		self.calculateTwoColumnLayout(420, 0)
		self.calculateSynopsisLayout(self.COLUMN1_W)
		
		self.CIVILOPEDIA_PANEL_X = self.COLUMN2_X
		self.CIVILOPEDIA_PANEL_Y = self.PAGE_CONTENT_Y
		self.CIVILOPEDIA_PANEL_W = self.COLUMN2_W
		self.CIVILOPEDIA_PANEL_H = self.main.PAGE_AREA_Y + self.main.PAGE_AREA_H - self.CIVILOPEDIA_PANEL_Y
		
		self.REQUIRES_PANEL_X = self.COLUMN1_X
		self.REQUIRES_PANEL_Y = self.SYNOPSIS_PANEL_Y + self.SYNOPSIS_PANEL_H + self.main.COMMON_MARGIN_H
		self.REQUIRES_PANEL_W = self.COLUMN1_W
		self.REQUIRES_PANEL_H = self.main.COMMON_ICON_PANEL_SIZE
		
		self.SPECIAL_PANEL_X = self.COLUMN1_X
		self.SPECIAL_PANEL_Y = self.REQUIRES_PANEL_Y + self.REQUIRES_PANEL_H + self.main.COMMON_MARGIN_H
		self.SPECIAL_PANEL_W = self.COLUMN1_W
		self.SPECIAL_PANEL_H = self.main.PAGE_AREA_Y + self.main.PAGE_AREA_H - self.SPECIAL_PANEL_Y
	
	def draw(self):
		if (not self.isEmpty()):
			return
		
		screen = self.getScreen()
		info = self.getInfo()
		ePlayer = gc.getGame().getActivePlayer()
		
		self.drawTitle(screen, info, ePlayer)
		self.drawSynopsis(screen, info, ePlayer)
		self.drawRequires(screen, info)
		self.drawSpecial(screen, CyGameTextMgr().getProjectHelp(self.eProject, True, None)[1:])
		self.drawCivilopedia(screen, info, "TXT_KEY_PEDIA_BACKGROUND")
	
	def drawStats(self, screen, info, ePlayer):
		# Happiness/health/commerce/great people modifiers
		screen.addListBoxGFC(self.record(self.STATS_ID), "", self.STATS_X, self.STATS_Y, self.STATS_W, self.STATS_H, TableStyles.TABLE_STYLE_EMPTY)
		screen.enableSelect(self.STATS_ID, False)
		
		if (isWorldProject(self.eProject)):
			iMaxInstances = info.getMaxGlobalInstances()
			szProjectType = localText.getText("TXT_KEY_PEDIA_WORLD_PROJECT", ())
			if (iMaxInstances > 1):
				szProjectType += " " + localText.getText("TXT_KEY_PEDIA_WONDER_INSTANCES", (iMaxInstances,))
			screen.appendListBoxString(self.STATS_ID, u"<font=3>" + szProjectType.upper() + u"</font>", WidgetTypes.WIDGET_GENERAL, 0, 0, CvUtil.FONT_LEFT_JUSTIFY)
#			screen.attachTextGFC(self.STATS_ID, "", szProjectType.upper(), FontTypes.GAME_FONT, WidgetTypes.WIDGET_GENERAL, -1, -1)
		
		if (isTeamProject(self.eProject)):
			iMaxInstances = info.getMaxTeamInstances()
			szProjectType = localText.getText("TXT_KEY_PEDIA_TEAM_PROJECT", ())
			if (iMaxInstances > 1):
				szProjectType += " " + localText.getText("TXT_KEY_PEDIA_WONDER_INSTANCES", (iMaxInstances,))
			screen.appendListBoxString(self.STATS_ID, u"<font=3>" + szProjectType.upper() + u"</font>", WidgetTypes.WIDGET_GENERAL, 0, 0, CvUtil.FONT_LEFT_JUSTIFY)
#			screen.attachTextGFC(self.STATS_ID, "", szProjectType.upper(), FontTypes.GAME_FONT, WidgetTypes.WIDGET_GENERAL, -1, -1)
		
		if (info.getProductionCost() > 0):
			if (ePlayer == -1):
				szCost = localText.getText("TXT_KEY_PEDIA_COST", ((info.getProductionCost() * gc.getDefineINT("PROJECT_PRODUCTION_PERCENT"))/100,))
			else:
				szCost = localText.getText("TXT_KEY_PEDIA_COST", (gc.getPlayer(ePlayer).getProjectProductionNeeded(self.eProject),))
			screen.appendListBoxString(self.STATS_ID, u"<font=3>" + szCost.upper() + u"%c" % gc.getYieldInfo(YieldTypes.YIELD_PRODUCTION).getChar() + u"</font>", WidgetTypes.WIDGET_GENERAL, 0, 0, CvUtil.FONT_LEFT_JUSTIFY)
#			screen.attachTextGFC(self.STATS_ID, "", szCost.upper() + u"%c" % gc.getYieldInfo(YieldTypes.YIELD_PRODUCTION).getChar(), FontTypes.GAME_FONT, WidgetTypes.WIDGET_GENERAL, -1, -1)
	
	def drawRequires(self, screen, info):
		screen.addPanel(self.record(self.REQUIRES_PANEL_ID), localText.getText("TXT_KEY_PEDIA_REQUIRES", ()), "", False, True, self.REQUIRES_PANEL_X, self.REQUIRES_PANEL_Y, self.REQUIRES_PANEL_W, self.REQUIRES_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		screen.enableSelect(self.REQUIRES_PANEL_ID, False)
		screen.attachLabel(self.REQUIRES_PANEL_ID, "", "  ")
		
		# Techs
		ePrereq = info.getTechPrereq()
		if (ePrereq >= 0):
			screen.attachImageButton(self.REQUIRES_PANEL_ID, "", gc.getTechInfo(ePrereq).getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_TECH, ePrereq, 1, False)
	
	# Place Special abilities
	"""
	def drawSpecial(self):
		
		screen = self.top.getScreen()
		
		panelName = self.top.getNextWidgetName()
		screen.addPanel( panelName, localText.getText("TXT_KEY_PEDIA_EFFECTS", ()), "", true, false,
				 self.X_SPECIAL, self.Y_SPECIAL, self.W_SPECIAL, self.H_SPECIAL, PanelStyles.PANEL_STYLE_BLUE50 )
		
		listName = self.top.getNextWidgetName()
		
		szSpecialText = CyGameTextMgr().getProjectHelp(self.eProject, True, None)[1:]
		screen.addMultilineText(listName, szSpecialText, self.X_SPECIAL+5, self.Y_SPECIAL+30, self.W_SPECIAL-10, self.H_SPECIAL-35, WidgetTypes.WIDGET_GENERAL, -1, -1, CvUtil.FONT_LEFT_JUSTIFY)	
	"""
	
	"""
	def drawText(self):
		
		screen = self.top.getScreen()
		
		panelName = self.top.getNextWidgetName()
		screen.addPanel( panelName, "", "", true, true,
				 self.X_TEXT, self.Y_TEXT, self.W_TEXT, self.H_TEXT, PanelStyles.PANEL_STYLE_BLUE50 )
		
		szText = gc.getProjectInfo(self.eProject).getCivilopedia()
		screen.attachMultilineText( panelName, "Text", szText, WidgetTypes.WIDGET_GENERAL, -1, -1, CvUtil.FONT_LEFT_JUSTIFY)
	"""
	
	"""
	def placeLinks(self, bRedraw):

		screen = self.top.getScreen()

		if bRedraw:		
			screen.clearListBoxGFC(self.top.LIST_ID)

		listSorted = self.getProjectSortedList()

		iSelected = 0
		i = 0
		for iI in range(len(listSorted)):
			if (not gc.getProjectInfo(listSorted[iI][1]).isGraphicalOnly()):
				if bRedraw:
					screen.appendListBoxString(self.top.LIST_ID, listSorted[iI][0], WidgetTypes.WIDGET_PEDIA_JUMP_TO_PROJECT, listSorted[iI][1], 0, CvUtil.FONT_LEFT_JUSTIFY)
				if listSorted[iI][1] == self.eProject:
					iSelected = i
				i += 1		

		screen.setSelectedListBoxStringGFC(self.top.LIST_ID, iSelected)
	"""

	def getProjectType(self, eProject):
		if (isWorldProject(eProject)):
			return (3)			

		if (isTeamProject(eProject)):
			return (2)

		# Regular project
		return (1)

	def getProjectSortedList(self):
		listOfAllTypes = []
		for iBuildingType in range(4):		
			listBuildings = []
			iCount = 0
			for iBuilding in range(gc.getNumProjectInfos()):
				if (self.getProjectType(iBuilding) == iBuildingType and not gc.getProjectInfo(iBuilding).isGraphicalOnly()):
					listBuildings.append(iBuilding)
					iCount += 1
			
			listSorted = [(0,0)] * iCount
			iI = 0
			for iBuilding in listBuildings:
				listSorted[iI] = (gc.getProjectInfo(iBuilding).getDescription(), iBuilding)
				iI += 1
			listSorted.sort()
			for i in range(len(listSorted)):
				listOfAllTypes.append(listSorted[i])
		return listOfAllTypes
	
	# Will handle the input for this screen...
	def handleInput (self, input):
		return 0

