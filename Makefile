build:
	gcc main.c filters.h image.h image.c utils.c utils.h common.c common.h -o image_editor -lm -Wextra -Wall -std=c99

clean:
	rm -f *.o
	rm -f *.h.gch
	rm -f image_editor