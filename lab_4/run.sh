clear
gcc -c serverUDP.c
gcc serverUDP.o -o server
gcc -c clientUDP.c
gcc clientUDP.o -o client
rm clientUDP.o
rm serverUDP.o