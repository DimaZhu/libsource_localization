#ifndef SPECFRAME_WRITER_H
#define SPECFRAME_WRITER_H

#include <specframe.h>

class SpecFrameWriter
{
public:
    SpecFrameWriter(){}

protected:
    void write_sampling_frequency(SpecFrame *frame, double fs) {frame->sampling_frequency = fs; }
    void write_central_frequency(SpecFrame *frame, double f0) {frame->f0 = f0;}
    void write_frequency_resolution(SpecFrame *frame, double f_res) {frame->freq_resolution = f_res;}
    void write_post_id(SpecFrame *frame, double post_id) {frame->postId = post_id;}
    void write_serial(SpecFrame *frame, int serial) {frame->serial = serial;}
    void write_bound(SpecFrame *frame, int bound) {frame->boundInd = bound;}
    void write_parent(SpecFrame * frame, SpecFrameParent *parent) {frame->parent = parent;}
    std::complex< float > **  get_mutable_data(SpecFrame *frame) {return frame->data;}
    void resize(SpecFrame *frame, int channels, int samp_per_ch) {frame->resize(channels, samp_per_ch);}
    void clear(SpecFrame *frame) {frame->clear();}

};

#endif // SPECFRAME_WRITER_H
