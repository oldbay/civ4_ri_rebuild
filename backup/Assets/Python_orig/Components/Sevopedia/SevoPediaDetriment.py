# Sid Meier's Civilization 4
# Copyright Firaxis Games 2005

#
# Sevopedia 2.3
#   sevotastic.blogspot.com
#   sevotastic@yahoo.com
#
# additional work by Gaurav, Progor, Ket, Vovan, Fitchn, LunarMongoose
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

class SevoPediaDetriment:

	def __init__(self, main):
		self.iDetriment = -1
		self.top = main
		
		self.X_ICON_PANE = self.top.X_PEDIA_PAGE
		self.Y_ICON_PANE = self.top.Y_PEDIA_PAGE
		self.W_ICON_PANE = self.top.W_PEDIA_PAGE
		self.H_ICON_PANE = 210

		self.W_ICON = 150
		self.H_ICON = 150
		self.X_ICON = self.X_ICON_PANE + (self.H_ICON_PANE - self.H_ICON) / 2
		self.Y_ICON = self.Y_ICON_PANE + (self.H_ICON_PANE - self.H_ICON) / 2
		self.ICON_SIZE = 64

		self.X_PROMOTIONS_PANE = self.X_ICON_PANE
		self.W_PROMOTIONS_PANE = self.W_ICON_PANE
		self.Y_PROMOTIONS_PANE = self.Y_ICON_PANE + self.H_ICON_PANE + 10
		self.H_PROMOTIONS_PANE = 110

		self.X_SUPPLIED_BY_PANE = self.X_ICON_PANE
		self.W_SUPPLIED_BY_PANE = self.W_ICON_PANE
		self.Y_SUPPLIED_BY_PANE = self.Y_PROMOTIONS_PANE + self.H_PROMOTIONS_PANE + 10
		self.H_SUPPLIED_BY_PANE = 110

		self.X_SPECIAL_PANE = self.X_ICON_PANE
		self.W_SPECIAL_PANE = self.W_ICON_PANE
		self.Y_SPECIAL_PANE = self.Y_SUPPLIED_BY_PANE + self.H_SUPPLIED_BY_PANE + 10
		self.H_SPECIAL_PANE = self.top.B_PEDIA_PAGE - self.Y_SPECIAL_PANE



	def interfaceScreen(self, iDetriment):
		self.iDetriment = iDetriment
		screen = self.top.getScreen()

		screen.addPanel( self.top.getNextWidgetName(), "", "", False, False, self.X_ICON_PANE, self.Y_ICON_PANE, self.W_ICON_PANE, self.H_ICON_PANE, PanelStyles.PANEL_STYLE_BLUE50)
		screen.addPanel(self.top.getNextWidgetName(), "", "", False, False, self.X_ICON, self.Y_ICON, self.W_ICON, self.H_ICON, PanelStyles.PANEL_STYLE_MAIN)
		screen.addDDSGFC(self.top.getNextWidgetName(), gc.getDetrimentInfo(self.iDetriment).getButton(), self.X_ICON + self.W_ICON/2 - self.ICON_SIZE/2, self.Y_ICON + self.H_ICON/2 - self.ICON_SIZE/2, self.ICON_SIZE, self.ICON_SIZE, WidgetTypes.WIDGET_GENERAL, -1, -1 )

		self.placePromotions()
		self.placeSuppliedBy()
		self.placeSpecial()



	def placePromotions(self):
		screen = self.top.getScreen()
		panelName = self.top.getNextWidgetName()
		screen.addPanel(panelName, localText.getText("TXT_KEY_PEDIA_CATEGORY_PROMOTION", ()), "", False, True, self.X_PROMOTIONS_PANE, self.Y_PROMOTIONS_PANE, self.W_PROMOTIONS_PANE, self.H_PROMOTIONS_PANE, PanelStyles.PANEL_STYLE_BLUE50)
		screen.attachLabel(panelName, "", "  ")
		for j in range(gc.getNUM_DETRIMENT_PROMOTIONS()):
			iPromotion = gc.getDetrimentInfo(self.iDetriment).getPromotions(j)
			if iPromotion > -1:
				screen.attachImageButton(panelName, "", gc.getPromotionInfo(iPromotion).getButton(), GenericButtonSizes.BUTTON_SIZE_CUSTOM, WidgetTypes.WIDGET_PEDIA_JUMP_TO_PROMOTION, iPromotion, 1, False)



	def placeSuppliedBy(self):
		screen = self.top.getScreen()
		panelName = self.top.getNextWidgetName()
		screen.addPanel(panelName, localText.getText("TXT_KEY_DETRIMENT_INFLICTED_BY", ()), "", False, True, self.X_SUPPLIED_BY_PANE, self.Y_SUPPLIED_BY_PANE, self.W_SUPPLIED_BY_PANE, self.H_SUPPLIED_BY_PANE, PanelStyles.PANEL_STYLE_BLUE50)
		screen.attachLabel(panelName, "", "  ")
		for eLoopUnit in range(gc.getNumUnitInfos()):
			if (eLoopUnit != -1):
				classInfo = gc.getUnitClassInfo(gc.getUnitInfo(eLoopUnit).getUnitClassType())
				for i in range(gc.getNumDetrimentInfos()):
					eLoopDetriment = classInfo.getDetrimentTypes(i)
					if eLoopDetriment == self.iDetriment:
						screen.attachImageButton(panelName, "", gc.getUnitInfo(eLoopUnit).getButton(), GenericButtonSizes.BUTTON_SIZE_CUSTOM, WidgetTypes.WIDGET_PEDIA_JUMP_TO_UNIT, eLoopUnit, 1, False)
						break
					elif eLoopDetriment == -1:
						break
		for eLoopImprovement in range(gc.getNumImprovementInfos()):
			if (eLoopImprovement != -1):
				improvementInfo = gc.getImprovementInfo(eLoopImprovement)
				for i in range(gc.getNumDetrimentInfos()):
					eLoopDetriment = improvementInfo.getDetrimentTypes(i)
					if eLoopDetriment == self.iDetriment:
						screen.attachImageButton(panelName, "", improvementInfo.getButton(), GenericButtonSizes.BUTTON_SIZE_CUSTOM, WidgetTypes.WIDGET_PEDIA_JUMP_TO_IMPROVEMENT, eLoopImprovement, 1, False)
						break
					elif eLoopDetriment == -1:
						break



	def placeSpecial(self):
		screen = self.top.getScreen()
		panelName = self.top.getNextWidgetName()
		screen.addPanel( panelName, localText.getText("TXT_KEY_PEDIA_SPECIAL_ABILITIES", ()), "", True, False, self.X_SPECIAL_PANE, self.Y_SPECIAL_PANE, self.W_SPECIAL_PANE, self.H_SPECIAL_PANE, PanelStyles.PANEL_STYLE_BLUE50 )
		listName = self.top.getNextWidgetName()
		screen.attachListBoxGFC( panelName, listName, "", TableStyles.TABLE_STYLE_EMPTY )
		screen.enableSelect(listName, False)
		szSpecialText = CyGameTextMgr().getDetrimentHelp(self.iDetriment, True)
		splitText = string.split( szSpecialText, "\n" )
		for special in splitText:
			if len( special ) != 0:
				screen.appendListBoxString( listName, special, WidgetTypes.WIDGET_GENERAL, -1, -1, CvUtil.FONT_LEFT_JUSTIFY )



	def handleInput (self, inputClass):
		return 0
