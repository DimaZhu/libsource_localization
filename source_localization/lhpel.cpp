#include "lhpel.h"

LhPel::LhPel(Antenna *ant, const SpecFrame * const data, int samp_start, int samp_stop, bool verbose)
{

        ant_pairs = ant->get_channels_total() * (ant->get_channels_total()-1) / 2;
        double wave_length = 3e8 / data->get_central_frequency();
        ang_freq = 2 * M_PI  / wave_length;
        complex2d signal  = data->get_data();

        if (samp_start >= data->get_length())
            samp_start = data->get_length() - 1;

        if (samp_stop >= data->get_length())
            samp_stop = data->get_length() - 1;

        vector<vector<float>> ant_coord = ant->get_elements();

        if (verbose){
                printf("Amplitudes:\n");
                for (int j = 0; j < data->get_length(); ++j)
                    for (int i = 0; i < ant->get_channels_total(); ++i )
                        printf("%f,  ", abs(signal[i][j]));

                printf("\n\nPhases:\n");
                for (int j = 0; j < data->get_length(); ++j)
                    for (int i = 0; i < ant->get_channels_total(); ++i )
                        printf("%f,  ", arg(signal[i][j]));

                printf("\n\nElements Coordinates:");
                for (int i = 0; i < ant->get_channels_total(); ++i )
                {
                    printf("\n");
                        for (int j = 0; j < DIM; ++j)
                        printf("%f,  ", ant_coord[j][i]);
                }

                printf("\nSignal indexes: [%i, %i]\n", samp_start, samp_stop);
                printf("\nSignal length:  %d\nElements total: %d\nCarrier: %e\n\n\n\n", data->get_length(), ant->get_channels_total(), data->get_central_frequency());

        }


        double amp_temp;
        double phase_temp;
        int pair = 0;

        for (int l = 0; l < ant->get_channels_total() - 1; ++l)
        {
            for (int m = l + 1; m < ant->get_channels_total(); ++m)
            {
                double sum_sin = 0;
                double sum_cos = 0;
                std::complex< double > sum_amp = 0;

                for (int samp = samp_start; samp <= samp_stop; ++samp)
                {
                    amp_temp =  abs(signal[m][samp]) * abs(signal[l][samp]);
                    phase_temp = arg(signal[m][samp]) - arg(signal[l][samp]);
                    sum_sin += amp_temp * sin(phase_temp);
                    sum_cos += amp_temp * cos(phase_temp);
                    complex<double> exp = cexp(1i * phase_temp);
                    sum_amp += amp_temp * exp;

                }

                mutual_amp.push_back(abs(sum_amp));
                pseudophase.push_back(carg(sum_cos + 1i * sum_sin));

                lh_max +=mutual_amp[pair];

                std::vector<float> buffer;
                for (int d = 0; d < DIM; ++d)
                    buffer.push_back(ant_coord[d][m] - ant_coord[d][l]);
                 coord_delta.push_back(buffer);
                ++pair;


            }
        }
}


double LhPel::calculate(double alpha, double betta) const
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

double LhPel::operator ()( const parameters_vector &arg) const
{
    return calculate(arg(0), arg(1));
}
