#include <QApplication>
#include <QFile>
#include <QTextStream>
#include "GUI/MainWindow.h"
#include "Core/Matrices.h"
#include "Core/RealSquareMatrices.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace cagd;

int main(int argc, char **argv)
{
    // creating an application object and setting one of its attributes
    QApplication app(argc, argv);

    QFile f(":qdarkstyle/style.qss");
    if (!f.exists())
    {
        printf("Unable to set stylesheet, file not found\n");
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }

//    Matrix<int> M(3,5);

//    for (GLuint r = 0; r < M.GetRowCount();  r++) {
//        for (GLuint c = 0; c < M.GetColumnCount(); c++) {
//            M(r,c) = r * M.GetColumnCount() + c;
//        }
//    }
//    cout << M << endl;

//    fstream f("test.txt", ios_base::in);
//    f >> M;
//    cout << M << endl;

//    RealSquareMatrix A(3);
//    A(0,0) = A(0,1) = A(0,2) = 1.0;
//    A(1,0) = 2.0; A(1,1) = -1.0; A(1,2) = 3;
//    A(2,0) = -1.0; A(2,1) = 3; A(2,2) = -4;

//    ColumnMatrix<GLdouble> B(3);
//    B[0] = 2; B[1] = 9; B[2] = -12;

//    ColumnMatrix<GLdouble> X;

//    A.SolveLinearSystem(B,X);

//    cout << X << endl;

//    TriangularMatrix<GLdouble> Q;
//    Q.ResizeRows(5);

//    cout << Q;

//    cin >> Q;
//    cout << Q;

    // if you have installed a different version of Qt, it may happen that
    // the application attribute Qt::AA_UseDesktopOpenGL is not recognized
    // on Windows its existence is critical for our applications
    // on Linux or Mac you can uncomment this line
    app.setAttribute(Qt::AA_UseDesktopOpenGL, true);

    // creating a main window object
    MainWindow mwnd;
    mwnd.showMaximized();

    // running the application
    return app.exec();
}
