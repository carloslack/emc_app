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

class emc_app {
    private:
        const char *name;
        const char *title;
        Evas_Object *evas_win_obj;
    public:
        emc_app(const char *n, const char *t) : name(n), title(t),
         evas_win_obj(elm_win_util_standard_add(name, title))
        {
             elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
        }
        ~emc_app() {}

        Evas_Object *get_evas(void) { return this->evas_win_obj;}
        ::elm_win emc_app_win_get(void)
          {
             ::elm_win win(evas_win_obj);
             win.autodel_set(true);
             return win;
          }
};

class emc_app_av : public emc_app
{
  private:
      std::string av_filename;
      double av_position;
      double av_volume;
      bool av_loop;
      ::elm_win win;
      const std::string file_get(void) { return this->av_filename; }
  public:
      emc_app_av(const char *n, const char *t) :
          emc_app(n,t), av_filename(""), av_position(0.0),
          av_volume(0.0), av_loop(false), win(emc_app_win_get()) { }
      ~emc_app_av() {}

      Eina_Bool file_set(const std::string &filename);
      Eina_Bool position_set(double position);
      Eina_Bool volume_set(double volume);
      Eina_Bool play_set(Eina_Bool value);
      Eina_Bool loop_set(bool loop);
};


#endif
