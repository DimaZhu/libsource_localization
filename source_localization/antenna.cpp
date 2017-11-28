#include "antenna.h"


Antenna::Antenna():
    orientation(0),
    model("NO NAME")
{
    baseCoordinates = vector<float>(DIM,0);
}

Antenna::Antenna(QString &filename) :
    orientation(0),
    model("NO NAME")
{
    initialization(filename);
}

Antenna::Antenna(char *str):
    orientation(0),
    model("NO NAME")
{

    QString filename(str);
    initialization(filename);
}

Antenna::Antenna(const Antenna &antenna) :
    orientation(0)
{
    baseCoordinates = vector<float>(DIM,0);

    set_model_coordinates(antenna.get_model_coordinates());

    set_base_coordinates(antenna.get_base_coordinates());

    set_orientation(antenna.get_orientation());

    set_model_name(antenna.get_model_name());
}

Antenna::~Antenna()
{
//    for (quint8 ch = 0; ch < channelNum; ++ch )
//    {
//            free(modelCoordinates[ch]);
//            free(elementsCoordinates[ch]);
//    }
//    free(modelCoordinates);
//    free(elementsCoordinates);
//    free(elementsOnStages);

}

void Antenna::initialization(QString &filename)
{
    baseCoordinates = vector<float>(DIM,0);

    //Считывание типа антенны
    QRegExp rx("(\\/|\\.)");
    QStringList list = filename.split(rx, QString::SkipEmptyParts);
    model = list.at(list.length() - 2);

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
        modelCoordinates.push_back(point);
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

    vector<vector<float>> coordinates1 = antenna1.get_elements_coordinates();
    vector<vector<float>> coordinates2 = antenna2.get_elements_coordinates();

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


void Antenna::set_model_coordinates(vector<vector<float>> coordinates)
{
    // Удаляем предыдущие данные
    elementsOnStages.clear();

    // Присваиваем
    modelCoordinates = coordinates;

    //Расчитываем сколько элементов на кажом этаже

    calculate_stages_number();
    calculate_elements_coordinates();


}


void Antenna::calculate_stages_number()
{
    if (std::isnan(orientation))
        return;

    elementsOnStages.clear();

    float height = modelCoordinates[0][2];

    // Количество элементов на этажах
    for (int ch = 0; ch < get_channels_total(); ++ch)
    {
        if (height != modelCoordinates[ch][2]) {
            height = modelCoordinates[ch][2];
            elementsOnStages.push_back(ch + 1);
        }

    }
}

void Antenna::set_base_coordinates( vector< float> coordinates)
{
    Q_ASSERT(coordinates.size() == DIM);
    baseCoordinates = coordinates;
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
    elementsCoordinates.clear();

    // Считаем
    for (quint8 ch = 0; ch < get_channels_total(); ++ch)
    {
        float x = modelCoordinates[ch][0] * cos( orientation ) - modelCoordinates[ch][1] * sin(orientation) + baseCoordinates[0];
        float y = modelCoordinates[ch][0] * sin( orientation) + modelCoordinates[ch][1] * cos(orientation) + baseCoordinates[1];
        float z = modelCoordinates[ch][2] + baseCoordinates[2];
        vector<float > point;
        point.push_back(x);
        point.push_back(y);
        point.push_back(z);
        elementsCoordinates.push_back(point);
    }
}

vector< vector< float>> Antenna::get_model_coordinates() const
{
    return modelCoordinates;
}


vector<float> Antenna::get_base_coordinates() const
{

    return baseCoordinates;
}


vector<float> Antenna::get_elements_on_stages() const
{

    return elementsOnStages;
}

vector<vector<float> > Antenna::get_elements_coordinates() const
{
    return elementsCoordinates;
}

bool Antenna::adjust_elements_coordinates(vector< vector< float>> correction)
{

    if (correction.size() != elementsCoordinates.size())
        return false;

    // Присваиваем
    for (quint8 ch = 0; ch < correction.size(); ++ch)
    {
       float x = elementsCoordinates[ch][0] + correction[ch][0];
       float y = elementsCoordinates[ch][1] + correction[ch][1];
       float z = elementsCoordinates[ch][2] + correction[ch][2];

       elementsCoordinates[ch][0] = x;
       elementsCoordinates[ch][1] = y;
       elementsCoordinates[ch][2] = z;

    }

    return true;
}

void Antenna::set_model_name(QString  i_model)
{
    model = i_model;
}

int Antenna::get_channels_total() const
{
    return modelCoordinates.size();
}

QString Antenna::get_model_name() const
{
    return model;
}

float Antenna::get_orientation() const
{
    return orientation;
}


void Antenna::save_in_file(QTextStream &out) const
{

    out << get_channels_total() << "\n";

    out << baseCoordinates[0] << " " << baseCoordinates[1] << " " << baseCoordinates[2] << "\n";

    for (int i = 0; i < get_channels_total(); ++i)
        out << modelCoordinates[i][0] << " " << modelCoordinates[i][1] << " " <<  modelCoordinates[i][2] << "\n";

    out << orientation << "\n";

    out << model << "\n";

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

    antenna.set_base_coordinates(baseCoordinate);


    vector<vector<float>> modelCoordinates;
    for (int i = 0; i < channelTotal; ++i) {
        vector<float> coordinate;
        in >> buffer;
        coordinate.push_back(buffer);
        in >> buffer;
        coordinate.push_back(buffer);
        in >> buffer;
        coordinate.push_back(buffer);

        modelCoordinates.push_back(coordinate);
    }

    antenna.set_model_coordinates(modelCoordinates);

    in >> buffer;
    antenna.set_orientation(buffer);

    QString name;
    in >> name;
    antenna.set_model_name(name);

    return antenna;
}


