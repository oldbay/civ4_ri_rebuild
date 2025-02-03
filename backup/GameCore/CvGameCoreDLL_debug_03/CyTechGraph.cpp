// MOD - START - Tech Graph

//#include "CvGameCoreDLL.h"
#include "CyTechGraph.h"

//
// Python wrapper class for CvTechGraph
//

CyTechGraph::CyTechGraph() : m_pTechGraph(new CvTechGraph())
{
}

CyTechGraph::CyTechGraph(CvTechGraph* pTechGraph) : m_pTechGraph(pTechGraph)
{

}

CyTechGraph::~CyTechGraph()
{
	SAFE_DELETE(m_pTechGraph);
}

void CyTechGraph::update()
{
	if (m_pTechGraph)
		m_pTechGraph->update();
}

void CyTechGraph::updateCivilization(int eCivilization)
{
	if (m_pTechGraph)
		m_pTechGraph->update((CivilizationTypes)eCivilization);
}

void CyTechGraph::updatePlayer(int ePlayer)
{
	if (m_pTechGraph)
		m_pTechGraph->update((PlayerTypes)ePlayer);
}

int CyTechGraph::getCivilization() const
{
	return m_pTechGraph ? m_pTechGraph->getCivilization() : -1;
}

int CyTechGraph::getPlayer() const
{
	return m_pTechGraph ? m_pTechGraph->getPlayer() : -1;
}

int CyTechGraph::getNumColumns() const
{
	return m_pTechGraph ? m_pTechGraph->getNumColumns() : -1;
}

CyTechGraphColumn* CyTechGraph::getColumn(int iColumnNumber) const
{
	return m_pTechGraph ? new CyTechGraphColumn(m_pTechGraph->getColumn(iColumnNumber)) : NULL;
}

CyTechGraphPanel* CyTechGraph::getPanel(int eTech) const
{
	return m_pTechGraph ? new CyTechGraphPanel(m_pTechGraph->getPanel((TechTypes)eTech)) : NULL;
}



//
// Python wrapper class for CyTechGraphColumn
//

CyTechGraphColumn::CyTechGraphColumn() : m_pTechGraphColumn(NULL)
{

}

CyTechGraphColumn::CyTechGraphColumn(CvTechGraphColumn* pTechGraphColumn) : m_pTechGraphColumn(pTechGraphColumn)
{

}

int CyTechGraphColumn::getNumIcons() const
{
	return m_pTechGraphColumn ? m_pTechGraphColumn->getNumIcons() : -1;
}

int CyTechGraphColumn::getNumTechs() const
{
	return m_pTechGraphColumn ? m_pTechGraphColumn->getNumTechs() : -1;
}

int CyTechGraphColumn::getTech(int iTechNumber) const
{
	return m_pTechGraphColumn ? m_pTechGraphColumn->getTech(iTechNumber) : -1;
}



//
// Python wrapper class for CyTechGraphPanel
//

CyTechGraphPanel::CyTechGraphPanel() : m_pTechGraphPanel(NULL)
{

}

CyTechGraphPanel::CyTechGraphPanel(CvTechGraphPanel* pTechGraphPanel) : m_pTechGraphPanel(pTechGraphPanel)
{

}

int CyTechGraphPanel::getTech() const
{
	return m_pTechGraphPanel ? m_pTechGraphPanel->getTech() : -1;
}

int CyTechGraphPanel::getNumIcons() const
{
	return m_pTechGraphPanel ? m_pTechGraphPanel->getNumIcons() : -1;
}

TechGraphIconArray CyTechGraphPanel::getIcons() const
{
	return m_pTechGraphPanel ? m_pTechGraphPanel->getIcons() : TechGraphIconArray();
}

CvTechGraphIcon CyTechGraphPanel::getIcon(int i) const
{
	return m_pTechGraphPanel ? m_pTechGraphPanel->getIcon(i) : CvTechGraphIcon();
}
// MOD - END - Tech Graph
