# C
CSRC_DIR = c/
COBJ_DIR = cobj/
CMAIN_NAME = manager client

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
CMAIN  = $(patsubst %,$(CSRC_DIR)%.c,$(CMAIN_NAME))

CSRC  = $(wildcard $(CSRC_DIR)*.c)
CSRC := $(filter-out $(CMAIN),$(CSRC))

COBJ  = $(notdir $(CSRC:.c=.o))
COBJ := $(addprefix $(COBJ_DIR),$(COBJ))


c: $(CMAIN_NAME)

$(CMAIN_NAME): %: $(COBJ) $(CSRC_DIR)%.c
	$(CC) -o $@ $(CFLAGS) $^

$(COBJ_DIR):
	@mkdir -p $(COBJ_DIR)

$(COBJ_DIR)%.o: $(CSRC_DIR)%.c $(CSRC_DIR)%.h | $(COBJ_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)

# Nettoyage des fichiers de compilation C
cleanc:
	rm -rf $(COBJ_DIR) $(CMAIN_NAME)


#################################
# COMPILATION DES FICHIERS JAVA #
#################################
JAR = $(addsuffix .jar,$(JMAIN_NAME))
JSRC = $(wildcard $(JSRC_DIR)*.java)

# Exemple
# java/Message.java -> jclass/Message.class
JCLASS = $(patsubst $(JSRC_DIR)%.java,$(JCLASS_DIR)%.class,$(JSRC))

.SECONDARY: $(JCLASS)

java: $(JAR)

%.jar: $(JCLASS)
	jar cfe $@ $* -C $(JCLASS_DIR) . 

$(JCLASS_DIR):
	@mkdir -p $(JCLASS_DIR)

$(JCLASS_DIR)%.class: $(JSRC_DIR)%.java | $(JCLASS_DIR)
	$(JC) -d $(JCLASS_DIR) -cp $(JSRC_DIR) $(JCFLAGS) $<

# Nettoyage des fichiers de compilation Java
cleanj:
	rm -rf $(JCLASS_DIR) $(JAR)


#############################
# COMPILATION DES SCENARIOS #
#############################
SCENARIO = $(wildcard scenario_*)
SCENARIO_DIR = $(addsuffix /,$(SCENARIO))

FIND_JSRC = $(wildcard $(dir)*.java)
SCENARIO_JCLASS := $(foreach dir,$(SCENARIO_DIR),$(FIND_JSRC:.java=.class))

FIND_CSRC = $(wildcard $(dir)*.c)
SCENARIO_COBJ := $(foreach dir,$(SCENARIO_DIR),$(FIND_CSRC:.c=.o))

SCENARIO_EXEC = $(join $(SCENARIO_DIR),$(SCENARIO))


scenario: c java $(SCENARIO_EXEC) $(SCENARIO_JCLASS)


$(SCENARIO_EXEC): $(SCENARIO_COBJ)
	$(if $(wildcard $(@D)/*.o),\
		$(CC) -o $@ $(CFLAGS) -I $(CSRC_DIR) $(wildcard $(@D)/*.o))

$(SCENARIO_COBJ): %.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) -I $(CSRC_DIR)

$(SCENARIO_JCLASS): %.class: %.java
	$(JC) -d $(@D) -classpath $(JCLASS_DIR):$(@D)/ $(JCFLAGS) $<

cleans:
	rm -rf $(SCENARIO_EXEC) $(SCENARIO_JCLASS) $(SCENARIO_COBJ)

# .SECONDEXPANSION:
# $(SCENARIO_EXEC): $$(filter $(dir $$@)/%.o,$(SCENARIO_COBJ))
# 	$(info $^)
# 	$(CC) -o $@ $^
