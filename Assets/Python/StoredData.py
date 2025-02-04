# Stored Data for Realism Invictus, implemented by AbsintheRed
# Based on the StoredData mechanics in the RFC-style mods
# We can easily store all kinds of various data here which is preserved between saves/loads of a given game

from CvPythonExtensions import *
import CvUtil
import pickle

# globals
gc = CyGlobalContext()


class StoredData:

	def __init__(self):
		self.setup()

	def load(self):
		'Loads and unpickles script data'
		#self.scriptDict = pickle.loads(gc.getGame().getScriptData())
		self.scriptDict.update(pickle.loads(gc.getPlayer(0).getScriptData()))
		print("AR scriptdata on load", self.scriptDict)

	def save(self):
		'Pickles and saves script data'
		# gc.getGame().setScriptData(pickle.dumps(self.scriptDict))
		gc.getPlayer(0).setScriptData(pickle.dumps(self.scriptDict))
		print("AR scriptdata on save", self.scriptDict)

	def setup(self):
		'Initialise the global script data dictionary for usage'
		self.scriptDict = {
			'lUsedCityNames': [],
                }

	# All read/write functions go here so that pickling is only done on load & preSave

	# from DynamicCityNaming.py
	def setUsedCityNames(self, lNewList):
		self.scriptDict['lUsedCityNames'] = lNewList

	def getUsedCityNames(self):
		return self.scriptDict['lUsedCityNames']


# All modules import the following single instance, not the class
sd = StoredData()
