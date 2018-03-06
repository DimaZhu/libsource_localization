#include "gridpel.h"


vector<vector<vector<double>>> generate_grid(Antenna ant, float f_max)
{
    //***Работает сугубо для цилиндрических антенных решеток.***

    vector<vector<vector<double>>> coord_grid;
//    float fs = f_max * 2.1;

    coordinates ant_el = ant.get_elements();

//    // Расчет координат цели
//    float radius = 1e4;
//    float target [3];
//    float alpha = M_PI;
//    float betta = 0;
//    target[0] = radius * cos(betta) * cos(alpha);
//    target[1] = radius * cos(betta) * cos(alpha);
//    target[2] = radius * sin(betta);

//    //Расчет вектора задержек
//    double tau[ant.get_channels_total()] = {0};

//    for (int ch = 0; ch < ant.get_channels_total(); ++ch)
//        tau[ch] = sqrt( pow(target[0] - ant_el[ch][0], 2) + pow(target[1] - ant_el[ch][1], 2) + pow(target[2] - ant_el[ch][2], 2));

//    // Генерация гармоники в спектральной области
//    float error = 4e6;
//    int length = fs / f_max;
//    int f_ind = 1;
//    while (abs(fs/ length * f_ind - f_max) > error )
//    {
//        f_ind *= 10;
//        length *= 10;
//    }

//    complex<double> s0[ant.get_channels_total()][length];

//    //Обнуление
//    for (int ch = 0; ch < ant.get_channels_total(); ++ch)
//        for (int s = 0; s < length; ++s)
//            s0[ch][s] = 0;

//    // Гармоника
//    for (int ch = 0; ch < ant.get_channels_total(); ++ch)
//    {
//        s0[ch][f_ind] = 1;
//        s0[ch][length - f_ind] = 1;
//    }

//    // Задержка
//    for (int ch = 0; ch < ant.get_channels_total(); ++ch)
//    {
//        s0[ch][f_ind] = cexp( - 1i * tau[ch]);
//        s0[ch][f_ind] = cexp( 1i * tau[ch]);
//    }

    float x_min = ant_el[0][0];
    float x_max = ant_el[0][0];
//    float y_min = ant_el[0][1];
//    float y_max = ant_el[0][1];
    float z_min = ant_el[2][0];
    float z_max = ant_el[2][0];

    for (int ch = 0; ch < ant.get_channels_total(); ++ch)
    {
        if(ant_el[0][ch] < x_min)
            x_min = ant_el[0][ch];

        if(ant_el[0][ch] > x_max)
            x_max = ant_el[0][ch];

//        if(ant_el[ch][1] < y_min)
//            y_min = ant_el[ch][1];

//        if(ant_el[ch][1] > y_max)
//            y_max = ant_el[ch][1];

        if(ant_el[2][ch] < z_min)
            z_min = ant_el[2][ch];

        if(ant_el[2][ch] > z_max)
            z_max = ant_el[2][ch];
    }

    float alpha_width = 3e8/f_max/(x_max - x_min)/2;
    float alpha_step = alpha_width/3;

    if (z_max == z_min)
        throw std::runtime_error("Нельзя с помощью двухмерной антенны пытаться определить угол места");

    float betta_width =3e8/f_max/(z_max - z_min)/2;
    float betta_step= betta_width/3;

    float alpha_start = 0 - alpha_step;
    float alpha_stop = 2 * M_PI + alpha_step;
    float betta_start = -M_PI/2; // - betta_step;
    float betta_stop = M_PI/2; // + betta_step;

    vector<vector<double>> betta_grid;
    vector<vector<double>> alpha_grid;
    for (float b = betta_start; b <= betta_stop; b+=betta_step)
    {
        vector<double> alpha_vec;
        vector<double> betta_vec;

        for (float a = alpha_start; a <= alpha_stop; a+=alpha_step)
        {
            alpha_vec.push_back(a);
            betta_vec.push_back(b);
        }
        alpha_grid.push_back(alpha_vec);
        betta_grid.push_back(betta_vec);
    }

    coord_grid.push_back(alpha_grid);
    coord_grid.push_back(betta_grid);

    return coord_grid;
}

