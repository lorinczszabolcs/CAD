#pragma once

#include <QWidget>
#include "ui_SideWidget.h"
#include <GL/glew.h>

namespace cagd
{
    class SideWidget: public QWidget, public Ui::SideWidget
    {
        Q_OBJECT

        public:
            // special and default constructor
            SideWidget(QWidget *parent = 0);

        public slots:

            // cylic
            void setControlPointTranslateEnabled(int value);
            void setControlPointX(double value);
            void setControlPointY(double value);
            void setControlPointZ(double value);


            // arc
            void setControlPointArcX(double value);
            void setControlPointArcY(double value);
            void setControlPointArcZ(double value);


            // curve
            void setArcNumber(int value);
            void setControlPointCurveX(double value);
            void setControlPointCurveY(double value);
            void setControlPointCurveZ(double value);

            // patch
            void setControlPointPatchX(double value);
            void setControlPointPatchY(double value);
            void setControlPointPatchZ(double value);

            // surface
            void setPatchNumber(int value);
            void setControlPointSurfaceX(double value);
            void setControlPointSurfaceY(double value);
            void setControlPointSurfaceZ(double value);
    };
}
