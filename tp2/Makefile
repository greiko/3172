
all: serveur client

serveur: serveur.c
	gcc -o server serveur.c

client: client.c
	gcc -o client client.c

clean:
	rm -f server
	rm -f client
	rm -f pipe*
	rm -f serveurP
