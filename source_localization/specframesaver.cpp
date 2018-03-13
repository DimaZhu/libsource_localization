#include "specframesaver.h"

SpecFrameSaver::SpecFrameSaver(int i_post_id) :
    post_id(i_post_id)
{

}

bool SpecFrameSaver::open(string filename)
{
    //string line;
    //ofile.open(filename, std::ios::binary);
    pFile = fopen(filename.c_str(), "wb");
    if (pFile != NULL)
        return true;
    else
        return false;
}

void SpecFrameSaver::save_title(int start_period)
{
    if(pFile == NULL)
        throw std::runtime_error("File wasn't opened");

    string prefix = "Post №" + to_string(post_id);
    fwrite(&start_period, sizeof(int), 1, pFile);
}
void SpecFrameSaver::save(SpecFrame *frame)
{
    if(pFile == NULL)
        throw std::runtime_error("File wasn't opened");


    int ch_total = frame->get_channels_total();
    int samp_per_ch = frame->get_full_frame_length();
    int serial = frame->get_serial();
    double fs = frame->get_sampling_frequency();
    double f_res = frame->get_frequency_resolution();
    int bound = frame->get_bound();
    double f0 = frame->get_central_frequency();
    FrameData data = frame->get_data();


    fwrite(&ch_total, sizeof(int), 1, pFile);
    fwrite(&samp_per_ch, sizeof(int), 1, pFile);
    fwrite(&serial, sizeof(int), 1, pFile);
    fwrite(&fs, sizeof(double), 1, pFile);
    fwrite(&f_res, sizeof(double), 1, pFile);
    fwrite(&bound, sizeof(int), 1, pFile);
    fwrite(&f0, sizeof(double), 1, pFile);

    for (int ch = 0; ch < ch_total; ++ch)
        fwrite(data[ch], sizeof(Complex), samp_per_ch, pFile);
}

int SpecFrameSaver::read_title()
{
    if(pFile == NULL)
        throw std::runtime_error("File wasn't opened");

    char * c_str = NULL;
    fgets(c_str, 20, pFile);
    string str(c_str);
    size_t found = str.find("№");
    int i_post_id = std::stof(std::string(str, found + 1, str.size() - 1));

    if ((post_id != -1) && (i_post_id != post_id))
         throw std::runtime_error("Data loaded from wrong post file");

    int start_period;
    fread(&start_period, sizeof(start_period), 1, pFile);

    return start_period;
}

bool SpecFrameSaver::read(SpecFrame *frame)
{
    if(pFile == NULL)
        throw std::runtime_error("File wasn't opened");

    if (feof(pFile))
        return false;


    int ch_total;
    int samp_per_ch;
    int serial;
    double fs;
    double f_res;
    int bound;
    double f0;


    fread(&ch_total, sizeof(int), 1, pFile);
    fread(&samp_per_ch, sizeof(int), 1, pFile);
    fread(&serial, sizeof(int), 1, pFile);
    fread(&fs, sizeof(double), 1, pFile);
    fread(&f_res, sizeof(double), 1, pFile);
    fread(&bound, sizeof(int), 1, pFile);
    fread(&f0, sizeof(double), 1, pFile);


    resize(frame, ch_total, samp_per_ch);
    write_serial(frame, serial);
    write_sampling_frequency(frame, fs);
    write_frequency_resolution(frame, f_res);
    write_bound(frame, bound);
    write_central_frequency(frame, f0);

    Complex2d data = get_mutable_data(frame);

    for (int ch = 0; ch < ch_total; ++ch)
        fread(data[ch], sizeof(Complex), samp_per_ch, pFile);

    return true;

}

void SpecFrameSaver::close()
{
    if(pFile != NULL)
        fclose(pFile);
}
