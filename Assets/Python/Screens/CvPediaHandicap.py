## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
from CvPythonExtensions import *
import CvUtil
import CvPediaPage

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

class CvPediaHandicap(CvPediaPage.CvPediaPage):
	"Civilopedia Screen for Difficulty Levels"

	def __init__(self, section):
		super(CvPediaHandicap, self).__init__(section, CivilopediaPageTypes.CIVILOPEDIA_PAGE_HANDICAP, WidgetTypes.WIDGET_PEDIA_JUMP_TO_HANDICAP, "Handicap")
		
		self.eHandicap = -1
	
	def getData(self):
		return (self.eHandicap, -1)
	
	def setData(self, eHandicap, iPageData2):
		if (self.eHandicap != eHandicap):
			self.clear()
			self.eHandicap = eHandicap
	
	def getInfo(self):
		return gc.getHandicapInfo(self.eHandicap)
	
	def calculateLayout(self):
		self.calculateTitleLayout()
		
		self.EFFECTS_PANEL_X = self.main.PAGE_AREA_X
		self.EFFECTS_PANEL_Y = self.PAGE_CONTENT_Y
		self.EFFECTS_PANEL_W = self.main.PAGE_AREA_W
		self.EFFECTS_PANEL_H = self.main.PAGE_AREA_Y + self.main.PAGE_AREA_H - self.EFFECTS_PANEL_Y
	
	def draw(self):
		if (not self.isEmpty()):
			return
		
		screen = self.getScreen()
		info = self.getInfo()
		ePlayer = gc.getGame().getActivePlayer()
		
		self.drawTitle(screen, info, ePlayer)
		#self.drawSynopsis(screen, info, ePlayer)
		self.drawEffects(screen, CyGameTextMgr().getHandicapHelp(self.eHandicap, True)[1:])
	