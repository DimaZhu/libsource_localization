#ifndef SPECFRAMELOADER_H
#define SPECFRAMELOADER_H

#include <specframe.h>
#include <specframewriter.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>

// Последовательно считывает обьекты класса SpecFrame.
//

// Формат файла:
//      Загаловок: номер станции и период старта, при котором происходила запись.
//      Frame0
//      Frame1
//       ....

// Перед считыванием, необходимо вызвать read_title.

using namespace std;
class SpecFrameLoader: public SpecFrameWriter
{
public:
    SpecFrameLoader(int post_id = -1);
    // При значении -1 читает все кадры подряд.
    bool open(string filename);
    float read_title(); // Возвращает период старта
    bool load(SpecFrame *frame);
    void close();

private:
    FILE *pFile;
    int post_id;
};

#endif // SPECFRAMELOADER_H
