#include <stdio.h>

#include "emc_avplayer.hh"

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

     {
        elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
        ::elm_win win(elm_win_util_standard_add("emc-window","Main EMC Frame"));
        win.autodel_set(true);
        emc_avplayer player(win);

        filename = argv[1];
        player.file_set(filename);
        player.position_set(0.0);
        player.volume_set(10.0);
        player.loop_set(false);
        player.play_set(true);
        std::cout << "references to win " << win.ref_get() << std::endl;
        test = win._eo_ptr();
        win._release();
     }
   std::cout << "references to win " << ::eo_ref_get(test) << std::endl;

   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
