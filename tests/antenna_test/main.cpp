#include <antenna.h>
#include <vector>
#include <string>
#include <QCoreApplication>

using namespace std;
int main(int argc, char *argv[])
{
    char *str = "/home/dima/.landing/AFS4.ini";
    Antenna ant(str);
//    vector<vector<float> > coord = ant.get_elements_coordinates();

//    int i_max =b coord.size();
//    int j_max= coord[0].size();
//    for (int i = 0; i < i_max; ++i)
//        for (int j = 0; j < j_max; ++j)
//            qDebug()<<coord[i][j];

    QCoreApplication a(argc, argv);
    return 0;
}
