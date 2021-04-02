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