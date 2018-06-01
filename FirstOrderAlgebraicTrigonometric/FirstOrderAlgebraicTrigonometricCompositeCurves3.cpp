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
    return GL_TRUE;
}

GLboolean FirstOrderAlgebraicTrigonometricCompositeCurve3::joinExistingArc(GLuint index1, GLuint index2, Direction direction1, Direction direction2)
{
    return GL_TRUE;
}

GLboolean FirstOrderAlgebraicTrigonometricCompositeCurve3::mergeExistingArc(GLuint index1, GLuint index2, Direction direction1, Direction direction2)
{
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

