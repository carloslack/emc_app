
#include <iostream>
#include <stdio.h>
#include "emc.hh"

Eina_Bool
emc_app_av::file_set(const std::string &filename)
{
   this->av_filename = filename;
   return EINA_TRUE;
}

Eina_Bool
emc_app_av::position_set(const double &position)
{
   this->av_position = position;
   return EINA_TRUE;
}

Eina_Bool
emc_app_av::volume_set(const double &volume)
{
   this->av_volume = volume;
   return EINA_TRUE;
}

Eina_Bool
emc_app_av::play_set(const Eina_Bool &play)
{
   Eo* test;
   Eina_Bool _play = this->av_play;

   if(_play == EINA_FALSE)
     {
        this->av_play = play;

        ::elm_win win = this->emc_app_win_get();

        elm_box bigbox ( efl::eo::parent = win );
        bigbox.size_hint_weight_set(EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        win.resize_object_add(bigbox);
        bigbox.visibility_set(true);
        win.callback_del_add(clean_ref(bigbox));

        win.size_set(300, 320);
        win.visibility_set(true);
        std::cout << "references to win " << win.ref_get() << std::endl;
        test = win._eo_ptr();
        win._release();

        return EINA_TRUE;
     }

   return EINA_FALSE;
}

