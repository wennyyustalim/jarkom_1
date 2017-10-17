#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "Receiver.h"
#include <PacketData.h>

int main (int argc, char** argv) {

    if (argc != 5) {
        fprintf (stderr, "Usage:\n");
        fprintf (stderr, "%s FILE WINDOW_SIZE BUFFER_SIZE PORT\n", argv[0]);

        return EXIT_FAILURE;
    }

    // Validate parameters.

    const char* path = argv[1];
    int win_size = atoi (argv[2]);
    int buf_size = atoi (argv[3]);
    int port = atoi (argv[4]);

    // Open file for writing.

    int fd_loc = open (path, O_WRONLY | O_CREAT);

    if (fd_loc < 0) {
        fprintf (stderr, "%s: Unable to open file: %s\n", argv[0], strerror (errno));
        return errno;
    }

    // Create a socket.

    int fd_net = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (fd_net < 0) {
        fprintf (stderr, "%s: Unable to create socket: %s\n", argv[0], strerror (errno));
        return errno;
    }

    // Bind socket address.

    struct sockaddr_in addr;

    memset ((void*) &addr, 0, sizeof (addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl (INADDR_ANY);
    addr.sin_port = htons (port);

    int ret_bind = bind (fd_net, (struct sockaddr*) &addr, sizeof (addr));

    if (ret_bind < 0) {
        fprintf (stderr, "%s: Unable to bind socket: %s\n", argv[0], strerror (errno));
        return errno;
    }

    // Listen for any data.

    Receiver receiver (fd_net, fd_loc);

    receiver.pref_win_size = win_size;
    receiver.win_size = std::min (receiver.data_size, receiver.pref_win_size);

    PacketData packet;

    while (1) {
        read (fd_net, &packet, sizeof (PacketData));

        printf ("DEBUG: Receive packet!\n");
        printf (" soh: %d\n", packet.soh);
        printf (" seq_num: %d\n", packet.seq_num);
        printf (" stx: %d\n", packet.stx);
        printf (" data: %d\n", packet.data);
        printf (" etx: %d\n", packet.etx);
        printf (" checksum: %d\n", packet.checksum);

        receiver.accept_data (packet);
    }

    return EXIT_SUCCESS;
}
