all: fifoCurses-server fifoCurses-client

fifoCurses-server: fifoCurses-server.c
	gcc -o fifoCurses-server fifoCurses-server.c -lncurses

fifoCurses-client: fifoCurses-client.c
	gcc -o fifoCurses-client fifoCurses-client.c -lncurses

clear:
	rm fifoCurses-server fifoCurses-client