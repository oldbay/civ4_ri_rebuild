## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
from CvPythonExtensions import *

from CvPediaSection import CvPediaSection
from CvPediaView import CvPediaView
#from CvPediaIntroduction import CvPediaIntroduction
from CvPediaConcept import CvPediaConcept
from CvPediaHints import CvPediaHints
from CvPediaHotKeys import CvPediaHotKeys

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

class CvPediaSectionHelp(CvPediaSection):
	"Civilopedia Section for Help"

	def __init__(self, main):
		super(CvPediaSectionHelp, self).__init__(main, CivilopediaSectionTypes.CIVILOPEDIA_SECTION_HELP, "TXT_KEY_PEDIA_SECTION_HELP", "Help")
		
		self.views = [
			#CvPediaView(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_HELP_INTRODUCTION, "All", CivilopediaPageTypes.CIVILOPEDIA_PAGE_INTRODUCTION, WidgetTypes.WIDGET_PEDIA_JUMP_TO_INTRODUCTION, self.getInfos(gc.getNumUnitInfos(), gc.getUnitInfo)),
			CvPediaView(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_HELP_CONCEPTS, "TXT_KEY_PEDIA_VIEW_CONCEPTS", CivilopediaPageTypes.CIVILOPEDIA_PAGE_CONCEPT, WidgetTypes.WIDGET_PEDIA_JUMP_TO_CONCEPT, lambda: self.getInfos(gc.getNumConceptInfos(), gc.getConceptInfo)),
			CvPediaView(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_HELP_HINTS, "TXT_KEY_PEDIA_VIEW_HINTS", CivilopediaPageTypes.CIVILOPEDIA_PAGE_HINTS, WidgetTypes.WIDGET_PEDIA_JUMP_TO_HINTS, lambda: []),
			CvPediaView(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_HELP_HOTKEY_CLASSES, "TXT_KEY_PEDIA_VIEW_HOTKEY_CLASSES", CivilopediaPageTypes.CIVILOPEDIA_PAGE_HOTKEY_CLASS, WidgetTypes.WIDGET_PEDIA_JUMP_TO_HOTKEY_CLASS, lambda: self.getInfos(gc.getNumHotKeyClassInfos(), gc.getHotKeyClassInfo), [], []),
		]
		
		self.pages = {
			#CivilopediaPageTypes.CIVILOPEDIA_PAGE_INTRODUCTION : CvPediaIntroduction(self),
			CivilopediaPageTypes.CIVILOPEDIA_PAGE_CONCEPT : CvPediaConcept(self),
			CivilopediaPageTypes.CIVILOPEDIA_PAGE_HINTS : CvPediaHints(self),
			CivilopediaPageTypes.CIVILOPEDIA_PAGE_HOTKEY_CLASS : CvPediaHotKeys(self)
		}
		
		self.setDefaults()
	