#include <QApplication>
#include <QLabel>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QLabel test("test");
    test.show();

    return app.exec();
}