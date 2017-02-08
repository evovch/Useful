#include "mainwindow.h"

#include <TApplication.h>
#include "QRootApplication.h"

int main(int argc, char *argv[])
{
    TApplication app("qtRootTreeTest", &argc, argv);
    QRootApplication myapp(argc, argv);

    MainWindow w;
    w.show();

    return myapp.exec();
}
