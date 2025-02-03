// MOD - START - Tech Graph
#pragma once

#ifndef CvTechGraph_H
#define CvTechGraph_H

struct CvTechGraphIcon
{
	const TCHAR* szTexture;
	WidgetTypes eWidgetType;
	int iData1;
	int iData2;
	bool bOption;
	bool bObsolete;

	CvTechGraphIcon() :
		szTexture(NULL),
		eWidgetType(WIDGET_PYTHON),
		iData1(0),
		iData2(0),
		bOption(false),
		bObsolete(false)
	{
	}

	CvTechGraphIcon(
		const TCHAR* szTextureVal,
		WidgetTypes eWidgetTypeVal,
		int iData1Val,
		int iData2Val,
		bool bOptionVal,
		bool bObsoleteVal)
	:
		szTexture(szTextureVal),
		eWidgetType(eWidgetTypeVal),
		iData1(iData1Val),
		iData2(iData2Val),
		bOption(bOptionVal),
		bObsolete(bObsoleteVal)
	{
	}
};

typedef std::vector<CvTechGraphIcon> TechGraphIconArray;





//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//  class : CvTechGraph
//
//  DESC:   
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CvTechGraphColumn;
class CvTechGraphPanel;
class CvTechGraph
{
public:

	CvTechGraph();
	virtual ~CvTechGraph();

	void init();

protected:

	void uninit();

public:

	void update(); // Exposed to Python
	void update(CivilizationTypes eCivilization); // Exposed to Python
	void update(PlayerTypes ePlayer); // Exposed to Python

protected:

	void update(CivilizationTypes eCivilization, PlayerTypes ePlayer);

public:

	CivilizationTypes getCivilization() const; // Exposed to Python
	PlayerTypes getPlayer() const; // Exposed to Python

	int getNumColumns() const; // Exposed to Python

	CvTechGraphColumn* getColumn(int iColumnNumber) const; // Exposed to Python
	CvTechGraphPanel* getPanel(TechTypes eTech) const; // Exposed to Python

//---------------------------------------PROTECTED MEMBER VARIABLES---------------------------------
protected:

	bool m_bInitialized;
	bool m_bReady;
	CivilizationTypes m_eCivilization;
	PlayerTypes m_ePlayer;

	std::vector<CvTechGraphColumn*> m_aColumns;
	std::vector<CvTechGraphPanel*> m_aPanels;

};



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//  class : CvTechGraphColumn
//
//  DESC:   
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CvTechGraphColumn
{

friend class CvTechGraph;

public:

	CvTechGraphColumn();
	virtual ~CvTechGraphColumn();

protected:

	void update(CvTechGraph* pTechGraph);

public:

	int getNumIcons() const; // Exposed to Python
	int getNumTechs() const; // Exposed to Python

	TechTypes getTech(int iTechNumber) const; // Exposed to Python

//---------------------------------------PROTECTED MEMBER VARIABLES---------------------------------
protected:

	int m_iNumIcons;
	int m_iNumTechs;

	TechTypes* m_peTechs;

};



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//  class : CvTechGraphPanel
//
//  DESC:   
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CvTechGraphPanel
{

friend class CvTechGraph;

public:

	CvTechGraphPanel(TechTypes eTech);
	virtual ~CvTechGraphPanel();

protected:

	void update(CivilizationTypes eCivilization, PlayerTypes ePlayer);

public:

	TechTypes getTech() const; // Exposed to Python
	int getNumIcons() const; // Exposed to Python
	TechGraphIconArray getIcons() const; // Exposed to Python
	CvTechGraphIcon getIcon(int i) const; // Exposed to Python

protected:

	TechTypes m_eTech;

	TechGraphIconArray m_aIcons;

};

#endif	// CvTechGraph_H
// MOD - END - Tech Graph