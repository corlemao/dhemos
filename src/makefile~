CC=g++
CFLAGS=-c -Wall
EXECUTABLE=dhemos-base
DIR_SRC=./
DIR_MODELS=./models/
DIR_OBJ=./../bin/obj/
DIR_EXE=./../bin/

all: $(EXECUTABLE)

$(EXECUTABLE): simulator.o com.o agenthandler.o modelhandler.o model_specialcase.o model_lineartheory.o model_indoor.o model_outdoor.o  model_hannover.o systemfunctions.o
	mkoctfile --link-stand-alone -o $(DIR_EXE)$(EXECUTABLE) $(DIR_OBJ)simulator.o $(DIR_OBJ)com.o $(DIR_OBJ)agenthandler.o $(DIR_OBJ)modelhandler.o $(DIR_OBJ)model_specialcase.o $(DIR_OBJ)model_lineartheory.o $(DIR_OBJ)model_indoor.o $(DIR_OBJ)model_outdoor.o $(DIR_OBJ)model_hannover.o $(DIR_OBJ)systemfunctions.o

simulator.o: $(DIR_SRC)simulator.cc
	$(CC) $(CFLAGS) $(DIR_SRC)simulator.cc -o $(DIR_OBJ)simulator.o
	
com.o: $(DIR_SRC)com.cc
	$(CC) $(CFLAGS) $(DIR_SRC)com.cc -o $(DIR_OBJ)com.o

agenthandler.o: $(DIR_SRC)agenthandler.cc
	$(CC) $(CFLAGS) $(DIR_SRC)agenthandler.cc -o $(DIR_OBJ)agenthandler.o

modelhandler.o: $(DIR_SRC)modelhandler.cc
	$(CC) $(CFLAGS) $(DIR_SRC)modelhandler.cc -o $(DIR_OBJ)modelhandler.o

model_specialcase.o: $(DIR_MODELS)model_specialcase.cc
	$(CC) $(CFLAGS) $(DIR_MODELS)model_specialcase.cc -o $(DIR_OBJ)model_specialcase.o

model_lineartheory.o: $(DIR_MODELS)model_lineartheory.cc
	$(CC) $(CFLAGS) $(DIR_MODELS)model_lineartheory.cc -o $(DIR_OBJ)model_lineartheory.o

model_indoor.o: $(DIR_MODELS)model_indoor.cc
	$(CC) $(CFLAGS) $(DIR_MODELS)model_indoor.cc -o $(DIR_OBJ)model_indoor.o
	
model_outdoor.o: $(DIR_MODELS)model_outdoor.cc
	$(CC) $(CFLAGS) $(DIR_MODELS)model_outdoor.cc -o $(DIR_OBJ)model_outdoor.o

model_hannover.o: $(DIR_MODELS)model_hannover.cc
	$(CC) $(CFLAGS) $(DIR_MODELS)model_hannover.cc -o $(DIR_OBJ)model_hannover.o
	
systemfunctions.o: $(DIR_SRC)systemfunctions.cc
	$(CC) $(CFLAGS) $(DIR_SRC)systemfunctions.cc -o $(DIR_OBJ)systemfunctions.o

clean:
	rm -rf $(DIR_OBJ)*.o *~ $(DIR_EXE)$(EXECUTABLE)