#include "FirstOrderAlgebraicTrigonometricCompositeSurfaces3.h"
#include "Core/Materials.h"
#include <iostream>

using namespace std;

using namespace cagd;

FirstOrderAlgebraicTrigonometricSurface3::PatchAttributes::PatchAttributes() :
    _patch(new FirstOrderAlgebraicTrigonometricPatch(1.0,1.0))
{
    this->_image = nullptr;
    this->_material = nullptr;
    this->_u_lines = nullptr;
    this->_v_lines = nullptr;
    this->_shader = nullptr;

    for(int i = N; i <= NW; i++)
    {
        this->_neighbors[i] = nullptr;
    }
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
            _material(attr._material),
            _u_lines(attr._u_lines),
            _v_lines(attr._v_lines),
            _shader(attr._shader)
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
    }
    else
    {
        this->_patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
    }

    if(attr._image != nullptr)
    {
        this->_image = new TriangulatedMesh3(*attr._image);
    }
    else
    {
        this->_image = new TriangulatedMesh3();
    }

    this->_material = attr._material;

    this->_shader = attr._shader;

    this->_u_lines = attr._u_lines;

    this->_v_lines = attr._v_lines;

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

GLboolean FirstOrderAlgebraicTrigonometricSurface3::insert(ShaderProgram* shader, Material *material)
{
    GLdouble dy = -16.0 + 32.0 * (GLdouble) rand() / RAND_MAX;

    FirstOrderAlgebraicTrigonometricPatch *temp = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
    temp->SetData(0, 0, -3.0, -4.0 + dy, 0.0 + dy);
    temp->SetData(0, 1, -3.0, -1.0 + dy, 0.0 + dy);
    temp->SetData(0, 2, -3.0, 1.0 + dy, 0.0 + dy);
    temp->SetData(0, 3, -3.0, 4.0 + dy, 0.0 + dy);

    temp->SetData(1, 0, -1.0, -4.0 + dy, 0.0 + dy);
    temp->SetData(1, 1, -1.0, -1.0 + dy, 2.0 + dy);
    temp->SetData(1, 2, -1.0, 1.0 + dy, 2.0 + dy);
    temp->SetData(1, 3, -1.0, 3.0 + dy, 0.0 + dy);

    temp->SetData(2, 0, 1.0, -4.0 + dy, 0.0 + dy);
    temp->SetData(2, 1, 1.0, -1.0 + dy, 2.0 + dy);
    temp->SetData(2, 2, 1.0, 1.0 + dy, 2.0 + dy);
    temp->SetData(2, 3, 1.0, 3.0 + dy, 0.0 + dy);

    temp->SetData(3, 0, 3.0, -4.0 + dy, 0.0 + dy);
    temp->SetData(3, 1, 3.0, -1.0 + dy, 0.0 + dy);
    temp->SetData(3, 2, 3.0, 1.0 + dy, 0.0 + dy);
    temp->SetData(3, 3, 3.0, 4.0 + dy, 0.0 + dy);

    if (!temp->UpdateVertexBufferObjectsOfData())
    {
        cout << "Could not update vertex buffer objects of data!\n";
        return GL_FALSE;
    }

    if (! insertIsolatedSurface(*temp, shader, material))
    {
        cout <<"Could not insert surface!\n";
        return GL_FALSE;
    }

    return GL_TRUE;
}

GLboolean FirstOrderAlgebraicTrigonometricSurface3::insertIsolatedSurface(
        FirstOrderAlgebraicTrigonometricPatch &patch,
        ShaderProgram* shader,
        Material *material)
{
    GLuint n = _attributes.size();

    if (n > 0)
    {
        cout << "Itt vagyok: >0\n";
        PatchAttributes *oldAttr = &_attributes[0];
        _attributes.resize(n + 1);
        ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
        PatchAttributes &newPatch = _attributes[n];
        newPatch._patch = &patch;
        newPatch._material = material;
        newPatch._shader = shader;

        newPatch._u_lines = newPatch._patch->GenerateUIsoparametricLines(5,1,100);
        newPatch._v_lines = newPatch._patch->GenerateVIsoparametricLines(5,1,100);

        for(GLuint i = 0; i < 5; i++)
        {
            (*newPatch._u_lines)[i]->UpdateVertexBufferObjects();
            (*newPatch._v_lines)[i]->UpdateVertexBufferObjects();
        }

        if (!UpdatePatchVBOGenerateImage(newPatch))
        {
            _attributes.pop_back();
            cout << "Could not update vertex buffer object or generate image!\n";
            return GL_FALSE;
        }
        cout << "Sikerult updatelni!\n";
    }
    else
    {
        cout << "Itt vagyok: =0\n";
        _attributes.resize(n + 1);
        PatchAttributes &newPatch = _attributes[n];
        newPatch._patch = &patch;
        newPatch._material = material;
        newPatch._shader = shader;
        newPatch._u_lines = newPatch._patch->GenerateUIsoparametricLines(5,1,100);
        newPatch._v_lines = newPatch._patch->GenerateVIsoparametricLines(5,1,100);

        for(GLuint i = 0; i < 5; i++)
        {
            (*newPatch._u_lines)[i]->UpdateVertexBufferObjects();
            (*newPatch._v_lines)[i]->UpdateVertexBufferObjects();
        }

        if (!UpdatePatchVBOGenerateImage(newPatch))
        {
            _attributes.pop_back();
            cout << "Could not update vertex buffer object or generate image!\n";
            return GL_FALSE;
        }
        cout << "Sikerult updatelni!\n";
    }

    return GL_TRUE;
}

GLboolean FirstOrderAlgebraicTrigonometricSurface3::UpdatePatchVBOGenerateImage(
        PatchAttributes &newPatch)
{
    if (!newPatch._patch->UpdateVertexBufferObjectsOfData())
    {
        return GL_FALSE;
    }

    cout << "Update1 kesz\n";

    newPatch._image = newPatch._patch->GenerateImage(30, 30, GL_STATIC_DRAW);

    if (!newPatch._image)
    {
        return GL_FALSE;
    }

    cout << "Generate image kesz!\n";

    if (!newPatch._image->UpdateVertexBufferObjects())
    {
        return GL_FALSE;
    }

    cout << "Update2 kesz\n";

    return GL_TRUE;
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::setMaterial(GLuint index, Material *material)
{
    _attributes[index]._material = material;
}


GLint FirstOrderAlgebraicTrigonometricSurface3::getAttributesSize()
{
    return _attributes.size();
}

DCoordinate3 FirstOrderAlgebraicTrigonometricSurface3::getData(GLint patch_index, GLint u_index, GLint v_index)
{
    DCoordinate3 aux;
    _attributes[patch_index]._patch->GetData(u_index, v_index, aux);

    return aux;
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::setData(GLint patch_index, GLint u_index, GLint v_index, GLdouble x, GLdouble y, GLdouble z)
{
    _attributes[patch_index]._patch->SetData(u_index, v_index, x, y, z);


    // north
    if (u_index > 1)
    {
        if (_attributes[patch_index]._neighbors[N] != nullptr)
        {
            // u = 3
            if (u_index > 2)
            {
                DCoordinate3 d1, d2;
                _attributes[patch_index]._patch->GetData(u_index, v_index, d1);
                _attributes[patch_index]._patch->GetData(2, v_index, d2);
                _attributes[patch_index]._neighbors[N]->_patch->SetData(0, v_index, d1);
                _attributes[patch_index]._neighbors[N]->_patch->SetData(1, v_index, 2.0 * d1 - d2);

            }
            // u = 2
            else
            {
                DCoordinate3 d1, d2;

                _attributes[patch_index]._patch->GetData(3, v_index, d1);
                _attributes[patch_index]._patch->GetData(u_index, v_index, d2);
                _attributes[patch_index]._neighbors[N]->_patch->SetData(1, v_index, 2.0 * d1 - d2);
            }

            _attributes[patch_index]._neighbors[N]->_u_lines = _attributes[patch_index]._neighbors[N]->_patch->GenerateUIsoparametricLines(5, 1, 100);
            _attributes[patch_index]._neighbors[N]->_v_lines = _attributes[patch_index]._neighbors[N]->_patch->GenerateVIsoparametricLines(5, 1, 100);

            for (GLint i = 0; i < 5; i++)
            {
                (*_attributes[patch_index]._neighbors[N]->_u_lines)[i]->UpdateVertexBufferObjects();
                (*_attributes[patch_index]._neighbors[N]->_v_lines)[i]->UpdateVertexBufferObjects();

            }

            UpdatePatchVBOGenerateImage((*_attributes[patch_index]._neighbors[N]));
        }
    }
    // south
    else
    {
        if (_attributes[patch_index]._neighbors[S] != nullptr)
        {
            // u = 0
            if (u_index < 1)
            {
                DCoordinate3 d1, d2;
                _attributes[patch_index]._patch->GetData(u_index, v_index, d1);
                _attributes[patch_index]._patch->GetData(1, v_index, d2);
                _attributes[patch_index]._neighbors[S]->_patch->SetData(3, v_index, d1);
                _attributes[patch_index]._neighbors[S]->_patch->SetData(2, v_index, 2 * d1 - d2);

            }
            // u = 1
            else
            {
                DCoordinate3 d1, d2;

                _attributes[patch_index]._patch->GetData(0, v_index, d1);
                _attributes[patch_index]._patch->GetData(u_index, v_index, d2);
                _attributes[patch_index]._neighbors[S]->_patch->SetData(2, v_index, 2 * d1 - d2);
            }

            _attributes[patch_index]._neighbors[S]->_u_lines = _attributes[patch_index]._neighbors[S]->_patch->GenerateUIsoparametricLines(5, 1, 100);
            _attributes[patch_index]._neighbors[S]->_v_lines = _attributes[patch_index]._neighbors[S]->_patch->GenerateVIsoparametricLines(5, 1, 100);

            for (GLint i = 0; i < 5; i++)
            {
                (*_attributes[patch_index]._neighbors[S]->_u_lines)[i]->UpdateVertexBufferObjects();
                (*_attributes[patch_index]._neighbors[S]->_v_lines)[i]->UpdateVertexBufferObjects();

            }

            UpdatePatchVBOGenerateImage((*_attributes[patch_index]._neighbors[S]));
        }
    }

    // east
    if (v_index > 1)
    {
        if (_attributes[patch_index]._neighbors[E] != nullptr)
        {
            // v = 3
            if (v_index > 2)
            {
                DCoordinate3 d1, d2;
                _attributes[patch_index]._patch->GetData(u_index, v_index, d1);
                _attributes[patch_index]._patch->GetData(u_index, 2, d2);
                _attributes[patch_index]._neighbors[E]->_patch->SetData(u_index, 0, d1);
                _attributes[patch_index]._neighbors[E]->_patch->SetData(u_index, 1, 2 * d1 - d2);

            }
            // v = 2
            else
            {
                DCoordinate3 d1, d2;
                _attributes[patch_index]._patch->GetData(u_index, 3, d1);
                _attributes[patch_index]._patch->GetData(u_index, v_index, d2);
                _attributes[patch_index]._neighbors[E]->_patch->SetData(u_index, 1, 2 * d1 - d2);
            }

            _attributes[patch_index]._neighbors[E]->_u_lines = _attributes[patch_index]._neighbors[E]->_patch->GenerateUIsoparametricLines(5, 1, 100);
            _attributes[patch_index]._neighbors[E]->_v_lines = _attributes[patch_index]._neighbors[E]->_patch->GenerateVIsoparametricLines(5, 1, 100);

            for (GLint i = 0; i < 5; i++)
            {
                (*_attributes[patch_index]._neighbors[E]->_u_lines)[i]->UpdateVertexBufferObjects();
                (*_attributes[patch_index]._neighbors[E]->_v_lines)[i]->UpdateVertexBufferObjects();

            }

            UpdatePatchVBOGenerateImage((*_attributes[patch_index]._neighbors[E]));
        }
    }
    // west
    else
    {
        if (_attributes[patch_index]._neighbors[W] != nullptr)
        {
            // v = 0
            if (v_index < 1)
            {
                DCoordinate3 d1, d2;
                _attributes[patch_index]._patch->GetData(u_index, v_index, d1);
                _attributes[patch_index]._patch->GetData(u_index, 1, d2);
                _attributes[patch_index]._neighbors[W]->_patch->SetData(u_index, 3, d1);
                _attributes[patch_index]._neighbors[W]->_patch->SetData(u_index, 2, 2 * d1 - d2);

            }
            // v = 1
            else
            {
                DCoordinate3 d1, d2;

                _attributes[patch_index]._patch->GetData(u_index, 0, d1);
                _attributes[patch_index]._patch->GetData(u_index, v_index, d2);
                _attributes[patch_index]._neighbors[W]->_patch->SetData(u_index, 2, 2 * d1 - d2);
            }


            _attributes[patch_index]._neighbors[W]->_u_lines = _attributes[patch_index]._neighbors[W]->_patch->GenerateUIsoparametricLines(5, 1, 100);
            _attributes[patch_index]._neighbors[W]->_v_lines = _attributes[patch_index]._neighbors[W]->_patch->GenerateVIsoparametricLines(5, 1, 100);

            for (GLint i = 0; i < 5; i++)
            {
                (*_attributes[patch_index]._neighbors[W]->_u_lines)[i]->UpdateVertexBufferObjects();
                (*_attributes[patch_index]._neighbors[W]->_v_lines)[i]->UpdateVertexBufferObjects();

            }

            UpdatePatchVBOGenerateImage((*_attributes[patch_index]._neighbors[W]));
        }
    }

    _attributes[patch_index]._u_lines = _attributes[patch_index]._patch->GenerateUIsoparametricLines(5, 1, 100);
    _attributes[patch_index]._v_lines = _attributes[patch_index]._patch->GenerateVIsoparametricLines(5, 1, 100);


    for (GLint i = 0; i < 5; i++)
    {
        (*_attributes[patch_index]._u_lines)[i]->UpdateVertexBufferObjects();
        (*_attributes[patch_index]._v_lines)[i]->UpdateVertexBufferObjects();

    }

    UpdatePatchVBOGenerateImage(_attributes[patch_index]);

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

GLboolean FirstOrderAlgebraicTrigonometricSurface3::joinExistingSurface(GLuint ind1, GLuint ind2, Direction dir1, Direction dir2, ShaderProgram *shader, Material* material)
{
    PatchAttributes &patch1 = _attributes[ind1];
    PatchAttributes &patch2 = _attributes[ind2];
    if((patch1._neighbors[dir1] != nullptr)||(patch2._neighbors[dir2] != nullptr))
        return GL_FALSE;

    switch (dir1) {
    case N:
        switch(dir2) {
        case N:
            joinPatchesN_N(patch1, patch2, shader, material);
            break;
        case E:
            joinPatchesN_E(patch1, patch2, shader, material);
            break;
        case S:
            joinPatchesN_S(patch1, patch2, shader, material);
            break;
        case W:
            joinPatchesW_S(patch2, patch1, shader, material);
            break;
        default:
            return GL_FALSE;
            break;
        }
        break;
    case E:
        switch(dir2) {
        case N:
            joinPatchesN_E(patch2, patch1, shader, material);
            break;
        case E:
            joinPatchesE_E(patch1, patch2, shader, material);
            break;
        case S:
            joinPatchesE_S(patch1, patch2, shader, material);
            break;
        case W:
            joinPatchesW_E(patch2, patch1, shader, material);
            break;
        default:
            return GL_FALSE;
            break;
        }
        break;
    case S:
        switch(dir2) {
        case N:
            joinPatchesN_S(patch2, patch1, shader, material);
            break;
        case E:
            joinPatchesE_S(patch2, patch1, shader, material);
            break;
        case S:
            joinPatchesS_S(patch1, patch2, shader, material);
            break;
        case W:
            joinPatchesW_S(patch2, patch1, shader, material);
            break;
        default:
            return GL_FALSE;
            break;
        }
        break;
    case W:
        switch(dir2) {
        case N:
            joinPatchesW_N(patch1, patch2, shader, material);
            break;
        case E:
            joinPatchesW_E(patch1, patch2, shader, material);
            break;
        case S:
            joinPatchesW_S(patch1, patch2, shader, material);
            break;
        case W:
            joinPatchesW_W(patch1, patch2, shader, material);
            break;
        default:
            return GL_FALSE;
            break;
        }
        break;
    case NW:
        switch(dir2) {
        case NW:
            joinPatchesNW_NW(patch1, patch2, shader, material);
            break;
        case NE:
            joinPatchesNW_NE(patch1, patch2, shader, material);
            break;
        case SE:
            joinPatchesNW_SE(patch1, patch2, shader, material);
            break;
        case SW:
            joinPatchesNW_SW(patch1, patch2, shader, material);
            break;
        default:
            return GL_FALSE;
            break;
        }
        break;
    case NE:
        switch(dir2) {
        case NW:
            joinPatchesNW_NE(patch2, patch1, shader, material);
            break;
        case NE:
            joinPatchesNE_NE(patch1, patch2, shader, material);
            break;
        case SE:
            joinPatchesNE_SE(patch1, patch2, shader, material);
            break;
        case SW:
            joinPatchesNE_SW(patch1, patch2, shader, material);
            break;
        default:
            return GL_FALSE;
            break;
        }
        break;
    case SE:
        switch(dir2) {
        case NW:
            joinPatchesNW_SE(patch2, patch1, shader, material);
            break;
        case NE:
            joinPatchesNE_SE(patch2, patch1, shader, material);
            break;
        case SE:
            joinPatchesSE_SE(patch1, patch2, shader, material);
            break;
        case SW:
            joinPatchesSE_SW(patch1, patch2, shader, material);
            break;
        default:
            return GL_FALSE;
            break;
        }
        break;
    case SW:
        switch(dir2) {
        case NW:
            joinPatchesNW_SW(patch2, patch1, shader, material);
            break;
        case NE:
            joinPatchesNE_SW(patch2, patch1, shader, material);
            break;
        case SE:
            joinPatchesSE_SW(patch2, patch1, shader, material);
            break;
        case SW:
            joinPatchesSW_SW(patch1, patch2, shader, material);
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

    patch1._u_lines = patch1._patch->GenerateUIsoparametricLines(5,1,100);
    patch1._v_lines = patch1._patch->GenerateVIsoparametricLines(5,1,100);

    patch2._u_lines = patch2._patch->GenerateUIsoparametricLines(5,1,100);
    patch2._v_lines = patch2._patch->GenerateVIsoparametricLines(5,1,100);

    for(GLuint i = 0; i < 5; i++)
    {
        (*patch1._u_lines)[i]->UpdateVertexBufferObjects();
        (*patch1._v_lines)[i]->UpdateVertexBufferObjects();
        (*patch2._u_lines)[i]->UpdateVertexBufferObjects();
        (*patch2._v_lines)[i]->UpdateVertexBufferObjects();
    }

    patch1._neighbors[dir1] = &patch2;
    patch2._neighbors[dir2] = &patch1;
    UpdatePatchVBOGenerateImage(patch1);
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
    _attributes.erase(_attributes.begin() + index, _attributes.begin() + index + 1);

    for(GLuint i = 0; i < _attributes.size(); i++)
    {
        PatchAttributes &local = _attributes[i];

        for(GLuint j = N; j <= NW; j++) {
            if (local._neighbors[j] > &tmp)
                local._neighbors[j]--;
        }

        if ((*local._u_lines).GetColumnCount() == 0)
        {
            local._u_lines = local._patch->GenerateUIsoparametricLines(5, 5, 100);
        }

        if ((*local._v_lines).GetColumnCount() == 0)
        {
            local._v_lines = local._patch->GenerateVIsoparametricLines(5, 5, 100);
        }

        for (GLuint j = 0; j < 5; j++)
        {
            (*local._u_lines)[j]->UpdateVertexBufferObjects();
            (*local._v_lines)[j]->UpdateVertexBufferObjects();
        }

        UpdatePatchVBOGenerateImage(_attributes[i]);
    }

    if(_attributes.size())
        ValidatePointersInPatchAttrs(oldAttr,&_attributes[0]);
}

GLboolean FirstOrderAlgebraicTrigonometricSurface3::renderSurfaces(GLboolean u_lin, GLboolean v_lin, GLboolean u_der, GLboolean v_der, GLboolean control, GLboolean surf, GLboolean shader, GLboolean normal_vectors)
{
    for(GLuint i = 0; i < _attributes.size(); i++)
    {
//        PatchAttributes &attr = _attributes[i];

        if (_attributes[i]._image)
        {

            if (surf)
            {
                glEnable(GL_LIGHTING);
                glEnable(GL_NORMALIZE);
                glEnable(GL_LIGHT0);

                if(_attributes[i]._material != nullptr)
                    _attributes[i]._material->Apply();
                else
                    MatFBBrass.Apply();

                if (shader)
                {
                    _attributes[i]._shader->Enable();
                }
                else
                {
                    _attributes[i]._shader->Disable();
                }

                _attributes[i]._image->Render();

                _attributes[i]._shader->Disable();

                glDisable(GL_LIGHTING);
                glDisable(GL_NORMALIZE);
                glDisable(GL_LIGHT0);
            }

            if (control)
            {
                glDisable(GL_LIGHTING);
                glDisable(GL_NORMALIZE);
                glDisable(GL_LIGHT0);
                glColor3f(0.4 ,0.8, 0.3);
                _attributes[i]._patch->RenderData(GL_LINE_STRIP);
            }

            if (u_lin)
            {
                glDisable(GL_LIGHTING);
                glDisable(GL_NORMALIZE);
                glDisable(GL_LIGHT0);

                glColor3f(0.2, 0.2, 0.2);
                for(GLuint j = 0; j < 5; j++)
                {
                    (*_attributes[i]._u_lines)[j]->RenderDerivatives(0, GL_LINE_STRIP);
                }
            }

            if (u_der)
            {
                glDisable(GL_LIGHTING);
                for(GLuint j = 0; j < 5; j++)
                {
                    glColor3f(0.0,0.2,0.0);
                    (*_attributes[i]._u_lines)[j]->RenderDerivatives(1, GL_LINES);
                    (*_attributes[i]._u_lines)[j]->RenderDerivatives(1, GL_POINTS);
                }
            }

            if (v_lin)
            {
                glDisable(GL_LIGHTING);
                glDisable(GL_NORMALIZE);
                glDisable(GL_LIGHT0);

                glColor3f(0.2, 0.2, 0.2);
                for(GLuint j = 0; j < 5; j++)
                {
                    (*_attributes[i]._v_lines)[j]->RenderDerivatives(0, GL_LINE_STRIP);
                }
            }

            if (v_der)
            {
                glDisable(GL_LIGHTING);
                for(GLuint j = 0; j < 5; j++)
                {
                    glColor3f(0.0,0.2,0.0);
                    (*_attributes[i]._v_lines)[j]->RenderDerivatives(1, GL_LINES);
                    (*_attributes[i]._v_lines)[j]->RenderDerivatives(1, GL_POINTS);
                }
            }

            if (normal_vectors)
            {
                glDisable(GL_LIGHTING);
                glDisable(GL_NORMALIZE);
                glDisable(GL_LIGHT0);
                glColor4f(0.6643, 0.1678, 0.1678, 1.0);
                if (_attributes[i]._image)
                {
                    _attributes[i]._image->RenderNormalVectors();
                }
            }
        }
    }
    return GL_TRUE;
}
GLboolean FirstOrderAlgebraicTrigonometricSurface3::mergePatchesN_N(
        FirstOrderAlgebraicTrigonometricPatch &patchN1,
        FirstOrderAlgebraicTrigonometricPatch &patchN2)
{
    for (int i = 0; i < 4; i++) {
        DCoordinate3 tmp = (patchN1(2, i) + patchN2(2, 3 - i)) / 2;
        patchN1(3, i) = tmp;
        patchN2(3, 3 - i) = tmp;
    }
    return GL_TRUE;
}

GLboolean FirstOrderAlgebraicTrigonometricSurface3::mergePatchesN_E(
        FirstOrderAlgebraicTrigonometricPatch &patchN,
        FirstOrderAlgebraicTrigonometricPatch &patchE)
{
    for (int i = 0; i < 4; i++) {
        DCoordinate3 tmp = (patchE(i, 2) + patchN(2, i)) / 2;
        patchE(i, 3) = tmp;
        patchN(3, i) = tmp;
    }
    return GL_TRUE;
}

GLboolean FirstOrderAlgebraicTrigonometricSurface3::mergePatchesN_S(
        FirstOrderAlgebraicTrigonometricPatch &patchN,
        FirstOrderAlgebraicTrigonometricPatch &patchS)
{
    for (int i = 0; i < 4; i++) {
        DCoordinate3 tmp = (patchS(1, i) + patchN(2, i)) / 2;
        patchS(0, i) = tmp;
        patchN(3, i) = tmp;
    }
    return GL_TRUE;
}

GLboolean FirstOrderAlgebraicTrigonometricSurface3::mergePatchesW_E(
        FirstOrderAlgebraicTrigonometricPatch &patchW,
        FirstOrderAlgebraicTrigonometricPatch &patchE)
{
    for (int i = 0; i < 4; i++) {
            DCoordinate3 tmp = (patchE(i, 2) + patchW(i, 1)) / 2;
            patchE(i, 3) = tmp;
            patchW(i, 0) = tmp;
        }
        return GL_TRUE;
}

GLboolean FirstOrderAlgebraicTrigonometricSurface3::mergePatchesW_N(
        FirstOrderAlgebraicTrigonometricPatch &patchW,
        FirstOrderAlgebraicTrigonometricPatch &patchN)
{
    for (int i = 0; i < 4; i++) {
        DCoordinate3 tmp = (patchN(2, i) + patchW(3 - i, 1)) / 2;
        patchN(3, i) = tmp;
        patchW(3 - i, 0) = tmp;
    }
    return GL_TRUE;
}

GLboolean FirstOrderAlgebraicTrigonometricSurface3::mergePatchesW_W(
        FirstOrderAlgebraicTrigonometricPatch &patchW1,
        FirstOrderAlgebraicTrigonometricPatch &patchW2)
{
    for (int i = 0; i < 4; i++) {
        DCoordinate3 tmp = (patchW1(i, 1) + patchW2(3 - i, 1)) / 2;
        patchW1(i, 0) = tmp;
        patchW2(3 - i, 0) = tmp;
    }
    return GL_TRUE;
}

GLboolean FirstOrderAlgebraicTrigonometricSurface3::mergePatchesW_S(
        FirstOrderAlgebraicTrigonometricPatch &patchW,
        FirstOrderAlgebraicTrigonometricPatch &patchS)
{
    for (int i = 0; i < 4; i++) {
        DCoordinate3 tmp = (patchS(1, i) + patchW(i, 1)) / 2;
        patchS(0, i) = tmp;
        patchW(i, 0) = tmp;
    }
    return GL_TRUE;
}

GLboolean FirstOrderAlgebraicTrigonometricSurface3::mergePatchesE_E(
        FirstOrderAlgebraicTrigonometricPatch &patchE1,
        FirstOrderAlgebraicTrigonometricPatch &patchE2)
{
    for (int i = 0; i < 4; i++)
        {
            DCoordinate3 tmp = (patchE1(i, 2) + patchE2(3 - i, 2)) / 2;
            patchE1(i, 3) = tmp;
            patchE2(3 - i, 3) = tmp;
        }
        return GL_TRUE;
}

GLboolean FirstOrderAlgebraicTrigonometricSurface3::mergePatchesE_S(
        FirstOrderAlgebraicTrigonometricPatch &patchE,
        FirstOrderAlgebraicTrigonometricPatch &patchS)
{
    for (int i = 0; i < 4; i++) {
        DCoordinate3 tmp = (patchS(1, i) + patchE(3 - i, 2)) / 2;
        patchS(0, i) = tmp;
        patchE(3 - i, 3) = tmp;
    }
    return GL_TRUE;
}

GLboolean FirstOrderAlgebraicTrigonometricSurface3::mergePatchesS_S(
        FirstOrderAlgebraicTrigonometricPatch &patchS1,
        FirstOrderAlgebraicTrigonometricPatch &patchS2)
{
    for (int i = 0; i < 4; i++) {
        DCoordinate3 tmp = (patchS1(1, i) + patchS2(1, 3 - i)) / 2;
        patchS1(0, i) = tmp;
        patchS2(0, 3 - i) = tmp;
    }
    return GL_TRUE;
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesN_N(PatchAttributes &patchN1, PatchAttributes &patchN2, ShaderProgram *shader, Material* material)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];

    joiner._material = material;
    joiner._shader = shader;

    joiner._patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
    for (int i = 0; i < 4; i++) {
        (*joiner._patch)(i, 0) = (*patchN1._patch)(3, i);
        (*joiner._patch)(i, 1) = 2*(*patchN1._patch)(3, i) - (*patchN1._patch)(2, i);
        (*joiner._patch)(i, 2) = 2*(*patchN2._patch)(3, i) - (*patchN2._patch)(2, i);
        (*joiner._patch)(i, 3) = (*patchN2._patch)(3, i);
    }

    joiner._u_lines = joiner._patch->GenerateUIsoparametricLines(5,1,100);
    joiner._v_lines = joiner._patch->GenerateVIsoparametricLines(5,1,100);

    for(GLuint i = 0; i < 5; i++)
    {
        (*joiner._u_lines)[i]->UpdateVertexBufferObjects();
        (*joiner._v_lines)[i]->UpdateVertexBufferObjects();
    }

    patchN1._neighbors[N] = &joiner;
    patchN2._neighbors[N] = &joiner;
    joiner._neighbors[N] = &patchN1;
    joiner._neighbors[S] = &patchN2;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesN_E(PatchAttributes &patchN, PatchAttributes &patchE, ShaderProgram *shader, Material* material)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];

    joiner._material = material;
    joiner._shader = shader;

    joiner._patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
    for (int i = 0; i < 4; i++) {
        (*joiner._patch)(i, 0) = (*patchN._patch)(3, i);
        (*joiner._patch)(i, 1) = 2*(*patchN._patch)(3, i) - (*patchN._patch)(2, i);
        (*joiner._patch)(i, 2) = 2*(*patchE._patch)(i, 3) - (*patchE._patch)(i, 2);
        (*joiner._patch)(i, 3) = (*patchE._patch)(i, 3);
    }

    joiner._u_lines = joiner._patch->GenerateUIsoparametricLines(5,1,100);
    joiner._v_lines = joiner._patch->GenerateVIsoparametricLines(5,1,100);

    for(GLuint i = 0; i < 5; i++)
    {
        (*joiner._u_lines)[i]->UpdateVertexBufferObjects();
        (*joiner._v_lines)[i]->UpdateVertexBufferObjects();
    }

    patchN._neighbors[N] = &joiner;
    patchE._neighbors[E] = &joiner;
    joiner._neighbors[W] = &patchN;
    joiner._neighbors[E] = &patchE;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesN_S(
        PatchAttributes &patchN, PatchAttributes &patchS, ShaderProgram* shader, Material* material)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];

    joiner._material = material;
    joiner._shader = shader;

    joiner._patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
    for (int i = 0; i < 4; i++) {
        (*joiner._patch)(i, 0) = (*patchN._patch)(3, i);
        (*joiner._patch)(i, 1) = 2*(*patchN._patch)(3, i) - (*patchN._patch)(2, i);
        (*joiner._patch)(i, 2) = 2*(*patchS._patch)(0, i) - (*patchS._patch)(1, i);
        (*joiner._patch)(i, 3) = (*patchS._patch)(0, i);
    }

    joiner._u_lines = joiner._patch->GenerateUIsoparametricLines(5,1,100);
    joiner._v_lines = joiner._patch->GenerateVIsoparametricLines(5,1,100);

    for(GLuint i = 0; i < 5; i++)
    {
        (*joiner._u_lines)[i]->UpdateVertexBufferObjects();
        (*joiner._v_lines)[i]->UpdateVertexBufferObjects();
    }

    patchN._neighbors[N] = &joiner;
    patchS._neighbors[S] = &joiner;
    joiner._neighbors[W] = &patchN;
    joiner._neighbors[E] = &patchS;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesW_E(
        PatchAttributes &patchW, PatchAttributes &patchE, ShaderProgram *shader, Material* material)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];

    joiner._material = material;
    joiner._shader = shader;

    joiner._patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
    for (int i = 0; i < 4; i++) {
        (*joiner._patch)(i, 0) = (*patchE._patch)(i, 3);
        (*joiner._patch)(i, 1) = 2*(*patchE._patch)(i, 3) - (*patchE._patch)(i, 2);
        (*joiner._patch)(i, 2) = 2*(*patchW._patch)(i, 0) - (*patchW._patch)(i, 1);
        (*joiner._patch)(i, 3) = (*patchW._patch)(i, 0);
    }

    joiner._u_lines = joiner._patch->GenerateUIsoparametricLines(5,1,100);
    joiner._v_lines = joiner._patch->GenerateVIsoparametricLines(5,1,100);

    for(GLuint i = 0; i < 5; i++)
    {
        (*joiner._u_lines)[i]->UpdateVertexBufferObjects();
        (*joiner._v_lines)[i]->UpdateVertexBufferObjects();
    }

    patchW._neighbors[W] = &joiner;
    patchE._neighbors[E] = &joiner;
    joiner._neighbors[W] = &patchE;
    joiner._neighbors[E] = &patchW;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesW_N(
        PatchAttributes &patchW, PatchAttributes &patchN, ShaderProgram *shader, Material* material)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];

    joiner._material = material;
    joiner._shader = shader;

    joiner._patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
    for (int i = 0; i < 4; i++) {
        (*joiner._patch)(i, 0) = (*patchW._patch)(i, 0);
        (*joiner._patch)(i, 1) = 2*(*patchW._patch)(i, 0) - (*patchW._patch)(i, 1);
        (*joiner._patch)(i, 2) = 2*(*patchN._patch)(3, i) - (*patchN._patch)(2, i);
        (*joiner._patch)(i, 3) = (*patchN._patch)(3, i);
    }

    joiner._u_lines = joiner._patch->GenerateUIsoparametricLines(5,1,100);
    joiner._v_lines = joiner._patch->GenerateVIsoparametricLines(5,1,100);

    for(GLuint i = 0; i < 5; i++)
    {
        (*joiner._u_lines)[i]->UpdateVertexBufferObjects();
        (*joiner._v_lines)[i]->UpdateVertexBufferObjects();
    }

    patchW._neighbors[W] = &joiner;
    patchN._neighbors[N] = &joiner;
    joiner._neighbors[W] = &patchN;
    joiner._neighbors[E] = &patchW;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesW_W(PatchAttributes &patchW1, PatchAttributes &patchW2, ShaderProgram *shader, Material *material)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];

    joiner._material = material;
    joiner._shader = shader;

    joiner._patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
    for (int i = 0; i < 4; i++) {
        (*joiner._patch)(i, 0) = (*patchW1._patch)(i, 0);
        (*joiner._patch)(i, 1) = 2*(*patchW1._patch)(i, 0) - (*patchW1._patch)(i, 1);
        (*joiner._patch)(i, 2) = 2*(*patchW2._patch)(i, 0) - (*patchW2._patch)(i, 1);
        (*joiner._patch)(i, 3) = (*patchW2._patch)(i, 0);
    }

    joiner._u_lines = joiner._patch->GenerateUIsoparametricLines(5,1,100);
    joiner._v_lines = joiner._patch->GenerateVIsoparametricLines(5,1,100);

    for(GLuint i = 0; i < 5; i++)
    {
        (*joiner._u_lines)[i]->UpdateVertexBufferObjects();
        (*joiner._v_lines)[i]->UpdateVertexBufferObjects();
    }

    patchW1._neighbors[W] = &joiner;
    patchW2._neighbors[W] = &joiner;
    joiner._neighbors[W] = &patchW1;
    joiner._neighbors[E] = &patchW2;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesW_S(PatchAttributes &patchW, PatchAttributes &patchS, ShaderProgram *shader, Material *material)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];

    joiner._material = material;
    joiner._shader = shader;

    joiner._patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
    for (int i = 0; i < 4; i++) {
        (*joiner._patch)(i, 0) = (*patchW._patch)(i, 0);
        (*joiner._patch)(i, 1) = 2*(*patchW._patch)(i, 0) - (*patchW._patch)(i, 1);
        (*joiner._patch)(i, 2) = 2*(*patchS._patch)(0, i) - (*patchS._patch)(1, i);
        (*joiner._patch)(i, 3) = (*patchS._patch)(0, i);
    }

    joiner._u_lines = joiner._patch->GenerateUIsoparametricLines(5,1,100);
    joiner._v_lines = joiner._patch->GenerateVIsoparametricLines(5,1,100);

    for(GLuint i = 0; i < 5; i++)
    {
        (*joiner._u_lines)[i]->UpdateVertexBufferObjects();
        (*joiner._v_lines)[i]->UpdateVertexBufferObjects();
    }

    patchW._neighbors[W] = &joiner;
    patchS._neighbors[S] = &joiner;
    joiner._neighbors[W] = &patchW;
    joiner._neighbors[E] = &patchS;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesE_E(PatchAttributes &patchE1, PatchAttributes &patchE2, ShaderProgram *shader, Material *material)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];

    joiner._material = material;
    joiner._shader = shader;

    joiner._patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
    for (int i = 0; i < 4; i++) {
        (*joiner._patch)(i, 0) = (*patchE1._patch)(i, 3);
        (*joiner._patch)(i, 1) = 2*(*patchE1._patch)(i, 3) - (*patchE1._patch)(i, 2);
        (*joiner._patch)(i, 2) = 2*(*patchE2._patch)(i, 3) - (*patchE2._patch)(i, 2);
        (*joiner._patch)(i, 3) = (*patchE2._patch)(i, 3);
    }

    joiner._u_lines = joiner._patch->GenerateUIsoparametricLines(5,1,100);
    joiner._v_lines = joiner._patch->GenerateVIsoparametricLines(5,1,100);

    for(GLuint i = 0; i < 5; i++)
    {
        (*joiner._u_lines)[i]->UpdateVertexBufferObjects();
        (*joiner._v_lines)[i]->UpdateVertexBufferObjects();
    }

    patchE1._neighbors[E] = &joiner;
    patchE2._neighbors[E] = &joiner;
    joiner._neighbors[W] = &patchE1;
    joiner._neighbors[E] = &patchE2;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesE_S(PatchAttributes &patchE, PatchAttributes &patchS, ShaderProgram *shader, Material *material)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];

    joiner._material = material;
    joiner._shader = shader;

    joiner._patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
    for (int i = 0; i < 4; i++) {
        (*joiner._patch)(i, 0) = (*patchE._patch)(i, 3);
        (*joiner._patch)(i, 1) = 2*(*patchE._patch)(i, 3) - (*patchE._patch)(i, 2);
        (*joiner._patch)(i, 2) = 2*(*patchS._patch)(0, i) - (*patchS._patch)(1, i);
        (*joiner._patch)(i, 3) = (*patchS._patch)(0, i);
    }

    joiner._u_lines = joiner._patch->GenerateUIsoparametricLines(5,1,100);
    joiner._v_lines = joiner._patch->GenerateVIsoparametricLines(5,1,100);

    for(GLuint i = 0; i < 5; i++)
    {
        (*joiner._u_lines)[i]->UpdateVertexBufferObjects();
        (*joiner._v_lines)[i]->UpdateVertexBufferObjects();
    }

    patchE._neighbors[E] = &joiner;
    patchS._neighbors[S] = &joiner;
    joiner._neighbors[W] = &patchE;
    joiner._neighbors[E] = &patchS;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesS_S(PatchAttributes &patchS1, PatchAttributes &patchS2, ShaderProgram *shader, Material *material)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];

    joiner._material = material;
    joiner._shader = shader;

    joiner._patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
    for (int i = 0; i < 4; i++) {
        (*joiner._patch)(i, 0) = (*patchS1._patch)(0, i);
        (*joiner._patch)(i, 1) = 2*(*patchS1._patch)(0, i) - (*patchS1._patch)(1, i);
        (*joiner._patch)(i, 2) = 2*(*patchS2._patch)(0, i) - (*patchS2._patch)(1, i);
        (*joiner._patch)(i, 3) = (*patchS2._patch)(0, i);
    }

    joiner._u_lines = joiner._patch->GenerateUIsoparametricLines(5,1,100);
    joiner._v_lines = joiner._patch->GenerateVIsoparametricLines(5,1,100);

    for(GLuint i = 0; i < 5; i++)
    {
        (*joiner._u_lines)[i]->UpdateVertexBufferObjects();
        (*joiner._v_lines)[i]->UpdateVertexBufferObjects();
    }

    patchS1._neighbors[E] = &joiner;
    patchS2._neighbors[S] = &joiner;
    joiner._neighbors[W] = &patchS1;
    joiner._neighbors[E] = &patchS2;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesNW_NW(PatchAttributes &patchNW1, PatchAttributes &patchNW2, ShaderProgram *shader, Material *material)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];

    joiner._material = material;
    joiner._shader = shader;

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


    joiner._u_lines = joiner._patch->GenerateUIsoparametricLines(5,1,100);
    joiner._v_lines = joiner._patch->GenerateVIsoparametricLines(5,1,100);

    for(GLuint i = 0; i < 5; i++)
    {
        (*joiner._u_lines)[i]->UpdateVertexBufferObjects();
        (*joiner._v_lines)[i]->UpdateVertexBufferObjects();
    }

    patchNW1._neighbors[NW] = &joiner;
    patchNW2._neighbors[NW] = &joiner;
    joiner._neighbors[SE] = &patchNW1;
    joiner._neighbors[NW] = &patchNW2;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesNW_NE(PatchAttributes &patchNW, PatchAttributes &patchNE, ShaderProgram *shader, Material *material)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];

    joiner._material = material;
    joiner._shader = shader;

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

    joiner._u_lines = joiner._patch->GenerateUIsoparametricLines(5,1,100);
    joiner._v_lines = joiner._patch->GenerateVIsoparametricLines(5,1,100);

    for(GLuint i = 0; i < 5; i++)
    {
        (*joiner._u_lines)[i]->UpdateVertexBufferObjects();
        (*joiner._v_lines)[i]->UpdateVertexBufferObjects();
    }

    patchNW._neighbors[NW] = &joiner;
    patchNE._neighbors[NE] = &joiner;
    joiner._neighbors[SE] = &patchNW;
    joiner._neighbors[NW] = &patchNE;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesNW_SE(PatchAttributes &patchNW, PatchAttributes &patchSE, ShaderProgram *shader, Material *material)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];

    joiner._material = material;
    joiner._shader = shader;

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

    joiner._u_lines = joiner._patch->GenerateUIsoparametricLines(5,1,100);
    joiner._v_lines = joiner._patch->GenerateVIsoparametricLines(5,1,100);

    for(GLuint i = 0; i < 5; i++)
    {
        (*joiner._u_lines)[i]->UpdateVertexBufferObjects();
        (*joiner._v_lines)[i]->UpdateVertexBufferObjects();
    }

    patchNW._neighbors[NW] = &joiner;
    patchSE._neighbors[SE] = &joiner;
    joiner._neighbors[SE] = &patchNW;
    joiner._neighbors[NW] = &patchSE;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesNW_SW(PatchAttributes &patchNW, PatchAttributes &patchSW, ShaderProgram *shader, Material *material)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];

    joiner._material = material;
    joiner._shader = shader;

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

    joiner._u_lines = joiner._patch->GenerateUIsoparametricLines(5,1,100);
    joiner._v_lines = joiner._patch->GenerateVIsoparametricLines(5,1,100);

    for(GLuint i = 0; i < 5; i++)
    {
        (*joiner._u_lines)[i]->UpdateVertexBufferObjects();
        (*joiner._v_lines)[i]->UpdateVertexBufferObjects();
    }

    patchNW._neighbors[NW] = &joiner;
    patchSW._neighbors[SW] = &joiner;
    joiner._neighbors[SE] = &patchNW;
    joiner._neighbors[NW] = &patchSW;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesNE_NE(PatchAttributes &patchNE1, PatchAttributes &patchNE2, ShaderProgram *shader, Material *material)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];

    joiner._material = material;
    joiner._shader = shader;

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

    joiner._u_lines = joiner._patch->GenerateUIsoparametricLines(5,1,100);
    joiner._v_lines = joiner._patch->GenerateVIsoparametricLines(5,1,100);

    for(GLuint i = 0; i < 5; i++)
    {
        (*joiner._u_lines)[i]->UpdateVertexBufferObjects();
        (*joiner._v_lines)[i]->UpdateVertexBufferObjects();
    }

    patchNE1._neighbors[NE] = &joiner;
    patchNE2._neighbors[NE] = &joiner;
    joiner._neighbors[SW] = &patchNE1;
    joiner._neighbors[NE] = &patchNE2;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesNE_SE(PatchAttributes &patchNE, PatchAttributes &patchSE, ShaderProgram *shader, Material *material)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];

    joiner._material = material;
    joiner._shader = shader;

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

    joiner._u_lines = joiner._patch->GenerateUIsoparametricLines(5,1,100);
    joiner._v_lines = joiner._patch->GenerateVIsoparametricLines(5,1,100);

    for(GLuint i = 0; i < 5; i++)
    {
        (*joiner._u_lines)[i]->UpdateVertexBufferObjects();
        (*joiner._v_lines)[i]->UpdateVertexBufferObjects();
    }

    patchNE._neighbors[NE] = &joiner;
    patchSE._neighbors[SE] = &joiner;
    joiner._neighbors[SW] = &patchNE;
    joiner._neighbors[NE] = &patchSE;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesNE_SW(PatchAttributes &patchNE, PatchAttributes &patchSW, ShaderProgram *shader, Material *material)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];

    joiner._material = material;
    joiner._shader = shader;

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

    joiner._u_lines = joiner._patch->GenerateUIsoparametricLines(5,1,100);
    joiner._v_lines = joiner._patch->GenerateVIsoparametricLines(5,1,100);

    for(GLuint i = 0; i < 5; i++)
    {
        (*joiner._u_lines)[i]->UpdateVertexBufferObjects();
        (*joiner._v_lines)[i]->UpdateVertexBufferObjects();
    }

    patchNE._neighbors[NE] = &joiner;
    patchSW._neighbors[SW] = &joiner;
    joiner._neighbors[SW] = &patchNE;
    joiner._neighbors[NE] = &patchSW;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesSE_SE(PatchAttributes &patchSE1, PatchAttributes &patchSE2, ShaderProgram *shader, Material *material)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];

    joiner._material = material;
    joiner._shader = shader;

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

    joiner._u_lines = joiner._patch->GenerateUIsoparametricLines(5,1,100);
    joiner._v_lines = joiner._patch->GenerateVIsoparametricLines(5,1,100);

    for(GLuint i = 0; i < 5; i++)
    {
        (*joiner._u_lines)[i]->UpdateVertexBufferObjects();
        (*joiner._v_lines)[i]->UpdateVertexBufferObjects();
    }

    patchSE1._neighbors[SE] = &joiner;
    patchSE2._neighbors[SE] = &joiner;
    joiner._neighbors[SE] = &patchSE1;
    joiner._neighbors[NW] = &patchSE2;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesSE_SW(PatchAttributes &patchSE, PatchAttributes &patchSW, ShaderProgram *shader, Material *material)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];

    joiner._material = material;
    joiner._shader = shader;

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

    joiner._u_lines = joiner._patch->GenerateUIsoparametricLines(5,1,100);
    joiner._v_lines = joiner._patch->GenerateVIsoparametricLines(5,1,100);

    for(GLuint i = 0; i < 5; i++)
    {
        (*joiner._u_lines)[i]->UpdateVertexBufferObjects();
        (*joiner._v_lines)[i]->UpdateVertexBufferObjects();
    }

    patchSE._neighbors[SE] = &joiner;
    patchSW._neighbors[NW] = &joiner;
    joiner._neighbors[SE] = &patchSE;
    joiner._neighbors[NW] = &patchSW;
    UpdatePatchVBOGenerateImage(joiner);
}

GLvoid FirstOrderAlgebraicTrigonometricSurface3::joinPatchesSW_SW(PatchAttributes &patchSW1, PatchAttributes &patchSW2, ShaderProgram *shader, Material *material)
{
    GLuint size = _attributes.size();

    PatchAttributes *oldAttr = &_attributes[0];
    _attributes.resize(size + 1);
    ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
    PatchAttributes &joiner =  _attributes[size];

    joiner._material = material;
    joiner._shader = shader;

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

    joiner._u_lines = joiner._patch->GenerateUIsoparametricLines(5,1,100);
    joiner._v_lines = joiner._patch->GenerateVIsoparametricLines(5,1,100);

    for(GLuint i = 0; i < 5; i++)
    {
        (*joiner._u_lines)[i]->UpdateVertexBufferObjects();
        (*joiner._v_lines)[i]->UpdateVertexBufferObjects();
    }

    patchSW1._neighbors[SE] = &joiner;
    patchSW2._neighbors[NW] = &joiner;
    joiner._neighbors[SE] = &patchSW1;
    joiner._neighbors[NW] = &patchSW2;
    UpdatePatchVBOGenerateImage(joiner);
}

GLboolean FirstOrderAlgebraicTrigonometricSurface3::continueExistingSurface(GLuint index, Direction direction, ShaderProgram *shader, Material* material)
{

    PatchAttributes& patch = _attributes[index];

        if(direction == E)
        {
            if(patch._neighbors[E] != nullptr)
                return GL_FALSE;

            GLuint n = _attributes.size();
            PatchAttributes *oldAttr = &_attributes[0];
            _attributes.resize(n + 1);
            ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
            PatchAttributes &newPatch = _attributes[n];
            newPatch._patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
            newPatch._material = material;
            newPatch._shader = shader;

            for(GLuint i = 0; i < 4; i++)
            {
                (*newPatch._patch)(i, 0) = (*patch._patch)(i, 3);
                (*newPatch._patch)(i, 1) = 2*(*patch._patch)(i, 3) - (*patch._patch)(i, 2);
                (*newPatch._patch)(i, 2) = 3*(*patch._patch)(i, 3) - 2*(*patch._patch)(i, 2);
                (*newPatch._patch)(i, 3) = 4*(*patch._patch)(i, 3) - 3*(*patch._patch)(i, 2);
            }

            newPatch._u_lines = newPatch._patch->GenerateUIsoparametricLines(5,1,100);
            newPatch._v_lines = newPatch._patch->GenerateVIsoparametricLines(5,1,100);

            for(GLuint i = 0; i < 5; i++)
            {
                (*newPatch._u_lines)[i]->UpdateVertexBufferObjects();
                (*newPatch._v_lines)[i]->UpdateVertexBufferObjects();
            }

            patch._neighbors[E] = &newPatch;
            newPatch._neighbors[W] = &patch;

            if (!UpdatePatchVBOGenerateImage(newPatch))
            {
                _attributes.pop_back();
                cout << "Could not update vertex buffer object or generate image!\n";
                return GL_FALSE;
            }
        }
        else if (direction == S)
        {
            if(patch._neighbors[S] != nullptr)
                return GL_FALSE;

            GLuint n = _attributes.size();
            PatchAttributes *oldAttr = &_attributes[0];
            _attributes.resize(n + 1);
            ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
            PatchAttributes &newPatch = _attributes[n];
            newPatch._patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
            newPatch._material = material;
            newPatch._shader = shader;

            for(GLuint i = 0; i < 4; i++)
            {
                (*newPatch._patch)(0, i) = 4*(*patch._patch)(0, i) - 3*(*patch._patch)(1, i);
                (*newPatch._patch)(1, i) = 3*(*patch._patch)(0, i) - 2*(*patch._patch)(1, i);
                (*newPatch._patch)(2, i) = 2*(*patch._patch)(0, i) - (*patch._patch)(1, i);
                (*newPatch._patch)(3, i) = (*patch._patch)(0, i);
            }

            newPatch._u_lines = newPatch._patch->GenerateUIsoparametricLines(5,1,100);
            newPatch._v_lines = newPatch._patch->GenerateVIsoparametricLines(5,1,100);

            for(GLuint i = 0; i < 5; i++)
            {
                (*newPatch._u_lines)[i]->UpdateVertexBufferObjects();
                (*newPatch._v_lines)[i]->UpdateVertexBufferObjects();
            }

            patch._neighbors[S] = &newPatch;
            newPatch._neighbors[N] = &patch;
            UpdatePatchVBOGenerateImage(newPatch);
        }
        else if(direction == N)
        {
            if(patch._neighbors[N] != nullptr)
                return GL_FALSE;

            GLuint n = _attributes.size();
            PatchAttributes *oldAttr = &_attributes[0];
            _attributes.resize(n + 1);
            ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
            PatchAttributes &newPatch = _attributes[n];
            newPatch._patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
            newPatch._material = material;
            newPatch._shader = shader;

            for(GLuint i = 0; i < 4; i++)
            {
                (*newPatch._patch)(i, 0) = 4*(*patch._patch)(3, i) - 3*(*patch._patch)(2, i);
                (*newPatch._patch)(i, 1) = 3*(*patch._patch)(3, i) - 2*(*patch._patch)(2, i);
                (*newPatch._patch)(i, 2) = 2*(*patch._patch)(3, i) - (*patch._patch)(2, i);
                (*newPatch._patch)(i, 3) = (*patch._patch)(3, i);
            }

            newPatch._u_lines = newPatch._patch->GenerateUIsoparametricLines(5,1,100);
            newPatch._v_lines = newPatch._patch->GenerateVIsoparametricLines(5,1,100);

            for(GLuint i = 0; i < 5; i++)
            {
                (*newPatch._u_lines)[i]->UpdateVertexBufferObjects();
                (*newPatch._v_lines)[i]->UpdateVertexBufferObjects();
            }


            patch._neighbors[N] = &newPatch;
            newPatch._neighbors[S] = &patch;
            UpdatePatchVBOGenerateImage(newPatch);
        }
        else if(direction == W)
        {
            if(patch._neighbors[W] != nullptr)
                return GL_FALSE;

            GLuint n = _attributes.size();
            PatchAttributes *oldAttr = &_attributes[0];
            _attributes.resize(n + 1);
            ValidatePointersInPatchAttrs(oldAttr, &_attributes[0]);
            PatchAttributes &newPatch = _attributes[n];
            newPatch._patch = new FirstOrderAlgebraicTrigonometricPatch(1.0, 1.0);
            newPatch._material = material;
            newPatch._shader = shader;

            for(GLuint i = 0; i < 4; i++)
            {
                (*newPatch._patch)(0, i) = (*patch._patch)(i, 0);
                (*newPatch._patch)(1, i) = 2*(*patch._patch)(i, 0) - (*patch._patch)(i, 1);
                (*newPatch._patch)(2, i) = 3*(*patch._patch)(i, 0) - 2*(*patch._patch)(i, 1);
                (*newPatch._patch)(3, i) = 4*(*patch._patch)(i, 0) - 3*(*patch._patch)(i, 1);
            }

            newPatch._u_lines = newPatch._patch->GenerateUIsoparametricLines(5,1,100);
            newPatch._v_lines = newPatch._patch->GenerateVIsoparametricLines(5,1,100);

            for(GLuint i = 0; i < 5; i++)
            {
                (*newPatch._u_lines)[i]->UpdateVertexBufferObjects();
                (*newPatch._v_lines)[i]->UpdateVertexBufferObjects();
            }

            patch._neighbors[W] = &newPatch;
            newPatch._neighbors[E] = &patch;
            UpdatePatchVBOGenerateImage(newPatch);
        }
        return GL_TRUE;
}
