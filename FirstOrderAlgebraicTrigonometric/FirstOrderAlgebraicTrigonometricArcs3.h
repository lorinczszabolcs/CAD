#pragma once

#include "../Core/LinearCombination3.h"

namespace cagd
{
    class FirstOrderAlgebraicTrigonometricArc3 : public LinearCombination3
    {
    protected:
        GLdouble _alpha; // shape parameters


    public:

        FirstOrderAlgebraicTrigonometricArc3(GLdouble alpha);
        GLboolean BlendingFunctionValues(GLdouble u, RowMatrix<GLdouble> &values) const;
        GLboolean CalculateDerivatives(GLuint max_order_of_derivatives, GLdouble u, Derivatives &d) const;
        GLdouble  f2(GLdouble t) const;
        GLdouble  f3(GLdouble t) const;
        GLdouble  d_f2(GLdouble t) const;
        GLdouble  d_f3(GLdouble t) const;
        GLdouble  d2_f2(GLdouble t) const;
        GLdouble  d2_f3(GLdouble t) const;

        GLdouble getAlpha() const;
        GLvoid setAlpha(GLdouble alpha);

        DCoordinate3 GetData(GLuint index);
        GLvoid SetData(GLuint point_index, GLdouble x, GLdouble y, GLdouble z);

    };
}
