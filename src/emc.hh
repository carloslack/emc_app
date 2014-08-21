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
#include <elm_actionslider.eo.hh>

#include <Eina.hh>

#include <deque>

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
      ::elm_win win;
      const std::string file_get(void) { return this->av_filename; }
      //::elm_video video;
      Eina_Bool widget_setup(void);
  public:
      // XXX: temporary public data
      bool av_loop;
      ::elm_video video;

      emc_app_av(::elm_win &_win) :
          av_filename(""), av_loop(false),
          win(_win), video(efl::eo::parent = _win)
   {
      video.size_hint_weight_set(EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
      win.resize_object_add(video);
      video.visibility_set(true);
      win.callback_del_add(clean_ref(video));
   }
      ~emc_app_av() {}

      Eina_Bool file_set(const std::string &filename);
      Eina_Bool position_set(double position);
      Eina_Bool volume_set(double volume);
      Eina_Bool play_set(Eina_Bool value);
      Eina_Bool loop_set(bool loop);
};


#endif
