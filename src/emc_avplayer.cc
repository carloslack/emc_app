/**
 * EMC - Enlightenment Media Center
 *    Audio/Video Player
 */
#include <iostream>
#include <stdio.h>

#include "emc_avplayer.hh"

#define EMC_ELM_PARENT_INIT(elm_obj,win) \
    elm_obj(efl::eo::parent = win)

// Class constructor
emc_avplayer::emc_avplayer(::elm_win &_win) :
          av_filename(""), av_loop(false),
          win(_win),
          EMC_ELM_PARENT_INIT(video, win),
          EMC_ELM_PARENT_INIT(bigbox, win),
          EMC_ELM_PARENT_INIT(buttons, win),
          EMC_ELM_PARENT_INIT(volbox, win),
          EMC_ELM_PARENT_INIT(volslider, win),
          EMC_ELM_PARENT_INIT(volcheck, win),
          EMC_ELM_PARENT_INIT(notify, win),
          EMC_ELM_PARENT_INIT(play, win),
          EMC_ELM_PARENT_INIT(pause, win)
   {
      // Set widget properties
      video.size_hint_weight_set(EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
      win.resize_object_add(video);
      video.visibility_set(true);
      win.callback_del_add(clean_ref(video));

      bigbox.size_hint_weight_set(EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
      win.resize_object_add(bigbox);
      bigbox.visibility_set(true);
      bigbox.horizontal_set(true);
      win.callback_del_add(clean_ref(bigbox));

      volbox.horizontal_set(false);
      volbox.visibility_set(true);
      win.callback_del_add(clean_ref(volbox));

      // VOlume slider
      volslider.min_max_set(0,10);
      volslider.horizontal_set(false);
      volslider.unit_format_set("%1.1f");
      volslider.indicator_format_set("%1.1f");
      volslider.visibility_set(true);
      volslider.inverted_set(true);
      volslider.size_hint_weight_set(EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
      volbox.pack_end(volslider);
      win.callback_del_add(clean_ref(volslider));

      Eina_Bool cstate = EINA_FALSE;
      volcheck.text_set("elm.text", "mute");
      volcheck.state_pointer_set(&cstate);
      volcheck.show();
      volcheck.visibility_set(true);
      volbox.pack_end(volcheck);
      win.callback_del_add(clean_ref(volcheck));


      // Buttons
      buttons.horizontal_set(EINA_TRUE);
      bigbox.pack_end(buttons);
      buttons.visibility_set(true);
      win.callback_del_add(clean_ref(buttons));

      // Play
      play.text_set("elm.text", "Play");
      buttons.pack_end(play);
      play.visibility_set(true);
      win.callback_del_add(clean_ref(play));

      // Pause
      pause.text_set("elm.text", "Pause");
      buttons.pack_end(pause);
      pause.visibility_set(true);
      win.callback_del_add(clean_ref(pause));

      bigbox.pack_end(volbox);
   }

// Callbacks
static void
video_obj_stopped_cb(void *data EINA_UNUSED, Evas_Object *obj, void *event_info EINA_UNUSED)
{
   emc_avplayer *t = static_cast<emc_avplayer*>(data);
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
   emc_avplayer *t = static_cast<emc_avplayer*>(data);
   (void)t;
}

// EMC
Eina_Bool
emc_avplayer::file_set(const std::string &filename)
{
   if(!filename.size())
     return EINA_FALSE;

   this->av_filename = filename;
   video.file_set(this->av_filename);

   return EINA_TRUE;
}

Eina_Bool
emc_avplayer::position_set(double position)
{
   video.play_position_set(position);
   return EINA_TRUE;
}

Eina_Bool
emc_avplayer::volume_set(double volume)
{
   video.audio_level_set(volume);
   //Set slider position to match volume level
   volslider.value_set(video.audio_level_get());
   return EINA_TRUE;
}

Eina_Bool
emc_avplayer::loop_set(bool loop)
{
   this->av_loop = loop;
   return EINA_TRUE;
}

Eina_Bool
emc_avplayer::play_set(Eina_Bool to_play)
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

    volslider.callback_changed_add
            (std::bind([this] ()
                {
                    std::cout << "New volume: " << volslider.value_get() << std::endl;
                    video.audio_level_set(volslider.value_get());
                }));

    volcheck.callback_changed_add
            (std::bind([this] ()
                {
                    if(volcheck.state_get() == true)
                    {
                        std::cout << "Checkbox: checked" << std::endl;
                        if(video.audio_mute_get() == false)
                            video.audio_mute_set(true);
                    } else
                    {
                        std::cout << "Checkbox: unchecked" << std::endl;
                        if(video.audio_mute_get() == true)
                            video.audio_mute_set(false);
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
        Evas_Object *_tmp_video_obj;
        eo_do(video. _eo_ptr(), _tmp_video_obj = ::elm_obj_video_emotion_get());
        evas_object_smart_callback_add(_tmp_video_obj, "decode_stop", video_obj_stopped_cb, this);
        evas_object_smart_callback_add(_tmp_video_obj, "progress_change", video_obj_progress_cb, this);
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

