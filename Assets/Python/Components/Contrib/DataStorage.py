## Sid Meier's Civilization 4
## Copyright Firaxis Games 2005
##
## DataStorage by Bhruic
##
## Based on sdToolKit by Stone-D (Laga Mahesa)
## Copyright Laga Mahesa 2005

from CvPythonExtensions import *
import CvUtil
import sys
import cPickle as pickle
	
gc = CyGlobalContext()

class DataStorage:
	def __init__(self):
		self.dataTable = {}
		self.modTable = {}
		self.modID = ''
		self.pEntity = None
		
	def initStorage(self, ModID, pEntity):
		try:
			self.dataTable = pickle.loads(pEntity.getScriptData())
		except:
			self.dataTable = {}
		if (not self.dataTable.has_key(ModID)):
			self.dataTable[ModID] = pickle.dumps({})
			pEntity.setScriptData(pickle.dumps(self.dataTable))
		self.pEntity = pEntity
		self.modID = ModID
		
	def loadModData(self, ModID, pEntity):
		if (self.modID != ModID) or (self.pEntity != pEntity):
			self.initStorage(ModID, pEntity)
			self.modTable = pickle.loads(self.dataTable[ModID])

	def saveModData(self, ModID, pEntity):
		self.dataTable = pickle.loads(pEntity.getScriptData())
		self.dataTable[ModID] = pickle.dumps(self.modTable)
		pEntity.setScriptData(pickle.dumps(self.dataTable))

	def delKey(self, ModID, key, pEntity):
		self.loadModData(ModID, pEntity)
		if (self.modTable.has_key(key)):
			del self.modTable[key]
			saveModData(ModID, pEntity)

	def hasKey(self, ModID, key, pEntity):
		self.loadModData(ModID, pEntity)
		return self.modTable.has_key(key)
		
	def getModTable(self, ModID, pEntity):
		self.loadModData(ModID, pEntity)
		return self.modTable
		
	def getKeyTable(self, ModID, key, pEntity):
		if (self.modTable.hasKey(ModID, key, pEntity)):
			return pickle.loads(self.modTable[key])
		else:
			return -1
	
	def getSingleVal(self, ModID, key, pEntity):
		if (self.hasKey(ModID, key, pEntity)):
			return self.modTable[key]
		else:
			return -1
	
	def setSingleVal(self, ModID, key, val, pEntity):
		self.loadModData(ModID, pEntity)
		self.modTable[key] = val
		self.saveModData(ModID, pEntity)

	def getVal(self, ModID, key, var, pEntity):
		if (self.hasKey(ModID, key, pEntity)):
			keyTable = pickle.loads(self.modTable[key])
			return keyTable[var]
		else:
			return -1
		
	def setVal(self, ModID, key, var, val, pEntity):
		if (not self.hasKey(ModID, key, pEntity)):
			self.modTable[key] = pickle.dumps({})
		keyTable = pickle.loads(self.modTable[key])
		keyTable[var] = val
		self.modTable[key] = pickle.dumps(keyTable)
		self.saveModData(ModID, pEntity)
