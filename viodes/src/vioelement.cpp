/* vioelement.cpp  - elementary data type */


/*
   Graphical IO for FAU Discrete Event Systems Library (libfaudes)

   Copyright (C) 2009 Ruediger Berndt, Thomas Moor;
   Copyright (C) 2010-2025 Thomas Moor.

*/


#include "vioelement.h"

/*
****************************************************************
****************************************************************
****************************************************************

Implementation: VioElement

****************************************************************
****************************************************************
****************************************************************
*/

// construct/destruct
VioElement::VioElement(void) : mEType(EVoid), mFTrans(0,0,0) {};
VioElement::VioElement(const VioElement& src) {
  mEType=src.mEType; mFTrans=src.mFTrans;};
const VioElement& VioElement::operator=(const VioElement& src) {
  mEType=src.mEType; mFTrans=src.mFTrans; return *this;}
VioElement::~VioElement(void) {};

// static constructors
VioElement VioElement::FromTrans(const faudes::Transition& ftrans) { 
  VioElement res; res.Trans(ftrans); return res;}; 
VioElement VioElement::FromState(const faudes::Idx& fstate) { 
  VioElement res; res.State(fstate); return res;}; 
VioElement VioElement::FromEvent(const faudes::Idx& fevent) { 
  VioElement res; res.Event(fevent); return res;}; 
VioElement VioElement::FromLine(const int& line) { 
  VioElement res; res.Line(line); return res;}; 
VioElement VioElement::FromType(const VioElement::EType etype) { 
  VioElement res; res.mEType=etype; return res;}; 
  
// get element type
VioElement::EType VioElement::Type(void) const { return mEType;};

// set value
void VioElement::Void(void) { 
  mFTrans=faudes::Transition(0,0,0); mEType=EVoid; };
void VioElement::Trans(const faudes::Transition& ftrans) { 
  mFTrans=ftrans; mEType=ETrans; };
void VioElement::State(const faudes::Idx& fstate) {
  mFTrans.X1=fstate; mEType=EState; };
void VioElement::Event(const faudes::Idx& fevent) { 
  mFTrans.Ev=fevent; mEType=EEvent; };
void VioElement::Line(const int& line) { 
  mFTrans.Ev=(faudes::Idx)line; mEType=ELine; };

// get value (todo: have const version)
faudes::Transition VioElement::Trans(void) const { 
  if(mEType==ETrans) return mFTrans;
  return faudes::Transition(0,0,0);
};
faudes::Idx VioElement::State(void) const { 
  if(mEType==EState) return mFTrans.X1;
  return 0;
};
faudes::Idx VioElement::Event(void) const { 
  if(mEType==EEvent) return mFTrans.Ev;
  return 0;
};
int VioElement::Line(void) const { 
  if(mEType==ELine) return mFTrans.Ev;
  return 0;
};
 
// untyped access
faudes::Idx VioElement::X1(void) const { return mFTrans.X1; };
faudes::Idx VioElement::Ev(void) const { return mFTrans.Ev; };
faudes::Idx VioElement::X2(void) const { return mFTrans.X2; };

// tell faudes elementary type
bool VioElement::IsVoid(void) const { return mEType==EVoid; };
bool VioElement::IsTrans(void) const { return mEType==ETrans; };
bool VioElement::IsState(void) const { return mEType==EState; };
bool VioElement::IsEvent(void) const { return mEType==EEvent; };
bool VioElement::IsLine(void) const { return mEType==ELine; };

// valid aka nonzero
bool VioElement::IsValid(void) const {
  if(mEType==ETrans && mFTrans.X1!=0 && mFTrans.Ev!=0 && mFTrans.X2!=0) return true;
  if(mEType==EState && mFTrans.X1!=0) return true;
  if(mEType==EEvent && mFTrans.Ev!=0) return true;
  if(mEType==ELine) return true;
  return false; 
};

// sort operator  
bool VioElement::operator<(const VioElement& rhs) const { 
  return mFTrans < rhs.mFTrans;};

// eauality test
bool VioElement::operator==(const VioElement& rhs) const { 
  return (mFTrans==rhs.mFTrans) && (mEType==rhs.mEType);};

// serialize elements: out, raw indexes
QDataStream& VioElement::DoWrite(QDataStream& out) const {
  out << (qint8) mEType;
  if(mEType==EVoid) { }
  else if(mEType==ETrans) { out << (qint32) mFTrans.X1 << (qint32) mFTrans.Ev << (qint32) mFTrans.X2; }
  else if(mEType==EState) { out << (qint32) mFTrans.X1; }
  else if(mEType==EEvent) { out << (qint32) mFTrans.Ev; }
  else if(mEType==ELine) { out << (qint32) mFTrans.Ev; }
  return out;
}

// serialize elements: in, raw indexes
QDataStream& VioElement::DoRead(QDataStream& in) {
  in >> (qint8&) mEType;
  if(mEType==EVoid) { }
  else if(mEType==ETrans) { in >> (qint32&) mFTrans.X1 >> (qint32&) mFTrans.Ev >> (qint32&) mFTrans.X2; }
  else if(mEType==EState) { in >> (qint32&) mFTrans.X1; }
  else if(mEType==EEvent) { in >> (qint32&) mFTrans.Ev; }
  else if(mEType==ELine) { in >> (qint32&) mFTrans.Ev; }
  return in;
}

// serialize elements: out, wrt generator for file io, pre 0.85
QDataStream& VioElement::DoWrite(QDataStream& out, const faudes::vGenerator* gen) const {
  out << (qint8) mEType;
  QString qevname=VioStyle::QStrFromStr(gen->EventName(mFTrans.Ev));
  if(mEType==EVoid) { }
  else if(mEType==ETrans) { out << (qint32) gen->MinStateIndex(mFTrans.X1) << qevname << (qint32) gen->MinStateIndex(mFTrans.X2); }
  else if(mEType==EState) { out << (qint32) gen->MinStateIndex(mFTrans.X1); }
  else if(mEType==EEvent) { out << qevname; }
  else if(mEType==ELine) { out << (qint32) mFTrans.Ev; }
  return out;
}

// serialize elements: in, wrt generator for file io, pre 0.85
QDataStream& VioElement::DoRead(QDataStream& in, const faudes::vGenerator* gen) {
  in >> (qint8&) mEType;
  QString qevname;
  if(mEType==EVoid) { }
  else if(mEType==ETrans) { in >> (qint32&) mFTrans.X1 >> qevname >> (qint32&) mFTrans.X2; }
  else if(mEType==EState) { in >> (qint32&) mFTrans.X1; }
  else if(mEType==EEvent) { in >> qevname; }
  else if(mEType==ELine) { in >> (qint32&) mFTrans.Ev; }
  if(qevname!="")
    mFTrans.Ev=gen->EventIndex(VioStyle::StrFromQStr(qevname));
  return in;
}

// serialize elements: out, wrt generator for file io
void VioElement::DoWrite(faudes::TokenWriter& rTw, const faudes::vGenerator* gen) const {
  faudes::Token etoken;
  switch(mEType) {
  case EEvent:    
    etoken.SetEmpty("Event");
    etoken.InsAttributeString("name",gen->EventName(mFTrans.Ev));
    break;
  case EState:
    etoken.SetEmpty("State");
    etoken.InsAttributeInteger("id",gen->MinStateIndex(mFTrans.X1));
    break;
  case ETrans:
    etoken.SetEmpty("Transition");
    etoken.InsAttributeInteger("x1",gen->MinStateIndex(mFTrans.X1));
    etoken.InsAttributeString("event",gen->EventName(mFTrans.Ev));
    etoken.InsAttributeInteger("x2",gen->MinStateIndex(mFTrans.X2));
    break;    
  case ELine:
    etoken.SetEmpty("Line");
    etoken.InsAttributeInteger("id",mFTrans.Ev);
    break;
  default:  
    etoken.SetEmpty("Void");
  }
  rTw.Write(etoken);
}

// serialize elements: out, wrt generator for file io
bool VioElement::DoRead(faudes::TokenReader& rTr, const faudes::vGenerator* gen) {
  faudes::Token etoken;
  mEType=EVoid;
  if(!rTr.Peek(etoken)) {
    return false;
  }  
  if(etoken.IsBegin("Void")) {
    rTr.Get(etoken);
    mEType=EVoid;
    rTr.ReadEnd("Void");
    return true;
  }  
  if(etoken.IsBegin("State")) {
    rTr.Get(etoken);
    mEType=EState;
    if(etoken.ExistsAttributeInteger("id"))    
      mFTrans.X1=etoken.AttributeIntegerValue("id");
    rTr.ReadEnd("State");
    return true;
  }  
  if(etoken.IsBegin("Event")) {
    rTr.Get(etoken);
    mEType=EEvent;
    if(etoken.ExistsAttributeString("name"))    
      mFTrans.Ev=gen->EventIndex(etoken.AttributeStringValue("name"));
    rTr.ReadEnd("Event");
    return true;
  }  
  if(etoken.IsBegin("Transition")) {
    rTr.Get(etoken);
    mEType=ETrans;
    if(etoken.ExistsAttributeInteger("x1"))    
      mFTrans.X1=etoken.AttributeIntegerValue("x1");
    if(etoken.ExistsAttributeString("event"))    
      mFTrans.Ev=gen->EventIndex(etoken.AttributeStringValue("event"));
    if(etoken.ExistsAttributeInteger("x2"))    
      mFTrans.X2=etoken.AttributeIntegerValue("x2");
    rTr.ReadEnd("Transition");
    return true;
  }  
  if(etoken.IsBegin("Line")) {
    rTr.Get(etoken);
    mEType=ELine;
    if(etoken.ExistsAttributeInteger("id"))    
      mFTrans.Ev=etoken.AttributeIntegerValue("id");
    rTr.ReadEnd("Line");
    return true;
  }  
  return false;
}

// faudes style debug string (todo: have generator ref)
std::string VioElement::Str(const faudes::vGenerator* gen) const { 
  std::string res="Void"; 
  if(gen==0) switch(mEType) {
    case ETrans: res="Trans["+mFTrans.Str()+"]"; break;
    case EState: res="State["+faudes::ToStringInteger(mFTrans.X1)+"]"; break;
    case EEvent: res="Event["+faudes::ToStringInteger(mFTrans.Ev)+"]"; break;
    case ELine: res="Line["+faudes::ToStringInteger(mFTrans.Ev)+"]"; break;
    default: break;
  }
  if(gen!=0) switch(mEType) {
    case ETrans: res=gen->TStr(mFTrans) + "[Trans]"; break;
    case EState: res=gen->SStr(mFTrans.X1) + "[State]"; break;
    case EEvent: res=gen->EStr(mFTrans.Ev) + "[Event]"; break;
    case ELine: res="Line["+faudes::ToStringInteger(mFTrans.Ev)+"]"; break;
    default: break;
  }
  return res;
};

// faudes style debug string for etype
std::string VioElement::TypeStr(EType etype) { 
  std::string res="Void"; 
  if(etype==ETrans) { res="Transition"; }
  else if(etype==EState) { res="State";}
  else if(etype==EEvent) { res="Event"; }
  else if(etype==ELine) { res="Line"; }
  return res;
};

// serialize lists etc
QDataStream& operator<<(QDataStream& out, const VioElement& elem) { return elem.DoWrite(out); };
QDataStream& operator>>(QDataStream& in , VioElement& elem) { return elem.DoRead(in); };



