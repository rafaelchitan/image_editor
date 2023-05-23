build:
	gcc -lm -Wextra -Wall -std=c99 main.c image.c utils.c common.c shapes.c convert.c -o image_editor 

clean:
	rm -f *.o
	rm -f *.h.gch
	rm -f image_editor