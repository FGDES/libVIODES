/* gioview.cpp  - qgraphicsview for gioscenes */

/*
   Graphical  IO for FAU Discrete Event Systems Library (libfaudes)

   Copyright (C) 2009 - 2024 Thomas Moor;


*/

// local debugging
#define FAUDES_DEBUG_VIO


#include "gioview.h"
#include <QtGlobal>


// construct
GioView::GioView(QWidget* parent) :  QGraphicsView(parent) {
  FD_DQ("GioView::GioView(" << parent << ")");
  setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
  setDragMode(RubberBandDrag);
  setMouseTracking(true);
  grabGesture(Qt::GestureType::PinchGesture);
  mScale=1;
}

// Scale
void GioView::Scale(qreal sc) {
  //setResizeAnchor(AnchorUnderMouse);
  //scale(sc,sc);
  //setTransformationAnchor(NoAnchor);
  setTransform(QTransform(sc,0,0,sc,0,0));
  mScale=sc;
}

// Scale
qreal GioView::Scale(void) {  
  return mScale;
}


// Reset Matrix
void GioView::Reset(void) {
  setTransform(QTransform());
}

// fit scene
void GioView::Fit(void){
  GioSceneRo* gs = qobject_cast<GioSceneRo*>(scene());
  if(!gs) return;
  // set scene rectangle
  QRectF rect=gs->itemsBoundingRect();
  rect.adjust(-0.01*gs->width(),-0.01*gs->height(),
    0.01*gs->width(),0.01*gs->height());
  gs->setSceneRect(rect);
  // default: fit entire scene
  QRectF target = gs->sceneRect();
  // alt: fit selection
  if(gs->selectedItems().size() >= 1) {
    target= QRect();
    foreach(QGraphicsItem* item, gs->selectedItems()) {
      QPolygonF itempoly =item->mapToScene(item->boundingRect());
      target |=itempoly.boundingRect();
    }
  }
  // doit
  fitInView(target, Qt::KeepAspectRatio); 
}


// all events to handle gestures
bool GioView::event(QEvent *event) {
  if(event->type() == QEvent::Gesture)
    return gestureEvent(static_cast<QGestureEvent*>(event));
  return QGraphicsView::event(event);
}

// wheel event
void GioView::wheelEvent(QWheelEvent *event) {
  FD_DQ("GioView::wheelEvent(..) at (" << QCursor::pos().x() << ", " << QCursor::pos().y()
	<< ") with value " << event->angleDelta().y());
  // test for device mpuse wheel vs. touchpad (tested with Qt 6.2.4)
#if QT_VERSION > QT_VERSION_CHECK(6, 2, 0)  
  bool handle=false;
  if(event->deviceType()==QInputDevice::DeviceType::Mouse) {
    FD_DQ("GioView::wheeleEvent(..): its a mouse wheel: do zoom")
    handle=true;
  }
  if(event->deviceType()==QInputDevice::DeviceType::TouchPad) {
    FD_DQ("GioView::wheeleEvent(..): its a touchpad: pass on for scroll")
    handle=false;
  }
  if(!handle) {
    FD_DQ("GioView::wheeleEvent(..): pass on to base");
    QGraphicsView::wheelEvent(event);
    return;
  }
#endif  
  // scale factor
  qreal degree= event->angleDelta().y() / 8.0;
  if(degree < -30) degree = -30.0;
  if(degree >  30) degree = 30.0;
  qreal sc = 1.0 + ( 5.0 * degree / 3000.0 ); // 5% steps per 30 degree
  // where we are before scalung
  QPointF opos=mapToScene(QCursor::pos());
  FD_DQ("GioView::wheelEvent(..) at scenepos (" << opos.x() << ", " << opos.y() << ")");
  // do the scale
  setResizeAnchor(AnchorUnderMouse);
  setTransformationAnchor(AnchorUnderMouse);
  scale(sc,sc);
  setTransformationAnchor(NoAnchor);
  // translate
  QPointF npos=mapToScene(QCursor::pos());
  QPointF diff=npos-opos;
  //QPointF vdiff=mapFromScene(diff);
  translate(diff.x(),diff.y());
  // done
  event->accept();
  // notify
  emit NotifyZoom(Scale());
};


// get pinch for zoom: event
bool GioView::gestureEvent(QGestureEvent *gevent) {
  QPinchGesture* pinch=static_cast<QPinchGesture*>(gevent->gesture(Qt::PinchGesture));
  if(!pinch) return false;
  FD_DQ("GioView::gestureEvent(..) pinch at (" << pinch->hotSpot().x()<< ", " << pinch->hotSpot().y() << ") sf " << pinch->totalScaleFactor())
  // do the scale
  qreal sc=pinch->scaleFactor();
  setResizeAnchor(AnchorUnderMouse);
  setTransformationAnchor(AnchorUnderMouse);
  scale(sc,sc);
  setTransformationAnchor(NoAnchor);
  // notify
  emit NotifyZoom(Scale());  return true;
}

// handle my events: mouse press
// note: when rubber band drag ends, press void deselects but is not passed to scene
// note: the rubber band also gets irritated when the scene has a context menu
void GioView::mousePressEvent(QMouseEvent * event) {
   FD_DQ("GioView::mousePress()");
   if(event->button() == Qt::RightButton) return;
   QGraphicsView::mousePressEvent(event);
   if(event->isAccepted())
     if(GioScene* sc=qobject_cast<GioScene*>(scene())) sc->fixSelection();
}


// handle my events: mouse release
// note: when rubber band drag ends, release is not passed to scene
void GioView::mouseReleaseEvent(QMouseEvent * event) {
   FD_DQ("GioView::mouseRelease()");
   QGraphicsView::mouseReleaseEvent(event);
   if(event->isAccepted())
   if(GioScene* sc=qobject_cast<GioScene*>(scene())) {
      sc->userSelectionUpdate(); 
   }
   if(event->isAccepted())
   if(GioScene* sc=qobject_cast<GioScene*>(scene())) {
     if(sc->mMoveSelection) event->ignore();
   }
}


// handle my events: mouse double click
void GioView::mouseDoubleClickEvent(QMouseEvent * event) {
   //FD_DQ("GioView::mouseDoubleClickEvent()");
   // pass on
   QGraphicsView::mouseDoubleClickEvent(event);
}

// handle my events: mouse move
void GioView::mouseMoveEvent(QMouseEvent * event) {
  //FD_DQG("GioView::mouseMove()");
   QGraphicsView::mouseMoveEvent(event);
}
