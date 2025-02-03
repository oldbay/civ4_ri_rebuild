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

class CvPediaUnit(CvPediaPage.CvPediaPage):
	"Civilopedia Screen for Units"

	def __init__(self, section):
		super(CvPediaUnit, self).__init__(section, CivilopediaPageTypes.CIVILOPEDIA_PAGE_UNIT, WidgetTypes.WIDGET_PEDIA_JUMP_TO_UNIT, "Unit")
		
		self.eUnit = -1
		self.eCivilization = -1
		
		self.REQUIRES_PANEL_ID = self.szWidget + "RequiresPanel"
		self.UPGRADES_TO_PANEL_ID = self.szWidget + "UpgradesToPanel"
		self.PROMOTIONS_PANEL_ID = self.szWidget + "PromotionsPanel"
		self.PROMOTIONS_LIST_ID = self.szWidget + "PromotionsList"
		self.ANIMATION_BACKGROUND_ID = self.szWidget + "AnimationBackground"
	
	def getData(self):
		return (self.eUnit, -1)
	
	def setData(self, eUnit, iPageData2):
		if (self.eUnit != eUnit):
			self.clear()
			self.eUnit = eUnit

	def setCivilization(self, eCivilization):
		if (self.eCivilization == eCivilization):
			return
		
		self.eCivilization = eCivilization
	
	def getInfo(self):
		return gc.getUnitInfo(self.eUnit)
	
	def getButton(self, info, ePlayer):
		if (self.eCivilization != -1):
			return gc.getUnitInfo(self.eUnit).getStyledButton(self.eCivilization)
		elif (ePlayer != -1):
			return gc.getPlayer(ePlayer).getUnitButton(self.eUnit)
		else:
			return self.getInfo().getButton()

	def calculateLayout(self):
		self.calculateTitleLayout()
		self.calculateTwoColumnLayout(433, 0)
		self.calculateSynopsisLayout(self.COLUMN1_W)
		self.calculateAnimationLayout(1.0)
		
		self.PROMOTION_ICON_SIZE = 32
		
		self.REQUIRES_PANEL_X = self.COLUMN1_X
		self.REQUIRES_PANEL_Y = self.SYNOPSIS_PANEL_Y + self.SYNOPSIS_PANEL_H + self.main.COMMON_MARGIN_H
		self.REQUIRES_PANEL_W = self.COLUMN1_W
		self.REQUIRES_PANEL_H = self.main.COMMON_ICON_PANEL_SIZE
		
		self.UPGRADES_TO_PANEL_X = self.COLUMN2_X
		self.UPGRADES_TO_PANEL_Y = self.SYNOPSIS_PANEL_Y + self.SYNOPSIS_PANEL_H + self.main.COMMON_MARGIN_H
		self.UPGRADES_TO_PANEL_W = self.COLUMN2_W
		self.UPGRADES_TO_PANEL_H = self.main.COMMON_ICON_PANEL_SIZE
		
		self.SPECIAL_PANEL_X = self.COLUMN1_X
		self.SPECIAL_PANEL_Y = self.REQUIRES_PANEL_Y + self.REQUIRES_PANEL_H + self.main.COMMON_MARGIN_H
		self.SPECIAL_PANEL_W = self.COLUMN1_W
		self.SPECIAL_PANEL_H = (self.main.PAGE_AREA_Y + self.main.PAGE_AREA_H - self.SPECIAL_PANEL_Y - self.main.COMMON_MARGIN_H) / 2
		
		self.CIVILOPEDIA_PANEL_X = self.COLUMN2_X
		self.CIVILOPEDIA_PANEL_Y = self.UPGRADES_TO_PANEL_Y + self.UPGRADES_TO_PANEL_H + self.main.COMMON_MARGIN_H
		self.CIVILOPEDIA_PANEL_W = self.COLUMN2_W
		self.CIVILOPEDIA_PANEL_H = self.main.PAGE_AREA_Y + self.main.PAGE_AREA_H - self.CIVILOPEDIA_PANEL_Y
		
		self.PROMOTIONS_PANEL_X = self.COLUMN1_X
		self.PROMOTIONS_PANEL_Y = self.SPECIAL_PANEL_Y + self.SPECIAL_PANEL_H + self.main.COMMON_MARGIN_H
		self.PROMOTIONS_PANEL_W = self.COLUMN1_W
		self.PROMOTIONS_PANEL_H = self.main.PAGE_AREA_Y + self.main.PAGE_AREA_H - self.PROMOTIONS_PANEL_Y
	
	def draw(self):
		if (not self.isEmpty()):
			return
		
		screen = self.getScreen()
		info = self.getInfo()
		ePlayer = gc.getGame().getActivePlayer()
		
		self.drawTitle(screen, info, ePlayer)
		self.drawSynopsis(screen, info, ePlayer)
		self.drawAnimation(screen)
		self.drawRequires(screen, info, ePlayer)
		self.drawUpgradesTo(screen, info, ePlayer)
		self.drawSpecial(screen, CyGameTextMgr().getUnitHelp(self.eUnit, True, False, False, None)[1:])
		self.drawCivilopedia(screen, info, "TXT_KEY_CIVILOPEDIA_HISTORY")
		self.drawPromotions(screen)
	
	def drawAnimation(self, screen):
		if self.eCivilization != -1:
			gc.getGame().setForcedStyle(self.eCivilization)
		screen.addPanel(self.record(self.ANIMATION_BACKGROUND_ID),"", "", True, False, self.BACKGROUND_X, self.BACKGROUND_Y, self.BACKGROUND_W, self.BACKGROUND_H, PanelStyles.PANEL_STYLE_BLUE50)
		screen.addUnitGraphicGFC(self.record(self.ANIMATION_ID), self.eUnit, self.ANIMATION_X, self.ANIMATION_Y, self.ANIMATION_W, self.ANIMATION_H, WidgetTypes.WIDGET_GENERAL, -1, -1, self.ANIMATION_ROTATION_X, self.ANIMATION_ROTATION_Z, self.ANIMATION_SCALE, True)
		gc.getGame().setForcedStyle(-1)

	# Place strength/movement
	def drawStats(self, screen, info, ePlayer):
		# Unit combat group
		"""
		iCombatType = info.getUnitCombatType()
		if (iCombatType != -1):
			screen.setImageButton(self.top.getNextWidgetName(), gc.getUnitCombatInfo(iCombatType).getButton(), self.STATS_PANEL_X, self.STATS_PANEL_Y - 40, 32, 32, WidgetTypes.WIDGET_PEDIA_JUMP_TO_UNIT_COMBAT, iCombatType, 0)
			screen.setText(self.top.getNextWidgetName(), "", u"<font=3>" + gc.getUnitCombatInfo(iCombatType).getDescription() + u"</font>", CvUtil.FONT_LEFT_JUSTIFY, self.STATS_PANEL_X + 37, self.STATS_PANEL_Y - 35, 0, FontTypes.TITLE_FONT, WidgetTypes.WIDGET_PEDIA_JUMP_TO_UNIT_COMBAT, iCombatType, 0)
		"""
		
		screen.addListBoxGFC(self.record(self.STATS_ID), "", self.STATS_X, self.STATS_Y, self.STATS_W, self.STATS_H, TableStyles.TABLE_STYLE_EMPTY)
		screen.enableSelect(self.STATS_ID, False)
		
		if (info.getAirCombat() > 0 and info.getCombat() == 0):
			iStrength = info.getAirCombat()
		else:
			iStrength = info.getCombat()
			
		szStrength = localText.getText("TXT_KEY_PEDIA_STRENGTH", ( iStrength, ) )
		screen.appendListBoxStringNoUpdate(self.STATS_ID, u"<font=3>" + szStrength.upper() + u"%c" % CyGame().getSymbolID(FontSymbols.STRENGTH_CHAR) + u"</font>", WidgetTypes.WIDGET_GENERAL, 0, 0, CvUtil.FONT_LEFT_JUSTIFY)
		
		szMovement = localText.getText("TXT_KEY_PEDIA_MOVEMENT", ( info.getMoves(), ) )
		screen.appendListBoxStringNoUpdate(self.STATS_ID, u"<font=3>" + szMovement.upper() + u"%c" % CyGame().getSymbolID(FontSymbols.MOVES_CHAR) + u"</font>", WidgetTypes.WIDGET_GENERAL, 0, 0, CvUtil.FONT_LEFT_JUSTIFY)

		if (info.getProductionCost() >= 0 and not info.isFound()):
			if ePlayer == -1:
				szCost = localText.getText("TXT_KEY_PEDIA_COST", ((info.getProductionCost() * gc.getDefineINT("UNIT_PRODUCTION_PERCENT"))/100,))
			else:
				szCost = localText.getText("TXT_KEY_PEDIA_COST", ( gc.getPlayer(ePlayer).getUnitProductionNeeded(self.eUnit), ) )
			screen.appendListBoxStringNoUpdate(self.STATS_ID, u"<font=3>" + szCost.upper() + u"%c" % gc.getYieldInfo(YieldTypes.YIELD_PRODUCTION).getChar() + u"</font>", WidgetTypes.WIDGET_GENERAL, 0, 0, CvUtil.FONT_LEFT_JUSTIFY)

		if (info.getAirRange() > 0):
			szRange = localText.getText("TXT_KEY_PEDIA_RANGE", ( info.getAirRange(), ) )
			screen.appendListBoxStringNoUpdate(self.STATS_ID, u"<font=3>" + szRange.upper() + u"</font>", WidgetTypes.WIDGET_GENERAL, 0, 0, CvUtil.FONT_LEFT_JUSTIFY)
			
		screen.updateListBox(self.STATS_ID)

	def drawRequires(self, screen, info, ePlayer):
		screen.addPanel(self.record(self.REQUIRES_PANEL_ID), localText.getText("TXT_KEY_PEDIA_REQUIRES", ()), "", False, True, self.REQUIRES_PANEL_X, self.REQUIRES_PANEL_Y, self.REQUIRES_PANEL_W, self.REQUIRES_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		
		screen.attachLabel(self.REQUIRES_PANEL_ID, "", "  ")
		
		bFirst = True
		
		# Techs
		for iPrereq in range(info.getNumPrereqAndTechs()):
			if (not bFirst):
				screen.attachLabel(self.REQUIRES_PANEL_ID, "", localText.getText("TXT_KEY_AND", ()))
			
			bFirst = False
			
			eLoopTech = info.getPrereqAndTechs(iPrereq)
			screen.attachImageButton(self.REQUIRES_PANEL_ID, "", gc.getTechInfo(eLoopTech).getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_TECH, eLoopTech, -1, False)
		
		# Religions
		eReligion = info.getPrereqReligion()
		if (eReligion >= 0):
			if (not bFirst):
				screen.attachLabel(self.REQUIRES_PANEL_ID, "", localText.getText("TXT_KEY_AND", ()))
			
			bFirst = False
			
			screen.attachImageButton(self.REQUIRES_PANEL_ID, "", gc.getReligionInfo(eReligion).getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_RELIGION, eReligion, -1, False)
		else:
			eReligion = info.getStateReligion()
			if (eReligion >= 0):
				if (not bFirst):
					screen.attachLabel(self.REQUIRES_PANEL_ID, "", localText.getText("TXT_KEY_AND", ()))
				
				bFirst = False
				
				screen.attachImageButton(self.REQUIRES_PANEL_ID, "", gc.getReligionInfo(eReligion).getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_RELIGION, eReligion, -1, False)
		
		# Civics
		eCivic = info.getPrereqCivic()
		if (eCivic >= 0):
			if (not bFirst):
				screen.attachLabel(self.REQUIRES_PANEL_ID, "", localText.getText("TXT_KEY_AND", ()))
			
			bFirst = False
			
			screen.attachImageButton(self.REQUIRES_PANEL_ID, "", gc.getCivicInfo(eCivic).getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_CIVIC, eCivic, -1, False)
		
		# Bonuses
		for iPrereq in range(info.getNumPrereqAndBonuses()):
			if (not bFirst):
				screen.attachLabel(self.REQUIRES_PANEL_ID, "", localText.getText("TXT_KEY_AND", ()))
			
			bFirst = False
			
			eLoopBonus = info.getPrereqAndBonuses(iPrereq)
			sButton = gc.getBonusInfo(eLoopBonus).getButton()
			sButton = sButton[:-4] + "_x64.dds"
			screen.attachImageButton(self.REQUIRES_PANEL_ID, "", sButton, self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_BONUS, eLoopBonus, -1, False)
		
		if (not bFirst and info.getNumPrereqOrBonuses() > 0):
			bFirst = False
			
			screen.attachLabel(self.REQUIRES_PANEL_ID, "", localText.getText("TXT_KEY_AND", ()))
			
			if (info.getNumPrereqOrBonuses() > 1):
				screen.attachLabel(self.REQUIRES_PANEL_ID, "", "( ")
		
		for iPrereq in range(info.getNumPrereqOrBonuses()):
			eLoopBonus = info.getPrereqOrBonuses(iPrereq)
			sButton = gc.getBonusInfo(eLoopBonus).getButton()
			sButton = sButton[:-4] + "_x64.dds"
			if (iPrereq > 0):
				screen.attachLabel(self.REQUIRES_PANEL_ID, "", localText.getText("TXT_KEY_OR", ()))
			screen.attachImageButton(self.REQUIRES_PANEL_ID, "", sButton, self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_BONUS, eLoopBonus, -1, False)
		
		if (info.getNumPrereqOrBonuses() > 1):
			screen.attachLabel(self.REQUIRES_PANEL_ID, "", " ) ")
		
		# Buildings
		for iPrereq in range(info.getNumPrereqAndBuildingClasses()):
			eLoopBuildingClass = info.getPrereqAndBuildingClasses(iPrereq)
			
			if (ePlayer == -1):
				eLoopBuilding = gc.getBuildingClassInfo(eLoopBuildingClass).getDefaultBuildingIndex()
			else:
				eLoopBuilding = gc.getCivilizationInfo(gc.getPlayer(ePlayer).getCivilizationType()).getCivilizationBuildings(eLoopBuildingClass)
			
			if (eLoopBuilding >= 0):
				if (not bFirst):
					screen.attachLabel(self.REQUIRES_PANEL_ID, "", localText.getText("TXT_KEY_AND", ()))
				
				bFirst = False
				
				screen.attachImageButton(self.REQUIRES_PANEL_ID, "", gc.getBuildingInfo(eLoopBuilding).getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_BUILDING, eLoopBuilding, -1, False)
		
		if (not bFirst and info.getNumPrereqOrBuildingClasses() > 0):
			bFirst = False
			
			screen.attachLabel(self.REQUIRES_PANEL_ID, "", localText.getText("TXT_KEY_AND", ()))
			
			if (info.getNumPrereqOrBuildingClasses() > 1):
				screen.attachLabel(self.REQUIRES_PANEL_ID, "", "( ")
		
		for iPrereq in range(info.getNumPrereqOrBuildingClasses()):
			eLoopBuildingClass = info.getPrereqOrBuildingClasses(iPrereq)
			
			if (ePlayer == -1):
				eLoopBuilding = gc.getBuildingClassInfo(eLoopBuildingClass).getDefaultBuildingIndex()
			else:
				eLoopBuilding = gc.getCivilizationInfo(gc.getPlayer(ePlayer).getCivilizationType()).getCivilizationBuildings(eLoopBuildingClass)
			
			if (eLoopBuilding >= 0):
				if (iPrereq > 0):
					screen.attachLabel(self.REQUIRES_PANEL_ID, "", localText.getText("TXT_KEY_OR", ()))
				screen.attachImageButton(self.REQUIRES_PANEL_ID, "", gc.getBuildingInfo(eLoopBuilding).getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_BUILDING, eLoopBuilding, -1, False)					
		
		if (info.getNumPrereqOrBuildingClasses() > 1):
			screen.attachLabel(self.REQUIRES_PANEL_ID, "", " ) ")
	
	def drawUpgradesTo(self, screen, info, ePlayer):
		screen.addPanel(self.record(self.UPGRADES_TO_PANEL_ID), localText.getText("TXT_KEY_PEDIA_UPGRADES_TO", ()), "", False, True, self.UPGRADES_TO_PANEL_X, self.UPGRADES_TO_PANEL_Y, self.UPGRADES_TO_PANEL_W, self.UPGRADES_TO_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		
		screen.attachLabel(self.UPGRADES_TO_PANEL_ID, "", "  ")
		
		for eLoopUnitClass in range(gc.getNumUnitClassInfos()):
			if (self.eCivilization != -1):
				eLoopUnit = gc.getCivilizationInfo(self.eCivilization).getCivilizationUnits(eLoopUnitClass)
			elif ePlayer == -1:
				eLoopUnit = gc.getUnitClassInfo(eLoopUnitClass).getDefaultUnitIndex()
			else:
				eLoopUnit = gc.getCivilizationInfo(gc.getPlayer(ePlayer).getCivilizationType()).getCivilizationUnits(eLoopUnitClass)
			
			if (eLoopUnit >= 0 and info.getUpgradeUnitClass(eLoopUnitClass)):
				if (self.eCivilization != -1):
					szButton = gc.getUnitInfo(eLoopUnit).getStyledButton(self.eCivilization)
				elif ePlayer != -1:
					szButton = gc.getPlayer(ePlayer).getUnitButton(eLoopUnit)
				else:
					szButton = gc.getUnitInfo(eLoopUnit).getButton()
				screen.attachImageButton(self.UPGRADES_TO_PANEL_ID, "", szButton, self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_UNIT, eLoopUnit, 1, False)

	# Place Special abilities
	def placeSpecial(self, screen):
		panelName = self.top.getNextWidgetName()
		screen.addPanel( panelName, localText.getText("TXT_KEY_PEDIA_SPECIAL_ABILITIES", ()), "", true, false, self.SPECIAL_PANEL_X, self.SPECIAL_PANEL_Y, self.SPECIAL_PANEL_W, self.SPECIAL_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50 )
		
		listName = self.top.getNextWidgetName()
		
		szSpecialText = CyGameTextMgr().getUnitHelp( self.eUnit, True, False, False, None )[1:]
		screen.addMultilineText(listName, szSpecialText, self.SPECIAL_PANEL_X+5, self.SPECIAL_PANEL_Y+30, self.SPECIAL_PANEL_W-10, self.SPECIAL_PANEL_H-35, WidgetTypes.WIDGET_GENERAL, -1, -1, CvUtil.FONT_LEFT_JUSTIFY)	

	def drawCivilopedia(self, screen, info, szHeadingKey):
		screen.addPanel(self.record(self.CIVILOPEDIA_PANEL_ID), localText.getText(szHeadingKey, ()), "", True, True, self.CIVILOPEDIA_PANEL_X, self.CIVILOPEDIA_PANEL_Y, self.CIVILOPEDIA_PANEL_W, self.CIVILOPEDIA_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		
		szText = u"" 
		if len(info.getStrategy()) > 0:
			szText += localText.getText("TXT_KEY_CIVILOPEDIA_STRATEGY", ())
			szText += info.getStrategy()
			szText += u"\n\n"
		szText += localText.getText("TXT_KEY_CIVILOPEDIA_BACKGROUND", ())
		szText += info.getCivilopedia()
		
		screen.addMultilineText(self.record(self.CIVILOPEDIA_TEXT_ID), szText, self.CIVILOPEDIA_PANEL_X + 15, self.CIVILOPEDIA_PANEL_Y + 40, self.CIVILOPEDIA_PANEL_W - (15 * 2), self.CIVILOPEDIA_PANEL_H - (15 * 2) - 25, WidgetTypes.WIDGET_GENERAL, -1, -1, CvUtil.FONT_LEFT_JUSTIFY)
	
	def drawPromotions(self, screen):
		screen.addPanel(self.record(self.PROMOTIONS_PANEL_ID), localText.getText("TXT_KEY_PEDIA_CATEGORY_PROMOTION", ()), "", True, True, self.PROMOTIONS_PANEL_X, self.PROMOTIONS_PANEL_Y, self.PROMOTIONS_PANEL_W, self.PROMOTIONS_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		
		screen.addMultiListControlGFC(self.record(self.PROMOTIONS_LIST_ID), "", self.PROMOTIONS_PANEL_X+15, self.PROMOTIONS_PANEL_Y+40, self.PROMOTIONS_PANEL_W-20, self.PROMOTIONS_PANEL_H-40, 1, self.PROMOTION_ICON_SIZE, self.PROMOTION_ICON_SIZE, TableStyles.TABLE_STYLE_STANDARD)
		
		for eLoopPromotion in range(gc.getNumPromotionInfos()):
			if (isPromotionValid(eLoopPromotion, self.eUnit, False)):
				pLoopPromotion = gc.getPromotionInfo(eLoopPromotion)
				if (not pLoopPromotion.isGraphicalOnly() and not pLoopPromotion.isSpecial() and not pLoopPromotion.isTemporary()):
					screen.appendMultiListButton(self.PROMOTIONS_LIST_ID, gc.getPromotionInfo(eLoopPromotion).getButton(), 0, WidgetTypes.WIDGET_PEDIA_JUMP_TO_PROMOTION, eLoopPromotion, -1, False)
