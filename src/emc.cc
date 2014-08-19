#include <stdio.h>

#include "emc.hh"

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

   emc_app_av player("emc-window","Main EMC Frame");

#if 0
   // C++ lambda example (gonna use Felipe's anyways...)
   auto func = [] (int x) { return x+1; };
   int ret = func(10);
   std::cout << "ret:" << ret << std::endl;
#endif

   player.file_set(filename);
   player.position_set(8.0);
   player.volume_set(0.9);
   player.play_set(EINA_TRUE); /**< player releases the window */

   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
