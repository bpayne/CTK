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

// Qt includes
#include <QDebug>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QStyleOptionSlider>
#include <QApplication>
#include <QStylePainter>
#include <QStyle>

// CTK includes
#include "ctkRangeSlider.h"

class ctkRangeSliderPrivate:public ctkPrivate<ctkRangeSlider>
{
public:
  ctkRangeSliderPrivate();
  void init();
  
  // Description:
  // Copied verbatim from QSliderPrivate class (see QSlider.cpp)
  int pixelPosToRangeValue(int pos) const;
  int pixelPosFromRangeValue(int val) const;

  // Description:
  // Draw the bottom and top sliders.
  void drawMinimumSlider( QStylePainter* painter ) const;
  void drawMaximumSlider( QStylePainter* painter ) const;
    
  // Description:
  // End points of the range on the Model
  int m_MaximumValue;
  int m_MinimumValue;

  // Description:
  // End points of the range on the GUI. This is synced with the model.
  int m_MaximumPosition;
  int m_MinimumPosition;

  // Description:
  // Controls selected ?
  QStyle::SubControl m_MinimumSliderSelected;
  QStyle::SubControl m_MaximumSliderSelected;

  // Description:
  // See QSliderPrivate::clickOffset. 
  // Overrides this ivar
  int m_SubclassClickOffset;
    
  // Description:
  // See QSliderPrivate::position
  // Overrides this ivar.
  int m_SubclassPosition;

  int m_SubclassWidth;
  // Description:
  // Boolean indicates the selected handle
  //   True for the minimum range handle, false for the maximum range handle
  enum Handle {
    NoHandle = 0x0,
    MinimumHandle = 0x1,
    MaximumHandle = 0x2
  };
  Q_DECLARE_FLAGS(Handles, Handle);
  ctkRangeSliderPrivate::Handles m_SelectedHandles;
};

// --------------------------------------------------------------------------
ctkRangeSliderPrivate::ctkRangeSliderPrivate()
{
  this->m_MinimumValue = 0;
  this->m_MaximumValue = 100;
  this->m_MinimumPosition = 0;
  this->m_MaximumPosition = 100;
  this->m_MinimumSliderSelected = QStyle::SC_None;
  this->m_MaximumSliderSelected = QStyle::SC_None;
  this->m_SubclassClickOffset = 0;
  this->m_SubclassPosition = 0;
  this->m_SubclassWidth = 0;
  this->m_SelectedHandles = 0;
}

// --------------------------------------------------------------------------
void ctkRangeSliderPrivate::init()
{
  CTK_P(ctkRangeSlider);
  this->m_MinimumValue = p->minimum();
  this->m_MaximumValue = p->maximum();
  this->m_MinimumPosition = p->minimum();
  this->m_MaximumPosition = p->maximum();
  p->connect(p, SIGNAL(rangeChanged(int, int)), p, SLOT(onRangeChanged(int, int)));
}

// --------------------------------------------------------------------------
// Copied verbatim from QSliderPrivate::pixelPosToRangeValue. See QSlider.cpp
//
int ctkRangeSliderPrivate::pixelPosToRangeValue( int pos ) const
{
  CTK_P(const ctkRangeSlider);
  QStyleOptionSlider option;
  p->initStyleOption( &option );

  QRect gr = p->style()->subControlRect( QStyle::CC_Slider, 
                                            &option, 
                                            QStyle::SC_SliderGroove, 
                                            p );
  QRect sr = p->style()->subControlRect( QStyle::CC_Slider, 
                                            &option, 
                                            QStyle::SC_SliderHandle, 
                                            p );
  int sliderMin, sliderMax, sliderLength;
  if (option.orientation == Qt::Horizontal) 
    {
    sliderLength = sr.width();
    sliderMin = gr.x();
    sliderMax = gr.right() - sliderLength + 1;
    }
  else
    {
    sliderLength = sr.height();
    sliderMin = gr.y();
    sliderMax = gr.bottom() - sliderLength + 1;
    }

  return QStyle::sliderValueFromPosition( p->minimum(), 
                                          p->maximum(), 
                                          pos - sliderMin,
                                          sliderMax - sliderMin, 
                                          option.upsideDown );
}

//---------------------------------------------------------------------------
int ctkRangeSliderPrivate::pixelPosFromRangeValue( int val ) const
{
  CTK_P(const ctkRangeSlider);
  QStyleOptionSlider option;
  p->initStyleOption( &option );

  QRect gr = p->style()->subControlRect( QStyle::CC_Slider, 
                                            &option, 
                                            QStyle::SC_SliderGroove, 
                                            p );
  QRect sr = p->style()->subControlRect( QStyle::CC_Slider, 
                                            &option, 
                                            QStyle::SC_SliderHandle, 
                                            p );
  int sliderMin, sliderMax, sliderLength;
  if (option.orientation == Qt::Horizontal) 
    {
    sliderLength = sr.width();
    sliderMin = gr.x();
    sliderMax = gr.right() - sliderLength + 1;
    }
  else
    {
    sliderLength = sr.height();
    sliderMin = gr.y();
    sliderMax = gr.bottom() - sliderLength + 1;
    }

  return QStyle::sliderPositionFromValue( p->minimum(), 
                                          p->maximum(), 
                                          val,
                                          sliderMax - sliderMin, 
                                          option.upsideDown ) + sliderMin;
}

//---------------------------------------------------------------------------
// Draw slider at the bottom end of the range
void ctkRangeSliderPrivate::drawMinimumSlider( QStylePainter* painter ) const
{
  CTK_P(const ctkRangeSlider);
  QStyleOptionSlider option;
  p->initMinimumSliderStyleOption( &option );

  option.subControls = QStyle::SC_SliderHandle;
  option.sliderValue = m_MinimumValue;
  option.sliderPosition = m_MinimumPosition;
  if (this->m_SelectedHandles & MinimumHandle)
    {
    option.activeSubControls = QStyle::SC_SliderHandle;
    option.state |= QStyle::State_Sunken;
    }

  painter->drawComplexControl(QStyle::CC_Slider, option);
}

//---------------------------------------------------------------------------
// Draw slider at the top end of the range
void ctkRangeSliderPrivate::drawMaximumSlider( QStylePainter* painter ) const
{
  CTK_P(const ctkRangeSlider);
  QStyleOptionSlider option;
  p->initMaximumSliderStyleOption( &option );

  option.subControls = QStyle::SC_SliderHandle;
  option.sliderValue = m_MaximumValue;
  option.sliderPosition = m_MaximumPosition;
  if (this->m_SelectedHandles & MaximumHandle)
    {
    option.activeSubControls = QStyle::SC_SliderHandle;
    option.state |= QStyle::State_Sunken;
    }
  painter->drawComplexControl(QStyle::CC_Slider, option);
}

// --------------------------------------------------------------------------
ctkRangeSlider::ctkRangeSlider(QWidget* parent)
  : QSlider(parent)
{
  CTK_INIT_PRIVATE(ctkRangeSlider);
  ctk_d()->init();
}

// --------------------------------------------------------------------------
ctkRangeSlider::ctkRangeSlider( Qt::Orientation o,
                                  QWidget* parentObject )
  :QSlider(o, parentObject)
{
  CTK_INIT_PRIVATE(ctkRangeSlider);
  ctk_d()->init();
}

// --------------------------------------------------------------------------
ctkRangeSlider::~ctkRangeSlider()
{
}

// --------------------------------------------------------------------------
int ctkRangeSlider::minimumValue() const
{
  CTK_D(const ctkRangeSlider);
  return d->m_MinimumValue;
}

// --------------------------------------------------------------------------
void ctkRangeSlider::setMinimumValue( int min )
{
  CTK_D(ctkRangeSlider);
  this->setValues( min, qMax(d->m_MaximumValue,min) );
}

// --------------------------------------------------------------------------
int ctkRangeSlider::maximumValue() const
{
  CTK_D(const ctkRangeSlider);
  return d->m_MaximumValue;
}

// --------------------------------------------------------------------------
void ctkRangeSlider::setMaximumValue( int max )
{
  CTK_D(ctkRangeSlider);
  this->setValues( qMin(d->m_MinimumValue, max), max );
}

// --------------------------------------------------------------------------
void ctkRangeSlider::setValues(int l, int u)
{
  CTK_D(ctkRangeSlider);
  const int minimumValue = 
    qBound(this->minimum(), qMin(l,u), this->maximum());
  const int maximumValue = 
    qBound(this->minimum(), qMax(l,u), this->maximum());
  bool emitMinValChanged = (minimumValue != d->m_MinimumValue);
  bool emitMaxValChanged = (maximumValue != d->m_MaximumValue);
  
  d->m_MinimumValue = minimumValue;
  d->m_MaximumValue = maximumValue;
  
  bool emitMinPosChanged = 
    (minimumValue != d->m_MinimumPosition);
  bool emitMaxPosChanged = 
    (maximumValue != d->m_MaximumPosition);
  d->m_MinimumPosition = minimumValue;
  d->m_MaximumPosition = maximumValue;
  
  if (isSliderDown())
    {
    if (emitMinPosChanged || emitMaxPosChanged)
      {
      emit positionsChanged(minimumValue, maximumValue);
      }
    if (emitMinPosChanged)
      {
      emit minimumPositionChanged(minimumValue);
      }
    if (emitMaxPosChanged)
      {
      emit maximumPositionChanged(maximumValue);
      }
    }
  if (emitMinValChanged || emitMaxValChanged)
    {
    emit valuesChanged(d->m_MinimumValue, 
                       d->m_MaximumValue);
    }
  if (emitMinValChanged)
    {
    emit minimumValueChanged(minimumValue);
    }
  if (emitMaxValChanged)
    {
    emit maximumValueChanged(maximumValue);
    }
  if (emitMinPosChanged || emitMaxPosChanged || 
      emitMinValChanged || emitMaxValChanged)
    {
    this->update();
    }
}

// --------------------------------------------------------------------------
int ctkRangeSlider::minimumPosition() const
{
  CTK_D(const ctkRangeSlider);
  return d->m_MinimumPosition;
}

// --------------------------------------------------------------------------
int ctkRangeSlider::maximumPosition() const
{
  CTK_D(const ctkRangeSlider);
  return d->m_MaximumPosition;
}

// --------------------------------------------------------------------------
void ctkRangeSlider::setMinimumPosition(int l)
{
  CTK_D(const ctkRangeSlider);
  this->setPositions(l, qMax(l, d->m_MaximumPosition));
}

// --------------------------------------------------------------------------
void ctkRangeSlider::setMaximumPosition(int u)
{
  CTK_D(const ctkRangeSlider);
  this->setPositions(qMin(d->m_MinimumPosition, u), u);
}

// --------------------------------------------------------------------------
void ctkRangeSlider::setPositions(int min, int max)
{
  CTK_D(ctkRangeSlider);
  const int minPosition = 
    qBound(this->minimum(), qMin(min, max), this->maximum());
  const int maxPosition = 
    qBound(this->minimum(), qMax(min, max), this->maximum());

  bool emitMinPosChanged = (minPosition != d->m_MinimumPosition);
  bool emitMaxPosChanged = (maxPosition != d->m_MaximumPosition);
  
  if (!emitMinPosChanged && !emitMaxPosChanged)
    {
    return;
    }

  d->m_MinimumPosition = minPosition;
  d->m_MaximumPosition = maxPosition;

  if (!this->hasTracking())
    {
    this->update();
    }
  if (isSliderDown())
    {
    if (emitMinPosChanged)
      {
      emit minimumPositionChanged(d->m_MinimumPosition);
      }
    if (emitMaxPosChanged)
      {
      emit maximumPositionChanged(d->m_MaximumPosition);
      }
    if (emitMinPosChanged || emitMaxPosChanged)
      {
      emit positionsChanged(d->m_MinimumPosition, d->m_MaximumPosition);
      }
    }
  if (this->hasTracking())
    {
    this->triggerAction(SliderMove);
    this->setValues(d->m_MinimumPosition, d->m_MaximumPosition);
    }
}

// --------------------------------------------------------------------------
void ctkRangeSlider::onRangeChanged(int minimum, int maximum)
{
  Q_UNUSED(minimum);
  Q_UNUSED(maximum);
  CTK_D(ctkRangeSlider);
  this->setValues(d->m_MinimumValue, d->m_MaximumValue);
}

// --------------------------------------------------------------------------
// Render
void ctkRangeSlider::paintEvent( QPaintEvent* )
{
  CTK_D(ctkRangeSlider);
  QStyleOptionSlider option;
  this->initStyleOption(&option);

  QStylePainter painter(this);
  option.subControls = QStyle::SC_SliderGroove;
  option.sliderPosition = this->minimum(); // don't highlight the SliderGroove
  painter.drawComplexControl(QStyle::CC_Slider, option);

  option.sliderPosition = d->m_MinimumPosition;
  const QRect lr = style()->subControlRect( QStyle::CC_Slider, 
                                            &option, 
                                            QStyle::SC_SliderHandle, 
                                            this);
  option.sliderPosition = d->m_MaximumPosition;

  const QRect ur = style()->subControlRect( QStyle::CC_Slider, 
                                            &option, 
                                            QStyle::SC_SliderHandle, 
                                            this);

  QRect sr = style()->subControlRect( QStyle::CC_Slider, 
                                      &option, 
                                      QStyle::SC_SliderGroove, 
                                      this);
  QRect rangeBox;
  if (option.orientation == Qt::Horizontal)
    {
    rangeBox = QRect(
      QPoint(qMin( lr.center().x(), ur.center().x() ), sr.center().y() - 2),
      QPoint(qMax( lr.center().x(), ur.center().x() ), sr.center().y() + 1));
    }
  else
    {
    rangeBox = QRect(
      QPoint(sr.center().x() - 2, qMin( lr.center().y(), ur.center().y() )),
      QPoint(sr.center().x() + 1, qMax( lr.center().y(), ur.center().y() )));
    }

  // -----------------------------
  // Render the range
  //
  QRect groove = this->style()->subControlRect( QStyle::CC_Slider, 
                                                &option, 
                                                QStyle::SC_SliderGroove, 
                                                this );
  groove.adjust(0, 0, -1, 0);

  // Create default colors based on the transfer function.
  //
  QColor highlight = this->palette().color(QPalette::Normal, QPalette::Highlight);
  QLinearGradient gradient;
  if (option.orientation == Qt::Horizontal)
    {
    gradient = QLinearGradient( groove.center().x(), groove.top(),
                                groove.center().x(), groove.bottom());
    }
  else
    {
    gradient = QLinearGradient( groove.left(), groove.center().y(),
                                groove.right(), groove.center().y());
    }

  // TODO: Set this based on the supplied transfer function
  QColor l = Qt::darkGray;
  QColor u = Qt::black;

  gradient.setColorAt(0, highlight.darker(120));
  gradient.setColorAt(1, highlight.lighter(160));

  painter.setPen(QPen(highlight.darker(150), 0));
  painter.setBrush(gradient);
  painter.drawRect( rangeBox.intersected(groove) );

  //  -----------------------------------
  // Render the sliders
  //
  if (d->m_SelectedHandles & ctkRangeSliderPrivate::MinimumHandle)
    {
    d->drawMaximumSlider( &painter );
    d->drawMinimumSlider( &painter );
    }
  else
    {
    d->drawMinimumSlider( &painter );
    d->drawMaximumSlider( &painter );
    }
}

// --------------------------------------------------------------------------
// Standard Qt UI events
void ctkRangeSlider::mousePressEvent(QMouseEvent* mouseEvent)
{
  CTK_D(ctkRangeSlider);
  if (minimum() == maximum() || (mouseEvent->buttons() ^ mouseEvent->button()))
    {
    mouseEvent->ignore();
    return;
    }
  int pos = this->orientation() == Qt::Horizontal ?
    mouseEvent->pos().x() : mouseEvent->pos().y();

  QStyleOptionSlider option;
  this->initStyleOption( &option );

  // Check if the first handle is pressed
  
  option.sliderPosition = d->m_MinimumPosition;
  option.sliderValue    = d->m_MinimumValue;

  QStyle::SubControl control;
  control = this->style()->hitTestComplexControl( QStyle::CC_Slider, 
                                                  &option, 
                                                  mouseEvent->pos(), 
                                                  this);
  const QRect lr = this->style()->subControlRect( QStyle::CC_Slider, 
                                                  &option, 
                                                  QStyle::SC_SliderHandle, 
                                                  this);
  if (control == QStyle::SC_SliderHandle)
    {
    d->m_SubclassPosition = d->m_MinimumPosition;

    // save the position of the mouse inside the handle for later
    d->m_SubclassClickOffset = pos - (this->orientation() == Qt::Horizontal ?
      lr.left() : lr.top());

    this->setSliderDown(true);

    if (d->m_SelectedHandles != ctkRangeSliderPrivate::MinimumHandle)
      {
      d->m_SelectedHandles = ctkRangeSliderPrivate::MinimumHandle;
      this->update(lr);
      }
    // Accept the mouseEvent
    mouseEvent->accept();
    return;
    }


  // The user didn't press on the minimum handle, 
  // Check if the other handle is pressed

  option.sliderPosition = d->m_MaximumPosition;
  option.sliderValue    = d->m_MaximumValue;

  control = this->style()->hitTestComplexControl( QStyle::CC_Slider, 
                                                  &option, 
                                                  mouseEvent->pos(), 
                                                  this);
  const QRect ur = this->style()->subControlRect( QStyle::CC_Slider, 
                                                  &option, 
                                                  QStyle::SC_SliderHandle, 
                                                  this);
  if (control == QStyle::SC_SliderHandle)
    {
    d->m_SubclassPosition = d->m_MaximumPosition;

    // save the position of the mouse inside the handle for later
    d->m_SubclassClickOffset = pos - (this->orientation() == Qt::Horizontal ?
      ur.left() : ur.top());

    this->setSliderDown(true);

    if (d->m_SelectedHandles != ctkRangeSliderPrivate::MaximumHandle)
      {
      d->m_SelectedHandles = ctkRangeSliderPrivate::MaximumHandle;
      this->update(ur);
      }
    // Accept the mouseEvent
    mouseEvent->accept();
    return;
    }

  // if we are here, no handles have been pressed
  // Check if we pressed on the groove between the 2 handles
  
  control = this->style()->hitTestComplexControl( QStyle::CC_Slider, 
                                                  &option, 
                                                  mouseEvent->pos(), 
                                                  this);
  QRect sr = style()->subControlRect( QStyle::CC_Slider, 
                                      &option, 
                                      QStyle::SC_SliderGroove, 
                                      this);
  int minCenter = (this->orientation() == Qt::Horizontal ?
    lr.center().x() : ur.center().y());
  int maxCenter = (this->orientation() == Qt::Horizontal ?
    ur.center().x() : lr.center().y());
  if (control == QStyle::SC_SliderGroove &&
      pos > minCenter && pos < maxCenter)
    {
    // warning lost of precision it might be fatal
    d->m_SubclassPosition = (d->m_MinimumPosition + d->m_MaximumPosition) / 2.;
    d->m_SubclassClickOffset = pos - d->pixelPosFromRangeValue(d->m_SubclassPosition);
    d->m_SubclassWidth = (d->m_MaximumPosition - d->m_MinimumPosition) / 2;
    qMax(d->m_SubclassPosition - d->m_MinimumPosition, d->m_MaximumPosition - d->m_SubclassPosition);
    this->setSliderDown(true);
    if (!(d->m_SelectedHandles & QFlags<ctkRangeSliderPrivate::Handle>(
            ctkRangeSliderPrivate::MinimumHandle)) || 
        !(d->m_SelectedHandles & QFlags<ctkRangeSliderPrivate::Handle>(ctkRangeSliderPrivate::MaximumHandle)))
      {
      d->m_SelectedHandles = 
        QFlags<ctkRangeSliderPrivate::Handle>(ctkRangeSliderPrivate::MinimumHandle) | 
        QFlags<ctkRangeSliderPrivate::Handle>(ctkRangeSliderPrivate::MaximumHandle);
      this->update(lr.united(ur).united(sr));
      }
    mouseEvent->accept();
    return;
    }
  mouseEvent->ignore();
}

// --------------------------------------------------------------------------
// Standard Qt UI events
void ctkRangeSlider::mouseMoveEvent(QMouseEvent* mouseEvent)
{
  CTK_D(ctkRangeSlider);
  if (!d->m_SelectedHandles)
    {
    mouseEvent->ignore();
    return;
    }
  int pos = this->orientation() == Qt::Horizontal ?
    mouseEvent->pos().x() : mouseEvent->pos().y();

  QStyleOptionSlider option;
  this->initStyleOption(&option);

  const int m = style()->pixelMetric( QStyle::PM_MaximumDragDistance, &option, this );

  int newPosition = d->pixelPosToRangeValue(pos - d->m_SubclassClickOffset);

  if (m >= 0)
    {
    const QRect r = rect().adjusted(-m, -m, m, m);
    if (!r.contains(mouseEvent->pos()))
      {
      newPosition = d->m_SubclassPosition;
      }
    }

  if (d->m_SelectedHandles == ctkRangeSliderPrivate::MinimumHandle)
    {
    this->setMinimumPosition(qMin(newPosition,d->m_MaximumPosition));
    }
  else if (d->m_SelectedHandles == ctkRangeSliderPrivate::MaximumHandle)
    {
    this->setMaximumPosition(qMax(d->m_MinimumPosition, newPosition));
    }
  else if (d->m_SelectedHandles & ctkRangeSliderPrivate::MinimumHandle && 
           d->m_SelectedHandles & ctkRangeSliderPrivate::MaximumHandle)
    {
    this->setPositions(newPosition - d->m_SubclassWidth, newPosition + d->m_SubclassWidth );
    }
  mouseEvent->accept();
}

// --------------------------------------------------------------------------
// Standard Qt UI mouseEvents
void ctkRangeSlider::mouseReleaseEvent(QMouseEvent* mouseEvent)
{
  CTK_D(ctkRangeSlider);
  this->QSlider::mouseReleaseEvent(mouseEvent);

  setSliderDown(false);
  d->m_SelectedHandles = 0;

  this->update();
}

// --------------------------------------------------------------------------
bool ctkRangeSlider::isMinimumSliderDown()const
{
  CTK_D(const ctkRangeSlider);
  return d->m_SelectedHandles & ctkRangeSliderPrivate::MinimumHandle;
}

// --------------------------------------------------------------------------
bool ctkRangeSlider::isMaximumSliderDown()const
{
  CTK_D(const ctkRangeSlider);
  return d->m_SelectedHandles & ctkRangeSliderPrivate::MaximumHandle;
}

// --------------------------------------------------------------------------
void ctkRangeSlider::initMinimumSliderStyleOption(QStyleOptionSlider* option) const
{
  this->initStyleOption(option);
}

// --------------------------------------------------------------------------
void ctkRangeSlider::initMaximumSliderStyleOption(QStyleOptionSlider* option) const
{
  this->initStyleOption(option);
}
