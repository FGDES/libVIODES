/* viohio.cpp  - viodes hiosys plugin */


/*
   Graphical IO for FAU Discrete Event Systems Library (libFAUDES)

   Copyright (C) 2009, 2024,  Thomas Moor;

*/

#include "viohio.h"


// tell name of this plugin
QString VioHiosysPlugin::Name(void) {
  return QString("VioHiosys-")+QString(VIODES_VERSION);
}

// register base types: all from config file
void VioHiosysPlugin::RegisterTypes(void) {
  FD_DQT("VioHiosysPlugin::RegisterTypes()");
}


// register my types: all from config file
void VioHiosysPlugin::FinaliseTypes(void) {
  FD_DQT("VioHiosysPlugin::FinaliseTypes()");
}
