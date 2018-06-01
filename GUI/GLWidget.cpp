#include "GLWidget.h"
#include <GL/glu.h>

#include <iostream>
using namespace std;

#include <Core/Exceptions.h>
#include "../Core/Matrices.h"
#include "../Test/TestFunctions.h"
#include "../Core/Constants.h"

namespace cagd
{
    //--------------------------------
    // special and default constructor
    //--------------------------------
    GLWidget::GLWidget(QWidget *parent, const QGLFormat &format): QGLWidget(format, parent)
    {

        // timer for off
        _timer = new QTimer(this);
        _timer->setInterval(0);

        connect(_timer, SIGNAL(timeout()), this, SLOT(_animate()));

        // initial value for tab
        _tab_index = 0;
    }

    //--------------------------------------------------------------------------------------
    // this virtual function is called once before the first call to paintGL() or resizeGL()
    //--------------------------------------------------------------------------------------
    void GLWidget::initializeGL()
    {
        // creating a perspective projection matrix
        glMatrixMode(GL_PROJECTION);

        glLoadIdentity();

        _aspect = (float)width() / (float)height();
        _z_near = 1.0;
        _z_far = 1000.0;
        _fovy = 45.0;

        gluPerspective(_fovy, _aspect, _z_near, _z_far);

        // setting the model view matrix
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        _eye[0] = _eye[1] = 0.0, _eye[2] = 6.0;
        _center[0] = _center[1] = _center[2] = 0.0;
        _up[0] = _up[2] = 0.0, _up[1] = 1.0;

        gluLookAt(_eye[0], _eye[1], _eye[2], _center[0], _center[1], _center[2], _up[0], _up[1], _up[2]);

        // enabling depth test
        glEnable(GL_DEPTH_TEST);

        // setting the color of background
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        // initial values of transformation parameters
        _angle_x = _angle_y = _angle_z = 0.0;
        _trans_x = _trans_y = _trans_z = 0.0;
        _zoom = 1.0;

        glEnable(GL_POINT_SMOOTH);
        glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        glEnable(GL_POLYGON_SMOOTH);
        glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

        glEnable(GL_DEPTH_TEST);

        try
        {
            // initializing the OpenGL Extension Wrangler library
            GLenum error = glewInit();

            if (error != GLEW_OK)
            {
                throw Exception("Could not initialize the OpenGL Extension Wrangler Library!");
            }

            if (!glewIsSupported("GL_VERSION_2_0"))
            {
                throw Exception("Your graphics card is not compatible with OpenGL 2.0+! "
                                "Try to update your driver or buy a new graphics adapter!");
            }

            // create and store your geometry in display lists or vertex buffer objects
            initializeParametricCurves();
            initializeCyclicCurve();
            initializeInterpolatingCyclicCurve();
            initializeModel();
            initializeParametricSurfaces();
            initializeShaders();
            initializeMaterials();
            buildFirstOrderAlgebraicTrigonometricArc();
            buildFirstOrderAlgebraicTrigonometricPatch();
        }
        catch (Exception &e)
        {
            cout << e << endl;
        }

    }

    //-----------------------
    // the rendering function
    //-----------------------
    void GLWidget::paintGL()
    {
        // clears the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glDisable(GL_NORMALIZE);

        // stores/duplicates the original model view matrix
        glPushMatrix();

            // applying transformations
            glRotatef(_angle_x, 1.0, 0.0, 0.0);
            glRotatef(_angle_y, 0.0, 1.0, 0.0);
            glRotatef(_angle_z, 0.0, 0.0, 1.0);
            glTranslated(_trans_x, _trans_y, _trans_z);
            glScaled(_zoom, _zoom, _zoom);

            if (_tab_index == 0)
            {
                paintParametricCurves();
            }
            else if (_tab_index == 1)
            {

                if(_cc_index == 0)
                {
                    paintCyclicCurve();
                }
                else if (_cc_index == 1)
                {
                    paintInterpolatingCyclicCurve();
                }
            }
            else if (_tab_index == 2)
            {
                paintModel();
            }
            else if (_tab_index == 3)
            {
                paintParametricSurfaces();
            }
            else if (_tab_index == 4)
            {
                renderFirstOrderAlgebraicTrigonometricArc();
            }
            else if (_tab_index == 5)
            {
                renderFirstOrderAlgebraicTrigonometricPatch();
            }

        // pops the current matrix stack, replacing the current matrix with the one below it on the stack,
        // i.e., the original model view matrix is restored
        glPopMatrix();

    }

    //----------------------------------------------------------------------------
    // when the main window is resized one needs to redefine the projection matrix
    //----------------------------------------------------------------------------
    void GLWidget::resizeGL(int w, int h)
    {
        // setting the new size of the rendering context
        glViewport(0, 0, w, h);

        // redefining the projection matrix
        glMatrixMode(GL_PROJECTION);

        glLoadIdentity();

        _aspect = (float)w / (float)h;

        gluPerspective(_fovy, _aspect, _z_near, _z_far);

        // switching back to the model view matrix
        glMatrixMode(GL_MODELVIEW);

        updateGL();
    }

    //----------------------------------
    // implementation of private methods
    //----------------------------------
    void GLWidget::initializeParametricCurves()
    {
        _pc.ResizeColumns(6);
        _image_of_pc.ResizeColumns(6);
        _pc_index = 0;
        _fd_p = true;
        _sd_p = true;

        RowMatrix<ParametricCurve3::Derivative> derivative(3);
        derivative(0) = spiral_on_cone::d0;
        derivative(1) = spiral_on_cone::d1;
        derivative(2) = spiral_on_cone::d2;

        _pc[0] = new (nothrow) ParametricCurve3(derivative, spiral_on_cone::u_min, spiral_on_cone::u_max);

        derivative(0) = butterfly::d0;
        derivative(1) = butterfly::d1;
        derivative(2) = butterfly::d2;

        _pc[1] = new (nothrow) ParametricCurve3(derivative, butterfly::u_min, butterfly::u_max);

        derivative(0) = astroid::d0;
        derivative(1) = astroid::d1;
        derivative(2) = astroid::d2;

        _pc[2] = new (nothrow) ParametricCurve3(derivative, astroid::u_min, astroid::u_max);

        derivative(0) = epicycloid::d0;
        derivative(1) = epicycloid::d1;
        derivative(2) = epicycloid::d2;

        _pc[3] = new (nothrow) ParametricCurve3(derivative, epicycloid::u_min, epicycloid::u_max);

        derivative(0) = epitrochoid::d0;
        derivative(1) = epitrochoid::d1;
        derivative(2) = epitrochoid::d2;

        _pc[4] = new (nothrow) ParametricCurve3(derivative, epitrochoid::u_min, epitrochoid::u_max);

        derivative(0) = plateau::d0;
        derivative(1) = plateau::d1;
        derivative(2) = plateau::d2;

        _pc[5] = new (nothrow) ParametricCurve3(derivative, plateau::u_min, plateau::u_max);

        GLuint div_point_count = 500;
        GLenum usage_flag = GL_STATIC_DRAW;

        try
        {
            for (GLuint i = 0; i < 6; i++)
            {
                _image_of_pc[i] = _pc[i]->GenerateImage(div_point_count,usage_flag);

                if (! _image_of_pc[i])
                {
                    throw("Could not initialize image of parametric curve!");
                }

                if (!_image_of_pc[i]->UpdateVertexBufferObjects(1.0, usage_flag))
                {
                    throw("Could not create vertex buffer object of the parametric curve!");
                }
            }
        }
        catch(Exception &e)
        {
            cout << e << "\n";
        }
    }

    void GLWidget::paintParametricCurves()
    {
        if (_image_of_pc[_pc_index])
        {
            glColor3f(1.0, 0.0, 0.0);
            _image_of_pc[_pc_index]->RenderDerivatives(0,GL_LINE_STRIP);

            if (_fd_p)
            {
                glPointSize(5.0);
                    glColor3f(0.0,0.2,0.0);
                    _image_of_pc[_pc_index]->RenderDerivatives(1, GL_LINES);
                    _image_of_pc[_pc_index]->RenderDerivatives(1, GL_POINTS);
                glPointSize(1.0);
            }

            if (_sd_p)
            {
                glPointSize(5.0);
                     glColor3f(0.1,0.2,0.4);
                     _image_of_pc[_pc_index]->RenderDerivatives(2, GL_LINES);
                     _image_of_pc[_pc_index]->RenderDerivatives(2, GL_POINTS);
                glPointSize(1.0);
            }
        }
    }

    void GLWidget::initializeCyclicCurve()
    {
        // initial value for cyclic curve
        _cc_index = 0;

        _fd_cc = true;
        _fd_cc = true;
        _control_cc = true;

        GLdouble n = 5;
        _cc = new (nothrow) CyclicCurve3(n);

        if (!_cc)
        {
            cout << "Could not create cyclic curve\n";
        }

        GLenum usage_flag = GL_STATIC_DRAW;

        GLdouble u_min;
        GLdouble u_max;
        _cc->GetDefinitionDomain(u_min, u_max);

        GLdouble step = (u_max - u_min) / (2 * n + 1);

        for (GLuint i = 0; i <= 2 * n; i++)
        {
            GLdouble u = min(u_min + i * step, u_max);
            (*_cc)[i][0] = sin(u);
            (*_cc)[i][1] = cos(u);
            (*_cc)[i][2] = -2.0 + 4.0 * (GLdouble) rand() / RAND_MAX;
        }

        try
        {
            if (!_cc->UpdateVertexBufferObjectsOfData(usage_flag))
            {
                throw("Can not update Vertex Buffer Objects of data.");
            }

            _image_of_cc = _cc->GenerateImage(n,100,usage_flag);
            if (!_image_of_cc)
            {
                throw("Could not create image of cyclic curve!");
            }

            if (!_image_of_cc->UpdateVertexBufferObjects(1.0, usage_flag))
            {
                throw("Could not create vertex buffer object of the cyclic curve!");
            }
        }
        catch (Exception &e)
        {
            cout << e << "\n";
        }
    }

    void GLWidget::paintCyclicCurve()
    {
        GLenum render_mode = GL_LINE_LOOP;

        if (_cc)
        {
            glColor3f(0.901, 0.494, 0.133);
            if (_control_cc)
            {
                _cc->RenderData(render_mode);
            }
        }
        if (_image_of_cc)
        {
            glColor3f(1.0, 0.0, 0.0);
            _image_of_cc->RenderDerivatives(0,GL_LINE_LOOP);

            if (_fd_cc)
            {
                glPointSize(5.0);
                    glColor3f(0.0,0.2,0.0);
                    _image_of_cc->RenderDerivatives(1, GL_LINES);
                    _image_of_cc->RenderDerivatives(1, GL_POINTS);
                glPointSize(1.0);
            }

            if (_sd_cc)
            {
                glPointSize(5.0);
                    glColor3f(0.1,0.2,0.4);
                    _image_of_cc->RenderDerivatives(2, GL_LINES);
                    _image_of_cc->RenderDerivatives(2, GL_POINTS);
                glPointSize(1.0);
            }
        }
    }

    void GLWidget::initializeInterpolatingCyclicCurve()
    {

        // initial value for control point
        _cp_index = 0;

        GLenum usage_flag = GL_STATIC_DRAW;

        GLdouble n = 2;
        _icc = new (nothrow) CyclicCurve3(n);

        if (!_icc)
        {
            cout << "Could not create cyclic curve\n";
        }

        GLdouble u_min, u_max;
        _icc->GetDefinitionDomain(u_min, u_max);

        _knot_vector = new ColumnMatrix::ColumnMatrix<GLdouble>(2 * n + 1);
        _data_points_to_interpolate = new ColumnMatrix::ColumnMatrix<DCoordinate3>(2 * n + 1);

        GLdouble step = (u_max - u_min) / (2 * n + 1);

        for (GLuint i = 0; i <= 2 * n; i++)
        {
            GLdouble u = min(u_min + i * step, u_max);
            (*_knot_vector)[i] = u;
            (*_data_points_to_interpolate)[i][0] = sin(u);
            (*_data_points_to_interpolate)[i][1] = cos(u);
            (*_data_points_to_interpolate)[i][2] = u;
        }

        try
        {
            if(!_icc->UpdateDataForInterpolation(*_knot_vector, *_data_points_to_interpolate))
            {
                throw("Could not update data for interpolation!");
            }

            if (!_icc->UpdateVertexBufferObjectsOfData(usage_flag))
            {
                throw("Can not update Vertex Buffer Objects of data!");
            }

            _image_of_icc = _icc->GenerateImage(n,100,usage_flag);
            if (!_image_of_icc)
            {
                throw("Could not create image of cyclic curve!");
            }

            if (!_image_of_icc->UpdateVertexBufferObjects(1.0, usage_flag))
            {
                throw("Could not create vertex buffer object of the cyclic curve!");
            }
        }
        catch (Exception &e)
        {
            cout << e << "\n";
        }

        emit xCoordinateChanged((*_cc)[0][0]);
        emit yCoordinateChanged((*_cc)[0][1]);
        emit zCoordinateChanged((*_cc)[0][2]);
    }

    void GLWidget::paintInterpolatingCyclicCurve()
    {
        GLenum render_mode = GL_LINE_LOOP;

        glPointSize(10);
        glColor3f(0.901, 0.494, 0.133);

        glBegin(GL_POINTS);

            for (GLuint i = 0; i < _data_points_to_interpolate->GetRowCount(); i++)
            {
                glVertex3dv(&(*_data_points_to_interpolate)[i][0]);
            }
        glEnd();

        glPointSize(1);

        if (_icc)
        {
            glColor3f(0.901, 0.494, 0.133);
            if (_control_cc)
            {
                _icc->RenderData(render_mode);
            }
        }
        if (_image_of_icc)
        {
            glColor3f(1.0, 0.0, 0.0);
            _image_of_icc->RenderDerivatives(0,GL_LINE_LOOP);

            if (_fd_cc)
            {
                glPointSize(5.0);
                    glColor3f(0.0,0.2,0.0);
                    _image_of_icc->RenderDerivatives(1, GL_LINES);
                    _image_of_icc->RenderDerivatives(1, GL_POINTS);
                glPointSize(1.0);
            }

            if (_sd_cc)
            {
                glPointSize(5.0);
                    glColor3f(0.1,0.2,0.4);
                    _image_of_icc->RenderDerivatives(2, GL_LINES);
                    _image_of_icc->RenderDerivatives(2, GL_POINTS);
                glPointSize(1.0);
            }
        }
    }

    void GLWidget::initializeModel()
    {

        // initial value for model
        _model_index = 0;

        _models.ResizeColumns(3);

        if (_models[0].LoadFromOFF("models/mouse.off", true))
        {
            if (_models[0].UpdateVertexBufferObjects(GL_DYNAMIC_DRAW))
            {
                _angle = 0.0;
                _timer->start();
            }
        }
        if (_models[1].LoadFromOFF("models/elephant.off", true))
        {
            if (_models[1].UpdateVertexBufferObjects(GL_DYNAMIC_DRAW))
            {
                _angle = 0.0;
                _timer->start();
            }
        }
        if (_models[2].LoadFromOFF("models/sphere.off", true))
        {
            if (_models[2].UpdateVertexBufferObjects(GL_DYNAMIC_DRAW))
            {
                _angle = 0.0;
                _timer->start();
            }
        }
    }

    void GLWidget::paintModel()
    {
        glEnable(GL_LIGHTING);
        glEnable(GL_NORMALIZE);
        glEnable(GL_LIGHT0);

        applyMaterial();
        enableShader();

        _models[_model_index].Render();

        disableShader();

        glDisable(GL_LIGHTING);
        glDisable(GL_NORMALIZE);
        glDisable(GL_LIGHT0);
    }

    void GLWidget::initializeParametricSurfaces()
    {
       _ps.ResizeColumns(6);
       _img_ps.ResizeColumns(6);

       _ps_index = 0;

       TriangularMatrix<ParametricSurface3::PartialDerivative>derivativeSurface(3);

       derivativeSurface(0,0) = Torus::d00;
       derivativeSurface(1,0) = Torus::d10;
       derivativeSurface(1,1) = Torus::d01;
       _ps[0] = new (nothrow) ParametricSurface3(derivativeSurface, Torus::u_min, Torus::u_max, Torus::v_min, Torus::v_max);

       derivativeSurface(0,0) = Hyperboloid::d00;
       derivativeSurface(1,0) = Hyperboloid::d10;
       derivativeSurface(1,1) = Hyperboloid::d01;
       _ps[1] = new (nothrow) ParametricSurface3(derivativeSurface, Hyperboloid::u_min, Hyperboloid::u_max, Hyperboloid::v_min, Hyperboloid::v_max);

       derivativeSurface(0,0) = Cylindrical_helicoid::d00;
       derivativeSurface(1,0) = Cylindrical_helicoid::d10;
       derivativeSurface(1,1) = Cylindrical_helicoid::d01;
       _ps[2] = new (nothrow) ParametricSurface3(derivativeSurface,  Cylindrical_helicoid::u_min, Cylindrical_helicoid::u_max, Cylindrical_helicoid::v_min, Cylindrical_helicoid::v_max);

       derivativeSurface(0,0) = Alfred_Gray::d00;
       derivativeSurface(1,0) = Alfred_Gray::d10;
       derivativeSurface(1,1) = Alfred_Gray::d01;
       _ps[3] = new (nothrow) ParametricSurface3(derivativeSurface,  Alfred_Gray::u_min, Alfred_Gray::u_max, Alfred_Gray::v_min, Alfred_Gray::v_max);

       derivativeSurface(0,0) = KleinBottle::d00;
       derivativeSurface(1,0) = KleinBottle::d10;
       derivativeSurface(1,1) = KleinBottle::d01;
       _ps[4] = new (nothrow) ParametricSurface3(derivativeSurface, KleinBottle::u_min, KleinBottle::u_max, KleinBottle::v_min, KleinBottle::v_max);

       derivativeSurface(0,0) = Dini::d00;
       derivativeSurface(1,0) = Dini::d10;
       derivativeSurface(1,1) = Dini::d01;
       _ps[5] = new (nothrow) ParametricSurface3(derivativeSurface, Dini::u_min, Dini::u_max, Dini::v_min, Dini::v_max);

       GLuint div_point_count_u = 500;
       GLuint div_point_count_v = 500;
       GLenum usage_flag = GL_STATIC_DRAW;

       for (GLuint i = 0; i < 6; i++) {
           _img_ps[i] = _ps[i]->GenerateImage(div_point_count_u,
                                              div_point_count_v,
                                              usage_flag);

           if (!_img_ps[i]) {
               throw Exception("Could not create image of parametric surface!");
           }

           if (!_img_ps[i]->UpdateVertexBufferObjects(usage_flag))
           {
              throw Exception("Could not create the vertex buffer object of the parametric surface!");
           }
       }
    }

    void GLWidget::paintParametricSurfaces()
    {
        if (_img_ps[_ps_index])
        {

            glEnable(GL_LIGHTING);
            glEnable(GL_NORMALIZE);
            glEnable(GL_LIGHT0);

            applyMaterial2();
            enableShader2();

            _img_ps[_ps_index]->Render();

            disableShader2();

            glDisable(GL_LIGHTING);
            glDisable(GL_NORMALIZE);
            glDisable(GL_LIGHT0);
        }
    }

    void GLWidget::initializeShaders()
    {
        _shaders.ResizeColumns(4);

        _shaders[0] = new ShaderProgram();
        _shaders[0]->InstallShaders("shaders/directional_light.vert","shaders/directional_light.frag");

        _shaders[1] = new ShaderProgram();
        _shaders[1]->InstallShaders("shaders/reflection_lines.vert","shaders/reflection_lines.frag");
        _shaders[1]->Enable();
        _shaders[1]->SetUniformVariable1f("scale_factor", 4.0);
        _scale_factor = 4.0;
        _shaders[1]->SetUniformVariable1f("smoothing", 2.0);
        _scale_factor = 2.0;
        _shaders[1]->SetUniformVariable1f("shading", 1.0);
        _scale_factor = 1.0;
        _shaders[1]->Disable();

        _shaders[2] = new ShaderProgram();
        _shaders[2]->InstallShaders("shaders/toon.vert","shaders/toon.frag");
        _shaders[2]->Enable();
        _shaders[2]->SetUniformVariable4f("default_outline_color", 120.0, 150.0, 170.0, 0.7);
        _outline.r() = 120.0;
        _outline.g() = 150.0;
        _outline.b() = 170.0;
        _outline.a() = 0.7;
        _shaders[2]->Disable();

        _shaders[3] = new ShaderProgram();
        _shaders[3]->InstallShaders("shaders/two_sided_lighting.vert","shaders/two_sided_lighting.frag");

        _shader_index = 0;
        _shader_enabled = true;

        _shaders2.ResizeColumns(4);

        _shaders2[0] = new ShaderProgram();
        _shaders2[0]->InstallShaders("shaders/directional_light.vert","shaders/directional_light.frag");

        _shaders2[1] = new ShaderProgram();
        _shaders2[1]->InstallShaders("shaders/reflection_lines.vert","shaders/reflection_lines.frag");
        _shaders2[1]->Enable();
        _shaders2[1]->SetUniformVariable1f("scale_factor", 4.0);
        _scale_factor2 = 4.0;
        _shaders2[1]->SetUniformVariable1f("smoothing", 2.0);
        _scale_factor2 = 2.0;
        _shaders2[1]->SetUniformVariable1f("shading", 1.0);
        _scale_factor2 = 1.0;
        _shaders2[1]->Disable();

        _shaders2[2] = new ShaderProgram();
        _shaders2[2]->InstallShaders("shaders/toon.vert","shaders/toon.frag");
        _shaders2[2]->Enable();
        _shaders2[2]->SetUniformVariable4f("default_outline_color", 120.0, 150.0, 170.0, 0.7);
        _outline2.r() = 120.0;
        _outline2.g() = 150.0;
        _outline2.b() = 170.0;
        _outline2.a() = 0.7;
        _shaders2[2]->Disable();

        _shaders2[3] = new ShaderProgram();
        _shaders2[3]->InstallShaders("shaders/two_sided_lighting.vert","shaders/two_sided_lighting.frag");

        _shader_index2 = 0;
        _shader_enabled2 = true;
    }

    void GLWidget::initializeMaterials()
    {
        _materials.ResizeColumns(7);
        _materials[0] = (&MatFBBrass);
        _materials[1] = (&MatFBGold);
        _materials[2] = (&MatFBSilver);
        _materials[3] = (&MatFBEmerald);
        _materials[4] = (&MatFBPearl);
        _materials[5] = (&MatFBRuby);
        _materials[6] = (&MatFBTurquoise);

        _material_index = 0;
        _material_enabled = true;

        _materials2.ResizeColumns(7);
        _materials2[0] = (&MatFBBrass);
        _materials2[1] = (&MatFBGold);
        _materials2[2] = (&MatFBSilver);
        _materials2[3] = (&MatFBEmerald);
        _materials2[4] = (&MatFBPearl);
        _materials2[5] = (&MatFBRuby);
        _materials2[6] = (&MatFBTurquoise);

        _material_index2 = 0;
        _material_enabled2 = true;
    }

    void GLWidget::buildFirstOrderAlgebraicTrigonometricArc()
    {

        // initial value for control point
        _cp_index_arc = 0;

        _arc = new (nothrow) FirstOrderAlgebraicTrigonometricArc3(1.0);
        _arc->SetData(0, -4.0, -4.0, 1.0);
        _arc->SetData(1, -3.0, -2.0, -2.0);
        _arc->SetData(2, -5.0, 1.0, 3.0);
        _arc->SetData(3, -3.0, 5.0, 3.0);

        _act_cp_arc_x = _arc->GetData(_cp_index_arc)[0];
        _act_cp_arc_y = _arc->GetData(_cp_index_arc)[1];
        _act_cp_arc_z = _arc->GetData(_cp_index_arc)[2];

        emit xCoordinateArcChanged(_arc->GetData(_cp_index_arc)[0]);
        emit yCoordinateArcChanged(_arc->GetData(_cp_index_arc)[1]);
        emit zCoordinateArcChanged(_arc->GetData(_cp_index_arc)[2]);

        if (!_arc->UpdateVertexBufferObjectsOfData())
        {
            throw Exception("Could not create arc!\n");
        }

        _image_of_arc = _arc->GenerateImage(2,200);

        if (!_image_of_arc)
        {
            throw Exception ("Could not create image of arc!\n");
        }

        if (!_image_of_arc->UpdateVertexBufferObjects())
        {
            throw Exception ("Could not create vertex buffer object of arc!\n");
        }
    }

    void GLWidget::renderFirstOrderAlgebraicTrigonometricArc()
    {
        if(_arc)
        {
            if (_c_arc)
            {
                glColor3f(0.82745098, 0.329411765, 0);
                _arc->RenderData(GL_LINE_STRIP);
            }
        }

        if (_image_of_arc)
        {
            glColor3f(0.180392157, 0.8, 0.443137255);
            _image_of_arc->RenderDerivatives(0, GL_LINE_STRIP);

            if (_fd_arc)
            {
                glPointSize(5.0);
                    glColor3f(0.0,0.2,0.0);
                    _image_of_arc->RenderDerivatives(1, GL_LINES);
                    _image_of_arc->RenderDerivatives(1, GL_POINTS);
                glPointSize(1.0);
            }

            if (_sd_arc)
            {
                glPointSize(5.0);
                    glColor3f(0.1,0.2,0.4);
                    _image_of_arc->RenderDerivatives(2, GL_LINES);
                    _image_of_arc->RenderDerivatives(2, GL_POINTS);
                glPointSize(1.0);
            }
        }
    }

    void GLWidget::buildFirstOrderAlgebraicTrigonometricPatch()
    {
        _before_int = GL_TRUE;
        _after_int = GL_TRUE;
        _u_lin = GL_TRUE;
        _v_lin = GL_TRUE;

        // initial value for control point
        _cp_index_u = 0;
        _cp_index_v = 0;

        _patch = new (nothrow) FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
        _patch->SetData(0, 0, -3.0, -4.0, 0.0);
        _patch->SetData(0, 1, -3.0, -1.0, 0.0);
        _patch->SetData(0, 2, -3.0, 1.0, 0.0);
        _patch->SetData(0, 3, -3.0, 4.0, 0.0);

        _patch->SetData(1, 0, -1.0, -4.0, 0.0);
        _patch->SetData(1, 1, -1.0, -1.0, 2.0);
        _patch->SetData(1, 2, -1.0, 1.0, 2.0);
        _patch->SetData(1, 3, -1.0, 3.0, 0.0);

        _patch->SetData(2, 0, 1.0, -4.0, 0.0);
        _patch->SetData(2, 1, 1.0, -1.0, 2.0);
        _patch->SetData(2, 2, 1.0, 1.0, 2.0);
        _patch->SetData(2, 3, 1.0, 3.0, 0.0);

        _patch->SetData(3, 0, 3.0, -4.0, 0.0);
        _patch->SetData(3, 1, 3.0, -1.0, 0.0);
        _patch->SetData(3, 2, 3.0, 1.0, 0.0);
        _patch->SetData(3, 3, 3.0, 4.0, 0.0);

        _patch->GetData(_cp_index_u, _cp_index_v, _act_cp_patch_x, _act_cp_patch_y, _act_cp_patch_z);

        emit xCoordinatePatchChanged(_act_cp_patch_x);
        emit yCoordinatePatchChanged(_act_cp_patch_y);
        emit zCoordinatePatchChanged(_act_cp_patch_z);

        if (! _patch->UpdateVertexBufferObjectsOfData())
        {
            throw Exception("Could not create patch!\n");
        }

        _before_interpolation = nullptr;
        _after_interpolation = nullptr;
        _u_lines = nullptr;
        _v_lines = nullptr;

        _before_int = true;
        _after_int = true;
        _u_lin = true;
        _v_lin = true;

        _u_lines = _patch->GenerateUIsoparametricLines(5, 1, 100);
        _v_lines = _patch->GenerateVIsoparametricLines(5, 1, 100);

        for(GLuint i = 0; i < 5; i++)
        {
            (*_u_lines)[i]->UpdateVertexBufferObjects();
            (*_v_lines)[i]->UpdateVertexBufferObjects();
        }

        _before_interpolation = _patch->GenerateImage(30, 30, GL_STATIC_DRAW);

        if(_before_interpolation)
            _before_interpolation->UpdateVertexBufferObjects();

        RowMatrix<GLdouble> u_knot_vector(4);
        u_knot_vector(0) = 0.0;
        u_knot_vector(1) = 1.0 / 3.0;
        u_knot_vector(2) = 2.0 / 3.0;
        u_knot_vector(3) = 1.0;

        ColumnMatrix<GLdouble> v_knot_vector(4);
        v_knot_vector(0) = 0.0;
        v_knot_vector(1) = 1.0 / 3.0;
        v_knot_vector(2) = 2.0 / 3.0;
        v_knot_vector(3) = 1.0;

        Matrix<DCoordinate3> data_points_to_interpolate(4, 4);
        for (int row = 0; row < 4; ++row)
            for (int column = 0; column < 4; ++column)
                _patch->GetData(row, column, data_points_to_interpolate(row, column));

        if(_patch->UpdateDataForInterpolation(u_knot_vector, v_knot_vector, data_points_to_interpolate))
        {
            _after_interpolation = _patch->GenerateImage(30, 30, GL_STATIC_DRAW);
            if(_after_interpolation)
                _after_interpolation->UpdateVertexBufferObjects();
        }
    }

    void GLWidget::renderFirstOrderAlgebraicTrigonometricPatch()
    {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_NORMALIZE);

        if(_patch)
        {
            _patch->RenderData(GL_LINE_STRIP);
        }

        if (_before_int)
        {
            if(_before_interpolation)
            {
                MatFBBrass.Apply();
                _before_interpolation->Render();
            }

        }
        if (_after_int)
        {
            if(_after_interpolation)
            {
                glEnable(GL_BLEND);

                glDepthMask(GL_FALSE);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                    MatFBEmerald.Apply();
                    _after_interpolation->Render();

                glDepthMask(GL_TRUE);
                glDisable(GL_BLEND);
            }
            glDisable(GL_LIGHTING);
        }

        if (_u_lin)
        {
            for(GLuint i = 0; i < 5; i++)
            {
                (*_u_lines)[i]->RenderDerivatives(0, GL_LINE_STRIP);
            }
        }

        if (_v_lin)
        {
            for(GLuint i = 0; i < 5; i++)
            {
                (*_v_lines)[i]->RenderDerivatives(0, GL_LINE_STRIP);
            }
        }
    }


    //-----------------------------------
    // implementation of the public slots
    //-----------------------------------
    void GLWidget::set_angle_x(int value)
    {
        if (_angle_x != value)
        {
            _angle_x = value;
            updateGL();
        }
    }

    void GLWidget::set_angle_y(int value)
    {
        if (_angle_y != value)
        {
            _angle_y = value;
            updateGL();
        }
    }

    void GLWidget::set_angle_z(int value)
    {
        if (_angle_z != value)
        {
            _angle_z = value;
            updateGL();
        }
    }

    void GLWidget::set_zoom_factor(double value)
    {
        if (_zoom != value)
        {
            _zoom = value;
            updateGL();
        }
    }

    void GLWidget::set_trans_x(double value)
    {
        if (_trans_x != value)
        {
            _trans_x = value;
            updateGL();
        }
    }

    void GLWidget::set_trans_y(double value)
    {
        if (_trans_y != value)
        {
            _trans_y = value;
            updateGL();
        }
    }

    void GLWidget::set_trans_z(double value)
    {
        if (_trans_z != value)
        {
            _trans_z = value;
            updateGL();
        }
    }

    void GLWidget::setTabIndex(int index)
    {
        if (_tab_index != index)
        {
            _tab_index = index;
            updateGL();
        }
    }

    void GLWidget::setParametricCurveIndex(int index)
    {
        if (index != _pc_index)
        {
            _pc_index = index;
            updateGL();
        }
    }

    void GLWidget::toggleFirstOrderDerivativesParametric(bool b)
    {
        if (_fd_p != b)
        {
            _fd_p = b;
            updateGL();
        }
    }

    void GLWidget::toggleSecondOrderDerivativesParametric(bool b)
    {
        if (_sd_p != b)
        {
            _sd_p = b;
            updateGL();
        }
    }

    void GLWidget::setCyclicCurveIndex(int index)
    {
        if(index != _cc_index)
        {
            _cc_index = index;
            updateGL();
        }
    }

    void GLWidget::toggleFirstOrderDerivativesCyclic(bool b)
    {
        if (_fd_cc != b)
        {
            _fd_cc = b;
            updateGL();
        }
    }

    void GLWidget::toggleSecondOrderDerivativesCyclic(bool b)
    {
        if (_sd_cc != b)
        {
            _sd_cc = b;
            updateGL();
        }
    }

    void GLWidget::toggleControlPolygonCyclic(bool b)
    {
        if (_control_cc != b)
        {
            _control_cc = b;
            updateGL();
        }
    }

    void GLWidget::setControlPointIndex(int index)
    {
        if (index != _cp_index)
        {
            _cp_index = index;
            emit xCoordinateChanged((*_data_points_to_interpolate)[_cp_index][0]);
            emit yCoordinateChanged((*_data_points_to_interpolate)[_cp_index][1]);
            emit zCoordinateChanged((*_data_points_to_interpolate)[_cp_index][2]);
        }
    }

    void GLWidget::setControlPointX(double value)
    {
        if((*_data_points_to_interpolate)[_cp_index][0] != value)
        {
            GLenum usage_flag = GL_STATIC_DRAW;

            (*_data_points_to_interpolate)[_cp_index][0] = value;
            try
            {
                if(!_icc->UpdateDataForInterpolation(*_knot_vector, *_data_points_to_interpolate))
                {
                    throw("Could not update data for interpolation!");
                }

                if (!_icc->UpdateVertexBufferObjectsOfData(usage_flag))
                {
                    throw("Can not update Vertex Buffer Objects of data!");
                }

                _image_of_icc = _icc->GenerateImage(_data_points_to_interpolate->GetRowCount(),100,usage_flag);
                if (!_image_of_icc)
                {
                    throw("Could not create image of cyclic curve!");
                }

                if (!_image_of_icc->UpdateVertexBufferObjects(1.0, usage_flag))
                {
                    throw("Could not create vertex buffer object of the cyclic curve!");
                }
            }
            catch (Exception &e)
            {
                cout << e << "\n";
            }
            updateGL();
        }
    }

    void GLWidget::setControlPointY(double value)
    {
        if((*_data_points_to_interpolate)[_cp_index][1] != value)
        {
            GLenum usage_flag = GL_STATIC_DRAW;
            (*_data_points_to_interpolate)[_cp_index][1] = value;
            try
            {
                if(!_icc->UpdateDataForInterpolation(*_knot_vector, *_data_points_to_interpolate))
                {
                    throw("Could not update data for interpolation!");
                }

                if (!_icc->UpdateVertexBufferObjectsOfData(usage_flag))
                {
                    throw("Can not update Vertex Buffer Objects of data!");
                }

                _image_of_icc = _icc->GenerateImage(_data_points_to_interpolate->GetRowCount(),100,usage_flag);
                if (!_image_of_icc)
                {
                    throw("Could not create image of cyclic curve!");
                }

                if (!_image_of_icc->UpdateVertexBufferObjects(1.0, usage_flag))
                {
                    throw("Could not create vertex buffer object of the cyclic curve!");
                }
            }
            catch (Exception &e)
            {
                cout << e << "\n";
            }
            updateGL();
        }
    }

    void GLWidget::setControlPointZ(double value)
    {
        if((*_data_points_to_interpolate)[_cp_index][2] != value)
        {
            GLenum usage_flag = GL_STATIC_DRAW;
            (*_data_points_to_interpolate)[_cp_index][2] = value;
            try
            {
                if(!_icc->UpdateDataForInterpolation(*_knot_vector, *_data_points_to_interpolate))
                {
                    throw("Could not update data for interpolation!");
                }

                if (!_icc->UpdateVertexBufferObjectsOfData(usage_flag))
                {
                    throw("Can not update Vertex Buffer Objects of data!");
                }

                if (_image_of_icc)
                {
                    delete _image_of_icc;
                }

                _image_of_icc = _icc->GenerateImage(_data_points_to_interpolate->GetRowCount(),100,usage_flag);
                if (!_image_of_icc)
                {
                    throw("Could not create image of cyclic curve!");
                }

                if (!_image_of_icc->UpdateVertexBufferObjects(1.0, usage_flag))
                {
                    throw("Could not create vertex buffer object of the cyclic curve!");
                }
            }
            catch (Exception &e)
            {
                cout << e << "\n";
            }
            updateGL();
        }
    }

    void GLWidget::setModelIndex(int index)
    {
        if (index != _model_index)
        {
            _model_index = index;
            updateGL();
        }
    }

    void GLWidget::_animate()
    {
        GLfloat *vertex = _models[_model_index].MapVertexBuffer(GL_READ_WRITE);
        GLfloat *normal = _models[_model_index].MapNormalBuffer(GL_READ_ONLY);

        _angle += DEG_TO_RADIAN;
        if (_angle >= TWO_PI) _angle -= TWO_PI;

        GLfloat scale = sin(_angle) / 3000.0;

        for (GLuint i = 0; i < _models[_model_index].VertexCount(); ++i)
        {
            for (GLuint coordinate = 0; coordinate < 3; ++coordinate, ++vertex, ++normal)
                *vertex += scale * (*normal);
        }

        _models[_model_index].UnmapVertexBuffer();
        _models[_model_index].UnmapNormalBuffer();

        updateGL();
    }

    void GLWidget::setParametricSurfaceIndex(int index)
    {
        if (index != _ps_index)
        {
            _ps_index = index;
            updateGL();
        }
    }

    void GLWidget::applyMaterial() {
        if (_material_enabled) {
            if (_materials[_material_index]) {
                _materials[_material_index]->Apply();
            }
        }
    }

    void GLWidget::applyMaterial2() {
        if (_material_enabled2) {
            if (_materials2[_material_index2]) {
                _materials2[_material_index2]->Apply();
            }
        }
    }

    void GLWidget::enableShader() {
        if(_shader_enabled) {
            if (_shaders[_shader_index]) {
                _shaders[_shader_index]->Enable();
            }
        }
    }

    void GLWidget::enableShader2() {
        if(_shader_enabled2) {
            if (_shaders2[_shader_index2]) {
                _shaders2[_shader_index2]->Enable();
            }
        }
    }

    void GLWidget::disableShader() {
        if(_shader_enabled) {
            if (_shaders[_shader_index]) {
                _shaders[_shader_index]->Disable();
            }
        }
    }

    void GLWidget::disableShader2() {
        if(_shader_enabled2) {
            if (_shaders2[_shader_index2]) {
                _shaders2[_shader_index2]->Disable();
            }
        }
    }

    void GLWidget::setShaderIndex(int index)
    {
        if (_shader_index != index)
        {
            _shader_index = index;
            updateGL();
        }
    }

    void GLWidget::setShaderIndex2(int index)
    {
        if (_shader_index2 != index)
        {
            _shader_index2 = index;
            updateGL();
        }
    }

    void GLWidget::setMaterialIndex(int index)
    {
        if (_material_index != index)
        {
            _material_index = index;
            updateGL();
        }
    }

    void GLWidget::setMaterialIndex2(int index)
    {
        if (_material_index2 != index)
        {
            _material_index2 = index;
            updateGL();
        }
    }

    void GLWidget::setScaleFactor(double value)
    {
        if (_scale_factor != value)
        {
            _shaders[1]->Enable();
            _shaders[1]->SetUniformVariable1f("scale_factor", value);
            _shaders[1]->Disable();
            _scale_factor = value;
            updateGL();
        }
    }

    void GLWidget::setScaleFactor2(double value)
    {
        if (_scale_factor2 != value)
        {
            _shaders2[1]->Enable();
            _shaders2[1]->SetUniformVariable1f("scale_factor", value);
            _shaders2[1]->Disable();
            _scale_factor2 = value;
            updateGL();
        }
    }

    void GLWidget::setSmoothing(double value)
    {
        if (_smoothing != value)
        {
            _shaders[1]->Enable();
            _shaders[1]->SetUniformVariable1f("smoothing", value);
            _shaders[1]->Disable();
            _smoothing = value;
            updateGL();
        }
    }

    void GLWidget::setSmoothing2(double value)
    {
        if (_smoothing2 != value)
        {
            _shaders2[1]->Enable();
            _shaders2[1]->SetUniformVariable1f("smoothing", value);
            _shaders2[1]->Disable();
            _smoothing2 = value;
            updateGL();
        }
    }

    void GLWidget::setShading(double value)
    {
        if (_shading != value)
        {
            _shaders[1]->Enable();
            _shaders[1]->SetUniformVariable1f("shading", value);
            _shaders[1]->Disable();
            _shading = value;
            updateGL();
        }
    }

    void GLWidget::setShading2(double value)
    {
        if (_shading2 != value)
        {
            _shaders2[1]->Enable();
            _shaders2[1]->SetUniformVariable1f("shading", value);
            _shaders2[1]->Disable();
            _shading2 = value;
            updateGL();
        }
    }

    void GLWidget::setOutlineR(double value)
    {
        if (_outline.r() != value)
        {
            _shaders[2]->Enable();
            _shaders[2]->SetUniformVariable4f("default_outline_color", value, _outline.g(), _outline.b(), _outline.a());
            _shaders[2]->Disable();
            _outline.r() = value;
            updateGL();
        }
    }

    void GLWidget::setOutlineG(double value)
    {
        if (_outline.g() != value)
        {
            _shaders[2]->Enable();
            _shaders[2]->SetUniformVariable4f("default_outline_color", _outline.r(), value, _outline.b(), _outline.a());
            _shaders[2]->Disable();
            _outline.g() = value;
            updateGL();
        }
    }

    void GLWidget::setOutlineB(double value)
    {
        if (_outline.b() != value)
        {
            _shaders[2]->Enable();
            _shaders[2]->SetUniformVariable4f("default_outline_color", _outline.r(), _outline.g(), value, _outline.a());
            _shaders[2]->Disable();
            _outline.b() = value;
            updateGL();
        }
    }

    void GLWidget::setOutlineA(double value)
    {
        if (_outline.a() != value)
        {
            _shaders[2]->Enable();
            _shaders[2]->SetUniformVariable4f("default_outline_color", _outline.r(), _outline.g(), _outline.b(), value);
            _shaders[2]->Disable();
            _outline.a() = value;
            updateGL();
        }
    }

    void GLWidget::setOutlineR2(double value)
    {
        if (_outline2.r() != value)
        {
            _shaders2[2]->Enable();
            _shaders2[2]->SetUniformVariable4f("default_outline_color", value, _outline2.g(), _outline2.b(), _outline2.a());
            _shaders2[2]->Disable();
            _outline2.r() = value;
            updateGL();
        }
    }

    void GLWidget::setOutlineG2(double value)
    {
        if (_outline2.g() != value)
        {
            _shaders2[2]->Enable();
            _shaders2[2]->SetUniformVariable4f("default_outline_color", _outline2.r(), value, _outline2.b(), _outline2.a());
            _shaders2[2]->Disable();
            _outline2.g() = value;
            updateGL();
        }
    }

    void GLWidget::setOutlineB2(double value)
    {
        if (_outline2.b() != value)
        {
            _shaders2[2]->Enable();
            _shaders2[2]->SetUniformVariable4f("default_outline_color", _outline2.r(), _outline2.g(), value, _outline2.a());
            _shaders2[2]->Disable();
            _outline2.b() = value;
            updateGL();
        }
    }

    void GLWidget::setOutlineA2(double value)
    {
        if (_outline2.a() != value)
        {
            _shaders2[2]->Enable();
            _shaders2[2]->SetUniformVariable4f("default_outline_color", _outline2.r(), _outline2.g(), _outline2.b(), value);
            _shaders2[2]->Disable();
            _outline2.a() = value;
            updateGL();
        }
    }

    void GLWidget::setBeforeInterpolate(bool value)
    {
        if (_before_int != value)
        {
            _before_int = value;
            updateGL();
        }
    }

    void GLWidget::setAfterInterpolate(bool value)
    {
        if (_after_int != value)
        {
            _after_int = value;
            updateGL();
        }
    }

    void GLWidget::setULines(bool value)
    {
        if (_u_lin != value)
        {
            _u_lin = value;
            updateGL();
        }
    }

    void GLWidget::setVLines(bool value)
    {
        if (_v_lin != value)
        {
            _v_lin = value;
            updateGL();
        }
    }

    void GLWidget::toggleFirstOrderDerivativesArc(bool b)
    {
        if (_fd_arc != b)
        {
            _fd_arc = b;
            updateGL();
        }
    }

    void GLWidget::toggleSecondOrderDerivativesArc(bool b)
    {
        if (_sd_arc != b)
        {
            _sd_arc = b;
            updateGL();
        }
    }

    void GLWidget::toggleControlPolygonArc(bool b)
    {
        if (_c_arc != b)
        {
            _c_arc = b;
            updateGL();
        }
    }

    void GLWidget::setControlPointArcIndex(int index)
    {
        if (_cp_index_arc != index)
        {
            _cp_index_arc = index;

            _act_cp_arc_x = _arc->GetData(_cp_index_arc)[0];
            _act_cp_arc_y = _arc->GetData(_cp_index_arc)[1];
            _act_cp_arc_z = _arc->GetData(_cp_index_arc)[2];

            emit xCoordinateArcChanged(_arc->GetData(_cp_index_arc)[0]);
            emit yCoordinateArcChanged(_arc->GetData(_cp_index_arc)[1]);
            emit zCoordinateArcChanged(_arc->GetData(_cp_index_arc)[2]);
        }
    }

    void GLWidget::setControlPointArcX(double value)
    {
        if (_act_cp_arc_x != value)
        {
            _act_cp_arc_x = value;
            _arc->SetData(_cp_index_arc, _act_cp_arc_x, _act_cp_arc_y, _act_cp_arc_z);

            if (!_arc->UpdateVertexBufferObjectsOfData())
            {
                throw Exception("Could not create arc!\n");
            }

            delete _image_of_arc, _image_of_arc = nullptr;

            _image_of_arc = _arc->GenerateImage(2,200);

            if (!_image_of_arc)
            {
                throw Exception ("Could not create image of arc!\n");
            }

            if (!_image_of_arc->UpdateVertexBufferObjects())
            {
                throw Exception ("Could not create vertex buffer object of arc!\n");
            }

            updateGL();
        }
    }

    void GLWidget::setControlPointArcY(double value)
    {
        if (_act_cp_arc_y != value)
        {
            _act_cp_arc_y = value;
            _arc->SetData(_cp_index_arc, _act_cp_arc_x, _act_cp_arc_y, _act_cp_arc_z);

            if (!_arc->UpdateVertexBufferObjectsOfData())
            {
                throw Exception("Could not create arc!\n");
            }

            delete _image_of_arc, _image_of_arc = nullptr;

            _image_of_arc = _arc->GenerateImage(2,200);

            if (!_image_of_arc)
            {
                throw Exception ("Could not create image of arc!\n");
            }

            if (!_image_of_arc->UpdateVertexBufferObjects())
            {
                throw Exception ("Could not create vertex buffer object of arc!\n");
            }
            updateGL();
        }
    }

    void GLWidget::setControlPointArcZ(double value)
    {
        if (_act_cp_arc_z != value)
        {
            _act_cp_arc_z = value;
            _arc->SetData(_cp_index_arc, _act_cp_arc_x, _act_cp_arc_y, _act_cp_arc_z);

            if (!_arc->UpdateVertexBufferObjectsOfData())
            {
                throw Exception("Could not create arc!\n");
            }

            delete _image_of_arc, _image_of_arc = 0;

            _image_of_arc = _arc->GenerateImage(2,200);

            if (!_image_of_arc)
            {
                throw Exception ("Could not create image of arc!\n");
            }

            if (!_image_of_arc->UpdateVertexBufferObjects())
            {
                throw Exception ("Could not create vertex buffer object of arc!\n");
            }
            updateGL();
        }
    }

    void GLWidget::setAlphaArc(double value)
    {
        if (_arc->getAlpha() != value)
        {
            _arc->setAlpha(value);

            if (!_arc->UpdateVertexBufferObjectsOfData())
            {
                throw Exception("Could not create arc!\n");
            }

            delete _image_of_arc, _image_of_arc = nullptr;

            _image_of_arc = _arc->GenerateImage(2,200);

            if (!_image_of_arc)
            {
                throw Exception ("Could not create image of arc!\n");
            }

            if (!_image_of_arc->UpdateVertexBufferObjects())
            {
                throw Exception ("Could not create vertex buffer object of arc!\n");
            }

            updateGL();
        }
    }

    void GLWidget::setControlPointUPatchIndex(int index)
    {
        if (_cp_index_u != index)
        {
            _cp_index_u = index;

            _patch->GetData(_cp_index_u, _cp_index_v, _act_cp_patch_x, _act_cp_patch_y, _act_cp_patch_z);

            emit xCoordinatePatchChanged(_act_cp_patch_x);
            emit yCoordinatePatchChanged(_act_cp_patch_y);
            emit zCoordinatePatchChanged(_act_cp_patch_z);
        }
    }

    void GLWidget::setControlPointVPatchIndex(int index)
    {
        if (_cp_index_v != index)
        {
            _cp_index_v = index;

            _patch->GetData(_cp_index_u, _cp_index_v, _act_cp_patch_x, _act_cp_patch_y, _act_cp_patch_z);

            emit xCoordinatePatchChanged(_act_cp_patch_x);
            emit yCoordinatePatchChanged(_act_cp_patch_y);
            emit zCoordinatePatchChanged(_act_cp_patch_z);
        }
    }

    void GLWidget::setControlPointPatchX(double value)
    {

        DCoordinate3 aux;

        _patch->GetData(_cp_index_u, _cp_index_v, _act_cp_patch_x, _act_cp_patch_y, _act_cp_patch_z);

        if (_act_cp_patch_x != value)
        {
            _act_cp_patch_x = value;
            _patch->SetData(_cp_index_u, _cp_index_v, _act_cp_patch_x, _act_cp_patch_y, _act_cp_patch_z);

            if (! _patch->UpdateVertexBufferObjectsOfData())
            {
                throw Exception("Could not create patch!\n");
            }

            _before_interpolation = nullptr;
            _after_interpolation = nullptr;
            _u_lines = nullptr;
            _v_lines = nullptr;

            _before_int = true;
            _after_int = true;
            _u_lin = true;
            _v_lin = true;

            _u_lines = _patch->GenerateUIsoparametricLines(5, 1, 100);
            _v_lines = _patch->GenerateVIsoparametricLines(5, 1, 100);

            for(GLuint i = 0; i < 5; i++)
            {
                (*_u_lines)[i]->UpdateVertexBufferObjects();
                (*_v_lines)[i]->UpdateVertexBufferObjects();
            }

            _before_interpolation = _patch->GenerateImage(30, 30, GL_STATIC_DRAW);

            if(_before_interpolation)
                _before_interpolation->UpdateVertexBufferObjects();

            RowMatrix<GLdouble> u_knot_vector(4);
            u_knot_vector(0) = 0.0;
            u_knot_vector(1) = 1.0 / 3.0;
            u_knot_vector(2) = 2.0 / 3.0;
            u_knot_vector(3) = 1.0;

            ColumnMatrix<GLdouble> v_knot_vector(4);
            v_knot_vector(0) = 0.0;
            v_knot_vector(1) = 1.0 / 3.0;
            v_knot_vector(2) = 2.0 / 3.0;
            v_knot_vector(3) = 1.0;

            Matrix<DCoordinate3> data_points_to_interpolate(4, 4);
            for (int row = 0; row < 4; ++row)
                for (int column = 0; column < 4; ++column)
                    _patch->GetData(row, column, data_points_to_interpolate(row, column));

            if(_patch->UpdateDataForInterpolation(u_knot_vector, v_knot_vector, data_points_to_interpolate))
            {
                _after_interpolation = _patch->GenerateImage(30, 30, GL_STATIC_DRAW);
                if(_after_interpolation)
                    _after_interpolation->UpdateVertexBufferObjects();
            }
            updateGL();
        }
    }

    void GLWidget::setControlPointPatchY(double value)
    {
        _patch->GetData(_cp_index_u, _cp_index_v, _act_cp_patch_x, _act_cp_patch_y, _act_cp_patch_z);

        if (_act_cp_patch_y != value)
        {
            _act_cp_patch_y = value;
            _patch->SetData(_cp_index_u, _cp_index_v, _act_cp_patch_x, _act_cp_patch_y, _act_cp_patch_z);

            if (! _patch->UpdateVertexBufferObjectsOfData())
            {
                throw Exception("Could not create patch!\n");
            }

            _before_interpolation = nullptr;
            _after_interpolation = nullptr;
            _u_lines = nullptr;
            _v_lines = nullptr;

            _before_int = true;
            _after_int = true;
            _u_lin = true;
            _v_lin = true;

            _u_lines = _patch->GenerateUIsoparametricLines(5, 1, 100);
            _v_lines = _patch->GenerateVIsoparametricLines(5, 1, 100);

            for(GLuint i = 0; i < 5; i++)
            {
                (*_u_lines)[i]->UpdateVertexBufferObjects();
                (*_v_lines)[i]->UpdateVertexBufferObjects();
            }

            _before_interpolation = _patch->GenerateImage(30, 30, GL_STATIC_DRAW);

            if(_before_interpolation)
                _before_interpolation->UpdateVertexBufferObjects();

            RowMatrix<GLdouble> u_knot_vector(4);
            u_knot_vector(0) = 0.0;
            u_knot_vector(1) = 1.0 / 3.0;
            u_knot_vector(2) = 2.0 / 3.0;
            u_knot_vector(3) = 1.0;

            ColumnMatrix<GLdouble> v_knot_vector(4);
            v_knot_vector(0) = 0.0;
            v_knot_vector(1) = 1.0 / 3.0;
            v_knot_vector(2) = 2.0 / 3.0;
            v_knot_vector(3) = 1.0;

            Matrix<DCoordinate3> data_points_to_interpolate(4, 4);
            for (int row = 0; row < 4; ++row)
                for (int column = 0; column < 4; ++column)
                    _patch->GetData(row, column, data_points_to_interpolate(row, column));

            if(_patch->UpdateDataForInterpolation(u_knot_vector, v_knot_vector, data_points_to_interpolate))
            {
                _after_interpolation = _patch->GenerateImage(30, 30, GL_STATIC_DRAW);
                if(_after_interpolation)
                    _after_interpolation->UpdateVertexBufferObjects();
            }
            updateGL();
        }
    }

    void GLWidget::setControlPointPatchZ(double value)
    {

        _patch->GetData(_cp_index_u, _cp_index_v, _act_cp_patch_x, _act_cp_patch_y, _act_cp_patch_z);

        if (_act_cp_patch_z != value)
        {
            _act_cp_patch_z = value;
            _patch->SetData(_cp_index_u, _cp_index_v, _act_cp_patch_x, _act_cp_patch_y, _act_cp_patch_z);

            if (! _patch->UpdateVertexBufferObjectsOfData())
            {
                throw Exception("Could not create patch!\n");
            }

            _before_interpolation = nullptr;
            _after_interpolation = nullptr;
            _u_lines = nullptr;
            _v_lines = nullptr;

            _before_int = true;
            _after_int = true;
            _u_lin = true;
            _v_lin = true;

            _u_lines = _patch->GenerateUIsoparametricLines(5, 1, 100);
            _v_lines = _patch->GenerateVIsoparametricLines(5, 1, 100);

            for(GLuint i = 0; i < 5; i++)
            {
                (*_u_lines)[i]->UpdateVertexBufferObjects();
                (*_v_lines)[i]->UpdateVertexBufferObjects();
            }

            _before_interpolation = _patch->GenerateImage(30, 30, GL_STATIC_DRAW);

            if(_before_interpolation)
                _before_interpolation->UpdateVertexBufferObjects();

            RowMatrix<GLdouble> u_knot_vector(4);
            u_knot_vector(0) = 0.0;
            u_knot_vector(1) = 1.0 / 3.0;
            u_knot_vector(2) = 2.0 / 3.0;
            u_knot_vector(3) = 1.0;

            ColumnMatrix<GLdouble> v_knot_vector(4);
            v_knot_vector(0) = 0.0;
            v_knot_vector(1) = 1.0 / 3.0;
            v_knot_vector(2) = 2.0 / 3.0;
            v_knot_vector(3) = 1.0;

            Matrix<DCoordinate3> data_points_to_interpolate(4, 4);
            for (int row = 0; row < 4; ++row)
                for (int column = 0; column < 4; ++column)
                    _patch->GetData(row, column, data_points_to_interpolate(row, column));

            if(_patch->UpdateDataForInterpolation(u_knot_vector, v_knot_vector, data_points_to_interpolate))
            {
                _after_interpolation = _patch->GenerateImage(30, 30, GL_STATIC_DRAW);
                if(_after_interpolation)
                    _after_interpolation->UpdateVertexBufferObjects();
            }
            updateGL();
        }
    }

    void GLWidget::setAlphaUPatch(double value)
    {
        if (_patch->getAlphaU() != value)
        {
            _patch->setAlphaU(value);

            if (! _patch->UpdateVertexBufferObjectsOfData())
            {
                throw Exception("Could not create patch!\n");
            }

            _before_interpolation = nullptr;
            _after_interpolation = nullptr;
            _u_lines = nullptr;
            _v_lines = nullptr;

            _before_int = true;
            _after_int = true;
            _u_lin = true;
            _v_lin = true;

            _u_lines = _patch->GenerateUIsoparametricLines(5, 1, 100);
            _v_lines = _patch->GenerateVIsoparametricLines(5, 1, 100);

            for(GLuint i = 0; i < 5; i++)
            {
                (*_u_lines)[i]->UpdateVertexBufferObjects();
                (*_v_lines)[i]->UpdateVertexBufferObjects();
            }

            _before_interpolation = _patch->GenerateImage(30, 30, GL_STATIC_DRAW);

            if(_before_interpolation)
                _before_interpolation->UpdateVertexBufferObjects();

            RowMatrix<GLdouble> u_knot_vector(4);
            u_knot_vector(0) = 0.0;
            u_knot_vector(1) = 1.0 / 3.0;
            u_knot_vector(2) = 2.0 / 3.0;
            u_knot_vector(3) = 1.0;

            ColumnMatrix<GLdouble> v_knot_vector(4);
            v_knot_vector(0) = 0.0;
            v_knot_vector(1) = 1.0 / 3.0;
            v_knot_vector(2) = 2.0 / 3.0;
            v_knot_vector(3) = 1.0;

            Matrix<DCoordinate3> data_points_to_interpolate(4, 4);
            for (int row = 0; row < 4; ++row)
                for (int column = 0; column < 4; ++column)
                    _patch->GetData(row, column, data_points_to_interpolate(row, column));

            if(_patch->UpdateDataForInterpolation(u_knot_vector, v_knot_vector, data_points_to_interpolate))
            {
                _after_interpolation = _patch->GenerateImage(30, 30, GL_STATIC_DRAW);
                if(_after_interpolation)
                    _after_interpolation->UpdateVertexBufferObjects();
            }
            updateGL();
        }
    }

    void GLWidget::setAlphaVPatch(double value)
    {
        if (_patch->getAlphaV() != value)
        {
            _patch->setAlphaV(value);

            if (! _patch->UpdateVertexBufferObjectsOfData())
            {
                throw Exception("Could not create patch!\n");
            }

            _before_interpolation = nullptr;
            _after_interpolation = nullptr;
            _u_lines = nullptr;
            _v_lines = nullptr;

            _before_int = true;
            _after_int = true;
            _u_lin = true;
            _v_lin = true;

            _u_lines = _patch->GenerateUIsoparametricLines(5, 1, 100);
            _v_lines = _patch->GenerateVIsoparametricLines(5, 1, 100);

            for(GLuint i = 0; i < 5; i++)
            {
                (*_u_lines)[i]->UpdateVertexBufferObjects();
                (*_v_lines)[i]->UpdateVertexBufferObjects();
            }

            _before_interpolation = _patch->GenerateImage(30, 30, GL_STATIC_DRAW);

            if(_before_interpolation)
                _before_interpolation->UpdateVertexBufferObjects();

            RowMatrix<GLdouble> u_knot_vector(4);
            u_knot_vector(0) = 0.0;
            u_knot_vector(1) = 1.0 / 3.0;
            u_knot_vector(2) = 2.0 / 3.0;
            u_knot_vector(3) = 1.0;

            ColumnMatrix<GLdouble> v_knot_vector(4);
            v_knot_vector(0) = 0.0;
            v_knot_vector(1) = 1.0 / 3.0;
            v_knot_vector(2) = 2.0 / 3.0;
            v_knot_vector(3) = 1.0;

            Matrix<DCoordinate3> data_points_to_interpolate(4, 4);
            for (int row = 0; row < 4; ++row)
                for (int column = 0; column < 4; ++column)
                    _patch->GetData(row, column, data_points_to_interpolate(row, column));

            if(_patch->UpdateDataForInterpolation(u_knot_vector, v_knot_vector, data_points_to_interpolate))
            {
                _after_interpolation = _patch->GenerateImage(30, 30, GL_STATIC_DRAW);
                if(_after_interpolation)
                    _after_interpolation->UpdateVertexBufferObjects();
            }
            updateGL();
        }
    }


    //-----------
    // Destructor
    //-----------
    GLWidget::~GLWidget()
    {
        if (_data_points_to_interpolate)
        {
            delete _data_points_to_interpolate, _data_points_to_interpolate = nullptr;
        }

        if (_knot_vector)
        {
            delete _knot_vector, _knot_vector = nullptr;
        }

        for (GLuint i = 0; i < 6 ; i++)
        {
            if (_pc[i])
            {
                delete _pc[i], _pc[i] = nullptr;
            }

            if (_image_of_pc[i])
            {
                delete _image_of_pc[i], _image_of_pc[i] = nullptr;
            }
        }
        if (_cc)
        {
            delete _cc;
        }

        if(_image_of_cc)
        {
            delete _image_of_cc, _image_of_cc = nullptr;
        }

        if (_icc)
        {
            delete _icc, _icc = nullptr;

        }

        if (_image_of_icc)
        {
            delete _image_of_icc, _image_of_cc = nullptr;
        }
        for (GLuint i = 0; i < 6 ; i++)
        {
            if (_ps[i])
            {
                delete _ps[i], _ps[i] = nullptr;
            }

            if (_img_ps[i])
            {
                delete _img_ps[i], _img_ps[i] = nullptr;
            }
        }
        for (GLuint i = 0; i < 4 ; i++)
        {
            if (_shaders[i])
            {
                delete _shaders[i], _shaders[i] = nullptr;
            }
            if (_shaders2[i])
            {
                delete _shaders2[i], _shaders2[i] = nullptr;
            }
        }

        if (_patch)
        {
            delete _patch, _patch = nullptr;
        }

        if (_before_interpolation)
        {
            delete _before_interpolation, _before_interpolation = nullptr;
        }


        if (_after_interpolation)
        {
            delete _after_interpolation, _after_interpolation = nullptr;
        }

        if (_u_lines)
        {
            delete _u_lines, _u_lines = nullptr;
        }

        if (_v_lines)
        {
            delete _v_lines, _v_lines = nullptr;
        }

        if (_arc)
        {
            delete _arc, _arc = nullptr;
        }

        if (_image_of_arc)
        {
            delete _image_of_arc, _image_of_arc = nullptr;
        }
    }
}
