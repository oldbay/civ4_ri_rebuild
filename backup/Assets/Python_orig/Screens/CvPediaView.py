## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005

from CvPythonExtensions import *
import CvUtil
import FontUtil

from CvPediaScreen import CvPediaScreen

from BugUtil import Timer

# globals
gc = CyGlobalContext()
localText = CyTranslator()

class CvPediaView(CvPediaScreen):
	"Civilopedia View"

	def __init__(self, section, eView, szTitle, ePage, eWidget, items, modes = [], sorts = [("TXT_KEY_PEDIA_SORT_LEXICAL", CivilopediaSortTypes.CIVILOPEDIA_SORT_LEXICAL)]):
		super(CvPediaView, self).__init__()
		
		self.main = section.main
		self.section = section
		self.eView = eView
		self.szTitle = szTitle
		self.ePage = ePage
		self.eWidget = eWidget
		self.items = items
		self.modes = modes
		self.sorts = sorts
		
		if (len(self.modes) > 0):
			self.eMode = self.modes[0][1]
		else:
			self.eMode = CivilopediaModeTypes.NO_CIVILOPEDIA_MODE
		
		if (len(self.sorts) > 0):
			self.eSort = self.sorts[0][1]
		else:
			self.eSort = CivilopediaSortTypes.NO_CIVILOPEDIA_SORT
		
		self.eCivilization = 0
		
		#self.groupedItems = lambda: []
		self.visibleItems = []
		
		materializedItems = self.getSortedItems()
		if (len(materializedItems) > 0):
			self.iPageData1 = materializedItems[0][1]
		else:
			self.iPageData1 = -1
		self.iPageData2 = -1
		
		self.iNumItems = len(self.getCurrentItems())
		
		self.iItemFillIndex = 0
		
		self.MODE_DROPDOWN_ID = "Pedia" + self.section.szWidget + "ModeDropdown"
		self.CIVILIZATION_DROPDOWN_ID = "Pedia" + self.section.szWidget + "CivilizationDropdown"
		
		self.SEARCH_PANEL_ID = "Pedia" + self.section.szWidget + "SearchPanel"
		self.SEARCH_LABEL_ID = "Pedia" + self.section.szWidget + "SearchLabel"
		self.CLEAR_SEARCH_FILTER_LABEL_ID = "Pedia" + self.section.szWidget + "ClearFilterLabel"
		self.CLEAR_SEARCH_FILTER_OVERLAY_ID = "Pedia" + self.section.szWidget + "ClearFilterOverlay"
		self.ITEM_PANEL_ID = "Pedia" + self.section.szWidget + "PrimaryItemPanel"
		
		self.CLEAR_SEARCH_FILTER_TEXT = u"%c" % CyGame().getSymbolID(FontSymbols.CANCEL_CHAR)
		self.CLEAR_SEARCH_FILTER_OVERLAY_TEXT = u"    "
	
	def update(self, fDelta):
		if (not self.isEmpty()):
			self.updateItemPane()
	
	def clear(self):
		super(CvPediaView, self).clear()
		self.visibleItems = []
	
	def getTitle(self):
		if self.szTitle.startswith("TXT_"):
			return unicode(localText.getText(self.szTitle, ()))
		else:
			return unicode(self.szTitle)
	
	def matchesView(self, eView, iViewData1, iViewData2):
		return self.eView == eView
	
	def matchesPage(self, ePage, iPageData1, iPageData2):
		if (self.ePage != ePage):
			return False
		
		if (iPageData1 == -1):
			return True
		
		for (szItemTitle, iItemPageData1, szButton) in self.items():
			if (iItemPageData1 == iPageData1):
				return True
		
		return False
	
	def getData(self):
		return (-1, -1)
	
	def setData(self, iViewData1, iViewData2):
		return
	
	def setMode(self, eMode):
		if (self.eMode == eMode):
			return
		
		if (not self.isModeApplicable(eMode)):
			return
		
		CvUtil.pyPrint(self.getTitle() + " Mode Change: " + str(eMode))
		
		self.eMode = eMode
		
		if (not self.isEmpty()):
			self.clear()
			self.calculateLayout()
			if (self.getScreen().isActive()):
				self.syncMode()
				self.syncSearch()
	
	def setCivilization(self, eCivilization):
		if (self.eCivilization == eCivilization):
			return
		
		self.eCivilization = eCivilization
		
		if (self.eMode == CivilopediaModeTypes.CIVILOPEDIA_MODE_CIVILIZATION and not self.isEmpty() and self.getScreen().isActive()):
			self.syncMode()
			self.syncSearch()
	
	def updatePlayerContext(self):
		ePlayer = gc.getGame().getActivePlayer()
		if (ePlayer == -1):
			return
		elif (self.isModeApplicable(CivilopediaModeTypes.CIVILOPEDIA_MODE_ACTIVE_PLAYER)):
			self.setMode(CivilopediaModeTypes.CIVILOPEDIA_MODE_ACTIVE_PLAYER)
		elif (self.isModeApplicable(CivilopediaModeTypes.CIVILOPEDIA_MODE_ACTIVE_GAME)):
			self.setMode(CivilopediaModeTypes.CIVILOPEDIA_MODE_ACTIVE_GAME)
		elif (self.isModeApplicable(CivilopediaModeTypes.CIVILOPEDIA_MODE_CIVILIZATION)):
			self.setMode(CivilopediaModeTypes.CIVILOPEDIA_MODE_CIVILIZATION)
			self.setCivilization(gc.getPlayer(ePlayer).getCivilizationType())
	
	def isModeApplicable(self, eMode):
		for (szText, eValidMode) in self.modes:
			if (eValidMode == eMode):
				return True
		return False
	
	def calculateLayout(self):
		iViewY = self.main.VIEW_AREA_Y
		
		if (len(self.modes) > 0):
			self.MODE_DROPDOWN_X = self.main.VIEW_AREA_X
			self.MODE_DROPDOWN_Y = iViewY + 2
			self.MODE_DROPDOWN_W = self.main.VIEW_AREA_W
			self.MODE_DROPDOWN_H = 30
			iViewY = self.MODE_DROPDOWN_Y + self.MODE_DROPDOWN_H
		
		if (self.eMode == CivilopediaModeTypes.CIVILOPEDIA_MODE_CIVILIZATION):
			self.CIVILIZATION_DROPDOWN_X = self.main.VIEW_AREA_X
			self.CIVILIZATION_DROPDOWN_Y = iViewY + 2
			self.CIVILIZATION_DROPDOWN_W = self.main.VIEW_AREA_W
			self.CIVILIZATION_DROPDOWN_H = 30
			iViewY = self.CIVILIZATION_DROPDOWN_Y + self.CIVILIZATION_DROPDOWN_H
		
		if (len(self.items()) > 0):
			self.SEARCH_PANEL_X = self.main.VIEW_AREA_X
			self.SEARCH_PANEL_Y = iViewY + 2
			self.SEARCH_PANEL_W = self.main.VIEW_AREA_W
			self.SEARCH_PANEL_H = 32
			iViewY = self.SEARCH_PANEL_Y + self.SEARCH_PANEL_H
			
			self.SEARCH_LABEL_X = self.SEARCH_PANEL_X + 10
			self.SEARCH_LABEL_Y = self.SEARCH_PANEL_Y + 8
			self.SEARCH_LABEL_DEFAULT = u"Type to filter..."
			
			self.CLEAR_SEARCH_FILTER_LABEL_X = self.SEARCH_PANEL_X + self.SEARCH_PANEL_W - 5
			self.CLEAR_SEARCH_FILTER_LABEL_Y = self.SEARCH_PANEL_Y + 10
			
			self.CLEAR_SEARCH_FILTER_OVERLAY_X = self.CLEAR_SEARCH_FILTER_LABEL_X
			self.CLEAR_SEARCH_FILTER_OVERLAY_Y = self.CLEAR_SEARCH_FILTER_LABEL_Y - 5
		
		self.ITEM_PANEL_X = self.main.VIEW_AREA_X
		self.ITEM_PANEL_Y = iViewY + 2
		self.ITEM_PANEL_W = self.main.VIEW_AREA_W
		self.ITEM_PANEL_H = self.main.VIEW_AREA_Y + self.main.VIEW_AREA_H - self.ITEM_PANEL_Y
	
	def draw(self):
		if (self.isEmpty()):
			self.syncMode()
			self.syncSearch()
	
	def setItem(self, iPageData1, iPageData2):
		self.iPageData1 = iPageData1
		self.iPageData2 = iPageData2
		iCount = len(self.visibleItems)
		screen = self.getScreen()
		for iRow in range(iCount):
			(szItemTitle, iItemPageData1, szButton) = self.visibleItems[iRow]
			if (iItemPageData1 == iPageData1):
				screen.selectRow(self.ITEM_PANEL_ID, iRow, False)
	
	def getCurrentItems(self):
		return self.items()
	
	def getFilteredItems(self):
		currentItems = self.getCurrentItems()
		
		if (len(self.section.szSearchString) == 0):
			return currentItems
		else:
			filteredItems = []
			for item in currentItems:
				(szItemTitle, iPageData1, szButton) = item
				if (self.section.szSearchString.lower() in szItemTitle.lower()):
					filteredItems.append(item)
			return filteredItems
	
	def getSortedItems(self):
		filteredItems = self.getFilteredItems()
		if (self.eSort == CivilopediaSortTypes.CIVILOPEDIA_SORT_LEXICAL):
			filteredItems.sort()
		return filteredItems
	
	def syncSearch(self):
		screen = self.getScreen()
		
		if (self.iNumItems > 0):
			self.syncSearchPanel(screen)
		
		self.syncItemPanel(screen)
	
	def syncMode(self):
		if (len(self.modes) == 0):
			return
		
		screen = self.getScreen()
		
		#self.clear()
		#self.syncSearchPanel(screen)
		self.syncModeDropdown(screen)
		self.syncCivilizationDropdown(screen)
		#self.syncItemPanel(screen)
	
	def syncSearchPanel(self, screen):
		szSearchString = self.section.szSearchString
		if (not szSearchString):
			szSearchString = self.SEARCH_LABEL_DEFAULT
		szSearchString = u"<font=3>" + szSearchString + u"</font>"
		
		if (not self.SEARCH_PANEL_ID in self.widgets):
			screen.addPanel(self.record(self.SEARCH_PANEL_ID), "", "", False, False, self.SEARCH_PANEL_X, self.SEARCH_PANEL_Y, self.SEARCH_PANEL_W, self.SEARCH_PANEL_H, PanelStyles.PANEL_STYLE_MAIN_BLACK25)
		
		if (not self.SEARCH_LABEL_ID in self.widgets):
			screen.setLabel(self.record(self.SEARCH_LABEL_ID), "Background", szSearchString, CvUtil.FONT_LEFT_JUSTIFY, self.SEARCH_LABEL_X, self.SEARCH_LABEL_Y, 0, FontTypes.GAME_FONT, WidgetTypes.WIDGET_GENERAL, -1, -1)
		else:
			screen.modifyLabel(self.SEARCH_LABEL_ID, szSearchString, CvUtil.FONT_LEFT_JUSTIFY)
		
		if (not self.CLEAR_SEARCH_FILTER_LABEL_ID in self.widgets):
			screen.setLabel(self.record(self.CLEAR_SEARCH_FILTER_LABEL_ID), "Background", self.CLEAR_SEARCH_FILTER_TEXT, CvUtil.FONT_RIGHT_JUSTIFY, self.CLEAR_SEARCH_FILTER_LABEL_X, self.CLEAR_SEARCH_FILTER_LABEL_Y, 0, FontTypes.GAME_FONT, WidgetTypes.WIDGET_PEDIA_CLEAR_SEARCH_FILTER, -1, -1)
		
		if (not self.CLEAR_SEARCH_FILTER_OVERLAY_ID in self.widgets):
			screen.setText(self.record(self.CLEAR_SEARCH_FILTER_OVERLAY_ID), "Background", self.CLEAR_SEARCH_FILTER_OVERLAY_TEXT, CvUtil.FONT_RIGHT_JUSTIFY, self.CLEAR_SEARCH_FILTER_OVERLAY_X, self.CLEAR_SEARCH_FILTER_OVERLAY_Y, 0, FontTypes.GAME_FONT, WidgetTypes.WIDGET_PEDIA_CLEAR_SEARCH_FILTER, -1, -1)
	
	def syncItemPanel(self, screen):
		timer = Timer(self.section.szWidget + ": Prep Item Pane")
		
		if (self.ITEM_PANEL_ID in self.widgets):
			screen.deleteWidget(self.ITEM_PANEL_ID)
		else:
			self.record(self.ITEM_PANEL_ID)
		
		screen.addTableControlGFC(self.ITEM_PANEL_ID, 1, self.ITEM_PANEL_X, self.ITEM_PANEL_Y+5, self.ITEM_PANEL_W, self.ITEM_PANEL_H-5, False, False, 24, 24, TableStyles.TABLE_STYLE_STANDARD)
		screen.setTableColumnHeader(self.ITEM_PANEL_ID, 0, "", self.ITEM_PANEL_W)
		screen.enableSelect(self.ITEM_PANEL_ID, True)
		screen.setStyle(self.ITEM_PANEL_ID, "Table_StandardCiv_Style")
		
		#list = getSortedList( gc.getNumTechInfos(), gc.getTechInfo )
		
		"""
		iRow = 0
		for item in self.list:
			screen.appendTableRow(self.ITEM_PANEL_ID)
			screen.setTableText(self.ITEM_PANEL_ID, 0, iRow, u"<font=3>" + item[0] + u"</font>", gc.getTechInfo(item[1]).getButton(), self.eWidget, item[1], 1, CvUtil.FONT_LEFT_JUSTIFY)
			iRow += 1
		"""
		
		timer.log("setup").start()
		
		self.visibleItems = self.getSortedItems()
		
		timer.log("filtering and sorting").start()
		
		screen.setTableNumRows(self.ITEM_PANEL_ID, len(self.visibleItems))
		timer.log("setTableNumRows to " + str(len(self.visibleItems))).start()
		
		self.iItemFillIndex = 0
		self.updateItemPane()
	
	def updateItemPane(self):
		iCount = len(self.visibleItems)
		if (self.iItemFillIndex < iCount):
			screen = self.getScreen()
			if (screen.isActive()):
				timer = Timer(self.section.szWidget + ": Fill Item Pane")
				iEnd = min(self.iItemFillIndex + 200, iCount)
				screen.hide(self.ITEM_PANEL_ID)
				for iRow in range(self.iItemFillIndex, iEnd):
					(szItemTitle, iPageData1, szButton) = self.visibleItems[iRow]
					screen.setTableText(self.ITEM_PANEL_ID, 0, iRow, u"<font=3>" + szItemTitle + u"</font>", szButton, self.eWidget, iPageData1, -1, CvUtil.FONT_LEFT_JUSTIFY)
					if (self.iPageData1 == iPageData1):
						screen.selectRow(self.ITEM_PANEL_ID, iRow, False)
				screen.show(self.ITEM_PANEL_ID)
				timer.log("chunk[" + str(self.iItemFillIndex) + "," + str(iEnd) + "]")
				self.iItemFillIndex = iEnd
	
	def syncModeDropdown(self, screen):
		if (not self.MODE_DROPDOWN_ID in self.widgets):
			self.record(self.MODE_DROPDOWN_ID)
		else:
			screen.deleteWidget(self.MODE_DROPDOWN_ID)
		
		screen.addDropDownBoxGFC(self.record(self.MODE_DROPDOWN_ID), self.MODE_DROPDOWN_X, self.MODE_DROPDOWN_Y, self.MODE_DROPDOWN_W, WidgetTypes.WIDGET_GENERAL, -1, -1, FontTypes.GAME_FONT)
		
		ePlayer = gc.getGame().getActivePlayer()
		
		for (szText, eMode) in self.modes:
			szText = localText.getText(szText, ())
			if ((ePlayer > -1) or (eMode != CivilopediaModeTypes.CIVILOPEDIA_MODE_ACTIVE_GAME and eMode != CivilopediaModeTypes.CIVILOPEDIA_MODE_ACTIVE_PLAYER)):
				if (eMode == self.eMode):
					screen.addPullDownString(self.MODE_DROPDOWN_ID, szText, eMode, eMode, True)
				else:
					screen.addPullDownString(self.MODE_DROPDOWN_ID, szText, eMode, eMode, False)
	
	def syncCivilizationDropdown(self, screen):
		if (self.eMode != CivilopediaModeTypes.CIVILOPEDIA_MODE_CIVILIZATION):
			if (self.CIVILIZATION_DROPDOWN_ID in self.widgets):
				screen.deleteWidget(self.CIVILIZATION_DROPDOWN_ID)
				self.widgets.remove(self.CIVILIZATION_DROPDOWN_ID)
			return
		
		if (not self.CIVILIZATION_DROPDOWN_ID in self.widgets):
			self.record(self.CIVILIZATION_DROPDOWN_ID)
		else:
			screen.deleteWidget(self.CIVILIZATION_DROPDOWN_ID)
		
		screen.addDropDownBoxGFC(self.record(self.CIVILIZATION_DROPDOWN_ID), self.CIVILIZATION_DROPDOWN_X, self.CIVILIZATION_DROPDOWN_Y, self.CIVILIZATION_DROPDOWN_W, WidgetTypes.WIDGET_GENERAL, -1, -1, FontTypes.GAME_FONT)
		
		for eCiv in range(gc.getNumCivilizationInfos()):
			szText = gc.getCivilizationInfo(eCiv).getDescription()
			if (eCiv == self.eCivilization):
				screen.addPullDownString(self.CIVILIZATION_DROPDOWN_ID, szText, eCiv, eCiv, True)
			else:
				screen.addPullDownString(self.CIVILIZATION_DROPDOWN_ID, szText, eCiv, eCiv, False)
	
	def handleInput(self, input):
		if (input.getNotifyCode() == NotifyCode.NOTIFY_LISTBOX_ITEM_SELECTED):
			if (input.getFunctionName() == self.MODE_DROPDOWN_ID):
				screen = self.getScreen()
				iSelection = screen.getSelectedPullDownID(self.MODE_DROPDOWN_ID)
				eMode = screen.getPullDownData(self.MODE_DROPDOWN_ID, iSelection)
				self.section.setMode(eMode)
			if (input.getFunctionName() == self.CIVILIZATION_DROPDOWN_ID):
				screen = self.getScreen()
				iSelection = screen.getSelectedPullDownID(self.CIVILIZATION_DROPDOWN_ID)
				eCivilization = screen.getPullDownData(self.CIVILIZATION_DROPDOWN_ID, iSelection)
				self.section.setCivilization(eCivilization)

