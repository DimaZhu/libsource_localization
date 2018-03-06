#include "antenna.h"


Antenna::Antenna():
    orientation(0),
    name("NO NAME")
{
    base = vector<float>(DIM,0);
    phase_center_model = vector<float>(DIM,0);
}


Antenna::Antenna(string str):
    orientation(0),
    name("NO NAME")
{

    string filename(str);
    base = vector<float>(DIM,0);
    phase_center_model = vector<float>(DIM,0);

    //Считывание типа антенны

    int stop = -1;
    for (int i = filename.size() - 1; i >= 0; --i)
    {
        if (filename.at(i) == '.')
            stop = i - 1 ;

        if (filename.at(i) == '/') {
            name = string(filename, i + 1, stop - i);
            break;
        }
    }

    // Инициализация антены по ini файлу

   string line;
   ifstream file(filename);
   if (file.is_open())
   {

       vector<float> x_vec;
       vector<float> y_vec;
       vector<float> z_vec;

       while(getline (file, line))
       {
           std::size_t found = line.find("center_x");
           if (found == 0) {
               found = line.find("=");
               float x= std::stof(std::string(line, found + 1, line.size() - 1));
               phase_center_model.push_back(x);
               continue;
           }

           found = line.find("center_y");
           if (found == 0) {
               found = line.find("=");
               float y= std::stof(std::string(line, found + 1, line.size() - 1));
               phase_center_model.push_back(y);
               continue;
           }

          found = line.find("center_z");
           if (found == 0) {
               found = line.find("=");
               float z= std::stof(std::string(line, found + 1, line.size() - 1));
               phase_center_model.push_back(z);
               continue;
           }

             found = line.find("x");
            if (found == 0) {
                found = line.find("=");
                float x= std::stof(std::string(line, found + 1, line.size() - 1));
                x_vec.push_back(x);
                continue;
            }

            found = line.find("y");
            if (found == 0) {
                found = line.find("=");
                float y = std::stof(string(line, found + 1, line.size() - 1));
                y_vec.push_back(y);
                continue;
            }

            found = line.find("z");
            if (found == 0) {
                found = line.find("=");
                float z = std::stof(string(line, found + 1, line.size() - 1));
                z_vec.push_back(z);
            }
       }

       model.push_back(x_vec);
       model.push_back(y_vec);
       model.push_back(z_vec);

       file.close();
   }

    //Расчитываем сколько элементов на кажом этаже
    calculate_stages_number();

    // Координаты антенных элементов в пространстве. В данном случае угол поворота = 0, как и координаты базы
    calculate_elements_coordinates();
}

Antenna::Antenna(const Antenna &antenna) :
    orientation(0)
{
    base = vector<float>(DIM,0);
    phase_center_model = vector<float>(DIM,0);

    set_model(antenna.get_model());

    set_base(antenna.get_base());

    set_orientation(antenna.get_orientation());

    //set_name(antenna.get_name());
}

Antenna::~Antenna()
{
//    for (quint8 ch = 0; ch < channelNum; ++ch )
//    {
//            free(model[ch]);
//            free(elements[ch]);
//    }
//    free(model);
//    free(elements);
//    free(elementsOnStages);

}

bool operator==(const Antenna &antenna1, const Antenna &antenna2)
{
    // Допустимая погрешность
    float delta = 1e-3;

    if (antenna1.get_channels_total() != antenna2.get_channels_total())
        return false;

    vector<vector<float> > coordinates1 = antenna1.get_elements();
    vector<vector<float> > coordinates2 = antenna2.get_elements();

    for (int i = 0; i < antenna1.get_channels_total(); ++i)
    {
        if ((coordinates1[i][0] - coordinates2[i][0]) > delta)
             return false;

        if ((coordinates1[i][1] - coordinates2[i][0]) > delta)
            return false;

        if ((coordinates1[i][2] - coordinates2[i][0]) > delta)
            return false;
    }

    return true;
}


void Antenna::set_model(vector<vector<float> > coordinates)
{
    // Удаляем предыдущие данные
    elementsOnStages.clear();
    model.clear();

    // Присваиваем
    model = coordinates;

    //Расчитываем сколько элементов на кажом этаже

    calculate_stages_number();
    calculate_elements_coordinates();


}


void Antenna::calculate_stages_number()
{
    if (std::isnan(orientation))
        return;

    elementsOnStages.clear();

    float height = model[2][0];

    // Количество элементов на этажах
    for (int ch = 0; ch < get_channels_total(); ++ch)
    {
        if (height != model[2][ch]) {
            height = model[2][ch];
            elementsOnStages.push_back(ch + 1);
        }

    }
}

void Antenna::set_base( vector< float> coordinates)
{
    assert(coordinates.size() == DIM);
    base = coordinates;

    calculate_elements_coordinates();
}

void Antenna::set_base(vector<vector<float> > coordinates)
{
    base[0] = coordinates[0][0];
    base[1] = coordinates[1][0];
    base[2] = coordinates[2][0];

    calculate_elements_coordinates();
}
void Antenna::set_orientation(float i_orientation)
{
    //Проверка предусловий
    assert((i_orientation < 2*M_PI) || (i_orientation > -2 * M_PI));

    orientation = i_orientation;
    calculate_elements_coordinates();
}

void Antenna::calculate_elements_coordinates()
{
    // Удаляем предыдущие данные
    elements.clear();
    phase_center_real.clear();

    // Считаем
    vector <float> x_vec;
    vector <float> y_vec;
    vector <float> z_vec;

    phase_center_real.push_back(phase_center_model[0] * cos( orientation ) - phase_center_model[1] * sin(orientation) + base[0]);
    phase_center_real.push_back(phase_center_model[0] * sin( orientation) + phase_center_model[1] * cos(orientation) + base[1]);
    phase_center_real.push_back(phase_center_model[2] + base[2]);

    for (int ch = 0; ch < get_channels_total(); ++ch)
    {
        float x = model[0][ch] * cos( orientation ) - model[1][ch] * sin(orientation) + base[0];
        float y = model[0][ch] * sin( orientation) + model[1][ch] * cos(orientation) + base[1];
        float z = model[2][ch] + base[2];

        x_vec.push_back(x);
        y_vec.push_back(y);
        z_vec.push_back(z);
    }

    elements.push_back(x_vec);
    elements.push_back(y_vec);
    elements.push_back(z_vec);
}

vector< vector< float> > Antenna::get_model() const
{
    return model;
}


vector<vector<float> > Antenna::get_base() const
{

    vector<vector<float>> output;
    vector<float> x(base.begin(), base.begin() + 1);
    vector<float> y(base.begin() + 1, base.begin() + 2);
    vector<float> z(base.begin() + 2, base.end());

    output.push_back(x);
    output.push_back(y);
    output.push_back(z);
    return output;
}


vector<float> Antenna::get_elements_on_stages() const
{

    return elementsOnStages;
}

vector<vector<float> > Antenna::get_elements() const
{
    return elements;
}

bool Antenna::adjust_elements_coordinates(vector< vector< float> > correction)
{

    if (correction.size() != elements.size())
        return false;

    // Присваиваем
    for (size_t ch = 0; ch < correction.size(); ++ch)
    {
       float x = elements[0][ch] + correction[0][ch];
       float y = elements[1][ch] + correction[1][ch];
       float z = elements[2][ch] + correction[2][ch];

       elements[0][ch] = x;
       elements[1][ch] = y;
       elements[2][ch] = z;

    }

    return true;
}

void Antenna::set_name(string  i_name)
{
    name = i_name;
}

int Antenna::get_channels_total() const
{
    return model[0].size();
}

string Antenna::get_name() const
{
    return name;
}

float Antenna::get_orientation() const
{
    return orientation;
}


//void Antenna::save_in_file(QTextStream &out) const
//{

//    out << get_channels_total() << "\n";

//    out << base[0] << " " << base[1] << " " << base[2] << "\n";

//    for (int i = 0; i < get_channels_total(); ++i)
//        out << model[i][0] << " " << model[i][1] << " " <<  model[i][2] << "\n";

//    out << orientation << "\n";

//    out << name << "\n";

//}

//Antenna Antenna::read_from_file(QTextStream &in)
//{
//    Antenna antenna;

//    int channelTotal;
//    in >> channelTotal;

//    vector<float> baseCoordinate;
//    float buffer;
//    in >> buffer;
//    baseCoordinate.push_back(buffer);
//    in >> buffer;
//    baseCoordinate.push_back(buffer);
//    in >> buffer;
//    baseCoordinate.push_back(buffer);

//    antenna.set_base(baseCoordinate);


//    vector<vector<float>> model;
//    for (int i = 0; i < channelTotal; ++i) {
//        vector<float> coordinate;
//        in >> buffer;
//        coordinate.push_back(buffer);
//        in >> buffer;
//        coordinate.push_back(buffer);
//        in >> buffer;
//        coordinate.push_back(buffer);

//        model.push_back(coordinate);
//    }

//    antenna.set_model(model);

//    in >> buffer;
//    antenna.set_orientation(buffer);

//    QString name;
//    in >> name;
//    antenna.set_name(name);

//    return antenna;
//}

void Antenna::set_phase_center(vector<float> center)
{
    if (center.size() == 3) {
        phase_center_model = center;
        calculate_elements_coordinates();
    }


}

void Antenna::set_phase_center(vector< vector<float> > center)
{
    if (center.size() == 3) {
        phase_center_model[0] = center[0][0];
        phase_center_model[1] = center[1][0];
        phase_center_model[2] = center[2][0];

        calculate_elements_coordinates();
    }
}

vector<vector<float>> Antenna::get_phase_center() const
{
    vector<vector<float>> output;
    vector<float> x(phase_center_real.begin(), phase_center_real.begin() + 1);
    vector<float> y(phase_center_real.begin() + 1, phase_center_real.begin() + 2);
    vector<float> z(phase_center_real.begin() + 2, phase_center_real.end());

    output.push_back(x);
    output.push_back(y);
    output.push_back(z);
    return output;
}
