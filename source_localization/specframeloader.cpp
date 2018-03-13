#include "specframeloader.h"

SpecFrameLoader::SpecFrameLoader(int i_post_id):
    post_id(i_post_id)
{

}

bool SpecFrameLoader::open(string filename)
{
    pFile = fopen(filename.c_str(), "r");
    if (pFile != NULL)
        return true;
    else
        return false;
}

float SpecFrameLoader::read_title()
{
    if(pFile == NULL)
        throw std::runtime_error("File wasn't opened");

    size_t c_str_len;
    fread(&c_str_len, sizeof(size_t), 1, pFile);
    c_str_len += 1;


    char * c_str = NULL;
    c_str = (char *) malloc(c_str_len);
    c_str[c_str_len - 1] = '\0';

    size_t readed =  fread(c_str, sizeof(char), c_str_len-1, pFile);

    if (readed != c_str_len-1) {
        close();
        throw std::runtime_error("Enable to read title");
    }

    string str(c_str);
    size_t found = str.find("N");

    if (found >= str.length()){
        close();
        throw std::runtime_error("Wrong file title");
    }

    int i_post_id = std::stof(std::string(str, found + 1, str.size() - 1));

    if ((post_id != -1) && (i_post_id != post_id)) {
         close();
         throw std::runtime_error("Data loaded from wrong post file");
    }

    float start_period;
    fread(&start_period, sizeof(start_period), 1, pFile);

    return start_period;
}

bool SpecFrameLoader::load(SpecFrame *frame)
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


    write_post_id(frame, post_id);
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

void SpecFrameLoader::close()
{
    if(pFile != NULL)
        fclose(pFile);
}
