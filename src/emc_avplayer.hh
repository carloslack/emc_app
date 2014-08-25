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
#include <elm_button.eo.hh>
#include <elm_notify.eo.hh>
#include <elm_slider.eo.hh>
#include <elm_check.eo.hh>
#include <elm_label.eo.hh>

#include <Eina.hh>

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

// Important: several variables and methods below are commented
// because we still miss a couple of audio events in EFL, forcing us
// to use old-style callbacks, and such callbacks are declared as static functions
// and need to have access to data that must not be private.
// Once video functionalities are made available we'll move all of them to private
// area.
class emc_avplayer
{
      std::string av_filename;
      //bool av_loop;
      ::elm_win win;
      ::elm_box bigbox; //1
      ::elm_box ctrlbox; //3.1
      //::elm_video video;
      ::elm_box buttons; //3.1
      ::elm_slider volslider;
      //::elm_slider progslider;
      ::elm_check volmute;
      ::elm_notify notify;
      ::elm_button play;
      ::elm_button pause;
      ::elm_button stop;
      //::elm_label elapse;
      //bool length_set;
      //int lh, lm, ls;
      const std::string file_get(void) {
           return this->av_filename;
      }

      //const void total_time_set(const double &len)
      //  {
      //     emc_total_time.lh = len / 3600;
      //     emc_total_time.lm = len / 60 - (emc_total_time.lh * 60);
      //     emc_total_time.ls = len - (emc_total_time.lh * 3600) - (emc_total_time.lm * 60);
      //  }

  public:
      bool av_loop; //XXX: use private
      bool length_set; //XXX: use private
      ::elm_video video; //XXX: use private
      ::elm_slider progslider;//XXX: use private
      ::elm_label elapse; //XXX: use private
      int lh, lm, ls;//XXX: use private
      emc_avplayer(::elm_win &_win);
      ~emc_avplayer() {}

      const void total_time_set(const double &len)//XXX: use private
        {
           lh = len / 3600; // hours
           lm = len / 60 - (lh * 60); // minutes
           ls = len - (lh * 3600) - (lm * 60); // seconds
        }

      Eina_Bool file_set(const std::string &filename);
      Eina_Bool position_set(double position);
      Eina_Bool volume_set(double volume);
      Eina_Bool play_set(Eina_Bool value);
      Eina_Bool loop_set(bool loop);
};


#endif
