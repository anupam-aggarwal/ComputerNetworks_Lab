clear
gcc -c sender.c
gcc sender.o -o sender
gcc -c receiver.c
gcc receiver.o -o receiver
rm receiver.o
rm sender.o