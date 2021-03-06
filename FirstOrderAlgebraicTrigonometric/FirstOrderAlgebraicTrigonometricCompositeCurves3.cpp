#include "FirstOrderAlgebraicTrigonometricCompositeCurve3.h"
#include "Core/Exceptions.h"
#include <iostream>
#include <ctime>


using namespace std;

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
    delete this->_arc, this->_arc = nullptr;
    delete this->_image, this->_image = nullptr;
    //delete this->_color;
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
//    if(this->_color)
//    {
//        delete this->_color;
//    }

    this->_arc = new FirstOrderAlgebraicTrigonometricArc3(*arc_atr._arc);
    this->_image = new GenericCurve3(*arc_atr._image);
    this->_color = arc_atr._color;

    return *this;
}

FirstOrderAlgebraicTrigonometricCompositeCurve3::FirstOrderAlgebraicTrigonometricCompositeCurve3(GLdouble alpha, GLuint max_arc_count)
{
    _attributes.reserve(max_arc_count);
    _alpha = alpha;
}

FirstOrderAlgebraicTrigonometricCompositeCurve3::~FirstOrderAlgebraicTrigonometricCompositeCurve3()
{
    _attributes.clear();
}

GLboolean FirstOrderAlgebraicTrigonometricCompositeCurve3::insert(Color4* color)
{

    cout << "A2\n";

    GLdouble z = -2.0 + 4.0 * (GLdouble) rand() / RAND_MAX;

    cout << "z" << z << "\n";

    FirstOrderAlgebraicTrigonometricArc3 *temp = new FirstOrderAlgebraicTrigonometricArc3(1.0);
    temp->SetData(0, 0.0, 0.0, z);
    temp->SetData(1, 1.0, 0.0, z);
    temp->SetData(2, 2.0, 0.0, z);
    temp->SetData(3, 3.0, 0.0, z);

    cout << "A3\n";


    if (!temp->UpdateVertexBufferObjectsOfData())
    {
        cout << "Could not update vertex buffer objects of data!\n";
        return GL_FALSE;
    }

    cout << "A4\n";


    if (! insertIsolatedArc(*temp, 2, 200, color))
    {
        cout << "Could not insert isolated arc!\n";
        return GL_FALSE;
    }

    cout << "A5\n";


    return GL_TRUE;
}

GLboolean FirstOrderAlgebraicTrigonometricCompositeCurve3::insertIsolatedArc(FirstOrderAlgebraicTrigonometricArc3 &trigArc, GLdouble mod, GLdouble div_point_count, Color4* color)
{
    GLuint n = _attributes.size();
    if (n > 0)
    {
        cout << "Itt vagyok: >0\n";
        ArcAttributes *old = &_attributes[0];
        _attributes.resize(n + 1);
        validateAttributes(old, &_attributes[0]);
        ArcAttributes &newArc = _attributes[n];
        newArc._arc = &trigArc;
        newArc._mod = mod;
        newArc._div_point_count = div_point_count;
        newArc._color = color;
        if (! updateArcVBOGenerateImage(newArc))
        {
            _attributes.pop_back();
            cout << "Could not update vertex buffer object or generate image!\n";
            return GL_FALSE;
        }
    }
    else
    {
        cout << "Itt vagyok: =0\n";
        _attributes.resize(n + 1);
        cout << "Resize\n";
        ArcAttributes &newArc = _attributes[n];
        cout << "Olyat akarok elerni, amit nem lehet\n";
        newArc._arc = &trigArc;
        newArc._mod = 2;
        newArc._div_point_count = 200;
        newArc._color = color;
        cout << "Referencia\n";
        if (! updateArcVBOGenerateImage(newArc))
        {
            _attributes.pop_back();
            return GL_FALSE;
        }
        cout << "Sikerult updatelni!\n";
    }

    cout << "Meret: " << _attributes.size()  << "\n";

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

GLboolean FirstOrderAlgebraicTrigonometricCompositeCurve3::updateArcVBOGenerateImage(ArcAttributes &arc_attr)
{
    cout << "Bent vagyok\n";
    if (!arc_attr._arc->UpdateVertexBufferObjectsOfData())
    {
        return GL_FALSE;
    }

    cout << "Update1kesz\n";

    arc_attr._image = arc_attr._arc->GenerateImage(arc_attr._mod, arc_attr._div_point_count);
    if (!arc_attr._image)
    {
        return GL_FALSE;
    }

    cout << "Generatimage kesz\n";

    if (!arc_attr._image->UpdateVertexBufferObjects())
    {
        return GL_FALSE;
    }

    cout << "Update2kesz\n";

    return GL_TRUE;
}

GLboolean FirstOrderAlgebraicTrigonometricCompositeCurve3::continueExistingArc(GLuint index, Direction direction, GLdouble mod, GLdouble div_point_count, Color4* color)
{
    ArcAttributes &arc = _attributes[index];

    cout << "Itt vagyok: continue; Dircetion: " << direction << "\n";

    if(direction == RIGHT)
    {
        cout << "RIIIGHT\n";
       if(arc._next != nullptr)
           return GL_FALSE;

       GLuint size = _attributes.size();
       ArcAttributes *oldAddrs = &_attributes[0];
       _attributes.resize(size + 1);
       validatePointersInArcAttr(oldAddrs, &_attributes[0]);
       ArcAttributes &added =  _attributes[size];

       added._arc = new FirstOrderAlgebraicTrigonometricArc3(1.0);
       added._color = color;
       added._div_point_count = div_point_count;
       added._mod = mod;
       (*added._arc)[0] = (*arc._arc)[3];
       (*added._arc)[1] = 2*(*arc._arc)[3] - (*arc._arc)[2];
       (*added._arc)[2] = 3*(*arc._arc)[3] - 2*(*arc._arc)[2];
       (*added._arc)[3] = 4*(*arc._arc)[3] - 3*(*arc._arc)[2];

       arc._next = &added;
       added._previous = &arc;
       updateArcVBOGenerateImage(added);
    }
    else
    {
        cout << "LEEEFT\n";
       if(arc._previous != nullptr)
           return GL_FALSE;

       GLuint size = _attributes.size();
       ArcAttributes *oldAddrs = &_attributes[0];
       _attributes.resize(size + 1);
       validateAttributes(oldAddrs,&_attributes[0]);
       ArcAttributes &added =  _attributes[size];

       added._arc = new FirstOrderAlgebraicTrigonometricArc3(_alpha);
       added._color = color;
       added._div_point_count = div_point_count;
       added._mod = mod;
       (*added._arc)[0] = 4*(*arc._arc)[0] - 3*(*arc._arc)[1];
       (*added._arc)[1] = 3*(*arc._arc)[0] - 2*(*arc._arc)[1];
       (*added._arc)[2] = 2*(*arc._arc)[0] - (*arc._arc)[1];
       (*added._arc)[3] = (*arc._arc)[0];

       arc._previous = &added;
       added._next = &arc;
       updateArcVBOGenerateImage(added);
    }
    return GL_TRUE;
}

GLboolean FirstOrderAlgebraicTrigonometricCompositeCurve3::joinExistingArc(GLuint index1, GLuint index2, Direction direction1, Direction direction2, GLdouble mod, GLdouble div_point_count, Color4* color)
{

    ArcAttributes &arc1 = _attributes[index1];
    ArcAttributes &arc2 = _attributes[index2];

    if(direction1 == RIGHT && direction2 == LEFT)
    {

        if(arc1._next != nullptr)
            return GL_FALSE;

        if(arc2._previous != nullptr)
            return GL_FALSE;

        GLuint size = _attributes.size();

        ArcAttributes *oldAttr = &_attributes[0];
        _attributes.resize(size + 1);
        validatePointersInArcAttr(oldAttr, &_attributes[0]);

        ArcAttributes &joiner = _attributes[size];

        joiner._color = color;
        joiner._div_point_count = div_point_count;
        joiner._mod = mod;

        (*joiner._arc) = FirstOrderAlgebraicTrigonometricArc3(1.0);
        (*joiner._arc)[0] = (*arc1._arc)[3];
        (*joiner._arc)[1] = 2 * (*arc1._arc)[3] - (*arc1._arc)[2];
        (*joiner._arc)[2] = 2 * (*arc2._arc)[0] - (*arc2._arc)[1];
        (*joiner._arc)[3] = (*arc2._arc)[0];

        arc1._next = &joiner;
        arc2._previous = &joiner;
        joiner._next = &arc2;
        joiner._previous = &arc1;
        updateArcVBOGenerateImage(joiner);
    }
    else
    {
        if(direction1 == RIGHT && direction2 == RIGHT){
            return GL_FALSE;
        }
        else
        {
            if(direction1 == LEFT && direction2 == LEFT)
            {
                return GL_FALSE;
            }
            else
            {
                //direction1 == LEFT && direction2 == RIGHT
                if(arc1._previous != nullptr)
                    return GL_FALSE;

                if(arc2._next != nullptr)
                    return GL_FALSE;

                GLuint size = _attributes.size();

                ArcAttributes *oldAttr = &_attributes[0];
                _attributes.resize(size + 1);
                validatePointersInArcAttr(oldAttr, &_attributes[0]);

                ArcAttributes &joiner = _attributes[size];

                joiner._color = color;
                joiner._div_point_count = div_point_count;
                joiner._mod = mod;

                (*joiner._arc) = FirstOrderAlgebraicTrigonometricArc3(1.0);
                (*joiner._arc)[0] = (*arc2._arc)[3];
                (*joiner._arc)[1] = 2 * (*arc2._arc)[3] - (*arc2._arc)[2];
                (*joiner._arc)[2] = 2 * (*arc1._arc)[0] - (*arc1._arc)[1];
                (*joiner._arc)[3] = (*arc1._arc)[0];

                arc1._previous = &joiner;
                arc2._next = &joiner;
                joiner._previous = &arc2;
                joiner._next = &arc1;
                updateArcVBOGenerateImage(joiner);
            }
        }
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
    updateArcVBOGenerateImage(arc1);
    updateArcVBOGenerateImage(arc2);

    return GL_TRUE;
}

GLboolean FirstOrderAlgebraicTrigonometricCompositeCurve3::eraseExistingArc(GLuint index)
{
    cout << "Bent: " << index << " \n";

    ArcAttributes *oldAttr = &_attributes[0];
    ArcAttributes &tmp = _attributes[index];

    cout << "Elerem s minden\n";

    if(tmp._next != nullptr) {
        tmp._next->_previous = nullptr;
    }

    cout << "tempnext megoldva\n";


    if (tmp._previous != nullptr)
    {
        if(tmp._previous->_next != nullptr)
        {
            tmp._previous->_next = nullptr;
        }
    }

    cout << "tempprevious megoldva\n";

    _attributes.erase(_attributes.begin() + index, _attributes.begin() + index + 1);

    cout << "torles megoldva\n";

    for(GLuint i = 0; i < _attributes.size(); i++)
    {
        cout << i << ". iteracio next-previous\n";
        if(_attributes[i]._next > &tmp)
            _attributes[i]._next--;
        if(_attributes[i]._previous > &tmp)
            _attributes[i]._previous--;

        updateArcVBOGenerateImage(_attributes[i]);
    }

    cout << "vegeztem az iteracioval\n";

    if (_attributes.size() > 0)
    {
        validatePointersInArcAttr(oldAttr, &_attributes[0]);
    }

    cout << "validacio kesz\n";

    return GL_TRUE;
}

GLvoid FirstOrderAlgebraicTrigonometricCompositeCurve3::validatePointersInArcAttr(ArcAttributes *oldAttr, ArcAttributes *newAttr)
{
    cout << "Mar megint bent\n";
    if(oldAttr != newAttr)
    {
        cout << "Meg bennebb\n";
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
            glPointSize(3.0);
            arc._arc->RenderData(GL_POINTS);
            glPointSize(1.0);

            glColor3f(0.0, 0.5, 0.0);
            arc._arc->RenderData(GL_LINE_STRIP);
            glPointSize(1.0);
        }
    }
}

GLboolean FirstOrderAlgebraicTrigonometricCompositeCurve3::renderCurves(GLuint order, GLenum render_mode)
{

//    cout << "Size: " << _attributes.size() << "\n";

    for(GLuint i = 0; i < _attributes.size(); ++i)
        {
            ArcAttributes &attr = _attributes[i];
            if (attr._image)
            {

                if(attr._color)
                    glColor3f(attr._color->r(),attr._color->g(),attr._color->b());
                else
                    glColor3f(1.0,1.0,1.0);

//                glPointSize(5.0);
//                attr._image->RenderDerivatives(0, GL_POINTS);
//                glPointSize(1.0);


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

DCoordinate3 FirstOrderAlgebraicTrigonometricCompositeCurve3::getData(GLint arc_index, GLint point_index)
{
    if (_attributes[arc_index]._arc != nullptr)
    {
        return _attributes[arc_index]._arc->GetData(point_index);
    }
    else{
        return DCoordinate3(0.0, 0.0, 0.0);
    }
}

GLvoid FirstOrderAlgebraicTrigonometricCompositeCurve3::setData(GLint arc_index, GLint point_index, GLdouble x, GLdouble y, GLdouble z)
{
    _attributes[arc_index]._arc->SetData(point_index, x, y, z);

    if (_attributes[arc_index]._next != nullptr)
    {
        if (point_index == 2)
        {
            DCoordinate3 aux = 2 * (*_attributes[arc_index]._arc)[3] - (*_attributes[arc_index]._arc)[2];
            _attributes[arc_index]._next->_arc->SetData(1, aux.x(), aux.y(), aux.z());
        }

        else if (point_index == 3)
        {
            DCoordinate3 aux = (*_attributes[arc_index]._arc)[3];
            _attributes[arc_index]._next->_arc->SetData(0, aux.x(), aux.y(), aux.z());
            aux = 2 * (*_attributes[arc_index]._arc)[3] - (*_attributes[arc_index]._arc)[2];
            _attributes[arc_index]._next->_arc->SetData(1, aux.x(), aux.y(), aux.z());
        }
        updateArcVBOGenerateImage((*_attributes[arc_index]._next));
    }

    if (_attributes[arc_index]._previous != nullptr)
    {
        if (point_index == 1)
        {
            DCoordinate3 aux = 2 * (*_attributes[arc_index]._arc)[0] - (*_attributes[arc_index]._arc)[1];
            _attributes[arc_index]._previous->_arc->SetData(2, aux.x(), aux.y(), aux.z());
        }

        else if (point_index == 0)
        {
            DCoordinate3 aux = (*_attributes[arc_index]._arc)[0];
            _attributes[arc_index]._previous->_arc->SetData(3, aux.x(), aux.y(), aux.z());
            aux = 2 * (*_attributes[arc_index]._arc)[0] - (*_attributes[arc_index]._arc)[1];
            _attributes[arc_index]._previous->_arc->SetData(2, aux.x(), aux.y(), aux.z());
        }

        updateArcVBOGenerateImage((*_attributes[arc_index]._previous));
    }

    updateArcVBOGenerateImage(_attributes[arc_index]);

}

GLint FirstOrderAlgebraicTrigonometricCompositeCurve3::getAttributesSize()
{
    return _attributes.size();
}


