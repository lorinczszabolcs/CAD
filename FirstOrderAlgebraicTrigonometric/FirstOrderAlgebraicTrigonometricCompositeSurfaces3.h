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
            FirstOrderAlgebraicTrigonometricPatch  *patch;
            TriangulatedMesh3                      *image;
            Material                               *material;
            ShaderProgram                          *shader;
            // get-set, ctor, dtor, copy ctor
            PatchAttributes                        *neighbors[8];
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
            GLvoid eraseExistingArc(GLuint index);
            //render
    };
}
