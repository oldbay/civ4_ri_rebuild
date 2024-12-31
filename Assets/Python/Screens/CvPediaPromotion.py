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

class CvPediaPromotion(CvPediaPage.CvPediaPage):
	"Civilopedia Screen for Promotions"

	def __init__(self, section):
		super(CvPediaPromotion, self).__init__(section, CivilopediaPageTypes.CIVILOPEDIA_PAGE_PROMOTION, WidgetTypes.WIDGET_PEDIA_JUMP_TO_PROMOTION, "Promotion")
		
		self.ePromotion = -1
		
		self.PREREQ_PANEL_ID = self.szWidget + "PrereqPanel"
		self.LEADS_TO_PANEL_ID = self.szWidget + "LeadsToPanel"
		self.UNIT_GROUP_PANEL_ID = self.szWidget + "UnitGroupPanel"
		self.UNIT_GROUP_ID = self.szWidget + "UnitGroups"
	
	def getData(self):
		return (self.ePromotion, -1)
	
	def setData(self, ePromotion, iPageData2):
		if (self.ePromotion != ePromotion):
			self.clear()
			self.ePromotion = ePromotion

	def calculateLayout(self):
		self.calculateTitleLayout()
		self.calculateSynopsisLayout(-1, self.main.COMMON_ICON_PANEL_SIZE * 2 + self.main.COMMON_MARGIN_H)
		self.calculateTwoColumnLayout(self.SYNOPSIS_PANEL_W, 0)
		
		self.PREREQ_PANEL_X = self.COLUMN2_X
		self.PREREQ_PANEL_Y = self.PAGE_CONTENT_Y
		self.PREREQ_PANEL_W = self.COLUMN2_W
		self.PREREQ_PANEL_H = self.main.COMMON_ICON_PANEL_SIZE
		
		self.LEADS_TO_PANEL_X = self.COLUMN2_X
		self.LEADS_TO_PANEL_Y = self.PREREQ_PANEL_Y + self.PREREQ_PANEL_H + self.main.COMMON_MARGIN_H
		self.LEADS_TO_PANEL_W = self.COLUMN2_W
		self.LEADS_TO_PANEL_H = self.main.COMMON_ICON_PANEL_SIZE
		
		self.SPECIAL_PANEL_X = self.main.PAGE_AREA_X
		self.SPECIAL_PANEL_Y = self.SYNOPSIS_PANEL_Y + self.SYNOPSIS_PANEL_H + self.main.COMMON_MARGIN_H
		self.SPECIAL_PANEL_W = self.main.PAGE_AREA_W
		self.SPECIAL_PANEL_H = self.main.PAGE_AREA_Y + self.main.PAGE_AREA_H - self.SPECIAL_PANEL_Y
		
		self.SPECIAL_PANEL_X = self.COLUMN2_X
		self.SPECIAL_PANEL_Y = self.LEADS_TO_PANEL_Y + self.LEADS_TO_PANEL_H + self.main.COMMON_MARGIN_H
		self.SPECIAL_PANEL_W = self.COLUMN2_W
		self.SPECIAL_PANEL_H = self.main.PAGE_AREA_Y + self.main.PAGE_AREA_H - self.SPECIAL_PANEL_Y
		
		self.UNIT_GROUP_PANEL_X = self.COLUMN1_X
		self.UNIT_GROUP_PANEL_Y = self.SYNOPSIS_PANEL_Y + self.SYNOPSIS_PANEL_H + self.main.COMMON_MARGIN_H
		self.UNIT_GROUP_PANEL_W = self.COLUMN1_W
		self.UNIT_GROUP_PANEL_H = self.main.PAGE_AREA_Y + self.main.PAGE_AREA_H - self.UNIT_GROUP_PANEL_Y
		
#		self.ITEMS_MARGIN = 18
#		self.ITEMS_SEPARATION = 2

	def draw(self):
		if (not self.isEmpty()):
			return
		
		info = gc.getPromotionInfo(self.ePromotion)
		screen = self.getScreen()
		ePlayer = gc.getGame().getActivePlayer()
		
		self.drawTitle(screen, info, ePlayer)
		self.drawSynopsis(screen, info, ePlayer)
		
		# Place required promotions
		self.drawPrereqs(screen, info)
		
		# Place allowing promotions
		self.drawLeadsTo(screen)
		
		# Place the special abilities block
		self.drawSpecial(screen, CyGameTextMgr().getPromotionHelp(self.ePromotion, True)[1:])
		
		# Place unit groups
		self.drawUnitGroups(screen, info)
	
	def drawPrereqs(self, screen, info):
		screen.addPanel( self.record(self.PREREQ_PANEL_ID), localText.getText("TXT_KEY_PEDIA_REQUIRES", ()), "", false, true,
				 self.PREREQ_PANEL_X, self.PREREQ_PANEL_Y, self.PREREQ_PANEL_W, self.PREREQ_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50 )
		
		screen.attachLabel(self.PREREQ_PANEL_ID, "", "  ")

		ePromo = info.getPrereqPromotion()
		if (ePromo > -1):
			screen.attachImageButton( self.PREREQ_PANEL_ID, "", gc.getPromotionInfo(ePromo).getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_PROMOTION, ePromo, 1, False )

		ePromoOr1 = info.getPrereqOrPromotion1()
		ePromoOr2 = info.getPrereqOrPromotion2()
		ePromoOr3 = info.getPrereqOrPromotion3() # K-Mod newline
		if (ePromoOr1 > -1):
			if (ePromo > -1):
				screen.attachLabel(self.PREREQ_PANEL_ID, "", localText.getText("TXT_KEY_AND", ()))
				
				if (ePromoOr2 > -1):
					screen.attachLabel(self.PREREQ_PANEL_ID, "", "(")
			
			screen.attachImageButton( self.PREREQ_PANEL_ID, "", gc.getPromotionInfo(ePromoOr1).getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_PROMOTION, ePromoOr1, 1, False )
			
			if (ePromoOr2 > -1):
				screen.attachLabel(self.PREREQ_PANEL_ID, "", localText.getText("TXT_KEY_OR", ()))
				screen.attachImageButton( self.PREREQ_PANEL_ID, "", gc.getPromotionInfo(ePromoOr2).getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_PROMOTION, ePromoOr2, 1, False )
			
				if (ePromo > -1 and ePromoOr3 < 0): # K-Mod added second condition
					screen.attachLabel(self.PREREQ_PANEL_ID, "", ")")
			
			# K-Mod new block
			if (ePromoOr3 > -1):
				screen.attachLabel(self.PREREQ_PANEL_ID, "", localText.getText("TXT_KEY_OR", ()))
				screen.attachImageButton( self.PREREQ_PANEL_ID, "", gc.getPromotionInfo(ePromoOr3).getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_PROMOTION, ePromoOr3, 1, False )
				
				if (ePromo > -1):
					screen.attachLabel(self.PREREQ_PANEL_ID, "", ")")
			# K-Mod end
		
		eTech = info.getTechPrereq()
		if (eTech > -1):
			screen.attachImageButton( self.PREREQ_PANEL_ID, "", gc.getTechInfo(eTech).getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_TECH, eTech, 1, False )		
						
		eReligion = info.getStateReligionPrereq()
		if (eReligion > -1):
			screen.attachImageButton( self.PREREQ_PANEL_ID, "", gc.getReligionInfo(eReligion).getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_RELIGION, eReligion, 1, False )

	def drawLeadsTo(self, screen):
		screen.addPanel( self.record(self.LEADS_TO_PANEL_ID), localText.getText("TXT_KEY_PEDIA_LEADS_TO", ()), "", false, true,
				 self.LEADS_TO_PANEL_X, self.LEADS_TO_PANEL_Y, self.LEADS_TO_PANEL_W, self.LEADS_TO_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50 )
		
		screen.attachLabel(self.LEADS_TO_PANEL_ID, "", "  ")
		
		for j in range(gc.getNumPromotionInfos()):
			relatedPromo = gc.getPromotionInfo(j)
			if (relatedPromo.getPrereqPromotion() == self.ePromotion or relatedPromo.getPrereqOrPromotion1() == self.ePromotion or relatedPromo.getPrereqOrPromotion2() == self.ePromotion or relatedPromo.getPrereqOrPromotion3() == self.ePromotion):
				screen.attachImageButton( self.LEADS_TO_PANEL_ID, "", relatedPromo.getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_PROMOTION, j, 1, False )
	
	"""
	def drawSpecial(self, screen):
		screen.addPanel( self.record(self.SPECIAL_PANEL_ID), localText.getText("TXT_KEY_PEDIA_SPECIAL_ABILITIES", ()), "", true, false,
				 self.SPECIAL_PANEL_X, self.SPECIAL_PANEL_Y, self.SPECIAL_PANEL_W, self.SPECIAL_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50 )
		
		szSpecialText = CyGameTextMgr().getPromotionHelp(self.ePromotion, True)[1:]
		screen.addMultilineText(self.SPECIAL_ID, szSpecialText, self.SPECIAL_PANEL_X+5, self.SPECIAL_PANEL_Y+30, self.SPECIAL_PANEL_W-10, self.SPECIAL_PANEL_H-35, WidgetTypes.WIDGET_GENERAL, -1, -1, CvUtil.FONT_LEFT_JUSTIFY)	
	"""

	def drawUnitGroups(self, screen, info):
		# Unit Group Panel
		screen.addPanel( self.record(self.UNIT_GROUP_PANEL_ID), localText.getText("TXT_KEY_PEDIA_PROMOTION_UNITS", ()), "", true, true, self.UNIT_GROUP_PANEL_X, self.UNIT_GROUP_PANEL_Y, self.UNIT_GROUP_PANEL_W, self.UNIT_GROUP_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50 )
		
		# Unit Groups
		screen.addTableControlGFC(self.record(self.UNIT_GROUP_ID), 1, self.UNIT_GROUP_PANEL_X + 10, self.UNIT_GROUP_PANEL_Y + 40, self.UNIT_GROUP_PANEL_W - 20, self.UNIT_GROUP_PANEL_H - 50, False, False, 24, 24, TableStyles.TABLE_STYLE_EMPTY)
		
		i = 0
		for iI in range(gc.getNumUnitCombatInfos()):
			if (0 != info.getUnitCombat(iI)):
				iRow = screen.appendTableRow(self.UNIT_GROUP_ID)
				unitCombat = gc.getUnitCombatInfo(iI)
				screen.setTableText(self.UNIT_GROUP_ID, 0, i, u"<font=2>" + unitCombat.getDescription() + u"</font>", unitCombat.getButton(), WidgetTypes.WIDGET_PEDIA_JUMP_TO_UNIT_COMBAT, iI, -1, CvUtil.FONT_LEFT_JUSTIFY)
				i += 1

	"""
	def placeLinks(self, bRedraw):

		screen = self.top.getScreen()

		if bRedraw:		               
			screen.clearListBoxGFC(self.top.LIST_ID)
		
		# sort techs alphabetically
		listSorted=[(0,0)]*gc.getNumPromotionInfos()
		for j in range(gc.getNumPromotionInfos()):
			listSorted[j] = (gc.getPromotionInfo(j).getDescription(), j)
		listSorted.sort()
			
		i = 0
		iSelected = 0
		for iI in range(gc.getNumPromotionInfos()):
			if (not gc.getPromotionInfo(listSorted[iI][1]).isGraphicalOnly()):
				if bRedraw:
					screen.appendListBoxStringNoUpdate( self.top.LIST_ID, listSorted[iI][0], WidgetTypes.WIDGET_PEDIA_JUMP_TO_PROMOTION, listSorted[iI][1], 0, CvUtil.FONT_LEFT_JUSTIFY )
				if listSorted[iI][1] == self.ePromotion:
					iSelected = i			
				i += 1
				
		if bRedraw:
			screen.updateListBox(self.top.LIST_ID)

		screen.setSelectedListBoxStringGFC(self.top.LIST_ID, iSelected)

	# Will handle the input for this screen...
	def handleInput (self, inputClass):
		return 0
	"""

