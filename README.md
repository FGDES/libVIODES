# libVIODES - Qt Widgets to Represent libFAUDES Objects


## Design Objectives


libFAUDES is readily extendible by deriving new classes for specialized
synthesis and analysis algorithms. The luabindings plug-in allows easy
integration of such extension in the scripting environment luafaudes.
We need a similar approach for libFAUDES based GUI applications. libVIODES
targets to fill the gap.

- using the libFAUDES type registry as basis
- having a general model/view approach for faudes::Type
- derive classes in parallel to faudes::Type derivates
- organise the generator widget such that is can pick up
  new attributes
- make attribute widgets really easy to create

We have migrated libVIODES to Qt6. See https://github.com/FGDES/libVIODES-Qt4-Pre2024 for the most recent stable Qt4 based variant

## Code Organisation

#### Base classes provided by VioCore  

* base class for configuration classes (VioStyle)
* base classes for faudes::Type widgets (VioModel, VioView, VioWidget)
* VioTypeRegistry to parallel the libFAUDES registry, i.e., to programmatically keep track derivatives of VioModels 
* helper for editing faudes symbols 
* base class for faudes::Attribute* widgets (VioAttributeModel, VioAttributeWidget)
* Qt style plugin for extensions


#### VioStyle

* one static instance to configure visual and libFAUDES related defaults, eg
colors, fonts and the dot executable path
* classes derived from VioStyle to configure representations of particular faudes objects.
* all attributes are read initially from a single XML file

#### libFAUDES types vs libVIODES types

Each faudes object derived from faudes::Type is identified by its name from the faudes type registry for short fType. An fType is wrapped in a Qt-style model to hold additional
data for visual representation, eg orders of lists or positions of nodes in a graph. 
The base class for this purpose is VioModel. To allow for a single derivate of VioModel 
to represent closely related fTypes, vio models are configured by a VioStyle. The latter 
specifies the fType which the model shall represent, either read from the configuration 
file or set programmatically. Eacht derivate of VioModel and each configuration thereof 
is registered with its fType by the VioTypeRegistry.

Example given, there may be various semantics attached to the faudes flag attribute 
faudes::AttributeFlags. Each interpretation is to be registered by a distinct fType in 
the faudes type registry, eg AttributeStateFlags, AttibuteEventFlags etc. The VioAttributeStyle 
is derived from VioStyle to hold bit positions and property names. Per fType, one instance
of VioAttributeStyle is required to configure VioAttributeModel objects to represent
the respective fType.

To actually show a graphical representation, each VioModel class requiers a corresponding
VioView. The view passes on user changes via callbacks on the model. The model signals
changes via a signal/slot mechanism to the view(s). There is also a VioWidget, that
holds its own internal model and view. The VioViews learn about the model configuration
to adapt their representation, ie have proper names of checkboxes.

#### Derived libVIODES types as Qt-Style Plug-Ins

To extend libVIODES to display a specific fType, you need to derive corresponding
VioStyle, VioModel, VioView and VioWidget classes. Via the Qt plugin mechanism, 
you can register your extensons with the VioTypeResistry. An example is the generator plug-in
VioGen:

* plain generator representation (VioGeneratorModel, VioGeneratorWidget)
* specific generator representation (VioGeneratorList*, VioGeneratorGraph*)
* specialised Qt models/views (Lio*, Gio*)

The code of this pluglin is taken from an previous project for graph
representation (libVIOGen). 

## Tutorial/Testing

The base usaage of libVIODES is demonstrated by the Qt application "vioedit" that loads VioModels
from file and displays the corresponding VioView. It is meant as a basis
for testing ant debugging.


## Build System

To compile libVIODES, you will need Qt series 6. The minor version should not be critical: for Linux we are compiling with Qt 6.2.4 which we conveniently found in the Ubuntu package manager;  for Mac OS X we got the at the time of migrating most recent Qt 6.6.2 via the online installer provided by the Qt Project; Windows should be likewise, we did not test yet. For all three plattforms, the Qt version needs to be somehow activated, e.g., by setting the PATH environment variable to point to `qmake`; cerify with `qmake -v`.

You will also need a configured and compiled copy of libFAUDES. This
is expected to be located in `./libVIODES/libFAUDES_for_VIODES`. For POSIX systems, we provide a skript `copyfaudes.sh` that performs this step:

    ./libVIOEDS> . ./copyfaudes.sh

The script expects a plain libFAUDES next to `./libVIODES`, copies the sources to `./libVIODES/libFAUDES_for_VIODES`, sets configuration options and compiles libFAUDES using the libFAUDES `makefile`. The script is meant as a documentation on which configuration options are recommended, please inspect carefully before execution. For best results, it is recommended to use the same toolchain when compiling either libVIODES or libFAUDES, so you link against the same C runtime. Preferably, this toolchain also matches the one Qt was compiled with. This is a no-issue on Linux and Mac OSX (which have a system wide C runtime) but used to be tricky MS Windows (we will see).

Once you are set, 
compiling libVIODES is controlled by the Qt project `./libviodes/viodes.pro`. 
To compile use

    ./libVIODES> qmake viodes.pro
    ./libVIODES> make


## Authors/Copyright/License

The initial version of libVIODES was developped in the context of Ruediger Berndt's
masters project in 2009. Back then, our main concern was to parallel the libFAUDES type system with elementary Qt models in a flexible manner. Actual graphical representation of automata was merged from Thomas Moor's predecessor project VIOGen, with quite some effort for the various plug-ins. Principle authors are Thomas Moor and Ruediger Berdt, distribution is by terms of the LGPL 3.
