#include <antenna.h>
#include <vector>
#include <string>
#include <QCoreApplication>
#include <iostream>

using namespace std;
int main(int argc, char *argv[])
{
    string str = "/home/dima/.landing/AFS4.ini";
    Antenna ant(str);
    vector<vector<float> > coord = ant.get_elements();

    cout << "\n";

    int i_max = coord.size();
    int j_max= coord[0].size();
    for (int i = 0; i < i_max; ++i)
    {
        for (int j = 0; j < j_max; ++j)
        {
            cout<<coord[i][j];
            cout<<"\n";
        }

        cout<<"\n";
    }
    cout << "\n";

    QCoreApplication a(argc, argv);
    return 0;
}
