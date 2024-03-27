## Variables

# Default Raspberry Pi IP address
IP_RPI?=192.168.234.131
# Path to cross-compiler binaries
PATH_CC_BINS?=/home/tomas/Bureau/'Objets Connectés'/tools-master/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin
# Default target fake root directory
TARGET_FAKEROOT_RPI?=
# Compiler command
CCC?=$(PATH_CC_BINS)/arm-linux-gnueabihf-gcc-4.8.3
# Programs to build
PROG=pimusiic pi2iserv rfidReader
# Path to rpi binaries
BIN_RPI_DIR=bin-pi
# Path to pc binaries
BIN_PC_DIR=bin-pc
# Programs for Raspberry Pi
PROG_RPI=$(addprefix $(BIN_RPI_DIR)/, $(PROG))
# Programs for PC
PROG_PC=$(addprefix $(BIN_PC_DIR)/, $(PROG))
# Path to source codes
SRC_DIR=src
# Path to header files
INCLUDE_DIR=include
# Path to obj files 
OBJ_DIR=obj
# Path to lib files
LIB_DIR=lib
# Compilation flags
CPFLAGS =-I$(INCLUDE_DIR)
# Linker flags
LB_FLAG =-lncurses -lwiringPi -lpthread -lm -lasound -lrfid -lbcm2835
LD_FLAGS =-L$(LIB_DIR)


## Rules
.PHONY:all clean docs install


all:$(PROG_RPI) docs

test:
	@echo "Test environnement :"
	@echo "CCC=$(CCC)"
	@echo "IP_RPI=$(IP_RPI)"
	@echo "PATH_CC_BINS=$(PATH_CC_BINS)"
	@echo "TARGET_FAKEROOT_RPI=$(TARGET_FAKEROOT_RPI)"


$(BIN_PC_DIR)/%: $(OBJ_DIR)/%-pc.o $(LIB_DIR)/libmusic-pc.a $(LIB_DIR)/libinet-pc.a
	@mkdir -p $(BIN_PC_DIR)
	@echo "Compilation du programme $@"
	@gcc -o $@ $< -I$(INCLUDE_DIR) -lmusic-pc -linet-pc $(LD_FLAGS) $(LB_FLAG) 

$(BIN_RPI_DIR)/%: $(OBJ_DIR)/%-pi.o $(LIB_DIR)/libmusic-pi.a  $(LIB_DIR)/libinet-pi.a
	@mkdir -p $(BIN_RPI_DIR)
	@echo "Compilation du programme $@"
	@$(CCC) -o $@ $< -I$(INCLUDE_DIR) -lmusic-pi -linet-pi $(LD_FLAGS) $(LB_FLAG)



######## FOR HOST ########
$(OBJ_DIR)/pimusiic-pc.o: $(SRC_DIR)/pimusiic.c
	@mkdir -p $(OBJ_DIR)
	@echo "\t\tCompilation du fichier objet $@"
	@gcc -o $@ -c  $< -I$(INCLUDE_DIR)

$(OBJ_DIR)/pi2iserv-pc.o: $(SRC_DIR)/pi2iserv.c
	@mkdir -p $(OBJ_DIR)
	@echo "\t\tCompilation du fichier objet $@"
	@gcc -o $@ -c  $< -I$(INCLUDE_DIR)

$(LIB_DIR)/libmusic-pc.a: $(OBJ_DIR)/graphicseq-pc.o $(OBJ_DIR)/mpp-pc.o $(OBJ_DIR)/note-pc.o $(OBJ_DIR)/sound-pc.o $(OBJ_DIR)/wiringseq-pc.o $(OBJ_DIR)/request-pc.o
	@mkdir -p $(LIB_DIR)
	@echo "\tCompilation de la librairie $@"
	@ar rcs $@ $^

$(LIB_DIR)/libinet-pc.a: $(OBJ_DIR)/data-pc.o $(OBJ_DIR)/session-pc.o $(OBJ_DIR)/mysyscall-pc.o
	@mkdir -p $(LIB_DIR)
	@echo "\tCompilation de la librairie $@"
	@ar rcs $@ $^

$(OBJ_DIR)/%-pc.o: $(SRC_DIR)/%.c $(INCLUDE_DIR)/%.h
	@mkdir -p $(OBJ_DIR)
	@echo "\t\tCompilation du fichier objet $@"
	@gcc -o $@ -c  $< -I$(INCLUDE_DIR) -DSESSION_DEBUG -DDATA_DEBUG

######## FOR TARGET ########
$(OBJ_DIR)/pimusiic-pi.o: $(SRC_DIR)/pimusiic.c
	@mkdir -p $(OBJ_DIR)
	@echo "\t\tCompilation du fichier objet $@"
	@$(CCC) -o $@ -c  $< -I$(INCLUDE_DIR) -g
	
$(OBJ_DIR)/rfidReader-pi.o: $(SRC_DIR)/rfidReader.c
	@mkdir -p $(OBJ_DIR)
	@echo "\t\tCompilation du fichier objet $@"
	@$(CCC) -o $@ -c  $< -I$(INCLUDE_DIR) -g

$(OBJ_DIR)/pi2iserv-pi.o: $(SRC_DIR)/pi2iserv.c
	@mkdir -p $(OBJ_DIR)
	@echo "\t\tCompilation du fichier objet $@"
	@$(CCC) -o $@ -c  $< -I$(INCLUDE_DIR) -g

$(LIB_DIR)/libmusic-pi.a: $(OBJ_DIR)/graphicseq-pi.o $(OBJ_DIR)/mpp-pi.o $(OBJ_DIR)/note-pi.o $(OBJ_DIR)/sound-pi.o $(OBJ_DIR)/wiringseq-pi.o $(OBJ_DIR)/request-pi.o
	@mkdir -p $(LIB_DIR)
	@echo "\tCompilation de la librairie $@"
	@ar rcs $@ $^

$(LIB_DIR)/libinet-pi.a: $(OBJ_DIR)/data-pi.o $(OBJ_DIR)/session-pi.o $(OBJ_DIR)/mysyscall-pi.o
	@mkdir -p $(LIB_DIR)
	@echo "\tCompilation de la librairie $@"
	@ar rcs $@ $^

$(OBJ_DIR)/%-pi.o: $(SRC_DIR)/%.c $(INCLUDE_DIR)/%.h
	@mkdir -p $(OBJ_DIR)
	@echo "\t\tCompilation du fichier objet $@"

	@$(CCC) -o $@ -c  $< -I$(INCLUDE_DIR) -DSESSION_DEBUG -DDATA_DEBUG -Wall -g

# installation rule
install:
	scp -r ressources $(BIN_RPI_DIR) pi@$(IP_RPI):testTomas/bin-pi

# Clean rule
clean:
	rm -rf $(OBJ_DIR)/* $(BIN_PC_DIR)/* $(BIN_RPI_DIR)/* $(LIB_DIR)/*

docs:
	@doxygen Doxyfile 

##
