clear
gcc -c server.c
gcc server.o -o server
gcc -c serverMulti.c
gcc serverMulti.o -o serverMulti
gcc -c client.c
gcc client.o -o client
rm client.o
rm server.o
rm serverMulti.o
