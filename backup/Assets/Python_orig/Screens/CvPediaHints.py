## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
from CvPythonExtensions import *
import CvUtil
import CvPediaPage

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

class CvPediaHints(CvPediaPage.CvPediaPage):
	"Civilopedia Screen for Hints"

	def __init__(self, section):
		super(CvPediaHints, self).__init__(section, CivilopediaPageTypes.CIVILOPEDIA_PAGE_HINTS, WidgetTypes.WIDGET_PEDIA_JUMP_TO_HINTS, "Hints")
		
		self.HINTS_LIST_ID = self.szWidget + "HintsList"
	
	def calculateLayout(self):
		self.calculateTitleLayout()
		
		self.HINTS_LIST_X = self.main.PAGE_AREA_X
		self.HINTS_LIST_Y = self.main.PAGE_AREA_Y
		self.HINTS_LIST_W = self.main.PAGE_AREA_W
		self.HINTS_LIST_H = self.main.PAGE_AREA_H
	
	def draw(self):
		if (not self.isEmpty()):
			return
		
		screen = self.getScreen()
		
		self.drawHints(screen, CyGameTextMgr().buildHintsList())
	
	def drawHints(self, screen, szText):
		screen.addListBoxGFC(self.record(self.HINTS_LIST_ID), "", self.HINTS_LIST_X, self.HINTS_LIST_Y, self.HINTS_LIST_W, self.HINTS_LIST_H, TableStyles.TABLE_STYLE_STANDARD)
		screen.enableSelect(self.HINTS_LIST_ID, False)
		hintText = szText.split("\n")
		for szHint in hintText:
			if len(szHint) != 0:
				screen.appendListBoxStringNoUpdate(self.HINTS_LIST_ID, szHint, WidgetTypes.WIDGET_GENERAL, -1, -1, CvUtil.FONT_LEFT_JUSTIFY)
		screen.updateListBox(self.HINTS_LIST_ID)
