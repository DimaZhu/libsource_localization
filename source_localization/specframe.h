#ifndef SPECFRAME_H
#define SPECFRAME_H

#include <cstring>
#include <complex>
#include <vector>
#include <deque>
#include <cassert>
#include <complex.h>
#undef complex
using namespace std;
typedef  std::complex<float >const  *const *const complex2d;
typedef  std::complex<float > const * const complex1d;

class SpecFrameParent
{
public:
    SpecFrameParent(){}

    virtual void erase_frame(int serial) = 0;
};


class SpecFrame
{
public:

    friend class SpecFrameWriter;

    SpecFrame(int i_channels_total = 0, int i_samp_per_ch = 0);
    SpecFrame(const SpecFrame &frame);
    ~SpecFrame();

    const SpecFrame &operator=(const SpecFrame& frame);

    SpecFrame *copy(); //Возвращает ссылку на себя. Только после того, как у всех копий будет вызван метод erase(),
    // будет послан сигнал erased.

    complex2d get_data(int &channels, int &samp_per_ch) const;

    complex2d get_data() const;

    void filter(complex2d freq_response); // Одновременная фильтрация всех каналов. КХЧ должны иметь размер полного кадра.

    void filter(complex1d freq_response, int ch_ind); // Фильтрация конкретного канала. КХЧ должна иметь размер полного кадра.

    void erase();  //После обработки кадра, каждый поток вызывает этот метод для уничтожения кадра.
    // Как только кадр освободят все потоки, шлется сигнал erased.

    int ind(int samp) const; //Пересчитвает нужный отсчет в индекс кадра

    bool is_band_limited() const; //Включен ли режим ограниченной полосы

    bool is_in_band(int samp) const; // Проверяет входит ли данный отсчет в частотную полосу, заполненую данными.

    double get_sampling_frequency() const; //Установка частоты дискретизации

    double get_central_frequency() const; // Установка центральной частоты приемника

    double get_frequency_resolution() const;

    double get_band_width() const; //Получить ширину частотной полосы, заполненную данными

    int get_channels_total() const; // Получить количество каналов

    int get_length() const; //Количество отсчетов в спектральной полосе, заполненной данными

    int get_full_frame_length() const; // Количество отсчетов в карде при передачи целого кадра

    int get_post_id() const; //Получить id станции

    int get_serial() const; //Получить номер кадра

    int get_bound() const;

private:
    void allocate_memory();

    void free_memory();
    void clear(); //очистить данные
    void resize(int i_channels_total, int i_samp_per_ch); // Изменить размеры массива данных

    SpecFrameParent *parent;
    std::complex< float > ** data;
    int channels_total;
    int samp_per_channel;

    double f0;
    double sampling_frequency;
    double freq_resolution;

    int postId;
    int serial;

    int boundInd; // В режиме ограниченной полосы, показывает какому отсчету
                  // целого кадра соответсвует первый отсчет

    int state;

    int reference_counter;
};
#endif // SPECFRAME_H
