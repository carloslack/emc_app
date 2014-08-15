#ifndef _EMC_HH
#define _EMC_HH

#ifdef HAVE_CONFIG_H
# include <elementary_config.h>
#endif

#define ELM_INTERNAL_API_ARGESFSDFEFC
#define ELM_INTERFACE_ATSPI_ACCESSIBLE_PROTECTED
#define ELM_INTERFACE_ATSPI_COMPONENT_PROTECTED
#define ELM_INTERFACE_ATSPI_ACTION_PROTECTED
#define ELM_INTERFACE_ATSPI_VALUE_PROTECTED
#define ELM_INTERFACE_ATSPI_EDITABLE_TEXT_PROTECTED
#define ELM_INTERFACE_ATSPI_TEXT_PROTECTED
#define ELM_INTERFACE_ATSPI_SELECTION_PROTECTED
#define ELM_INTERFACE_ATSPI_IMAGE_PROTECTED
#define ELM_INTERFACE_ATSPI_WIDGET_ACTION_PROTECTED


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

#include <Eina.hh>

#include <deque>

#define __GO(x, y, z) ::elm_win x(elm_win_util_standard_add(#y, #z))

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
    public:
        emc_app(const char *n, const char *t) : name(n), title(t) {}

        ::elm_win emc_app_win_get(void)
          {
             elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

             Evas_Object *evas = elm_win_util_standard_add(name, title);
             ::elm_win win(evas);
             win.autodel_set(true);
             return win;
          }
};

class emc_app_av : public virtual emc_app
{
    private:
        std::string av_filename;
        double av_position;
        double av_volume;
  public:
      emc_app_av(const char *n, const char *t) : emc_app(n,t) {}

      void file_set(std::string &filename);
      void position_set(const double &position);
      void volume_set(const double &volume);
};


#endif
