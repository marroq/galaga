CC=gcc -g
CFLAGS=-Wall -I/usr/local/include -L/usr/local/lib
ALLEGRO_CFLAGS=-Wall -I/usr/local/include -L/usr/local/lib -lallegro_image -lallegro -lallegro_dialog -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec

galaga: galaga.c
	$(CC) galaga.c -o galaga $(ALLEGRO_CFLAGS)
	
