#include "specframesaver.h"

SpecFrameSaver::SpecFrameSaver(int i_post_id) :
    post_id(i_post_id)
{

}

bool SpecFrameSaver::open(string filename)
{
    //string line;
    //ofile.open(filename, std::ios::binary);
    pFile = fopen(filename.c_str(), "w");
    if (pFile != NULL)
        return true;
    else
        return false;
}

void SpecFrameSaver::save_title(float start_period)
{
    if(pFile == NULL)
        throw std::runtime_error("File wasn't opened");

    string prefix = "Post N" + to_string(post_id);
    size_t prefix_len = prefix.length();

    fwrite(&prefix_len, sizeof(size_t), 1, pFile);
    fwrite(prefix.c_str(), sizeof(char), prefix.length(), pFile);
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

void SpecFrameSaver::close()
{
    if(pFile != NULL)
        fclose(pFile);
}
