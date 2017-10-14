#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

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

    // Create a socket.

    int fd = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (fd < 0) {
        fprintf (stderr, "%s: Unable to create socket: %s\n", argv[0], strerror (errno));
        return errno;
    }

    // Bind socket address.

    struct sockaddr_in addr;

    memset ((void*) &addr, 0, sizeof (addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl (INADDR_ANY);
    addr.sin_port = htons (port);

    int retval = bind (fd, (struct sockaddr*) &addr, sizeof (addr));

    if (retval < 0) {
        fprintf (stderr, "%s: Unable to bind socket: %s\n", argv[0], strerror (errno));
        return errno;
    }

    // Listen for any data.

    while (1) {
        char buf[1024];

        int len = read (fd, buf, sizeof (buf));
        buf[len] = 0;

        printf ("%d: %s\n", len, buf);
    }

    return EXIT_SUCCESS;
}
