/* viogen.h  - viodes generator widget plugin */


/*
   Graphical IO for FAU Discrete Event Systems Library (libfaudes)

   Copyright (C) 2009 - 2024 Thomas Moor;

*/

#ifndef FAUDES_VIOGEN_H
#define FAUDES_VIOGEN_H

// std includes
#include "libviodes.h"

// interface class
class VioGeneratorPlugin : 
  public QObject,
  public VioTypePlugin {

  Q_OBJECT
  Q_INTERFACES(VioTypePlugin) 
  Q_PLUGIN_METADATA(IID "org.faudes.viodes.VioTypePlugin")

public:

  // tell name of this plugin
  QString Name(void);

  // register base types 
  void RegisterTypes(void);

  // register more types 
  void FinaliseTypes(void);
};

#endif
