## BugEraColorsOptionsTab
##
## Tab for the BUG EraColors Options.
##
## Copyright (c) 2007-2008 The BUG Mod.
##
## Author: EmperorFool/Cruel

import BugOptionsTab

class BugColorsOptionsTab(BugOptionsTab.BugOptionsTab):
	"Bug Colors Options Tab"
	
	def __init__(self, screen):
		BugOptionsTab.BugOptionsTab.__init__(self, "Colors", "Colors")

	def create(self, screen):
		tab = self.createTab(screen)
		panel = self.createMainPanel(screen)
		upperPanel = self.addOneColumnLayout(screen, panel)
		
		leftPanel, centerPanel, rightPanel = self.addThreeColumnLayout(screen, upperPanel, "Colors")
		
		self.addCheckbox(screen, leftPanel, "NJAGC__ShowEra")
		self.addCheckbox(screen, leftPanel, "NJAGC__ShowEraColor")
		self.addCheckbox(screen, leftPanel, "Advisors__ShowTechEra")
		
		centerPanelL, centerPanelR = self.addTwoColumnLayout(screen, centerPanel, "ShowEraColor_Column")
		self.addColorDropdown(screen, centerPanelL, centerPanelR, "NJAGC__Color_ERA_ANCIENT", True)
		self.addColorDropdown(screen, centerPanelL, centerPanelR, "NJAGC__Color_ERA_CLASSICAL", True)
		self.addColorDropdown(screen, centerPanelL, centerPanelR, "NJAGC__Color_ERA_MEDIEVAL", True)
		self.addColorDropdown(screen, centerPanelL, centerPanelR, "NJAGC__Color_ERA_RENAISSANCE", True)
		self.addColorDropdown(screen, centerPanelL, centerPanelR, "NJAGC__Color_ERA_INDUSTRIAL", True)
		self.addColorDropdown(screen, centerPanelL, centerPanelR, "NJAGC__Color_ERA_MODERN", True)
		self.addColorDropdown(screen, centerPanelL, centerPanelR, "NJAGC__Color_ERA_FUTURE", True)
		self.addSpacer(screen, centerPanel, "EraColors_Tab")