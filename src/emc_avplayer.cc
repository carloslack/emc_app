/**
 * EMC - Enlightenment Media Center
 *    Audio/Video Player
 */
#include <iostream>
#include <sstream>

#include "emc_avplayer.hh"

#define EMC_ELM_PARENT_INIT(elm_obj,win) \
    elm_obj(efl::eo::parent = win)

// Class constructor
emc_avplayer::emc_avplayer(::elm_win &_win) :
          av_filename(""), av_loop(false),
          win(_win),
          EMC_ELM_PARENT_INIT(bigbox, win),
          EMC_ELM_PARENT_INIT(volbox, win),
          EMC_ELM_PARENT_INIT(video, win),
          EMC_ELM_PARENT_INIT(buttons, win),
          EMC_ELM_PARENT_INIT(volslider, win),
          EMC_ELM_PARENT_INIT(progslider, win),
          EMC_ELM_PARENT_INIT(volmute, win),
          EMC_ELM_PARENT_INIT(notify, win),
          EMC_ELM_PARENT_INIT(play, win),
          EMC_ELM_PARENT_INIT(pause, win),
          length_set(false)
{
   notify.align_set(0.5, 1.0);
   notify.size_hint_weight_set(EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   notify.timeout_set(3.0);
   notify.show();

   // Bigbox holding everything
   bigbox.size_hint_weight_set(EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   win.resize_object_add(bigbox);
   bigbox.visibility_set(true);
   bigbox.horizontal_set(false);
   win.callback_del_add(clean_ref(bigbox));

   // Set widget properties
   video.size_hint_weight_set(EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   win.resize_object_add(video);
   video.visibility_set(true);
   bigbox.pack_end(video);

   // Unref twice - added bigbox
   win.callback_del_add(clean_ref(video));
   win.callback_del_add(clean_ref(video));

   volbox.horizontal_set(false);
   volbox.visibility_set(true);
   win.callback_del_add(clean_ref(volbox));

   // VOlume slider
   volslider.min_max_set(0,100);
   volslider.horizontal_set(true);
   volslider.unit_format_set("%3.f%%");
   volslider.indicator_format_set("%3f");
   volslider.visibility_set(true);
   volslider.inverted_set(false);
   volslider.text_set("elm.text", "volume ");
   volslider.size_hint_weight_set(EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   volbox.pack_end(volslider);
   win.callback_del_add(clean_ref(volslider));

   Eina_Bool cstate = EINA_FALSE;
   volmute.state_pointer_set(&cstate);
   volmute.visibility_set(true);
   volbox.pack_end(volmute);
   volmute.text_set("elm.text", "mute");
   win.callback_del_add(clean_ref(volmute));

   // Buttons
   buttons.horizontal_set(EINA_TRUE);
   bigbox.pack_end(buttons);
   buttons.visibility_set(true);

   // Unref twice - added to bigbox
   win.callback_del_add(clean_ref(buttons));
   win.callback_del_add(clean_ref(buttons));

   //progslider.min_max_set(0,video.play_length_get());
   progslider.visibility_set(true);
   progslider.unit_format_set("%3.2f");
   progslider.indicator_format_set("%2f");
   progslider.size_hint_align_set(EVAS_HINT_FILL, 0);
   bigbox.pack_before(progslider,buttons);
   win.callback_del_add(clean_ref(progslider));

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
   emc_avplayer *t = static_cast<emc_avplayer*>(data);
   if(!t->length_set)
     {
        t->length_set = true;
        t->progslider.min_max_set(0, t->video.play_length_get());
    }
  t->progslider.value_set(t->video.play_position_get());
#if 0
   double pos, len, scale;
   //char buf[256];
   int ph, pm, ps, pf, lh, lm, ls;
   std::ostringstream out;

   pos = t->video.play_position_get();
   len = t->video.play_length_get();
   scale = (len > 0.0) ? pos / len : 0.0;

   //std::cout << len/3600 << std::endl;
    //edje_object_part_drag_value_set(edje, "video_progress", scale, 0.0);

   lh = len / 3600;
   lm = len / 60 - (lh * 60);
   ls = len - (lh * 3600) - (lm * 60);

   ph = pos / 3600;
   pm = pos / 60 - (ph * 60);
   ps = pos - (ph * 3600) - (pm * 60);

   out << ph << ":" << pm << ":" << ps;
   pf = pos * 100 - (ps * 100) - (pm * 60 * 100) - (ph * 60 * 60 * 100);
#endif
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
   video.audio_level_set(volume/10);
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
                    if(!video.is_playing_get()) video.play();
                }));

        // Embedded lambda function for Pause button
        pause.callback_clicked_add
            (std::bind([this] ()
                {
                    if(video.is_playing_get()) video.pause();
                }));

    volslider.callback_changed_add
            (std::bind([this] ()
                {
                    video.audio_level_set(volslider.value_get()/10);
                }));

    progslider.callback_changed_add
            (std::bind([this] ()
                {
                    // Update video position. Progress slider is updated as side effect.
                    video.play_position_set(progslider.value_get());
                }));

    volmute.callback_changed_add
            (std::bind([this] ()
                {
                    if(volmute.state_get() == true)
                    {
                        if(video.audio_mute_get() == false)
                            video.audio_mute_set(true);
                    } else
                    {
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
        evas_object_smart_callback_add(_tmp_video_obj, "frame_decode", video_obj_progress_cb, this);
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

