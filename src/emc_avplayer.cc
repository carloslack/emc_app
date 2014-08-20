#include <iostream>
#include <stdio.h>

#include "emc.hh"

struct AV_Data
{
   efl::eo::wref<elm_video> video;
   bool loop;
};

Eina_Bool
emc_app_av::file_set(const std::string &filename)
{
   if(!filename.size())
     return EINA_FALSE;

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
emc_app_av::loop_set(bool loop)
{
   this->av_loop = loop;
   return EINA_TRUE;
}

static void
video_obj_stopped_cb(void *data EINA_UNUSED, Evas_Object *obj, void *event_info EINA_UNUSED)
{
   struct AV_Data *av = static_cast<struct AV_Data*>(data);
   std::cout  << "video stopped!" << std::endl;

   if(av->loop == true)
     {
        if(efl::eina::optional<elm_video> video = av->video.lock())
          {
             video->play_position_set(0.0);
             video->play();
          }
     }
}
static void
video_obj_progress_cb(void *data EINA_UNUSED, Evas_Object *obj, void *event_info EINA_UNUSED)
{
   struct AV_Data *av = (struct AV_Data*)data;

   if(efl::eina::optional<elm_video> video = av->video.lock())
     {
     }
}


Eina_Bool
emc_app_av::play_set(Eina_Bool play)
{
   Eo* test;
   struct AV_Data tdata;
   memset(&tdata, 0, sizeof(struct AV_Data));

   if(play == EINA_TRUE)
     {
        elm_box bigbox( efl::eo::parent = win );
        bigbox.size_hint_weight_set(EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        win.resize_object_add(bigbox);
        bigbox.visibility_set(true);
        bigbox.horizontal_set(true);
        win.callback_del_add(clean_ref(bigbox));

        //XXX: Implement lambda+signals
        ::elm_video video ( efl::eo::parent = win );
        video.file_set(file_get());
        video.play_position_set(this->av_position);
        video.audio_level_set(this->av_volume);


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

        // Play
        elm_button play ( efl::eo::parent = win );
        play.text_set("elm.text", "Play");
        buttons.pack_end(play);
        play.visibility_set(true);

        // Embedded lambda function for Play button
        play.callback_clicked_add
            (std::bind([&tdata] (void)
                {
                    std::cout << "Video Play" << std::endl;
                    if(efl::eina::optional<elm_video> video = tdata.video.lock())
                        {
                            if(!video->is_playing_get())
                                video->play();
                        }
                }));
        win.callback_del_add(clean_ref(play));

        // Pause
        elm_button pause ( efl::eo::parent = win );
        pause.text_set("elm.text", "Pause");
        buttons.pack_end(pause);
        pause.visibility_set(true);

        // Embedded lambda function for Pause button
        pause.callback_clicked_add
            (std::bind([&tdata] (void)
                {
                    std::cout << "Video Pause" << std::endl;
                    if(efl::eina::optional<elm_video> video = tdata.video.lock())
                    {
                        if(video->is_playing_get())
                            video->pause();
                    }
                }));
        win.callback_del_add(clean_ref(pause));

        tdata.video = video;
        tdata.loop = this->av_loop;

#if 0 // Nothing happens :(
    video.callback_decode_stop_add
            (std::bind([&tdata] (void)
                {
                    std::cout << "Decode stop" << std::endl;
                }));
    video.callback_progress_change_add
            (std::bind([&tdata] (void)
                {
                    std::cout << "Progress change" << std::endl;
                }));

#else
        static struct AV_Data _stdata; //only for test
        _stdata.video = video;
        _stdata.loop = this->av_loop;
        Evas_Object *_tmp_obj;
        eo_do(video._eo_ptr(), _tmp_obj = ::elm_obj_video_emotion_get());
        evas_object_smart_callback_add(_tmp_obj, "decode_stop", video_obj_stopped_cb, &_stdata);
        evas_object_smart_callback_add(_tmp_obj, "progress_change", video_obj_progress_cb, &_stdata);
#endif
        video.play();

        std::cout << "File: " << file_get() << std::endl;
        std::cout << "Position: " << video.play_position_get() << std::endl;
        std::cout << "Volume: " << video.audio_level_get() << std::endl;

        win.size_set(300, 320);
        win.visibility_set(true);
        std::cout << "references to win " << win.ref_get() << std::endl;
        test = win._eo_ptr();
        win._release();

        return EINA_TRUE;
     }
   return EINA_FALSE;
}

