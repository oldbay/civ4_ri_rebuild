## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
from CvPythonExtensions import *
import PyHelpers
import CvUtil
import ScreenInput
import CvScreenEnums
import BugDll
import BugUtil
import PlayerUtil
import TradeUtil

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

class CvPopulationAdvisor:

	def __init__(self):
		self.widgets = [set(), set(), set()]
		
		self.WIDGET_SCREEN = 0
		self.WIDGET_PAGE = 1
		self.WIDGET_GRAPH = 2
		
		self.SCREEN_ID = "PopulationAdvisor"
		self.BACKGROUND_ID = "PopulationAdvisorBackground"
		self.TOP_PANEL_ID = "PopulationAdvisorTopPanel"
		self.BOTTOM_PANEL_ID = "PopulationAdvisorBottomPanel"
		self.PLAYER_DROPDOWN_ID =  "PopulationAdvisorPlayerWidget"
		self.WIDGET_ID = "PopulationAdvisorWidget"
		self.TITLE_ID = "PopulationAdvisorWidgetHeader"
		self.TAB_BUTTON_ID = "PopulationAdvisorTabButton"
		self.EXIT_ID = "PopulationAdvisorExitWidget"
		self.MAIN_PANEL_ID = "PopulationAdvisorMainPanel"
		self.METRIC_CLASS_TITLE_ID = "PopulationAdvisorMetricClassTitle"
		self.METRIC_TITLE_ID = "PopulationAdvisorMetricTitle"
		self.METRIC_GRAPH_ID = "PopulationAdvisorMetricGraph"
		self.METRIC_LINE_ID = "PAML"
		self.METRIC_X_AXIS_ID = "PAMXA"
		
		self.PAGE_NAME_LIST = [
			"TXT_KEY_POPULATION_ADVISOR_CITY_TAB",
			"TXT_KEY_POPULATION_ADVISOR_GLOBAL_TAB",
			]
		self.PAGE_DRAW_LIST = [
			self.drawCity,
			self.drawGlobal,
			]
		
		self.ePlayer = -1
		self.iCityID = -1
		self.iPage = 0
		
		self.nWidgetCount = 0
	
	def getScreen(self):
		return CyGInterfaceScreen(self.SCREEN_ID, CvScreenEnums.POPULATION_ADVISOR)
	
	def interfaceScreen(self, ePlayer, iCityID):
		if (ePlayer != -1):
			self.ePlayer = ePlayer
			self.iCityID = iCityID
		else:
			self.ePlayer = CyGame().getActivePlayer()
			self.iCityID = -1
		
		if self.iCityID >= 0:
			self.iPage = 0
		else:
			self.iPage = 1
		
		screen = self.getScreen()
		if screen.isActive():
			return
		screen.setRenderInterfaceOnly(True)
		screen.showScreen(PopupStates.POPUPSTATE_IMMEDIATE, True)
		
		self.calculateLayout()
		
		# Set the background and exit button, and show the screen
		screen.setDimensions(screen.centerX(0), screen.centerY(0), self.SCREEN_W, self.SCREEN_H)
		
		self.draw()
		
		screen.showScreen(PopupStates.POPUPSTATE_IMMEDIATE, True)
	
	def calculateLayout(self):
		screen = self.getScreen()
		
		self.SCREEN_X = 0
		self.SCREEN_Y = 0
		self.SCREEN_W = screen.getXResolution()
		self.SCREEN_H = screen.getYResolution()
		self.SCREEN_W = 1024
		self.SCREEN_H = 768
		
		self.BACKGROUND_X = self.SCREEN_X
		self.BACKGROUND_Y = self.SCREEN_Y
		self.BACKGROUND_W = self.SCREEN_W
		self.BACKGROUND_H = self.SCREEN_H
		self.BACKGROUND_Z = -2.1
		
		self.TOP_PANEL_X = 0
		self.TOP_PANEL_Y = 0
		self.TOP_PANEL_W = self.SCREEN_W
		self.TOP_PANEL_H = 55
		
		self.BOTTOM_PANEL_X = 0
		self.BOTTOM_PANEL_W = self.SCREEN_W
		self.BOTTOM_PANEL_H = 55
		self.BOTTOM_PANEL_Y = self.SCREEN_H - self.BOTTOM_PANEL_H
		
		self.MAIN_PANEL_X = -8
		self.MAIN_PANEL_Y = self.TOP_PANEL_Y + self.TOP_PANEL_H - 13
		self.MAIN_PANEL_W = self.SCREEN_W + 16
		self.MAIN_PANEL_H = self.BOTTOM_PANEL_Y - self.MAIN_PANEL_Y - 15
		
		self.MAIN_PANEL_INTERIOR_W = self.MAIN_PANEL_W - 12
		
		self.TITLE_X = self.SCREEN_W / 2
		self.TITLE_Y = 12
		
		self.EXIT_X = self.SCREEN_W - 30
		self.EXIT_Y = self.SCREEN_H - 42
		
		self.METRIC_CLASS_PANEL_X = 0
		self.METRIC_CLASS_PANEL_W = self.MAIN_PANEL_INTERIOR_W
		
		self.METRIC_CLASS_TITLE_X = 10
		self.METRIC_CLASS_TITLE_Y = 0
		self.METRIC_CLASS_TITLE_DY = 20
		
		self.METRIC_TITLE_X = 20
		self.METRIC_TITLE_Y = 0
		self.METRIC_TITLE_DY = 20
		
		self.METRIC_GRAPH_X = 20
		self.METRIC_GRAPH_Y = 0
		self.METRIC_GRAPH_W = self.MAIN_PANEL_INTERIOR_W - (self.METRIC_GRAPH_X * 2) - 20
		self.METRIC_GRAPH_H = 100
		self.METRIC_GRAPH_HALF_H = self.METRIC_GRAPH_H / 2
		self.METRIC_GRAPH_DY = self.METRIC_GRAPH_H + 10
		
		self.CONTROL_Z = self.BACKGROUND_Z - 0.2
		
		self.TEXT_MARGIN = 15
		self.DZ = -0.2
		
		self.LINK_DX = self.EXIT_X / (len (self.PAGE_NAME_LIST) + 1)
		self.LINK_Y = self.SCREEN_H - 42
	
	def draw(self):
		screen = self.getScreen()
		
		# Background
		screen.addDDSGFC(self.record(self.BACKGROUND_ID, self.WIDGET_SCREEN), ArtFileMgr.getInterfaceArtInfo("MAINMENU_SLIDESHOW_LOAD").getPath(), self.BACKGROUND_X, self.BACKGROUND_Y, self.BACKGROUND_W, self.BACKGROUND_H, WidgetTypes.WIDGET_GENERAL, -1, -1)
		
		# Top Panel
		screen.addPanel(self.record(self.TOP_PANEL_ID, self.WIDGET_SCREEN), u"", u"", True, False, self.TOP_PANEL_X, self.TOP_PANEL_Y, self.TOP_PANEL_W, self.TOP_PANEL_H, PanelStyles.PANEL_STYLE_TOPBAR)
		
		# Bottom Panel
		screen.addPanel(self.record(self.BOTTOM_PANEL_ID, self.WIDGET_SCREEN), u"", u"", True, False, self.BOTTOM_PANEL_X, self.BOTTOM_PANEL_Y, self.BOTTOM_PANEL_W, self.BOTTOM_PANEL_H, PanelStyles.PANEL_STYLE_BOTTOMBAR)
		
		screen.showWindowBackground(False)
		
		# Exit
		screen.setText(self.record(self.EXIT_ID, self.WIDGET_SCREEN), "Background", u"<font=4>" + localText.getText("TXT_KEY_PEDIA_SCREEN_EXIT", ()).upper() + "</font>", CvUtil.FONT_RIGHT_JUSTIFY, self.EXIT_X, self.EXIT_Y, self.CONTROL_Z, FontTypes.TITLE_FONT, WidgetTypes.WIDGET_CLOSE_SCREEN, -1, -1 )
		
		# Header...
		screen.setLabel(self.record(self.TITLE_ID, self.WIDGET_SCREEN), "Background", u"<font=4b>" + localText.getText("TXT_KEY_POPULATION_ADVISOR_TITLE", ()).upper() + u"</font>", CvUtil.FONT_CENTER_JUSTIFY, self.TITLE_X, self.TITLE_Y, self.CONTROL_Z, FontTypes.TITLE_FONT, WidgetTypes.WIDGET_GENERAL, -1, -1)
		
		# Player Selection (Debug Mode)
		if (CyGame().isDebugMode()):
			screen.addDropDownBoxGFC(self.record(self.DEBUG_DROPDOWN_ID, self.WIDGET_SCREEN), 22, 12, 300, WidgetTypes.WIDGET_GENERAL, -1, -1, FontTypes.GAME_FONT)
			for j in range(gc.getMAX_CIV_PLAYERS()):
				if (gc.getPlayer(j).isAlive()):
					screen.addPullDownString(self.szDropdownName, gc.getPlayer(j).getName(), j, j, False)
		self.drawContents()
	
	def drawContents(self):
		screen = self.getScreen()
		
		#self.clear(self.WIDGET_GRAPH)
		#self.clear(self.WIDGET_PAGE)
		
		# draw the current page
		if (self.iPage >= 0 and self.iPage < len(self.PAGE_NAME_LIST)):
			self.PAGE_DRAW_LIST[self.iPage]()
		
		# Link to other economics advisor pages...
		iX = self.LINK_DX / 2
		
		for i in range (len(self.PAGE_NAME_LIST)):
			szTextId = self.TAB_BUTTON_ID+str(i)
			if (self.iPage != i):
				screen.setText(self.record(szTextId, self.WIDGET_SCREEN), "", u"<font=4>" + localText.getText (self.PAGE_NAME_LIST[i], ()).upper() + u"</font>", CvUtil.FONT_CENTER_JUSTIFY, iX, self.LINK_Y, 0, FontTypes.TITLE_FONT, WidgetTypes.WIDGET_GENERAL, i, -1)
			else:
				screen.setText(self.record(szTextId, self.WIDGET_SCREEN), "", u"<font=4>" + localText.getColorText (self.PAGE_NAME_LIST[i], (), gc.getInfoTypeForString ("COLOR_YELLOW")).upper() + u"</font>", CvUtil.FONT_CENTER_JUSTIFY, iX, self.LINK_Y, 0, FontTypes.TITLE_FONT, WidgetTypes.WIDGET_GENERAL, -1, -1)
			iX += self.LINK_DX
	
	def drawCity(self):
		screen = self.getScreen()
		player = gc.getPlayer(self.ePlayer)
		city = player.getCity(self.iCityID)
		
		if city == None:
			return
		
		# Header...
		#screen.setLabel(self.TITLE_ID, "Background", u"<font=4b>" + localText.getText("TXT_KEY_ECONOMICS_ADVISOR_FINANCE_TAB", ()).upper() + u"</font>", CvUtil.FONT_CENTER_JUSTIFY, self.SCREEN_X, self.TITLE_Y, self.CONTROL_Z, FontTypes.TITLE_FONT, WidgetTypes.WIDGET_GENERAL, -1, -1)
		
		screen.setLabel(self.record(self.TITLE_ID, self.WIDGET_SCREEN), "Background", u"<font=4b>" + city.getName().upper() + u" " + localText.getText("TXT_KEY_POPULATION_ADVISOR_TITLE", ()).upper() + u"</font>", CvUtil.FONT_CENTER_JUSTIFY, self.TITLE_X, self.TITLE_Y, self.CONTROL_Z, FontTypes.TITLE_FONT, WidgetTypes.WIDGET_GENERAL, -1, -1)
		
		
		# PANEL_STYLE_BLUE50
		# PANEL_STYLE_IN
		screen.addScrollPanel(self.record(self.MAIN_PANEL_ID, self.WIDGET_PAGE), u"", self.MAIN_PANEL_X, self.MAIN_PANEL_Y, self.MAIN_PANEL_W, self.MAIN_PANEL_H, PanelStyles.PANEL_STYLE_EMPTY)
		#screen.setStyle(self.MAIN_PANEL_ID, "Panel_Black25_Style")
		
		iY = 0
		
		iGameTurn = CyGame().getGameTurn()
		#iStartTurn = city.getGameTurnFounded()
		iStartTurn = CyGame().getStartTurn()
		iTotalTurns = iGameTurn - iStartTurn
		
		fScaleX = float(self.METRIC_GRAPH_W) / float(iTotalTurns)
		fScaleY = float(self.METRIC_GRAPH_H) / 4000.0
		
		eAxisColor = gc.getInfoTypeForString("COLOR_BLACK")
		eLineColor = gc.getInfoTypeForString("COLOR_PLAYER_ORANGE")
		
		if (iTotalTurns > 1):
			for eMetricClass in range(gc.getNumMetricClassInfos()):
				kMetricClass = gc.getMetricClassInfo(eMetricClass)
				if kMetricClass.getNumMetrics() > 0:
					#screen.setLabelAt(self.record(self.METRIC_CLASS_TITLE_ID + str(eMetricClass), self.WIDGET_PAGE), self.MAIN_PANEL_ID,  u"<font=3>" + kMetricClass.getDescription() + u" [" + str(kMetricClass.getNumMetrics()) + u"/" + str(gc.getNumMetricInfos()) + u"]" + u"</font>", CvUtil.FONT_LEFT_JUSTIFY, self.METRIC_CLASS_TITLE_X, self.METRIC_CLASS_TITLE_Y + iY, self.CONTROL_Z, FontTypes.GAME_FONT, WidgetTypes.WIDGET_GENERAL, -1, -1)
					screen.setLabelAt(self.record(self.METRIC_CLASS_TITLE_ID + str(eMetricClass), self.WIDGET_PAGE), self.MAIN_PANEL_ID,  u"<font=3>" + kMetricClass.getDescription().upper() + u"</font>", CvUtil.FONT_LEFT_JUSTIFY, self.METRIC_CLASS_TITLE_X, self.METRIC_CLASS_TITLE_Y + iY, self.CONTROL_Z, FontTypes.GAME_FONT, WidgetTypes.WIDGET_GENERAL, -1, -1)
					iY += self.METRIC_CLASS_TITLE_DY
					for iMem in range(kMetricClass.getNumMetrics()):
						eMetric = kMetricClass.getMetrics(iMem)
						kMetric = gc.getMetricInfo(eMetric)
						szMetricID = self.METRIC_GRAPH_ID + str(eMetric)
						
						# Draw Label
						if (kMetric.getDescription() != kMetricClass.getDescription()):
							screen.setLabelAt(self.record(self.METRIC_TITLE_ID + str(eMetric), self.WIDGET_PAGE), self.MAIN_PANEL_ID,  u"<font=3>" + kMetric.getDescription() + u"</font>", CvUtil.FONT_LEFT_JUSTIFY, self.METRIC_TITLE_X, self.METRIC_TITLE_Y + iY, self.CONTROL_Z, FontTypes.GAME_FONT, WidgetTypes.WIDGET_GENERAL, -1, -1)
							iY += self.METRIC_TITLE_DY
						
						# Draw Graph Canvas
						screen.addDrawControlAt(self.record(szMetricID, self.WIDGET_GRAPH), self.MAIN_PANEL_ID, ArtFileMgr.getInterfaceArtInfo("SCREEN_BG").getPath(), self.METRIC_GRAPH_X, self.METRIC_GRAPH_Y + iY, self.METRIC_GRAPH_W, self.METRIC_GRAPH_H, WidgetTypes.WIDGET_GENERAL, -1, -1)
						iY += self.METRIC_GRAPH_DY
						
						# Draw Graph Y Axis
						screen.addLineGFC(szMetricID, self.record(self.METRIC_X_AXIS_ID + str(eMetric), self.WIDGET_GRAPH), 0, self.METRIC_GRAPH_HALF_H, self.METRIC_GRAPH_W, self.METRIC_GRAPH_HALF_H, eAxisColor)
						
						# Draw Graph Lines
						iLastValue = city.getMetricTurnValue(eMetric, iStartTurn)
						for iI in range(iTotalTurns - 1):
							iTurn = iStartTurn + iI + 1
							iValue = city.getMetricTurnValue(eMetric, iTurn)
							if not iValue == 0 or not iLastValue == 0:
								x0 = int((iTurn - 1) * fScaleX)
								x1 = int(iTurn * fScaleX)
								y0 = self.METRIC_GRAPH_HALF_H - int(iLastValue * fScaleY)
								y1 = self.METRIC_GRAPH_HALF_H - int(iValue * fScaleY)
								screen.addLineGFC(szMetricID, self.record(self.METRIC_LINE_ID + str(eMetric) + "-" + str(iI), self.WIDGET_GRAPH), x0, y0, x1, y1, eLineColor)
							iLastValue = iValue
	
	def drawGlobal(self):
		screen = self.getScreen()
		
		# Header...
		screen.setLabel(self.TITLE_ID, "Background", u"<font=4b>" + localText.getText("TXT_KEY_ECONOMICS_ADVISOR_ENVIRONMENT_TAB", ()).upper() + u"</font>", CvUtil.FONT_CENTER_JUSTIFY, self.SCREEN_X, self.TITLE_Y, self.CONTROL_Z, FontTypes.TITLE_FONT, WidgetTypes.WIDGET_GENERAL, -1, -1)
		
		game = CyGame()
		player = gc.getPlayer(self.ePlayer)
		
		for city in PlayerUtil.playerCities(player):
			return
	
	# Will handle the input for this screen...
	def handleInput (self, inputClass):
		'Calls function mapped in FinanceAdvisorInputMap'
		iNotifyCode = inputClass.getNotifyCode()
		if (iNotifyCode == NotifyCode.NOTIFY_LISTBOX_ITEM_SELECTED):
			screen = self.getScreen()
			iIndex = screen.getSelectedPullDownID(self.DEBUG_DROPDOWN_ID)
			self.ePlayer = screen.getPullDownData(self.DEBUG_DROPDOWN_ID, iIndex)
			self.drawContents()
		elif (iNotifyCode == NotifyCode.NOTIFY_CLICKED):
			# There are only a few clickable widgets in this screen, so lets just assume that if it was a "general" button,
			# then it was one of the change-tab buttons. (or exit)
			if (inputClass.getButtonType() == WidgetTypes.WIDGET_GENERAL):
				iData1 = inputClass.getData1()
				if (iData1 != self.iPage and iData1 >= 0 and iData1 < len(self.PAGE_NAME_LIST)):
					self.iPage = iData1
					self.drawContents()
		return 0
	
	def update(self, fDelta):
		if (CyInterface().isDirty(InterfaceDirtyBits.Financial_Screen_DIRTY_BIT) == True):
			CyInterface().setDirty(InterfaceDirtyBits.Financial_Screen_DIRTY_BIT, False)
			self.drawContents()
		return
	
	def record(self, szWidgetID, iBucket):
		self.widgets[iBucket].add(szWidgetID)
		return szWidgetID
	
	def clear(self, iBucket):
		screen = self.getScreen()
		for szWidgetID in self.widgets[iBucket]:
			screen.deleteWidget(szWidgetID)
		self.widgets[iBucket].clear()
	
	def isEmpty(self, iBucket):
		return len(self.widgets[iBucket]) == 0
