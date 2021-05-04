# C
CSRC_DIR = c/
COBJ_DIR = cobj/
CPROG_NAME = manager

CC	= gcc
CFLAGS	= -g -Wall -pthread

# JAVA
JSRC_DIR   = java/
JCLASS_DIR = jclass/
JPROG =

JC = javac
# Pour JCFLAGS ne pas mettre d'option -cp ou -d
JCFLAGS = -Xlint:all

# TAR
TAR_NAME = netradio.tar

#########################################
# NE PAS MODIFIER CE QUI EST EN DESSOUS #
#########################################

all: c java

#############################
# COMPILATION DES FICHIER C #
#############################
CPROG  = $(addprefix $(CSRC_DIR), $(CPROG_NAME))
CPROG := $(addsuffix .c, $(CPROG))

CSRC  = $(wildcard $(CSRC_DIR)*.c)
CSRC := $(filter-out $(CPROG), $(CSRC))

COBJ  = $(notdir $(CSRC:.c=.o))
COBJ := $(addprefix $(COBJ_DIR), $(COBJ))


c: $(CPROG_NAME)

$(CPROG_NAME): $(COBJ)
	$(CC) -o $@ $(CFLAGS) $^ $(CSRC_DIR)$@.c

$(COBJ_DIR):
	@mkdir -p $(COBJ_DIR)

$(COBJ_DIR)%.o: $(CSRC_DIR)%.c | $(COBJ_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)

#################################
# COMPILATION DES FICHIERS JAVA #
#################################
JSRC = $(wildcard $(JSRC_DIR)*.java)
JCLASS = $(notdir $(JSRC:.java=.class))
JCLASS := $(addprefix $(JCLASS_DIR), $(JCLASS))

java: $(JCLASS)

$(JCLASS_DIR):
	@mkdir -p $(JCLASS_DIR)

$(JCLASS_DIR)%.class: $(JSRC_DIR)%.java | $(JCLASS_DIR)
	$(JC) -d $(JCLASS_DIR) -cp $(JSRC_DIR) $(JCFLAGS) $<

#########################################
# NETTOYAGE DES DOSSIERS DE COMPILATION #
#########################################
cleanc:
	rm -rf $(COBJ_DIR) $(CPROG_NAME)

cleanj:
	rm -rf  $(JCLASS_DIR)

clean: cleanc cleanj

#############################
# CREATION DU TAR DU PROJET #
#############################
tar:
	tar -cvf $(TAR_NAME) Makefile $(JSRC_DIR) $(CSRC_DIR)
