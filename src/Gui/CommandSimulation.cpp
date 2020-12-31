#include "PreCompiled.h"

#ifndef _PreComp_
# include <sstream>
# include <Inventor/actions/SoGetBoundingBoxAction.h>
# include <Inventor/events/SoMouseButtonEvent.h>
# include <Inventor/nodes/SoOrthographicCamera.h>
# include <Inventor/nodes/SoPerspectiveCamera.h>
# include <QFile>
# include <QFileInfo>
# include <QFont>
# include <QFontMetrics>
# include <QMessageBox>
# include <QPainter>
# include <QTextStream>
# include <boost/bind.hpp>
#endif

#include "Command.h"
#include "Action.h"
#include "Application.h"
#include "BitmapFactory.h"
#include "Control.h"
#include "Clipping.h"
#include "FileDialog.h"
#include "MainWindow.h"
#include "Tree.h"
#include "View.h"
#include "Document.h"
#include "Macro.h"
#include "DlgDisplayPropertiesImp.h"
#include "DlgSettingsImageImp.h"
#include "Selection.h"
#include "SoFCOffscreenRenderer.h"
#include "SoFCBoundingBox.h"
#include "SoFCUnifiedSelection.h"
#include "SoAxisCrossKit.h"
#include "View3DInventor.h"
#include "View3DInventorViewer.h"
#include "ViewParams.h"
#include "WaitCursor.h"
#include "ViewProviderMeasureDistance.h"
#include "ViewProviderGeometryObject.h"
#include "SceneInspector.h"
#include "DemoMode.h"
#include "TextureMapping.h"
#include "Utilities.h"
#include "NavigationStyle.h"

#include <Base/Console.h>
#include <Base/Tools2D.h>
#include <Base/Exception.h>
#include <Base/FileInfo.h>
#include <Base/Reader.h>
#include <Base/Parameter.h>
#include <App/Application.h>
#include <App/Document.h>
#include <App/GeoFeature.h>
#include <App/DocumentObjectGroup.h>
#include <App/MeasureDistance.h>
#include <App/DocumentObject.h>
#include <App/ComplexGeoDataPy.h>
#include <App/GeoFeatureGroupExtension.h>

#include <QDomDocument>
#include <QDomElement>

using namespace Gui;
using Gui::Dialog::DlgSettingsImageImp;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//===========================================================================
// Std_DefineMaterials
//===========================================================================
DEF_STD_CMD_A(StdCmdDefineMaterials)

StdCmdDefineMaterials::StdCmdDefineMaterials()
    : Command("Std_DefineMaterials")
{
    sGroup = QT_TR_NOOP("Standard-View");
    sMenuText = QT_TR_NOOP("Define Materials");
    sToolTipText = QT_TR_NOOP("Grayed out Define Materials");
    sWhatsThis = "Std_DefineMaterials";
    sStatusTip = QT_TR_NOOP("Grayed out Define Materials");
    eType = Alter3DView;
}

void StdCmdDefineMaterials::activated(int iMsg)
{
    Q_UNUSED(iMsg);
}

bool StdCmdDefineMaterials::isActive(void)
{
    return false;
}

//===========================================================================
// Std_DefineSection
//===========================================================================
DEF_STD_CMD_A(StdCmdDefineSection)

StdCmdDefineSection::StdCmdDefineSection()
    : Command("Std_DefineSection")
{
    sGroup = QT_TR_NOOP("Standard-View");
    sMenuText = QT_TR_NOOP("Define Section");
    sToolTipText = QT_TR_NOOP("Grayed out Define Section");
    sWhatsThis = "Std_DefineSection";
    sStatusTip = QT_TR_NOOP("Grayed out Define Section");
    eType = Alter3DView;
}

void StdCmdDefineSection::activated(int iMsg)
{
    Q_UNUSED(iMsg);
}

bool StdCmdDefineSection::isActive(void)
{
    return false;
}

//===========================================================================
// Std_DefineBoundCon
//===========================================================================
DEF_STD_CMD_A(StdCmdDefineBoundCon)

StdCmdDefineBoundCon::StdCmdDefineBoundCon()
    : Command("Std_DefineBoundCon")
{
    sGroup = QT_TR_NOOP("Standard-View");
    sMenuText = QT_TR_NOOP("Define Boundary Conditions");
    sToolTipText = QT_TR_NOOP("Grayed out Define Boundary Conditions");
    sWhatsThis = "Std_DefineBoundCon";
    sStatusTip = QT_TR_NOOP("Grayed out Define Boundary Condittions");
    eType = Alter3DView;
}

void StdCmdDefineBoundCon::activated(int iMsg)
{
    Q_UNUSED(iMsg);
}

bool StdCmdDefineBoundCon::isActive(void)
{
    return false;
}

//===========================================================================
// Std_DefineLoads
//===========================================================================
DEF_STD_CMD_A(StdCmdDefineLoads)

StdCmdDefineLoads::StdCmdDefineLoads()
    : Command("Std_DefineLoads")
{
    sGroup = QT_TR_NOOP("Standard-View");
    sMenuText = QT_TR_NOOP("Define Loads");
    sToolTipText = QT_TR_NOOP("Grayed out Define Loads");
    sWhatsThis = "Std_DefineLoads";
    sStatusTip = QT_TR_NOOP("Grayed out Define Loads");
    eType = Alter3DView;
}

void StdCmdDefineLoads::activated(int iMsg)
{
    Q_UNUSED(iMsg);
}

bool StdCmdDefineLoads::isActive(void)
{
    return false;
}

//===========================================================================
// Std_RunAnalysis
//===========================================================================
DEF_STD_CMD_A(StdCmdRunAnalysis)

StdCmdRunAnalysis::StdCmdRunAnalysis()
    : Command("Std_RunAnalysis")
{
    sGroup = QT_TR_NOOP("Standard-View");
    sMenuText = QT_TR_NOOP("Run Analysis");
    sToolTipText = QT_TR_NOOP("Grayed out Run Analysis");
    sWhatsThis = "Std_RunAnalysis";
    sStatusTip = QT_TR_NOOP("Grayed out Run Analysis");
    eType = Alter3DView;
}

void StdCmdRunAnalysis::activated(int iMsg)
{
    Q_UNUSED(iMsg);
}

bool StdCmdRunAnalysis::isActive(void)
{
    return false;
}

//===========================================================================
// Std_SectionProp
//===========================================================================
DEF_STD_CMD_A(StdCmdSectionProp)

StdCmdSectionProp::StdCmdSectionProp()
    : Command("Std_SectionProp")
{
    sGroup = QT_TR_NOOP("Standard-View");
    sMenuText = QT_TR_NOOP("Setion Properties");
    sToolTipText = QT_TR_NOOP("Grayed out Section Properties");
    sWhatsThis = "Std_SectionProp";
    sStatusTip = QT_TR_NOOP("Grayed out Section Properties");
    eType = Alter3DView;
}

void StdCmdSectionProp::activated(int iMsg)
{
    Q_UNUSED(iMsg);
}

bool StdCmdSectionProp::isActive(void)
{
    return false;
}

//===========================================================================
// Std_PlotResults
//===========================================================================
DEF_STD_CMD_A(StdCmdPlotResults)

StdCmdPlotResults::StdCmdPlotResults()
    : Command("Std_PlotResults")
{
    sGroup = QT_TR_NOOP("Standard-View");
    sMenuText = QT_TR_NOOP("Plot Results");
    sToolTipText = QT_TR_NOOP("Grayed out Plot Results");
    sWhatsThis = "Std_PlotResults";
    sStatusTip = QT_TR_NOOP("Grayed out Plot Results");
    eType = Alter3DView;
}

void StdCmdPlotResults::activated(int iMsg)
{
    Q_UNUSED(iMsg);
}

bool StdCmdPlotResults::isActive(void)
{
    return false;
}

//===========================================================================
// Std_ListResults
//===========================================================================
DEF_STD_CMD_A(StdCmdListResults)

StdCmdListResults::StdCmdListResults()
    : Command("Std_ListResults")
{
    sGroup = QT_TR_NOOP("Standard-View");
    sMenuText = QT_TR_NOOP("List Results");
    sToolTipText = QT_TR_NOOP("Grayed out List Results");
    sWhatsThis = "Std_ListResults";
    sStatusTip = QT_TR_NOOP("Grayed out List Results");
    eType = Alter3DView;
}

void StdCmdListResults::activated(int iMsg)
{
    Q_UNUSED(iMsg);
}

bool StdCmdListResults::isActive(void)
{
    return false;
}

//===========================================================================
// Std_ResultTools
//===========================================================================
DEF_STD_CMD_A(StdCmdResultTools)

StdCmdResultTools::StdCmdResultTools()
    : Command("Std_ResultTools")
{
    sGroup = QT_TR_NOOP("Standard-View");
    sMenuText = QT_TR_NOOP("Result Tools");
    sToolTipText = QT_TR_NOOP("Grayed out Result Tools");
    sWhatsThis = "Std_ResultTools";
    sStatusTip = QT_TR_NOOP("Grayed out Result Tools");
    eType = Alter3DView;
}

void StdCmdResultTools::activated(int iMsg)
{
    Q_UNUSED(iMsg);
}

bool StdCmdResultTools::isActive(void)
{
    return false;
}

//===========================================================================
// Instantiation
//===========================================================================

namespace Gui 
{
    void CreateSimulationCommands(void)
    {
        CommandManager& rcCmdMgr = Application::Instance->commandManager();

        // simulation
        rcCmdMgr.addCommand(new StdCmdDefineMaterials());
        rcCmdMgr.addCommand(new StdCmdDefineSection());
        rcCmdMgr.addCommand(new StdCmdDefineBoundCon());
        rcCmdMgr.addCommand(new StdCmdDefineLoads());
        rcCmdMgr.addCommand(new StdCmdRunAnalysis());
        rcCmdMgr.addCommand(new StdCmdSectionProp());
        rcCmdMgr.addCommand(new StdCmdPlotResults());
        rcCmdMgr.addCommand(new StdCmdListResults());
        rcCmdMgr.addCommand(new StdCmdResultTools());

        //auto hGrp = App::GetApplication().GetParameterGroupByPath("User parameter:BaseApp/Preferences/View");
        //if (hGrp->GetASCII("GestureRollFwdCommand").empty())
        //    hGrp->SetASCII("GestureRollFwdCommand", "Std_SelForward");
        //if (hGrp->GetASCII("GestureRollBackCommand").empty())
        //    hGrp->SetASCII("GestureRollBackCommand", "Std_SelBack");
    }
} // namespace Gui
