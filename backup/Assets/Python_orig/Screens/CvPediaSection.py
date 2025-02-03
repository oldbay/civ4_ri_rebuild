## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005

from CvPythonExtensions import *
import CvPediaScreen
import CvUtil

from BugUtil import Timer

# globals
gc = CyGlobalContext()
localText = CyTranslator()

class CvPediaSection(CvPediaScreen.CvPediaScreen):
	"Civilopedia Section Base"

	def __init__(self, main, eSection, szTitle, szWidget):
		super(CvPediaSection, self).__init__()
		
		self.main = main
		self.eSection = eSection
		self.szTitle = szTitle
		self.szWidget = szWidget
		
		self.views = []
		self.pages = {}
		self.view = None
		self.page = None
		
		self.szSearchString = u""
		self.bPlayableOnly = True
		
		self.VIEW_PANE_ID = "Pedia" + self.szWidget + "ViewPane"
	
	def update(self, fDelta):
		self.view.update(fDelta)
		self.page.update(fDelta)
	
	def pediaClearSearchFilter(self):
		if (len(self.szSearchString) > 0):
			self.szSearchString = u""
			self.view.syncSearch()
	
	def pediaTogglePlayableFilter(self):
		self.bPlayableOnly = not self.bPlayableOnly
		self.page.updatePlayableOnlyFilter(self.bPlayableOnly)
	
	def pediaAction(self, eView = CivilopediaViewTypes.NO_CIVILOPEDIA_VIEW, iViewData1 = -1, iViewData2 = -1, ePage = CivilopediaPageTypes.NO_CIVILOPEDIA_PAGE, iPageData1 = -1, iPageData2 = -1):
		if (eView != CivilopediaViewTypes.NO_CIVILOPEDIA_VIEW):
			view = self.getView(eView, iViewData1, iViewData2)
			if (view != None):
				self.setView(view, iViewData1, iViewData2)
				if (ePage == CivilopediaPageTypes.NO_CIVILOPEDIA_PAGE):
					(iPageData1, iPageData2) = self.page.getData()
					if (not self.view.matchesPage(self.page.ePage, iPageData1, iPageData2)):
						page = self.getPage(view.ePage, view.iPageData1, view.iPageData2)
						if (page != None):
							self.setPage(page, view.iPageData1, view.iPageData2)
		
		if (ePage != CivilopediaPageTypes.NO_CIVILOPEDIA_PAGE):
			page = self.getPage(ePage, iPageData1, iPageData2)
			if (page != None):
				self.setPage(page, iPageData1, iPageData2)
				if (not self.view.matchesPage(ePage, iPageData1, iPageData2)):
					view = self.getViewForPage(ePage, iPageData1, iPageData2)
					if (view != None):
						self.setView(view) # TODO: Determine correct view data
				self.view.setItem(iPageData1, iPageData2)
		
		self.calculateLayoutHierarchy()
		self.drawHierarchy()
		#CvUtil.pyPrint(self.szWidget + " Section: " + str(eView))
		
		#self.getScreen().setFocus(self.VIEW_PANE_ID)
		
		"""
		if (ePage != CivilopediaPageTypes.NO_CIVILOPEDIA_PAGE):
			if (ePage in self.pages):
				if (self.activePage != None):
					self.activePage.clear()
				self.ePage = ePage
				self.activePage = self.pages[ePage]
		
		self.calculateLayout()
		
		self.updateViewPane()
		self.updateSearchPane()
		self.updateItemPane()
		"""
	
	def getTitle(self):
		return localText.getText(self.szTitle, ())
	
	def setView(self, view, iViewData1 = -1, iViewData2 = -1):
		if (view != self.view):
			self.view.clear()
			self.view = view
			if (iViewData1 != -1):
				self.view.setData(iViewData1, iViewData2)
			if (not self.isEmpty()):
				self.updateViewPane()
		
		"""
		if (eView == self.main.eView and iViewData1 == self.main.iViewData1 and iViewData2 == self.main.iViewData2):
			return
		
		self.clear()
		
		(szViewTitle, eView, iViewData1, ePage, eWidget, items) = self.getViewProperties(eView, iViewData1, iViewData2)
		self.main.eView = eView
		self.main.iViewData1 = iViewData1
		self.main.iViewData2 = iViewData2
		
		#if (not ePage in self.pages):
		#	# TODO: Set the default page
			
		#if (self.isEmpty()):
		#	self.calculateLayout()
		
		#self.updateViewPane()
		# TODO: Wipe out the old search text?
		#self.updateSearchPane()
		
		# TODO: Determine whether the current selection is still valid and keep it if it is
		
		# TODO: Only set the default page if this is a different section
		CvUtil.pyPrint(self.szWidget + " Section: View set to " + szViewTitle + " / " + str(iViewData1) + " / " + str(iViewData2))
		(szItemTitle, szButton, iPageData1, iPageData2) = items[0]
		self.setActivePage(ePage, iPageData1, iPageData2)
		"""
	
	def setPage(self, page, iPageData1, iPageData2):
		if (page != self.page):
			self.page.clear()
			self.page = page
		
		if (iPageData1 != -1):
			self.page.setData(iPageData1, iPageData2)
		
		"""
		if (ePage == self.main.ePage and iPageData1 == self.main.iPageData1 and iPageData2 == self.main.iPageData2):
			return
		
		if (not ePage in self.pages):
			CvUtil.pyPrint(self.szWidget + " Section: Page not found: " + str(ePage))
			return
		
		# Switch to a valid view for the page if necessary
		eView = CivilopediaViewTypes.NO_CIVILOPEDIA_VIEW
		iViewData = -1
		for (szViewTitle, eCandidateView, iCandidateViewData, eCandidatePage, eWidget, items) in self.views:
			if (eCandidatePage == ePage):
				bValid = False
				if (iPageData1 == -1):
					bValid = True
				else:
					for (szItemTitle, szButton, iCandidatePageData1, iCandidatePageData2) in items:
						if (iCandidatePageData1 == iPageData1 and iCandidatePageData2 == iPageData2):
							bValid = True
							break
				if (bValid):
					if (eView == CivilopediaViewTypes.NO_CIVILOPEDIA_VIEW or (eCandidateView == self.main.eView and iCandidateViewData == self.main.iViewData1)):
						eView = eCandidateView
						iViewData = iCandidateViewData
		
		if (eView == CivilopediaViewTypes.NO_CIVILOPEDIA_VIEW):
			CvUtil.pyPrint(self.szWidget + " Section: Suitable view not found: " + str(ePage))
			return
		
		# Set the active view
		if (eView != self.main.eView):
			self.main.eView = eView
			self.main.iViewData1 = iViewData
			self.main.iViewData2 = -1 # TODO: Evaluate this
			CvUtil.pyPrint(self.szWidget + " Section: Reconciled view for page")
		
		# Set the active page
		if (ePage != self.main.ePage):
			if (self.main.activePage != None):
				self.main.activePage.clear()
			self.main.ePage = ePage
			self.main.activePage = self.pages[ePage]
		
		CvUtil.pyPrint(self.szWidget + " Section: Page set to " + str(ePage) + " / " + str(iPageData1) + " / " + str(iPageData2))
		
		if (self.isEmpty()):
			self.calculateLayout()
			
			self.updateViewPane()
			self.updateSearchPane()
			self.updateItemPane()
		
		self.main.activePage.pediaAction(iPageData1, iPageData2)
		"""
	
	def getView(self, eView, iViewData1, iViewData2):
		for view in self.views:
			if (view.matchesView(eView, iViewData1, iViewData2)):
				return view
		return None
	
	def getViewForPage(self, ePage, iPageData1, iPageData2):
		for view in self.views:
			if (view.matchesPage(ePage, iPageData1, iPageData2)):
				return view
		return None
	
	def getPage(self, ePage, iPageData1, iPageData2):
		if (ePage in self.pages):
			return self.pages[ePage]
		else:
			return None
	
	def setMode(self, eMode):
		for view in self.views:
			view.setMode(eMode)
		
		for ePage in self.pages:
			self.pages[ePage].setMode(eMode)
	
	def setCivilization(self, eCivilization):
		for view in self.views:
			view.setCivilization(eCivilization)
		
		for ePage in self.pages:
			self.pages[ePage].setCivilization(eCivilization)
	
	def updatePlayerContext(self):
		for view in self.views:
			view.updatePlayerContext()
		
		for ePage in self.pages:
			self.pages[ePage].updatePlayerContext()
	
	def setDefaults(self):
		self.view = self.views[0]
		self.page = self.getPage(self.view.ePage, self.view.iPageData1, self.view.iPageData2)
		self.page.setData(self.view.iPageData1, self.view.iPageData2)
	
	def clearHierarchy(self):
		self.clear()
		self.view.clear()
		self.page.clear()
	
	def calculateLayoutHierarchy(self):
		self.calculateLayout()
		self.view.calculateLayout()
		self.page.calculateLayout()
	
	def calculateLayout(self):
		self.VIEW_PANE_X = self.main.SECTION_AREA_X
		self.VIEW_PANE_Y = self.main.SECTION_AREA_Y
		if (self.main.SECTION_AREA_W >= 1380):
			self.VIEW_PANE_W = 400
		elif (self.main.SECTION_AREA_W >= 1260):
			self.VIEW_PANE_W = 245
		else:
			self.VIEW_PANE_W = 225
		self.VIEW_PANE_H = len(self.views) * 28 + 2
		
		self.main.VIEW_AREA_X = self.main.SECTION_AREA_X
		self.main.VIEW_AREA_Y = self.VIEW_PANE_Y + self.VIEW_PANE_H
		self.main.VIEW_AREA_W = self.VIEW_PANE_W
		self.main.VIEW_AREA_H = self.main.SECTION_AREA_H - self.VIEW_PANE_H
		
		self.main.OUTER_PAGE_AREA_X = self.main.SECTION_AREA_X + self.VIEW_PANE_W
		self.main.OUTER_PAGE_AREA_Y = self.main.SECTION_AREA_Y
		self.main.OUTER_PAGE_AREA_W = self.main.SECTION_AREA_W - self.VIEW_PANE_W
		self.main.OUTER_PAGE_AREA_H = self.main.SECTION_AREA_H
		
		self.main.PAGE_AREA_X = self.main.OUTER_PAGE_AREA_X + self.main.COMMON_MARGIN_W
		self.main.PAGE_AREA_Y = self.main.OUTER_PAGE_AREA_Y
		self.main.PAGE_AREA_W = self.main.OUTER_PAGE_AREA_W - self.main.COMMON_MARGIN_W * 2
		self.main.PAGE_AREA_H = self.main.OUTER_PAGE_AREA_H
	
	def drawHierarchy(self):
		self.draw()
		self.view.draw()
		self.page.draw()
	
	def draw(self):
		if (not self.isEmpty()):
			return
		
		screen = self.getScreen()
		
		self.updateViewPane()
	
	def updateViewPane(self):
		screen = self.getScreen()
		
		if (not self.VIEW_PANE_ID in self.widgets):
			screen.addListBoxGFC(self.record(self.VIEW_PANE_ID), "", self.VIEW_PANE_X, self.VIEW_PANE_Y, self.VIEW_PANE_W, self.VIEW_PANE_H, TableStyles.TABLE_STYLE_STANDARD)
			screen.enableSelect(self.VIEW_PANE_ID, True)
			screen.setStyle(self.VIEW_PANE_ID, "Table_StandardCiv_Style")
			
			for view in self.views:
				(iViewData1, iViewData2) = view.getData()
				screen.appendListBoxStringNoUpdate(self.VIEW_PANE_ID, view.getTitle(), WidgetTypes.WIDGET_PEDIA_VIEW, view.eView, iViewData1, CvUtil.FONT_LEFT_JUSTIFY)
			
			screen.updateListBox(self.VIEW_PANE_ID)
		
		i = 0
		for view in self.views:
			if (self.view == view):
				screen.setSelectedListBoxStringGFC(self.VIEW_PANE_ID, i)
				break
			i += 1
	
	def handleInput(self, input):
		if (input.getNotifyCode() == NotifyCode.NOTIFY_CHARACTER):
			if (input.getData() == int(InputTypes.KB_RETURN)):
				CvUtil.pyPrint("RETURN!")
				return 0
			elif (input.isKeydown() and not input.isCtrlKeyDown() and not input.isAltKeyDown()):
				screen = self.getScreen()
				if (screen.isActive()):
					szUpdatedSearchString = input.modifyString(self.szSearchString)
					if (self.szSearchString != szUpdatedSearchString):
						self.szSearchString = szUpdatedSearchString
						self.view.syncSearch()
						#self.updateSearchPane()
						#self.updateItemPane()
						return 1
		
		self.view.handleInput(input)
		self.page.handleInput(input)
