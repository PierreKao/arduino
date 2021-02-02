DIR_Config   = ./lib/Config
DIR_Driver   = ./lib/Driver
DIR_LD3320   = ./lib/LD3320
DIR_Examples = ./examples
DIR_BIN      = ./bin

OBJ_C = $(wildcard ${DIR_Driver}/*.c ${DIR_Config}/*.c ${DIR_LD3320}/*.c ${DIR_Examples}/*.c)
OBJ_O = $(patsubst %.c,${DIR_BIN}/%.o,$(notdir ${OBJ_C}))

TARGET = main

# USELIB = USE_BCM2835_LIB
USELIB = USE_WIRINGPI_LIB
# USELIB = USE_DEV_LIB
DEBUG = -D $(USELIB)
ifeq ($(USELIB), USE_BCM2835_LIB)
    LIB = -lbcm2835 -lm 
else ifeq ($(USELIB), USE_WIRINGPI_LIB)
    LIB = -lwiringPi -lm 
endif
LIB += -lpthread

CC = gcc
MSG = -g -O0 -Wall
CFLAGS += $(MSG) $(DEBUG)

${TARGET}:${OBJ_O}
	$(CC) $(CFLAGS) $(OBJ_O) -o $@ $(LIB)
    
${DIR_BIN}/%.o:$(DIR_Examples)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@ -I $(DIR_LD3320) -I $(DIR_Config) -I $(DIR_Driver)
    
${DIR_BIN}/%.o:$(DIR_Driver)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@ -I $(DIR_Config)
    
${DIR_BIN}/%.o:$(DIR_LD3320)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@ -I $(DIR_Driver) -I $(DIR_Config)

${DIR_BIN}/%.o:$(DIR_Config)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@ $(LIB)
	
clean :
	rm $(DIR_BIN)/*.* 
	rm $(TARGET) 
