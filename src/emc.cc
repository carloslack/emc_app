#include <iostream>


extern int emc_avplayer(void);

int main(int argc, char **argv)
{
    std::cout << __FUNCTION__ << std::endl;
    emc_avplayer();
    return 0;
}
