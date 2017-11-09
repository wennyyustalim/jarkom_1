.PHONY: all clean

all: recvfile sendfile

src/%.o: src/%.cpp
	g++ -g -c -I. -std=c++11 -o $@ $<

recvfile: src/Buffer.o src/Node.o src/PacketAck.o src/PacketData.o src/Receiver.o src/recvfile.o
	g++ -g -std=c++11 -o $@ $^

sendfile: src/Buffer.o src/Node.o src/PacketAck.o src/PacketData.o src/Sender.o src/sendfile.o
	g++ -g -std=c++11 -o $@ $^

clean:
	rm -f src/*.o
	rm -f recvfile sendfile
