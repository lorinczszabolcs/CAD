#include "FirstOrderAlgebraicTrigonometricCompositeSurfaces3.h"
#include "Core/Materials.h"

using namespace cagd;

FirstOrderAlgebraicTrigonometricSurface3::PatchAttributes::PatchAttributes() :
    _patch(new FirstOrderAlgebraicTrigonometricPatch())
{}

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

FirstOrderAlgebraicTrigonometricSurface3::PatchAttributes& FirstOrderAlgebraicTrigonometricSurface3::PatchAttributes::operator=(const PatchAttributes &attr)
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
        this->_patch = new FirstOrderAlgebraicTrigonometricPatch();
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

FirstOrderAlgebraicTrigonometricSurface3::FirstOrderAlgebraicTrigonometricSurface3(const FirstOrderAlgebraicTrigonometricSurface3 &surface)
{
    for(std::vector<PatchAttributes>::iterator i = _attributes.begin(); i != _attributes.end(); i++)
    {

    }
}
