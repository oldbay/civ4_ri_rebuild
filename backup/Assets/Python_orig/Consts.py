# Constants of Realism Invictus, by AbsintheRed
# a file containing the IDs of various objects, for easy access/comparison
# this is usually the order of those in the various XML files, as the .dll uses that in most cases

from CvPythonExtensions import *

# Civilizations: initialize player variables to player IDs (note that the civ order can be set in the WBS file, so it's not the base XML order for scenarios)
iNumPlayableCivs = 34
(iUnitedStates, iArabia, iArmenia, iAustronesia, iAztec, iBerber, iCarthage, iCelts, iChina, iDravida,
iEgypt, iEngland, iEthiopia, iFrance, iGermany, iGreece, iHindi, iHungary, iInca, iJapan,
iKorea, iMali, iMaya, iMongol, iNguni, iPersia, iPoland, iRome, iRussia, iViking, iSouthChina,
iSpain, iTransoxiana, iTurkey) = range(iNumPlayableCivs)

#Bonuses: initialize bonus variables to bonus indices from XML
iNumBonus = 74
(iBauxite, iCoal, iCopper, iHorse, iIron, iMarble, iOil, iStone, iUranium, iBanana,
iClam, iCorn, iCow, iCrab, iDeer, iFish, iPig, iRice, iSheep, iWheat,
iDye, iFur, iGems, iGold, iIncense, iIvory, iSilk, iSilver, iSpices, iSugar,
iWine, iWhale, iDrama, iMusic, iMovies, iHemp, iSalt, iPotato, iCoffee, iSulphur,
iPearls, iCitrus, iCotton, iTuna, iAlcohol, iPrimeTimber, iNaturalGas, iBronze, iSteel, iFabric,
iFireArms, iFireArms2, iFireArms3, iArtillery, iFuel, iElectricGear, iNuclearFuel, iCars, iPlastic, iMicroChips,
iHomeAppliances, iFurniture, iCannedFood, iMissiles, iClothes, iTobacco, iAmber, iNavalSupplies, iPills, iMasonryMaterials,
iFertilizer, iCement, iAluminium, iGunpowder) = range(iNumBonus)

#Improvements: initialize improvement variables to improvement indices from XML
iNumImprovements = 90
(iImpLandWorked, iImpDepletedLand, iImpWaterWorked, iImpCityRuins, iImpGoodyHut, iImpFarm, iImpWuguFarmSChina, iImpCommunalFarmKorea, iImpPancakrstiFieldHindi, iImpFolwarkPoland, 
iImpKemetFarmEgypt, iImpSlaveFarm, iImpLatifundiaRome, iImpMechanizedFarm, iImpFishingBoats, iImpFishingVesselViking, iImpFishingFlotillaViking, iImpFishingFleetViking, iImpWhalingBoats, 
iImpWhalingVesselViking, iImpMine, iImpPreciousMine, iImpMineriaSpain, iImpCanalChina, iImpLumbermill, iImpWindmill, iImpAndenInca, iImpWatermill, iImpWaterWheelGreece, iImpPlantation, 
iImpCoffeePlantationEthiopia, iImpPepperPlantationDravida, iImpTradingColonyCarthage, iImpCommercialPlantationPersia, iImpCommercialPlantationTrans1, iImpCommercialPlantationTrans2, 
iImpCommercialPlantationTrans3, iImpIndustrialPlantation, iImpFoodCropsPlantation, iImpQuarry, iImpSaltPit, iImpPasture, iImpHorseBreederHungary, iImpRanchUSA, iImpCamp, iImpTrapperLodge, 
iImpLoggingCamp, iImpMahoutCamp, iImpOilWell, iImpGasWell, iImpOffshoreOil, iImpOffshoreGas, iImpWinery, iImpVineyardFrance, iImpCottage, iImpHamlet, iImpVillage, iImpTown, iImpFort, 
iImpForestPreserve, iImpHotSprings, iImpHotSpringsGerman, iImpSettlement, iImpKsarBerber, iImpBeduinCampArab, iImpHunterGathererCampBarb, iImpSlashBurnFarm, iImpGrazingGroundMongol, 
iImpAutobahnGerman, iImpCattleTrekNguni, iImpChinampaAztec, iImpLovischcheRussia, iImpPetKotMaya, iImpPanningSiteLargeMali, iImpPanningSiteSmallMali, iImpPanningSiteDepletedMali,
iImpFloatsamRecoveryEngland, iImpTraditionalFishingJapan, iImpTimarTurks, iImpFortMonasteryCelts, iImpPastureArmenia, iImpFishingAustronesia, iImpWheatCultivation, iImpCornCultivation,
iImpRiceCultivation, iImpPotatoCultivation, iImpGrapeCultivationFrance, iImpSpiceCultivationDravida, iImpCoffeeCultivationEthiopia, iImpDyeCultivationCarthage) = range(iNumImprovements)

#Civics:
iNumCivic = 40
(iTribalUnion, iAutocracy, iRepublic, iMonarchy, iTheocracy, iDemocracy, iDictatorship, iFederalism,
iRuleOfFear, iTraditionalCustom, iCivilService, iFeudalAristocracy, iPlutocracy, iRepresentation, iCollectivism, iSocialJustice,
iTribalism, iSlavery, iSerfdom, iCasteSystem, iFreeCommoners, iWorkingClass, iForcedLabor, iLaborUnion,
iSubstinence, iPastoralNomadism, GuildMonopoly, MerchantPrinces, Protectionism, FreeMarket, PlannedEconomy, WelfareState,
Paganism, Animism, CivilReligion, Monasticism, Pacifism, Militancy, FreeReligion, CultOfPersonality) = range(iNumCivic)

