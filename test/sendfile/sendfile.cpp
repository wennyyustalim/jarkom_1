#include <arpa/inet.h>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "Sender.h"

int main (int argc, char** argv) {

    if (argc != 6) {
        fprintf (stderr, "Usage:\n");
        fprintf (stderr, "%s FILE WINDOW_SIZE BUFFER_SIZE DEST_IP DEST_PORT\n", argv[0]);

        return EXIT_FAILURE;
    }

    // Validate parameters.

    const char* path = argv[1];
    int win_size = atoi (argv[2]);
    int buf_size = atoi (argv[3]);
    const char* host = argv[4];
    int port = atoi (argv[5]);
    
    // Open file for reading.

    int fd_local = open (path, O_RDONLY);
    
    if (fd_local < 0) {
        fprintf (stderr, "%s: Unable to open file: %s\n", argv[0], strerror (errno));
        return errno;
    }

    // Create a socket.

    int fd_net = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    if (fd_net < 0) {
        fprintf (stderr, "%s: Unable to create socket: %s\n", argv[0], strerror (errno));
        return errno;
    }

    // Connect socket address.

    struct sockaddr_in addr;
    
    memset ((void*) &addr, 0, sizeof (addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons (port);
    inet_pton (AF_INET, host, &(addr.sin_addr));

    int ret_connect = connect (fd_net, (struct sockaddr*) &addr, sizeof (addr));

    if (ret_connect < 0) {
        fprintf (stderr, "%s: Unable to connect socket: %s\n", argv[0], strerror (errno));
        return errno;
    }

    // Begin transmitting.

    Sender node (buf_size);

    node.fd_net = fd_net;
    node.fd_local = fd_local;
    node.win_size = win_size;

    return node.run ();
}
