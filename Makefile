HEADERS = controls.h shader_utils.h 
OBJECTS = controls.o shader_utils.o main.o
LINKFLAGS = -lGL -lGLEW -lglfw
BUILDDIR=build

%.o: %.c $(HEADERS)
	gcc -c -g3 $< -o $@

build: $(OBJECTS)
	gcc $(OBJECTS) $(LINKFLAGS) -o mandelbrot

clean:
	-rm -f $(OBJECTS)
	-rm -f program