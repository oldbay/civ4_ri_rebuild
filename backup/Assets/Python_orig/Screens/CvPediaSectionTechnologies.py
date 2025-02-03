## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
from CvPythonExtensions import *

from CvPediaSection import CvPediaSection
from CvPediaView import CvPediaView
from CvPediaViewTechGraph import CvPediaViewTechGraph
from CvPediaViewTechnologiesEra import CvPediaViewTechnologiesEra
from CvPediaTechGraph import CvPediaTechGraph
from CvPediaTech import CvPediaTech

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

class CvPediaSectionTechnologies(CvPediaSection):
	"Civilopedia Section for Technologies"

	def __init__(self, main):
		super(CvPediaSectionTechnologies, self).__init__(main, CivilopediaSectionTypes.CIVILOPEDIA_SECTION_TECHNOLOGIES, "TXT_KEY_PEDIA_SECTION_TECHNOLOGIES", "Technologies")
		
		self.views = [
			CvPediaViewTechGraph(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_TECHNOLOGIES_GRAPH, "TXT_KEY_PEDIA_VIEW_TECHNOLOGIES_GRAPH"),
			CvPediaView(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_TECHNOLOGIES_ALL, "TXT_KEY_PEDIA_VIEW_TECHNOLOGIES_ALL", CivilopediaPageTypes.CIVILOPEDIA_PAGE_TECHNOLOGY, WidgetTypes.WIDGET_PEDIA_JUMP_TO_TECH, lambda: self.getInfos(gc.getNumTechInfos(), gc.getTechInfo))
		]
		
		for eEra in range(gc.getNumEraInfos()):
			self.views.append(CvPediaViewTechnologiesEra(self, eEra))
			#self.views.append(CvPediaView(self, CivilopediaViewTypes.CIVILOPEDIA_VIEW_TECHNOLOGIES_ERA, i, gc.getEraInfo(i).getDescription(), CivilopediaPageTypes.CIVILOPEDIA_PAGE_TECHNOLOGY, WidgetTypes.WIDGET_PEDIA_JUMP_TO_TECH, self.getViewItems(gc.getNumTechInfos(), gc.getTechInfo, -1, self.filterTechEra, i)))
		
		self.pages = {
			CivilopediaPageTypes.CIVILOPEDIA_PAGE_TECHNOLOGY_GRAPH : CvPediaTechGraph(self),
			CivilopediaPageTypes.CIVILOPEDIA_PAGE_TECHNOLOGY : CvPediaTech(self)
		}
		
		self.setDefaults()

