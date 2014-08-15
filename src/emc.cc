#include <stdio.h>

#include "emc.hh"

extern int emc_avplayer(elm_box &box, const std::string &str);

struct emc_app {
     ::elm_win frame_get(const char *name, const char *title)
       {
          elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

          ::elm_win win (elm_win_util_standard_add(name, title));
          win.autodel_set(true);

          return win;
       }
};

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Eo* test;

   if(argc < 2)
     {
        std::cout << "Use: ./" << argv[0]
            << " <filename>" << std::endl;
        return 1;
     }
     {
        emc_app app;
        ::elm_win win = app.frame_get("emc-window","Main EMC Frame");

        elm_box bigbox ( efl::eo::parent = win );
        bigbox.size_hint_weight_set(EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        win.resize_object_add(bigbox);
        bigbox.visibility_set(true);
        win.callback_del_add(clean_ref(bigbox));

        win.size_set(300, 320);
        win.visibility_set(true);

        emc_avplayer(bigbox, argv[1]);

        std::cout << "references to win " << win.ref_get() << std::endl;
        test = win._eo_ptr();
        win._release();
     }

   std::cout << "references to win " << eo_ref_get(test) << std::endl;

   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
