#pragma once

#include "../FirstOrderAlgebraicTrigonometric/FirstOrderAlgebraicTrigonometricArcs3.h"

using namespace cagd;


FirstOrderAlgebraicTrigonometricArc3::FirstOrderAlgebraicTrigonometricArc3(GLdouble alpha) : LinearCombination3(0.0, alpha, 4)
{
    this->_alpha = alpha;
}

GLdouble FirstOrderAlgebraicTrigonometricArc3::getAlpha() const
{
    return _alpha;
}

GLvoid FirstOrderAlgebraicTrigonometricArc3::setAlpha(GLdouble alpha)
{
    this->_alpha = alpha;
}

GLdouble FirstOrderAlgebraicTrigonometricArc3::f2(GLdouble t) const
{

    GLdouble numerator, denominator;

    numerator = (_alpha - t + sin(_alpha - t) + sin(t) - sin(_alpha) + t * cos(_alpha) - _alpha * cos(t)) * sin(_alpha);
    denominator = (_alpha - sin(_alpha)) * (2 * sin(_alpha) - _alpha - _alpha * cos(_alpha));

    return numerator / denominator;
}

GLdouble FirstOrderAlgebraicTrigonometricArc3::f3(GLdouble t) const
{
    return (t - sin(t))/(_alpha - sin(_alpha));
}

GLdouble FirstOrderAlgebraicTrigonometricArc3::d_f2(GLdouble t) const
{
    return (-sin(t - _alpha) + sin(2*t - _alpha) + (t - _alpha)*cos(t) + _alpha*cos(2*t) - 1.0/2*t*cos(t - _alpha) - 1.0/2*t*cos(_alpha + t) + sin(t) - sin(2*t))/((_alpha - sin(_alpha))*(_alpha - 2*sin(_alpha) + _alpha*cos(_alpha)));

}

GLdouble FirstOrderAlgebraicTrigonometricArc3::d_f3(GLdouble t) const
{
    return (1 - cos(t))/(_alpha - sin(_alpha));
}

GLdouble FirstOrderAlgebraicTrigonometricArc3::d2_f2(GLdouble t) const
{
    return (-cos(t - _alpha) + 2.0*cos(2*t - _alpha) + cos(t) - t*sin(t) + _alpha*sin(t) - 2.0*_alpha*sin(2*t) - 1.0/2*(cos(t - _alpha)-t*sin(t - _alpha)) - 1.0/2*(cos(_alpha + t)-t*sin(_alpha + t)) + cos(t) - 2.0*cos(2*t))/((_alpha - sin(_alpha))*(_alpha - 2*sin(_alpha) + _alpha*cos(_alpha)));

}

GLdouble FirstOrderAlgebraicTrigonometricArc3::d2_f3(GLdouble t) const
{
    return sin(t)/(_alpha - sin(_alpha));
}


GLboolean FirstOrderAlgebraicTrigonometricArc3::BlendingFunctionValues(GLdouble u, RowMatrix<GLdouble> &values) const
{
    if (u < 0.0 || u > 1.0)
    {
        return GL_FALSE;
    }

    values.ResizeColumns(4);

    values(0) = f3(_alpha - u);
    values(1) = f2(_alpha - u);
    values(2) = f2(u);
    values(3) = f3(u);

    return GL_TRUE;
}

GLboolean FirstOrderAlgebraicTrigonometricArc3::CalculateDerivatives(GLuint max_order_of_derivatives, GLdouble u, Derivatives &d) const
{
    if (max_order_of_derivatives > 3)
    {
        d.ResizeRows(0);
        return GL_FALSE;
    }

    Matrix<GLdouble> blending_values(max_order_of_derivatives + 1, _data.GetRowCount());

    blending_values(0,0) = f3(_alpha - u);
    blending_values(0,1) = f2(_alpha - u);
    blending_values(0,2) = f2(u);
    blending_values(0,3) = f3(u);

    // TODO: higher order derivatives
    if (max_order_of_derivatives >= 1)
    {
        blending_values(1,0) = -d_f3(_alpha - u);
        blending_values(1,1) = -d_f2(_alpha - u);
        blending_values(1,2) = d_f2(u);
        blending_values(1,3) = d_f3(u);
    }

    if (max_order_of_derivatives >= 2)
    {
        blending_values(2,0) = d2_f3(_alpha - u);
        blending_values(2,1) = d2_f2(_alpha - u);
        blending_values(2,2) = d2_f2(u);
        blending_values(2,3) = d2_f3(u);
    }

    d.ResizeRows(max_order_of_derivatives + 1);
    d.LoadNullVectors();

    for (GLuint order = 0; order <= max_order_of_derivatives; order++)
    {
        for(GLuint i = 0; i < _data.GetRowCount(); i++)
        {
            d[order] += _data[i] * blending_values(order, i);
        }
    }
    return GL_TRUE;
}

DCoordinate3 FirstOrderAlgebraicTrigonometricArc3::GetData(GLuint point_index)
{
    return DCoordinate3((*this)[point_index].x(), (*this)[point_index].y(), (*this)[point_index].z());
}

GLvoid FirstOrderAlgebraicTrigonometricArc3::SetData(GLuint point_index, GLdouble x, GLdouble y, GLdouble z)
{
    (*this)[point_index].x() = x;
    (*this)[point_index].y() = y;
    (*this)[point_index].z() = z;
}
