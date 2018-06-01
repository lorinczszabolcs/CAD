#include "FirstOrderAlgebraicTrigonometricPatches3.h"
#include <cmath>

using namespace cagd;

FirstOrderAlgebraicTrigonometricPatch::FirstOrderAlgebraicTrigonometricPatch(GLdouble _alpha_u, GLdouble _alpha_v): TensorProductSurface3(0.0, 1.0, 0.0, 1.0, 4, 4, GL_TRUE, GL_TRUE)
{
    this->_alpha_u = _alpha_u;
    this->_alpha_v = _alpha_v;
}

GLdouble FirstOrderAlgebraicTrigonometricPatch::getAlphaU() const
{
    return _alpha_u;
}
void FirstOrderAlgebraicTrigonometricPatch::setAlphaU(GLdouble alpha_u)
{
    _alpha_u = alpha_u;
}

GLdouble FirstOrderAlgebraicTrigonometricPatch::getAlphaV() const
{
    return _alpha_v;
}

void FirstOrderAlgebraicTrigonometricPatch::setAlphaV(GLdouble alpha_v)
{
    _alpha_v = alpha_v;
}

GLdouble FirstOrderAlgebraicTrigonometricPatch::f2(GLdouble t) const
{

    GLdouble numerator, denominator;

    numerator = (_alpha_u - t + sin(_alpha_u - t) + sin(t) - sin(_alpha_u) + t * cos(_alpha_u) - _alpha_u * cos(t)) * sin(_alpha_u);
    denominator = (_alpha_u - sin(_alpha_u)) * (2 * sin(_alpha_u) - _alpha_u - _alpha_u * cos(_alpha_u));

    return numerator / denominator;
}

GLdouble FirstOrderAlgebraicTrigonometricPatch::f3(GLdouble t) const
{
    return (t - sin(t))/(_alpha_u - sin(_alpha_u));
}

GLdouble FirstOrderAlgebraicTrigonometricPatch::d_f2(GLdouble t) const
{
    return (-sin(t - _alpha_u) + sin(2*t - _alpha_u) + (t - _alpha_u)*cos(t) + _alpha_u*cos(2*t) - 1.0/2*t*cos(t - _alpha_u) - 1.0/2*t*cos(_alpha_u + t) + sin(t) - sin(2*t))/((_alpha_u - sin(_alpha_u))*(_alpha_u - 2*sin(_alpha_u) + _alpha_u*cos(_alpha_u)));

}

GLdouble FirstOrderAlgebraicTrigonometricPatch::d_f3(GLdouble t) const
{
    return (1 - cos(t))/(_alpha_u - sin(_alpha_u));
}

GLboolean FirstOrderAlgebraicTrigonometricPatch::UBlendingFunctionValues(GLdouble u_knot, RowMatrix<GLdouble> &blending_values) const
{
    if((u_knot < 0.0) || (u_knot > 1.0))
        return GL_FALSE;

    blending_values.ResizeColumns(4);

    blending_values(0) = f3(_alpha_u - u_knot);
    blending_values(1) = f2(_alpha_u - u_knot);
    blending_values(2) = f2(u_knot);
    blending_values(3) = f3(u_knot);

    return GL_TRUE;
}

GLboolean FirstOrderAlgebraicTrigonometricPatch::VBlendingFunctionValues(GLdouble v_knot, RowMatrix<GLdouble> &blending_values) const
{
    if((v_knot < 0.0) || (v_knot > 1.0))
        return GL_FALSE;

    blending_values.ResizeColumns(4);

    blending_values(0) = f3(_alpha_u - v_knot);
    blending_values(1) = f2(_alpha_u - v_knot);
    blending_values(2) = f2(v_knot);
    blending_values(3) = f3(v_knot);

    return GL_TRUE;
}

GLboolean FirstOrderAlgebraicTrigonometricPatch::CalculatePartialDerivatives(GLuint maximum_order_of_partial_derivatives, GLdouble u, GLdouble v, PartialDerivatives &pd) const
{
    if(((u < 0.0) || (u > 1.0)) || ((v < 0.0) || (v > 1.0)) || (maximum_order_of_partial_derivatives > 1))
        return GL_FALSE;

    RowMatrix<GLdouble> u_blending_values(4), d1_u_blending_values(4);

    u_blending_values(0) = f3(_alpha_u - u);
    u_blending_values(1) = f2(_alpha_u - u);
    u_blending_values(2) = f2(u);
    u_blending_values(3) = f3(u);

    d1_u_blending_values(0) = -d_f3(_alpha_u - u);
    d1_u_blending_values(1) = -d_f2(_alpha_u - u);
    d1_u_blending_values(2) = d_f2(u);
    d1_u_blending_values(3) = d_f3(u);

     RowMatrix<GLdouble> v_blending_values(4), d1_v_blending_values(4);

     v_blending_values(0) = f3(_alpha_v - v);
     v_blending_values(1) = f2(_alpha_v - v);
     v_blending_values(2) = f2(v);
     v_blending_values(3) = f3(v);

     d1_v_blending_values(0) = -d_f3(_alpha_v - v);
     d1_v_blending_values(1) = -d_f2(_alpha_v - v);
     d1_v_blending_values(2) = d_f2(v);
     d1_v_blending_values(3) = d_f3(v);

     pd.ResizeRows(2);
     pd.LoadNullVectors();
     for(GLuint row = 0; row < 4; ++row)
     {
         DCoordinate3 aux_d0_v, aux_d1_v;
         for(GLuint column = 0; column < 4; ++column)
         {
            aux_d0_v += _data(row, column) * v_blending_values(column);
            aux_d1_v += _data(row, column) *d1_v_blending_values(column);
         }
         pd(0, 0) += aux_d0_v * u_blending_values(row);
         pd(1, 0) += aux_d0_v * d1_u_blending_values(row);
         pd(1, 1) += aux_d1_v * u_blending_values(row);
     }
     return GL_TRUE;
}


