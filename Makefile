audio: audio.o udp.o json.o openal.o
	$(CC) -o audio audio.o udp.o json.o openal.o `pkg-config --libs openal` -lm

udp.o: udp.c

audio.o: audio.c

json.o: json.c

openal.o: openal.c
	$(CC) -c openal.c `pkg-config --cflags openal`

clean:
	rm -f *.o audio
