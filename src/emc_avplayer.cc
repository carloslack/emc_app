#include <iostream>
#include <stdio.h>

#include "emc.hh"


Eina_Bool
emc_app_av::file_set(const std::string &filename)
{
   if(!filename.size())
     return EINA_FALSE;

   this->av_filename = filename;
   video.file_set(this->av_filename);

   return EINA_TRUE;
}

Eina_Bool
emc_app_av::position_set(double position)
{
   video.play_position_set(position);
   return EINA_TRUE;
}

Eina_Bool
emc_app_av::volume_set(double volume)
{
   video.audio_level_set(volume);
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
   emc_app_av *t = static_cast<emc_app_av*>(data);
   std::cout  << "video stopped!" << std::endl;
   if(t && t->av_loop)
     {
        std::cout  << "[loop] replaying video!" << std::endl;
        t->video.play_position_set(0.0);
     }
}
static void
video_obj_progress_cb(void *data EINA_UNUSED, Evas_Object *obj, void *event_info EINA_UNUSED)
{
   //XXX: implement
   emc_app_av *t = static_cast<emc_app_av*>(data);
   (void)t;
}

Eina_Bool
emc_app_av::play_set(Eina_Bool to_play)
{
   Eo* test;
   if(to_play == EINA_TRUE)
     {
        // Embedded lambda function for Play button
        play.callback_clicked_add
            (std::bind([this] ()
                {
                    std::cout << "Video Play" << std::endl;
                            if(!video.is_playing_get())
                                video.play();
                }));

        // Embedded lambda function for Pause button
        pause.callback_clicked_add
            (std::bind([this] ()
                {
                    std::cout << "Video Pause IN" << std::endl;
                        std::cout << "Got instance" << std::endl;
                        if(video.is_playing_get())
                        {
                            std::cout << "pausing..." << std::endl;
                            video.pause();
                        }
                }));

#ifndef EMC_DISABLE_EO_CALLBACK // waiting for E support. See elm_video.eo events
    video.callback_decode_stop_add
            (std::bind([this] ()
                {
                    std::cout << "Decode stop" << std::endl;
                }));
    video.callback_progress_change_add
            (std::bind([this] ()
                {
                    std::cout << "Progress change" << std::endl;
                }));

#else // temporary impl
        Evas_Object *_tmp_obj;
        eo_do(video._eo_ptr(), _tmp_obj = ::elm_obj_video_emotion_get());
        evas_object_smart_callback_add(_tmp_obj, "decode_stop", video_obj_stopped_cb, this);
        evas_object_smart_callback_add(_tmp_obj, "progress_change", video_obj_progress_cb, this);
#endif
        video.play();

        std::cout << "File: " << file_get() << std::endl;
        std::cout << "Position: " << video.play_position_get() << std::endl;
        std::cout << "Volume: " << video.audio_level_get() << std::endl;

        win.size_set(300, 320);
        win.visibility_set(true);
        return EINA_TRUE;
     }
   return EINA_FALSE;
}

