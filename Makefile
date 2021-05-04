# C
CSRC_DIR = c/
COBJ_DIR = cobj/

CC	= gcc
CFLAGS	= -g -Wall -pthread

# JAVA
JSRC_DIR   = java/
JCLASS_DIR = jclass/

JC = javac
# Pour JCFLAGS ne pas mettre d'option -cp ou -d
JCFLAGS = -Xlint:all


#########################################
# NE PAS MODIFIER CE QUI EST EN DESSOUS #
#########################################

CSRC  = $(wildcard $(CSRC_DIR)*.c)
COBJ = $(notdir $(CSRC:.c=.o))
COBJ := $(addprefix $(COBJ_DIR), $(COBJ))


JSRC = $(wildcard $(JSRC_DIR)*.java)
JCLASS = $(notdir $(JSRC:.java=.class))
JCLASS := $(addprefix $(JCLASS_DIR), $(JCLASS))


all: c java

cleanc:
	rm -rf $(COBJ_DIR)

cleanj:
	rm -rf  $(JCLASS_DIR)

clean: cleanc cleanj

#############################
# COMPILATION DES FICHIER C #
#############################

c: $(COBJ)

$(COBJ_DIR):
	@mkdir -p $(COBJ_DIR)

$(COBJ_DIR)%.o: $(CSRC_DIR)%.c | $(COBJ_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)


#################################
# COMPILATION DES FICHIERS JAVA #
#################################

java: $(JCLASS)

$(JCLASS_DIR):
	@mkdir -p $(JCLASS_DIR)

$(JCLASS_DIR)%.class: $(JSRC_DIR)%.java | $(JCLASS_DIR)
	$(JC) -d $(JCLASS_DIR) -cp $(JSRC_DIR) $(JCFLAGS) $<
