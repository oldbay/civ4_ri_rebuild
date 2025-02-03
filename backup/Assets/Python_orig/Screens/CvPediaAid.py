## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
from CvPythonExtensions import *
import CvUtil
import CvPediaPage

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

class CvPediaAid(CvPediaPage.CvPediaPage):
	"Civilopedia Screen for Aid"

	def __init__(self, section):
		super(CvPediaAid, self).__init__(section, CivilopediaPageTypes.CIVILOPEDIA_PAGE_AID, WidgetTypes.WIDGET_PEDIA_JUMP_TO_AID, "Aid")
		
		self.eAid = -1
		
		self.PROMOTIONS_PANEL_ID = self.szWidget + "PromotionsPanel"
		self.SUPPLIED_BY_PANEL_ID = self.szWidget + "SuppliedByPanel"
		self.SUPPLIED_BY_LIST_ID = self.szWidget + "SuppliedByList"
	
	def getData(self):
		return (self.eAid, -1)
	
	def setData(self, eAid, iPageData2):
		if (self.eAid != eAid):
			self.clear()
			self.eAid = eAid
	
	def getInfo(self):
		return gc.getAidInfo(self.eAid)
	
	def calculateLayout(self):
		self.calculateTitleLayout()
		self.calculateSynopsisLayout()
		self.calculateTwoColumnLayout(self.SYNOPSIS_PANEL_W, 0)
		
		self.SPECIAL_PANEL_X = self.COLUMN2_X
		self.SPECIAL_PANEL_Y = self.PAGE_CONTENT_Y
		self.SPECIAL_PANEL_W = self.COLUMN2_W
		self.SPECIAL_PANEL_H = self.SYNOPSIS_PANEL_H
		
		self.PROMOTIONS_PANEL_X = self.main.PAGE_AREA_X
		self.PROMOTIONS_PANEL_W = self.main.PAGE_AREA_W
		self.PROMOTIONS_PANEL_Y = self.SYNOPSIS_PANEL_Y + self.SYNOPSIS_PANEL_H + self.main.COMMON_MARGIN_H
		self.PROMOTIONS_PANEL_H = self.main.COMMON_ICON_PANEL_SIZE
		
		if (self.main.COMMON_ICON_SIZE == GenericButtonSizes.BUTTON_SIZE_46):
			self.SUPPLIER_ICON_SIZE = 46
		else:
			self.SUPPLIER_ICON_SIZE = 64
		
		self.SUPPLIED_BY_PANEL_X = self.main.PAGE_AREA_X
		self.SUPPLIED_BY_PANEL_W = self.main.PAGE_AREA_W
		self.SUPPLIED_BY_PANEL_Y = self.PROMOTIONS_PANEL_Y + self.PROMOTIONS_PANEL_H + self.main.COMMON_MARGIN_H
		self.SUPPLIED_BY_PANEL_H = self.main.PAGE_AREA_Y + self.main.PAGE_AREA_H - self.SUPPLIED_BY_PANEL_Y
	
	def draw(self):
		if (not self.isEmpty()):
			return
		
		screen = self.getScreen()
		info = self.getInfo()
		ePlayer = gc.getGame().getActivePlayer()
		
		self.drawTitle(screen, info, ePlayer)
		self.drawSynopsis(screen, info, ePlayer)
		self.drawPromotions(screen, info)
		self.drawSuppliedBy(screen, ePlayer)
		self.drawSpecial(screen, CyGameTextMgr().getAidHelp(self.eAid, True)[1:])
	
	def drawPromotions(self, screen, info):
		screen.addPanel(self.record(self.PROMOTIONS_PANEL_ID), localText.getText("TXT_KEY_PEDIA_CATEGORY_PROMOTION", ()), "", False, True, self.PROMOTIONS_PANEL_X, self.PROMOTIONS_PANEL_Y, self.PROMOTIONS_PANEL_W, self.PROMOTIONS_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		screen.attachLabel(self.PROMOTIONS_PANEL_ID, "", "  ")
		for j in range(info.getNumPromotions()):
			ePromotion = info.getPromotions(j)
			screen.attachImageButton(self.PROMOTIONS_PANEL_ID, "", gc.getPromotionInfo(ePromotion).getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_PROMOTION, ePromotion, -1, False)
	
	def drawSuppliedBy(self, screen, ePlayer):
		screen.addPanel(self.record(self.SUPPLIED_BY_PANEL_ID), localText.getText("TXT_KEY_AID_SUPPLIED_BY", ()), "", False, True, self.SUPPLIED_BY_PANEL_X, self.SUPPLIED_BY_PANEL_Y, self.SUPPLIED_BY_PANEL_W, self.SUPPLIED_BY_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		screen.addMultiListControlGFC(self.record(self.SUPPLIED_BY_LIST_ID), "", self.SUPPLIED_BY_PANEL_X+15, self.SUPPLIED_BY_PANEL_Y+40, self.SUPPLIED_BY_PANEL_W-20, self.SUPPLIED_BY_PANEL_H-40, 1, self.SUPPLIER_ICON_SIZE, self.SUPPLIER_ICON_SIZE, TableStyles.TABLE_STYLE_STANDARD)
		
		if (ePlayer != -1):
			pCivilization = gc.getCivilizationInfo(gc.getPlayer(ePlayer).getCivilizationType())
		else:
			pCivilization = None
		
		for eLoopUnitClass in range(gc.getNumUnitClassInfos()):
			pUnitClassInfo = gc.getUnitClassInfo(eLoopUnitClass)
			if (pCivilization != None):
				eLoopUnit = pCivilization.getCivilizationUnits(eLoopUnitClass)
			else:
				eLoopUnit = pUnitClassInfo.getRepresentativeUnit()
			
			if (eLoopUnit > -1):
				pLoopUnit = gc.getUnitInfo(eLoopUnit)
				for i in range(pUnitClassInfo.getNumAidTypes()):
					eLoopAid = pUnitClassInfo.getAidTypes(i)
					if eLoopAid == self.eAid:
						if ePlayer != -1:
							szButton = gc.getPlayer(ePlayer).getUnitButton(eLoopUnit)
						else:
							szButton = pLoopUnit.getButton()
						screen.appendMultiListButton(self.SUPPLIED_BY_LIST_ID, szButton, 0, WidgetTypes.WIDGET_PEDIA_JUMP_TO_UNIT, eLoopUnit, -1, False)
						break
		
		for eLoopImprovement in range(gc.getNumImprovementInfos()):
			if (eLoopImprovement != -1):
				pImprovementInfo = gc.getImprovementInfo(eLoopImprovement)
				for i in range(pImprovementInfo.getNumAidTypes()):
					eLoopAid = pImprovementInfo.getAidTypes(i)
					if eLoopAid == self.eAid:
						screen.appendMultiListButton(self.SUPPLIED_BY_LIST_ID, pImprovementInfo.getButton(), 0, WidgetTypes.WIDGET_PEDIA_JUMP_TO_IMPROVEMENT, eLoopImprovement, -1, False)
						break
	