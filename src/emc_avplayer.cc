
#include <iostream>
#include "emc.hh"

void
emc_app_av::file_set(std::string &filename)
{
   av_filename = filename;
}

void
emc_app_av::position_set(const double &position)
{
   av_position = position;
}

void
emc_app_av::volume_set(const double &volume)
{
   av_volume = volume;
}

int emc_avplayer(elm_box &box, const std::string &str)
{
   std::cout << "reading from file: " << str << std::endl;
   return 0;
}
