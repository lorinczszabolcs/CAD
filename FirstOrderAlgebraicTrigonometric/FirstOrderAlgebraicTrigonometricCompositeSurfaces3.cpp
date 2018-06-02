#include "FirstOrderAlgebraicTrigonometricCompositeSurfaces3.h"
#include "Core/Materials.h"

using namespace cagd;

FirstOrderAlgebraicTrigonometricSurface3::PatchAttributes::PatchAttributes() :
    _patch(new FirstOrderAlgebraicTrigonometricPatch(1.0,1.0))
{
    this->_image = nullptr;
    this->_material = nullptr;
}

FirstOrderAlgebraicTrigonometricSurface3::PatchAttributes::~PatchAttributes()
{
    if(this->_patch)
    {
        delete this->_patch;
    }
    if(this->_image)
    {
        delete this->_image;
    }
}

FirstOrderAlgebraicTrigonometricSurface3::PatchAttributes::PatchAttributes(
        const PatchAttributes &attr) :
            _patch(new FirstOrderAlgebraicTrigonometricPatch(*attr._patch)),
            _image(new TriangulatedMesh3(*attr._image)),
            _material(attr._material)
{
    for(int i = N; i <= NW; i++)
    {
        this->_neighbors[i] = attr._neighbors[i];
    }
}

FirstOrderAlgebraicTrigonometricSurface3::PatchAttributes&
    FirstOrderAlgebraicTrigonometricSurface3::PatchAttributes::
    operator=(const PatchAttributes &attr)
{
    if(this == &attr)
    {
        return *this;
    }
    if(this->_patch)
    {
        delete this->_patch;
        this->_patch = nullptr;
    }
    if(this->_image)
    {
        delete this->_image;
        this->_image = nullptr;
    }

    if(attr._patch != nullptr)
    {
        this->_patch = new FirstOrderAlgebraicTrigonometricPatch(*attr._patch);
    } else
    {
        this->_patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
    }

    if(attr._image != nullptr)
    {
        this->_image = new TriangulatedMesh3(*attr._image);
    } else
    {
        this->_image = new TriangulatedMesh3();
    }

    this->_material = attr._material;

    for(int i = N; i <= NW; i++)
    {
        this->_neighbors[i] = attr._neighbors[i];
    }
    return *this;
}

FirstOrderAlgebraicTrigonometricSurface3::FirstOrderAlgebraicTrigonometricSurface3()
{
    _attributes.reserve(1000);
}

FirstOrderAlgebraicTrigonometricSurface3::FirstOrderAlgebraicTrigonometricSurface3(
        const FirstOrderAlgebraicTrigonometricSurface3 &surface)
{
    _attributes.reserve(1000);
    _attributes = surface._attributes;
}

FirstOrderAlgebraicTrigonometricSurface3& FirstOrderAlgebraicTrigonometricSurface3::
    operator =(const FirstOrderAlgebraicTrigonometricSurface3& surface)
{
    _attributes = surface._attributes;
    return *this;
}

FirstOrderAlgebraicTrigonometricSurface3::~FirstOrderAlgebraicTrigonometricSurface3()
{
    _attributes.clear();
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::insertIsolatedSurface(
        FirstOrderAlgebraicTrigonometricPatch &patch)
{
    GLuint size = _attributes.size();
    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &newPatch = _attributes[size];
    newPatch._patch = &patch;
    UpdatePatchVBOGenerateImage(newPatch);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::UpdatePatchVBOGenerateImage(
        PatchAttributes &newPatch)
{
    newPatch._patch->UpdateVertexBufferObjectsOfData();
    newPatch._image = newPatch._patch->GenerateImage(30, 30, GL_STATIC_DRAW);
    if(newPatch._image)
    {
        newPatch._image->UpdateVertexBufferObjects();
    }
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::setMaterial(GLuint index, Material *material)
{
    _attributes[index]._material = material;
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::ValidatePointersInPatchAttrs(
        PatchAttributes *oldAttr, PatchAttributes *newAttr)
{
    if(oldAttr != newAttr) {
        for(GLuint i = 0; i < _attributes.size(); i++)
        {
            for(int j = N; j <= NW; j++) {
                _attributes[i]._neighbors[j] = _attributes[i]._neighbors[j] - oldAttr + newAttr;
            }
        }
    }
}

GLboolean FirstOrderAlgebraicTrigonometricSurface3::joinExistingSurface(GLuint ind1, GLuint ind2, Direction dir1, Direction dir2)
{
    PatchAttributes &patch1 = _attributes[ind1];
    PatchAttributes &patch2 = _attributes[ind2];
    if((patch1._neighbors[dir1] != nullptr)||(patch2._neighbors[dir2] != nullptr))
        return GL_FALSE;

    switch (dir1) {
    case N:
        switch(dir2) {
        case N:
            joinPatchesN_N(patch1, patch2);
            break;
        case E:
            joinPatchesN_E(patch1, patch2);
            break;
        case S:
            joinPatchesN_S(patch1, patch2);
            break;
        case W:
            joinPatchesW_S(patch2, patch1);
            break;
        default:
            return GL_FALSE;
            break;
        }
        break;
    case E:
        switch(dir2) {
        case N:
            joinPatchesN_E(patch2, patch1);
            break;
        case E:
            joinPatchesE_E(patch1, patch2);
            break;
        case S:
            joinPatchesE_S(patch1, patch2);
            break;
        case W:
            joinPatchesW_E(patch2, patch1);
            break;
        default:
            return GL_FALSE;
            break;
        }
        break;
    case S:
        switch(dir2) {
        case N:
            joinPatchesN_S(patch2, patch1);
            break;
        case E:
            joinPatchesE_S(patch2, patch1);
            break;
        case S:
            joinPatchesS_S(patch1, patch2);
            break;
        case W:
            joinPatchesW_S(patch2, patch1);
            break;
        default:
            return GL_FALSE;
            break;
        }
        break;
    case W:
        switch(dir2) {
        case N:
            joinPatchesW_N(patch1, patch2);
            break;
        case E:
            joinPatchesW_E(patch1, patch2);
            break;
        case S:
            joinPatchesW_S(patch1, patch2);
            break;
        case W:
            joinPatchesW_W(patch1, patch2);
            break;
        default:
            return GL_FALSE;
            break;
        }
        break;
    case NW:
        switch(dir2) {
        case NW:
            joinPatchesNW_NW(patch1, patch2);
            break;
        case NE:
            joinPatchesNW_NE(patch1, patch2);
            break;
        case SE:
            joinPatchesNW_SE(patch1, patch2);
            break;
        case SW:
            joinPatchesNW_SW(patch1, patch2);
            break;
        default:
            return GL_FALSE;
            break;
        }
        break;
    case NE:
        switch(dir2) {
        case NW:
            joinPatchesNW_NE(patch2, patch1);
            break;
        case NE:
            joinPatchesNE_NE(patch1, patch2);
            break;
        case SE:
            joinPatchesNE_SE(patch1, patch2);
            break;
        case SW:
            joinPatchesNE_SW(patch1, patch2);
            break;
        default:
            return GL_FALSE;
            break;
        }
        break;
    case SE:
        switch(dir2) {
        case NW:
            joinPatchesNW_SE(patch2, patch1);
            break;
        case NE:
            joinPatchesNE_SE(patch2, patch1);
            break;
        case SE:
            joinPatchesSE_SE(patch1, patch2);
            break;
        case SW:
            joinPatchesSE_SW(patch1, patch2);
            break;
        default:
            return GL_FALSE;
            break;
        }
        break;
    case SW:
        switch(dir2) {
        case NW:
            joinPatchesNW_SW(patch2, patch1);
            break;
        case NE:
            joinPatchesNE_SW(patch2, patch1);
            break;
        case SE:
            joinPatchesSE_SW(patch2, patch1);
            break;
        case SW:
            joinPatchesSW_SW(patch1, patch2);
            break;
        default:
            return GL_FALSE;
            break;
        }
        break;
    default:
        return GL_FALSE;
        break;
    }
    return GL_TRUE;
}

GLboolean FirstOrderAlgebraicTrigonometricSurface3::mergeExistingSurface(GLuint ind1, GLuint ind2, Direction dir1, Direction dir2)
{
    PatchAttributes &patch1 = _attributes[ind1];
    PatchAttributes &patch2 = _attributes[ind2];
    if((patch1._neighbors[dir1] != nullptr)||(patch2._neighbors[dir2] != nullptr))
        return GL_FALSE;

    switch(dir1) {
    case N:
        switch(dir2) {
        case N:
            mergePatchesN_N((*patch1._patch), (*patch2._patch));
            break;
        case W:
            mergePatchesW_N((*patch2._patch), (*patch1._patch));
            break;
        case S:
            mergePatchesN_S((*patch1._patch), (*patch2._patch));
            break;
        case E:
            mergePatchesN_E((*patch1._patch), (*patch2._patch));
            break;
        default:
            return GL_FALSE;
        }
        break;
    case W:
        switch(dir2) {
        case N:
            mergePatchesW_N((*patch1._patch), (*patch2._patch));
            break;
        case W:
            mergePatchesW_W((*patch1._patch), (*patch2._patch));
            break;
        case S:
            mergePatchesW_S((*patch1._patch), (*patch2._patch));
            break;
        case E:
            mergePatchesW_E((*patch1._patch), (*patch2._patch));
            break;
        default:
            return GL_FALSE;
        }
        break;
    case S:
        switch(dir2) {
        case N:
            mergePatchesN_S((*patch2._patch), (*patch1._patch));
            break;
        case W:
            mergePatchesW_S((*patch2._patch), (*patch1._patch));
            break;
        case S:
            mergePatchesS_S((*patch1._patch), (*patch2._patch));//hitler
            break;
        case E:
            mergePatchesE_S((*patch2._patch), (*patch1._patch));
            break;
        default:
            return GL_FALSE;
        }
        break;
    case E:
        switch(dir2) {
        case N:
            mergePatchesN_E((*patch2._patch), (*patch1._patch));
            break;
        case W:
            mergePatchesW_E((*patch2._patch), (*patch1._patch));
            break;
        case S:
            mergePatchesE_S((*patch1._patch), (*patch2._patch));
            break;
        case E:
            mergePatchesE_E((*patch1._patch), (*patch2._patch));
            break;
        default:
            return GL_FALSE;
        }
        break;
    default:
        return GL_FALSE;
    }
    patch1._neighbors[dir1] = &patch2;
    patch2._neighbors[dir2] = &patch1;
    UpdatePatchVBOGenerateImage(patch1);
    //UpdatePatchVBOGenerateImage(patch1);
    UpdatePatchVBOGenerateImage(patch2);
    return GL_TRUE;
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::eraseExistingSurface(GLuint index)
{
    PatchAttributes *oldAttr = &_attributes[0];
    PatchAttributes &tmp = _attributes[index];
    for(int i = N; i <= NW; i++) {
        if (tmp._neighbors[i] != nullptr)
            for(int j = N; j <= NW; j++) {
                if (tmp._neighbors[i]->_neighbors[j] == &tmp)
                    tmp._neighbors[i]->_neighbors[j] = nullptr;
            }
    }
    _attributes.erase(_attributes.begin() + index);
    for(GLuint i = 0; i < _attributes.size() - 1; i++)
    {
        PatchAttributes *local = &_attributes[i];
        for(int i = N; i <= NW; i++) {
            if (local->_neighbors[i] > &tmp)
                local->_neighbors[i]--;
        }
    }
    ValidatePointersInPatchAttrs(oldAttr,&_attributes[0]);
}

GLboolean FirstOrderAlgebraicTrigonometricSurface3::renderSurfaces()
{
    for(GLuint i = 0; i < _attributes.size(); i++)
    {
        PatchAttributes &attr = _attributes[i];

        if (attr._image)
        {
            if(attr._material != nullptr)
                attr._material->Apply();
            else
                MatFBBrass.Apply();
            attr._image ->Render();
            MatFBPearl.Apply();
            attr._patch->RenderData(GL_LINE_STRIP);
        }
    }
    return GL_TRUE;
}

GLboolean FirstOrderAlgebraicTrigonometricSurface3::mergePatchesN_N(
        FirstOrderAlgebraicTrigonometricPatch &patchN1,
        FirstOrderAlgebraicTrigonometricPatch &patchN2)
{
    for (int i = 0; i < 4; i++)
    {
        DCoordinate3 tmp = (patchN1(i, 2) + patchN2(3 - i, 2)) / 2;
        patchN1(i, 3) = tmp;
        patchN2(3 - i, 3) = tmp;
    }
    return GL_TRUE;
}

GLboolean FirstOrderAlgebraicTrigonometricSurface3::mergePatchesN_E(
        FirstOrderAlgebraicTrigonometricPatch &patchN,
        FirstOrderAlgebraicTrigonometricPatch &patchE)
{
    for (int i = 0; i < 4; i++) {
        DCoordinate3 tmp = (patchN(i, 2) + patchE(2, i)) / 2;
        patchN(i, 3) = tmp;
        patchE(3, i) = tmp;
    }
    return GL_TRUE;
}

GLboolean FirstOrderAlgebraicTrigonometricSurface3::mergePatchesN_S(
        FirstOrderAlgebraicTrigonometricPatch &patchN,
        FirstOrderAlgebraicTrigonometricPatch &patchS)
{
    for (int i = 0; i < 4; i++) {
        DCoordinate3 tmp = (patchN(i, 2) + patchS(i, 1)) / 2;
        patchN(i, 3) = tmp;
        patchS(i, 0) = tmp;
    }
    return GL_TRUE;
}

GLboolean FirstOrderAlgebraicTrigonometricSurface3::mergePatchesW_E(
        FirstOrderAlgebraicTrigonometricPatch &patchW,
        FirstOrderAlgebraicTrigonometricPatch &patchE)
{
    for (int i = 0; i < 4; i++) {
        DCoordinate3 tmp = (patchW(1, i) + patchE(2, i)) / 2;
        patchW(0, i) = tmp;
        patchE(3, i) = tmp;
    }
    return GL_TRUE;
}

GLboolean FirstOrderAlgebraicTrigonometricSurface3::mergePatchesW_N(
        FirstOrderAlgebraicTrigonometricPatch &patchW,
        FirstOrderAlgebraicTrigonometricPatch &patchN)
{
    for (int i = 0; i < 4; i++) {
        DCoordinate3 tmp = (patchW(1, i) + patchN(3 - i, 2)) / 2;
        patchW(0, i) = tmp;
        patchN(3 - i, 3) = tmp;
    }
    return GL_TRUE;
}

GLboolean FirstOrderAlgebraicTrigonometricSurface3::mergePatchesW_W(
        FirstOrderAlgebraicTrigonometricPatch &patchW1,
        FirstOrderAlgebraicTrigonometricPatch &patchW2)
{
    for (int i = 0; i < 4; i++) {
        DCoordinate3 tmp = (patchW1(1, i) + patchW2(1, 3 - i)) / 2;
        patchW1(0, i) = tmp;
        patchW2(0, 3 - i) = tmp;
    }
    return GL_TRUE;
}

GLboolean FirstOrderAlgebraicTrigonometricSurface3::mergePatchesW_S(
        FirstOrderAlgebraicTrigonometricPatch &patchW,
        FirstOrderAlgebraicTrigonometricPatch &patchS)
{
    for (int i = 0; i < 4; i++) {
        DCoordinate3 tmp = (patchW(1, i) + patchS(i, 1)) / 2;
        patchW(0, i) = tmp;
        patchS(i, 0) = tmp;
    }
    return GL_TRUE;
}

GLboolean FirstOrderAlgebraicTrigonometricSurface3::mergePatchesE_E(
        FirstOrderAlgebraicTrigonometricPatch &patchE1,
        FirstOrderAlgebraicTrigonometricPatch &patchE2)
{
    for (int i = 0; i < 4; i++) {
        DCoordinate3 tmp = (patchE1(2, i) + patchE2(2, 3 - i)) / 2;
        patchE1(3, i) = tmp;
        patchE2(3, 3 - i) = tmp;
    }
    return GL_TRUE;
}

GLboolean FirstOrderAlgebraicTrigonometricSurface3::mergePatchesE_S(
        FirstOrderAlgebraicTrigonometricPatch &patchE,
        FirstOrderAlgebraicTrigonometricPatch &patchS)
{
    for (int i = 0; i < 4; i++) {
        DCoordinate3 tmp = (patchE(2, i) + patchS(3 - i, 1)) / 2;
        patchE(3, i) = tmp;
        patchS(3 - i, 0) = tmp;
    }
    return GL_TRUE;
}

GLboolean FirstOrderAlgebraicTrigonometricSurface3::mergePatchesS_S(
        FirstOrderAlgebraicTrigonometricPatch &patchS1,
        FirstOrderAlgebraicTrigonometricPatch &patchS2)
{
    for (int i = 0; i < 4; i++) {
        DCoordinate3 tmp = (patchS1(i, 1) + patchS2(3 - i, 1)) / 2;
        patchS1(i, 0) = tmp;
        patchS2(3 - i, 0) = tmp;
    }
    return GL_TRUE;
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesN_N(
        PatchAttributes &patchN1, PatchAttributes &patchN2)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];

    joiner._patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
    for (int i = 0; i < 4; i++) {
        (*joiner._patch)(i, 0) = (*patchN1._patch)(3, i);
        (*joiner._patch)(i, 1) = 2*(*patchN1._patch)(3, i) - (*patchN1._patch)(2, i);
        (*joiner._patch)(i, 2) = 2*(*patchN2._patch)(3, i) - (*patchN2._patch)(2, i);
        (*joiner._patch)(i, 3) = (*patchN2._patch)(3, i);
    }
    patchN1._neighbors[N] = &joiner;
    patchN2._neighbors[N] = &joiner;
    joiner._neighbors[N] = &patchN1;
    joiner._neighbors[S] = &patchN2;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesN_E(
        PatchAttributes &patchN, PatchAttributes &patchE)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];

    joiner._patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
    for (int i = 0; i < 4; i++) {
        (*joiner._patch)(i, 0) = (*patchN._patch)(3, i);
        (*joiner._patch)(i, 1) = 2*(*patchN._patch)(3, i) - (*patchN._patch)(2, i);
        (*joiner._patch)(i, 2) = 2*(*patchE._patch)(i, 3) - (*patchE._patch)(i, 2);
        (*joiner._patch)(i, 3) = (*patchE._patch)(i, 3);
    }
    patchN._neighbors[N] = &joiner;
    patchE._neighbors[E] = &joiner;
    joiner._neighbors[W] = &patchN;
    joiner._neighbors[E] = &patchE;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesN_S(
        PatchAttributes &patchN, PatchAttributes &patchS)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];

    joiner._patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
    for (int i = 0; i < 4; i++) {
        (*joiner._patch)(i, 0) = (*patchN._patch)(3, i);
        (*joiner._patch)(i, 1) = 2*(*patchN._patch)(3, i) - (*patchN._patch)(2, i);
        (*joiner._patch)(i, 2) = 2*(*patchS._patch)(0, i) - (*patchS._patch)(1, i);
        (*joiner._patch)(i, 3) = (*patchS._patch)(0, i);
    }
    patchN._neighbors[N] = &joiner;
    patchS._neighbors[S] = &joiner;
    joiner._neighbors[W] = &patchN;
    joiner._neighbors[E] = &patchS;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesW_E(PatchAttributes &patchW, PatchAttributes &patchE)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];

    joiner._patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
    for (int i = 0; i < 4; i++) {
        (*joiner._patch)(i, 0) = (*patchE._patch)(i, 3);
        (*joiner._patch)(i, 1) = 2*(*patchE._patch)(i, 3) - (*patchE._patch)(i, 2);
        (*joiner._patch)(i, 2) = 2*(*patchW._patch)(i, 0) - (*patchW._patch)(i, 1);
        (*joiner._patch)(i, 3) = (*patchW._patch)(i, 0);
    }
    patchW._neighbors[W] = &joiner;
    patchE._neighbors[E] = &joiner;
    joiner._neighbors[W] = &patchE;
    joiner._neighbors[E] = &patchW;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesW_N(
        PatchAttributes &patchW, PatchAttributes &patchN)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];

    joiner._patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
    for (int i = 0; i < 4; i++) {
        (*joiner._patch)(i, 0) = (*patchW._patch)(i, 0);
        (*joiner._patch)(i, 1) = 2*(*patchW._patch)(i, 0) - (*patchW._patch)(i, 1);
        (*joiner._patch)(i, 2) = 2*(*patchN._patch)(3, i) - (*patchN._patch)(2, i);
        (*joiner._patch)(i, 3) = (*patchN._patch)(3, i);
    }
    patchW._neighbors[W] = &joiner;
    patchN._neighbors[N] = &joiner;
    joiner._neighbors[W] = &patchN;
    joiner._neighbors[E] = &patchW;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesW_W(
        PatchAttributes &patchW1, PatchAttributes &patchW2)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];

    joiner._patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
    for (int i = 0; i < 4; i++) {
        (*joiner._patch)(i, 0) = (*patchW1._patch)(i, 0);
        (*joiner._patch)(i, 1) = 2*(*patchW1._patch)(i, 0) - (*patchW1._patch)(i, 1);
        (*joiner._patch)(i, 2) = 2*(*patchW2._patch)(i, 0) - (*patchW2._patch)(i, 1);
        (*joiner._patch)(i, 3) = (*patchW2._patch)(i, 0);
    }
    patchW1._neighbors[W] = &joiner;
    patchW2._neighbors[W] = &joiner;
    joiner._neighbors[W] = &patchW1;
    joiner._neighbors[E] = &patchW2;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesW_S(
        PatchAttributes &patchW, PatchAttributes &patchS)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];

    joiner._patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
    for (int i = 0; i < 4; i++) {
        (*joiner._patch)(i, 0) = (*patchW._patch)(i, 0);
        (*joiner._patch)(i, 1) = 2*(*patchW._patch)(i, 0) - (*patchW._patch)(i, 1);
        (*joiner._patch)(i, 2) = 2*(*patchS._patch)(0, i) - (*patchS._patch)(1, i);
        (*joiner._patch)(i, 3) = (*patchS._patch)(0, i);
    }
    patchW._neighbors[W] = &joiner;
    patchS._neighbors[S] = &joiner;
    joiner._neighbors[W] = &patchW;
    joiner._neighbors[E] = &patchS;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesE_E(
        PatchAttributes &patchE1, PatchAttributes &patchE2)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];

    joiner._patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
    for (int i = 0; i < 4; i++) {
        (*joiner._patch)(i, 0) = (*patchE1._patch)(i, 3);
        (*joiner._patch)(i, 1) = 2*(*patchE1._patch)(i, 3) - (*patchE1._patch)(i, 2);
        (*joiner._patch)(i, 2) = 2*(*patchE2._patch)(i, 3) - (*patchE2._patch)(i, 2);
        (*joiner._patch)(i, 3) = (*patchE2._patch)(i, 3);
    }
    patchE1._neighbors[E] = &joiner;
    patchE2._neighbors[E] = &joiner;
    joiner._neighbors[W] = &patchE1;
    joiner._neighbors[E] = &patchE2;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesE_S(
        PatchAttributes &patchE, PatchAttributes &patchS)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];

    joiner._patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
    for (int i = 0; i < 4; i++) {
        (*joiner._patch)(i, 0) = (*patchE._patch)(i, 3);
        (*joiner._patch)(i, 1) = 2*(*patchE._patch)(i, 3) - (*patchE._patch)(i, 2);
        (*joiner._patch)(i, 2) = 2*(*patchS._patch)(0, i) - (*patchS._patch)(1, i);
        (*joiner._patch)(i, 3) = (*patchS._patch)(0, i);
    }
    patchE._neighbors[E] = &joiner;
    patchS._neighbors[S] = &joiner;
    joiner._neighbors[W] = &patchE;
    joiner._neighbors[E] = &patchS;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesS_S(
        PatchAttributes &patchS1, PatchAttributes &patchS2)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];

    joiner._patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
    for (int i = 0; i < 4; i++) {
        (*joiner._patch)(i, 0) = (*patchS1._patch)(0, i);
        (*joiner._patch)(i, 1) = 2*(*patchS1._patch)(0, i) - (*patchS1._patch)(1, i);
        (*joiner._patch)(i, 2) = 2*(*patchS2._patch)(0, i) - (*patchS2._patch)(1, i);
        (*joiner._patch)(i, 3) = (*patchS2._patch)(0, i);
    }
    patchS1._neighbors[E] = &joiner;
    patchS2._neighbors[S] = &joiner;
    joiner._neighbors[W] = &patchS1;
    joiner._neighbors[E] = &patchS2;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesNW_NW(
        PatchAttributes &patchNW1, PatchAttributes &patchNW2)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];
    FirstOrderAlgebraicTrigonometricPatch &patch_1 = (*patchNW1._patch);
    FirstOrderAlgebraicTrigonometricPatch &patch_2 = (*patchNW2._patch);
    joiner._patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
    (*joiner._patch)(0, 3) = patch_1(3, 0);
    (*joiner._patch)(0, 2) = 2 * patch_1(3, 0) - patch_1(3, 1);
    (*joiner._patch)(1, 3) = 2 * patch_1(3, 0) - patch_1(2, 0);

    (*joiner._patch)(3, 0) = patch_2(3, 0);
    (*joiner._patch)(2, 0) = 2 * patch_2(3, 0) - patch_2(3, 1);
    (*joiner._patch)(3, 1) = 2 * patch_2(3, 0) - patch_2(2, 0);

    (*joiner._patch)(1, 0) = 2 * (*joiner._patch)(2, 0) - (*joiner._patch)(3, 0);
    (*joiner._patch)(0, 0) = 2 * (*joiner._patch)(1, 0) - (*joiner._patch)(2, 0);

    (*joiner._patch)(3, 2) = 2 * (*joiner._patch)(3, 1) - (*joiner._patch)(3, 0);
    (*joiner._patch)(3, 3) = 2 * (*joiner._patch)(3, 2) - (*joiner._patch)(3, 1);

    (*joiner._patch)(0, 1) = 2 * (*joiner._patch)(0, 2) - (*joiner._patch)(0, 3);
    (*joiner._patch)(2, 3) = 2 * (*joiner._patch)(1, 3) - (*joiner._patch)(0, 3);

    (*joiner._patch)(2, 1) = (((*joiner._patch)(0, 3)) / 3.0) + ((*joiner._patch)(3, 0) * 2.0 / 3.0);
    (*joiner._patch)(1, 2) = (((*joiner._patch)(3, 0)) / 3.0) + ((*joiner._patch)(0, 3) * 2.0 / 3.0);

    (*joiner._patch)(1, 1) = 2 * (*joiner._patch)(2, 1) - (*joiner._patch)(3, 1);
    (*joiner._patch)(2, 2) = 2 * (*joiner._patch)(1, 2) - (*joiner._patch)(0, 2);

    patchNW1._neighbors[NW] = &joiner;
    patchNW2._neighbors[NW] = &joiner;
    joiner._neighbors[SE] = &patchNW1;
    joiner._neighbors[NW] = &patchNW2;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesNW_NE(
        PatchAttributes &patchNW, PatchAttributes &patchNE)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];
    FirstOrderAlgebraicTrigonometricPatch &patch_1 = (*patchNW._patch);
    FirstOrderAlgebraicTrigonometricPatch &patch_2 = (*patchNE._patch);
    joiner._patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
    (*joiner._patch)(0, 3) = patch_1(3, 0);
    (*joiner._patch)(0, 2) = 2 * patch_1(3, 0) - patch_1(3, 1);
    (*joiner._patch)(1, 3) = 2 * patch_1(3, 0) - patch_1(2, 0);

    (*joiner._patch)(3, 0) = patch_2(3, 3);
    (*joiner._patch)(2, 0) = 2 * patch_2(3, 3) - patch_2(3, 2);
    (*joiner._patch)(3, 1) = 2 * patch_2(3, 3) - patch_2(2, 3);

    (*joiner._patch)(1, 0) = 2 * (*joiner._patch)(2, 0) - (*joiner._patch)(3, 0);
    (*joiner._patch)(0, 0) = 2 * (*joiner._patch)(1, 0) - (*joiner._patch)(2, 0);

    (*joiner._patch)(3, 2) = 2 * (*joiner._patch)(3, 1) - (*joiner._patch)(3, 0);
    (*joiner._patch)(3, 3) = 2 * (*joiner._patch)(3, 2) - (*joiner._patch)(3, 1);

    (*joiner._patch)(0, 1) = 2 * (*joiner._patch)(0, 2) - (*joiner._patch)(0, 3);
    (*joiner._patch)(2, 3) = 2 * (*joiner._patch)(1, 3) - (*joiner._patch)(0, 3);

    (*joiner._patch)(2, 1) = (((*joiner._patch)(0, 3)) / 3.0) + ((*joiner._patch)(3, 0) * 2.0 / 3.0);
    (*joiner._patch)(1, 2) = (((*joiner._patch)(3, 0)) / 3.0) + ((*joiner._patch)(0, 3) * 2.0 / 3.0);

    (*joiner._patch)(1, 1) = 2 * (*joiner._patch)(2, 1) - (*joiner._patch)(3, 1);
    (*joiner._patch)(2, 2) = 2 * (*joiner._patch)(1, 2) - (*joiner._patch)(0, 2);

    patchNW._neighbors[NW] = &joiner;
    patchNE._neighbors[NE] = &joiner;
    joiner._neighbors[SE] = &patchNW;
    joiner._neighbors[NW] = &patchNE;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesNW_SE(
        PatchAttributes &patchNW, PatchAttributes &patchSE)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];
    FirstOrderAlgebraicTrigonometricPatch &patch_1 = (*patchNW._patch);
    FirstOrderAlgebraicTrigonometricPatch &patch_2 = (*patchSE._patch);
    joiner._patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
    (*joiner._patch)(0, 3) = patch_1(3, 0);
    (*joiner._patch)(0, 2) = 2 * patch_1(3, 0) - patch_1(3, 1);
    (*joiner._patch)(1, 3) = 2 * patch_1(3, 0) - patch_1(2, 0);

    (*joiner._patch)(3, 0) = patch_2(0, 3);
    (*joiner._patch)(2, 0) = 2 * patch_2(0, 3) - patch_2(1, 3);
    (*joiner._patch)(3, 1) = 2 * patch_2(0, 3) - patch_2(0, 2);

    (*joiner._patch)(1, 0) = 2 * (*joiner._patch)(2, 0) - (*joiner._patch)(3, 0);
    (*joiner._patch)(0, 0) = 2 * (*joiner._patch)(1, 0) - (*joiner._patch)(2, 0);

    (*joiner._patch)(3, 2) = 2 * (*joiner._patch)(3, 1) - (*joiner._patch)(3, 0);
    (*joiner._patch)(3, 3) = 2 * (*joiner._patch)(3, 2) - (*joiner._patch)(3, 1);

    (*joiner._patch)(0, 1) = 2 * (*joiner._patch)(0, 2) - (*joiner._patch)(0, 3);
    (*joiner._patch)(2, 3) = 2 * (*joiner._patch)(1, 3) - (*joiner._patch)(0, 3);

    (*joiner._patch)(2, 1) = (((*joiner._patch)(0, 3)) / 3.0) + ((*joiner._patch)(3, 0) * 2.0 / 3.0);
    (*joiner._patch)(1, 2) = (((*joiner._patch)(3, 0)) / 3.0) + ((*joiner._patch)(0, 3) * 2.0 / 3.0);

    (*joiner._patch)(1, 1) = 2 * (*joiner._patch)(2, 1) - (*joiner._patch)(3, 1);
    (*joiner._patch)(2, 2) = 2 * (*joiner._patch)(1, 2) - (*joiner._patch)(0, 2);

    patchNW._neighbors[NW] = &joiner;
    patchSE._neighbors[SE] = &joiner;
    joiner._neighbors[SE] = &patchNW;
    joiner._neighbors[NW] = &patchSE;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesNW_SW(
        PatchAttributes &patchNW, PatchAttributes &patchSW)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];
    FirstOrderAlgebraicTrigonometricPatch &patch_1 = (*patchNW._patch);
    FirstOrderAlgebraicTrigonometricPatch &patch_2 = (*patchSW._patch);
    joiner._patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
    (*joiner._patch)(0, 3) = patch_1(3, 0);
    (*joiner._patch)(0, 2) = 2 * patch_1(3, 0) - patch_1(3, 1);
    (*joiner._patch)(1, 3) = 2 * patch_1(3, 0) - patch_1(2, 0);

    (*joiner._patch)(3, 0) = patch_2(0, 3);
    (*joiner._patch)(2, 0) = 2 * patch_2(0, 3) - patch_2(1, 3);
    (*joiner._patch)(3, 1) = 2 * patch_2(0, 3) - patch_2(0, 2);

    (*joiner._patch)(1, 0) = 2 * (*joiner._patch)(2, 0) - (*joiner._patch)(3, 0);
    (*joiner._patch)(0, 0) = 2 * (*joiner._patch)(1, 0) - (*joiner._patch)(2, 0);

    (*joiner._patch)(3, 2) = 2 * (*joiner._patch)(3, 1) - (*joiner._patch)(3, 0);
    (*joiner._patch)(3, 3) = 2 * (*joiner._patch)(3, 2) - (*joiner._patch)(3, 1);

    (*joiner._patch)(0, 1) = 2 * (*joiner._patch)(0, 2) - (*joiner._patch)(0, 3);
    (*joiner._patch)(2, 3) = 2 * (*joiner._patch)(1, 3) - (*joiner._patch)(0, 3);

    (*joiner._patch)(2, 1) = (((*joiner._patch)(0, 3)) / 3.0) + ((*joiner._patch)(3, 0) * 2.0 / 3.0);
    (*joiner._patch)(1, 2) = (((*joiner._patch)(3, 0)) / 3.0) + ((*joiner._patch)(0, 3) * 2.0 / 3.0);

    (*joiner._patch)(1, 1) = 2 * (*joiner._patch)(2, 1) - (*joiner._patch)(3, 1);
    (*joiner._patch)(2, 2) = 2 * (*joiner._patch)(1, 2) - (*joiner._patch)(0, 2);

    patchNW._neighbors[NW] = &joiner;
    patchSW._neighbors[SW] = &joiner;
    joiner._neighbors[SE] = &patchNW;
    joiner._neighbors[NW] = &patchSW;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesNE_NE(
        PatchAttributes &patchNE1, PatchAttributes &patchNE2)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];
    FirstOrderAlgebraicTrigonometricPatch &patch_1 = (*patchNE1._patch);
    FirstOrderAlgebraicTrigonometricPatch &patch_2 = (*patchNE2._patch);
    joiner._patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
    (*joiner._patch)(0, 0) = patch_1(3, 3);
    (*joiner._patch)(0, 1) = 2 * patch_1(3, 3) - patch_1(3, 2);
    (*joiner._patch)(1, 0) = 2 * patch_1(3, 3) - patch_1(2, 3);

    (*joiner._patch)(3, 3) = patch_2(3, 3);
    (*joiner._patch)(2, 3) = 2 * patch_2(3, 3) - patch_2(3, 2);
    (*joiner._patch)(3, 2) = 2 * patch_2(3, 3) - patch_2(2, 3);

    (*joiner._patch)(0, 2) = 2 * (*joiner._patch)(0, 1) - (*joiner._patch)(0, 0);
    (*joiner._patch)(0, 3) = 2 * (*joiner._patch)(0, 2) - (*joiner._patch)(0, 1);

    (*joiner._patch)(2, 0) = 2 * (*joiner._patch)(1, 0) - (*joiner._patch)(0, 0);
    (*joiner._patch)(3, 0) = 2 * (*joiner._patch)(2, 0) - (*joiner._patch)(1, 0);

    (*joiner._patch)(1, 3) = 2 * (*joiner._patch)(2, 3) - (*joiner._patch)(3, 3);
    (*joiner._patch)(3, 1) = 2 * (*joiner._patch)(3, 2) - (*joiner._patch)(3, 3);

    (*joiner._patch)(1, 1) = (((*joiner._patch)(3, 3)) / 3.0) + ((*joiner._patch)(0, 0) * 2.0 / 3.0);
    (*joiner._patch)(2, 2) = (((*joiner._patch)(0, 0)) / 3.0) + ((*joiner._patch)(3, 3) * 2.0 / 3.0);

    (*joiner._patch)(2, 1) = 2 * (*joiner._patch)(1, 1) - (*joiner._patch)(1, 1);
    (*joiner._patch)(1, 2) = 2 * (*joiner._patch)(2, 2) - (*joiner._patch)(2, 3);

    patchNE1._neighbors[NE] = &joiner;
    patchNE2._neighbors[NE] = &joiner;
    joiner._neighbors[SW] = &patchNE1;
    joiner._neighbors[NE] = &patchNE2;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesNE_SE(
        PatchAttributes &patchNE, PatchAttributes &patchSE)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];
    FirstOrderAlgebraicTrigonometricPatch &patch_1 = (*patchNE._patch);
    FirstOrderAlgebraicTrigonometricPatch &patch_2 = (*patchSE._patch);
    joiner._patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
    (*joiner._patch)(0, 0) = patch_1(3, 3);
    (*joiner._patch)(0, 1) = 2 * patch_1(3, 3) - patch_1(3, 2);
    (*joiner._patch)(1, 0) = 2 * patch_1(3, 3) - patch_1(2, 3);

    (*joiner._patch)(3, 3) = patch_2(0, 3);
    (*joiner._patch)(2, 3) = 2 * patch_2(0, 3) - patch_2(1, 3);
    (*joiner._patch)(3, 2) = 2 * patch_2(0, 3) - patch_2(0, 2);

    (*joiner._patch)(0, 2) = 2 * (*joiner._patch)(0, 1) - (*joiner._patch)(0, 0);
    (*joiner._patch)(0, 3) = 2 * (*joiner._patch)(0, 2) - (*joiner._patch)(0, 1);

    (*joiner._patch)(2, 0) = 2 * (*joiner._patch)(1, 0) - (*joiner._patch)(0, 0);
    (*joiner._patch)(3, 0) = 2 * (*joiner._patch)(2, 0) - (*joiner._patch)(1, 0);

    (*joiner._patch)(1, 3) = 2 * (*joiner._patch)(2, 3) - (*joiner._patch)(3, 3);
    (*joiner._patch)(3, 1) = 2 * (*joiner._patch)(3, 2) - (*joiner._patch)(3, 3);

    (*joiner._patch)(1, 1) = (((*joiner._patch)(3, 3)) / 3.0) + ((*joiner._patch)(0, 0) * 2.0 / 3.0);
    (*joiner._patch)(2, 2) = (((*joiner._patch)(0, 0)) / 3.0) + ((*joiner._patch)(3, 3) * 2.0 / 3.0);

    (*joiner._patch)(2, 1) = 2 * (*joiner._patch)(1, 1) - (*joiner._patch)(0, 1);
    (*joiner._patch)(1, 2) = 2 * (*joiner._patch)(2, 2) - (*joiner._patch)(3, 2);

    patchNE._neighbors[NE] = &joiner;
    patchSE._neighbors[SE] = &joiner;
    joiner._neighbors[SW] = &patchNE;
    joiner._neighbors[NE] = &patchSE;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesNE_SW(
        PatchAttributes &patchNE, PatchAttributes &patchSW)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];
    FirstOrderAlgebraicTrigonometricPatch &patch_1 = (*patchNE._patch);
    FirstOrderAlgebraicTrigonometricPatch &patch_2 = (*patchSW._patch);
    joiner._patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
    (*joiner._patch)(0, 0) = patch_1(3, 3);
    (*joiner._patch)(0, 1) = 2 * patch_1(3, 3) - patch_1(3, 2);
    (*joiner._patch)(1, 0) = 2 * patch_1(3, 3) - patch_1(2, 3);

    (*joiner._patch)(3, 3) = patch_2(0, 0);
    (*joiner._patch)(2, 3) = 2 * patch_2(0, 0) - patch_2(1, 0);
    (*joiner._patch)(3, 2) = 2 * patch_2(0, 0) - patch_2(0, 1);

    (*joiner._patch)(0, 2) = 2 * (*joiner._patch)(0, 1) - (*joiner._patch)(0, 0);
    (*joiner._patch)(0, 3) = 2 * (*joiner._patch)(0, 2) - (*joiner._patch)(0, 1);

    (*joiner._patch)(2, 0) = 2 * (*joiner._patch)(1, 0) - (*joiner._patch)(0, 0);
    (*joiner._patch)(3, 0) = 2 * (*joiner._patch)(2, 0) - (*joiner._patch)(1, 0);

    (*joiner._patch)(1, 3) = 2 * (*joiner._patch)(2, 3) - (*joiner._patch)(3, 3);
    (*joiner._patch)(3, 1) = 2 * (*joiner._patch)(3, 2) - (*joiner._patch)(3, 3);

    (*joiner._patch)(1, 1) = (((*joiner._patch)(3, 3)) / 3.0) + ((*joiner._patch)(0, 0) * 2.0 / 3.0);
    (*joiner._patch)(2, 2) = (((*joiner._patch)(0, 0)) / 3.0) + ((*joiner._patch)(3, 3) * 2.0 / 3.0);

    (*joiner._patch)(2, 1) = 2 * (*joiner._patch)(1, 1) - (*joiner._patch)(0, 1);
    (*joiner._patch)(1, 2) = 2 * (*joiner._patch)(2, 2) - (*joiner._patch)(3, 2);

    patchNE._neighbors[NE] = &joiner;
    patchSW._neighbors[SW] = &joiner;
    joiner._neighbors[SW] = &patchNE;
    joiner._neighbors[NE] = &patchSW;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesSE_SE(
        PatchAttributes &patchSE1, PatchAttributes &patchSE2)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];
    FirstOrderAlgebraicTrigonometricPatch &patch_1 = (*patchSE1._patch);
    FirstOrderAlgebraicTrigonometricPatch &patch_2 = (*patchSE2._patch);
    joiner._patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
    (*joiner._patch)(0, 0) = patch_1(0, 3);
    (*joiner._patch)(0, 1) = 2 * patch_1(0, 3) - patch_1(0, 2);
    (*joiner._patch)(1, 0) = 2 * patch_1(0, 3) - patch_1(1, 3);

    (*joiner._patch)(3, 3) = patch_2(0, 3);
    (*joiner._patch)(2, 3) = 2 * patch_2(0, 3) - patch_2(1, 3);
    (*joiner._patch)(3, 2) = 2 * patch_2(0, 3) - patch_2(0, 2);

    (*joiner._patch)(0, 2) = 2 * (*joiner._patch)(0, 1) - (*joiner._patch)(0, 0);
    (*joiner._patch)(0, 3) = 2 * (*joiner._patch)(0, 2) - (*joiner._patch)(0, 1);

    (*joiner._patch)(2, 0) = 2 * (*joiner._patch)(1, 0) - (*joiner._patch)(0, 0);
    (*joiner._patch)(3, 0) = 2 * (*joiner._patch)(2, 0) - (*joiner._patch)(1, 0);

    (*joiner._patch)(1, 3) = 2 * (*joiner._patch)(2, 3) - (*joiner._patch)(3, 3);
    (*joiner._patch)(3, 1) = 2 * (*joiner._patch)(3, 2) - (*joiner._patch)(3, 3);

    (*joiner._patch)(1, 1) = (((*joiner._patch)(3, 3)) / 3.0) + ((*joiner._patch)(0, 0) * 2.0 / 3.0);
    (*joiner._patch)(2, 2) = (((*joiner._patch)(0, 0)) / 3.0) + ((*joiner._patch)(3, 3) * 2.0 / 3.0);

    (*joiner._patch)(2, 1) = 2 * (*joiner._patch)(1, 1) - (*joiner._patch)(0, 1);
    (*joiner._patch)(1, 2) = 2 * (*joiner._patch)(2, 2) - (*joiner._patch)(3, 2);

    patchSE1._neighbors[SE] = &joiner;
    patchSE2._neighbors[SE] = &joiner;
    joiner._neighbors[SE] = &patchSE1;
    joiner._neighbors[NW] = &patchSE2;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesSE_SW(
        PatchAttributes &patchSE, PatchAttributes &patchSW)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];
    FirstOrderAlgebraicTrigonometricPatch &patch_1 = (*patchSE._patch);
    FirstOrderAlgebraicTrigonometricPatch &patch_2 = (*patchSW._patch);
    joiner._patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
    (*joiner._patch)(0, 0) = patch_1(0, 3);
    (*joiner._patch)(0, 1) = 2 * patch_1(0, 3) - patch_1(0, 2);
    (*joiner._patch)(1, 0) = 2 * patch_1(0, 3) - patch_1(1, 3);

    (*joiner._patch)(3, 3) = patch_2(0, 0);
    (*joiner._patch)(2, 3) = 2 * patch_2(0, 0) - patch_2(1, 0);
    (*joiner._patch)(3, 2) = 2 * patch_2(0, 0) - patch_2(0, 1);

    (*joiner._patch)(0, 2) = 2 * (*joiner._patch)(0, 1) - (*joiner._patch)(0, 0);
    (*joiner._patch)(0, 3) = 2 * (*joiner._patch)(0, 2) - (*joiner._patch)(0, 1);

    (*joiner._patch)(2, 0) = 2 * (*joiner._patch)(1, 0) - (*joiner._patch)(0, 0);
    (*joiner._patch)(3, 0) = 2 * (*joiner._patch)(2, 0) - (*joiner._patch)(1, 0);

    (*joiner._patch)(1, 3) = 2 * (*joiner._patch)(2, 3) - (*joiner._patch)(3, 3);
    (*joiner._patch)(3, 1) = 2 * (*joiner._patch)(3, 2) - (*joiner._patch)(3, 3);

    (*joiner._patch)(1, 1) = (((*joiner._patch)(3, 3)) / 3.0) + ((*joiner._patch)(0, 0) * 2.0 / 3.0);
    (*joiner._patch)(2, 2) = (((*joiner._patch)(0, 0)) / 3.0) + ((*joiner._patch)(3, 3) * 2.0 / 3.0);

    (*joiner._patch)(2, 1) = 2 * (*joiner._patch)(1, 1) - (*joiner._patch)(0, 1);
    (*joiner._patch)(1, 2) = 2 * (*joiner._patch)(2, 2) - (*joiner._patch)(3, 2);

    patchSE._neighbors[SE] = &joiner;
    patchSW._neighbors[NW] = &joiner;
    joiner._neighbors[SE] = &patchSE;
    joiner._neighbors[NW] = &patchSW;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesSW_SW(
        PatchAttributes &patchSW1, PatchAttributes &patchSW2)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];
    FirstOrderAlgebraicTrigonometricPatch &patch_1 = (*patchSW1._patch);
    FirstOrderAlgebraicTrigonometricPatch &patch_2 = (*patchSW2._patch);
    joiner._patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
    (*joiner._patch)(0, 0) = patch_1(0, 3);
    (*joiner._patch)(0, 1) = 2 * patch_1(0, 3) - patch_1(0, 2);
    (*joiner._patch)(1, 0) = 2 * patch_1(0, 3) - patch_1(1, 3);

    (*joiner._patch)(3, 3) = patch_2(0, 3);
    (*joiner._patch)(2, 3) = 2 * patch_2(0, 3) - patch_2(1, 3);
    (*joiner._patch)(3, 2) = 2 * patch_2(0, 3) - patch_2(0, 2);

    (*joiner._patch)(0, 2) = 2 * (*joiner._patch)(0, 1) - (*joiner._patch)(0, 0);
    (*joiner._patch)(0, 3) = 2 * (*joiner._patch)(0, 2) - (*joiner._patch)(0, 1);

    (*joiner._patch)(2, 0) = 2 * (*joiner._patch)(1, 0) - (*joiner._patch)(0, 0);
    (*joiner._patch)(3, 0) = 2 * (*joiner._patch)(2, 0) - (*joiner._patch)(1, 0);

    (*joiner._patch)(1, 3) = 2 * (*joiner._patch)(2, 3) - (*joiner._patch)(3, 3);
    (*joiner._patch)(3, 1) = 2 * (*joiner._patch)(3, 2) - (*joiner._patch)(3, 3);

    (*joiner._patch)(1, 1) = (((*joiner._patch)(3, 3)) / 3.0) + ((*joiner._patch)(0, 0) * 2.0 / 3.0);
    (*joiner._patch)(2, 2) = (((*joiner._patch)(0, 0)) / 3.0) + ((*joiner._patch)(3, 3) * 2.0 / 3.0);

    (*joiner._patch)(2, 1) = 2 * (*joiner._patch)(1, 1) - (*joiner._patch)(0, 1);
    (*joiner._patch)(1, 2) = 2 * (*joiner._patch)(2, 2) - (*joiner._patch)(3, 2);

    patchSW1._neighbors[SE] = &joiner;
    patchSW2._neighbors[NW] = &joiner;
    joiner._neighbors[SE] = &patchSW1;
    joiner._neighbors[NW] = &patchSW2;
    UpdatePatchVBOGenerateImage(joiner);
}

GLboolean FirstOrderAlgebraicTrigonometricSurface3::continueExistingSurface(GLuint index, Direction direction)
{

    PatchAttributes &patch = _attributes[index];

        if(direction == E)
        {
            if(patch._neighbors[E] != nullptr)
                return GL_FALSE;

            GLuint size = _attributes.size();
            PatchAttributes *oldAddrs = &_attributes[0];
            _attributes.resize(size + 1);
            ValidatePointersInPatchAttrs(oldAddrs, &_attributes[0]);
            PatchAttributes &added =  _attributes[size];

            added._patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
            for(GLuint i = 0; i < 4; i++)
            {
                (*added._patch)(0, i) = (*patch._patch)(3,i);
                (*added._patch)(1, i) = 2*(*patch._patch)(3,i) - (*patch._patch)(2,i);
                (*added._patch)(2, i) = 3*(*patch._patch)(3,i) - 2*(*patch._patch)(2,i);
                (*added._patch)(3, i) = 4*(*patch._patch)(3,i) - 3*(*patch._patch)(2,i);
            }

            patch._neighbors[E] = &added;
            added._neighbors[W] = &patch;
            UpdatePatchVBOGenerateImage(added);

        } else {
            if(patch._neighbors[W] != nullptr)
                return GL_FALSE;

            GLuint size = _attributes.size();
            PatchAttributes *oldAddrs = &_attributes[0];
            _attributes.resize(size + 1);
            ValidatePointersInPatchAttrs(oldAddrs, &_attributes[0]);
            PatchAttributes &added = _attributes[size];

            added._patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
            for(GLuint i = 0; i < 4; i++)
            {
                (*added._patch)(0, i) = 4*(*patch._patch)(0, i) - 3*(*patch._patch)(1, i);
                (*added._patch)(1, i) = 3*(*patch._patch)(0, i) - 2*(*patch._patch)(1, i);
                (*added._patch)(2, i) = 2*(*patch._patch)(0, i) - (*patch._patch)(1, i);
                (*added._patch)(3, i) = (*patch._patch)(0, i);
            }
            patch._neighbors[W] = &added;
            added._neighbors[E] = &patch;
            UpdatePatchVBOGenerateImage(added);
        }
        return GL_TRUE;
}
