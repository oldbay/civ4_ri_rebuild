
from CvGameCoreDLL import *

from .utils import monitor

# Extensions/Mods
PB_MOD = True


class CyAudioGame:

    @monitor
    def Destroy2DSound(self, soundhandle):
        pass

    @monitor
    def Destroy3DSound(self, soundhandle):
        pass

    @monitor
    def Is2DSoundPlaying(self, soundhandle):
        return bool()

    @monitor
    def Is3DSoundPlaying(self, soundhandle):
        return bool()

    @monitor
    def Play2DSound(self, scriptname):
        return int()

    @monitor
    def Play2DSoundWithId(self, scriptId):
        return int()

    @monitor
    def Play3DSound(self, scriptname, x, y, z):
        return int()

    @monitor
    def Play3DSoundWithId(self, scriptId, x, y, z):
        return int()

    @monitor
    def Set2DSoundVolume(self, soundhandle, volume):
        pass

    @monitor
    def Set3DSoundPosition(self, soundhandle, x, y, z):
        pass

    @monitor
    def Set3DSoundVolume(self, soundhandle, volume):
        pass


class CyCamera:

    @monitor
    def GetBasePitch(self):
        return float()

    @monitor
    def GetBaseTurn(self):
        return float()

    @monitor
    def GetCameraMovementSpeed(self):
        return float()

    @monitor
    def GetCurrentPosition(self):
        return NiPoint3(0, 0, 0)

    @monitor
    def GetDefaultViewPortCenter(self):
        return NiPoint2(0, 0)

    @monitor
    def GetDestinationPosition(self):
        return NiPoint3(0, 0, 0)

    @monitor
    def GetLookAt(self, pt3LookAt):
        pass

    @monitor
    def GetLookAtSpeed(self):
        return float()

    @monitor
    def GetTargetDestination(self,):
        return NiPoint3(0, 0, 0)

    @monitor
    def GetZoom(self):
        return float()

    @monitor
    def JustLookAt(self, p3LookAt):
        pass

    @monitor
    def JustLookAtPlot(self, pPlot):
        pass

    @monitor
    def LookAt(self, pt3LookAt, CameraType, attackDirection):
        pass

    @monitor
    def LookAtUnit(self, unit):
        pass

    @monitor
    def MoveBaseTurnLeft(self, increment):
        pass

    @monitor
    def MoveBaseTurnRight(self, increment):
        pass

    @monitor
    def ReleaseLockedCamera(self,):
        pass

    @monitor
    def ResetZoom(self):
        pass

    @monitor
    def SetBasePitch(self, fBasePitch):
        pass

    @monitor
    def SetBaseTurn(self, baseTurn):
        pass

    @monitor
    def SetCameraMovementSpeed(self, eSpeed):
        pass

    @monitor
    def SetCurrentPosition(self, point):
        pass

    @monitor
    def SetDestinationPosition(self, point):
        pass

    @monitor
    def SetLookAtSpeed(self, fSpeed):
        pass

    @monitor
    def SetTargetDestination(self, point):
        pass

    @monitor
    def SetViewPortCenter(self, pt2Center):
        pass

    @monitor
    def SetZoom(self, zoom):
        pass

    @monitor
    def SimpleLookAt(self, position, target):
        pass

    @monitor
    def Translate(self, translation):
        pass

    @monitor
    def ZoomIn(self, increment):
        pass

    @monitor
    def ZoomOut(self, increment):
        pass

    @monitor
    def isMoving(self):
        return bool()

    @monitor
    def setOrthoCamera(self, bNewValue):
        pass


class CyDiplomacy:

    @monitor
    def addUserComment(self, eComment, iData1, iData2, szString, txtArgs):
        pass

    @monitor
    def atWar(self):
        return bool()

    @monitor
    def clearUserComments(self):
        pass

    @monitor
    def closeScreen(self):
        pass

    @monitor
    def counterPropose(self):
        return bool()

    @monitor
    def declareWar(self):
        pass

    @monitor
    def diploEvent(self, iDiploEvent, iData1, iData2):
        pass

    @monitor
    def endTrade(self):
        pass

    @monitor
    def getData(self):
        return int()

    @monitor
    def getOpponentCivName(self):
        return str()

    @monitor
    def getOpponentName(self):
        return str()

    @monitor
    def getOurCivName(self):
        return str()

    @monitor
    def getOurName(self):
        return str()

    @monitor
    def getOurScore(self):
        return int()

    @monitor
    def getPlayerTradeOffer(self, iIndex):
        return TradeData()

    @monitor
    def getTheirScore(self):
        return int()

    @monitor
    def getTheirTradeOffer(self, iIndex):
        return TradeData()

    @monitor
    def getWhoTradingWith(self):
        return -1  # Type

    @monitor
    def hasAnnualDeal(self):
        return bool()

    @monitor
    def implementDeal(self):
        pass

    @monitor
    def isAIOffer(self):
        return bool()

    @monitor
    def isSeparateTeams(self):
        return bool()

    @monitor
    def makePeace(self):
        pass

    @monitor
    def offerDeal(self):
        return bool()

    @monitor
    def ourOfferEmpty():
        return bool()

    @monitor
    def performHeadAction(self, eAction):
        pass

    @monitor
    def setAIComment(self, iComment):
        pass

    @monitor
    def setAIOffer(self, bOffer):
        pass

    @monitor
    def setAIString(self, szString, txtArgs):
        pass

    @monitor
    def showAllTrade(self, bShow):
        pass

    @monitor
    def startTrade(self, iComment, bRenegotiate):
        pass

    @monitor
    def theirOfferEmpty(self):
        return bool()

    @monitor
    def theirVassalTribute(self):
        return bool()


class CvDiplomacyResponse:

    @monitor
    def getAttitudeTypes(self, i=-1):
        return bool()

    @monitor
    def getCivilizationTypes(self, i=-1):
        return bool()

    @monitor
    def getDiplomacyPowerTypes(self, i=-1):
        return bool()

    @monitor
    def getDiplomacyText(self, i=-1):
        return str()

    @monitor
    def getLeaderHeadTypes(self, i=-1):
        return bool()

    @monitor
    def getNumDiplomacyText(self):
        return int()

    @monitor
    def setAttitudeTypes(self, i, bVal):
        pass

    @monitor
    def setCivilizationTypes(self, i, bVal):
        pass

    @monitor
    def setDiplomacyPowerTypes(self, i, bVal):
        pass

    @monitor
    def setLeaderHeadTypes(self, i, bVal):
        pass

    @monitor
    def setNumDiplomacyText(self, i):
        pass
    pass


class CyEngine:

    @monitor
    def addColoredPlot(self, plotX, plotY, color, iLayer):
        pass

    @monitor
    def addColoredPlotAlt(self, plotX, plotY, iPlotStyle, iLayer, szColor, fAlpha):
        pass

    @monitor
    def addLandmark(self, pPlot, caption):
        pass

    @monitor
    def addLandmarkPopup(self, pPlot):
        pass

    @monitor
    def addSign(self, plot, playerType, caption):
        pass

    @monitor
    def clearAreaBorderPlots(self, iLayer):
        pass

    @monitor
    def clearColoredPlots(self, iLayer):
        pass

    @monitor
    def fillAreaBorderPlot(self, plotX, plotY, color, iLayer):
        pass

    @monitor
    def fillAreaBorderPlotAlt(self, plotX, plotY, iLayer, szColor, fAlpha):
        pass

    @monitor
    def getCityBillboardVisibility(self):
        return bool()

    @monitor
    def getCultureVisibility(self):
        return bool()

    @monitor
    def getNumSigns(self):
        return int()

    @monitor
    def getSelectionCursorVisibility(self):
        return bool()

    @monitor
    def getSignByIndex(self, index):
        return CySign()

    @monitor
    def getUnitFlagVisibility(self):
        return bool()

    @monitor
    def getUpdateRate(self):
        return float()

    @monitor
    def isDirty(self, eBit):
        return bool()

    @monitor
    def isGlobeviewUp(self):
        return bool()

    @monitor
    def isNone(self):
        return bool()

    @monitor
    def reloadEffectInfos(self):
        pass

    @monitor
    def removeLandmark(self, pPlot):
        pass

    @monitor
    def removeSign(self, pPlot, playerType):
        pass

    @monitor
    def setCityBillboardVisibility(self, bState):
        pass

    @monitor
    def setCultureVisibility(self, bState):
        pass

    @monitor
    def setDirty(self, eBit, bNewValue):
        pass

    @monitor
    def setFogOfWar(self, bState):
        pass

    @monitor
    def setSelectionCursorVisibility(self, bState):
        pass

    @monitor
    def setUnitFlagVisibility(self, bState):
        pass

    @monitor
    def setUpdateRate(self, fUpdateRate):
        pass

    @monitor
    def toggleGlobeview(self):
        pass

    @monitor
    def triggerEffect(self, iEffect, plotPoint):
        pass


class CyFractal:

    class FracValClass:
        DEFAULT_FRAC_X_EXP = -1
        DEFAULT_FRAC_Y_EXP = -1
        FRAC_INVERT_HEIGHTS = -1
        FRAC_POLAR = -1
        FRAC_CENTER_RIFT = -1
        FRAC_WRAP_X = -1
        FRAC_WRAP_Y = -1

    FracVals = FracValClass()

    @monitor
    def fracInit(self, iNewXs, iNewYs, iGrain, random, iFlags, iFracXExp, iFracYExp):
        pass

    @monitor
    def fracInitHints(
        self, iNewXs, iNewYs, iGrain, random, iFlags, pRifts, iFracXExp, iFracYExp
    ):
        pass

    @monitor
    def fracInitRifts(
        self, iNewXs, iNewYs, iGrain, random, iFlags, hintsData, iFracXExp, iFracYExp
    ):
        pass

    @monitor
    def getHeight(self, x, y):
        return int()

    @monitor
    def getHeightFromPercent(self, iPercent):
        return int()


class CyGFlyoutMenu:

    @monitor
    def addTextItem(self, szLabel, szPythonCBModule, szPythonCBFxn):
        pass

    @monitor
    def create(self):
        pass

    @monitor
    def destroy(self):
        pass

    @monitor
    def hide(self):
        pass

    @monitor
    def show(self):
        pass


class CyGInterfaceScreen:

    @monitor
    def addBonusGraphicGFC(
        self, szName, iBonus, iX, iY, iWidth, iHeight,
        eWidgetType, iData1, iData2, fxRotation,
        fzRotation, fScale, bShowBackground
    ):
        pass

    @monitor
    def addBuildingGraphicGFC(
        self, szName, iBuilding, iX, iY, iWidth, iHeight,
        eWidgetType, iData1, iData2, fxRotation,
        fzRotation, fScale, bShowBackground
    ):
        pass

    @monitor
    def addCheckBoxGFC(
        self, szName, szTexture, szHiliteTexture, iX, iY, iWidth, iHeight,
        eWidgetType, iData1, iData2, eStyle
    ):
        pass

    @monitor
    def addCheckBoxGFCAt(
        self, szName, szTexture, szHiliteTexture, iX, iY, iWidth, iHeight,
        eWidgetType, iData1, iData2, eStyle, bSafeFocus
    ):
        pass

    @monitor
    def addDDSGFC(
        self, szName, szTexture, iX, iY, iWidth, iHeight, eWidgetType, iData1, iData2
    ):
        pass

    @monitor
    def addDDSGFCAt(
        self, szName, szAttachTo, szTexture, iX, iY, iWidth, iHeight,
        eWidgetType, iData1, iData2, bOption
    ):
        pass

    @monitor
    def addDrawControl(
        self, szName, szTexture, iX, iY, iWidth, iHeight, eWidgetType, iData1, iData2
    ):
        pass

    @monitor
    def addDrawControlAt(
        self, szName, szAttachTo, szTexture, iX, iY, iWidth, iHeight,
        eWidgetType, iData1, iData2
    ):
        pass

    @monitor
    def addDropDownBoxGFC(
        self, szName, iX, iY, iWidth, eWidgetType, iData1, iData2, eFontType
    ):
        pass

    @monitor
    def addEditBoxGFC(
        self, szName, iX, iY, iWidth, iHeight, eWidgetType, iData1, iData2, eFont
    ):
        pass

    @monitor
    def addFlagWidgetGFC(
        self, szName, iX, iY, iWidth, iHeight, iOwner, eWidgetType, iData1, iData2
    ):
        pass

    @monitor
    def addGraphData(self, szName, fX, fY, uiLayer):
        pass

    @monitor
    def addGraphLayer(self, szName, uiLayer, iColor):
        pass

    @monitor
    def addGraphWidget(
        self, szName, szAttachTo, szFile, fX, fY, fZ, fWidth, fHeight,
        eWidgetType, iData1, iData2
    ):
        pass

    @monitor
    def addImprovementGraphicGFC(
        self, szName, iImprovement, iX, iY, iWidth, iHeight, eWidgetType,
        iData1, iData2, fxRotation, fzRotation, fScale, bShowBackground
    ):
        pass

    @monitor
    def addItemToTableGFC(self, szAttachTo, szText, eWidgetType, iData1, iData2):
        pass

    @monitor
    def addLeaderheadGFC(
        self, szName, eWho, eInitAttitude, iX, iY,
        iWidth, iHeight, eWidget, iData1, iData2
    ):
        pass

    @monitor
    def addLineGFC(self, szDrawCtrlName, szName, iStartX, iStartY, iEndX, iEndY, eColor):
        pass

    @monitor
    def addListBoxGFC(self, szName, helpText, iX, iY, iWidth, iHeight, eStyle):
        pass

    @monitor
    def addModelGraphicGFC(
        self, szName, szFile, iX, iY, iWidth, iHeight, eWidgetType,
        iData1, iData2, fxRotation, fzRotation, fScale
    ):
        pass

    @monitor
    def addMultiListControlGFC(
        self, szName, helpText, iX, iY, iWidth, iHeight,
        numLists, defaultWidth, defaultHeight, eStyle
    ):
        pass

    @monitor
    def addMultiListControlGFCAt(
        self, szName, helpText, iX, iY, iWidth, iHeight,
        numLists, defaultWidth, defaultHeight, eStyle
    ):
        pass

    @monitor
    def addMultilineText(
        self, szName, szText, iX, iY, iWidth, iHeight,
        eType, iData1, iData2, iJustify
    ):
        pass

    @monitor
    def addPanel(
        self, szName, title, helpText, bVerticalLayout, bScrollable,
        iX, iY, iWidth, iHeight, eStyle
    ):
        pass

    @monitor
    def addPlotGraphicGFC(
        self, szName, iX, iY, iWidth, iHeight, pPlot, iDistance,
        renderUnits, eWidgetType, iData1, iData2
    ):
        pass

    @monitor
    def addPullDownString(self, szName, szString, iType, iData, bSelected):
        pass

    @monitor
    def addReligionMovieWidgetGFC(
        self, szName, szFile, iX, iY, iWidth, iHeight, eWidgetType, iData1, iData2
    ):
        pass

    @monitor
    def addScrollPanel(self, szName, title, iX, iY, iWidth, iHeight, eStyle):
        pass

    @monitor
    def addSimpleTableControlGFC(self, szName, iX, iY, iWidth, iHeight, eStyle):
        pass

    @monitor
    def addSlider(
        self, szName, iX, iY, iWidth, iHeight, iDefault, iMin, iMax,
        eWidgetType, iData1, iData2, bIsVertical
    ):
        pass

    @monitor
    def addSpaceShipWidgetGFC(
        self, szName, iX, iY, iWidth, iHeight, projectType, artType,
        eWidgetType, iData1, iData2
    ):
        pass

    @monitor
    def addSpecificUnitGraphicGFC(
        self, szName, pUnit, iX, iY, iWidth, iHeight, eWidgetType, iData1, iData2,
        fxRotation, fzRotation, fScale, bShowBackground
    ):
        pass

    @monitor
    def addStackedBarGFC(
        self, szName, iX, iY, iWidth, iHeight, iNumBars, eWidgetType, iData1, iData2
    ):
        pass

    @monitor
    def addStackedBarGFCAt(
        self, szName, szAttachTo, iX, iY, iWidth, iHeight, iNumBars,
        eWidgetType, iData1, iData2
    ):
        pass

    @monitor
    def addTableControlGFC(
        self, szName, numColumns, iX, iY, iWidth, iHeight, bIncludeHeaders, bDrawGrid,
        iconWidth, iconHeight, style
    ):
        pass

    @monitor
    def addTableControlGFCWithHelp(
        self, szName, numColumns, iX, iY, iWidth, iHeight, bIncludeHeaders, bDrawGrid,
        iconWidth, iconHeight, style, szHelpText
    ):
        pass

    @monitor
    def addTableHeaderGFC(self, szAttachTo, szText, iCol, eWidgetType, iData1, iData2):
        pass

    @monitor
    def addToModelGraphicGFC(self, szName, szFile):
        pass

    @monitor
    def addUnitGraphicGFC(
        self, szName, iUnit, iX, iY, iWidth, iHeight, eWidgetType, iData1, iData2,
        fxRotation, fzRotation, fScale, bShowBackground
    ):
        pass

    @monitor
    def appendListBoxString(self, szAttachTo, item, eType, iData1, iData2, iJustify):
        pass

    @monitor
    def appendListBoxStringNoUpdate(self, szAttachTo, item, eType, iData1, iData2, iJustify):
        pass

    @monitor
    def appendMultiListButton(
        self, szAttachTo, szTexture, listId, eWidgetType, iData1, iData2, bOption
    ):
        pass

    @monitor
    def appendTableRow(self, szName):
        return int()

    @monitor
    def attachButtonGFC(self, szAttachTo, szName, szText, eWidgetType, iData1, iData2):
        pass

    @monitor
    def attachCheckBoxGFC(
        self, szAttachTo, szName, szTexture, szHiliteTexture, iWidth, iHeight,
        eWidgetType, iData1, iData2, eStyle
    ):
        pass

    @monitor
    def attachControlToTableCell(self, szControlName, szTableName, iRow, iColumn):
        pass

    @monitor
    def attachDropDownBoxGFC(self, szAttachTo, szName, bExpand):
        pass

    @monitor
    def attachImageButton(
        self, szAttachTo, szName, szTexture, eSize, eWidgetType, iData1, iData2, bOption
    ):
        pass

    @monitor
    def attachLabel(self, szAttachTo, szName, szText):
        pass

    @monitor
    def attachListBoxGFC(self, szAttachTo, szName, helpText, eStyle):
        pass

    @monitor
    def attachMultiListControlGFC(
        self, szAttachTo, szName, helpText, numLists, defaultWidth, defaultHeight, eStyle
    ):
        pass

    @monitor
    def attachMultilineText(self, szAttachTo, szName, szText, eType, iData1, iData2, iJustify):
        pass

    @monitor
    def attachPanel(
        self, szAttachTo, szName, title, helpText, bVerticalLayout, bScrollable, eStyle
    ):
        pass

    @monitor
    def attachPanelAt(
        self, szAttachTo, szName, title, helpText, bVerticalLayout, bScrollable, eStyle,
        iX, iY, iWidth, iHeight, eWidgetType, iData1, iData2
    ):
        pass

    @monitor
    def attachSeparator(self, szAttachTo, szName, bVertical):
        pass

    @monitor
    def attachSlider(
        self, szAttachTo, szName, iX, iY, iWidth, iHeight, iDefault, iMin, iMax,
        eWidgetType, iData1, iData2, bIsVertical
    ):
        pass

    @monitor
    def attachTableControlGFC(
        self, szAttachTo, szName, numColumns, bIncludeHeaders, bDrawGrid,
        iconWidth, iconHeight, style
    ):
        pass

    @monitor
    def attachTextGFC(self, szAttachTo, szName, text, eFont, eType, iData1, iData2):
        pass

    @monitor
    def bringMinimapToFront(self):
        pass

    @monitor
    def centerX(self, iX):
        return int()

    @monitor
    def centerY(self, iY):
        return int()

    @monitor
    def changeDDSGFC(self, szName, szTexture):
        pass

    @monitor
    def changeDrawControl(self, szName, szTexture):
        pass

    @monitor
    def changeImageButton(self, szName, szTexture):
        pass

    @monitor
    def changeModelGraphicTextureGFC(self, szName, szFile):
        pass

    @monitor
    def clearGraphData(self, szName, uiLayer):
        pass

    @monitor
    def clearListBoxGFC(self, szListBoxName):
        pass

    @monitor
    def clearMultiList(self, szName):
        pass

    @monitor
    def commitTableRow(self, szAttachTo):
        pass

    @monitor
    def deleteWidget(self, pszName):
        pass

    @monitor
    def disableMultiListButton(self, szName, iListId, iIndexId, szTexture):
        pass

    @monitor
    def enable(self, szName, bEnable):
        pass

    @monitor
    def enableGridlines(self, szName, bVertical, bHorizontal):
        pass

    @monitor
    def enableMultiListPulse(self, szName, bEnable, listId, iIndexId):
        pass

    @monitor
    def enableSelect(self, szControlName, bEnable):
        pass

    @monitor
    def enableSort(self, szName):
        pass

    @monitor
    def enableWorldSounds(self, bEnable):
        pass

    @monitor
    def getCheckBoxState(self, szName):
        return bool()

    @monitor
    def getCurrentTime(self):
        return int()

    @monitor
    def getEditBoxString(self, szName):
        return str()

    @monitor
    def getPullDownData(self, szName, iIndex):
        return int()

    @monitor
    def getPullDownType(self, szName, iIndex):
        return int()

    @monitor
    def getPythonFileID(self):
        return int()

    @monitor
    def getRenderInterfaceOnly(self):
        return bool()

    @monitor
    def getScreenGroup(self):
        return int()

    @monitor
    def getSelectedPullDownID(self, szName):
        return int()

    @monitor
    def getTableNumColumns(self, szName):
        return int()

    @monitor
    def getTableNumRows(self, szName):
        return int()

    @monitor
    def getTableText(self, szName, iColumn, iRow):
        pass

    @monitor
    def getXResolution(self):
        return int()

    @monitor
    def getYResolution(self):
        return int()

    @monitor
    def hide(self, szName):
        pass

    @monitor
    def hideEndTurn(self, szName):
        pass

    @monitor
    def hideList(self, iID):
        pass

    @monitor
    def hideScreen(self):
        pass

    @monitor
    def initMinimap(self, iLeft, iRight, iTop, iBottom, fZ):
        pass

    @monitor
    def isActive(self):
        return bool()

    @monitor
    def isAlwaysShown(self):
        return bool()

    @monitor
    def isPersistent(self):
        return bool()

    @monitor
    def isRequiredForcedRedraw(self):
        return bool()

    @monitor
    def isRowSelected(self, szName, iRow):
        return bool()

    @monitor
    def leaderheadKeyInput(self, szName, key):
        pass

    @monitor
    def markMinimapTexturePlotDirty(self, iPlotX, iPlotY):
        pass

    @monitor
    def markRenderTexturesDirty(self):
        pass

    @monitor
    def minimapClearAllFlashingTiles(self):
        pass

    @monitor
    def minimapFlashPlot(self, iX, iY, eColor, fSeconds):
        pass

    @monitor
    def modifyLabel(self, szName, szText, uiFlags):
        pass

    @monitor
    def modifyString(self, szName, szText, uiFlags):
        pass

    @monitor
    def moveBackward(self, szName):
        pass

    @monitor
    def moveForward(self, szName):
        pass

    @monitor
    def moveItem(self, szName, fX, fY, fZ):
        pass

    @monitor
    def moveToBack(self, szName):
        pass

    @monitor
    def moveToFront(self, szName):
        pass

    @monitor
    def performLeaderheadAction(self, szName, eAction):
        pass

    @monitor
    def playMovie(self, szMovieName, fX, fY, fWidth, fHeight, fZ):
        pass

    @monitor
    def prependListBoxString(self, szAttachTo, item, eType, iData1, iData2, iJustify):
        pass

    @monitor
    def registerHideList(self, szNames, iSize, iID):
        pass

    @monitor
    def removeLineGFC(self, szDrawCtrlName, szName):
        pass

    @monitor
    def renderMinimapWorldTexture(self):
        pass

    @monitor
    def selectMultiList(self, szName, iListID):
        pass

    @monitor
    def selectRow(self, szName, iRow, bSelected):
        pass

    @monitor
    def setActivation(self, szName, activation):
        pass

    @monitor
    def setAlwaysShown(self, bAlwaysShown):
        pass

    @monitor
    def setBarPercentage(self, szName, iBar, fPercent):
        pass

    @monitor
    def setButtonGFC(
        self, szName, szText, szTexture, iX, iY, imageWidth, imageHeight,
        eWidgetType, iData1, iData2, eStyle
    ):
        pass

    @monitor
    def setCloseOnEscape(self, bCloseOnEscape):
        pass

    @monitor
    def setDimensions(self, iX, iY, iWidth, iHeight):
        pass

    @monitor
    def setDying(self, bDying):
        pass

    @monitor
    def setEditBoxMaxCharCount(self, szName, maxCharCount, preferredCharCount):
        pass

    @monitor
    def setEditBoxString(self, szName, szString):
        pass

    @monitor
    def setEditBoxTextColor(self, szName, kColor):
        pass

    @monitor
    def setEndTurnState(self, szName, szText):
        pass

    @monitor
    def setExitText(self, szText, uiFlags, fX, fY, fZ, eFont):
        pass

    @monitor
    def setFocus(self, szName):
        pass

    @monitor
    def setForcedRedraw(self, bRequiresForcedRedraw):
        pass

    @monitor
    def setGraphGrid(self, szName, fXstart, fdX, fYstart, fdY):
        pass

    @monitor
    def setGraphLabelX(self, szName, szLabel):
        pass

    @monitor
    def setGraphLabelY(self, szName, szLabel):
        pass

    @monitor
    def setGraphXDataRange(self, szName, fXmin, fXmax):
        pass

    @monitor
    def setGraphYDataRange(self, szName, fYmin, fYmax):
        pass

    @monitor
    def setHelpLabel(self, szName, szAtttachTo, szText, uiFlags, fX, fY, fZ, eFont, szHelpText):
        pass

    @monitor
    def setHelpTextArea(
        self, fWidth, eFont, fX, fY, fZ, bFloating, szArtFile, bExpandRight,
        bExpandDown, uiFlags, iMinWidth
    ):
        pass

    @monitor
    def setHelpTextString(self, szString):
        pass

    @monitor
    def setHitTest(self, szName, hitTest):
        pass

    @monitor
    def setImageButton(
        self, szName, szTexture, iX, iY, iWidth, iHeight, eWidgetType, iData1, iData2
    ):
        pass

    @monitor
    def setImageButtonAt(
        self, szName, szAttachTo, szTexture, iX, iY, iWidth, iHeight,
        eWidgetType, iData1, iData2
    ):
        pass

    @monitor
    def setLabel(
        self, szName, szAtttachTo, szText, uiFlags, fX, fY, fZ,
        eFont, eType, iData1, iData2
    ):
        pass

    @monitor
    def setLabelAt(
        self, szName, szAttachTo, szText, uiFlags, fX, fY, fZ,
        eFont, eType, iData1, iData2
    ):
        pass

    @monitor
    def setLeaderheadAdvisor(self, szName, eAdvisor):
        pass

    @monitor
    def setLeaderheadMood(self, szName, eAttitude):
        pass

    @monitor
    def setListBoxStringGFC(self, szName, item, szText, eType, iData1, iData2, iJustify):
        pass

    @monitor
    def setMainInterface(self, bMain):
        pass

    @monitor
    def setMinimapColor(self, eMinimapMode, iX, iY, iColor, fAlpha):
        pass

    @monitor
    def setMinimapMap(self, pReplayInfo, iLeft, iRight, iTop, iBottom, fZ):
        pass

    @monitor
    def setMinimapMode(self, eMode):
        pass

    @monitor
    def setMinimapNoRender(self, value):
        pass

    @monitor
    def setMinimapSectionOverride(self, left, bottom, right, top):
        pass

    @monitor
    def setModelGraphicRotationRateGFC(self, szName, rate):
        pass

    @monitor
    def setPanelColor(self, szName, iRed, iGreen, iBlue):
        pass

    @monitor
    def setPanelSize(self, szName, iX, iY, iWidth, iHeight):
        pass

    @monitor
    def setPersistent(self, bPersistent):
        pass

    @monitor
    def setRenderInterfaceOnly(self, val):
        pass

    @monitor
    def setScreenGroup(self, iGroup):
        pass

    @monitor
    def setSelectedListBoxStringGFC(self, szName, item):
        pass

    @monitor
    def setShowFor(self, i):
        pass

    @monitor
    def setSound(self, pszSound):
        pass

    @monitor
    def setSoundId(self, iSoundId):
        pass

    @monitor
    def setSpaceShip(self, projectType):
        pass

    @monitor
    def setStackedBarColors(self, szName, iBar, eColor):
        pass

    @monitor
    def setStackedBarColorsAlpha(self, szName, iBar, eColor, fAlpha):
        pass

    @monitor
    def setStackedBarColorsRGB(self, szName, iBar, iRed, iGreen, iBlue, fAlpha):
        pass

    @monitor
    def setState(self, szName, eState):
        pass

    @monitor
    def setStyle(self, szName, szStyle):
        pass

    @monitor
    def setTableColumnHeader(self, szName, iColumn, header, iWidth):
        pass

    @monitor
    def setTableColumnRightJustify(self, szName, iCol):
        pass

    @monitor
    def setTableDate(
        self, szName, iColumn, iRow, text, szIcon, eWidgetType, iData1, iData2, iJustify
    ):
        pass

    @monitor
    def setTableInt(
        self, szName, iColumn, iRow, text, szIcon, eWidgetType, iData1, iData2, iJustify
    ):
        pass

    @monitor
    def setTableNumRows(self, szName, numRows):
        pass

    @monitor
    def setTableRowHeight(self, szName, iRow, iHeight):
        pass

    @monitor
    def setTableText(
        self, szName, iColumn, iRow, text, szIcon, eWidgetType, iData1, iData2, iJustify
    ):
        pass

    @monitor
    def setTableTextKey(
        self, szName, iColumn, szKey, iRowTest, text, eWidgetType,
        iData1, iData2, iJustify, iNumRows
    ):
        pass

    @monitor
    def setText(
        self, szName, szAtttachTo, szText, uiFlags, fX, fY, fZ,
        eFont, eType, iData1, iData2
    ):
        pass

    @monitor
    def setTextAt(
        self, szName, szAttachTo, szText, uiFlags, fX, fY, fZ,
        eFont, eType, iData1, iData2
    ):
        pass

    @monitor
    def setToolTipAlignment(self, szName, alignment):
        pass

    @monitor
    def setViewMin(self, szName, iWidth, iHeight):
        pass

    @monitor
    def show(self, szName):
        pass

    @monitor
    def showEndTurn(self, szName):
        pass

    @monitor
    def showRange(self, szName, iLow, iHigh):
        pass

    @monitor
    def showScreen(self, bState, bPassInput):
        pass

    @monitor
    def showWindowBackground(self, bShow):
        pass

    @monitor
    def spaceShipCanChangeType(self, projectType):
        return bool()

    @monitor
    def spaceShipChangeType(self, projectType):
        pass

    @monitor
    def spaceShipFinalize(self):
        pass

    @monitor
    def spaceShipLaunch(self):
        pass

    @monitor
    def spaceShipZoom(self, projectType):
        pass

    @monitor
    def updateAppropriateCitySelection(self, szName, iNumRows):
        pass

    @monitor
    def updateListBox(self, szAttachTo):
        pass

    @monitor
    def updateMinimap(self, fTime):
        pass

    @monitor
    def updateMinimapColorFromMap(self, eMode, fAlpha):
        pass

    @monitor
    def updateMinimapSection(self, bWholeMap):
        pass

    @monitor
    def updateMinimapVisibility(self):
        pass


class CyGTabCtrl:

    @monitor
    def addSectionButton(
        self, szLabel, szPythonCBModule, szPythonCBFxn, szPythonID, iTabIndex
    ):
        pass

    @monitor
    def addSectionCheckbox(
        self, szLabel, szPythonCBModule, szPythonCBFxn,
        szPythonID, iTabIndex, bInitialState
    ):
        pass

    @monitor
    def addSectionDropdown(
        self, szItems, szPythonCBModule, szPythonCBFxn,
        szPythonID, iTabIndex, iInitialSelection
    ):
        pass

    @monitor
    def addSectionEditCtrl(
        self, szLabel, szPythonCBModule, szPythonCBFxn, szPythonID, iTabIndex
    ):
        pass

    @monitor
    def addSectionLabel(self, szLabel, iTabIndex):
        pass

    @monitor
    def addSectionRadioButton(
        self, szLabel, szPythonCBModule, szPythonCBFxn,
        szPythonID, iTabIndex, bInitialState
    ):
        pass

    @monitor
    def addSectionSeparator(self, iTab):
        pass

    @monitor
    def addSectionSlider(
        self, szLabel, szPythonCBModule, szPythonCBFxn, szPythonID, iTabIndex,
        iMin, iMax, iInitialVal, iFormatNumber, iFormatDecimal
    ):
        pass

    @monitor
    def addSectionSpinner(
        self, szLabel, szPythonCBModule, szPythonCBFxn, szPythonID, iTabIndex,
        fMin, fMax, fInc, fInitialVal
    ):
        pass

    @monitor
    def addTabSection(self, szLabel):
        pass

    @monitor
    def attachButton(
        self, szParent, szName, szLabel, szPythonCBModule, szPythonCBFxn, szPythonID
    ):
        pass

    @monitor
    def attachCheckBox(
        self, szParent, szName, szLabel, szPythonCBModule, szPythonCBFxn,
        szPythonID, bInitialState
    ):
        pass

    @monitor
    def attachDropDown(
        self, szParent, szName, szID, szItems, szPythonCBModule, szPythonCBFxn,
        szPythonID, iInitialSelection
    ):
        pass

    @monitor
    def attachEdit(
        self, szParent, szName, szLabel, szPythonCBModule, szPythonCBFxn, szPythonID
    ):
        pass

    @monitor
    def attachExpandSpacer(self, self, szParent):
        pass

    @monitor
    def attachFixedSpacer(self, szParent, iSize):
        pass

    @monitor
    def attachHBox(self, szParent, szName):
        pass

    @monitor
    def attachHSeparator(self, szParent, szName):
        pass

    @monitor
    def attachHSlider(
        self, szParent, szName, szPythonCBModule, szPythonCBFxn, szPythonID,
        iMin, iMax, iInitialVal
    ):
        pass

    @monitor
    def attachImage(self, szParent, szName, szFilename):
        pass

    @monitor
    def attachLabel(self, szParent, szName, szLabel):
        pass

    @monitor
    def attachPanel(self, szParent, szName):
        pass

    @monitor
    def attachRadioButton(
        self, szParent, szName, szLabel, szPythonCBModule,
        szPythonCBFxn, szPythonID, bInitialState
    ):
        pass

    @monitor
    def attachScrollPanel(self, szParent, szName):
        pass

    @monitor
    def attachSpacer(self, szParent):
        pass

    @monitor
    def attachSpinner(
        self, szParent, szName, szPythonCBModule, szPythonCBFxn, szPythonID,
        fMin, fMax, fInc, fInitialVal, iFormatNumber, iFormatDecimal
    ):
        pass

    @monitor
    def attachTabItem(self, szName, szLabel):
        pass

    @monitor
    def attachTitledPanel(self, szParent, szName, szLabel):
        pass

    @monitor
    def attachVBox(self, szParent, szName):
        pass

    @monitor
    def attachVSeparator(self, szParent, szName):
        pass

    @monitor
    def attachVSlider(
        self, szParent, szName, szPythonCBModule, szPythonCBFxn, szPythonID,
        iMin, iMax, iInitialVal
    ):
        pass

    @monitor
    def changeDropdownContents(self, szID, szItems):
        pass

    @monitor
    def create(self):
        pass

    @monitor
    def createByName(self, name):
        pass

    @monitor
    def destroy(self):
        pass

    @monitor
    def enable(self, bVal):
        pass

    @monitor
    def getActiveTab(self):
        return bool()

    @monitor
    def getCheckBoxState(self, szTabName, szButtonText):
        pass

    @monitor
    def getControlsExpanding(self):
        return bool()

    @monitor
    def getDropDownSelection(self, szTabName, szID):
        pass

    @monitor
    def getRadioButtonState(self, szTabName, szButtonText):
        pass

    @monitor
    def getRadioValue(self, szName):
        return float()

    @monitor
    def getText(self, szName):
        return str()

    @monitor
    def getValue(self, szName):
        return float()

    @monitor
    def isEnabled(self):
        return bool()

    @monitor
    def isNone(self):
        return bool()

    @monitor
    def setActivation(self, szName, szActivationType):
        pass

    @monitor
    def setCheckBoxState(self, szTabName, szButtonText, bState):
        pass

    @monitor
    def setColumnLength(self, iSize):
        pass

    @monitor
    def setControlFlag(self, szName, szFlag):
        pass

    @monitor
    def setControlsExpanding(self, bExp):
        pass

    @monitor
    def setDropDownSelection(self, szTabName, szID, iSelection):
        pass

    @monitor
    def setEditCtrlText(self, szTabName, szEditCtrlText, szNewText):
        pass

    @monitor
    def setEnabled(self, szName, bEnabled):
        pass

    @monitor
    def setFocus(self, szName):
        pass

    @monitor
    def setHitTest(self, szName, szHitTestType):
        pass

    @monitor
    def setKeyFocus(self, szName, szKey, szTarget):
        pass

    @monitor
    def setLayoutFlag(self, szName, szFlag):
        pass

    @monitor
    def setModal(self, modal):
        pass

    @monitor
    def setNumColumns(self, iSize):
        pass

    @monitor
    def setRadioButtonState(self, szTabName, szButtonText, bState):
        pass

    @monitor
    def setRadioValue(self, szName, fValue):
        pass

    @monitor
    def setSize(self, width, height):
        pass

    @monitor
    def setSliderWidth(self, szName, iWidth):
        pass

    @monitor
    def setStyle(self, szName, szStyle):
        pass

    @monitor
    def setTabFocus(self, szName, szNext, szPrev):
        pass

    @monitor
    def setText(self, szName, szText):
        pass

    @monitor
    def setToolTip(self, szName, szHelpText):
        pass

    @monitor
    def setValue(self, szName, fValue):
        pass

    @monitor
    def toggle(self):
        pass


class CyGlobeLayer:

    @monitor
    def getButtonStyle(self):
        return str()

    @monitor
    def getCurrentOption(self):
        return int()

    @monitor
    def getName(self):
        return str()

    @monitor
    def getNumOptions(self):
        return int()

    @monitor
    def getOptionButton(self, iOptionID):
        return str()

    @monitor
    def getOptionName(self, iOptionID):
        return str()

    @monitor
    def isGlobeviewRequired(self):
        return bool()

    @monitor
    def isNone(self):
        return bool()

    @monitor
    def needsPlayerFilter(self):
        return bool()

    @monitor
    def registerGlobeLayer(self):
        pass

    @monitor
    def shouldCitiesZoom(self):
        return bool()


class CyGlobeLayerManager:

    @monitor
    def getCurrentLayer(self):
        return CyGlobeLayer()

    @monitor
    def getCurrentLayerID(self):
        return int()

    @monitor
    def getCurrentLayerName(self):
        return str()

    @monitor
    def getLayer(self, i):
        return CyGlobeLayer()

    @monitor
    def getLayerID(self, layer):
        return int()

    @monitor
    def getNumLayers(self):
        return int()

    @monitor
    def setCurrentLayer(self):
        pass


class CyInterface:

    @monitor
    def DoSoundtrack(self, szSoundtrackScript):
        return bool()

    @monitor
    def addCombatMessage(self, ePlayer, szString):
        pass

    @monitor
    def addImmediateMessage(self, szString, szSound):
        pass

    @monitor
    def addMessage(
        self, ePlayer, bForce, iLength, szString, szSound, eType, szIcon, eFlashColor,
        iFlashX, iFlashY, bShowOffScreenArrows, bShowOnScreenArrows
    ):
        pass

    @monitor
    def addQuestMessage(self, ePlayer, szString):
        pass

    @monitor
    def addSelectedCity(self, pNewValue):
        pass

    @monitor
    def cacheInterfacePlotUnits(self, pPlot):
        pass

    @monitor
    def canCreateGroup(self):
        return bool()

    @monitor
    def canDeleteGroup(self):
        return bool()

    @monitor
    def canHandleAction(self, iAction, bTestVisible):
        return bool()

    @monitor
    def canSelectHeadUnit(self):
        return bool()

    @monitor
    def checkFlashReset(self, ePlayer):
        pass

    @monitor
    def checkFlashUpdate(self):
        return bool()

    @monitor
    def clearSelectedCities(self):
        pass

    @monitor
    def clearSelectionList(self):
        pass

    @monitor
    def countEntities(self, iI):
        return int()

    @monitor
    def determineWidth(self, szBuffer):
        return int()

    @monitor
    def doPing(self, iX, iY, ePlayer):
        pass

    @monitor
    def endTimer(self, szOutputText):
        pass

    @monitor
    def exitingToMainMenu(self, szLoadFile):
        pass

    @monitor
    def getActionsToShow(self):
        return tuple()

    @monitor
    def getCachedInterfacePlotUnit(self, iIndex):
        return CyUnit()

    @monitor
    def getCityTabSelectionRow(self):
        return int()

    @monitor
    def getCursorPlot(self):
        return CyPlot()

    @monitor
    def getEndTurnState(self):
        return -1

    @monitor
    def getGotoPlot(self):
        return CyPlot()

    @monitor
    def getHeadSelectedCity(self):
        return CyCity()

    @monitor
    def getHeadSelectedUnit(self):
        return CyUnit()

    @monitor
    def getHelpString(self):
        return str()

    @monitor
    def getHighlightPlot(self):
        return CyPlot()

    @monitor
    def getInterfaceMode(self):
        return -1  # Type

    @monitor
    def getInterfacePlotUnit(self):
        return CyUnit()

    @monitor
    def getLengthSelectionList(self):
        return int()

    @monitor
    def getMouseOverPlot(self):
        return CyPlot()

    @monitor
    def getMousePos(self):
        return POINT()

    @monitor
    def getNumCachedInterfacePlotUnits(self):
        return int()

    @monitor
    def getNumOrdersQueued(self):
        return int()

    @monitor
    def getNumVisibleUnits(self):
        return int()

    @monitor
    def getOrderNodeData1(self, iNode):
        return int()

    @monitor
    def getOrderNodeData2(self, iNode):
        return int()

    @monitor
    def getOrderNodeSave(self, iNode):
        return bool()

    @monitor
    def getOrderNodeType(self, iNode):
        return -1  # Type

    @monitor
    def getPlotListColumn(self):
        return int()

    @monitor
    def getPlotListOffset(self):
        return int()

    @monitor
    def getSelectionPlot(self):
        return CyPlot()

    @monitor
    def getSelectionUnit(self):
        return CyUnit()

    @monitor
    def getShowInterface(self):
        return -1

    @monitor
    def insertIntoSelectionList(self, pUnit, bClear, bToggle, bGroup, bSound):
        pass

    @monitor
    def isCityScreenUp(self):
        return bool()

    @monitor
    def isCitySelected(self, pCity):
        return bool()

    @monitor
    def isCitySelection(self):
        return bool()

    @monitor
    def isDirty(self, eDirty):
        return bool()

    @monitor
    def isFlashing(self):
        return bool()

    @monitor
    def isFlashingPlayer(self, iPlayer):
        return bool()

    @monitor
    def isFocusedWidget(self):
        return bool()

    @monitor
    def isFocused(self):
        return bool()

    @monitor
    def isInAdvancedStart(self):
        return bool()

    @monitor
    def isInMainMenu(self):
        return bool()

    @monitor
    def isLeftMouseDown(self):
        return bool()

    @monitor
    def isNetStatsVisible(self):
        return bool()

    @monitor
    def isOOSVisible(self):
        return bool()

    @monitor
    def isOneCitySelected(self):
        return bool()

    @monitor
    def isRightMouseDown(self):
        return bool()

    @monitor
    def isScoresMinimized(self):
        return bool()

    @monitor
    def isScoresVisible(self):
        return bool()

    @monitor
    def isScreenUp(self, iEnumVal):
        return bool()

    @monitor
    def isUnitFocus(self):
        return bool()

    @monitor
    def isYieldVisibleMode(self):
        return bool()

    @monitor
    def lookAtCityBuilding(self, iCity, iBuilding):
        pass

    @monitor
    def lookAtCityOffset(self, iCity):
        pass

    @monitor
    def makeInterfaceDirty(self):
        pass

    @monitor
    def mirrorsSelectionGroup(self):
        return bool()

    @monitor
    def noTechSplash(self):
        return bool()

    @monitor
    def playAdvisorSound(self, pszSound):
        pass

    @monitor
    def playGeneralSound(self, pszSound):
        pass

    @monitor
    def playGeneralSoundAtPlot(self, iScriptID, pPlot):
        pass

    @monitor
    def playGeneralSoundByID(self, iScriptID):
        pass

    @monitor
    def removeFromSelectionList(self, pUnit):
        pass

    @monitor
    def selectAll(self, pPlot):
        pass

    @monitor
    def selectCity(self, pNewValue, bTestProduction):
        pass

    @monitor
    def selectGroup(self, pUnit, bShift, bCtrl, bAlt):
        pass

    @monitor
    def selectHotKeyUnit(self, iHotKeyNumber):
        return int()

    @monitor
    def selectUnit(self, pUnit, bClear, bToggle, bSound):
        pass

    @monitor
    def setBusy(self, bBusy):
        pass

    @monitor
    def setCityTabSelectionRow(self, eIndex):
        pass

    @monitor
    def setDirty(self, eDirty, bDirty):
        pass

    @monitor
    def setInterfaceMode(self, eMode):
        pass

    @monitor
    def setPausedPopups(self, bPausedPopups):
        pass

    @monitor
    def setShowInterface(self, eInterfaceVisibility):
        pass

    @monitor
    def setSoundSelectionReady(self, bReady):
        pass

    @monitor
    def setWorldBuilder(self, bTurnOn):
        pass

    @monitor
    def shiftKey(self):
        return bool()

    @monitor
    def shouldDisplayEndTurn(self):
        return bool()

    @monitor
    def shouldDisplayEndTurnButton(self):
        return bool()

    @monitor
    def shouldDisplayFlag(self):
        return bool()

    @monitor
    def shouldDisplayReturn(self):
        return bool()

    @monitor
    def shouldDisplayUnitModel(self):
        return bool()

    @monitor
    def shouldDisplayWaitingOthers(self):
        return bool()

    @monitor
    def shouldDisplayWaitingYou(self):
        return bool()

    @monitor
    def shouldFlash(self, iPlayer):
        return bool()

    @monitor
    def shouldShowAction(self, iAction):
        return bool()

    @monitor
    def shouldShowChangeResearchButton(self):
        return bool()

    @monitor
    def shouldShowResearchButtons(self):
        return bool()

    @monitor
    def shouldShowSelectionButtons(self):
        return bool()

    @monitor
    def shouldShowYieldVisibleButton(self):
        return bool()

    @monitor
    def startTimer(self):
        pass

    @monitor
    def stop2DSound(self):
        pass

    @monitor
    def stopAdvisorSound(self):
        pass

    @monitor
    def toggleBareMapMode(self):
        pass

    @monitor
    def toggleMusicOn(self):
        pass

    @monitor
    def toggleNetStatsVisible(self):
        pass

    @monitor
    def toggleScoresMinimized(self):
        pass

    @monitor
    def toggleScoresVisible(self):
        pass

    @monitor
    def toggleYieldVisibleMode(self):
        pass


class CyMessageControl:

    @monitor
    def GetConnState(self, iPlayer):
        return int()

    @monitor
    def GetFirstBadConnection(self):
        return int()

    @monitor
    def sendAdvancedStartAction(self, eAction, ePlayer, iX, iY, iData, bAdd):
        pass

    @monitor
    def sendApplyEvent(self, iContextID, eContextType, userData):
        return bool()

    @monitor
    def sendConvert(self, iReligion):
        pass

    @monitor
    def sendDoTask(self, iCity, eTask, iData1, iData2, bOption, bAlt, bShift, bCtrl):
        pass

    @monitor
    def sendEmpireSplit(self, ePlayer, iAreaId):
        pass

    @monitor
    def sendEspionageSpendingWeightChange(self, eTargetTeam, iChange):
        pass

    @monitor
    def sendModNetMessage(self, iData1, iData2, iData3, iData4, iData5):
        pass

    @monitor
    def sendPlayerOption(self, eOption, bValue):
        pass

    @monitor
    def sendPushOrder(self, iCityID, eOrder, iData, bAlt, bShift, bCtrl):
        pass

    @monitor
    def sendResearch(self, eTech, bShift):
        pass

    @monitor
    def sendTurnComplete(self):
        pass

    @monitor
    def sendUpdateCivics(self, iCivics):
        pass

    if PB_MOD:
        @monitor
        def sendTurnCompleteAll(self):
            pass


class CyPopup:

    @monitor
    def addButton(self, szText):
        pass

    @monitor
    def addButtonXY(self, szText, iX, iY):
        pass

    @monitor
    def addDDS(self, szPathName, iX, iY, iWidth, iHeight):
        pass

    @monitor
    def addFixedSeparator(self, iSpace):
        pass

    @monitor
    def addLeaderhead(self, szPathName, eWho, eInitAttitude, iX, iY):
        pass

    @monitor
    def addListBoxString(self, szText, iID, iGroup):
        pass

    @monitor
    def addPullDownString(self, szText, iID, iGroup):
        pass

    @monitor
    def addPythonButton(
        self, szFunctionName, szBtnText, szHelpText, szArtFile, iData1, iData2, bOption
    ):
        pass

    @monitor
    def addPythonButtonXY(
        self, szFunctionName, szBtnText, szHelpText, szArtFile,
        iData1, iData2, bOption, iX, iY
    ):
        pass

    @monitor
    def addPythonDDS(self, szPathName, szText, iX, iY, iWidth, iHeight):
        pass

    @monitor
    def addSeparator(self):
        pass

    @monitor
    def addTableCellDDS(self, iRow, iCol, szFile, iX, iY, iWidth, iHeight, iGroup):
        pass

    @monitor
    def addTableCellImage(self, iRow, iCol, szFile, iGroup):
        pass

    @monitor
    def addTableCellText(self, iRow, iCol, szText, iGroup):
        pass

    @monitor
    def completeTableAndAttach(self, iGroup):
        pass

    @monitor
    def completeTableAndAttachXY(self, iGroup, iX, iY):
        pass

    @monitor
    def createCheckBoxes(self, iNumBoxes, iGroup):
        pass

    @monitor
    def createEditBox(self, szText, iGroup):
        pass

    @monitor
    def createEditBoxXY(self, szText, iGroup, iX, iY):
        pass

    @monitor
    def createListBox(self, iGroup):
        pass

    @monitor
    def createListBoxXY(self, iGroup, iX, iY):
        pass

    @monitor
    def createPullDown(self, iGroup):
        pass

    @monitor
    def createPullDownXY(self, iGroup, iX, iY):
        pass

    @monitor
    def createPythonCheckBoxes(self, iNumBoxes, iGroup):
        pass

    @monitor
    def createPythonEditBox(self, szText, szHelpText, iGroup):
        pass

    @monitor
    def createPythonEditBoxXY(self, szText, szHelpText, iGroup, iX, iY):
        pass

    @monitor
    def createPythonListBox(self, szText, iGroup):
        pass

    @monitor
    def createPythonListBoxXY(self, szText, iGroup, iX, iY):
        pass

    @monitor
    def createPythonPullDown(self, szText, iGroup):
        pass

    @monitor
    def createPythonPullDownXY(self, szText, iGroup, iX, iY):
        pass

    @monitor
    def createPythonRadioButtons(self, iNumButtons, iGroup):
        pass

    @monitor
    def createRadioButtons(self, iNumButtons, iGroup):
        pass

    @monitor
    def createSpinBox(self, iIndex, szHelpText, iDefault, iIncrement, iMax, iMin):
        pass

    @monitor
    def createTable(self, iRows, iCols, iGroup):
        pass

    @monitor
    def isNone(self):
        return bool()

    @monitor
    def launch(self, bCreateOK, eState):
        return bool()

    @monitor
    def setBodyString(self, szText, uiFlags):
        pass

    @monitor
    def setCheckBoxText(self, iCheckBoxID, szText, iGroup):
        pass

    @monitor
    def setEditBoxMaxCharCount(self, maxCharCount, preferredCharCount, iGroup):
        pass

    @monitor
    def setHeaderString(self, szText, uiFlags):
        pass

    @monitor
    def setPosition(self, iX, iY):
        pass

    @monitor
    def setPythonBodyString(self, szDefText, szName, szText, uiFlags):
        pass

    @monitor
    def setPythonCheckBoxText(self, iCheckBoxID, szText, szHelpText, iGroup):
        pass

    @monitor
    def setPythonRadioButtonText(self, iRadioButtonID, szText, szHelpText, iGroup):
        pass

    @monitor
    def setRadioButtonText(self, iRadioButtonID, szText, iGroup):
        pass

    @monitor
    def setSelectedListBoxString(self, iID, iGroup):
        pass

    @monitor
    def setSelectedPulldownID(self, iID, iGroup):
        pass

    @monitor
    def setSize(self, iXS, iYS):
        pass

    @monitor
    def setTableCellSize(self, iCol, iPixels, iGroup):
        pass

    @monitor
    def setTableYSize(self, iRow, iSize, iGroup):
        pass

    @monitor
    def setTimer(self, uiTime):
        pass

    @monitor
    def setUserData(self, userData):
        pass


class CyPopupInfo:

    @monitor
    def addPopup(self, iPlayer):
        pass

    @monitor
    def addPythonButton(self, szText, szArt):
        pass

    @monitor
    def getButtonPopupType(self):
        return -1  # Type

    @monitor
    def getData1(self):
        return int()

    @monitor
    def getData2(self):
        return int()

    @monitor
    def getData3(self):
        return int()

    @monitor
    def getFlags(self):
        return int()

    @monitor
    def getNumPythonButtons(self):
        return int()

    @monitor
    def getOnClickedPythonCallback(self):
        return str()

    @monitor
    def getOnFocusPythonCallback(self):
        return str()

    @monitor
    def getOption1(self):
        return bool()

    @monitor
    def getOption2(self):
        return bool()

    @monitor
    def getPythonButtonArt(self):
        return str()

    @monitor
    def getPythonButtonText(self):
        return str()

    @monitor
    def getPythonModule(self):
        return str()

    @monitor
    def getText(self):
        return str()

    @monitor
    def isNone(self):
        return bool()

    @monitor
    def setButtonPopupType(self, eValue):
        pass

    @monitor
    def setData1(self, iValue):
        pass

    @monitor
    def setData2(self, iValue):
        pass

    @monitor
    def setData3(self, iValue):
        pass

    @monitor
    def setFlags(self, iValue):
        pass

    @monitor
    def setOnClickedPythonCallback(self, szOnFocus):
        pass

    @monitor
    def setOnFocusPythonCallback(self, szOnFocus):
        pass

    @monitor
    def setOption1(self, bValue):
        pass

    @monitor
    def setOption2(self, bValue):
        pass

    @monitor
    def setPythonModule(self, szOnFocus):
        pass

    @monitor
    def setText(self, szText):
        pass


class CyPopupReturn:

    @monitor
    def getButtonClicked(self, iGroup):
        return int()

    @monitor
    def getEditBoxString(self, iGroup):
        return str()

    @monitor
    def getSelectedListBoxValue(self, iGroup):
        return int()

    @monitor
    def getSelectedPullDownValue(self, iGroup):
        return int()

    @monitor
    def getSelectedRadioButton(self, iGroup):
        return int()

    @monitor
    def getSpinnerWidgetValue(self, iGroup):
        return int()

    @monitor
    def isNone(self):
        return bool()


class CyPythonMgr:

    @monitor
    def allowDefaultImpl(self):
        pass

    @monitor
    def debugMsg(self, msg):
        pass

    @monitor
    def debugMsgWide(self, msg):
        pass

    @monitor
    def errorMsg(self, msg):
        pass

    @monitor
    def errorMsgWide(self, msg):
        pass


class CySign:

    @monitor
    def getCaption(self):
        return str()

    @monitor
    def getPlayerType(self):
        return -1  # Type

    @monitor
    def getPlot(self):
        return CyPlot()


class CyStatistics:

    @monitor
    def getPlayerNumBuildingsBuilt(self, iPlayerID, iBuildingID):
        return int()

    @monitor
    def getPlayerNumCitiesBuilt(self, iPlayerID):
        return int()

    @monitor
    def getPlayerNumCitiesRazed(self, iPlayerID):
        return int()

    @monitor
    def getPlayerNumGoldenAges(self, iPlayerID):
        return int()

    @monitor
    def getPlayerNumUnitsBuilt(self, iPlayerID, iUnitID):
        return int()

    @monitor
    def getPlayerNumUnitsKilled(self, iPlayerID, iUnitID):
        return int()

    @monitor
    def getPlayerNumUnitsLost(self, iPlayerID, iUnitID):
        return int()

    @monitor
    def getPlayerReligionFounded(self, iPlayerID, iReligionID):
        return bool()

    @monitor
    def getPlayerTimePlayed(self, iPlayerID):
        return int()


class CyTranslator:

    @monitor
    def changeTextColor(self, szText, iColor):
        return str()

    @monitor
    def getColorText(self, szTag, args, iColor):
        return str()

    @monitor
    def getObjectText(self, szTag, i):
        return str()

    @monitor
    def getText(self, szTag, args):
        return str()

    @monitor
    def stripHTML(self, szText):
        return str()


class CyUnitEntity:

    @monitor
    def GetSubEntity(self, i):
        return CyUnitSubEntity()

    @monitor
    def GetSubEntityCount(self):
        return int()

    @monitor
    def GetUnitsCurrentlyAlive(self):
        return int()

    @monitor
    def MoveTo(self, x, y, z, rad):
        pass

    @monitor
    def NotifyEntity(self, e):
        pass

    @monitor
    def getScale(self):
        return float()

    @monitor
    def getUnit(self):
        return CyUnit()

    @monitor
    def isNone(self):
        return bool()

    @monitor
    def setScale(self, fScale):
        pass


class CyUnitSubEntity:

    @monitor
    def PlayAnimationPath(self, i):
        pass

    @monitor
    def isNone(self):
        return bool()

    @monitor
    def setUnitShadow(self, b):
        pass

    @monitor
    def setVisible(self, b):
        pass


class CyUserProfile:

    @monitor
    def deleteProfileFile(self, szNewName):
        return bool()

    @monitor
    def getAmbienceVolume(self):
        return int()

    @monitor
    def getAntiAliasing(self):
        return int()

    @monitor
    def getAntiAliasingMaxMultiSamples(self):
        return int()

    @monitor
    def getCaptureDeviceDesc(self, iIndex):
        return str()

    @monitor
    def getCaptureDeviceIndex(self):
        return int()

    @monitor
    def getCaptureVolume(self):
        return int()

    @monitor
    def getCurrentVersion(self):
        return int()

    @monitor
    def getGlobeLayer(self):
        return int()

    @monitor
    def getGlobeViewRenderLevel(self):
        return int()

    @monitor
    def getGraphicOption(self, i):
        return bool()

    @monitor
    def getGraphicsLevel(self):
        return int()

    @monitor
    def getGrid(self):
        return bool()

    @monitor
    def getInterfaceVolume(self):
        return int()

    @monitor
    def getMainMenu(self):
        return int()

    @monitor
    def getMap(self):
        return bool()

    @monitor
    def getMasterVolume(self):
        return int()

    @monitor
    def getMaxCaptureVolume(self):
        return int()

    @monitor
    def getMaxPlaybackVolume(self):
        return int()

    @monitor
    def getMovieQualityLevel(self):
        return int()

    @monitor
    def getMusicPath(self):
        return str()

    @monitor
    def getMusicVolume(self):
        return int()

    @monitor
    def getNumCaptureDevices(self):
        return int()

    @monitor
    def getNumPlaybackDevices(self):
        return int()

    @monitor
    def getNumProfileFiles(self):
        return int()

    @monitor
    def getPlaybackDeviceDesc(self, iIndex):
        return str()

    @monitor
    def getPlaybackDeviceIndex(self):
        return int()

    @monitor
    def getPlaybackVolume(self):
        return int()

    @monitor
    def getPlayerOption(self, i):
        return bool()

    @monitor
    def getProfileFileName(self, iFileID):
        return str()

    @monitor
    def getProfileName(self):
        return str()

    @monitor
    def getProfileVersion(self):
        return int()

    @monitor
    def getRenderQualityLevel(self):
        return int()

    @monitor
    def getResolution(self):
        return int()

    @monitor
    def getResolutionMaxModes(self):
        return int()

    @monitor
    def getResolutionString(self, iResolution):
        return str()

    @monitor
    def getScores(self):
        return bool()

    @monitor
    def getSoundEffectsVolume(self):
        return int()

    @monitor
    def getSpeakerConfig(self):
        return str()

    @monitor
    def getSpeakerConfigFromList(self, iIndex):
        return str()

    @monitor
    def getSpeechVolume(self):
        return int()

    @monitor
    def getVolumeStops(self):
        return int()

    @monitor
    def getYields(self):
        return bool()

    @monitor
    def is24Hours(self):
        return bool()

    @monitor
    def isAmbienceNoSound(self):
        return bool()

    @monitor
    def isClockOn(self):
        return bool()

    @monitor
    def isInterfaceNoSound(self):
        return bool()

    @monitor
    def isMasterNoSound(self):
        return bool()

    @monitor
    def isMusicNoSound(self):
        return bool()

    @monitor
    def isProfileFileExist(self, szNewName):
        return bool()

    @monitor
    def isSoundEffectsNoSound(self):
        return bool()

    @monitor
    def isSpeechNoSound(self):
        return bool()

    @monitor
    def loadProfileFileNames(self):
        pass

    @monitor
    def musicPathDialogBox(self):
        pass

    @monitor
    def readFromFile(self, szFileName):
        return bool()

    @monitor
    def recalculateAudioSettings(self):
        pass

    @monitor
    def resetOptions(self, resetTab):
        pass

    @monitor
    def set24Hours(self, bValue):
        pass

    @monitor
    def setAmbienceNoSound(self, b):
        pass

    @monitor
    def setAmbienceVolume(self, i):
        pass

    @monitor
    def setAntiAliasing(self, i):
        pass

    @monitor
    def setCaptureDevice(self, device):
        pass

    @monitor
    def setCaptureVolume(self, volume):
        pass

    @monitor
    def setClockJustTurnedOn(self, bValue):
        pass

    @monitor
    def setClockOn(self, bValue):
        pass

    @monitor
    def setGlobeViewRenderLevel(self, level):
        pass

    @monitor
    def setGraphicOption(self, i, b):
        pass

    @monitor
    def setGraphicsLevel(self, i):
        pass

    @monitor
    def setInterfaceNoSound(self, b):
        pass

    @monitor
    def setInterfaceVolume(self, i):
        pass

    @monitor
    def setMainMenu(self, i):
        pass

    @monitor
    def setMasterNoSound(self, b):
        pass

    @monitor
    def setMasterVolume(self, i):
        pass

    @monitor
    def setMovieQualityLevel(self, level):
        pass

    @monitor
    def setMusicNoSound(self, b):
        pass

    @monitor
    def setMusicPath(self, szMusicPath):
        pass

    @monitor
    def setMusicVolume(self, i):
        pass

    @monitor
    def setPlaybackDevice(self, device):
        pass

    @monitor
    def setPlaybackVolume(self, volume):
        pass

    @monitor
    def setProfileName(self, szNewName):
        pass

    @monitor
    def setRenderQualityLevel(self, level):
        pass

    @monitor
    def setResolution(self, i):
        pass

    @monitor
    def setSoundEffectsNoSound(self, b):
        pass

    @monitor
    def setSoundEffectsVolume(self, i):
        pass

    @monitor
    def setSpeakerConfig(self, szConfigName):
        pass

    @monitor
    def setSpeechNoSound(self, b):
        pass

    @monitor
    def setSpeechVolume(self, i):
        pass

    @monitor
    def setUseVoice(self, b):
        pass

    @monitor
    def useVoice(self):
        return bool()

    @monitor
    def wasClockJustTurnedOn(self):
        return bool()

    @monitor
    def writeToFile(self, szFileName):
        pass


class CyVariableSystem:

    @monitor
    def getFirstVariableName(self):
        return str()

    @monitor
    def getNextVariableName(self):
        return str()

    @monitor
    def getValueFloat(self, szVarName):
        return float()

    @monitor
    def getValueInt(self, szVarName):
        return int()

    @monitor
    def getValueString(self, szVarName):
        return str()

    @monitor
    def getVariableType(self, szVariable):
        return str()

    @monitor
    def isNone(self):
        return bool()

    @monitor
    def setValueFloat(self, szVarName, fValue):
        pass

    @monitor
    def setValueInt(self, szVarName, iValue):
        pass

    @monitor
    def setValueString(self, szVarName, szValue):
        pass


class NiMatrix3:

    @monitor
    def GetEntry(self, uiRow, uiCol):
        return float()

    @monitor
    def MakeIdentity(self):
        pass

    @monitor
    def SetEntry(self, uiRow, uiCol, fEntry):
        pass


class Response:
    pass


class WidgetAnim:
    pass
