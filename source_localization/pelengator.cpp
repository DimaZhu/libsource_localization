#include "pelengator.h"

Pelengator::Pelengator(const Antenna &i_ant):
    antenna(i_ant)
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
    vector<vector<float>> ant_coord = antenna.get_model_coordinates();
    coord_grid = pel_grid(ant_coord, f0, df, f_res, fs);
    if (coord_grid.empty()) {
       throw std::runtime_error("Empty coordinate grid. Can't estimate position");
    }

}

estimationT Pelengator::estimate(SpecFrame frame)
{
    //Предусловия
    Q_ASSERT(frame.get_carrier() != 0 && !std::isnan(frame.get_carrier()));
    Q_ASSERT(frame.get_sampling_frequency() != 0 && !std::isnan(frame.get_sampling_frequency()));
    Q_ASSERT(frame.get_band_width() != 0 && !std::isnan(frame.get_band_width()));
    Q_ASSERT(frame.get_frequency_resolution() != 0 && !std::isnan(frame.get_frequency_resolution()));

    vector<float> estimation;

   //Расчет матрицы функции правдоподобия
    lh_matrix.clear();
    Lh_Pel lh(&antenna, &frame);

    for (uint i = 0; i < coord_grid.size(); ++i)
    {
        vector<double> lh_vector;
        for (uint j = 0; j < coord_grid[0].size(); ++j)
        {
            double val = lh.calculate(coord_grid[i][j][0], coord_grid[i][j][1]);
            lh_vector.push_back(val);
        }
        lh_matrix.push_back(lh_vector);
    }

    //Грубая оценка
    float lh_min = 0;
    double alpha_min = 0;
    double betta_min = 0;

    for (uint i = 0; i < lh_matrix.size(); ++i)
    {
        for (uint j = 0; j < lh_matrix[0].size(); ++j)
        {
            if (lh_matrix[i][j] < lh_min){
                lh_min = lh_matrix[i][j];
                alpha_min = coord_grid[i][j][0];
                betta_min = coord_grid[i][j][1];
            }
        }
    }

    qDebug() << "Lh at true min: " + QString::number(lh.calculate(10 * 180 / M_PI, 10 * 180 / M_PI));
    qDebug() << "Lh at rough min" + QString::number(lh_min);
    qDebug() << "Rough estimation [deg]: alpha: " + QString::number(alpha_min * 180 /M_PI) + ", betta: " + QString::number(betta_min * 180 / M_PI);
    //Интерполяция
    //

    //Доуточнение
    parameters_vector starting_point(2);
    starting_point = alpha_min, betta_min;
    dlib::find_min_using_approximate_derivatives(dlib::bfgs_search_strategy(), dlib::objective_delta_stop_strategy(1e-7), lh, starting_point, -1);

    qDebug() << "Lh at precise min" + QString::number(lh(starting_point));
    estimation.push_back(starting_point(0));
    estimation.push_back(starting_point(1));
    return estimation;
}
