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

class CvPediaImprovement(CvPediaPage.CvPediaPage):
	"Civilopedia Screen for tile Improvements"

	def __init__(self, section):
		super(CvPediaImprovement, self).__init__(section, CivilopediaPageTypes.CIVILOPEDIA_PAGE_IMPROVEMENT, WidgetTypes.WIDGET_PEDIA_JUMP_TO_IMPROVEMENT, "Improvement")
		
		self.eImprovement = -1
		
		self.ANIMATION_ID = self.szWidget + "Animation"
		self.IMPROVEMENTS_PANEL_ID = self.szWidget + "ImprovementsPanel"
		self.IMPROVEMENTS_TEXT_ID = self.szWidget + "ImprovementsText"
		self.BONUS_YIELDS_PANEL_ID = self.szWidget + "BonusYieldsPanel"
		self.BONUS_YIELDS_EFFECT_PANEL_ID = self.szWidget + "BonusYieldsEffectPanel"
		self.REQUIRES_PANEL_ID = self.szWidget + "RequiresPanel"
		self.TERRAIN_PANEL_ID = self.szWidget + "TerrainPanel"
		self.EFFECTS_PANEL_ID = self.szWidget + "EffectsPanel"
		self.ANIMATION_BACKGROUND_ID = self.szWidget + "AnimationBackground"
	
	def getData(self):
		return (self.eImprovement, -1)
	
	def setData(self, eImprovement, iPageData2):
		if (self.eImprovement != eImprovement):
			self.clear()
			self.eImprovement = eImprovement
	
	def getInfo(self):
		return gc.getImprovementInfo(self.eImprovement)

	def calculateLayout(self):
		self.calculateTitleLayout()
		self.calculateTwoColumnLayout()
		self.calculateSynopsisLayout(self.COLUMN1_W)
		self.calculateAnimationLayout(0.8)
		self.calculateFourColumnLayout(25, 20, 125, 20, 150, 10, 0, 50)
		
		self.REQUIRES_PANEL_X = self.COLUMN1_X
		self.REQUIRES_PANEL_Y = self.SYNOPSIS_PANEL_Y + self.SYNOPSIS_PANEL_H + self.main.COMMON_MARGIN_H
		self.REQUIRES_PANEL_W = self.COLUMN1_W
		self.REQUIRES_PANEL_H = self.main.COMMON_ICON_PANEL_SIZE
		
		self.TERRAIN_PANEL_X = self.COLUMN2_X
		self.TERRAIN_PANEL_Y = self.REQUIRES_PANEL_Y
		self.TERRAIN_PANEL_W = self.COLUMN2_W
		self.TERRAIN_PANEL_H = self.main.COMMON_ICON_PANEL_SIZE
		
		self.BONUS_YIELDS_PANEL_X = self.COLUMN3_X
		self.BONUS_YIELDS_PANEL_Y = self.SYNOPSIS_PANEL_Y + self.SYNOPSIS_PANEL_H + self.main.COMMON_MARGIN_H
		self.BONUS_YIELDS_PANEL_W = self.COLUMN3_W
		self.BONUS_YIELDS_PANEL_H = self.main.PAGE_AREA_Y + self.main.PAGE_AREA_H - self.BONUS_YIELDS_PANEL_Y
		
		self.CIVILOPEDIA_PANEL_X = self.COLUMN4_X
		self.CIVILOPEDIA_PANEL_Y = self.SYNOPSIS_PANEL_Y + self.SYNOPSIS_PANEL_H + self.main.COMMON_MARGIN_H
		self.CIVILOPEDIA_PANEL_W = self.COLUMN4_W
		self.CIVILOPEDIA_PANEL_H = self.main.PAGE_AREA_Y + self.main.PAGE_AREA_H - self.CIVILOPEDIA_PANEL_Y
		
		self.calculateThreeColumnLayout(0, 100, self.COLUMN3_W, 0, self.COLUMN4_W, 0)
		
		#self.calculateTwoColumnLayout(0, 50, self.COLUMN3_W, 0)
		
		self.IMPROVEMENTS_PANEL_X = self.COLUMN1_X
		self.IMPROVEMENTS_PANEL_Y = self.REQUIRES_PANEL_Y + self.REQUIRES_PANEL_H + self.main.COMMON_MARGIN_H
		self.IMPROVEMENTS_PANEL_W = self.COLUMN1_W
		self.IMPROVEMENTS_PANEL_H = (self.main.PAGE_AREA_Y + self.main.PAGE_AREA_H - self.IMPROVEMENTS_PANEL_Y - self.main.COMMON_MARGIN_H) / 2
		
		self.EFFECTS_PANEL_X = self.COLUMN1_X
		self.EFFECTS_PANEL_Y = self.IMPROVEMENTS_PANEL_Y + self.IMPROVEMENTS_PANEL_H + self.main.COMMON_MARGIN_H
		self.EFFECTS_PANEL_W = self.COLUMN1_W
		self.EFFECTS_PANEL_H = self.main.PAGE_AREA_Y + self.main.PAGE_AREA_H - self.EFFECTS_PANEL_Y

	def draw(self):
		if (not self.isEmpty()):
			return
		
		screen = self.getScreen()
		info = self.getInfo()
		eActivePlayer = gc.getGame().getActivePlayer()
		
		self.drawTitle(screen, info, eActivePlayer)
		self.drawSynopsis(screen, info, eActivePlayer)
		self.drawAnimation(screen)
		self.drawImprovements(screen, CyGameTextMgr().getImprovementHelp(self.eImprovement, False, True)[1:])
		self.drawRequires(screen)
		self.drawTerrain(screen, info)
		self.drawEffects(screen, CyGameTextMgr().getImprovementHelp(self.eImprovement, True, False)[1:])
		self.drawBonusYields(screen, info)
		self.drawCivilopedia(screen, info, "TXT_KEY_PEDIA_DESCRIPTION")

	def drawAnimation(self, screen):
		screen.addPanel(self.record(self.ANIMATION_BACKGROUND_ID),"", "", True, False, self.BACKGROUND_X, self.BACKGROUND_Y, self.BACKGROUND_W, self.BACKGROUND_H, PanelStyles.PANEL_STYLE_BLUE50)
		screen.addImprovementGraphicGFC(self.record(self.ANIMATION_ID), self.eImprovement, self.ANIMATION_X, self.ANIMATION_Y, self.ANIMATION_W, self.ANIMATION_H, WidgetTypes.WIDGET_GENERAL, -1, -1, self.ANIMATION_ROTATION_X, self.ANIMATION_ROTATION_Z, self.ANIMATION_SCALE, True)

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

	def drawRequires(self, screen):
		screen.addPanel(self.record(self.REQUIRES_PANEL_ID), localText.getText("TXT_KEY_PEDIA_REQUIRES", ()), "", False, True, self.REQUIRES_PANEL_X, self.REQUIRES_PANEL_Y, self.REQUIRES_PANEL_W, self.REQUIRES_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		
		screen.attachLabel(self.REQUIRES_PANEL_ID, "", "  ")
		
		for eBuild in range(gc.getNumBuildInfos()):
			if (gc.getBuildInfo(eBuild).getImprovement() == self.eImprovement):	 
				eTech = gc.getBuildInfo(eBuild).getTechPrereq()
				if (eTech > -1):
					screen.attachImageButton(self.REQUIRES_PANEL_ID, "", gc.getTechInfo(eTech).getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_TECH, eTech, -1, False)

	def drawTerrain(self, screen, info):
		screen.addPanel(self.record(self.TERRAIN_PANEL_ID), localText.getText("TXT_KEY_PEDIA_CATEGORY_TERRAIN", ()), "", False, True, self.TERRAIN_PANEL_X, self.TERRAIN_PANEL_Y, self.TERRAIN_PANEL_W, self.TERRAIN_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		
		screen.attachLabel(self.TERRAIN_PANEL_ID, "", "  ")
		
		for eTerrain in range(gc.getNumTerrainInfos()):
			if info.getTerrainMakesValid(eTerrain):
				screen.attachImageButton(self.TERRAIN_PANEL_ID, "", gc.getTerrainInfo(eTerrain).getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_TERRAIN, eTerrain, -1, False)
		
		if info.isHillsMakesValid():
			eHills = gc.getInfoTypeForString('TERRAIN_HILL')
			screen.attachImageButton(self.TERRAIN_PANEL_ID, "", gc.getTerrainInfo(eHills).getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_TERRAIN, eHills, -1, False)
		
		for eFeature in range(gc.getNumFeatureInfos()):
			if info.getFeatureMakesValid(eFeature):
				screen.attachImageButton(self.TERRAIN_PANEL_ID, "", gc.getFeatureInfo(eFeature).getButton(), self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_FEATURE, eFeature, -1, False)
	
	def drawImprovements(self, screen, szText):
		screen.addPanel(self.record(self.IMPROVEMENTS_PANEL_ID), localText.getText("TXT_KEY_PEDIA_CATEGORY_IMPROVEMENT", ()), "", True, True, self.IMPROVEMENTS_PANEL_X, self.IMPROVEMENTS_PANEL_Y, self.IMPROVEMENTS_PANEL_W, self.IMPROVEMENTS_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		
		"""
		szYield = u""
		for k in range(YieldTypes.NUM_YIELD_TYPES):
			iYieldChange = info.getYieldChange(k)
			if (iYieldChange != 0):										
				if (iYieldChange > 0):
					sign = "+"
				else:
					sign = ""
					
				szYield += (u"%s: %s%i%c\n" % (gc.getYieldInfo(k).getDescription(), sign, iYieldChange, gc.getYieldInfo(k).getChar()))
		
		for k in range(YieldTypes.NUM_YIELD_TYPES):
			iYieldChange = info.getIrrigatedYieldChange(k)
			if (iYieldChange != 0):
				szYield += localText.getText("TXT_KEY_PEDIA_IRRIGATED_YIELD", (gc.getYieldInfo(k).getTextKey(), iYieldChange, gc.getYieldInfo(k).getChar())) + u"\n"
		
		for k in range(YieldTypes.NUM_YIELD_TYPES):
			iYieldChange = info.getHillsYieldChange(k)
			if (iYieldChange != 0):										
				szYield += localText.getText("TXT_KEY_PEDIA_HILLS_YIELD", (gc.getYieldInfo(k).getTextKey(), iYieldChange, gc.getYieldInfo(k).getChar())) + u"\n"	
		
		for k in range(YieldTypes.NUM_YIELD_TYPES):
			iYieldChange = info.getRiverSideYieldChange(k)
			if (iYieldChange != 0):										
				szYield += localText.getText("TXT_KEY_PEDIA_RIVER_YIELD", (gc.getYieldInfo(k).getTextKey(), iYieldChange, gc.getYieldInfo(k).getChar())) + u"\n"	
		
		for iTech in range(gc.getNumTechInfos()):
			for k in range(YieldTypes.NUM_YIELD_TYPES):
				iYieldChange = info.getTechYieldChanges(iTech, k)
				if (iYieldChange != 0):										
					szYield += localText.getText("TXT_KEY_PEDIA_TECH_YIELD", (gc.getYieldInfo(k).getTextKey(), iYieldChange, gc.getYieldInfo(k).getChar(), gc.getTechInfo(iTech).getTextKey())) + u"\n"	
		
		for iCivic in range(gc.getNumCivicInfos()):
			for k in range(YieldTypes.NUM_YIELD_TYPES):
				iYieldChange = gc.getCivicInfo(iCivic).getImprovementYieldChanges(self.eImprovement, k)
				if (iYieldChange != 0):										
					szYield += localText.getText("TXT_KEY_PEDIA_TECH_YIELD", (gc.getYieldInfo(k).getTextKey(), iYieldChange, gc.getYieldInfo(k).getChar(), gc.getCivicInfo(iCivic).getTextKey())) + u"\n"		
		
		for iRoute in range(gc.getNumRouteInfos()):
			for k in range(YieldTypes.NUM_YIELD_TYPES):
				iYieldChange = info.getRouteYieldChanges(iRoute, k)
				if (iYieldChange != 0):										
					szYield += localText.getText("TXT_KEY_PEDIA_ROUTE_YIELD", (gc.getYieldInfo(k).getTextKey(), iYieldChange, gc.getYieldInfo(k).getChar(), gc.getRouteInfo(iRoute).getTextKey())) + u"\n"
		"""
		
		screen.addMultilineText(self.record(self.IMPROVEMENTS_TEXT_ID), szText, self.IMPROVEMENTS_PANEL_X+5, self.IMPROVEMENTS_PANEL_Y+30, self.IMPROVEMENTS_PANEL_W-10, self.IMPROVEMENTS_PANEL_H-35, WidgetTypes.WIDGET_GENERAL, -1, -1, CvUtil.FONT_LEFT_JUSTIFY)	
	
	def drawBonusYields(self, screen, info):
		screen.addPanel(self.record(self.BONUS_YIELDS_PANEL_ID), localText.getText("TXT_KEY_PEDIA_BONUS_YIELDS", ()), "", True, True, self.BONUS_YIELDS_PANEL_X, self.BONUS_YIELDS_PANEL_Y, self.BONUS_YIELDS_PANEL_W, self.BONUS_YIELDS_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		
		for j in range(gc.getNumBonusInfos()):
			bFirst = True
			szYield = u""
			bEffect = False
			
			for k in range(YieldTypes.NUM_YIELD_TYPES):
				iYieldChange = info.getImprovementBonusYield(j, k)
				if (iYieldChange != 0):
					bEffect = True
					if (bFirst):
						bFirst = False
					else:
						szYield += u", "
					
					if (iYieldChange > 0):
						sign = u"+"
					else:
						sign = u""
					
					szYield += (u"%s%i%c" % (sign, iYieldChange, gc.getYieldInfo(k).getChar()))
			
			if (bEffect):
				szChildPanelID = self.BONUS_YIELDS_EFFECT_PANEL_ID + str(j)
				screen.attachPanel(self.BONUS_YIELDS_PANEL_ID, szChildPanelID, "", "", False, False, PanelStyles.PANEL_STYLE_EMPTY)
				
				screen.attachLabel(szChildPanelID, "", "  ")
				sButton = gc.getBonusInfo(j).getButton()
				sButton = sButton[:-4] + "_x64.dds"
				screen.attachImageButton(szChildPanelID, "", sButton, self.main.COMMON_ICON_SIZE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_BONUS, j, -1, False)
				screen.attachLabel(szChildPanelID, "", u"<font=4>" + szYield + u"</font>")
