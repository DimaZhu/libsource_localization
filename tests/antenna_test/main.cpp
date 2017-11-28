#include<antenna.h>
#include <QCoreApplication>
#include <QString>
#include <vector>
#include <QDebug>

using namespace std;
int main(int argc, char *argv[])
{
    QString str = "/home/dima/.landing/AFS4.ini";
    Antenna ant(str.toLatin1().data());
    vector<vector<float>> coord = ant.get_elements_coordinates();

    int i_max = coord.size();
    int j_max= coord[0].size();
    for (int i = 0; i < i_max; ++i)
        for (int j = 0; j < j_max; ++j)
            qDebug()<<coord[i][j];

    QCoreApplication a(argc, argv);
    return a.exec();
}
