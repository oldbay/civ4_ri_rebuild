// MOD - START - Tech Graph
#include "CvGameCoreDLL.h"
#include "CyTechGraph.h"
//#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

//
// published python interface for CyTechGraph
//

void CyTechGraphPythonInterface()
{
    #if defined(__GNUC__)
    std::clog << "Python Extension Module - CyTechGraphPythonInterface\n";
    #else
    OutputDebugString("Python Extension Module - CyTechGraphPythonInterface\n");
    #endif

	python::class_<CvTechGraphIcon>("CvTechGraphIcon")
		.def_readwrite("eWidgetType", &CvTechGraphIcon::eWidgetType)
		.def_readwrite("iData1", &CvTechGraphIcon::iData1)
		.def_readwrite("iData2", &CvTechGraphIcon::iData2)
		.def_readwrite("bOption", &CvTechGraphIcon::bOption)
		.def_readwrite("bObsolete", &CvTechGraphIcon::bObsolete)
		.def_readonly("szTexture", &CvTechGraphIcon::szTexture)
		;

	// Alas, the commented code below doesn't work due to a bug
	// in the version of boost that Civ4 uses:
	// http://mail.python.org/pipermail/cplusplus-sig/2004-September/007795.html
	/*
	python::class_<TechGraphIconArray>("TechGraphIconArray")
		.def(python::vector_indexing_suite<TechGraphIconArray>());
	*/

	python::class_<CyTechGraph>("CyTechGraph")
		.def("isNone", &CyTechGraph::isNone, "bool () - valid CyTechGraph() interface")
		.def("update", &CyTechGraph::update, "() - updates the tech graph")
		.def("updateCivilization", &CyTechGraph::updateCivilization, "(eCivilization) - updates tech graph for the given the civilization and implies NO_PLAYER")
		.def("updatePlayer", &CyTechGraph::updatePlayer, "(ePlayer) - updates the tech graph for the given player and implies civilization")
		.def("getCivilization", &CyTechGraph::getCivilization, "int () - the civilization the current graph is for")
		.def("getPlayer", &CyTechGraph::getPlayer, "int () - the player the current graph is for")
		.def("getNumColumns", &CyTechGraph::getNumColumns, "int () - total graph columns")
		.def("getColumn", &CyTechGraph::getColumn, python::return_value_policy<python::manage_new_object>(), "PyTechGraphColumn (int iColumnNumber) - retrieves the specified column")
		.def("getPanel", &CyTechGraph::getPanel, python::return_value_policy<python::manage_new_object>(), "PyTechGraphPanel (int iTech) - retrieves the panel for the given tech")
		;

	python::class_<CyTechGraphColumn>("CyTechGraphColumn")
		.def("isNone", &CyTechGraphColumn::isNone, "bool () - valid CyTechGraphColumn() interface")
		.def("getNumIcons", &CyTechGraphColumn::getNumIcons, "int () - maximum number of icons in any of the column's panels")
		.def("getNumTechs", &CyTechGraphColumn::getNumTechs, "int () - total techs within this column")
		.def("getTech", &CyTechGraphColumn::getTech, "int (int iColumnNumber) - retrieves the tech associated with the given column number")
		;

	python::class_<CyTechGraphPanel>("CyTechGraphPanel")
		.def("isNone", &CyTechGraphPanel::isNone, "bool () - valid CyTechGraphPanel() interface")
		.def("getTech", &CyTechGraphPanel::getTech, "() - retrieves the tech this panel represents")
		.def("getNumIcons", &CyTechGraphPanel::getNumIcons, "int () - total number of icons")
		//.def("getIcons", &CyTechGraphPanel::getIcons, "() - retrieves the icon array")
		.def("getIcon", &CyTechGraphPanel::getIcon, "CvTechGraphIcon (int i) - retrieves the specified icon")
		;
}
// MOD - END - Tech Graph
