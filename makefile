LIBS=`pkg-config --libs gtk+-3.0 --cflags gtk+-3.0` -lmysqlclient
all:
	gcc main.c ${LIBS} -o programmer
