from CvPythonExtensions import *
import CvScreensInterface
import Popup as PyPopup
import CvUtil
import string

gc = CyGlobalContext()
localText = CyTranslator()
UserProfile = CyUserProfile()

#"""
#OPTIONS SCREEN CALLBACK INTERFACE - Any time something is changed in the Options Screen the result is determined here
#"""

def saveProfile():
	if (UserProfile.getProfileName() != ""):
		# MOD - START - Graphical Paging
		UserProfile.setGraphicOption(GraphicOptionTypes.GRAPHICOPTION_FROZEN_ANIMATIONS, gc.getGraphicOption(GraphicOptionTypes.GRAPHICOPTION_GRAPHICAL_PAGING))
		UserProfile.writeToFile(UserProfile.getProfileName())
		UserProfile.setGraphicOption(GraphicOptionTypes.GRAPHICOPTION_FROZEN_ANIMATIONS, False)
		# MOD - END - Graphical Paging
	
def getOptionsScreen():
	return CvScreensInterface.optionsScreen
	
def getTabControl():
	return getOptionsScreen().getTabControl()

def refresh():
	getOptionsScreen().refreshScreen()
	
def restartPopup(bForceShowing = false):
	
	if (CyInterface().isInMainMenu() == false or bForceShowing == true):
		
		# create popup
		popup = PyPopup.PyPopup()
		popup.setHeaderString("")
		popup.setBodyString(localText.getText("TXT_KEY_OPTIONS_NEED_TO_RESTART", ()))
		popup.launch()

def isNumber(s):
	
	for l in s:
		if l not in string.digits:
			return False
			
	return True

def DummyCallback( argsList ):
	"This is the callback function for controls which shouldn't do anything when modified (editboxes, mainly)"
	return
	
######################################## GAME OPTIONS ########################################
	
def handleGameOptionsClicked ( argsList ): 
	"Handles checkbox clicked input"
	bValue, szName = argsList
	
	iGameOption = int(szName[szName.find("_")+1:])
	CyMessageControl().sendPlayerOption(iGameOption, bValue)
	return 1
	
def handleLanguagesDropdownBoxInput ( argsList ):
	"Handles Languages Dropdown Box input"
	iValue, szName = argsList
	
	CyGame().setCurrentLanguage(iValue)
	
	popup = PyPopup.PyPopup()
	popup.setHeaderString("")
	popup.setBodyString(localText.getText("TXT_KEY_FEAT_ACCOMPLISHED_OK", ()))
	popup.launch()
	
	return 1
	
def handleGameReset ( argsList ):
	"Resets these options"
	szName = argsList
	
	UserProfile.resetOptions(TabGroupTypes.TABGROUP_GAME)
	refresh()
	saveProfile()
	
	return 1
	
######################################## GRAPHIC OPTIONS ########################################
	
def handleGraphicOptionsClicked ( argsList ): 
	"Handles checkbox clicked input"
	bValue, szName = argsList
	iGraphicOption = int(szName[szName.find("_")+1:])

	# MOD - START - Graphical Paging
	if (iGraphicOption == GraphicOptionTypes.GRAPHICOPTION_GRAPHICAL_PAGING):
		gc.setGraphicOptionGraphicalPaging(bValue)
		return 1
	# MOD - END - Graphical Paging
	
	UserProfile.setGraphicOption(iGraphicOption, bValue)
	
	if (iGraphicOption == GraphicOptionTypes.GRAPHICOPTION_SINGLE_UNIT_GRAPHICS or
		iGraphicOption == GraphicOptionTypes.GRAPHICOPTION_FULLSCREEN):
		restartPopup(true)
		
	if (iGraphicOption == GraphicOptionTypes.GRAPHICOPTION_HIRES_TERRAIN):
		restartPopup(false)
		
	return 1
	
def handleGraphicsLevelDropdownBoxInput ( argsList ):
	"Handles Graphics Level Dropdown Box input"
	iValue, szName = argsList
	
	UserProfile.setGraphicsLevel(iValue)
	refresh()
	
	restartPopup(true)
	return 1
	
def handleRenderQualityDropdownBoxInput ( argsList ):
	"Handles Render Quality Dropdown Box input"
	iValue, szName = argsList
	
	UserProfile.setRenderQualityLevel(iValue)
	
	return 1
	
def handleGlobeViewDropdownBoxInput ( argsList ):
	"Handles Globe View Dropdown Box input"
	iValue, szName = argsList
	
	UserProfile.setGlobeViewRenderLevel(iValue)
	
	return 1
		
def handleMovieDropdownBoxInput ( argsList ):
	"Handles Movie Dropdown Box input"
	iValue, szName = argsList
	
	UserProfile.setMovieQualityLevel(iValue)
	
	return 1
		
def handleMainMenuDropdownBoxInput ( argsList ):
	"Handles Main Menu Dropdown Box input"
	iValue, szName = argsList
	
	UserProfile.setMainMenu(iValue)
	refresh()
	saveProfile()
	
	return 1
		
def handleResolutionDropdownInput ( argsList ):
	"Handles Resolution Dropdown Box input"
	iValue, szName = argsList
	
	UserProfile.setResolution(iValue)
	# MOD - START - Graphical Paging
	resX, x, resY = UserProfile.getResolutionString(UserProfile.getResolution()).split(' ')
	gc.getGame().setResolution(int(resX), int(resY))
	# MOD - END - Graphical Paging
	
	return 1
	
def handleAntiAliasingDropdownInput ( argsList ):
	"Handles Anti-Aliasing Dropdown Box input"
	iValue, szName = argsList
	
	UserProfile.setAntiAliasing(iValue)
	return 1
	
def handleGraphicsReset ( argsList ):
	"Resets these options"
	szName = argsList
	
	UserProfile.resetOptions(TabGroupTypes.TABGROUP_GRAPHICS)
	refresh()
	restartPopup()
	saveProfile()
	
	return 1
	
######################################## AUDIO OPTIONS ########################################
	
def handleVolumeSlidersInput ( argsList ):
	"Handles Volume slider input"
	iValue, szName = argsList
	iVolumeType = int(szName[szName.find("_")+1:])
	
 	iMax = UserProfile.getVolumeStops()
	
	if (iVolumeType == 0):		# Master Volume
		UserProfile.setMasterVolume(iMax - iValue)
	elif (iVolumeType == 1):	# Music Volume
		UserProfile.setMusicVolume(iMax - iValue)
	elif (iVolumeType == 2):	# Sound Effects Volume
		UserProfile.setSoundEffectsVolume(iMax - iValue)
	elif (iVolumeType == 3):	# Speech Volume
		UserProfile.setSpeechVolume(iMax - iValue)
	elif (iVolumeType == 4):	# Ambience Volume
		UserProfile.setAmbienceVolume(iMax - iValue)
	elif (iVolumeType == 5):	# Interface Volume
		UserProfile.setInterfaceVolume(iMax - iValue)
	
	return 1
	
def handleVolumeCheckboxesInput ( argsList ): 
	"Handles checkbox clicked input"
	bValue, szName = argsList
	iVolumeType = int(szName[szName.find("_")+1:])
	
	if (iVolumeType == 0):		# Master Volume
		UserProfile.setMasterNoSound(bValue)
	elif (iVolumeType == 1):	# Music Volume
		UserProfile.setMusicNoSound(bValue)
	elif (iVolumeType == 2):	# Sound Effects Volume
		UserProfile.setSoundEffectsNoSound(bValue)
	elif (iVolumeType == 3):	# Speech Volume
		UserProfile.setSpeechNoSound(bValue)
	elif (iVolumeType == 4):	# Ambience Volume
		UserProfile.setAmbienceNoSound(bValue)
	elif (iVolumeType == 5):	# Interface Volume
		UserProfile.setInterfaceNoSound(bValue)
	
	return 1
	
def handleCustomMusicPathCheckboxInput ( argsList ): 
	"Handles Custom Music Path text changed input"
	bValue, szName = argsList
	
	if (bValue):
		UserProfile.setMusicPath(CvUtil.convertToStr(getOptionsScreen().getMusicPath()))
	else:
		UserProfile.setMusicPath("")
	
	return 1
	
def handleCustomMusicPathButtonInput ( argsList ):
	"Handles Custom Music Path Browse Button clicked input"
	szName = argsList
	
	UserProfile.musicPathDialogBox()
	return 1
	
def handleSpeakerConfigDropdownInput ( argsList ):
	"Handles Speaker Config Dropdown Box input"
	iValue, szName = argsList
	szSpeakerConfigName = UserProfile.getSpeakerConfigFromList(iValue)
	
	UserProfile.setSpeakerConfig(szSpeakerConfigName)
	restartPopup(true)
	
	return 1
	
def handleVoiceCheckboxInput ( argsList ): 
	"Handles voice checkbox clicked input"
	bValue, szName = argsList
	UserProfile.setUseVoice(bValue)
	return 1
	
def handleCaptureDeviceDropdownInput ( argsList ): 
	"Handles Capture Device Config Dropdown Box input"
	iValue, szName = argsList
	UserProfile.setCaptureDevice(iValue)
	return 1
	
def handleCaptureVolumeSliderInput ( argsList ):
	"Handles Capture Volume slider input"
	iValue, szName = argsList
 	iMax = UserProfile.getMaxPlaybackVolume()
	
	UserProfile.setCaptureVolume(iValue)
	return 1
	
def handlePlaybackDeviceDropdownInput ( argsList ): 
	"Handles Playback Device Dropdown Box input"
	iValue, szName = argsList
	UserProfile.setPlaybackDevice(iValue)
	return 1
	
def handlePlaybackVolumeSliderInput ( argsList ):
	"Handles Playback Volume slider input"
	iValue, szName = argsList
 	iMax = UserProfile.getMaxPlaybackVolume()
 
# 	UserProfile.setPlaybackVolume(iMax - iValue)
 	UserProfile.setPlaybackVolume(iValue)
	return 1
	
def handleAudioReset ( argsList ):
	"Resets these options"
	szName = argsList
	
	UserProfile.resetOptions(TabGroupTypes.TABGROUP_AUDIO)
	refresh()
	restartPopup(true)
	saveProfile()
	
	return 1
	
######################################## NETWORK OPTIONS ########################################

def handleBroadbandSelected ( argsList ):
	"Handles bandwidth selection"
	bSelected, szName = argsList
	if (bSelected):
		CyGame().setModem(false)
	return 1
	
def handleModemSelected ( argsList ):
	"Handles bandwidth selection"
	bSelected, szName = argsList
	if (bSelected):
		CyGame().setModem(true)
	return 1
	
######################################## CLOCK OPTIONS ########################################
	
def handleClockOnCheckboxInput ( argsList ):
	"Handles Clock On/Off checkbox clicked input"
	bValue, szName = argsList
	
	UserProfile.setClockOn(bValue)
	return 1
	
def handle24HourClockCheckboxInput ( argsList ):
	"Handles 24 Hour Clock On/Off checkbox clicked input"
	bValue, szName = argsList
	
	UserProfile.set24Hours(bValue)
	return 1
	
def handleAlarmOnCheckboxInput ( argsList ):
	"Handles Alarm On/Off checkbox clicked input"
	bValue, szName = argsList
	
	iHour = getOptionsScreen().getAlarmHour()
	iMin = getOptionsScreen().getAlarmMin()
	
	if (isNumber(iHour) and iHour != "" and isNumber(iMin) and iMin != "" ):
	    
	    iHour = int(iHour)
	    iMin = int(iMin)
	    
	    if (iHour > 0 or iMin > 0):
		    toggleAlarm(bValue, iHour, iMin)
	
	return 1
	
def handleOtherReset ( argsList ):
	"Resets these options"
	szName = argsList
	
	UserProfile.resetOptions(TabGroupTypes.TABGROUP_CLOCK)
	refresh()
	saveProfile()
	
	return 1
	
######################################## PROFILES ########################################
	
def handleProfilesDropdownInput ( argsList ):
	"Handles Profiles tab dropdown box input"
	
	iValue, szName = argsList
	saveProfile()
		
	# Load other file
	szFilename = UserProfile.getProfileFileName(iValue)
	szProfile = szFilename[szFilename.find("PROFILES\\")+9:-4]
	
	bSuccess = loadProfile(szProfile)
	return bSuccess
	
def handleNewProfileButtonInput ( argsList ):
	"Handles New Profile Button clicked input"
	szName = argsList
		
	szNewProfileName = getOptionsScreen().getProfileEditCtrlText()
	szNarrow = szNewProfileName.encode("latin_1")
	UserProfile.setProfileName(szNarrow)
	UserProfile.writeToFile(szNarrow)
	
	# Recalculate file info when new file is saved out
	UserProfile.loadProfileFileNames()
	
	saveProfile()
	
	refresh()
	
	# create popup
	popup = PyPopup.PyPopup()
	popup.setHeaderString("")
	popup.setBodyString(localText.getText("TXT_KEY_OPTIONS_SAVED_PROFILE", (szNewProfileName, )))
	popup.launch()
	
	return 1
	
def handleDeleteProfileButtonInput ( argsList ):
	"Handles Delete Profile Button clicked input"
	szName = argsList
	
	szProfileName =CvUtil.convertToStr(getOptionsScreen().getProfileEditCtrlText())
	
	if (UserProfile.deleteProfileFile(szProfileName)):    # Note that this function automatically checks to see if the string passed is a valid file to be deleted (it must have the proper file extension though)
		
		# Recalculate list of stuff
		UserProfile.loadProfileFileNames()
		
		# create popup
		popup = PyPopup.PyPopup()
		popup.setHeaderString("")
		popup.setBodyString(localText.getText("TXT_KEY_OPTIONS_DELETED_PROFILE", (szProfileName, )))
		popup.launch()
		
		bSuccess = true
		
		if (szProfileName == UserProfile.getProfileName()):
			
			UserProfile.setProfileName("")
				
			# Load other file
			szFilename = UserProfile.getProfileFileName(0)
			szProfile = szFilename[szFilename.find("PROFILES\\")+9:-4]
			
			bSuccess = loadProfile(szProfile)
		
		refresh()
		
		return bSuccess
		
	return 0
	
def loadProfile(szProfile):
	
	bReadSuccessful = UserProfile.readFromFile(szProfile)
	
	if (bReadSuccessful):
		UserProfile.recalculateAudioSettings()
		
		getOptionsScreen().setProfileEditCtrlText(szProfile)
		
		########### Now we have to update everything we loaded since nothing is done except the serialization on load ###########
		
		# Game Options
		for iOptionLoop in range(PlayerOptionTypes.NUM_PLAYEROPTION_TYPES):
			bValue = UserProfile.getPlayerOption(iOptionLoop)
			CyMessageControl().sendPlayerOption(iOptionLoop, bValue)
		
		# Graphics Options
		for iOptionLoop in range(GraphicOptionTypes.NUM_GRAPHICOPTION_TYPES):
			bValue = UserProfile.getGraphicOption(iOptionLoop)
			# MOD - START - Graphical Paging
			# MOD - START - No Frozen Animations
			if (iOptionLoop == GraphicOptionTypes.GRAPHICOPTION_FROZEN_ANIMATIONS):
				gc.setGraphicOptionGraphicalPaging(bValue)
				bValue = False
			# MOD - END - No Frozen Animations
			# MOD - END - Graphical Paging
			UserProfile.setGraphicOption(iOptionLoop, bValue)
			
		# Beware! These guys aren't safe to change:
		UserProfile.setAntiAliasing(UserProfile.getAntiAliasing())
		UserProfile.setResolution(UserProfile.getResolution())
		# MOD - START - Graphical Paging
		resX, x, resY = UserProfile.getResolutionString(UserProfile.getResolution()).split(' ')
		gc.getGame().setResolution(int(resX), int(resY))
		# MOD - END - Graphical Paging
		
		# Audio Options
		UserProfile.setSpeakerConfig(UserProfile.getSpeakerConfig())
		UserProfile.setMusicPath(UserProfile.getMusicPath())
		UserProfile.setUseVoice(UserProfile.useVoice())
		UserProfile.setCaptureDevice(UserProfile.getCaptureDeviceIndex())
		UserProfile.setPlaybackDevice(UserProfile.getPlaybackDeviceIndex())
		UserProfile.setCaptureVolume(UserProfile.getCaptureVolume())
		UserProfile.setPlaybackVolume(UserProfile.getPlaybackVolume())
		
		# Clock Options
		UserProfile.setClockOn(UserProfile.isClockOn())
		
		#################
		
		# create popup
		popup = PyPopup.PyPopup()
		popup.setHeaderString("")
		popup.setBodyString(localText.getText("TXT_KEY_OPTIONS_LOADED_PROFILE", (szProfile, )))
		popup.launch()
		
		# Refresh options screen with updated values
		refresh()
		
		return 1
		
	# Load failed
	else:
		
		# create popup
		popup = PyPopup.PyPopup()
		popup.setHeaderString("")
		popup.setBodyString(localText.getText("TXT_KEY_OPTIONS_LOAD_PROFILE_FAIL", ()))
		popup.launch()
		
		return 0
		
def handleExitButtonInput ( argsList ):
	"Exits the screen"
	szName = argsList
	
	saveProfile()
	getTabControl().destroy()
	
	return 1
