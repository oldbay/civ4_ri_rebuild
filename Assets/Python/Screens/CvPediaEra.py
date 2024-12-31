## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
from CvPythonExtensions import *
import CvUtil
import CvPediaPage

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

class CvPediaEra(CvPediaPage.CvPediaPage):
	"Civilopedia Screen for Eras"

	def __init__(self, section):
		super(CvPediaEra, self).__init__(section, CivilopediaPageTypes.CIVILOPEDIA_PAGE_ERA, WidgetTypes.WIDGET_PEDIA_JUMP_TO_ERA, "Era")
		
		self.eEra = -1
		
		self.ERA_GRAPHIC_ID = self.szWidget + "EraGraphic"
	
	def getData(self):
		return (self.eEra, -1)
	
	def setData(self, eEra, iPageData2):
		if (self.eEra != eEra):
			self.clear()
			self.eEra = eEra
	
	def getInfo(self):
		return gc.getEraInfo(self.eEra)
	
	def calculateLayout(self):
		self.calculateTitleLayout()
		
		self.EFFECTS_PANEL_Y = self.PAGE_CONTENT_Y
		self.EFFECTS_PANEL_H = 100
		
		self.ERA_GRAPHIC_Y = self.EFFECTS_PANEL_Y + self.EFFECTS_PANEL_H + self.main.COMMON_MARGIN_H
		self.ERA_GRAPHIC_H = self.main.PAGE_AREA_Y + self.main.PAGE_AREA_H - self.ERA_GRAPHIC_Y
		
		self.ERA_GRAPHIC_W = min(self.main.PAGE_AREA_W, int(self.ERA_GRAPHIC_H * 4 / 3))
		self.ERA_GRAPHIC_X = self.main.PAGE_AREA_X + (self.main.PAGE_AREA_W - self.ERA_GRAPHIC_W) / 2
		
		self.EFFECTS_PANEL_X = self.ERA_GRAPHIC_X
		self.EFFECTS_PANEL_W = self.ERA_GRAPHIC_W
	
	def draw(self):
		if (not self.isEmpty()):
			return
		
		screen = self.getScreen()
		info = self.getInfo()
		ePlayer = gc.getGame().getActivePlayer()
		
		self.drawTitle(screen, info, ePlayer)
		self.drawEraGraphic(screen)
		self.drawEffects(screen, CyGameTextMgr().getEraHelp(self.eEra, False, True)[1:])
	
	def drawEraGraphic(self, screen):
		if self.eEra == 0:
			szMovie = "Art/Movies/Era/Era01-Classical.dds"
		elif self.eEra == 1:
			szMovie = "Art/Movies/Era/Era01-Classical.dds"
		elif self.eEra == 2:
			szMovie = "Art/Movies/Era/Era02-Medeival.dds"
		elif self.eEra == 3:
			szMovie = "Art/Movies/Era/Era03-Renaissance.dds"
		elif self.eEra == 4:
			szMovie = "Art/Movies/Era/Era04-Industrial.dds"
		else:
			szMovie = "Art/Movies/Era/Era05-Modern.dds"
		
		screen.addDDSGFC(self.record(self.ERA_GRAPHIC_ID), szMovie, self.ERA_GRAPHIC_X, self.ERA_GRAPHIC_Y, self.ERA_GRAPHIC_W, self.ERA_GRAPHIC_H, WidgetTypes.WIDGET_GENERAL, -1, -1)
