## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
import CvUtil

from CvPythonExtensions import *


# Class to decipher and make screen input easy to read...
class ScreenInput:

	# Init call...
	# MOD - START - Improved Screen Input
	#def __init__ (self, argsList):
	def __init__ (self, argsList, bKeydown):
	# MOD - END - Improved Screen Input
		self.eNotifyCode = argsList[0]
		self.iData = argsList[1]
		self.uiFlags = argsList[2]
		self.iItemID = argsList[3]
		self.ePythonFileEnum = argsList[4]
		self.szFunctionName = argsList[5]
		self.bShift = argsList[6]
		self.bCtrl = argsList[7]
		self.bAlt = argsList[8]
		self.iMouseX = argsList[9]
		self.iMouseY = argsList[10]
		self.iButtonType = argsList[11]
		self.iData1 = argsList[12]
		self.iData2 = argsList[13]
		self.bOption = argsList[14]
		# MOD - START - Improved Screen Input
		self.bKeydown = bKeydown
		# MOD - END - Improved Screen Input

	# NotifyCode
	def getNotifyCode (self):
		return self.eNotifyCode

	# Data
	def getData (self):
		return self.iData

	# Flags
	def getFlags (self):
		return self.uiFlags

	# Item ID
	def getID (self):
		return self.iItemID

	# Python File
	def getPythonFile (self):
		return self.ePythonFileEnum

	# Function Name...
	def getFunctionName (self):
		return self.szFunctionName

	# Shift Key Down
	def isShiftKeyDown (self):
		return self.bShift

	# Ctrl Key Down
	def isCtrlKeyDown (self):
		return self.bCtrl

	# Alt Key Down
	def isAltKeyDown (self):
		return self.bAlt

	# X location of the mouse cursor
	def getMouseX (self):
		return self.iMouseX

	# Y location of the mouse cursor
	def getMouseY (self):
		return self.iMouseY

	# WidgetType
	def getButtonType (self):
		return self.iButtonType

	# Widget Data 1
	def getData1 (self):
		return self.iData1

	# Widget Data 2
	def getData2 (self):
		return self.iData2

	# Widget Option
	def getOption (self):
		return self.bOption
	
	# MOD - START - Improved Screen Input
	def isKeydown (self):
		return self.bKeydown
	
	def getVisibleCharacter (self):
		key = int(self.iData)
		if (key >= int(InputTypes.KB_A) and key <= int(InputTypes.KB_Z)):
			letterOffset = key - int(InputTypes.KB_A)
			if (self.bShift):
				return unichr(65 + letterOffset)
			else:
				return unichr(97 + letterOffset)
		elif (key == int(InputTypes.KB_SPACE)):
			return u" "
		elif (key >= int(InputTypes.KB_0) and key <= int(InputTypes.KB_9)):
			numberOffset = key - int(InputTypes.KB_0)
			return unichr(48 + numberOffset)
		return u""
	
	def modifyString (self, szContent):
		character = self.getVisibleCharacter()
		if (len(character) > 0):
			if (szContent):
				return szContent + character
			else:
				return character
		
		key = int(self.iData)
		if (key == int(InputTypes.KB_BACKSPACE)):
			return szContent[:-1]
		
		return szContent
		
	# MOD - END - Improved Screen Input

