#include <stdio.h>

#include "emc.hh"

extern int emc_avplayer(elm_box &box, const std::string &str);


EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Eo* test;
   std::string filename;

   if(argc < 2)
     {
        std::cout << "Use: ./" << argv[0]
            << " <filename>" << std::endl;
        return 1;
     }

   filename = argv[1];

   emc_app_av player("t","y");
   ::elm_win win = player.emc_app_win_get();

   player.file_set(filename);
   player.position_set(0.0);
   player.volume_set(0.50);


#if 0
   if(argc < 2)
     {
        std::cout << "Use: ./" << argv[0]
            << " <filename>" << std::endl;
        return 1;
     }
     {
        emc_app app("emc-window","Main EMC Frame");
        ::elm_win win = player.emc_app_win_get();

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
#endif
   return 0;
}
ELM_MAIN()
