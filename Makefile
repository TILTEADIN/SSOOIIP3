DIREXE := exec/
DIRHEA := include/
DIRSRC := src/
DIRRESULTS := results/

CFLAGS := -I$(DIRHEA)
LDLIBS := -pthread -std=c++11
CC := g++

all : dirs manager

dirs:
	mkdir -p $(DIREXE) $(DIRRESULTS)

manager: $(DIRSRC)Manager.cpp 
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS) $(CFLAGS)

run:
	./$(DIREXE)manager

clean : 
	rm -rf *~ core $(DIREXE) $(DIRRESULTS) $(DIRHEA)*~ $(DIRSRC)*~