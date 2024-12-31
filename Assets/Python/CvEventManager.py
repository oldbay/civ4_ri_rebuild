## Sid Meier's Civilization 4
## Copyright Firaxis Games 2006
## 
## CvEventManager
## This class is passed an argsList from CvAppInterface.onEvent
## The argsList can contain anything from mouse location to key info
## The EVENTLIST that are being notified can be found 


from CvPythonExtensions import *
import CvUtil
import CvScreensInterface
import CvDebugTools
import PyHelpers
import Popup as PyPopup
import CvCameraControls
import CvTopCivs
import sys
import CvAdvisorUtils
import CvTechChooser
## Platy Builder ##
import WBCityEditScreen
import WBUnitScreen
import WBPlayerScreen
import WBGameDataScreen
import WBPlotScreen
import CvPlatyBuilderScreen
## Platy Builder ##
## Dynamic City Naming ##
if not CyGlobalContext().getGame().isOption(GameOptionTypes.GAMEOPTION_NO_CITY_NAMING):
	import DynamicCityNaming
## Dynamic City Naming ##
## Stored Data Handling ##
from StoredData import sd
## Stored Data Handling ##
## Revolutions ##
import Revolutions
## Revolutions ##
import DynamicCivNaming
## Barbarian Civ ##
import BarbCiv
## Barbarian Civ ##
## Influence Driven War ##
import IDW
## Influence Driven War ##
import Consts as con

gc = CyGlobalContext()
localText = CyTranslator()
PyPlayer = PyHelpers.PyPlayer
PyInfo = PyHelpers.PyInfo

## Stored Data Handling ##
if gc.getMap().getMapScriptName(): # loads stored data on module reload
	sd.load()
## Stored Data Handling ##

# globals
###################################################
class CvEventManager:
	def __init__(self):
		#################### ON EVENT MAP ######################
		#print "EVENTMANAGER INIT"
				
		self.bCtrl = False
		self.bShift = False
		self.bAlt = False
		self.bAllowCheats = False
		
		# OnEvent Enums
		self.EventLButtonDown=1
		self.EventLcButtonDblClick=2
		self.EventRButtonDown=3
		self.EventBack=4
		self.EventForward=5
		self.EventKeyDown=6
		self.EventKeyUp=7
	
		self.__LOG_MOVEMENT = 0
		self.__LOG_BUILDING = 0
		self.__LOG_COMBAT = 0
		self.__LOG_CONTACT = 0
		self.__LOG_IMPROVEMENT =0
		self.__LOG_CITYLOST = 0
		self.__LOG_CITYBUILDING = 0
		self.__LOG_TECH = 0
		self.__LOG_UNITBUILD = 0
		self.__LOG_UNITKILLED = 1
		self.__LOG_UNITLOST = 0
		self.__LOG_UNITPROMOTED = 0
		self.__LOG_UNITSELECTED = 0
		self.__LOG_UNITPILLAGE = 0
		self.__LOG_GOODYRECEIVED = 0
		self.__LOG_GREATPERSON = 0
		self.__LOG_RELIGION = 0
		self.__LOG_RELIGIONSPREAD = 0
		self.__LOG_GOLDENAGE = 0
		self.__LOG_ENDGOLDENAGE = 0
		self.__LOG_WARPEACE = 0
		self.__LOG_PUSH_MISSION = 0
		
		## EVENTLIST
		self.EventHandlerMap = {
			'mouseEvent'			: self.onMouseEvent,
			'kbdEvent' 				: self.onKbdEvent,
			'ModNetMessage'			: self.onModNetMessage,
			'Init'					: self.onInit,
			'Update'				: self.onUpdate,
			'UnInit'				: self.onUnInit,
			'OnSave'				: self.onSaveGame,
			'OnPreSave'				: self.onPreSave,
			'OnLoad'				: self.onLoadGame,
			'GameStart'				: self.onGameStart,
			'GameEnd'				: self.onGameEnd,
			'plotRevealed' 			: self.onPlotRevealed,
			'plotFeatureRemoved' 	: self.onPlotFeatureRemoved,
			'plotPicked'			: self.onPlotPicked,
			'nukeExplosion'			: self.onNukeExplosion,
			'gotoPlotSet'			: self.onGotoPlotSet,
			'BeginGameTurn'			: self.onBeginGameTurn,
			'EndGameTurn'			: self.onEndGameTurn,
			'BeginPlayerTurn'		: self.onBeginPlayerTurn,
			'EndPlayerTurn'			: self.onEndPlayerTurn,
			'endTurnReady'			: self.onEndTurnReady,
			'combatResult' 			: self.onCombatResult,
			'combatLogCalc'	 		: self.onCombatLogCalc,
			'combatLogHit'			: self.onCombatLogHit,
			'improvementBuilt' 		: self.onImprovementBuilt,
			'improvementDestroyed' 	: self.onImprovementDestroyed,
			'routeBuilt' 			: self.onRouteBuilt,
			'firstContact' 			: self.onFirstContact,
			'cityBuilt' 			: self.onCityBuilt,
			'cityRazed'				: self.onCityRazed,
			'cityAcquired' 			: self.onCityAcquired,
			'cityAcquiredAndKept' 	: self.onCityAcquiredAndKept,
			'cityLost'				: self.onCityLost,
			'cultureExpansion' 		: self.onCultureExpansion,
			'cityGrowth' 			: self.onCityGrowth,
			'cityDoTurn' 			: self.onCityDoTurn,
			'cityBuildingUnit'		: self.onCityBuildingUnit,
			'cityBuildingBuilding'	: self.onCityBuildingBuilding,
			'cityRename'			: self.onCityRename,
			'cityHurry'				: self.onCityHurry,
			'selectionGroupPushMission'		: self.onSelectionGroupPushMission,
			'unitMove' 				: self.onUnitMove,
			'unitSetXY' 			: self.onUnitSetXY,
			'unitCreated' 			: self.onUnitCreated,
			'unitBuilt' 			: self.onUnitBuilt,
			'unitKilled'			: self.onUnitKilled,
			'unitLost'				: self.onUnitLost,
			'unitPromoted'			: self.onUnitPromoted,
			'unitSelected'			: self.onUnitSelected, 
			'UnitRename'				: self.onUnitRename,
			'unitPillage'				: self.onUnitPillage,
			'unitSpreadReligionAttempt'	: self.onUnitSpreadReligionAttempt,
			'unitGifted'				: self.onUnitGifted,
			'unitBuildImprovement'				: self.onUnitBuildImprovement,
			'goodyReceived'        	: self.onGoodyReceived,
			'greatPersonBorn'      	: self.onGreatPersonBorn,
			'buildingBuilt' 		: self.onBuildingBuilt,
			'projectBuilt' 			: self.onProjectBuilt,
			'techAcquired'			: self.onTechAcquired,
			'techSelected'			: self.onTechSelected,
			'religionFounded'		: self.onReligionFounded,
			'religionSpread'		: self.onReligionSpread, 
			'religionRemove'		: self.onReligionRemove, 
			'corporationFounded'	: self.onCorporationFounded,
			'corporationSpread'		: self.onCorporationSpread, 
			'corporationRemove'		: self.onCorporationRemove, 
			'goldenAge'				: self.onGoldenAge,
			'endGoldenAge'			: self.onEndGoldenAge,
			'chat' 					: self.onChat,
			'victory'				: self.onVictory,
			'vassalState'			: self.onVassalState,
			'playerRevolution'		: self.onPlayerRevolution,
			'changeWar'				: self.onChangeWar,
			'setPlayerAlive'		: self.onSetPlayerAlive,
			'playerChangeStateReligion'		: self.onPlayerChangeStateReligion,
			'playerGoldTrade'		: self.onPlayerGoldTrade,
			'windowActivation'		: self.onWindowActivation,
			'gameUpdate'			: self.onGameUpdate,		# sample generic event
		}

		################## Events List ###############################
		#
		# Dictionary of Events, indexed by EventID (also used at popup context id)
		#   entries have name, beginFunction, applyFunction [, randomization weight...]
		#
		# Normal events first, random events after
		#	
		################## Events List ###############################
		self.Events={
			CvUtil.EventEditCityName : ('EditCityName', self.__eventEditCityNameApply, self.__eventEditCityNameBegin),
			CvUtil.EventPlaceObject : ('PlaceObject', self.__eventPlaceObjectApply, self.__eventPlaceObjectBegin),
			CvUtil.EventAwardTechsAndGold: ('AwardTechsAndGold', self.__eventAwardTechsAndGoldApply, self.__eventAwardTechsAndGoldBegin),
			CvUtil.EventEditUnitName : ('EditUnitName', self.__eventEditUnitNameApply, self.__eventEditUnitNameBegin),
## Platy Builder ##
			CvUtil.EventWBLandmarkPopup : ('WBLandmarkPopup', self.__eventWBLandmarkPopupApply, self.__eventWBScriptPopupBegin),
			CvUtil.EventShowWonder: ('ShowWonder', self.__eventShowWonderApply, self.__eventShowWonderBegin),
			1111 : ('WBPlayerScript', self.__eventWBPlayerScriptPopupApply, self.__eventWBScriptPopupBegin),
			2222 : ('WBCityScript', self.__eventWBCityScriptPopupApply, self.__eventWBScriptPopupBegin),
			3333 : ('WBUnitScript', self.__eventWBUnitScriptPopupApply, self.__eventWBScriptPopupBegin),
			4444 : ('WBGameScript', self.__eventWBGameScriptPopupApply, self.__eventWBScriptPopupBegin),
			5555 : ('WBPlotScript', self.__eventWBPlotScriptPopupApply, self.__eventWBScriptPopupBegin),
## Platy Builder ##
		}	
#################### EVENT STARTERS ######################
	def handleEvent(self, argsList):
		'EventMgr entry point'
		# extract the last 6 args in the list, the first arg has already been consumed
		self.origArgsList = argsList	# point to original
		tag = argsList[0]				# event type string
		idx = len(argsList)-6
		bDummy = false
		self.bDbg, bDummy, self.bAlt, self.bCtrl, self.bShift, self.bAllowCheats = argsList[idx:]
		ret = 0
		if self.EventHandlerMap.has_key(tag):
			fxn = self.EventHandlerMap[tag]
			ret = fxn(argsList[1:idx])
		return ret
		
#################### EVENT APPLY ######################	
	def beginEvent( self, context, argsList=-1 ):
		'Begin Event'
		entry = self.Events[context]
		return entry[2]( argsList )
	
	def applyEvent( self, argsList ):
		'Apply the effects of an event '
		context, playerID, netUserData, popupReturn = argsList
		
		if context == CvUtil.PopupTypeEffectViewer:
			return CvDebugTools.g_CvDebugTools.applyEffectViewer( playerID, netUserData, popupReturn )
		
		entry = self.Events[context]
				
		if ( context not in CvUtil.SilentEvents ):
			self.reportEvent(entry, context, (playerID, netUserData, popupReturn) )
		return entry[1]( playerID, netUserData, popupReturn )   # the apply function

	def reportEvent(self, entry, context, argsList):
		'Report an Event to Events.log '
		if (gc.getGame().getActivePlayer() != -1):
			message = "DEBUG Event: %s (%s)" %(entry[0], gc.getActivePlayer().getName())
			CyInterface().addImmediateMessage(message,"")
			CvUtil.pyPrint(message)
		return 0
		
#################### ON EVENTS ######################
	def onKbdEvent(self, argsList):
		'keypress handler - return 1 if the event was consumed'

		eventType,key,mx,my,px,py = argsList
		game = gc.getGame()

		if (self.bAllowCheats):
			# notify debug tools of input to allow it to override the control
			argsList = (eventType,key,self.bCtrl,self.bShift,self.bAlt,mx,my,px,py,gc.getGame().isNetworkMultiPlayer())
			if ( CvDebugTools.g_CvDebugTools.notifyInput(argsList) ):
				return 0
		
		if ( eventType == self.EventKeyDown ):
			theKey=int(key)
			
			CvCameraControls.g_CameraControls.handleInput( theKey )
			
## Platy Helpers ##
			if self.bCtrl and theKey == int(InputTypes.KB_F1):
				iPlayer = CyGame().getActivePlayer()
				CyInterface().addMessage(iPlayer,True,30,CyInterface().getHelpString(),'',0, '', -1, -1, -1, True,True)
				return 1
## Platy Helpers ##	
						
			if (self.bAllowCheats):
				# Shift - T (Debug - No MP)
				if (theKey == int(InputTypes.KB_T)):
					if ( self.bShift ):
						self.beginEvent(CvUtil.EventAwardTechsAndGold)
						#self.beginEvent(CvUtil.EventCameraControlPopup)
						return 1
							
				elif (theKey == int(InputTypes.KB_W)):
					if ( self.bShift and self.bCtrl):
						self.beginEvent(CvUtil.EventShowWonder)
						return 1
							
				# Shift - ] (Debug - currently mouse-overd unit, health += 10
				elif (theKey == int(InputTypes.KB_LBRACKET) and self.bShift ):
					unit = CyMap().plot(px, py).getUnit(0)
					if ( not unit.isNone() ):
						d = min( unit.maxHitPoints()-1, unit.getDamage() + 10 )
						unit.setDamage( d, PlayerTypes.NO_PLAYER )
					
				# Shift - [ (Debug - currently mouse-overd unit, health -= 10
				elif (theKey == int(InputTypes.KB_RBRACKET) and self.bShift ):
					unit = CyMap().plot(px, py).getUnit(0)
					if ( not unit.isNone() ):
						d = max( 0, unit.getDamage() - 10 )
						unit.setDamage( d, PlayerTypes.NO_PLAYER )
					
				elif (theKey == int(InputTypes.KB_F1)):
					if ( self.bShift ):
						CvScreensInterface.replayScreen.showScreen(False)
						return 1
					# don't return 1 unless you want the input consumed
				
				elif (theKey == int(InputTypes.KB_F2)):
					if ( self.bShift ):
						import CvDebugInfoScreen
						CvScreensInterface.showDebugInfoScreen()
						return 1
				
				elif (theKey == int(InputTypes.KB_F3)):
					if ( self.bShift ):
						CvScreensInterface.showDanQuayleScreen(())
						return 1
						
				elif (theKey == int(InputTypes.KB_F4)):
					if ( self.bShift ):
						CvScreensInterface.showUnVictoryScreen(())
						return 1
											
		return 0

	def onModNetMessage(self, argsList):
		'Called whenever CyMessageControl().sendModNetMessage() is called - this is all for you modders!'
		
		iData1, iData2, iData3, iData4, iData5 = argsList
		
		print("Modder's net message!")
		
		CvUtil.pyPrint( 'onModNetMessage' )

	def onInit(self, argsList):
		'Called when Civ starts up'
		CvUtil.pyPrint( 'OnInit' )
		
	def onUpdate(self, argsList):
		'Called every frame'
		fDeltaTime = argsList[0]
		
		# allow camera to be updated
		CvCameraControls.g_CameraControls.onUpdate( fDeltaTime )
		
	def onWindowActivation(self, argsList):
		'Called when the game window activates or deactivates'
		bActive = argsList[0]
		
	def onUnInit(self, argsList):
		'Called when Civ shuts down'
		CvUtil.pyPrint('OnUnInit')
	
	def onPreSave(self, argsList):
		"called before a game is actually saved"
		CvUtil.pyPrint('OnPreSave')
		## Stored Data Handling ##
		sd.save() # pickle & save script data
		## Stored Data Handling ##
	
	def onSaveGame(self, argsList):
		"return the string to be saved - Must be a string"
		return ""

	def onLoadGame(self, argsList):
		## Stored Data Handling ##
		sd.load() # load & unpickle script data
		## Stored Data Handling ##

		if not gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_AHEAD):		
			CyGame().setAheadOfTimeEra(0)
			if (gc.getGame().getGameTurnYear() < -2000):
				for iTech in xrange(gc.getNumTechInfos()):
					TechInfo = gc.getTechInfo(iTech)
					if TechInfo.getEra() == 1:
						TechInfo.setAheadOfTime(100)
					elif TechInfo.getEra() == 2:
						TechInfo.setAheadOfTime(200)
					elif TechInfo.getEra() == 3:
						TechInfo.setAheadOfTime(300)
					elif TechInfo.getEra() == 4:
						TechInfo.setAheadOfTime(400)
					elif TechInfo.getEra() == 5:
						TechInfo.setAheadOfTime(500)
			if (gc.getGame().getGameTurnYear() > -2001):
				CyGame().setAheadOfTimeEra(1)
				for iTech in xrange(gc.getNumTechInfos()):
					TechInfo = gc.getTechInfo(iTech)
					if TechInfo.getEra() == 1:
						TechInfo.setAheadOfTime(50)
					elif TechInfo.getEra() == 2:
						TechInfo.setAheadOfTime(100)
					elif TechInfo.getEra() == 3:
						TechInfo.setAheadOfTime(200)
					elif TechInfo.getEra() == 4:
						TechInfo.setAheadOfTime(300)
					elif TechInfo.getEra() == 5:
						TechInfo.setAheadOfTime(400)
			if (gc.getGame().getGameTurnYear() > -1201):
				CyGame().setAheadOfTimeEra(2)
				for iTech in xrange(gc.getNumTechInfos()):
					TechInfo = gc.getTechInfo(iTech)
					if TechInfo.getEra() == 1:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 2:
						TechInfo.setAheadOfTime(100)
					elif TechInfo.getEra() == 3:
						TechInfo.setAheadOfTime(200)
					elif TechInfo.getEra() == 4:
						TechInfo.setAheadOfTime(300)
					elif TechInfo.getEra() == 5:
						TechInfo.setAheadOfTime(400)
			if (gc.getGame().getGameTurnYear() > 0):
				CyGame().setAheadOfTimeEra(3)
				for iTech in xrange(gc.getNumTechInfos()):
					TechInfo = gc.getTechInfo(iTech)
					if TechInfo.getEra() == 1:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 2:
						TechInfo.setAheadOfTime(50)
					elif TechInfo.getEra() == 3:
						TechInfo.setAheadOfTime(100)
					elif TechInfo.getEra() == 4:
						TechInfo.setAheadOfTime(200)
					elif TechInfo.getEra() == 5:
						TechInfo.setAheadOfTime(300)
			if (gc.getGame().getGameTurnYear() > 400):
				CyGame().setAheadOfTimeEra(4)
				for iTech in xrange(gc.getNumTechInfos()):
					TechInfo = gc.getTechInfo(iTech)
					if TechInfo.getEra() == 1:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 2:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 3:
						TechInfo.setAheadOfTime(100)
					elif TechInfo.getEra() == 4:
						TechInfo.setAheadOfTime(200)
					elif TechInfo.getEra() == 5:
						TechInfo.setAheadOfTime(300)
			if (gc.getGame().getGameTurnYear() > 1100):
				CyGame().setAheadOfTimeEra(5)
				for iTech in xrange(gc.getNumTechInfos()):
					TechInfo = gc.getTechInfo(iTech)
					if TechInfo.getEra() == 1:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 2:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 3:
						TechInfo.setAheadOfTime(50)
					elif TechInfo.getEra() == 4:
						TechInfo.setAheadOfTime(100)
					elif TechInfo.getEra() == 5:
						TechInfo.setAheadOfTime(200)
			if (gc.getGame().getGameTurnYear() > 1400):
				CyGame().setAheadOfTimeEra(6)
				for iTech in xrange(gc.getNumTechInfos()):
					TechInfo = gc.getTechInfo(iTech)
					if TechInfo.getEra() == 1:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 2:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 3:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 4:
						TechInfo.setAheadOfTime(100)
					elif TechInfo.getEra() == 5:
						TechInfo.setAheadOfTime(200)
			if (gc.getGame().getGameTurnYear() > 1600):
				CyGame().setAheadOfTimeEra(7)
				for iTech in xrange(gc.getNumTechInfos()):
					TechInfo = gc.getTechInfo(iTech)
					if TechInfo.getEra() == 1:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 2:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 3:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 4:
						TechInfo.setAheadOfTime(50)
					elif TechInfo.getEra() == 5:
						TechInfo.setAheadOfTime(100)
			if (gc.getGame().getGameTurnYear() > 1750):
				CyGame().setAheadOfTimeEra(8)
				for iTech in xrange(gc.getNumTechInfos()):
					TechInfo = gc.getTechInfo(iTech)
					if TechInfo.getEra() == 1:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 2:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 3:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 4:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 5:
						TechInfo.setAheadOfTime(100)
			if (gc.getGame().getGameTurnYear() > 1910):
				CyGame().setAheadOfTimeEra(9)
				for iTech in xrange(gc.getNumTechInfos()):
					TechInfo = gc.getTechInfo(iTech)
					if TechInfo.getEra() == 1:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 2:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 3:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 4:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 5:
						TechInfo.setAheadOfTime(50)
			if (gc.getGame().getGameTurnYear() > 1940):
				CyGame().setAheadOfTimeEra(10)
				for iTech in xrange(gc.getNumTechInfos()):
					TechInfo = gc.getTechInfo(iTech)
					if TechInfo.getEra() == 1:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 2:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 3:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 4:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 5:
						TechInfo.setAheadOfTime(0)

		CvAdvisorUtils.resetNoLiberateCities()
		return 0

	def onGameStart(self, argsList):
		'Called at the start of the game'
		iTeamCount = 0
		for iTeam in range(gc.getMAX_TEAMS()):
			team = gc.getTeam(iTeam)
			if team.isAlive(): iTeamCount += 1
		CyGame().setInitialNumPlayers(iTeamCount) ## Stores the starting number of teams which is later compared when calculating global modifiers; doesn't change within a given game 
		if not gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_AHEAD):
			CyGame().setAheadOfTimeEra(0)
			if (gc.getGame().getGameTurnYear() == gc.getDefineINT("START_YEAR")) or (gc.getGame().getGameTurnYear() < -2000):
				for iTech in xrange(gc.getNumTechInfos()):
					TechInfo = gc.getTechInfo(iTech)
					if TechInfo.getEra() == 1:
						TechInfo.setAheadOfTime(100)
					elif TechInfo.getEra() == 2:
						TechInfo.setAheadOfTime(200)
					elif TechInfo.getEra() == 3:
						TechInfo.setAheadOfTime(300)
					elif TechInfo.getEra() == 4:
						TechInfo.setAheadOfTime(400)
					elif TechInfo.getEra() == 5:
						TechInfo.setAheadOfTime(500)
			if (gc.getGame().getGameTurnYear() > -2001):
				CyGame().setAheadOfTimeEra(1)
				for iTech in xrange(gc.getNumTechInfos()):
					TechInfo = gc.getTechInfo(iTech)
					if TechInfo.getEra() == 1:
						TechInfo.setAheadOfTime(50)
					elif TechInfo.getEra() == 2:
						TechInfo.setAheadOfTime(100)
					elif TechInfo.getEra() == 3:
						TechInfo.setAheadOfTime(200)
					elif TechInfo.getEra() == 4:
						TechInfo.setAheadOfTime(300)
					elif TechInfo.getEra() == 5:
						TechInfo.setAheadOfTime(400)
			if (gc.getGame().getGameTurnYear() > -1201):
				CyGame().setAheadOfTimeEra(2)
				for iTech in xrange(gc.getNumTechInfos()):
					TechInfo = gc.getTechInfo(iTech)
					if TechInfo.getEra() == 1:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 2:
						TechInfo.setAheadOfTime(100)
					elif TechInfo.getEra() == 3:
						TechInfo.setAheadOfTime(200)
					elif TechInfo.getEra() == 4:
						TechInfo.setAheadOfTime(300)
					elif TechInfo.getEra() == 5:
						TechInfo.setAheadOfTime(400)
			if (gc.getGame().getGameTurnYear() > 0):
				CyGame().setAheadOfTimeEra(3)
				for iTech in xrange(gc.getNumTechInfos()):
					TechInfo = gc.getTechInfo(iTech)
					if TechInfo.getEra() == 1:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 2:
						TechInfo.setAheadOfTime(50)
					elif TechInfo.getEra() == 3:
						TechInfo.setAheadOfTime(100)
					elif TechInfo.getEra() == 4:
						TechInfo.setAheadOfTime(200)
					elif TechInfo.getEra() == 5:
						TechInfo.setAheadOfTime(300)
			if (gc.getGame().getGameTurnYear() > 400):
				CyGame().setAheadOfTimeEra(4)
				for iTech in xrange(gc.getNumTechInfos()):
					TechInfo = gc.getTechInfo(iTech)
					if TechInfo.getEra() == 1:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 2:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 3:
						TechInfo.setAheadOfTime(100)
					elif TechInfo.getEra() == 4:
						TechInfo.setAheadOfTime(200)
					elif TechInfo.getEra() == 5:
						TechInfo.setAheadOfTime(300)
			if (gc.getGame().getGameTurnYear() > 1100):
				CyGame().setAheadOfTimeEra(5)
				for iTech in xrange(gc.getNumTechInfos()):
					TechInfo = gc.getTechInfo(iTech)
					if TechInfo.getEra() == 1:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 2:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 3:
						TechInfo.setAheadOfTime(50)
					elif TechInfo.getEra() == 4:
						TechInfo.setAheadOfTime(100)
					elif TechInfo.getEra() == 5:
						TechInfo.setAheadOfTime(200)
			if (gc.getGame().getGameTurnYear() > 1400):
				CyGame().setAheadOfTimeEra(6)
				for iTech in xrange(gc.getNumTechInfos()):
					TechInfo = gc.getTechInfo(iTech)
					if TechInfo.getEra() == 1:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 2:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 3:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 4:
						TechInfo.setAheadOfTime(100)
					elif TechInfo.getEra() == 5:
						TechInfo.setAheadOfTime(200)
			if (gc.getGame().getGameTurnYear() > 1600):
				CyGame().setAheadOfTimeEra(7)
				for iTech in xrange(gc.getNumTechInfos()):
					TechInfo = gc.getTechInfo(iTech)
					if TechInfo.getEra() == 1:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 2:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 3:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 4:
						TechInfo.setAheadOfTime(50)
					elif TechInfo.getEra() == 5:
						TechInfo.setAheadOfTime(100)
			if (gc.getGame().getGameTurnYear() > 1750):
				CyGame().setAheadOfTimeEra(8)
				for iTech in xrange(gc.getNumTechInfos()):
					TechInfo = gc.getTechInfo(iTech)
					if TechInfo.getEra() == 1:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 2:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 3:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 4:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 5:
						TechInfo.setAheadOfTime(100)
			if (gc.getGame().getGameTurnYear() > 1910):
				CyGame().setAheadOfTimeEra(9)
				for iTech in xrange(gc.getNumTechInfos()):
					TechInfo = gc.getTechInfo(iTech)
					if TechInfo.getEra() == 1:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 2:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 3:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 4:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 5:
						TechInfo.setAheadOfTime(50)
			if (gc.getGame().getGameTurnYear() > 1940):
				CyGame().setAheadOfTimeEra(10)
				for iTech in xrange(gc.getNumTechInfos()):
					TechInfo = gc.getTechInfo(iTech)
					if TechInfo.getEra() == 1:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 2:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 3:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 4:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 5:
						TechInfo.setAheadOfTime(0)				
		
		## Stabilizer ##
		for i in xrange(gc.getNumGameOptionInfos()):
			Info = gc.getGameOptionInfo(i)
			if Info.getVisible(): continue
			CyGame().setOption(i, Info.getDefault())
		## Stabilizer ##
		
		if gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_UNIT_COST_SCALING):				## Balances barbarian unit outputs for scenarios where barbarian cities are pre-placed ##
			iBarb = gc.getBARBARIAN_PLAYER()
			pBarb = gc.getPlayer(iBarb)
			if pBarb.getNumCities() > 0:
				(loopCity, iter) = pBarb.firstCity(False)
				while(loopCity):
					loopCity.setNumRealBuilding(gc.getInfoTypeForString("BUILDING_BARBARIAN_NERF"), 1)
					(loopCity, iter) = pBarb.nextCity(iter, False)
					
		if (gc.getGame().getGameTurnYear() == gc.getDefineINT("START_YEAR") and not gc.getGame().isOption(GameOptionTypes.GAMEOPTION_ADVANCED_START)):
			for iPlayer in range(gc.getMAX_PLAYERS()):
				player = gc.getPlayer(iPlayer)
				if not player.isNone():
					if not gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_DYNAMIC_CIV_FLAGS):
						iCiv = player.getCivilizationType()
						CivInfo = gc.getCivilizationInfo(iCiv)
						if CivInfo:
							sFlag = player.getFlagDecal()
							if not (sFlag[24:28] == str("Scn_") or (sFlag[24:28] == str("Msc_")) or (sFlag[24:28] == str("Fire"))):	
								sFlag = CivInfo.getFlagTexture()
								player.setFlagDecal(sFlag, true)
								if not (sFlag[24:39] == str("FlagDECAL_Skull") or sFlag[24:29] == str("X_def")):
									player.setWhiteFlag(true)
					DynamicCivNaming.DynamicCivNaming().nameStart(iPlayer)
				if (player.isAlive() and player.isHuman()):
					popupInfo = CyPopupInfo()
					popupInfo.setButtonPopupType(ButtonPopupTypes.BUTTONPOPUP_PYTHON_SCREEN)
					popupInfo.setText(u"showDawnOfMan")
					popupInfo.addPopup(iPlayer)
		else:
			CyInterface().setSoundSelectionReady(true)
			
## 		print ("Current civ ID list")
## 		for iCiv in xrange(gc.getNumCivilizationInfos()):
## 			print ("Civ " + str(iCiv) + " " + str(gc.getCivilizationInfo(iCiv).getDescription()))
			
			
		## Stored Data Handling ##
		sd.setup() # initialise global script data
		## Stored Data Handling ##

		## Stored Data Handling ##
		sd.save()
		## Stored Data Handling ##

		if gc.getGame().isPbem():
			for iPlayer in range(gc.getMAX_PLAYERS()):
				player = gc.getPlayer(iPlayer)
				if (player.isAlive() and player.isHuman()):
					popupInfo = CyPopupInfo()
					popupInfo.setButtonPopupType(ButtonPopupTypes.BUTTONPOPUP_DETAILS)
					popupInfo.setOption1(true)
					popupInfo.addPopup(iPlayer)

		CvAdvisorUtils.resetNoLiberateCities()

		## Dynamic City Naming ##
		if not gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_CITY_NAMING):
			DynamicCityNaming.onGameStart()
		## Dynamic City Naming ##

		# Absinthe: adding unique improvements instead of normal ones for preset maps
		#			this function is kinda half-baked, as unfortunately it can't be done entirely automatically with the way unique improvements are implemented
		#			so it has to be set manually somewhere which are the to-be-replaced unique improvement - base improvement pairs
		#			anyway, if the cultural owner of a tile has access to a given UI, and the tile has the required improvement (and resource if needed), it replaces it
		# loop through all plots
		for iX in range(gc.getMap().getGridWidth()):
			for iY in range(gc.getMap().getGridHeight()):
				pPlot = gc.getMap().plot(iX, iY)
				iOwner = pPlot.getOwner()
				# if there is an owner (so it's inside any civ's cultural borders)
				if iOwner != -1:
					iImprovementType = pPlot.getImprovementType()
					# all of this only if the owner has access to the given improvement:
					# Horse Breeder instead of Pasture and Horse
					if gc.getCivilizationInfo(gc.getPlayer(iOwner).getCivilizationType()).canProduceImprovement(con.iImpHorseBreederHungary):
						if iImprovementType == con.iImpPasture and pPlot.getBonusType(-1) == con.iHorse:
							pPlot.setImprovementType(con.iImpHorseBreederHungary)
							print ("new Horse Breeder improvement was set: ", iOwner, iImprovementType, con.iImpHorseBreederHungary)
							continue
					# Mineria instead of Precious Mine
					if gc.getCivilizationInfo(gc.getPlayer(iOwner).getCivilizationType()).canProduceImprovement(con.iImpMineriaSpain):
						if iImprovementType == con.iImpPreciousMine:
							pPlot.setImprovementType(con.iImpMineriaSpain)
							print ("new Mineria improvement was set: ", iOwner, iImprovementType, con.iImpMineriaSpain)
							continue
					# Fishing and Whaling Fleet instead of Fishing and Whaling Boat
					if gc.getCivilizationInfo(gc.getPlayer(iOwner).getCivilizationType()).canProduceImprovement(con.iImpFishingVesselViking):
						if iImprovementType == con.iImpFishingBoats:
							pPlot.setImprovementType(con.iImpFishingVesselViking)
							print ("new Fishing Fleet improvement was set: ", iOwner, iImprovementType, con.iImpFishingVesselViking)
							continue
					if gc.getCivilizationInfo(gc.getPlayer(iOwner).getCivilizationType()).canProduceImprovement(con.iImpWhalingVesselViking):
						if iImprovementType == con.iImpWhalingBoats:
							pPlot.setImprovementType(con.iImpWhalingVesselViking)
							print ("new Whaling Fleet improvement was set: ", iOwner, iImprovementType, con.iImpWhalingVesselViking)
							continue
					# Loviche instead of Trapper Lodge
					if gc.getCivilizationInfo(gc.getPlayer(iOwner).getCivilizationType()).canProduceImprovement(con.iImpLovischcheRussia):
						if iImprovementType == con.iImpTrapperLodge:
							pPlot.setImprovementType(con.iImpLovischcheRussia)
							print ("new Loviche improvement was set: ", iOwner, iImprovementType, con.iImpLovischcheRussia)
							continue
					# Terroir Vineyard instead of Winery
					if gc.getCivilizationInfo(gc.getPlayer(iOwner).getCivilizationType()).canProduceImprovement(con.iImpVineyardFrance):
						if iImprovementType == con.iImpWinery:
							pPlot.setImprovementType(con.iImpVineyardFrance)
							print ("new Terroir Vineyard improvement was set: ", iOwner, iImprovementType, con.iImpVineyardFrance)
							continue
					# Water Wheel instead of Watermill
					if gc.getCivilizationInfo(gc.getPlayer(iOwner).getCivilizationType()).canProduceImprovement(con.iImpWaterWheelGreece):
						if iImprovementType == con.iImpWatermill:
							pPlot.setImprovementType(con.iImpWaterWheelGreece)
							print ("new Water Wheel improvement was set: ", iOwner, iImprovementType, con.iImpWaterWheelGreece)
							continue
					# Bedouin Camp and Ksar instead of Settlement
					for iUniqueImprovement in [con.iImpKsarBerber, con.iImpBeduinCampArab]:
						if gc.getCivilizationInfo(gc.getPlayer(iOwner).getCivilizationType()).canProduceImprovement(iUniqueImprovement):
							if iImprovementType == con.iImpSettlement:
								pPlot.setImprovementType(iUniqueImprovement)
								print ("new unique Settlement improvement was set: ", iOwner, iImprovementType, iUniqueImprovement)
								break
					# Farm UIs instead of Farm
					for iUniqueImprovement in [con.iImpWuguFarmSChina, con.iImpCommunalFarmKorea, con.iImpPancakrstiFieldHindi, con.iImpFolwarkPoland]:
						if gc.getCivilizationInfo(gc.getPlayer(iOwner).getCivilizationType()).canProduceImprovement(iUniqueImprovement):
							if iImprovementType == con.iImpFarm:
								pPlot.setImprovementType(iUniqueImprovement)
								print ("new unique Farm improvement was set: ", iOwner, iImprovementType, iUniqueImprovement)
								break
		# Absinthe: end
		
		# Generate warnings about weird starting conditions
		errorText = ""
		errorCount = 0
		if (not gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_TECH_TRANSFER)) and (not gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_TECH_TRADING)):
			errorText += "CRITICAL WARNING: both Tech Trading and Tech Transfer custom options were turned on for this game. These two mechanics were not designed to work with each other. It is highly recommended to restart with Tech Trading switched off. The mod was balanced to be played with Tech Trading turned off and Tech Transfer turned on."
			errorCount += 1

		if gc.getGame().isOption(GameOptionTypes.GAMEOPTION_PICK_RELIGION):
			if errorCount > 0:
				errorText += "\n\n"
			errorText += "CRITICAL WARNING: Pick Religion custom game option is on. It is not normally selectable in this mod and should be off by default. It is recommended you restart another mod or vanilla game you were playing before and deselect it before restarting this mod, as it seems to be a case of custom game options being incorrectly inherited from another mod or vanilla game. The religions in this mod were balanced without the possibility to use this option in mind, and it might lead to extremely unbalanced results."
			errorCount += 1

		if (not gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_REVOLUTIONS)) and (CyGame().getInitialNumPlayers() > (gc.getMAX_PLAYERS() - 2)) and (not gc.getMap().getMapScriptName().endswith("Scenario.CivBeyondSwordWBSave")):
			if errorCount > 0:
				errorText += "\n\n"
			errorText += "CRITICAL WARNING: You have selected almost all available player slots while turning on Revolutions. Revolutions component REQUIRES several (preferably over 10) free slots to be avaliable for spawning new civilizations, otherwise it will fail, potentially producing game-breaking errors."
			errorCount += 1
		elif (not gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_REVOLUTIONS)) and CyGame().getInitialNumPlayers() > (gc.getMAX_PLAYERS() - 10) and (not gc.getMap().getMapScriptName().endswith("Scenario.CivBeyondSwordWBSave")):
			if errorCount > 0:
				errorText += "\n\n"
			errorText += "SEVERE WARNING: You have selected a very high amount of available player slots while turning on Revolutions. Revolutions component REQUIRES several (preferably over 10) free slots to be avaliable for spawning new civilizations, otherwise it will fail, potentially producing game-breaking errors, and the few slots left to it might not be enough."
			errorCount += 1
		elif (gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_REVOLUTIONS)) and CyGame().getInitialNumPlayers() > (gc.getMAX_PLAYERS() - 10) and (not gc.getMap().getMapScriptName().endswith("Scenario.CivBeyondSwordWBSave")):
			if errorCount > 0:
				errorText += "\n\n"
			errorText += "SEVERE WARNING: You have selected a very high amount of starting civilizations. Since some game calculations increase in multiplicative manner from the amount of civs, this will likely adversely affect your performance, especially later on. The mod was also not balanced with such a high amount of active civs in mind, and as such you may encountered severe balance issues."
			errorCount += 1
		elif (CyGame().getInitialNumPlayers() > (gc.getWorldInfo(gc.getMap().getWorldSize()).getDefaultPlayers()*2)) and (not gc.getMap().getMapScriptName().endswith("Scenario.CivBeyondSwordWBSave")):
			if errorCount > 0:
				errorText += "\n\n"
			errorText += "WARNING: You have selected over twice the recommended player count for this map size. Since some game calculations increase in multiplicative manner from the amount of civs, this will likely adversely affect your performance, especially later on. The mod was also not balanced with such a high amount of active civs in mind, and as such you may encountered severe balance issues."
			errorCount += 1
			
		if gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_BARBARIAN_CIV) and gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_REVOLUTIONS) and CyGame().getInitialNumPlayers() < (gc.getWorldInfo(gc.getMap().getWorldSize()).getDefaultPlayers()/2):
			if errorCount > 0:
				errorText += "\n\n"
			errorText += "WARNING: you have selected too few civilizations for this map size without any method (barbarian civ or revolutions) for spawning more. This might negatively affect your game's balance."
			errorCount += 1
		
		if (gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_TECH_TRANSFER)) and (gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_TECH_TRADING)):
			if errorCount > 0:
				errorText += "\n\n"
			errorText += "WARNING: both Tech Trading and Tech Transfer custom options were turned off for this game. The game currently has no enabled means of spreading tech progress between civilizations, and this will potentially affect the balance in unpredictable ways. It is recommended to turn on Tech Transfer."
			errorCount += 1
		
		if errorCount > 0:
			iPlayerNum = 0
			for iPlayer in range(gc.getMAX_PLAYERS()):
				player = gc.getPlayer(iPlayer)
				if player.isAlive():
					iPlayerNum = iPlayerNum + 1
					if player.isHuman():
						popupInfo = CyPopupInfo()
						popupInfo.setButtonPopupType(ButtonPopupTypes.BUTTONPOPUP_PYTHON)
						popupInfo.setText(errorText)
						popupInfo.setOnClickedPythonCallback("")
						popupInfo.addPythonButton("Ok", "")
						popupInfo.addPopup(iPlayer)

	def onGameEnd(self, argsList):
		'Called at the End of the game'
		print("Game is ending")
		return

	def onBeginGameTurn(self, argsList):
		'Called at the beginning of the end of each turn'
		iGameTurn = argsList[0]
		CvTopCivs.CvTopCivs().turnChecker(iGameTurn)

		# Absinthe: cultivation improvements has an automatic chance to spawn the corresponding resource
		for iX in range(gc.getMap().getGridWidth()):
			for iY in range(gc.getMap().getGridHeight()):
				pPlot = gc.getMap().plot(iX, iY)
				iImprovementType = pPlot.getImprovementType()
				iOwner = pPlot.getOwner()
				# only check if the tile is inside anyone's cultural borders and there is an improvement
				if iOwner != -1:
					if iImprovementType != -1:
						for tCultivation in [(con.iImpWheatCultivation, con.iWheat, 100), (con.iImpCornCultivation, con.iCorn, 100),
												(con.iImpRiceCultivation, con.iRice, 100), (con.iImpPotatoCultivation, con.iPotato, 100),
												(con.iImpGrapeCultivationFrance, con.iWine, 100), (con.iImpSpiceCultivationDravida, con.iSpices, 100),
												(con.iImpCoffeeCultivationEthiopia, con.iCoffee, 100), (con.iImpDyeCultivationCarthage, con.iDye, 100)]:
							iCultivation, iBonus, iChance = tCultivation
							if iImprovementType == iCultivation:
								iRand = gc.getGame().getSorenRandNum(100, "successful cultivation")
								if iRand < iChance:
									# add the resource, remove the cultivation improvement and the fertile soil tile feature
									pPlot.setBonusType(iBonus)
									pPlot.setImprovementType(-1)
									pPlot.setFeatureType(FeatureTypes.NO_FEATURE, -1)
									#pPlot.setFeatureType(-1, 0)
									# interface message about it for the human player
									iHuman = gc.getGame().getActivePlayer()
									if iHuman == iOwner:
										sBonus = gc.getBonusInfo(iBonus).getTextKey()
										sImprovement = gc.getImprovementInfo(iCultivation).getTextKey()
										CyInterface().addMessage(iHuman, False, 15, CyTranslator().getText("TXT_KEY_CULTIVATION_SUCCESS",(sBonus,sImprovement)), "AS2D_DISCOVERBONUS", InterfaceMessageTypes.MESSAGE_TYPE_MINOR_EVENT, gc.getBonusInfo(iBonus).getButton(), ColorTypes(44), iX, iY, True, True)
		# Absinthe: end


	def onEndGameTurn(self, argsList):
		'Called at the end of the end of each turn'
		iGameTurn = argsList[0]
		## Barbarian Civ ##
		if not gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_BARBARIAN_CIV):
			BarbCiv.BarbCiv().checkBarb()
		## Barbarian Civ ##
		
		if not gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_AHEAD):
			if (gc.getGame().getGameTurnYear() > -2001) and (CyGame().getAheadOfTimeEra() < 1):
				CyGame().setAheadOfTimeEra(1)
				for iTech in xrange(gc.getNumTechInfos()):
					TechInfo = gc.getTechInfo(iTech)
					if TechInfo.getEra() == 1:
						TechInfo.setAheadOfTime(50)
					elif TechInfo.getEra() == 2:
						TechInfo.setAheadOfTime(100)
					elif TechInfo.getEra() == 3:
						TechInfo.setAheadOfTime(200)
					elif TechInfo.getEra() == 4:
						TechInfo.setAheadOfTime(300)
					elif TechInfo.getEra() == 5:
						TechInfo.setAheadOfTime(400)
				CyInterface().addImmediateMessage(CyTranslator().getText("TXT_BRON_ERA_AHEAD", ()), "")
			elif (gc.getGame().getGameTurnYear() > -1201) and (CyGame().getAheadOfTimeEra() < 2):
				CyGame().setAheadOfTimeEra(2)
				for iTech in xrange(gc.getNumTechInfos()):
					TechInfo = gc.getTechInfo(iTech)
					if TechInfo.getEra() == 1:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 2:
						TechInfo.setAheadOfTime(100)
					elif TechInfo.getEra() == 3:
						TechInfo.setAheadOfTime(200)
					elif TechInfo.getEra() == 4:
						TechInfo.setAheadOfTime(300)
					elif TechInfo.getEra() == 5:
						TechInfo.setAheadOfTime(400)
				CyInterface().addImmediateMessage(CyTranslator().getText("TXT_CLAS_ERA_AHEAD", ()), "")
			elif (gc.getGame().getGameTurnYear() > 0) and (CyGame().getAheadOfTimeEra() < 3):
				CyGame().setAheadOfTimeEra(3)
				for iTech in xrange(gc.getNumTechInfos()):
					TechInfo = gc.getTechInfo(iTech)
					if TechInfo.getEra() == 1:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 2:
						TechInfo.setAheadOfTime(50)
					elif TechInfo.getEra() == 3:
						TechInfo.setAheadOfTime(100)
					elif TechInfo.getEra() == 4:
						TechInfo.setAheadOfTime(200)
					elif TechInfo.getEra() == 5:
						TechInfo.setAheadOfTime(300)
				CyInterface().addImmediateMessage(CyTranslator().getText("TXT_IMP_ERA_AHEAD", ()), "")	
			elif (gc.getGame().getGameTurnYear() > 400) and (CyGame().getAheadOfTimeEra() < 4):
				CyGame().setAheadOfTimeEra(4)
				for iTech in xrange(gc.getNumTechInfos()):
					TechInfo = gc.getTechInfo(iTech)
					if TechInfo.getEra() == 1:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 2:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 3:
						TechInfo.setAheadOfTime(100)
					elif TechInfo.getEra() == 4:
						TechInfo.setAheadOfTime(200)
					elif TechInfo.getEra() == 5:
						TechInfo.setAheadOfTime(300)
				CyInterface().addImmediateMessage(CyTranslator().getText("TXT_MED_ERA_AHEAD", ()), "")
			elif (gc.getGame().getGameTurnYear() > 1100) and (CyGame().getAheadOfTimeEra() < 5):
				CyGame().setAheadOfTimeEra(5)
				for iTech in xrange(gc.getNumTechInfos()):
					TechInfo = gc.getTechInfo(iTech)
					if TechInfo.getEra() == 1:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 2:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 3:
						TechInfo.setAheadOfTime(50)
					elif TechInfo.getEra() == 4:
						TechInfo.setAheadOfTime(100)
					elif TechInfo.getEra() == 5:
						TechInfo.setAheadOfTime(200)
				CyInterface().addImmediateMessage(CyTranslator().getText("TXT_CRUS_ERA_AHEAD", ()), "")			
			elif (gc.getGame().getGameTurnYear() > 1400) and (CyGame().getAheadOfTimeEra() < 6):
				CyGame().setAheadOfTimeEra(6)
				for iTech in xrange(gc.getNumTechInfos()):
					TechInfo = gc.getTechInfo(iTech)
					if TechInfo.getEra() == 1:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 2:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 3:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 4:
						TechInfo.setAheadOfTime(100)
					elif TechInfo.getEra() == 5:
						TechInfo.setAheadOfTime(200)
				CyInterface().addImmediateMessage(CyTranslator().getText("TXT_REN_ERA_AHEAD", ()), "")
			elif (gc.getGame().getGameTurnYear() > 1600) and (CyGame().getAheadOfTimeEra() < 7):
				CyGame().setAheadOfTimeEra(7)
				for iTech in xrange(gc.getNumTechInfos()):
					TechInfo = gc.getTechInfo(iTech)
					if TechInfo.getEra() == 1:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 2:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 3:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 4:
						TechInfo.setAheadOfTime(50)
					elif TechInfo.getEra() == 5:
						TechInfo.setAheadOfTime(100)
				CyInterface().addImmediateMessage(CyTranslator().getText("TXT_NAT_ERA_AHEAD", ()), "")				
			elif (gc.getGame().getGameTurnYear() > 1750) and (CyGame().getAheadOfTimeEra() < 8):
				CyGame().setAheadOfTimeEra(8)
				for iTech in xrange(gc.getNumTechInfos()):
					TechInfo = gc.getTechInfo(iTech)
					if TechInfo.getEra() == 1:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 2:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 3:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 4:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 5:
						TechInfo.setAheadOfTime(100)
				CyInterface().addImmediateMessage(CyTranslator().getText("TXT_IND_ERA_AHEAD", ()), "")	
			elif (gc.getGame().getGameTurnYear() > 1910) and (CyGame().getAheadOfTimeEra() < 9):
				CyGame().setAheadOfTimeEra(9)
				for iTech in xrange(gc.getNumTechInfos()):
					TechInfo = gc.getTechInfo(iTech)
					if TechInfo.getEra() == 1:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 2:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 3:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 4:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 5:
						TechInfo.setAheadOfTime(50)
				CyInterface().addImmediateMessage(CyTranslator().getText("TXT_WW_ERA_AHEAD", ()), "")	
			elif (gc.getGame().getGameTurnYear() > 1940) and (CyGame().getAheadOfTimeEra() < 10):
				CyGame().setAheadOfTimeEra(10)
				for iTech in xrange(gc.getNumTechInfos()):
					TechInfo = gc.getTechInfo(iTech)
					if TechInfo.getEra() == 1:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 2:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 3:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 4:
						TechInfo.setAheadOfTime(0)
					elif TechInfo.getEra() == 5:
						TechInfo.setAheadOfTime(0)
				CyInterface().addImmediateMessage(CyTranslator().getText("TXT_MOD_ERA_AHEAD", ()), "")
		
	def onBeginPlayerTurn(self, argsList):
		'Called at the beginning of a players turn'
		iGameTurn, iPlayer = argsList

	def onEndPlayerTurn(self, argsList):
		'Called at the end of a players turn'
		iGameTurn, iPlayer = argsList
## Revolutions ##
		if not gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_REVOLUTIONS):
			Revolutions.Revolutions().checkRevolutions(iPlayer)
## Revolutions ##
		if (gc.getGame().getElapsedGameTurns() == 1):
			if (gc.getPlayer(iPlayer).isHuman()):
				if (gc.getPlayer(iPlayer).canRevolution(0)):
					popupInfo = CyPopupInfo()
					popupInfo.setButtonPopupType(ButtonPopupTypes.BUTTONPOPUP_CHANGECIVIC)
					popupInfo.addPopup(iPlayer)
		
		CvAdvisorUtils.resetAdvisorNags()
		CvAdvisorUtils.endTurnFeats(iPlayer)
		
	def onEndTurnReady(self, argsList):
		iGameTurn = argsList[0]

	def onFirstContact(self, argsList):
	## Platy Builder ##
		if CyGame().GetWorldBuilderMode() and not CvPlatyBuilderScreen.bPython: return
	## Platy Builder ##
		'Contact'
		iTeamX,iHasMetTeamY = argsList
		if (not self.__LOG_CONTACT):
			return
		CvUtil.pyPrint('Team %d has met Team %d' %(iTeamX, iHasMetTeamY))
	
	def onCombatResult(self, argsList):
		'Combat Result'
		pWinner,pLoser = argsList
		playerX = PyPlayer(pWinner.getOwner())
		unitX = PyInfo.UnitInfo(pWinner.getUnitType())
		playerY = PyPlayer(pLoser.getOwner())
		unitY = PyInfo.UnitInfo(pLoser.getUnitType())
		## Influence Driven War ##
		if gc.getGame().isOption(GameOptionTypes.GAMEOPTION_IDW):
			if pLoser.getDomainType() == (2 or 3):
				IDW.IDW().checkCulture(pWinner, pLoser)
		## Influence Driven War ##
		
## Raider Start ##

		pPlayer = gc.getPlayer(pWinner.getOwner())

		if pWinner.isHasPromotion(gc.getInfoTypeForString('PROMOTION_RAIDER')):

			pPlayer = gc.getPlayer(pWinner.getOwner())

			iGold = playerY.getGold( )
			message = 0

			iGoldStolen = ( iGold//25 )

			if playerY.getGold( ) >= 500:
				playerY.changeGold( -20 )
			elif playerY.getGold( ) >= 25:
				playerY.changeGold( -iGoldStolen )
			elif (playerY.getGold( ) >= 1) and (playerY.getGold( ) < 25):
				playerY.changeGold( -1 )

			iGold2 = playerX.getGold( )
			if playerY.getGold( ) >= 500:
				playerX.changeGold( +20 )
				message = 1
			elif playerY.getGold( ) >= 25:
				playerX.changeGold( +iGoldStolen )
				message = 2
			else:
				playerX.changeGold( +1 )
				message = 3

			pPID = pPlayer.getID()
			iX = pWinner.getX()
			iY = pWinner.getY()
			szName = pPlayer.getName()

			## This only controls the text, all actual gold amounts are done above:
			iGoldStolenMax = ( 500//25 )
			iGoldStolenMin = ( 25//25 )

			if ( message == 1 ):
				CyInterface().addMessage(pPID,false,15,CyTranslator().getText("TXT_KEY_RAIDER_GOLD1",(szName,iGoldStolenMax)),'',0,',Art/Interface/Buttons/TechTree/Banking.dds,Art/Interface/Buttons/TechTree_Atlas.dds,8,1',ColorTypes(44), iX, iY, True,True)
				### message: %s1 has plundered %d2 [ICON_GOLD]!###
			if ( message == 2 ):
				CyInterface().addMessage(pPID,false,15,CyTranslator().getText("TXT_KEY_RAIDER_GOLD2",(szName,iGoldStolen)),'',0,',Art/Interface/Buttons/TechTree/Banking.dds,Art/Interface/Buttons/TechTree_Atlas.dds,8,1',ColorTypes(44), iX, iY, True,True)
				### message: %s1 has plundered %d2 [ICON_GOLD]!###
			if ( message == 3 ):
				CyInterface().addMessage(pPID,false,15,CyTranslator().getText("TXT_KEY_RAIDER_GOLD3",(szName,iGoldStolenMin)),'',0,',Art/Interface/Buttons/TechTree/Banking.dds,Art/Interface/Buttons/TechTree_Atlas.dds,8,1',ColorTypes(44), iX, iY, True,True)
				### message: %s1 has plundered %d2 [ICON_GOLD]!###

## Raider End ##

		if (not self.__LOG_COMBAT):
			return
		if playerX and playerX and unitX and playerY:
			CvUtil.pyPrint('Player %d Civilization %s Unit %s has defeated Player %d Civilization %s Unit %s' 
				%(playerX.getID(), playerX.getCivilizationName(), unitX.getDescription(), 
				playerY.getID(), playerY.getCivilizationName(), unitY.getDescription()))

	def onCombatLogCalc(self, argsList):
		'Combat Result'	
		genericArgs = argsList[0][0]
		cdAttacker = genericArgs[0]
		cdDefender = genericArgs[1]
		iCombatOdds = genericArgs[2]
		CvUtil.combatMessageBuilder(cdAttacker, cdDefender, iCombatOdds)
		
	def onCombatLogHit(self, argsList):
		'Combat Message'
		global gCombatMessages, gCombatLog
		genericArgs = argsList[0][0]
		cdAttacker = genericArgs[0]
		cdDefender = genericArgs[1]
		iIsAttacker = genericArgs[2]
		iDamage = genericArgs[3]
		
		if cdDefender.eOwner == cdDefender.eVisualOwner:
			szDefenderName = gc.getPlayer(cdDefender.eOwner).getNameKey()
		else:
			szDefenderName = localText.getText("TXT_KEY_TRAIT_PLAYER_UNKNOWN", ())
		if cdAttacker.eOwner == cdAttacker.eVisualOwner:
			szAttackerName = gc.getPlayer(cdAttacker.eOwner).getNameKey()
		else:
			szAttackerName = localText.getText("TXT_KEY_TRAIT_PLAYER_UNKNOWN", ())

		if (iIsAttacker == 0):				
			combatMessage = localText.getText("TXT_KEY_COMBAT_MESSAGE_HIT", (szDefenderName, cdDefender.sUnitName, iDamage, cdDefender.iCurrHitPoints, cdDefender.iMaxHitPoints))
			CyInterface().addCombatMessage(cdAttacker.eOwner,combatMessage)
			CyInterface().addCombatMessage(cdDefender.eOwner,combatMessage)
			if (cdDefender.iCurrHitPoints <= 0):
				combatMessage = localText.getText("TXT_KEY_COMBAT_MESSAGE_DEFEATED", (szAttackerName, cdAttacker.sUnitName, szDefenderName, cdDefender.sUnitName))
				CyInterface().addCombatMessage(cdAttacker.eOwner,combatMessage)
				CyInterface().addCombatMessage(cdDefender.eOwner,combatMessage)
		elif (iIsAttacker == 1):
			combatMessage = localText.getText("TXT_KEY_COMBAT_MESSAGE_HIT", (szAttackerName, cdAttacker.sUnitName, iDamage, cdAttacker.iCurrHitPoints, cdAttacker.iMaxHitPoints))
			CyInterface().addCombatMessage(cdAttacker.eOwner,combatMessage)
			CyInterface().addCombatMessage(cdDefender.eOwner,combatMessage)
			if (cdAttacker.iCurrHitPoints <= 0):
				combatMessage = localText.getText("TXT_KEY_COMBAT_MESSAGE_DEFEATED", (szDefenderName, cdDefender.sUnitName, szAttackerName, cdAttacker.sUnitName))
				CyInterface().addCombatMessage(cdAttacker.eOwner,combatMessage)
				CyInterface().addCombatMessage(cdDefender.eOwner,combatMessage)

	def onImprovementBuilt(self, argsList):
	## Platy Builder ##
		if CyGame().GetWorldBuilderMode() and not CvPlatyBuilderScreen.bPython: return
	## Platy Builder ##
		'Improvement Built'
		iImprovement, iX, iY = argsList
		pPlot = gc.getMap().plot(iX, iY)
		if pPlot.getFeatureType() == gc.getInfoTypeForString("FEATURE_CANAL"):
			if not iImprovement == con.iImpCanalChina:
				pPlot.setFeatureType(FeatureTypes.NO_FEATURE, -1)
		if pPlot.getFeatureType() == gc.getInfoTypeForString("FEATURE_CANAL_FLOOD"):
			if not iImprovement == con.iImpCanalChina:
				pPlot.setFeatureType(gc.getInfoTypeForString("FEATURE_FLOOD_PLAINS"), -1)
		if iImprovement == con.iImpCanalChina:
			if pPlot.getFeatureType() == gc.getInfoTypeForString("FEATURE_FLOOD_PLAINS"):
				pPlot.setFeatureType(gc.getInfoTypeForString("FEATURE_CANAL_FLOOD"), -1)
			else:
				pPlot.setFeatureType(gc.getInfoTypeForString("FEATURE_CANAL"), -1)
		if (not self.__LOG_IMPROVEMENT):
			return
		CvUtil.pyPrint('Improvement %s was built at %d, %d'
			%(PyInfo.ImprovementInfo(iImprovement).getDescription(), iX, iY))

	def onImprovementDestroyed(self, argsList):
	## Platy Builder ##
		if CyGame().GetWorldBuilderMode() and not CvPlatyBuilderScreen.bPython: return
	## Platy Builder ##
		'Improvement Destroyed'
		iImprovement, iOwner, iX, iY = argsList
		if iImprovement == con.iImpCanalChina:
			pPlot = gc.getMap().plot(iX, iY)
			pPlot.setFeatureType(FeatureTypes.NO_FEATURE, -1)
		if (not self.__LOG_IMPROVEMENT):
			return
		CvUtil.pyPrint('Improvement %s was Destroyed at %d, %d'
			%(PyInfo.ImprovementInfo(iImprovement).getDescription(), iX, iY))

	def onRouteBuilt(self, argsList):
	## Platy Builder ##
		if CyGame().GetWorldBuilderMode() and not CvPlatyBuilderScreen.bPython: return
	## Platy Builder ##
		'Route Built'
		iRoute, iX, iY = argsList
		if (not self.__LOG_IMPROVEMENT):
			return
		CvUtil.pyPrint('Route %s was built at %d, %d'
			%(gc.getRouteInfo(iRoute).getDescription(), iX, iY))

	def onPlotRevealed(self, argsList):
	## Platy Builder ##
		if CyGame().GetWorldBuilderMode() and not CvPlatyBuilderScreen.bPython: return
	## Platy Builder ##
		'Plot Revealed'
		pPlot = argsList[0]
		iTeam = argsList[1]

	def onPlotFeatureRemoved(self, argsList):
		'Plot Revealed'
		pPlot = argsList[0]
		iFeatureType = argsList[1]
		pCity = argsList[2] # This can be null

	def onPlotPicked(self, argsList):
		'Plot Picked'
		pPlot = argsList[0]
		CvUtil.pyPrint('Plot was picked at %d, %d'
			%(pPlot.getX(), pPlot.getY()))

	def onNukeExplosion(self, argsList):
		'Nuke Explosion'
		pPlot, pNukeUnit = argsList
		CvUtil.pyPrint('Nuke detonated at %d, %d'
			%(pPlot.getX(), pPlot.getY()))

	def onGotoPlotSet(self, argsList):
		'Nuke Explosion'
		pPlot, iPlayer = argsList

	def onBuildingBuilt(self, argsList):
		'Building Completed'
		pCity, iBuildingType = argsList
		game = gc.getGame()

		if iBuildingType  == gc.getInfoTypeForString("BUILDING_FEDERAL_PARLIAMENT"):
			if not (gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_DYNAMIC_CIV_FLAGS) and gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_DYNAMIC_CIV_NAMES)):
				DynamicCivNaming.DynamicCivNaming().nameStart(pCity.getOwner())

		if iBuildingType  == gc.getInfoTypeForString("BUILDING_EIFFEL_TOWER"):
			iTeamX = gc.getPlayer(pCity.getOwner()).getTeam()
			pTeamX = gc.getTeam(iTeamX)
			for iPlayer in range(gc.getMAX_PLAYERS()):
				player = gc.getPlayer(iPlayer)
				if not player.isNone():
					if player.isAlive():
						iTeam = player.getTeam()
						if pTeamX.isHasMet(iTeam):
							player.AI_setAttitudeExtra(pCity.getOwner(),2)
							
		if iBuildingType  == gc.getInfoTypeForString("BUILDING_COLOSSEUM1"):
			pPlayer = gc.getPlayer(pCity.getOwner())
			iSlaveCount = (pPlayer.getSlaveAfrican() + pPlayer.getSlaveAsian() + pPlayer.getSlaveEuropean() + pPlayer.getSlaveIndian() + pPlayer.getSlaveMid()	+ pPlayer.getSlaveNewworld() + pPlayer.getSlaveSlavic()) / 10 + 1
			for iGladiator in range(iSlaveCount):
				pPlayer.initUnit(gc.getInfoTypeForString("UNIT_GLADIATOR"), pCity.getX(), pCity.getY(), UnitAITypes.NO_UNITAI, DirectionTypes.NO_DIRECTION)
			szMessage = localText.getText("TXT_KEY_GLADIATOR_MESSAGE", ("", ))
			CyInterface().addMessage(pCity.getOwner(), true, gc.getEVENT_MESSAGE_TIME(), szMessage, "", InterfaceMessageTypes.MESSAGE_TYPE_INFO, gc.getUnitInfo(gc.getInfoTypeForString("UNIT_GLADIATOR")).getButton(), gc.getInfoTypeForString("COLOR_GREEN"), pCity.getX(), pCity.getY(), true, true)
		
		if ((not gc.getGame().isNetworkMultiPlayer()) and (pCity.getOwner() == gc.getGame().getActivePlayer()) and (isWorldWonderClass(gc.getBuildingInfo(iBuildingType).getBuildingClassType()) or isNationalWonderClass(gc.getBuildingInfo(iBuildingType).getBuildingClassType()))):
							
		## Platy Builder ##
			if not CyGame().GetWorldBuilderMode():
		## Platy Builder ##
			# If this is a wonder...
				popupInfo = CyPopupInfo()
				popupInfo.setButtonPopupType(ButtonPopupTypes.BUTTONPOPUP_PYTHON_SCREEN)
				popupInfo.setData1(iBuildingType)
				popupInfo.setData2(pCity.getID())
				popupInfo.setData3(0)
				popupInfo.setText(u"showWonderMovie")
				popupInfo.addPopup(pCity.getOwner())

		CvAdvisorUtils.buildingBuiltFeats(pCity, iBuildingType)

		if (not self.__LOG_BUILDING):
			return
		CvUtil.pyPrint('%s was finished by Player %d Civilization %s' 
			%(PyInfo.BuildingInfo(iBuildingType).getDescription(), pCity.getOwner(), gc.getPlayer(pCity.getOwner()).getCivilizationDescription(0)))
	
	def onProjectBuilt(self, argsList):
		'Project Completed'
		pCity, iProjectType = argsList
		game = gc.getGame()
		if ((not gc.getGame().isNetworkMultiPlayer()) and (pCity.getOwner() == gc.getGame().getActivePlayer())):
	## Platy Builder ##
			if not CyGame().GetWorldBuilderMode():
	## Platy Builder ##
				popupInfo = CyPopupInfo()
				popupInfo.setButtonPopupType(ButtonPopupTypes.BUTTONPOPUP_PYTHON_SCREEN)
				popupInfo.setData1(iProjectType)
				popupInfo.setData2(pCity.getID())
				popupInfo.setData3(2)
				popupInfo.setText(u"showWonderMovie")
				popupInfo.addPopup(pCity.getOwner())
				
	def onSelectionGroupPushMission(self, argsList):
		'selection group mission'
		eOwner = argsList[0]
		eMission = argsList[1]
		iNumUnits = argsList[2]
		listUnitIds = argsList[3]
		
		if (not self.__LOG_PUSH_MISSION):
			return
		if pHeadUnit:
			CvUtil.pyPrint("Selection Group pushed mission %d" %(eMission))
	
	def onUnitMove(self, argsList):
		'unit move'
		pPlot,pUnit,pOldPlot = argsList
		player = PyPlayer(pUnit.getOwner())
		unitInfo = PyInfo.UnitInfo(pUnit.getUnitType())
		if (pUnit.getCargo() > 0) and (pUnit.getDomainType() == 0):
			if pPlot.isOwned():
				iTeam = pPlot.getTeam()
				pTeam = gc.getTeam(iTeam)
				pPlayer = gc.getPlayer(pUnit.getOwner())
				iActivePlayerTeam = pPlayer.getTeam() 
				if (pTeam.isAtWar(iActivePlayerTeam)):
					if pOldPlot.isOwned():
						iTeamX = pOldPlot.getTeam()
						if iTeamX != iTeam:
							for i in xrange(pPlot.getNumUnits()):
								pCargoUnit = pPlot.getUnit(i)
								if pCargoUnit.isCargo():
									pTransport = pCargoUnit.getTransportUnit()
									if pTransport.getID() == pUnit.getID():
										if pCargoUnit.getDomainType() == gc.getInfoTypeForString('DOMAIN_LAND'):
											if not pCargoUnit.isAmphib():
												if (pCargoUnit.canMove() or pCargoUnit.getMoves() != 0):
													pCargoUnit.finishMoves()
					else:
						for i in xrange(pPlot.getNumUnits()):
							pCargoUnit = pPlot.getUnit(i)
							if pCargoUnit.isCargo():
								pTransport = pCargoUnit.getTransportUnit()
								if pTransport.getID() == pUnit.getID():
									if pCargoUnit.getDomainType() == gc.getInfoTypeForString('DOMAIN_LAND'):
										if not pCargoUnit.isAmphib():
											if (pCargoUnit.canMove() or pCargoUnit.getMoves() != 0):
												pCargoUnit.finishMoves()
		if (not self.__LOG_MOVEMENT):
			return
		if player and unitInfo:
			CvUtil.pyPrint('Player %d Civilization %s unit %s is moving to %d, %d' 
				%(player.getID(), player.getCivilizationName(), unitInfo.getDescription(), 
				pUnit.getX(), pUnit.getY()))

	def onUnitSetXY(self, argsList):
		'units xy coords set manually'
		pPlot,pUnit = argsList
		player = PyPlayer(pUnit.getOwner())
		unitInfo = PyInfo.UnitInfo(pUnit.getUnitType())
		if (not self.__LOG_MOVEMENT):
			return
		
	def onUnitCreated(self, argsList):
	## Platy Builder ##
		if CyGame().GetWorldBuilderMode() and not CvPlatyBuilderScreen.bPython: return
	## Platy Builder ##
		'Unit Completed'
		unit = argsList[0]
		player = PyPlayer(unit.getOwner())
		if (not self.__LOG_UNITBUILD):
			return

	def onUnitBuilt(self, argsList):
		'Unit Completed'
		city = argsList[0]
		unit = argsList[1]
		player = PyPlayer(city.getOwner())

		if player.isBarbarian():
	## RI Barbarian buildings - START ##
			iCombat = unit.getUnitCombatType()
	## Barbarian building 1 - Famous Weaponsmith - Start ##
			if city.getNumActiveBuilding(gc.getInfoTypeForString("BUILDING_BARBARIAN_BUILDING_1")) == true:
				if iCombat == gc.getInfoTypeForString("UNITCOMBAT_MELEE") or iCombat == gc.getInfoTypeForString("UNITCOMBAT_RECON") or iCombat == gc.getInfoTypeForString("UNITCOMBAT_ARCHER"):
					unit.setBaseCombatStr(unit.baseCombatStr() +1)
	## Barbarian building 1 - Famous Weaponsmith - End ##
	## Barbarian building 2 - Quality Horse Breeding - Start ##
			if city.getNumActiveBuilding(gc.getInfoTypeForString("BUILDING_BARBARIAN_BUILDING_2")) == true:
				if iCombat == gc.getInfoTypeForString("UNITCOMBAT_MOUNTED") or iCombat ==  gc.getInfoTypeForString("UNITCOMBAT_MOUNTED_LIGHT"):
					unit.setBaseCombatStr(unit.baseCombatStr() +1)
	## Barbarian building 2 - Quality Horse Breeding - End ##
	## RI Barbarian buildings - END ##

		CvAdvisorUtils.unitBuiltFeats(city, unit)
		
		if (not self.__LOG_UNITBUILD):
			return
		CvUtil.pyPrint('%s was finished by Player %d Civilization %s' 
			%(PyInfo.UnitInfo(unit.getUnitType()).getDescription(), player.getID(), player.getCivilizationName()))
	
	def onUnitKilled(self, argsList):
		'Unit Killed'
		unit, iAttacker = argsList
		player = PyPlayer(unit.getOwner())
		attacker = PyPlayer(iAttacker)
		if (not self.__LOG_UNITKILLED):
			return
		CvUtil.pyPrint('Player %d Civilization %s Unit %s was killed by Player %d' 
			%(player.getID(), player.getCivilizationName(), PyInfo.UnitInfo(unit.getUnitType()).getDescription(), attacker.getID()))

	def onUnitLost(self, argsList):
	## Platy Builder ##
		if CyGame().GetWorldBuilderMode() and not CvPlatyBuilderScreen.bPython: return
	## Platy Builder ##
		'Unit Lost'
		unit = argsList[0]
		player = PyPlayer(unit.getOwner())
		if (not self.__LOG_UNITLOST):
			return
		CvUtil.pyPrint('%s was lost by Player %d Civilization %s' 
			%(PyInfo.UnitInfo(unit.getUnitType()).getDescription(), player.getID(), player.getCivilizationName()))
	
	def onUnitPromoted(self, argsList):
		'Unit Promoted'
		pUnit, iPromotion = argsList
		player = PyPlayer(pUnit.getOwner())
		if (not self.__LOG_UNITPROMOTED):
			return
		CvUtil.pyPrint('Unit Promotion Event: %s - %s' %(player.getCivilizationName(), pUnit.getName(),))
	
	def onUnitSelected(self, argsList):
		'Unit Selected'
		unit = argsList[0]
		player = PyPlayer(unit.getOwner())
		if (not self.__LOG_UNITSELECTED):
			return
		CvUtil.pyPrint('%s was selected by Player %d Civilization %s' 
			%(PyInfo.UnitInfo(unit.getUnitType()).getDescription(), player.getID(), player.getCivilizationName()))
	
	def onUnitRename(self, argsList):
		'Unit is renamed'
		pUnit = argsList[0]
		if (pUnit.getOwner() == gc.getGame().getActivePlayer()):
			self.__eventEditUnitNameBegin(pUnit)
	
	def onUnitPillage(self, argsList):
		'Unit pillages a plot'
		pUnit, iImprovement, iRoute, iOwner = argsList
		iPlotX = pUnit.getX()
		iPlotY = pUnit.getY()
		pPlot = CyMap().plot(iPlotX, iPlotY)
###road destroyed message
		if iRoute!=-1:
						if pPlot.getOwner()>-1:
								if gc.getPlayer(pPlot.getOwner()).isHuman():                        
										sUnit = gc.getUnitInfo(pUnit.getUnitType ()).getDescription ()
										sRoute = gc.getRouteInfo(iRoute).getDescription ()
										sAdj = gc.getPlayer(pUnit.getOwner()).getCivilizationAdjective (0)
										if pPlot.getOwner()==pUnit.getOwner():
												iColour = gc.getInfoTypeForString("COLOR_GREEN")
										else:
												iColour = gc.getInfoTypeForString("COLOR_RED")
										sText = CyTranslator().getText("TXT_KEY_MISC_YOU_IMP_WAS_DESTROYED",(sRoute,sUnit,sAdj))
										CyInterface().addMessage (pPlot.getOwner(), True, 30, sText,"Assets\\Sounds\\Pillage.wav", 0, "", iColour, pUnit.getX(), pUnit.getY(), False,False)

						if gc.getPlayer(pUnit.getOwner()).isHuman():
								sUnit = gc.getUnitInfo(pUnit.getUnitType ()).getDescription ()
								sRoute = gc.getRouteInfo(iRoute).getDescription ()
								iColour = gc.getInfoTypeForString("COLOR_GREEN")
								sText = CyTranslator().getText("TXT_KEY_MISC_YOU_UNIT_DESTROYED_IMP",(sUnit,sRoute))
								CyInterface().addMessage (pUnit.getOwner(), True, 30, sText,"Assets\\Sounds\\Pillage.wav", 0, "", iColour, pUnit.getX(), pUnit.getY(), False,False)                                
###road destroyed message		
		
		if (not self.__LOG_UNITPILLAGE):
			return
		CvUtil.pyPrint("Player %d's %s pillaged improvement %d and route %d at plot at (%d, %d)" 
			%(iOwner, PyInfo.UnitInfo(pUnit.getUnitType()).getDescription(), iImprovement, iRoute, iPlotX, iPlotY))
	
	def onUnitSpreadReligionAttempt(self, argsList):
		'Unit tries to spread religion to a city'
		pUnit, iReligion, bSuccess = argsList
		
		iX = pUnit.getX()
		iY = pUnit.getY()
		pPlot = CyMap().plot(iX, iY)
		pCity = pPlot.getPlotCity()
	
	def onUnitGifted(self, argsList):
		'Unit is gifted from one player to another'
		pUnit, iGiftingPlayer, pPlotLocation = argsList
	
	def onUnitBuildImprovement(self, argsList):
		'Unit begins enacting a Build (building an Improvement or Route)'
		pUnit, iBuild, bFinished = argsList

	def onGoodyReceived(self, argsList):
		'Goody received'
		iPlayer, pPlot, pUnit, iGoodyType = argsList
		if (not self.__LOG_GOODYRECEIVED):
			return
		CvUtil.pyPrint('%s received a goody' %(gc.getPlayer(iPlayer).getCivilizationDescription(0)),)
	
	def onGreatPersonBorn(self, argsList):
	## Platy Builder ##
		if CyGame().GetWorldBuilderMode() and not CvPlatyBuilderScreen.bPython: return
	## Platy Builder ##
		'Unit Promoted'
		pUnit, iPlayer, pCity = argsList
		player = PyPlayer(iPlayer)
		if pUnit.isNone() or pCity.isNone():
			return
		if (not self.__LOG_GREATPERSON):
			return
		CvUtil.pyPrint('A %s was born for %s in %s' %(pUnit.getName(), player.getCivilizationName(), pCity.getName()))
	
	def onTechAcquired(self, argsList):
	## Platy Builder ##
		if CyGame().GetWorldBuilderMode() and not CvPlatyBuilderScreen.bPython: return
	## Platy Builder ##
		'Tech Acquired'
		iTechType, iTeam, iPlayer, bAnnounce = argsList
		# Note that iPlayer may be NULL (-1) and not a refer to a player object
		
		# Show tech splash when applicable
		if (iPlayer > -1 and bAnnounce and not CyInterface().noTechSplash()):
			if (gc.getGame().isFinalInitialized() and not gc.getGame().GetWorldBuilderMode()):
				if ((not gc.getGame().isNetworkMultiPlayer()) and (iPlayer == gc.getGame().getActivePlayer())):
					popupInfo = CyPopupInfo()
					popupInfo.setButtonPopupType(ButtonPopupTypes.BUTTONPOPUP_PYTHON_SCREEN)
					popupInfo.setData1(iTechType)
					popupInfo.setText(u"showTechSplash")
					popupInfo.addPopup(iPlayer)
				
		if (not self.__LOG_TECH):
			return
		CvUtil.pyPrint('%s was finished by Team %d' 
			%(PyInfo.TechnologyInfo(iTechType).getDescription(), iTeam))
	
	def onTechSelected(self, argsList):
		'Tech Selected'
		iTechType, iPlayer = argsList
		if (not self.__LOG_TECH):
			return
		CvUtil.pyPrint('%s was selected by Player %d' %(PyInfo.TechnologyInfo(iTechType).getDescription(), iPlayer))
	
	def onReligionFounded(self, argsList):
	## Platy Builder ##
		if CyGame().GetWorldBuilderMode() and not CvPlatyBuilderScreen.bPython: return
	## Platy Builder ##
		'Religion Founded'
		iReligion, iFounder = argsList
		player = PyPlayer(iFounder)
		
		iCityId = gc.getGame().getHolyCity(iReligion).getID()
		if (gc.getGame().isFinalInitialized() and not gc.getGame().GetWorldBuilderMode()):
			if ((not gc.getGame().isNetworkMultiPlayer()) and (iFounder == gc.getGame().getActivePlayer())):
				popupInfo = CyPopupInfo()
				popupInfo.setButtonPopupType(ButtonPopupTypes.BUTTONPOPUP_PYTHON_SCREEN)
				popupInfo.setData1(iReligion)
				popupInfo.setData2(iCityId)
				popupInfo.setData3(1)
				popupInfo.setText(u"showWonderMovie")
				popupInfo.addPopup(iFounder)
		
		# Religion shock ##
		if not gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_RELIGION_SHOCK):
			iLatestDate = float(CyGame().getEstimateEndTurn()) / 5
			iShockDistanceModifier = gc.getGame().getGameTurn() * 100 / iLatestDate
			if iShockDistanceModifier > 100: 
				iShockDistanceModifier = 100
			iShockMaxDistance = CyMap().getGridWidth() / 10
			iShockDistance = iShockMaxDistance * iShockDistanceModifier / 100
			print ("Religion shock: shock distance " + str(iShockDistance))
			
			for iPlayerX in xrange(gc.getMAX_CIV_PLAYERS()):
				pPlayerX = gc.getPlayer(iPlayerX)
				if pPlayerX.isAlive():
					(loopCity, iter) = pPlayerX.firstCity(False)
					while(loopCity):
						rangeX = abs(loopCity.getX() - gc.getGame().getHolyCity(iReligion).getX())
						rangeY = abs(loopCity.getY() - gc.getGame().getHolyCity(iReligion).getY())
						if rangeX == 0 and rangeY == 0:
							return
						if rangeX**2 + rangeY**2 < iShockDistance**2 * 2:
							print ("X range = " + str(rangeX))
							print ("Y range = " + str(rangeY))
							print (str(rangeX**2 + rangeY**2) + " should be less than " + str(iShockDistance**2 * 2))
							for iReligionX in xrange(gc.getNumReligionInfos()):
								if loopCity.isHasReligion(iReligionX) and not loopCity.isHolyCityByType(iReligionX):
									loopCity.setHasReligion(iReligionX,false,true,true)
									if pPlayerX.isHuman():
										CityName = loopCity.getName()
										ReligionName = gc.getReligionInfo(iReligion).getDescription()
										CyInterface().addMessage(iPlayerX,False,15,CyTranslator().getText("TXT_KEY_RELIGION_SHOCK",(CityName,ReligionName)),'',0,'Art/Interface/Buttons/General/warning_popup.dds',ColorTypes(gc.getInfoTypeForString("COLOR_RED")), loopCity.getX(), loopCity.getY(), True,True)
									print ("Religion shock: religion purged from " + loopCity.getName())
						(loopCity, iter) = pPlayerX.nextCity(iter, False)
		
		if (not self.__LOG_RELIGION):
			return
		CvUtil.pyPrint('Player %d Civilization %s has founded %s'
			%(iFounder, player.getCivilizationName(), gc.getReligionInfo(iReligion).getDescription()))
			

	def onReligionSpread(self, argsList):
	## Platy Builder ##
		if CyGame().GetWorldBuilderMode() and not CvPlatyBuilderScreen.bPython: return
	## Platy Builder ##
		'Religion Has Spread to a City'
		iReligion, iOwner, pSpreadCity = argsList
		player = PyPlayer(iOwner)
		if (not self.__LOG_RELIGIONSPREAD):
			return
		CvUtil.pyPrint('%s has spread to Player %d Civilization %s city of %s'
			%(gc.getReligionInfo(iReligion).getDescription(), iOwner, player.getCivilizationName(), pSpreadCity.getName()))

	def onReligionRemove(self, argsList):
	## Platy Builder ##
		if CyGame().GetWorldBuilderMode() and not CvPlatyBuilderScreen.bPython: return
	## Platy Builder ##
		'Religion Has been removed from a City'
		iReligion, iOwner, pRemoveCity = argsList
		player = PyPlayer(iOwner)
		if (not self.__LOG_RELIGIONSPREAD):
			return
		CvUtil.pyPrint('%s has been removed from Player %d Civilization %s city of %s'
			%(gc.getReligionInfo(iReligion).getDescription(), iOwner, player.getCivilizationName(), pRemoveCity.getName()))
				
	def onCorporationFounded(self, argsList):
	## Platy Builder ##
		if CyGame().GetWorldBuilderMode() and not CvPlatyBuilderScreen.bPython: return
	## Platy Builder ##
		'Corporation Founded'
		iCorporation, iFounder = argsList
		player = PyPlayer(iFounder)
		
		if (not self.__LOG_RELIGION):
			return
		CvUtil.pyPrint('Player %d Civilization %s has founded %s'
			%(iFounder, player.getCivilizationName(), gc.getCorporationInfo(iCorporation).getDescription()))

	def onCorporationSpread(self, argsList):
	## Platy Builder ##
		if CyGame().GetWorldBuilderMode() and not CvPlatyBuilderScreen.bPython: return
	## Platy Builder ##
		'Corporation Has Spread to a City'
		iCorporation, iOwner, pSpreadCity = argsList
		player = PyPlayer(iOwner)
		if (not self.__LOG_RELIGIONSPREAD):
			return
		CvUtil.pyPrint('%s has spread to Player %d Civilization %s city of %s'
			%(gc.getCorporationInfo(iCorporation).getDescription(), iOwner, player.getCivilizationName(), pSpreadCity.getName()))

	def onCorporationRemove(self, argsList):
	## Platy Builder ##
		if CyGame().GetWorldBuilderMode() and not CvPlatyBuilderScreen.bPython: return
	## Platy Builder ##
		'Corporation Has been removed from a City'
		iCorporation, iOwner, pRemoveCity = argsList
		player = PyPlayer(iOwner)
		if (not self.__LOG_RELIGIONSPREAD):
			return
		CvUtil.pyPrint('%s has been removed from Player %d Civilization %s city of %s'
			%(gc.getReligionInfo(iReligion).getDescription(), iOwner, player.getCivilizationName(), pRemoveCity.getName()))
				
	def onGoldenAge(self, argsList):
	## Platy Builder ##
		if CyGame().GetWorldBuilderMode() and not CvPlatyBuilderScreen.bPython: return
	## Platy Builder ##
		'Golden Age'
		iPlayer = argsList[0]
		player = PyPlayer(iPlayer)
		if (not self.__LOG_GOLDENAGE):
			return
		CvUtil.pyPrint('Player %d Civilization %s has begun a golden age'
			%(iPlayer, player.getCivilizationName()))

	def onEndGoldenAge(self, argsList):
	## Platy Builder ##
		if CyGame().GetWorldBuilderMode() and not CvPlatyBuilderScreen.bPython: return
	## Platy Builder ##
		'End Golden Age'
		iPlayer = argsList[0]
		player = PyPlayer(iPlayer)
		if (not self.__LOG_ENDGOLDENAGE):
			return
		CvUtil.pyPrint('Player %d Civilization %s golden age has ended'
			%(iPlayer, player.getCivilizationName()))

	def onChangeWar(self, argsList):
	## Platy Builder ##
		if CyGame().GetWorldBuilderMode() and not CvPlatyBuilderScreen.bPython: return
	## Platy Builder ##
		'War Status Changes'
		bIsWar = argsList[0]
		iTeam = argsList[1]
		iRivalTeam = argsList[2]
		if (not self.__LOG_WARPEACE):
			return
		if (bIsWar):
			strStatus = "declared war"
		else:
			strStatus = "declared peace"
		CvUtil.pyPrint('Team %d has %s on Team %d'
			%(iTeam, strStatus, iRivalTeam))
	
	def onChat(self, argsList):
		'Chat Message Event'
		chatMessage = "%s" %(argsList[0],)
		
	def onSetPlayerAlive(self, argsList):
		'Set Player Alive Event'
		iPlayerID = argsList[0]
		bNewValue = argsList[1]
		CvUtil.pyPrint("Player %d's alive status set to: %d" %(iPlayerID, int(bNewValue)))
		
	def onPlayerChangeStateReligion(self, argsList):
	## Platy Builder ##
		if CyGame().GetWorldBuilderMode() and not CvPlatyBuilderScreen.bPython: return
	## Platy Builder ##
		'Player changes his state religion'
		iPlayer, iNewReligion, iOldReligion = argsList
		
	def onPlayerGoldTrade(self, argsList):
		'Player Trades gold to another player'
		iFromPlayer, iToPlayer, iGoldAmount = argsList
		
	def onCityBuilt(self, argsList):
		'City Built'
		city = argsList[0]
		if (city.getOwner() == gc.getGame().getActivePlayer()):
			self.__eventEditCityNameBegin(city, False)	
		CvUtil.pyPrint('City Built Event: %s' %(city.getName()))
		
	def onCityRazed(self, argsList):
		'City Razed'
		city, iPlayer = argsList
		iOwner = city.findHighestCulture()
#### messages - wonder destroyed start ####
		pCity = city		
		NumWonders = pCity.getNumWorldWonders
		if NumWonders ()>0:
						Counter = 0
						for i in range(gc.getNumBuildingInfos ()):
								thisbuilding = gc.getBuildingInfo (i)
								if pCity.getNumBuilding(i)>0:
										iBuildingClass = thisbuilding.getBuildingClassType ()
										thisbuildingclass = gc.getBuildingClassInfo (iBuildingClass)
										if thisbuildingclass.getMaxGlobalInstances ()==1:
												ConquerPlayer = gc.getPlayer(city.getOwner())
												iConquerTeam = ConquerPlayer.getTeam()
												ConquerName = ConquerPlayer.getName ()
												WonderName = thisbuilding.getDescription ()
												iX = pCity.getX()
												iY = pCity.getY()
												for iAllPlayer in range (gc.getMAX_CIV_PLAYERS ()):
														ThisPlayer = gc.getPlayer(iAllPlayer)
														iThisTeam = ThisPlayer.getTeam()
														ThisTeam = gc.getTeam(iThisTeam)
														if ThisTeam.isHasMet(iConquerTeam):
																if iAllPlayer == city.getOwner():
																		CyInterface().addMessage(iAllPlayer,False,15,CyTranslator().getText("TXT_KEY_YOU_DESTROYED_WONDER",(ConquerName,WonderName)),'',0,'Art/Interface/Buttons/General/warning_popup.dds',ColorTypes(gc.getInfoTypeForString("COLOR_RED")), iX, iY, True,True)
																else:
																		CyInterface().addMessage(iAllPlayer,False,15,CyTranslator().getText("TXT_KEY_DESTROYED_WONDER",(ConquerName,WonderName)),'',0,'Art/Interface/Buttons/General/warning_popup.dds',ColorTypes(gc.getInfoTypeForString("COLOR_RED")), iX, iY, True,True)
#### messages - wonder destroyed end ####		
		
		# Partisans!
		# f1rpo (bugfix by SmokeyTheBear): Missing parentheses after getPopulation
		if city.getPopulation() > 1 and iOwner != -1 and iPlayer != -1:
			owner = gc.getPlayer(iOwner)
			player = gc.getPlayer(iPlayer)
			if not owner.isBarbarian() and owner.getNumCities() > 0 and player.getNumCities() > 0:
				if gc.getTeam(owner.getTeam()).isAtWar(gc.getPlayer(iPlayer).getTeam()):
					if gc.getNumEventTriggerInfos() > 0: # prevents mods that don't have events from getting an error
						iEvent = CvUtil.findInfoTypeNum(gc.getEventTriggerInfo, gc.getNumEventTriggerInfos(),'EVENTTRIGGER_PARTISANS')
						if iEvent != -1 and gc.getGame().isEventActive(iEvent) and owner.getEventTriggerWeight(iEvent) < 0:
							triggerData = owner.initTriggeredData(iEvent, true, -1, city.getX(), city.getY(), iPlayer, city.getID(), -1, -1, -1, -1)
			
		CvUtil.pyPrint("City Razed Event: %s" %(city.getName(),))
	
	def onCityAcquired(self, argsList):
	## Platy Builder ##
		if CyGame().GetWorldBuilderMode() and not CvPlatyBuilderScreen.bPython: return
	## Platy Builder ##
		'City Acquired'
		iPreviousOwner,iNewOwner,pCity,bConquest,bTrade = argsList
		CvUtil.pyPrint('City Acquired Event: %s' %(pCity.getName()))

		## Dynamic City Naming ##
		if not gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_CITY_NAMING):
			DynamicCityNaming.renameVarietyCity(pCity, iNewOwner)
		## Dynamic City Naming ##
		#### messages - wonder captured start ####		
		NumWonders = pCity.getNumWorldWonders
		if NumWonders > 0:
			for i in range(gc.getNumBuildingInfos ()):
				thisbuilding = gc.getBuildingInfo (i)
				if pCity.getNumBuilding(i) > 0:
					iBuildingClass = thisbuilding.getBuildingClassType ()
					thisbuildingclass = gc.getBuildingClassInfo (iBuildingClass)
					if thisbuildingclass.getMaxGlobalInstances () == 1:
						ConquerPlayer = gc.getPlayer(pCity.getOwner())
						iConquerTeam = ConquerPlayer.getTeam()
						ConquerName = ConquerPlayer.getName ()
						WonderName = thisbuilding.getDescription ()
						iX = pCity.getX()
						iY = pCity.getY()
						for iPlayer in range (gc.getMAX_CIV_PLAYERS ()):
							ThisPlayer = gc.getPlayer(iPlayer)
							iThisTeam = ThisPlayer.getTeam()
							ThisTeam = gc.getTeam(iThisTeam)
							if ThisTeam.isHasMet(iConquerTeam):
								if iPlayer == pCity.getOwner():
									CyInterface().addMessage(iPlayer,False,15,CyTranslator().getText("TXT_KEY_YOU_CAPTURED_WONDER",(ConquerName,WonderName)),'',0,'Art/Interface/Buttons/General/happy_person.dds',ColorTypes(gc.getInfoTypeForString("COLOR_GREEN")), iX, iY, True,True)
								else:
									CyInterface().addMessage(iPlayer,False,15,CyTranslator().getText("TXT_KEY_CAPTURED_WONDER",(ConquerName,WonderName)),'',0,'Art/Interface/Buttons/General/warning_popup.dds',ColorTypes(gc.getInfoTypeForString("COLOR_RED")), iX, iY, True,True)
		#### messages - wonder captured end ####

		#### destroy any buildings that aren't supposed to change hands ####
		for j in range(gc.getNumBuildingInfos ()):
			thisbuilding = gc.getBuildingInfo (j)
			if pCity.getNumBuilding(j) > 0:
				if thisbuilding.isNeverCapture():
					pCity.setNumRealBuilding(j,0)
				elif (thisbuilding.getConquestProbability() == 0):
					pCity.setNumRealBuilding(j,0)

		if ((not gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_REVOLUTIONS)) or (gc.getPlayer(pCity.getOwner()).isBarbarian())):
			pCity.setNumRealBuilding(gc.getInfoTypeForString("BUILDING_SUBJUGATION"), 1)

	def onCityAcquiredAndKept(self, argsList):
		'City Acquired and Kept'
		'Called when city conquered and not razed'
		'Not called when city traded or liberated'
		iOwner,pCity = argsList

		CvUtil.pyPrint('City Acquired and Kept Event: %s' %(pCity.getName()))
	
	def onCityLost(self, argsList):
		'City Lost'
		city = argsList[0]
		player = PyPlayer(city.getOwner())
		if (not self.__LOG_CITYLOST):
			return
		CvUtil.pyPrint('City %s was lost by Player %d Civilization %s' 
			%(city.getName(), player.getID(), player.getCivilizationName()))
	
	def onCultureExpansion(self, argsList):
	## Platy Builder ##
		if CyGame().GetWorldBuilderMode() and not CvPlatyBuilderScreen.bPython: return
	## Platy Builder ##
		'City Culture Expansion'
		pCity = argsList[0]
		iPlayer = argsList[1]
		CvUtil.pyPrint("City %s's culture has expanded" %(pCity.getName(),))
	
	def onCityGrowth(self, argsList):
		'City Population Growth'
		pCity = argsList[0]
		iPlayer = argsList[1]
		CvUtil.pyPrint("%s has grown" %(pCity.getName(),))
	
	def onCityDoTurn(self, argsList):
		'City Production'
		pCity = argsList[0]
		iPlayer = argsList[1]

		CvAdvisorUtils.cityAdvise(pCity, iPlayer)
	
	def onCityBuildingUnit(self, argsList):
		'City begins building a unit'
		pCity = argsList[0]
		iUnitType = argsList[1]
		if (not self.__LOG_CITYBUILDING):
			return
		CvUtil.pyPrint("%s has begun building a %s" %(pCity.getName(),gc.getUnitInfo(iUnitType).getDescription()))
	
	def onCityBuildingBuilding(self, argsList):
		'City begins building a Building'
		pCity = argsList[0]
		iBuildingType = argsList[1]
		if (not self.__LOG_CITYBUILDING):
			return
		CvUtil.pyPrint("%s has begun building a %s" %(pCity.getName(),gc.getBuildingInfo(iBuildingType).getDescription()))
	
	def onCityRename(self, argsList):
		'City is renamed'
		pCity = argsList[0]
		if (pCity.getOwner() == gc.getGame().getActivePlayer()):
			self.__eventEditCityNameBegin(pCity, True)	
	
	def onCityHurry(self, argsList):
		'City is renamed'
		pCity = argsList[0]
		iHurryType = argsList[1]

	def onVictory(self, argsList):
		'Victory'
		iTeam, iVictory = argsList
		if (iVictory >= 0 and iVictory < gc.getNumVictoryInfos()):
			victoryInfo = gc.getVictoryInfo(int(iVictory))
			CvUtil.pyPrint("Victory!  Team %d achieves a %s victory"
				%(iTeam, victoryInfo.getDescription()))
	
	def onVassalState(self, argsList):
		'Vassal State'
		iMaster, iVassal, bVassal = argsList

		DynamicCivNaming.DynamicCivNaming().nameStart(iVassal)

		if (bVassal):
			CvUtil.pyPrint("Team %d becomes a Vassal State of Team %d"
				%(iVassal, iMaster))
		else:
			CvUtil.pyPrint("Team %d revolts and is no longer a Vassal State of Team %d"
				%(iVassal, iMaster))
	
	def onPlayerRevolution(self, argsList):
		ePlayer, iAnarchyTurns, leOldCivics, leNewCivics = argsList
	
	def onGameUpdate(self, argsList):
		'sample generic event, called on each game turn slice'
		genericArgs = argsList[0][0]	# tuple of tuple of my args
		turnSlice = genericArgs[0]
	
	def onMouseEvent(self, argsList):
		'mouse handler - returns 1 if the event was consumed'
		eventType,mx,my,px,py,interfaceConsumed,screens = argsList
		if ( px!=-1 and py!=-1 ):
			if ( eventType == self.EventLButtonDown ):
				if (self.bAllowCheats and self.bCtrl and self.bAlt and CyMap().plot(px,py).isCity() and not interfaceConsumed):
					# Launch Edit City Event
					self.beginEvent( CvUtil.EventEditCity, (px,py) )
					return 1
				
				elif (self.bAllowCheats and self.bCtrl and self.bShift and not interfaceConsumed):
					# Launch Place Object Event
					self.beginEvent( CvUtil.EventPlaceObject, (px, py) )
					return 1
			
		if ( eventType == self.EventBack ):
			return CvScreensInterface.handleBack(screens)
		elif ( eventType == self.EventForward ):
			return CvScreensInterface.handleForward(screens)
		
		return 0
		

#################### TRIGGERED EVENTS ##################	
				
	def __eventEditCityNameBegin(self, city, bRename):
		popup = PyPopup.PyPopup(CvUtil.EventEditCityName, EventContextTypes.EVENTCONTEXT_ALL)
		popup.setUserData((city.getID(), bRename, CyGame().getActivePlayer()))
		popup.setHeaderString(localText.getText("TXT_KEY_NAME_CITY", ()))
		popup.setBodyString(localText.getText("TXT_KEY_SETTLE_NEW_CITY_NAME", ()))
		## Dynamic City Naming ##
		name = city.getName()
		#name = name.decode('utf-8')
		popup.createEditBox(name)
		## Dynamic City Naming ##
		popup.setEditBoxMaxCharCount( 25 )
		popup.launch()
	
	def __eventEditCityNameApply(self, playerID, userData, popupReturn):	
		city = gc.getPlayer(userData[2]).getCity(userData[0])
		cityName = popupReturn.getEditBoxString(0)
		## Dynamic City Naming ##
		# removes the new name from the leader-specific city name lists
		if not gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_CITY_NAMING):
			DynamicCityNaming.removeNewCityName(cityName)
		## Dynamic City Naming ##
		city.setName(cityName, not userData[1])
## Platy Builder ##
		if CyGame().GetWorldBuilderMode() and not CyGame().isInAdvancedStart():
			WBCityEditScreen.WBCityEditScreen().placeStats()
## Platy Builder ##

	def __eventPlaceObjectBegin(self, argsList):
		'Place Object Event'
		CvDebugTools.CvDebugTools().initUnitPicker(argsList)
	
	def __eventPlaceObjectApply(self, playerID, userData, popupReturn):
		'Place Object Event Apply'
		if (getChtLvl() > 0):
			CvDebugTools.CvDebugTools().applyUnitPicker( (popupReturn, userData) )

	def __eventAwardTechsAndGoldBegin(self, argsList):
		'Award Techs & Gold Event'
		CvDebugTools.CvDebugTools().cheatTechs()
	
	def __eventAwardTechsAndGoldApply(self, playerID, netUserData, popupReturn):
		'Award Techs & Gold Event Apply'
		if (getChtLvl() > 0):
			CvDebugTools.CvDebugTools().applyTechCheat( (popupReturn) )
	
	def __eventShowWonderBegin(self, argsList):
		'Show Wonder Event'
		CvDebugTools.CvDebugTools().wonderMovie()
	
	def __eventShowWonderApply(self, playerID, netUserData, popupReturn):
		'Wonder Movie Apply'
		if (getChtLvl() > 0):
			CvDebugTools.CvDebugTools().applyWonderMovie( (popupReturn) )
	
	def __eventEditUnitNameBegin(self, argsList):
		pUnit = argsList
		popup = PyPopup.PyPopup(CvUtil.EventEditUnitName, EventContextTypes.EVENTCONTEXT_ALL)
		popup.setUserData((pUnit.getID(),))
		popup.setBodyString(localText.getText("TXT_KEY_RENAME_UNIT", ()))
		popup.createEditBox(pUnit.getNameNoDesc())
		popup.launch()

	def __eventEditUnitNameApply(self, playerID, userData, popupReturn):	
		'Edit Unit Name Event'
		iUnitID = userData[0]
		unit = gc.getPlayer(playerID).getUnit(iUnitID)
		newName = popupReturn.getEditBoxString(0)
		if (len(newName) > 25):
			newName = newName[:25]			
		unit.setName(newName)
## Platy Builder ##
		if CyGame().GetWorldBuilderMode():
			WBUnitScreen.WBUnitScreen(CvPlatyBuilderScreen.CvWorldBuilderScreen()).placeStats()
			WBUnitScreen.WBUnitScreen(CvPlatyBuilderScreen.CvWorldBuilderScreen()).placeCurrentUnit()	

	def __eventWBPlayerScriptPopupApply(self, playerID, userData, popupReturn):
		sScript = popupReturn.getEditBoxString(0)
		gc.getPlayer(userData[0]).setScriptData(CvUtil.convertToStr(sScript))
		WBPlayerScreen.WBPlayerScreen().placeScript()
		return

	def __eventWBCityScriptPopupApply(self, playerID, userData, popupReturn):
		sScript = popupReturn.getEditBoxString(0)
		pCity = gc.getPlayer(userData[0]).getCity(userData[1])
		pCity.setScriptData(CvUtil.convertToStr(sScript))
		WBCityEditScreen.WBCityEditScreen().placeScript()
		return

	def __eventWBUnitScriptPopupApply(self, playerID, userData, popupReturn):
		sScript = popupReturn.getEditBoxString(0)
		pUnit = gc.getPlayer(userData[0]).getUnit(userData[1])
		pUnit.setScriptData(CvUtil.convertToStr(sScript))
		WBUnitScreen.WBUnitScreen(CvPlatyBuilderScreen.CvWorldBuilderScreen()).placeScript()
		return

	def __eventWBScriptPopupBegin(self):
		return

	def __eventWBGameScriptPopupApply(self, playerID, userData, popupReturn):
		sScript = popupReturn.getEditBoxString(0)
		CyGame().setScriptData(CvUtil.convertToStr(sScript))
		WBGameDataScreen.WBGameDataScreen(CvPlatyBuilderScreen.CvWorldBuilderScreen()).placeScript()
		return

	def __eventWBPlotScriptPopupApply(self, playerID, userData, popupReturn):
		sScript = popupReturn.getEditBoxString(0)
		pPlot = CyMap().plot(userData[0], userData[1])
		pPlot.setScriptData(CvUtil.convertToStr(sScript))
		WBPlotScreen.WBPlotScreen().placeScript()
		return

	def __eventWBLandmarkPopupApply(self, playerID, userData, popupReturn):
		sScript = popupReturn.getEditBoxString(0)
		pPlot = CyMap().plot(userData[0], userData[1])
		iPlayer = userData[2]
		if userData[3] > -1:
			pSign = CyEngine().getSignByIndex(userData[3])
			iPlayer = pSign.getPlayerType()
			CyEngine().removeSign(pPlot, iPlayer)
		if len(sScript):
			if iPlayer == gc.getBARBARIAN_PLAYER():
				CyEngine().addLandmark(pPlot, CvUtil.convertToStr(sScript))
			else:
				CyEngine().addSign(pPlot, iPlayer, CvUtil.convertToStr(sScript))
		WBPlotScreen.iCounter = 10
		return