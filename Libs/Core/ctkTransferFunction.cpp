/*=========================================================================

  Library:   CTK
 
  Copyright (c) 2010  Kitware Inc.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.commontk.org/LICENSE

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 
=========================================================================*/
/// CTK includes
#include "ctkTransferFunction.h"
#include "ctkTransferFunctionRepresentation.h"

//-----------------------------------------------------------------------------
ctkControlPoint::~ctkControlPoint()
{ 
}

//-----------------------------------------------------------------------------
ctkBezierControlPoint::~ctkBezierControlPoint()
{
}

//-----------------------------------------------------------------------------
ctkNonLinearControlPoint::~ctkNonLinearControlPoint()
{
}

//-----------------------------------------------------------------------------
class ctkTransferFunctionPrivate:public ctkPrivate<ctkTransferFunction>
{
public:
  ctkTransferFunctionPrivate();
  ctkTransferFunctionRepresentation* Representation;
};

//-----------------------------------------------------------------------------
ctkTransferFunctionPrivate::ctkTransferFunctionPrivate()
{
  this->Representation = 0;
}

//-----------------------------------------------------------------------------
ctkTransferFunction::ctkTransferFunction(QObject* parentObject)
  :QObject(parentObject)
{
  CTK_INIT_PRIVATE(ctkTransferFunction);
  CTK_D(ctkTransferFunction);
  d->Representation = new ctkTransferFunctionRepresentation(this);
}

//-----------------------------------------------------------------------------
ctkTransferFunction::~ctkTransferFunction()
{
  // foreach(ctkControlPoint* cp, this->ControlPoints)
  //   {
  //   delete cp;
  //   }
  // this->ControlPoints->clear();
  // emit changed();
}

//-----------------------------------------------------------------------------
ctkTransferFunctionRepresentation* ctkTransferFunction::representation()const
{
  CTK_D(const ctkTransferFunction);
  return d->Representation;
}
