#include "pel_lh.h"

Pel_Lh::Pel_Lh(const  double * const * const amp,
               const double  * const * const phase,
               int sig_length,
               const double * const * const antenna_coordinates,
               int elemets_total,
               double f0):
               pseudophase(nullptr),
               mutual_amp(nullptr),
               coord_delta(nullptr)
{
    Q_ASSERT(sig_length > 0 );
    Q_CHECK_PTR(amp);
    Q_CHECK_PTR(phase);
    Q_CHECK_PTR(antenna_coordinates);
    Q_ASSERT(elemets_total > 1);
    Q_ASSERT(f0 > 0);

    ant_pairs = elemets_total * (elemets_total-1) / 2;
    double wave_length = 3e8 / f0;
    ang_freq = M_2_PI  / wave_length;

    pseudophase = (double *) malloc(ant_pairs * sizeof(double));
    mutual_amp = (double *) malloc(ant_pairs * sizeof(double));
    coord_delta = (double **) malloc(ant_pairs * sizeof(double *));
    for (int pair = 0; pair < ant_pairs; ++pair)
        coord_delta[pair] = (double *) malloc(DIM * sizeof(double));

    double amp_temp;
    double phase_temp;
    int pair = 0;

    for (int l = 0; l < ant_pairs - 1; ++l)
    {
        for (int m = l + 1; m < ant_pairs; ++m)
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
                complex<double> a = cexp(1i * (phase[m][samp] - phase[l][samp]));
                sum_amp = sum_amp + amp[m][samp] * amp[l][samp] * a;

            }

            mutual_amp[pair] = abs(sum_amp);
            pseudophase[pair] = carg(sum_cos + 1i * sum_sin);

            for (int d = 0; d < DIM; ++d)
            {
                coord_delta[pair][0] = antenna_coordinates[m][0] - antenna_coordinates[l][0];
                coord_delta[pair][1] = antenna_coordinates[m][1] - antenna_coordinates[l][1];
                coord_delta[pair][2] = antenna_coordinates[m][2] - antenna_coordinates[l][2];
            }

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

double Pel_Lh::operator ()( const column_vector &arg) const
{
    double lh = 0;
    double r[3] = {cos(arg(1)) * cos(arg(0)), cos(arg(1)) * sin(arg(0)), sin(arg(1))};
    for (int pair = 0; pair < ant_pairs; ++pair)
    {
        double full_phase = ang_freq * (coord_delta[pair][0] * r[0]
                                                        + coord_delta[pair][1] * r[1]
                                                        + coord_delta[pair][2] * r[2]) - pseudophase[pair];
        lh += mutual_amp[pair] * cos(full_phase);
    }

    return lh;
}
