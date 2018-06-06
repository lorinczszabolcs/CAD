#include "FirstOrderAlgebraicTrigonometricPatches3.h"
#include "../Core/TriangulatedMeshes3.h"
#include "../Core/Materials.h"
#include "../Core/ShaderPrograms.h"

namespace cagd
{
    class FirstOrderAlgebraicTrigonometricSurface3
    {
    public:
        enum Direction{N = 0, NE, E, SE, S, SW, W, NW};
        class PatchAttributes
        {
        public:
            FirstOrderAlgebraicTrigonometricPatch  *_patch;
            TriangulatedMesh3                      *_image;
            Material                               *_material;
            RowMatrix<GenericCurve3*>              *_u_lines;
            RowMatrix<GenericCurve3*>              *_v_lines;
            //ShaderProgram                          *_shader;
            // get-set, ctor, dtor, copy ctor
            PatchAttributes                        *_neighbors[8];
            PatchAttributes();
            ~PatchAttributes();
            PatchAttributes(const PatchAttributes &attr);
            PatchAttributes& operator =(const PatchAttributes &attr);
        };

        protected:
            //GLdouble _alpha_u, _alpha_v;
            std::vector<PatchAttributes> _attributes;
        public:
            // ctor, copy ctor, dtor, op=
            FirstOrderAlgebraicTrigonometricSurface3();
            FirstOrderAlgebraicTrigonometricSurface3(const FirstOrderAlgebraicTrigonometricSurface3 &surface);
            ~FirstOrderAlgebraicTrigonometricSurface3();
            FirstOrderAlgebraicTrigonometricSurface3& operator=(const FirstOrderAlgebraicTrigonometricSurface3 & surface);

            // insert, join, merge, continue, erase, stb.
            //megvan
            GLboolean insert(Material *material = &MatFBBrass);
            GLboolean insertIsolatedSurface(FirstOrderAlgebraicTrigonometricPatch &patch, Material *material = &MatFBBrass);
            GLboolean continueExistingSurface(GLuint index, Direction direction, Material *material);
            //megvan
            GLboolean joinExistingSurface(GLuint index1, GLuint index2, Direction direction1, Direction direction2, Material *material);
            //megvan
            GLboolean mergeExistingSurface(GLuint index1, GLuint index2, Direction direction1, Direction direction2);
            //megvan
            GLvoid eraseExistingSurface(GLuint index);

            //render
            GLboolean renderSurfaces(GLboolean u_lin, GLboolean v_lin, GLboolean control, GLboolean surf);

            //megvan
            GLvoid setMaterial(GLuint index, Material *material);

            //megvan
            GLboolean UpdatePatchVBOGenerateImage(PatchAttributes &newPatch);

            GLint getAttributesSize();
        private:
            GLvoid ValidatePointersInPatchAttrs(PatchAttributes *oldAttr, PatchAttributes *newAttr);

            //GLboolean mergePatches(GLuint ind1, GLuint ind2, Direction dir1, Direction dir2);
            GLboolean mergePatchesN_N(FirstOrderAlgebraicTrigonometricPatch &patchN1, FirstOrderAlgebraicTrigonometricPatch &patchN2);
            GLboolean mergePatchesN_E(FirstOrderAlgebraicTrigonometricPatch &patchN, FirstOrderAlgebraicTrigonometricPatch &patchE);
            GLboolean mergePatchesN_S(FirstOrderAlgebraicTrigonometricPatch &patchN, FirstOrderAlgebraicTrigonometricPatch &patchS);
            GLboolean mergePatchesW_E(FirstOrderAlgebraicTrigonometricPatch &patchW, FirstOrderAlgebraicTrigonometricPatch &patchE);
            GLboolean mergePatchesW_N(FirstOrderAlgebraicTrigonometricPatch &patchW, FirstOrderAlgebraicTrigonometricPatch &patchN);
            GLboolean mergePatchesW_W(FirstOrderAlgebraicTrigonometricPatch &patchW1, FirstOrderAlgebraicTrigonometricPatch &patchW2);
            GLboolean mergePatchesW_S(FirstOrderAlgebraicTrigonometricPatch &patchW, FirstOrderAlgebraicTrigonometricPatch &patchS);
            GLboolean mergePatchesE_E(FirstOrderAlgebraicTrigonometricPatch &patchE1, FirstOrderAlgebraicTrigonometricPatch &patchE2);
            GLboolean mergePatchesE_S(FirstOrderAlgebraicTrigonometricPatch &patchE, FirstOrderAlgebraicTrigonometricPatch &patchS);
            GLboolean mergePatchesS_S(FirstOrderAlgebraicTrigonometricPatch &patchS1, FirstOrderAlgebraicTrigonometricPatch &patchS2);

            //GLboolean joinPatches(GLuint ind1, GLuint ind2, Direction dir1, Direction dir2);
            GLvoid joinPatchesN_N(PatchAttributes &patchN, PatchAttributes &patchN2, Material *material);
            GLvoid joinPatchesN_E(PatchAttributes &patchN, PatchAttributes &patchE, Material *material);
            GLvoid joinPatchesN_S(PatchAttributes &patchN, PatchAttributes &patchS, Material *material);
            GLvoid joinPatchesW_E(PatchAttributes &patchW, PatchAttributes &patchE, Material *material);
            GLvoid joinPatchesW_N(PatchAttributes &patchW, PatchAttributes &patchN, Material *material);
            GLvoid joinPatchesW_W(PatchAttributes &patchW, PatchAttributes &patchW2, Material* material);
            GLvoid joinPatchesW_S(PatchAttributes &patchW, PatchAttributes &patchS, Material* material);
            GLvoid joinPatchesE_E(PatchAttributes &patchE, PatchAttributes &patchE2, Material* material);
            GLvoid joinPatchesE_S(PatchAttributes &patchE, PatchAttributes &patchS, Material* material);
            GLvoid joinPatchesS_S(PatchAttributes &patchS, PatchAttributes &patchS2, Material* material);

            GLvoid joinPatchesNW_NW(PatchAttributes &patchNW, PatchAttributes &patchNW2, Material* material);
            GLvoid joinPatchesNW_NE(PatchAttributes &patchNW, PatchAttributes &patchNE, Material* material);
            GLvoid joinPatchesNW_SE(PatchAttributes &patchWE, PatchAttributes &patchSE, Material* material);
            GLvoid joinPatchesNW_SW(PatchAttributes &patchNW, PatchAttributes &patchSW, Material* material);
            GLvoid joinPatchesNE_NE(PatchAttributes &patchNE, PatchAttributes &patchNE2, Material* material);
            GLvoid joinPatchesNE_SE(PatchAttributes &patchNE, PatchAttributes &patchSE, Material* material);
            GLvoid joinPatchesNE_SW(PatchAttributes &patchNE, PatchAttributes &patchSW, Material* material);
            GLvoid joinPatchesSE_SE(PatchAttributes &patchSE, PatchAttributes &patchSE2, Material* material);
            GLvoid joinPatchesSE_SW(PatchAttributes &patchSE, PatchAttributes &patchSW, Material* material);
            GLvoid joinPatchesSW_SW(PatchAttributes &patchSW, PatchAttributes &patchSW2, Material* material);
    };
}
