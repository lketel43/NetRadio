# C
CSRC_DIR = c/
COBJ_DIR = cobj/
CMAIN_NAME = manager

CC	= gcc
CFLAGS	= -g -Wall -pthread

# JAVA
JSRC_DIR   = java/
JCLASS_DIR = jclass/
JMAIN_NAME = JavaClient Streamer

JC = javac
# Pour JCFLAGS ne pas mettre d'option -cp ou -d
JCFLAGS = -Xlint:all

# TAR
TAR_NAME = netradio.tar

#########################################
# NE PAS MODIFIER CE QUI EST EN DESSOUS #
#########################################
# Compilation des fichiers C et Java
all: c java scenario

# Nettoyage des fichiers de compilation C et Java et Scenarios
clean: cleanc cleanj cleans

# Création du .tar contenant les fichiers .java, .c et Makefile
tar:
	tar -cvf $(TAR_NAME) Makefile $(JSRC_DIR) $(CSRC_DIR)


#############################
# COMPILATION DES FICHIER C #
#############################
CMAIN  = $(addprefix $(CSRC_DIR), $(CMAIN_NAME))
CMAIN := $(addsuffix .c, $(CMAIN))

CSRC  = $(wildcard $(CSRC_DIR)*.c)
CSRC := $(filter-out $(CMAIN), $(CSRC))

COBJ  = $(notdir $(CSRC:.c=.o))
COBJ := $(addprefix $(COBJ_DIR), $(COBJ))


c: $(CMAIN_NAME)

$(CMAIN_NAME): $(COBJ)
	$(CC) -o $@ $(CFLAGS) $^ $(CSRC_DIR)$@.c

$(COBJ_DIR):
	@mkdir -p $(COBJ_DIR)

$(COBJ_DIR)%.o: $(CSRC_DIR)%.c | $(COBJ_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)

# Nettoyage des fichiers de compilation C
cleanc:
	rm -rf $(COBJ_DIR) $(CMAIN_NAME)


#################################
# COMPILATION DES FICHIERS JAVA #
#################################
JMAIN = $(addsuffix .jar, $(JMAIN_NAME))

JSRC = $(wildcard $(JSRC_DIR)*.java)

JCLASS  = $(notdir $(JSRC:.java=.class))
JCLASS := $(addprefix $(JCLASS_DIR), $(JCLASS))


java: $(JMAIN)

$(JMAIN): $(JMAIN_NAME)

$(JMAIN_NAME): $(JCLASS)
	jar cfe $@.jar $@ -C $(JCLASS_DIR) . 

$(JCLASS_DIR):
	@mkdir -p $(JCLASS_DIR)

$(JCLASS_DIR)%.class: $(JSRC_DIR)%.java | $(JCLASS_DIR)
	$(JC) -d $(JCLASS_DIR) -cp $(JSRC_DIR) $(JCFLAGS) $<

# Nettoyage des fichiers de compilation Java
cleanj:
	rm -rf  $(JCLASS_DIR) $(JMAIN)


#############################
# COMPILATION DES SCENARIOS #
#############################
SCENARIO = $(wildcard scenario_*)

SCENARIO_EXEC = $(join $(SCENARIO)/, $(SCENARIO))

SCENARIO_CLASS = $(notdir $(SCENARIO:.java=.class))


scenario: $(SCENARIO_EXEC) $(SCENARIO_CLASS)

$(SCENARIO)/$(SCENARIO): $(wildcard $(SCENARIO)/*.c) $(COBJ)
	$(CC) $(CFLAGS) -I $(CSRC_DIR) -o $@ $^

$(SCENARIO)/%.class: $(SCENARIO)/%.java $(JCLASS)
	$(JC) -d $(SCENARIO) $(JCFLAGS) -cp $(JCLASS_DIR);$(SCENARIO)/

cleans:
	rm -rf $(SCENARIO_EXEC)
