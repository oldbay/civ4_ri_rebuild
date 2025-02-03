## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
from CvPythonExtensions import *
import CvUtil
import CvPediaPage

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

class CvPediaHotKeys(CvPediaPage.CvPediaPage):
	"Civilopedia Screen for HotKeys"

	def __init__(self, section):
		super(CvPediaHotKeys, self).__init__(section, CivilopediaPageTypes.CIVILOPEDIA_PAGE_HOTKEY_CLASS, WidgetTypes.WIDGET_PEDIA_JUMP_TO_HOTKEY_CLASS, "HotKeys")
		
		self.eHotKeyClass = -1
		
		self.HOTKEYS_LIST_ID = self.szWidget + "HotKeysList"
	
	def getData(self):
		return (self.eHotKeyClass, -1)
	
	def setData(self, eHotKeyClass, iPageData2):
		if (self.eHotKeyClass != eHotKeyClass):
			self.clear()
			self.eHotKeyClass = eHotKeyClass
	
	def getInfo(self):
		return gc.getHotKeyClassInfo(self.eHotKeyClass)
	
	def calculateLayout(self):
		self.calculateTitleLayout()
		
		self.HOTKEYS_LIST_X = self.main.PAGE_AREA_X
		self.HOTKEYS_LIST_Y = self.PAGE_CONTENT_Y
		self.HOTKEYS_LIST_W = self.main.PAGE_AREA_W
		self.HOTKEYS_LIST_H = self.main.PAGE_AREA_Y + self.main.PAGE_AREA_H - self.HOTKEYS_LIST_Y
	
	def draw(self):
		if (not self.isEmpty()):
			return
		
		screen = self.getScreen()
		info = self.getInfo()
		ePlayer = gc.getGame().getActivePlayer()
		
		self.drawTitle(screen, info, ePlayer)
		self.drawHotKeys(screen, info)
	
	def drawHotKeys(self, screen, info):
		screen.addListBoxGFC(self.record(self.HOTKEYS_LIST_ID), "", self.HOTKEYS_LIST_X, self.HOTKEYS_LIST_Y, self.HOTKEYS_LIST_W, self.HOTKEYS_LIST_H, TableStyles.TABLE_STYLE_STANDARD)
		screen.enableSelect(self.HOTKEYS_LIST_ID, False)
		for iRel in range(info.getNumActions()):
			eAction = info.getActions(iRel)
			kAction = gc.getActionInfo(eAction)
			screen.appendListBoxStringNoUpdate(self.HOTKEYS_LIST_ID, kAction.getHotKeyDescription(), WidgetTypes.WIDGET_GENERAL, -1, -1, CvUtil.FONT_LEFT_JUSTIFY)
		screen.updateListBox(self.HOTKEYS_LIST_ID)
