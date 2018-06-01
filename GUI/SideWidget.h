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
            void setControlPointTranslateEnabled(int value);
            void setControlPointX(double value);
            void setControlPointY(double value);
            void setControlPointZ(double value);

            void setControlPointArcX(double value);
            void setControlPointArcY(double value);
            void setControlPointArcZ(double value);
    };
}
