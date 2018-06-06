#include "MainWindow.h"

namespace cagd
{
    MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
    {
        setupUi(this);

    /*

      the structure of the main window's central widget

     *---------------------------------------------------*
     |                 central widget                    |
     |                                                   |
     |  *---------------------------*-----------------*  |
     |  |     rendering context     |   scroll area   |  |
     |  |       OpenGL widget       | *-------------* |  |
     |  |                           | | side widget | |  |
     |  |                           | |             | |  |
     |  |                           | |             | |  |
     |  |                           | *-------------* |  |
     |  *---------------------------*-----------------*  |
     |                                                   |
     *---------------------------------------------------*

    */
        _side_widget = new SideWidget(this);

        _scroll_area = new QScrollArea(this);
        _scroll_area->setWidget(_side_widget);
        _scroll_area->setSizePolicy(_side_widget->sizePolicy());
        _scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        _gl_widget = new GLWidget(this);

        _gl_widget->adjustSize();

        _scroll_area->setFixedWidth(610);

        centralWidget()->setLayout(new QHBoxLayout());
        centralWidget()->layout()->addWidget(_gl_widget);
        centralWidget()->layout()->addWidget(_scroll_area);


        // ------------------- Rendering -------------------
        connect(_side_widget->rotate_x_slider, SIGNAL(valueChanged(int)), _gl_widget, SLOT(set_angle_x(int)));
        connect(_side_widget->rotate_y_slider, SIGNAL(valueChanged(int)), _gl_widget, SLOT(set_angle_y(int)));
        connect(_side_widget->rotate_z_slider, SIGNAL(valueChanged(int)), _gl_widget, SLOT(set_angle_z(int)));

        // ------------------- Zoom -------------------
        connect(_side_widget->zoom_factor_spin_box, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_zoom_factor(double)));

        // ------------------- Translation -------------------
        connect(_side_widget->trans_x_spin_box, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_trans_x(double)));
        connect(_side_widget->trans_y_spin_box, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_trans_y(double)));
        connect(_side_widget->trans_z_spin_box, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_trans_z(double)));

        // ------------------- Tabs -------------------
        connect(_side_widget->tabWidget, SIGNAL(currentChanged(int)), _gl_widget, SLOT(setTabIndex(int)));

        // ------------------- Parametric -------------------
        connect(_side_widget->ParametricCurveCombo, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(setParametricCurveIndex(int)));
        connect(_side_widget->firstOrderDerivativeCheckBox, SIGNAL(toggled(bool)), _gl_widget, SLOT(toggleFirstOrderDerivativesParametric(bool)));
        connect(_side_widget->secondOrderDerivativeCheckBox, SIGNAL(toggled(bool)), _gl_widget, SLOT(toggleSecondOrderDerivativesParametric(bool)));

        // ------------------- Cyclic -------------------
        connect(_side_widget->CyclicCurveCombo, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(setCyclicCurveIndex(int)));
        connect(_side_widget->firstOrderDerivativeCheckBox_2, SIGNAL(toggled(bool)), _gl_widget, SLOT(toggleFirstOrderDerivativesCyclic(bool)));
        connect(_side_widget->secondOrderDerivativeCheckBox_2, SIGNAL(toggled(bool)), _gl_widget, SLOT(toggleSecondOrderDerivativesCyclic(bool)));
        connect(_side_widget->controlPolygonCheckBox, SIGNAL(toggled(bool)), _gl_widget, SLOT(toggleControlPolygonCyclic(bool)));
        connect(_side_widget->CyclicCurveCombo, SIGNAL(currentIndexChanged(int)), _side_widget, SLOT(setControlPointTranslateEnabled(int)));

        // ------------------- Interpolating -------------------
        connect(_side_widget->controlPointSlider, SIGNAL(valueChanged(int)), _gl_widget, SLOT(setControlPointIndex(int)));
        connect(_side_widget->controlPointTranslateX, SIGNAL(valueChanged(double)), _gl_widget, SLOT(setControlPointX(double)));
        connect(_side_widget->controlPointTranslateY, SIGNAL(valueChanged(double)), _gl_widget, SLOT(setControlPointY(double)));
        connect(_side_widget->controlPointTranslateZ, SIGNAL(valueChanged(double)), _gl_widget, SLOT(setControlPointZ(double)));
        connect(_gl_widget, SIGNAL(xCoordinateChanged(double)), _side_widget, SLOT(setControlPointX(double)));
        connect(_gl_widget, SIGNAL(yCoordinateChanged(double)), _side_widget, SLOT(setControlPointY(double)));
        connect(_gl_widget, SIGNAL(zCoordinateChanged(double)), _side_widget, SLOT(setControlPointZ(double)));

        // ------------------- Models -------------------
        connect(_side_widget->modelComboBox, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(setModelIndex(int)));

        // ------------------- Parametric surfaces -------------------
        connect(_side_widget->ParametricSurfaceCombo, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(setParametricSurfaceIndex(int)));

        // ------------------- Shader + materials -------------------
        connect(_side_widget->shaderTab, SIGNAL(currentChanged(int)), _gl_widget, SLOT(setShaderIndex(int)));
        connect(_side_widget->shaderTab_2, SIGNAL(currentChanged(int)), _gl_widget, SLOT(setShaderIndex2(int)));
        connect(_side_widget->materialCombo, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(setMaterialIndex(int)));
        connect(_side_widget->materialCombo_2, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(setMaterialIndex2(int)));
        connect(_side_widget->scaleSpinBox, SIGNAL(valueChanged(double)), _gl_widget, SLOT(setScaleFactor(double)));
        connect(_side_widget->scaleSpinBox_2, SIGNAL(valueChanged(double)), _gl_widget, SLOT(setScaleFactor2(double)));
        connect(_side_widget->smoothSpinBox, SIGNAL(valueChanged(double)), _gl_widget, SLOT(setSmoothing(double)));
        connect(_side_widget->smoothSpinBox_2, SIGNAL(valueChanged(double)), _gl_widget, SLOT(setSmoothing2(double)));
        connect(_side_widget->shadeSpinBox, SIGNAL(valueChanged(double)), _gl_widget, SLOT(setShading(double)));
        connect(_side_widget->shadeSpinBox_2, SIGNAL(valueChanged(double)), _gl_widget, SLOT(setShading2(double)));
        connect(_side_widget->outlineSpinBoxR, SIGNAL(valueChanged(double)), _gl_widget, SLOT(setOutlineR(double)));
        connect(_side_widget->outlineSpinBoxR_2, SIGNAL(valueChanged(double)), _gl_widget, SLOT(setOutlineR2(double)));
        connect(_side_widget->outlineSpinBoxG, SIGNAL(valueChanged(double)), _gl_widget, SLOT(setOutlineG(double)));
        connect(_side_widget->outlineSpinBoxG_2, SIGNAL(valueChanged(double)), _gl_widget, SLOT(setOutlineG2(double)));
        connect(_side_widget->outlineSpinBoxB, SIGNAL(valueChanged(double)), _gl_widget, SLOT(setOutlineB(double)));
        connect(_side_widget->outlineSpinBoxB_2, SIGNAL(valueChanged(double)), _gl_widget, SLOT(setOutlineB2(double)));
        connect(_side_widget->outlineSpinBoxA, SIGNAL(valueChanged(double)), _gl_widget, SLOT(setOutlineA(double)));
        connect(_side_widget->outlineSpinBoxA_2, SIGNAL(valueChanged(double)), _gl_widget, SLOT(setOutlineA2(double)));

        // Arc
        connect(_side_widget->arcFirstOrderDerivativeCheckBox, SIGNAL(toggled(bool)), _gl_widget, SLOT(toggleFirstOrderDerivativesArc(bool)));
        connect(_side_widget->arcSecondOrderDerivativeCheckBox, SIGNAL(toggled(bool)), _gl_widget, SLOT(toggleSecondOrderDerivativesArc(bool)));
        connect(_side_widget->controlPointsArcCheckBox, SIGNAL(toggled(bool)), _gl_widget, SLOT(toggleControlPolygonArc(bool)));
        connect(_side_widget->doubleSpinBoxArcAlpha, SIGNAL(valueChanged(double)), _gl_widget, SLOT(setAlphaArc(double)));

        connect(_side_widget->controlPointArcSlider, SIGNAL(valueChanged(int)), _gl_widget, SLOT(setControlPointArcIndex(int)));
        connect(_side_widget->controlPointTranslateArcX, SIGNAL(valueChanged(double)), _gl_widget, SLOT(setControlPointArcX(double)));
        connect(_side_widget->controlPointTranslateArcY, SIGNAL(valueChanged(double)), _gl_widget, SLOT(setControlPointArcY(double)));
        connect(_side_widget->controlPointTranslateArcZ, SIGNAL(valueChanged(double)), _gl_widget, SLOT(setControlPointArcZ(double)));
        connect(_gl_widget, SIGNAL(xCoordinateArcChanged(double)), _side_widget, SLOT(setControlPointArcX(double)));
        connect(_gl_widget, SIGNAL(yCoordinateArcChanged(double)), _side_widget, SLOT(setControlPointArcY(double)));
        connect(_gl_widget, SIGNAL(zCoordinateArcChanged(double)), _side_widget, SLOT(setControlPointArcZ(double)));

        // ------------------- Curve -------------------

        // arc number
        connect(_gl_widget, SIGNAL(arcNumberChanged(int)), _side_widget, SLOT(setArcNumber(int)));
        // rendering
        connect(_side_widget->modCurveSpinBox, SIGNAL(valueChanged(int)), _gl_widget, SLOT(setMaxOrderOfDerivativesCurve(int)));

        // insert
        connect(_side_widget->compCurveInsertComboBoxColor, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(setColorCurveInsert(int)));
        connect(_side_widget->compCurveInsertButton, SIGNAL(clicked(bool)), _gl_widget, SLOT(insertArc(bool)));

        // continue
        connect(_side_widget->compCurveContinueButton, SIGNAL(clicked(bool)), _gl_widget, SLOT(continueArc(bool)));
        connect(_side_widget->compCurveContinueComboBoxColor, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(setColorCurveContinue(int)));
        connect(_side_widget->compCurveContinueDirectionComboBox, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(setContinueDirectionCurve(int)));
        connect(_side_widget->compCurveContinueIndexSpinBox, SIGNAL(valueChanged(int)), _gl_widget, SLOT(setContinueIndexCurve(int)));

        // join
        connect(_side_widget->compCurveJoinButton, SIGNAL(clicked(bool)), _gl_widget, SLOT(joinArc(bool)));
        connect(_side_widget->compCurveJoinFirstIndexSpinBox, SIGNAL(valueChanged(int)), _gl_widget, SLOT(setJoinIndex1Curve(int)));
        connect(_side_widget->compCurveJoinSecondIndexSpinBox, SIGNAL(valueChanged(int)), _gl_widget, SLOT(setJoinIndex1Curve(int)));
        connect(_side_widget->compCurveJoinFirstDirectionComboBox, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(setJoinDirection1Curve(int)));
        connect(_side_widget->compCurveJoinSecondDirectionComboBox, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(setJoinDirection2Curve(int)));
        connect(_side_widget->compCurveJoinComboBoxColor, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(setColorCurveJoin(int)));

        // merge
        connect(_side_widget->compCurveMergeButton, SIGNAL(clicked(bool)), _gl_widget, SLOT(mergeArc(bool)));
        connect(_side_widget->compCurveMergeFirstIndexSpinBox, SIGNAL(valueChanged(int)), _gl_widget, SLOT(setMergeIndex1Curve(int)));
        connect(_side_widget->compCurveMergeSecondIndexSpinBox, SIGNAL(valueChanged(int)), _gl_widget, SLOT(setMergeIndex2Curve(int)));
        connect(_side_widget->compCurveMergeFirstDirectionComboBox, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(setMergeDirection1Curve(int)));
        connect(_side_widget->compCurveMergeSecondDirectionComboBox, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(setMergeDirection2Curve(int)));

        // erase
        connect(_side_widget->compCurveEraseButton, SIGNAL(clicked(bool)), _gl_widget, SLOT(eraseArc(bool)));
        connect(_side_widget->compCurveEraseIndexSpinBox, SIGNAL(valueChanged(int)), _gl_widget, SLOT(setEraseIndexCurve(int)));

        // ------------------- Patch -------------------

        // booleans
        connect(_side_widget->checkBoxBeforeInt, SIGNAL(toggled(bool)), _gl_widget, SLOT(setBeforeInterpolate(bool)));
        connect(_side_widget->checkBoxAfterInt, SIGNAL(toggled(bool)), _gl_widget, SLOT(setAfterInterpolate(bool)));
        connect(_side_widget->checkBoxULines, SIGNAL(toggled(bool)), _gl_widget, SLOT(setULines(bool)));
        connect(_side_widget->checkBoxVLines, SIGNAL(toggled(bool)), _gl_widget, SLOT(setVLines(bool)));
        connect(_side_widget->controlNetPatchCheckBox, SIGNAL(toggled(bool)), _gl_widget, SLOT(toggleControlNetPatch(bool)));

        // alpha parameter along u and v
        connect(_side_widget->doubleSpinBoxPatchAlphaU, SIGNAL(valueChanged(double)), _gl_widget, SLOT(setAlphaUPatch(double)));
        connect(_side_widget->doubleSpinBoxPatchAlphaV, SIGNAL(valueChanged(double)), _gl_widget, SLOT(setAlphaVPatch(double)));

        // cp index along u and v
        connect(_side_widget->controlPointPatchSliderU, SIGNAL(valueChanged(int)), _gl_widget, SLOT(setControlPointUPatchIndex(int)));
        connect(_side_widget->controlPointPatchSliderV, SIGNAL(valueChanged(int)), _gl_widget, SLOT(setControlPointVPatchIndex(int)));

        // controlpoint coordinate
        connect(_side_widget->controlPointTranslatePatchX, SIGNAL(valueChanged(double)), _gl_widget, SLOT(setControlPointPatchX(double)));
        connect(_side_widget->controlPointTranslatePatchY, SIGNAL(valueChanged(double)), _gl_widget, SLOT(setControlPointPatchY(double)));
        connect(_side_widget->controlPointTranslatePatchZ, SIGNAL(valueChanged(double)), _gl_widget, SLOT(setControlPointPatchZ(double)));

        // signal emitter - update spinbox
        connect(_gl_widget, SIGNAL(xCoordinatePatchChanged(double)), _side_widget, SLOT(setControlPointPatchX(double)));
        connect(_gl_widget, SIGNAL(yCoordinatePatchChanged(double)), _side_widget, SLOT(setControlPointPatchY(double)));
        connect(_gl_widget, SIGNAL(zCoordinatePatchChanged(double)), _side_widget, SLOT(setControlPointPatchZ(double)));

        // ------------------- Surface -------------------

        // arc number
        connect(_gl_widget, SIGNAL(patchNumberChanged(int)), _side_widget, SLOT(setPatchNumber(int)));

        // rendering
        connect(_side_widget->compSurfaceULinesCheckBox, SIGNAL(toggled(bool)), _gl_widget, SLOT(toggleULinesSurface(bool)));
        connect(_side_widget->compSurfaceVLinesCheckBox, SIGNAL(toggled(bool)), _gl_widget, SLOT(toggleVLinesSurface(bool)));
        connect(_side_widget->compSurfaceControlCheckBox, SIGNAL(toggled(bool)), _gl_widget, SLOT(toggleControlNetSurface(bool)));
        connect(_side_widget->compSurfaceSurfaceCheckBox, SIGNAL(toggled(bool)), _gl_widget, SLOT(toggleSurfaceSurface(bool)));

        // insert
        connect(_side_widget->compSurfaceInsertComboBoxMaterial, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(setMaterialSurfaceInsert(int)));
        connect(_side_widget->compSurfaceInsertButton, SIGNAL(clicked(bool)), _gl_widget, SLOT(insertPatch(bool)));

        // continue
        connect(_side_widget->compSurfaceContinueButton, SIGNAL(clicked(bool)), _gl_widget, SLOT(continuePatch(bool)));
        connect(_side_widget->compSurfaceContinueComboBoxMaterial, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(setMaterialSurfaceContinue(int)));
        connect(_side_widget->compSurfaceContinueDirectionComboBox, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(setContinueDirectionSurface(int)));
        connect(_side_widget->compSurfaceContinueIndexSpinBox, SIGNAL(valueChanged(int)), _gl_widget, SLOT(setContinueIndexSurface(int)));

        // join
        connect(_side_widget->compSurfaceJoinButton, SIGNAL(clicked(bool)), _gl_widget, SLOT(joinPatch(bool)));
        connect(_side_widget->compSurfaceJoinFirstIndexSpinBox, SIGNAL(valueChanged(int)), _gl_widget, SLOT(setJoinIndex1Surface(int)));
        connect(_side_widget->compSurfaceJoinSecondIndexSpinBox, SIGNAL(valueChanged(int)), _gl_widget, SLOT(setJoinIndex2Surface(int)));
        connect(_side_widget->compSurfaceJoinFirstDirectionComboBox, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(setJoinDirection1Surface(int)));
        connect(_side_widget->compSurfaceJoinSecondDirectionComboBox, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(setJoinDirection2Surface(int)));
        connect(_side_widget->compSurfaceJoinComboBoxMaterial, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(setMaterialSurfaceJoin(int)));

        // merge
        connect(_side_widget->compSurfaceMergeButton, SIGNAL(clicked(bool)), _gl_widget, SLOT(mergePatch(bool)));
        connect(_side_widget->compSurfaceMergeFirstIndexSpinBox, SIGNAL(valueChanged(int)), _gl_widget, SLOT(setMergeIndex1Surface(int)));
        connect(_side_widget->compSurfaceMergeSecondIndexSpinBox, SIGNAL(valueChanged(int)), _gl_widget, SLOT(setMergeIndex2Surface(int)));
        connect(_side_widget->compSurfaceMergeFirstDirectionComboBox, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(setMergeDirection1Surface(int)));
        connect(_side_widget->compSurfaceMergeSecondDirectionComboBox, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(setMergeDirection2Surface(int)));

        // erase
        connect(_side_widget->compSurfaceEraseButton, SIGNAL(clicked(bool)), _gl_widget, SLOT(erasePatch(bool)));
        connect(_side_widget->compSurfaceEraseIndexSpinBox, SIGNAL(valueChanged(int)), _gl_widget, SLOT(setEraseIndexSurface(int)));
    }

    //--------------------------------
    // implementation of private slots
    //--------------------------------
    void MainWindow::on_action_Quit_triggered()
    {
        qApp->exit(0);
    }
}
