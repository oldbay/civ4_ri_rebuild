## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005

from CvPythonExtensions import *
import string
import CvUtil
import ScreenInput
import CvScreenEnums
import CvPediaScreen		# base class

import CvPediaSectionTechnologies
import CvPediaSectionUnits
import CvPediaSectionMilitary
import CvPediaSectionBuildings
import CvPediaSectionSociety
import CvPediaSectionWorld
import CvPediaSectionHelp

"""
import CvPediaPromotion
import CvPediaBonus
import CvPediaTerrain
import CvPediaFeature
import CvPediaImprovement
import CvPediaCivic
import CvPediaCivilization
import CvPediaLeader
import CvPediaSpecialist
import CvPediaHistory
import CvPediaProject
import CvPediaReligion
import CvPediaCorporation
"""

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

class CvPediaMain(CvPediaScreen.CvPediaScreen):
	"Civilopedia Main Screen"

	def __init__(self):
		super(CvPediaMain, self).__init__()
	
		self.PEDIA_MAIN_SCREEN_NAME = "PediaMainScreen"
		self.INTERFACE_ART_INFO = "SCREEN_BG_OPAQUE"
		
		self.WIDGET_ID = "PediaMainWidget"
		self.SECTION_NAV_PANEL_ID = "PediaMainSectionPanel"
		self.SECTION_NAV_LINK_ID = "PediaMainSectionNav"
		self.BACKGROUND_ID = "PediaMainBackground"
		self.TOP_PANEL_ID = "PediaMainTopPanel"
		self.BOTTOM_PANEL_ID = "PediaMainBottomPanel"
		self.TITLE_ID = "PediaMainTitle"
		self.EXIT_ID = "PediaMainExitWidget"
		self.BACK_ID = "PediaMainBack"
		self.NEXT_ID = "PediaMainForward"
		self.TOP_ID = "PediaMainTop"
		self.LIST_ID = "PediaMainList"
		
		self.EXIT_TEXT = u"<font=4>" + localText.getText("TXT_KEY_PEDIA_SCREEN_EXIT", ()).upper() + "</font>"
		self.BACK_TEXT = u"<font=4>" + localText.getText("TXT_KEY_PEDIA_SCREEN_BACK", ()).upper() + "</font>"
		self.FORWARD_TEXT = u"<font=4>" + localText.getText("TXT_KEY_PEDIA_SCREEN_FORWARD", ()).upper() + "</font>"
		self.MENU_TEXT = u"<font=4>" + localText.getText("TXT_KEY_PEDIA_SCREEN_TOP", ()).upper() + "</font>"
		
		self.nWidgetCount = 0
		
		self.SCREEN_X = 0
		self.SCREEN_Y = 0
		self.SCREEN_W = 0
		self.SCREEN_H = 0
		
		"""
		self.eSection = CivilopediaSectionTypes.NO_CIVILOPEDIA_SECTION
		self.eView = CivilopediaViewTypes.NO_CIVILOPEDIA_VIEW
		self.iViewData1 = -1
		self.iViewData2 = -1
		self.ePage = CivilopediaPageTypes.NO_CIVILOPEDIA_PAGE
		self.iPageData1 = -1
		self.iPageData2 = -1

		self.activeSection = None
		self.activePage = None
		
		self.eDefaultSection = CivilopediaSectionTypes.CIVILOPEDIA_SECTION_TECHNOLOGIES
		"""
		
		# Sections
		self.sections = [
			CvPediaSectionTechnologies.CvPediaSectionTechnologies(self),
			CvPediaSectionUnits.CvPediaSectionUnits(self),
			CvPediaSectionMilitary.CvPediaSectionMilitary(self),
			CvPediaSectionBuildings.CvPediaSectionBuildings(self),
			CvPediaSectionSociety.CvPediaSectionSociety(self),
			CvPediaSectionWorld.CvPediaSectionWorld(self),
			CvPediaSectionHelp.CvPediaSectionHelp(self)
		]
		
		self.section = self.sections[0]
		
		# Navigation
		self.pediaHistory = []
		self.pediaFuture = []
		
		screen = self.getScreen()
		screen.setRenderInterfaceOnly(True)
		screen.setScreenGroup(1)

	def update(self, fDelta):
		self.section.update(fDelta)
	
	def pediaShow(self):
		screen = self.getScreen()
		
		if (screen.getXResolution() != self.SCREEN_W or screen.getYResolution() != self.SCREEN_H):
			self.clearHierarchy()
			self.calculateLayoutHierarchy()
			screen.setDimensions(self.SCREEN_X, self.SCREEN_Y, self.SCREEN_W, self.SCREEN_H)
			self.drawHierarchy()
			CvUtil.pyPrint("Pedia Screen Resolution: " + str(self.SCREEN_W) + "x" + str(self.SCREEN_H))
		elif not screen.isActive():
			self.clearHierarchy()
			self.drawHierarchy()
			CvUtil.pyPrint("Restoring pedia")
		
		if not screen.isActive():
			screen.showScreen(PopupStates.POPUPSTATE_IMMEDIATE, False)
		
		if (len(self.pediaHistory) == 0):
			self.pediaHistory.append(self.getState())

	def pediaBack(self):
		if (len(self.pediaHistory) > 1):
			self.pediaFuture.append(self.pediaHistory.pop())
			current = self.pediaHistory[-1]
			self.processAction(current[0], current[1], current[2], current[3], current[4], current[5], current[6])
		
	def pediaForward(self):
		if (len(self.pediaFuture) > 0):
			self.pediaHistory.append(self.pediaFuture.pop())
			current = self.pediaHistory[-1]
			self.processAction(current[0], current[1], current[2], current[3], current[4], current[5], current[6])
	
	def pediaClearSearchFilter(self):
		CvUtil.pyPrint("Clearing pedia search filter")
		self.section.pediaClearSearchFilter()
	
	def pediaTogglePlayableFilter(self):
		CvUtil.pyPrint("Toggling pedia playable filter")
		self.section.pediaTogglePlayableFilter()

	def pediaAction(self, eSection, eView, iViewData1, iViewData2, ePage, iPageData1, iPageData2):
		CvUtil.pyPrint("PediaAction [" + str(eSection) + ", " + str(eView) + ", " + str(iViewData1) + ", " + str(iViewData2) + ", " + str(ePage) + ", " + str(iPageData1) + ", " + str(iPageData2) + "]")
		
		self.processAction(eSection, eView, iViewData1, iViewData2, ePage, iPageData1, iPageData2)
		
		state = self.getState()
		
		if (not self.pediaHistory):
			# First entry
			self.pediaHistory.append(state)
			self.pediaFuture = []
		elif (self.pediaHistory[-1][4] != state[4] or self.pediaHistory[-1][5] != state[5] or self.pediaHistory[-1][6] != state[6]):
			# New page
			self.pediaHistory.append(state)
			self.pediaFuture = []
		elif (self.pediaHistory[-1][1] != state[1] or self.pediaHistory[-1][2] != state[2] or self.pediaHistory[-1][3] != state[3]):
			# New view of same page
			# TODO: Check whether the page is still shown within the view?
			self.pediaHistory.pop()
			self.pediaHistory.append(state)
	
	def processAction(self, eSection, eView, iViewData1, iViewData2, ePage, iPageData1, iPageData2):
		screen = self.getScreen()
		
		# Force drawing if the screen was hidden or the resolution has changed
		if (screen.getXResolution() != self.SCREEN_W or screen.getYResolution() != self.SCREEN_H):
			self.clearHierarchy()
			self.calculateLayout()
			screen.setDimensions(self.SCREEN_X, self.SCREEN_Y, self.SCREEN_W, self.SCREEN_H)
			self.draw()
		elif (not screen.isActive()):
			self.clearHierarchy()
			self.draw()
		
		if (eSection != CivilopediaSectionTypes.NO_CIVILOPEDIA_SECTION):
			section = self.getSection(eSection)
		elif (eView != CivilopediaViewTypes.NO_CIVILOPEDIA_VIEW):
			section = self.getSectionForView(eView, iViewData1, iViewData2)
		elif (ePage != CivilopediaPageTypes.NO_CIVILOPEDIA_PAGE):
			section = self.getSectionForPage(ePage, iPageData1, iPageData2)
		else:
			section = None
		
		if (section != None):
			self.setSection(section)
			self.section.pediaAction(eView, iViewData1, iViewData2, ePage, iPageData1, iPageData2)
		
		if not screen.isActive():
			screen.showScreen(PopupStates.POPUPSTATE_IMMEDIATE, False)
	
	def getState(self):
		(iViewData1, iViewData2) = self.section.view.getData()
		(iPageData1, iPageData2) = self.section.page.getData()
		return (self.section.eSection, self.section.view.eView, iViewData1, iViewData2, self.section.page.ePage, iPageData1, iPageData2)
	
	def setSection(self, section):
		if (section != self.section):
			self.section.clearHierarchy()
			self.section = section
			self.drawSectionLinks(self.getScreen())
	
	def updatePlayerContext(self):
		for section in self.sections:
			section.updatePlayerContext()
	
	def setMode(self, eMode):
		for section in self.sections:
			section.setMode(eMode)
	
	def setCivilization(self, eCivilization):
		for section in self.sections:
			section.setCivilization(eCivilization)

	"""
	def setActiveView(self, eView, iViewData1, iViewData2):
		if (eView == self.main.eView and iViewData1 == self.main.iViewData1 and iViewData2 == self.main.iViewData2):
			return
		
		self.clear()
		
		(szViewTitle, eView, iViewData1, ePage, eWidget, items) = self.getViewProperties(eView, iViewData1, iViewData2)
		self.main.eView = eView
		self.main.iViewData1 = iViewData1
		self.main.iViewData2 = iViewData2
	"""
	
	def getSection(self, eSection):
		for section in self.sections:
			if (section.eSection == eSection):
				return section
		return None
	
	def getSectionForView(self, eView, iViewData1, iViewData2):
		for section in self.sections:
			if (section.getView(eView, iViewData1, iViewData2) != None):
				return section
		return None
	
	def getSectionForPage(self, ePage, iPageData1, iPageData2):
		for section in self.sections:
			if (section.getViewForPage(ePage, iPageData1, iPageData2) != None):
				return section
		return None
	
	# Will handle the input for this screen...
	def handleInput (self, input):
		return self.section.handleInput(input)
	
	def clearHierarchy(self):
		self.clear()
		self.section.clearHierarchy()
	
	def calculateLayoutHierarchy(self):
		self.calculateLayout()
		self.section.calculateLayoutHierarchy()
	
	def calculateLayout(self):
		screen = self.getScreen()
		
		self.SCREEN_W = screen.getXResolution()
		self.SCREEN_H = screen.getYResolution()
		
		self.SECTION_NAV_PANEL_X = 200
		self.SECTION_NAV_PANEL_Y = 6
		self.SECTION_NAV_PANEL_W = self.SCREEN_W - self.SECTION_NAV_PANEL_X
		self.SECTION_NAV_PANEL_H = 50
		self.SECTION_NAV_PANEL_SPACING_X = 20
		
		self.TOP_PANEL_X = 0
		self.TOP_PANEL_Y = 0
		self.TOP_PANEL_W = self.SCREEN_W
		self.TOP_PANEL_H = 55
		
		self.BOTTOM_PANEL_X = 0
		self.BOTTOM_PANEL_Y = self.SCREEN_H - 55
		self.BOTTOM_PANEL_W = self.SCREEN_W
		self.BOTTOM_PANEL_H = 55
		
		self.TITLE_X = 100
		self.TITLE_Y = 10
		
		self.EXIT_X = self.SCREEN_W - 30
		self.EXIT_Y = self.SCREEN_H - 38
		
		self.BACK_X = 50
		self.BACK_Y = self.EXIT_Y
		
		self.FORWARD_X = 200
		self.FORWARD_Y = self.EXIT_Y
		
		if (self.SCREEN_W >= 1600 and self.SCREEN_H >= 900):
			self.COMMON_MARGIN_W = 24
			self.COMMON_MARGIN_H = 24
			self.COMMON_ICON_SIZE = GenericButtonSizes.BUTTON_SIZE_CUSTOM
			self.COMMON_ICON_PANEL_SIZE = 124
		elif (self.SCREEN_W >= 1280 and self.SCREEN_H >= 960):
			self.COMMON_MARGIN_W = 18
			self.COMMON_MARGIN_H = 12
			self.COMMON_ICON_SIZE = GenericButtonSizes.BUTTON_SIZE_CUSTOM
			self.COMMON_ICON_PANEL_SIZE = 124
		else:
			self.COMMON_MARGIN_W = 16
			self.COMMON_MARGIN_H = 10
			self.COMMON_ICON_SIZE = GenericButtonSizes.BUTTON_SIZE_46
			self.COMMON_ICON_PANEL_SIZE = 96
		
		#self.COMMON_ICON_PANE_SIZE = 150
		#self.COMMON_ICON_SIZE = 64
		
		self.SECTION_AREA_X = 0
		self.SECTION_AREA_Y = 51
		self.SECTION_AREA_W = self.SCREEN_W
		self.SECTION_AREA_H = self.SCREEN_H - self.SECTION_AREA_Y - 56
		
		self.VIEW_AREA_X = self.SECTION_AREA_X
		self.VIEW_AREA_Y = self.SECTION_AREA_Y
		self.VIEW_AREA_W = self.SECTION_AREA_W
		self.VIEW_AREA_H = self.SECTION_AREA_H
		
		self.OUTER_PAGE_AREA_X = self.SECTION_AREA_X
		self.OUTER_PAGE_AREA_Y = self.SECTION_AREA_Y
		self.OUTER_PAGE_AREA_W = self.SECTION_AREA_W
		self.OUTER_PAGE_AREA_H = self.SECTION_AREA_H
		
		self.PAGE_AREA_X = self.OUTER_PAGE_AREA_X + self.COMMON_MARGIN_W
		self.PAGE_AREA_Y = self.OUTER_PAGE_AREA_Y
		self.PAGE_AREA_W = self.OUTER_PAGE_AREA_W - self.COMMON_MARGIN_W * 2
		self.PAGE_AREA_H = self.OUTER_PAGE_AREA_H
		
		# The rest will go away eventually
		self.X_MENU = 450
		self.Y_MENU = self.EXIT_Y
		
		self.BUTTON_SIZE = 64
		self.BUTTON_COLUMNS = 9
		
		self.X_ITEMS_PANE = 30
		self.Y_ITEMS_PANE = 80
		self.H_ITEMS_PANE = 610
		self.W_ITEMS_PANE = 740
		
		self.ITEMS_MARGIN = 18
		self.ITEMS_SEPARATION = 3
				
		self.X_LINKS = 797
		self.Y_LINKS = 51
		self.H_LINKS = 665
		if (self.SCREEN_W >= 1600 and self.SCREEN_H >= 900):
			self.W_LINKS = 325
		else:
			self.W_LINKS = 225

	def drawHierarchy(self):
		self.draw()
		self.section.drawHierarchy()
	
	def draw(self):
		if (not self.isEmpty()):
			return
		
		screen = self.getScreen()
		
		self.clear()
		
		#screen.setDimensions(screen.centerX(0), screen.centerY(0), self.SCREEN_W, self.SCREEN_H)

		# Background
		screen.addDDSGFC(self.record(self.BACKGROUND_ID), ArtFileMgr.getInterfaceArtInfo("SCREEN_BG_OPAQUE").getPath(), 0, 0, self.SCREEN_W, self.SCREEN_H, WidgetTypes.WIDGET_GENERAL, -1, -1 )
		screen.addPanel(self.record(self.TOP_PANEL_ID), u"", u"", True, False, self.TOP_PANEL_X, self.TOP_PANEL_Y, self.TOP_PANEL_W, self.TOP_PANEL_H, PanelStyles.PANEL_STYLE_TOPBAR )
		screen.addPanel(self.record(self.BOTTOM_PANEL_ID), u"", u"", True, False, self.BOTTOM_PANEL_X, self.BOTTOM_PANEL_Y, self.BOTTOM_PANEL_W, self.BOTTOM_PANEL_H, PanelStyles.PANEL_STYLE_BOTTOMBAR )
		
		# Title
		szTitle = u"<font=4b>" + localText.getText("TXT_KEY_PEDIA_TITLE", ()).upper() + u"</font>"
		screen.setLabel(self.record(self.TITLE_ID), "Background", szTitle, CvUtil.FONT_CENTER_JUSTIFY, self.TITLE_X, self.TITLE_Y, 0, FontTypes.TITLE_FONT, WidgetTypes.WIDGET_PEDIA_DESCRIPTION, -1, -1)
		
		# Section navigation panel
		screen.addPanel(self.record(self.SECTION_NAV_PANEL_ID), "", "", False, False, self.SECTION_NAV_PANEL_X, self.SECTION_NAV_PANEL_Y, self.SECTION_NAV_PANEL_W, self.SECTION_NAV_PANEL_H, PanelStyles.PANEL_STYLE_EMPTY)
		
		# Section navigation links
		self.drawSectionLinks(screen)

		# Exit button
		screen.setText(self.record(self.EXIT_ID), "Background", self.EXIT_TEXT, CvUtil.FONT_RIGHT_JUSTIFY, self.EXIT_X, self.EXIT_Y, 0, FontTypes.TITLE_FONT, WidgetTypes.WIDGET_CLOSE_SCREEN, -1, -1)

		# Back
		screen.setText(self.record(self.BACK_ID), "Background", self.BACK_TEXT, CvUtil.FONT_LEFT_JUSTIFY, self.BACK_X, self.BACK_Y, 0, FontTypes.TITLE_FONT, WidgetTypes.WIDGET_PEDIA_BACK, 1, -1)
			
		# Forward
		screen.setText(self.record(self.NEXT_ID), "Background", self.FORWARD_TEXT, CvUtil.FONT_LEFT_JUSTIFY, self.FORWARD_X, self.FORWARD_Y, 0, FontTypes.TITLE_FONT, WidgetTypes.WIDGET_PEDIA_FORWARD, 1, -1)

		# List of items on the right
		#screen.addListBoxGFC(self.LIST_ID, "", self.X_LINKS, self.Y_LINKS, self.W_LINKS, self.H_LINKS, TableStyles.TABLE_STYLE_STANDARD)
		#screen.enableSelect(self.LIST_ID, True)
		#screen.setStyle(self.LIST_ID, "Table_StandardCiv_Style")
	
	def drawSectionLinks(self, screen):
		iNavX = 0
		iCharacterScale = 1.6
		
		for section in self.sections:
			if (iNavX > 0):
				iNavX += self.SECTION_NAV_PANEL_SPACING_X
			szTitle = section.getTitle()
			szText = u"<font=4b>" + szTitle + u"</font>"
			if (section == self.section):
				szText = localText.changeTextColor(szText, gc.getInfoTypeForString("COLOR_THEME_SELECTED_TEXT"))
			screen.setTextAt(self.record(self.SECTION_NAV_LINK_ID + section.szWidget), self.SECTION_NAV_PANEL_ID, szText, CvUtil.FONT_LEFT_JUSTIFY, iNavX, 0, 0, FontTypes.TITLE_FONT, WidgetTypes.WIDGET_PEDIA_SECTION, section.eSection, -1)
			iNavX += int(CyInterface().determineWidth(szTitle) * iCharacterScale)
			iNavX += 10
		
		screen.setPanelSize(self.SECTION_NAV_PANEL_ID, int(self.SECTION_NAV_PANEL_X + ((self.SCREEN_W - self.SECTION_NAV_PANEL_X - iNavX) / 2)), self.SECTION_NAV_PANEL_Y, iNavX, 50)
	
	"""
	def placeTechs(self):
		screen = self.getScreen()
		
		# Create and place a tech pane									
		list = self.getSortedList( gc.getNumTechInfos(), gc.getTechInfo )
		
		nColumns = 4
		nEntries = len(list)
		nRows = nEntries // nColumns
		if (nEntries % nColumns):
			nRows += 1
		tableName = self.getNextWidgetName()
		screen.addTableControlGFC(tableName, nColumns, self.X_ITEMS_PANE, self.Y_ITEMS_PANE+5, self.W_ITEMS_PANE, self.H_ITEMS_PANE-5, False, False, 24, 24, TableStyles.TABLE_STYLE_STANDARD);
		screen.enableSelect(tableName, False)
		for i in range(nColumns):
			screen.setTableColumnHeader(tableName, i, "", self.W_ITEMS_PANE/nColumns)
		
		iCounter = 0
		iNumRows = 0
		for item in list:
			iRow = iCounter % nRows
			iColumn = iCounter // nRows
			if iRow >= iNumRows:
				iNumRows += 1
				screen.appendTableRow(tableName)
			screen.setTableText(tableName, iColumn, iRow, u"<font=3>" + item[0] + u"</font>", gc.getTechInfo(item[1]).getButton(), WidgetTypes.WIDGET_PEDIA_JUMP_TO_TECH, item[1], 1, CvUtil.FONT_LEFT_JUSTIFY)
			iCounter += 1
	
	def placeUnits(self):
		screen = self.getScreen()
		
		# Create and place a tech pane									
		list = self.getSortedList( gc.getNumUnitInfos(), gc.getUnitInfo )
		
		if gc.getDefineINT("CIVILOPEDIA_SHOW_ACTIVE_CIVS_ONLY") and gc.getGame().isFinalInitialized():
			listCopy = list[:]
			for item in listCopy:
				if not gc.getGame().isUnitEverActive(item[1]):
					list.remove(item)

		nColumns = 5
		nEntries = len(list)
		nRows = nEntries // nColumns
		if (nEntries % nColumns):
			nRows += 1
		tableName = self.getNextWidgetName()
		screen.addTableControlGFC(tableName, nColumns, self.X_ITEMS_PANE, self.Y_ITEMS_PANE+5, self.W_ITEMS_PANE, self.H_ITEMS_PANE-5, False, False, 24, 24, TableStyles.TABLE_STYLE_STANDARD);
		screen.enableSelect(tableName, False)
		for i in range(nColumns):
			screen.setTableColumnHeader(tableName, i, "", self.W_ITEMS_PANE/nColumns)
		
		iCounter = 0
		iNumRows = 0
		for item in list:
			iRow = iCounter % nRows
			iColumn = iCounter // nRows
			if iRow >= iNumRows:
				iNumRows += 1
				screen.appendTableRow(tableName)
			szButton = gc.getUnitInfo(item[1]).getButton()
			if self.iActivePlayer != -1:
				szButton = gc.getPlayer(self.iActivePlayer).getUnitButton(item[1])
			screen.setTableText(tableName, iColumn, iRow, u"<font=3>" + item[0] + u"</font>", szButton, WidgetTypes.WIDGET_PEDIA_JUMP_TO_UNIT, item[1], 1, CvUtil.FONT_LEFT_JUSTIFY)
			iCounter += 1
	
	def placeBuildings(self):
		screen = self.getScreen()
		
		# Create and place a tech pane									
		list = self.pediaBuildingScreen.getBuildingSortedList(false)

		if gc.getDefineINT("CIVILOPEDIA_SHOW_ACTIVE_CIVS_ONLY") and gc.getGame().isFinalInitialized():
			listCopy = list[:]
			for item in listCopy:
				if not gc.getGame().isBuildingEverActive(item[1]):
					list.remove(item)

		nColumns = 4
		nEntries = len(list)
		nRows = nEntries // nColumns
		if (nEntries % nColumns):
			nRows += 1
		tableName = self.getNextWidgetName()
		screen.addTableControlGFC(tableName, nColumns, self.X_ITEMS_PANE, self.Y_ITEMS_PANE+5, self.W_ITEMS_PANE, self.H_ITEMS_PANE-5, False, False, 24, 24, TableStyles.TABLE_STYLE_STANDARD);
		screen.enableSelect(tableName, False)
		for i in range(nColumns):
			screen.setTableColumnHeader(tableName, i, "", self.W_ITEMS_PANE/nColumns)
		
		iCounter = 0
		iNumRows = 0
		for item in list:
			iRow = iCounter % nRows
			iColumn = iCounter // nRows
			if iRow >= iNumRows:
				iNumRows += 1
				screen.appendTableRow(tableName)
			screen.setTableText(tableName, iColumn, iRow, u"<font=3>" + item[0] + u"</font>", gc.getBuildingInfo(item[1]).getButton(), WidgetTypes.WIDGET_PEDIA_JUMP_TO_BUILDING, item[1], 1, CvUtil.FONT_LEFT_JUSTIFY)
			iCounter += 1
	
	def placeWonders(self):
		screen = self.getScreen()
		
		# Create and place a tech pane									
		list = self.pediaBuildingScreen.getBuildingSortedList(true)

		nColumns = 3
		nEntries = len(list)
		nRows = nEntries // nColumns
		if (nEntries % nColumns):
			nRows += 1
		tableName = self.getNextWidgetName()
		screen.addTableControlGFC(tableName, nColumns, self.X_ITEMS_PANE, self.Y_ITEMS_PANE+5, self.W_ITEMS_PANE, self.H_ITEMS_PANE-5, False, False, 24, 24, TableStyles.TABLE_STYLE_STANDARD);
		screen.enableSelect(tableName, False)
		for i in range(nColumns):
			screen.setTableColumnHeader(tableName, i, "", self.W_ITEMS_PANE/nColumns)
		
		iCounter = 0
		iNumRows = 0
		for item in list:
			iRow = iCounter % nRows
			iColumn = iCounter // nRows
			if iRow >= iNumRows:
				iNumRows += 1
				screen.appendTableRow(tableName)
			screen.setTableText(tableName, iColumn, iRow, u"<font=3>" + item[0] + u"</font>", gc.getBuildingInfo(item[1]).getButton(), WidgetTypes.WIDGET_PEDIA_JUMP_TO_BUILDING, item[1], 1, CvUtil.FONT_LEFT_JUSTIFY)
			iCounter += 1
	
	def placeBoni(self):
		screen = self.getScreen()
		
		# Create and place a tech pane									
		list = self.getSortedList( gc.getNumBonusInfos(), gc.getBonusInfo )

		nColumns = 2
		nEntries = len(list)
		nRows = nEntries // nColumns
		if (nEntries % nColumns):
			nRows += 1
		tableName = self.getNextWidgetName()
		screen.addTableControlGFC(tableName, nColumns, self.X_ITEMS_PANE, self.Y_ITEMS_PANE+5, self.W_ITEMS_PANE, self.H_ITEMS_PANE-5, False, False, 24, 24, TableStyles.TABLE_STYLE_STANDARD);
		screen.enableSelect(tableName, False)
		for i in range(nColumns):
			screen.setTableColumnHeader(tableName, i, "", self.W_ITEMS_PANE/nColumns)
		
		iCounter = 0
		iNumRows = 0
		for item in list:
			iRow = iCounter % nRows
			iColumn = iCounter // nRows
			if iRow >= iNumRows:
				iNumRows += 1
				screen.appendTableRow(tableName)
			screen.setTableText(tableName, iColumn, iRow, u"<font=3>" + item[0] + u"</font>", gc.getBonusInfo(item[1]).getButton(), WidgetTypes.WIDGET_PEDIA_JUMP_TO_BONUS, item[1], 1, CvUtil.FONT_LEFT_JUSTIFY)
			iCounter += 1
	
	def placeImprovements(self):
		screen = self.getScreen()
		
		# Create and place a tech pane									
		list = self.getSortedList( gc.getNumImprovementInfos(), gc.getImprovementInfo )

		nColumns = 1
		nEntries = len(list)
		nRows = nEntries // nColumns
		if (nEntries % nColumns):
			nRows += 1
		tableName = self.getNextWidgetName()
		screen.addTableControlGFC(tableName, nColumns, self.X_ITEMS_PANE, self.Y_ITEMS_PANE+5, self.W_ITEMS_PANE, self.H_ITEMS_PANE-5, False, False, 24, 24, TableStyles.TABLE_STYLE_STANDARD);
		screen.enableSelect(tableName, False)
		for i in range(nColumns):
			screen.setTableColumnHeader(tableName, i, "", self.W_ITEMS_PANE/nColumns)
		
		iCounter = 0
		iNumRows = 0
		for item in list:
			iRow = iCounter % nRows
			iColumn = iCounter // nRows
			if iRow >= iNumRows:
				iNumRows += 1
				screen.appendTableRow(tableName)
			screen.setTableText(tableName, iColumn, iRow, u"<font=3>" + item[0] + u"</font>", gc.getImprovementInfo(item[1]).getButton(), WidgetTypes.WIDGET_PEDIA_JUMP_TO_IMPROVEMENT, item[1], 1, CvUtil.FONT_LEFT_JUSTIFY)
			iCounter += 1
	
	def placePromotions(self):
		screen = self.getScreen()
		
		# Create and place a tech pane									
		list = self.getSortedList( gc.getNumPromotionInfos(), gc.getPromotionInfo )

		nColumns = 3
		nEntries = len(list)
		nRows = nEntries // nColumns
		if (nEntries % nColumns):
			nRows += 1
		tableName = self.getNextWidgetName()
		screen.addTableControlGFC(tableName, nColumns, self.X_ITEMS_PANE, self.Y_ITEMS_PANE+5, self.W_ITEMS_PANE, self.H_ITEMS_PANE-5, False, False, 24, 24, TableStyles.TABLE_STYLE_STANDARD);
		screen.enableSelect(tableName, False)
		for i in range(nColumns):
			screen.setTableColumnHeader(tableName, i, "", self.W_ITEMS_PANE/nColumns)
		
		iCounter = 0
		iNumRows = 0
		for item in list:
			iRow = iCounter % nRows
			iColumn = iCounter // nRows
			if iRow >= iNumRows:
				iNumRows += 1
				screen.appendTableRow(tableName)
			screen.setTableText(tableName, iColumn, iRow, u"<font=3>" + item[0] + u"</font>", gc.getPromotionInfo(item[1]).getButton(), WidgetTypes.WIDGET_PEDIA_JUMP_TO_PROMOTION, item[1], 1, CvUtil.FONT_LEFT_JUSTIFY)
			iCounter += 1

	def placeUnitGroups(self):
		screen = self.getScreen()
		
		# Create and place a tech pane									
		list = self.getSortedList( gc.getNumUnitCombatInfos(), gc.getUnitCombatInfo )

		nColumns = 1
		nEntries = len(list)
		nRows = nEntries // nColumns
		if (nEntries % nColumns):
			nRows += 1
		tableName = self.getNextWidgetName()
		screen.addTableControlGFC(tableName, nColumns, self.X_ITEMS_PANE, self.Y_ITEMS_PANE+5, self.W_ITEMS_PANE, self.H_ITEMS_PANE-5, False, False, 24, 24, TableStyles.TABLE_STYLE_STANDARD);
		screen.enableSelect(tableName, False)
		for i in range(nColumns):
			screen.setTableColumnHeader(tableName, i, "", self.W_ITEMS_PANE/nColumns)
		
		iCounter = 0
		iNumRows = 0
		for item in list:
			iRow = iCounter % nRows
			iColumn = iCounter // nRows
			if iRow >= iNumRows:
				iNumRows += 1
				screen.appendTableRow(tableName)
			screen.setTableText(tableName, iColumn, iRow, u"<font=3>" + item[0] + u"</font>", gc.getUnitCombatInfo(item[1]).getButton(), WidgetTypes.WIDGET_PEDIA_JUMP_TO_UNIT_COMBAT, item[1], 1, CvUtil.FONT_LEFT_JUSTIFY)
			iCounter += 1

	def placeCivs(self):
		screen = self.getScreen()
		
		# Create and place a tech pane									
		list = self.getSortedList( gc.getNumCivilizationInfos(), gc.getCivilizationInfo )

		if gc.getDefineINT("CIVILOPEDIA_SHOW_ACTIVE_CIVS_ONLY") and gc.getGame().isFinalInitialized():
			listCopy = list[:]
			for item in listCopy:
				if not gc.getGame().isCivEverActive(item[1]):
					list.remove(item)

		nColumns = 2
		nEntries = len(list)
		nRows = nEntries // nColumns
		if (nEntries % nColumns):
			nRows += 1
		tableName = self.getNextWidgetName()
		screen.addTableControlGFC(tableName, nColumns, self.X_ITEMS_PANE, self.Y_ITEMS_PANE+5, self.W_ITEMS_PANE, self.H_ITEMS_PANE-5, False, False, 24, 24, TableStyles.TABLE_STYLE_STANDARD);
		screen.enableSelect(tableName, False)
		for i in range(nColumns):
			screen.setTableColumnHeader(tableName, i, "", self.W_ITEMS_PANE/nColumns)
		
		iCounter = 0
		iNumRows = 0
		for item in list:
			iRow = iCounter % nRows
			iColumn = iCounter // nRows
			if (gc.getCivilizationInfo(item[1]).isPlayable()):
				if iRow >= iNumRows:
					iNumRows += 1
					screen.appendTableRow(tableName)
				screen.setTableText(tableName, iColumn, iRow, u"<font=3>" + item[0] + u"</font>", gc.getCivilizationInfo(item[1]).getButton(), WidgetTypes.WIDGET_PEDIA_JUMP_TO_CIV, item[1], 1, CvUtil.FONT_LEFT_JUSTIFY)
				iCounter += 1
						
	def placeLeaders(self):
		screen = self.getScreen()
		
		# Create and place a tech pane									
		list = self.getSortedList( gc.getNumLeaderHeadInfos(), gc.getLeaderHeadInfo )
		listCopy = list[:]
		for item in listCopy:
			if item[1] == gc.getDefineINT("BARBARIAN_LEADER"):
				list.remove(item)
			elif gc.getDefineINT("CIVILOPEDIA_SHOW_ACTIVE_CIVS_ONLY") and gc.getGame().isFinalInitialized():
				if not gc.getGame().isLeaderEverActive(item[1]):
					list.remove(item)

		nColumns = 3
		nEntries = len(list)
		nRows = nEntries // nColumns
		if (nEntries % nColumns):
			nRows += 1
		tableName = self.getNextWidgetName()
		screen.addTableControlGFC(tableName, nColumns, self.X_ITEMS_PANE, self.Y_ITEMS_PANE+5, self.W_ITEMS_PANE, self.H_ITEMS_PANE-5, False, False, 24, 24, TableStyles.TABLE_STYLE_STANDARD);
		screen.enableSelect(tableName, False)
		for i in range(nColumns):
			screen.setTableColumnHeader(tableName, i, "", self.W_ITEMS_PANE/nColumns)
		
		iCounter = 0
		iNumRows = 0
		for item in list:
			iRow = iCounter % nRows
			iColumn = iCounter // nRows
			if iRow >= iNumRows:
				iNumRows += 1
				screen.appendTableRow(tableName)
			
			iNumCivs = 0
			iLeaderCiv = -1
			for iCiv in range(gc.getNumCivilizationInfos()):
				civ = gc.getCivilizationInfo(iCiv)
				if civ.isLeaders(item[1]):
					iNumCivs += 1
					iLeaderCiv = iCiv

			if iNumCivs != 1:
				iLeaderCiv = -1

			screen.setTableText(tableName, iColumn, iRow, u"<font=3>" + item[0] + u"</font>", gc.getLeaderHeadInfo(item[1]).getButton(), WidgetTypes.WIDGET_PEDIA_JUMP_TO_LEADER, item[1], iLeaderCiv, CvUtil.FONT_LEFT_JUSTIFY)
			iCounter += 1

	def placeReligions(self):
		screen = self.getScreen()
		
		# Create and place a tech pane									
		list = self.getSortedList( gc.getNumReligionInfos(), gc.getReligionInfo )

		nColumns = 1
		nEntries = len(list)
		nRows = nEntries // nColumns
		if (nEntries % nColumns):
			nRows += 1
		tableName = self.getNextWidgetName()
		screen.addTableControlGFC(tableName, nColumns, self.X_ITEMS_PANE, self.Y_ITEMS_PANE+5, self.W_ITEMS_PANE, self.H_ITEMS_PANE-5, False, False, 24, 24, TableStyles.TABLE_STYLE_STANDARD);
		screen.enableSelect(tableName, False)
		for i in range(nColumns):
			screen.setTableColumnHeader(tableName, i, "", self.W_ITEMS_PANE/nColumns)
		
		iCounter = 0
		iNumRows = 0
		for item in list:
			iRow = iCounter % nRows
			iColumn = iCounter // nRows
			if iRow >= iNumRows:
				iNumRows += 1
				screen.appendTableRow(tableName)
			screen.setTableText(tableName, iColumn, iRow, u"<font=3>" + item[0] + u"</font>", gc.getReligionInfo(item[1]).getButton(), WidgetTypes.WIDGET_PEDIA_JUMP_TO_RELIGION, item[1], 1, CvUtil.FONT_LEFT_JUSTIFY)
			iCounter += 1
						
	def placeCorporations(self):
		screen = self.getScreen()
		
		# Create and place a tech pane									
		list = self.getSortedList( gc.getNumCorporationInfos(), gc.getCorporationInfo )

		nColumns = 1
		nEntries = len(list)
		nRows = nEntries // nColumns
		if (nEntries % nColumns):
			nRows += 1
		tableName = self.getNextWidgetName()
		screen.addTableControlGFC(tableName, nColumns, self.X_ITEMS_PANE, self.Y_ITEMS_PANE+5, self.W_ITEMS_PANE, self.H_ITEMS_PANE-5, False, False, 24, 24, TableStyles.TABLE_STYLE_STANDARD);
		screen.enableSelect(tableName, False)
		for i in range(nColumns):
			screen.setTableColumnHeader(tableName, i, "", self.W_ITEMS_PANE/nColumns)
		
		iCounter = 0
		iNumRows = 0
		for item in list:
			iRow = iCounter % nRows
			iColumn = iCounter // nRows
			if iRow >= iNumRows:
				iNumRows += 1
				screen.appendTableRow(tableName)
			screen.setTableText(tableName, iColumn, iRow, u"<font=3>" + item[0] + u"</font>", gc.getCorporationInfo(item[1]).getButton(), WidgetTypes.WIDGET_PEDIA_JUMP_TO_CORPORATION, item[1], 1, CvUtil.FONT_LEFT_JUSTIFY)
			iCounter += 1
						
	def placeCivics(self):
		screen = self.getScreen()
		
		# Create and place a tech pane									
		list = self.getSortedList( gc.getNumCivicInfos(), gc.getCivicInfo )

		nColumns = 1
		nEntries = len(list)
		nRows = nEntries // nColumns
		if (nEntries % nColumns):
			nRows += 1
		tableName = self.getNextWidgetName()
		screen.addTableControlGFC(tableName, nColumns, self.X_ITEMS_PANE, self.Y_ITEMS_PANE+5, self.W_ITEMS_PANE, self.H_ITEMS_PANE-5, False, False, 24, 24, TableStyles.TABLE_STYLE_STANDARD);
		screen.enableSelect(tableName, False)
		for i in range(nColumns):
			screen.setTableColumnHeader(tableName, i, "", self.W_ITEMS_PANE/nColumns)
		
		iCounter = 0
		iNumRows = 0
		for item in list:
			iRow = iCounter % nRows
			iColumn = iCounter // nRows
			if iRow >= iNumRows:
				iNumRows += 1
				screen.appendTableRow(tableName)
			screen.setTableText(tableName, iColumn, iRow, u"<font=3>" + item[0] + u"</font>", gc.getCivicInfo(item[1]).getButton(), WidgetTypes.WIDGET_PEDIA_JUMP_TO_CIVIC, item[1], 1, CvUtil.FONT_LEFT_JUSTIFY)
			iCounter += 1

	def placeProjects(self):
		screen = self.getScreen()
		
		# Create and place a tech pane									
		list = self.pediaProjectScreen.getProjectSortedList()

		nColumns = 1
		nEntries = len(list)
		nRows = nEntries // nColumns
		if (nEntries % nColumns):
			nRows += 1
		tableName = self.getNextWidgetName()
		screen.addTableControlGFC(tableName, nColumns, self.X_ITEMS_PANE, self.Y_ITEMS_PANE+5, self.W_ITEMS_PANE, self.H_ITEMS_PANE-5, False, False, 24, 24, TableStyles.TABLE_STYLE_STANDARD);
		screen.enableSelect(tableName, False)
		for i in range(nColumns):
			screen.setTableColumnHeader(tableName, i, "", self.W_ITEMS_PANE/nColumns)
		
		iCounter = 0
		iNumRows = 0
		for item in list:
			iRow = iCounter % nRows
			iColumn = iCounter // nRows
			if iRow >= iNumRows:
				iNumRows += 1
				screen.appendTableRow(tableName)
			screen.setTableText(tableName, iColumn, iRow, u"<font=3>" + item[0] + u"</font>", gc.getProjectInfo(item[1]).getButton(), WidgetTypes.WIDGET_PEDIA_JUMP_TO_PROJECT, item[1], 1, CvUtil.FONT_LEFT_JUSTIFY)
			iCounter += 1

	def placeTerrains(self):
		screen = self.getScreen()
		
		# Create and place a tech pane									
		list = self.getSortedList( gc.getNumTerrainInfos(), gc.getTerrainInfo )

		nColumns = 1
		nEntries = len(list)
		nRows = nEntries // nColumns
		if (nEntries % nColumns):
			nRows += 1
		tableName = self.getNextWidgetName()
		screen.addTableControlGFC(tableName, nColumns, self.X_ITEMS_PANE, self.Y_ITEMS_PANE+5, self.W_ITEMS_PANE, self.H_ITEMS_PANE-5, False, False, 24, 24, TableStyles.TABLE_STYLE_STANDARD);
		screen.enableSelect(tableName, False)
		for i in range(nColumns):
			screen.setTableColumnHeader(tableName, i, "", self.W_ITEMS_PANE/nColumns)
		
		iCounter = 0
		iNumRows = 0
		for item in list:
			iColumn = iCounter // nRows
			iRow = iCounter % nRows
			if iRow >= iNumRows:
				iNumRows += 1
				screen.appendTableRow(tableName)
			screen.setTableText(tableName, iColumn, iRow, u"<font=3>" + item[0] + u"</font>", gc.getTerrainInfo(item[1]).getButton(), WidgetTypes.WIDGET_PEDIA_JUMP_TO_TERRAIN, item[1], 1, CvUtil.FONT_LEFT_JUSTIFY)
			iCounter += 1

						
	def placeFeatures(self):
		screen = self.getScreen()
		
		# Create and place a tech pane									
		list = self.getSortedList( gc.getNumFeatureInfos(), gc.getFeatureInfo )

		nColumns = 1
		nEntries = len(list)
		nRows = nEntries // nColumns
		if (nEntries % nColumns):
			nRows += 1
		tableName = self.getNextWidgetName()
		screen.addTableControlGFC(tableName, nColumns, self.X_ITEMS_PANE, self.Y_ITEMS_PANE+5, self.W_ITEMS_PANE, self.H_ITEMS_PANE-5, False, False, 24, 24, TableStyles.TABLE_STYLE_STANDARD);
		screen.enableSelect(tableName, False)
		for i in range(nColumns):
			screen.setTableColumnHeader(tableName, i, "", self.W_ITEMS_PANE/nColumns)
		
		iCounter = 0
		iNumRows = 0
		for item in list:
			iColumn = iCounter // nRows
			iRow = iCounter % nRows
			if iRow >= iNumRows:
				iNumRows += 1
				screen.appendTableRow(tableName)
			screen.setTableText(tableName, iColumn, iRow, u"<font=3>" + item[0] + u"</font>", gc.getFeatureInfo(item[1]).getButton(), WidgetTypes.WIDGET_PEDIA_JUMP_TO_FEATURE, item[1], 1, CvUtil.FONT_LEFT_JUSTIFY)
			iCounter += 1
						
	def placeConcepts(self):
		screen = self.getScreen()
		
		# Create and place a tech pane									
		list = self.getSortedList( gc.getNumConceptInfos(), gc.getConceptInfo )

		nColumns = 3
		nEntries = len(list)
		nRows = nEntries // nColumns
		if (nEntries % nColumns):
			nRows += 1
		tableName = self.getNextWidgetName()
		screen.addTableControlGFC(tableName, nColumns, self.X_ITEMS_PANE, self.Y_ITEMS_PANE+5, self.W_ITEMS_PANE, self.H_ITEMS_PANE-5, False, False, 24, 24, TableStyles.TABLE_STYLE_STANDARD);
		screen.enableSelect(tableName, False)
		for i in range(nColumns):
			screen.setTableColumnHeader(tableName, i, "", self.W_ITEMS_PANE/nColumns)
		
		iCounter = 0
		iNumRows = 0
		for item in list:
			iColumn = iCounter // nRows
			iRow = iCounter % nRows
			if iRow >= iNumRows:
				iNumRows += 1
				screen.appendTableRow(tableName)
			screen.setTableText(tableName, iColumn, iRow, u"<font=3>" + item[0] + u"</font>", gc.getConceptInfo(item[1]).getButton(), WidgetTypes.WIDGET_PEDIA_DESCRIPTION, CivilopediaPageTypes.CIVILOPEDIA_PAGE_CONCEPT, item[1], CvUtil.FONT_LEFT_JUSTIFY)
			iCounter += 1

						
	def placeNewConcepts(self):
		screen = self.getScreen()
		
		# Create and place a tech pane									
		list = self.getSortedList( gc.getNumNewConceptInfos(), gc.getNewConceptInfo )

		nColumns = 3
		nEntries = len(list)
		nRows = nEntries // nColumns
		if (nEntries % nColumns):
			nRows += 1
		tableName = self.getNextWidgetName()
		screen.addTableControlGFC(tableName, nColumns, self.X_ITEMS_PANE, self.Y_ITEMS_PANE+5, self.W_ITEMS_PANE, self.H_ITEMS_PANE-5, False, False, 24, 24, TableStyles.TABLE_STYLE_STANDARD);
		screen.enableSelect(tableName, False)
		for i in range(nColumns):
			screen.setTableColumnHeader(tableName, i, "", self.W_ITEMS_PANE/nColumns)
		
		iCounter = 0
		iNumRows = 0
		for item in list:
			iColumn = iCounter // nRows
			iRow = iCounter % nRows
			if iRow >= iNumRows:
				iNumRows += 1
				screen.appendTableRow(tableName)
			screen.setTableText(tableName, iColumn, iRow, u"<font=3>" + item[0] + u"</font>", gc.getNewConceptInfo(item[1]).getButton(), WidgetTypes.WIDGET_PEDIA_DESCRIPTION, CivilopediaPageTypes.CIVILOPEDIA_PAGE_CONCEPT_NEW, item[1], CvUtil.FONT_LEFT_JUSTIFY)
			iCounter += 1

						
	def placeSpecialists(self):
		screen = self.getScreen()
		
		# Create and place a tech pane									
		list = self.getSortedList( gc.getNumSpecialistInfos(), gc.getSpecialistInfo )

		nColumns = 1
		nEntries = len(list)
		nRows = nEntries // nColumns
		if (nEntries % nColumns):
			nRows += 1
		tableName = self.getNextWidgetName()
		screen.addTableControlGFC(tableName, nColumns, self.X_ITEMS_PANE, self.Y_ITEMS_PANE+5, self.W_ITEMS_PANE, self.H_ITEMS_PANE-5, False, False, 24, 24, TableStyles.TABLE_STYLE_STANDARD);
		screen.enableSelect(tableName, False)
		for i in range(nColumns):
			screen.setTableColumnHeader(tableName, i, "", self.W_ITEMS_PANE/nColumns)
		
		iCounter = 0
		iNumRows = 0
		for item in list:
			iColumn = iCounter // nRows
			iRow = iCounter % nRows
			if iRow >= iNumRows:
				iNumRows += 1
				screen.appendTableRow(tableName)
			screen.setTableText(tableName, iColumn, iRow, u"<font=3>" + item[0] + u"</font>", gc.getSpecialistInfo(item[1]).getButton(), WidgetTypes.WIDGET_PEDIA_JUMP_TO_SPECIALIST, item[1], 1, CvUtil.FONT_LEFT_JUSTIFY)
			iCounter += 1
						
	def placeHints(self):
		screen = self.getScreen()
				                
		self.szAreaId = self.getNextWidgetName()
		screen.addListBoxGFC( self.szAreaId, "",
                                      self.X_ITEMS_PANE, self.Y_ITEMS_PANE, self.W_ITEMS_PANE, self.H_ITEMS_PANE, TableStyles.TABLE_STYLE_STANDARD )
		
		szHintsText = CyGameTextMgr().buildHintsList()
		hintText = string.split( szHintsText, "\n" )
		for hint in hintText:
			if len( hint ) != 0:
				screen.appendListBoxStringNoUpdate( self.szAreaId, hint, WidgetTypes.WIDGET_GENERAL, -1, -1, CvUtil.FONT_LEFT_JUSTIFY )
				
		screen.updateListBox(self.szAreaId)
	
	def placeLinks(self, bRedraw):
		
		screen = self.getScreen()
		
		if bRedraw:
			screen.clearListBoxGFC(self.LIST_ID)

		i = 0
		for szCategory in self.listCategories:
			if bRedraw:
				screen.appendListBoxStringNoUpdate(self.LIST_ID, szCategory, WidgetTypes.WIDGET_PEDIA_MAIN, i, 0, CvUtil.FONT_LEFT_JUSTIFY )
			i += 1
		
		if bRedraw:
			screen.updateListBox(self.LIST_ID)

		screen.setSelectedListBoxStringGFC(self.LIST_ID, self.iCategory)
	"""

