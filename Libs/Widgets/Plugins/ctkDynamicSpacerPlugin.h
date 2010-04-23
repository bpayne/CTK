/*=========================================================================

  Library:   CTK

  Copyright (c) Kitware Inc. 
  All rights reserved.
  Distributed under a BSD License. See LICENSE.txt file.

  This software is distributed "AS IS" WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the above copyright notice for more information.

=========================================================================*/

#ifndef __ctkDynamicSpacerPlugin_h
#define __ctkDynamicSpacerPlugin_h

// CTK includes
#include "ctkWidgetsAbstractPlugin.h"

class CTK_WIDGETS_PLUGINS_EXPORT ctkDynamicSpacerPlugin :
  public QObject,
  public ctkWidgetsAbstractPlugin
{
  Q_OBJECT

public:
  ctkDynamicSpacerPlugin(QObject *_parent = 0);

  QWidget *createWidget(QWidget *_parent);
  QString  domXml() const;
  QIcon    icon() const;
  QString  includeFile() const;
  bool     isContainer() const;
  QString  name() const;

};

#endif