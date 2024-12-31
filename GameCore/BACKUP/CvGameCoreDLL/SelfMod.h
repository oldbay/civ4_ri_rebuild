#pragma once
#ifndef SELF_MOD_H
#define SELF_MOD_H

/*	smcpoc: Let's put all runtime patches for Civ4BeyondSword.exe (v3.19)
	in a single place. (But, so far, there's only one.) */

class Civ4BeyondSwordMods
{
public:
	// (Unused for now)
	bool isPlotIndicatorSizePatched() const { return m_bPlotIndicatorSizePatched; }
	void patchPlotIndicatorSize();
private:
	bool m_bPlotIndicatorSizePatched;
};

namespace smc
{
	extern Civ4BeyondSwordMods BtS_EXE;
};

#endif
