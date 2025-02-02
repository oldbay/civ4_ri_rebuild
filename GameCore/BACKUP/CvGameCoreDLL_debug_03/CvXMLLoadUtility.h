#pragma once

//  $Header:
//------------------------------------------------------------------------------------------------
//
//  FILE:    CvXMLLoadUtility.h
//
//  AUTHOR:  Eric MacDonald  --  8/2003
//
//  PURPOSE: Group of functions to load in the xml files for Civilization 4
//
//------------------------------------------------------------------------------------------------
//  Copyright (c) 2003 Firaxis Games, Inc. All rights reserved.
//------------------------------------------------------------------------------------------------
#ifndef XML_LOAD_UTILITY_H
#define XML_LOAD_UTILITY_H


//#include "CvGameCoreDLL.h"
//#include "CvStructs.h"
#include "CvDepends.h"
#include "CvInfos.h"
#include "CvGlobals.h"
#include "FVariableSystem.h"
#include "CvDLLUtilityIFaceBase.h"
#include "CvDLLXMLIFaceBase.h"

//???
//#include "CvDLLXMLIFaceBase.h"
//#include "FVariableSystem.h"
//???

class FXmlSchemaCache;
class FXml;
class CvGameText;
class CvCacheObject;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//  class : CvXMLLoadUtility
//
//  DESC:   Group of functions to load in the xml files for Civilization 4
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CvXMLLoadUtility
{
//---------------------------------------PUBLIC INTERFACE---------------------------------
public:
	// default constructor
	DllExport CvXMLLoadUtility();
	// default destructor
	DllExport ~CvXMLLoadUtility(void);

	bool CreateFXml();
	void DestroyFXml();

	DllExport bool LoadPostMenuGlobals();
	DllExport bool LoadPreMenuGlobals();
	DllExport bool LoadBasicInfos();
	DllExport bool LoadPlayerOptions();
	DllExport bool LoadGraphicOptions();

	// read the global defines from a specific file
	bool ReadGlobalDefines(const TCHAR* szXMLFileName, CvCacheObject* cache);
	// loads globaldefines.xml and calls various other functions to load relevant global variables
	DllExport bool SetGlobalDefines();
	// loads globaltypes.xml and calls various other functions to load relevant global variables
	DllExport bool SetGlobalTypes();
	// loads calls various functions that load xml files that in turn load relevant global variables
	bool SetGlobals();
	// loads globaldefines.xml and calls various other functions to load relevant global variables
	DllExport bool SetPostGlobalsGlobalDefines();

	// calls various functions to release the memory associated with the global variables
	DllExport void CleanUpGlobalVariables();

	// releases global variables associated with items that can be reloaded
	DllExport void ResetLandscapeInfo();
	DllExport bool SetupGlobalLandscapeInfo();
	DllExport bool SetGlobalArtDefines();
	DllExport bool LoadGlobalText();
	bool SetHelpText();
	DllExport void ResetGlobalEffectInfo();

// for progress bars
	typedef void (*ProgressCB)(int iStepNum, int iTotalSteps, const char* szMessage);
	static int GetNumProgressSteps();
	void RegisterProgressCB(ProgressCB cbFxn) { m_pCBFxn = cbFxn; }

	// moves the current xml node from where it is now to the next non-comment node, returns false if it can't find one
	bool SkipToNextVal();

	// overloaded function that gets the child value of the tag with szName if there is only one child
	// value of that name
	void MapChildren();	// call this before GetChildXMLValByName to use fast searching
	bool GetChildXmlValByName(std::string& pszVal, const TCHAR* szName, char* pszDefault = NULL);
	bool GetChildXmlValByName(std::wstring& pszVal, const TCHAR* szName, wchar* pszDefault = NULL);
	// overloaded function that gets the child value of the tag with szName if there is only one child
	// value of that name
	bool GetChildXmlValByName(char* pszVal, const TCHAR* szName, char* pszDefault = NULL);
	bool GetChildXmlValByName(wchar* pszVal, const TCHAR* szName, wchar* pszDefault = NULL);
	// overloaded function that gets the child value of the tag with szName if there is only one child
	// value of that name
	bool GetChildXmlValByName(int* piVal, const TCHAR* szName, int iDefault = 0);
	// overloaded function that gets the child value of the tag with szName if there is only one child
	// value of that name
	bool GetChildXmlValByName(float* pfVal, const TCHAR* szName, float fDefault = 0.0f);
	// overloaded function that gets the child value of the tag with szName if there is only one child
	// value of that name
	bool GetChildXmlValByName(bool* pbVal, const TCHAR* szName, bool bDefault = false);

	// loads an xml file into the FXml variable.  The szFilename parameter has
	// the m_szXmlPath member variable pre-pended to it to form the full pathname
	bool LoadCivXml(FXml* pFXml, const TCHAR* szFilename);

	// overloaded function that gets either the current xml node's or the next non-comment xml node's string value
	// depending on if the current node is a non-comment node or not
	bool GetXmlVal(std::wstring& pszVal, wchar* pszDefault = NULL);
	bool GetXmlVal(std::string& pszVal, char* pszDefault = NULL);
	// overloaded function that gets either the current xml node's or the next non-comment xml node's string value
	// depending on if the current node is a non-comment node or not
	bool GetXmlVal(wchar* pszVal, wchar* pszDefault = NULL);
	bool GetXmlVal(char* pszVal, char* pszDefault = NULL);
	// overloaded function that gets either the current xml node's or the next non-comment xml node's int value
	// depending on if the current node is a non-comment node or not
	bool GetXmlVal(int* piVal, int iDefault = 0);
	// overloaded function that gets either the current xml node's or the next non-comment xml node's float value
	// depending on if the current node is a non-comment node or not
	bool GetXmlVal(float* pfVal, float fDefault = 0.0f);
	// overloaded function that gets either the current xml node's or the next non-comment xml node's boolean value
	// depending on if the current node is a non-comment node or not
	bool GetXmlVal(bool* pbVal, bool bDefault = false);

	// overloaded function that sets the current xml node to it's next sibling and then
	//	gets the next non-comment xml node's string value
	bool GetNextXmlVal(std::string& pszVal, char* pszDefault = NULL);
	bool GetNextXmlVal(std::wstring& pszVal, wchar* pszDefault = NULL);
	// overloaded function that sets the current xml node to it's next sibling and then
	//	gets the next non-comment xml node's string value
	bool GetNextXmlVal(char* pszVal, char* pszDefault = NULL);
	bool GetNextXmlVal(wchar* pszVal, wchar* pszDefault = NULL);
	// overloaded function that sets the current xml node to it's next sibling and then
	//	gets the next non-comment xml node's int value
	bool GetNextXmlVal(int* piVal, int iDefault = 0);
	// overloaded function that sets the current xml node to it's next sibling and then
	//	gets the next non-comment xml node's float value
	bool GetNextXmlVal(float* pfVal, float fDefault = 0.0f);
	// overloaded function that sets the current xml node to it's next sibling and then
	//	gets the next non-comment xml node's boolean value
	bool GetNextXmlVal(bool* pbVal, bool bDefault = false);

	// overloaded function that sets the current xml node to it's first non-comment child node
	//	and then gets that node's string value
	bool GetChildXmlVal(std::string& pszVal, char* pszDefault = NULL);
	bool GetChildXmlVal(std::wstring& pszVal, wchar* pszDefault = NULL);
	// overloaded function that sets the current xml node to it's first non-comment child node
	//	and then gets that node's string value
	bool GetChildXmlVal(char* pszVal, char* pszDefault = NULL);
	bool GetChildXmlVal(wchar* pszVal, wchar* pszDefault = NULL);
	// overloaded function that sets the current xml node to it's first non-comment child node
	//	and then gets that node's integer value
	bool GetChildXmlVal(int* piVal, int iDefault = 0);
	// overloaded function that sets the current xml node to it's first non-comment child node
	//	and then gets that node's float value
	bool GetChildXmlVal(float* pfVal, float fDefault = 0.0f);
	// overloaded function that sets the current xml node to it's first non-comment child node
	//	and then gets that node's boolean value
	bool GetChildXmlVal(bool* pbVal, bool bDefault = false);

#if (defined(__GNUC__) || defined(_USRDLL))
//#ifdef _USRDLL
	FXml* GetXML() { return m_pFXml; }
#endif

	// loads the local yield from the xml file
	int SetYields(int** ppiYield);

#if (defined(__GNUC__) || defined(_USRDLL))
//#ifdef _USRDLL
	template <class T>
	int SetCommerce(T** ppiCommerce);
#endif

	// allocate and set the feature struct variables for the CvBuildInfo class
	void SetFeatureStruct(int** ppiFeatureTech, int** ppiFeatureTime, int** ppiFeatureProduction, bool** ppbFeatureRemove);
	// loads the improvement bonuses from the xml file
	void SetImprovementBonuses(CvImprovementBonusInfo** ppImprovementBonus);

	// MOD - START - Improved XML Loading
	template <class T, class InfoType>
	void SetInfoValueArray(int *piLength, T **ppaList, const TCHAR* szContainerTagName, const TCHAR* szItemTagName, const TCHAR* szInfoTagName, const TCHAR* szValueTagName);
	template <class T, class TypeA, class TypeB>
	void SetBasicEffectValues(std::vector<T>& aEffects, const TCHAR* szContainerTagName, const TCHAR* szItemTagName, const TCHAR* szTypeATagName, const TCHAR* szTypeBTagName, const TCHAR* szValueTagName);
	template <class InfoType, class EffectType>
	void SetEconomicEffectMap(std::map<std::pair<InfoType, EffectType>, int>& map, const TCHAR* szContainerTagName, const TCHAR* szItemTagName, const TCHAR* szInfoTagName, const TCHAR* szCollectionTagName, const TCHAR* szValueTagName, int iMaxCollectionSize);
	template <class InfoType, class EffectType>
	void SetEconomicEffectLookup(const std::map<std::pair<InfoType, EffectType>, int>& map, int*** pppiLookup, int iNumInfoTypes, int iNumEffectTypes);
	template <class T, class InfoType, class EffectType>
	void SetEconomicEffectEnumeration(const std::map<std::pair<InfoType, EffectType>, int>& map, T** ppaEnumeration, int* piNumEffects);
	template <class M, class T, class InfoType, class EffectType>
	void SetEconomicEffectEnumeration( T** ppaEnumeration, int* piNumEffects, const TCHAR* szContainerTagName, const TCHAR* szItemTagName, const TCHAR* szInfoTagName, const TCHAR* szCollectionTagName, const TCHAR* szValueTagName, int iMaxCollectionSize);
	//template <class InfoType>
	//void CopyYieldEffects(const std::map<std::pair<InfoType, YieldTypes>, int>& aEconomicEffectMap, int** pppiList[NUM_YIELD_TYPES], int iNumInfoTypes);
	//void SetEconomicEffectValues(int** ppTypes, int** ppAmounts, int* piNum, int iNumEffects, int*** pppDirectLookup, int iDefaultVal, const TCHAR* szContainerTagName, const TCHAR* szItemTagName, const TCHAR* szTypeATagName, const TCHAR* szTypeBTagName, const TCHAR* szValueTagName);
	// MOD - END - Improved XML Loading

	// check through the pszList parameter for the pszVal and returns the location a match
	// is found if one is found
	static int FindInInfoClass(const TCHAR* pszVal, bool hideAssert = false);

#if (defined(__GNUC__) || defined(_USRDLL))
//#ifdef _USRDLL
	template <class T>
	void InitList(T **ppList, int iListLen, T val = 0);
#endif
	void InitStringList(CvString **ppszList, int iListLen, CvString szString);

	void InitImprovementBonusList(CvImprovementBonusInfo** ppImprovementBonus, int iListLen);
	// allocate and initialize the civilization's default buildings
	void InitBuildingDefaults(int **ppiDefaults);
	// allocate and initialize the civilization's default units
	void InitUnitDefaults(int **ppiDefaults);
	// allocate and initialize a 2 dimensional array of int pointers
	void Init2DIntList(int*** pppiList, int iSizeX, int iSizeY);
	// allocate and initialize a 2 dimensional array of float pointers
	void Init2DFloatList(float*** pppfList, int iSizeX, int iSizeY);
	// allocate and initialize a 2D array of DirectionTypes
	void Init2DDirectionTypesList(DirectionTypes*** pppiList, int iSizeX, int iSizeY);
	// allocate an array of int pointers
	void InitPointerIntList(int*** pppiList, int iSizeX);
	// allocate an array of float pointers
	void InitPointerFloatList(float*** pppfList, int iSizeX);

	// allocate and initialize a list from a tag pair in the xml
	void SetVariableListTagPair(int **ppiList, const TCHAR* szRootTagName,
		int iInfoBaseSize, int iInfoBaseLength, int iDefaultListVal = 0);

	// allocate and initialize a list from a tag pair in the xml
	void SetVariableListTagPair(bool **ppbList, const TCHAR* szRootTagName,
		int iInfoBaseSize, int iInfoBaseLength, bool bDefaultListVal = false);

	// allocate and initialize a list from a tag pair in the xml
	void SetVariableListTagPair(float **ppfList, const TCHAR* szRootTagName,
		int iInfoBaseSize, int iInfoBaseLength, float fDefaultListVal = 0.0f);

	// allocate and initialize a list from a tag pair in the xml
	void SetVariableListTagPair(CvString **ppszList, const TCHAR* szRootTagName,
		int iInfoBaseSize, int iInfoBaseLength, CvString szDefaultListVal = "");

	// allocate and initialize a list from a tag pair in the xml
	void SetVariableListTagPair(int **ppiList, const TCHAR* szRootTagName,
		CvString* m_paszTagList, int iTagListLength, int iDefaultListVal = 0);

	// allocate and initialize a list from a tag pair in the xml for audio scripts
	void SetVariableListTagPairForAudioScripts(int **ppiList, const TCHAR* szRootTagName,
		CvString* m_paszTagList, int iTagListLength, int iDefaultListVal = -1);

	// allocate and initialize a list from a tag pair in the xml
	void SetVariableListTagPairForAudioScripts(int **ppiList, const TCHAR* szRootTagName,
		int iInfoBaseLength, int iDefaultListVal = -1);

	// allocate and initialize a list from a tag pair in the xml
	void SetVariableListTagPair(bool **ppbList, const TCHAR* szRootTagName,
		CvString* m_paszTagList, int iTagListLength, bool bDefaultListVal = false);

	// allocate and initialize a list from a tag pair in the xml
	void SetVariableListTagPair(CvString **ppszList, const TCHAR* szRootTagName,
		CvString* m_paszTagList, int iTagListLength, CvString szDefaultListVal = "");

	// MOD - START - Improved XML Loading
	// Allocates, initializes and records the length of an array of type references
	void SetInfoArray(int *piLength, int **ppiList, const TCHAR* szRootTagName);
	static void SetInfoArray(int *piLength, int **ppiList, std::vector<CvString>& aszTagNames);
	void CollectInfoArray(std::vector<CvString>& aszValues, const TCHAR* szRootTagName);
	// MOD - END - Improved XML Loading

	// create a hot key from a description
	CvWString CreateHotKeyFromDescription(const TCHAR* pszHotKey, bool bShift = false, bool bAlt = false, bool bCtrl = false);

	// set the variable to a default and load it from the xml if there are any children
	bool SetAndLoadVar(int** ppiVar, int iDefault=0);

	// function that sets the number of strings in a list, initializes the string to the correct length, and fills it from the
	// current xml file, it assumes that the current node is the parent node of the string list children
	bool SetStringList(CvString** ppszStringArray, int* piSize);

	// get the integer value for the keyboard mapping of the hotkey if it exists
	int GetHotKeyInt(const TCHAR* pszHotKeyVal);

	//---------------------------------------PRIVATE MEMBER VARIABLES---------------------------------
private:
	FXml* m_pFXml;						// member variable pointer to the current FXml class
	FXmlSchemaCache* m_pSchemaCache;	// keep a single schema cache, instead of loading the same schemas multiple times
	int m_iCurProgressStep;
	ProgressCB m_pCBFxn;

//---------------------------------------PRIVATE INTERFACE---------------------------------
private:
	void UpdateProgressCB(const char* szMessage=NULL);

	// take a character string of hex values and return their unsigned int value
	void MakeMaskFromString(unsigned int *puiMask, char* szMask);

	// find the tag name in the xml file and set the string parameter and num val parameter based on it's value
	void SetGlobalStringArray(CvString** ppszString, char* szTagName, int* iNumVals, bool bUseEnum=false);
	void SetDiplomacyCommentTypes(CvString** ppszString, int* iNumVals);	// sets diplomacy comments

	void SetGlobalUnitScales(float* pfLargeScale, float* pfSmallScale, char* szTagName);

#if (defined(__GNUC__) || defined(_USRDLL))
//#ifdef _USRDLL
	template <class T>
		void SetGlobalDefine(const char* szDefineName, T*& piDefVal)
	{ GC.getDefinesVarSystem()->GetValue(szDefineName, piDefVal); }
#endif
	//
	// template which can handle all info classes
	//
	// a dynamic value for the list size
#if (defined(__GNUC__) || defined(_USRDLL))
//#ifdef _USRDLL
	template <class T>
	void SetGlobalClassInfo(std::vector<T*>& aInfos, const char* szTagName, bool bTwoPass);
	template <class T>
	void LoadGlobalClassInfo(std::vector<T*>& aInfos, const char* szFileRoot, const char* szFileDirectory, const char* szXmlPath, bool bTwoPass, CvCacheObject* (CvDLLUtilityIFaceBase::*pArgFunction) (const TCHAR*) = NULL);
#endif
	void SetDiplomacyInfo(std::vector<CvDiplomacyInfo*>& DiploInfos, const char* szTagName);
	void LoadDiplomacyInfo(std::vector<CvDiplomacyInfo*>& DiploInfos, const char* szFileRoot, const char* szFileDirectory, const char* szXmlPath, CvCacheObject* (CvDLLUtilityIFaceBase::*pArgFunction) (const TCHAR*));

	//
	// special cases of set class info which don't use the template because of extra code they have
	//
	void SetGlobalActionInfo();
	void SetGlobalAnimationPathInfo(CvAnimationPathInfo** ppAnimationPathInfo, char* szTagName, int* iNumVals);
	//void SetGameText(const char* szTextGroup, const char* szTagName);
	void SetGameText(const char* szTextGroup, const char* szTagName, const std::string& language_name); // K-Mod

	// create a keyboard string from a KB code, Delete would be returned for KB_DELETE
	CvWString CreateKeyStringFromKBCode(const TCHAR* pszHotKey);

	void orderHotkeyInfo(int** ppiSortedIndex, int* pHotkeyIndex, int iLength);
    //void logMsg(char* format, ... ); //PORT OLD
    void logMsg(const char* format, ... ); //PORT NEW
};

#if (defined(__GNUC__) || defined(_USRDLL))
//#ifdef _USRDLL
//
/////////////////////////// inlines / templates
//

// MOD - START - Improved XML Loading

// Allocates, initializes and records the length of an array of type references with an accompanying integer value
template <class T, class InfoType>
void CvXMLLoadUtility::SetInfoValueArray(int *piLength, T **ppaList, const TCHAR* szContainerTagName, const TCHAR* szItemTagName, const TCHAR* szInfoTagName, const TCHAR* szValueTagName)
{
	char szInfo[256];
	int iValue;

	*piLength = 0;
	SAFE_DELETE_ARRAY(*ppaList);

	if (gDLL->getXMLIFace()->SetToChildByTagName(GetXML(), szContainerTagName))
	{
		if (gDLL->getXMLIFace()->SetToChildByTagName(GetXML(), szItemTagName))
		{
			std::vector<T> aValues;

			do
			{
				if (GetChildXmlValByName(szInfo, szInfoTagName) &&
					GetChildXmlValByName(&iValue, szValueTagName))
				{
					InfoType eInfo = (InfoType) FindInInfoClass(szInfo);
					if (eInfo > -1)
					{
						T kOutput = { eInfo, iValue };
						aValues.push_back(kOutput);
					}
				}
			} while(gDLL->getXMLIFace()->LocateNextSiblingNodeByTagName(GetXML(), const_cast<TCHAR*>(szItemTagName)));

			if (aValues.size() > 0)
			{
				*piLength = (int)aValues.size();
				*ppaList = new T[aValues.size()];
				for (uint i = 0; i < aValues.size(); i++)
				{
					(*ppaList)[i] = aValues[i];
				}
			}

			gDLL->getXMLIFace()->SetToParent(GetXML());
		}
		gDLL->getXMLIFace()->SetToParent(GetXML());
	}
}

template <class T, class TypeA, class TypeB>
void CvXMLLoadUtility::SetBasicEffectValues(std::vector<T>& aEffects, const TCHAR* szContainerTagName, const TCHAR* szItemTagName, const TCHAR* szTypeATagName, const TCHAR* szTypeBTagName, const TCHAR* szValueTagName)
{
	char szTypeA[256];
	char szTypeB[256];
	int iValue;

	if (gDLL->getXMLIFace()->SetToChildByTagName(GetXML(), szContainerTagName))
	{
		if (gDLL->getXMLIFace()->SetToChildByTagName(GetXML(), szItemTagName))
		{
			do
			{
				if (GetChildXmlValByName(szTypeA, szTypeATagName) &&
					GetChildXmlValByName(szTypeB, szTypeBTagName) &&
					GetChildXmlValByName(&iValue, szValueTagName))
				{
					TypeA eTypeA = (TypeA) FindInInfoClass(szTypeA);
					TypeB eTypeB = (TypeB) FindInInfoClass(szTypeB);

					if (eTypeA > -1 && eTypeB > -1)
					{
						T kOutput = { eTypeA, eTypeB, iValue };
						aEffects.push_back(kOutput);
					}
				}
			} while(gDLL->getXMLIFace()->LocateNextSiblingNodeByTagName(GetXML(), const_cast<TCHAR*>(szItemTagName)));

			gDLL->getXMLIFace()->SetToParent(GetXML());
		}
		gDLL->getXMLIFace()->SetToParent(GetXML());
	}
}

template <class InfoType, class EffectType>
void CvXMLLoadUtility::SetEconomicEffectMap(std::map<std::pair<InfoType, EffectType>, int>& map, const TCHAR* szContainerTagName, const TCHAR* szItemTagName, const TCHAR* szInfoTagName, const TCHAR* szCollectionTagName, const TCHAR* szValueTagName, int iMaxCollectionSize)
{
	char szInfo[256];
	int iValue;

	if (gDLL->getXMLIFace()->SetToChildByTagName(GetXML(), szContainerTagName))
	{
		if (gDLL->getXMLIFace()->SetToChildByTagName(GetXML(), szItemTagName))
		{
			do
			{
				if (GetChildXmlValByName(szInfo, szInfoTagName))
				{
					InfoType eInfo = (InfoType) FindInInfoClass(szInfo);
					if (eInfo > -1)
					{
						if (gDLL->getXMLIFace()->SetToChildByTagName(GetXML(), szCollectionTagName))
						{
							if (gDLL->getXMLIFace()->SetToChildByTagName(GetXML(), szValueTagName))
							{
								EffectType eEffect = (EffectType)0;
								do
								{
									FAssertMsg(eEffect < iMaxCollectionSize, "Too many items specified in XML effect collection");
									if (eEffect >= iMaxCollectionSize)
									{
										break;
									}

									gDLL->getXMLIFace()->GetLastNodeValue(m_pFXml, &iValue);

									if (iValue != 0)
									{
                                        std::pair<typename std::map<std::pair<InfoType, EffectType>, int>::iterator, bool> it = map.insert(std::make_pair(std::make_pair(eInfo, eEffect), iValue));

										if (!it.second)
										{
											(*(it.first)).second += iValue;
										}
									}

									eEffect = (EffectType)(eEffect + 1);
								} while(gDLL->getXMLIFace()->LocateNextSiblingNodeByTagName(GetXML(), const_cast<TCHAR*>(szValueTagName)));

								gDLL->getXMLIFace()->SetToParent(GetXML());
							}
							gDLL->getXMLIFace()->SetToParent(GetXML());
						}
					}
				}
			} while(gDLL->getXMLIFace()->LocateNextSiblingNodeByTagName(GetXML(), const_cast<TCHAR*>(szItemTagName)));

			gDLL->getXMLIFace()->SetToParent(GetXML());
		}
		gDLL->getXMLIFace()->SetToParent(GetXML());
	}
}

template <class InfoType, class EffectType>
void CvXMLLoadUtility::SetEconomicEffectLookup(const std::map<std::pair<InfoType, EffectType>, int>& map, int*** pppiLookup, int iNumInfoTypes, int iNumEffectTypes)
{
	// Allocate and initialize the array
	Init2DIntList(pppiLookup, iNumInfoTypes, iNumEffectTypes);

	// Dereference for convenience
	int** ppiLookup = *pppiLookup;

	// Copy data from the map (a binary tree which is properly sorted) to the lookup array
    for (typename std::map<std::pair<InfoType, EffectType>, int>::const_iterator it = map.begin(); it != map.end(); ++it)
	{
		ppiLookup[(*it).first.first][(*it).first.second] = (*it).second;
	}
}

template <class M, class T, class InfoType, class EffectType>
void CvXMLLoadUtility::SetEconomicEffectEnumeration(T** ppaEnumeration, int* piNumEffects, const TCHAR* szContainerTagName, const TCHAR* szItemTagName, const TCHAR* szInfoTagName, const TCHAR* szCollectionTagName, const TCHAR* szValueTagName, int iMaxCollectionSize)
{
	M map;
	SetEconomicEffectMap<InfoType, EffectType>(map, szContainerTagName, szItemTagName, szInfoTagName, szCollectionTagName, szValueTagName, iMaxCollectionSize);
	SetEconomicEffectEnumeration<T, InfoType, EffectType>(map, ppaEnumeration, piNumEffects);
	map.clear();
}

template <class T, class InfoType, class EffectType>
void CvXMLLoadUtility::SetEconomicEffectEnumeration(const std::map<std::pair<InfoType, EffectType>, int>& map, T** ppaEnumeration, int* piNumEffects)
{
	// Safely delete old data
	SAFE_DELETE_ARRAY(*ppaEnumeration);

	// Record the size of the array
	*piNumEffects = (int)map.size();

	// Allocate the array
	*ppaEnumeration = new T[map.size()];

	// Dereference for convenience
	T* paEnumeration = *ppaEnumeration;

	// Copy data from the map (a binary tree which is properly sorted) to the enumeration array
	int i = 0;
    for (typename std::map<std::pair<InfoType, EffectType>, int>::const_iterator it = map.begin(); it != map.end(); ++it)
	{
		T kOutput = { (*it).first.first, (*it).first.second, (*it).second };
		paEnumeration[i++] = kOutput;
	}
}

/*
template <class InfoType>
void CvXMLLoadUtility::CopyYieldEffects(const std::map<std::pair<InfoType, YieldTypes>, int>& aEconomicEffectMap, int** pppiList[NUM_YIELD_TYPES], int iNumInfoTypes)
{
	(*pppiList) = new int[iNumInfoTypes][NUM_YIELD_TYPES];
	//Init2DIntList(pppiList, iNumInfoTypes, iNumEffectTypes);
	int** ppiList = *pppiList;
	for (std::map<std::pair<InfoType, EffectType>, int>::const_iterator it = aEconomicEffectMap.begin(); it != aEconomicEffectMap.end(); ++it)
	{
		ppiList[(*it).first.first][(*it).first.second] = (*it).second;
	}
}
*/
// MOD - END - Improved XML Loading

template <class T>
void CvXMLLoadUtility::InitList(T **ppList, int iListLen, T val)
{
	FAssertMsg((0 <= iListLen),"list size to allocate is less than 0");
	*ppList = new T[iListLen];

	for (int i=0;i<iListLen;i++)
		(*ppList)[i] = val;
}

template <class T>
int CvXMLLoadUtility::SetCommerce(T** ppbCommerce)
{
	int i=0;			//loop counter
	int iNumSibs=0;		// the number of siblings the current xml node has
	T *pbCommerce;	// local pointer for the Commerce memory

	// Skip any comments and stop at the next value we might want
	if (SkipToNextVal())
	{
		// get the total number of children the current xml node has
		iNumSibs = gDLL->getXMLIFace()->GetNumChildren(m_pFXml);
		InitList(ppbCommerce, NUM_COMMERCE_TYPES);

		pbCommerce = *ppbCommerce;
		if (0 < iNumSibs)
		{
			// if the call to the function that sets the current xml node to it's first non-comment
			// child and sets the parameter with the new node's value succeeds
			if (GetChildXmlVal(&pbCommerce[0]))
			{
				FAssertMsg((iNumSibs <= NUM_COMMERCE_TYPES) , "For loop iterator is greater than array size");
				// loop through all the siblings, we start at 1 since we already have the first value
				for (i=1;i<iNumSibs;i++)
				{
					if (!GetNextXmlVal(&pbCommerce[i]))
					{
						break;
					}
				}
				gDLL->getXMLIFace()->SetToParent(m_pFXml);
			}
		}
	}

	return iNumSibs;
}
#endif

#endif	// XML_LOAD_UTILITY_H
