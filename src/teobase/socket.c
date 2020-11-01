#include "teobase/socket.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "teobase/types.h"

#include "teobase/platform.h"

#if defined(TEONET_OS_WINDOWS)
// TODO: Stop using deprecated functions and remove this define.
#include "teobase/windows.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#else
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#endif

#include "teobase/logging.h"
#include "teobase/time.h"

// Set value of timeval structure to time value specified in milliseconds.
void teosockTimevalFromMs(struct timeval* timeval_ptr, int64_t time_value_ms) {
    if (time_value_ms != 0) {
        // Note: On Windows tv_sec is defined as long. Explicit cast to long is needed to avoid warning.
        // On LP64 (i.e. 4/8/8) systems tv_sec is defined as time_t and size of long type match size of time_t.
        timeval_ptr->tv_sec = (long)(time_value_ms / MILLISECONDS_IN_SECOND);
        timeval_ptr->tv_usec = (time_value_ms % MILLISECONDS_IN_SECOND) * MICROSECONDS_IN_MILLISECOND;
    } else {
        timeval_ptr->tv_sec = 0;
        timeval_ptr->tv_usec = 0;
    }
}

// Set value of timeval structure to time value specified in microseconds.
void teosockTimevalFromUs(struct timeval* timeval_ptr, int64_t time_value_us) {
    if (time_value_us != 0) {
        // Note: On Windows tv_sec is defined as long. Explicit cast to long is needed to avoid warning.
        // On LP64 (i.e. 4/8/8) systems tv_sec is defined as time_t and size of long type match size of time_t.
        timeval_ptr->tv_sec = (long)(time_value_us / MICROSECONDS_IN_SECOND);
        timeval_ptr->tv_usec = time_value_us % MICROSECONDS_IN_SECOND;
    } else {
        timeval_ptr->tv_sec = 0;
        timeval_ptr->tv_usec = 0;
    }
}

// Creates a TCP socket.
teonetSocket teosockCreateTcp() {
    return socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
}

// Establishes a connection to a specified server.
teosockConnectResult teosockConnect(teonetSocket socket, const char* server, uint16_t port) {
    struct sockaddr_in serveraddr;

    memset(&serveraddr, 0, sizeof(struct sockaddr_in));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);

#if !defined(TEONET_COMPILER_MINGW)
    int result = inet_pton(AF_INET, server, &serveraddr.sin_addr);

    // Resolve host address if needed.
    if (result != 1) {
        struct hostent* hostp = gethostbyname(server);
        if (hostp == NULL) {
            return TEOSOCK_CONNECT_HOST_NOT_FOUND;
        }

        memcpy(&serveraddr.sin_addr, hostp->h_addr_list[0], sizeof(serveraddr.sin_addr));
    }
#else
    // MinGW compiler still doest not support inet_pton().
    serveraddr.sin_addr.s_addr = inet_addr(server);

    // Resolve host address if needed.
    if (serveraddr.sin_addr.s_addr == (unsigned long)INADDR_NONE) {
        struct hostent* hostp = gethostbyname(server);
        if (hostp == NULL) {
            return TEOSOCK_CONNECT_HOST_NOT_FOUND;
        }

        memcpy(&serveraddr.sin_addr, hostp->h_addr_list[0], sizeof(serveraddr.sin_addr));
    }
#endif

    // Connect to server.
    int connect_result = connect(socket, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (connect_result != 0 && errno != EINPROGRESS) {
        return TEOSOCK_CONNECT_FAILED;
    }

    return TEOSOCK_CONNECT_SUCCESS;
}

// Establishes a connection to a specified server.
teosockConnectResult teosockConnectTimeout(teonetSocket* sock, const char* server, uint16_t port, int timeout_ms) {
    struct addrinfo hints;
    struct addrinfo *rp;
    struct addrinfo *res;
    memset(&hints, '\0', sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_UNSPEC;
    hints.ai_protocol = IPPROTO_TCP;
    int fd, n;
    int connect_result = -1;

    char port_ch[10];
    sprintf(port_ch, "%d", port);
    if ( (n = getaddrinfo(server, port_ch, &hints, &res)) != 0) {
        LTRACK_E("TeonetClient", "getaddrinfo: %s", gai_strerror(n));
        return TEOSOCK_CONNECT_HOST_NOT_FOUND;
    }

    for (rp = res; rp != NULL; rp = rp->ai_next) {
        fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (fd == -1) continue;

        int set_non_locking_result = teosockSetBlockingMode(fd, TEOSOCK_NON_BLOCKING_MODE);

        if (set_non_locking_result == TEOSOCK_SOCKET_ERROR) {
            teosockClose(fd);
            continue;
        }

        connect_result = connect(fd, rp->ai_addr, rp->ai_addrlen);
        if (connect_result == 0) {
            goto success_connect;
        } else {
            int in_progress = 0;

            #if defined(TEONET_OS_WINDOWS)
            int error_code = WSAGetLastError();

            if (error_code == WSAEWOULDBLOCK) {
                in_progress = 1;
            }
            #else
            int error_code = errno;

            if (error_code == EINPROGRESS) {
                in_progress = 1;
            }
            #endif

            if (in_progress != 1) {
                teosockClose(fd);
                continue;
            }
        }

        int select_result = teosockSelect(fd, TEOSOCK_SELECT_MODE_WRITE | TEOSOCK_SELECT_MODE_ERROR, timeout_ms);

        if (select_result == TEOSOCK_SELECT_TIMEOUT || select_result == TEOSOCK_SELECT_ERROR) {
            teosockClose(fd);
            continue;
        }

        int error = 0;
        socklen_t error_len = sizeof(error);

        int getsockopt_result = getsockopt(fd, SOL_SOCKET, SO_ERROR, (char*)&error, &error_len);

        if (getsockopt_result == TEOSOCK_SOCKET_ERROR || error != 0) {
            teosockClose(fd);
            continue;
        } else {
            goto success_connect;
        }

        teosockClose(fd);
    }

    freeaddrinfo(res);
    return TEOSOCK_CONNECT_FAILED;

success_connect:
    freeaddrinfo(res);
    *sock = fd;
    return TEOSOCK_CONNECT_SUCCESS;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// #if !defined(TEONET_COMPILER_MINGW)
//     int result = inet_pton(AF_INET, server, &serveraddr.sin_addr);

//     // Resolve host address if needed.
//     if (result != 1) {
//         struct hostent* hostp = gethostbyname(server);
//         if (hostp == NULL) {
//             return TEOSOCK_CONNECT_HOST_NOT_FOUND;
//         }

//         memcpy(&serveraddr.sin_addr, hostp->h_addr_list[0], sizeof(serveraddr.sin_addr));
//     }
// #else
//     // MinGW compiler still doest not support inet_pton().
//     serveraddr.sin_addr.s_addr = inet_addr(server);

//     // Resolve host address if needed.
//     if (serveraddr.sin_addr.s_addr == (unsigned long)INADDR_NONE) {
//         struct hostent* hostp = gethostbyname(server);
//         if (hostp == NULL) {
//             return TEOSOCK_CONNECT_HOST_NOT_FOUND;
//         }

//         memcpy(&serveraddr.sin_addr, hostp->h_addr_list[0], sizeof(serveraddr.sin_addr));
//     }
// #endif

//     int set_non_locking_result = teosockSetBlockingMode(socket, TEOSOCK_NON_BLOCKING_MODE);

//     if (set_non_locking_result == TEOSOCK_SOCKET_ERROR) {
//         teosockClose(socket);
//         return TEOSOCK_CONNECT_FAILED;
//     }

//     // Connect to server.
//     int connect_result = connect(socket, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

//     if (connect_result == 0) {
//         return TEOSOCK_CONNECT_SUCCESS;
//     }
//     else {
//         int in_progress = 0;

// #if defined(TEONET_OS_WINDOWS)
//         int error_code = WSAGetLastError();

//         if (error_code == WSAEWOULDBLOCK) {
//             in_progress = 1;
//         }
// #else
//         int error_code = errno;

//         if (error_code == EINPROGRESS) {
//             in_progress = 1;
//         }
// #endif

//         if (in_progress != 1) {
//             teosockClose(socket);
//             return TEOSOCK_CONNECT_FAILED;
//         }
//     }

//     int select_result = teosockSelect(socket, TEOSOCK_SELECT_MODE_WRITE | TEOSOCK_SELECT_MODE_ERROR, timeout_ms);

//     if (select_result == TEOSOCK_SELECT_TIMEOUT || select_result == TEOSOCK_SELECT_ERROR) {
//         teosockClose(socket);
//         return TEOSOCK_CONNECT_FAILED;
//     }

//     int error = 0;
//     socklen_t error_len = sizeof(error);

//     int getsockopt_result = getsockopt(socket, SOL_SOCKET, SO_ERROR, (char*)&error, &error_len);

//     if (getsockopt_result == TEOSOCK_SOCKET_ERROR || error != 0) {
//         teosockClose(socket);
//         return TEOSOCK_CONNECT_FAILED;
//     }

//     return TEOSOCK_CONNECT_SUCCESS;
}

// Receives data from a connected socket.
ssize_t teosockRecv(teonetSocket socket, uint8_t* data, size_t length) {
#if defined(TEONET_OS_WINDOWS)
    if (length > (ssize_t)INT_MAX) {
        // Can't receive this much data.
        return TEOSOCK_SOCKET_ERROR;
    }

    return recv(socket, (char*)data, (int)length, 0);
#else
    return read(socket, data, length);
#endif
}

// Check if error code from recvfrom is recoverable socket error.
static bool teosockRecvfromErrorIsRecoverable(int error_code) {
#if defined(TEONET_OS_WINDOWS)
    return error_code == WSAEWOULDBLOCK;
#else
    // EWOULDBLOCK may be not defined or may have same value as EAGAIN.
#if defined(EWOULDBLOCK) && EWOULDBLOCK != EAGAIN
    return error_code == EAGAIN || error_code == EWOULDBLOCK;
#else
    return error_code == EAGAIN;
#endif
#endif
}

// Check if error code from recvfrom is fatal socket error.
static bool teosockRecvfromErrorIsFatal(int error_code) {
#if defined(TEONET_OS_WINDOWS)
    return error_code == WSAEFAULT || error_code == WSAEINVAL ||
           error_code == WSAENOTSOCK || error_code == WSAESHUTDOWN;
#else
    return error_code == ENOTCONN || error_code == EBADF;
#endif
}

// Receives data from a connection-mode or connectionless-mode socket.
teosockRecvfromResult teosockRecvfrom(
    teonetSocket socket, uint8_t *buffer, size_t buffer_size,
    struct sockaddr *__restrict address, socklen_t *address_length,
    size_t *received_length, int *error_code) {
    teosockRecvfromResult udp_recvfrom_result = TEOSOCK_RECVFROM_UNKNOWN_ERROR;
    int flags = 0;

#if defined(TEONET_OS_WINDOWS)
    if (buffer_size > (size_t)INT_MAX) {
        buffer_size = (size_t)INT_MAX;
    }
    ssize_t recvlen =
        recvfrom(socket, buffer, (int)buffer_size, flags, address, address_length);
#else
    ssize_t recvlen =
        recvfrom(socket, buffer, buffer_size, flags, address, address_length);
#endif

    if (recvlen == -1) {
#if defined(TEONET_OS_WINDOWS)
        int recv_errno = WSAGetLastError();
#else
        int recv_errno = errno;
#endif
        if (error_code != NULL) {
            *error_code = recv_errno;
        }

        if (teosockRecvfromErrorIsRecoverable(recv_errno)) {
            udp_recvfrom_result = TEOSOCK_RECVFROM_TRY_AGAIN;
        } else if (teosockRecvfromErrorIsFatal(recv_errno)) {
            udp_recvfrom_result = TEOSOCK_RECVFROM_FATAL_ERROR;
        } else {
            udp_recvfrom_result = TEOSOCK_RECVFROM_UNKNOWN_ERROR;
        }
    } else if (recvlen == 0) {
        udp_recvfrom_result = TEOSOCK_RECVFROM_ORDERLY_CLOSED;
    } else {
        if (received_length != NULL) {
            *received_length = (size_t)recvlen;
        }

        udp_recvfrom_result = TEOSOCK_RECVFROM_DATA_RECEIVED;
    }

    return udp_recvfrom_result;
}

// Sends data on a connected socket.
ssize_t teosockSend(teonetSocket socket, const uint8_t* data, size_t length) {
#if defined(TEONET_OS_WINDOWS)
    if (length > (ssize_t)INT_MAX) {
        // Can't send this much data.
        return TEOSOCK_SOCKET_ERROR;
    }

    return send(socket, (const char*)data, (int)length, 0);
#else
    return write(socket, data, length);
#endif
}

// Determines the status of the socket, waiting if necessary, to perform synchronous operation.
teosockSelectResult teosockSelect(teonetSocket socket, int status_mask, int timeout_ms) {
    fd_set socket_fd_set;
    memset(&socket_fd_set, 0, sizeof(socket_fd_set));

    // Create a descriptor set with specified socket.
    FD_ZERO(&socket_fd_set);
    FD_SET(socket, &socket_fd_set);

    fd_set* read_fd_set = (status_mask & TEOSOCK_SELECT_MODE_READ) ? &socket_fd_set : NULL;
    fd_set* write_fd_set = (status_mask & TEOSOCK_SELECT_MODE_WRITE) ? &socket_fd_set : NULL;
    fd_set* error_fd_set = (status_mask & TEOSOCK_SELECT_MODE_ERROR) ? &socket_fd_set : NULL;

    struct timeval timeval_timeout;
    memset(&timeval_timeout, 0, sizeof(timeval_timeout));

    teosockTimevalFromMs(&timeval_timeout, timeout_ms);

#if defined(TEONET_OS_WINDOWS)
    int result = select(0, read_fd_set, write_fd_set, error_fd_set, &timeval_timeout);
#else
    int result = select(socket + 1, read_fd_set, write_fd_set, error_fd_set, &timeval_timeout);
#endif

    // Make sure that return value is correct.
    if (result > 0) {
        result = TEOSOCK_SELECT_READY;
    }

    return result;
}

// Closes a socket.
int teosockClose(teonetSocket socket) {
#if defined(TEONET_OS_WINDOWS)
    return closesocket(socket);
#else
    return close(socket);
#endif
}

// Disables sends and/or receives on a socket.
int teosockShutdown(teonetSocket socket, teosockShutdownMode mode) {
    return shutdown(socket, mode);
}

// Sets blocking or non-blocking mode for specified socket.
int teosockSetBlockingMode(teonetSocket socket, teosockBlockingMode blocking_mode) {
#if defined(TEONET_OS_WINDOWS)
    u_long mode = (u_long)blocking_mode;

    return ioctlsocket(socket, FIONBIO, &mode);
#else
    int flags = fcntl(socket, F_GETFL, 0);

    if (flags == -1) {
        return TEOSOCK_SOCKET_ERROR;
    } else {
        int new_flags;
        if (blocking_mode == TEOSOCK_BLOCKING_MODE) {
            new_flags = flags & ~O_NONBLOCK;
        } else {
            new_flags = flags | O_NONBLOCK;
        }

        int result;
        if (new_flags == flags) {
            result = TEOSOCK_SOCKET_SUCCESS;
        } else {
            result = fcntl(socket, F_SETFL, new_flags);

            if (result != TEOSOCK_SOCKET_ERROR) {
                result = TEOSOCK_SOCKET_SUCCESS;
            }
        }

        return result;
    }
#endif
}

// Set TCP_NODELAY option on specified socket.
int teosockSetTcpNodelay(teonetSocket socket) {
    int flag = 1;

    int result = setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag));

    if (result != TEOSOCK_SOCKET_SUCCESS) {
        result = TEOSOCK_SOCKET_ERROR;
    }

    return result;
}

// Initialize socket library.
int teosockInit() {
#if defined(TEONET_OS_WINDOWS)
    WORD required_version = MAKEWORD(2, 2);

    WSADATA wsa_data;
    memset(&wsa_data, 0, sizeof(wsa_data));

    int result = WSAStartup(required_version, &wsa_data);

    if (result == TEOSOCK_SOCKET_SUCCESS) {
        // Check that windows socket library support v 2.2.
        if (LOBYTE(wsa_data.wVersion) != 2 || HIBYTE(wsa_data.wVersion) != 2) {
            WSACleanup();
            result = TEOSOCK_SOCKET_ERROR;
        }
    } else {
        result = TEOSOCK_SOCKET_ERROR;
    }

    return result;
#else
    return TEOSOCK_SOCKET_SUCCESS;
#endif
}

// Cleanup socket library.
int teosockCleanup() {
#if defined(TEONET_OS_WINDOWS)
    return WSACleanup();
#else
    return TEOSOCK_SOCKET_SUCCESS;
#endif
}
