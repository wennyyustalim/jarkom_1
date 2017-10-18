.PHONY: all clean

all: recvfile sendfile

%.o: %.cpp
	g++ -c -I. -std=c++11 -o $@ $<

recvfile: Buffer.o Node.o PacketAck.o PacketData.o Receiver.o recvfile.o
	g++ -std=c++11 -o $@ $^

sendfile: Buffer.o Node.o PacketAck.o PacketData.o Sender.o sendfile.o
	g++ -std=c++11 -o $@ $^

clean:
	rm *.o
	rm recvfile sendfile

