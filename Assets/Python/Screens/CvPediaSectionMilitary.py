## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
from CvPythonExtensions import *

from CvPediaSection import CvPediaSection
from CvPediaView import CvPediaView
from CvPediaPromotion import CvPediaPromotion
from CvPediaPromotionTree import CvPediaPromotionTree
from CvPediaBuilding import CvPediaBuilding
from CvPediaAid import CvPediaAid
from CvPediaDetriment import CvPediaDetriment

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

class CvPediaSectionMilitary(CvPediaSection):
	"Civilopedia Section for Military"

	def __init__(self, main):
		super(CvPediaSectionMilitary, self).__init__(main, CivilopediaSectionTypes.CIVILOPEDIA_SECTION_MILITARY, "TXT_KEY_PEDIA_SECTION_MILITARY", "Military")
		
		self.views = [
			CvPediaView(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_MILITARY_CONVENTIONAL_PROMOTIONS, "TXT_KEY_PEDIA_VIEW_MILITARY_CONVENTIONAL_PROMOTIONS", CivilopediaPageTypes.CIVILOPEDIA_PAGE_PROMOTION, WidgetTypes.WIDGET_PEDIA_JUMP_TO_PROMOTION, lambda: self.getInfos(gc.getNumPromotionInfos(), gc.getPromotionInfo, self.isConventionalPromotion)),
			CvPediaView(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_MILITARY_SPECIAL_PROMOTIONS, "TXT_KEY_PEDIA_VIEW_MILITARY_SPECIAL_PROMOTIONS", CivilopediaPageTypes.CIVILOPEDIA_PAGE_PROMOTION, WidgetTypes.WIDGET_PEDIA_JUMP_TO_PROMOTION, lambda: self.getInfos(gc.getNumPromotionInfos(), gc.getPromotionInfo, self.isSpecialPromotion)),
			CvPediaView(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_MILITARY_PROMOTION_TREE, "TXT_KEY_PEDIA_VIEW_MILITARY_PROMOTION_TREE", CivilopediaPageTypes.CIVILOPEDIA_PAGE_PROMOTION_TREE, WidgetTypes.WIDGET_PEDIA_JUMP_TO_PROMOTION_TREE, lambda: [], [], []),
			CvPediaView(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_MILITARY_DOCTRINES, "TXT_KEY_PEDIA_VIEW_MILITARY_DOCTRINES", CivilopediaPageTypes.CIVILOPEDIA_PAGE_BUILDING, WidgetTypes.WIDGET_PEDIA_JUMP_TO_BUILDING, lambda: self.getInfos(gc.getNumBuildingInfos(), gc.getBuildingInfo, self.isDoctrine)),
			CvPediaView(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_MILITARY_TRADITIONS, "TXT_KEY_PEDIA_VIEW_MILITARY_TRADITIONS", CivilopediaPageTypes.CIVILOPEDIA_PAGE_BUILDING, WidgetTypes.WIDGET_PEDIA_JUMP_TO_BUILDING, lambda: self.getInfos(gc.getNumBuildingInfos(), gc.getBuildingInfo, self.isTradition)),
			CvPediaView(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_MILITARY_AID, "TXT_KEY_PEDIA_VIEW_MILITARY_AID", CivilopediaPageTypes.CIVILOPEDIA_PAGE_AID, WidgetTypes.WIDGET_PEDIA_JUMP_TO_AID, lambda: self.getInfos(gc.getNumAidInfos(), gc.getAidInfo)),
			CvPediaView(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_MILITARY_DETRIMENTS, "TXT_KEY_PEDIA_VIEW_MILITARY_DETRIMENTS", CivilopediaPageTypes.CIVILOPEDIA_PAGE_DETRIMENT, WidgetTypes.WIDGET_PEDIA_JUMP_TO_DETRIMENT, lambda: self.getInfos(gc.getNumDetrimentInfos(), gc.getDetrimentInfo))
		]
		
		self.pages = {
			CivilopediaPageTypes.CIVILOPEDIA_PAGE_PROMOTION : CvPediaPromotion(self),
			CivilopediaPageTypes.CIVILOPEDIA_PAGE_PROMOTION_TREE : CvPediaPromotionTree(self),
			CivilopediaPageTypes.CIVILOPEDIA_PAGE_BUILDING : CvPediaBuilding(self),
			CivilopediaPageTypes.CIVILOPEDIA_PAGE_AID : CvPediaAid(self),
			CivilopediaPageTypes.CIVILOPEDIA_PAGE_DETRIMENT : CvPediaDetriment(self)
		}
		
		self.setDefaults()
	
	def isConventionalPromotion(self, info, iData):
		return not info.isSpecial() and not info.isTemporary() and not info.isSpecialRequired()
	
	def isSpecialPromotion(self, info, iData):
		return info.isSpecial() or info.isTemporary() or info.isSpecialRequired()
	
	def isDoctrine(self, info, iData):
		return info.isMilitaryDoctrine()
	
	def isTradition(self, info, iData):
		return info.isMilitaryTradition()
