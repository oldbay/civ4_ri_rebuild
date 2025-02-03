#pragma once

// plotGroup.h

#ifndef CIV4_PLOT_GROUP_H
#define CIV4_PLOT_GROUP_H

//#include "CvGameCoreDLL.h"
#include "CvStructs.h"
#include "CvCity.h"
#include "LinkedList.h"

class CvPlot;
class CvPlotGroup
{

public:

	CvPlotGroup();
	virtual ~CvPlotGroup();

	void init(int iID, PlayerTypes eOwner, CvPlot* pPlot);
	void uninit();
	void reset(int iID = 0, PlayerTypes eOwner = NO_PLAYER, bool bConstructorCall=false);

	void addPlot(CvPlot* pPlot);
	void removePlot(CvPlot* pPlot);
	// MOD - START - PlotGroup Cities
	void addCity(CvCity* pCity);
	void removeCity(CvCity* pCity);
	// MOD - END - PlotGroup Cities
	void recalculatePlots();														

	int getID() const;
	void setID(int iID);

	PlayerTypes getOwner() const;
#if (defined(__GNUC__) || defined(_USRDLL))
//#ifdef _USRDLL
	inline PlayerTypes getOwnerINLINE() const
	{
		return m_eOwner;
	}
#endif
	int getNumBonuses(BonusTypes eBonus) const;
	bool hasBonus(BonusTypes eBonus);										
	void changeNumBonuses(BonusTypes eBonus, int iChange);
	// MOD - START - PlotGroup Bonus Efficiency
	void changeNumBonuses(std::vector<CvBonusAmountChange>& aBonusChanges);
	// MOD - END - PlotGroup Bonus Efficiency

	// MOD - START - Bonus Converters
	int getNumBonusConverterConsumedBonuses(BuildingTypes eBuilding, BonusTypes eBonus) const;
	int getNumBonusConverterConsumedBonusesByIndex(BuildingTypes eBuilding, int iConsumedBonusIndex) const;
	void changeNumBonusConverterConsumedBonuses(BuildingTypes eBuilding, BonusTypes eBonus, int iChange);

	//void getShuffledBonusConverterProcessingData(CLinkList<CvBonusConverterProcessingData>&);
	// MOD - END - Bonus Converters

	void insertAtEndPlots(XYCoords xy);			
	CLLNode<XYCoords>* deletePlotsNode(CLLNode<XYCoords>* pNode);
	CLLNode<XYCoords>* nextPlotsNode(CLLNode<XYCoords>* pNode) const;
	int getLengthPlots() const;
	CLLNode<XYCoords>* headPlotsNode() const;

	// MOD - START - PlotGroup Cities
	void insertAtEndCityPlots(XYCoords xy);			
	CLLNode<XYCoords>* deleteCityPlotsNode(CLLNode<XYCoords>* pNode);
	CLLNode<XYCoords>* nextCityPlotsNode(CLLNode<XYCoords>* pNode) const;
	int getLengthCityPlots() const;
	CLLNode<XYCoords>* headCityPlotsNode() const;
	// MOD - END - PlotGroup Cities

	// for serialization
	void read(FDataStreamBase* pStream);
	void write(FDataStreamBase* pStream);

protected:

	int m_iID;

	PlayerTypes m_eOwner;

	int* m_paiNumBonuses;

	// MOD - START - Bonus Converters
	int** m_ppaiNumBonusConverterConsumedBonuses;
	// MOD - END - Bonus Converters

	CLinkList<XYCoords> m_plots;
	// MOD - START - PlotGroup Cities
	CLinkList<XYCoords> m_cityPlots;
	// MOD - END - PlotGroup Cities
};

#endif
