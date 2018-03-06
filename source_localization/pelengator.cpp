#include "pelengator.h"

Pelengator::Pelengator(const Antenna &i_ant, double f_res, double fs):
    antenna(i_ant),
    interpolation(true),
    verbose(false),
    active(false)
{
//    Py_Initialize();
//    import_grid();
//    //PyInit_grid();
//    PyErr_Print();

    sampling_frequency = fs;
    frequency_resolution = f_res;
}

Pelengator::~Pelengator()
{
//    Py_Finalize();
}

//void Pelengator::set_signal_param(double central_frequency, double df, int i_carrier_samp)
//{
//    int samp_start = i_carrier_samp - round(df / frequency_resolution);
//    int samp_stop = i_carrier_samp + round(df / frequency_resolution);
//    set_signal_param(central_frequency, samp_start, samp_stop);
//}

void Pelengator::set_signal_param(double carrier, int i_samp_start, int i_samp_stop)
{
    if(verbose)
        printf("\n\n__Setting signal parameters in pelengator__\n");


    samp_start = i_samp_start;
    samp_stop = i_samp_stop;
    double df = (i_samp_start - i_samp_stop) / 2 * frequency_resolution;
    double f_max = carrier + df/2;

    vector<vector<float>> ant_coord = antenna.get_model();
    coord_grid = generate_grid(antenna, f_max);

    if (coord_grid.empty()) {
       throw std::runtime_error("Empty coordinate grid. Can't estimate position");
    }

    if (verbose) {
        printf("Grid size: %i x %i\n", (int)coord_grid[0].size(), (int)coord_grid[0][0].size());
        float delta_alpha =  coord_grid[0][0][1] - coord_grid[0][0][0];
        float delta_betta =  coord_grid[1][1][0] - coord_grid[1][0][0];
        printf("Alpha step: %f\n Betta step: %f\n\n", delta_alpha * 180 / M_PI, delta_betta * 180 / M_PI);
    }

}

Peleng Pelengator::estimate(SpecFrame *frame)
{
    //Предусловия
    assert(frame->get_central_frequency() != 0 && !std::isnan(frame->get_central_frequency()));
    assert(frame->get_sampling_frequency() != 0 && !std::isnan(frame->get_sampling_frequency()));
    assert(!std::isnan(frame->get_band_width()));
    assert(frame->get_frequency_resolution() != 0 && !std::isnan(frame->get_frequency_resolution()));

    if(verbose)
        printf("\n__Estimating signal parameters__\n");


    Peleng estimation;

   //Расчет матрицы функции правдоподобия
    lh_matrix.clear();

    LhPel lh(&antenna, frame, samp_start, samp_stop);
    Lh *lh_ptr = &lh;

    for (uint i = 0; i < coord_grid[0].size(); ++i)
    {
        vector<double> lh_vector;
        for (uint j = 0; j < coord_grid[0][0].size(); ++j)
        {
            double val = lh.calculate(coord_grid[0][i][j], coord_grid[1][i][j]);
            lh_vector.push_back(val);
        }
        lh_matrix.push_back(lh_vector);
    }

    //Grid Search
    float lh_min_enum = 0;
    double alpha_min_enum = 0;
    double betta_min_enum = 0;

    for (uint i = 0; i < lh_matrix.size(); ++i)
    {
        for (uint j = 0; j < lh_matrix[0].size(); ++j)
        {
            if (lh_matrix[i][j] < lh_min_enum){
                lh_min_enum = lh_matrix[i][j];
                alpha_min_enum = coord_grid[0][i][j];
                betta_min_enum = coord_grid[1][i][j];
            }
        }
    }

    if(verbose) {
        printf("Lh at grid search min: %f\n", abs(lh_min_enum));
        printf("Enumeration estimation [deg]: alpha: %f, betta %f\n", alpha_min_enum * 180 /M_PI, betta_min_enum * 180 / M_PI);
    }

    double alpha_min;
    double betta_min;
    double lh_min;

    if (interpolation) {

        //Интерполяция
        double alpha_min_int;
        double betta_min_int;
        double lh_min_int;

        Interpolator interpolator;
        vector<vector<double>> output = interpolator.find_minimum2d(coord_grid, lh_matrix, lh_ptr);
        alpha_min_int = output[0][0];
        betta_min_int = output[0][1];
        lh_min_int = output[1][0];

        if(verbose) {
            printf("Lh at interpolation extreme: %f\n", abs(lh_min_int));
            printf("Interpolation estimation [deg]: alpha: %f, betta %f\n", alpha_min_int * 180 /M_PI, betta_min_int * 180 / M_PI);
        }

        if (lh_min_int < lh_min_enum) {
            alpha_min = alpha_min_int;
            betta_min = betta_min_int;
            lh_min = lh_min_int;
        } else {
            alpha_min = alpha_min_enum;
            betta_min = betta_min_enum;
            lh_min = lh_min_enum;
        }

    } else {
        alpha_min = alpha_min_enum;
        betta_min = betta_min_enum;
        lh_min = lh_min_enum;
    }


    //Поиск локального экстремума
    parameters_vector starting_point(2);
    starting_point = alpha_min, betta_min;
    dlib::find_min_using_approximate_derivatives(dlib::bfgs_search_strategy(), dlib::objective_delta_stop_strategy(1e-7), lh, starting_point, -1);

    if (verbose)
        printf("Lh at precise min: %f\n",abs(lh(starting_point)));

    estimation.azimuth = starting_point(0);
    estimation.elevation = starting_point(1);
    estimation.probability = abs(lh_min);
    estimation.phase_center = antenna.get_phase_center();

    return estimation;
}

void Pelengator::turn_on_interpolation(bool turn_on)
{
    interpolation = turn_on;
}

bool Pelengator::is_active()
{
    return active;
}

void Pelengator::turn_on(bool arg)
{
    active = arg;
}

void Pelengator::set_verbose(bool arg)
{
    verbose = arg;
}
