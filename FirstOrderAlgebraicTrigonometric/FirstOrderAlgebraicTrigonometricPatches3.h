#include "../Core/TensorProductSurfaces3.h"

namespace cagd
{
    class FirstOrderAlgebraicTrigonometricPatch : public TensorProductSurface3
    {
    protected:
        GLdouble _alpha_u, _alpha_v;

    public:
        FirstOrderAlgebraicTrigonometricPatch(GLdouble alpha_u, GLdouble alpha_v);
        GLboolean UBlendingFunctionValues(GLdouble u_knot, RowMatrix<GLdouble> &blending_values) const;
        GLboolean VBlendingFunctionValues(GLdouble v_knot, RowMatrix<GLdouble> &blending_values) const;
        GLboolean CalculatePartialDerivatives(GLuint maximum_order_of_partial_derivatives, GLdouble u, GLdouble v, PartialDerivatives &pd) const;
        GLdouble  f2(GLdouble t) const;
        GLdouble  f3(GLdouble t) const;
        GLdouble  d_f2(GLdouble t) const;
        GLdouble  d_f3(GLdouble t) const;
    };
}
