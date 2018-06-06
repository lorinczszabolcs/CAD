#pragma once

#include <GL/glew.h>
#include <QGLWidget>
#include <QGLFormat>
#include <QTimer>
#include "../Parametric/ParametricCurves3.h"
#include "../Cyclic/CyclicCurves3.h"
#include "../Core/TriangulatedMeshes3.h"
#include "../Parametric/ParametricSurfaces3.h"
#include "../Core/Lights.h"
#include "../Core/ShaderPrograms.h"
#include "../Core/Materials.h"
#include "../Core/Colors4.h"
#include "../FirstOrderAlgebraicTrigonometric/FirstOrderAlgebraicTrigonometricArcs3.h"
#include "../FirstOrderAlgebraicTrigonometric/FirstOrderAlgebraicTrigonometricCompositeCurve3.h"
#include "../FirstOrderAlgebraicTrigonometric/FirstOrderAlgebraicTrigonometricPatches3.h"
#include "../FirstOrderAlgebraicTrigonometric/FirstOrderAlgebraicTrigonometricCompositeSurfaces3.h"

namespace cagd
{
    class GLWidget: public QGLWidget
    {
        Q_OBJECT

    private:

        // variables defining the projection matrix
        float       _aspect;            // aspect ratio of the rendering window
        float       _fovy;              // field of view in direction y
        float       _z_near, _z_far;    // distance of near and far clipping planes

        // variables defining the model-view matrix
        float       _eye[3], _center[3], _up[3];

        // variables needed by transformations
        int         _angle_x, _angle_y, _angle_z;
        double      _zoom;
        double      _trans_x, _trans_y, _trans_z;

        // Tab
        GLint       _tab_index;

        // Parametric curves
        GLint                           _pc_index;
        RowMatrix<ParametricCurve3*>    _pc;
        RowMatrix<GenericCurve3*>       _image_of_pc;
        GLboolean                       _fd_p, _sd_p;

        // Cyclic curve
        GLint                           _cc_index;
        CyclicCurve3*                   _cc;
        GenericCurve3*                  _image_of_cc;
        GLboolean                       _fd_cc, _sd_cc, _control_cc;

        // Interpolating cyclic curve
        CyclicCurve3*                   _icc;
        GenericCurve3*                  _image_of_icc;
        ColumnMatrix<GLdouble>*         _knot_vector;
        ColumnMatrix<DCoordinate3>*     _data_points_to_interpolate;
        GLint                           _cp_index;

        // Triangulated objects
        QTimer                         *_timer;
        GLfloat                         _angle;
        GLint                           _model_index;
        RowMatrix<TriangulatedMesh3>    _models;

        // Parametric surfaces
        RowMatrix<ParametricSurface3*> _ps;
        RowMatrix<TriangulatedMesh3*>  _img_ps;
        GLuint                         _ps_index;

        // Shaders
        RowMatrix<ShaderProgram*>      _shaders;
        RowMatrix<ShaderProgram*>      _shaders2;
        GLboolean                      _shader_enabled;
        GLboolean                      _shader_enabled2;
        GLint                          _shader_index;
        GLint                          _shader_index2;

        GLdouble                       _scale_factor;
        GLdouble                       _smoothing;
        GLdouble                       _shading;
        GLdouble                       _scale_factor2;
        GLdouble                       _smoothing2;
        GLdouble                       _shading2;

        Color4                         _outline;
        Color4                         _outline2;

        // Materials
        RowMatrix<Material*>           _materials;
        RowMatrix<Material*>           _materials2;
        GLboolean                      _material_enabled;
        GLboolean                      _material_enabled2;
        GLint                          _material_index;
        GLint                          _material_index2;

        // Arc
        FirstOrderAlgebraicTrigonometricArc3  *_arc;
        GenericCurve3                         *_image_of_arc;
        GLboolean                             _fd_arc, _sd_arc, _c_arc;
        GLint                                 _cp_index_arc;
        GLdouble                              _act_cp_arc_x, _act_cp_arc_y, _act_cp_arc_z;

        // curve
        FirstOrderAlgebraicTrigonometricCompositeCurve3 *_curve;
        GLint                                           _mod_curve;
        Color4*                                         _colors[3];
        GLint                                           _color_index_insert_curve, _color_index_continue_curve,
                                                        _color_index_join_curve, _color_index_curve,    // for color setting
                                                        _erase_index_c, _continue_index_c, _join_index1_c,
                                                        _join_index2_c, _merge_index1_c, _merge_index2_c;
        GLint                                           _continue_direction_c, _join_direction1_c, _join_direction2_c,
                                                        _merge_direction1_c, _merge_direction2_c;
        // Patch
        FirstOrderAlgebraicTrigonometricPatch *_patch_aux;
        FirstOrderAlgebraicTrigonometricPatch *_patch;
        RowMatrix<GenericCurve3*>             *_u_lines, *_v_lines;
        TriangulatedMesh3                     *_before_interpolation, *_after_interpolation;
        GLboolean                             _before_int, _after_int, _u_lin, _v_lin, _control_patch;
        GLint                                 _cp_index_u, _cp_index_v;
        GLdouble                              _act_cp_patch_x, _act_cp_patch_y, _act_cp_patch_z;
        GLdouble                              _alpha_u_patch, _alpha_v_patch;

        // surface
        FirstOrderAlgebraicTrigonometricSurface3 *_surface;
        GLboolean                                _surf, _u_lin_surf, _v_lin_surf, _control_surf;
        GLint                                    _material_index_insert_surf, _material_index_continue_surf,
                                                 _material_index_join_surf, _material_index_surf,
                                                 _continue_index_s, _erase_index_s, _join_index1_s,
                                                 _join_index2_s, _merge_index1_s, _merge_index2_s,
                                                 _merge_direction1_s, _merge_direction2_s, _join_direction1_s,
                                                 _join_direction2_s, _continue_direction_s;
        // Lights
        DirectionalLight *_dl;
        PointLight       *_pl;
        Spotlight        *_sl;

    public:
        // special and default constructor
        // the format specifies the properties of the rendering window
        GLWidget(QWidget* parent = 0, const QGLFormat& format = QGL::Rgba | QGL::DepthBuffer | QGL::DoubleBuffer);

        // redeclared virtual functions
        void initializeGL();
        void paintGL();
        void resizeGL(int w, int h);
        virtual ~GLWidget();

        // other public functions

        // parametric curve
        void initializeParametricCurves();
        void paintParametricCurves();

        // cyclic curve
        void initializeCyclicCurve();
        void paintCyclicCurve();

        // interpolating cyclic
        void initializeInterpolatingCyclicCurve();
        void paintInterpolatingCyclicCurve();

        // model
        void initializeModel();
        void paintModel();

        // parametric surface
        void initializeParametricSurfaces();
        void paintParametricSurfaces();

        // shaders
        void initializeShaders();
        void enableShader();
        void enableShader2();
        void disableShader();
        void disableShader2();

        // materials
        void initializeMaterials();
        void applyMaterial();
        void applyMaterial2();

        // arc
        void buildFirstOrderAlgebraicTrigonometricArc();
        void renderFirstOrderAlgebraicTrigonometricArc();

        // curve
        void buildFirstOrderAlgebraicTrigonometricCurve();
        void renderFirstOrderAlgebraicTrigonometricCurve();

        // patch
        void buildFirstOrderAlgebraicTrigonometricPatch();
        void renderFirstOrderAlgebraicTrigonometricPatch();

        // surface
        void buildFirstOrderAlgebraicTrigonometricSurface();
        void renderFirstOrderAlgebraicTrigonometricSurface();


    public slots:
        // public event handling methods/slots

        // trans + rot + zoom

        void set_angle_x(int value);
        void set_angle_y(int value);
        void set_angle_z(int value);

        void set_zoom_factor(double value);

        void set_trans_x(double value);
        void set_trans_y(double value);
        void set_trans_z(double value);


        // tabindex
        void setTabIndex(int index);

        // parametric
        void setParametricCurveIndex(int index);
        void toggleFirstOrderDerivativesParametric(bool b);
        void toggleSecondOrderDerivativesParametric(bool b);

        // cyclic
        void setCyclicCurveIndex(int index);
        void toggleFirstOrderDerivativesCyclic(bool b);
        void toggleSecondOrderDerivativesCyclic(bool b);
        void toggleControlPolygonCyclic(bool b);

        // interpolating
        void setControlPointIndex(int index);
        void setControlPointX (double value);
        void setControlPointY (double value);
        void setControlPointZ (double value);

        // model
        void setModelIndex(int index);

        // parametric surface
        void setParametricSurfaceIndex(int index);

        // shaders
        void setShaderIndex(int index);
        void setShaderIndex2(int index);
        void setMaterialIndex(int index);
        void setMaterialIndex2(int index);

        void setScaleFactor(double value);
        void setSmoothing(double value);
        void setShading(double value);
        void setOutlineR(double value);
        void setOutlineG(double value);
        void setOutlineB(double value);
        void setOutlineA(double value);

        void setScaleFactor2(double value);
        void setSmoothing2(double value);
        void setShading2(double value);
        void setOutlineR2(double value);
        void setOutlineG2(double value);
        void setOutlineB2(double value);
        void setOutlineA2(double value);

        // arc
        void toggleFirstOrderDerivativesArc(bool value);
        void toggleSecondOrderDerivativesArc(bool value);
        void toggleControlPolygonArc(bool value);
        void setAlphaArc(double value);
        void setControlPointArcIndex(int index);
        void setControlPointArcX (double value);
        void setControlPointArcY (double value);
        void setControlPointArcZ (double value);

        // curve

        // render
        void setMaxOrderOfDerivativesCurve(int value);

        // insert
        void setColorCurveInsert(int value);
        void insertArc(bool value);

        // continue
        void setContinueIndexCurve(int value);
        void setContinueDirectionCurve(int value);
        void setColorCurveContinue(int value);
        void continueArc(bool value);

        // join
        void setJoinIndex1Curve(int value);
        void setJoinIndex2Curve(int value);
        void setJoinDirection1Curve(int value);
        void setJoinDirection2Curve(int value);
        void setColorCurveJoin(int value);
        void joinArc(bool value);

        // merge
        void setMergeIndex1Curve(int value);
        void setMergeIndex2Curve(int value);
        void setMergeDirection1Curve(int value);
        void setMergeDirection2Curve(int value);
        void mergeArc(bool value);

        // erase
        void setEraseIndexCurve(int value);
        void eraseArc(bool value);


        // patch
        void setBeforeInterpolate(bool value);
        void setAfterInterpolate(bool value);
        void setULines(bool value);
        void setVLines(bool value);
        void setAlphaUPatch(double value);
        void setAlphaVPatch(double value);
        void setControlPointUPatchIndex(int index);
        void setControlPointVPatchIndex(int index);
        void toggleControlNetPatch(bool value);
        void setControlPointPatchX (double value);
        void setControlPointPatchY (double value);
        void setControlPointPatchZ (double value);

        // surface

        // render
        void toggleULinesSurface(bool value);
        void toggleVLinesSurface(bool value);
        void toggleControlNetSurface(bool value);
        void toggleSurfaceSurface(bool value);

        // insert
        void setMaterialSurfaceInsert(int value);
        void insertPatch(bool value);

        // continue
        void setContinueIndexSurface(int value);
        void setContinueDirectionSurface(int value);
        void setMaterialSurfaceContinue(int value);
        void continuePatch(bool value);

        // join
        void setJoinIndex1Surface(int value);
        void setJoinIndex2Surface(int value);
        void setJoinDirection1Surface(int value);
        void setJoinDirection2Surface(int value);
        void setMaterialSurfaceJoin(int value);
        void joinPatch(bool value);

        // merge
        void setMergeIndex1Surface(int value);
        void setMergeIndex2Surface(int value);
        void setMergeDirection1Surface(int value);
        void setMergeDirection2Surface(int value);
        void mergePatch(bool value);

        // erase
        void setEraseIndexSurface(int value);
        void erasePatch(bool value);

    private slots:
        void _animate();

    signals:

        // interpolating
        void xCoordinateChanged(double);
        void yCoordinateChanged(double);
        void zCoordinateChanged(double);

        // arc
        void xCoordinateArcChanged(double);
        void yCoordinateArcChanged(double);
        void zCoordinateArcChanged(double);

        // curve
        void arcNumberChanged(int);

        // patch
        void xCoordinatePatchChanged(double);
        void yCoordinatePatchChanged(double);
        void zCoordinatePatchChanged(double);

        // surface
        void patchNumberChanged(int);
    };
}
