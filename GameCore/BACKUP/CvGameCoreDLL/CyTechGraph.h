// MOD - START - Tech Graph
#pragma once

#ifndef CyTechGraph_H
#define CyTechGraph_H

class CvTechGraph;
class CvTechGraphColumn;
class CvTechGraphPanel;

//
// Python wrapper class for CvTechGraph
// SINGLETON
//

class CyTechGraphColumn;
class CyTechGraphPanel;
class CyTechGraph
{
public:
	CyTechGraph();
	CyTechGraph(CvTechGraph* pTechGraph); // Call from C++
	~CyTechGraph();
	CvTechGraph* getTechGraph() { return m_pTechGraph; } // Call from C++
	bool isNone() { return (m_pTechGraph==NULL); }

	void update();
	void updateCivilization(int eCivilization);
	void updatePlayer(int ePlayer);

	int getCivilization() const;
	int getPlayer() const;

	int getNumColumns() const;

	CyTechGraphColumn* getColumn(int iColumnNumber) const;
	CyTechGraphPanel* getPanel(int eTech) const;

protected:
	CvTechGraph* m_pTechGraph;
};



//
// Python wrapper class for CyTechGraphColumn
// SINGLETON
//

class CyTechGraphColumn
{
public:
	CyTechGraphColumn();
	CyTechGraphColumn(CvTechGraphColumn* pTechGraphColumn); // Call from C++
	CvTechGraphColumn* getTechGraphColumn() { return m_pTechGraphColumn; } // Call from C++
	bool isNone() { return (m_pTechGraphColumn==NULL); }

	int getNumIcons() const;
	int getNumTechs() const;

	int getTech(int iTechNumber) const;

protected:
	CvTechGraphColumn* m_pTechGraphColumn;
};



//
// Python wrapper class for CyTechGraphPanel
// SINGLETON
//

class CyTechGraphPanel
{
public:
	CyTechGraphPanel();
	CyTechGraphPanel(CvTechGraphPanel* pTechGraphPanel); // Call from C++
	CvTechGraphPanel* getTechGraphPanel() { return m_pTechGraphPanel; } // Call from C++
	bool isNone() { return (m_pTechGraphPanel==NULL); }

	int getTech() const;
	int getNumIcons() const;
	TechGraphIconArray getIcons() const;
	CvTechGraphIcon getIcon(int i) const; // Exposed to Python

protected:
	CvTechGraphPanel* m_pTechGraphPanel;
};

#endif	// CyTechGraph_H
// MOD - END - Tech Graph