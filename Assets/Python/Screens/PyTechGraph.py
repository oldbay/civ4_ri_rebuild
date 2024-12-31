## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
from CvPythonExtensions import *
import CvUtil
import ScreenInput
import CvScreenEnums
import CvScreensInterface
import math

import BugUtil

CIV_HAS_TECH = 0
CIV_IS_RESEARCHING = 1
CIV_NO_RESEARCH = 2
CIV_TECH_AVAILABLE = 3

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

# BUG - GP Tech Prefs - start
import BugCore
BugOpt = BugCore.game.Advisors
ClockOpt = BugCore.game.NJAGC
# BUG - GP Tech Prefs - end

# BUG - Tech Era Colors - start
def getEraDescription(eWidgetType, iData1, iData2, bOption):
	return gc.getEraInfo(iData1).getDescription()
# BUG - Tech Era Colors - end

class PyTechGraph:
	"Tech Graph"

	def __init__(self, container, szWidget):
		self.container = container
		self.graph = CyTechGraph()
		self.szWidget = szWidget
		
		self.widgets = set()
		
		self.aiColumnX = []
		self.aiColumnWidth = []
		
		self.aiCurrentState = []
		# MOD - START - Tech Transfer
		self.aiCurrentTechTransferCount = []
		# MOD - END - Tech Transfer
		
		self.TECH_GRAPH_ID = self.szWidget + "Panel"
		
		# Advanced Start
		self.m_iSelectedTech = -1
		self.m_bSelectedTechDirty = False
		self.m_bTechRecordsDirty = False
		
		# Performance Evaluation
		self.timer = BugUtil.Timer("PyTechGraph")

	def cacheArtInfo(self):
		self.ARROW_X = ArtFileMgr.getInterfaceArtInfo("ARROW_X").getPath()
		self.ARROW_Y = ArtFileMgr.getInterfaceArtInfo("ARROW_Y").getPath()
		self.ARROW_MXMY = ArtFileMgr.getInterfaceArtInfo("ARROW_MXMY").getPath()
		self.ARROW_XY = ArtFileMgr.getInterfaceArtInfo("ARROW_XY").getPath()
		self.ARROW_MXY = ArtFileMgr.getInterfaceArtInfo("ARROW_MXY").getPath()
		self.ARROW_XMY = ArtFileMgr.getInterfaceArtInfo("ARROW_XMY").getPath()
		self.ARROW_HEAD = ArtFileMgr.getInterfaceArtInfo("ARROW_HEAD").getPath()
		
		self.OBSOLETE = ArtFileMgr.getInterfaceArtInfo("INTERFACE_BUTTONS_RED_X").getPath()
	
	def setCivilization(self, eCivilization):
		if (eCivilization != -1 and eCivilization == gc.getGame().getActiveCivilizationType()):
			ePlayer = gc.getGame().getActivePlayer()
			if (ePlayer != self.graph.getPlayer()):
				self.setPlayer(ePlayer)
		elif (eCivilization != self.graph.getCivilization() or self.graph.getPlayer() != -1):
			self.graph.updateCivilization(eCivilization)
			if (not self.isEmpty()):
				self.redraw()
	
	def getCivilization(self):
		return self.graph.getCivilization()
	
	def setPlayer(self, ePlayer):
		if ePlayer > -1:
			self.graph.updatePlayer(ePlayer)
		if (not self.isEmpty()):
			self.redraw()
	
	def getPlayer(self):
		return self.graph.getPlayer()
	
	def calculateLayout(self):
		self.graph.update()
		
		self.TECH_GRAPH_X = self.container.TECH_GRAPH_X
		self.TECH_GRAPH_Y = self.container.TECH_GRAPH_Y
		self.TECH_GRAPH_W = self.container.TECH_GRAPH_W
		self.TECH_GRAPH_H = self.container.TECH_GRAPH_H
		
		iPadding = int(max(self.TECH_GRAPH_H - 700, 0) / 6)
		
		self.TECH_GRAPH_PADDING_TOP = iPadding
		self.TECH_GRAPH_PADDING_RIGHT = iPadding / 4
		self.TECH_GRAPH_PADDING_BOTTOM = iPadding
		self.TECH_GRAPH_PADDING_LEFT = iPadding / 4
		
		self.TECH_PANEL_MARGIN_HORIZONTAL = 40
		self.TECH_PANEL_MARGIN_VERTICAL = 9
		self.TECH_PANEL_BORDER_TOP = 8
		self.TECH_PANEL_BORDER_RIGHT = 8
		self.TECH_PANEL_BORDER_BOTTOM = 8
		self.TECH_PANEL_BORDER_LEFT = 8
		self.TECH_PANEL_PADDING_TOP = 4
		self.TECH_PANEL_PADDING_RIGHT = 4
		self.TECH_PANEL_PADDING_BOTTOM = 4
		self.TECH_PANEL_PADDING_LEFT = 4
		
		self.ICON_TEXTURE_SIZE = 24
		self.ICON_SPACING = 3
		
		self.PIXELS_RESERVED_PER_CHARACTER = 6
		
		self.TECH_PANEL_HEIGHT = self.TECH_PANEL_BORDER_TOP + self.TECH_PANEL_PADDING_TOP + self.ICON_TEXTURE_SIZE + self.ICON_SPACING + self.ICON_TEXTURE_SIZE + self.TECH_PANEL_PADDING_BOTTOM + self.TECH_PANEL_BORDER_BOTTOM
		
		self.aiColumnX = []
		self.aiColumnWidth = []
		
		iColumnX = 0
		iMaxGridY = 0
		for iColumn in range(self.graph.getNumColumns()):
			self.aiColumnX.append(iColumnX)
			column = self.graph.getColumn(iColumn)
			if (column and column.getNumTechs() > 0):
				iIconWidth = (column.getNumIcons() * (self.ICON_TEXTURE_SIZE + self.ICON_SPACING)) - self.ICON_SPACING + (self.ICON_TEXTURE_SIZE * 2)
				iHeaderWidth = 0
				for iTechIndex in range( column.getNumTechs() ):
					eTech = column.getTech(iTechIndex)
					# Estimate the number of pixels per character and include an extra 5 characters for
					# the research turns count that is shown when researching the tech
					iDescriptionWidth = (len(gc.getTechInfo(eTech).getDescription()) + 12) * self.PIXELS_RESERVED_PER_CHARACTER
					iNumPrereqAndTechs = 0
					for j in range( gc.getNUM_AND_TECH_PREREQS() ):
						ePrereqTech = gc.getTechInfo(eTech).getPrereqAndTechs(j)
						if ( ePrereqTech > -1 ):
							iNumPrereqAndTechs += 1
					iAndPrereqWidth = (iNumPrereqAndTechs * (self.ICON_TEXTURE_SIZE + self.ICON_SPACING)) - self.ICON_SPACING
					iHeaderWidth = max(iHeaderWidth, iDescriptionWidth + iAndPrereqWidth)
					iMaxGridY = max(iMaxGridY, gc.getTechInfo(eTech).getGridY())
				
				iColumnWidth = self.TECH_PANEL_BORDER_LEFT + self.TECH_PANEL_PADDING_LEFT + max(iHeaderWidth, iIconWidth) + self.TECH_PANEL_PADDING_RIGHT + self.TECH_PANEL_BORDER_RIGHT
				iColumnX += iColumnWidth + self.TECH_PANEL_MARGIN_HORIZONTAL
				self.aiColumnWidth.append(iColumnWidth)
			else:
				self.aiColumnWidth.append(0)
		
		iConsumedHeight = self.TECH_PANEL_HEIGHT + (self.TECH_PANEL_HEIGHT * (iMaxGridY - 1) / 2)
		iExtraHeight = max(0, self.TECH_GRAPH_H - iConsumedHeight - self.TECH_GRAPH_PADDING_TOP - self.TECH_GRAPH_PADDING_BOTTOM)
		
		self.TECH_PANEL_MARGIN_VERTICAL = iExtraHeight * 2 / (iMaxGridY - 1)
		self.ROW_STEPPING = ( self.TECH_PANEL_HEIGHT + self.TECH_PANEL_MARGIN_VERTICAL ) / 2
		
		#BugUtil.debug("PyTechGraph: Stepping Calc (%i, %i, %i, %i)", self.TECH_GRAPH_H, iTotalPanelHeight, self.ROW_STEPPING, iExtraStepping)
		
		#self.ROW_STEPPING += iExtraStepping
		
		self.VIEWPORT_WIDTH = self.TECH_GRAPH_PADDING_LEFT + self.aiColumnX[-1] + self.aiColumnWidth[-1] + self.TECH_GRAPH_PADDING_RIGHT + 10
		self.VIEWPORT_HEIGHT = self.TECH_GRAPH_PADDING_TOP + ((iMaxGridY - 1) * self.ROW_STEPPING) + self.TECH_PANEL_HEIGHT + self.TECH_GRAPH_PADDING_BOTTOM
	
	def draw(self):
		BugUtil.debug("PyTechGraph: draw (%i, %i)", self.graph.getCivilization(), self.graph.getPlayer())
		
		if (not self.isEmptyOfElements()):
			return
		
		screen = self.getScreen()
		
		self.timer.reset()
		self.timer.start()
		
		self.aiCurrentState = []
		# MOD - START - Tech Transfer
		self.aiCurrentTechTransferCount = []
		# MOD - END - Tech Transfer
		
		self.cacheArtInfo()
		
		if (not self.TECH_GRAPH_ID in self.widgets):
			screen.addScrollPanel(self.record(self.TECH_GRAPH_ID), u"", self.TECH_GRAPH_X, self.TECH_GRAPH_Y, self.TECH_GRAPH_W, self.TECH_GRAPH_H, PanelStyles.PANEL_STYLE_STANDARD)
			screen.setActivation(self.TECH_GRAPH_ID, ActivationTypes.ACTIVATE_NORMAL)
		
		screen.hide(self.TECH_GRAPH_ID)
		
		# Place the tech blocks
		self.drawPanels(screen)
		
		# Draw the arrows
		self.drawArrows(screen)
		
		screen.setViewMin(self.TECH_GRAPH_ID, self.VIEWPORT_WIDTH, self.VIEWPORT_HEIGHT)
		
		screen.show(self.TECH_GRAPH_ID)
		
		self.timer.logSpan("total")
	
	def resize(self):
		self.clear()
		self.calculateLayout()
		self.draw()
	
	def redraw(self):
		self.clearElements()
		self.calculateLayout()
		self.draw()
	
	def drawPanels(self, screen):
		BugUtil.debug("PyTechGraph: drawPanels")
		
		iContext = packWordID(self.graph.getCivilization(), self.graph.getPlayer())
		
		# Go through all the techs
		for eTech in range(gc.getNumTechInfos()):
			iColumn = gc.getTechInfo(eTech).getGridX()
			iPanelX = self.TECH_GRAPH_PADDING_LEFT + self.aiColumnX[iColumn]
			iPanelY = self.TECH_GRAPH_PADDING_TOP + (gc.getTechInfo(eTech).getGridY() - 1) * self.ROW_STEPPING
			iPanelWidth = self.aiColumnWidth[iColumn]
			szTech = self.szWidget + "Tech" + str(eTech)
			szTechPanelID = szTech + "Panel"
			
			# BUG - Tech Era Colors - start
			if BugOpt.isShowTechEra():
				szTechShadowID = szTech + "Shadow"
				iShadowOffset = 9
				#screen.attachPanelAt(self.TECH_GRAPH_ID, szTechShadowID, u"", u"", True, False, PanelStyles.PANEL_STYLE_TECH, iPanelX + iShadowOffset, iPanelY + iShadowOffset, iPanelWidth, self.TECH_PANEL_HEIGHT, WidgetTypes.WIDGET_TECH_CHOOSER_ERA, gc.getTechInfo(eTech).getEra(), -1 )
				if (not gc.getTechInfo(eTech).isDisable()):
					screen.attachPanelAt(self.TECH_GRAPH_ID, szTechShadowID, u"", u"", True, False, PanelStyles.PANEL_STYLE_TECH, iPanelX + iShadowOffset, iPanelY + iShadowOffset, iPanelWidth, self.TECH_PANEL_HEIGHT, WidgetTypes.WIDGET_TECH_TREE, eTech, iContext)
					self.setTechPanelShadowColor(screen, szTechShadowID, gc.getTechInfo(eTech).getEra())
			# BUG - Tech Era Colors - end

			if (not gc.getTechInfo(eTech).isDisable()):
				screen.attachPanelAt(self.TECH_GRAPH_ID, self.record(szTechPanelID), u"", u"", True, False, PanelStyles.PANEL_STYLE_TECH, iPanelX, iPanelY, iPanelWidth, self.TECH_PANEL_HEIGHT, WidgetTypes.WIDGET_TECH_TREE, eTech, iContext)
				screen.setActivation(szTechPanelID, ActivationTypes.ACTIVATE_MIMICPARENTFOCUS)
			
			state = self.getTechPanelState(eTech)
			
			self.aiCurrentState.append(state[0])
			# MOD - START - Tech Transfer
			self.aiCurrentTechTransferCount.append(state[1])
			# MOD - END - Tech Transfer
			screen.setPanelColor(szTechPanelID, state[2], state[3], state[4])
			
			# Tech Icon
			szTechButtonID = szTech + "Button"
			if (not gc.getTechInfo(eTech).isDisable()):
				screen.addDDSGFCAt(self.record(szTechButtonID), szTechPanelID, gc.getTechInfo(eTech).getButton(), self.TECH_PANEL_BORDER_LEFT + self.TECH_PANEL_PADDING_LEFT, self.TECH_PANEL_BORDER_TOP + self.TECH_PANEL_PADDING_TOP, self.ICON_TEXTURE_SIZE * 2, self.ICON_TEXTURE_SIZE * 2, WidgetTypes.WIDGET_TECH_TREE, eTech, iContext, False)
			
			# Tech Name
			szTechNameID = szTech + "Name"
			szTechString = self.getTechPanelDescription(eTech)
			if (not gc.getTechInfo(eTech).isDisable()):
				screen.setTextAt(self.record(szTechNameID), szTechPanelID, szTechString, CvUtil.FONT_LEFT_JUSTIFY, self.TECH_PANEL_BORDER_LEFT + self.TECH_PANEL_PADDING_LEFT + self.ICON_TEXTURE_SIZE * 2 + self.ICON_SPACING, self.TECH_PANEL_BORDER_TOP + self.TECH_PANEL_PADDING_TOP - 2, -0.1, FontTypes.SMALL_FONT, WidgetTypes.WIDGET_TECH_TREE, eTech, iContext)
				screen.setActivation(szTechNameID, ActivationTypes.ACTIVATE_MIMICPARENTFOCUS )
			
			# AND Prerequisites
			fX = iPanelWidth - (self.TECH_PANEL_BORDER_RIGHT + self.TECH_PANEL_PADDING_RIGHT + self.ICON_TEXTURE_SIZE)
			for j in range( gc.getNUM_AND_TECH_PREREQS() ):
				ePrereqTech = gc.getTechInfo(eTech).getPrereqAndTechs(j)
				if ( ePrereqTech > -1 ):
					szTechPrereqID = szTech + "Prereq" + str(j)
					screen.addDDSGFCAt(self.record(szTechPrereqID), szTechPanelID, gc.getTechInfo(ePrereqTech).getButton(), fX, self.TECH_PANEL_BORDER_TOP + self.TECH_PANEL_PADDING_TOP, self.ICON_TEXTURE_SIZE, self.ICON_TEXTURE_SIZE, WidgetTypes.WIDGET_HELP_TECH_PREPREQ, ePrereqTech, -1, False)
					
					#szTechPrereqBorderID = "TechPrereqBorderID" + str((eTech * 1000) + j)
					#screen.addDDSGFCAt( szTechPrereqBorderID, szTechPanelID, ArtFileMgr.getInterfaceArtInfo("TECH_TREE_BUTTON_BORDER").getPath(), iX + fX + 4, iY + 22, 32, 32, WidgetTypes.WIDGET_HELP_TECH_PREPREQ, ePrereqTech, -1, False )
					
					fX -= (self.ICON_TEXTURE_SIZE + self.ICON_SPACING)
			
			# Tech Details
			panel = self.graph.getPanel(eTech)
			iIconX = self.TECH_PANEL_BORDER_LEFT + self.TECH_PANEL_PADDING_LEFT + self.ICON_TEXTURE_SIZE * 2 
			
			for j in range( panel.getNumIcons() ):
				icon = panel.getIcon(j)
				szTechIconID = szTech + "Icon" + str(j)
				if (not gc.getTechInfo(eTech).isDisable()):
					screen.addDDSGFCAt(self.record(szTechIconID), szTechPanelID, icon.szTexture, iIconX, self.TECH_PANEL_BORDER_TOP + self.TECH_PANEL_PADDING_TOP + self.ICON_TEXTURE_SIZE + self.ICON_SPACING, self.ICON_TEXTURE_SIZE, self.ICON_TEXTURE_SIZE, icon.eWidgetType, icon.iData1, icon.iData2, icon.bOption)
					if (icon.bObsolete):
						szObsoleteTechIconID = szTechIconID + "Obsolete"
						screen.addDDSGFCAt(self.record(szObsoleteTechIconID), szTechPanelID, self.OBSOLETE, iIconX, self.TECH_PANEL_BORDER_TOP + self.TECH_PANEL_PADDING_TOP + self.ICON_TEXTURE_SIZE + self.ICON_SPACING, self.ICON_TEXTURE_SIZE, self.ICON_TEXTURE_SIZE, icon.eWidgetType, icon.iData1, icon.iData2, icon.bOption)
				iIconX += (self.ICON_TEXTURE_SIZE + self.ICON_SPACING)

			if (not gc.getTechInfo(eTech).isDisable()):
				screen.show(szTechPanelID)
	
	# Will draw the arrows
	def drawArrows(self, screen):
		BugUtil.debug("PyTechGraph: drawArrows")
		
		iLoop = 0
		
		for eTech in range(gc.getNumTechInfos()):
			iColumn = gc.getTechInfo(eTech).getGridX()
			iRow = gc.getTechInfo(eTech).getGridY()
			iPanelX = self.TECH_GRAPH_PADDING_LEFT + self.aiColumnX[iColumn]
			iPanelY = self.TECH_GRAPH_PADDING_TOP + (iRow - 1) * self.ROW_STEPPING
			iPanelWidth = self.aiColumnWidth[iColumn]
			szTech = self.szWidget + "Tech" + str(eTech)
			
			j = 0
			
			# OR Prerequisites
			for j in range( gc.getNUM_OR_TECH_PREREQS() ):
				ePrereqTech = gc.getTechInfo(eTech).getPrereqOrTechs(j)
				if ( ePrereqTech > -1 ):
					iPrereqColumn = gc.getTechInfo(ePrereqTech).getGridX()
					iPrereqRow = gc.getTechInfo(ePrereqTech).getGridY()
					iPrereqPanelX = self.TECH_GRAPH_PADDING_LEFT + self.aiColumnX[iPrereqColumn]
					iPrereqPanelY = self.TECH_GRAPH_PADDING_TOP + (iPrereqRow - 1) * self.ROW_STEPPING
					
					iDeltaRow = iRow - iPrereqRow
					
					iStartX = iPrereqPanelX + self.aiColumnWidth[iPrereqColumn] - 6
					iStartY = iPrereqPanelY + int(self.TECH_PANEL_HEIGHT / 2)
					
					iEndX = iPanelX + 4
					iEndY = iPanelY + int(self.TECH_PANEL_HEIGHT / 2)
					
					iOffsetY = abs(iDeltaRow)
					iOffsetY += iOffsetY % 2 # Offset increases every other row
					iOffsetY *= 4
					
					if (iDeltaRow < 0):
						# There is a bend upward
						iStartY -= iOffsetY
						iEndY += iOffsetY
					if (iDeltaRow > 0):
						# There is a bend downward
						iStartY += iOffsetY
						iEndY -= iOffsetY
					
					iDeltaX = iEndX - iStartX
					iDeltaY = iEndY - iStartY
					iDeltaX1 = int(math.floor(iDeltaX / 2.0))
					iDeltaX2 = int(math.ceil(iDeltaX / 2.0))
					iMidpointX = iStartX + iDeltaX1
					
					szTechArrowID = szTech + "Arrow" + str(j)
					
					if (iDeltaY == 0):
						# Straight line
						screen.addDDSGFCAt(self.record(szTechArrowID + "X"),    self.TECH_GRAPH_ID, self.ARROW_X,    iStartX,        iStartY - 4, iDeltaX - 4,   8,                WidgetTypes.WIDGET_GENERAL, -1, -1, False)
						screen.addDDSGFCAt(self.record(szTechArrowID + "Head"), self.TECH_GRAPH_ID, self.ARROW_HEAD, iEndX - 4,      iEndY - 4,   8,             8,                WidgetTypes.WIDGET_GENERAL, -1, -1, False)
					elif (iDeltaY < 0):
						# There is a bend upward
						screen.addDDSGFCAt(self.record(szTechArrowID + "X1"),   self.TECH_GRAPH_ID, self.ARROW_X,    iStartX,        iStartY - 4, iDeltaX1 - 4,  8,                WidgetTypes.WIDGET_GENERAL, -1, -1, False)
						screen.addDDSGFCAt(self.record(szTechArrowID + "XY"),   self.TECH_GRAPH_ID, self.ARROW_XY,   iMidpointX - 4, iStartY - 4, 8,             8,                WidgetTypes.WIDGET_GENERAL, -1, -1, False)
						screen.addDDSGFCAt(self.record(szTechArrowID + "Y"),    self.TECH_GRAPH_ID, self.ARROW_Y,    iMidpointX - 4, iEndY + 4,   8,             abs(iDeltaY) - 8, WidgetTypes.WIDGET_GENERAL, -1, -1, False)
						screen.addDDSGFCAt(self.record(szTechArrowID + "XMY"),  self.TECH_GRAPH_ID, self.ARROW_XMY,  iMidpointX - 4, iEndY - 4,   8,             8,                WidgetTypes.WIDGET_GENERAL, -1, -1, False)
						screen.addDDSGFCAt(self.record(szTechArrowID + "X2"),   self.TECH_GRAPH_ID, self.ARROW_X,    iMidpointX + 4, iEndY - 4,   iDeltaX2 - 8,  8,                WidgetTypes.WIDGET_GENERAL, -1, -1, False)
						screen.addDDSGFCAt(self.record(szTechArrowID + "Head"), self.TECH_GRAPH_ID, self.ARROW_HEAD, iEndX - 4,      iEndY - 4,   8,             8,                WidgetTypes.WIDGET_GENERAL, -1, -1, False)
					elif (iDeltaY > 0):
						# There is a bend downward
						screen.addDDSGFCAt(self.record(szTechArrowID + "X1"),   self.TECH_GRAPH_ID, self.ARROW_X,    iStartX,        iStartY - 4, iDeltaX1 - 4,  8,                WidgetTypes.WIDGET_GENERAL, -1, -1, False)
						screen.addDDSGFCAt(self.record(szTechArrowID + "MXMY"), self.TECH_GRAPH_ID, self.ARROW_MXMY, iMidpointX - 4, iStartY - 4, 8,             8,                WidgetTypes.WIDGET_GENERAL, -1, -1, False)
						screen.addDDSGFCAt(self.record(szTechArrowID + "Y"),    self.TECH_GRAPH_ID, self.ARROW_Y,    iMidpointX - 4, iStartY + 4, 8,             abs(iDeltaY) - 8, WidgetTypes.WIDGET_GENERAL, -1, -1, False)
						screen.addDDSGFCAt(self.record(szTechArrowID + "MXY"),  self.TECH_GRAPH_ID, self.ARROW_MXY,  iMidpointX - 4, iEndY - 4,   8,             8,                WidgetTypes.WIDGET_GENERAL, -1, -1, False)
						screen.addDDSGFCAt(self.record(szTechArrowID + "X2"),   self.TECH_GRAPH_ID, self.ARROW_X,    iMidpointX + 4, iEndY - 4,   iDeltaX2 - 8,  8,                WidgetTypes.WIDGET_GENERAL, -1, -1, False)
						screen.addDDSGFCAt(self.record(szTechArrowID + "Head"), self.TECH_GRAPH_ID, self.ARROW_HEAD, iEndX - 4,      iEndY - 4,   8,             8,                WidgetTypes.WIDGET_GENERAL, -1, -1, False)
	
	# Will update the tech records based on color, researching, researched, queued, etc.
	def refresh(self):
		BugUtil.debug("PyTechGraph: refresh")
		
		# Updates are only valid when an active player is present
		if (self.graph.getPlayer() == -1):
			return
		
		iContext = packWordID(self.graph.getCivilization(), self.graph.getPlayer())
		
		screen = self.getScreen()
		
		pPlayer = gc.getPlayer(self.graph.getPlayer())
		
		# Go through all the techs
		for eTech in range(gc.getNumTechInfos()):
			state = self.getTechPanelState(eTech)
			
			if (self.aiCurrentState[eTech] != state[0] or self.aiCurrentTechTransferCount[eTech] != state[1] or pPlayer.isResearchingTech(eTech)):
				self.aiCurrentState[eTech] = state[0]
				self.aiCurrentTechTransferCount[eTech] = state[1]
				
				szTech = self.szWidget + "Tech" + str(eTech)
				szTechPanelID = szTech + "Panel"
				screen.setPanelColor(szTechPanelID, state[2], state[3], state[4])
				
				szTechNameID = szTech + "Name"
				szTechString = self.getTechPanelDescription(eTech)
				screen.setTextAt(szTechNameID, szTechPanelID, szTechString, CvUtil.FONT_LEFT_JUSTIFY, self.TECH_PANEL_BORDER_LEFT + self.TECH_PANEL_PADDING_LEFT + self.ICON_TEXTURE_SIZE * 2 + self.ICON_SPACING, self.TECH_PANEL_BORDER_TOP + self.TECH_PANEL_PADDING_TOP, -0.1, FontTypes.SMALL_FONT, WidgetTypes.WIDGET_TECH_TREE, eTech, iContext)
				screen.setActivation(szTechNameID, ActivationTypes.ACTIVATE_MIMICPARENTFOCUS)
	
	def getTechPanelState(self, eTech):
		if (self.graph.getPlayer() != -1):
			pPlayer = gc.getPlayer(self.graph.getPlayer())
			pTeam = gc.getTeam(pPlayer.getTeam())
			# MOD - START - Tech Transfer
			iTechTransferCount = pTeam.getTechTransferCount(eTech)
			# MOD - END - Tech Transfer

			if ( pTeam.isHasTech(eTech) ):
				return CIV_HAS_TECH, iTechTransferCount, 85, 150, 87
			elif ( pPlayer.getCurrentResearch() == eTech or pPlayer.isResearchingTech(eTech) ):
				# MOD - START - Tech Transfer
				if (pTeam.isTechTransfer(eTech)):
					return CIV_IS_RESEARCHING, iTechTransferCount, 104, 158, 255
				else:
					return CIV_IS_RESEARCHING, iTechTransferCount, 104, 158, 165
				# MOD - END - Tech Transfer
			elif ( pPlayer.canEverResearch(eTech) ):
				# MOD - START - Tech Transfer
				if (pTeam.isTechTransfer(eTech)):
					return CIV_NO_RESEARCH, iTechTransferCount, 30, 74, 150
				else:
					return CIV_NO_RESEARCH, iTechTransferCount, 100, 104, 140
				# MOD - END - Tech Transfer
			else:
				return CIV_TECH_AVAILABLE, iTechTransferCount, 206, 65, 69
		else:
			return CIV_NO_RESEARCH, 0, 100, 104, 140
	
	def getTechPanelDescription(self, eTech):
		szTechString = "<font=1>"
		
		if (self.graph.getPlayer() != -1):
			pPlayer = gc.getPlayer(self.graph.getPlayer())
			if ( pPlayer.isResearchingTech(eTech) ):
				szTechString += unicode(pPlayer.getQueuePosition(eTech)) + ". "
				szTechString += gc.getTechInfo(eTech).getDescription()
				szTechString += " ("
				szTechString += str(pPlayer.getResearchTurnsLeft(eTech, ( pPlayer.getCurrentResearch() == eTech )))
				szTechString += ")"
			else:
				szTechString += gc.getTechInfo(eTech).getDescription()
		else:
			szTechString += gc.getTechInfo(eTech).getDescription()
		
		szTechString += "</font>"
		
		return szTechString

# BUG - Tech Era Colors - start
	def setTechPanelShadowColor(self, screen, sPanel, iEra):
		szEra = gc.getEraInfo(iEra).getType()
		iColor = ClockOpt.getEraColor(szEra)
		if iColor != -1:
			color = gc.getColorInfo(iColor)
			if color:
				rgb = color.getColor() # NiColorA object
				if rgb:
					screen.setPanelColor(sPanel, int(100 * rgb.r), int(100 * rgb.g), int(100 * rgb.b))
# BUG - Tech Era Colors - end
	
	def getScreen(self):
		return self.container.getScreen()
	
	def record(self, szWidgetID):
		self.widgets.add(szWidgetID)
		return szWidgetID
	
	def clear(self):
		screen = self.getScreen()
		for szWidgetID in self.widgets:
			screen.deleteWidget(szWidgetID)
		self.widgets.clear()
	
	def clearElements(self):
		screen = self.getScreen()
		bFound = False
		for szWidgetID in self.widgets:
			if (szWidgetID != self.TECH_GRAPH_ID):
				screen.deleteWidget(szWidgetID)
			else:
				bFound = True
		self.widgets.clear()
		if (bFound):
			self.widgets.add(self.TECH_GRAPH_ID)
	
	def isEmptyOfElements(self):
		return len(self.widgets) <= 1
	
	def isEmpty(self):
		return len(self.widgets) == 0
