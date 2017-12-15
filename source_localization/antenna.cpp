#include "antenna.h"


Antenna::Antenna():
    orientation(0),
    name("NO NAME")
{
    base = vector<float>(DIM,0);
}

Antenna::Antenna(QString &filename) :
    orientation(0),
    name("NO NAME")
{
    initialization(filename);
}

Antenna::Antenna(char *str):
    orientation(0),
    name("NO NAME")
{

    QString filename(str);
    initialization(filename);
}

Antenna::Antenna(const Antenna &antenna) :
    orientation(0)
{
    base = vector<float>(DIM,0);

    set_model(antenna.get_model());

    set_base(antenna.get_base());

    set_orientation(antenna.get_orientation());

    set_name(antenna.get_name());
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

void Antenna::initialization(QString &filename)
{
    base = vector<float>(DIM,0);

    //Считывание типа антенны
    QRegExp rx("(\\/|\\.)");
    QStringList list = filename.split(rx, QString::SkipEmptyParts);
    name= list.at(list.length() - 2);

    // Инициализация антены по ini файлу
    bool ok = true;
    QSettings antennaSettings(filename, QSettings::IniFormat);

    quint8 chTotal = antennaSettings.value(QString::fromUtf8("DEFAULT/channels number"), "-1").toInt(&ok);

    for (quint8 ch = 0; ch < chTotal; ++ch)
    {

        vector<float> point;

        float alpha = antennaSettings.value(QString::fromUtf8("DEFAULT/alpha%1").arg(ch), "-1").toDouble(&ok);
        alpha *= M_PI / 180;
//            if( ok != true) {
//                emit error("Error occured while reading from " + filename);
//                return;
//            }

        float radius = antennaSettings.value(QString::fromUtf8("DEFAULT/r%1").arg(ch), "-1").toDouble(&ok);
//            if( ok != true) {
//                emit error("Error occured while reading from " + filename);
//                return;
//            }

        float z = antennaSettings.value(QString::fromUtf8("DEFAULT/z%1").arg(ch), "-1").toDouble(&ok);
//            if( ok != true) {
//                emit error("Error occured while reading from " + filename);
//                return;
//            }

        float x =radius * cos( alpha);
        float y =  radius * sin( alpha);

        point.push_back(x);
        point.push_back(y);
        point.push_back(z);
        model.push_back(point);
    }

    //Расчитываем сколько элементов на кажом этаже
    calculate_stages_number();

    // Координаты антенных элементов в пространстве. В данном случае угол поворота = 0, как и координаты базы
    calculate_elements_coordinates();
}

bool operator==(const Antenna &antenna1, const Antenna &antenna2)
{
    // Допустимая погрешность
    float delta = 1e-3;

    if (antenna1.get_channels_total() != antenna2.get_channels_total())
        return false;

    vector<vector<float>> coordinates1 = antenna1.get_elements();
    vector<vector<float>> coordinates2 = antenna2.get_elements();

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


void Antenna::set_model(vector<vector<float>> coordinates)
{
    // Удаляем предыдущие данные
    elementsOnStages.clear();

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

    float height = model[0][2];

    // Количество элементов на этажах
    for (int ch = 0; ch < get_channels_total(); ++ch)
    {
        if (height != model[ch][2]) {
            height = model[ch][2];
            elementsOnStages.push_back(ch + 1);
        }

    }
}

void Antenna::set_base( vector< float> coordinates)
{
    Q_ASSERT(coordinates.size() == DIM);
    base = coordinates;
}

void Antenna::set_orientation(float i_orientation)
{
    //Проверка предусловий
    Q_ASSERT_X((i_orientation < 2*M_PI) || (i_orientation > -2 * M_PI), "Seems that oriantation is in degress, not in radians", "antenna.cpp, set_orientation");

    orientation = i_orientation;
    calculate_elements_coordinates();
}

void Antenna::calculate_elements_coordinates()
{
    // Удаляем предыдущие данные
    elements.clear();

    // Считаем
    for (quint8 ch = 0; ch < get_channels_total(); ++ch)
    {
        float x = model[ch][0] * cos( orientation ) - model[ch][1] * sin(orientation) + base[0];
        float y = model[ch][0] * sin( orientation) + model[ch][1] * cos(orientation) + base[1];
        float z = model[ch][2] + base[2];
        vector<float > point;
        point.push_back(x);
        point.push_back(y);
        point.push_back(z);
        elements.push_back(point);
    }
}

vector< vector< float>> Antenna::get_model() const
{
    return model;
}


vector<float> Antenna::get_base() const
{

    return base;
}


vector<float> Antenna::get_elements_on_stages() const
{

    return elementsOnStages;
}

vector<vector<float> > Antenna::get_elements() const
{
    return elements;
}

bool Antenna::adjust_elements_coordinates(vector< vector< float>> correction)
{

    if (correction.size() != elements.size())
        return false;

    // Присваиваем
    for (quint8 ch = 0; ch < correction.size(); ++ch)
    {
       float x = elements[ch][0] + correction[ch][0];
       float y = elements[ch][1] + correction[ch][1];
       float z = elements[ch][2] + correction[ch][2];

       elements[ch][0] = x;
       elements[ch][1] = y;
       elements[ch][2] = z;

    }

    return true;
}

void Antenna::set_name(QString  i_name)
{
    name = i_name;
}

int Antenna::get_channels_total() const
{
    return model.size();
}

QString Antenna::get_name() const
{
    return name;
}

float Antenna::get_orientation() const
{
    return orientation;
}


void Antenna::save_in_file(QTextStream &out) const
{

    out << get_channels_total() << "\n";

    out << base[0] << " " << base[1] << " " << base[2] << "\n";

    for (int i = 0; i < get_channels_total(); ++i)
        out << model[i][0] << " " << model[i][1] << " " <<  model[i][2] << "\n";

    out << orientation << "\n";

    out << name << "\n";

}

Antenna Antenna::read_from_file(QTextStream &in)
{
    Antenna antenna;

    int channelTotal;
    in >> channelTotal;

    vector<float> baseCoordinate;
    float buffer;
    in >> buffer;
    baseCoordinate.push_back(buffer);
    in >> buffer;
    baseCoordinate.push_back(buffer);
    in >> buffer;
    baseCoordinate.push_back(buffer);

    antenna.set_base(baseCoordinate);


    vector<vector<float>> model;
    for (int i = 0; i < channelTotal; ++i) {
        vector<float> coordinate;
        in >> buffer;
        coordinate.push_back(buffer);
        in >> buffer;
        coordinate.push_back(buffer);
        in >> buffer;
        coordinate.push_back(buffer);

        model.push_back(coordinate);
    }

    antenna.set_model(model);

    in >> buffer;
    antenna.set_orientation(buffer);

    QString name;
    in >> name;
    antenna.set_name(name);

    return antenna;
}


