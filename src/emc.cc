#include <iostream>

#include <Eina.h>
#include <Ecore.h>
extern int emc_avplayer(void);

int main(int argc, char **argv)
{
   ecore_init();

   std::cout << __FUNCTION__ << std::endl;
   emc_avplayer();

   ecore_shutdown();
   return 0;
}
