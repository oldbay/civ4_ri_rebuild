# Sid Meier's Civilization 4
# Copyright Firaxis Games 2005

#
# Sevopedia 2.3
#   sevotastic.blogspot.com
#   sevotastic@yahoo.com
#
# additional work by Gaurav, Progor, Ket, Vovan, Fitchn, LunarMongoose, Xyth
# see ReadMe for details
#

from CvPythonExtensions import *
import CvUtil
import ScreenInput
import SevoScreenEnums
import string

gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

class SevoPediaImprovement:

	def __init__(self, main):
		self.iImprovement = -1
		self.top = main

		self.X_IMPROVEMENT_ANIMATION = self.top.X_PEDIA_PAGE
		self.Y_IMPROVEMENT_ANIMATION = self.top.Y_PEDIA_PAGE
		self.W_IMPROVEMENT_ANIMATION = 350
		self.H_IMPROVEMENT_ANIMATION = 230
		self.X_ROTATION_IMPROVEMENT_ANIMATION = -20
		self.Z_ROTATION_IMPROVEMENT_ANIMATION = 30
		self.SCALE_ANIMATION = 0.6

		self.X_IMPROVEMENTS_PANE = self.X_IMPROVEMENT_ANIMATION
		self.Y_IMPROVEMENTS_PANE = self.Y_IMPROVEMENT_ANIMATION + self.H_IMPROVEMENT_ANIMATION
		self.W_IMPROVEMENTS_PANE = self.W_IMPROVEMENT_ANIMATION
		self.H_IMPROVEMENTS_PANE = 154

		self.X_EFFECTS = self.X_IMPROVEMENTS_PANE
		self.Y_EFFECTS = self.Y_IMPROVEMENTS_PANE + self.H_IMPROVEMENTS_PANE
		self.W_EFFECTS = self.W_IMPROVEMENTS_PANE
		self.H_EFFECTS = 124

		self.X_HISTORY_PANE = self.X_EFFECTS
		self.Y_HISTORY_PANE = self.Y_EFFECTS + self.H_EFFECTS
		self.W_HISTORY_PANE = self.W_IMPROVEMENTS_PANE
		self.H_HISTORY_PANE = self.top.B_PEDIA_PAGE - self.Y_HISTORY_PANE

		self.X_REQUIRES = self.X_IMPROVEMENT_ANIMATION + self.W_IMPROVEMENT_ANIMATION + 10
		self.Y_REQUIRES = self.top.Y_PEDIA_PAGE
		self.W_REQUIRES = self.top.R_PEDIA_PAGE - self.X_REQUIRES
		self.H_REQUIRES = 110

		self.X_TERRAINS = self.X_IMPROVEMENT_ANIMATION + self.W_IMPROVEMENT_ANIMATION + 10
		self.Y_TERRAINS = self.Y_REQUIRES + self.H_REQUIRES + 10
		self.W_TERRAINS = self.top.R_PEDIA_PAGE - self.X_TERRAINS
		self.H_TERRAINS = 110

		self.X_BONUS_YIELDS_PANE = self.X_REQUIRES
		self.Y_BONUS_YIELDS_PANE = self.Y_TERRAINS + self.H_TERRAINS
		self.W_BONUS_YIELDS_PANE = self.W_REQUIRES
		self.H_BONUS_YIELDS_PANE = self.top.B_PEDIA_PAGE - self.Y_BONUS_YIELDS_PANE



	def interfaceScreen(self, iImprovement):
		self.iImprovement = iImprovement
		screen = self.top.getScreen()
		screen.addImprovementGraphicGFC(self.top.getNextWidgetName(), self.iImprovement, self.X_IMPROVEMENT_ANIMATION, self.Y_IMPROVEMENT_ANIMATION, self.W_IMPROVEMENT_ANIMATION, self.H_IMPROVEMENT_ANIMATION, WidgetTypes.WIDGET_GENERAL, -1, -1, self.X_ROTATION_IMPROVEMENT_ANIMATION, self.Z_ROTATION_IMPROVEMENT_ANIMATION, self.SCALE_ANIMATION, True)
		self.placeYield()
		self.placeSpecial()
		self.placeHistory()
		self.placeRequires()
		self.placeTerrains()
		self.placeBonusYield()



	def placeYield(self):
		screen = self.top.getScreen()
		panelName = self.top.getNextWidgetName()
		screen.addPanel( panelName, localText.getText("TXT_KEY_PEDIA_CATEGORY_IMPROVEMENT", ()), "", True, True, self.X_IMPROVEMENTS_PANE, self.Y_IMPROVEMENTS_PANE, self.W_IMPROVEMENTS_PANE, self.H_IMPROVEMENTS_PANE, PanelStyles.PANEL_STYLE_BLUE50 )
		listName = self.top.getNextWidgetName()
		screen.attachListBoxGFC( panelName, listName, "", TableStyles.TABLE_STYLE_EMPTY )
		screen.enableSelect(listName, False)
		info = gc.getImprovementInfo(self.iImprovement)
		for k in range(YieldTypes.NUM_YIELD_TYPES):
			iYieldChange = gc.getImprovementInfo(self.iImprovement).getYieldChange(k)
			if (iYieldChange != 0):
				szYield = u""
				if (iYieldChange > 0):
					sign = "+"
				else:
					sign = ""
				szYield += (u"%s: %s%i%c" % (gc.getYieldInfo(k).getDescription(), sign, iYieldChange, gc.getYieldInfo(k).getChar()))
				screen.appendListBoxString( listName, szYield, WidgetTypes.WIDGET_GENERAL, -1, -1, CvUtil.FONT_LEFT_JUSTIFY )

		for k in range(YieldTypes.NUM_YIELD_TYPES):
			iYieldChange = gc.getImprovementInfo(self.iImprovement).getIrrigatedYieldChange(k)
			if (iYieldChange != 0):
				szYield = localText.getText("TXT_KEY_PEDIA_IRRIGATED_YIELD", (gc.getYieldInfo(k).getTextKey(), iYieldChange, gc.getYieldInfo(k).getChar()))
				screen.appendListBoxString( listName, szYield, WidgetTypes.WIDGET_GENERAL, -1, -1, CvUtil.FONT_LEFT_JUSTIFY )

		for k in range(YieldTypes.NUM_YIELD_TYPES):
			iYieldChange = gc.getImprovementInfo(self.iImprovement).getHillsYieldChange(k)
			if (iYieldChange != 0):
				szYield = localText.getText("TXT_KEY_PEDIA_HILLS_YIELD", (gc.getYieldInfo(k).getTextKey(), iYieldChange, gc.getYieldInfo(k).getChar()))
				screen.appendListBoxString( listName, szYield, WidgetTypes.WIDGET_GENERAL, -1, -1, CvUtil.FONT_LEFT_JUSTIFY )

		for k in range(YieldTypes.NUM_YIELD_TYPES):
			iYieldChange = gc.getImprovementInfo(self.iImprovement).getRiverSideYieldChange(k)
			if (iYieldChange != 0):
				szYield = localText.getText("TXT_KEY_PEDIA_RIVER_YIELD", (gc.getYieldInfo(k).getTextKey(), iYieldChange, gc.getYieldInfo(k).getChar()))
				screen.appendListBoxString( listName, szYield, WidgetTypes.WIDGET_GENERAL, -1, -1, CvUtil.FONT_LEFT_JUSTIFY )

		for iTech in range(gc.getNumTechInfos()):
			for k in range(YieldTypes.NUM_YIELD_TYPES):
				iYieldChange = gc.getImprovementInfo(self.iImprovement).getTechYieldChanges(iTech, k)
				if (iYieldChange != 0):
					szYield = localText.getText("TXT_KEY_PEDIA_TECH_YIELD", (gc.getYieldInfo(k).getTextKey(), iYieldChange, gc.getYieldInfo(k).getChar(), gc.getTechInfo(iTech).getDescription()))
					screen.appendListBoxString( listName, szYield, WidgetTypes.WIDGET_GENERAL, -1, -1, CvUtil.FONT_LEFT_JUSTIFY )

		for iCivic in range(gc.getNumCivicInfos()):
			for k in range(YieldTypes.NUM_YIELD_TYPES):
				iYieldChange = gc.getCivicInfo(iCivic).getImprovementYieldChanges(self.iImprovement, k)
				if (iYieldChange != 0):
					szYield = localText.getText("TXT_KEY_PEDIA_TECH_YIELD", (gc.getYieldInfo(k).getTextKey(), iYieldChange, gc.getYieldInfo(k).getChar(), gc.getCivicInfo(iCivic).getDescription()))
					screen.appendListBoxString( listName, szYield, WidgetTypes.WIDGET_GENERAL, -1, -1, CvUtil.FONT_LEFT_JUSTIFY )

		for iRoute in range(gc.getNumRouteInfos()):
			for k in range(YieldTypes.NUM_YIELD_TYPES):
				iYieldChange = gc.getImprovementInfo(self.iImprovement).getRouteYieldChanges(iRoute, k)
				if (iYieldChange != 0):
					szYield = localText.getText("TXT_KEY_PEDIA_ROUTE_YIELD", (gc.getYieldInfo(k).getTextKey(), iYieldChange, gc.getYieldInfo(k).getChar(), gc.getRouteInfo(iRoute).getTextKey()))
					screen.appendListBoxString( listName, szYield, WidgetTypes.WIDGET_GENERAL, -1, -1, CvUtil.FONT_LEFT_JUSTIFY )



	def placeSpecial(self):
		screen = self.top.getScreen()
		panelName = self.top.getNextWidgetName()
		screen.addPanel( panelName, localText.getText("TXT_KEY_PEDIA_EFFECTS", ()), "", True, False, self.X_EFFECTS, self.Y_EFFECTS, self.W_EFFECTS, self.H_EFFECTS, PanelStyles.PANEL_STYLE_BLUE50 )
		listName = self.top.getNextWidgetName()
		szSpecialText = CyGameTextMgr().getImprovementHelp(self.iImprovement, True)
		szSpecialText += gc.getImprovementInfo(self.iImprovement).getHelp()
		screen.addMultilineText(listName, szSpecialText, self.X_EFFECTS + 5, self.Y_EFFECTS+5, self.W_EFFECTS-10, self.H_EFFECTS-10, WidgetTypes.WIDGET_GENERAL, -1, -1, CvUtil.FONT_LEFT_JUSTIFY)



	def placeHistory(self):
		screen = self.top.getScreen()
		panelName = self.top.getNextWidgetName()
		screen.addPanel( panelName, localText.getText("Description", ()), "", True, True, self.X_HISTORY_PANE, self.Y_HISTORY_PANE, self.W_HISTORY_PANE, self.H_HISTORY_PANE, PanelStyles.PANEL_STYLE_BLUE50)
		screen.attachLabel(panelName, "", "  ")
		textName = self.top.getNextWidgetName()
		screen.addMultilineText( textName, gc.getImprovementInfo(self.iImprovement).getCivilopedia(), self.X_HISTORY_PANE + 15, self.Y_HISTORY_PANE + 40, self.W_HISTORY_PANE - (15 * 2), self.H_HISTORY_PANE - (15 * 2) - 25, WidgetTypes.WIDGET_GENERAL, -1, -1, CvUtil.FONT_LEFT_JUSTIFY)



	def placeBonusYield(self):
		screen = self.top.getScreen()
		panelName = self.top.getNextWidgetName()
		screen.addPanel( panelName, localText.getText("TXT_KEY_PEDIA_BONUS_YIELDS", ()), "", True, True, self.X_BONUS_YIELDS_PANE, self.Y_BONUS_YIELDS_PANE, self.W_BONUS_YIELDS_PANE, self.H_BONUS_YIELDS_PANE, PanelStyles.PANEL_STYLE_BLUE50 )
		info = gc.getImprovementInfo(self.iImprovement)
		for j in range(gc.getNumBonusInfos()):
			bFirst = True
			szYield = u""
			bEffect = False
			for k in range(YieldTypes.NUM_YIELD_TYPES):
				iYieldChange = info.getImprovementBonusYield(j, k)
				if (iYieldChange != 0):
					bEffect = True
					# Uncomment for 3.13 behavior. Note that Uranium shows incorrect hammer yield (should be +2)
					#iYieldChange += info.getYieldChange(k)
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
				childPanelName = self.top.getNextWidgetName()
				screen.attachPanel(panelName, childPanelName, "", "", False, False, PanelStyles.PANEL_STYLE_EMPTY)
				screen.attachLabel(childPanelName, "", "  ")
				screen.attachImageButton( childPanelName, "", gc.getBonusInfo(j).getButton(), GenericButtonSizes.BUTTON_SIZE_CUSTOM, WidgetTypes.WIDGET_PEDIA_JUMP_TO_BONUS, j, 1, False )
				screen.attachLabel(childPanelName, "", u"<font=4>" + szYield + u"</font>")



	def placeRequires(self):
		screen = self.top.getScreen()
		panelName = self.top.getNextWidgetName()
		screen.addPanel( panelName, localText.getText("TXT_KEY_PEDIA_REQUIRES", ()), "", False, True, self.X_REQUIRES, self.Y_REQUIRES, self.W_REQUIRES, self.H_REQUIRES, PanelStyles.PANEL_STYLE_BLUE50 )
		screen.attachLabel(panelName, "", "  ")
		for iBuild in range(gc.getNumBuildInfos()):
			if (gc.getBuildInfo(iBuild).getImprovement() == self.iImprovement):
				iTech = gc.getBuildInfo(iBuild).getTechPrereq()
				if (iTech > -1):
					screen.attachImageButton( panelName, "", gc.getTechInfo(iTech).getButton(), GenericButtonSizes.BUTTON_SIZE_CUSTOM, WidgetTypes.WIDGET_PEDIA_JUMP_TO_TECH, iTech, 1, False)



	def placeTerrains(self):
		screen = self.top.getScreen()
		panelName = self.top.getNextWidgetName()
		screen.addPanel( panelName, localText.getText("TXT_KEY_PEDIA_CATEGORY_TERRAIN", ()), "", False, True, self.X_TERRAINS, self.Y_TERRAINS, self.W_TERRAINS, self.H_TERRAINS, PanelStyles.PANEL_STYLE_BLUE50 )
		screen.attachLabel(panelName, "", "  ")
		for iTerrain in range(gc.getNumTerrainInfos()):
			if gc.getImprovementInfo(self.iImprovement).getTerrainMakesValid(iTerrain):
				screen.attachImageButton(panelName, "", gc.getTerrainInfo(iTerrain).getButton(), GenericButtonSizes.BUTTON_SIZE_CUSTOM, WidgetTypes.WIDGET_PEDIA_JUMP_TO_TERRAIN, iTerrain, 1, False)
		if gc.getImprovementInfo(self.iImprovement).isHillsMakesValid():
			iHills = gc.getInfoTypeForString('TERRAIN_HILL')
			screen.attachImageButton(panelName, "", gc.getTerrainInfo(iHills).getButton(), GenericButtonSizes.BUTTON_SIZE_CUSTOM, WidgetTypes.WIDGET_PEDIA_JUMP_TO_TERRAIN, iHills, 1, False)
		for iFeature in range(gc.getNumFeatureInfos()):
			if gc.getImprovementInfo(self.iImprovement).getFeatureMakesValid(iFeature):
				screen.attachImageButton(panelName, "", gc.getFeatureInfo(iFeature).getButton(), GenericButtonSizes.BUTTON_SIZE_CUSTOM, WidgetTypes.WIDGET_PEDIA_JUMP_TO_FEATURE, iFeature, 1, False)



	def handleInput (self, inputClass):
		return 0
