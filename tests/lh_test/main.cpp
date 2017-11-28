#include <QCoreApplication>
#include<lh_pel.h>
#include<QDebug>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    double **amp;
    double **phase;
    double **ant;
    double f0 = 2e5;
    int n = 1;
    int el = 4;
    int dim = 3;

    amp = (double **)malloc(sizeof(double *) * el);
    for (int i = 0; i < el; ++i)
        amp[i] = (double *)malloc(sizeof(double) * n);


   phase = (double **)malloc(sizeof(double *) * el);
    for (int i = 0; i < el; ++i)
        phase[i] = (double *)malloc(sizeof(double) * n);

    ant = (double **)malloc(sizeof(double *) * dim);
    for (int i = 0; i < dim; ++i)
        ant[i] = (double *)malloc(sizeof(double) * el);

    for (int i = 0; i < el; ++i)
        for (int j = 0; j < n; ++j)
        {
            amp[i][j] = 0;
            phase[i][j] = 0;
        }

    amp[0][0] = 1521.98677077;
    amp[1][0] = amp[0][0];
    amp[2][0] = amp[0][0];
    amp[3][0] = amp[0][0];

    phase[0][0] = 1.10970736;
    phase[1][0] = 1.11389406;
    phase[2][0] = 1.11808494;
    phase[3][0] = 1.11389406;

    for (int i = 0; i < dim; ++i)
        for (int j = 0; j < el; ++j)
            ant[i][j] = 0;

    ant[0][0] = 1;
    ant[1][1] = 1;
    ant[0][2] = -1;
    ant[1][3] = -1;


   Lh_Pel lh(amp, phase, n, ant, el, f0);
   column_vector arg(0, -1.57979);
   double ans = lh.calculate(0,  -1.570796);
   qDebug() << ans;
   return a.exec();
}
