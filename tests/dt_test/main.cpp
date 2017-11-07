#include <QCoreApplication>
#include <antenna.h>
#include <QString>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString name = "/home/dima/Projects/Touchdown/.landing/AFS4.ini";
    dt::Antenna ant(name.toLatin1().data());
    vector<vector<float>> cord = ant.get_elements_coordinates();

    return a.exec();
}
