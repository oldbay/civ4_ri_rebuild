## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
from CvPythonExtensions import *
import CvUtil
import CvPediaPage

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

class CvPediaConcept(CvPediaPage.CvPediaPage):
	"Civilopedia Screen for Concepts"

	def __init__(self, section):
		super(CvPediaConcept, self).__init__(section, CivilopediaPageTypes.CIVILOPEDIA_PAGE_CONCEPT, WidgetTypes.WIDGET_PEDIA_JUMP_TO_CONCEPT, "Concept")
		
		self.eConcept = -1
	
	def getData(self):
		return (self.eConcept, -1)
	
	def setData(self, eConcept, iPageData2):
		if (self.eConcept != eConcept):
			self.clear()
			self.eConcept = eConcept
	
	def getInfo(self):
		return gc.getConceptInfo(self.eConcept)
	
	def calculateLayout(self):
		self.calculateTitleLayout()
		
		self.CIVILOPEDIA_PANEL_X = self.main.PAGE_AREA_X
		self.CIVILOPEDIA_PANEL_Y = self.PAGE_CONTENT_Y
		self.CIVILOPEDIA_PANEL_W = self.main.PAGE_AREA_W
		self.CIVILOPEDIA_PANEL_H = self.main.PAGE_AREA_Y + self.main.PAGE_AREA_H - self.CIVILOPEDIA_PANEL_Y
	
	def draw(self):
		if (not self.isEmpty()):
			return
		
		screen = self.getScreen()
		info = self.getInfo()
		ePlayer = gc.getGame().getActivePlayer()
		
		self.drawTitle(screen, info, ePlayer)
		#self.drawSynopsis(screen, info, ePlayer)
		self.drawCivilopedia(screen, info, "")
	