#include "pelengator.h"

Pelengator::Pelengator(const Antenna &i_ant):
    antenna(i_ant),
    interpolation(true)
{
    Py_Initialize();
    PyInit_grid();
    PyErr_Print();
}

Pelengator::~Pelengator()
{
    Py_Finalize();
}

void Pelengator::set_signal_param(double f0, double df, double f_res, double fs)
{
    #ifdef QT_DEBUG
        printf("__Setting signal parameters in pelengator__\n");
    #endif

    vector<vector<float>> ant_coord = antenna.get_model_coordinates();
    coord_grid = pel_grid(ant_coord, f0, df, f_res, fs);
    if (coord_grid.empty()) {
       throw std::runtime_error("Empty coordinate grid. Can't estimate position");
    }

    #ifdef QT_DEBUG
        float delta_alpha =  coord_grid[0][0][1] - coord_grid[0][0][0];
        float delta_betta =  coord_grid[1][1][0] - coord_grid[1][0][0];
        printf("Alpha step: %f\n Betta step: %f\n\n", delta_alpha * 180 / M_PI, delta_betta * 180 / M_PI);
    #endif

}

estimationT Pelengator::estimate(SpecFrame frame)
{
    //Предусловия
    Q_ASSERT(frame.get_carrier() != 0 && !std::isnan(frame.get_carrier()));
    Q_ASSERT(frame.get_sampling_frequency() != 0 && !std::isnan(frame.get_sampling_frequency()));
    Q_ASSERT(frame.get_band_width() != 0 && !std::isnan(frame.get_band_width()));
    Q_ASSERT(frame.get_frequency_resolution() != 0 && !std::isnan(frame.get_frequency_resolution()));

    #ifdef QT_DEBUG
        printf("__Estimating signal parameters__\n");
    #endif

    vector<double> estimation;

   //Расчет матрицы функции правдоподобия
    lh_matrix.clear();
    Lh_Pel lh(&antenna, &frame);
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

    #ifdef QT_DEBUG
        printf("Lh at grid search min: %f\n", lh_min_enum);
        printf("Enumeration estimation [deg]: alpha: %f, betta %f\n", alpha_min_enum * 180 /M_PI, betta_min_enum * 180 / M_PI);
    #endif

    double alpha_min;
    double betta_min;

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

        #ifdef QT_DEBUG
            printf("Lh at interpolation extreme: %f\n", lh_min_int);
            printf("Interpolation estimation [deg]: alpha: %f, betta %f\n", alpha_min_int * 180 /M_PI, betta_min_int * 180 / M_PI);
        #endif

        if (lh_min_int < lh_min_enum) {
            alpha_min = alpha_min_int;
            betta_min = betta_min_int;
        } else {
            alpha_min = alpha_min_enum;
            betta_min = betta_min_enum;
        }

    } else {
        alpha_min = alpha_min_enum;
        betta_min = betta_min_enum;
    }


    //Поиск локального экстремума
    parameters_vector starting_point(2);
    starting_point = alpha_min, betta_min;
    dlib::find_min_using_approximate_derivatives(dlib::bfgs_search_strategy(), dlib::objective_delta_stop_strategy(1e-7), lh, starting_point, -1);

    printf("Lh at precise min: %f\n",lh(starting_point));
    estimation.push_back(starting_point(0));
    estimation.push_back(starting_point(1));
    return estimation;
}

void Pelengator::turn_on_interpolation(bool turn_on)
{
    interpolation = turn_on;
}
