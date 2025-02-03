## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
from CvPythonExtensions import *
import CvUtil
import CvPediaPage
#import ScreenInput

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

class CvPediaLeader(CvPediaPage.CvPediaPage):
	"Civilopedia Screen for Leaders"

	def __init__(self, section):
		super(CvPediaLeader, self).__init__(section, CivilopediaPageTypes.CIVILOPEDIA_PAGE_LEADER, WidgetTypes.WIDGET_PEDIA_JUMP_TO_LEADER, "Leader")
		
		self.eLeader = -1
		
		self.LEADER_PANEL_ID = self.szWidget + "LeaderPanel"
		self.LEADER_GRAPHIC_ID = self.szWidget + "LeaderGraphic"
		self.TRAITS_PANEL_ID = self.szWidget + "TraitsPanel"
		self.TRAITS_TEXT_ID = self.szWidget + "TraitsText"
		self.CIVILIZATION_PANEL_ID = self.szWidget + "CivilizationPanel"
		#self.CIVILIZATION_ICON_ID = self.szWidget + "CivilizationIcon"
		self.FAVORITE_CIVIC_PANEL_ID = self.szWidget + "FavoriteCivicPanel"
		#self.FAVORITE_CIVIC_TEXT_ID = self.szWidget + "FavoriteCivicText"
		self.FAVORITE_RELIGION_PANEL_ID = self.szWidget + "FavoriteReligionPanel"
		self.AI_PANEL_ID = self.szWidget + "AIPanel"
		self.AI_TEXT_ID = self.szWidget + "AIText"
	
	def getData(self):
		return (self.eLeader, -1)
	
	def setData(self, eLeader, iPageData2):
		if (self.eLeader != eLeader):
			self.clear()
			self.eLeader = eLeader
	
	def getInfo(self):
		return gc.getLeaderHeadInfo(self.eLeader)
	
	def calculateLayout(self):
		self.calculateTitleLayout()
		self.calculateThreeColumnLayout(287, 0, 40, 50, 140, 50)
		
		iLeaderGraphicPadding = 16
		
		self.LEADERHEAD_PANEL_X = self.COLUMN1_X
		self.LEADERHEAD_PANEL_Y = self.PAGE_CONTENT_Y
		self.LEADERHEAD_PANEL_W = self.COLUMN1_W
		self.LEADERHEAD_PANEL_H = 360
		
		self.LEADERHEAD_W = self.LEADERHEAD_PANEL_W - iLeaderGraphicPadding * 2
		self.LEADERHEAD_H = self.LEADERHEAD_PANEL_H - (iLeaderGraphicPadding * 2 + 4) # Fudge factor
		self.LEADERHEAD_X = self.LEADERHEAD_PANEL_X + (self.LEADERHEAD_PANEL_W - self.LEADERHEAD_W) / 2
		self.LEADERHEAD_Y = self.LEADERHEAD_PANEL_Y + (self.LEADERHEAD_PANEL_H - self.LEADERHEAD_H) / 2 + 2
		
		self.CIVILIZATION_PANEL_X = self.COLUMN2_X
		self.CIVILIZATION_PANEL_Y = self.PAGE_CONTENT_Y
		self.CIVILIZATION_PANEL_W = self.COLUMN2_W
		self.CIVILIZATION_PANEL_H = int((self.LEADERHEAD_PANEL_H - self.main.COMMON_MARGIN_H * 2) / 3)
		
		self.FAVORITE_CIVIC_PANEL_X = self.COLUMN2_X
		self.FAVORITE_CIVIC_PANEL_Y = self.CIVILIZATION_PANEL_Y + self.CIVILIZATION_PANEL_H + self.main.COMMON_MARGIN_H
		self.FAVORITE_CIVIC_PANEL_W = self.COLUMN2_W
		self.FAVORITE_CIVIC_PANEL_H = self.CIVILIZATION_PANEL_H
		
		self.FAVORITE_RELIGION_PANEL_X = self.COLUMN2_X
		self.FAVORITE_RELIGION_PANEL_Y = self.FAVORITE_CIVIC_PANEL_Y + self.FAVORITE_CIVIC_PANEL_H + self.main.COMMON_MARGIN_H
		self.FAVORITE_RELIGION_PANEL_W = self.COLUMN2_W
		self.FAVORITE_RELIGION_PANEL_H = self.CIVILIZATION_PANEL_H
		
		self.TRAITS_PANEL_X = self.COLUMN3_X
		self.TRAITS_PANEL_Y = self.PAGE_CONTENT_Y
		self.TRAITS_PANEL_W = self.COLUMN3_W
		self.TRAITS_PANEL_H = 360
		
		self.CIVILOPEDIA_PANEL_X = self.COLUMN1_X
		self.CIVILOPEDIA_PANEL_Y = self.LEADERHEAD_PANEL_Y + self.LEADERHEAD_PANEL_H + self.main.COMMON_MARGIN_H
		self.CIVILOPEDIA_PANEL_W = self.COLUMN1_W + self.main.COMMON_MARGIN_W + self.COLUMN2_W
		self.CIVILOPEDIA_PANEL_H = self.main.PAGE_AREA_Y + self.main.PAGE_AREA_H - self.CIVILOPEDIA_PANEL_Y
		
		self.AI_PANEL_X = self.COLUMN3_X
		self.AI_PANEL_Y = self.LEADERHEAD_PANEL_Y + self.LEADERHEAD_PANEL_H + self.main.COMMON_MARGIN_H
		self.AI_PANEL_W = self.COLUMN3_W
		self.AI_PANEL_H = self.main.PAGE_AREA_Y + self.main.PAGE_AREA_H - self.CIVILOPEDIA_PANEL_Y

	def draw(self):
		if (not self.isEmpty()):
			return
		
		screen = self.getScreen()
		info = self.getInfo()
		ePlayer = gc.getGame().getActivePlayer()
		
		self.drawTitle(screen, info, ePlayer)
		self.drawLeader(screen)
		self.drawTraits(screen)
		self.drawCivilization(screen)
		self.drawFavoriteCivic(screen, info)
		self.drawFavoriteReligion(screen, info)
		self.drawAI(screen, info)
		self.drawCivilopedia(screen, info, "TXT_KEY_PEDIA_BACKGROUND")
	
	def drawLeader(self, screen):
		screen.addPanel(self.record(self.LEADER_PANEL_ID), "", "", True, True, self.LEADERHEAD_PANEL_X, self.LEADERHEAD_PANEL_Y, self.LEADERHEAD_PANEL_W, self.LEADERHEAD_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		screen.addLeaderheadGFC(self.record(self.LEADER_GRAPHIC_ID), self.eLeader, AttitudeTypes.ATTITUDE_PLEASED, self.LEADERHEAD_X, self.LEADERHEAD_Y, self.LEADERHEAD_W, self.LEADERHEAD_H, WidgetTypes.WIDGET_GENERAL, -1, -1)
	
	def drawCivilization(self, screen):
		screen.addPanel(self.record(self.CIVILIZATION_PANEL_ID), localText.getText("TXT_KEY_PEDIA_CATEGORY_CIV", ()), "", False, True, self.CIVILIZATION_PANEL_X, self.CIVILIZATION_PANEL_Y, self.CIVILIZATION_PANEL_W, self.CIVILIZATION_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		screen.attachLabel(self.CIVILIZATION_PANEL_ID, "", "  ")
		
		for eCiv in range(gc.getNumCivilizationInfos()):
			pCiv = gc.getCivilizationInfo(eCiv)
			if pCiv.isLeaders(self.eLeader):
				screen.attachImageButton(self.CIVILIZATION_PANEL_ID, "", pCiv.getButton(), GenericButtonSizes.BUTTON_SIZE_CUSTOM, WidgetTypes.WIDGET_PEDIA_JUMP_TO_CIV, eCiv, -1, False)
				#screen.setImageButton(self.record(self.CIVILIZATION_ICON_ID), pCiv.getButton(), self.CIVILIZATION_ICON_X, self.CIVILIZATION_ICON_Y, self.CIVILIZATION_ICON_W, self.CIVILIZATION_ICON_H, WidgetTypes.WIDGET_PEDIA_JUMP_TO_CIV, eCiv, 1)
	
	def drawFavoriteCivic(self, screen, info):
		screen.addPanel(self.record(self.FAVORITE_CIVIC_PANEL_ID), localText.getText("TXT_KEY_PEDIA_FAV_CIVIC", ()), "", False, True, self.FAVORITE_CIVIC_PANEL_X, self.FAVORITE_CIVIC_PANEL_Y, self.FAVORITE_CIVIC_PANEL_W, self.FAVORITE_CIVIC_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		screen.attachLabel(self.FAVORITE_CIVIC_PANEL_ID, "", "  ")
		
		eCivic = info.getFavoriteCivic()
		if (eCivic != -1):
			screen.attachImageButton(self.FAVORITE_CIVIC_PANEL_ID, "", gc.getCivicInfo(eCivic).getButton(), GenericButtonSizes.BUTTON_SIZE_CUSTOM, WidgetTypes.WIDGET_PEDIA_JUMP_TO_CIVIC, eCivic, -1, False)
			#szCivicText = u"<link=literal>" + gc.getCivicInfo(eCivic).getDescription() + u"</link>"
			#screen.addMultilineText(self.record(self.FAVORITE_CIVIC_TEXT_ID), szCivicText, self.FAVORITE_CIVIC_PANEL_X+5, self.FAVORITE_CIVIC_PANEL_Y+30, self.FAVORITE_CIVIC_PANEL_W-10, self.FAVORITE_CIVIC_PANEL_H-10, WidgetTypes.WIDGET_GENERAL, -1, -1, CvUtil.FONT_LEFT_JUSTIFY)
	
	def drawFavoriteReligion(self, screen, info):
		screen.addPanel(self.record(self.FAVORITE_RELIGION_PANEL_ID), localText.getText("TXT_KEY_PEDIA_FAV_RELIGION", ()), "", False, True, self.FAVORITE_RELIGION_PANEL_X, self.FAVORITE_RELIGION_PANEL_Y, self.FAVORITE_RELIGION_PANEL_W, self.FAVORITE_RELIGION_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		screen.attachLabel(self.FAVORITE_RELIGION_PANEL_ID, "", "  ")
		
		eReligion = info.getFavoriteReligion()
		if (eReligion != -1):
			screen.attachImageButton(self.FAVORITE_RELIGION_PANEL_ID, "", gc.getReligionInfo(eReligion).getButton(), GenericButtonSizes.BUTTON_SIZE_CUSTOM, WidgetTypes.WIDGET_PEDIA_JUMP_TO_RELIGION, eReligion, -1, False)
	
	def drawTraits(self, screen):
		screen.addPanel(self.record(self.TRAITS_PANEL_ID), localText.getText("TXT_KEY_PEDIA_TRAITS", ()), "", True, False, self.TRAITS_PANEL_X, self.TRAITS_PANEL_Y, self.TRAITS_PANEL_W, self.TRAITS_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		szSpecialText = CyGameTextMgr().parseLeaderTraits(self.eLeader, -1, False, True)[1:]
		screen.addMultilineText(self.record(self.TRAITS_TEXT_ID), szSpecialText, self.TRAITS_PANEL_X+5, self.TRAITS_PANEL_Y+30, self.TRAITS_PANEL_W-10, self.TRAITS_PANEL_H-35, WidgetTypes.WIDGET_GENERAL, -1, -1, CvUtil.FONT_LEFT_JUSTIFY)

	def drawAI(self, screen, info):
		screen.addPanel(self.record(self.AI_PANEL_ID), localText.getText("TXT_KEY_PEDIA_AI", ()), "", True, False, self.AI_PANEL_X, self.AI_PANEL_Y, self.AI_PANEL_W, self.AI_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		szSpecialText = str("Leader AI personality has the following distinctive qualities:")

		szSpecialText += u"<font=3>" + "\n" + "\n" + "Economy" + "</font>"
		
		if info.getWonderConstructRand() == 0:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Never builds wonders" + "</font>"
		elif info.getWonderConstructRand() < 20:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Rarely tries to build wonders" + "</font>"
		elif info.getWonderConstructRand() > 40:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Always tries to build wonders" + "</font>"	
		elif info.getWonderConstructRand() > 30:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Very often tries to build wonders" + "</font>"		
		elif info.getWonderConstructRand() > 20:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Often tries to build wonders" + "</font>"
		else:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Average probability to build wonders" + "</font>"
			
		if info.getBuildUnitProb() < 8:		
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Tends to build less units" + "</font>"
		elif info.getBuildUnitProb() > 18:		
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Builds large amounts of units" + "</font>"
		elif info.getBuildUnitProb() > 13:		
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Tends to build more units" + "</font>"
					
		if info.getMaxGoldTradePercent() < 6:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Unwilling to trade away large lump sums of gold" + "</font>"	
		elif info.getMaxGoldTradePercent() > 11:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Willing to trade away large lump sums of gold" + "</font>"	
			
		if info.getMaxGoldPerTurnTradePercent() < 6:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Unwilling to trade away large amounts of gold per turn" + "</font>"
		elif info.getMaxGoldPerTurnTradePercent() < 6:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Willing to trade away large amounts of gold per turn" + "</font>"	
			
		for iImprovement in xrange(gc.getNumImprovementInfos()):
			if info.getImprovementWeightModifier(iImprovement) > 0:
				szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Favors improvement: " + gc.getImprovementInfo(iImprovement).getDescription() + "</font>"	

		szSpecialText += u"<font=3>" + "\n" + "\n" + "Diplomacy" + "</font>"			
		
		if info.getBaseAttitude() == -1:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Tends to initially dislike other leaders" + "</font>"	
		elif info.getBaseAttitude() == 1:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Tends to initially like other leaders" + "</font>"	
		elif info.getBaseAttitude() > 1:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Gets a large initial relations boost with other leaders" + "</font>"	
		else: 
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Neutral initial attitude to other leaders" + "</font>"
			
		if info.getBasePeaceWeight() < 3:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Strongly favors warmongers" + "</font>"
		elif info.getBasePeaceWeight() > 8:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Strongly favors pacifists" + "</font>"	
		elif info.getBasePeaceWeight() < 5:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Tends to favor more aggressive leaders" + "</font>"
		elif info.getBasePeaceWeight() > 6:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Tends to favor more peaceful leaders" + "</font>"

		if info.getPeaceWeightRand() > 5:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Very inconsistent in picking favorites" + "</font>"
		elif info.getPeaceWeightRand() > 3:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Rather inconsistent in picking favorites" + "</font>"
			
		if info.getRefuseToTalkWarThreshold() < 7:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Willing to discuss peace soon after war began" + "</font>"	
		elif info.getRefuseToTalkWarThreshold() > 10:			
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Unwilling to discuss peace for a long while after war began" + "</font>"	
			
		if info.getMakePeaceRand() < 21:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Very interested in getting a peace deal in an ongoing war" + "</font>"	
		elif info.getMakePeaceRand() < 31:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Interested in getting a peace deal in an ongoing war" + "</font>"	
		elif info.getMakePeaceRand() > 79:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Much more interested in the war itself than getting a peace deal" + "</font>"
		elif info.getMakePeaceRand() > 59:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "More interested in the war itself than getting a peace deal" + "</font>"	

		if info.getWorseRankDifferenceAttitudeChange() < -3:	
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Hates weaker civilizations" + "</font>"			
		elif info.getWorseRankDifferenceAttitudeChange() < 0:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Dislikes weaker civilizations" + "</font>"
		elif info.getWorseRankDifferenceAttitudeChange() == 0:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Indifferent to weaker civilizations" + "</font>"
		elif info.getWorseRankDifferenceAttitudeChange() > 0:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Likes weaker civilizations" + "</font>"
			
		if info.getBetterRankDifferenceAttitudeChange() < -2:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Very jealous stronger civilizations" + "</font>"
		elif info.getBetterRankDifferenceAttitudeChange() < -1:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Jealous of stronger civilizations" + "</font>"
		elif info.getBetterRankDifferenceAttitudeChange() < 1:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Somewhat jealous of stronger civilizations" + "</font>"
		else:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Indifferent to stronger civilizations" + "</font>"
			
		if info.getCloseBordersAttitudeChange() < -4:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "As a tribal leader, extremely intolerant to border pressure" + "</font>"
		elif info.getCloseBordersAttitudeChange() < -3:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Very intolerant to border pressure" + "</font>"
		elif info.getCloseBordersAttitudeChange() < -2:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Intolerant to border pressure" + "</font>"
		elif info.getCloseBordersAttitudeChange() < -1:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Tolerant to border pressure" + "</font>"
		elif info.getCloseBordersAttitudeChange() > -2:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Very tolerant to border pressure" + "</font>"
			
		if info.getSameReligionAttitudeChangeLimit() < 3:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Cares little for leaders following the same state religion" + "</font>"
		elif info.getSameReligionAttitudeChangeLimit() > 4:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Cares a lot for leaders following the same state religion" + "</font>"
			
		if info.getDifferentReligionAttitudeChange() < -1:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Hates leaders following a different state religion" + "</font>"
		elif info.getDifferentReligionAttitudeChange() < 0:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Dislikes leaders following a different state religion" + "</font>"
		elif info.getDifferentReligionAttitudeChange() < 1:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Does not care about leaders following a different state religion" + "</font>"
			
		if info.getDifferentIdeologyAttitudeChangeLimit() == 1:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Hates leaders following a different ideology" + "</font>"
		elif info.getDifferentIdeologyAttitudeChangeLimit() == 3:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Dislikes leaders following a different ideology" + "</font>"
		elif info.getDifferentIdeologyAttitudeChangeLimit() == 5:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Pragmatic towards leaders following a different ideology" + "</font>"			
			
		if info.getFavoriteCivicAttitudeChangeLimit() < 2:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Cares very little for other leaders running favorite civic" + "</font>"
		elif info.getFavoriteCivicAttitudeChangeLimit() < 3:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Cares little for other leaders running favorite civic" + "</font>"
		elif info.getFavoriteCivicAttitudeChangeLimit() > 6:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Loves other leaders running favorite civic" + "</font>"
		elif info.getFavoriteCivicAttitudeChangeLimit() > 5:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Likes other leaders running favorite civic" + "</font>"
			
		if info.getOpenBordersRefuseAttitudeThreshold() < 1:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Loves Open Borders" + "</font>"
		elif info.getOpenBordersRefuseAttitudeThreshold() < 2:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Likes Open Borders" + "</font>"
		elif info.getOpenBordersRefuseAttitudeThreshold() < 4:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Dislikes Open Borders" + "</font>"
		elif info.getOpenBordersRefuseAttitudeThreshold() < 5:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Never signs Open Borders" + "</font>"
			
		if info.getStrategicBonusRefuseAttitudeThreshold() < 2:	
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Easily trades away Strategic Resources" + "</font>"
		elif info.getStrategicBonusRefuseAttitudeThreshold() < 4:	
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Reluctant to trade away Strategic Resources" + "</font>"
		elif info.getStrategicBonusRefuseAttitudeThreshold() < 5:	
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Never trades away Strategic Resources" + "</font>"
			
		if info.getHappinessBonusRefuseAttitudeThreshold() < 1:	
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Easily trades away Luxury Resources" + "</font>"
		elif info.getHappinessBonusRefuseAttitudeThreshold() < 2:	
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Reluctant to trade away Luxury Resources" + "</font>"
		elif info.getHappinessBonusRefuseAttitudeThreshold() < 5:	
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Never trades away Luxury Resources" + "</font>"
			
		if info.getMapRefuseAttitudeThreshold() < 2:	
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Easily trades away maps" + "</font>"
		elif info.getMapRefuseAttitudeThreshold() < 4:	
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Reluctant to trade away maps" + "</font>"
		elif info.getMapRefuseAttitudeThreshold() < 5:	
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Never trades away maps" + "</font>"
			
		szSpecialText += u"<font=3>" + "\n" + "\n" + "Warfare" + "</font>"		
		
		if info.getMaxWarRand() < 26:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Extremely likely to declare wars" + "</font>"
		elif info.getMaxWarRand() > 399:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Extremely unlikely to declare wars" + "</font>"
		elif info.getMaxWarRand() < 51:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Very likely to declare wars" + "</font>"
		elif info.getMaxWarRand() > 299:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Very unlikely to declare wars" + "</font>"
		elif info.getMaxWarRand() < 101:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Likely to declare wars" + "</font>"
		elif info.getMaxWarRand() > 199:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Unlikely to declare wars" + "</font>"
		else:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Average probability to declare wars" + "</font>"
			
		if info.getMaxWarNearbyPowerRatio() < 55:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Will only attack much weaker neighbors" + "</font>"
		elif info.getMaxWarNearbyPowerRatio() < 70:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Will only attack weaker neighbors" + "</font>"		
		elif info.getMaxWarNearbyPowerRatio() > 95:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Can attack strong neighbors" + "</font>"		
		elif info.getMaxWarNearbyPowerRatio() > 75:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Can attack relatively strong neighbors" + "</font>"	

		if info.getMaxWarDistantPowerRatio() < 25:	
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Unwilling to wage distant wars" + "</font>"
		elif info.getMaxWarDistantPowerRatio() < 45:	
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Willing to wage distant wars only against very weak targets" + "</font>"
		elif info.getMaxWarDistantPowerRatio() > 75:	
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Willing to wage distant wars" + "</font>"
			
		if info.getMaxWarMinAdjacentLandPercent() < 2:	
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Wars usually ideologically motivated" + "</font>"
		elif info.getMaxWarMinAdjacentLandPercent() > 3:	
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Wars usually motivated by greed" + "</font>"
			
		if info.getDogpileWarRand() < 20:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Very likely to join ongoing wars" + "</font>"
		elif info.getDogpileWarRand() < 30:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Likely to join ongoing wars" + "</font>"
		elif info.getDogpileWarRand() > 100:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Very unlikely to join ongoing wars" + "</font>"
		elif info.getDogpileWarRand() > 90:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Unlikely to join ongoing wars" + "</font>"
			
		if info.getDemandRebukedWarProb() < 10:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Will not instantly declare a war over refused tribute" + "</font>"	
		elif info.getDemandRebukedWarProb() < 26:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Unlikely to instantly declare a war over refused tribute" + "</font>"
		elif info.getDemandRebukedWarProb() > 49:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Likely to instantly declare a war over refused tribute" + "</font>"	
		
		if info.getDemandRebukedSneakProb() < 10:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Will not start preparing for a war over refused tribute" + "</font>"
		elif info.getDemandRebukedSneakProb() < 26:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Unlikely to start preparing for a war over refused tribute" + "</font>"
		elif info.getDemandRebukedSneakProb() > 51:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Likely to start preparing for a war over refused tribute" + "</font>"	
		elif info.getDemandRebukedSneakProb() > 79:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Almost certain to start preparing for a war over refused tribute" + "</font>"			
			
		if info.getRazeCityProb() < -99:	
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Unwilling to raze cities unless absolutely needed" + "</font>"
		elif info.getRazeCityProb() < -49:	
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Dislikes razing cities" + "</font>"
		elif info.getRazeCityProb() > -26:	
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Likes razing cities" + "</font>"
		elif info.getRazeCityProb() > -1:	
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Loves razing cities" + "</font>"
			
		if info.getBaseAttackOddsChange() > 0:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Tends to be overconfident when attacking" + "</font>"
			
		if info.getAttackOddsChangeRand() > 12:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Bad at estimating combat victory chances" + "</font>"
		elif info.getAttackOddsChangeRand() > 8:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Can sometimes poorly estimate combat victory chances" + "</font>"
			
		if info.getNoWarAttitudeProb(4) < 100:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Can declare war on Friendly relations" + "</font>"
		else:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Normally never declares war on Friendly relations" + "</font>"
		
		if info.getNoWarAttitudeProb(3) == 100:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Normally never declares war on Pleased relations" + "</font>"
			
		if info.getUnitAIWeightModifier(4) > 0:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Likes offensive units" + "</font>"
			
		if info.getUnitAIWeightModifier(5) > 0:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Likes city attack units" + "</font>"
			
		if info.getUnitAIWeightModifier(6) > 0:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Likes artillery" + "</font>"
			
		if info.getUnitAIWeightModifier(7) > 0:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Likes pillaging" + "</font>"
			
		if info.getUnitAIWeightModifier(9) > 0:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Likes diverse stacks" + "</font>"
			
		if info.getUnitAIWeightModifier(10) > 0:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Likes defensive units" + "</font>"
			
		if info.getUnitAIWeightModifier(11) > 0:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Likes defensive units" + "</font>"
			
		if info.getUnitAIWeightModifier(13) > 0:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Likes exploring" + "</font>"
			
		if info.getUnitAIWeightModifier(24) > 0:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Likes naval supremacy" + "</font>"
			
		if info.getUnitAIWeightModifier(28) > 0:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Likes naval invasions" + "</font>"
		
		szSpecialText += u"<font=3>" + "\n" + "\n" + "Other" + "</font>"	

		if info.getEspionageWeight() < 60:	
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Neglects espionage" + "</font>"
		elif info.getEspionageWeight() > 130:	
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Loves espionage" + "</font>"
		elif info.getEspionageWeight() < 80:	
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Somewhat neglects espionage" + "</font>"	
		elif info.getEspionageWeight() > 110:	
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Somewhat likes espionage" + "</font>"			
		
		if info.getFlavorValue(0) > 5:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Overwhelmingly favors military-oriented civics, techs and buildings" + "</font>"
		elif info.getFlavorValue(0) > 3:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Strongly favors military-oriented civics, techs and buildings" + "</font>"
		elif info.getFlavorValue(0) > 1:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Favors military-oriented civics, techs and buildings" + "</font>"

		if info.getFlavorValue(1) > 5:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Overwhelmingly favors religion-oriented civics, techs and buildings" + "</font>"			
		elif info.getFlavorValue(1) > 3:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Strongly favors religion-oriented civics, techs and buildings" + "</font>"
		elif info.getFlavorValue(1) > 1:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Favors religion-oriented civics, techs and buildings" + "</font>"
			
		if info.getFlavorValue(2) > 5:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Overwhelmingly favors production-oriented civics, techs and buildings" + "</font>"
		elif info.getFlavorValue(2) > 3:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Strongly favors production-oriented civics, techs and buildings" + "</font>"
		elif info.getFlavorValue(2) > 1:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Favors production-oriented civics, techs and buildings" + "</font>"
			
		if info.getFlavorValue(3) > 5:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Overwhelmingly favors economy-oriented civics, techs and buildings" + "</font>"
		elif info.getFlavorValue(3) > 3:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Strongly favors economy-oriented civics, techs and buildings" + "</font>"
		elif info.getFlavorValue(3) > 1:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Favors economy-oriented civics, techs and buildings" + "</font>"
			
		if info.getFlavorValue(4) > 5:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Overwhelmingly favors research-oriented civics, techs and buildings" + "</font>"
		elif info.getFlavorValue(4) > 3:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Strongly favors research-oriented civics, techs and buildings" + "</font>"
		elif info.getFlavorValue(4) > 1:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Favors research-oriented civics, techs and buildings" + "</font>"
			
		if info.getFlavorValue(5) > 5:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Overwhelmingly favors culture-oriented civics, techs and buildings" + "</font>"
		elif info.getFlavorValue(5) > 3:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Strongly favors culture-oriented civics, techs and buildings" + "</font>"
		elif info.getFlavorValue(5) > 1:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Favors culture-oriented civics, techs and buildings" + "</font>"
			
		if info.getFlavorValue(6) > 5:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Overwhelmingly favors growth-oriented civics, techs and buildings" + "</font>"
		elif info.getFlavorValue(6) > 3:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Strongly favors growth-oriented civics, techs and buildings" + "</font>"
		elif info.getFlavorValue(6) > 1:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Favors growth-oriented civics, techs and buildings" + "</font>"
			
		if info.getFlavorValue(7) > 5:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Overwhelmingly favors espionage-oriented civics, techs and buildings" + "</font>"
		elif info.getFlavorValue(7) > 3:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Strongly favors espionage-oriented civics, techs and buildings" + "</font>"
		elif info.getFlavorValue(7) > 1:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Favors espionage-oriented civics, techs and buildings" + "</font>"
			
		if info.getFlavorValue(10) == 5:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Devout communist" + "</font>"
		elif info.getFlavorValue(10) == 2:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Left-leaning economically" + "</font>"
		elif info.getFlavorValue(10) == -2:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Right-leaning economically" + "</font>"	
		elif info.getFlavorValue(10) == -5:
			szSpecialText += u"<font=2>" + "\n" + CyTranslator().getText("[ICON_BULLET]", ()) + "Staunch anti-communist" + "</font>"						
 
		screen.addMultilineText(self.record(self.AI_TEXT_ID), szSpecialText, self.AI_PANEL_X+5, self.AI_PANEL_Y+30, self.AI_PANEL_W-10, self.AI_PANEL_H-35, WidgetTypes.WIDGET_GENERAL, -1, -1, CvUtil.FONT_LEFT_JUSTIFY)
		
	"""
	def drawHistory(self):
		screen = self.top.getScreen()
		
		panelName = self.top.getNextWidgetName()
		screen.addPanel( panelName, "", "", true, true,
			self.HISTORY_X, self.HISTORY_Y, self.HISTORY_W, self.HISTORY_H, PanelStyles.PANEL_STYLE_BLUE50 )
		
		historyTextName = self.top.getNextWidgetName()
		CivilopediaText = gc.getLeaderHeadInfo(self.eLeader).getCivilopedia()
		CivilopediaText = u"<font=2>" + CivilopediaText + u"</font>"
		screen.attachMultilineText( panelName, historyTextName, CivilopediaText,
			WidgetTypes.WIDGET_GENERAL,-1,-1, CvUtil.FONT_LEFT_JUSTIFY )
	"""

	"""
	def placeLinks(self, bRedraw):
		screen = self.top.getScreen()
		
		if bRedraw:
			screen.clearListBoxGFC(self.top.LIST_ID)

		# sort leaders alphabetically
		rowListName=[(0,0)]*gc.getNumLeaderHeadInfos()
		for j in range(gc.getNumLeaderHeadInfos()):
			rowListName[j] = (gc.getLeaderHeadInfo(j).getDescription(), j)
		rowListName.sort()	
		
		i = 0
		iSelected = 0
		for iI in range(gc.getNumLeaderHeadInfos()):
			if (rowListName[iI][1] != gc.getDefineINT("BARBARIAN_LEADER") and not gc.getLeaderHeadInfo(rowListName[iI][1]).isGraphicalOnly()):
				if (not gc.getDefineINT("CIVILOPEDIA_SHOW_ACTIVE_CIVS_ONLY") or not gc.getGame().isFinalInitialized() or gc.getGame().isLeaderEverActive(rowListName[iI][1])):
					if bRedraw:
						screen.appendListBoxStringNoUpdate(self.top.LIST_ID, rowListName[iI][0], WidgetTypes.WIDGET_PEDIA_JUMP_TO_LEADER, rowListName[iI][1], -1, CvUtil.FONT_LEFT_JUSTIFY)
					if rowListName[iI][1] == self.eLeader:
						iSelected = i
					i += 1
					
		if bRedraw:
			screen.updateListBox(self.top.LIST_ID)
		
		screen.setSelectedListBoxStringGFC(self.top.LIST_ID, iSelected)
	"""

	# Will handle the input for this screen...
	def handleInput (self, input):
		if (input.getNotifyCode() == NotifyCode.NOTIFY_CHARACTER):
			if (input.getData() == int(InputTypes.KB_0)):
				self.getScreen().performLeaderheadAction(self.LEADER_GRAPHIC_ID, LeaderheadAction.LEADERANIM_GREETING)
			elif (input.getData() == int(InputTypes.KB_6)):
				self.getScreen().performLeaderheadAction(self.LEADER_GRAPHIC_ID, LeaderheadAction.LEADERANIM_DISAGREE)
			elif (input.getData() == int(InputTypes.KB_7)):
				self.getScreen().performLeaderheadAction(self.LEADER_GRAPHIC_ID, LeaderheadAction.LEADERANIM_AGREE)
			elif (input.getData() == int(InputTypes.KB_1)):
				self.getScreen().setLeaderheadMood(self.LEADER_GRAPHIC_ID, AttitudeTypes.ATTITUDE_FRIENDLY)
				self.getScreen().performLeaderheadAction(self.LEADER_GRAPHIC_ID, LeaderheadAction.NO_LEADERANIM)
			elif (input.getData() == int(InputTypes.KB_2)):
				self.getScreen().setLeaderheadMood(self.LEADER_GRAPHIC_ID, AttitudeTypes.ATTITUDE_PLEASED)
				self.getScreen().performLeaderheadAction(self.LEADER_GRAPHIC_ID, LeaderheadAction.NO_LEADERANIM)
			elif (input.getData() == int(InputTypes.KB_3)):
				self.getScreen().setLeaderheadMood(self.LEADER_GRAPHIC_ID, AttitudeTypes.ATTITUDE_CAUTIOUS)
				self.getScreen().performLeaderheadAction(self.LEADER_GRAPHIC_ID, LeaderheadAction.NO_LEADERANIM)
			elif (input.getData() == int(InputTypes.KB_4)):
				self.getScreen().setLeaderheadMood(self.LEADER_GRAPHIC_ID, AttitudeTypes.ATTITUDE_ANNOYED)
				self.getScreen().performLeaderheadAction(self.LEADER_GRAPHIC_ID, LeaderheadAction.NO_LEADERANIM)
			elif (input.getData() == int(InputTypes.KB_5)):
				self.getScreen().setLeaderheadMood(self.LEADER_GRAPHIC_ID, AttitudeTypes.ATTITUDE_FURIOUS)
				self.getScreen().performLeaderheadAction(self.LEADER_GRAPHIC_ID, LeaderheadAction.NO_LEADERANIM)
			else:
				self.getScreen().leaderheadKeyInput(self.LEADER_GRAPHIC_ID, input.getData())
		return 0


