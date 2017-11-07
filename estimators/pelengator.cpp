#include "pelengator.h"

Pelengator::Pelengator(Antenna i_ant):
    antenna(i_ant)
{

}

void Pelengator::set_signal_param(double f0, double df, double f_res, double fs)
{
    grid = pel_grid(antenna, f0, df, f_res, fs);
}

void Pelengator::estimate(SpecFrame frame)
{
    lh
}
