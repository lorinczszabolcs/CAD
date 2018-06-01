#include "FirstOrderAlgebraicTrigonometricArcs3.h"
#include "../Core/Colors4.h"

namespace cagd
{
    class FirstOrderAlgebraicTrigonometricCompositeCurve3
    {
    public:
        enum Direction{LEFT = 0, RIGHT};
        class ArcAttributes
        {
        public:
            FirstOrderAlgebraicTrigonometricArc3 *_arc;
            GenericCurve3                        *_image;
            Color4                               *_color;

            ArcAttributes                        *_next, *_previous;
            ArcAttributes();
            ~ArcAttributes();
            ArcAttributes(const ArcAttributes &arc_atr);
            ArcAttributes& operator =(const ArcAttributes &arc_atr);
        };

    protected:
        std::vector<ArcAttributes> _attributes;
        GLdouble                   _alpha;

        GLuint                     _mod; //max order of derivates
        GLuint                     _div_point_count; //div point count = 200

    public:
        FirstOrderAlgebraicTrigonometricCompositeCurve3(GLdouble alpha, GLuint max_arc_count = 1000);

        FirstOrderAlgebraicTrigonometricCompositeCurve3();

        // insert new isolated arc
        GLboolean insertIsolatedArc(FirstOrderAlgebraicTrigonometricArc3 &trigArc);
        GLvoid validateAttributes(ArcAttributes *oldAttr, ArcAttributes *newAttr);

        GLvoid updateArcVBOGenerateImage(ArcAttributes &arc);

        // continue existing arc -> index, direction, haromszorosan megnyujtani, degeneralodik szakasz
        GLboolean continueExistingArc(GLuint index, Direction direction);

        // join existing arcs -> index1, index2 (attributes), direction1, direction2 -> ketto koze ujat
        GLboolean joinExistingArc(GLuint index1, GLuint index2, Direction direction1, Direction direction2);

        // merge existing arcs -> index1, index2 (attributes), direction1, direction2 -> rendertex labelekhez -> kettot ossze
        GLboolean mergeExistingArc(GLuint index1, GLuint index2, Direction direction1, Direction direction2);

        // erase existing arc
        GLboolean eraseExistingArc(GLuint index);

        GLvoid validatePointersInArcAttr(ArcAttributes *oldAttr, ArcAttributes *newAttr);

        // render
        GLvoid renderControlPolygon();
        GLboolean renderCurves(GLuint order, GLenum render_mode);
        GLboolean renderDerivatives(GLuint order, GLenum render_mode);

        // get-set -> GUI

        // dtor
        ~FirstOrderAlgebraicTrigonometricCompositeCurve3();

        // copy ctor
        FirstOrderAlgebraicTrigonometricCompositeCurve3(const FirstOrderAlgebraicTrigonometricCompositeCurve3 &curve);

        // operator=
        FirstOrderAlgebraicTrigonometricCompositeCurve3& operator=(const FirstOrderAlgebraicTrigonometricCompositeCurve3 &curve);
    };
}
