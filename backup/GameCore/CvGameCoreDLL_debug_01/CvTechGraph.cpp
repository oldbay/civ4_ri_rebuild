// MOD - START - Tech Graph

//#include "CvGameCoreDLL.h"
#include "CvTechGraph.h"

//======================================================================================================
//					CvTechGraph
//======================================================================================================

CvTechGraph::CvTechGraph() :
m_bInitialized(false),
m_bReady(false),
m_eCivilization(NO_CIVILIZATION),
m_ePlayer(NO_PLAYER)
{
}

CvTechGraph::~CvTechGraph()
{
	uninit();
}

void CvTechGraph::init()
{
	if (GC.getNumTechInfos() == 0)
	{
		return;
	}

	// Group the techs by column index
	std::vector< std::pair< int, TechTypes > > aGroupedTechs;
	aGroupedTechs.reserve(GC.getNumTechInfos());
	for (int i=0; i < GC.getNumTechInfos(); ++i)
	{
		aGroupedTechs.push_back(std::make_pair(GC.getTechInfo((TechTypes)i).getGridX(), (TechTypes)i));
	}
	std::sort(aGroupedTechs.begin(), aGroupedTechs.end());

	// Don't allocate anything if there are no techs
	if (aGroupedTechs.size() == 0)
		return;

	// Allocate columns
	int iNumColumns = aGroupedTechs[aGroupedTechs.size() - 1].first;
	m_aColumns.resize(iNumColumns + 1, NULL);

	for (int i=0; i <= iNumColumns; i++)
	{
		m_aColumns[i] = new CvTechGraphColumn();
	}

	// Allocate tech arrays
	{
		int iTechCount = 0;
		int iPreviousColumn = -1;

		for (int i=0; i < GC.getNumTechInfos(); ++i)
		{
			int iColumn = aGroupedTechs[i].first;

			if (iColumn != iPreviousColumn)
			{
				if (iPreviousColumn >= 0)
				{
					m_aColumns[iPreviousColumn]->m_iNumTechs = iTechCount;
					m_aColumns[iPreviousColumn]->m_peTechs = new TechTypes[iTechCount];
				}

				iPreviousColumn = iColumn;
				iTechCount = 0;
			}
			iTechCount++;
		}

		m_aColumns[iPreviousColumn]->m_iNumTechs = iTechCount;
		m_aColumns[iPreviousColumn]->m_peTechs = new TechTypes[iTechCount];
	}

	// Set tech values
	{
		int iTechIndex = 0;
		int iPreviousColumn = -1;

		for (int i=0; i < GC.getNumTechInfos(); ++i)
		{
			int iColumn = aGroupedTechs[i].first;
			TechTypes eTech = aGroupedTechs[i].second;

			if (iColumn != iPreviousColumn)
			{
				iPreviousColumn = iColumn;
				iTechIndex = 0;
			}

			m_aColumns[iPreviousColumn]->m_peTechs[iTechIndex] = eTech;

			iTechIndex++;
		}
	}

	// Create panels
	m_aPanels.reserve(GC.getNumTechInfos());
	for (int i=0; i < GC.getNumTechInfos(); ++i)
	{
		m_aPanels.push_back(new CvTechGraphPanel((TechTypes)i));
	}

	m_bInitialized = true;
}

void CvTechGraph::uninit()
{
	for (std::vector<CvTechGraphColumn*>::iterator it = m_aColumns.begin(); it != m_aColumns.end(); ++it)
	{
		SAFE_DELETE(*it);
	}
	m_aColumns.clear();

	for (std::vector<CvTechGraphPanel*>::iterator it = m_aPanels.begin(); it != m_aPanels.end(); ++it)
	{
		SAFE_DELETE(*it);
	}
	m_aPanels.clear();

	m_bInitialized = false;
}

void CvTechGraph::update()
{
	update(m_eCivilization, m_ePlayer);
}

void CvTechGraph::update(CivilizationTypes eCivilization)
{
	update(eCivilization, NO_PLAYER);
}

void CvTechGraph::update(PlayerTypes ePlayer)
{
	if (ePlayer == NO_PLAYER)
		update(NO_CIVILIZATION, NO_PLAYER);
	else
		update(GET_PLAYER(ePlayer).getCivilizationType(), ePlayer);
}

void CvTechGraph::update(CivilizationTypes eCivilization, PlayerTypes ePlayer)
{
	if (!m_bInitialized)
	{
		init();
	}

	if (!m_bReady || m_eCivilization != eCivilization || m_ePlayer != ePlayer)
	{
		m_bReady = true;
		m_eCivilization = eCivilization;
		m_ePlayer = ePlayer;

		// Update panel icons first so that the columns can update based on the new numbers
		for (std::vector<CvTechGraphPanel*>::iterator it = m_aPanels.begin(); it != m_aPanels.end(); ++it)
		{
			(*it)->update(eCivilization, ePlayer);
		}

		// Update column icon counts
		for (std::vector<CvTechGraphColumn*>::iterator it = m_aColumns.begin(); it != m_aColumns.end(); ++it)
		{
			if ((*it) != NULL)
			{
				(*it)->update(this);
			}
		}
	}
}

CivilizationTypes CvTechGraph::getCivilization() const
{
	return m_eCivilization;
}

PlayerTypes CvTechGraph::getPlayer() const
{
	return m_ePlayer;
}

int CvTechGraph::getNumColumns() const
{
	return (int)m_aColumns.size();
}

CvTechGraphColumn* CvTechGraph::getColumn(int iColumnNumber) const
{
	FASSERT_BOUNDS(0, getNumColumns(), iColumnNumber, "CvTechGraph::getColumn");
	return m_aColumns[iColumnNumber];
}

CvTechGraphPanel* CvTechGraph::getPanel(TechTypes eTech) const
{
	FASSERT_BOUNDS(0, GC.getNumTechInfos(), eTech, "CvTechGraph::getPanel");
	return m_aPanels[eTech];
}





//======================================================================================================
//					CvTechGraphColumn
//======================================================================================================

CvTechGraphColumn::CvTechGraphColumn() :
m_iNumIcons(0),
m_iNumTechs(0),
m_peTechs(NULL)
{
}

CvTechGraphColumn::~CvTechGraphColumn()
{
	SAFE_DELETE_ARRAY(m_peTechs);
}

void CvTechGraphColumn::update(CvTechGraph* pTechGraph)
{
	m_iNumIcons = 0;
	for (int i = 0; i < m_iNumTechs; i++)
	{
		CvTechGraphPanel* pPanel = pTechGraph->getPanel(getTech(i));
		m_iNumIcons = std::max(m_iNumIcons, pPanel->getNumIcons());
	}
}

int CvTechGraphColumn::getNumIcons() const
{
	return m_iNumIcons;
}

int CvTechGraphColumn::getNumTechs() const
{
	return m_iNumTechs;
}

TechTypes CvTechGraphColumn::getTech(int iTechNumber) const
{
	FASSERT_BOUNDS(0, m_iNumTechs, iTechNumber, "CvTechGraphColumn::getTech");
	return m_peTechs ? m_peTechs[iTechNumber] : NO_TECH;
}





//======================================================================================================
//					CvTechGraphPanel
//======================================================================================================

CvTechGraphPanel::CvTechGraphPanel(TechTypes eTech) :
m_eTech(eTech)
{
}

CvTechGraphPanel::~CvTechGraphPanel()
{
	m_aIcons.clear();
}

// MOD - START - Relation Caching
// Note: Relation caching is used throughout to speed up computation
void CvTechGraphPanel::update(CivilizationTypes eCivilization, PlayerTypes ePlayer)
{
	if (ePlayer != NO_PLAYER)
	{
		//FAssertMsg(false, "CvTechGraphPanel::update - START");
	}
	m_aIcons.resize(0); // Empty the vector without freeing memory
	if (ePlayer != NO_PLAYER)
	{
		//FAssertMsg(false, "CvTechGraphPanel::update - RESIZED");
	}

	const CvTechInfo& kTech = GC.getTechInfo(m_eTech);
	TeamTypes eTeam = (ePlayer != NO_PLAYER) ? GET_PLAYER(ePlayer).getTeam() : NO_TEAM;

	// Unlockable units...
	for (int iRel = 0; iRel < kTech.getNumRelatedUnitClasses(); iRel++)
	{
		UnitClassTypes eUnitClass = (UnitClassTypes) kTech.getRelatedUnitClasses(iRel);
		UnitTypes eLoopUnit;
		if (eCivilization != NO_CIVILIZATION)
		{
			eLoopUnit = (UnitTypes)GC.getCivilizationInfo(eCivilization).getCivilizationUnits(eUnitClass);
		}
		else
		{
			eLoopUnit = (UnitTypes)GC.getUnitClassInfo(eUnitClass).getDefaultUnitIndex();
		}

		if (eLoopUnit != NO_UNIT)
		{
			const CvUnitInfo& kUnit = GC.getUnitInfo(eLoopUnit);
			if (kUnit.getPrimaryPrereqTech() == m_eTech)
			{
				const TCHAR* szButton = (ePlayer != NO_PLAYER) ? GET_PLAYER(ePlayer).getUnitButton(eLoopUnit) : kUnit.getButton();
				m_aIcons.push_back(CvTechGraphIcon(szButton, WIDGET_PEDIA_JUMP_TO_UNIT, eLoopUnit, -1, true, false));
			}
		}
	}

	// Unlockable Buildings...
	for (int iRel = 0; iRel < kTech.getNumRelatedBuildingClasses(); iRel++)
	{
		BuildingClassTypes eBuildingClass = (BuildingClassTypes) kTech.getRelatedBuildingClasses(iRel);
		BuildingTypes eLoopBuilding;
		if (eCivilization != NO_CIVILIZATION)
		{
			eLoopBuilding = (BuildingTypes)GC.getCivilizationInfo(eCivilization).getCivilizationBuildings(eBuildingClass);
		}
		else
		{
			eLoopBuilding = (BuildingTypes)GC.getBuildingClassInfo(eBuildingClass).getDefaultBuildingIndex();
		}

		if (eLoopBuilding != NO_BUILDING)
		{
			const CvBuildingInfo& kBuilding = GC.getBuildingInfo(eLoopBuilding);
			if (kBuilding.getPrimaryPrereqTech() == m_eTech)
			{
				m_aIcons.push_back(CvTechGraphIcon(kBuilding.getButton(), WIDGET_PEDIA_JUMP_TO_BUILDING, eLoopBuilding, 1, true, false));
			}
		}
	}

	// Obsolete Buildings...
	for (int iRel = 0; iRel < kTech.getNumRelatedBuildingClasses(); iRel++)
	{
		BuildingClassTypes eBuildingClass = (BuildingClassTypes) kTech.getRelatedBuildingClasses(iRel);
		BuildingTypes eLoopBuilding;
		if (eCivilization != NO_CIVILIZATION)
		{
			eLoopBuilding = (BuildingTypes)GC.getCivilizationInfo(eCivilization).getCivilizationBuildings(eBuildingClass);
		}
		else
		{
			eLoopBuilding = (BuildingTypes)GC.getBuildingClassInfo(eBuildingClass).getDefaultBuildingIndex();
		}

		if (eLoopBuilding != NO_BUILDING)
		{
			const CvBuildingInfo& kBuilding = GC.getBuildingInfo(eLoopBuilding);
			if (kBuilding.getObsoleteTech() == m_eTech)
			{
				m_aIcons.push_back(CvTechGraphIcon(kBuilding.getButton(), WIDGET_HELP_OBSOLETE, eLoopBuilding, -1, false, true));
			}
		}
	}

	// Obsolete Bonuses...
	for (int iRel = 0; iRel < kTech.getNumRelatedBonuses(); iRel++)
	{
		BonusTypes eBonus = (BonusTypes) kTech.getRelatedBonuses(iRel);
		const CvBonusInfo& kBonus = GC.getBonusInfo(eBonus);
		if (kBonus.getTechObsolete() == m_eTech)
		{
			m_aIcons.push_back(CvTechGraphIcon(kBonus.getButton(), WIDGET_HELP_OBSOLETE_BONUS, eBonus, -1, false, true));
		}
	}

	// Obsolete Monastaries...
	for (int iSpecialBuilding = 0; iSpecialBuilding < GC.getNumSpecialBuildingInfos(); iSpecialBuilding++)
	{
		const CvSpecialBuildingInfo& kSpecialBuildingInfo = GC.getSpecialBuildingInfo((SpecialBuildingTypes)iSpecialBuilding);
		if (kSpecialBuildingInfo.getObsoleteTech() == m_eTech)
		{
			m_aIcons.push_back(CvTechGraphIcon(kSpecialBuildingInfo.getButton(), WIDGET_HELP_OBSOLETE_SPECIAL, iSpecialBuilding, -1, false, true));
		}
	}

	// Route movement change
	for (int iRoute = 0; iRoute < GC.getNumRouteInfos(); iRoute++)
	{
		const CvRouteInfo& kRoute = GC.getRouteInfo((RouteTypes)iRoute);
		if (kRoute.getTechMovementChange(m_eTech) != 0)
		{
			m_aIcons.push_back(CvTechGraphIcon(ARTFILEMGR.getInterfaceArtInfo("INTERFACE_TECH_MOVE_BONUS")->getPath(), WIDGET_HELP_MOVE_BONUS, m_eTech, -1, false, false));
		}
	}

	// Promotion Info
	for (int iPromotion = 0; iPromotion < GC.getNumPromotionInfos(); iPromotion++)
	{
		const CvPromotionInfo& kPromotion = GC.getPromotionInfo((PromotionTypes)iPromotion);
		if (kPromotion.getTechPrereq() == m_eTech)
		{
			m_aIcons.push_back(CvTechGraphIcon(kPromotion.getButton(), WIDGET_PEDIA_JUMP_TO_PROMOTION, iPromotion, -1, false, false));
		}
	}

	// Free unit
	if (kTech.getFirstFreeUnitClass() != NO_UNITCLASS)
	{
		UnitTypes eFreeUnit;

		if (eCivilization != NO_CIVILIZATION)
		{
			eFreeUnit = (UnitTypes)GC.getCivilizationInfo(eCivilization).getCivilizationUnits(kTech.getFirstFreeUnitClass());
		}
		else
		{
			eFreeUnit = (UnitTypes)GC.getUnitClassInfo((UnitClassTypes)kTech.getFirstFreeUnitClass()).getDefaultUnitIndex();
		}

		if (eFreeUnit != NO_UNIT)
		{
			if ((GC.getUnitInfo(eFreeUnit).getEspionagePoints() == 0) || !GC.getGame().isOption(GAMEOPTION_NO_ESPIONAGE))
			{
				const TCHAR* szButton;
				if (ePlayer != NO_PLAYER)
				{
					szButton = GET_PLAYER(ePlayer).getUnitButton(eFreeUnit);
				}
				else
				{
					szButton = GC.getUnitInfo(eFreeUnit).getButton();
				}
				m_aIcons.push_back(CvTechGraphIcon(szButton, WIDGET_HELP_FREE_UNIT, eFreeUnit, m_eTech, false, false));
			}
		}
	}

	// Feature production modifier
	if (kTech.getFeatureProductionModifier() != 0)
	{
		m_aIcons.push_back(CvTechGraphIcon(ARTFILEMGR.getInterfaceArtInfo("INTERFACE_TECH_FEATURE_PRODUCTION")->getPath(), WIDGET_HELP_FEATURE_PRODUCTION, m_eTech, -1, false, false));
	}

	// Worker speed
	if (kTech.getWorkerSpeedModifier() != 0)
	{
		m_aIcons.push_back(CvTechGraphIcon(ARTFILEMGR.getInterfaceArtInfo("INTERFACE_TECH_WORKER_SPEED")->getPath(), WIDGET_HELP_WORKER_RATE, m_eTech, -1, false, false));
	}

	// Trade Routes per City change
	if (kTech.getTradeRoutes() != 0)
	{
		m_aIcons.push_back(CvTechGraphIcon(ARTFILEMGR.getInterfaceArtInfo("INTERFACE_TECH_TRADE_ROUTES")->getPath(), WIDGET_HELP_TRADE_ROUTES, m_eTech, -1, false, false));
	}

	// MOD - START - Congestion
	// Congestion threshold change from this tech...
	if (kTech.getInsideCityCongestionThresholdChange() != 0)
	{
		m_aIcons.push_back(CvTechGraphIcon(ARTFILEMGR.getInterfaceArtInfo("INTERFACE_TECH_CONGESTION_INSIDE_CITY_CHANGE")->getPath(), WIDGET_CONGESTION_INSIDE_CITY_THRESHOLD_CHANGE, m_eTech, -1, false, false));
	}

	if (kTech.getOutsideCityCongestionThresholdChange() != 0)
	{
		m_aIcons.push_back(CvTechGraphIcon(ARTFILEMGR.getInterfaceArtInfo("INTERFACE_TECH_CONGESTION_OUTSIDE_CITY_CHANGE")->getPath(), WIDGET_CONGESTION_OUTSIDE_CITY_THRESHOLD_CHANGE, m_eTech, -1, false, false));
	}
	// MOD - END - Congestion

	// Health Rate bonus from this tech...
	if (kTech.getHealth() != 0)
	{
		m_aIcons.push_back(CvTechGraphIcon(ARTFILEMGR.getInterfaceArtInfo("INTERFACE_TECH_HEALTH")->getPath(), WIDGET_HELP_HEALTH_RATE, m_eTech, -1, false, false));
	}

	// MOD - START - Epidemics
	// Epidemic rate from this tech...
	if (kTech.getGlobalEpidemicModifier() != 0)
	{
		if (kTech.getGlobalEpidemicModifier() > 0)
		{
			m_aIcons.push_back(CvTechGraphIcon(ARTFILEMGR.getInterfaceArtInfo("EPIDEMIC_START")->getPath(), WIDGET_HELP_EPIDEMIC_PROBABILITY_GLOBAL_MODIFIER, m_eTech, -1, false, false));
		}
		else
		{
			m_aIcons.push_back(CvTechGraphIcon(ARTFILEMGR.getInterfaceArtInfo("EPIDEMIC_END")->getPath(), WIDGET_HELP_EPIDEMIC_PROBABILITY_GLOBAL_MODIFIER, m_eTech, -1, false, false));
		}
	}
	// MOD - END - Epidemics

	// Happiness Rate bonus from this tech...
	if (kTech.getHappiness() != 0)
	{
		m_aIcons.push_back(CvTechGraphIcon(ARTFILEMGR.getInterfaceArtInfo("INTERFACE_TECH_HAPPINESS")->getPath(), WIDGET_HELP_HAPPINESS_RATE, m_eTech, -1, false, false));
	}

	// Free Techs
	if (kTech.getFirstFreeTechs() > 0)
	{
		m_aIcons.push_back(CvTechGraphIcon(ARTFILEMGR.getInterfaceArtInfo("INTERFACE_TECH_FREETECH")->getPath(), WIDGET_HELP_FREE_TECH, m_eTech, -1, false, false));
	}

	// Line of Sight bonus...
	if (kTech.isExtraWaterSeeFrom())
	{
		m_aIcons.push_back(CvTechGraphIcon(ARTFILEMGR.getInterfaceArtInfo("INTERFACE_TECH_LOS")->getPath(), WIDGET_HELP_LOS_BONUS, m_eTech, -1, false, false));
	}

	// Map Center Bonus...
	if (kTech.isMapCentering())
	{
		m_aIcons.push_back(CvTechGraphIcon(ARTFILEMGR.getInterfaceArtInfo("INTERFACE_TECH_MAPCENTER")->getPath(), WIDGET_HELP_MAP_CENTER, m_eTech, -1, false, false));
	}

	// Map Reveal...
	if (kTech.isMapVisible())
	{
		m_aIcons.push_back(CvTechGraphIcon(ARTFILEMGR.getInterfaceArtInfo("INTERFACE_TECH_MAPREVEAL")->getPath(), WIDGET_HELP_MAP_REVEAL, m_eTech, -1, false, false));
	}

	// Map Trading
	if (kTech.isMapTrading())
	{
		m_aIcons.push_back(CvTechGraphIcon(ARTFILEMGR.getInterfaceArtInfo("INTERFACE_TECH_MAPTRADING")->getPath(), WIDGET_HELP_MAP_TRADE, m_eTech, -1, false, false));
	}

	// Tech Trading
	if ((kTech.isTechTrading() && (!(GC.getGameINLINE().isOption(GAMEOPTION_NO_TECH_TRADING)))))
	{
		m_aIcons.push_back(CvTechGraphIcon(ARTFILEMGR.getInterfaceArtInfo("INTERFACE_TECH_TECHTRADING")->getPath(), WIDGET_HELP_TECH_TRADE, m_eTech, -1, false, false));
	}

	// Gold Trading
	if (kTech.isGoldTrading())
	{
		m_aIcons.push_back(CvTechGraphIcon(ARTFILEMGR.getInterfaceArtInfo("INTERFACE_TECH_GOLDTRADING")->getPath(), WIDGET_HELP_GOLD_TRADE, m_eTech, -1, false, false));
	}

	// Open Borders
	if (kTech.isOpenBordersTrading())
	{
		m_aIcons.push_back(CvTechGraphIcon(ARTFILEMGR.getInterfaceArtInfo("INTERFACE_TECH_OPENBORDERS")->getPath(), WIDGET_HELP_OPEN_BORDERS, m_eTech, -1, false, false));
	}

	// Defensive Pact
	if ((kTech.isDefensivePactTrading()) && (GC.getGameINLINE().isOption(GAMEOPTION_ALLIANCES)))
	{
		m_aIcons.push_back(CvTechGraphIcon(ARTFILEMGR.getInterfaceArtInfo("INTERFACE_TECH_DEFENSIVEPACT")->getPath(), WIDGET_HELP_DEFENSIVE_PACT, m_eTech, -1, false, false));
	}

	// Permanent Alliance
	if ((kTech.isPermanentAllianceTrading()) && (GC.getGameINLINE().isOption(GAMEOPTION_PERMANENT_ALLIANCES)))
	{
		m_aIcons.push_back(CvTechGraphIcon(ARTFILEMGR.getInterfaceArtInfo("INTERFACE_TECH_PERMALLIANCE")->getPath(), WIDGET_HELP_PERMANENT_ALLIANCE, m_eTech, -1, false, false));
	}

	// Vassal States
	if (kTech.isVassalStateTrading())
	{
		m_aIcons.push_back(CvTechGraphIcon(ARTFILEMGR.getInterfaceArtInfo("INTERFACE_TECH_VASSAL")->getPath(), WIDGET_HELP_VASSAL_STATE, m_eTech, -1, false, false));
	}

	// Bridge Building
	if (kTech.isBridgeBuilding())
	{
		m_aIcons.push_back(CvTechGraphIcon(ARTFILEMGR.getInterfaceArtInfo("INTERFACE_TECH_BRIDGEBUILDING")->getPath(), WIDGET_HELP_BUILD_BRIDGE, m_eTech, -1, false, false));
	}

	// Irrigation unlocked...
	if (kTech.isIrrigation())
	{
		m_aIcons.push_back(CvTechGraphIcon(ARTFILEMGR.getInterfaceArtInfo("INTERFACE_TECH_IRRIGATION")->getPath(), WIDGET_HELP_IRRIGATION, m_eTech, -1, false, false));
	}

	// Ignore Irrigation unlocked...
	if (kTech.isIgnoreIrrigation())
	{
		m_aIcons.push_back(CvTechGraphIcon(ARTFILEMGR.getInterfaceArtInfo("INTERFACE_TECH_NOIRRIGATION")->getPath(), WIDGET_HELP_IGNORE_IRRIGATION, m_eTech, -1, false, false));
	}

	// Coastal Work unlocked...
	if (kTech.isWaterWork())
	{
		m_aIcons.push_back(CvTechGraphIcon(ARTFILEMGR.getInterfaceArtInfo("INTERFACE_TECH_WATERWORK")->getPath(), WIDGET_HELP_WATER_WORK, m_eTech, -1, false, false));
	}

	// Improvements
	for (int iBuild = 0; iBuild < GC.getNumBuildInfos(); iBuild++)
	{
		const CvBuildInfo& kBuild = GC.getBuildInfo((BuildTypes)iBuild);

		bool bTechFound = false;

		if (kBuild.getTechPrereq() == NO_TECH)
		{
			for (int iFeature = 0; iFeature < GC.getNumFeatureInfos(); iFeature++)
			{
				if (kBuild.getFeatureTech(iFeature) == m_eTech)
				{
					bTechFound = true;
					break;
				}
			}
		}
		else if (kBuild.getTechPrereq() == m_eTech)
		{
			bTechFound = true;
		}

		if (bTechFound)
		{
			// MOD - START - Unique Improvements
			//m_aIcons.push_back(kBuild.getButton(), WIDGET_HELP_IMPROVEMENT, m_eTech, iBuild, false, false));
			if ((eCivilization == NO_CIVILIZATION) || GC.getCivilizationInfo(eCivilization).canBuild(iBuild))
			{
				m_aIcons.push_back(CvTechGraphIcon(kBuild.getButton(), WIDGET_HELP_IMPROVEMENT, m_eTech, iBuild, false, false));
			}
			// MOD - END - Unique Improvements
		}
	}

	// Domain Extra Moves
	for (int iDomain = 0; iDomain < NUM_DOMAIN_TYPES; iDomain++)
	{
		if (kTech.getDomainExtraMoves(iDomain) != 0)
		{
			m_aIcons.push_back(CvTechGraphIcon(ARTFILEMGR.getInterfaceArtInfo("INTERFACE_TECH_WATERMOVES")->getPath(), WIDGET_HELP_DOMAIN_EXTRA_MOVES, m_eTech, iDomain, false, false));
		}
	}

	// K-Mod. Commerce modifiers
	for (int iCommerce = 0; iCommerce < NUM_COMMERCE_TYPES; iCommerce++)
	{
		if (kTech.getCommerceModifier(iCommerce) != 0)
		{
			const CvCommerceInfo& kCommerceInfo = GC.getCommerceInfo((CommerceTypes)iCommerce);
			m_aIcons.push_back(CvTechGraphIcon(kCommerceInfo.getButton(), WIDGET_HELP_GLOBAL_COMMERCE_MODIFIER, m_eTech, iCommerce, false, false));
		}
	}

	// K-Mod. Extra specialist commerce
	for (int iCommerce = 0; iCommerce < NUM_COMMERCE_TYPES; iCommerce++)
	{
		if (kTech.getAllSpecialistCommerce(iCommerce) != 0)
		{
			if (GC.getDefineINT("DEFAULT_SPECIALIST") != NO_SPECIALIST)
			{
				const CvSpecialistInfo& kSpecialistInfo = GC.getSpecialistInfo((SpecialistTypes)GC.getDefineINT("DEFAULT_SPECIALIST"));
				m_aIcons.push_back(CvTechGraphIcon(kSpecialistInfo.getButton(), WIDGET_HELP_EXTRA_SPECIALIST_COMMERCE, m_eTech, iCommerce, false, false));
			}
		}
	}

	// Adjustments
	for (int iCommerce = 0; iCommerce < NUM_COMMERCE_TYPES; iCommerce++)
	{
		CommerceTypes eCommerce = (CommerceTypes)iCommerce;
		if (kTech.isCommerceFlexible(eCommerce) && (eTeam == NO_TEAM || !GET_TEAM(eTeam).isCommerceFlexible(eCommerce)))
		{
			const TCHAR* szTexture;
			if ( eCommerce == COMMERCE_CULTURE )
			{
				szTexture = ARTFILEMGR.getInterfaceArtInfo("INTERFACE_TECH_CULTURE")->getPath();
			}
			else if ( eCommerce == COMMERCE_ESPIONAGE )
			{
				szTexture = ARTFILEMGR.getInterfaceArtInfo("INTERFACE_TECH_ESPIONAGE")->getPath();
			}
			else
			{
				szTexture = ARTFILEMGR.getInterfaceArtInfo("INTERFACE_GENERAL_QUESTIONMARK")->getPath();
			}
			m_aIcons.push_back(CvTechGraphIcon(szTexture, WIDGET_HELP_ADJUST, m_eTech, iCommerce, false, false));
		}
	}

	// Terrain opens up as a trade route
	for (int iTerrain = 0; iTerrain < GC.getNumTerrainInfos(); iTerrain++)
	{
		TerrainTypes eTerrain = (TerrainTypes) iTerrain;
		if (kTech.isTerrainTrade(eTerrain) && (eTeam == NO_TEAM || !GET_TEAM(eTeam).isTerrainTrade(eTerrain)))
		{
			m_aIcons.push_back(CvTechGraphIcon(ARTFILEMGR.getInterfaceArtInfo("INTERFACE_TECH_WATERTRADE")->getPath(), WIDGET_HELP_TERRAIN_TRADE, m_eTech, iTerrain, false, false));
		}
	}

	if (kTech.isRiverTrade() && (eTeam == NO_TEAM || !GET_TEAM(eTeam).isRiverTrade()))
	{
		m_aIcons.push_back(CvTechGraphIcon(ARTFILEMGR.getInterfaceArtInfo("INTERFACE_TECH_RIVERTRADE")->getPath(), WIDGET_HELP_TERRAIN_TRADE, m_eTech, GC.getNumTerrainInfos(), false, false));
	}

	// Special buildings like monestaries...
	for (int iSpecialBuilding = 0; iSpecialBuilding < GC.getNumSpecialBuildingInfos(); iSpecialBuilding++)
	{
		const CvSpecialBuildingInfo& kSpecialBuildingInfo = GC.getSpecialBuildingInfo((SpecialBuildingTypes)iSpecialBuilding);
		if (kSpecialBuildingInfo.getTechPrereq() == m_eTech)
		{
			m_aIcons.push_back(CvTechGraphIcon(kSpecialBuildingInfo.getButton(), WIDGET_HELP_SPECIAL_BUILDING, m_eTech, iSpecialBuilding, false, false));
		}
	}

	// Yield change
	for (int iRel = 0; iRel < kTech.getNumRelatedImprovements(); iRel++)
	{
		ImprovementTypes eImprovement = (ImprovementTypes) kTech.getRelatedImprovements(iRel);
		const CvImprovementInfo& kImprovement = GC.getImprovementInfo(eImprovement);

		for (int iYield = 0; iYield < NUM_YIELD_TYPES; iYield++)
		{
			// MOD - START - Unique Improvements
			if (kImprovement.getTechYieldChanges(m_eTech, iYield))
			{
				if ((eCivilization == NO_CIVILIZATION) || GC.getCivilizationInfo(eCivilization).canProduceImprovement(eImprovement))
				{
					m_aIcons.push_back(CvTechGraphIcon(kImprovement.getButton(), WIDGET_HELP_YIELD_CHANGE, m_eTech, eImprovement, false, false));
				}
				break;
			}

			// MOD - START - Advanced Yield and Commerce Effects
			if (kImprovement.getTechIrrigatedYieldChanges(m_eTech, iYield))
			{
				if ((eCivilization == NO_CIVILIZATION) || GC.getCivilizationInfo(eCivilization).canProduceImprovement(eImprovement))
				{
					m_aIcons.push_back(CvTechGraphIcon(kImprovement.getButton(), WIDGET_HELP_IRRIGATED_YIELD_CHANGE, m_eTech, eImprovement, false, false));
				}
				break;
			}
			// MOD - END - Advanced Yield and Commerce Effects
			// MOD - END - Unique Improvements
		}
	}

	// Bonuses revealed
	for (int iRel = 0; iRel < kTech.getNumRelatedBonuses(); iRel++)
	{
		BonusTypes eBonus = (BonusTypes) kTech.getRelatedBonuses(iRel);
		const CvBonusInfo& kBonus = GC.getBonusInfo(eBonus);
		if (kBonus.getTechReveal() == m_eTech)
		{
			m_aIcons.push_back(CvTechGraphIcon(kBonus.getButton(), WIDGET_HELP_BONUS_REVEAL, m_eTech, eBonus, false, false));
		}
	}

	// Civic options
	for (int iCivic = 0; iCivic < GC.getNumCivicInfos(); iCivic++)
	{
		const CvCivicInfo& kCivic = GC.getCivicInfo((CivicTypes)iCivic);
		if (kCivic.getTechPrereq() == m_eTech)
		{
			m_aIcons.push_back(CvTechGraphIcon(kCivic.getButton(), WIDGET_HELP_CIVIC_REVEAL, m_eTech, iCivic, false, false));
		}
	}

	// Projects possible
	for (int iProject = 0; iProject < GC.getNumProjectInfos(); iProject++)
	{
		const CvProjectInfo& kProject = GC.getProjectInfo((ProjectTypes)iProject);
		if (kProject.getTechPrereq() == m_eTech)
		{
			m_aIcons.push_back(CvTechGraphIcon(kProject.getButton(), WIDGET_PEDIA_JUMP_TO_PROJECT, iProject, 1, false, false));
		}
	}

	// Processes possible
	for (int iProcess = 0; iProcess < GC.getNumProcessInfos(); iProcess++)
	{
		const CvProcessInfo& kProcess = GC.getProcessInfo((ProcessTypes)iProcess);
		if (kProcess.getTechPrereq() == m_eTech)
		{
			m_aIcons.push_back(CvTechGraphIcon(kProcess.getButton(), WIDGET_HELP_PROCESS_INFO, m_eTech, iProcess, false, false));
		}
	}

	// Religions unlocked
	for (int iReligion = 0; iReligion < GC.getNumReligionInfos(); iReligion++)
	{
		const CvReligionInfo& kReligion = GC.getReligionInfo((ReligionTypes)iReligion);
		if (kReligion.getTechPrereq() == m_eTech)
		{
			const TCHAR* szTexture;
			if (GC.getGame().isOption(GAMEOPTION_PICK_RELIGION))
			{
				szTexture = ARTFILEMGR.getInterfaceArtInfo("INTERFACE_POPUPBUTTON_RELIGION")->getPath();
			}
			else
			{
				szTexture = kReligion.getButton();
			}
			m_aIcons.push_back(CvTechGraphIcon(szTexture, WIDGET_HELP_FOUND_RELIGION, m_eTech, iReligion, false, false));
		}
	}

	// Corporations unlocked
	for (int iCorp = 0; iCorp < GC.getNumCorporationInfos(); iCorp++)
	{
		const CvCorporationInfo& kCorp = GC.getCorporationInfo((CorporationTypes)iCorp);
		if (kCorp.getTechPrereq() == m_eTech)
		{
			m_aIcons.push_back(CvTechGraphIcon(kCorp.getButton(), WIDGET_HELP_FOUND_CORPORATION, m_eTech, iCorp, false, false));
		}
	}
	
	if (ePlayer != NO_PLAYER)
	{
		//FAssertMsg(false, "CvTechGraphPanel::update - END");
	}
}
// MOD - END - Relation Caching

TechTypes CvTechGraphPanel::getTech() const
{
	return m_eTech;
}

int CvTechGraphPanel::getNumIcons() const
{
	return (int)m_aIcons.size();
}

TechGraphIconArray CvTechGraphPanel::getIcons() const
{
	return m_aIcons;
}

CvTechGraphIcon CvTechGraphPanel::getIcon(int i) const
{
	FASSERT_BOUNDS(0, getNumIcons(), i, "CvTechGraphPanel::getIcon");
	return m_aIcons[i];
}
// MOD - END - Tech Graph
