#include "SideWidget.h"

namespace cagd
{
    SideWidget::SideWidget(QWidget *parent): QWidget(parent)
    {
        setupUi(this);

        QPalette p = rotate_x_slider->palette();

        p.setColor(QPalette::Highlight, QColor(255,50,10).lighter());

        rotate_x_slider->setPalette(p);

        p = rotate_y_slider->palette();

        p.setColor(QPalette::Highlight, QColor(50,255,10).lighter());

        rotate_y_slider->setPalette(p);
    }

    void SideWidget::setControlPointTranslateEnabled(int value)
    {
        if (value == 0)
        {
            controlPointSlider->setEnabled(false);
            controlPointTranslateX->setEnabled(false);
            controlPointTranslateY->setEnabled(false);
            controlPointTranslateZ->setEnabled(false);
        }
        else
        {
            controlPointSlider->setEnabled(true);
            controlPointTranslateX->setEnabled(true);
            controlPointTranslateY->setEnabled(true);
            controlPointTranslateZ->setEnabled(true);
        }
    }

    void SideWidget::setControlPointX(double value)
    {
        if (controlPointTranslateX->value() != value)
        {
            controlPointTranslateX->setValue(value);
        }
    }

    void SideWidget::setControlPointY(double value)
    {
        if (controlPointTranslateY->value() != value)
        {
            controlPointTranslateY->setValue(value);
        }
    }

    void SideWidget::setControlPointZ(double value)
    {
        if (controlPointTranslateZ->value() != value)
        {
            controlPointTranslateZ->setValue(value);
        }
    }

    void SideWidget::setControlPointArcX(double value)
    {
        if (controlPointTranslateArcX->value() != value)
        {
            controlPointTranslateArcX->setValue(value);
        }
    }

    void SideWidget::setControlPointArcY(double value)
    {
        if (controlPointTranslateArcY->value() != value)
        {
            controlPointTranslateArcY->setValue(value);
        }
    }

    void SideWidget::setControlPointArcZ(double value)
    {
        if (controlPointTranslateArcZ->value() != value)
        {
            controlPointTranslateArcZ->setValue(value);
        }
    }

    void SideWidget::setControlPointPatchX(double value)
    {
        if (controlPointTranslatePatchX->value() != value)
        {
            controlPointTranslatePatchX->setValue(value);
        }
    }

    void SideWidget::setControlPointPatchY(double value)
    {
        if (controlPointTranslatePatchY->value() != value)
        {
            controlPointTranslatePatchY->setValue(value);
        }
    }

    void SideWidget::setControlPointPatchZ(double value)
    {
        if (controlPointTranslatePatchZ->value() != value)
        {
            controlPointTranslatePatchZ->setValue(value);
        }
    }

}
