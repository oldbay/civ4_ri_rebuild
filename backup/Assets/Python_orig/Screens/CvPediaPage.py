## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
from CvPythonExtensions import *
from CvPediaScreen import CvPediaScreen
import CvUtil
import string

# globals
localText = CyTranslator()

class CvPediaPage(CvPediaScreen):
	"Civilopedia Page Base"

	def __init__(self, section, ePage, eWidget, szWidget):
		super(CvPediaPage, self).__init__()
		
		self.main = section.main
		self.section = section
		self.ePage = ePage
		self.eWidget = eWidget
		self.szWidget = "Pedia" + szWidget + "Page"
		
		self.TITLE_PANEL_ID = self.szWidget + "TitlePanel"
		self.TITLE_ID = self.szWidget + "Title"
		self.SYNOPSIS_PANEL_ID = self.szWidget + "InfoPanel"
		self.ICON_PANEL_ID = self.szWidget + "IconPanel"
		self.ICON_ID = self.szWidget + "Icon"
		self.STATS_ID = self.szWidget + "Stats"
		self.ANIMATION_ID = self.szWidget + "Animation"
		self.EFFECTS_PANEL_ID = self.szWidget + "EffectsPanel"
		self.EFFECTS_TEXT_ID = self.szWidget + "EffectsText"
		self.SPECIAL_PANEL_ID = self.szWidget + "SpecialPanel"
		self.SPECIAL_TEXT_ID = self.szWidget + "SpecialText"
		self.CIVILOPEDIA_PANEL_ID = self.szWidget + "CivilopediaPanel"
		self.CIVILOPEDIA_TEXT_ID = self.szWidget + "CivilopediaText"
	
	def handleInput(self, input):
		return 0
	
	def update(self, fDelta):
		return
	
	def getNextWidgetName(self):
		return self.record(self.szWidget + str(len(self.widgets)))
	
	def updateTitle(self):
		return
	
	def calculateLayout(self):
		return
	
	def calculateTitleLayout(self):
		self.TITLE_PANEL_X = self.main.PAGE_AREA_X
		self.TITLE_PANEL_Y = self.main.PAGE_AREA_Y
		self.TITLE_PANEL_W = self.main.PAGE_AREA_W
		self.TITLE_PANEL_H = 50
		
		self.TITLE_X = self.main.PAGE_AREA_X + self.main.PAGE_AREA_W / 2 # Center justified
		self.TITLE_Y = self.main.PAGE_AREA_Y + 12
		
		self.PAGE_CONTENT_Y = self.TITLE_PANEL_Y + self.TITLE_PANEL_H + self.main.COMMON_MARGIN_H / 2
	
	def calculateSynopsisLayout(self, iWidth = -1, iHeight = -1):
		self.ICON_SIZE = 64
		
		if (iHeight > -1):
			iOuterPadding = (iHeight - self.ICON_SIZE) / 4
			iInnerPadding = iOuterPadding
		elif (self.main.SCREEN_H >= 1024):
			iOuterPadding = 32
			iInnerPadding = 32
		elif (self.main.SCREEN_H >= 1400):
			iOuterPadding = 48
			iInnerPadding = 48
		else:
			iOuterPadding = 16
			iInnerPadding = 24
		
		self.SYNOPSIS_PANEL_X = self.main.PAGE_AREA_X
		self.SYNOPSIS_PANEL_Y = self.PAGE_CONTENT_Y
		if (iWidth > -1):
			self.SYNOPSIS_PANEL_W = iWidth
		else:
			self.SYNOPSIS_PANEL_W = self.ICON_SIZE + (iInnerPadding * 2) + (iOuterPadding * 2)
		
		self.ICON_PANEL_X = self.SYNOPSIS_PANEL_X + iOuterPadding
		self.ICON_PANEL_Y = self.SYNOPSIS_PANEL_Y + iOuterPadding + 2
		self.ICON_PANEL_W = self.ICON_SIZE + (iInnerPadding * 2) # 150
		self.ICON_PANEL_H = self.ICON_SIZE + (iInnerPadding * 2) # 150
		
		if (self.main.SCREEN_H <= 1024):
			self.SYNOPSIS_PANEL_H = self.ICON_PANEL_H + (iOuterPadding * 2) # 210
		else:
			self.SYNOPSIS_PANEL_H = self.ICON_PANEL_H + (iOuterPadding * 2) + (self.main.SCREEN_H / 10)
		
		self.ICON_X = self.ICON_PANEL_X + self.ICON_PANEL_W/2 - self.ICON_SIZE/2
		self.ICON_Y = self.ICON_PANEL_Y + self.ICON_PANEL_H/2 - self.ICON_SIZE/2
		
		self.STATS_X = self.ICON_PANEL_X + self.ICON_PANEL_W + 15
		self.STATS_Y = self.ICON_PANEL_Y + 15
		self.STATS_W = self.SYNOPSIS_PANEL_W - 30 - self.ICON_PANEL_W - 15 - 15
		self.STATS_H = self.SYNOPSIS_PANEL_H - 30
	
	def calculateAnimationLayout(self, iScale = 1.0):
		self.ANIMATION_X = self.SYNOPSIS_PANEL_X + self.SYNOPSIS_PANEL_W + self.main.COMMON_MARGIN_W + 2
		self.BACKGROUND_X = self.SYNOPSIS_PANEL_X + self.SYNOPSIS_PANEL_W + self.main.COMMON_MARGIN_W
		self.ANIMATION_Y = self.SYNOPSIS_PANEL_Y + 8
		self.BACKGROUND_Y = self.SYNOPSIS_PANEL_Y
		self.ANIMATION_W = self.main.PAGE_AREA_X + self.main.PAGE_AREA_W - self.BACKGROUND_X
		self.BACKGROUND_W = self.main.PAGE_AREA_X + self.main.PAGE_AREA_W - self.BACKGROUND_X
		if self.ANIMATION_W > 750: self.ANIMATION_W = 750
		if self.ANIMATION_W == 750: self.ANIMATION_X = self.ANIMATION_X + (self.BACKGROUND_W - 750)/2
		self.ANIMATION_H = self.SYNOPSIS_PANEL_H - 14
		self.BACKGROUND_H = self.SYNOPSIS_PANEL_H
		self.ANIMATION_ROTATION_X = -20
		self.ANIMATION_ROTATION_Z = 30
		self.ANIMATION_SCALE = iScale
	
	def calculateSpecialLayout(self):
		self.SPECIAL_PANEL_X = self.main.PAGE_AREA_X
		self.SPECIAL_PANEL_Y = self.SYNOPSIS_PANEL_Y + self.SYNOPSIS_PANEL_H + self.main.COMMON_MARGIN_H
		self.SPECIAL_PANEL_W = self.main.PAGE_AREA_W
		self.SPECIAL_PANEL_H = self.main.PAGE_AREA_Y + self.main.PAGE_AREA_H - self.SPECIAL_PANEL_Y
	
	def calculateWeightedSpace(self, iSpaceAvailable, specs):
		iTotalFixed = 0
		iTotalWeight = 0
		
		"""
		for (iFixed, iWeight) in specs:
			iTotalFixed += iFixed
			iTotalWeight += iWeight
		
		if (iSpaceAvailable <= iFixed):
			return iSpaceAvailable
		return iSpaceAvailable - 
		"""
	
	def calculateTwoColumnLayout(self, iColumn1FixedWidth = 0, iColumn1ExtraWidthWeight = 50, iColumn2FixedWidth = 0, iColumn2ExtraWidthWeight = 50):
		iAvailableWidth = self.main.PAGE_AREA_W - self.main.COMMON_MARGIN_W # Margin between columns
		iExtraWidth = iAvailableWidth - iColumn1FixedWidth - iColumn2FixedWidth
		iTotalWeight = iColumn1ExtraWidthWeight + iColumn2ExtraWidthWeight
		
		self.COLUMN1_X = self.main.PAGE_AREA_X
		self.COLUMN1_W = iColumn1FixedWidth + int(iExtraWidth * iColumn1ExtraWidthWeight / iTotalWeight)
		
		self.COLUMN2_X = self.COLUMN1_X + self.COLUMN1_W + self.main.COMMON_MARGIN_W
		self.COLUMN2_W = iAvailableWidth - self.COLUMN1_W
	
	def calculateThreeColumnLayout(self, iColumn1FixedWidth = 0, iColumn1ExtraWidthWeight = 50, iColumn2FixedWidth = 0, iColumn2ExtraWidthWeight = 50, iColumn3FixedWidth = 0, iColumn3ExtraWidthWeight = 50):
		iAvailableWidth = self.main.PAGE_AREA_W - self.main.COMMON_MARGIN_W * 2 # Margin between columns
		iExtraWidth = iAvailableWidth - iColumn1FixedWidth - iColumn2FixedWidth - iColumn3FixedWidth
		iTotalWeight = iColumn1ExtraWidthWeight + iColumn2ExtraWidthWeight + iColumn3ExtraWidthWeight
		
		self.COLUMN1_X = self.main.PAGE_AREA_X
		self.COLUMN1_W = iColumn1FixedWidth + int(iExtraWidth * iColumn1ExtraWidthWeight / iTotalWeight)
		
		self.COLUMN2_X = self.COLUMN1_X + self.COLUMN1_W + self.main.COMMON_MARGIN_W
		self.COLUMN2_W = iColumn2FixedWidth + int(iExtraWidth * iColumn2ExtraWidthWeight / iTotalWeight)
		
		self.COLUMN3_X = self.COLUMN2_X + self.COLUMN2_W + self.main.COMMON_MARGIN_W
		self.COLUMN3_W = iColumn3FixedWidth + int(iExtraWidth * iColumn3ExtraWidthWeight / iTotalWeight)
	
	def calculateFourColumnLayout(self, iColumn1FixedWidth = 0, iColumn1ExtraWidthWeight = 50, iColumn2FixedWidth = 0, iColumn2ExtraWidthWeight = 50, iColumn3FixedWidth = 0, iColumn3ExtraWidthWeight = 50, iColumn4FixedWidth = 0, iColumn4ExtraWidthWeight = 50):
		iAvailableWidth = self.main.PAGE_AREA_W - self.main.COMMON_MARGIN_W * 3 # Margin between columns
		iExtraWidth = iAvailableWidth - iColumn1FixedWidth - iColumn2FixedWidth - iColumn3FixedWidth - iColumn4FixedWidth
		iTotalWeight = iColumn1ExtraWidthWeight + iColumn2ExtraWidthWeight + iColumn3ExtraWidthWeight + iColumn4ExtraWidthWeight
		
		self.COLUMN1_X = self.main.PAGE_AREA_X
		self.COLUMN1_W = iColumn1FixedWidth + int(iExtraWidth * iColumn1ExtraWidthWeight / iTotalWeight)
		
		self.COLUMN2_X = self.COLUMN1_X + self.COLUMN1_W + self.main.COMMON_MARGIN_W
		self.COLUMN2_W = iColumn2FixedWidth + int(iExtraWidth * iColumn2ExtraWidthWeight / iTotalWeight)
		
		self.COLUMN3_X = self.COLUMN2_X + self.COLUMN2_W + self.main.COMMON_MARGIN_W
		self.COLUMN3_W = iColumn3FixedWidth + int(iExtraWidth * iColumn3ExtraWidthWeight / iTotalWeight)
		
		self.COLUMN4_X = self.COLUMN3_X + self.COLUMN3_W + self.main.COMMON_MARGIN_W
		self.COLUMN4_W = iColumn4FixedWidth + int(iExtraWidth * iColumn4ExtraWidthWeight / iTotalWeight)
	
	def getData(self):
		return (-1, -1)
	
	def setData(self, iPageData1, iPageData2):
		return
	
	def setMode(self, eMode):
		return
	
	def setCivilization(self, eCivilization):
		return
	
	def updatePlayerContext(self):
		return
	
	def updatePlayableOnlyFilter(self, on):
		return
	
	def getInfo(self):
		return
	
	def getButton(self, info, ePlayer):
		return info.getButton()
	
	def draw(self):
		if (not self.isEmpty()):
			return
		
		screen = self.getScreen()
		info = self.getInfo()
		ePlayer = gc.getGame().getActivePlayer()
		
		self.drawTitle(screen, info, ePlayer)
		self.drawSynopsis(screen, info, ePlayer)
	
	def drawTitle(self, screen, info, ePlayer):
		szTitle = u"<font=4b>" + info.getDescription().upper() + u"</font>"
		
		# Title Panel
		#screen.addPanel(self.record(self.TITLE_PANEL_ID), "", "", False, False, self.TITLE_PANEL_X, self.TITLE_PANEL_Y, self.TITLE_PANEL_W, self.TITLE_PANEL_H, PanelStyles.PANEL_STYLE_MAIN_TAN)
		
		# Title
		screen.setLabel(self.record(self.TITLE_ID), "Background", szTitle, CvUtil.FONT_CENTER_JUSTIFY, self.TITLE_X, self.TITLE_Y, 0, FontTypes.TITLE_FONT, WidgetTypes.WIDGET_GENERAL, -1, -1)
	
	def drawSynopsis(self, screen, info, ePlayer):
		# Info Panel
		screen.addPanel(self.record(self.SYNOPSIS_PANEL_ID), "", "", False, False, self.SYNOPSIS_PANEL_X, self.SYNOPSIS_PANEL_Y, self.SYNOPSIS_PANEL_W, self.SYNOPSIS_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		
		# Icon Panel
		screen.addPanel(self.record(self.ICON_PANEL_ID), "", "", False, False, self.ICON_PANEL_X, self.ICON_PANEL_Y, self.ICON_PANEL_W, self.ICON_PANEL_H, PanelStyles.PANEL_STYLE_MAIN)
		
		# Icon
		linkData = self.getData()
		screen.addDDSGFC(self.record(self.ICON_ID), self.getButton(info, ePlayer), self.ICON_X, self.ICON_Y, self.ICON_SIZE, self.ICON_SIZE, self.eWidget, linkData[0], linkData[1])
		
		# Stats
		self.drawStats(screen, info, ePlayer)
	
	def drawStats(self, screen, info, ePlayer):
		return
	
	def drawEffects(self, screen, szText):
		screen.addPanel(self.record(self.EFFECTS_PANEL_ID), localText.getText("TXT_KEY_PEDIA_EFFECTS", ()), "", True, False, self.EFFECTS_PANEL_X, self.EFFECTS_PANEL_Y, self.EFFECTS_PANEL_W, self.EFFECTS_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		screen.addMultilineText(self.record(self.EFFECTS_TEXT_ID), szText, self.EFFECTS_PANEL_X+5, self.EFFECTS_PANEL_Y+30, self.EFFECTS_PANEL_W-10, self.EFFECTS_PANEL_H-35, WidgetTypes.WIDGET_GENERAL, -1, -1, CvUtil.FONT_LEFT_JUSTIFY)
	
	def drawSpecial(self, screen, szText):
		screen.addPanel(self.record(self.SPECIAL_PANEL_ID), localText.getText("TXT_KEY_PEDIA_SPECIAL_ABILITIES", ()), "", True, False, self.SPECIAL_PANEL_X, self.SPECIAL_PANEL_Y, self.SPECIAL_PANEL_W, self.SPECIAL_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		screen.addMultilineText(self.record(self.SPECIAL_TEXT_ID), szText, self.SPECIAL_PANEL_X+5, self.SPECIAL_PANEL_Y+30, self.SPECIAL_PANEL_W-10, self.SPECIAL_PANEL_H-35, WidgetTypes.WIDGET_GENERAL, -1, -1, CvUtil.FONT_LEFT_JUSTIFY)

	def drawCivilopedia(self, screen, info, szHeadingKey):
		screen.addPanel(self.record(self.CIVILOPEDIA_PANEL_ID), localText.getText(szHeadingKey, ()), "", True, True, self.CIVILOPEDIA_PANEL_X, self.CIVILOPEDIA_PANEL_Y, self.CIVILOPEDIA_PANEL_W, self.CIVILOPEDIA_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		screen.attachLabel(self.CIVILOPEDIA_PANEL_ID, "", "  ")
		screen.addMultilineText(self.record(self.CIVILOPEDIA_TEXT_ID), info.getCivilopedia(), self.CIVILOPEDIA_PANEL_X + 15, self.CIVILOPEDIA_PANEL_Y + 40, self.CIVILOPEDIA_PANEL_W - (15 * 2), self.CIVILOPEDIA_PANEL_H - (15 * 2) - 25, WidgetTypes.WIDGET_GENERAL, -1, -1, CvUtil.FONT_LEFT_JUSTIFY)

