## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
from CvPythonExtensions import *

from CvPediaSection import CvPediaSection
from CvPediaView import CvPediaView
from CvPediaViewCivilizations import CvPediaViewCivilizations
from CvPediaViewLeaders import CvPediaViewLeaders
from CvPediaCivilization import CvPediaCivilization
from CvPediaLeader import CvPediaLeader
from CvPediaTrait import CvPediaTrait
from CvPediaCivic import CvPediaCivic
from CvPediaReligion import CvPediaReligion
from CvPediaSpecialist import CvPediaSpecialist
from CvPediaEra import CvPediaEra

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

class CvPediaSectionSociety(CvPediaSection):
	"Civilopedia Section for Society"

	def __init__(self, main):
		super(CvPediaSectionSociety, self).__init__(main, CivilopediaSectionTypes.CIVILOPEDIA_SECTION_SOCIETY, "TXT_KEY_PEDIA_SECTION_SOCIETY", "Society")
		
		self.views = [
			CvPediaViewCivilizations(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_SOCIETY_CIVILIZATIONS, "TXT_KEY_PEDIA_VIEW_SOCIETY_CIVILIZATIONS"),
			CvPediaViewLeaders(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_SOCIETY_LEADERS, "TXT_KEY_PEDIA_VIEW_SOCIETY_LEADERS"),
			CvPediaView(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_SOCIETY_TRAITS, "TXT_KEY_PEDIA_VIEW_SOCIETY_TRAITS", CivilopediaPageTypes.CIVILOPEDIA_PAGE_TRAIT, WidgetTypes.WIDGET_PEDIA_JUMP_TO_TRAIT, lambda: self.getInfos(gc.getNumTraitInfos(), gc.getTraitInfo)),
			CvPediaView(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_SOCIETY_CIVICS, "TXT_KEY_PEDIA_VIEW_SOCIETY_CIVICS", CivilopediaPageTypes.CIVILOPEDIA_PAGE_CIVIC, WidgetTypes.WIDGET_PEDIA_JUMP_TO_CIVIC, lambda: self.getInfos(gc.getNumCivicInfos(), gc.getCivicInfo)),
			CvPediaView(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_SOCIETY_RELIGIONS, "TXT_KEY_PEDIA_VIEW_SOCIETY_RELIGIONS", CivilopediaPageTypes.CIVILOPEDIA_PAGE_RELIGION, WidgetTypes.WIDGET_PEDIA_JUMP_TO_RELIGION, lambda: self.getInfos(gc.getNumReligionInfos(), gc.getReligionInfo), [], []),
			CvPediaView(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_SOCIETY_SPECIALISTS, "TXT_KEY_PEDIA_VIEW_SOCIETY_SPECIALISTS", CivilopediaPageTypes.CIVILOPEDIA_PAGE_SPECIALIST, WidgetTypes.WIDGET_PEDIA_JUMP_TO_SPECIALIST, lambda: self.getInfos(gc.getNumSpecialistInfos(), gc.getSpecialistInfo), [], []),
			CvPediaView(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_SOCIETY_ERAS, "TXT_KEY_PEDIA_VIEW_SOCIETY_ERAS", CivilopediaPageTypes.CIVILOPEDIA_PAGE_ERA, WidgetTypes.WIDGET_PEDIA_JUMP_TO_ERA, lambda: self.getInfos(gc.getNumEraInfos(), gc.getEraInfo), [], [])
		]
		
		self.pages = {
			CivilopediaPageTypes.CIVILOPEDIA_PAGE_CIVILIZATION : CvPediaCivilization(self),
			CivilopediaPageTypes.CIVILOPEDIA_PAGE_LEADER : CvPediaLeader(self),
			CivilopediaPageTypes.CIVILOPEDIA_PAGE_TRAIT : CvPediaTrait(self),
			CivilopediaPageTypes.CIVILOPEDIA_PAGE_CIVIC : CvPediaCivic(self),
			CivilopediaPageTypes.CIVILOPEDIA_PAGE_RELIGION : CvPediaReligion(self),
			CivilopediaPageTypes.CIVILOPEDIA_PAGE_SPECIALIST : CvPediaSpecialist(self),
			CivilopediaPageTypes.CIVILOPEDIA_PAGE_ERA : CvPediaEra(self)
		}
		
		self.setDefaults()