## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
from CvPythonExtensions import *
import CvUtil
import CvPediaPage

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

class CvPediaTech(CvPediaPage.CvPediaPage):
	"Civilopedia Screen for Techs"

	def __init__(self, section):
		super(CvPediaTech, self).__init__(section, CivilopediaPageTypes.CIVILOPEDIA_PAGE_TECHNOLOGY, WidgetTypes.WIDGET_PEDIA_JUMP_TO_TECH, "Tech")
		
		self.eTech = -1
		
		self.COST_ID = self.szWidget + "Cost"
		self.QUOTE_PANEL_ID = self.szWidget + "QuotePanel"
		self.QUOTE_TEXT_ID = self.szWidget + "QuoteText"
		self.PREREQ_PANEL_ID = self.szWidget + "PrereqPanel"
		self.LEADS_TO_PANEL_ID = self.szWidget + "LeadsToPanel"
		self.UNIT_PANEL_ID = self.szWidget + "UnitsPanel"
		self.BUILDING_PANEL_ID = self.szWidget + "BuildingsPanel"
		self.SPECIAL_PANEL_ID = self.szWidget + "SpecialPanel"
		self.SPECIAL_TEXT_ID = self.szWidget + "SpecialText"
	
	def getData(self):
		return (self.eTech, -1)
	
	def setData(self, eTech, iPageData2):
		if (self.eTech != eTech):
			self.clear()
			self.eTech = eTech
	
	def getInfo(self):
		return gc.getTechInfo(self.eTech)
	
	def calculateLayout(self):
		self.calculateTitleLayout()
		self.calculateTwoColumnLayout(368, 0)
		self.calculateSynopsisLayout(self.COLUMN1_W)
		
		self.COST_X = self.STATS_X
		self.COST_Y = self.STATS_Y
		
		self.QUOTE_PANEL_X = self.COLUMN2_X
		self.QUOTE_PANEL_Y = self.PAGE_CONTENT_Y
		self.QUOTE_PANEL_W = self.COLUMN2_W
		self.QUOTE_PANEL_H = self.SYNOPSIS_PANEL_H
		
		self.PREREQ_PANEL_X = self.COLUMN1_X
		self.PREREQ_PANEL_Y = self.SYNOPSIS_PANEL_Y + self.SYNOPSIS_PANEL_H + self.main.COMMON_MARGIN_H
		self.PREREQ_PANEL_W = self.COLUMN1_W
		self.PREREQ_PANEL_H = self.main.COMMON_ICON_PANEL_SIZE
		
		self.LEADS_TO_PANEL_X = self.COLUMN2_X
		self.LEADS_TO_PANEL_Y = self.QUOTE_PANEL_Y + self.QUOTE_PANEL_H + self.main.COMMON_MARGIN_H
		self.LEADS_TO_PANEL_W = self.COLUMN2_W
		self.LEADS_TO_PANEL_H = self.main.COMMON_ICON_PANEL_SIZE
		
		self.BUILDING_PANEL_X = self.COLUMN1_X
		self.BUILDING_PANEL_Y = self.PREREQ_PANEL_Y + self.PREREQ_PANEL_H + self.main.COMMON_MARGIN_H
		self.BUILDING_PANEL_W = self.COLUMN1_W
		self.BUILDING_PANEL_H = self.main.COMMON_ICON_PANEL_SIZE
		
		self.UNIT_PANEL_X = self.COLUMN2_X
		self.UNIT_PANEL_Y = self.LEADS_TO_PANEL_Y + self.LEADS_TO_PANEL_H + self.main.COMMON_MARGIN_H
		self.UNIT_PANEL_W = self.COLUMN2_W
		self.UNIT_PANEL_H = self.main.COMMON_ICON_PANEL_SIZE
		
		self.SPECIAL_PANEL_X = self.COLUMN1_X
		self.SPECIAL_PANEL_Y = self.BUILDING_PANEL_Y + self.BUILDING_PANEL_H + self.main.COMMON_MARGIN_H
		self.SPECIAL_PANEL_W = self.COLUMN1_W
		self.SPECIAL_PANEL_H = self.main.PAGE_AREA_H - (self.SPECIAL_PANEL_Y - self.main.PAGE_AREA_Y)
		
		self.CIVILOPEDIA_PANEL_X = self.COLUMN2_X
		self.CIVILOPEDIA_PANEL_Y = self.UNIT_PANEL_Y + self.UNIT_PANEL_H + self.main.COMMON_MARGIN_H
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
		self.drawPrereqs(screen, info)
		self.drawLeadsTo(screen)
		self.drawUnits(screen, ePlayer)
		self.drawBuildings(screen, ePlayer)
		self.drawSpecial(screen, CyGameTextMgr().getTechHelp(self.eTech, True, False, False, False, -1)[1:])
		self.drawQuote(screen, info)
		self.drawCivilopedia(screen, info, "TXT_KEY_CIVILOPEDIA_HISTORY")
	
	def drawStats(self, screen, info, ePlayer):
		if (ePlayer == -1):
			szCostText = localText.getText("TXT_KEY_PEDIA_COST", ( info.getResearchCost(), ) ) + u"%c" % (gc.getCommerceInfo(CommerceTypes.COMMERCE_RESEARCH).getChar())
		else:
			szCostText = localText.getText("TXT_KEY_PEDIA_COST", ( gc.getTeam(gc.getGame().getActiveTeam()).getResearchCost(self.eTech), ) ) + u"%c" % (gc.getCommerceInfo(CommerceTypes.COMMERCE_RESEARCH).getChar())
		screen.setLabel(self.record(self.COST_ID), "Background", u"<font=4>" + szCostText.upper() + u"</font>", CvUtil.FONT_LEFT_JUSTIFY, self.COST_X + 25, self.COST_Y, 0, FontTypes.TITLE_FONT, WidgetTypes.WIDGET_GENERAL, -1, -1)
	
	def drawPrereqs(self, screen, info):
		szRequires = localText.getText("TXT_KEY_PEDIA_REQUIRES", ())
		
		screen.addPanel(self.record(self.PREREQ_PANEL_ID), szRequires, "", false, true, self.PREREQ_PANEL_X, self.PREREQ_PANEL_Y, self.PREREQ_PANEL_W, self.PREREQ_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		
		screen.attachLabel(self.PREREQ_PANEL_ID, "", "  ")
		
		bFirst = True
		for j in range(gc.getNUM_AND_TECH_PREREQS()):
			eTech = info.getPrereqAndTechs(j)
			if (eTech > -1):
				if (not bFirst):
					screen.attachLabel(self.PREREQ_PANEL_ID, "", localText.getText("TXT_KEY_AND", ()))
				else:
					bFirst = False
				screen.attachImageButton(self.PREREQ_PANEL_ID, "", gc.getTechInfo(eTech).getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_REQUIRED_TECH, eTech, j, False)
		
		# count the number of OR techs
		nOrTechs = 0
		for j in range(gc.getNUM_OR_TECH_PREREQS()):
			if (info.getPrereqOrTechs(j) > -1):
				nOrTechs += 1
		
		szLeftDelimeter = ""
		szRightDelimeter = ""
		#  Display a bracket if we have more than one OR tech and at least one AND tech
		if (not bFirst):
			if (nOrTechs > 1):
				szLeftDelimeter = localText.getText("TXT_KEY_AND", ()) + "( "
				szRightDelimeter = " ) "
			elif (nOrTechs > 0):
				szLeftDelimeter = localText.getText("TXT_KEY_AND", ())
			else:
				return

		if len(szLeftDelimeter) > 0:
			screen.attachLabel(self.PREREQ_PANEL_ID, "", szLeftDelimeter)
			
		bFirst = True
		for j in range(gc.getNUM_OR_TECH_PREREQS()):
			eTech = info.getPrereqOrTechs(j)
			if (eTech > -1):
				if (not bFirst):
					screen.attachLabel(self.PREREQ_PANEL_ID, "", localText.getText("TXT_KEY_OR", ()))
				else:
					bFirst = False
				screen.attachImageButton(self.PREREQ_PANEL_ID, "", gc.getTechInfo(eTech).getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_REQUIRED_TECH, eTech, j, False)					
			
		if len(szRightDelimeter) > 0:
			screen.attachLabel(self.PREREQ_PANEL_ID, "", szRightDelimeter)

	def drawLeadsTo(self, screen):
		screen = self.getScreen()
		
		# add pane and text
		szLeadsTo = localText.getText("TXT_KEY_PEDIA_LEADS_TO", ())
		
		screen.addPanel(self.record(self.LEADS_TO_PANEL_ID), szLeadsTo, "", false, true, self.LEADS_TO_PANEL_X, self.LEADS_TO_PANEL_Y, self.LEADS_TO_PANEL_W, self.LEADS_TO_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50 )
		
		screen.attachLabel(self.LEADS_TO_PANEL_ID, "", "  ")
		
		for j in range(gc.getNumTechInfos()):
			for k in range(gc.getNUM_OR_TECH_PREREQS()):
				iPrereq = gc.getTechInfo(j).getPrereqOrTechs(k)
				if (iPrereq == self.eTech):
					screen.attachImageButton( self.LEADS_TO_PANEL_ID, "", gc.getTechInfo(j).getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_DERIVED_TECH, j, self.eTech, False )
			for k in range(gc.getNUM_AND_TECH_PREREQS()):
				iPrereq = gc.getTechInfo(j).getPrereqAndTechs(k)
				if (iPrereq == self.eTech):
					screen.attachImageButton( self.LEADS_TO_PANEL_ID, "", gc.getTechInfo(j).getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_DERIVED_TECH, j, self.eTech, False )
	
	def drawUnits(self, screen, ePlayer):
		screen.addPanel(self.record(self.UNIT_PANEL_ID), localText.getText("TXT_KEY_PEDIA_UNITS_ENABLED", ()), "", false, true, self.UNIT_PANEL_X, self.UNIT_PANEL_Y, self.UNIT_PANEL_W, self.UNIT_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		
		screen.attachLabel(self.UNIT_PANEL_ID, "", "  ")
		
		if (ePlayer != -1):
			pCivilization = gc.getCivilizationInfo(gc.getPlayer(ePlayer).getCivilizationType())
		else:
			pCivilization = None
		
		for eLoopUnitClass in range(gc.getNumUnitClassInfos()):
			if (pCivilization != None):
				eLoopUnit = pCivilization.getCivilizationUnits(eLoopUnitClass)
			else:
				eLoopUnit = gc.getUnitClassInfo(eLoopUnitClass).getRepresentativeUnit()
			
			if (eLoopUnit != -1):
				if (isTechRequiredForUnit(self.eTech, eLoopUnit)):
					if ePlayer != -1:
						szButton = gc.getPlayer(ePlayer).getUnitButton(eLoopUnit)
					else:
						szButton = gc.getUnitInfo(eLoopUnit).getButton()
					screen.attachImageButton(self.UNIT_PANEL_ID, "", szButton, self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_UNIT, eLoopUnit, 1, False)
	
	def drawBuildings(self, screen, ePlayer):
		screen.addPanel(self.record(self.BUILDING_PANEL_ID), localText.getText("TXT_KEY_PEDIA_BUILDINGS_ENABLED", ()), "", false, true, self.BUILDING_PANEL_X, self.BUILDING_PANEL_Y, self.BUILDING_PANEL_W, self.BUILDING_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		
		screen.attachLabel(self.BUILDING_PANEL_ID, "", "  ")
		
		if (ePlayer != -1):
			pCivilization = gc.getCivilizationInfo(gc.getPlayer(ePlayer).getCivilizationType())
		else:
			pCivilization = None
		
		for eLoopBuildingClass in range(gc.getNumBuildingClassInfos()):
			if (pCivilization != None):
				eLoopBuilding = pCivilization.getCivilizationBuildings(eLoopBuildingClass)
			else:
				eLoopBuilding = gc.getBuildingClassInfo(eLoopBuildingClass).getRepresentativeBuilding()
			
			if (eLoopBuilding != -1):
				if (isTechRequiredForBuilding(self.eTech, eLoopBuilding)):
					screen.attachImageButton(self.BUILDING_PANEL_ID, "", gc.getBuildingInfo(eLoopBuilding).getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_BUILDING, eLoopBuilding, 1, False)
		
		for eLoopProject in range(gc.getNumProjectInfos()):
			if (isTechRequiredForProject(self.eTech, eLoopProject)):
				screen.attachImageButton(self.BUILDING_PANEL_ID, "", gc.getProjectInfo(eLoopProject).getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_PROJECT, eLoopProject, 1, False)

	"""
	def drawSpecial(self, screen):
		screen.addPanel( self.record(self.SPECIAL_PANEL_ID), localText.getText("TXT_KEY_PEDIA_SPECIAL_ABILITIES", ()), "", true, false, self.SPECIAL_PANEL_X, self.SPECIAL_PANEL_Y, self.SPECIAL_PANEL_W, self.SPECIAL_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50 )
		
		szSpecialText = CyGameTextMgr().getTechHelp(self.eTech, True, False, False, False, -1)[1:]
		screen.addMultilineText(self.record(self.SPECIAL_TEXT_ID), szSpecialText, self.SPECIAL_PANEL_X+5, self.SPECIAL_PANEL_Y+30, self.SPECIAL_PANEL_W-35, self.SPECIAL_PANEL_H-10, WidgetTypes.WIDGET_GENERAL, -1, -1, CvUtil.FONT_LEFT_JUSTIFY)
	"""

	def drawQuote(self, screen, info):
		screen.addPanel(self.record(self.QUOTE_PANEL_ID), "", "", true, true, self.QUOTE_PANEL_X, self.QUOTE_PANEL_Y, self.QUOTE_PANEL_W, self.QUOTE_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		
		szQuote = info.getQuote()
		#szQuote += u"\n\n" + info.getCivilopedia()
		
		screen.addMultilineText( self.record(self.QUOTE_TEXT_ID), szQuote, self.QUOTE_PANEL_X + 15, self.QUOTE_PANEL_Y + 15, self.QUOTE_PANEL_W - (15 * 2), self.QUOTE_PANEL_H - (15 * 2), WidgetTypes.WIDGET_GENERAL, -1, -1, CvUtil.FONT_LEFT_JUSTIFY)
	
	"""
	def placeLinks(self, bRedraw):
		screen = self.top.getScreen()

		if bRedraw:	
			screen.clearListBoxGFC(self.top.LIST_ID)
		
		techsList = self.getSortedList( gc.getNumTechInfos(), gc.getTechInfo )
		
		iSelected = 0			
		i = 0
		for iI in range(gc.getNumTechInfos()):
			if (not gc.getTechInfo(techsList[iI][1]).isGraphicalOnly()): 
				if bRedraw:
					screen.appendListBoxStringNoUpdate(self.top.LIST_ID, techsList[iI][0], WidgetTypes.WIDGET_PEDIA_JUMP_TO_TECH, techsList[iI][1], 0, CvUtil.FONT_LEFT_JUSTIFY )
				if techsList[iI][1] == self.eTech:
					iSelected = i
				i += 1		
				
		if bRedraw:
			screen.updateListBox(self.top.LIST_ID)	

		screen.setSelectedListBoxStringGFC(self.top.LIST_ID, iSelected)

	# Will handle the input for this screen...
	def handleInput (self, inputClass):
		return 0
	"""
