exec = leto.out
sources = $(wildcard src/*.c)
objects = $(sources:.c=.o)
flags = -g `pkg-config --cflags coelum` -lsqlite3 -lathena -lhermes -I../coelum/GL/include `pkg-config --libs coelum`


$(exec): $(objects)
	gcc $(objects) $(flags) -o $(exec)

%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@

clean:
	-rm *.out
	-rm *.o
	-rm src/*.o
