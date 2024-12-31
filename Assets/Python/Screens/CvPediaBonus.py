## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
from CvPythonExtensions import *
import CvUtil
import CvPediaPage
import ScreenInput
import CvScreenEnums
import string

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

class CvPediaBonus(CvPediaPage.CvPediaPage):
	"Civilopedia Screen for Bonus Resources"

	def __init__(self, section):
		super(CvPediaBonus, self).__init__(section, CivilopediaPageTypes.CIVILOPEDIA_PAGE_BONUS, WidgetTypes.WIDGET_PEDIA_JUMP_TO_BONUS, "Bonus")
		
		self.eBonus = -1
		
		self.IMPROVEMENTS_PANEL_ID = self.szWidget + "ImprovementsPanel"
		self.REQUIRES_PANEL_ID = self.szWidget + "RequiresPanel"
		self.CONSUMED_BY_PANEL_ID = self.szWidget + "ConsumedByPanel"
		self.PRODUCED_BY_PANEL_ID = self.szWidget + "ProducedByPanel"
		self.ALLOWS_PANEL_ID = self.szWidget + "AllowsPanel"
		self.ANIMATION_BACKGROUND_ID = self.szWidget + "AnimationBackground"
	
	def getData(self):
		return (self.eBonus, -1)
	
	def setData(self, eBonus, iPageData2):
		if (self.eBonus != eBonus):
			self.clear()
			self.eBonus = eBonus
	
	def getInfo(self):
		return gc.getBonusInfo(self.eBonus)
	
	def calculateLayout(self):
		self.calculateTitleLayout()
		self.calculateTwoColumnLayout()
		self.calculateSynopsisLayout(self.COLUMN1_W)
		self.calculateAnimationLayout(0.6)
		
		self.calculateThreeColumnLayout()
		
		self.REQUIRES_PANEL_X = self.COLUMN1_X
		self.REQUIRES_PANEL_Y = self.SYNOPSIS_PANEL_Y + self.SYNOPSIS_PANEL_H + self.main.COMMON_MARGIN_H
		self.REQUIRES_PANEL_W = self.COLUMN1_W
		self.REQUIRES_PANEL_H = self.main.COMMON_ICON_PANEL_SIZE
		
		self.PRODUCED_BY_PANEL_X = self.COLUMN2_X
		self.PRODUCED_BY_PANEL_Y = self.REQUIRES_PANEL_Y
		self.PRODUCED_BY_PANEL_W = self.COLUMN3_W
		self.PRODUCED_BY_PANEL_H = self.main.COMMON_ICON_PANEL_SIZE
		
		self.CONSUMED_BY_PANEL_X = self.COLUMN3_X
		self.CONSUMED_BY_PANEL_Y = self.PRODUCED_BY_PANEL_Y
		self.CONSUMED_BY_PANEL_W = self.COLUMN3_W
		self.CONSUMED_BY_PANEL_H = self.main.COMMON_ICON_PANEL_SIZE
		
		self.ALLOWS_PANEL_X = self.main.PAGE_AREA_X
		self.ALLOWS_PANEL_Y = self.REQUIRES_PANEL_Y + self.REQUIRES_PANEL_H + self.main.COMMON_MARGIN_H
		self.ALLOWS_PANEL_W = self.main.PAGE_AREA_W
		self.ALLOWS_PANEL_H = self.main.COMMON_ICON_PANEL_SIZE
		
		self.IMPROVEMENTS_PANEL_X = self.COLUMN1_X
		self.IMPROVEMENTS_PANEL_Y = self.ALLOWS_PANEL_Y + self.ALLOWS_PANEL_H + self.main.COMMON_MARGIN_H
		self.IMPROVEMENTS_PANEL_W = self.COLUMN1_W
		self.IMPROVEMENTS_PANEL_H = self.main.PAGE_AREA_Y + self.main.PAGE_AREA_H - self.IMPROVEMENTS_PANEL_Y
		
		self.EFFECTS_PANEL_X = self.COLUMN2_X
		self.EFFECTS_PANEL_Y = self.IMPROVEMENTS_PANEL_Y
		self.EFFECTS_PANEL_W = self.COLUMN2_W
		self.EFFECTS_PANEL_H = self.main.PAGE_AREA_Y + self.main.PAGE_AREA_H - self.EFFECTS_PANEL_Y
		
		self.CIVILOPEDIA_PANEL_X = self.COLUMN3_X
		self.CIVILOPEDIA_PANEL_Y = self.EFFECTS_PANEL_Y
		self.CIVILOPEDIA_PANEL_W = self.COLUMN3_W
		self.CIVILOPEDIA_PANEL_H = self.main.PAGE_AREA_Y + self.main.PAGE_AREA_H - self.CIVILOPEDIA_PANEL_Y
		
		self.ICON_SIZE = 128
		self.ICON_X -= 32
		self.ICON_Y -= 32
	
	def draw(self):
		if (not self.isEmpty()):
			return
		
		screen = self.getScreen()
		info = self.getInfo()
		ePlayer = gc.getGame().getActivePlayer()
		
		self.drawTitle(screen, info, ePlayer)
		self.drawSynopsis(screen, info, ePlayer)
		self.drawAnimation(screen)
		self.drawRequires(screen, info)
		self.drawConsumedBy(screen, info, ePlayer)
		self.drawProducedBy(screen, info, ePlayer)
		self.drawImprovements(screen, info, ePlayer)
		self.drawAllows(screen, info, ePlayer)
		self.drawEffects(screen, CyGameTextMgr().getBonusHelp(self.eBonus, True)[1:])
		self.drawCivilopedia(screen, info, "TXT_KEY_PEDIA_DESCRIPTION")
	
	def drawAnimation(self, screen):
		screen.addPanel(self.record(self.ANIMATION_BACKGROUND_ID),"", "", True, False, self.BACKGROUND_X, self.BACKGROUND_Y, self.BACKGROUND_W, self.BACKGROUND_H, PanelStyles.PANEL_STYLE_BLUE50)
		screen.addBonusGraphicGFC(self.record(self.ANIMATION_ID), self.eBonus, self.ANIMATION_X, self.ANIMATION_Y, self.ANIMATION_W, self.ANIMATION_H, WidgetTypes.WIDGET_GENERAL, -1, -1, self.ANIMATION_ROTATION_X, self.ANIMATION_ROTATION_Z, self.ANIMATION_SCALE, True)
	
	def drawStats(self, screen, info, ePlayer):
		screen.addListBoxGFC(self.record(self.STATS_ID), "", self.STATS_X, self.STATS_Y, self.STATS_W, self.STATS_H, TableStyles.TABLE_STYLE_EMPTY)
		screen.enableSelect(self.STATS_ID, False)
		
		for k in range(YieldTypes.NUM_YIELD_TYPES):
			iYieldChange = info.getYieldChange(k)
			if (iYieldChange != 0):
				if (iYieldChange > 0):
					szSign = "+"
				else:
					szSign = ""
				szYield = (u"%s: %s%i %c" % (gc.getYieldInfo(k).getDescription().upper(), szSign, iYieldChange, gc.getYieldInfo(k).getChar()))
				screen.appendListBoxStringNoUpdate(self.STATS_ID, u"<font=3>" + szYield + u"</font>", WidgetTypes.WIDGET_GENERAL, 0, 0, CvUtil.FONT_LEFT_JUSTIFY)
		
		screen.updateListBox(self.STATS_ID)
	
	def drawRequires(self, screen, info):
		screen.addPanel(self.record(self.REQUIRES_PANEL_ID), localText.getText("TXT_KEY_PEDIA_REQUIRES", ()), "", False, True, self.REQUIRES_PANEL_X, self.REQUIRES_PANEL_Y, self.REQUIRES_PANEL_W, self.REQUIRES_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		
		screen.attachLabel(self.REQUIRES_PANEL_ID, "", "  ")
		
		eTech = info.getTechReveal()
		if (eTech > -1):
			screen.attachImageButton( self.REQUIRES_PANEL_ID, "", gc.getTechInfo(eTech).getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_TECH, eTech, 1, False )
			screen.attachLabel(self.REQUIRES_PANEL_ID, "", u"(" + localText.getText("TXT_KEY_PEDIA_BONUS_APPEARANCE", ()) + u")")
		eTech = info.getTechCityTrade()
		if (eTech > -1):
			screen.attachImageButton( self.REQUIRES_PANEL_ID, "", gc.getTechInfo(eTech).getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_TECH, eTech, 1, False )
			screen.attachLabel(self.REQUIRES_PANEL_ID, "", u"(" + localText.getText("TXT_KEY_PEDIA_BONUS_TRADE", ()) + u")")
	
	def drawConsumedBy(self, screen, info, ePlayer):
		screen.addPanel(self.record(self.CONSUMED_BY_PANEL_ID), localText.getText("TXT_KEY_PEDIA_CONSUMED_BY", ()), "", False, True, self.CONSUMED_BY_PANEL_X, self.CONSUMED_BY_PANEL_Y, self.CONSUMED_BY_PANEL_W, self.CONSUMED_BY_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		
		screen.attachLabel(self.CONSUMED_BY_PANEL_ID, "", "  ")
		
		for iRel in range(info.getNumRelatedBuildingClasses()):
			eLoopBuildingClass = info.getRelatedBuildingClasses(iRel)
			if ePlayer != -1:
				eLoopBuilding = gc.getCivilizationInfo(gc.getPlayer(ePlayer).getCivilizationType()).getCivilizationBuildings(eLoopBuildingClass)
			else:
				eLoopBuilding = gc.getBuildingClassInfo(eLoopBuildingClass).getRepresentativeBuilding()
			if eLoopBuilding != -1:
				pLoopBuilding = gc.getBuildingInfo(eLoopBuilding)
				if pLoopBuilding.isConsumerOfBonus(self.eBonus):
					screen.attachImageButton(self.CONSUMED_BY_PANEL_ID, "", pLoopBuilding.getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_BUILDING, eLoopBuilding, -1, False)
	
	def drawProducedBy(self, screen, info, ePlayer):
		screen.addPanel(self.record(self.PRODUCED_BY_PANEL_ID), localText.getText("TXT_KEY_PEDIA_PRODUCED_BY", ()), "", False, True, self.PRODUCED_BY_PANEL_X, self.PRODUCED_BY_PANEL_Y, self.PRODUCED_BY_PANEL_W, self.PRODUCED_BY_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		
		screen.attachLabel(self.PRODUCED_BY_PANEL_ID, "", "  ")
		
		for iRel in range(info.getNumRelatedBuildingClasses()):
			eLoopBuildingClass = info.getRelatedBuildingClasses(iRel)
			if ePlayer != -1:
				eLoopBuilding = gc.getCivilizationInfo(gc.getPlayer(ePlayer).getCivilizationType()).getCivilizationBuildings(eLoopBuildingClass)
			else:
				eLoopBuilding = gc.getBuildingClassInfo(eLoopBuildingClass).getRepresentativeBuilding()
			if eLoopBuilding != -1:
				pLoopBuilding = gc.getBuildingInfo(eLoopBuilding)
				if pLoopBuilding.isProducerOfBonus(self.eBonus) or pLoopBuilding.getFreeBonus() == self.eBonus:
					screen.attachImageButton(self.PRODUCED_BY_PANEL_ID, "", pLoopBuilding.getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_BUILDING, eLoopBuilding, -1, False)
	
	def drawImprovements(self, screen, info, ePlayer):
		screen.addPanel(self.record(self.IMPROVEMENTS_PANEL_ID), localText.getText("TXT_KEY_PEDIA_CATEGORY_IMPROVEMENT", ()), "", True, True, self.IMPROVEMENTS_PANEL_X, self.IMPROVEMENTS_PANEL_Y, self.IMPROVEMENTS_PANEL_W, self.IMPROVEMENTS_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		
		for eLoopImprovement in range(gc.getNumImprovementInfos()):
			if ePlayer == -1 or gc.getCivilizationInfo(gc.getPlayer(ePlayer).getCivilizationType()).canProduceImprovement(eLoopImprovement):
				pLoopImprovement = gc.getImprovementInfo(eLoopImprovement)
				bFirst = True
				szYield = u""
				bEffect = False
				for eLoopYield in range(YieldTypes.NUM_YIELD_TYPES):
					iYieldChange = pLoopImprovement.getImprovementBonusYield(self.eBonus, eLoopYield)
					if (iYieldChange != 0):
						bEffect = True
						iYieldChange += pLoopImprovement.getYieldChange(eLoopYield)
						
						if (bFirst):
							bFirst = False
						else:
							szYield += ", "
						
						if (iYieldChange > 0):
							sign = "+"
						else:
							sign = ""
						
						szYield += (u"%s%i%c" % (sign, iYieldChange, gc.getYieldInfo(eLoopYield).getChar()))
				if (bEffect):
					childPanelName = self.IMPROVEMENTS_PANEL_ID + str(eLoopImprovement)
					screen.attachPanel(self.IMPROVEMENTS_PANEL_ID, childPanelName, "", "", False, False, PanelStyles.PANEL_STYLE_EMPTY)
					
					screen.attachLabel(childPanelName, "", "  ")
					screen.attachImageButton(childPanelName, "", pLoopImprovement.getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_IMPROVEMENT, eLoopImprovement, -1, False)
					#screen.attachLabel(childPanelName, "", szYield)
					screen.attachLabel(childPanelName, "", u"<font=4>" + szYield + u"</font>") # K-Mod
	
	def drawAllows(self, screen, info, ePlayer):
		screen.addPanel(self.record(self.ALLOWS_PANEL_ID), localText.getText("TXT_KEY_PEDIA_ALLOWS", ()), "", False, True, self.ALLOWS_PANEL_X, self.ALLOWS_PANEL_Y, self.ALLOWS_PANEL_W, self.ALLOWS_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		
		screen.attachLabel(self.ALLOWS_PANEL_ID, "", "  ")
		
		# Buildings
		for iRel in range(info.getNumRelatedBuildingClasses()):
			eLoopBuildingClass = info.getRelatedBuildingClasses(iRel)
			if ePlayer != -1:
				eLoopBuilding = gc.getCivilizationInfo(gc.getPlayer(ePlayer).getCivilizationType()).getCivilizationBuildings(eLoopBuildingClass)
			else:
				eLoopBuilding = gc.getBuildingClassInfo(eLoopBuildingClass).getRepresentativeBuilding()
			if eLoopBuilding != -1:
				pLoopBuilding = gc.getBuildingInfo(eLoopBuilding)
				bNeedsBonus = False
				for iPrereq in range(pLoopBuilding.getNumPrereqAndBonuses()):
					if (pLoopBuilding.getPrereqAndBonuses(iPrereq) == self.eBonus):
						bNeedsBonus = True
						break
				for iPrereq in range(pLoopBuilding.getNumPrereqOrBonuses()):
					if (pLoopBuilding.getPrereqOrBonuses(iPrereq) == self.eBonus):
						bNeedsBonus = True
						break
				if bNeedsBonus:
					screen.attachImageButton(self.ALLOWS_PANEL_ID, "", pLoopBuilding.getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_BUILDING, eLoopBuilding, -1, False)
		
		# Units
		for iRel in range(info.getNumRelatedUnitClasses()):
			eLoopUnitClass = info.getRelatedUnitClasses(iRel)
			if ePlayer != -1:
				eLoopUnit = gc.getCivilizationInfo(gc.getPlayer(ePlayer).getCivilizationType()).getCivilizationUnits(eLoopUnitClass)
			else:
				eLoopUnit = gc.getUnitClassInfo(eLoopUnitClass).getRepresentativeUnit()
			if eLoopUnit != -1:
				pLoopUnit = gc.getUnitInfo(eLoopUnit)
				bNeedsBonus = False
				for iPrereq in range(pLoopUnit.getNumPrereqAndBonuses()):
					if (pLoopUnit.getPrereqAndBonuses(iPrereq) == self.eBonus):
						bNeedsBonus = True
						break
				for iPrereq in range(pLoopUnit.getNumPrereqOrBonuses()):
					if (pLoopUnit.getPrereqOrBonuses(iPrereq) == self.eBonus):
						bNeedsBonus = True
						break
				if bNeedsBonus:
					if ePlayer != -1:
						szButton = gc.getPlayer(ePlayer).getUnitButton(eLoopUnit)
					else:
						szButton = pLoopUnit.getButton()
					screen.attachImageButton(self.ALLOWS_PANEL_ID, "", szButton, self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_UNIT, eLoopUnit, -1, False)
