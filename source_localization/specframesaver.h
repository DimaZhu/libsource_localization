#ifndef FRAME_SAVER_H
#define FRAME_SAVER_H

#include <specframe.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>

// Последовательно сохраняет обьекты класса SpecFrame.
//

// Формат файла:
//      Загаловок: номер станции и период старта, при котором происходила запись.
//      Frame0
//      Frame1
//       ....

// Перед тем как начать записывать кадры, вызывается save_title метод

using namespace std;
class SpecFrameSaver
{
public:
    SpecFrameSaver(int post_id = -1);//
    // При значении -1 читает все кадры подряд.
    bool open(string filename);
    void save_title(float start_period);
    void save(SpecFrame *frame);
    void close();

private:
    FILE *pFile;
    int post_id;
};

#endif // FRAME_SAVER_H
