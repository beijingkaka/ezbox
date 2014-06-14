CC	= gcc

CFLAGS	= -I../ `pkg-config lua --cflags` -fno-rtti -fno-exceptions \
	  -I../luajson

LIBS    = `pkg-config lua --libs`

SRC	= ../llsplib.cpp ../llspaux.cpp \
	  ../luajson/luajson.cpp ../luajson/luajson_parser.cpp

all: mod_mongoose

mod_mongoose:
	$(CC) -DTHREAD_SAFE $(CFLAGS) -shared -fPIC -o libluasp-mongoose.so $(SRC) $(LIBS)
