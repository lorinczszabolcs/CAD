#include <cmath>
#include "TestFunctions.h"
#include "../Core/Constants.h"

using namespace cagd;
using namespace std;

GLdouble spiral_on_cone::u_min = -TWO_PI;
GLdouble spiral_on_cone::u_max = +TWO_PI;

DCoordinate3 spiral_on_cone::d0(GLdouble u)
{
    return DCoordinate3(u * cos(u), u * sin(u), u);
}

DCoordinate3 spiral_on_cone::d1(GLdouble u)
{
    GLdouble c = cos(u), s = sin(u);
    return DCoordinate3(c - u * s, s + u * c, 1.0);
}

DCoordinate3 spiral_on_cone::d2(GLdouble u)
{
    GLdouble c = cos(u), s = sin(u);
    return DCoordinate3(-2.0 * s - u * c, 2.0 * c - u * s, 0);
}

GLdouble butterfly::u_min = -PI;
GLdouble butterfly::u_max = PI;

DCoordinate3 butterfly::d0(GLdouble u)
{
    GLdouble c = cos(u), s = sin(u);
    GLdouble x = s * (pow(EULER,cos(u)) - 2 * cos(4 * u) - pow(sin(u / 12),5));
    GLdouble y = c * (pow(EULER,c) - 2 * cos(4 * u) - pow(sin(u / 12),5));
    return DCoordinate3(x, y, u);
}

DCoordinate3 butterfly::d1(GLdouble u)
{
    GLdouble c = cos(u), s = sin(u);
    GLdouble x = c * (-pow(sin(u/12),5) + pow(EULER,c) - 2 * cos(4*u)) + s * ( 8*sin(4*u)-5/12*pow(sin(u / 12),4)*cos(u/12) - s * pow(EULER,c));
    GLdouble y = - s * (-pow(sin(u/12),5) + pow(EULER,c) - 2 * cos(4*u)) + c * ( 8*sin(4*u)-5/12*pow(sin(u / 12),4)*cos(u/12) - s * pow(EULER,c));
    return DCoordinate3(x, y, 1);
}

DCoordinate3 butterfly::d2(GLdouble u)
{
    GLdouble c = cos(u), s = sin(u);
    GLdouble x = (2304*c*sin(4*u)+4896*s*cos(4*u)+144*pow(EULER,c)*pow(s,3)+(-432*pow(EULER,c)*c-144*pow(EULER,c)+149*pow(sin(u/12),5)-20*pow(cos(u/12),2)*pow(sin(u/12),3)*s-120*cos(u/12)*pow(sin(u/12),4)*c)/144);
    GLdouble y = (120*cos(u/12)*pow(sin(u/12),4)*sin(u)+57600*pow(c,5)+(-144*pow(EULER,c)-66816)*pow(c,3)-432*pow(EULER,c)*pow(c,2)+(169*pow(sin(u/12),5)-20*pow(sin(u/12),3)+14112)*c+288*pow(EULER,c)/144);
    return DCoordinate3(x,y,0);
}

GLdouble astroid::a = 10;
GLdouble astroid::u_min = -TWO_PI;
GLdouble astroid::u_max = TWO_PI;

DCoordinate3 astroid::d0(GLdouble u)
{
    GLdouble c = cos(u), s = sin(u);
    GLdouble x = a * pow(c,3);
    GLdouble y = a * pow(s,3);
    return DCoordinate3(x, y, 3 * u);
}

DCoordinate3 astroid::d1(GLdouble u)
{
    GLdouble c = cos(u), s = sin(u);
    GLdouble x = -3 * a * pow(c,2) * s;
    GLdouble y = 3 * a * pow(s,2) * c;
    return DCoordinate3(x, y, 3);
}

DCoordinate3 astroid::d2(GLdouble u)
{
    GLdouble c = cos(u), s = sin(u);
    GLdouble x = -3/4 * a * (c + 3 * cos(3*u));
    GLdouble y = -3/4 * a * s - 3 * sin(3 * u);
    return DCoordinate3(x, y, 0);
}

GLdouble epicycloid::a = 2;
GLdouble epicycloid::b = 20;
GLdouble epicycloid::u_min = -TWO_PI;
GLdouble epicycloid::u_max = TWO_PI;

DCoordinate3 epicycloid::d0(GLdouble u)
{
    GLdouble c = cos(u), s = sin(u);
    GLdouble x = (a + b) * c - b * cos(((a / b) + 1) * u);
    GLdouble y = (a + b) * s - b * sin(((a / b) + 1) * u);
    return DCoordinate3(x, y, 3 * u);
}

DCoordinate3 epicycloid::d1(GLdouble u)
{
    GLdouble c = cos(u), s = sin(u);
    GLdouble x = (a + b) * (sin((u * (a + b))/b) - s);
    GLdouble y = (a + b) * (c - cos((u * (a + b))/b));
    return DCoordinate3(x, y, 3);
}

DCoordinate3 epicycloid::d2(GLdouble u)
{
    GLdouble c = cos(u), s = sin(u);
    GLdouble x = (a + b) * (((a + b) * cos((u * (a+b)) / b)) / b - c);
    GLdouble y = (a + b) * (((a + b) * sin((u * (a+b)) / b)) / b - s);
    return DCoordinate3(x, y, 0);
}

GLdouble epitrochoid::a = 5;
GLdouble epitrochoid::b = 4;
GLdouble epitrochoid::d = 5;
GLdouble epitrochoid::u_min = -TWO_PI;
GLdouble epitrochoid::u_max = TWO_PI;

DCoordinate3 epitrochoid::d0(GLdouble u)
{
    GLdouble c = cos(u), s = sin(u);
    GLdouble x = (a + b) * c - d * cos(((a / b) + 1) * u);
    GLdouble y = (a + b) * s - d * sin(((a / b) + 1) * u);
    return DCoordinate3(x, y, 3 * u);
}

DCoordinate3 epitrochoid::d1(GLdouble u)
{
    GLdouble x = ((b+a)*(d*sin((a/b+1)*u)-b*sin(u)))/b;
    GLdouble y = -((b+a)*(d*cos((a/b+1)*u)-b*cos(u)))/b;
    return DCoordinate3(x, y, 3);
}

DCoordinate3 epitrochoid::d2(GLdouble u)
{
    GLdouble x = ((b+a)*((a/b+1)*d*cos((a/b+1)*u)-b*cos(u)))/b;
    GLdouble y = -((b+a)*(b*sin(u)-(a/b+1)*d*sin((a/b+1)*u)))/b;
    return DCoordinate3(x, y, 0);
}

GLdouble plateau::a = 1;
GLdouble plateau::m = 0.8;
GLdouble plateau::n = 1;
GLdouble plateau::u_min = -TWO_PI;
GLdouble plateau::u_max = TWO_PI;

DCoordinate3 plateau::d0(GLdouble u)
{
    GLdouble x = a * sin((m + n) * u) / (sin ((m - n) * u));
    GLdouble y = 2 * a * sin(m * u) * sin(n * u) / (sin((m - n) * u));
    return DCoordinate3(x, y, u);
}

DCoordinate3 plateau::d1(GLdouble u)
{
    GLdouble x = (a*(n+m)*cos((n+m)*u))/sin((m-n)*u)-(a*(m-n)*cos((m-n)*u)*sin((n+m)*u))/pow(sin((m-n)*u),2);
    GLdouble y = a*(m*(cos(2*n*u)-1)-n*(cos(2*m*u)-1))*(1/pow(sin((n-m)*u),2));
    return DCoordinate3(x, y, 1);
}

DCoordinate3 plateau::d2(GLdouble u)
{
    GLdouble x = -a*(pow(1/sin((n-m)*u),3)*(pow(m,2)*sin((3*n-m)*u)+(pow(n,2)-4*m*n+pow(m,2))*sin((n+m)*u)-pow(n,2)*sin((n-3*m)*u)));
    GLdouble y = a*(2*m*n*sin(2*m*u)-2*m*n*sin(2*n*u))*(1/pow(sin((n-m)*u),2)-2*a*(n-m)*(m*(cos(2*n*u)-1)-n*(cos(2*m*u)-1))*sin((n-m)*u)/cos((n-m)*u)*(1/pow(sin((n-m)*u),2)));
    return DCoordinate3(x, y, 0);
}

GLdouble Torus::u_min = 0;
GLdouble Torus::u_max = TWO_PI;
GLdouble Torus::v_min = 0;
GLdouble Torus::v_max = TWO_PI;
GLdouble Torus::R     = 2;
GLdouble Torus::r     = 1;

DCoordinate3 Torus::d00(GLdouble u, GLdouble v)
{
    return DCoordinate3((R + r*sin(u)) * cos(v),
                        (R + r*sin(u)) * sin(v),
                        r*cos(u));
}
DCoordinate3 Torus::d10(GLdouble u, GLdouble v)
{
    return DCoordinate3( r * cos(u) * cos(v),
                         r * cos(u) * sin(v),
                         -r*sin(u));
}
DCoordinate3 Torus::d01(GLdouble u, GLdouble v)
{
    return DCoordinate3(-(R + r*sin(u)) * sin(v),
                         (R + r*sin(u)) * cos(v), 0);
}

GLdouble Hyperboloid::u_min = 0;
GLdouble Hyperboloid::u_max = 3;
GLdouble Hyperboloid::v_min = 0;
GLdouble Hyperboloid::v_max = TWO_PI;

DCoordinate3 Hyperboloid::d00(GLdouble u, GLdouble v)
{
    return DCoordinate3((1 + cosh(u-3.0/2))*sin(v),
                        (1+ cosh(u-3.0/2))*cos(v),
                        sinh(u-3.0/2));
}
DCoordinate3 Hyperboloid::d10(GLdouble u, GLdouble v)
{
    return DCoordinate3(sinh(3.0/2-u)*(-sin(v)),
                        sinh(3.0/2-u)*(-cos(v)),
                        cosh(3.0/2-u));
}
DCoordinate3 Hyperboloid::d01(GLdouble u, GLdouble v)
{
    return DCoordinate3((cosh(3.0/2-u)+1)*cos(v),
                        -(cosh(3.0/2-u)+1)*sin(v),
                        0);
}

GLdouble Cylindrical_helicoid::u_min = 0;
GLdouble Cylindrical_helicoid::u_max = 2;
GLdouble Cylindrical_helicoid::v_min = 0;
GLdouble Cylindrical_helicoid::v_max = TWO_PI;

DCoordinate3 Cylindrical_helicoid::d00(GLdouble u, GLdouble v)
{
    return DCoordinate3((2+u)*cos(v),
                        (2+u)*sin(v),
                        v);
}
DCoordinate3 Cylindrical_helicoid::d10(GLdouble u, GLdouble v)
{
    return DCoordinate3(cos(v),
                        sin(v),
                        0);
}
DCoordinate3 Cylindrical_helicoid::d01(GLdouble u, GLdouble v)
{
    return DCoordinate3(-(u+2)*sin(v),
                        (u+2)*cos(v),
                        1);
}

GLdouble Alfred_Gray::u_min = 0;
GLdouble Alfred_Gray::u_max = 2;
GLdouble Alfred_Gray::v_min = 0;
GLdouble Alfred_Gray::v_max = TWO_PI;

DCoordinate3 Alfred_Gray::d00(GLdouble u, GLdouble v)
{
    return DCoordinate3(3*cos(u) + 1.0/2*(1+cos(2*u))*sin(v) - 1.0/2*sin(2*u)*sin(2*v),
                        3*sin(u) + 1.0/2*sin(2*u)*sin(u) - 1.0/2*(1-cos(2*u))*sin(2*v),
                        cos(u)*sin(2*u)+sin(u)*sin(v));
}
DCoordinate3 Alfred_Gray::d10(GLdouble u, GLdouble v)
{
    return DCoordinate3(-sin(2*u)*sin(v)-cos(2*u)*sin(2*v)-3*u*sin(u)+3*cos(u),
                        -sin(2*u)*sin(2*v)+3*u*cos(u)+sin(u)*cos(u)*cos(u)+sin(u)*cos(2*u),
                        cos(u)*(3*cos(2*u)+sin(v)-1));
}
DCoordinate3 Alfred_Gray::d01(GLdouble u, GLdouble v)
{
    return DCoordinate3(cos(u)*cos(u)*cos(v)-sin(2*u)*cos(2*v),
                        -2*sin(u)*sin(u)*cos(2*v),
                        sin(u)*cos(v));
}

GLdouble KleinBottle::u_min = 0.0;
GLdouble KleinBottle::u_max = TWO_PI;
GLdouble KleinBottle::v_min = 0.0;
GLdouble KleinBottle::v_max = TWO_PI;

DCoordinate3 KleinBottle::d00(GLdouble u, GLdouble v)
{
        return DCoordinate3(3 * cos(u) + 1.0/2.0 * (1 + cos(2 * u)) * sin(v) - 1/2 * sin(2 * u) * sin(2 * v),
                                                3 * sin(u) + 1/2 * sin(2 * u) * sin(v) - 1.0/2.0 * (1 - cos(2 * u)) * sin(2 * v),
                                                cos(u) * sin(2 * v) + sin(u) * sin(v));
}

DCoordinate3 KleinBottle::d10(GLdouble u, GLdouble v)
{
        return DCoordinate3(-3 * sin(u) + (1/2 - sin(2 * u)) * sin(v) + cos(2 * u) * sin(2 * v),
                                                3 * cos(u) + cos(2 * u) * sin(v) -(1.0/2.0 + sin(2 * u)) * sin(2 * v),
                                                -sin(u) * sin(2 * v) + cos(u) * sin(v));
}

DCoordinate3 KleinBottle::d01(GLdouble u, GLdouble v)
{
        return DCoordinate3(3 * cos(u) + 1/2 * (1 + cos(2 * u)) * cos(v) - sin(2 * u) * cos(2 * v),
                                                3 * sin(u) + 1.0/2.0 * sin(2 * u) * cos(v) + (1 - cos(2 * u)) * cos(2 * v),
                                                2 * cos(u) * cos(2 * v) + sin(u) * cos(v));
}

GLdouble Dini::u_min = 0.0;
GLdouble Dini::u_max = 2 * TWO_PI;
GLdouble Dini::v_min = 0.0;
GLdouble Dini::v_max = 2;
GLdouble Dini::a = 1.0;
GLdouble Dini::b = 0.15;

DCoordinate3 Dini::d00(GLdouble u, GLdouble v)
{
    return DCoordinate3(a * cos(u) * sin(v), a * sin(u) * sin(v), a * (cos(v) + log(tan(v/2))) + b * u);
}

DCoordinate3 Dini::d10(GLdouble u, GLdouble v)
{
    return DCoordinate3(-a * sin(u) * sin(v), a * cos(u) * sin(v), b);
}

DCoordinate3 Dini::d01(GLdouble u, GLdouble v)
{
    return DCoordinate3(a * cos(u) * cos(v), a * sin(u) * cos(v), a * cos(v) * cos(v)/sin(v));
}

