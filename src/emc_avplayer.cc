
#include <iostream>
#include <stdio.h>


#include "emc.hh"

struct AV_Data
{
};

static void
debug_show(emc_app_av *av)
{
   std::cout << "File: " << av->file_get() << std::endl;
   std::cout << "Position: " << av->position_get() << std::endl;
   std::cout << "Volume: " << av->volume_get() << std::endl;
   std::cout << "Play: " << ((av->play_get() == EINA_TRUE) ? "true" : "false") << std::endl;
}

Eina_Bool
emc_app_av::file_set(const std::string &filename)
{
   this->av_filename = filename;
   return EINA_TRUE;
}

Eina_Bool
emc_app_av::position_set(double position)
{
   this->av_position = position;
   return EINA_TRUE;
}

Eina_Bool
emc_app_av::volume_set(double volume)
{
   this->av_volume = volume;
   return EINA_TRUE;
}

Eina_Bool
emc_app_av::play_set(Eina_Bool play)
{
   Eo* test;
   struct AV_Data avdata;
   memset(&avdata, 0, sizeof(struct AV_Data));

   if(this->av_play == EINA_FALSE)
     {
        this->av_play = play;

        ::elm_win win = this->emc_app_win_get();

        elm_box bigbox( efl::eo::parent = win );
        bigbox.size_hint_weight_set(EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        win.resize_object_add(bigbox);
        bigbox.visibility_set(true);
        bigbox.horizontal_set(true);
        bigbox.show();
        win.callback_del_add(clean_ref(bigbox));

        //XXX: Implement lambda+signals
        elm_video video ( efl::eo::parent = win );
        video.file_set(file_get());
        video.play_position_set(position_get());
        video.audio_level_set(volume_get());

        // Fits the window size
        video.size_hint_weight_set(EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        win.resize_object_add(video);

        video.visibility_set(true);
        win.callback_del_add(clean_ref(video));

        // Buttons
        elm_box buttons ( efl::eo::parent = win );
        buttons.horizontal_set(EINA_TRUE);
        bigbox.pack_end(buttons);
        buttons.visibility_set(true);
        win.callback_del_add(clean_ref(buttons));

        elm_button play ( efl::eo::parent = win );
        play.text_set("elm.text", "Play");
        buttons.pack_end(play);
        play.visibility_set(true);

        // lambda 'Play' button
        play.callback_clicked_add
            (std::bind([&avdata]
                       { //XXX: implement

                       }));
        win.callback_del_add(clean_ref(play));

        elm_button pause ( efl::eo::parent = win );
        pause.text_set("elm.text", "Pause");
        buttons.pack_end(pause);
        pause.visibility_set(true);

        // lambda 'Pause' button
        pause.callback_clicked_add
            (std::bind([&avdata]
                       { //XXX: implement

                       }));
        win.callback_del_add(clean_ref(pause));

        debug_show(this);
        video.play();

        win.size_set(300, 320);
        win.visibility_set(true);
        std::cout << "references to win " << win.ref_get() << std::endl;
        test = win._eo_ptr();
        win._release();

        return EINA_TRUE;
     }

   return EINA_FALSE;
}

