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

    void SideWidget::setArcNumber(int value)
    {
        if (value < 0)
        {
            compCurveEraseButton->setDisabled(true);
            compCurveContinueButton->setDisabled(true);
            compCurveMergeButton->setDisabled(true);
            compCurveJoinButton->setDisabled(true);
            compCurveArcSpinBox->setDisabled(true);
            compCurvePointSlider->setDisabled(true);
            compCurveTranslateX->setDisabled(true);
            compCurveTranslateY->setDisabled(true);
            compCurveTranslateZ->setDisabled(true);
        }
        else
        {
            compCurveEraseButton->setDisabled(false);
            compCurveContinueButton->setDisabled(false);
            compCurveMergeButton->setDisabled(true);
            compCurveJoinButton->setDisabled(true);
            compCurveArcSpinBox->setDisabled(false);
            compCurvePointSlider->setDisabled(false);
            compCurveTranslateX->setDisabled(false);
            compCurveTranslateY->setDisabled(false);
            compCurveTranslateZ->setDisabled(false);
            if (value >= 1)
            {
                compCurveJoinButton->setDisabled(false);
                compCurveMergeButton->setDisabled(false);
            }
        }

        if (value < 0)
        {
            value = 0;
        }
        compCurveJoinFirstIndexSpinBox->setMaximum(value);
        compCurveJoinSecondIndexSpinBox->setMaximum(value);
        compCurveMergeFirstIndexSpinBox->setMaximum(value);
        compCurveMergeSecondIndexSpinBox->setMaximum(value);
        compCurveContinueIndexSpinBox->setMaximum(value);
        compCurveEraseIndexSpinBox->setMaximum(value);
        compCurveArcSpinBox->setMaximum(value);
    }

    void SideWidget::setControlPointCurveX(double value)
    {
        if (compCurveTranslateX->value() != value)
        {
            compCurveTranslateX->setValue(value);
        }
    }

    void SideWidget::setControlPointCurveY(double value)
    {
        if (compCurveTranslateY->value() != value)
        {
            compCurveTranslateY->setValue(value);
        }
    }

    void SideWidget::setControlPointCurveZ(double value)
    {
        if (compCurveTranslateZ->value() != value)
        {
            compCurveTranslateZ->setValue(value);
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

    void SideWidget::setPatchNumber(int value)
    {
        if (value < 0)
        {
            compSurfaceEraseButton->setDisabled(true);
            compSurfaceContinueButton->setDisabled(true);
            compSurfaceMergeButton->setDisabled(true);
            compSurfaceJoinButton->setDisabled(true);

            compSurfacePatchSpinBox->setDisabled(true);
            compSurfaceUPointSlider->setDisabled(true);
            compSurfaceVPointSlider->setDisabled(true);
            compSurfaceTranslateX->setDisabled(true);
            compSurfaceTranslateY->setDisabled(true);
            compSurfaceTranslateZ->setDisabled(true);
        }
        else
        {
            compSurfaceEraseButton->setDisabled(false);
            compSurfaceContinueButton->setDisabled(false);
            compSurfaceMergeButton->setDisabled(true);
            compSurfaceJoinButton->setDisabled(true);
            compSurfacePatchSpinBox->setDisabled(false);
            compSurfaceUPointSlider->setDisabled(false);
            compSurfaceVPointSlider->setDisabled(false);
            compSurfaceTranslateX->setDisabled(false);
            compSurfaceTranslateY->setDisabled(false);
            compSurfaceTranslateZ->setDisabled(false);
            if (value >= 1)
            {
                compSurfaceJoinButton->setDisabled(false);
                compSurfaceMergeButton->setDisabled(false);
            }
        }

        if (value < 0)
        {
            value = 0;
        }
        compSurfaceJoinFirstIndexSpinBox->setMaximum(value);
        compSurfaceJoinSecondIndexSpinBox->setMaximum(value);
        compSurfaceMergeFirstIndexSpinBox->setMaximum(value);
        compSurfaceMergeSecondIndexSpinBox->setMaximum(value);
        compSurfaceContinueIndexSpinBox->setMaximum(value);
        compSurfaceEraseIndexSpinBox->setMaximum(value);
        compSurfacePatchSpinBox->setMaximum(value);
    }

    void SideWidget::setControlPointSurfaceX(double value)
    {
        if (compSurfaceTranslateX->value() != value)
        {
            compSurfaceTranslateX->setValue(value);
        }
    }

    void SideWidget::setControlPointSurfaceY(double value)
    {
        if (compSurfaceTranslateY->value() != value)
        {
            compSurfaceTranslateY->setValue(value);
        }
    }

    void SideWidget::setControlPointSurfaceZ(double value)
    {
        if (compSurfaceTranslateZ->value() != value)
        {
            compSurfaceTranslateZ->setValue(value);
        }
    }

}
