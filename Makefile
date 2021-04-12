.PHONY=clean all
CC=gcc
CFLAGS = -g -Wall
JC=javac
CLASSPATH=-classpath src/
JFLAGS=

%.class : %.java
	$(JC) $(CLASSPATH) $*.java

%.o : %.c
	$(CC) -o $@ -c $< $(CFLAGS)


CFILES=\
		src/message.c \
		src/manager.c \

OBJ = $(CFILES:%.c=%.o)

CLASSES=\
		src/Message.java \
		src/Streamer.java \ 
		src/MulticastService.java \
		src/ClientService.java \
		src/JavaClient.java \

all:
	$(MAKE) classes
	$(MAKE) exec

classes: $(CLASSES:.java=.class)
	mv src/*.class src/target

exec: $(OBJ)
	$(CC) -o $@ $^ $(FLAGS)
	mv $(OBJ) src/target
	mv exec src/target

clean:
	rm src/target/*.o src/target/*.class src/target/exec
