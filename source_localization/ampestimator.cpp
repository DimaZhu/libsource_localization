#include "ampestimator.h"

AmpEstimator::AmpEstimator()
{

}

void AmpEstimator::set_signal_parameter(int i_samp_start, int i_samp_stop)
{

    if (i_samp_stop < i_samp_start) {
        int buf = i_samp_start;
        i_samp_start = i_samp_stop;
        i_samp_stop = buf;
    }

    samp_start = i_samp_start;
    samp_stop = i_samp_stop;
}

Estimation AmpEstimator::estimate(SpecFrame *frame) const
{
      if (samp_start < frame->get_bound())
          throw std::runtime_error("Sample is out of frame bounds");

      if (samp_stop > (frame->get_bound() + frame->get_length()))
          throw std::runtime_error("Sample is out of frame bounds");

      Estimation estimation;
      complex2d data = frame->get_data();


      for (int ch = 0; ch < frame->get_channels_total(); ++ch)
      {

        double amp = 0;
        for(int s = samp_start; s < samp_stop; ++s)
        {
            double  value = abs(data[ch][s]);

            if ( value == 0.0) //Проверка на потерю данных
                  amp = NAN;
            else
                  amp +=value;
        }

        amp /= samp_start - samp_stop + 1;
        estimation.push_back(amp);
      }
      return estimation;

}
