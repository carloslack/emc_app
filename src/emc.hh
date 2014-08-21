#ifndef _EMC_HH
#define _EMC_HH

#ifdef HAVE_CONFIG_H
# include <elementary_config.h>
#endif

#include <iostream>

#include <Eo.h>
#include <Evas.h>
#include <Elementary.h>
#include <elm_widget.h>

#include "elm_interface_atspi_accessible.h"
#include "elm_interface_atspi_accessible.eo.h"

#include "elm_interface_atspi_widget_action.h"
#include "elm_interface_atspi_widget_action.eo.h"
#include <elm_win.eo.hh>
#include <elm_box.eo.hh>
#include <elm_button.eo.hh>
#include <elm_video.eo.hh>
#include <elm_hover.eo.hh>
#include <elm_button.eo.hh>
//#include <elm_actionslider.eo.hh>
#include <elm_notify.eo.hh>

#include <Eina.hh>

#define EMC_ELM_PARENT_INIT(elm_obj,win) \
    elm_obj(efl::eo::parent = win)

struct clean_ref
{
  clean_ref(efl::eo::base base)
    : _ref(base._eo_ptr())
  {}

  template <typename T>
  void operator()(T const&, Eo_Event_Description const&, void*) const
  {
    if(_ref)
      eo_unref(_ref);
  }
  Eo* _ref;
};

class emc_app_av
{
  private:
      std::string av_filename;
      //bool av_loop;
      const std::string file_get(void) { return this->av_filename; }
      ::elm_win win;
      //::elm_video video;
      ::elm_box bigbox;
      ::elm_box buttons;
      ::elm_notify notify;
      ::elm_button play;
      ::elm_button pause;
  public:
      bool av_loop; //XXX: use private
      ::elm_video video; //XXX: use private

      emc_app_av(::elm_win &_win) :
          av_filename(""), av_loop(false),
          win(_win),
          EMC_ELM_PARENT_INIT(video, win),
          EMC_ELM_PARENT_INIT(bigbox, win),
          EMC_ELM_PARENT_INIT(buttons, win),
          EMC_ELM_PARENT_INIT(notify, win),
          EMC_ELM_PARENT_INIT(play, win),
          EMC_ELM_PARENT_INIT(pause, win)
   {
      video.size_hint_weight_set(EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
      win.resize_object_add(video);
      video.visibility_set(true);
      win.callback_del_add(clean_ref(video));

      bigbox.size_hint_weight_set(EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
      win.resize_object_add(bigbox);
      bigbox.visibility_set(true);
      bigbox.horizontal_set(true);
      win.callback_del_add(clean_ref(bigbox));

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
   }

      ~emc_app_av() {}

      Eina_Bool file_set(const std::string &filename);
      Eina_Bool position_set(double position);
      Eina_Bool volume_set(double volume);
      Eina_Bool play_set(Eina_Bool value);
      Eina_Bool loop_set(bool loop);
};


#endif
