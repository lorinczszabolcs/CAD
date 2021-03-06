#include "FirstOrderAlgebraicTrigonometricArcs3.h"
#include "../Core/Colors4.h"

namespace cagd
{
    class FirstOrderAlgebraicTrigonometricCompositeCurve3
    {
    public:
        enum Direction{LEFT, RIGHT};
        class ArcAttributes
        {
        public:
            FirstOrderAlgebraicTrigonometricArc3 *_arc;
            GenericCurve3                        *_image;
            Color4                               *_color;
            GLuint                               _mod; //max order of derivates
            GLuint                               _div_point_count; //div point count = 200


            ArcAttributes                        *_next, *_previous;
            ArcAttributes();
            ~ArcAttributes();
            ArcAttributes(const ArcAttributes &arc_atr);
            ArcAttributes& operator =(const ArcAttributes &arc_atr);

        };

    protected:
        std::vector<ArcAttributes> _attributes;
        GLdouble                   _alpha;

    public:
        FirstOrderAlgebraicTrigonometricCompositeCurve3(GLdouble alpha, GLuint max_arc_count = 1000);

        FirstOrderAlgebraicTrigonometricCompositeCurve3();

        // insert new isolated arc
        GLboolean insert(Color4 *color = new Color4());
        GLboolean insertIsolatedArc(FirstOrderAlgebraicTrigonometricArc3 &trigArc, GLdouble mod = 2, GLdouble div_point_count = 200, Color4 *color = new Color4(0.4, 0.2, 0.3));
        GLvoid validateAttributes(ArcAttributes *oldAttr, ArcAttributes *newAttr);

        GLboolean updateArcVBOGenerateImage(ArcAttributes &arc);

        // continue existing arc -> index, direction, haromszorosan megnyujtani, degeneralodik szakasz
        GLboolean continueExistingArc(GLuint index, Direction direction, GLdouble mod = 2, GLdouble div_point_count = 200, Color4 *color = new Color4(0.4, 0.2, 0.3));

        // join existing arcs -> index1, index2 (attributes), direction1, direction2 -> ketto koze ujat
        GLboolean joinExistingArc(GLuint index1, GLuint index2, Direction direction1, Direction direction2, GLdouble mod = 2, GLdouble div_point_count = 200, Color4 *color = new Color4(0.4, 0.2, 0.3));

        // merge existing arcs -> index1, index2 (attributes), direction1, direction2 -> rendertex labelekhez -> kettot ossze
        GLboolean mergeExistingArc(GLuint index1, GLuint index2, Direction direction1, Direction direction2);

        // erase existing arc
        GLboolean eraseExistingArc(GLuint index);

        GLvoid validatePointersInArcAttr(ArcAttributes *oldAttr, ArcAttributes *newAttr);

        // render
        GLvoid renderControlPolygon();
        GLboolean renderCurves(GLuint order, GLenum render_mode);

        // get-set -> GUI
        DCoordinate3 getData(GLint arc_index, GLint point_index);
        GLvoid setData(GLint arc_index, GLint point_index, GLdouble x, GLdouble y, GLdouble z);
        GLint getAttributesSize();

        // dtor
        ~FirstOrderAlgebraicTrigonometricCompositeCurve3();

        // copy ctor
        FirstOrderAlgebraicTrigonometricCompositeCurve3(const FirstOrderAlgebraicTrigonometricCompositeCurve3 &curve);

        // operator=
        FirstOrderAlgebraicTrigonometricCompositeCurve3& operator=(const FirstOrderAlgebraicTrigonometricCompositeCurve3 &curve);

    };
}
