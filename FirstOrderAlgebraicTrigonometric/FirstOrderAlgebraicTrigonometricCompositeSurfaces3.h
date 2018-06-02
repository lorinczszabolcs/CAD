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
            GLvoid insertIsolatedSurface(FirstOrderAlgebraicTrigonometricPatch &patch);
            GLvoid continueExistingSurface(GLuint index, Direction direction);
            GLvoid joinExistingSurface(GLuint index1, GLuint index2, Direction direction1, Direction direction2);
            GLvoid mergeExistingSurface(GLuint index1, GLuint index2, Direction direction1, Direction direction2);
            GLvoid eraseExistingSurface(GLuint index);

            //render
            GLboolean renderSurfaces();

            GLvoid UpdatePatchVBOGenerateImage(PatchAttributes &newPatch);
        private:
            GLvoid ValidatePointersInPatchAttrs(PatchAttributes *oldAttr, PatchAttributes *newAttr);

            GLboolean mergePatches(GLuint ind1, GLuint ind2, Direction dir1, Direction dir2);
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

            GLboolean joinPatches(GLuint ind1, GLuint ind2, Direction dir1, Direction dir2);
            GLvoid joinPatchesN_N(PatchAttributes &patchN, PatchAttributes &patchN2);
            GLvoid joinPatchesN_E(PatchAttributes &patchN, PatchAttributes &patchE);
            GLvoid joinPatchesN_S(PatchAttributes &patchN, PatchAttributes &patchS);
            GLvoid joinPatchesW_E(PatchAttributes &patchW, PatchAttributes &patchE);
            GLvoid joinPatchesW_N(PatchAttributes &patchW, PatchAttributes &patchN);
            GLvoid joinPatchesW_W(PatchAttributes &patchW, PatchAttributes &patchW2);
            GLvoid joinPatchesW_S(PatchAttributes &patchW, PatchAttributes &patchS);
            GLvoid joinPatchesE_E(PatchAttributes &patchE, PatchAttributes &patchE2);
            GLvoid joinPatchesE_S(PatchAttributes &patchE, PatchAttributes &patchS);
            GLvoid joinPatchesS_S(PatchAttributes &patchS, PatchAttributes &patchS2);

            GLvoid joinPatchesNW_NW(PatchAttributes &patchNW, PatchAttributes &patchNW2);
            GLvoid joinPatchesNW_NE(PatchAttributes &patchNW, PatchAttributes &patchNE);
            GLvoid joinPatchesNW_SE(PatchAttributes &patchWE, PatchAttributes &patchSE);
            GLvoid joinPatchesNW_SW(PatchAttributes &patchNW, PatchAttributes &patchSW);
            GLvoid joinPatchesNE_NE(PatchAttributes &patchNE, PatchAttributes &patchNE2);
            GLvoid joinPatchesNE_SE(PatchAttributes &patchNE, PatchAttributes &patchSE);
            GLvoid joinPatchesNE_SW(PatchAttributes &patchNE, PatchAttributes &patchSW);
            GLvoid joinPatchesSE_SE(PatchAttributes &patchSE, PatchAttributes &patchSE2);
            GLvoid joinPatchesSE_SW(PatchAttributes &patchSE, PatchAttributes &patchSW);
            GLvoid joinPatchesSW_SW(PatchAttributes &patchSW, PatchAttributes &patchSW2);
    };
}
