## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
from CvPythonExtensions import *
import PyHelpers
import CvUtil
import ScreenInput
import CvScreenEnums
import string

PyPlayer = PyHelpers.PyPlayer
PyInfo = PyHelpers.PyInfo

# globals
gc = CyGlobalContext()
ArtFileMgr = CyArtFileMgr()
localText = CyTranslator()

class CvEraMovieScreen:
	"Wonder Movie Screen"
	def interfaceScreen (self, eEra):
		
		# K-Mod note: I've taken out all reference to X_SCREEN and Y_SCREEN, and instead set it up to be automatically centered. (I haven't left the old code in. I've just deleted it.)
		self.SCREEN_W = 775
		self.SCREEN_H = 720  # old 660
		
		self.TITLE_Y = 20
		
		self.GRAPHIC_X = 27
		self.GRAPHIC_Y = 50
		self.GRAPHIC_W = 720
		self.GRAPHIC_H = 540
		
		self.EXIT_X = self.SCREEN_W / 2 - 50
		self.EXIT_Y = self.SCREEN_H - 50
		self.EXIT_W = 120
		self.EXIT_H = 30
		
		self.EFFECTS_X = 27
		self.EFFECTS_Y = self.GRAPHIC_Y + self.GRAPHIC_H + 7
		self.EFFECTS_W = 720
		self.EFFECTS_H = self.EXIT_Y - self.EFFECTS_Y - 7
		
		if (CyInterface().noTechSplash()):
			return 0
		
		player = PyPlayer(CyGame().getActivePlayer())
		
		screen = CyGInterfaceScreen( "EraMovieScreen" + str(eEra), CvScreenEnums.ERA_MOVIE_SCREEN)
		#screen.setDimensions(screen.centerX(0), screen.centerY(0), self.SCREEN_W, self.SCREEN_H) # This doesn't work. Those 'center' functions assume a particular window size. (not original code)
		screen.setDimensions(screen.getXResolution()/2-self.SCREEN_W/2, screen.getYResolution()/2-self.SCREEN_H/2, self.SCREEN_W, self.SCREEN_H) # K-Mod
		screen.addPanel("EraMoviePanel", "", "", True, True, 0, 0, self.SCREEN_W, self.SCREEN_H, PanelStyles.PANEL_STYLE_MAIN)
		
		screen.showWindowBackground(True)
		screen.setRenderInterfaceOnly(False)
		screen.setSound("AS2D_NEW_ERA")
		screen.showScreen(PopupStates.POPUPSTATE_MINIMIZED, False)
		
		# Header...
		szHeader = localText.getText("TXT_KEY_ERA_SPLASH_SCREEN", (gc.getEraInfo(eEra).getTextKey(), ))
		szHeaderId = "EraTitleHeader" + str(eEra)
		screen.setText(szHeaderId, "Background", szHeader, CvUtil.FONT_CENTER_JUSTIFY, self.SCREEN_W / 2, self.TITLE_Y, 0, FontTypes.TITLE_FONT, WidgetTypes.WIDGET_GENERAL, -1, -1)
		# MOD - START - Era Effects
		szEffects = CyGameTextMgr().getEraHelp(eEra, True, False)[1:]
		screen.addMultilineText("EraSplashEffectText", szEffects, self.EFFECTS_X, self.EFFECTS_Y, self.EFFECTS_W, self.EFFECTS_H, WidgetTypes.WIDGET_GENERAL, -1, -1, CvUtil.FONT_CENTER_JUSTIFY)
		#screen.setText("EraSplashEffectText", "Background", szEffects, CvUtil.FONT_CENTER_JUSTIFY, self.SCREEN_W / 2, self.EFFECTS_Y - 30, 0, FontTypes.TITLE_FONT, WidgetTypes.WIDGET_GENERAL, -1, -1)
		"""
		if (gc.getEraInfo(eEra).getDiscoverHappiness() != 0):
			if (gc.getEraInfo(eEra).getDiscoverHappiness() > 0):
				szHeader = localText.getText("TXT_KEY_ERA_SPLASH_HAPPY", (gc.getEraInfo(eEra).getDiscoverHappiness(), ))
			else:
				szHeader = localText.getText("TXT_KEY_ERA_SPLASH_SCREEN3", (-(gc.getEraInfo(eEra).getDiscoverHappiness()), ))
			screen.setText("EraHappyText" + str(eEra), "Background", szHeader, CvUtil.FONT_CENTER_JUSTIFY, self.SCREEN_W / 2, self.EFFECTS_Y - 30, 0, FontTypes.TITLE_FONT, WidgetTypes.WIDGET_GENERAL, -1, -1)

		if (gc.getEraInfo(eEra).getDiscoverHealth() != 0):
			if (gc.getEraInfo(eEra).getDiscoverHealth() > 0):
				szHeader = localText.getText("TXT_KEY_ERA_SPLASH_HEALTHY", (gc.getEraInfo(eEra).getDiscoverHealth(), ))
			else:
				szHeader = localText.getText("TXT_KEY_ERA_SPLASH_UNHEALTHY", (-(gc.getEraInfo(eEra).getDiscoverHealth()), ))
			screen.setText("EraHealthText" + str(eEra), "Background", szHeader, CvUtil.FONT_CENTER_JUSTIFY, self.SCREEN_W / 2, self.EFFECTS_Y, 0, FontTypes.TITLE_FONT, WidgetTypes.WIDGET_GENERAL, -1, -1)
		"""
		# MOD - END- Era Effects
		screen.setButtonGFC("EraExit" + str(eEra), localText.getText("TXT_KEY_MAIN_MENU_OK", ()), "", self.EXIT_X, self.EXIT_Y, self.EXIT_W, self.EXIT_H, WidgetTypes.WIDGET_CLOSE_SCREEN, -1, -1, ButtonStyles.BUTTON_STYLE_STANDARD )

		# Play the movie
		if eEra == 1:
			szMovie = "Art/Movies/Era/Era01-Classical.dds"
		elif eEra == 2:
			szMovie = "Art/Movies/Era/Era02-Medeival.dds"
		elif eEra == 3:
			szMovie = "Art/Movies/Era/Era03-Renaissance.dds"
		elif eEra == 4:
			szMovie = "Art/Movies/Era/Era04-Industrial.dds"
		else:
			szMovie = "Art/Movies/Era/Era05-Modern.dds"

		screen.addDDSGFC("EraMovieMovie" + str(eEra), szMovie, self.GRAPHIC_X, self.GRAPHIC_Y, self.GRAPHIC_W, self.GRAPHIC_H, WidgetTypes.WIDGET_GENERAL, -1, -1)

		return 0

	# Will handle the input for this screen...
	def handleInput (self, inputClass):
		return 0

	def update(self, fDelta):
		return

