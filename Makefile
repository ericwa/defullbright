all: defullbright.exe defullbright

defullbright.exe: defullbright.c
	i686-w64-mingw32-gcc -o defullbright.exe defullbright.c

defullbright: defullbright.c
	gcc -Wall -o defullbright defullbright.c -lm

clean:
	rm -f defullbright defullbright.exe
