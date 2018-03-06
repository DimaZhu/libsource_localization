#include "triangulator.h"

Triangulator::Triangulator():
    active(false)
{

}

void Triangulator::set_active(bool arg)
{
    active = arg;
}

bool Triangulator::is_active()
{
    return active;
}

coordinates Triangulator::estimate(vector<Peleng> pelengs)
{
    // Оценка местоположения это центр масс равномерного многоугольника, образованного пеленгами.
    // Вершина многоугольника это середина кратчайщего расстояния между двумя скрещивающимися
    // прямыми
    //

    int vertex_total = pelengs.size() * (pelengs.size() - 1) / 2;
    dlib::matrix<double, 3, 1> verteces;
    verteces.set_size(3, vertex_total);
    coordinates target = {vector<float>{0}, vector<float>{0}, vector<float>{0}};
    for (uint i = 0; i < pelengs.size() - 1; ++i)
    {
        for (uint j = i + 1; j < pelengs.size(); ++j)
        {
            dlib::matrix<double, 3, 0> vertex;
            vertex = calculate_vertex(pelengs[i], pelengs[j]);
            target[0][0] += vertex(0,0);
            target[1][0] += vertex(1, 0);
            target[2][0] += vertex(2, 0);
        }
    }

    target[0][0] /= vertex_total;
    target[1][0] /= vertex_total;
    target[2][0] /= vertex_total;
    return target;
}


dlib::matrix<double, 3, 1> Triangulator::calculate_vertex(Peleng &peleng0, Peleng &peleng1) const
{
    double x0 = peleng0.phase_center[0][0];
    double y0 = peleng0.phase_center[1][0];
    double z0 = peleng0.phase_center[2][0];
    double alpha0 = peleng0.azimuth;
    double betta0 = peleng0.elevation;

    double x1 = peleng1.phase_center[0][0];
    double y1 = peleng1.phase_center[1][0];
    double z1 = peleng1.phase_center[2][0];
    double alpha1 = peleng1.azimuth;
    double betta1 = peleng1.elevation;

    // Составляем уравнения для плоскости парралельной прямой a и проходящей через прямую b,
    // а так же для плоскостей, образующих прямую b
    dlib::matrix<double,3, 1> normal;
    normal = sin(alpha0)*sin(betta1)*cos(betta0) - sin(alpha1)*sin(betta0)*cos(betta1),
                    sin(betta0)*cos(alpha1)*cos(betta1) - sin(betta1)*cos(alpha0)*cos(betta0),
                    -sin(alpha0)*cos(alpha1)*cos(betta0)*cos(betta1) + sin(alpha1)*cos(alpha0)*cos(betta0)*cos(betta1);

    dlib::matrix<double, 5, 3> A;
    A = -normal(1)*sin(betta0) + normal(2)*sin(alpha0)*cos(betta0),  normal(0)*sin(betta0) - normal(2)*cos(alpha0)*cos(betta0),  -normal(0)*sin(alpha0)*cos(betta0) + normal(1)*cos(alpha0)*cos(betta0),
           normal(0), normal(1), normal(2),
           cos(betta1)*sin(alpha1), - cos(betta1)*cos(alpha1), 0,
           sin(betta1), 0, -cos(betta1)*cos(alpha1),
           0, sin(betta1), -cos(betta1)*sin(alpha1);

    dlib::matrix<double, 5, 1> b;
    b = normal(0)*y0*sin(betta0) - normal(0)*z0*sin(alpha0)*cos(betta0) - normal(1)*x0*sin(betta0) + normal(1)*z0*cos(alpha0)*cos(betta0) + normal(2)*x0*sin(alpha0)*cos(betta0) - normal(2)*y0*cos(alpha0)*cos(betta0),
          normal(0)*x1 + normal(1)*y1 + normal(2)*z1,
          x1*cos(betta1)*sin(alpha1) - y1*cos(betta1)*cos(alpha1),
          x1*sin(betta1) - z1*cos(betta1)*cos(alpha1),
          y1*sin(betta1) - z1*cos(betta1)*sin(alpha1);

    // Точка пересечения проекции прямой a и прямой b
    dlib::matrix<double, 3, 1> p;
    p = dlib::inv(dlib::trans(A) * A) * dlib::trans(A) * b;


    // Cоставляем уравнения для плоскостей образующих прямую a, а так же для плоскостей образующих
    // прямую, заданую точко p и нормалью normal

    dlib::matrix<double, 6, 3> B;
    B =cos(betta0)*sin(alpha0), - cos(betta0)*cos(alpha0), 0,
          sin(betta0), 0, - cos(betta0)*cos(alpha0),
          0, sin(betta0), -cos(betta0)*sin(alpha0),
          normal(1), -normal(0), 0,
          normal(2), 0, -normal(0),
          0, normal(2), -normal(1);

    dlib::matrix<double, 6, 1> c;
    c = x0*cos(betta0)*sin(alpha0) - y0*cos(betta0)*cos(alpha0),
          x0*sin(betta0) - z0*cos(betta0)*cos(alpha0),
          y0*sin(betta0) - z0*cos(betta0)*sin(alpha0),
          p(0)*normal(1) - p(1)*normal(0),
          p(0)*normal(2) - p(2)*normal(0),
          p(1)*normal(2) - p(2)*normal(1);

    dlib::matrix<double, 3, 1> q;
    q = dlib::inv(dlib::trans(B) * B) * dlib::trans(B) * c;

    dlib::matrix<double, 3, 1> vertex;
    vertex = (p + q) / 2;

    return vertex;
}
