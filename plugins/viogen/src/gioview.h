/* gioview.h  - qgraphicsview for gioscenes */

/*
   Graphical  IO for FAU Discrete Event Systems Library (libfaudes)

   Copyright (C) 2009 - 2024 Thomas Moor;

*/


#ifndef FAUDES_GIOVIEW_H
#define FAUDES_GIOVIEW_H

#include <QtGui>

#include "gioscenero.h"
#include "gioscene.h"

/*
 ************************************************
 ************************************************

 A GioView is a QGraphicsView intended to show a 
 GioSceneRo or a GioScene.  It adds zoom via 
 mouse wheel and other features.

 ************************************************
 ************************************************
 */


 
class GioView : public QGraphicsView {
  Q_OBJECT

public:
  // construct from scene
  GioView(QWidget* parent=nullptr);

public slots:

  // set views matrix
  void Reset(void);
  void Scale(qreal sc);
  qreal Scale(void);
  void Fit(void);

signals:

  // report user zoom
  void NotifyZoom(qreal sf);



protected:
  // reimplement mouse events
  void wheelEvent(QWheelEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void mouseDoubleClickEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;

  // get and implement gesture events
  bool event(QEvent *event) override;
  bool gestureEvent(QGestureEvent* gevent);

  // record scale
  qreal mScale;



};


#endif
