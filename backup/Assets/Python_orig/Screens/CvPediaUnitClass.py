## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
from CvPythonExtensions import *
import CvUtil
import CvPediaPage

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

class CvPediaUnitClass(CvPediaPage.CvPediaPage):
	"Civilopedia Screen for Unit Classes"

	def __init__(self, section):
		super(CvPediaUnitClass, self).__init__(section, CivilopediaPageTypes.CIVILOPEDIA_PAGE_UNIT_CLASS, WidgetTypes.WIDGET_PEDIA_JUMP_TO_UNIT, "UnitClass") # FIXME
		
		self.eUnitClass = -1
	
	def getData(self):
		return (self.eUnitClass, -1)
	
	def setData(self, eUnitClass, iPageData2):
		if (self.eUnitClass != eUnitClass):
			self.clear()
			self.eUnitClass = eUnitClass
	
	def getInfo(self):
		return gc.getUnitClassInfo(self.eUnitClass)
	
	def calculateLayout(self):
		self.calculateTitleLayout()
	
	def draw(self):
		if (not self.isEmpty()):
			return
		
		screen = self.getScreen()
		info = self.getInfo()
		ePlayer = gc.getGame().getActivePlayer()
		
		self.drawTitle(screen, info, ePlayer)
		#self.drawSynopsis(screen, info, ePlayer)
	