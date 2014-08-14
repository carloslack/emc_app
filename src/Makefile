ACLOCAL_AMFLAGS = -I m4
PROJ=emc
SRCS=emc.cc emc_avplayer.cc


CC=g++
all: 
	$(CC) $(SRCS) -o $(PROJ)
	@echo ${EMC_LIBS}

clean:
	rm -f emc *.o
