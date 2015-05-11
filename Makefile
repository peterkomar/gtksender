CC = gcc
#FLAGS = `pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0` -lX11 -lXrender -lXtst
HEDERS = `pkg-config --cflags gtk+-2.0`
LIBS = `pkg-config --libs gtk+-2.0` -lX11 -lXrender -lXtst
DIR_SRC = src/
CONF_SRC = conf/
OBJ_DIR = obj/
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p
DEL_DIR       = rmdir


#SOURCES  = implementation.h interface.c main.c
#if [![ -e $(OBJ_DIR1) ]]
#then
#mkdir $(OBJ_DIR1)
#fi
all: make_obj bik_bak bik_bak_conf

make_obj:
	@$(CHK_DIR_EXISTS) $(OBJ_DIR) || $(MKDIR) $(OBJ_DIR)

bik_bak: $(OBJ_DIR)main.o $(OBJ_DIR)interface.o $(OBJ_DIR)keyboard.o $(OBJ_DIR)xlib.o $(OBJ_DIR)configfile.o
	$(CC) -o bin/bik_bak_deamon $(OBJ_DIR)main.o $(OBJ_DIR)keyboard.o $(OBJ_DIR)interface.o $(OBJ_DIR)xlib.o $(OBJ_DIR)configfile.o $(LIBS)
	
bik_bak_conf: $(OBJ_DIR)conf_main.o $(OBJ_DIR)configurator.o $(OBJ_DIR)configfile.o $(OBJ_DIR)xlib.o
	$(CC) -o bin/bik_bak_conf $(OBJ_DIR)conf_main.o $(OBJ_DIR)configurator.o $(OBJ_DIR)configfile.o $(OBJ_DIR)xlib.o $(LIBS)

$(OBJ_DIR)conf_main.o: $(CONF_SRC)conf_main.c $(CONF_SRC)configurator.h
	$(CC) -c -o $(OBJ_DIR)conf_main.o $(CONF_SRC)conf_main.c $(HEDERS)
	
$(OBJ_DIR)configurator.o: $(CONF_SRC)configurator.c $(CONF_SRC)configurator.h $(CONF_SRC)configfile.h $(DIR_SRC)xlib.h
	$(CC) -c -o $(OBJ_DIR)configurator.o $(CONF_SRC)configurator.c $(HEDERS)

$(OBJ_DIR)main.o: $(DIR_SRC)main.c $(DIR_SRC)interface.h
	$(CC) -c -o $(OBJ_DIR)main.o $(DIR_SRC)main.c $(HEDERS)
$(OBJ_DIR)interface.o: $(DIR_SRC)interface.c $(DIR_SRC)interface.h
	$(CC) -c -o $(OBJ_DIR)interface.o $(DIR_SRC)interface.c $(HEDERS)
$(OBJ_DIR)keyboard.o: $(DIR_SRC)keyboard.c $(DIR_SRC)keyboard.h
	$(CC) -c -o $(OBJ_DIR)keyboard.o $(DIR_SRC)keyboard.c $(HEDERS)
$(OBJ_DIR)xlib.o: $(DIR_SRC)xlib.c $(DIR_SRC)xlib.h
	$(CC) -c -o $(OBJ_DIR)xlib.o $(DIR_SRC)xlib.c $(HEDERS)
$(OBJ_DIR)configfile.o: $(CONF_SRC)configfile.c $(CONF_SRC)configfile.h
	$(CC) -c -o $(OBJ_DIR)configfile.o $(CONF_SRC)configfile.c $(HEDERS)
clean: 
	rm -f $(OBJ_DIR)*.o
