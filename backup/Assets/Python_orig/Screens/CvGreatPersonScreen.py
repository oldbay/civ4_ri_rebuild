## Sid Meier's Civilization 4
from CvPythonExtensions import *
import PyHelpers
import CvUtil
import ScreenInput
import CvScreenEnums
import string
import os
import BugPath
import GreatPersonNaming

# ScS  import the new file that we created with the .bat file
# ScS  import os.path to deal with checking whether files exist.

PyPlayer = PyHelpers.PyPlayer
PyInfo = PyHelpers.PyInfo

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

# ScS Variable to snage the art Path from the .bat file.

class CvGreatPersonScreen:
	"Great Person Screen"
	
	def __init__(self):

		self.X_SCREEN = 255
		self.Y_SCREEN = 40
		self.W_SCREEN = 515
		self.H_SCREEN = 570
		self.Z_BACKGROUND = -1.1
		self.Z_CONTROLS = self.Z_BACKGROUND - 0.2
		self.DZ = -0.2
		

		self.iMarginSpace = 15

		
		self.Y_TITLE = self.Y_SCREEN + 25
		
		self.W_PORTRAIT = 460
		self.H_PORTRAIT = 460
				
		self.X_EXIT = self.X_SCREEN + self.W_SCREEN/2 - 66
		self.Y_EXIT = self.Y_SCREEN + self.H_SCREEN - 50
		self.W_EXIT = 120
		self.H_EXIT = 30
		
		self.PICTURE_DIR = "Art\\GreatPeople\\"
		self.PICTURE_FILE = "%s.dds"
		self.PICTURE_PATH = self.PICTURE_DIR + self.PICTURE_FILE
	
	
	def interfaceScreen (self,iData):

		d = GreatPersonNaming.g_dict
		pUnit, iPlayer, pCity = d.pop(iData)
		player = gc.getPlayer(iPlayer)
	
		szPersonName = pUnit.getNameNoDesc()

		# if the name is the empty string, then create a new, generic name from the unit type.
		szDefaultName = gc.getUnitInfo(pUnit.getUnitType()).getDescription()
		if szPersonName == "":
			szPersonName = szDefaultName

		szImageName = szPersonName
		bFound = false

		# if there is a file in the GreatPeople directory with the name, we'll load it.
		if gc.findAssetFile(str(self.PICTURE_PATH %(szImageName))):
			bFound = True

		#if not, load the great "type of person" file
		if not bFound:
			szImageName = szDefaultName
			if gc.findAssetFile(str(self.PICTURE_PATH %(szImageName))):
				bFound = True

		#if not, load the great person file
		if not bFound:
			szImageName = "Great Person"

		szPicturePath = str(self.PICTURE_PATH %(szImageName))

		screen = CyGInterfaceScreen( "GreatPersonScreen", CvScreenEnums.ERA_MOVIE_SCREEN)
		screen.addPanel("GreatPersonPanel", "", "", true, true,
			self.X_SCREEN, self.Y_SCREEN, self.W_SCREEN, self.H_SCREEN, PanelStyles.PANEL_STYLE_MAIN)
		
		screen.showWindowBackground(True)
		screen.setSound("AS2D_NEW_ERA")
		screen.showScreen(PopupStates.POPUPSTATE_MINIMIZED, False)
		screen.setDimensions(screen.centerX(0), screen.centerY(0), 1024, 768)
		screen.setRenderInterfaceOnly(False)


		# Header...
		szHeader = localText.getText("TXT_KEY_MISC_GP_BORN", (szPersonName, pCity.getName()))  #ScS
		szHeaderId = "GreatPersonTitleHeader"
		screen.setText(szHeaderId, "Background", szHeader, CvUtil.FONT_CENTER_JUSTIFY,self.X_SCREEN + self.W_SCREEN / 2, self.Y_TITLE, 0, FontTypes.TITLE_FONT, WidgetTypes.WIDGET_GENERAL, -1, -1)
		screen.setButtonGFC("GreatPersonExit", localText.getText("TXT_KEY_MAIN_MENU_OK", ()), "", self.X_EXIT, self.Y_EXIT, self.W_EXIT, self.H_EXIT, WidgetTypes.WIDGET_CLOSE_SCREEN, -1, -1, ButtonStyles.BUTTON_STYLE_STANDARD )
		screen.addDDSGFC("GreatPersonPortrait", szPicturePath, self.X_SCREEN + 27, self.Y_SCREEN + 50, self.W_PORTRAIT, self.H_PORTRAIT, WidgetTypes.WIDGET_GENERAL, -1, -1 )

		return 0
		
	# Will handle the input for this screen...
	def handleInput (self, inputClass):
		return 0

	def update(self, fDelta):
		return

