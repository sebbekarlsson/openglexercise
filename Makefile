OS := $(shell uname)

$(info $(OS))

G_FLAGZ= -Wall -g -fPIC -lglfw -IGL/include

ifeq ($(OS), Darwin)
    FLAGZ=$(G_FLAGZ) -ldl -fPIC
else
    FLAGZ=$(G_FLAGZ) -ldl -fPIC
endif

EXEC = app.out
SOURCES := $(wildcard src/*.c)
SOURCES += $(wildcard GL/src/*.c)
OBJECTS := $(SOURCES:.c=.o)
OBJECTS_NO_MAIN := $(filter-out src/main.o,$(OBJECTS))

$(info $(OBJECTS_NO_MAIN))

$(EXEC): $(OBJECTS)
	gcc $(OBJECTS) $(FLAGZ) -o $(EXEC) -fPIC

%.o: %.c ../include/glad/%.h
	gcc -c $(G_FLAGZ) $< -o $@ -fPIC

%.o: %.c include/%.h
	gcc -c $(G_FLAGZ) $< -o $@ -fPIC

# liblearnopengl.so: $(OBJECTS_NO_MAIN)

clean:
	-rm *.out
	-rm *.o
	-rm src/*.o
	-rm src/*/*.o
	-rm src/*/*/*.o
