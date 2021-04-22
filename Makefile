.PHONY=clean all
CC=gcc
CFLAGS = -g -Wall -pthread
JC=javac
CLASSPATH=-classpath src/
JFLAGS=

%.class : %.java
	$(JC) $(CLASSPATH) $*.java

%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $< 


CFILES=\
		src/message.c \
		src/manager.c \
		src/utils.c

OBJ = $(CFILES:%.c=%.o)

CLASSES=\
		src/Message.java \
		src/MulticastService.java \
		src/ClientService.java \
		src/JavaClient.java \
		src/Streamer.java \
		src/ManagerService.java

all:
	$(MAKE) classes
	$(MAKE) exec

classes: $(CLASSES:.java=.class)
	mv src/*.class src/target

exec: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ 
	mv $(OBJ) src/target
	mv exec src/target

clean:
	rm src/target/*.o src/target/*.class src/target/exec
