## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
from CvPythonExtensions import *
import CvUtil
import CvPediaPage
import ScreenInput
import CvScreenEnums
## Revolutions ##
import Revolutions
## Revolutions ##

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

class CvPediaBuilding(CvPediaPage.CvPediaPage):
	"Civilopedia Screen for Buildings"
	
	def __init__(self, section):
		super(CvPediaBuilding, self).__init__(section, CivilopediaPageTypes.CIVILOPEDIA_PAGE_BUILDING, WidgetTypes.WIDGET_PEDIA_JUMP_TO_BUILDING, "Building")
		
		self.eBuilding = -1
		
		self.REQUIRES_PANEL_ID = self.szWidget + "RequiresPanel"
		self.CONSUMES_PANEL_ID = self.szWidget + "ConsumesPanel"
		self.PRODUCES_PANEL_ID = self.szWidget + "ProducesPanel"
		self.ANIMATION_BACKGROUND_ID = self.szWidget + "AnimationBackground"
	
	def getData(self):
		return (self.eBuilding, -1)
	
	def setData(self, eBuilding, iPageData2):
		if (self.eBuilding != eBuilding):
			self.clear()
			self.eBuilding = eBuilding
	
	def getInfo(self):
		return gc.getBuildingInfo(self.eBuilding)
	
	def calculateLayout(self):
		self.calculateTitleLayout()
		self.calculateTwoColumnLayout()
		self.calculateSynopsisLayout(self.COLUMN1_W)
		self.calculateAnimationLayout(1.0)
		
		self.calculateThreeColumnLayout()
		
		self.REQUIRES_PANEL_X = self.COLUMN1_X
		self.REQUIRES_PANEL_Y = self.SYNOPSIS_PANEL_Y + self.SYNOPSIS_PANEL_H + self.main.COMMON_MARGIN_H
		self.REQUIRES_PANEL_W = self.COLUMN1_W
		self.REQUIRES_PANEL_H = self.main.COMMON_ICON_PANEL_SIZE
		
		self.CONSUMES_PANEL_X = self.COLUMN2_X
		self.CONSUMES_PANEL_Y = self.REQUIRES_PANEL_Y
		self.CONSUMES_PANEL_W = self.COLUMN2_W
		self.CONSUMES_PANEL_H = self.main.COMMON_ICON_PANEL_SIZE
		
		self.PRODUCES_PANEL_X = self.COLUMN3_X
		self.PRODUCES_PANEL_Y = self.REQUIRES_PANEL_Y
		self.PRODUCES_PANEL_W = self.COLUMN3_W
		self.PRODUCES_PANEL_H = self.main.COMMON_ICON_PANEL_SIZE
		
		self.calculateTwoColumnLayout()
		
		self.SPECIAL_PANEL_X = self.COLUMN1_X
		self.SPECIAL_PANEL_Y = self.REQUIRES_PANEL_Y + self.REQUIRES_PANEL_H + self.main.COMMON_MARGIN_H
		self.SPECIAL_PANEL_W = self.COLUMN1_W
		self.SPECIAL_PANEL_H = self.main.PAGE_AREA_Y + self.main.PAGE_AREA_H - self.SPECIAL_PANEL_Y
		
		self.CIVILOPEDIA_PANEL_X = self.COLUMN2_X
		self.CIVILOPEDIA_PANEL_Y = self.SPECIAL_PANEL_Y
		self.CIVILOPEDIA_PANEL_W = self.COLUMN2_W
		self.CIVILOPEDIA_PANEL_H = self.main.PAGE_AREA_Y + self.main.PAGE_AREA_H - self.CIVILOPEDIA_PANEL_Y
	
	def draw(self):
		if (not self.isEmpty() or self.eBuilding < 0):
			return
		
		screen = self.getScreen()
		info = self.getInfo()
		ePlayer = gc.getGame().getActivePlayer()
		
		self.drawTitle(screen, info, ePlayer)
		self.drawSynopsis(screen, info, ePlayer)
		self.drawAnimation(screen)
		self.drawRequires(screen, info, ePlayer)
		self.drawConsumes(screen, info)
		self.drawProduces(screen, info)
		self.drawSpecialBuilding(screen, CyGameTextMgr().getBuildingHelp(self.eBuilding, True, False, False, None)[1:])
		self.drawCivilopedia(screen, info, "TXT_KEY_CIVILOPEDIA_HISTORY")
		
	def drawSpecialBuilding(self, screen, szText):
		screen.addPanel(self.record(self.SPECIAL_PANEL_ID), localText.getText("TXT_KEY_PEDIA_SPECIAL_ABILITIES", ()), "", True, False, self.SPECIAL_PANEL_X, self.SPECIAL_PANEL_Y, self.SPECIAL_PANEL_W, self.SPECIAL_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)

## Revolutions ##
		if (gc.getBuildingInfo(self.eBuilding).getGlobalSeparatism() != 0):
			szText = u"%s%+d %c</font>" %(CyTranslator().getText("[ICON_BULLET]", ()), gc.getBuildingInfo(self.eBuilding).getGlobalSeparatism(), (CyGame().getSymbolID(FontSymbols.SEPARATISM_CHAR))) + " in All Cities" + u"\n" + szText
			
		if (gc.getBuildingInfo(self.eBuilding).getSeparatism() != 0):
			szText = u"%s%+d %c</font>" %(CyTranslator().getText("[ICON_BULLET]", ()), gc.getBuildingInfo(self.eBuilding).getSeparatism(), (CyGame().getSymbolID(FontSymbols.SEPARATISM_CHAR))) + u"\n" + szText
## Revolutions ##
		screen.addMultilineText(self.record(self.SPECIAL_TEXT_ID), szText, self.SPECIAL_PANEL_X+5, self.SPECIAL_PANEL_Y+30, self.SPECIAL_PANEL_W-10, self.SPECIAL_PANEL_H-35, WidgetTypes.WIDGET_GENERAL, -1, -1, CvUtil.FONT_LEFT_JUSTIFY)
		
	def drawAnimation(self, screen):
		screen.addPanel(self.record(self.ANIMATION_BACKGROUND_ID),"", "", True, False, self.BACKGROUND_X, self.BACKGROUND_Y, self.BACKGROUND_W, self.BACKGROUND_H, PanelStyles.PANEL_STYLE_BLUE50)
		screen.addBuildingGraphicGFC(self.record(self.ANIMATION_ID), self.eBuilding, self.ANIMATION_X, self.ANIMATION_Y, self.ANIMATION_W, self.ANIMATION_H, WidgetTypes.WIDGET_GENERAL, -1, -1, self.ANIMATION_ROTATION_X, self.ANIMATION_ROTATION_Z, self.ANIMATION_SCALE, True)
	
	# Place happiness/health/commerce/great people modifiers
	def drawStats(self, screen, info, ePlayer):
		screen.addListBoxGFC(self.record(self.STATS_ID), "", self.STATS_X, self.STATS_Y, self.STATS_W, self.STATS_H, TableStyles.TABLE_STYLE_EMPTY)
		screen.enableSelect(self.STATS_ID, False)
		
		if (isWorldWonderClass(info.getBuildingClassType())):
			iMaxInstances = gc.getBuildingClassInfo(info.getBuildingClassType()).getMaxGlobalInstances()
			szBuildingType = localText.getText("TXT_KEY_PEDIA_WORLD_WONDER", ())
			if (iMaxInstances > 1):
				szBuildingType += " " + localText.getText("TXT_KEY_PEDIA_WONDER_INSTANCES", (iMaxInstances,))
				screen.appendListBoxStringNoUpdate(self.STATS_ID, u"<font=3>" + szBuildingType.upper() + u"</font>", WidgetTypes.WIDGET_GENERAL, 0, 0, CvUtil.FONT_LEFT_JUSTIFY)

		if (isTeamWonderClass(info.getBuildingClassType())):
			iMaxInstances = gc.getBuildingClassInfo(info.getBuildingClassType()).getMaxTeamInstances()
			szBuildingType = localText.getText("TXT_KEY_PEDIA_TEAM_WONDER", ())
			if (iMaxInstances > 1):
				szBuildingType += " " + localText.getText("TXT_KEY_PEDIA_WONDER_INSTANCES", (iMaxInstances,))
				screen.appendListBoxStringNoUpdate(self.STATS_ID, u"<font=3>" + szBuildingType.upper() + u"</font>", WidgetTypes.WIDGET_GENERAL, 0, 0, CvUtil.FONT_LEFT_JUSTIFY)

		if (isNationalWonderClass(info.getBuildingClassType())):
			iMaxInstances = gc.getBuildingClassInfo(info.getBuildingClassType()).getMaxPlayerInstances()
			szBuildingType = localText.getText("TXT_KEY_PEDIA_NATIONAL_WONDER", ())
			if (iMaxInstances > 1):
				szBuildingType += " " + localText.getText("TXT_KEY_PEDIA_WONDER_INSTANCES", (iMaxInstances,))
				screen.appendListBoxStringNoUpdate(self.STATS_ID, u"<font=3>" + szBuildingType.upper() + u"</font>", WidgetTypes.WIDGET_GENERAL, 0, 0, CvUtil.FONT_LEFT_JUSTIFY)

		if (info.getProductionCost() > 0):
			if (ePlayer == -1):
				szCost = localText.getText("TXT_KEY_PEDIA_COST", ((info.getProductionCost() * gc.getDefineINT("BUILDING_PRODUCTION_PERCENT"))/100,))
			else:
				szCost = localText.getText("TXT_KEY_PEDIA_COST", (gc.getPlayer(ePlayer).getBuildingProductionNeeded(self.eBuilding),))
			screen.appendListBoxStringNoUpdate(self.STATS_ID, u"<font=3>" + szCost.upper() + u" %c" % gc.getYieldInfo(YieldTypes.YIELD_PRODUCTION).getChar() + u"</font>", WidgetTypes.WIDGET_GENERAL, 0, 0, CvUtil.FONT_LEFT_JUSTIFY)

		for k in range(YieldTypes.NUM_YIELD_TYPES):
			iYieldChange = info.getYieldChange(k)
			if (iYieldChange != 0):
				if (iYieldChange > 0):
					szSign = "+"
				else:
					szSign = ""
				szYield = (u"%s: %s%i %c" % (gc.getYieldInfo(k).getDescription().upper(), szSign, iYieldChange, gc.getYieldInfo(k).getChar()))
				screen.appendListBoxStringNoUpdate(self.STATS_ID, u"<font=3>" + szYield + u"</font>", WidgetTypes.WIDGET_GENERAL, 0, 0, CvUtil.FONT_LEFT_JUSTIFY)
		
		for k in range(CommerceTypes.NUM_COMMERCE_TYPES):
			iCommerceChange = info.getObsoleteSafeCommerceChange(k) + info.getCommerceChange(k)
			if (iCommerceChange != 0):
				if (iCommerceChange > 0):
					szSign = "+"
				else:
					szSign = ""
				szCommerce = (u"%s: %s%i %c" % (gc.getCommerceInfo(k).getDescription().upper(), szSign, iCommerceChange, gc.getCommerceInfo(k).getChar()))
				screen.appendListBoxStringNoUpdate(self.STATS_ID, u"<font=3>" + szCommerce + u"</font>", WidgetTypes.WIDGET_GENERAL, 0, 0, CvUtil.FONT_LEFT_JUSTIFY)
		
		iHappiness = info.getHappiness()
		if (ePlayer != -1):
			if (self.eBuilding == gc.getCivilizationInfo(gc.getPlayer(ePlayer).getCivilizationType()).getCivilizationBuildings(info.getBuildingClassType())):
				iHappiness += gc.getPlayer(ePlayer).getExtraBuildingHappiness(self.eBuilding)
		
		if (iHappiness > 0):
			szText = localText.getText("TXT_KEY_PEDIA_HAPPY", (iHappiness,)).upper()
			screen.appendListBoxStringNoUpdate(self.STATS_ID, u"<font=3>" + szText + u" %c" % CyGame().getSymbolID(FontSymbols.HAPPY_CHAR) + u"</font>", WidgetTypes.WIDGET_GENERAL, 0, 0, CvUtil.FONT_LEFT_JUSTIFY)
		
		elif (iHappiness < 0):
			szText = localText.getText("TXT_KEY_PEDIA_UNHAPPY", (-iHappiness,)).upper()
			screen.appendListBoxStringNoUpdate(self.STATS_ID, u"<font=3>" + szText + u" %c" % CyGame().getSymbolID(FontSymbols.UNHAPPY_CHAR) + u"</font>", WidgetTypes.WIDGET_GENERAL, 0, 0, CvUtil.FONT_LEFT_JUSTIFY)
		
		iHealth = info.getHealth()
		if (ePlayer != -1):
			if (self.eBuilding == gc.getCivilizationInfo(gc.getPlayer(ePlayer).getCivilizationType()).getCivilizationBuildings(info.getBuildingClassType())):
				iHealth += gc.getPlayer(ePlayer).getExtraBuildingHealth(self.eBuilding)
		
		if (iHealth > 0):
			szText = localText.getText("TXT_KEY_PEDIA_HEALTHY", (iHealth,)).upper()
			screen.appendListBoxStringNoUpdate(self.STATS_ID, u"<font=3>" + szText + u" %c" % CyGame().getSymbolID(FontSymbols.HEALTHY_CHAR) + u"</font>", WidgetTypes.WIDGET_GENERAL, 0, 0, CvUtil.FONT_LEFT_JUSTIFY)
		
		elif (iHealth < 0):
			szText = localText.getText("TXT_KEY_PEDIA_UNHEALTHY", (-iHealth,)).upper()
			screen.appendListBoxStringNoUpdate(self.STATS_ID, u"<font=3>" + szText + u" %c" % CyGame().getSymbolID(FontSymbols.UNHEALTHY_CHAR) + u"</font>", WidgetTypes.WIDGET_GENERAL, 0, 0, CvUtil.FONT_LEFT_JUSTIFY)
		
		if (info.getGreatPeopleRateChange() != 0):
			szText = localText.getText("TXT_KEY_PEDIA_GREAT_PEOPLE", (info.getGreatPeopleRateChange(),)).upper()
			screen.appendListBoxStringNoUpdate(self.STATS_ID, u"<font=3>" + szText + u" %c" % CyGame().getSymbolID(FontSymbols.GREAT_PEOPLE_CHAR) + u"</font>", WidgetTypes.WIDGET_GENERAL, 0, 0, CvUtil.FONT_LEFT_JUSTIFY)
			
		if (isWorldWonderClass(info.getBuildingClassType())):
			iConquest = 100 - info.getConquestProbability()
			szText = localText.getText("TXT_KEY_PEDIA_CONQUEST_PROB", (iConquest,)).upper()
			screen.appendListBoxStringNoUpdate(self.STATS_ID, u"<font=3>" + szText + u"</font>", WidgetTypes.WIDGET_GENERAL, 0, 0, CvUtil.FONT_LEFT_JUSTIFY)
		
		screen.updateListBox(self.STATS_ID)
	
	def drawRequires(self, screen, info, ePlayer):
		screen.addPanel(self.record(self.REQUIRES_PANEL_ID), localText.getText("TXT_KEY_PEDIA_REQUIRES", ()), "", False, True, self.REQUIRES_PANEL_X, self.REQUIRES_PANEL_Y, self.REQUIRES_PANEL_W, self.REQUIRES_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		
		screen.attachLabel(self.REQUIRES_PANEL_ID, "", "  ")
		
		bFirst = True
		
		# Techs
		for eLoopTech in range(gc.getNumTechInfos()):
			if isTechRequiredForBuilding(eLoopTech, self.eBuilding):
				if (not bFirst):
					screen.attachLabel(self.REQUIRES_PANEL_ID, "", localText.getText("TXT_KEY_AND", ()))
				
				bFirst = False
				
				screen.attachImageButton(self.REQUIRES_PANEL_ID, "", gc.getTechInfo(eLoopTech).getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_TECH, eLoopTech, -1, False)
		
		# Religions
		eReligion = info.getPrereqReligion()
		if (eReligion >= 0):
			if (not bFirst):
				screen.attachLabel(self.REQUIRES_PANEL_ID, "", localText.getText("TXT_KEY_AND", ()))
			
			bFirst = False
			
			screen.attachImageButton(self.REQUIRES_PANEL_ID, "", gc.getReligionInfo(eReligion).getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_RELIGION, eReligion, -1, False)
		
		# Civics
		for iPrereq in range(info.getNumPrereqAndCivics()):
			if (not bFirst):
				screen.attachLabel(self.REQUIRES_PANEL_ID, "", localText.getText("TXT_KEY_AND", ()))
			
			bFirst = False
			
			eLoopCivic = info.getPrereqAndCivics(iPrereq)
			screen.attachImageButton(self.REQUIRES_PANEL_ID, "", gc.getCivicInfo(eLoopCivic).getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_CIVIC, eLoopCivic, -1, False)
		
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
			if (iPrereq > 0):
				screen.attachLabel(self.REQUIRES_PANEL_ID, "", localText.getText("TXT_KEY_OR", ()))
			sButton = gc.getBonusInfo(eLoopBonus).getButton()
			sButton = sButton[:-4] + "_x64.dds"		
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
	
	def drawConsumes(self, screen, info):
		screen.addPanel(self.record(self.CONSUMES_PANEL_ID), localText.getText("TXT_KEY_CONSUMES", ()), "", False, True, self.CONSUMES_PANEL_X, self.CONSUMES_PANEL_Y, self.CONSUMES_PANEL_W, self.CONSUMES_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		
		screen.attachLabel(self.CONSUMES_PANEL_ID, "", "  ")
		
		bFirst = True
		for iConsumedBonusIndex in range(info.getNumConsumedBonuses()):
			eConsumedBonus = info.getConsumedBonusType(iConsumedBonusIndex)
			iConsumedAmount = info.getConsumedBonusAmount(iConsumedBonusIndex)
			sButton = gc.getBonusInfo(eConsumedBonus).getButton()
			sButton = sButton[:-4] + "_x64.dds"	
			if not bFirst:
				screen.attachLabel(self.CONSUMES_PANEL_ID, "", localText.getText("TXT_KEY_AND", ()))
			else:
				bFirst = False
			if (iConsumedAmount > 1):
				screen.attachLabel(self.CONSUMES_PANEL_ID, "", u"<font=3>%d</font>" % iConsumedAmount)
			screen.attachImageButton(self.CONSUMES_PANEL_ID, "", sButton, self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_BONUS, eConsumedBonus, -1, False)
	
	def drawProduces(self, screen, info):
		screen.addPanel(self.record(self.PRODUCES_PANEL_ID), localText.getText("TXT_KEY_PRODUCES", ()), "", False, True, self.PRODUCES_PANEL_X, self.PRODUCES_PANEL_Y, self.PRODUCES_PANEL_W, self.PRODUCES_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		
		screen.attachLabel(self.PRODUCES_PANEL_ID, "", "  ")
		
		bFirst = True
		for iProducedBonusIndex in range(info.getNumProducedBonuses()):
			eProducedBonus = info.getProducedBonusType(iProducedBonusIndex)
			iProducedAmount = info.getProducedBonusAmount(iProducedBonusIndex)
			sButton = gc.getBonusInfo(eProducedBonus).getButton()
			sButton = sButton[:-4] + "_x64.dds"
			if not bFirst:
				screen.attachLabel(self.PRODUCES_PANEL_ID, "", localText.getText("TXT_KEY_AND", ()))
			else:
				bFirst = False
			if (iProducedAmount > 1):
				screen.attachLabel(self.PRODUCES_PANEL_ID, "", u"<font=3>%d</font>" % iProducedAmount)
			screen.attachImageButton(self.PRODUCES_PANEL_ID, "", sButton, self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_BONUS, eProducedBonus, -1, False)
		
		eFreeBonus = info.getFreeBonus()
		if (eFreeBonus > -1):
			sButton = gc.getBonusInfo(eFreeBonus).getButton()
			sButton = sButton[:-4] + "_x64.dds"
			iNumFreeBonuses = CyGame().getNumFreeBonuses(self.eBuilding)
			if not bFirst:
				screen.attachLabel(self.PRODUCES_PANEL_ID, "", localText.getText("TXT_KEY_AND", ()))
			else:
				bFirst = False
			if (iNumFreeBonuses > 1):
				screen.attachLabel(self.PRODUCES_PANEL_ID, "", u"<font=3>%d</font>" % iNumFreeBonuses)
			screen.attachImageButton(self.PRODUCES_PANEL_ID, "", sButton, self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_BONUS, eFreeBonus, -1, False)

	def drawCivilopedia(self, screen, info, szHeadingKey):
		screen.addPanel(self.record(self.CIVILOPEDIA_PANEL_ID), localText.getText(szHeadingKey, ()), "", True, True, self.CIVILOPEDIA_PANEL_X, self.CIVILOPEDIA_PANEL_Y, self.CIVILOPEDIA_PANEL_W, self.CIVILOPEDIA_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		
		szText = u"" 
				
		if not (isWorldWonderClass(gc.getBuildingInfo(self.eBuilding).getBuildingClassType())):
			if len(info.getStrategy()) > 0:
				szText += localText.getText("TXT_KEY_CIVILOPEDIA_STRATEGY", ())
				szText += info.getStrategy()
				szText += u"\n\n"
		szText += localText.getText("TXT_KEY_CIVILOPEDIA_BACKGROUND", ())
		szText += info.getCivilopedia()
		screen.addMultilineText(self.record(self.CIVILOPEDIA_TEXT_ID), szText, self.CIVILOPEDIA_PANEL_X + 15, self.CIVILOPEDIA_PANEL_Y + 40, self.CIVILOPEDIA_PANEL_W - (15 * 2), self.CIVILOPEDIA_PANEL_H - (15 * 2) - 25, WidgetTypes.WIDGET_GENERAL, -1, -1, CvUtil.FONT_LEFT_JUSTIFY)

	def getBuildingType(self, eBuilding):
		if (isWorldWonderClass(gc.getBuildingInfo(eBuilding).getBuildingClassType())):
			return True			
		
		if (isTeamWonderClass(gc.getBuildingInfo(eBuilding).getBuildingClassType())):
			return True
		
		if (isNationalWonderClass(gc.getBuildingInfo(eBuilding).getBuildingClassType())):
			return True
		
		# Regular building
		return False
	
	def getBuildingSortedList(self, bWonder):
		listBuildings = []
		iCount = 0
		for eBuilding in range(gc.getNumBuildingInfos()):
			if (self.getBuildingType(eBuilding) == bWonder and not gc.getBuildingInfo(eBuilding).isGraphicalOnly()):
				listBuildings.append(eBuilding)
				iCount += 1
		
		listSorted = [(0,0)] * iCount
		iI = 0
		for eBuilding in listBuildings:
			listSorted[iI] = (gc.getBuildingInfo(eBuilding).getDescription(), eBuilding)
			iI += 1
		listSorted.sort()
		return listSorted
	
	# Will handle the input for this screen...
	def handleInput (self, input):
		return 0

