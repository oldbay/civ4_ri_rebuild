## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005

from CvPythonExtensions import *
import CvPediaView

# globals
gc = CyGlobalContext()

class CvPediaViewTechnologiesEra(CvPediaView.CvPediaView):
	"Civilopedia View"

	def __init__(self, section, eEra):
		super(CvPediaViewTechnologiesEra, self).__init__(section, CivilopediaViewTypes.CIVILOPEDIA_VIEW_TECHNOLOGIES_ERA, "", CivilopediaPageTypes.CIVILOPEDIA_PAGE_TECHNOLOGY, WidgetTypes.WIDGET_PEDIA_JUMP_TO_TECH, lambda: self.getInfos(gc.getNumTechInfos(), gc.getTechInfo, self.filterTechEra, eEra))
		
		self.eEra = eEra
	
	def getTitle(self):
		return gc.getEraInfo(self.eEra).getDescription()
	
	def matchesView(self, eView, eEra, iViewData2):
		return self.eView == eView and self.eEra == eEra
	
	def getData(self):
		return (self.eEra, -1)
	
	def filterTechEra(self, info, eEra):
		if (info.getEra() == eEra):
			return True
		else:
			return False
	