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
		src/utils.c \
		src/client.c

CCLIENT=\
		src/message.c \
		src/utils.c \
		src/client.c

CMANAGER=\
		src/message.c \
		src/manager.c \
		src/utils.c

OBJMANAGER = $(CMANAGER:%.c=%.o)

OBJCLIENT = $(CCLIENT:%.c=%.o)

CLASSES=\
		src/Message.java \
		src/MulticastService.java \
		src/ClientService.java \
		src/JavaClient.java \
		src/Streamer.java \
		src/ManagerService.java \
		src/StreamFile.java

all:
	$(MAKE) classes
	$(MAKE) manager
	$(MAKE) clientc

classes: $(CLASSES:.java=.class)
	mv src/*.class src/target

manager: $(OBJMANAGER)
	$(CC) $(CFLAGS) -o $@ $^ 
	mv $(OBJMANAGER) src/target
	mv manager src/target

clientc: $(OBJCLIENT)
	$(CC) $(CFLAGS) -o $@ $^
	mv $(OBJCLIENT) src/target
	mv clientc src/target

clean:
	rm src/target/*.o src/target/*.class src/target/manager src/target/clientc
