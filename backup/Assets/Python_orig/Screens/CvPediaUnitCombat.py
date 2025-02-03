## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
from CvPythonExtensions import *
import CvUtil
import CvPediaPage

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

class CvPediaUnitCombat(CvPediaPage.CvPediaPage):
	"Civilopedia Screen for Unit Combat Groups"

	def __init__(self, section):
		super(CvPediaUnitCombat, self).__init__(section, CivilopediaPageTypes.CIVILOPEDIA_PAGE_UNIT_COMBAT, WidgetTypes.WIDGET_PEDIA_JUMP_TO_UNIT_COMBAT, "UnitCombat")
		
		self.eUnitCombat = -1
		
		self.OUTER_BACKGROUND_PANEL_ID = self.szWidget + "OuterBackgroundPanel"
		self.INNER_BACKGROUND_PANEL_ID = self.szWidget + "InnerBackgroundPanel"
		self.UNIT_TABLE_ID = self.szWidget + "UnitTable"
	
	def getData(self):
		return (self.eUnitCombat, -1)
	
	def setData(self, eUnitCombat, iPageData2):
		if (self.eUnitCombat != eUnitCombat):
			self.clear()
			self.eUnitCombat = eUnitCombat
	
	def getInfo(self):
		return gc.getUnitCombatInfo(self.eUnitCombat)
	
	def calculateLayout(self):
		self.calculateTitleLayout()
		
		iMargin = 20
		
		self.OUTER_BACKGROUND_PANEL_X = self.main.PAGE_AREA_X
		self.OUTER_BACKGROUND_PANEL_Y = self.PAGE_CONTENT_Y
		self.OUTER_BACKGROUND_PANEL_W = self.main.PAGE_AREA_W
		self.OUTER_BACKGROUND_PANEL_H = self.main.PAGE_AREA_Y + self.main.PAGE_AREA_H - self.OUTER_BACKGROUND_PANEL_Y
		
		self.INNER_BACKGROUND_PANEL_X = self.OUTER_BACKGROUND_PANEL_X + iMargin
		self.INNER_BACKGROUND_PANEL_Y = self.OUTER_BACKGROUND_PANEL_Y + iMargin
		self.INNER_BACKGROUND_PANEL_W = self.OUTER_BACKGROUND_PANEL_W - iMargin * 2
		self.INNER_BACKGROUND_PANEL_H = self.OUTER_BACKGROUND_PANEL_H - iMargin * 2
		
		self.UNIT_TABLE_X = self.INNER_BACKGROUND_PANEL_X + 1
		self.UNIT_TABLE_Y = self.INNER_BACKGROUND_PANEL_Y + 7
		self.UNIT_TABLE_W = self.INNER_BACKGROUND_PANEL_W - 1
		self.UNIT_TABLE_H = self.INNER_BACKGROUND_PANEL_H - 10
	
	def draw(self):
		if (not self.isEmpty()):
			return
		
		screen = self.getScreen()
		info = self.getInfo()
		ePlayer = gc.getGame().getActivePlayer()
		
		self.drawTitle(screen, info, ePlayer)
		self.drawUnitTable(screen, info, ePlayer)
	
	def drawUnitTable(self, screen, info, ePlayer):
		screen.addPanel(self.record(self.OUTER_BACKGROUND_PANEL_ID), "", "", True, True, self.OUTER_BACKGROUND_PANEL_X, self.OUTER_BACKGROUND_PANEL_Y, self.OUTER_BACKGROUND_PANEL_W, self.OUTER_BACKGROUND_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		screen.addPanel(self.record(self.INNER_BACKGROUND_PANEL_ID), "", "", True, True, self.INNER_BACKGROUND_PANEL_X, self.INNER_BACKGROUND_PANEL_Y, self.INNER_BACKGROUND_PANEL_W, self.INNER_BACKGROUND_PANEL_H, PanelStyles.PANEL_STYLE_BLUE50)
		
		screen.addTableControlGFC(self.record(self.UNIT_TABLE_ID), 4, self.UNIT_TABLE_X, self.UNIT_TABLE_Y, self.UNIT_TABLE_W, self.UNIT_TABLE_H, True, False, 32,32, TableStyles.TABLE_STYLE_EMPTY)
		screen.enableSort(self.UNIT_TABLE_ID)
		
#		screen.attachTableControlGFC( panelName, self.UNIT_TABLE_ID, 4, False, True, 32, 32, TableStyles.TABLE_STYLE_EMPTY );
		
		iColWidth = int(self.UNIT_TABLE_W * (7 / 19.0))
		screen.setTableColumnHeader(self.UNIT_TABLE_ID, 0, "", iColWidth)
		iColWidth = int(self.UNIT_TABLE_W * (4 / 19.0))
		screen.setTableColumnHeader(self.UNIT_TABLE_ID, 1, u"%c" % CyGame().getSymbolID(FontSymbols.STRENGTH_CHAR), iColWidth)
		screen.setTableColumnHeader(self.UNIT_TABLE_ID, 2, u"%c" % CyGame().getSymbolID(FontSymbols.MOVES_CHAR), iColWidth)
		screen.setTableColumnHeader(self.UNIT_TABLE_ID, 3, u"%c" % gc.getYieldInfo(YieldTypes.YIELD_PRODUCTION).getChar(), iColWidth)
		
		# count units in this group
		nUnits = 0
		for j in range(gc.getNumUnitInfos()):
			if (self.eUnitCombat == gc.getUnitInfo(j).getUnitCombatType() or self.eUnitCombat == gc.getNumUnitCombatInfos()):
				nUnits += 1
		
		# sort Units by strength
		i = 0
		unitsList=[(0,0,0,0,0)]*nUnits
		for j in range(gc.getNumUnitInfos()):
			if (self.eUnitCombat == gc.getUnitInfo(j).getUnitCombatType() or self.eUnitCombat == gc.getNumUnitCombatInfos()):
			
				if (gc.getUnitInfo(j).getDomainType() == DomainTypes.DOMAIN_AIR):
					iStrength = unicode(gc.getUnitInfo(j).getAirCombat())
					iMovement = unicode(gc.getUnitInfo(j).getAirRange())
				else:
					iStrength = unicode(gc.getUnitInfo(j).getCombat())
					iMovement = unicode(gc.getUnitInfo(j).getMoves())
					
				if (gc.getUnitInfo(j).getProductionCost() < 0):
					szCost = localText.getText("TXT_KEY_NON_APPLICABLE", ())
				else:
					szCost = unicode(gc.getUnitInfo(j).getProductionCost())# + u"%c" % gc.getYieldInfo(YieldTypes.YIELD_PRODUCTION).getChar()
					
				unitsList[i] = (iStrength, iMovement, szCost, gc.getUnitInfo(j).getDescription(), j)
				i += 1
		
		for i in range(nUnits):			
			iRow = screen.appendTableRow(self.UNIT_TABLE_ID)
			screen.setTableText(self.UNIT_TABLE_ID, 0, iRow, u"<font=3>" + unitsList[i][3] + u"</font>", "", WidgetTypes.WIDGET_PEDIA_JUMP_TO_UNIT, unitsList[i][4], 1, CvUtil.FONT_LEFT_JUSTIFY)						
			screen.setTableInt(self.UNIT_TABLE_ID, 1, iRow, u"<font=3>" + unicode(unitsList[i][0]) + u"</font>", "", WidgetTypes.WIDGET_GENERAL, -1, -1, CvUtil.FONT_LEFT_JUSTIFY)
			screen.setTableInt(self.UNIT_TABLE_ID, 2, iRow, u"<font=3>" + unicode(unitsList[i][1]) + u"</font>", "", WidgetTypes.WIDGET_GENERAL, -1, -1, CvUtil.FONT_LEFT_JUSTIFY)
			screen.setTableInt(self.UNIT_TABLE_ID, 3, iRow, u"<font=3>" + unicode(unitsList[i][2]) + u"</font>", "", WidgetTypes.WIDGET_GENERAL, -1, -1, CvUtil.FONT_LEFT_JUSTIFY)
