#include "FirstOrderAlgebraicTrigonometricCompositeCurve3.h"
#include "Core/Exceptions.h"

using namespace cagd;

FirstOrderAlgebraicTrigonometricCompositeCurve3::ArcAttributes::ArcAttributes()
{
    this->_arc = new FirstOrderAlgebraicTrigonometricArc3(1.0);
    this->_image = nullptr;
    this->_color = nullptr;
    this->_next = nullptr;
    this->_previous = nullptr;
}

FirstOrderAlgebraicTrigonometricCompositeCurve3::ArcAttributes::~ArcAttributes()
{
    delete this->_arc;
    delete this->_image;
    delete this->_color;
}

FirstOrderAlgebraicTrigonometricCompositeCurve3::ArcAttributes::ArcAttributes(
    const ArcAttributes &arc_atr) :
        _arc(new FirstOrderAlgebraicTrigonometricArc3(*arc_atr._arc)),
        _image(new GenericCurve3(*arc_atr._image)),
        _color(new Color4(*arc_atr._color)),
        _next(arc_atr._next),
        _previous(arc_atr._previous)
{
}

FirstOrderAlgebraicTrigonometricCompositeCurve3::ArcAttributes& FirstOrderAlgebraicTrigonometricCompositeCurve3::ArcAttributes::operator=(const ArcAttributes &arc_atr)
{
    if(this == &arc_atr)
    {
        return *this;
    }
    if(this->_arc)
    {
        delete this->_arc;
    }
    if(this->_color)
    {
        delete this->_color;
    }

    this->_arc = new FirstOrderAlgebraicTrigonometricArc3(*arc_atr._arc);
    this->_image = new GenericCurve3(*arc_atr._image);
    this->_color = new Color4(*arc_atr._color);

    return *this;
}

FirstOrderAlgebraicTrigonometricCompositeCurve3::FirstOrderAlgebraicTrigonometricCompositeCurve3(GLdouble alpha, GLuint max_arc_count)
{
    _attributes.reserve(max_arc_count);
    _alpha = alpha;
    _mod = 5;
    _div_point_count = 200;
}

FirstOrderAlgebraicTrigonometricCompositeCurve3::~FirstOrderAlgebraicTrigonometricCompositeCurve3()
{
    _attributes.clear();
}

GLboolean FirstOrderAlgebraicTrigonometricCompositeCurve3::insertIsolatedArc(FirstOrderAlgebraicTrigonometricArc3 &trigArc)
{
    GLuint size = _attributes.size();
    ArcAttributes *old = &_attributes[0];
    _attributes.resize(size + 1);
    validateAttributes(old, &_attributes[0]);
    ArcAttributes &newArc = _attributes[size];
    newArc._arc = &trigArc;
    UpdateArcVBOGenerateImage(newArc);

    return GL_TRUE;
}

GLvoid FirstOrderAlgebraicTrigonometricCompositeCurve3::validateAttributes(ArcAttributes *oldAttr, ArcAttributes *newAttr)
{
    if(oldAttr != newAttr)
    {
        for(GLuint i = 0; i < _attributes.size(); i++)
        {
            if(_attributes[i]._next)
               _attributes[i]._next = _attributes[i]._next - oldAttr + newAttr;
            if(_attributes[i]._previous)
                _attributes[i]._previous = _attributes[i]._previous - oldAttr + newAttr;
        }
    }
}

GLvoid FirstOrderAlgebraicTrigonometricCompositeCurve3::UpdateArcVBOGenerateImage(ArcAttributes &arc_attr)
{
    if (!arc_attr._arc->UpdateVertexBufferObjectsOfData()) {
        throw Exception("Could not update the vertex buffer objects of the control polygon in FirstOrderAlgebraicTrigonometricCompositeCurves!");
    }

    arc_attr._image = arc_attr._arc->GenerateImage(_mod, _div_point_count);
    if (!arc_attr._image) {
        throw Exception("Could not generate the image of arc in FirstOrderAlgebraicTrigonometricCompositeCurves!");
    }

    if (!arc_attr._image->UpdateVertexBufferObjects()) {
        throw Exception("Could not update the VBOS arc of image in FirstOrderAlgebraicTrigonometricCompositeCurves");
    }
}

GLboolean FirstOrderAlgebraicTrigonometricCompositeCurve3::continueExistingArc(GLuint index, Direction direction)
{
    ArcAttributes &arc = _attributes[index];
       if(direction == RIGHT)
       {
           if(arc._next != nullptr)
               return GL_FALSE;

           GLuint size = _attributes.size();
           ArcAttributes *oldAddrs = &_attributes[0];
           _attributes.resize(size + 1);
           validatePointersInArcAttr(oldAddrs, &_attributes[0]);
           ArcAttributes &added =  _attributes[size];

           added._arc = new FirstOrderAlgebraicTrigonometricArc3(1.0);
           (*added._arc)[0] = (*arc._arc)[3];
           (*added._arc)[1] = 2*(*arc._arc)[3] - (*arc._arc)[2];
           (*added._arc)[2] = 3*(*arc._arc)[3] - 2*(*arc._arc)[2];
           (*added._arc)[3] = 4*(*arc._arc)[3] - 3*(*arc._arc)[2];

           arc._next = &added;
           added._previous = &arc;
           UpdateArcVBOGenerateImage(added);
       } else {
           if(arc._previous != nullptr)
               return GL_FALSE;

           GLuint size = _attributes.size();
           ArcAttributes *oldAddrs = &_attributes[0];
           _attributes.resize(size + 1);
           validateAttributes(oldAddrs,&_attributes[0]);
           ArcAttributes &added =  _attributes[size];

           added._arc = new FirstOrderAlgebraicTrigonometricArc3(_alpha);
           (*added._arc)[0] = 4*(*arc._arc)[0] - 3*(*arc._arc)[1];
           (*added._arc)[1] = 3*(*arc._arc)[0] - 2*(*arc._arc)[1];
           (*added._arc)[2] = 2*(*arc._arc)[0] - (*arc._arc)[1];
           (*added._arc)[3] = (*arc._arc)[0];

           arc._previous = &added;
           added._next = &arc;
           UpdateArcVBOGenerateImage(added);
       }
    return GL_TRUE;
}

GLboolean FirstOrderAlgebraicTrigonometricCompositeCurve3::joinExistingArc(GLuint index1, GLuint index2, Direction direction1, Direction direction2)
{

    ArcAttributes &arc1 = _attributes[index1];
    ArcAttributes &arc2 = _attributes[index2];

    if(direction1 == RIGHT && direction2 == LEFT)
    {

        if(arc1._next != nullptr)
            return GL_FALSE;

        if(arc2._previous != nullptr)
            return GL_TRUE;

        GLuint size = _attributes.size();

        ArcAttributes *oldAddrs = &_attributes[0];
        _attributes.resize(size + 1);
        validatePointersInArcAttr(oldAddrs, &_attributes[0]);

        ArcAttributes &joiner = _attributes[size];

        (*joiner._arc) = FirstOrderAlgebraicTrigonometricArc3(1.0);
        (*joiner._arc)[0] = (*arc1._arc)[3];
        (*joiner._arc)[1] = 2*(*arc1._arc)[3] - (*arc1._arc)[2];
        (*joiner._arc)[2] = 2*(*arc2._arc)[0] - (*arc2._arc)[1];
        (*joiner._arc)[3] = (*arc2._arc)[0];

        arc1._next = &joiner;
        arc2._previous = &joiner;
        joiner._next = &arc2;
        joiner._previous = &arc1;
        UpdateArcVBOGenerateImage(joiner);
    }
    return GL_TRUE;
}

GLboolean FirstOrderAlgebraicTrigonometricCompositeCurve3::mergeExistingArc(GLuint index1, GLuint index2, Direction direction1, Direction direction2)
{
    ArcAttributes &arc1 = _attributes[index1];
    ArcAttributes &arc2 = _attributes[index2];

    if(direction1 == RIGHT && direction2 == LEFT)
    {
        if(arc1._next != nullptr)
            return GL_FALSE;
        if(arc2._previous != nullptr)
            return GL_FALSE;

            DCoordinate3 mid = ((*arc1._arc)[2]+(*arc2._arc)[1])/2;

            (*arc1._arc)[3] = mid;
            (*arc2._arc)[0] = mid;

            arc1._next = &arc2;
            arc2._previous = &arc1;

    } else if(direction1 == LEFT && direction2 == RIGHT){

        if(arc1._previous != nullptr)
            return GL_FALSE;
         if(arc2._next != nullptr)
            return GL_FALSE;

         DCoordinate3 mid = ((*arc1._arc)[1]+(*arc2._arc)[2])/2;

         (*arc1._arc)[0] = mid;
         (*arc2._arc)[3] = mid;

         arc2._next = &arc1;
         arc1._previous = &arc2;
    }
    UpdateArcVBOGenerateImage(arc1);
    UpdateArcVBOGenerateImage(arc2);

    return GL_TRUE;
}

GLboolean FirstOrderAlgebraicTrigonometricCompositeCurve3::eraseExistingArc(GLuint index)
{
    ArcAttributes *oldAttr = &_attributes[0];
    ArcAttributes &tmp = _attributes[index];
    if(tmp._next != nullptr) {
        tmp._next->_previous = nullptr;
    }
    if(tmp._previous->_next != nullptr)
    {
        tmp._previous->_next = nullptr;
    }
    _attributes.erase(_attributes.begin() + index);

    for(GLuint i = 0; i < _attributes.size(); i++)
    {
        if(_attributes[i]._next > &tmp)
            _attributes[i]._next--;
        if(_attributes[i]._previous > &tmp)
            _attributes[i]._previous--;
    }
    validatePointersInArcAttr(oldAttr, &_attributes[0]);

    return GL_TRUE;
}

GLvoid FirstOrderAlgebraicTrigonometricCompositeCurve3::validatePointersInArcAttr(ArcAttributes *oldAttr, ArcAttributes *newAttr)
{
    if(oldAttr != newAttr)
    {
        for(GLuint i = 0; i < _attributes.size(); i++)
        {
            if(_attributes[i]._next)
               _attributes[i]._next = _attributes[i]._next - oldAttr + newAttr;
            if(_attributes[i]._previous)
                _attributes[i]._previous = _attributes[i]._previous - oldAttr + newAttr;
        }
    }
}

GLvoid FirstOrderAlgebraicTrigonometricCompositeCurve3::renderControlPolygon()
{
    for(GLuint i = 0; i < _attributes.size(); i++)
    {
        ArcAttributes& arc = _attributes[i];

        if(arc._arc)
        {
            glColor3f(0.0, 0.5, 0.0);
            arc._arc->RenderData(GL_LINE_STRIP);
        }
    }
}

GLboolean FirstOrderAlgebraicTrigonometricCompositeCurve3::renderCurves(GLuint order, GLenum render_mode)
{
    for(GLuint i = 0; i < _attributes.size(); ++i)
        {
            ArcAttributes &attr = _attributes[i];
            if (attr._image)
            {
                if(attr._color)
                    glColor3f(attr.color->r(),attr.color->g(),attr.color->b());
                else
                    glColor3f(1.0,1.0,1.0);

                glPointSize(1.0);
                attr._image->RenderDerivatives(0, render_mode);
                glPointSize(0.5);

                if(order > 0) {

                    glColor3f(0.0, 0.5, 0.0);

                    attr._image->RenderDerivatives(1, GL_POINTS);
                    attr._image->RenderDerivatives(1, GL_LINES);
                }

                if(order > 1) {
                    glColor3f(0.1, 0.5, 0.9);

                    attr._image->RenderDerivatives(2, GL_POINTS);
                    attr._image->RenderDerivatives(2, GL_LINES);
                }
                 glPointSize(1.0);
            }
        }
        return GL_TRUE;
}


