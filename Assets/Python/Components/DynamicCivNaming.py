import PyHelpers
from CvPythonExtensions import *
gc = CyGlobalContext()
PyPlayer = PyHelpers.PyPlayer
##################################################################################################
##					Conditions																	##
## Religions:	"RELIGION_HINDUISM":	Requires Specific Religion       						##
##		"RELIGION_NONE":	Requires No State Religion       									##
## Civics:	"CIVIC_DESPOTISM":	Requires Specific Civics										##
## Era: 	"ERA_MODERN":		Current Era >= Specific Era										##
## States:	"STATE_GOLDEN":		Golden Age														##
##		"STATE_ANARCHY":	Anarchy																##
##		"STATE_VASSAL":		Vassal States														##
##		"STATE_MINOR":		Minor Civ															##
## Cities:	"CITY_MAX_050":		At Most 50 Cities												##
##		"CITY_MIN_020":		At Least 20 Cities													##
##################################################################################################
##					Name Change																	##
## [Prefix, Desc, Suffix, Short, Adj						   									##
## Prefix will be added in front of Desc and Short				   								##
## Suffix will be added behind Desc and Short				     								##
## ["AAA", "", "", "", ""] will just add a Prefix of "AAA" without changing the rest			##
## ["", "BBB", "", "CCC", ""] will just change the Desc and Short without changing Adj			##
##################################################################################################
##################################################################################################
##					Input Keys																	##
## "KEY_DESC":	Uses the existing Description of the Civ			   							##
## "KEY_SHORT":	Uses the existing Short of the Civ		     									##
## "KEY_ADJ":	Uses the existing Adjective of the Civ											##
##################################################################################################

Dynamic ={
	## All other civilizations not listed below ##
	"MASTER_AMERICA":[		[[""],
							["TXT_KEY_CIV_AMERICA_VASSAL_DESC", "KEY_SHORT", "", "KEY_SHORT", "KEY_ADJ", "gen", "2"]],
				],
	"MASTER_AZTEC":[				[["ERA_INDUSTRIAL"],							
							["TXT_KEY_CIV_AMERICA_VASSAL_DESC", "KEY_SHORT", "", "KEY_SHORT", "KEY_ADJ", "gen", "2"]],
							
							[["ERA_RENAISSANCE"],
							["TXT_KEY_CIV_AZTEC_VASSAL_REN_DESC", "KEY_SHORT", "", "KEY_SHORT", "KEY_ADJ", "gen", "2"]],

							[[""],
							["TXT_KEY_CIV_AZTEC_VASSAL_DESC", "KEY_SHORT", "", "KEY_SHORT", "KEY_ADJ", "gen", "2"]],
				],
	"MASTER_CHINA":[				[["ERA_INDUSTRIAL"],							
							["KEY_ADJ", "TXT_KEY_CIV_CHINA_VASSAL_IND_DESC", "", "KEY_SHORT", "KEY_ADJ", "gen", "2"]],

							[[""],
							["TXT_KEY_CIV_CHINA_VASSAL_DESC", "KEY_SHORT", "", "KEY_SHORT", "KEY_ADJ", "gen", "2"]],
				],
	"MASTER_SOUTH_CHINA":[			[["ERA_INDUSTRIAL"],							
							["KEY_ADJ", "TXT_KEY_CIV_CHINA_VASSAL_IND_DESC", "", "KEY_SHORT", "KEY_ADJ", "gen", "2"]],

							[[""],
							["TXT_KEY_CIV_CHINA_VASSAL_DESC", "KEY_SHORT", "", "KEY_SHORT", "KEY_ADJ", "gen", "2"]],
				],
	"MASTER_ENGLAND":[				[["ERA_INDUSTRIAL"],							
							["TXT_KEY_CIV_ENGLAND_VASSAL_IND_DESC", "KEY_SHORT", "", "KEY_SHORT", "KEY_ADJ", "gen", "2"]],
							
							[["ERA_RENAISSANCE"],
							["TXT_KEY_CIV_ENGLAND_VASSAL_REN_DESC", "KEY_SHORT", "", "KEY_SHORT", "KEY_ADJ", "gen", "2"]],

							[[""],
							["TXT_KEY_CIV_ENGLAND_VASSAL_DESC", "KEY_SHORT", "", "KEY_SHORT", "KEY_ADJ", "gen", "2"]],
				],
	"MASTER_FRANCE":[				[["ERA_INDUSTRIAL"],							
							["TXT_KEY_CIV_FRANCE_VASSAL_IND_DESC", "KEY_SHORT", "", "KEY_SHORT", "KEY_ADJ", "gen", "2"]],
							
							[["ERA_RENAISSANCE"],
							["TXT_KEY_CIV_FRANCE_VASSAL_REN_DESC", "KEY_SHORT", "", "KEY_SHORT", "KEY_ADJ", "gen", "2"]],

							[[""],
							["TXT_KEY_CIV_FRANCE_VASSAL_DESC", "KEY_SHORT", "", "KEY_SHORT", "KEY_ADJ", "gen", "2"]],
				],
	"MASTER_GERMANY":[				[["ERA_INDUSTRIAL"],							
							["TXT_KEY_CIV_GERMANY_VASSAL_IND_DESC", "KEY_SHORT", "", "KEY_SHORT", "KEY_ADJ", "gen", "2"]],

							[[""],
							["TXT_KEY_CIV_GERMANY_VASSAL_DESC", "KEY_SHORT", "", "KEY_SHORT", "KEY_ADJ", "gen", "2"]],
				],
	"MASTER_JAPAN":[				[["ERA_INDUSTRIAL"],							
							["KEY_ADJ", "TXT_KEY_CIV_JAPAN_VASSAL_IND_DESC", "", "KEY_SHORT", "KEY_ADJ", "gen", "2"]],

							[[""],
							["TXT_KEY_CIV_CHINA_VASSAL_DESC", "KEY_SHORT", "", "KEY_SHORT", "KEY_ADJ", "gen", "2"]],
				],
	"MASTER_RUSSIA":[				[["ERA_INDUSTRIAL"],							
							["KEY_ADJ", "TXT_KEY_CIV_RUSSIA_VASSAL_IND_DESC", "", "KEY_SHORT", "KEY_ADJ", "gen", "2"]],
							
							[["ERA_RENAISSANCE"],
							["TXT_KEY_CIV_RUSSIA_VASSAL_REN_DESC", "KEY_SHORT", "", "KEY_SHORT", "KEY_ADJ", "gen", "2"]],

							[[""],
							["TXT_KEY_CIV_GENERIC_VASSAL_DESC", "KEY_SHORT", "", "KEY_SHORT", "KEY_ADJ", "gen", "2"]],
				],
	"MASTER_SPAIN":[				[["ERA_INDUSTRIAL"],							
							["TXT_KEY_CIV_AMERICA_VASSAL_DESC", "KEY_SHORT", "", "KEY_SHORT", "KEY_ADJ", "gen", "2"]],
							
							[["ERA_RENAISSANCE"],
							["TXT_KEY_CIV_SPAIN_VASSAL_REN_DESC", "KEY_SHORT", "", "KEY_SHORT", "KEY_ADJ", "gen", "2"]],

							[[""],
							["TXT_KEY_CIV_GENERIC_VASSAL_DESC", "KEY_SHORT", "", "KEY_SHORT", "KEY_ADJ", "gen", "2"]],
				],
	"MASTER_OTTOMAN":[				[["ERA_INDUSTRIAL"],							
							["TXT_KEY_CIV_OTTOMAN_VASSAL_IND_DESC", "KEY_SHORT", "", "KEY_SHORT", "KEY_ADJ", "gen", "2"]],
							
							[["ERA_RENAISSANCE"],
							["TXT_KEY_CIV_OTTOMAN_VASSAL_REN_DESC", "KEY_SHORT", "", "KEY_SHORT", "KEY_ADJ", "gen", "2"]],

							[[""],
							["TXT_KEY_CIV_OTTOMAN_VASSAL_DESC", "KEY_SHORT", "", "KEY_SHORT", "KEY_ADJ", "gen", "2"]],
				],
	"MASTER_OTHERS":[				[["ERA_INDUSTRIAL"],							
							["TXT_KEY_CIV_GENERIC_VASSAL_IND_DESC", "KEY_SHORT", "", "KEY_SHORT", "KEY_ADJ", "gen", "2"]],
							
							[["ERA_RENAISSANCE"],
							["TXT_KEY_CIV_GENERIC_VASSAL_REN_DESC", "KEY_SHORT", "", "KEY_SHORT", "KEY_ADJ", "gen", "2"]],

							[[""],
							["TXT_KEY_CIV_GENERIC_VASSAL_DESC", "KEY_SHORT", "", "KEY_SHORT", "KEY_ADJ", "gen", "2"]],
				],
	"CIVILIZATION_OTHERS":[				[["CIVIC_DESPOTISM"],							
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_TRIBAL_DESC", "", "", "", ""]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],

							[["CIVIC_AUTOCRACY"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_OLIGARCHY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC2_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_KINGDOM_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_THEOCRACY"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],

							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
				],

	## Specific Civilizations ##
	"CIVILIZATION_AMERICA": [				[["CIVIC_DESPOTISM"],
							["", "TXT_KEY_CIV_AMERICA_TRIBAL_DESC", "", "TXT_KEY_CIV_AMERICA_TRIBAL_SHORT", "TXT_KEY_CIV_AMERICA_TRIBAL_ADJ", "tri", "1"]],
													
							[["CIVIC_STATE_PROPERTY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "KEY_SHORT", "KEY_ADJ", "com", "1"]],

							[["CIVIC_AUTOCRACY"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "mon", "1"]],
							
							[["CIVIC_OLIGARCHY"],
							["", "TXT_KEY_CIV_AMERICA_OLIGO_DESC", "", "KEY_SHORT", "KEY_ADJ", "con", "1"]],
							
							[["CIVIC_REPUBLIC"],
							["", "TXT_KEY_CIV_AMERICA_REPUBLIC_DESC", "", "KEY_SHORT", "KEY_ADJ", "rep", "1"]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_KINGDOM_DESC", "KEY_SHORT", "KEY_ADJ", "mon", "1"]],
							
							[["CIVIC_THEOCRACY"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "the", "1"]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "TXT_KEY_CIV_AMERICA_DEMOCRACY_DESC", "", "KEY_SHORT", "KEY_ADJ", "fed", "1"]],
							
							[["CIVIC_DEMOCRACY"],
							["", "TXT_KEY_CIV_AMERICA_DEMOCRACY_DESC", "", "KEY_SHORT", "KEY_ADJ", "dem", "1"]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "KEY_SHORT", "KEY_ADJ", "mon", "1"]],
				],
				
	"CIVILIZATION_ARABIA": [				[["CIVIC_DESPOTISM"],
							["", "TXT_KEY_CIV_ARABIA_TRIBAL_DESC", "", "TXT_KEY_CIV_ARABIA_TRIBAL_SHORT", "TXT_KEY_CIV_ARABIA_TRIBAL_ADJ", "tri", "1"]],

							[["CIVIC_STATE_PROPERTY", "LEADER_BILQUIS"],
							["", "TXT_KEY_CIV_ARABIA_COM_YEM_DESC", "", "KEY_SHORT", "KEY_ADJ", "sye", "1"]],
														
							[["CIVIC_STATE_PROPERTY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "KEY_SHORT", "KEY_ADJ", "com", "1"]],
							
							[["CIVIC_AUTOCRACY", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_ARABIA_DES_REN_DESC", "", "KEY_SHORT", "KEY_ADJ", "rde", "1"]],
							
							[["CIVIC_AUTOCRACY"],
							["", "TXT_KEY_CIV_ARABIA_DES_DESC", "", "KEY_SHORT", "KEY_ADJ", "des", "1"]],

							[["CIVIC_OLIGARCHY", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_ARABIA_REP_REN_DESC", "", "KEY_SHORT", "KEY_ADJ", "rre", "1"]],

							[["CIVIC_OLIGARCHY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_ARABIA_OLIGO_DESC", "KEY_SHORT", "KEY_ADJ", "con", "1"]],

							[["CIVIC_REPUBLIC", "ERA_RENAISSANCE"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "ire", "1"]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "rep", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_RENAISSANCE", "LEADER_BILQUIS"],
							["", "TXT_KEY_CIV_ARABIA_MON_YEM_DESC", "", "KEY_SHORT", "KEY_ADJ", "yem", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_RENAISSANCE", "LEADER_IBN_SAUD"],
							["", "TXT_KEY_CIV_ARABIA_MON_NAJ_DESC", "", "KEY_SHORT", "KEY_ADJ", "naj", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_ARABIA_MON_REN_DESC", "", "KEY_SHORT", "KEY_ADJ", "rmo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "LEADER_BILQUIS"],
							["", "TXT_KEY_CIV_ARABIA_MON_HIM_DESC", "", "KEY_SHORT", "KEY_ADJ", "him", "1"]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "TXT_KEY_CIV_ARABIA_MON_DESC", "", "KEY_SHORT", "KEY_ADJ", "mon", "1"]],
							
							[["CIVIC_THEOCRACY"],
							["", "TXT_KEY_CIV_ARABIA_THEO_DESC", "", "KEY_SHORT", "KEY_ADJ", "the", "1"]],
														
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT" "LEADER_BILQUIS"],
							["", "TXT_KEY_CIV_ARABIA_FED_YEM_DESC", "", "KEY_SHORT", "KEY_ADJ", "sfe", "1"]],
														
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "TXT_KEY_CIV_ARABIA_FED_DESC", "", "KEY_SHORT", "KEY_ADJ", "fed", "1"]],
							
							[["CIVIC_DEMOCRACY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "dem", "1"]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "TXT_KEY_CIV_ARABIA_DIC_DESC", "", "KEY_SHORT", "KEY_ADJ", "dic", "1"]],
				],
				
	"CIVILIZATION_ARMENIA": [				[["CIVIC_DESPOTISM"],
							["", "TXT_KEY_CIV_ARMENIA_TRIBAL_DESC", "", "TXT_KEY_CIV_ARMENIA_TRIBAL_SHORT", "TXT_KEY_CIV_ARMENIA_TRIBAL_SHORT", "tri", "1"]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "TXT_KEY_CIV_ARMENIA_COM_DESC", "", "KEY_SHORT", "KEY_ADJ", "com", "1"]],

							[["CIVIC_AUTOCRACY"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "des", "1"]],

							[["CIVIC_OLIGARCHY", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_ARMENIA_REP_DESC", "", "KEY_SHORT", "KEY_ADJ", "mre", "1"]],
							
							[["CIVIC_OLIGARCHY"],
							["", "TXT_KEY_CIV_ARMENIA_REP_DESC", "", "KEY_SHORT", "KEY_ADJ", "con", "1"]],
							
							[["CIVIC_REPUBLIC", "ERA_RENAISSANCE"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "ire", "1"]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "rep", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_MEDIEVAL", "LEADER_THOROS_II"],
							["", "TXT_KEY_CIV_ARMENIA_CIL_DESC", "", "KEY_SHORT", "KEY_ADJ", "cil", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_MEDIEVAL"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_KINGDOM_DESC", "KEY_SHORT", "KEY_ADJ", "mmo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "LEADER_ARGISHTI"],
							["", "TXT_KEY_CIV_ARMENIA_URA_DESC", "", "KEY_SHORT", "KEY_ADJ", "mon", "1"]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_KINGDOM_DESC", "KEY_SHORT", "KEY_ADJ", "mon", "1"]],
							
							[["CIVIC_THEOCRACY"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "the", "1"]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "TXT_KEY_CIV_ARMENIA_FEDERAL_DESC", "", "KEY_SHORT", "KEY_ADJ", "fed", "1"]],
							
							[["CIVIC_DEMOCRACY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "dem", "1"]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "TXT_KEY_CIV_ARMENIA_DIC_DESC", "", "KEY_SHORT", "KEY_ADJ", "dic", "1"]],
				],
				
	"CIVILIZATION_AUSTRONESIA": [				[["CIVIC_DESPOTISM"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_TRIBAL_DESC", "", "", "tri", "1"]],

							[["CIVIC_STATE_PROPERTY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "KEY_SHORT", "KEY_ADJ", "com", "1"]],

							[["CIVIC_AUTOCRACY"],
							["", "TXT_KEY_CIV_AUSTRONESIA_MONARCH_DESC", "", "TXT_KEY_CIV_AUSTRONESIA_MONARCH_SHORT", "TXT_KEY_CIV_AUSTRONESIA_MONARCH_SHORT", "des", "1"]],
							
							[["CIVIC_OLIGARCHY"],
							["", "TXT_KEY_CIV_AUSTRONESIA_REPUBLIC_DESC", "", "TXT_KEY_CIV_AUSTRONESIA_REPUBLIC_SHORT", "TXT_KEY_CIV_AUSTRONESIA_REPUBLIC_ADJ", "rep", "1"]],
							
							[["CIVIC_REPUBLIC"],
							["", "TXT_KEY_CIV_AUSTRONESIA_REPUBLIC2_DESC", "", "TXT_KEY_CIV_AUSTRONESIA_REPUBLIC2_SHORT", "TXT_KEY_CIV_AUSTRONESIA_REPUBLIC2_ADJ", "man", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "LEADER_SULTAN_AGUNG", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_AUSTRONESIA_MAT_DESC", "", "TXT_KEY_CIV_AUSTRONESIA_MAT_ADJ", "TXT_KEY_CIV_AUSTRONESIA_MAT_ADJ", "mat", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "LEADER_PARAMESWARA"],
							["", "TXT_KEY_CIV_AUSTRONESIA_TEM_DESC", "", "TXT_KEY_CIV_AUSTRONESIA_TEM_SHORT", "TXT_KEY_CIV_AUSTRONESIA_TEM_ADJ", "tem", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "LEADER_ISKANDAR_MUDA"],
							["", "TXT_KEY_CIV_AUSTRONESIA_ACE_DESC", "", "TXT_KEY_CIV_AUSTRONESIA_ACE_SHORT", "TXT_KEY_CIV_AUSTRONESIA_ACE_ADJ", "ace", "1"]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "TXT_KEY_CIV_AUSTRONESIA_KINGDOM_DESC", "", "TXT_KEY_CIV_AUSTRONESIA_KINGDOM_ADJ", "TXT_KEY_CIV_AUSTRONESIA_KINGDOM_SHORT", "mon", "1"]],
							
							[["CIVIC_THEOCRACY"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "the", "1"]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT", "LEADER_PARAMESWARA"],
							["", "TXT_KEY_CIV_AUSTRONESIA_FED_MAL_DESC", "", "TXT_KEY_CIV_AUSTRONESIA_MAL_SHORT", "TXT_KEY_CIV_AUSTRONESIA_MAL_ADJ", "mal", "1"]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "TXT_KEY_CIV_AUSTRONESIA_FED_DESC", "", "TXT_KEY_CIV_AUSTRONESIA_DEM_SHORT", "TXT_KEY_CIV_AUSTRONESIA_DEM_ADJ", "fed", "1"]],
							
							[["CIVIC_DEMOCRACY", "LEADER_PARAMESWARA"],
							["", "TXT_KEY_CIV_AUSTRONESIA_DEM_MALAY_DESC", "", "TXT_KEY_CIV_AUSTRONESIA_MAL_SHORT", "TXT_KEY_CIV_AUSTRONESIA_MAL_ADJ", "mau", "1"]],
							
							[["CIVIC_DEMOCRACY"],
							["", "TXT_KEY_CIV_AUSTRONESIA_DEM_DESC", "", "TXT_KEY_CIV_AUSTRONESIA_DEM_SHORT", "TXT_KEY_CIV_AUSTRONESIA_DEM_ADJ", "dem", "1"]],
							
							[["CIVIC_DICTATORSHIP", "LEADER_PARAMESWARA"],
							["", "TXT_KEY_CIV_AUSTRONESIA_DIC_MAL_DESC", "", "TXT_KEY_CIV_AUSTRONESIA_MAL_SHORT", "TXT_KEY_CIV_AUSTRONESIA_MAL_ADJ", "mel", "1"]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "TXT_KEY_CIV_AUSTRONESIA_DIC_DESC", "", "TXT_KEY_CIV_AUSTRONESIA_DEM_SHORT", "TXT_KEY_CIV_AUSTRONESIA_DEM_ADJ", "dic", "1"]],
				],
				
	"CIVILIZATION_AZTEC":[				[["CIVIC_DESPOTISM"],							
							["", "TXT_KEY_CIV_AZTEC_TRIBAL_DESC", "", "TXT_KEY_CIV_AZTEC_TRIBAL_SHORT", "TXT_KEY_CIV_AZTEC_TRIBAL_ADJ", "tri", "1"]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "TXT_KEY_CIV_AZTEC_COM_DESC", "", "TXT_KEY_CIV_AZTEC_MOD_SHORT", "TXT_KEY_CIV_AZTEC_MOD_ADJ", "com", "1"]],

							[["CIVIC_AUTOCRACY", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_AZTEC_DIC_DESC", "", "TXT_KEY_CIV_AZTEC_MOD_SHORT", "TXT_KEY_CIV_AZTEC_MOD_SHORT", "dic", "1"]],

							[["CIVIC_AUTOCRACY"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "mon", "1"]],

							[["CIVIC_OLIGARCHY", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_AZTEC_REP_DESC", "", "KEY_SHORT", "KEY_ADJ", "rre", "1"]],
							
							[["CIVIC_OLIGARCHY"],
							["", "TXT_KEY_CIV_AZTEC_REP_DESC", "", "KEY_SHORT", "KEY_ADJ", "rep", "1"]],
							
							[["CIVIC_REPUBLIC", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_AZTEC_REP3_DESC", "", "KEY_SHORT", "KEY_ADJ", "ire", "1"]],
							
							[["CIVIC_REPUBLIC"],
							["", "TXT_KEY_CIV_AZTEC_REP2_DESC", "", "KEY_SHORT", "KEY_ADJ", "tla", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_AZTEC_MOD_ADJ", "TXT_KEY_CIV_GENERIC_KINGDOM_DESC", "TXT_KEY_CIV_AZTEC_MOD_SHORT", "TXT_KEY_CIV_AZTEC_MOD_SHORT", "rmo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "TXT_KEY_CIV_AZTEC_MON_DESC", "KEY_CAPITAL", "KEY_SHORT", "KEY_ADJ", "mon", "1"]],
							
							[["CIVIC_THEOCRACY"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "the", "1"]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "TXT_KEY_CIV_AZTEC_FED_DESC", "", "TXT_KEY_CIV_AZTEC_MOD_SHORT", "TXT_KEY_CIV_AZTEC_MOD_ADJ", "fed", "1"]],
							
							[["CIVIC_DEMOCRACY"],
							["", "TXT_KEY_CIV_AZTEC_DEM_DESC", "", "TXT_KEY_CIV_AZTEC_MOD_SHORT", "TXT_KEY_CIV_AZTEC_MOD_ADJ", "dem", "1"]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "TXT_KEY_CIV_AZTEC_MOD_SHORT", "TXT_KEY_CIV_AZTEC_MOD_ADJ", "fas", "1"]],
				],				
				
	"CIVILIZATION_BERBER":[				[["CIVIC_DESPOTISM"],							
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_TRIBAL_DESC", "", "", "tri", "1"]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "KEY_SHORT", "KEY_ADJ", "com", "1"]],
							
							[["CIVIC_AUTOCRACY", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_BERBER_DES_DESC", "", "KEY_SHORT", "KEY_ADJ", "the", "1"]],
							
							[["CIVIC_AUTOCRACY"],
							["", "TXT_KEY_CIV_BERBER_ANC_DES_DESC", "", "KEY_SHORT", "KEY_ADJ", "des", "1"]],
							
							[["CIVIC_OLIGARCHY", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_BERBER_OLI_DESC", "", "KEY_SHORT", "KEY_ADJ", "mre", "1"]],
							
							[["CIVIC_OLIGARCHY"],
							["", "TXT_KEY_CIV_BERBER_REP_DESC", "", "KEY_SHORT", "KEY_ADJ", "rep", "1"]],
							
							[["CIVIC_REPUBLIC"],
							["", "TXT_KEY_CIV_BERBER_RRE_DESC", "", "KEY_SHORT", "KEY_ADJ", "rre", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_INDUSTRIAL"],
							["", "TXT_KEY_CIV_BERBER_MOD_DESC", "", "TXT_KEY_CIV_BERBER_MOD_SHORT", "TXT_KEY_CIV_BERBER_MOD_ADJ", "dic", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_BERBER_MOD_DESC", "", "TXT_KEY_CIV_BERBER_MOD_SHORT", "TXT_KEY_CIV_BERBER_MOD_ADJ", "rmo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "TXT_KEY_CIV_BERBER_MON_DESC", "", "KEY_SHORT", "KEY_ADJ", "mon", "1"]],
							
							[["CIVIC_THEOCRACY"],
							["", "TXT_KEY_CIV_BERBER_THEO_DESC", "", "KEY_SHORT", "KEY_ADJ", "the", "1"]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "KEY_SHORT", "KEY_ADJ", "fed", "1"]],
							
							[["CIVIC_DEMOCRACY"],
							["", "TXT_KEY_CIV_BERBER_DEM_DESC", "", "KEY_SHORT", "KEY_ADJ", "dem", "1"]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "KEY_SHORT", "KEY_ADJ", "dic", "1"]],
				],
	
		"CIVILIZATION_CARTHAGE":[				[["CIVIC_DESPOTISM"],							
							["", "TXT_KEY_CIV_CARTHAGE_TRIBAL_DESC", "", "TXT_KEY_CIV_CARTHAGE_TRIBAL_SHORT", "TXT_KEY_CIV_CARTHAGE_TRIBAL_ADJ", "tri", "1"]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "TXT_KEY_CIV_CARTHAGE_MOD_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "TXT_KEY_CIV_CARTHAGE_MOD_SHORT", "TXT_KEY_CIV_CARTHAGE_MOD_ADJ", "com", "1"]],

							[["CIVIC_AUTOCRACY", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_CARTHAGE_MOD_DESC", "", "TXT_KEY_CIV_CARTHAGE_MOD_SHORT", "TXT_KEY_CIV_CARTHAGE_MOD_ADJ", "mdi", "1"]],
							
							[["CIVIC_AUTOCRACY"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "des", "1"]],

							[["CIVIC_OLIGARCHY", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_CARTHAGE_MOD_ADJ", "TXT_KEY_CIV_CARTHAGE_CONF_DESC", "TXT_KEY_CIV_CARTHAGE_MOD_SHORT", "TXT_KEY_CIV_CARTHAGE_MOD_ADJ", "mre", "1"]],
							
							[["CIVIC_OLIGARCHY"],
							["", "TXT_KEY_CIV_CARTHAGE_TRIBAL_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC2_DESC", "KEY_SHORT", "TXT_KEY_CIV_CARTHAGE_TRIBAL_ADJ", "pun", "1"]],
							
							[["CIVIC_REPUBLIC", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_CARTHAGE_TRIP_DESC", "", "TXT_KEY_CIV_CARTHAGE_TRIP_SHORT", "TXT_KEY_CIV_CARTHAGE_TRIP_ADJ", "trr", "1"]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "rep", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_CARTHAGE_MON_DESC", "", "TXT_KEY_CIV_CARTHAGE_MON_SHORT", "TXT_KEY_CIV_CARTHAGE_MON_ADJ", "mmo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "LEADER_GENSERIC"],
							["", "TXT_KEY_CIV_CARTHAGE_VAN_DESC", "", "TXT_KEY_CIV_CARTHAGE_VAN_SHORT", "TXT_KEY_CIV_CARTHAGE_VAN_ADJ", "van", "1"]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "TXT_KEY_CIV_CARTHAGE_AMON_DESC", "", "TXT_KEY_CIV_CARTHAGE_AMON_SHORT", "TXT_KEY_CIV_CARTHAGE_AMON_ADJ", "mon", "1"]],
							
							[["CIVIC_THEOCRACY"],
							["", "TXT_KEY_CIV_CARTHAGE_THEO_DESC", "", "TXT_KEY_CIV_CARTHAGE_MOD_SHORT", "TXT_KEY_CIV_CARTHAGE_MOD_ADJ", "the", "1"]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "TXT_KEY_CIV_CARTHAGE_MOD_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "TXT_KEY_CIV_CARTHAGE_MOD_SHORT", "TXT_KEY_CIV_CARTHAGE_MOD_ADJ", "fed", "1"]],
							
							[["CIVIC_DEMOCRACY", "LEADER_HABIB_BOURGUIBA"],
							["", "TXT_KEY_CIV_CARTHAGE_DEM_TUN_DESC", "", "TXT_KEY_CIV_CARTHAGE_DEM_TUN_SHORT", "TXT_KEY_CIV_CARTHAGE_DEM_TUN_ADJ", "tun", "1"]],
							
							[["CIVIC_DEMOCRACY"],
							["", "TXT_KEY_CIV_CARTHAGE_DEM_DESC", "", "TXT_KEY_CIV_CARTHAGE_DEM_SHORT", "TXT_KEY_CIV_CARTHAGE_DEM_ADJ", "dem", "1"]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "TXT_KEY_CIV_CARTHAGE_MOD_ADJ", "TXT_KEY_CIV_CARTHAGE_DIC_DESC", "TXT_KEY_CIV_CARTHAGE_MOD_SHORT", "TXT_KEY_CIV_CARTHAGE_MOD_ADJ", "dic", "1"]],
				],
				
		"CIVILIZATION_CELT":[				[["CIVIC_DESPOTISM"],							
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_TRIBAL_DESC", "", "", "tri", "1"]],
							
							[["CIVIC_STATE_PROPERTY", "LEADER_JAMES_IV_SCOTLAND"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "KEY_SHORT", "KEY_ADJ", "sco", "1"]],
							
							[["CIVIC_STATE_PROPERTY", "LEADER_ROBERT_THE_BRUCE"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "KEY_SHORT", "KEY_ADJ", "sco", "1"]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "KEY_SHORT", "KEY_ADJ", "com", "1"]],

							[["CIVIC_AUTOCRACY", "ERA_INDUSTRIAL"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "ide", "1"]],

							[["CIVIC_AUTOCRACY", "ERA_MEDIEVAL"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "mde", "1"]],
							
							[["CIVIC_AUTOCRACY"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "des", "1"]],
							
							[["CIVIC_OLIGARCHY", "ERA_MEDIEVAL", "LEADER_JAMES_IV_SCOTLAND"],
							["", "TXT_KEY_CIV_CELT_REP_SCO_DESC", "", "TXT_KEY_CIV_CELT_SCO_SHORT", "TXT_KEY_CIV_CELT_SCO_ADJ", "cov", "1"]],
							
							[["CIVIC_OLIGARCHY", "ERA_MEDIEVAL", "LEADER_ROBERT_THE_BRUCE"],
							["", "TXT_KEY_CIV_CELT_REP_SCO_DESC", "", "TXT_KEY_CIV_CELT_SCO_SHORT", "TXT_KEY_CIV_CELT_SCO_ADJ", "cov", "1"]],
							
							[["CIVIC_OLIGARCHY", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_CELT_REP_MED_DESC", "", "TXT_KEY_CIV_CELT_IRE_SHORT", "TXT_KEY_CIV_CELT_IRE_ADJ", "cir", "1"]],
							
							[["CIVIC_OLIGARCHY", "LEADER_ROBERT_THE_BRUCE"],
							["", "TXT_KEY_CIV_PICT_REP_DESC", "", "TXT_KEY_CIV_PICT_SHORT", "TXT_KEY_CIV_PICT_ADJ", "pic", "1"]],
							
							[["CIVIC_OLIGARCHY"],
							["", "TXT_KEY_CIV_CELT_REP_DESC", "", "KEY_SHORT", "KEY_ADJ", "rep", "1"]],							
							
							[["CIVIC_REPUBLIC", "ERA_MEDIEVAL", "LEADER_JAMES_IV_SCOTLAND"],
							["", "TXT_KEY_CIV_CELT_REP_SCO2_DESC", "", "TXT_KEY_CIV_CELT_SCO_SHORT", "TXT_KEY_CIV_CELT_SCO_ADJ", "sre", "1"]],
							
							[["CIVIC_REPUBLIC", "ERA_MEDIEVAL", "LEADER_ROBERT_THE_BRUCE"],
							["", "TXT_KEY_CIV_CELT_REP_SCO2_DESC", "", "TXT_KEY_CIV_CELT_SCO_SHORT", "TXT_KEY_CIV_CELT_SCO_ADJ", "sre", "1"]],
							
							[["CIVIC_REPUBLIC", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_CELT_REP_MED2_DESC", "", "TXT_KEY_CIV_CELT_IRE_SHORT", "TXT_KEY_CIV_CELT_IRE_ADJ", "mre", "1"]],
							
							[["CIVIC_REPUBLIC", "LEADER_JAMES_IV_SCOTLAND"],
							["", "TXT_KEY_CIV_PICT_REP_DESC", "", "TXT_KEY_CIV_PICT_SHORT", "TXT_KEY_CIV_PICT_ADJ", "pct", "1"]],
							
							[["CIVIC_REPUBLIC", "LEADER_ROBERT_THE_BRUCE"],
							["", "TXT_KEY_CIV_PICT_REP2_DESC", "", "TXT_KEY_CIV_PICT_SHORT", "TXT_KEY_CIV_PICT_ADJ", "pct", "1"]],
							
							[["CIVIC_REPUBLIC"],
							["", "TXT_KEY_CIV_PICT_REP2_DESC", "", "KEY_SHORT", "KEY_ADJ", "aed", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_MEDIEVAL", "LEADER_ROBERT_THE_BRUCE"],
							["", "TXT_KEY_CIV_CELT_MON_SCO_DESC", "", "TXT_KEY_CIV_CELT_SCO_SHORT", "TXT_KEY_CIV_CELT_SCO_ADJ", "smo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_MEDIEVAL", "LEADER_JAMES_IV_SCOTLAND"],
							["", "TXT_KEY_CIV_CELT_MON_SCO_DESC", "", "TXT_KEY_CIV_CELT_SCO_SHORT", "TXT_KEY_CIV_CELT_SCO_ADJ", "smo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_CELT_MON_MED_DESC", "", "TXT_KEY_CIV_CELT_IRE_SHORT", "TXT_KEY_CIV_CELT_IRE_ADJ", "mmo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "LEADER_ROBERT_THE_BRUCE"],
							["", "TXT_KEY_CIV_PICT_MON_DESC", "", "TXT_KEY_CIV_PICT_SHORT", "TXT_KEY_CIV_PICT_ADJ", "alb", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "LEADER_JAMES_IV_SCOTLAND"],
							["", "TXT_KEY_CIV_PICT_MON_DESC", "", "TXT_KEY_CIV_PICT_SHORT", "TXT_KEY_CIV_PICT_ADJ", "alb", "1"]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "TXT_KEY_CIV_CELT_MON_DESC", "", "KEY_SHORT", "KEY_ADJ", "mon", "1"]],

							[["CIVIC_THEOCRACY", "LEADER_ROBERT_THE_BRUCE"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "ths", "1"]],
							
							[["CIVIC_THEOCRACY", "LEADER_JAMES_IV_SCOTLAND"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "ths", "1"]],
							
							[["CIVIC_THEOCRACY"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "the", "1"]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "KEY_SHORT", "KEY_ADJ", "fed", "1"]],
							
							[["CIVIC_DEMOCRACY", "LEADER_ROBERT_THE_BRUCE"],
							["", "TXT_KEY_CIV_CELT_REP_SCO_DESC", "", "TXT_KEY_CIV_CELT_SCO_SHORT", "TXT_KEY_CIV_CELT_SCO_ADJ", "sre", "1"]],
							
							[["CIVIC_DEMOCRACY", "LEADER_JAMES_IV_SCOTLAND"],
							["", "TXT_KEY_CIV_CELT_REP_SCO_DESC", "", "TXT_KEY_CIV_CELT_SCO_SHORT", "TXT_KEY_CIV_CELT_SCO_ADJ", "sre", "1"]],
							
							[["CIVIC_DEMOCRACY"],
							["", "TXT_KEY_CIV_CELT_DEM_DESC", "", "TXT_KEY_CIV_CELT_IRE_SHORT", "TXT_KEY_CIV_CELT_IRE_ADJ", "dem", "1"]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "KEY_SHORT", "KEY_ADJ", "dic", "1"]],
				],
				
		"CIVILIZATION_CHINA":[				[["CIVIC_DESPOTISM"],							
							["", "TXT_KEY_CIV_CHINA_TRIBAL_DESC", "", "TXT_KEY_CIV_CHINA_TRIBAL_SHORT", "TXT_KEY_CIV_CHINA_TRIBAL_ADJ", "tri", "1"]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "TXT_KEY_CIV_CHINA_COM_DESC", "", "KEY_SHORT", "KEY_ADJ", "com", "1"]],
							
							[["CIVIC_AUTOCRACY", "ERA_MEDIEVAL"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "mde", "1"]],

							[["CIVIC_AUTOCRACY", "LEADER_CAO_CAO"],
							["", "TXT_KEY_CIV_CHINA_DES_CAO_DESC", "", "KEY_SHORT", "KEY_ADJ", "cao", "1"]],

							[["CIVIC_AUTOCRACY"],
							["", "TXT_KEY_CIV_CHINA_DES_DESC", "", "KEY_SHORT", "KEY_ADJ", "des", "1"]],

							[["CIVIC_OLIGARCHY", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_CHINA_FEN_DESC", "", "KEY_SHORT", "KEY_ADJ", "fen", "1"]],
							
							[["CIVIC_OLIGARCHY"],
							["", "TXT_KEY_CIV_CHINA_NOR_DESC", "", "KEY_SHORT", "KEY_ADJ", "nor", "1"]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC2_DESC", "KEY_SHORT", "KEY_ADJ", "rep", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "LEADER_WEN_OF_SUI"],
							["", "TXT_KEY_CIV_CHINA_SUI_DESC", "", "KEY_SHORT", "KEY_ADJ", "sui", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_CHINA_MON_MED_DESC", "", "KEY_SHORT", "KEY_ADJ", "mon", "1"]],

							[["CIVIC_HEREDITARY_RULE", "LEADER_CAO_CAO"],
							["", "TXT_KEY_CIV_CHINA_MON_CAO_DESC", "", "KEY_SHORT", "KEY_ADJ", "caw", "1"]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "TXT_KEY_CIV_CHINA_MON_DESC", "", "KEY_SHORT", "KEY_ADJ", "mon", "1"]],
							
							[["CIVIC_THEOCRACY"],
							["", "TXT_KEY_CIV_CHINA_THE_DESC", "", "KEY_SHORT", "KEY_ADJ", "the", "1"]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "TXT_KEY_CIV_CHINA_DEM_SHORT", "TXT_KEY_CIV_CHINA_DEM_ADJ", "dem", "1"]],
							
							[["CIVIC_DEMOCRACY"],
							["", "TXT_KEY_CIV_CHINA_DEM_DESC", "", "TXT_KEY_CIV_CHINA_DEM_SHORT", "TXT_KEY_CIV_CHINA_DEM_ADJ", "dem", "1"]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "KEY_DESC", "", "TXT_KEY_CIV_CHINA_DEM_SHORT", "TXT_KEY_CIV_CHINA_DEM_ADJ", "dic", "1"]],
				],

		"CIVILIZATION_SOUTH_CHINA":[				[["CIVIC_DESPOTISM"],							
							["", "TXT_KEY_CIV_SOUTH_CHINA_TRIBAL_DESC", "", "TXT_KEY_CIV_SOUTH_CHINA_TRIBAL_SHORT", "TXT_KEY_CIV_SOUTH_CHINA_TRIBAL_ADJ", "tri", "1"]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "TXT_KEY_CIV_SOUTH_CHINA_COM_DESC", "", "KEY_SHORT", "KEY_ADJ", "com", "1"]],
							
							[["CIVIC_AUTOCRACY", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_SOUTH_CHINA_DES_MED_DESC", "", "TXT_KEY_CIV_SOUTH_CHINA_DES_MED_SHORT", "TXT_KEY_CIV_SOUTH_CHINA_DES_MED_ADJ", "mde", "1"]],

							[["CIVIC_AUTOCRACY"],
							["", "TXT_KEY_CIV_SOUTH_CHINA_DES_DESC", "", "TXT_KEY_CIV_SOUTH_CHINA_TRIBAL_SHORT", "TXT_KEY_CIV_SOUTH_CHINA_TRIBAL_ADJ", "des", "1"]],

							[["CIVIC_OLIGARCHY", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_CHINA_GUA_DESC", "", "KEY_SHORT", "KEY_ADJ", "gua", "1"]],
							
							[["CIVIC_OLIGARCHY"],
							["", "TXT_KEY_CIV_CHINA_SOU_DESC", "", "KEY_SHORT", "KEY_ADJ", "sou", "1"]],
							
							[["CIVIC_REPUBLIC"],
							["", "TXT_KEY_CIV_SOUTH_CHINA_REP_DESC", "", "TXT_KEY_CIV_SOUTH_CHINA_REP_SHORT", "TXT_KEY_CIV_SOUTH_CHINA_REP_ADJ", "rep", "1"]],
														
							[["CIVIC_HEREDITARY_RULE", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_SOUTH_CHINA_MON_MED_DESC", "", "KEY_SHORT", "KEY_ADJ", "mmo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "TXT_KEY_CIV_SOUTH_CHINA_MON_DESC", "", "TXT_KEY_CIV_SOUTH_CHINA_MON_SHORT", "TXT_KEY_CIV_SOUTH_CHINA_MON_ADJ", "mon", "1"]],
							
							[["CIVIC_THEOCRACY"],
							["", "TXT_KEY_CIV_SOUTH_CHINA_THE_DESC", "", "TXT_KEY_CIV_SOUTH_CHINA_THE_SHORT", "TXT_KEY_CIV_SOUTH_CHINA_THE_ADJ", "the", "1"]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "TXT_KEY_CIV_SOUTH_CHINA_FED_DESC", "", "TXT_KEY_CIV_SOUTH_CHINA_DEM_SHORT", "TXT_KEY_CIV_SOUTH_CHINA_DEM_ADJ", "dem", "1"]],
							
							[["CIVIC_DEMOCRACY"],
							["", "TXT_KEY_CIV_CHINA_UNITED_DEM_DESC", "", "TXT_KEY_CIV_SOUTH_CHINA_DEM_SHORT", "TXT_KEY_CIV_SOUTH_CHINA_DEM_ADJ", "dem", "1"]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "TXT_KEY_CIV_SOUTH_CHINA_DIC_DESC", "", "TXT_KEY_CIV_SOUTH_CHINA_DEM_SHORT", "TXT_KEY_CIV_SOUTH_CHINA_DEM_ADJ", "dic", "1"]],
				],

		"CIVILIZATION_CHINA_UNITED":[				[["CIVIC_DESPOTISM"],							
							["", "TXT_KEY_CIV_CHINA_TRIBAL_DESC", "", "TXT_KEY_CIV_CHINA_TRIBAL_SHORT", "TXT_KEY_CIV_CHINA_TRIBAL_ADJ", "utr", "1"]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "TXT_KEY_CIV_CHINA_COM_DESC", "", "TXT_KEY_CIV_CHINA_UNITED_SHORT", "TXT_KEY_CIV_CHINA_UNITED_ADJ", "uco", "1"]],
														
							[["CIVIC_AUTOCRACY", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_CHINA_UNITED_DESC", "", "TXT_KEY_CIV_CHINA_UNITED_SHORT", "TXT_KEY_CIV_CHINA_UNITED_ADJ", "mde", "1"]],
							
							[["CIVIC_AUTOCRACY"],
							["", "TXT_KEY_CIV_CHINA_DES_DESC", "", "TXT_KEY_CIV_CHINA_UNITED_SHORT", "TXT_KEY_CIV_CHINA_UNITED_ADJ", "ude", "1"]],

							[["CIVIC_OLIGARCHY"],
							["", "TXT_KEY_CIV_CHINA_UNITED_NSD_DESC", "", "TXT_KEY_CIV_CHINA_UNITED_SHORT", "TXT_KEY_CIV_CHINA_UNITED_ADJ", "ucn", "1"]],
							
							[["CIVIC_REPUBLIC"],
							["", "TXT_KEY_CIV_SOUTH_CHINA_REP_DESC", "", "TXT_KEY_CIV_CHINA_UNITED_SHORT", "TXT_KEY_CIV_CHINA_UNITED_ADJ", "ure", "1"]],

							[["CIVIC_HEREDITARY_RULE", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_CHINA_MON_MED_DESC", "", "KEY_SHORT", "KEY_ADJ", "mon", "1"]],

							[["CIVIC_HEREDITARY_RULE"],
							["", "TXT_KEY_CIV_CHINA_MON_DESC", "", "TXT_KEY_CIV_CHINA_UNITED_SHORT", "TXT_KEY_CIV_CHINA_UNITED_ADJ", "umo", "1"]],
							
							[["CIVIC_THEOCRACY"],
							["", "TXT_KEY_CIV_SOUTH_CHINA_THE_DESC", "", "TXT_KEY_CIV_SOUTH_CHINA_THE_SHORT", "TXT_KEY_CIV_SOUTH_CHINA_THE_ADJ", "the", "1"]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "TXT_KEY_CIV_CHINA_UNITED_FED_DESC", "", "TXT_KEY_CIV_CHINA_UNITED_SHORT", "TXT_KEY_CIV_CHINA_UNITED_ADJ", "dem", "1"]],
							
							[["CIVIC_DEMOCRACY"],
							["", "TXT_KEY_CIV_CHINA_UNITED_DEM_DESC", "", "TXT_KEY_CIV_CHINA_UNITED_SHORT", "TXT_KEY_CIV_CHINA_UNITED_ADJ", "dem", "1"]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "TXT_KEY_CIV_CHINA_UNITED_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "TXT_KEY_CIV_CHINA_UNITED_SHORT", "TXT_KEY_CIV_CHINA_UNITED_ADJ", "dic", "1"]],
				],
				
		"CIVILIZATION_DRAVIDIAN":[				[["CIVIC_DESPOTISM"],							
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_TRIBAL_DESC", "", "", "tri", "1"]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "KEY_SHORT", "KEY_ADJ", "com", "1"]],

							[["CIVIC_AUTOCRACY", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_DRAV_DES_MED_DESC", "", "KEY_SHORT", "KEY_ADJ", "mde", "1"]],
							
							[["CIVIC_AUTOCRACY"],
							["", "TXT_KEY_CIV_DRAV_DES_DESC", "", "TXT_KEY_CIV_DRAV_MON_SHORT", "TXT_KEY_CIV_DRAV_MON_ADJ", "des", "1"]],
							
							[["CIVIC_OLIGARCHY"],
							["", "TXT_KEY_CIV_DRAV_CON_DESC", "", "KEY_SHORT", "KEY_ADJ", "rep", "1"]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "mre", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_DRAV_MON_MED_DESC", "", "TXT_KEY_CIV_DRAV_MON_MED_SHORT", "TXT_KEY_CIV_DRAV_MON_MED_ADJ", "rmo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_DRAV_MON_MED_DESC", "", "TXT_KEY_CIV_DRAV_MON_MED_SHORT", "TXT_KEY_CIV_DRAV_MON_MED_ADJ", "mmo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "TXT_KEY_CIV_DRAV_MON_DESC", "", "TXT_KEY_CIV_DRAV_MON_SHORT", "TXT_KEY_CIV_DRAV_MON_ADJ", "mon", "1"]],
							
							[["CIVIC_THEOCRACY"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "the", "1"]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "KEY_SHORT", "KEY_ADJ", "fed", "1"]],
							
							[["CIVIC_DEMOCRACY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "dem", "1"]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "TXT_KEY_CIV_DRAV_DIC_DESC", "", "KEY_SHORT", "KEY_ADJ", "dic", "1"]],
				],
				
		"CIVILIZATION_INDIA_UNITED":[				[["CIVIC_DESPOTISM"],							
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_TRIBAL_DESC", "", "", "tri", "1"]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "KEY_SHORT", "KEY_ADJ", "com", "1"]],

							[["CIVIC_AUTOCRACY", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_DRAV_DES_MED_DESC", "", "KEY_SHORT", "KEY_ADJ", "mde", "1"]],
							
							[["CIVIC_AUTOCRACY"],
							["", "TXT_KEY_CIV_DRAV_DES_DESC", "", "TXT_KEY_CIV_DRAV_MON_SHORT", "TXT_KEY_CIV_DRAV_MON_ADJ", "des", "1"]],
							
							[["CIVIC_OLIGARCHY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC2_DESC", "KEY_SHORT", "KEY_ADJ", "rep", "1"]],
							
							[["CIVIC_REPUBLIC", "ERA_MEDIEVAL"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "mre", "1"]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "are", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_DRAV_MON_MED_DESC", "", "TXT_KEY_CIV_DRAV_MON_MED_SHORT", "TXT_KEY_CIV_DRAV_MON_MED_ADJ", "mmo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "TXT_KEY_CIV_DRAV_MON_DESC", "", "TXT_KEY_CIV_DRAV_MON_SHORT", "TXT_KEY_CIV_DRAV_MON_ADJ", "mon", "1"]],
							
							[["CIVIC_THEOCRACY"],
							["", "TXT_KEY_CIV_INDIA_THE_DESC", "", "KEY_SHORT", "KEY_ADJ", "uth", "1"]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "TXT_KEY_CIV_INDIA_DEM_DESC", "", "TXT_KEY_CIV_INDIA_DEM_SHORT", "TXT_KEY_CIV_INDIA_DEM_ADJ", "ude", "1"]],
							
							[["CIVIC_DEMOCRACY"],
							["", "TXT_KEY_CIV_INDIA_DEM_DESC", "", "TXT_KEY_CIV_INDIA_DEM_SHORT", "TXT_KEY_CIV_INDIA_DEM_ADJ", "ude", "1"]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "TXT_KEY_CIV_INDIA_DIC_DESC", "", "TXT_KEY_CIV_INDIA_DEM_SHORT", "TXT_KEY_CIV_INDIA_DEM_ADJ", "udi", "1"]],
				],
				
		"CIVILIZATION_EGYPT":[				[["CIVIC_DESPOTISM"],							
							["", "TXT_KEY_CIV_EGYPT_TRI_DESC", "", "", "", "tri", "1"]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "KEY_SHORT", "KEY_ADJ", "com", "1"]],
							
							[["CIVIC_AUTOCRACY", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_EGYPT_MDE_DESC", "", "TXT_KEY_CIV_EGYPT_MDE_SHORT", "TXT_KEY_CIV_EGYPT_MDE_ADJ", "mde", "1"]],

							[["CIVIC_AUTOCRACY", "LEADER_PTOLEMY_SOTER"],
							["", "TXT_KEY_CIV_EGYPT_PDE_DESC", "", "KEY_SHORT", "KEY_ADJ", "pde", "1"]],

							[["CIVIC_AUTOCRACY"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "des", "1"]],

							[["CIVIC_OLIGARCHY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC2_DESC", "KEY_SHORT", "KEY_ADJ", "rep", "1"]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "rep", "1"]],

							[["CIVIC_HEREDITARY_RULE", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_EGYPT_RMO_DESC", "", "KEY_SHORT", "KEY_ADJ", "rmo", "1"]],

							[["CIVIC_HEREDITARY_RULE", "ERA_MEDIEVAL", "LEADER_MEHMET_ALIPASHA"],
							["", "TXT_KEY_CIV_EGYPT_MMO_DESC", "", "KEY_SHORT", "KEY_ADJ", "mmo", "1"]],

							[["CIVIC_HEREDITARY_RULE", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_EGYPT_MMO_DESC", "", "KEY_SHORT", "KEY_ADJ", "mmo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "LEADER_PTOLEMY_SOTER"],
							["", "TXT_KEY_CIV_EGYPT_PMO_DESC", "", "KEY_SHORT", "KEY_ADJ", "pmo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_KINGDOM_DESC", "KEY_SHORT", "KEY_ADJ", "mon", "1"]],
							
							[["CIVIC_THEOCRACY", "LEADER_AKHENATON"],
							["", "TXT_KEY_CIV_EGYPT_ATE_DESC", "", "KEY_SHORT", "KEY_ADJ", "ate", "1"]],
							
							[["CIVIC_THEOCRACY"],
							["", "TXT_KEY_CIV_EGYPT_THE_DESC", "", "TXT_KEY_CIV_EGYPT_THE_SHORT", "TXT_KEY_CIV_EGYPT_THE_ADJ", "the", "1"]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "KEY_SHORT", "KEY_ADJ", "fed", "1"]],
							
							[["CIVIC_DEMOCRACY"],
							["", "TXT_KEY_CIV_EGYPT_DEM_DESC", "", "KEY_SHORT", "KEY_ADJ", "dem", "1"]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "KEY_SHORT", "KEY_ADJ", "dic", "1"]],
				],
				
	"CIVILIZATION_ENGLAND":[				[["CIVIC_DESPOTISM"],							
							["", "TXT_KEY_CIV_ENGLAND_TRI_DESC", "", "TXT_KEY_CIV_ENGLAND_TRI_SHORT", "TXT_KEY_CIV_ENGLAND_TRI_ADJ", "tri", "1"]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "KEY_SHORT", "KEY_ADJ", "com", "1"]],

							[["CIVIC_AUTOCRACY", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_ENGLAND_RDE_DESC", "", "TXT_KEY_CIV_ENGLAND_BRIT_SHORT", "TXT_KEY_CIV_ENGLAND_BRIT_ADJ", "rde", "1"]],

							[["CIVIC_AUTOCRACY", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_ENGLAND_MDE_DESC", "", "KEY_SHORT", "KEY_ADJ", "mde", "1"]],
							
							[["CIVIC_AUTOCRACY"],
							["", "TXT_KEY_CIV_ENGLAND_DES_DESC", "", "KEY_SHORT", "KEY_ADJ", "des", "1"]],
							
							[["CIVIC_OLIGARCHY", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_ENGLAND_REP_DESC", "", "KEY_SHORT", "KEY_ADJ", "rre", "1"]],
							
							[["CIVIC_OLIGARCHY", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_ENGLAND_REP_DESC", "", "KEY_SHORT", "KEY_ADJ", "rep", "1"]],
							
							[["CIVIC_OLIGARCHY"],
							["", "TXT_KEY_CIV_ENGLAND_REP_ANC_DESC", "", "TXT_KEY_CIV_ENGLAND_TRI_SHORT", "TXT_KEY_CIV_ENGLAND_TRI_ADJ", "are", "1"]],
							
							[["CIVIC_REPUBLIC", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_ENGLAND_REP_DESC", "", "KEY_SHORT", "KEY_ADJ", "rre", "1"]],
							
							[["CIVIC_REPUBLIC", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_ENGLAND_REP_DESC", "", "KEY_SHORT", "KEY_ADJ", "rep", "1"]],
							
							[["CIVIC_REPUBLIC"],
							["", "TXT_KEY_CIV_ENGLAND_TRI_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "TXT_KEY_CIV_ENGLAND_TRI_SHORT", "TXT_KEY_CIV_ENGLAND_TRI_ADJ", "ess", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_INDUSTRIAL"],
							["", "TXT_KEY_CIV_ENGLAND_RMO_DESC", "", "TXT_KEY_CIV_ENGLAND_BRIT_SHORT", "TXT_KEY_CIV_ENGLAND_BRIT_ADJ", "imo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_ENGLAND_RMO_DESC", "", "TXT_KEY_CIV_ENGLAND_BRIT_SHORT", "TXT_KEY_CIV_ENGLAND_BRIT_ADJ", "rmo", "1"]],
														
							[["CIVIC_HEREDITARY_RULE", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_ENGLAND_MMO_DESC", "", "KEY_SHORT", "KEY_ADJ", "mmo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "TXT_KEY_CIV_ENGLAND_MON_DESC", "", "KEY_SHORT", "KEY_ADJ", "mon", "1"]],
							
							[["CIVIC_THEOCRACY"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "the", "1"]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "TXT_KEY_CIV_ENGLAND_FED_DESC", "", "TXT_KEY_CIV_ENGLAND_BRIT_SHORT", "TXT_KEY_CIV_ENGLAND_BRIT_ADJ", "fed", "1"]],
							
							[["CIVIC_DEMOCRACY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "dem", "1"]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "TXT_KEY_CIV_ENGLAND_BRIT_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "TXT_KEY_CIV_ENGLAND_BRIT_SHORT", "TXT_KEY_CIV_ENGLAND_BRIT_ADJ", "dic", "1"]],
				],
	"CIVILIZATION_ETHIOPIA":[				[["CIVIC_DESPOTISM"],							
							["", "TXT_KEY_CIV_ETHIOPIA_TRI_DESC", "", "TXT_KEY_CIV_ETHIOPIA_TRI_SHORT", "TXT_KEY_CIV_ETHIOPIA_TRI_ADJ", "tri", "1"]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "TXT_KEY_CIV_ETHIOPIA_COM_DESC", "", "KEY_SHORT", "KEY_ADJ", "com", "1"]],

							[["CIVIC_AUTOCRACY", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_ETHIOPIA_DES_DESC", "", "TXT_KEY_CIV_ETHIOPIA_RMO_SHORT", "TXT_KEY_CIV_ETHIOPIA_RMO_ADJ", "rde", "1"]],
							
							[["CIVIC_AUTOCRACY"],
							["", "TXT_KEY_CIV_ETHIOPIA_DES_DESC", "", "TXT_KEY_CIV_ETHIOPIA_DES_SHORT", "TXT_KEY_CIV_ETHIOPIA_DES_ADJ", "des", "1"]],

							[["CIVIC_OLIGARCHY", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_ETHIOPIA_RMO_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC2_DESC", "TXT_KEY_CIV_ETHIOPIA_RMO_SHORT", "TXT_KEY_CIV_ETHIOPIA_RMO_ADJ", "con", "1"]],
							
							[["CIVIC_OLIGARCHY"],
							["", "TXT_KEY_CIV_ETHIOPIA_DES_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC2_DESC", "TXT_KEY_CIV_ETHIOPIA_DES_SHORT", "TXT_KEY_CIV_ETHIOPIA_DES_ADJ", "aco", "1"]],
							
							[["CIVIC_REPUBLIC", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_ETHIOPIA_RMO_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "TXT_KEY_CIV_ETHIOPIA_RMO_SHORT", "TXT_KEY_CIV_ETHIOPIA_RMO_ADJ", "rre", "1"]],
							
							[["CIVIC_REPUBLIC"],
							["", "TXT_KEY_CIV_ETHIOPIA_DES_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "TXT_KEY_CIV_ETHIOPIA_DES_SHORT", "TXT_KEY_CIV_ETHIOPIA_DES_ADJ", "rep", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_ETHIOPIA_RMO_DESC", "", "TXT_KEY_CIV_ETHIOPIA_RMO_SHORT", "TXT_KEY_CIV_ETHIOPIA_RMO_ADJ", "rmo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "TXT_KEY_CIV_ETHIOPIA_MON_DESC", "", "TXT_KEY_CIV_ETHIOPIA_DES_SHORT", "TXT_KEY_CIV_ETHIOPIA_DES_ADJ", "mon", "1"]],
							
							[["CIVIC_THEOCRACY"],
							["", "TXT_KEY_CIV_ETHIOPIA_THE_DESC", "", "TXT_KEY_CIV_ETHIOPIA_THE_SHORT", "KEY_ADJ", "the", "1"]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "KEY_SHORT", "KEY_ADJ", "fed", "1"]],
							
							[["CIVIC_DEMOCRACY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "dem", "1"]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "TXT_KEY_CIV_ETHIOPIA_DIC_DESC", "", "TXT_KEY_CIV_ETHIOPIA_DIC_DESC", "KEY_ADJ", "dic", "1"]],
				],
	"CIVILIZATION_FRANCE":[				[["CIVIC_DESPOTISM"],							
							["", "TXT_KEY_CIV_FRANCE_TRI_DESC", "", "TXT_KEY_CIV_FRANCE_TRI_SHORT", "TXT_KEY_CIV_FRANCE_TRI_ADJ", "tri", "1"]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "KEY_CAPITAL", "TXT_KEY_CIV_FRANCE_COM_DESC", "KEY_SHORT", "KEY_ADJ", "coa", "1"]],

							[["CIVIC_AUTOCRACY", "ERA_INDUSTRIAL"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "ide", "1"]],
							
							[["CIVIC_AUTOCRACY", "ERA_RENAISSANCE", "LEADER_NAPOLEON"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "nap", "1"]],
							
							[["CIVIC_AUTOCRACY", "ERA_RENAISSANCE"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "rde", "1"]],
							
							[["CIVIC_AUTOCRACY", "ERA_MEDIEVAL"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "mmo", "1"]],
							
							[["CIVIC_AUTOCRACY"],
							["", "TXT_KEY_CIV_FRANCE_DES_DESC", "", "TXT_KEY_CIV_FRANCE_TRI_SHORT", "TXT_KEY_CIV_FRANCE_TRI_ADJ", "des", "1"]],
							
							[["CIVIC_OLIGARCHY", "ERA_MEDIEVAL"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC3_DESC", "KEY_SHORT", "KEY_ADJ", "mmo", "1"]],
							
							[["CIVIC_OLIGARCHY"],
							["", "TXT_KEY_CIV_FRANCE_TRI_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC3_DESC", "TXT_KEY_CIV_FRANCE_TRI_SHORT", "TXT_KEY_CIV_FRANCE_TRI_ADJ", "fra", "1"]],
							
							[["CIVIC_REPUBLIC", "ERA_RENAISSANCE"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "dem", "1"]],
							
							[["CIVIC_REPUBLIC", "ERA_MEDIEVAL"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "mmo", "1"]],
							
							[["CIVIC_REPUBLIC"],
							["", "TXT_KEY_CIV_FRANCE_TRI_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "TXT_KEY_CIV_FRANCE_TRI_SHORT", "TXT_KEY_CIV_FRANCE_TRI_ADJ", "rep", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_INDUSTRIAL"],
							["", "TXT_KEY_CIV_FRANCE_MON_MED_DESC", "", "KEY_SHORT", "KEY_ADJ", "imo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_FRANCE_MON_MED_DESC", "", "KEY_SHORT", "KEY_ADJ", "rmo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_FRANCE_MON_MED_DESC", "", "KEY_SHORT", "KEY_ADJ", "mmo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "TXT_KEY_CIV_FRANCE_MON_DESC", "", "TXT_KEY_CIV_FRANCE_TRI_SHORT", "TXT_KEY_CIV_FRANCE_TRI_ADJ", "mon", "1"]],
							
							[["CIVIC_THEOCRACY"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "the", "1"]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "TXT_KEY_CIV_FRANCE_FED_DESC", "", "KEY_SHORT", "KEY_ADJ", "dem", "1"]],
							
							[["CIVIC_DEMOCRACY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "dem", "1"]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "KEY_SHORT", "KEY_ADJ", "dic", "1"]],
				],
	"CIVILIZATION_GERMANY":[				[["CIVIC_DESPOTISM"],							
							["", "TXT_KEY_CIV_GERMANY_TRI_DESC", "", "KEY_SHORT", "TXT_KEY_CIV_GERMANY_TRI_ADJ", "tri", "1"]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "TXT_KEY_CIV_GERMANY_COM_DESC", "", "KEY_SHORT", "KEY_ADJ", "com", "1"]],

							[["CIVIC_AUTOCRACY", "ERA_INDUSTRIAL"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "ide", "1"]],
							
							[["CIVIC_AUTOCRACY", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_GERMANY_DES_DESC", "", "KEY_SHORT", "KEY_ADJ", "des", "1"]],
							
							[["CIVIC_AUTOCRACY"],
							["", "TXT_KEY_CIV_GERMANY_ADE_DESC", "", "KEY_SHORT", "TXT_KEY_CIV_GERMANY_TRI_ADJ", "ade", "1"]],
							
							[["CIVIC_OLIGARCHY", "LEADER_FREDERICK", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_GERMANY_RRE_PRU_DESC", "", "KEY_SHORT", "KEY_ADJ", "ngc", "1"]],
							
							[["CIVIC_OLIGARCHY", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_GERMANY_RRE_DESC", "", "KEY_SHORT", "KEY_ADJ", "rre", "1"]],
							
							[["CIVIC_OLIGARCHY", "LEADER_FREDERICK"],
							["", "TXT_KEY_CIV_GERMANY_RES_PRU_DESC", "", "TXT_KEY_CIV_GERMANY_RMO_SHORT", "TXT_KEY_CIV_GERMANY_RMO_ADJ", "rpr", "1"]],
							
							[["CIVIC_OLIGARCHY", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_GERMANY_MRE_DESC", "", "KEY_SHORT", "KEY_ADJ", "mre", "1"]],
							
							[["CIVIC_OLIGARCHY"],
							["", "TXT_KEY_CIV_GERMANY_RES_DESC", "", "KEY_SHORT", "TXT_KEY_CIV_GERMANY_TRI_ADJ", "rep", "1"]],
							
							[["CIVIC_REPUBLIC", "LEADER_FREDERICK", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_GERMANY_RRE_PRU2_DESC", "", "KEY_SHORT", "KEY_ADJ", "fsp", "1"]],
							
							[["CIVIC_REPUBLIC", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_GERMANY_RRE_DESC", "", "KEY_SHORT", "KEY_ADJ", "rre", "1"]],
							
							[["CIVIC_REPUBLIC", "LEADER_FREDERICK"],
							["", "TXT_KEY_CIV_GERMANY_RMO_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "TXT_KEY_CIV_GERMANY_RMO_SHORT", "TXT_KEY_CIV_GERMANY_RMO_ADJ", "prr", "1"]],
							
							[["CIVIC_REPUBLIC", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_GERMANY_MRE2_DESC", "", "KEY_SHORT", "KEY_ADJ", "mre", "1"]],
							
							[["CIVIC_REPUBLIC"],
							["", "TXT_KEY_CIV_GERMANY_TRI_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "TXT_KEY_CIV_GERMANY_TRI_ADJ", "sue", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_GERMANY_RMO_DESC", "", "TXT_KEY_CIV_GERMANY_RMO_SHORT", "TXT_KEY_CIV_GERMANY_RMO_ADJ", "rmo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "LEADER_FREDERICK"],
							["", "TXT_KEY_CIV_GERMANY_MON_BRA_DESC", "", "TXT_KEY_CIV_GERMANY_MON_BRA_SHORT", "TXT_KEY_CIV_GERMANY_MON_BRA_ADJ", "bra", "1"]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "TXT_KEY_CIV_GERMANY_MON_DESC", "", "KEY_SHORT", "KEY_ADJ", "mon", "1"]],
							
							[["CIVIC_THEOCRACY"],
							["", "TXT_KEY_CIV_GERMANY_THE_DESC", "", "TXT_KEY_CIV_GERMANY_THE_SHORT", "TXT_KEY_CIV_GERMANY_THE_ADJ", "the", "1"]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "TXT_KEY_CIV_GERMANY_FED_DESC", "", "KEY_SHORT", "KEY_ADJ", "fed", "1"]],
							
							[["CIVIC_DEMOCRACY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "dem", "1"]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "TXT_KEY_CIV_GERMANY_DIC_DESC", "", "KEY_SHORT", "KEY_ADJ", "dic", "1"]],
				],
	"CIVILIZATION_GREECE":[				[["CIVIC_DESPOTISM"],							
							["", "TXT_KEY_CIV_GREECE_TRI_DESC", "", "TXT_KEY_CIV_GREECE_TRI_SHORT", "TXT_KEY_CIV_GREECE_TRI_ADJ", "tri", "1"]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "KEY_SHORT", "KEY_ADJ", "com", "1"]],

							[["CIVIC_AUTOCRACY", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_GREECE_MDE_DESC", "", "TXT_KEY_CIV_GREECE_MDE_SHORT", "TXT_KEY_CIV_GREECE_MDE_ADJ", "mde", "1"]],

							[["CIVIC_AUTOCRACY", "LEADER_THEMISTOCLES"],
							["", "TXT_KEY_CIV_GREECE_DES_ATH_DESC", "", "TXT_KEY_CIV_GREECE_DES_ATH_SHORT", "TXT_KEY_CIV_GREECE_DES_ATH_ADJ", "ath", "1"]],
							
							[["CIVIC_AUTOCRACY", "LEADER_PERICLES"],
							["", "TXT_KEY_CIV_GREECE_DES_ATH_DESC", "", "TXT_KEY_CIV_GREECE_DES_ATH_SHORT", "TXT_KEY_CIV_GREECE_DES_ATH_ADJ", "ath", "1"]],
							
							[["CIVIC_AUTOCRACY"],
							["", "TXT_KEY_CIV_GREECE_DES_DESC", "", "TXT_KEY_CIV_GREECE_DES_SHORT", "TXT_KEY_CIV_GREECE_DES_ADJ", "des", "1"]],
							
							[["CIVIC_OLIGARCHY", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_GREECE_RRE_DESC", "", "KEY_SHORT", "KEY_ADJ", "rre", "1"]],

							[["CIVIC_OLIGARCHY", "LEADER_BASIL"],
							["", "TXT_KEY_CIV_GREECE_REP_DESC", "", "KEY_SHORT", "KEY_ADJ", "rco", "1"]],
							
							[["CIVIC_OLIGARCHY", "LEADER_JOHN_II_KOMNENOS"],
							["", "TXT_KEY_CIV_GREECE_REP_DESC", "", "KEY_SHORT", "KEY_ADJ", "rco", "1"]],
							
							[["CIVIC_OLIGARCHY"],
							["", "TXT_KEY_CIV_GREECE_REP_DESC", "", "KEY_SHORT", "KEY_ADJ", "rep", "1"]],
							
							[["CIVIC_REPUBLIC", "ERA_RENAISSANCE"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "rev", "1"]],
							
							[["CIVIC_REPUBLIC", "LEADER_BASIL"],
							["", "TXT_KEY_CIV_GREECE_REP2_DESC", "KEY_CAPITAL", "KEY_SHORT", "KEY_ADJ", "adb", "1"]],
							
							[["CIVIC_REPUBLIC", "LEADER_JOHN_II_KOMNENOS"],
							["", "TXT_KEY_CIV_GREECE_REP2_DESC", "KEY_CAPITAL", "KEY_SHORT", "KEY_ADJ", "adb", "1"]],
							
							[["CIVIC_REPUBLIC"],
							["", "TXT_KEY_CIV_GREECE_REP2_DESC", "KEY_CAPITAL", "KEY_SHORT", "KEY_ADJ", "ade", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_GREECE_MMO_DESC", "", "KEY_SHORT", "KEY_ADJ", "mmo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "LEADER_ALEXANDER"],
							["", "TXT_KEY_CIV_GREECE_MON_MAC_DESC", "", "TXT_KEY_CIV_GREECE_DES_SHORT", "TXT_KEY_CIV_GREECE_DES_ADJ", "mac", "1"]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "TXT_KEY_CIV_GREECE_MON_DESC", "", "KEY_SHORT", "KEY_ADJ", "mon", "1"]],
							
							[["CIVIC_THEOCRACY"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "the", "1"]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "KEY_SHORT", "KEY_ADJ", "dem", "1"]],
							
							[["CIVIC_DEMOCRACY"],
							["", "TXT_KEY_CIV_GREECE_DEM_DESC", "", "KEY_SHORT", "KEY_ADJ", "dem", "1"]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "KEY_SHORT", "KEY_ADJ", "dic", "1"]],
				],
	"CIVILIZATION_INDIA":[				[["CIVIC_DESPOTISM"],							
							["", "TXT_KEY_CIV_INDIA_TRI_DESC", "", "TXT_KEY_CIV_INDIA_TRI_SHORT", "TXT_KEY_CIV_INDIA_TRI_ADJ", "tri", "1"]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "KEY_SHORT", "KEY_ADJ", "com", "1"]],

							[["CIVIC_AUTOCRACY", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_INDIA_RDE_DESC", "", "TXT_KEY_CIV_INDIA_RDE_SHORT", "TXT_KEY_CIV_INDIA_RDE_ADJ", "rde", "1"]],

							[["CIVIC_AUTOCRACY", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_INDIA_MDE_DESC", "", "TXT_KEY_CIV_INDIA_MDE_SHORT", "TXT_KEY_CIV_INDIA_MDE_ADJ", "mde", "1"]],

							[["CIVIC_AUTOCRACY"],
							["", "TXT_KEY_CIV_INDIA_DES_DESC", "", "TXT_KEY_CIV_INDIA_DES_SHORT", "TXT_KEY_CIV_INDIA_DES_ADJ", "des", "1"]],
							
							[["CIVIC_OLIGARCHY", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_INDIA_MRE_DESC", "", "TXT_KEY_CIV_INDIA_MRE_SHORT", "TXT_KEY_CIV_INDIA_MRE_ADJ", "rde", "1"]],

							[["CIVIC_OLIGARCHY"],
							["", "TXT_KEY_CIV_INDIA_REP_DESC", "", "TXT_KEY_CIV_INDIA_REP_SHORT", "TXT_KEY_CIV_INDIA_REP_ADJ", "rep", "1"]],
							
							[["CIVIC_REPUBLIC", "ERA_MEDIEVAL"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "rre", "1"]],
						
							[["CIVIC_REPUBLIC"],
							["", "TXT_KEY_CIV_INDIA_REP2_DESC", "", "KEY_SHORT", "KEY_ADJ", "are", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_INDIA_MMO_DESC", "", "TXT_KEY_CIV_INDIA_MMO_SHORT", "TXT_KEY_CIV_INDIA_MMO_ADJ", "mmo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "TXT_KEY_CIV_INDIA_MON_DESC", "", "KEY_SHORT", "KEY_ADJ", "mon", "1"]],
							
							[["CIVIC_THEOCRACY"],
							["", "TXT_KEY_CIV_INDIA_THE_DESC", "", "KEY_SHORT", "KEY_ADJ", "the", "1"]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "TXT_KEY_CIV_INDIA_DEM_DESC", "", "TXT_KEY_CIV_INDIA_DEM_SHORT", "TXT_KEY_CIV_INDIA_DEM_ADJ", "dem", "1"]],
							
							[["CIVIC_DEMOCRACY"],
							["", "TXT_KEY_CIV_INDIA_DEM_DESC", "", "TXT_KEY_CIV_INDIA_DEM_SHORT", "TXT_KEY_CIV_INDIA_DEM_ADJ", "dem", "1"]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "TXT_KEY_CIV_INDIA_DIC_DESC", "", "TXT_KEY_CIV_INDIA_DEM_SHORT", "TXT_KEY_CIV_INDIA_DEM_ADJ", "dic", "1"]],
				],
	"CIVILIZATION_HUNGARY":[				[["CIVIC_DESPOTISM"],							
							["", "TXT_KEY_CIV_HUNGARY_TRI_DESC", "", "TXT_KEY_CIV_HUNGARY_TRI_SHORT", "TXT_KEY_CIV_HUNGARY_TRI_ADJ", "tri", "1"]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "TXT_KEY_CIV_HUNGARY_COM_DESC", "", "KEY_SHORT", "KEY_ADJ", "com", "1"]],

							[["CIVIC_AUTOCRACY", "ERA_RENAISSANCE"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "rde", "1"]],

							[["CIVIC_AUTOCRACY"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "des", "1"]],
							
							[["CIVIC_OLIGARCHY"],
							["", "TXT_KEY_CIV_HUNGARY_CON_DESC", "", "TXT_KEY_CIV_HUNGARY_TRI_SHORT", "TXT_KEY_CIV_HUNGARY_TRI_ADJ", "con", "1"]],
							
							[["CIVIC_REPUBLIC", "ERA_RENAISSANCE"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "rre", "1"]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "rep", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_HUNGARY_MON_DESC", "", "KEY_SHORT", "KEY_ADJ", "rmo", "1"]],
		
							[["CIVIC_HEREDITARY_RULE", "LEADER_LOUIS_I"],
							["", "TXT_KEY_CIV_HUNGARY_MON_DESC", "", "KEY_SHORT", "KEY_ADJ", "lou", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "LEADER_MATTHIAS_CORVINUS"],
							["", "TXT_KEY_CIV_HUNGARY_MON_DESC", "", "KEY_SHORT", "KEY_ADJ", "cor", "1"]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "TXT_KEY_CIV_HUNGARY_MON_DESC", "", "KEY_SHORT", "KEY_ADJ", "mon", "1"]],
							
							[["CIVIC_THEOCRACY"],
							["", "TXT_KEY_CIV_HUNGARY_THE_DESC", "", "KEY_SHORT", "KEY_ADJ", "the", "1"]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "KEY_SHORT", "KEY_ADJ", "dem", "1"]],
							
							[["CIVIC_DEMOCRACY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "dem", "1"]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "KEY_SHORT", "KEY_ADJ", "dic", "1"]],
				],
	"CIVILIZATION_INCA":[				[["CIVIC_DESPOTISM"],							
							["", "TXT_KEY_CIV_INCA_TRI_DESC", "", "TXT_KEY_CIV_INCA_TRI_SHORT", "TXT_KEY_CIV_INCA_TRI_ADJ", "tri", "1"]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "TXT_KEY_CIV_INCA_COM_DESC", "", "TXT_KEY_CIV_INCA_MOD_SHORT", "TXT_KEY_CIV_INCA_MOD_ADJ", "com", "1"]],

							[["CIVIC_AUTOCRACY"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "des", "1"]],
							
							[["CIVIC_OLIGARCHY", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_INCA_REP_DESC", "", "KEY_SHORT", "KEY_ADJ", "nre", "1"]],
							
							[["CIVIC_OLIGARCHY"],
							["", "TXT_KEY_CIV_INCA_REP_DESC", "", "KEY_SHORT", "KEY_ADJ", "rep", "1"]],
							
							[["CIVIC_REPUBLIC", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_INCA_MOD_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "TXT_KEY_CIV_INCA_MOD_SHORT", "TXT_KEY_CIV_INCA_MOD_ADJ", "rre", "1"]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "rep", "1"]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "TXT_KEY_CIV_INCA_MON_DESC", "KEY_CAPITAL", "KEY_SHORT", "KEY_ADJ", "mon", "1"]],
							
							[["CIVIC_THEOCRACY"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "the", "1"]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT", "ERA_MODERN"],
							["", "TXT_KEY_CIV_INCA_MOD_FED_DESC", "", "TXT_KEY_CIV_INCA_MOD_SHORT", "TXT_KEY_CIV_INCA_MOD_ADJ", "mfe", "1"]],							
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "TXT_KEY_CIV_INCA_FED_DESC", "", "TXT_KEY_CIV_INCA_MOD_SHORT", "TXT_KEY_CIV_INCA_MOD_ADJ", "fed", "1"]],
							
							[["CIVIC_DEMOCRACY"],
							["", "TXT_KEY_CIV_INCA_DEM_DESC", "", "TXT_KEY_CIV_INCA_MOD_SHORT", "TXT_KEY_CIV_INCA_MOD_ADJ", "dem", "1"]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "TXT_KEY_CIV_INCA_MOD_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "TXT_KEY_CIV_INCA_MOD_SHORT", "TXT_KEY_CIV_INCA_MOD_ADJ", "dic", "1"]],
				],
	"CIVILIZATION_JAPAN":[				[["CIVIC_DESPOTISM"],							
							["", "TXT_KEY_CIV_JAPAN_TRI_DESC", "", "TXT_KEY_CIV_JAPAN_TRI_SHORT", "TXT_KEY_CIV_JAPAN_TRI_ADJ", "tri", "1"]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "KEY_SHORT", "KEY_ADJ", "com", "1"]],

							[["CIVIC_AUTOCRACY", "ERA_INDUSTRIAL"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "rde", "1"]],

							[["CIVIC_AUTOCRACY"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "des", "1"]],
							
							[["CIVIC_OLIGARCHY"],
							["", "TXT_KEY_CIV_JAPAN_CON_DESC", "", "KEY_SHORT", "KEY_ADJ", "con", "1"]],
							
							[["CIVIC_REPUBLIC", "ERA_RENAISSANCE"],
							["", "KEY_CAPITAL", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "rre", "1"]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_CAPITAL", "TXT_KEY_CIV_JAPAN_REP_DESC", "KEY_SHORT", "KEY_ADJ", "rep", "1"]],
						
							[["CIVIC_HEREDITARY_RULE", "ERA_RENAISSANCE"],
							["", "KEY_CAPITAL", "TXT_KEY_CIV_JAPAN_MON_DESC", "KEY_SHORT", "KEY_ADJ", "rmo", "1"]],
														
							[["CIVIC_HEREDITARY_RULE", "LEADER_TOKUGAWA"],
							["", "KEY_CAPITAL", "TXT_KEY_CIV_JAPAN_MON_DESC", "KEY_SHORT", "KEY_ADJ", "toc", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "LEADER_ODA_NOBUNAGA"],
							["", "KEY_CAPITAL", "TXT_KEY_CIV_JAPAN_MON_DESC", "KEY_SHORT", "KEY_ADJ", "oda", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "LEADER_HIDEYOSHI"],
							["", "KEY_CAPITAL", "TXT_KEY_CIV_JAPAN_MON_DESC", "KEY_SHORT", "KEY_ADJ", "toy", "1"]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "KEY_CAPITAL", "TXT_KEY_CIV_JAPAN_MON_DESC", "KEY_SHORT", "KEY_ADJ", "mon", "1"]],
							
							[["CIVIC_THEOCRACY"],
							["", "TXT_KEY_CIV_JAPAN_THE_DESC", "", "KEY_SHORT", "KEY_ADJ", "the", "1"]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "KEY_SHORT", "KEY_ADJ", "rde", "1"]],
							
							[["CIVIC_DEMOCRACY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "rde", "1"]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "TXT_KEY_CIV_JAPAN_DIC_DESC", "", "KEY_SHORT", "KEY_ADJ", "dic", "1"]],
				],
	"CIVILIZATION_KOREA":[				[["CIVIC_DESPOTISM"],							
							["", "TXT_KEY_CIV_KOREA_TRI_DESC", "", "TXT_KEY_CIV_KOREA_TRI_SHORT", "TXT_KEY_CIV_KOREA_TRI_ADJ", "tri", "1"]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "TXT_KEY_CIV_KOREA_COM_DESC", "", "KEY_SHORT", "KEY_ADJ", "com", "1"]],

							[["CIVIC_AUTOCRACY", "ERA_RENAISSANCE"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "rde", "1"]],

							[["CIVIC_AUTOCRACY"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "des", "1"]],
							
							[["CIVIC_OLIGARCHY"],
							["", "TXT_KEY_CIV_KOREA_REP_DESC", "", "KEY_SHORT", "KEY_ADJ", "rep", "1"]],
							
							[["CIVIC_REPUBLIC"],
							["", "TXT_KEY_CIV_KOREA_ARE_DESC", "", "KEY_SHORT", "KEY_ADJ", "are", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_KOREA_MON_DESC", "", "KEY_SHORT", "KEY_ADJ", "rmo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "TXT_KEY_CIV_KOREA_MON_DESC", "", "KEY_SHORT", "KEY_ADJ", "mon", "1"]],
							
							[["CIVIC_THEOCRACY"],
							["", "TXT_KEY_CIV_KOREA_THE_DESC", "", "KEY_SHORT", "KEY_ADJ", "the", "1"]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "TXT_KEY_CIV_KOREA_FED_DESC", "", "KEY_SHORT", "KEY_ADJ", "fed", "1"]],
							
							[["CIVIC_DEMOCRACY"],
							["", "TXT_KEY_CIV_KOREA_DEM_DESC", "", "KEY_SHORT", "KEY_ADJ", "dem", "1"]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "KEY_SHORT", "KEY_ADJ", "dic", "1"]],
				],
	"CIVILIZATION_MALI":[				[["CIVIC_DESPOTISM"],							
							["", "TXT_KEY_CIV_MALI_TRI_DESC", "", "TXT_KEY_CIV_MALI_TRI_SHORT", "TXT_KEY_CIV_MALI_TRI_ADJ", "tri", "1"]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "KEY_SHORT", "KEY_ADJ", "com", "1"]],

							[["CIVIC_AUTOCRACY", "LEADER_SUNDIATA_KEITA"],
							["", "TXT_KEY_CIV_MALI_SON_DESC", "", "TXT_KEY_CIV_MALI_SON_SHORT", "TXT_KEY_CIV_MALI_SON_ADJ", "des", "1"]],

							[["CIVIC_AUTOCRACY"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "des", "1"]],
							
							[["CIVIC_OLIGARCHY"],
							["", "TXT_KEY_CIV_MALI_REP_DESC", "", "TXT_KEY_CIV_MALI_REP_SHORT", "TXT_KEY_CIV_MALI_REP_ADJ", "con", "1"]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_CAPITAL", "TXT_KEY_CIV_MALI_REP2_DESC", "KEY_SHORT", "KEY_ADJ", "rep", "1"]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "TXT_KEY_CIV_INCA_MON_DESC", "KEY_CAPITAL", "KEY_SHORT", "KEY_ADJ", "mon", "1"]],
							
							[["CIVIC_THEOCRACY"],
							["", "TXT_KEY_CIV_MALI_THE_DESC", "", "TXT_KEY_CIV_MALI_THE_SHORT", "TXT_KEY_CIV_MALI_THE_ADJ", "the", "1"]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "KEY_SHORT", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "KEY_SHORT", "KEY_ADJ", "fed", "1"]],
							
							[["CIVIC_DEMOCRACY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "dem", "1"]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "TXT_KEY_CIV_MALI_DIC_DESC", "", "KEY_SHORT", "KEY_ADJ", "dic", "1"]],
				],
	"CIVILIZATION_MAYA":[				[["CIVIC_DESPOTISM"],							
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_TRIBAL_DESC", "", "", "tri", "1"]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "TXT_KEY_CIV_MAYA_COM_DESC", "", "KEY_SHORT", "KEY_ADJ", "com", "1"]],

							[["CIVIC_AUTOCRACY", "ERA_RENAISSANCE"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "rde", "1"]],

							[["CIVIC_AUTOCRACY"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "des", "1"]],
							
							[["CIVIC_OLIGARCHY"],
							["", "TXT_KEY_CIV_MAYA_REP_DESC", "", "KEY_SHORT", "KEY_ADJ", "rep", "1"]],
							
							[["CIVIC_REPUBLIC", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_MAYA_RRE_DESC", "", "TXT_KEY_CIV_MAYA_RRE_SHORT", "KEY_ADJ", "rre", "1"]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_CAPITAL", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "are", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_INCA_MON_DESC", "KEY_CAPITAL", "KEY_SHORT", "KEY_ADJ", "rmo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "TXT_KEY_CIV_INCA_MON_DESC", "KEY_CAPITAL", "KEY_SHORT", "KEY_ADJ", "mon", "1"]],
							
							[["CIVIC_THEOCRACY"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "the", "1"]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "TXT_KEY_CIV_MAYA_FED_DESC", "", "KEY_SHORT", "KEY_ADJ", "fed", "1"]],
							
							[["CIVIC_DEMOCRACY"],
							["", "TXT_KEY_CIV_MAYA_DEM_DESC", "", "TXT_KEY_CIV_MAYA_DEM_SHORT", "TXT_KEY_CIV_MAYA_DEM_ADJ", "dem", "1"]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "TXT_KEY_CIV_MAYA_DIC_DESC", "", "TXT_KEY_CIV_MAYA_DEM_SHORT", "TXT_KEY_CIV_MAYA_DEM_ADJ", "dic", "1"]],
				],			
	"CIVILIZATION_MONGOL":[				[["CIVIC_DESPOTISM"],							
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_TRIBAL_DESC", "", "", "tri", "1"]],
							
							[["CIVIC_STATE_PROPERTY", "LEADER_PELJIDIIN_GENDEN"],
							["", "TXT_KEY_CIV_MONGOL_COM_DESC", "", "KEY_SHORT", "KEY_ADJ", "gdn", "1"]],
							
							[["CIVIC_STATE_PROPERTY", "LEADER_KHORLOOGIIN"],
							["", "TXT_KEY_CIV_MONGOL_COM_DESC", "", "KEY_SHORT", "KEY_ADJ", "cho", "1"]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "TXT_KEY_CIV_MONGOL_COM_DESC", "", "KEY_SHORT", "KEY_ADJ", "com", "1"]],
							
							[["CIVIC_OLIGARCHY"],
							["", "TXT_KEY_CIV_MONGOL_REP_DESC", "", "TXT_KEY_CIV_MONGOL_REP_SHORT", "KEY_ADJ", "rep", "1"]],
							
							[["CIVIC_AUTOCRACY"],
							["", "TXT_KEY_CIV_MONGOL_DES_DESC", "", "KEY_SHORT", "KEY_ADJ", "des", "1"]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_CAPITAL", "TXT_KEY_CIV_MONGOL_REP_ANC_DESC", "KEY_SHORT", "KEY_ADJ", "are", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "LEADER_KUBLAI_KHAN"],
							["", "TXT_KEY_CIV_MONGOL_YUAN_DESC", "", "TXT_KEY_CIV_MONGOL_MON_SHORT", "KEY_ADJ", "yua", "1"]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "TXT_KEY_CIV_MONGOL_MON_DESC", "", "TXT_KEY_CIV_MONGOL_MON_SHORT", "KEY_ADJ", "mon", "1"]],
							
							[["CIVIC_THEOCRACY"],
							["", "TXT_KEY_CIV_MONGOL_THE_DESC", "", "KEY_SHORT", "KEY_ADJ", "the", "1"]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "KEY_SHORT", "KEY_ADJ", "dem", "1"]],
							
							[["CIVIC_DEMOCRACY"],
							["", "TXT_KEY_CIV_MONGOL_DEM_DESC", "", "KEY_SHORT", "KEY_ADJ", "dem", "1"]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "TXT_KEY_CIV_MONGOL_DIC_DESC", "", "KEY_SHORT", "KEY_ADJ", "dic", "1"]],
				],
	"CIVILIZATION_PERSIA":[				[["CIVIC_DESPOTISM"],							
							["", "TXT_KEY_CIV_PERSIA_TRI_DESC", "", "TXT_KEY_CIV_PERSIA_TRI_SHORT", "TXT_KEY_CIV_PERSIA_TRI_ADJ", "tri", "1"]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "TXT_KEY_CIV_PERSIA_COM_DESC", "", "KEY_SHORT", "KEY_ADJ", "com", "1"]],

							[["CIVIC_AUTOCRACY", "ERA_RENAISSANCE"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "rde", "1"]],
							
							[["CIVIC_AUTOCRACY", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_PERSIA_DES_MED_DESC", "", "KEY_SHORT", "KEY_ADJ", "mmo", "1"]],

							[["CIVIC_AUTOCRACY", "LEADER_SELEUCUS_NICATOR"],
							["", "TXT_KEY_CIV_PERSIA_DES_SEL_DESC", "", "TXT_KEY_CIV_PERSIA_MON_SEL_SHORT", "TXT_KEY_CIV_PERSIA_MON_SEL_ADJ", "sel", "1"]],

							[["CIVIC_AUTOCRACY"],
							["", "TXT_KEY_CIV_PERSIA_DES_DESC", "", "KEY_SHORT", "KEY_ADJ", "des", "1"]],

							[["CIVIC_OLIGARCHY", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_PERSIA_RES_MED_DESC", "", "KEY_SHORT", "KEY_ADJ", "mre", "1"]],
							
							[["CIVIC_OLIGARCHY"],
							["", "TXT_KEY_CIV_PERSIA_RES_DESC", "", "KEY_SHORT", "KEY_ADJ", "rep", "1"]],
							
							[["CIVIC_REPUBLIC", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_PERSIA_RES_IND_DESC", "", "KEY_SHORT", "KEY_ADJ", "ire", "1"]],
							
							[["CIVIC_REPUBLIC"],
							["", "TXT_KEY_CIV_PERSIA_RES2_DESC", "", "KEY_SHORT", "KEY_ADJ", "are", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_INDUSTRIAL"],
							["", "TXT_KEY_CIV_PERSIA_MON_IND_DESC", "", "KEY_SHORT", "KEY_ADJ", "imo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_PERSIA_MON_MED_DESC", "", "KEY_SHORT", "KEY_ADJ", "rmo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_PERSIA_MON_MED_DESC", "", "KEY_SHORT", "KEY_ADJ", "mmo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "LEADER_SELEUCUS_NICATOR"],
							["", "TXT_KEY_CIV_PERSIA_MON_SEL_DESC", "", "TXT_KEY_CIV_PERSIA_MON_SEL_SHORT", "TXT_KEY_CIV_PERSIA_MON_SEL_ADJ", "sel", "1"]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "TXT_KEY_CIV_PERSIA_MON_DESC", "", "KEY_SHORT", "KEY_ADJ", "mon", "1"]],
							
							[["CIVIC_THEOCRACY", "ERA_RENAISSANCE"],
							["KEY_RELIGION", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "TXT_KEY_CIV_PERSIA_OF_IRAN_DESC", "TXT_KEY_CIV_PERSIA_OF_IRAN_SHORT", "TXT_KEY_CIV_PERSIA_OF_IRAN_ADJ", "rth", "1"]],
							
							[["CIVIC_THEOCRACY"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "the", "1"]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "KEY_SHORT", "KEY_ADJ", "dem", "1"]],
							
							[["CIVIC_DEMOCRACY"],
							["", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "TXT_KEY_CIV_PERSIA_OF_IRAN_DESC", "TXT_KEY_CIV_PERSIA_OF_IRAN_SHORT", "TXT_KEY_CIV_PERSIA_OF_IRAN_ADJ", "dem", "1"]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "TXT_KEY_CIV_PERSIA_OF_IRAN_DESC", "TXT_KEY_CIV_PERSIA_OF_IRAN_SHORT", "TXT_KEY_CIV_PERSIA_OF_IRAN_ADJ", "dic", "1"]],
				],			
	"CIVILIZATION_POLAND":[				[["CIVIC_DESPOTISM"],							
							["", "TXT_KEY_CIV_POLAND_TRI_DESC", "", "TXT_KEY_CIV_POLAND_TRI_SHORT", "TXT_KEY_CIV_POLAND_TRI_ADJ", "tri", "1"]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "TXT_KEY_CIV_POLAND_COM_DESC", "", "KEY_SHORT", "KEY_ADJ", "com", "1"]],

							[["CIVIC_AUTOCRACY", "ERA_RENAISSANCE"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "rde", "1"]],

							[["CIVIC_AUTOCRACY"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "des", "1"]],
							
							[["CIVIC_OLIGARCHY"],
							["", "TXT_KEY_CIV_POLAND_REP_ANC_DESC", "", "KEY_SHORT", "KEY_ADJ", "are", "1"]],
							
							[["CIVIC_REPUBLIC", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_POLAND_REP_DESC", "", "KEY_SHORT", "KEY_ADJ", "rre", "1"]],
							
							[["CIVIC_REPUBLIC", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_POLAND_REP_DESC", "", "KEY_SHORT", "KEY_ADJ", "rep", "1"]],
							
							[["CIVIC_REPUBLIC"],
							["", "TXT_KEY_CIV_POLAND_REP_DESC", "", "KEY_SHORT", "KEY_ADJ", "are", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_POLAND_MON_DESC", "", "KEY_SHORT", "KEY_ADJ", "rmo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "TXT_KEY_CIV_POLAND_MON_DESC", "", "KEY_SHORT", "KEY_ADJ", "mon", "1"]],
							
							[["CIVIC_THEOCRACY"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "the", "1"]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "KEY_SHORT", "KEY_ADJ", "dem", "1"]],
							
							[["CIVIC_DEMOCRACY"],
							["", "TXT_KEY_CIV_POLAND_DEM_DESC", "", "KEY_SHORT", "KEY_ADJ", "dem", "1"]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "KEY_SHORT", "KEY_ADJ", "dic", "1"]],
				],
	"CIVILIZATION_ROME": [				[["CIVIC_DESPOTISM"],							
							["", "TXT_KEY_CIV_ROME_TRI_DESC", "", "TXT_KEY_CIV_ROME_TRI_SHORT", "TXT_KEY_CIV_ROME_TRI_ADJ", "tri", "1"]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "TXT_KEY_CIV_ROME_ITA_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "TXT_KEY_CIV_ROME_ITA_SHORT", "TXT_KEY_CIV_ROME_ITA_ADJ", "com", "1"]],

							[["CIVIC_AUTOCRACY", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_ROME_ITA_DES_DESC", "", "TXT_KEY_CIV_ROME_ITA_SHORT", "TXT_KEY_CIV_ROME_ITA_ADJ", "rde", "1"]],

							[["CIVIC_AUTOCRACY"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "des", "1"]],

							[["CIVIC_OLIGARCHY", "ERA_INDUSTRIAL"],							
							["", "TXT_KEY_CIV_ROME_CON_MED_DESC", "", "TXT_KEY_CIV_ROME_ITA_SHORT", "TXT_KEY_CIV_ROME_ITA_ADJ", "ico", "1"]],
							
							[["CIVIC_OLIGARCHY", "ERA_MEDIEVAL"],							
							["", "TXT_KEY_CIV_ROME_CON_MED_DESC", "", "TXT_KEY_CIV_ROME_ITA_SHORT", "TXT_KEY_CIV_ROME_ITA_ADJ", "mco", "1"]],
							
							[["CIVIC_OLIGARCHY"],							
							["", "TXT_KEY_CIV_ROME_CON_DESC", "", "TXT_KEY_CIV_ROME_TRI_SHORT", "TXT_KEY_CIV_ROME_TRI_ADJ", "con", "1"]],

							[["CIVIC_REPUBLIC", "ERA_INDUSTRIAL"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "ire", "1"]],

							[["CIVIC_REPUBLIC", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_ROME_ITA_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "TXT_KEY_CIV_ROME_ITA_SHORT", "TXT_KEY_CIV_ROME_ITA_ADJ", "rre", "1"]],

							[["CIVIC_REPUBLIC", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_ROME_REP_MED_DESC", "", "KEY_SHORT", "KEY_ADJ", "mre", "1"]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "rep", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_INDUSTRIAL"],
							["", "TXT_KEY_CIV_ROME_ITA_MON_DESC", "", "TXT_KEY_CIV_ROME_ITA_SHORT", "TXT_KEY_CIV_ROME_ITA_ADJ", "imo", "1"]],		
							
							[["CIVIC_HEREDITARY_RULE", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_ROME_ITA_MON_DESC", "", "TXT_KEY_CIV_ROME_ITA_SHORT", "TXT_KEY_CIV_ROME_ITA_ADJ", "rmo", "1"]],							
							
							[["CIVIC_HEREDITARY_RULE", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_ROME_ITA_MON_DESC", "", "TXT_KEY_CIV_ROME_ITA_SHORT", "TXT_KEY_CIV_ROME_ITA_ADJ", "mmo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_KINGDOM_DESC", "KEY_SHORT", "KEY_ADJ", "mon", "1"]],
							
							[["CIVIC_THEOCRACY"],
							["", "TXT_KEY_CIV_ROME_THE_DESC", "", "TXT_KEY_CIV_ROME_THE_SHORT", "TXT_KEY_CIV_ROME_THE_ADJ", "the", "1"]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "TXT_KEY_CIV_ROME_ITA_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "TXT_KEY_CIV_ROME_ITA_SHORT", "TXT_KEY_CIV_ROME_ITA_ADJ", "dem", "1"]],
							
							[["CIVIC_DEMOCRACY"],
							["", "TXT_KEY_CIV_ROME_ITA_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "TXT_KEY_CIV_ROME_ITA_SHORT", "TXT_KEY_CIV_ROME_ITA_ADJ", "dem", "1"]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "TXT_KEY_CIV_ROME_DIC_DESC", "", "TXT_KEY_CIV_ROME_ITA_SHORT", "TXT_KEY_CIV_ROME_ITA_ADJ", "dic", "1"]],
				],

	"CIVILIZATION_RUSSIA": [				[["CIVIC_DESPOTISM"],							
							["", "TXT_KEY_CIV_RUSSIA_TRI_DESC", "", "TXT_KEY_CIV_RUSSIA_TRI_SHORT", "TXT_KEY_CIV_RUSSIA_TRI_ADJ", "tri", "1"]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "TXT_KEY_CIV_RUSSIA_COM_DESC", "", "TXT_KEY_CIV_RUSSIA_COM_SHORT", "TXT_KEY_CIV_RUSSIA_COM_ADJ", "com", "1"]],

							[["CIVIC_AUTOCRACY", "ERA_INDUSTRIAL"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "ide", "1"]],

							[["CIVIC_AUTOCRACY", "ERA_RENAISSANCE"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "rde", "1"]],

							[["CIVIC_AUTOCRACY", "ERA_MEDIEVAL"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "mde", "1"]],

							[["CIVIC_AUTOCRACY"],
							["", "TXT_KEY_CIV_RUSSIA_DES_DESC", "", "KEY_SHORT", "KEY_ADJ", "des", "1"]],
							
							[["CIVIC_OLIGARCHY"],
							["", "TXT_KEY_CIV_RUSSIA_REP_DESC", "", "KEY_SHORT", "KEY_ADJ", "rep", "1"]],
							
							[["CIVIC_REPUBLIC", "ERA_RENAISSANCE"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "rmo", "1"]],

							[["CIVIC_REPUBLIC", "LEADER_ALEXANDERII"],
							["", "TXT_KEY_CIV_RUSSIA_LADOGA_DESC", "TXT_KEY_CIV_RUSSIA_REP2_DESC", "KEY_SHORT", "KEY_ADJ", "spr", "1"]],
							
							[["CIVIC_REPUBLIC", "LEADER_PETER"],
							["", "TXT_KEY_CIV_RUSSIA_LADOGA_DESC", "TXT_KEY_CIV_RUSSIA_REP2_DESC", "KEY_SHORT", "KEY_ADJ", "spr", "1"]],
							
							[["CIVIC_REPUBLIC", "LEADER_CATHERINE"],
							["", "TXT_KEY_CIV_RUSSIA_LADOGA_DESC", "TXT_KEY_CIV_RUSSIA_REP2_DESC", "KEY_SHORT", "KEY_ADJ", "spr", "1"]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_CAPITAL", "TXT_KEY_CIV_RUSSIA_REP2_DESC", "KEY_SHORT", "KEY_ADJ", "are", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_INDUSTRIAL"],
							["", "KEY_ADJ", "TXT_KEY_CIV_RUSSIA_MON_TSAR_DESC", "KEY_SHORT", "KEY_ADJ", "imo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_RENAISSANCE"],
							["", "KEY_ADJ", "TXT_KEY_CIV_RUSSIA_MON_TSAR_DESC", "KEY_SHORT", "KEY_ADJ", "rmo", "1"]],

							[["CIVIC_HEREDITARY_RULE", "LEADER_ALEXANDERII"],
							["", "TXT_KEY_CIV_RUSSIA_MON_DESC", "TXT_KEY_CIV_RUSSIA_LADOGA_DESC", "KEY_SHORT", "KEY_ADJ", "spb", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "LEADER_PETER"],
							["", "TXT_KEY_CIV_RUSSIA_MON_DESC", "TXT_KEY_CIV_RUSSIA_LADOGA_DESC", "KEY_SHORT", "KEY_ADJ", "spb", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "LEADER_CATHERINE"],
							["", "TXT_KEY_CIV_RUSSIA_MON_DESC", "TXT_KEY_CIV_RUSSIA_LADOGA_DESC", "KEY_SHORT", "KEY_ADJ", "spb", "1"]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "TXT_KEY_CIV_RUSSIA_MON_DESC", "KEY_CAPITAL", "KEY_SHORT", "KEY_ADJ", "mon", "1"]],
							
							[["CIVIC_THEOCRACY"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "the", "1"]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "KEY_SHORT", "KEY_ADJ", "dem", "1"]],
							
							[["CIVIC_DEMOCRACY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "dem", "1"]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "KEY_SHORT", "KEY_ADJ", "dic", "1"]],
				],
				
	"CIVILIZATION_VIKING":[				[["CIVIC_DESPOTISM"],							
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_TRIBAL_DESC", "", "", "tri", "1"]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "TXT_KEY_CIV_VIKING_SCA_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "KEY_SHORT", "KEY_ADJ", "com", "1"]],

							[["CIVIC_AUTOCRACY", "ERA_RENAISSANCE", "LEADER_CHRISTIAN_IV"],
							["", "TXT_KEY_CIV_VIKING_DEN_DESC", "", "TXT_KEY_CIV_VIKING_DEN_SHORT", "TXT_KEY_CIV_VIKING_DEN_ADJ", "dde", "1"]],

							[["CIVIC_AUTOCRACY", "ERA_MODERN"],
							["", "TXT_KEY_CIV_VIKING_SWE_DESC", "", "TXT_KEY_CIV_VIKING_SWE_SHORT", "TXT_KEY_CIV_VIKING_SWE_ADJ", "mod", "1"]],
							
							[["CIVIC_AUTOCRACY", "ERA_INDUSTRIAL"],
							["", "TXT_KEY_CIV_VIKING_SWE_DESC", "", "TXT_KEY_CIV_VIKING_SWE_SHORT", "TXT_KEY_CIV_VIKING_SWE_ADJ", "ide", "1"]],

							[["CIVIC_AUTOCRACY", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_VIKING_SWE_DESC", "", "TXT_KEY_CIV_VIKING_SWE_SHORT", "TXT_KEY_CIV_VIKING_SWE_ADJ", "rde", "1"]],

							[["CIVIC_AUTOCRACY"],
							["", "TXT_KEY_CIV_VIKING_DES_DESC", "", "TXT_KEY_CIV_VIKING_VIK_SHORT", "TXT_KEY_CIV_VIKING_VIK_ADJ", "des", "1"]],

							[["CIVIC_OLIGARCHY", "ERA_MEDIEVAL"],							
							["", "TXT_KEY_CIV_VIKING_CON_MED_DESC", "", "KEY_SHORT", "KEY_ADJ", "con", "1"]],
							
							[["CIVIC_OLIGARCHY"],							
							["", "TXT_KEY_CIV_VIKING_CON_DESC", "", "TXT_KEY_CIV_VIKING_VIK_SHORT", "TXT_KEY_CIV_VIKING_VIK_ADJ", "aco", "1"]],
							
							[["CIVIC_REPUBLIC", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_VIKING_REP_REN_DESC", "", "TXT_KEY_CIV_VIKING_REP_SHORT", "TXT_KEY_CIV_VIKING_REP_ADJ", "dem", "1"]],
							
							[["CIVIC_REPUBLIC"],
							["", "TXT_KEY_CIV_VIKING_REP_DESC", "", "TXT_KEY_CIV_VIKING_REP_SHORT", "TXT_KEY_CIV_VIKING_REP_ADJ", "rep", "1"]],

							[["CIVIC_HEREDITARY_RULE", "LEADER_CHRISTIAN_IV", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_INCA_MON_DESC", "TXT_KEY_CIV_VIKING_DEN_SHORT", "TXT_KEY_CIV_VIKING_DEN_SHORT", "TXT_KEY_CIV_VIKING_DEN_ADJ", "dmr", "1"]],

							[["CIVIC_HEREDITARY_RULE", "LEADER_CHRISTIAN_IV"],
							["", "TXT_KEY_CIV_INCA_MON_DESC", "TXT_KEY_CIV_VIKING_DEN_SHORT", "TXT_KEY_CIV_VIKING_DEN_SHORT", "TXT_KEY_CIV_VIKING_DEN_ADJ", "dmo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_MODERN"],
							["", "TXT_KEY_CIV_INCA_MON_DESC", "TXT_KEY_CIV_VIKING_SWE_SHORT", "TXT_KEY_CIV_VIKING_SWE_SHORT", "TXT_KEY_CIV_VIKING_SWE_ADJ", "mom", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_INDUSTRIAL"],
							["", "TXT_KEY_CIV_INCA_MON_DESC", "TXT_KEY_CIV_VIKING_SWE_SHORT", "TXT_KEY_CIV_VIKING_SWE_SHORT", "TXT_KEY_CIV_VIKING_SWE_ADJ", "imo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_INCA_MON_DESC", "TXT_KEY_CIV_VIKING_SWE_SHORT", "TXT_KEY_CIV_VIKING_SWE_SHORT", "TXT_KEY_CIV_VIKING_SWE_ADJ", "mmo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "TXT_KEY_CIV_INCA_MON_DESC", "TXT_KEY_CIV_VIKING_SVE_SHORT", "TXT_KEY_CIV_VIKING_SWE_SHORT", "TXT_KEY_CIV_VIKING_SWE_ADJ", "mon", "1"]],
							
							[["CIVIC_THEOCRACY"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "TXT_KEY_CIV_VIKING_SCA_DESC", "", "KEY_SHORT", "KEY_ADJ", "the", "1"]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "TXT_KEY_CIV_VIKING_SCA_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "KEY_SHORT", "KEY_ADJ", "fed", "1"]],
							
							[["CIVIC_DEMOCRACY", "LEADER_CHRISTIAN_IV"],
							["", "TXT_KEY_CIV_VIKING_DEN_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "dde", "1"]],
							
							[["CIVIC_DEMOCRACY"],
							["", "TXT_KEY_CIV_VIKING_SWE_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "mod", "1"]],

							[["CIVIC_DICTATORSHIP", "LEADER_CHRISTIAN_IV"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "KEY_SHORT", "KEY_ADJ", "ddi", "1"]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "KEY_SHORT", "KEY_ADJ", "dic", "1"]],
				],			
				
	"CIVILIZATION_SPAIN":[				[["CIVIC_DESPOTISM"],							
							["", "TXT_KEY_CIV_SPAIN_IBE_ADJ", "TXT_KEY_CIV_GENERIC_TRIBAL_DESC", "TXT_KEY_CIV_SPAIN_IBE_SHORT", "TXT_KEY_CIV_SPAIN_IBE_ADJ", "tri", "1"]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "KEY_SHORT", "KEY_ADJ", "com", "1"]],

							[["CIVIC_AUTOCRACY", "ERA_RENAISSANCE"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "rde", "1"]],

							[["CIVIC_AUTOCRACY", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_SPAIN_CAS_DESC", "", "TXT_KEY_CIV_SPAIN_CAS_SHORT", "TXT_KEY_CIV_SPAIN_CAS_ADJ", "des", "1"]],
							
							[["CIVIC_AUTOCRACY"],
							["", "TXT_KEY_CIV_SPAIN_IBE_DESC", "", "TXT_KEY_CIV_SPAIN_IBE_SHORT", "TXT_KEY_CIV_SPAIN_IBE_ADJ", "ade", "1"]],
							
							[["CIVIC_OLIGARCHY"],
							["", "TXT_KEY_CIV_SPAIN_REP_DESC", "", "TXT_KEY_CIV_SPAIN_IBE_SHORT", "TXT_KEY_CIV_SPAIN_IBE_ADJ", "rep", "1"]],
							
							[["CIVIC_REPUBLIC", "ERA_RENAISSANCE"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "rre", "1"]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "rep", "1"]],

							[["CIVIC_HEREDITARY_RULE", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_SPAIN_RMO_DESC", "", "KEY_SHORT", "KEY_ADJ", "rmo", "1"]],

							[["CIVIC_HEREDITARY_RULE", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_SPAIN_MON_DESC", "", "TXT_KEY_CIV_SPAIN_CAS_SHORT", "TXT_KEY_CIV_SPAIN_CAS_ADJ", "mmo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "TXT_KEY_CIV_SPAIN_AST_DESC", "", "KEY_SHORT", "KEY_ADJ", "mon", "1"]],
							
							[["CIVIC_THEOCRACY", "ERA_RENAISSANCE"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "the", "1"]],
							
							[["CIVIC_THEOCRACY"],
							["", "TXT_KEY_CIV_SPAIN_THE_DESC", "", "KEY_SHORT", "KEY_ADJ", "ath", "1"]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "KEY_SHORT", "KEY_ADJ", "dem", "1"]],
							
							[["CIVIC_DEMOCRACY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "dem", "1"]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "KEY_SHORT", "KEY_ADJ", "dic", "1"]],
				],
	"CIVILIZATION_TRANSOXIANA":[				[["CIVIC_DESPOTISM", "LEADER_DURRANI"],							
							["", "TXT_KEY_CIV_TRANS_ATR_DESC", "", "TXT_KEY_CIV_TRANS_ATR_SHORT", "TXT_KEY_CIV_TRANS_ATR_ADJ", "atr", "1"]],
							
							[["CIVIC_DESPOTISM"],							
							["", "TXT_KEY_CIV_TRANS_TRI_DESC", "", "TXT_KEY_CIV_TRANS_TRI_SHORT", "TXT_KEY_CIV_TRANS_TRI_ADJ", "tri", "1"]],

							[["CIVIC_STATE_PROPERTY", "LEADER_DURRANI"],
							["", "TXT_KEY_CIV_TRANS_COM_AFG_DESC", "", "TXT_KEY_CIV_TRANS_AFG_SHORT", "TXT_KEY_CIV_TRANS_AFG_ADJ", "aco", "1"]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "TXT_KEY_CIV_TRANS_COM_DESC", "", "TXT_KEY_CIV_TRANS_UZB_SHORT", "TXT_KEY_CIV_TRANS_UZB_ADJ", "com", "1"]],

							[["CIVIC_AUTOCRACY", "ERA_MEDIEVAL", "LEADER_DURRANI"],
							["", "TXT_KEY_CIV_TRANS_DES_AFG_DESC", "", "TXT_KEY_CIV_TRANS_AFG_SHORT", "TXT_KEY_CIV_TRANS_AFG_ADJ", "ard", "1"]],

							[["CIVIC_AUTOCRACY", "ERA_MEDIEVAL", "LEADER_ISMAIL_SAMANI"],
							["", "TXT_KEY_CIV_TRANS_SAM_DES_DESC", "", "TXT_KEY_CIV_TRANS_SAM_SHORT", "TXT_KEY_CIV_TRANS_SAM_ADJ", "sde", "1"]],

							[["CIVIC_AUTOCRACY", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_TRANS_UZB_DIC_DESC", "", "TXT_KEY_CIV_TRANS_UZB_SHORT", "TXT_KEY_CIV_TRANS_UZB_ADJ", "rde", "1"]],

							[["CIVIC_AUTOCRACY", "LEADER_DEMETRIUS_OF_BACTRIA"],
							["", "TXT_KEY_CIV_TRANS_BDE_DESC", "", "TXT_KEY_CIV_TRANS_BAC_SHORT", "TXT_KEY_CIV_TRANS_BAC_ADJ", "bmo", "1"]],

							[["CIVIC_AUTOCRACY", "LEADER_MITHRIDATES_I_PARTHIA"],
							["", "TXT_KEY_CIV_TRANS_PAR_DES_DESC", "", "TXT_KEY_CIV_TRANS_PAR_SHORT", "TXT_KEY_CIV_TRANS_PAR_ADJ", "pde", "1"]],
							
							[["CIVIC_AUTOCRACY", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_TRANS_TIM_DESC", "", "TXT_KEY_CIV_TRANS_TIM_SHORT", "TXT_KEY_CIV_TRANS_TIM_ADJ", "mde", "1"]],

							[["CIVIC_AUTOCRACY"],
							["", "TXT_KEY_CIV_TRANS_DES_DESC", "", "TXT_KEY_CIV_TRANS_DES_SHORT", "TXT_KEY_CIV_TRANS_DES_ADJ", "des", "1"]],

							[["CIVIC_OLIGARCHY", "LEADER_DURRANI"],
							["", "TXT_KEY_CIV_TRANS_AFG_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC2_DESC", "TXT_KEY_CIV_TRANS_AFG_SHORT", "TXT_KEY_CIV_TRANS_AFG_ADJ", "are", "1"]],

							[["CIVIC_OLIGARCHY", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_TRANS_NOG_DESC", "", "TXT_KEY_CIV_TRANS_NOG_SHORT", "TXT_KEY_CIV_TRANS_NOG_ADJ", "rre", "1"]],
							
							[["CIVIC_OLIGARCHY", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_TRANS_TIM_REP_DESC", "", "TXT_KEY_CIV_TRANS_TIM_SHORT", "TXT_KEY_CIV_TRANS_TIM_ADJ", "mmo", "1"]],
							
							[["CIVIC_OLIGARCHY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC2_DESC", "TXT_KEY_CIV_TRANS_REP_SHORT", "TXT_KEY_CIV_TRANS_REP_ADJ", "rep", "1"]],
							
							[["CIVIC_REPUBLIC", "LEADER_DURRANI"],
							["", "TXT_KEY_CIV_TRANS_AFG_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "TXT_KEY_CIV_TRANS_AFG_SHORT", "TXT_KEY_CIV_TRANS_AFG_ADJ", "afr", "1"]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "TXT_KEY_CIV_TRANS_REP_SHORT", "TXT_KEY_CIV_TRANS_REP_ADJ", "mre", "1"]],

							[["CIVIC_HEREDITARY_RULE", "LEADER_DURRANI", "ERA_MODERN"],
							["", "TXT_KEY_CIV_INCA_MON_DESC", "TXT_KEY_CIV_TRANS_AFG_SHORT", "TXT_KEY_CIV_TRANS_AFG_SHORT", "TXT_KEY_CIV_TRANS_AFG_ADJ", "amm", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_MEDIEVAL", "LEADER_ISMAIL_SAMANI"],
							["", "TXT_KEY_CIV_TRANS_SAM_MON_DESC", "", "TXT_KEY_CIV_TRANS_SAM_SHORT", "TXT_KEY_CIV_TRANS_SAM_ADJ", "sde", "1"]],

							[["CIVIC_HEREDITARY_RULE", "LEADER_DURRANI", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_INCA_MON_DESC", "TXT_KEY_CIV_TRANS_AFG_SHORT", "TXT_KEY_CIV_TRANS_AFG_SHORT", "TXT_KEY_CIV_TRANS_AFG_ADJ", "arm", "1"]],

							[["CIVIC_HEREDITARY_RULE", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_TRANS_KAZ_DESC", "", "TXT_KEY_CIV_TRANS_KAZ_SHORT", "TXT_KEY_CIV_TRANS_KAZ_ADJ", "rmo", "1"]],

							[["CIVIC_HEREDITARY_RULE", "LEADER_MITHRIDATES_I_PARTHIA"],
							["", "TXT_KEY_CIV_TRANS_PAR_MON_DESC", "", "TXT_KEY_CIV_TRANS_PAR_SHORT", "TXT_KEY_CIV_TRANS_PAR_ADJ", "pmo", "1"]],

							[["CIVIC_HEREDITARY_RULE", "LEADER_DEMETRIUS_OF_BACTRIA"],
							["", "TXT_KEY_CIV_TRANS_BMO_DESC", "", "TXT_KEY_CIV_TRANS_BAC_SHORT", "TXT_KEY_CIV_TRANS_BAC_ADJ", "bmo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_TRANS_TIM_MON_DESC", "", "TXT_KEY_CIV_TRANS_TIM_SHORT", "TXT_KEY_CIV_TRANS_TIM_ADJ", "mmo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "TXT_KEY_CIV_TRANS_MON_DESC", "", "TXT_KEY_CIV_TRANS_DES_SHORT", "TXT_KEY_CIV_TRANS_DES_ADJ", "mon", "1"]],
							
							[["CIVIC_THEOCRACY", "LEADER_DURRANI"],
							["TXT_KEY_CIV_TRANS_THE_DESC", "TXT_KEY_CIV_TRANS_AFG_SHORT", "", "TXT_KEY_CIV_TRANS_AFG_SHORT", "TXT_KEY_CIV_TRANS_AFG_ADJ", "the", "1"]],
							
							[["CIVIC_THEOCRACY"],
							["TXT_KEY_CIV_TRANS_THE_DESC", "TXT_KEY_CIV_TRANS_UZB_SHORT", "", "TXT_KEY_CIV_TRANS_UZB_SHORT", "TXT_KEY_CIV_TRANS_UZB_ADJ", "the", "1"]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "KEY_SHORT", "KEY_ADJ", "fed", "1"]],
							
							[["CIVIC_DEMOCRACY", "LEADER_DURRANI"],
							["", "TXT_KEY_CIV_TRANS_DEM_AFG_DESC", "", "TXT_KEY_CIV_TRANS_AFG_SHORT", "TXT_KEY_CIV_TRANS_AFG_ADJ", "ade", "1"]],
							
							[["CIVIC_DEMOCRACY"],
							["", "TXT_KEY_CIV_TRANS_UZB_DEM_DESC", "", "TXT_KEY_CIV_TRANS_UZB_SHORT", "TXT_KEY_CIV_TRANS_UZB_ADJ", "dem", "1"]],

							[["CIVIC_DICTATORSHIP", "LEADER_DURRANI"],
							["", "TXT_KEY_CIV_TRANS_DIC_AFG_DESC", "", "TXT_KEY_CIV_TRANS_AFG_SHORT", "TXT_KEY_CIV_TRANS_AFG_ADJ", "adi", "1"]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "TXT_KEY_CIV_TRANS_DIC_DESC", "", "TXT_KEY_CIV_TRANS_DIC_SHORT", "TXT_KEY_CIV_TRANS_DIC_ADJ", "dic", "1"]],
				],			
	"CIVILIZATION_OTTOMAN":[				[["CIVIC_DESPOTISM"],							
							["", "TXT_KEY_CIV_TUR_TRI_DESC", "", "KEY_SHORT", "TXT_KEY_CIV_TUR_TRI_ADJ", "tri", "1"]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "KEY_SHORT", "KEY_ADJ", "com", "1"]],

							[["CIVIC_AUTOCRACY", "ERA_INDUSTRIAL"],
							["", "TXT_KEY_CIV_TUR_DES_OTT_DESC", "", "TXT_KEY_CIV_TUR_OTT_SHORT", "TXT_KEY_CIV_TUR_OTT_ADJ", "ide", "1"]],
							
							[["CIVIC_AUTOCRACY", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_TUR_DES_OTT_DESC", "", "TXT_KEY_CIV_TUR_OTT_SHORT", "TXT_KEY_CIV_TUR_OTT_ADJ", "rde", "1"]],

							[["CIVIC_AUTOCRACY", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_TUR_DES_MED_DESC", "", "TXT_KEY_CIV_TUR_DES_MED_SHORT", "TXT_KEY_CIV_TUR_DES_MED_ADJ", "mde", "1"]],

							[["CIVIC_AUTOCRACY"],
							["", "TXT_KEY_CIV_TUR_DES_DESC", "", "TXT_KEY_CIV_TUR_DES_SHORT", "TXT_KEY_CIV_TUR_DES_ADJ", "des", "1"]],
							
							[["CIVIC_OLIGARCHY", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_TUR_REP_REN_DESC", "", "KEY_SHORT", "KEY_ADJ", "rre", "1"]],
							
							[["CIVIC_OLIGARCHY", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_TUR_REP_MED_DESC", "", "TXT_KEY_CIV_TUR_REP_MED_SHORT", "TXT_KEY_CIV_TUR_REP_MED_ADJ", "mre", "1"]],
							
							[["CIVIC_OLIGARCHY"],
							["", "TXT_KEY_CIV_TUR_REP_DESC", "", "TXT_KEY_CIV_TUR_DES_SHORT", "TXT_KEY_CIV_TUR_DES_ADJ", "rep", "1"]],
							
							[["CIVIC_REPUBLIC", "ERA_INDUSTRIAL"],
							["", "KEY_CAPITAL", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "ire", "1"]],

							[["CIVIC_REPUBLIC"],
							["", "TXT_KEY_CIV_TUR_REP2_DESC", "", "KEY_SHORT", "KEY_ADJ", "ahi", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_INDUSTRIAL"],
							["", "TXT_KEY_CIV_TUR_REN_MON_DESC", "", "TXT_KEY_CIV_TUR_OTT_SHORT", "TXT_KEY_CIV_TUR_OTT_ADJ", "imo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_TUR_REN_MON_DESC", "", "TXT_KEY_CIV_TUR_OTT_SHORT", "TXT_KEY_CIV_TUR_OTT_ADJ", "rmo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_MEDIEVAL"],
							["", "TXT_KEY_CIV_TUR_MED_MON_DESC", "", "KEY_SHORT", "KEY_ADJ", "mmo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "TXT_KEY_CIV_TUR_MON_DESC", "", "TXT_KEY_CIV_TUR_DES_SHORT", "TXT_KEY_CIV_TUR_DES_ADJ", "mon", "1"]],
							
							[["CIVIC_THEOCRACY"],
							["", "TXT_KEY_CIV_TUR_THE_OTT_DESC", "", "TXT_KEY_CIV_TUR_OTT_SHORT", "TXT_KEY_CIV_TUR_OTT_ADJ", "the", "1"]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "TXT_KEY_CIV_TUR_TRI_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "KEY_SHORT", "KEY_ADJ", "fed", "1"]],
							
							[["CIVIC_DEMOCRACY"],
							["", "TXT_KEY_CIV_TUR_DEM_DESC", "", "KEY_SHORT", "KEY_ADJ", "dem", "1"]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "KEY_SHORT", "KEY_ADJ", "dic", "1"]],
				],
	"CIVILIZATION_ZULU": [			[["CIVIC_DESPOTISM"],							
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_TRIBAL_DESC", "", "", "tri", "1"]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "TXT_KEY_CIV_ZULU_COM_DESC", "", "KEY_SHORT", "KEY_ADJ", "com", "1"]],

							[["CIVIC_AUTOCRACY"],
							["", "TXT_KEY_CIV_ZULU_DES_DESC", "", "TXT_KEY_CIV_ZULU_DES_SHORT", "TXT_KEY_CIV_ZULU_DES_ADJ", "des", "1"]],
							
							[["CIVIC_OLIGARCHY"],
							["", "TXT_KEY_CIV_ZULU_REP_DESC", "", "TXT_KEY_CIV_ZULU_DES_SHORT", "TXT_KEY_CIV_ZULU_DES_ADJ", "rep", "1"]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_CAPITAL", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "TXT_KEY_CIV_ZULU_DES_SHORT", "TXT_KEY_CIV_ZULU_DES_ADJ", "are", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_RENAISSANCE", "LEADER_MOSHOESHOE_I"],
							["", "TXT_KEY_CIV_ZULU_MON_BAS_DESC", "", "TXT_KEY_CIV_ZULU_MON_BAS_SHORT", "TXT_KEY_CIV_ZULU_MON_BAS_ADJ", "bas", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_RENAISSANCE", "LEADER_NYATSIMBA"],
							["", "TXT_KEY_CIV_ZULU_MON_DESC", "", "KEY_SHORT", "KEY_ADJ", "mon", "1"]],
							
							[["CIVIC_HEREDITARY_RULE", "ERA_RENAISSANCE"],
							["", "TXT_KEY_CIV_ZULU_MON_REN_DESC", "", "TXT_KEY_CIV_ZULU_DES_SHORT", "TXT_KEY_CIV_ZULU_DES_ADJ", "rmo", "1"]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "TXT_KEY_CIV_ZULU_MON_DESC", "", "KEY_SHORT", "KEY_ADJ", "mon", "1"]],
							
							[["CIVIC_THEOCRACY"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "the", "1"]],

							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "TXT_KEY_CIV_ZULU_FED_DESC", "", "TXT_KEY_CIV_ZULU_FED_SHORT", "TXT_KEY_CIV_ZULU_FED_ADJ", "fed", "1"]],
							
							[["CIVIC_DEMOCRACY"],
							["", "TXT_KEY_CIV_ZULU_DEM_DESC", "", "TXT_KEY_CIV_ZULU_DEM_SHORT", "TXT_KEY_CIV_ZULU_DEM_ADJ", "dem", "1"]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "TXT_KEY_CIV_ZULU_DIC_DESC", "", "KEY_SHORT", "KEY_ADJ", "dic", "1"]],
				],
	"CIVILIZATION_AUSTRIA":[	[["CIVIC_DICTATORSHIP"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "KEY_SHORT", "KEY_ADJ", "dic", "1"]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "KEY_SHORT", "KEY_ADJ", "com", "1"]],
							
							[["ERA_MODERN"],
							["", "", "", "", "", "mod", "1"]],
							
							[["ERA_RENAISSANCE"],
							["", "", "", "", "", "ind", "1"]],
	
							[["CIVIC_DESPOTISM"],							
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_TRIBAL_DESC", "", "", "", ""]],

							[["CIVIC_AUTOCRACY"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_KINGDOM_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_THEOCRACY"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
				],
	"CIVILIZATION_BABYLON":[		[["CIVIC_STATE_PROPERTY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "KEY_SHORT", "KEY_ADJ", "com", "1"]],	
							
							[["ERA_MODERN"],
							["", "", "", "", "", "mod", "1"]],
							
							[["ERA_INDUSTRIAL"],
							["", "", "", "", "", "ind", "1"]],
							
							[["ERA_MEDIEVAL"],
							["", "", "", "", "", "med", "1"]],
							
							[["CIVIC_DESPOTISM"],							
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_TRIBAL_DESC", "", "", "tri", "1"]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],

							[["CIVIC_AUTOCRACY"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC2_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_KINGDOM_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_THEOCRACY"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
				],
	"CIVILIZATION_BRAZIL":[		[["CIVIC_STATE_PROPERTY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "KEY_SHORT", "KEY_ADJ", "com", "1"]],

							[["CIVIC_DICTATORSHIP"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "KEY_SHORT", "KEY_ADJ", "dic", "1"]],		
	
							[["ERA_INDUSTRIAL"],
							["", "", "", "", "", "mod", "1"]],		
	
							[["CIVIC_DESPOTISM"],							
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_TRIBAL_DESC", "", "", "tri", "1"]],

							[["CIVIC_AUTOCRACY"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_KINGDOM_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_THEOCRACY"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
				],				
	"CIVILIZATION_FINLAND":[	[["CIVIC_STATE_PROPERTY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "KEY_SHORT", "KEY_ADJ", "com", "1"]],
							
							[["ERA_INDUSTRIAL"],
							["", "", "", "", "", "mod", "1"]],
							
							[["CIVIC_DESPOTISM"],							
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_TRIBAL_DESC", "", "", "tri", "1"]],

							[["CIVIC_AUTOCRACY"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_KINGDOM_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_THEOCRACY"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
				],
	"CIVILIZATION_HALYCH_VOLHYNIA":[	[["CIVIC_STATE_PROPERTY"],
							["", "TXT_KEY_CIV_HALYCH_COM_DESC", "", "KEY_SHORT", "KEY_ADJ", "com", "1"]],		
							
							[["ERA_INDUSTRIAL"],
							["", "", "", "", "", "mod", "1"]],
							
							[["ERA_RENAISSANCE"],
							["", "", "", "", "", "ren", "1"]],							
	
							[["CIVIC_DESPOTISM"],							
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_TRIBAL_DESC", "", "", "", ""]],

							[["CIVIC_AUTOCRACY"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_KINGDOM_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_THEOCRACY"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
				],
	"CIVILIZATION_ISRAEL":[		[["CIVIC_STATE_PROPERTY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "KEY_SHORT", "KEY_ADJ", "com", "1"]],		
	
							[["ERA_INDUSTRIAL"],
							["", "", "", "", "", "mod", "1"]],	

							[["CIVIC_DESPOTISM"],							
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_TRIBAL_DESC", "", "", "tri", "1"]],

							[["CIVIC_AUTOCRACY"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_KINGDOM_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_THEOCRACY"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
				],
	"CIVILIZATION_KHMER":[			[["CIVIC_STATE_PROPERTY"],
							["", "TXT_KEY_CIV_KHMER_COM_DESC", "", "KEY_SHORT", "KEY_ADJ", "com", "1"]],		
							
							[["ERA_INDUSTRIAL"],
							["", "", "", "", "", "mod", "1"]],		
	
							[["CIVIC_DESPOTISM"],							
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_TRIBAL_DESC", "", "", "tri", "1"]],

							[["CIVIC_AUTOCRACY"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC2_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_KINGDOM_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_THEOCRACY"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
				],
	"CIVILIZATION_MEXICO":[	[["CIVIC_DESPOTISM"],							
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_TRIBAL_DESC", "", "", "", ""]],
							
							[["CIVIC_STATE_PROPERTY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "KEY_SHORT", "KEY_ADJ", "com", "1"]],
							
							[["ERA_RENAISSANCE"],
							["", "", "", "", "", "mod", "1"]],		
	
							[["CIVIC_AUTOCRACY"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_KINGDOM_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_THEOCRACY"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
				],		
	"CIVILIZATION_MUGHAL":[		[["CIVIC_STATE_PROPERTY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "KEY_SHORT", "KEY_ADJ", "com", "1"]],			
							
							[["ERA_INDUSTRIAL"],
							["", "", "", "", "", "mod", "1"]],		
	
							[["CIVIC_DESPOTISM"],							
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_TRIBAL_DESC", "", "", "tri", "1"]],

							[["CIVIC_AUTOCRACY"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_KINGDOM_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_THEOCRACY"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
				],	
	"CIVILIZATION_NETHERLANDS":[	[["CIVIC_STATE_PROPERTY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "KEY_SHORT", "KEY_ADJ", "com", "1"]],

							[["ERA_INDUSTRIAL"],
							["", "", "", "", "", "ind", "1"]],	

							[["ERA_RENAISSANCE"],
							["", "", "", "", "", "ren", "1"]],							
	
							[["CIVIC_DESPOTISM"],							
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_TRIBAL_DESC", "", "", "", ""]],

							[["CIVIC_AUTOCRACY"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_KINGDOM_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_THEOCRACY"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
				],				
	"CIVILIZATION_NUBIA":[		[["CIVIC_STATE_PROPERTY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "KEY_SHORT", "KEY_ADJ", "com", "1"]],

							[["ERA_MODERN"],
							["", "", "", "", "", "mod", "1"]],
							
							[["ERA_INDUSTRIAL"],
							["", "", "", "", "", "ind", "1"]],	

							[["ERA_MEDIEVAL"],
							["", "", "", "", "", "med", "1"]],							
	
							[["CIVIC_DESPOTISM"],							
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_TRIBAL_DESC", "", "", "tri", "1"]],

							[["CIVIC_AUTOCRACY"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC2_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_KINGDOM_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_THEOCRACY"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
				],					
	"CIVILIZATION_PONTUS":[		[["CIVIC_STATE_PROPERTY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "KEY_SHORT", "KEY_ADJ", "com", "1"]],

							[["ERA_INDUSTRIAL"],
							["", "", "", "", "", "mod", "1"]],							
	
							[["CIVIC_DESPOTISM"],							
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_TRIBAL_DESC", "", "", "", ""]],
							

							[["CIVIC_AUTOCRACY"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_KINGDOM_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_THEOCRACY"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
				],			
	"CIVILIZATION_PORTUGAL":[		[["CIVIC_STATE_PROPERTY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "KEY_SHORT", "KEY_ADJ", "com", "1"]],

							[["CIVIC_DICTATORSHIP"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "KEY_SHORT", "KEY_ADJ", "dic", "1"]],	
							
							[["ERA_INDUSTRIAL"],
							["", "", "", "", "", "mod", "1"]],	
							
							[["ERA_RENAISSANCE"],
							["", "", "", "", "", "ren", "1"]],

							[["ERA_MEDIEVAL"],
							["", "", "", "", "", "med", "1"]],								
							
							[["ERA_ANCIENT"],
							["", "", "", "", "", "anc", "1"]],							
	
							[["CIVIC_DESPOTISM"],							
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_TRIBAL_DESC", "", "", "tri", "1"]],

							[["CIVIC_AUTOCRACY"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_KINGDOM_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_THEOCRACY"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
				],	
	"CIVILIZATION_TAIWAN":[		[["CIVIC_STATE_PROPERTY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "KEY_SHORT", "KEY_ADJ", "com", "1"]],

							[["ERA_INDUSTRIAL"],
							["", "", "", "", "", "alt", "1"]],
	
							[["CIVIC_DESPOTISM"],							
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_TRIBAL_DESC", "", "", "tri", "1"]],

							[["CIVIC_AUTOCRACY"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_KINGDOM_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_THEOCRACY"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
				],				
	"CIVILIZATION_TARTAR":[			[["CIVIC_STATE_PROPERTY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "KEY_SHORT", "KEY_ADJ", "com", "1"]],

							[["ERA_INDUSTRIAL"],
							["", "", "", "", "", "mod", "1"]],							
	
							[["CIVIC_DESPOTISM"],							
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_TRIBAL_DESC", "", "", "", ""]],

							[["CIVIC_AUTOCRACY"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_KINGDOM_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_THEOCRACY"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
				],	
	"CIVILIZATION_TEXAS":[			[["CIVIC_STATE_PROPERTY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "KEY_SHORT", "KEY_ADJ", "com", "1"]],

							[["ERA_INDUSTRIAL"],
							["", "", "", "", "", "mod", "1"]],		
	
							[["CIVIC_DESPOTISM"],							
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_TRIBAL_DESC", "", "", "", ""]],

							[["CIVIC_AUTOCRACY"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_KINGDOM_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_THEOCRACY"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
				],					
	"CIVILIZATION_UKRAINE":[			[["CIVIC_STATE_PROPERTY"],
							["", "TXT_KEY_CIV_UKRAINE_COM_DESC", "", "KEY_SHORT", "KEY_ADJ", "com", "1"]],	
							
							[["ERA_INDUSTRIAL"],
							["", "", "", "", "", "mod", "1"]],

							[["ERA_RENAISSANCE"],
							["", "", "", "", "", "ren", "1"]],							
	
							[["CIVIC_DESPOTISM"],							
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_TRIBAL_DESC", "", "", "tri", "1"]],
							
							[["CIVIC_AUTOCRACY"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_KINGDOM_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_THEOCRACY"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
				],	
	"CIVILIZATION_VENICE":[			[["CIVIC_STATE_PROPERTY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_COMMUNIST_DESC", "KEY_SHORT", "KEY_ADJ", "com", "1"]],

							[["ERA_INDUSTRIAL"],
							["", "", "", "", "", "mod", "1"]],
	
							[["CIVIC_DESPOTISM"],							
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_TRIBAL_DESC", "", "", "", ""]],

							[["CIVIC_AUTOCRACY"],
							["", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_REPUBLIC"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_HEREDITARY_RULE"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_KINGDOM_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_THEOCRACY"],
							["TXT_KEY_CIV_GENERIC_THEO_DESC", "KEY_DESC", "", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY", "BUILDINGCLASS_FEDERAL_PARLIAMENT"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_FEDERATION_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DEMOCRACY"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_REPUBLIC_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
							
							[["CIVIC_DICTATORSHIP"],
							["", "KEY_ADJ", "TXT_KEY_CIV_GENERIC_DICTATORSHIP_DESC", "KEY_SHORT", "KEY_ADJ", "", ""]],
				],															
}


class DynamicCivNaming:
	def nameStart(self, iPlayer):
		pPlayer = gc.getPlayer(iPlayer)
		if pPlayer.isNone(): return
		if pPlayer.isBarbarian(): return
		if not pPlayer.isAlive(): return
		
		sFlag = pPlayer.getFlagDecal()
		
		if sFlag[24:28] == str("Scn_"):	return 
		if sFlag[24:28] == str("msc_"):	return 
		if sFlag[24:28] == str("fire"):	return 
		
		if sFlag[24:29] == str("rebel"):
			if pPlayer.getNumCities() > 2:
				iCiv = pPlayer.getCivilizationType()
				CivInfo = gc.getCivilizationInfo(iCiv)
				sDesc = CivInfo.getDescription()
				sShort = CivInfo.getShortDescription(0)
				sAdj = CivInfo.getAdjective(0)
				pPlayer.setCivName(sDesc, sShort, sAdj)
				
				sFlag = CivInfo.getFlagTexture()
				pPlayer.setFlagDecal(sFlag, true)
				pPlayer.setWhiteFlag(true)
			else: return 
		
		if sFlag[24:28] == str("euro"):	
			if not gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_DYNAMIC_CIV_NAMES):
				sFlag = CivInfo.getFlagTexture()
				pPlayer.setFlagDecal(sFlag, true)
				pPlayer.setWhiteFlag(true)
			else: return 	
			
		if not gc.getTeam(pPlayer.getTeam()).isAVassal():
			iCiv = pPlayer.getCivilizationType()
			CivInfo = gc.getCivilizationInfo(iCiv)
			sCiv = CivInfo.getType()
		else:
			for iPlayerX in xrange(gc.getMAX_PLAYERS()):
				pPlayerX = gc.getPlayer(iPlayerX)
				if (gc.getTeam(pPlayer.getTeam()).isVassal(pPlayerX.getTeam())):
					iCiv = pPlayerX.getCivilizationType()
					CivInfo = gc.getCivilizationInfo(iCiv)
					sCiv = CivInfo.getType()
					sCiv = sCiv.replace("CIVILIZATION","MASTER") 
			if sCiv not in Dynamic:
				sCiv = "MASTER_OTHERS"
			iCiv = pPlayer.getCivilizationType()
			CivInfo = gc.getCivilizationInfo(iCiv)
		
		if sCiv not in Dynamic:
			sCiv = "CIVILIZATION_OTHERS"

##################################################################################################			
## China unity test
##################################################################################################

		sChinaUnited = "false"
		
		if sCiv == "CIVILIZATION_CHINA":
			sChinaUnited = "true"
			for iPlayerX in xrange(gc.getMAX_PLAYERS()):
				pPlayerX = gc.getPlayer(iPlayerX)
				if pPlayerX.isNone(): continue
				if pPlayerX.isBarbarian(): continue
				if not pPlayerX.isAlive(): continue
				iCivX = pPlayerX.getCivilizationType()
				CivInfoX = gc.getCivilizationInfo(iCivX)
				sCivX = CivInfoX.getType()
				if sCivX == "CIVILIZATION_SOUTH_CHINA": 
					sChinaUnited = "false"

		if sCiv == "CIVILIZATION_SOUTH_CHINA":
			sChinaUnited = "true"
			for iPlayerX in xrange(gc.getMAX_PLAYERS()):
				pPlayerX = gc.getPlayer(iPlayerX)
				if pPlayerX.isNone(): continue
				if pPlayerX.isBarbarian(): continue
				if not pPlayerX.isAlive(): continue
				iCivX = pPlayerX.getCivilizationType()
				CivInfoX = gc.getCivilizationInfo(iCivX)
				sCivX = CivInfoX.getType()
				if sCivX == "CIVILIZATION_CHINA":
					sChinaUnited = "false"
					
		if sChinaUnited == "true":
			sCiv = "CIVILIZATION_CHINA_UNITED"
			
##################################################################################################			
## India unity test
##################################################################################################

		sIndiaUnited = "false"
		
		if sCiv == "CIVILIZATION_DRAVIDIAN":
			sIndiaUnited = "true"
			for iPlayerX in xrange(gc.getMAX_PLAYERS()):
				pPlayerX = gc.getPlayer(iPlayerX)
				if pPlayerX.isNone(): continue
				if pPlayerX.isBarbarian(): continue
				if not pPlayerX.isAlive(): continue
				iCivX = pPlayerX.getCivilizationType()
				CivInfoX = gc.getCivilizationInfo(iCivX)
				sCivX = CivInfoX.getType()
				if sCivX == "CIVILIZATION_INDIA": 
					sIndiaUnited = "false"

		if sIndiaUnited == "true":
			sCiv = "CIVILIZATION_INDIA_UNITED"

##################################################################################################
##
##################################################################################################

		sCapital = "Exile"
		if pPlayer.getNumCities() > 0: sCapital = pPlayer.getCapitalCity().getName()
		sDesc = CivInfo.getDescription()
		sShort = CivInfo.getShortDescription(0)
		sAdj = CivInfo.getAdjective(0)
		sFlag = pPlayer.getFlagDecal()
		sFlagY = -1
		iCivCounter = 0	
		
		lNewName = ["", "", "", "", "", "", ""]

		for Conditions in Dynamic[sCiv]:
			bFulfill = self.checkRequirements(Conditions[0], iPlayer)
			if bFulfill:
				for i in xrange(len(lNewName)):
					if lNewName[i]: continue
					sInput = Conditions[1][i]
					if sInput == "KEY_DESC":
						sText = sDesc
					elif sInput == "KEY_SHORT":
						sText = sShort
					elif sInput == "KEY_ADJ":
						sText = sAdj
					elif sInput == "KEY_CAPITAL":
						sText = sCapital
					elif sInput == "KEY_RELIGION":	
						if pPlayer.getStateReligion() > -1:
							sText = CyTranslator().getText(str(pPlayer.getStateReligionKey() + "_ADJECTIVE"),())	
						else:
							sText = CyTranslator().getText("TXT_KEY_PAGAN_THEO",())
					else:
						sText = CyTranslator().getText(Conditions[1][i], ())
					if sText:
						lNewName[i] = sText
				if lNewName[1]:
					break

		if not gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_DYNAMIC_CIV_NAMES):
			if lNewName[1]:
				sDesc = lNewName[1]
			if lNewName[3]:
				sShort = lNewName[3]
			if lNewName[4]:
				sAdj = lNewName[4]
			if lNewName[0]:
				sDesc = lNewName[0] + " " + sDesc
			if lNewName[2]:
				sDesc = sDesc + " " + lNewName[2]

##################################################################################################
## Multiple civ test
##################################################################################################

			if iPlayer > 0: 
				for iPlayerX in xrange(0,iPlayer):				## Let's check if more of the same civ have been spawned before
					pPlayerX = gc.getPlayer(iPlayerX)
					if pPlayerX.isNone(): continue
					if pPlayerX.isBarbarian(): continue
					if not pPlayerX.isAlive(): continue
					iCivX = pPlayerX.getCivilizationType()
					CivInfoX = gc.getCivilizationInfo(iCivX)
					sCivX = CivInfoX.getType()
					if str(sCiv) == str(sCivX):
						iCivCounter += 1

				if iCivCounter == 1:							## If it's the second civ of its type 
					print ("Second Civ Initiated")
					iPlayerY = -1
					for iPlayerX in xrange(0,iPlayer):
						pPlayerX = gc.getPlayer(iPlayerX)
						if pPlayerX.isNone(): continue
						if pPlayerX.isBarbarian(): continue
						if not pPlayerX.isAlive(): continue
						iCivX = pPlayerX.getCivilizationType()
						CivInfoX = gc.getCivilizationInfo(iCivX)
						sCivX = CivInfoX.getType()
						if str(sCiv) == str(sCivX):
							iPlayerY = iPlayerX
							
					if iPlayerY > -1:	
						print ("Second Civ Player Found")
						pPlayerY = gc.getPlayer(iPlayerY)
						iCivY = pPlayerY.getCivilizationType()
						CivInfoY = gc.getCivilizationInfo(iCivY)
						sDescY = str(pPlayerY.getCivilizationDescription(1))
						sFlagY = pPlayerY.getFlagDecal()
						print (sDesc)
						print (sDescY)
						if (str(sDesc) == str(sDescY)):
							if pPlayerY.getNumCities() > 0:	plotOld = pPlayerY.getCapitalCity().plot()
							else: plotOld = pPlayerY.getStartingPlot()

							if pPlayer.getNumCities() > 0: plotNew = pPlayer.getCapitalCity().plot()	
							else: plotNew = pPlayer.getStartingPlot()
							
							plotOldX = plotOld.getX()
							plotOldY = plotOld.getY()
							plotNewX = plotNew.getX()
							plotNewY = plotNew.getY()

							
							if abs(plotOldX - plotNewX) > abs(plotOldY - plotNewY):
								if plotOldX > plotNewX:
									sDesc = "West " + sDesc
									sShort = "West " + sShort
									sAdj = "West " + sAdj
								else:
									sDesc = "East " + sDesc
									sShort = "East " + sShort
									sAdj = "East " + sAdj
							else:
								if plotOldY > plotNewY:
									sDesc = "South " + sDesc
									sShort = "South " + sShort
									sAdj = "South " + sAdj
								else:
									sDesc = "North " + sDesc
									sShort = "North " + sShort
									sAdj = "North " + sAdj

				elif iCivCounter == 2:						## If there is already more than one
					print ("Third Civ Initiated")
					sDesc = "New " + sDesc
					sShort = "New " + sShort
					sAdj = "New " + sAdj
					
				elif iCivCounter > 2:
					print ("Nth Civ Initiated")
					sDesc = str(iCivCounter+1) + "th " + sDesc
					sShort = str(iCivCounter+1) + "th " + sShort
					sAdj = str(iCivCounter+1) + "th " + sAdj

##################################################################################################
##
##################################################################################################					
					
			pPlayer.setCivName(sDesc, sShort, sAdj)

##################################################################################################
## Multiple civ flag test
##################################################################################################

		if gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_DYNAMIC_CIV_NAMES) and not gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_DYNAMIC_CIV_FLAGS):
			iCivCounter = 0								

			if iPlayer > 0: 
				for iPlayerX in xrange(0,iPlayer):
					pPlayerX = gc.getPlayer(iPlayerX)
					if pPlayerX.isNone(): continue
					if pPlayerX.isBarbarian(): continue
					if not pPlayerX.isAlive(): continue
					iCivX = pPlayerX.getCivilizationType()
					CivInfoX = gc.getCivilizationInfo(iCivX)
					sCivX = CivInfoX.getType()
					if sCiv == sCivX:
						iCivCounter += 1
				
				if iCivCounter == 1:							## If it's the second civ of its type 
					print ("Second Civ Flag Initiated")
					iPlayerY = -1
					for iPlayerX in xrange(0,iPlayer):
						pPlayerX = gc.getPlayer(iPlayerX)
						if pPlayerX.isNone(): continue
						if pPlayerX.isBarbarian(): continue
						if not pPlayerX.isAlive(): continue
						iCivX = pPlayerX.getCivilizationType()
						CivInfoX = gc.getCivilizationInfo(iCivX)
						sCivX = CivInfoX.getType()
						if sCiv == sCivX:
							iPlayerY = iPlayerX
							return 
					if iPlayerY > -1:	
						pPlayerY = gc.getPlayer(iPlayerY)
						sFlagY = pPlayerY.getFlagDecal()

##################################################################################################
##
##################################################################################################

		if lNewName[5]: 
			if not gc.getGame().isOption(GameOptionTypes.GAMEOPTION_NO_DYNAMIC_CIV_FLAGS):
				sFlag = sFlag[:-7] + lNewName[5] + ".dds"
		else:
			sFlag = sFlag[:-7] + "def.dds"
			gc.getPlayer(iPlayer).setWhiteFlag(true)
		
		if (iCivCounter == 1) and (str(sFlagY) == str(sFlag)):
			print ("Second Civ Flag Created")
			sFlag = sFlag[:-7] + "gen.dds"
			gc.getPlayer(iPlayer).setWhiteFlag(false)
			lNewName[6] = 2
		elif iCivCounter > 1:
			print ("Third or More Civ Flag Created")
			sFlag = sFlag[:-7] + "gen.dds"
			print (str(sFlag))
			gc.getPlayer(iPlayer).setWhiteFlag(false)
			lNewName[6] = 2
			
		pPlayer.setFlagDecal(sFlag, true)
		CyInterface().setDirty(InterfaceDirtyBits.Flag_DIRTY_BIT, True)
		
		if lNewName[6]:
			if int(lNewName[6]) == 1:
				gc.getPlayer(iPlayer).setWhiteFlag(true)
			elif int(lNewName[6]) == 2:
				gc.getPlayer(iPlayer).setWhiteFlag(false)

	def checkRequirements(self, lRequirements, iPlayer):
		pPlayer = gc.getPlayer(iPlayer)
		for sRequirement in lRequirements:
			iRequirement = gc.getInfoTypeForString(sRequirement)
			if sRequirement.find("RELIGION_") == 0:
				if pPlayer.getStateReligion() != iRequirement:
					return False
			elif sRequirement.find("LEADER_") == 0:
				if pPlayer.getLeaderType() != iRequirement:
					return False
			elif sRequirement.find("BUILDINGCLASS_") == 0:
				if pPlayer.getBuildingClassCount(iRequirement) < 1:
					return False		
			elif sRequirement.find("CIVIC_") == 0:
				if not pPlayer.isCivic(iRequirement):
					return False
			elif sRequirement.find("ERA_") == 0:
				if pPlayer.getCurrentEra() < iRequirement:
					return False
			elif sRequirement.find("STATE_") == 0:
				if sRequirement == "STATE_VASSAL":
					if not gc.getTeam(pPlayer.getTeam()).isAVassal():
						return False
				elif sRequirement == "STATE_GOLDEN":
					if not pPlayer.isGoldenAge():
						return False
				elif sRequirement == "STATE_ANARCHY":
					if not pPlayer.isAnarchy():
						return False
				elif sRequirement == "STATE_MINOR":
					if not pPlayer.isMinorCiv():
						return False
			elif sRequirement.find("CITY_MAX") == 0:
				iNum = int(sRequirement[-3:])
				if pPlayer.getNumCities() > iNum:
					return False
			elif sRequirement.find("CITY_MIN") == 0:
				iNum = int(sRequirement[-3:])
				if pPlayer.getNumCities() < iNum:
					return False
		return True
		