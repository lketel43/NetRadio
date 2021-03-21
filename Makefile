.PHONY=clean all
CC=gcc
CFLAGS = -g -Wall
JC=javac
JFLAGS=

%.class : %.java
	$(JC) $*.java

%.o : %.c
	$(CC) -o $@ -c $< $(CFLAGS)


CFILES=\
		src/message.c \
		src/manager.c \
		src/main.c

OBJ = $(CFILES:%.c=%.o)

CLASSES=\
		src/Broadcaster.java \
		src/Message.java

classes: $(CLASSES:.java=.class)

exec: $(OBJ)
	$(CC) -o $@ $^ $(FLAGS)
	mv exec src/

all:
	$(MAKE) classes
	$(MAKE) exec


clean:
	rm src/*.o src/*.class src/exec