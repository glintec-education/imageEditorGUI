#include "imgeditorgui.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImgEditorGUI w;
    w.show();
    return a.exec();
}
