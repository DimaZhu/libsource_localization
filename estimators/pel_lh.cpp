#include "pel_lh.h"

Pel_Lh::Pel_Lh( double ** amp,
               double  ** phase,
               int sig_length,
               double ** antenna_coordinates,
               int elemets_total,
               double f0):
               pseudophase(nullptr),
               mutual_amp(nullptr),
               coord_delta(nullptr),
               lh_max(0)
{
//    Q_ASSERT(sig_length > 0 );
//    Q_CHECK_PTR(amp);
//    Q_CHECK_PTR(phase);
//    Q_CHECK_PTR(antenna_coordinates);
//    Q_ASSERT(elemets_total > 1);
//    Q_ASSERT(f0 > 0);

    ant_pairs = elemets_total * (elemets_total-1) / 2;
    double wave_length = 3e8 / f0;
    ang_freq = 2 * M_PI  / wave_length;

    pseudophase = (double *) malloc(ant_pairs * sizeof(double));
    mutual_amp = (double *) malloc(ant_pairs * sizeof(double));
    coord_delta = (double **) malloc(ant_pairs * sizeof(double *));
    for (int pair = 0; pair < ant_pairs; ++pair)
        coord_delta[pair] = (double *) malloc(DIM * sizeof(double));

    for (int i = 0; i < ant_pairs; ++i)
        pseudophase[i] = NAN;

    for (int i = 0; i < ant_pairs; ++i)
        mutual_amp[i] = NAN;

    for (int i = 0; i < ant_pairs; ++i)
        for (int d = 0; d < DIM; ++d )
            coord_delta[i][d] = NAN;

//    printf("Amplitudes:\n");
//    for (int j = 0; j < sig_length; ++j)
//        for (int i = 0; i < elemets_total; ++i )
//            printf("%f,  ", amp[i][j]);

//    printf("\nPhases:\n");
//    for (int j = 0; j < sig_length; ++j)
//        for (int i = 0; i < elemets_total; ++i )
//            printf("%f,  ", phase[i][j]);

//    printf("\nElements Coordinates:\n");
//    for (int j = 0; j < DIM; ++j)
//    {
//        printf("\n");
//        for (int i = 0; i < elemets_total; ++i )
//            printf("%f,  ", antenna_coordinates[j][i]);
//    }

//    printf("\nSignal length:  %d\nElements total: %d\nCarrier: %e", sig_length, elemets_total, f0);


    double amp_temp;
    double phase_temp;
    int pair = 0;

    for (int l = 0; l < elemets_total - 1; ++l)
    {
        for (int m = l + 1; m < elemets_total; ++m)
        {
            double sum_sin = 0;
            double sum_cos = 0;
            complex<double> sum_amp = 0;

            for (int samp = 0; samp < sig_length; ++samp)
            {
                amp_temp = amp[m][samp] * amp[l][samp];
                phase_temp = phase[m][samp] - phase[l][samp];
                sum_sin += amp_temp * sin(phase_temp);
                sum_cos += amp_temp * cos(phase_temp);
                complex<double> exp = cexp(1i * phase_temp);
                sum_amp += amp_temp * exp;

            }

            mutual_amp[pair] = abs(sum_amp);            
            pseudophase[pair] = carg(sum_cos + 1i * sum_sin);

            lh_max +=mutual_amp[pair];

            for (int d = 0; d < DIM; ++d)
                coord_delta[pair][d] = antenna_coordinates[d][m] - antenna_coordinates[d][l];

            ++pair;


        }
    }
}

Pel_Lh::~Pel_Lh()
{
    free(pseudophase);
    free(mutual_amp);
    for (int pair = 0; pair < ant_pairs; ++pair)
        free(coord_delta[pair]);
    free(coord_delta);
}


double Pel_Lh::calculate(double alpha, double betta) const
{
    double lh = 0;
    double r[3] = {cos(betta) * cos(alpha), cos(betta) * sin(alpha), sin(betta)};
    for (int pair = 0; pair < ant_pairs; ++pair)
    {
        double full_phase = ang_freq * (coord_delta[pair][0] * r[0]
                                                        + coord_delta[pair][1] * r[1]
                                                        + coord_delta[pair][2] * r[2]) - pseudophase[pair];
        lh -= mutual_amp[pair] * cos(full_phase);
    }

    lh /= lh_max;
    return lh;
}

double Pel_Lh::operator ()( const column_vector &arg)
{
    return calculate(arg(0), arg(1));
}
