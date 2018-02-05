#include "interpolator.h"

Interpolator::Interpolator()
{

}

 vector<vector<double>> Interpolator::find_minimum2d(vector<vector<vector<double>>> x, vector<vector<double>> y, Lh *function)
{
/* Функция find_minimum2d осуществляет оценку глобального минимума целевой функции
 * интерполяцией массива данных парабалоидом размерностью [3][3]. Экстремум, которого находится по известным
 * формулам. В полученном минимуме находится значение целовой функции. Наименьшее
 * Входными аргументами являются:
 * 1) Матрица x размерностью [2][m][n], где  x[0] - двухмерная матрица значений 1 аргумента,
 *      x[1] -двухмерная матрица значений 2го аргумента.
 * 2) Матрица y размерностью [m][n]. Это матрица значений целевой функции, вычисленная
 *     по значениям x.
 * 3) function - ссылка на целевую функцию
 *
*/
    dlib::matrix<double, 2, 1> arg_min;
    arg_min = x[0][0][0], x[1][0][0];

    double fun_min = y[0][0];

    try {
        if ((x[0].size() != y.size()) || (x[0][0].size() != y[0].size())) {
            throw std::runtime_error("Error in find_minimum2d. Matrices x and y doesn't match");
        }


        int arg1_length = x[0][0].size();
        int arg2_length = x[0].size();

        for (int i = 0; i < arg2_length - 2; ++i)
        {
            for (int j = 0; j < arg1_length - 2; ++j)
            {
                double x1 = x[0][i][j];
                double y1 = x[1][i][j];
                double z1 = y[i][ j];

                double x2 = x[0][i][j+1];
                double y2 = x[1][i][j+1];
                double z2 = y[i][j+1];

                double x3 = x[0][i][j+2];
                double y3 = x[1][i][j+2];
                double z3 = y[i][j+2];

                double x4 = x[0][i+2][j];
                double y4 = x[1][i+2][j];
                double z4 = y[i+2][j];

                double x5 = x[0][i+1][j+1];
                double y5 = x[1][i+1][j+1];
                double z5 = y[i+1][j+1];

                double x6 = x[0][i+2][j+2];
                double y6 = x[1][i+2][j+2];
                double z6 = y[i+2][j+2];

                dlib::matrix<double, 6, 6> observation_matrix;
                observation_matrix = 1, y1, x1, x1*y1, y1*y1, x1*x1,
                                                   1, y2, x2, x2*y2, y2*y2, x2*x2,
                                                   1, y3, x3, x3*y3, y3*y3, x3*x3,
                                                   1, y4, x4, x4*y4, y4*y4, x4*x4,
                                                   1, y5, x5, x5*y5, y5*y5, x5*x5,
                                                   1, y6, x6, x6*y6, y6*y6, x6*x6;

                dlib::matrix<double, 6, 1> observations;
                observations = z1,
                                        z2,
                                        z3,
                                        z4,
                                        z5,
                                        z6;

                // Оценка коэффициентов полинома методом наименьших квадратов
                dlib::matrix<double, 6, 1> pol_coef;
                pol_coef = dlib::inv(dlib::trans(observation_matrix) * observation_matrix) * dlib::trans(observation_matrix) * observations;

                // Поиск экстремума
                dlib::matrix<double, 2, 2> A;
                A = 2 * pol_coef(0, 5), pol_coef(0, 3),
                      pol_coef(0, 3), 2 * pol_coef(0, 4);

                dlib::matrix<double, 2, 1> b;
                b = - pol_coef(0, 2),
                      - pol_coef(0, 1);

                dlib::matrix<double, 2, 1> extreme;
                extreme = dlib::inv(dlib::trans(A) * A) * dlib::trans(A) * b;



                //Проверка на выход допустимых значений
                if ((extreme(0, 0) < x[0][0][1]) || (extreme(0, 0) > x[0][0][arg1_length - 2]))
                    continue;
                if ((extreme(1, 0) < x[1][1][0]) || (extreme(1, 0) > x[1][arg2_length - 2][0]))
                    continue;

                // Проверка на признак параболоида
                if (pol_coef(5) * pol_coef(4) < 0)
                    continue;

                double fun_val = (*function)(extreme);

                if (fun_val < fun_min){
                    fun_min = fun_val;
                    arg_min = extreme;
                }
            }
        }



     } catch (std::exception &e){
         // qDebug() << e.what();
     }



     vector<vector<double>> output;
     vector<double> buf;
     buf.push_back(arg_min(0, 0));
     buf.push_back(arg_min(1, 0));
     output.push_back(buf);

     buf.clear();
     buf.push_back(fun_min);
     output.push_back(buf);
     return output;
}
