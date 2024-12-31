## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
from CvPythonExtensions import *
import CvUtil
import ScreenInput
import CvScreenEnums
import CvScreensInterface
import math
import PyTechGraph

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

# BUG - GP Tech Prefs - start
import TechPrefs
import BugCore
BugOpt = BugCore.game.Advisors
ClockOpt = BugCore.game.NJAGC

import BugUtil

FLAVORS = [ TechPrefs.FLAVOR_PRODUCTION, TechPrefs.FLAVOR_GOLD, TechPrefs.FLAVOR_SCIENCE,
			TechPrefs.FLAVOR_CULTURE, TechPrefs.FLAVOR_RELIGION ]
UNIT_CLASSES = [ "UNITCLASS_ENGINEER", "UNITCLASS_MERCHANT", "UNITCLASS_SCIENTIST",
				 "UNITCLASS_ARTIST", "UNITCLASS_PROPHET" ]
# BUG - GP Tech Prefs - end

# BUG - 3.19 No Espionage - start
import GameUtil
# BUG - 3.19 No Espionage - end

# BUG - Mac Support - start
BugUtil.fixSets(globals())
# BUG - Mac Support - end

# BUG - Tech Era Colors - start
def getEraDescription(eWidgetType, iData1, iData2, bOption):
	return gc.getEraInfo(iData1).getDescription()
# BUG - Tech Era Colors - end

# BUG - GP Tech Prefs - start
def resetTechPrefs(args=[]):
	CvScreensInterface.techChooser.resetTechPrefs()

def getAllTechPrefsHover(widgetType, iData1, iData2, bOption):
	#return buildTechPrefsHover("TXT_KEY_BUG_TECH_PREFS_ALL", CvScreensInterface.techChooser.pPrefs.getAllFlavorTechs(iData1))
	return buildTechPrefsHover("TXT_KEY_BUG_TECH_PREFS_ALL", CvScreensInterface.techChooser.pPrefs.getRemainingFlavorTechs(iData1)) # K-Mod

def getCurrentTechPrefsHover(widgetType, iData1, iData2, bOption):
	return buildTechPrefsHover("TXT_KEY_BUG_TECH_PREFS_CURRENT", CvScreensInterface.techChooser.pPrefs.getCurrentFlavorTechs(iData1))

def getFutureTechPrefsHover(widgetType, iData1, iData2, bOption):
	pPlayer = gc.getPlayer(iData2)
	sTechs = set()
	for i in range(gc.getNumTechInfos()):
		if (pPlayer.isResearchingTech(i)):
			sTechs.add(CvScreensInterface.techChooser.pPrefs.getTech(i))
	return buildTechPrefsHover("TXT_KEY_BUG_TECH_PREFS_FUTURE", CvScreensInterface.techChooser.pPrefs.getCurrentWithFlavorTechs(iData1, sTechs))

def buildTechPrefsHover(key, lTechs):
	szText = BugUtil.getPlainText(key) + "\n"
	for pTech in lTechs:
		szText += "<img=%s size=24></img>" % pTech.getInfo().getButton().replace(" ", "_")
	return szText
# BUG - GP Tech Prefs - end

class CvTechChooser:
	"Tech Chooser Screen"
	
	def __init__(self):
		self.szWidget = "TechChooser"
		
		self.graph = PyTechGraph.PyTechGraph(self, self.szWidget)
		
		self.widgets = set()
		
		self.PLAYER_DROPDOWN_ID = self.szWidget + "PlayerDropDown"
		self.ADD_TECH_BUTTON_ID = self.szWidget + "AddTechButton"
		self.TOP_PANEL_ID = self.szWidget + "TopPanel"
		self.BG_ID = self.szWidget + "BG"
		self.BOTTOM_PANEL_ID = self.szWidget + "BottomPanel"
		self.EXIT_BUTTON_ID = self.szWidget + "ExitButton"
		self.TITLE_ID = self.szWidget + "Title"
		self.GP_TECH_PREF_PANEL_ID = self.szWidget + "GPPrefPanel"
		self.AS_POINTS_LABEL_ID = self.szWidget + "ASPointsLabel"
		self.SELECTED_TECH_LABEL_ID = self.szWidget + "SelectedTechLabel"
		
		# Advanced Start
		self.m_eSelectedTech = -1
		self.m_bSelectedTechDirty = False
		self.m_bTechRecordsDirty = False
		
# BUG - GP Tech Prefs - start
		self.bPrefsShowing = False
		self.resetTechPrefs()
# BUG - GP Tech Prefs - end
		
		self.timer = BugUtil.Timer("CvTechChooser")
	
	def interfaceScreen(self):
		BugUtil.debug("CvTechChooser: interfaceScreen")
		
		if (CyGame().isPitbossHost()):
			return
		
		if (CyGame().getActivePlayer() == -1):
			return
		
		screen = self.getScreen()
		
		screen.showScreen(PopupStates.POPUPSTATE_IMMEDIATE, False)
		
		self.cacheArtInfo()

		if (self.isEmpty() or not screen.isPersistent()):
			self.clear()
			self.createScreen()
		else:
			self.updateScreen(False)
	
	def update(self, fDelta):
		if (CyGame().isPitbossHost()):
			return
		
		if (CyInterface().isDirty(InterfaceDirtyBits.Advanced_Start_DIRTY_BIT)):
			CyInterface().setDirty(InterfaceDirtyBits.Advanced_Start_DIRTY_BIT, False)
			
			if (self.m_bSelectedTechDirty):
				self.m_bSelectedTechDirty = False
				self.updateControls()
			
			if (self.m_bTechRecordsDirty):
				self.m_bTechRecordsDirty = False
				self.graph.refresh()
	
	def updatePlayerContext(self):
		if (CyGame().isPitbossHost()):
			return
	
		self.m_eSelectedTech = -1
		self.clear()
	
	def cacheArtInfo(self):
		# BUG - GP Tech Prefs - start
		self.NO_TECH_ART = ArtFileMgr.getInterfaceArtInfo("INTERFACE_BUTTONS_CANCEL").getPath()
		# BUG - GP Tech Prefs - end

	def createScreen(self):
		BugUtil.debug("CvTechChooser: createScreen")
		
		self.graph.setPlayer(CyGame().getActivePlayer())
		
		screen = self.getScreen()
		
		screen.setRenderInterfaceOnly(True)
		screen.showWindowBackground(False)
		screen.setPersistent(True)
		
		self.calculateLayout()
		
		screen.setDimensions((screen.getXResolution() - self.SCREEN_W) / 2, screen.centerY(0), self.SCREEN_W, self.SCREEN_H)
		
		self.bPrefsShowing = self.isShowGPTechPrefs()
		
		self.createControls()
		self.updateControls()
		self.graph.calculateLayout()
		self.graph.draw()
	
	def updateScreen(self, bRedraw):
		BugUtil.debug("CvTechChooser: updateScreen")
		
		bRefresh = True
		
		if (self.graph.getCivilization() != CyGame().getActiveCivilizationType() or self.graph.getPlayer() != CyGame().getActivePlayer()):
			# The active player has changed
			# Note: Setting a new player value will force a redraw of the graph
			self.graph.setPlayer(CyGame().getActivePlayer())
			bRedraw = False
			bRefresh = False
		
		if (self.bPrefsShowing != self.isShowGPTechPrefs()):
			self.calculateLayout()
			self.graph.resize()
		elif (bRedraw):
			self.graph.redraw()
		elif (bRefresh):
			# The tech graph matches our last display values for player and civilization
			# so we don't need to redraw everything
			self.graph.refresh()
		
		self.updateControls()
	
	def calculateLayout(self):
		BugUtil.debug("CvTechChooser: calculateLayout")
		
		screen = self.getScreen()
		
		self.TOP_PANEL_H = 45
		self.BOTTOM_PANEL_H = 45
		
		self.GP_TECH_PREF_PANEL_X = 0
		self.GP_TECH_PREF_PANEL_W = 80
		self.GP_TECH_PREF_PANEL_H = 80
		
		self.PREF_ICON_SIZE = 24
		self.PREF_ICON_TOP = 168
		self.PREF_ICON_LEFT = 10
		
		self.SCROLLBAR_DIAMETER = 16
		self.BAR_SHADOW_DIAMETER = 10
		self.SCROLL_PANEL_OFFSET = 8
		
		self.SCREEN_W = screen.getXResolution()
		self.SCREEN_H = 768
		
		if (self.SCREEN_W > 1024):
			self.SCREEN_W -= 60
		
		self.TECH_GRAPH_X = 0
		self.TECH_GRAPH_W = self.SCREEN_W
		
		if (self.isShowGPTechPrefs()):
			self.TECH_GRAPH_X += self.GP_TECH_PREF_PANEL_W
			self.TECH_GRAPH_W -= self.GP_TECH_PREF_PANEL_W
		
		self.TECH_GRAPH_X -= self.SCROLL_PANEL_OFFSET
		self.TECH_GRAPH_W += self.SCROLL_PANEL_OFFSET * 2
		
		self.TECH_GRAPH_Y = self.TOP_PANEL_H - self.BAR_SHADOW_DIAMETER
		self.TECH_GRAPH_H = self.SCREEN_H - (self.TOP_PANEL_H - self.BAR_SHADOW_DIAMETER) - (self.BOTTOM_PANEL_H - self.BAR_SHADOW_DIAMETER) - self.SCROLL_PANEL_OFFSET * 3
		
		self.BG_Y = self.TOP_PANEL_H - 5
		self.BG_H = self.SCREEN_H - self.TOP_PANEL_H + 5 - self.BOTTOM_PANEL_H + 10
		
		self.TITLE_X = self.SCREEN_W / 2 # Note: Text is center-justified
		self.TITLE_Y = (self.TOP_PANEL_H - self.BAR_SHADOW_DIAMETER - 24) / 2

		self.SELECTOR_X = 22
		self.SELECTOR_Y = (self.TOP_PANEL_H - self.BAR_SHADOW_DIAMETER - 26) / 2
		self.SELECTOR_W = 220
		
		self.ADD_TECH_X = 30
		self.ADD_TECH_Y = self.SCREEN_H - (self.BOTTOM_PANEL_H - self.BAR_SHADOW_DIAMETER) + (self.BOTTOM_PANEL_H - self.BAR_SHADOW_DIAMETER - 30) / 2
		self.ADD_TECH_W = 150
		self.ADD_TECH_H = 30
		
		self.ADVANCED_START_POINTS_X = self.ADD_TECH_X + self.ADD_TECH_W + 20
		self.ADVANCED_START_POINTS_Y = self.ADD_TECH_Y
		
		self.SELECTED_TECH_X = self.ADVANCED_START_POINTS_X + 250
		self.SELECTED_TECH_Y = self.ADVANCED_START_POINTS_Y
		
		self.EXIT_X = self.SCREEN_W - 30
		self.EXIT_Y = self.SCREEN_H - (self.BOTTOM_PANEL_H - self.BAR_SHADOW_DIAMETER) + (self.BOTTOM_PANEL_H - self.BAR_SHADOW_DIAMETER - 30) / 2
	
	def createControls(self):
		BugUtil.debug("CvTechChooser: createControls")
		
		screen = self.getScreen()
		
		# Graphics
		screen.addPanel(self.record(self.TOP_PANEL_ID), u"", u"", True, False, 0, 0, self.SCREEN_W, self.TOP_PANEL_H, PanelStyles.PANEL_STYLE_TOPBAR)
		screen.addDDSGFC(self.record(self.BG_ID), ArtFileMgr.getInterfaceArtInfo("SCREEN_BG_OPAQUE").getPath(), 0, self.BG_Y, self.SCREEN_W, self.BG_H, WidgetTypes.WIDGET_GENERAL, -1, -1)
		screen.addPanel(self.record(self.BOTTOM_PANEL_ID), u"", u"", True, False, 0, self.SCREEN_H - self.BOTTOM_PANEL_H, self.SCREEN_W, self.BOTTOM_PANEL_H, PanelStyles.PANEL_STYLE_BOTTOMBAR)
		
		# Display a player dropdown when accessed in-game with debug mode
		screen.addDropDownBoxGFC(self.record(self.PLAYER_DROPDOWN_ID), self.SELECTOR_X, self.SELECTOR_Y, self.SELECTOR_W, WidgetTypes.WIDGET_GENERAL, -1, -1, FontTypes.SMALL_FONT)
		screen.hide(self.PLAYER_DROPDOWN_ID)
		
		# Advanced Start
		szText = localText.getText("TXT_KEY_WB_AS_ADD_TECH", ())
		screen.setButtonGFC(self.record(self.ADD_TECH_BUTTON_ID), szText, "", self.ADD_TECH_X, self.ADD_TECH_Y, self.ADD_TECH_W, self.ADD_TECH_H, WidgetTypes.WIDGET_GENERAL, -1, -1, ButtonStyles.BUTTON_STYLE_STANDARD)
		screen.hide(self.ADD_TECH_BUTTON_ID)
		self.record(self.AS_POINTS_LABEL_ID)
		self.record(self.SELECTED_TECH_LABEL_ID)
		
		# Exit
		screen.setText(self.record(self.EXIT_BUTTON_ID), "Background", u"<font=4>" + CyTranslator().getText("TXT_KEY_PEDIA_SCREEN_EXIT", ()).upper() + "</font>", CvUtil.FONT_RIGHT_JUSTIFY, self.EXIT_X, self.EXIT_Y, 0, FontTypes.TITLE_FONT, WidgetTypes.WIDGET_CLOSE_SCREEN, -1, -1)
		screen.setActivation(self.EXIT_BUTTON_ID, ActivationTypes.ACTIVATE_MIMICPARENTFOCUS)
		
		# Header...
		szText = u"<font=4>"
		szText = szText + localText.getText("TXT_KEY_TECH_CHOOSER_TITLE", ()).upper()
		szText = szText + u"</font>"
		screen.setLabel(self.record(self.TITLE_ID), "Background", szText, CvUtil.FONT_CENTER_JUSTIFY, self.TITLE_X, self.TITLE_Y, 0, FontTypes.TITLE_FONT, WidgetTypes.WIDGET_GENERAL, -1, -1)
		
# BUG - GP Tech Prefs - start
		screen.addPanel(self.record(self.GP_TECH_PREF_PANEL_ID), u"", u"", True, False, 0, 51, 80, self.SCREEN_H - 95, PanelStyles.PANEL_STYLE_MAIN_WHITE )
		screen.hide(self.GP_TECH_PREF_PANEL_ID)
# BUG - GP Tech Prefs - end
		
		#screen.moveToFront(self.PLAYER_DROPDOWN_ID)
		#screen.moveToFront(self.ADD_TECH_BUTTON_ID)
		
		# Add the Highlight
		#screen.addDDSGFC( "TechHighlight", ArtFileMgr.getInterfaceArtInfo("TECH_HIGHLIGHT").getPath(), 0, 0, self.getXStart() + 6, 12 + ( self.BOX_INCREMENT_HEIGHT * self.PIXEL_INCREMENT ), WidgetTypes.WIDGET_GENERAL, -1, -1 )
		#screen.hide( "TechHighlight" )
	
	def updateControls(self):
		BugUtil.debug("CvTechChooser: updateControls")
		
		screen = self.getScreen()
		
		if (CyGame().getActivePlayer() != -1 and CyGame().isDebugMode()):
			# Display a player dropdown when accessed in-game with debug mode
			BugUtil.debug("CvTechChooser: Debug ON")
			# Always redraw dropdown in case a different player has been selected or alive status has changed
			screen.deleteWidget(self.PLAYER_DROPDOWN_ID)
			screen.addDropDownBoxGFC(self.PLAYER_DROPDOWN_ID, self.SELECTOR_X, self.SELECTOR_Y, self.SELECTOR_W, WidgetTypes.WIDGET_GENERAL, -1, -1, FontTypes.SMALL_FONT)
			screen.setActivation(self.PLAYER_DROPDOWN_ID, ActivationTypes.ACTIVATE_MIMICPARENTFOCUS)
			for j in range(gc.getMAX_PLAYERS()):
				if (gc.getPlayer(j).isAlive()):
					screen.addPullDownString(self.PLAYER_DROPDOWN_ID, gc.getPlayer(j).getName(), j, j, j == self.graph.getPlayer())
			screen.show(self.PLAYER_DROPDOWN_ID)
		else:
			BugUtil.debug("CvTechChooser: Debug OFF")
			screen.hide(self.PLAYER_DROPDOWN_ID)
		
		# Advanced Start
		if (self.isAdvancedStartMode() and self.m_eSelectedTech != -1):
			BugUtil.debug("CvTechChooser: Advanced Start Mode")
			pPlayer = gc.getPlayer(self.graph.getPlayer())
			
			szName = gc.getTechInfo(self.m_eSelectedTech).getDescription()
			iCost = pPlayer.getAdvancedStartTechCost(self.m_eSelectedTech, True)
			
			# Always show the selected tech label
			szText = u"<font=4>"
			szText += localText.getText("TXT_KEY_WB_AS_SELECTED_TECH", (szName,))
			szText += u"</font>"
			screen.setLabel(self.SELECTED_TECH_LABEL_ID, "Background", szText, CvUtil.FONT_LEFT_JUSTIFY, self.SELECTED_TECH_X, self.SELECTED_TECH_Y, 0, FontTypes.TITLE_FONT, WidgetTypes.WIDGET_GENERAL, -1, -1)
			
			# Only show the point cost if a tech is selected that has a non-zero cost
			if iCost > 0:
				szText = u"<font=4>" + localText.getText("TXT_KEY_WB_AS_SELECTED_TECH_COST", (iCost, pPlayer.getAdvancedStartPoints())) + u"</font>"
				screen.setLabel(self.AS_POINTS_LABEL_ID, "Background", szText, CvUtil.FONT_LEFT_JUSTIFY, self.ADVANCED_START_POINTS_X, self.ADVANCED_START_POINTS_Y, 0, FontTypes.TITLE_FONT, WidgetTypes.WIDGET_GENERAL, -1, -1)
			else:
				screen.hide(self.AS_POINTS_LABEL_ID)
			
			# Only show the add tech button if it the selected tech is viable for purchase
			if (iCost != -1):
				screen.show(self.ADD_TECH_BUTTON_ID)
			else:
				screen.hide(self.ADD_TECH_BUTTON_ID)
		else:
			screen.hide(self.ADD_TECH_BUTTON_ID)
			screen.hide(self.AS_POINTS_LABEL_ID)
			screen.hide(self.SELECTED_TECH_LABEL_ID)
		
		# BUG - GP Tech Prefs - start
		if (self.isShowGPTechPrefs()):
			BugUtil.debug("CvTechChooser: GP Tech Prefs ON")
			self.updateTechPrefs()
			screen.show(self.GP_TECH_PREF_PANEL_ID)
			self.bPrefsShowing = True
		else:
			BugUtil.debug("CvTechChooser: GP Tech Prefs OFF")
			screen.hide(self.GP_TECH_PREF_PANEL_ID)
			self.bPrefsShowing = False
		# BUG - GP Tech Prefs - end
	
	def hideScreen(self):
		# Get the screen
		screen = self.getScreen()
		
		# Hide the screen
		screen.hideScreen()
	
	def isAdvancedStartMode(self):
		if (self.graph.getPlayer() == -1):
			return False
		
		if (gc.getPlayer(self.graph.getPlayer()).getAdvancedStartPoints() >= 0):
			return True
		
		return False
	
	def isShowGPTechPrefs(self):
		if (self.graph.getPlayer() == -1):
			return False
		
		if not BugOpt.isShowGPTechPrefs():
			return False
		
		if (self.isAdvancedStartMode()):
			return False
		
		return True
	
# BUG - GP Tech Prefs - start
	def resetTechPrefs(self):
		self.pPrefs = TechPrefs.TechPrefs()
	
	def updateTechPrefs(self):
		#BugUtil.debug("cvTechChooser: updateTechPrefs")
		
		# Tech prefs are only valid when an active player is present
		if (self.graph.getPlayer() == -1):
			return
		
		# These don't seem to be setup when screen is first opened
		if (gc.getNumTechInfos() <= 0 or gc.getNumFlavorTypes() <= 0):
			return
		
		# Get the screen and player
		screen = self.getScreen()
		pPlayer = gc.getPlayer(self.graph.getPlayer())
		
		# Always redraw the GP icons because otherwise they are prone to disappearing
		# discover icon heading
		iIconSize = 48
		iX = self.PREF_ICON_LEFT + 5 * self.PREF_ICON_SIZE / 4 - iIconSize / 2
		iY = self.PREF_ICON_TOP - iIconSize - 40
		screen.addDDSGFCAt(self.szWidget + "GreatPersonHeading", self.GP_TECH_PREF_PANEL_ID, ArtFileMgr.getInterfaceArtInfo("DISCOVER_TECHNOLOGY_BUTTON").getPath(), iX, iY, iIconSize, iIconSize, WidgetTypes.WIDGET_GENERAL, -1, self.graph.getPlayer(), False)
		
		for i, f in enumerate(FLAVORS):
			# GP icon
			iUnitClass = gc.getInfoTypeForString(UNIT_CLASSES[i])
			iUnitType = gc.getUnitClassInfo(iUnitClass).getDefaultUnitIndex()
			pUnitInfo = gc.getUnitInfo(iUnitType)
			iX = self.PREF_ICON_LEFT
			iY = self.PREF_ICON_TOP + 4 * i * self.PREF_ICON_SIZE
			screen.addDDSGFCAt(self.szWidget + "GreatPerson" + str(f), self.GP_TECH_PREF_PANEL_ID, pUnitInfo.getButton(), iX, iY, self.PREF_ICON_SIZE, self.PREF_ICON_SIZE, WidgetTypes.WIDGET_TECH_PREFS_ALL, f, self.graph.getPlayer(), False)
		
		# Remove any techs researched since last call, creating tree if necessary
		if (not self.pPrefs):
			self.resetTechPrefs()
		self.pPrefs.removeKnownTechs()
		
		# Add all techs in research queue to set of soon-to-be-known techs
		sTechs = set()
		for i in range(gc.getNumTechInfos()):
			if (pPlayer.isResearchingTech(i)):
				sTechs.add(self.pPrefs.getTech(i))
		
		# Update the buttons to reflect the new tech prefs
		for i, f in enumerate(FLAVORS):
			# GP button
			screen.show(self.szWidget + "GreatPerson" + str(f))
			
			# Current tech GP will pop
			szButtonName = self.szWidget + "GreatPersonTech" + str(f)
			pTech = self.pPrefs.getNextResearchableFlavorTech(f)
			iX = self.PREF_ICON_LEFT + 3 * self.PREF_ICON_SIZE / 2
			iY = self.PREF_ICON_TOP + 4 * i * self.PREF_ICON_SIZE
			if (pTech):
				screen.addDDSGFCAt(szButtonName, self.GP_TECH_PREF_PANEL_ID, pTech.getInfo().getButton(), iX, iY, self.PREF_ICON_SIZE, self.PREF_ICON_SIZE, WidgetTypes.WIDGET_TECH_PREFS_CURRENT, f, self.graph.getPlayer(), False)
			else:
				screen.addDDSGFCAt(szButtonName, self.GP_TECH_PREF_PANEL_ID, self.NO_TECH_ART, iX, iY, self.PREF_ICON_SIZE, self.PREF_ICON_SIZE, WidgetTypes.WIDGET_TECH_PREFS_CURRENT, f, self.graph.getPlayer(), False)
			screen.show(szButtonName)
			
			# Tech GP will pop once selected techs are researched
			szButtonName = self.szWidget + "GreatPersonTechNext" + str(f)
			pTech = self.pPrefs.getNextResearchableWithFlavorTech(f, sTechs)
			iX = self.PREF_ICON_LEFT + 3 * self.PREF_ICON_SIZE / 2
			iY = self.PREF_ICON_TOP + 4 * i * self.PREF_ICON_SIZE + 3 * self.PREF_ICON_SIZE / 2
			if (pTech):
				screen.addDDSGFCAt(szButtonName, self.GP_TECH_PREF_PANEL_ID, pTech.getInfo().getButton(), iX, iY, self.PREF_ICON_SIZE, self.PREF_ICON_SIZE, WidgetTypes.WIDGET_TECH_PREFS_FUTURE, f, self.graph.getPlayer(), False)
			else:
				screen.addDDSGFCAt(szButtonName, self.GP_TECH_PREF_PANEL_ID, self.NO_TECH_ART, iX, iY, self.PREF_ICON_SIZE, self.PREF_ICON_SIZE, WidgetTypes.WIDGET_TECH_PREFS_FUTURE, f, self.graph.getPlayer(), False)
			screen.show(szButtonName)
# BUG - GP Tech Prefs - end
	
	def onClose(self):
		BugUtil.debug("CvTechChooser: onClose")
		if (self.graph.getPlayer() != -1 and gc.getPlayer(self.graph.getPlayer()).getAdvancedStartPoints() >= 0):
			CyInterface().setDirty(InterfaceDirtyBits.Advanced_Start_DIRTY_BIT, True)
		return 0
	
	def TechRecord(self, inputClass):
		return 0
	
	# Clicked the parent?
	def ParentClick(self, inputClass):
		return 0
	
	def PlayerDropDown(self, inputClass):
		if (inputClass.getNotifyCode() == NotifyCode.NOTIFY_LISTBOX_ITEM_SELECTED):
			screen = self.getScreen()
			iIndex = screen.getSelectedPullDownID(self.PLAYER_DROPDOWN_ID)
			self.graph.setPlayer(screen.getPullDownData(self.PLAYER_DROPDOWN_ID, iIndex))
			self.updateControls()
	
	# Will handle the input for this screen...
	def handleInput (self, inputClass):
		BugUtil.debug("CvTechChooser: handleInput")
		BugUtil.debugInput(inputClass)
		
		# Get the screen
		screen = self.getScreen()
		
		szWidgetName = inputClass.getFunctionName() + str(inputClass.getID())
		
		# Advanced Start Stuff
		if (self.graph.getPlayer() != -1):
			pPlayer = gc.getPlayer(self.graph.getPlayer())
			if (pPlayer.getAdvancedStartPoints() >= 0):
				# Add tech button
				if (inputClass.getFunctionName() == self.ADD_TECH_BUTTON_ID):
					if (pPlayer.getAdvancedStartTechCost(self.m_eSelectedTech, True) != -1):
						CyMessageControl().sendAdvancedStartAction(AdvancedStartActionTypes.ADVANCEDSTARTACTION_TECH, self.graph.getPlayer(), -1, -1, self.m_eSelectedTech, True)	#Action, Player, X, Y, Data, bAdd
						self.m_bTechRecordsDirty = True
						self.m_bSelectedTechDirty = True
				
				# Tech clicked on
				elif (inputClass.getNotifyCode() == NotifyCode.NOTIFY_CLICKED):
					if (inputClass.getButtonType() == WidgetTypes.WIDGET_TECH_TREE):
						self.m_eSelectedTech = inputClass.getData1()
						self.updateControls()
		
		' Calls function mapped in TechChooserInputMap'
		# only get from the map if it has the key
		if (inputClass.getNotifyCode() == NotifyCode.NOTIFY_LISTBOX_ITEM_SELECTED):
			BugUtil.debug("CvTechChooser: handleInput - dropdown")
			self.PlayerDropDown(inputClass)
			return 1
		
		return 0
	
	def getScreen(self):
		return CyGInterfaceScreen("TechChooser", CvScreenEnums.TECH_CHOOSER)
	
	def record(self, szWidgetID):
		self.widgets.add(szWidgetID)
		return szWidgetID
	
	def clear(self):
		self.graph.clear()
		
		screen = self.getScreen()
		for szWidgetID in self.widgets:
			screen.deleteWidget(szWidgetID)
		self.widgets.clear()
	
	def isEmpty(self):
		return len(self.widgets) == 0

class TechChooserMaps:

	TechChooserInputMap = {
		'TechRecord'			: CvTechChooser.TechRecord,
		'TechID'				: CvTechChooser.ParentClick,
		'TechPane'				: CvTechChooser.ParentClick,
		'TechButtonID'			: CvTechChooser.ParentClick,
		'TechButtonBorder'		: CvTechChooser.ParentClick,
		'Unit'					: CvTechChooser.ParentClick,
		'Building'				: CvTechChooser.ParentClick,
		'Obsolete'				: CvTechChooser.ParentClick,
		'ObsoleteX'				: CvTechChooser.ParentClick,
		'Move'					: CvTechChooser.ParentClick,
		'FreeUnit'				: CvTechChooser.ParentClick,
		'FeatureProduction'			: CvTechChooser.ParentClick,
		'Worker'				: CvTechChooser.ParentClick,
		'TradeRoutes'			: CvTechChooser.ParentClick,
		'HealthRate'			: CvTechChooser.ParentClick,
		'HappinessRate'			: CvTechChooser.ParentClick,
		'FreeTech'				: CvTechChooser.ParentClick,
		'LOS'					: CvTechChooser.ParentClick,
		'MapCenter'				: CvTechChooser.ParentClick,
		'MapReveal'				: CvTechChooser.ParentClick,
		'MapTrade'				: CvTechChooser.ParentClick,
		'TechTrade'				: CvTechChooser.ParentClick,
		'OpenBorders'		: CvTechChooser.ParentClick,
		'BuildBridge'			: CvTechChooser.ParentClick,
		'Irrigation'			: CvTechChooser.ParentClick,
		'Improvement'			: CvTechChooser.ParentClick,
		'DomainExtraMoves'			: CvTechChooser.ParentClick,
		'AdjustButton'			: CvTechChooser.ParentClick,
		'TerrainTradeButton'	: CvTechChooser.ParentClick,
		'SpecialBuildingButton'	: CvTechChooser.ParentClick,
		'YieldChangeButton'		: CvTechChooser.ParentClick,
		'BonusRevealButton'		: CvTechChooser.ParentClick,
		'CivicRevealButton'		: CvTechChooser.ParentClick,
		'ProjectInfoButton'		: CvTechChooser.ParentClick,
		'ProcessInfoButton'		: CvTechChooser.ParentClick,
		'FoundReligionButton'	: CvTechChooser.ParentClick,
		'PlayerDropDown'		: CvTechChooser.PlayerDropDown,
		}

