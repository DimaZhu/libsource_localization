#ifndef SPECFRAME_H
#define SPECFRAME_H

#include <QtGlobal>
#include <QObject>
#include <cstring>
#include <complex>
#include <vector>


using namespace std;
typedef  vector<vector<complex<double>>> complex2d;
typedef vector<complex<double>> complex1d;

class SpecFrame : public QObject
{
    Q_OBJECT
public:
    SpecFrame();
    SpecFrame(const SpecFrame &frame);

    const SpecFrame &operator=(const SpecFrame& frame);
    complex2d *get_data();
    complex2d get_data(int samp_start, int samp_stop) const;
    complex2d get_data(int ch_start, int ch_stop, int samp_start, int samp_stop) const;
    void push_back(int channels_total, int samp_in_channel, complex<double> **data);
    void set_data(complex2d data);

    void clear();
    void erase();

    //Пересчитвает нужный отсчет в индекс кадра
    int ind(int samp) const;

    bool is_band_limited() const; //Включен ли режим ограниченной полосы
    bool is_in_band(int samp) const; //Находится ли данный отсчет в полосе сигнала

    void set_sampling_frequency(double fs);
    double get_sampling_frequency() const;

    void set_carrier(double f0);
    double get_carrier() const;

    void set_frequency_resolution(double f_res);
    double get_frequency_resolution() const;

    double get_band_width() const;

    int get_channels_total() const;

    int get_length() const;

    int get_full_frame_length() const;

    void set_post_id(int id);
    int get_post_id() const;

    void set_count(int );
    int get_count() const;

    void set_bound(int bound);
    int get_bound() const;

signals:
    void erased(int);

private:
    complex2d data;

    double f0;
    double sampling_frequency;
    double freq_resolution;

    int postId;
    int count;

    int boundInd; // В режиме ограниченной полосы, показывает какому отсчету
                  // целого кадра соответсвует первый отсчет

};


#endif // SPECFRAME_H
