## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
from CvPythonExtensions import *
import CvUtil
import CvPediaPage

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

class CvPediaRoute(CvPediaPage.CvPediaPage):
	"Civilopedia Screen for Routes"

	def __init__(self, section):
		super(CvPediaRoute, self).__init__(section, CivilopediaPageTypes.CIVILOPEDIA_PAGE_ROUTE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_ROUTE, "Route")
		
		self.eRoute = -1
		
		self.REQUIRES_PANEL_ID = self.szWidget + "RequiresPanel"
	
	def getData(self):
		return (self.eRoute, -1)
	
	def setData(self, eRoute, iPageData2):
		if (self.eRoute != eRoute):
			self.clear()
			self.eRoute = eRoute
	
	def getInfo(self):
		return gc.getRouteInfo(self.eRoute)
	
	def calculateLayout(self):
		self.calculateTitleLayout()
		#self.calculateSynopsisLayout(self.main.PAGE_AREA_W, self.main.COMMON_ICON_PANEL_SIZE * 2 + self.main.COMMON_MARGIN_H)
		self.calculateSynopsisLayout(-1, self.main.COMMON_ICON_PANEL_SIZE * 2 + self.main.COMMON_MARGIN_H)
		self.calculateTwoColumnLayout(0, 50, 0, 50)
		
		self.REQUIRES_PANEL_X = self.COLUMN1_X
		self.REQUIRES_PANEL_Y = self.SYNOPSIS_PANEL_Y + self.SYNOPSIS_PANEL_H + self.main.COMMON_MARGIN_H
		self.REQUIRES_PANEL_W = self.COLUMN1_W
		self.REQUIRES_PANEL_H = self.main.COMMON_ICON_PANEL_SIZE
		
		self.EFFECTS_PANEL_X = self.COLUMN1_X
		self.EFFECTS_PANEL_Y = self.REQUIRES_PANEL_Y + self.REQUIRES_PANEL_H + self.main.COMMON_MARGIN_H
		self.EFFECTS_PANEL_W = self.COLUMN1_W
		self.EFFECTS_PANEL_H = self.main.PAGE_AREA_Y + self.main.PAGE_AREA_H - self.EFFECTS_PANEL_Y
	
	def draw(self):
		if (not self.isEmpty()):
			return
		
		screen = self.getScreen()
		info = self.getInfo()
		ePlayer = gc.getGame().getActivePlayer()
		
		self.drawTitle(screen, info, ePlayer)
		self.drawSynopsis(screen, info, ePlayer)
		self.drawRequires(screen, info, ePlayer)
		self.drawEffects(screen, CyGameTextMgr().getRouteHelp(self.eRoute, True)[1:])
	
	def drawRequires(self, screen, info, ePlayer):
		screen.addPanel(self.record(self.REQUIRES_PANEL_ID), localText.getText("TXT_KEY_PEDIA_REQUIRES", ()), "", False, True, self.REQUIRES_PANEL_X, self.REQUIRES_PANEL_Y, self.REQUIRES_PANEL_W, self.REQUIRES_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		
		screen.attachLabel(self.REQUIRES_PANEL_ID, "", "  ")
		
		bFirst = True
		
		# Techs
		for eLoopBuild in range(gc.getNumBuildInfos()):
			pLoopBuild = gc.getBuildInfo(eLoopBuild)
			if (pLoopBuild.getRoute() == self.eRoute):
				ePrereqTech = pLoopBuild.getTechPrereq()
				if (ePrereqTech >= 0):
					if (not bFirst):
						screen.attachLabel(self.REQUIRES_PANEL_ID, "", localText.getText("TXT_KEY_AND", ()))
					
					bFirst = False
					screen.attachImageButton(self.REQUIRES_PANEL_ID, "", gc.getTechInfo(ePrereqTech).getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_TECH, ePrereqTech, -1, False)
		
		# Bonuses
		eBonus = info.getPrereqBonus()
		if (eBonus >= 0):
			if (not bFirst):
				screen.attachLabel(self.REQUIRES_PANEL_ID, "", localText.getText("TXT_KEY_AND", ()))
			sButton = gc.getBonusInfo(eBonus).getButton()
			sButton = sButton[:-4] + "_x64.dds"
			bFirst = False
			screen.attachImageButton(self.REQUIRES_PANEL_ID, "", sButton, self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_BONUS, eBonus, -1, False)
		
		iNumPrereqOrBonuses = 0
		
		for iPrereq in range(gc.getNUM_ROUTE_PREREQ_OR_BONUSES()):
			eLoopBonus = info.getPrereqOrBonus(iPrereq)
			if (eLoopBonus >= 0):
				iNumPrereqOrBonuses += 1
		
		if (not bFirst and iNumPrereqOrBonuses > 0):
			bFirst = False
			
			screen.attachLabel(self.REQUIRES_PANEL_ID, "", localText.getText("TXT_KEY_AND", ()))
			
			if (iNumPrereqOrBonuses > 1):
				screen.attachLabel(self.REQUIRES_PANEL_ID, "", "( ")
		
		for iPrereq in range(gc.getNUM_ROUTE_PREREQ_OR_BONUSES()):
			eLoopBonus = info.getPrereqOrBonus(iPrereq)
			if (eLoopBonus >= 0):
				sButton = gc.getBonusInfo(eLoopBonus).getButton()
				sButton = sButton[:-4] + "_x64.dds"
				if (iPrereq > 0):
					screen.attachLabel(self.REQUIRES_PANEL_ID, "", localText.getText("TXT_KEY_OR", ()))
				screen.attachImageButton(self.REQUIRES_PANEL_ID, "", sButton, self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_BONUS, eLoopBonus, -1, False)
		
		if (iNumPrereqOrBonuses > 1):
			screen.attachLabel(self.REQUIRES_PANEL_ID, "", " ) ")
	
	"""
	def drawStats(self, screen, info, ePlayer):
		screen.addListBoxGFC(self.record(self.STATS_ID), "", self.STATS_X, self.STATS_Y, self.STATS_W, self.STATS_H, TableStyles.TABLE_STYLE_EMPTY)
		screen.enableSelect(self.STATS_ID, False)
		
		iMovementCost = info.getMovementCost()
		iFlatMovementCost = info.getFlatMovementCost()
		
		if (iMovementCost > 0):
			iMoves = (gc.getMOVE_DENOMINATOR() / iMovementCost)
			if ((iMoves * iMovementCost) < gc.getMOVE_DENOMINATOR()):
				iMoves += 1
		else:
			iMoves = gc.getMOVE_DENOMINATOR()
		
		if (iFlatMovementCost > 0):
			iFlatMoves = (gc.getMOVE_DENOMINATOR() / iFlatMovementCost)
			if ((iFlatMoves * iFlatMovementCost) < gc.getMOVE_DENOMINATOR()):
				iFlatMoves += 1
		else:
			iFlatMoves = gc.getMOVE_DENOMINATOR()
		
		if ((iMoves > 1) or (iFlatMoves > 1)):
			if (iMoves >= iFlatMoves):
				screen.appendListBoxStringNoUpdate(self.STATS_ID, u"<font=3>" + localText.getText("TXT_KEY_ACTION_MOVEMENT_COST", (iMoves,))[1:] + u"</font>", WidgetTypes.WIDGET_GENERAL, 0, 0, CvUtil.FONT_LEFT_JUSTIFY)
			else:
				screen.appendListBoxStringNoUpdate(self.STATS_ID, u"<font=3>" + localText.getText("TXT_KEY_ACTION_FLAT_MOVEMENT_COST", (iFlatMoves,))[1:] + u"</font>", WidgetTypes.WIDGET_GENERAL, 0, 0, CvUtil.FONT_LEFT_JUSTIFY)
		
		screen.updateListBox(self.STATS_ID)
	"""
	