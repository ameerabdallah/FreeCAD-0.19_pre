#include "PreCompiled.h"
#ifndef _PreComp_
# include <cfloat>
# include "InventorAll.h"
# include <QAction>
# include <QActionGroup>
# include <QApplication>
# include <QByteArray>
# include <QCursor>
# include <QList>
# include <QMenu>
# include <QMetaObject>
# include <QRegExp>
#endif

#include <Inventor/sensors/SoTimerSensor.h>

#include <App/Application.h>
#include "NavigationStyle.h"
#include "View3DInventorViewer.h"
#include "Application.h"
#include "MenuManager.h"
#include "MouseSelection.h"

using namespace Gui;

// ----------------------------------------------------------------------------------

/* TRANSLATOR Gui::PanNavigationStyle */

TYPESYSTEM_SOURCE(Gui::PanNavigationStyle, Gui::UserNavigationStyle)

PanNavigationStyle::PanNavigationStyle() : lockButton1(false)
{
}

PanNavigationStyle::~PanNavigationStyle()
{
}

const char* PanNavigationStyle::mouseButtons(ViewerMode mode)
{
    switch (mode) {
    case NavigationStyle::SELECTION:
        return QT_TR_NOOP("Press left mouse button");
    case NavigationStyle::PANNING:
        return QT_TR_NOOP("Press SHIFT and middle mouse button");
    case NavigationStyle::DRAGGING:
        return QT_TR_NOOP("Press middle mouse button");
    case NavigationStyle::ZOOMING:
        return QT_TR_NOOP("Scroll middle mouse button");
    default:
        return "No description";
    }
}

SbBool PanNavigationStyle::processSoEvent(const SoEvent* const ev)
{
    // Events when in "ready-to-seek" mode are ignored, except those
    // which influence the seek mode itself -- these are handled further
    // up the inheritance hierarchy.
    if (this->isSeekMode()) { return inherited::processSoEvent(ev); }
    // Switch off viewing mode (Bug #0000911)
    if (!this->isSeekMode() && !this->isAnimating() && this->isViewing())
        this->setViewing(false); // by default disable viewing mode to render the scene

    const SoType type(ev->getTypeId());

    const SbViewportRegion& vp = viewer->getSoRenderManager()->getViewportRegion();
    const SbVec2s size(vp.getViewportSizePixels());
    const SbVec2f prevnormalized = this->lastmouseposition;
    const SbVec2s pos(ev->getPosition());
    const SbVec2f posn((float)pos[0] / (float)std::max((int)(size[0] - 1), 1),
        (float)pos[1] / (float)std::max((int)(size[1] - 1), 1));

    this->lastmouseposition = posn;

    // Set to true if any event processing happened. Note that it is not
    // necessary to restrict ourselves to only do one "action" for an
    // event, we only need this flag to see if any processing happened
    // at all.
    SbBool processed = false;

    const ViewerMode curmode = this->currentmode;
    ViewerMode newmode = curmode;

    // Mouse Button / Spaceball Button handling
    if (type.isDerivedFrom(SoMouseButtonEvent::getClassTypeId())) {
        const SoMouseButtonEvent* const event = (const SoMouseButtonEvent*)ev;
        const int button = event->getButton();
        const SbBool press = event->getState() == SoButtonEvent::DOWN ? true : false;

        //SoDebugError::postInfo("processSoEvent", "button = %d", button);
        switch (button) {
        case SoMouseButtonEvent::BUTTON1:
            this->lockrecenter = true;
            this->button1down = press;
            if (press && (this->currentmode == NavigationStyle::SEEK_WAIT_MODE)) {
                newmode = NavigationStyle::PANNING;
                this->seekToPoint(pos); // implicitly calls interactiveCountInc()
                processed = true;
            }

            else if (press && (this->currentmode == NavigationStyle::PANNING ||
                this->currentmode == NavigationStyle::ZOOMING)) {
                newmode = NavigationStyle::PANNING;
                saveCursorPosition(ev);
                this->centerTime = ev->getTime();
                processed = true;
            }
            else if (viewer->isEditing() && (this->currentmode == NavigationStyle::SPINNING)) {
                processed = true;
            }
            // issue #0002433: avoid to swallow the UP event if down the
            // scene graph somewhere a dialog gets opened
            else if (press) {
                SbTime tmp = (ev->getTime() - mouseDownConsumedEvent.getTime());
                float dci = (float)QApplication::doubleClickInterval() / 1000.0f;
                // a double-click?
                if (tmp.getValue() < dci) {
                    mouseDownConsumedEvent = *event;
                    mouseDownConsumedEvent.setTime(ev->getTime());
                    processed = true;
                }
                else {
                    mouseDownConsumedEvent.setTime(ev->getTime());
                    // 'ANY' is used to mark that we don't know yet if it will
                    // be a double-click event.
                    mouseDownConsumedEvent.setButton(SoMouseButtonEvent::ANY);
                }
            }
            else if (!press) {
                if (mouseDownConsumedEvent.getButton() == SoMouseButtonEvent::BUTTON1) {
                    // now handle the postponed event
                    inherited::processSoEvent(&mouseDownConsumedEvent);
                    mouseDownConsumedEvent.setButton(SoMouseButtonEvent::ANY);
                }
            }
            break;
        case SoMouseButtonEvent::BUTTON2:
            // If we are in edit mode then simply ignore the RMB events
            // to pass the event to the base class.
            this->lockrecenter = true;
            if (!viewer->isEditing()) {
                // If we are in zoom or pan mode ignore RMB events otherwise
                // the canvas doesn't get any release events 
                if (this->currentmode != NavigationStyle::ZOOMING &&
                    this->currentmode != NavigationStyle::PANNING &&
                    this->currentmode != NavigationStyle::DRAGGING) {
                    if (this->isPopupMenuEnabled()) {
                        if (!press) { // release right mouse button
                            //this->openPopupMenu(event->getPosition());
                        }
                    }
                }
            }
            // Alternative way of rotating & zooming
            if (press && (this->currentmode == NavigationStyle::PANNING ||
                this->currentmode == NavigationStyle::ZOOMING)) {
                newmode = NavigationStyle::DRAGGING;
                saveCursorPosition(ev);
                this->centerTime = ev->getTime();
                processed = true;
            }
            this->button2down = press;
            break;
        case SoMouseButtonEvent::BUTTON3:
            if (press) {
                this->centerTime = ev->getTime();
                float ratio = vp.getViewportAspectRatio();
                SbViewVolume vv = viewer->getSoRenderManager()->getCamera()->getViewVolume(ratio);
                this->panningplane = vv.getPlane(viewer->getSoRenderManager()->getCamera()->focalDistance.getValue());
                this->lockrecenter = false;
            }
            else {
                SbTime tmp = (ev->getTime() - this->centerTime);
                float dci = (float)QApplication::doubleClickInterval() / 1000.0f;
                // is it just a middle click?
                if (tmp.getValue() < dci && !this->lockrecenter) {
                    if (!this->lookAtPoint(pos)) {
                        panToCenter(panningplane, posn);
                        this->interactiveCountDec();
                    }
                    processed = true;
                }
            }
            this->button3down = press;
            break;
        case SoMouseButtonEvent::BUTTON4:
            doZoom(viewer->getSoRenderManager()->getCamera(), true, posn);
            processed = true;
            break;
        case SoMouseButtonEvent::BUTTON5:
            doZoom(viewer->getSoRenderManager()->getCamera(), false, posn);
            processed = true;
            break;
        default:
            break;
        }
    }

    // Mouse Movement handling
    if (type.isDerivedFrom(SoLocation2Event::getClassTypeId())) {
        this->lockrecenter = true;
        const SoLocation2Event* const event = (const SoLocation2Event*)ev;
        if (this->currentmode == NavigationStyle::ZOOMING) {
            this->zoomByCursor(posn, prevnormalized);
            newmode = NavigationStyle::SELECTION;
            processed = true;
        }
        else if (this->currentmode == NavigationStyle::PANNING) {
            float ratio = vp.getViewportAspectRatio();
            panCamera(viewer->getSoRenderManager()->getCamera(), ratio, this->panningplane, posn, prevnormalized);
            newmode = NavigationStyle::SELECTION;
            processed = true;
        }
        else if (this->currentmode == NavigationStyle::DRAGGING) {
            this->addToLog(event->getPosition(), event->getTime());
            this->spin(posn);
            moveCursorPosition();
            processed = true;
        }
    }

    // Spaceball & Joystick handling
    if (type.isDerivedFrom(SoMotion3Event::getClassTypeId())) {
        const SoMotion3Event* const event = static_cast<const SoMotion3Event*>(ev);
        if (event)
            this->processMotionEvent(event);
        processed = true;
    }

    enum {
        BUTTON1DOWN = 1 << 0,
        BUTTON3DOWN = 1 << 1,
        CTRLDOWN = 1 << 2,
        SHIFTDOWN = 1 << 3,
        BUTTON2DOWN = 1 << 4
    };
    unsigned int combo =
        (this->button1down ? BUTTON1DOWN : 0) |
        (this->button2down ? BUTTON2DOWN : 0) |
        (this->button3down ? BUTTON3DOWN : 0) |
        (this->ctrldown ? CTRLDOWN : 0) |
        (this->shiftdown ? SHIFTDOWN : 0);

    switch (combo) {
    case 0:
        if (curmode == NavigationStyle::SPINNING) { break; }
        newmode = NavigationStyle::IDLE;
        // The left mouse button has been released right now but
        // we want to avoid that the event is processed elsewhere
        if (this->lockButton1) {
            this->lockButton1 = false;
            processed = true;
        }

        break;
    case BUTTON1DOWN:
    case BUTTON2DOWN:
    case BUTTON3DOWN:
    case BUTTON1DOWN | BUTTON2DOWN:
    case BUTTON1DOWN | BUTTON3DOWN:
    case BUTTON2DOWN | BUTTON3DOWN:
        newmode = NavigationStyle::PANNING;
        break;
    default:
        break;
    }

    if (newmode != curmode) {
        this->setViewingMode(newmode);
    }

    // If not handled in this class, pass on upwards in the inheritance
    // hierarchy.
    if (/*(curmode == NavigationStyle::SELECTION || viewer->isEditing()) && */!processed)
        processed = inherited::processSoEvent(ev);
    else
        return true;

    return processed;
}