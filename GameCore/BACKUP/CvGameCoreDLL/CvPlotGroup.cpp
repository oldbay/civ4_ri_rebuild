// plotGroup.cpp

#include "CvGameCoreDLL.h"
#include "CvPlotGroup.h"
#include "CvPlot.h"
#include "CvGlobals.h"
#include "CvPlayerAI.h"
#include "CvMap.h"
#include "CvCity.h"
#include "CvDLLFAStarIFaceBase.h"
#include "FProfiler.h"

// Public Functions...

CvPlotGroup::CvPlotGroup()
{
	m_paiNumBonuses = NULL;
	// MOD - START - Bonus Converters
	m_ppaiNumBonusConverterConsumedBonuses = NULL;
	// MOD - END - Bonus Converters

	reset(0, NO_PLAYER, true);
}


CvPlotGroup::~CvPlotGroup()
{
	uninit();
}


void CvPlotGroup::init(int iID, PlayerTypes eOwner, CvPlot* pPlot)
{
	//--------------------------------
	// Init saved data
	reset(iID, eOwner);

	//--------------------------------
	// Init non-saved data

	//--------------------------------
	// Init other game data
	addPlot(pPlot);
}


void CvPlotGroup::uninit()
{
	SAFE_DELETE_ARRAY(m_paiNumBonuses);

	// MOD - START - Bonus Converters
	if (m_ppaiNumBonusConverterConsumedBonuses != NULL)
	{
		for (int iI = 0; iI < GC.getNumBuildingInfos(); iI++)
		{
			SAFE_DELETE_ARRAY(m_ppaiNumBonusConverterConsumedBonuses[iI]);
		}
		SAFE_DELETE_ARRAY(m_ppaiNumBonusConverterConsumedBonuses);
	}
	// MOD - END - Bonus Converters

	m_plots.clear();

	// MOD - START - PlotGroup Cities
	m_cityPlots.clear();
	// MOD - END - PlotGroup Cities
}

// FUNCTION: reset()
// Initializes data members that are serialized.
void CvPlotGroup::reset(int iID, PlayerTypes eOwner, bool bConstructorCall)
{
	int iI;

	//--------------------------------
	// Uninit class
	uninit();

	m_iID = iID;
	m_eOwner = eOwner;

	if (!bConstructorCall)
	{
		FAssertMsg((0 < GC.getNumBonusInfos()), "GC.getNumBonusInfos() is not greater than zero but an array is being allocated in CvPlotGroup::reset");
		m_paiNumBonuses = new int [GC.getNumBonusInfos()];
		for (iI = 0; iI < GC.getNumBonusInfos(); iI++)
		{
			m_paiNumBonuses[iI] = 0;
		}
	}
}


void CvPlotGroup::addPlot(CvPlot* pPlot)
{
	XYCoords xy;

	xy.iX = pPlot->getX_INLINE();
	xy.iY = pPlot->getY_INLINE();

	insertAtEndPlots(xy);

	pPlot->setPlotGroup(getOwnerINLINE(), this);
}


void CvPlotGroup::removePlot(CvPlot* pPlot)
{
	CLLNode<XYCoords>* pPlotNode;

	pPlotNode = headPlotsNode();

	while (pPlotNode != NULL)
	{
		if (GC.getMapINLINE().plotSorenINLINE(pPlotNode->m_data.iX, pPlotNode->m_data.iY) == pPlot)
		{
			pPlot->setPlotGroup(getOwnerINLINE(), NULL);

			pPlotNode = deletePlotsNode(pPlotNode); // can delete this PlotGroup...
			break;
		}
		else
		{
			pPlotNode = nextPlotsNode(pPlotNode);
		}
	}
}


// MOD - START - PlotGroup Cities
void CvPlotGroup::addCity(CvCity* pCity)
{
	XYCoords xy;

	xy.iX = pCity->getX_INLINE();
	xy.iY = pCity->getY_INLINE();

	insertAtEndCityPlots(xy);
}


void CvPlotGroup::removeCity(CvCity* pCity)
{
	CvPlot* pCityPlot = pCity->plot();
	CLLNode<XYCoords>* pCityPlotNode = headCityPlotsNode();

	while (pCityPlotNode != NULL)
	{
		if (GC.getMapINLINE().plotSorenINLINE(pCityPlotNode->m_data.iX, pCityPlotNode->m_data.iY) == pCityPlot)
		{
			deleteCityPlotsNode(pCityPlotNode);
			return;
		}
		else
		{
			pCityPlotNode = nextCityPlotsNode(pCityPlotNode);
		}
	}

	FAssertMsg(false, "Attempted to remove city from plot group but city does not exist in plot group cities");
}
// MOD - END - PlotGroup Cities


void CvPlotGroup::recalculatePlots()
{
	PROFILE_FUNC();

	CLLNode<XYCoords>* pPlotNode;
	CvPlot* pPlot;
	CLinkList<XYCoords> oldPlotGroup;
	XYCoords xy;
	PlayerTypes eOwner;
	int iCount;

	eOwner = getOwnerINLINE();

	pPlotNode = headPlotsNode();

	if (pPlotNode != NULL)
	{
		pPlot = GC.getMapINLINE().plotSorenINLINE(pPlotNode->m_data.iX, pPlotNode->m_data.iY);

		iCount = 0;

		gDLL->getFAStarIFace()->SetData(&GC.getPlotGroupFinder(), &iCount);
		gDLL->getFAStarIFace()->GeneratePath(&GC.getPlotGroupFinder(), pPlot->getX_INLINE(), pPlot->getY_INLINE(), -1, -1, false, eOwner);

		if (iCount == getLengthPlots())
		{
			return;
		}
	}

	{
		PROFILE("CvPlotGroup::recalculatePlots update");

		oldPlotGroup.clear();

		pPlotNode = headPlotsNode();

		while (pPlotNode != NULL)
		{
			PROFILE("CvPlotGroup::recalculatePlots update 1");

			pPlot = GC.getMapINLINE().plotSorenINLINE(pPlotNode->m_data.iX, pPlotNode->m_data.iY);

			FAssertMsg(pPlot != NULL, "Plot is not assigned a valid value");

			xy.iX = pPlot->getX_INLINE();
			xy.iY = pPlot->getY_INLINE();

			oldPlotGroup.insertAtEnd(xy);

			pPlot->setPlotGroup(eOwner, NULL);

			pPlotNode = deletePlotsNode(pPlotNode); // will delete this PlotGroup...
		}

		pPlotNode = oldPlotGroup.head();

		while (pPlotNode != NULL)
		{
			PROFILE("CvPlotGroup::recalculatePlots update 2");

			pPlot = GC.getMapINLINE().plotSorenINLINE(pPlotNode->m_data.iX, pPlotNode->m_data.iY);

			FAssertMsg(pPlot != NULL, "Plot is not assigned a valid value");

			pPlot->updatePlotGroup(eOwner, true);

			pPlotNode = oldPlotGroup.deleteNode(pPlotNode);
		}
	}
}


int CvPlotGroup::getID() const
{
	return m_iID;
}


void CvPlotGroup::setID(int iID)
{
	m_iID = iID;
}


PlayerTypes CvPlotGroup::getOwner() const
{
	return getOwnerINLINE();
}


int CvPlotGroup::getNumBonuses(BonusTypes eBonus) const
{
	FAssertMsg(eBonus >= 0, "eBonus is expected to be non-negative (invalid Index)");
	FAssertMsg(eBonus < GC.getNumBonusInfos(), "eBonus is expected to be within maximum bounds (invalid Index)");
	return m_paiNumBonuses[eBonus];
}


bool CvPlotGroup::hasBonus(BonusTypes eBonus)
{
	return(getNumBonuses(eBonus) > 0);
}


void CvPlotGroup::changeNumBonuses(BonusTypes eBonus, int iChange)
{
	FAssertMsg(eBonus >= 0, "eBonus is expected to be non-negative (invalid Index)");
	FAssertMsg(eBonus < GC.getNumBonusInfos(), "eBonus is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		//iOldNumBonuses = getNumBonuses(eBonus);

		m_paiNumBonuses[eBonus] = (m_paiNumBonuses[eBonus] + iChange);

		// MOD - START - PlotGroup Bonus Efficiency
		// Note: The count can go negative if the capital city is exporting a bonus and the plot group is in the middle of recalculation
		// MOD - END - PlotGroup Bonus Efficiency
		//FAssertMsg(m_paiNumBonuses[eBonus] >= 0, "m_paiNumBonuses[eBonus] is expected to be non-negative (invalid Index)"); XXX

		// K-Mod note, m_paiNumBonuses[eBonus] is often temporarily negative while plot groups are being updated.
		// It's an unfortuante side effect of the way the update is implemented. ... and so this assert is invalid.
		// (This isn't my fault. I haven't changed it. It has always been like this.)

		// MOD - START - PlotGroup Bonus Efficiency
		/*
		CLLNode<XYCoords>* pPlotNode = headPlotsNode();

		while (pPlotNode != NULL)
		{
			CvCity* pCity = GC.getMapINLINE().plotSorenINLINE(pPlotNode->m_data.iX, pPlotNode->m_data.iY)->getPlotCity();

			if (pCity != NULL)
			{
				if (pCity->getOwnerINLINE() == getOwnerINLINE())
				{
					pCity->changeNumBonuses(eBonus, iChange);
				}
			}

			pPlotNode = nextPlotsNode(pPlotNode);
		}
		*/

		CLLNode<XYCoords>* pPlotNode = headCityPlotsNode();

		while (pPlotNode != NULL)
		{
			CvCity* pCity = GC.getMapINLINE().plotSorenINLINE(pPlotNode->m_data.iX, pPlotNode->m_data.iY)->getPlotCity();

			FAssertMsg(pCity != NULL, "A plot is without a city in a plot group's city plots list");
			FAssertMsg(pCity->getOwnerINLINE() == getOwnerINLINE(), "A city in a plot group's city list is not owned by the plot group owner");

			pCity->changeNumBonuses(eBonus, iChange);

			pPlotNode = nextCityPlotsNode(pPlotNode);
		}
		// MOD - END - PlotGroup Bonus Efficiency
	}
}


// MOD - START - PlotGroup Bonus Efficiency
void CvPlotGroup::changeNumBonuses(std::vector<CvBonusAmountChange>& aBonusChanges)
{
	CLLNode<XYCoords>* pPlotNode;
	CvCity* pCity;

	if (aBonusChanges.size() > 0)
	{
		for (std::vector<CvBonusAmountChange>::const_iterator it = aBonusChanges.begin(); it != aBonusChanges.end(); ++it)
		{
			FAssertMsg((*it).m_eBonus >= 0, "m_eBonus is expected to be non-negative (invalid Index)");
			FAssertMsg((*it).m_eBonus < GC.getNumBonusInfos(), "m_eBonus is expected to be within maximum bounds (invalid Index)");

			m_paiNumBonuses[(*it).m_eBonus] = (m_paiNumBonuses[(*it).m_eBonus] + (*it).m_iChange);
			// Note: The count can go negative if the capital city is exporting a bonus and the plot group is in the middle of recalculation
			//FAssertMsg(m_paiNumBonuses[(*it).m_eBonus] >= 0, "m_paiNumBonuses[eBonus] is expected to be non-negative");
		}

		pPlotNode = headCityPlotsNode();

		while (pPlotNode != NULL)
		{
			pCity = GC.getMapINLINE().plotSorenINLINE(pPlotNode->m_data.iX, pPlotNode->m_data.iY)->getPlotCity();

			FAssertMsg(pCity != NULL, "A plot is without a city in a plot group's city plots list");
			FAssertMsg(pCity->getOwnerINLINE() == getOwnerINLINE(), "A city in a plot group's city list is not owned by the plot group owner");

			for (std::vector<CvBonusAmountChange>::const_iterator it = aBonusChanges.begin(); it != aBonusChanges.end(); ++it)
			{
				FAssertMsg((*it).m_eBonus >= 0, "m_eBonus is expected to be non-negative (invalid Index)");
				FAssertMsg((*it).m_eBonus < GC.getNumBonusInfos(), "m_eBonus is expected to be within maximum bounds (invalid Index)");

				pCity->changeNumBonuses((*it).m_eBonus, (*it).m_iChange);
			}

			pPlotNode = nextCityPlotsNode(pPlotNode);
		}
	}
}
// MOD - END - PlotGroup Bonus Efficiency


// MOD - START - Bonus Converters
int CvPlotGroup::getNumBonusConverterConsumedBonusesByIndex(BuildingTypes eBuilding, int iConsumedBonusIndex) const
{
	FAssertMsg(eBuilding >= 0, "eBuilding expected to be >= 0");
	FAssertMsg(eBuilding < GC.getNumBuildingInfos(), "eBuilding is expected to be within maximum bounds (invalid Index)");
	FAssertMsg(iConsumedBonusIndex >= 0, "iConsumedBonusIndex is expected to be non-negative (invalid Index)");
	FAssertMsg(iConsumedBonusIndex < GC.getBuildingInfo(eBuilding).getNumConsumedBonuses(), "iConsumedBonusIndex is expected to be within maximum bounds (invalid Index)");

	if (m_ppaiNumBonusConverterConsumedBonuses == NULL)
	{
		return 0;
	}
	else
	{
		if (m_ppaiNumBonusConverterConsumedBonuses[eBuilding] == NULL)
		{
			return 0;
		}
		else
		{
			return m_ppaiNumBonusConverterConsumedBonuses[eBuilding][iConsumedBonusIndex];
		}
	}
}

int CvPlotGroup::getNumBonusConverterConsumedBonuses(BuildingTypes eBuilding, BonusTypes eBonus) const
{
	FAssertMsg(eBuilding >= 0, "eBuilding expected to be >= 0");
	FAssertMsg(eBuilding < GC.getNumBuildingInfos(), "eBuilding is expected to be within maximum bounds (invalid Index)");
	FAssertMsg(eBonus >= 0, "eBonus is expected to be non-negative (invalid Index)");
	FAssertMsg(eBonus < GC.getNumBonusInfos(), "eBonus is expected to be within maximum bounds (invalid Index)");

	int iConsumedBonusIndex = GC.getBuildingInfo(eBuilding).getConsumedBonusIndex(eBonus);
	FAssertMsg(iConsumedBonusIndex >= 0, "Query for bonus converter bonus that isn't consumed in getNumBonusConverterConsumedBonuses");
	return getNumBonusConverterConsumedBonusesByIndex(eBuilding, iConsumedBonusIndex);
}


void CvPlotGroup::changeNumBonusConverterConsumedBonuses(BuildingTypes eBuilding, BonusTypes eBonus, int iChange)
{
	FAssertMsg(eBuilding >= 0, "eBuilding expected to be >= 0");
	FAssertMsg(eBuilding < GC.getNumBuildingInfos(), "eBuilding is expected to be within maximum bounds (invalid Index)");
	FAssertMsg(eBonus >= 0, "eBonus is expected to be non-negative (invalid Index)");
	FAssertMsg(eBonus < GC.getNumBonusInfos(), "eBonus is expected to be within maximum bounds (invalid Index)");

	if (iChange != 0)
	{
		CvBuildingInfo& kBuilding = GC.getBuildingInfo(eBuilding);
		int iConsumedBonusIndex = kBuilding.getConsumedBonusIndex(eBonus);
		FAssertMsg(iConsumedBonusIndex >= 0, "Attempt to change consumed bonus count for bonus converter bonus that isn't consumed in changeNumBonusConverterConsumedBonuses");

		if (m_ppaiNumBonusConverterConsumedBonuses == NULL)
		{
			m_ppaiNumBonusConverterConsumedBonuses = new int*[GC.getNumBuildingInfos()];
			for (int iI = 0; iI < GC.getNumBuildingInfos(); iI++)
			{
				m_ppaiNumBonusConverterConsumedBonuses[iI] = NULL;
			}
		}

		if (m_ppaiNumBonusConverterConsumedBonuses[eBuilding] == NULL)
		{
			int iNumBonusTypes = kBuilding.getNumConsumedBonuses();
			FAssert(iNumBonusTypes > 0);
			m_ppaiNumBonusConverterConsumedBonuses[eBuilding] = new int[iNumBonusTypes];
			for (int iI = 0; iI < iNumBonusTypes; iI++)
			{
				m_ppaiNumBonusConverterConsumedBonuses[eBuilding][iI] = 0;
			}
		}

		m_ppaiNumBonusConverterConsumedBonuses[eBuilding][iConsumedBonusIndex] += iChange;

		FAssert(m_ppaiNumBonusConverterConsumedBonuses[eBuilding][iConsumedBonusIndex] >= 0);
	}
}


/*
// Note: This is old code from a bygone version, but it may be inspirational.
// Specifically, it may be useful in the future if we need to randomize
// which converters get shut off when there are multiple candidates
void CvPlotGroup::getShuffledBonusConverterProcessingData(CLinkList<CvBonusConverterProcessingData>& aBonusConverterProcessingData)
{
	CLinkList<CvBonusConverterProcessingData> aShuffledBonusConverterProcessingData;
	CLLNode<CvBuildingCount>* pBuildingCountNode = headBonusConverterCountNode();

	while (pBuildingCountNode != NULL)
	{
		if (GC.getGameINLINE().getSorenRandNum(100, "Shuffle Bonus Converter Counts") > 50)
		{
			aShuffledBonusConverterProcessingData.insertAtEnd(CvBonusConverterProcessingData(pBuildingCountNode->m_data));
		}
		else
		{
			aShuffledBonusConverterProcessingData.insertAtBeginning(CvBonusConverterProcessingData(pBuildingCountNode->m_data));
		}

		pBuildingCountNode = nextBonusConverterCountNode(pBuildingCountNode);
	}
}
*/
// MOD - END - Bonus Converters


void CvPlotGroup::insertAtEndPlots(XYCoords xy)
{
	m_plots.insertAtEnd(xy);
}


CLLNode<XYCoords>* CvPlotGroup::deletePlotsNode(CLLNode<XYCoords>* pNode)
{
	CLLNode<XYCoords>* pPlotNode;

	pPlotNode = m_plots.deleteNode(pNode);

	if (getLengthPlots() == 0)
	{
		GET_PLAYER(getOwnerINLINE()).deletePlotGroup(getID());
	}

  return pPlotNode;
}


CLLNode<XYCoords>* CvPlotGroup::nextPlotsNode(CLLNode<XYCoords>* pNode) const
{
	return m_plots.next(pNode);
}


int CvPlotGroup::getLengthPlots() const
{
	return m_plots.getLength();
}


CLLNode<XYCoords>* CvPlotGroup::headPlotsNode() const
{
	return m_plots.head();
}


// MOD - START - PlotGroup Cities
void CvPlotGroup::insertAtEndCityPlots(XYCoords xy)
{
	m_cityPlots.insertAtEnd(xy);
}


CLLNode<XYCoords>* CvPlotGroup::deleteCityPlotsNode(CLLNode<XYCoords>* pNode)
{
	return m_cityPlots.deleteNode(pNode);
}


CLLNode<XYCoords>* CvPlotGroup::nextCityPlotsNode(CLLNode<XYCoords>* pNode) const
{
	return m_cityPlots.next(pNode);
}


int CvPlotGroup::getLengthCityPlots() const
{
	return m_cityPlots.getLength();
}


CLLNode<XYCoords>* CvPlotGroup::headCityPlotsNode() const
{
	return m_cityPlots.head();
}
// MOD - END - PlotGroup Cities


void CvPlotGroup::read(FDataStreamBase* pStream)
{
	// Init saved data
	reset();

	uint uiFlag=0;
	pStream->Read(&uiFlag);	// flags for expansion

	pStream->Read(&m_iID);

	pStream->Read((int*)&m_eOwner);

	FAssertMsg((0 < GC.getNumBonusInfos()), "GC.getNumBonusInfos() is not greater than zero but an array is being allocated in CvPlotGroup::read");
	pStream->Read(GC.getNumBonusInfos(), m_paiNumBonuses);

	// MOD - START - Bonus Converters
	int iBuildingCount;
	int iBonusCount;

	if (m_ppaiNumBonusConverterConsumedBonuses != NULL)
	{
		for (int iI=0; iI <GC.getNumBuildingInfos(); iI++)
		{
			SAFE_DELETE_ARRAY(m_ppaiNumBonusConverterConsumedBonuses[iI]);
		}
		SAFE_DELETE_ARRAY(m_ppaiNumBonusConverterConsumedBonuses);
	}

	pStream->Read(&iBuildingCount);
	if (iBuildingCount > 0)
	{
		m_ppaiNumBonusConverterConsumedBonuses = new int*[GC.getNumBuildingInfos()];
		for (int iI = 0; iI < GC.getNumBuildingInfos(); iI++)
		{
			pStream->Read(&iBonusCount);
			if (iBonusCount > 0)
			{
				int iNumBonusTypes = GC.getBuildingInfo((BuildingTypes)iI).getNumConsumedBonuses();
				m_ppaiNumBonusConverterConsumedBonuses[iI] = new int[iNumBonusTypes];
				pStream->Read(iNumBonusTypes, m_ppaiNumBonusConverterConsumedBonuses[iI]);
			}
			else
			{
				m_ppaiNumBonusConverterConsumedBonuses[iI] = NULL;
			}
		}
	}
	// MOD - END - Bonus Converters

	m_plots.Read(pStream);

	// MOD - START - PlotGroup Cities
	m_cityPlots.Read(pStream);
	// MOD - END - PlotGroup Cities
}


void CvPlotGroup::write(FDataStreamBase* pStream)
{
	uint uiFlag=0;
	pStream->Write(uiFlag);		// flag for expansion

	pStream->Write(m_iID);

	pStream->Write(m_eOwner);

	FAssertMsg((0 < GC.getNumBonusInfos()), "GC.getNumBonusInfos() is not greater than zero but an array is being allocated in CvPlotGroup::write");
	pStream->Write(GC.getNumBonusInfos(), m_paiNumBonuses);

	// MOD - START - Bonus Converters
	if (m_ppaiNumBonusConverterConsumedBonuses == NULL)
	{
		pStream->Write((int)0);
	}
	else
	{
		pStream->Write((int)GC.getNumBuildingInfos());

		for (int iI = 0; iI < GC.getNumBuildingInfos(); iI++)
		{
			if (m_ppaiNumBonusConverterConsumedBonuses[iI] == NULL)
			{
				pStream->Write((int)0);
			}
			else
			{
				int iNumBonusTypes = GC.getBuildingInfo((BuildingTypes)iI).getNumConsumedBonuses();
				pStream->Write(iNumBonusTypes);
				pStream->Write(iNumBonusTypes, m_ppaiNumBonusConverterConsumedBonuses[iI]);
			}
		}
	}
	// MOD - END - Bonus Converters

	m_plots.Write(pStream);

	// MOD - START - PlotGroup Cities
	m_cityPlots.Write(pStream);
	// MOD - END - PlotGroup Cities
}
