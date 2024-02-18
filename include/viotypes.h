/* viotypes.h  - vio type registry */


/*
   Graphical IO for FAU Discrete Event Systems Library (libfaudes)

   Copyright (C) 2009  Thomas Moor, Ruediger Berndt
   Copyright (C) 2010-2024 Thomas Moor

*/

#ifndef FAUDES_VIOTYPES_H
#define FAUDES_VIOTYPES_H

// std includes
#include "viostyle.h"
#include "viotoken.h" 
#include "vioelement.h" 


// debugging: types and plugins
#ifdef FAUDES_DEBUG_VIO_TYPE
#define FD_DQT(message) FAUDES_WRITE_CONSOLE("FAUDES_VIO_TYPES: " << message)
#else
#define FD_DQT(message)
#endif



// forward
class VioData;
class VioModel;
class VioView;
class VioWidget;

/*
 ************************************************
 ************************************************

 VioData is the base class for static data 
 for a widget representation of faudes type objects. 
 It holds the faudes object and any further data
 relevant for graphical representation.

 By static data we mean that the VioData object
 has no methods to edit the data. The latter is
 implemented by the according VioModel which operates
 on VioData. The separation of data and operation
 on data is used for the implementation of file i/o,
 cut/paste and undo/redo.

 The base VioData holds nothing but the faudes
 object and a formtated string for token based
 editing as a fallback representation of faudes
 objects.

 ************************************************
 ************************************************
 */

class VIODES_API VioData : public QObject {

Q_OBJECT
   
  friend class VioModel;
  friend class VioView;

public:

  // destruct
  virtual ~VioData(void);

  // clear to default/empty faudes object
  virtual void Clear(void);

  // serialisation, interface
  void Write(faudes::TokenWriter& rTw, const QString& ftype="") const;
  void Read(faudes::TokenReader& rTr, const QString& ftype="");

  // mime conversion (ret 0 on sucess)
  virtual QMimeData* ToMime(void);
  virtual int FromMime(const QMimeData* pMime);
  virtual int TestMime(const QMimeData* pMime);

  // access to faudes object
  virtual faudes::Type* FaudesObject(void);
  virtual const faudes::Type* FaudesObject(void) const;
  virtual void FaudesObject(faudes::Type* fobject);
  virtual QString FaudesType(void);

  // public data
  QString mText;

protected:

  // construct
  VioData(QObject* parent=0);

  // protected data: faudes object
  faudes::Type* mFaudesObject;
  
  // serialisation, virtual hooks
  virtual void DoWriteCore(faudes::TokenWriter& rTw, const QString& ftype="") const;
  virtual void DoWrite(faudes::TokenWriter& rTw, const QString& ftype="") const;
  virtual void DoReadCore(faudes::TokenReader& rTr, const QString& ftype="");
  virtual void DoRead(faudes::TokenReader& rTr, const QString& ftype="");
};



/*
 ************************************************
 ************************************************

 A VioModel is the base class for visually representable
 faudes type objects. It holds the faudes object plus
 any extra data required by a visual representation.
 Functionality provided is limited to token io. The
 actual representation is generated by a VioView.
 Errors are reported via faudes exceptions

 Naturally, the data relevant for a visual representation
 depends of the type of the faudes object. This base 
 class supports a textual representation based on
 faudes token io. Derived classes support tailord
 data structures, eg for graph representation of
 generators. To derive a class, you need to reimplement
 the constructor, the protected virtual methods, and
 the factory methods New___


 ************************************************
 ************************************************
 */



class VIODES_API VioModel : public QObject {

Q_OBJECT

public:

  // construct/destruct
  VioModel(QObject* parent=0, VioStyle* config=0, bool alloc=true);
  virtual ~VioModel(void);

  // construct on heap (factory methods)
  virtual VioModel* NewModel(QObject* parent=0) const;
  virtual VioData* NewData(QObject* parent=0) const;
  virtual VioView* NewView(QWidget* parent=0) const;
  virtual VioView* NewPropertyView(QWidget* parent=0) const;
  virtual VioView* NewConfigView(QWidget* parent=0) const;
  virtual VioWidget* NewWidget(QWidget* parent=0) const;

  // tell configuration
  VioStyle* Configuration(void) const;

  // clear to default/empty faudes object
  void Clear(void);

  // type info 
  const QString& FaudesType(void) const;

  // assignment: try to cast/extract data from other viomodel
  VioModel& Assign(const VioModel& rSrc);

  // set/get faudes object (return non-zero on error)

  // this has changed!!

  // note: if you set a non-const pointer, the model takes ownership of the faudes object
  // note: if you set a const pointer, the model allocates and copies the faudes object
  // note: if you get a non-const pointer, the viomodel becomes locked; you must set it in due course
  int FaudesObject(const faudes::Type* fobject);
  const faudes::Type* FaudesObject(void) const;
  faudes::Type* TakeFaudesObject(void);
  int InsertFaudesObject(faudes::Type* fobject);
  bool FaudesLocked(void) const;

  // trigger visuals to flush data: this should be automatic
  // in faudesobject access ... need to get rid of all the "const" concept
  void FlushViews(void);

  // faudes object access: name (incl signal on change)
  QString FaudesName(void) const;
  void FaudesName(const QString& rName);

  // token io (faudes exceptions on errors)
  void Write(faudes::TokenWriter& rTw);
  void Write(const QString& rFilename);
  void Read(faudes::TokenReader& rTr);
  void Read(const QString& rFilename);

  // faudes file import export (faudes exceptions on error)
  virtual void ExportFaudesFile(const QString& rFilename) const;
  virtual void ImportFaudesFile(const QString& rFilename);

  // vio data access
  virtual VioData* Data(void);
  virtual int Data(const VioData* pData);
  virtual int TypeCheckData(const VioData* pData);

  // synchronized item selection 
  virtual bool IsSelected(const VioElement& elem) const;
  virtual const QList<VioElement>& Selection(void) const;

  // typed vio data access (exception on error)
  void VioText(const QString& text);
  const QString& VioText(void) const;



  // record/query user changes 
  bool Modified(void) const;
  virtual void Modified(bool ch);



public slots:

  // collect and pass on modifications of childs
  void ChildModified(bool ch);  
  void ParentModified(bool ch);

  // highlite/show request (default: emit request to pass on to all views)
  virtual void Highlite(const VioElement& elem, bool on=true);
  virtual void HighliteClear(void);
  virtual void Show(const VioElement& elem);

  // state based undo scheme: undo/redo
  virtual void UndoStackClear(void);
  virtual void UndoEditStart(void);      
  virtual void UndoEditStop(void);       
  virtual void UndoEditCancel(void);
  virtual void Undo(void);
  virtual void Redo(void);

  // item selection: write access (todo: set based select/deselect for signal efficiency)
  void Select(const VioElement& elem, bool on=true);
  void SelectionClear(void);

signals:

  // notify application on changes by user
  void NotifyModified(bool ch);

  // notify views/application on changes, both, user and programmatic 
  void NotifyAnyChange(void);  
  void NotifyNameChange(void);  

  // forward highlite/show requests
  void NotifyHighlite(const VioElement& elem, bool on=true);
  void NotifyHighliteClear(void);
  void NotifyShow(const VioElement& elem);

  // notify application on user clicks (some view -> this model -> appl & other views)
  void MouseClick(const VioElement& elem);
  void MouseDoubleClick(const VioElement& elem);

  // notify application on status and errors
  void StatusMessage(const QString& message);
  void ErrorMessage(const QString& message);

  // notify application of selection change
  void NotifySelectionElement(const VioElement& elem, bool on);
  void NotifySelectionClear(void);
  void NotifySelectionAny(void);
  void NotifySelectionChange(void);

  // notify view about comming request to faudes object or model data (e.g. file output) 
  // lazy views get a last chance here to save their data in the model
  void NotifyFlush(void);

protected:

  // allocate faudes object and visual model data
  virtual void DoFaudesAllocate(void);
  virtual void DoVioAllocate(void);

  // assignment: try to cast/extract data from other viomodel
  virtual VioModel& DoAssign(const VioModel& rSrc);

  // test whether we can host this faudes object
  virtual int DoTypeCheck(const faudes::Type* fobject) const;

  // token io, implementation 
  virtual void DoVioWrite(faudes::TokenWriter& rTw) const;
  virtual void DoVioRead(faudes::TokenReader& rTr);

  // update visual data from (new) faudes object
  virtual void DoVioUpdate(void);

  // data access: merge data (return 1 on changes, 0 else)
  virtual int DoMergeData(const VioData* pData);

  // my configuration
  VioStyle* pConfig;

  // my data to operate on
  VioData* mData;

  // my faudes object lock
  bool mFaudesLocked;

  // my type
  QString mFaudesType;

  // record user changes
  bool mModified;

  // selection: list of elements
  QList<VioElement> mSelection;

  // state based undo scheme: data
  int mUndoEditLevel;
  int mUndoCurrent;
  QVector<VioData*> mUndoStack;
  static QList<VioModel*> msUndoDropOrder;

  // state based undo scheme: internal helper functions
  virtual void UndoStackPushBack(void); 
  virtual void UndoStackPopBack(void);   
  virtual void UndoStackPopFront(void);  


};


/*
 ************************************************
 ************************************************

 A VioView is the base class for visual representations
 of faudes objects encapsulated in VioModels. The interface 
 includes set/get by VioModel, and a signal-slot mechanism 
 to synchronize a model and its view.

 The actual representation is text based and uses
 faudes token io. Derived classes support tailord
 representations, eg graphs or lists for generators.

 ************************************************
 ************************************************
 */



class VIODES_API VioView : public QWidget {

Q_OBJECT

public:
  // construct/destruct
  VioView(QWidget* parent=0, VioStyle* config=0, bool alloc=true);
  virtual ~VioView(void);

  // types we can host
  const QString& FaudesType(void) const;

  // report configuration
  VioStyle* Configuration(void) const;

  // set/get the model
  // note: the view becomes not the owner of the model
  virtual int Model(VioModel* model);
  const VioModel* Model(void) const;

  // get faudes object 
  const faudes::Type* FaudesObject(void) const;

  // directed vio data access
  // note: the model only knows about item selections
  virtual int InsertData(const VioData* pData);
  virtual VioData* SelectionData(void);
  virtual void DeleteSelection(void);

  // directed mime access (convenience wrapper using mime encoding provided by VioData)
  int TestMimeData(const QMimeData* pMimeData);
  int InsertMimeData(const QMimeData* pMimeData);
  QMimeData* SelectionMimeData(void);

  // non std contribution to menus
  const QList<QAction*>& FileActions() const { return mFileActions; };
  const QList<QAction*>& EditActions() const { return mEditActions; };
  const QList<QAction*>& ViewActions() const { return mViewActions; };

  // std actions, view
  QAction* CutAction(void) const { return mCutAction; };
  QAction* CopyAction(void) const { return mCopyAction; };
  QAction* PasteAction(void) const { return mPasteAction; };

  // std actions, pass on to model
  QAction* UndoAction(void) const { return mUndoAction; };
  QAction* RedoAction(void) const { return mRedoAction; };

  // record/query user changes 
  bool Modified(void) const;
  virtual void Modified(bool ch);

public slots:

  // collect and pass on modifications of childs/parents
  void ChildModified(bool ch);
  void ParentModified(bool ch);

  // highlite/show request (process)
  virtual void Highlite(const VioElement& elem, bool on=true);
  virtual void HighliteClear(void);
  virtual void Show(const VioElement& elem);

  // set model from visual representation
  virtual void UpdateModel(void);

  // set view from model
  virtual void UpdateView(void);

  // run faudes function
  int ApplyFaudesFunction(const QString& ffnct);
  void ApplyEditFunction(void);

  // std edit, view
  virtual void Cut(void);
  virtual void Copy(void);
  virtual void Paste(void);

  // std edit, pass on to model
  virtual void Undo(void);
  virtual void Redo(void);

signals:

  // notify application on changees
  void NotifyModified(bool ch);

  // notify application on status and errors
  void StatusMessage(const QString& message);
  void ErrorMessage(const QString& message);

  // notify application on user clicks (this view -> model -> appl & other views)
  void MouseClick(const VioElement& elem);
  void MouseDoubleClick(const VioElement& elem);

  // forward show request (this view -> model -> other views)
  void NotifyShow(const VioElement& elem);
  void NotifyHighlite(const VioElement& elem, bool on);
  void NotifyHighliteClear(void);


protected:

  // allocate visual view data
  virtual void DoVioAllocate(void);

  // update view from (new) model
  virtual void DoVioUpdate(void);

  // update model from view (exception on error)
  virtual void DoModelUpdate(void);

  // the view can be used as eventfilter on child widgets
  // currently, we only make sure that short cuts remain with the view
  bool eventFilter(QObject *obj, QEvent *ev);

  // the type we can host
  QString mFaudesType;

  // my configuration
  VioStyle* pConfig;

  // representation data
  VioModel* pModel;

  // layout: general
  QVBoxLayout* mVbox;  

  // record changes
  bool mModified;

  // actions
  QList<QAction*> mFileActions;
  QList<QAction*> mEditActions;
  QList<QAction*> mViewActions;
  QAction* mCutAction;
  QAction* mCopyAction;
  QAction* mPasteAction;
  QAction* mRedoAction;
  QAction* mUndoAction;


  // default visaul representation widgets (debugging)
  QLabel* mTextInfo;
  VioTokenEditor* mTextEdit; 
  QPushButton* mApplyButton;

};


/*
 ************************************************
 ************************************************

 A VioWidget is the base class for widgets that host
 faudes objects. The visual representation is
 generated from an internal VioModel with an interal
 VioView. The interface includes token io, set/get 
 by VioModel or faudes object, signal based modified 
 detection.

 ************************************************
 ************************************************
 */


class VIODES_API VioWidget : public QWidget {

Q_OBJECT

public:
  // construct/destruct
  VioWidget(QWidget* parent=0, VioStyle* config=0, bool alloc=true);
  virtual ~VioWidget(void);

  // tell configuration
  VioStyle* Configuration(void) const;

  // clear to default/empty faudes object
  void Clear(void);

  // type info 
  const QString& FaudesType(void) const;

  // set/get by faudes object (return non-zero on error)
  // note: if you set a non-const pointer, the internal model takes ownweship of the faudes object
  // note: if you set a const pointer, the model allocates and copies the faudes object
  // note: if you get a non-const pointer, the viomodel becomes locked; you must  set it in due course
  int InsertFaudesObject(faudes::Type* fobject);
  faudes::Type* TakeFaudesObject(void);
  int FaudesObject(const faudes::Type* fobject);
  const faudes::Type* FaudesObject(void) const;


  // set/get by vio model
  // note: the widget becomes the owner of the model
  virtual int Model(VioModel* model);
  const VioModel* Model(void) const;

  // get view
  const VioView* View(void) const;

  // token io (faudes exceptions on errors)
  void Write(faudes::TokenWriter& rTw);
  void Write(const QString& rFilename);
  void Read(faudes::TokenReader& rTr);
  void Read(const QString& rFilename);

  // faudes file import export
  void ExportFaudesFile(const QString& rFilename) const;
  void ImportFaudesFile(const QString& rFilename);

  // nonstd contributions to menus
  const QList<QAction*>& FileActions() const;
  const QList<QAction*>& EditActions() const;
  const QList<QAction*>& ViewActions() const;

  // std actions
  QAction* CutAction(void) const;
  QAction* CopyAction(void) const;
  QAction* PasteAction(void) const;
  QAction* RedoAction(void) const;
  QAction* UndoAction(void) const;

  // record/query changes 
  bool Modified(void) const;
  virtual void Modified(bool ch);


public slots:

  // highlite/show request
  virtual void Highlite(const VioElement& elem, bool on=true);
  virtual void HighliteClear(void);
  virtual void Show(const VioElement& elem);

  // collect and pass on modifications of childs
  void ChildModified(bool ch);

  // std edit 
  void Cut(void);
  void Copy(void);
  void Paste(void);
  void Undo(void);
  void Redo(void);

signals:

  // notify application on changees
  void NotifyModified(bool ch);

  // notify application on changes, both, user and programmatic 
  void NotifyAnyChange(void);  

  // notify application on status and errors
  void StatusMessage(const QString& message);
  void ErrorMessage(const QString& message);

  // notify application on user clicks
  void MouseClick(const VioElement& elem);
  void MouseDoubleClick(const VioElement& elem);


protected:

  // allocate vio model/view
  virtual void DoVioAllocate(void);

  // my configuration
  VioStyle* pConfig;

  // model/view
  VioModel* mModel;
  VioView* mView;

  // layout: view in box
  QVBoxLayout* mVbox;  

  // record changes
  bool mModified;


};



#endif
