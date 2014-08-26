/**
 * EMC - Enlightenment Media Center
 *    Audio/Video Player
 */
#include "emc_avplayer.hh"

// Class constructor
emc_avplayer::emc_avplayer(::elm_win &_win) :
          av_filename(""),
          av_loop(false), length_set(false),
          win(_win),
          bigbox(efl::eo::parent = win),
          ctrlbox(efl::eo::parent = win),
          video(efl::eo::parent = win),
          buttons(efl::eo::parent = win),
          volslider(efl::eo::parent = win),
          progslider(efl::eo::parent = win),
          volmute(efl::eo::parent = win),
          notify(efl::eo::parent = win),
          play(efl::eo::parent = win),
          pause(efl::eo::parent = win),
          stop(efl::eo::parent = win),
          elapse(efl::eo::parent = win)
{
   Eina_Bool mutestate = EINA_FALSE;

   // Notify - hide/unhide controllers
   notify.align_set(0.5, 1.0);
   notify.size_hint_weight_set(EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   notify.timeout_set(5.0);
   notify.visibility_set(true);
   notify.allow_events_set(true);

   // Bigbox holding everything
   bigbox.size_hint_weight_set(EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   win.resize_object_add(bigbox);
   bigbox.visibility_set(true);

   // Set widget properties
   video.size_hint_weight_set(EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   win.resize_object_add(video);
   video.visibility_set(true);
   bigbox.pack_end(video);

   // Controllers box
   ctrlbox.horizontal_set(true);
   ctrlbox.visibility_set(true);
   bigbox.pack_end(ctrlbox);

   // VOlume slider
   volslider.min_max_set(0,100);
   volslider.horizontal_set(true);
   volslider.unit_format_set("%3.f%%");
   volslider.indicator_format_set("%3f");
   volslider.visibility_set(true);
   volslider.inverted_set(false);
   volslider.text_set("elm.text", "volume");
   ctrlbox.pack_end(volslider);

   // Mute checkbox
   volmute.state_pointer_set(&mutestate);
   volmute.visibility_set(true);
   volmute.text_set("elm.text", "mute");
   ctrlbox.pack_end(volmute);

   // Total length is only known in run time.
   // Here we setup display options
   progslider.visibility_set(true);
   ctrlbox.pack_end(progslider);

   // Elapse time label
   elapse.visibility_set(true);
   ctrlbox.pack_end(elapse);

   // Buttons
   buttons.horizontal_set(EINA_TRUE);
   ctrlbox.pack_end(buttons);
   buttons.visibility_set(true);

   // Play buttom
   play.text_set("elm.text", "Play");
   play.visibility_set(true);
   buttons.pack_end(play);

   // Pause buttom
   pause.text_set("elm.text", "Pause");
   pause.visibility_set(true);
   buttons.pack_end(pause);

   // Stop buttom
   stop.text_set("elm.text", "Stop");
   stop.visibility_set(true);
   buttons.pack_end(stop);

   // Ref cleanup
   win.callback_del_add(clean_ref(bigbox));

   win.callback_del_add(clean_ref(ctrlbox));

   // Unref twice - added bigbox
   win.callback_del_add(clean_ref(video));
   win.callback_del_add(clean_ref(video));

   // Unref remaining objects
   win.callback_del_add(clean_ref(volslider));
   win.callback_del_add(clean_ref(buttons));
   win.callback_del_add(clean_ref(volmute));
   win.callback_del_add(clean_ref(progslider));
   win.callback_del_add(clean_ref(elapse));
   win.callback_del_add(clean_ref(play));
   win.callback_del_add(clean_ref(pause));
   win.callback_del_add(clean_ref(stop));
}

// Callbacks
static void
video_obj_stopped_cb(void *data EINA_UNUSED, Evas_Object *obj, void *event_info EINA_UNUSED)
{
   emc_avplayer *t = static_cast<emc_avplayer*>(data);
   if(!t)
     {
        std::string errmsg("Invalid Data\n");
        std::cerr << "Error: " << errmsg;
        return;
     }
   if(t->av_loop) t->video.play_position_set(0.0);
}
static void
video_obj_progress_cb(void *data EINA_UNUSED, Evas_Object *obj, void *event_info EINA_UNUSED)
{
   emc_avplayer *t = static_cast<emc_avplayer*>(data);
   std::ostringstream label_text;
   int ph, pm, ps, lh, lm, ls;
   double pos = t->video.play_position_get();
   double len = t->video.play_length_get();

   if(!t)
     {
        std::string errmsg("Invalid Data\n");
        std::cerr << "Error: " << errmsg;
        return;
     }

   if(!t->length_set)
     {
        t->length_set = true;
        t->progslider.min_max_set(0, t->video.play_length_get());
        t->total_time_set(len);
     }

   // Update slider
   t->progslider.value_set(pos);

   ph = pos / 3600;
   pm = pos / 60 - (ph * 60);
   ps = pos - (ph * 3600) - (pm * 60);

   label_text << ph << ":" << pm << ":" << ps << "/" <<
       t->lh << ":" << t->lm << ":" << t->ls;

   // Update elapse time
   t->elapse.text_set("elm.text", label_text.str());
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
            (std::bind([this]
                {
                    if(!video.is_playing_get()) video.play();
                }
            ));

        // Embedded lambda function for Pause button
        pause.callback_clicked_add
            (std::bind([this]
                {
                    if(video.is_playing_get()) video.pause();
                }
            ));

        // Embedded lambda function for Pause button
        stop.callback_clicked_add
            (std::bind([this]
                {
                    video.play_position_set(0.0);
                    video.stop();
                }
            ));

        // Embedded lambda function for Volume Slider
        volslider.callback_changed_add
            (std::bind([this]
                {
                    video.audio_level_set(volslider.value_get()/10);
                }
            ));

        // Embedded lambda function for Progress Slider
        progslider.callback_changed_add
            (std::bind([this]
                {
                    // Update video position. Progress slider is updated as side effect.
                    video.play_position_set(progslider.value_get());
                }
            ));

        // Embedded lambda function for Mute Checkbox
        volmute.callback_changed_add
            (std::bind([this]
                {
                    if(volmute.state_get() == true)
                    {
                        if(video.audio_mute_get() == false)
                            video.audio_mute_set(true);
                    } else {
                        if(video.audio_mute_get() == true)
                            video.audio_mute_set(false);
                    }
                }
            ));

        // When the mouse out, set timeout
        bigbox.callback_mouse_out_add
            (std::bind([this]
                {
                    notify.timeout_set(5.0);
                }
            ));

        // When the mouse out, set timeout
        bigbox.callback_mouse_move_add
            (std::bind([this]
                {
                    if(ctrlbox.visibility_get() == false)
                    {
                        ctrlbox.visibility_set(true);
                    }
                    notify.timeout_set(5.0);
                    notify.visibility_set(true);
                }
            ));

        // Mouse wheel: same as mouse move
        bigbox.callback_mouse_wheel_add
            (std::bind([this]
                {
                    if(ctrlbox.visibility_get() == false)
                    {
                        ctrlbox.visibility_set(true);
                    }
                    notify.timeout_set(5.0);
                    notify.visibility_set(true);
                }
            ));

        // When timeout expires hide Controllers box
        notify.callback_timeout_add
            (std::bind([this]
                {
                    ctrlbox.visibility_set(false);
                }
            ));

#ifndef EMC_DISABLE_EO_CALLBACK // waiting for E support. See elm_video.eo events
        video.callback_decode_stop_add
            (std::bind([this]
                {
                    std::cout << "Decode stop" << std::endl;
                }
            ));
        video.callback_progress_change_add
            (std::bind([this]
                {
                    std::cout << "Progress change" << std::endl;
                }
            ));

#else // temporary old-style impl
        Evas_Object *_tmp_video_obj;
        eo_do(video. _eo_ptr(), _tmp_video_obj = ::elm_obj_video_emotion_get());
        evas_object_smart_callback_add(_tmp_video_obj, "decode_stop", video_obj_stopped_cb, this);
        evas_object_smart_callback_add(_tmp_video_obj, "frame_decode", video_obj_progress_cb, this);
#endif
        video.play(); // do it!

        std::cout << "File: " << file_get() << std::endl;
        std::cout << "Position: " << video.play_position_get() << std::endl;
        std::cout << "Volume: " << video.audio_level_get() << std::endl;

        win.size_set(300, 320);
        win.visibility_set(true);
        return EINA_TRUE;
     }
   return EINA_FALSE;
}

