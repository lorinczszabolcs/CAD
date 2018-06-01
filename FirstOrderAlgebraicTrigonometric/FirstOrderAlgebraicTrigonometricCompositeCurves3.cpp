#include "FirstOrderAlgebraicTrigonometricCompositeCurve3.h"
#include "Core/Exceptions.h"
//#include "Core/GenericCurves3.h"

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

FirstOrderAlgebraicTrigonometricCompositeCurve3::FirstOrderAlgebraicTrigonometricCompositeCurve3()
{}

FirstOrderAlgebraicTrigonometricCompositeCurve3::~FirstOrderAlgebraicTrigonometricCompositeCurve3()
{
    _attributes.clear();
}

GLvoid FirstOrderAlgebraicTrigonometricCompositeCurve3::insertIsolatedArc(FirstOrderAlgebraicTrigonometricArc3 &trigArc)
{
    GLuint size = _attributes.size();
    ArcAttributes *old = &_attributes[0];
    _attributes.resize(size + 1);
    validateAttributes(old, &_attributes[0]);
    ArcAttributes &newArc = _attributes[size];
    newArc.arc = &trigArc;
    UpdateArcVBOGenerateImage(newArc);
}

GLvoid FirstOrderAlgebraicTrigonometricCompositeCurve3::validateAttributes(ArcAttributes *oldAttr, ArcAttributes *newAttr)
{
    if(oldAttr != newAttr)
    {
        for(GLuint i = 0; i < _attributes.size(); i++)
        {
            if(_attributes[i].next)
               _attributes[i].next = _attributes[i].next - oldAttr + newAttr;
            if(_attributes[i].previous)
                _attributes[i].previous = _attributes[i].previous - oldAttr + newAttr;
        }
    }
}

GLvoid FirstOrderAlgebraicTrigonometricCompositeCurve3::UpdateArcVBOGenerateImage(ArcAttributes &arc_attr)
{
    if (!arc_attr.arc->UpdateVertexBufferObjectsOfData()) {
        throw Exception("Could not update the vertex buffer objects of the control polygon in FirstOrderAlgebraicTrigonometricCompositeCurves!");
    }

    arc_attr.image = arc_attr.arc->GenerateImage(_mod, _div_point_count);
    if (!arc_attr.image) {
        throw Exception("Could not generate the image of arc in FirstOrderAlgebraicTrigonometricCompositeCurves!");
    }

    if (!arc_attr.image->UpdateVertexBufferObjects()) {
        throw Exception("Could not update the VBOS arc of image in FirstOrderAlgebraicTrigonometricCompositeCurves");
    }
}

GLvoid FirstOrderAlgebraicTrigonometricCompositeCurve3::continueExistingArc(GLuint index, Direction direction)
{

}

GLvoid FirstOrderAlgebraicTrigonometricCompositeCurve3::joinExistingArc(GLuint index1, GLuint index2, Direction direction1, Direction direction2)
{

}

GLvoid FirstOrderAlgebraicTrigonometricCompositeCurve3::mergeExistingArc(GLuint index1, GLuint index2, Direction direction1, Direction direction2)
{

}

GLvoid FirstOrderAlgebraicTrigonometricCompositeCurve3::eraseExistingArc(GLuint index)
{
    ArcAttributes *oldAttr = &_attributes[0];
    ArcAttributes &tmp = _attributes[index];
    if(tmp.next != nullptr) {
        tmp.next->previous = nullptr;
    }
    if(tmp.previous->next != nullptr)
    {
        tmp.previous->next = nullptr;
    }
    _attributes.erase(_attributes.begin() + index);
    for(GLuint i = 0; i < _attributes.size(); i++)
    {
        if(_attributes[i].next > &tmp)
            _attributes[i].next--;
        if(_attributes[i].previous > &tmp)
            _attributes[i].previous--;
    }
    validatePointersInArcAttr(oldAttr, &_attributes[0]);
}

GLvoid FirstOrderAlgebraicTrigonometricCompositeCurve3::validatePointersInArcAttr(ArcAttributes *oldAttr, ArcAttributes *newAttr)
{
    if(oldAttr != newAttr)
    {
        for(GLuint i = 0; i < _attributes.size(); i++)
        {
            if(_attributes[i].next)
               _attributes[i].next = _attributes[i].next - oldAttr + newAttr;
            if(_attributes[i].previous)
                _attributes[i].previous = _attributes[i].previous - oldAttr + newAttr;
        }
    }
}

