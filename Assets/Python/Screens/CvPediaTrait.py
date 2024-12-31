## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
from CvPythonExtensions import *
import CvUtil
import CvPediaPage

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

class CvPediaTrait(CvPediaPage.CvPediaPage):
	"Civilopedia Screen for Traits"

	def __init__(self, section):
		super(CvPediaTrait, self).__init__(section, CivilopediaPageTypes.CIVILOPEDIA_PAGE_TRAIT, WidgetTypes.WIDGET_PEDIA_JUMP_TO_TRAIT, "Trait")
		
		self.eTrait = -1
		
		self.LEADERS_PANEL_ID = self.szWidget + "LeadersPanel"
		self.LEADERS_LIST_ID = self.szWidget + "LeadersList"
		self.LEADERS_PLAYABLE_LABEL_ID = self.szWidget + "LeadersPlayableLabel"
		self.LEADERS_PLAYABLE_CHECKBOX_ID = self.szWidget + "LeadersPlayableCheckbox"
	
	def getData(self):
		return (self.eTrait, -1)
	
	def setData(self, eTrait, iPageData2):
		if (self.eTrait != eTrait):
			self.clear()
			self.eTrait = eTrait
	
	def updatePlayableOnlyFilter(self, on):
		screen = self.getScreen()
		self.drawLeaders(screen)
	
	def getInfo(self):
		return gc.getTraitInfo(self.eTrait)
	
	def calculateLayout(self):
		self.calculateTitleLayout()
		self.calculateSynopsisLayout(-1, self.main.COMMON_ICON_PANEL_SIZE * 2 + self.main.COMMON_MARGIN_H)
		self.calculateTwoColumnLayout(self.SYNOPSIS_PANEL_W, 0)
		
		self.LEADERS_PANEL_X = self.COLUMN2_X
		self.LEADERS_PANEL_Y = self.PAGE_CONTENT_Y
		self.LEADERS_PANEL_W = self.COLUMN2_W
		self.LEADERS_PANEL_H = self.SYNOPSIS_PANEL_H
		
		self.LEADER_ICON_SIZE = 46
		
		self.LEADERS_PLAYABLE_LABEL_W = 120
		self.LEADERS_PLAYABLE_LABEL_X = self.LEADERS_PANEL_X + self.LEADERS_PANEL_W - self.LEADERS_PLAYABLE_LABEL_W
		self.LEADERS_PLAYABLE_LABEL_Y = self.LEADERS_PANEL_Y + 2
		
		self.LEADERS_PLAYABLE_CHECKBOX_W = 24
		self.LEADERS_PLAYABLE_CHECKBOX_H = 24
		self.LEADERS_PLAYABLE_CHECKBOX_X = self.LEADERS_PLAYABLE_LABEL_X - self.LEADERS_PLAYABLE_CHECKBOX_W
		self.LEADERS_PLAYABLE_CHECKBOX_Y = self.LEADERS_PLAYABLE_LABEL_Y + 1
		
		self.calculateTwoColumnLayout(0, 50, 0, 50)
		
		self.EFFECTS_PANEL_X = self.COLUMN1_X
		self.EFFECTS_PANEL_Y = self.SYNOPSIS_PANEL_Y + self.SYNOPSIS_PANEL_H + self.main.COMMON_MARGIN_H
		self.EFFECTS_PANEL_W = self.COLUMN1_W
		self.EFFECTS_PANEL_H = self.main.PAGE_AREA_Y + self.main.PAGE_AREA_H - self.EFFECTS_PANEL_Y
		
		self.CIVILOPEDIA_PANEL_X = self.COLUMN2_X
		self.CIVILOPEDIA_PANEL_Y = self.SYNOPSIS_PANEL_Y + self.SYNOPSIS_PANEL_H + self.main.COMMON_MARGIN_H
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
		
		self.drawLeaders(screen)
		self.drawEffects(screen, CyGameTextMgr().parseTraits(self.eTrait, -1, False, True)[1:])
		self.drawCivilopedia(screen, info, "TXT_KEY_PEDIA_BACKGROUND")

	def drawLeaders(self, screen):
		screen.addPanel(self.record(self.LEADERS_PANEL_ID), localText.getText("TXT_KEY_CONCEPT_LEADERS", ()), "", True, True, self.LEADERS_PANEL_X, self.LEADERS_PANEL_Y, self.LEADERS_PANEL_W, self.LEADERS_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		
		screen.addCheckBoxGFC(self.record(self.LEADERS_PLAYABLE_CHECKBOX_ID), ArtFileMgr.getInterfaceArtInfo("INTERFACE_CHECKBOX_OFF").getPath(), ArtFileMgr.getInterfaceArtInfo("INTERFACE_CHECKBOX_ON").getPath(), self.LEADERS_PLAYABLE_CHECKBOX_X, self.LEADERS_PLAYABLE_CHECKBOX_Y, self.LEADERS_PLAYABLE_CHECKBOX_W, self.LEADERS_PLAYABLE_CHECKBOX_H,  WidgetTypes.WIDGET_PEDIA_TOGGLE_PLAYABLE_FILTER, -1, -1, ButtonStyles.BUTTON_STYLE_LABEL)
		screen.setState(self.LEADERS_PLAYABLE_CHECKBOX_ID, self.section.bPlayableOnly)
		
		screen.setText(self.record(self.LEADERS_PLAYABLE_LABEL_ID), "",  u"<font=3>" + localText.getText("TXT_KEY_PEDIA_FILTER_PLAYABLE_ONLY", ()) + u"</font>", CvUtil.FONT_LEFT_JUSTIFY, self.LEADERS_PLAYABLE_LABEL_X, self.LEADERS_PLAYABLE_LABEL_Y, 0, FontTypes.TITLE_FONT, WidgetTypes.WIDGET_PEDIA_TOGGLE_PLAYABLE_FILTER, -1, -1)
		#screen.setStyle(self.LEADERS_PLAYABLE_LABEL_ID, "Panel_Black25_Style")
		
		#screen.addMultiListControlGFC(self.record(self.LEADERS_LIST_ID), "", self.LEADERS_PANEL_X+15, self.LEADERS_PANEL_Y+40, self.LEADERS_PANEL_W-20, self.LEADERS_PANEL_H-40, 1, self.LEADER_ICON_SIZE, self.LEADER_ICON_SIZE, TableStyles.TABLE_STYLE_STANDARD)
		screen.addMultiListControlGFC(self.record(self.LEADERS_LIST_ID), "", self.LEADERS_PANEL_X+10, self.LEADERS_PANEL_Y+40, self.LEADERS_PANEL_W-20, self.LEADERS_PANEL_H-50, 1, self.LEADER_ICON_SIZE, self.LEADER_ICON_SIZE, TableStyles.TABLE_STYLE_STANDARD)
		
		for eLeader in range(gc.getNumLeaderHeadInfos()):
			pLeader = gc.getLeaderHeadInfo(eLeader)
			if pLeader.hasTrait(self.eTrait):
				if (not self.section.bPlayableOnly or pLeader.isPlayable()):
					screen.appendMultiListButton(self.LEADERS_LIST_ID, pLeader.getButton(), 0, WidgetTypes.WIDGET_PEDIA_JUMP_TO_LEADER, eLeader, -1, False)
	
	def drawStrategy(self, screen):
		screen = self.top.getScreen()
		panelName = self.top.getNextWidgetName()
		screen.addPanel( panelName, "", "", True, True, self.X_TEXT, self.Y_TEXT, self.W_TEXT, self.H_TEXT, PanelStyles.PANEL_STYLE_BLUE50 )
		szText = gc.getNewConceptInfo(self.iConcept).getCivilopedia()
		screen.attachMultilineText( panelName, "Text", szText, WidgetTypes.WIDGET_GENERAL, -1, -1, CvUtil.FONT_LEFT_JUSTIFY)
