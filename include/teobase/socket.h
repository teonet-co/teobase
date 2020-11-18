/**
 * @file teobase/socket.h
 * @brief Cross platform wrappers for socket specific routines.
 */

#pragma once

#ifndef TEOBASE_SOCKET_H
#define TEOBASE_SOCKET_H

#include "teobase/types.h"

#include "teobase/platform.h"

#if defined(TEONET_OS_WINDOWS)
#include "teobase/windows.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#endif

#include "teobase/api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Set value of timeval structure to time value specified in milliseconds.
 *
 * @param timeval_ptr [out] A pointer to timeval structure.
 * @param time_value_ms [in] Time value in milliseconds.
 */
TEOBASE_API void teosockTimevalFromMs(struct timeval* timeval_ptr, int64_t time_value_ms);

/**
 * Set value of timeval structure to time value specified in microseconds.
 *
 * @param timeval_ptr [out] A pointer to timeval structure.
 * @param time_value_us [in] Time value in microseconds.
 */
TEOBASE_API void teosockTimevalFromUs(struct timeval* timeval_ptr, int64_t time_value_us);

/// Alias for socket type on current platform. SOCKET on windows, int on Linux.
#if defined(TEONET_OS_WINDOWS)
typedef SOCKET teonetSocket;
#else
typedef int teonetSocket;
#endif

/// Unnamed enumeration with integer constants.
typedef enum {
    TEOSOCK_SOCKET_SUCCESS = 0,  ///< Value for indicating successful result in socket function.

#if defined(TEONET_OS_WINDOWS)
    TEOSOCK_SOCKET_ERROR = SOCKET_ERROR,  ///< Value for indicating error result in socket function.
    TEOSOCK_INVALID_SOCKET = INVALID_SOCKET,  ///< Value for indicating invalid socket descriptor.
#else
    TEOSOCK_SOCKET_ERROR = -1,  ///< Value for indicating error result in socket function.
    TEOSOCK_INVALID_SOCKET = -1,  ///< Value for indicating invalid socket descriptor.
#endif
} TEOSOCK_ERRORS;

/**
 * Creates a TCP socket.
 *
 * @returns TEOSOCK_INVALID_SOCKET on error, socket handle otherwise.
 */
TEOBASE_API teonetSocket teosockCreateTcp(void);

/// Result enumeration for teosockConnect() function.
typedef enum teosockConnectResult {
    TEOSOCK_CONNECT_SUCCESS = 1,  ///< Successful connection.
    TEOSOCK_CONNECT_HOST_NOT_FOUND = -1,  ///< Failed to resolve host address.
    TEOSOCK_CONNECT_FAILED = -2,  ///< Failed to connect to server.
} teosockConnectResult;

/**
 * Establishes a connection to a specified server.
 *
 * @param socket_descriptor Socket descriptor obtained using teosockCreateTcp() function.
 * @param server Server IP address or domain name.
 * @param port Port to connect to.
 *
 * @returns Result of operation.
 *
 * @retval TEOSOCK_CONNECT_SUCCESS if connection successfully established.
 * @retval TEOSOCK_CONNECT_HOST_NOT_FOUND if failed to resolve host address.
 * @retval TEOSOCK_CONNECT_FAILED if failed to connect to server.
 */
TEOBASE_API teosockConnectResult teosockConnect(
    teonetSocket socket_descriptor,
    const char* server,
    uint16_t port);

/**
 * Establishes a connection to a specified server.
 *
 * @param socket_descriptor Socket descriptor obtained using teosockCreateTcp() function.
 * @param server Server IP address or domain name.
 * @param port Port to connect to.
 * @param timeout_ms Maximum amount of time to wait before returning error, in milliseconds.
 *
 * @returns Result of operation.
 *
 * @retval TEOSOCK_CONNECT_SUCCESS if connection successfully established.
 * @retval TEOSOCK_CONNECT_HOST_NOT_FOUND if failed to resolve host address.
 * @retval TEOSOCK_CONNECT_FAILED if failed to connect to server.
 *
 * @note Socket will be left in non-blocking mode.
 */
TEOBASE_API teosockConnectResult teosockConnectTimeout(
    teonetSocket* socket_descriptor,
    const char* server,
    uint16_t port,
    int timeout_ms);

/**
 * Receives data from a connected socket.
 *
 * @param socket_descriptor Socket descriptor obtained using teosockCreateTcp() function.
 * @param data A pointer to the buffer to store the data.
 * @param length The length of buffer in bytes.
 *
 * @returns TEOSOCK_SOCKET_ERROR on error, amount of received bytes otherwise.
 */
TEOBASE_API ssize_t teosockRecv(
    teonetSocket socket_descriptor,
    uint8_t* data,
    size_t length);

/// Result enumeration for teosockRecvfrom() function.
typedef enum teosockRecvfromResult {
    TEOSOCK_RECVFROM_DATA_RECEIVED = 0,  ///< Data was received. The length of received message is stored in @a received_length argument. @a address structure is filled with remote address.
    TEOSOCK_RECVFROM_ORDERLY_CLOSED = 1,  ///< The socket was orderly shut down.
    TEOSOCK_RECVFROM_TRY_AGAIN = 2,  ///< Recoverable error occurred. No data was received. Error code is returned in @a error_code argument.
    TEOSOCK_RECVFROM_FATAL_ERROR = 3,  ///< An unrecoverable error occurred. Error code is returned in @a error_code argument.
    TEOSOCK_RECVFROM_UNKNOWN_ERROR = 4,  ///< An unknown error occurred. Error code is returned in @a error_code argument.
} teosockRecvfromResult;

/**
 * Receives data from a connection-mode or connectionless-mode socket.
 *
 * @param[in] socket_descriptor Socket descriptor obtained using teosockCreateTcp() function.
 * @param[in] buffer A pointer to the buffer to store the data.
 * @param[in] buffer_size The length of buffer in bytes.
 * @param[out] address A sockaddr structure in which the sending address is to be stored.
 * @param[in,out] address_length The length of a structure pointed to by @a address argument.
 * @param[out] received_length A null pointer, or points to a variable in which the length of received message in bytes is to be stored if data was received.
 * @param[out] error_code A null pointer, or points to a variable in which the error code is to be stored.
 *
 * @returns Result of operation.
 */
TEOBASE_API teosockRecvfromResult teosockRecvfrom(
    teonetSocket socket_descriptor,
    uint8_t* buffer,
    size_t buffer_size,
    struct sockaddr* __restrict address,
    socklen_t* address_length,
    size_t* received_length,
    int* error_code);

/**
 * Sends data on a connected socket.
 *
 * @param socket_descriptor Socket descriptor obtained using teosockCreateTcp() function.
 * @param data A pointer to the buffer with data.
 * @param length The length of data to be transmitted, in bytes.
 *
 * @returns TEOSOCK_SOCKET_ERROR on error, amount of sent bytes otherwise.
 *
 * @note Amount of bytes sent can be less than the number requested to be sent
 * in the @p length parameter.
 */
TEOBASE_API ssize_t teosockSend(
    teonetSocket socket_descriptor,
    const uint8_t* data,
    size_t length);

/// Enumeration with bit flags for status masks for teosockSelect function.
typedef enum teosockSelectMode {
    TEOSOCK_SELECT_MODE_READ = 1 << 0,  ///< Check socket for readability.
    TEOSOCK_SELECT_MODE_WRITE = 1 << 1,  ///< Check socket for writability.
    TEOSOCK_SELECT_MODE_ERROR = 1 << 2,  ///< Check socket for errors.
} teosockSelectMode;

/// Result enumeration for teosockSelect() function.
typedef enum teosockSelectResult {
    TEOSOCK_SELECT_READY = 1,  ///< Socket is ready or have data to be read.
    TEOSOCK_SELECT_TIMEOUT = 0,  ///< Socket is not ready or no data was received before reaching timeout.
    TEOSOCK_SELECT_ERROR = -1,  ///< An error occurred.
} teosockSelectResult;

/**
 * Determines the status of the socket, waiting if necessary, to perform synchronous operation.
 *
 * @param socket_descriptor Socket descriptor obtained using teosockCreateTcp() function.
 * @param status_mask A combination of teosockSelectMode flags defining modes to check.
 * @param timeout_ms The amount of time to wait before returning timeout, in milliseconds.
 *
 * @returns Result of operation.
 *
 * @retval TEOSOCK_SELECT_READY if socket have data ready to be read.
 * @retval TEOSOCK_SELECT_TIMEOUT if no data was received before reaching timeout.
 * @retval TEOSOCK_SELECT_ERROR if an error occurred.
 */
TEOBASE_API teosockSelectResult teosockSelect(
    teonetSocket socket_descriptor,
    int status_mask,
    int timeout_ms);

/**
 * Closes a socket.
 *
 * @param socket_descriptor Socket descriptor obtained using teosockCreateTcp() function.
 *
 * @returns Result of operation.
 *
 * @retval TEOSOCK_SOCKET_SUCCESS if operation completed successfully.
 * @retval TEOSOCK_SOCKET_ERROR if operation failed.
 */
TEOBASE_API int teosockClose(teonetSocket socket_descriptor);

/// Enumeration for specifying socket shutdown mode in teosockShutdown() function.
typedef enum teosockShutdownMode {
    /// Shutdown receiving data. SHUT_RD on Unix, SD_RECEIVE on Windows.
    /// @note Data that is already in socket buffer still may be received.
    TEOSOCK_SHUTDOWN_RD = 0,
    /// Shutdown sending data. SHUT_WR on Unix, SD_SEND on Windows.
    TEOSOCK_SHUTDOWN_WR = 1,
    /// Shutdown both receiving and sending data. SHUT_RDWR on Unix, SD_BOTH on Windows.
    TEOSOCK_SHUTDOWN_RDWR = 2,
} teosockShutdownMode;

/**
 * Disables sends and/or receives on a socket.
 *
 * @param socket_descriptor Socket descriptor obtained using teosockCreateTcp() function.
 * @param mode Socket shutdown mode. See #teosockShutdownMode.
 *
 * @returns Result of operation.
 *
 * @retval TEOSOCK_SOCKET_SUCCESS if operation completed successfully.
 * @retval TEOSOCK_SOCKET_ERROR if operation failed.
 */
TEOBASE_API int teosockShutdown(
    teonetSocket socket_descriptor,
    teosockShutdownMode mode);

/// Enumeration for specifying socket blocking mode in teosockShutdown() function.
typedef enum teosockBlockingMode {
    /// Set socket to blocking mode.
    TEOSOCK_BLOCKING_MODE = 0,
    /// Set socket to non-blocking mode.
    TEOSOCK_NON_BLOCKING_MODE = 1,
} teosockBlockingMode;

/**
 * Set blocking or non-blocking mode on a socket.
 *
 * @param socket_descriptor Socket descriptor obtained using teosockCreateTcp() function.
 * @param blocking_mode Blocking mode to set socket to.
 *
 * @returns Result of operation.
 *
 * @retval TEOSOCK_SOCKET_SUCCESS if operation completed successfully.
 * @retval TEOSOCK_SOCKET_ERROR if operation failed.
 */
TEOBASE_API int teosockSetBlockingMode(
    teonetSocket socket_descriptor,
    teosockBlockingMode blocking_mode);

/**
 * Set TCP_NODELAY option on a socket.
 *
 * @param socket_descriptor Socket descriptor obtained using teosockCreateTcp() function.
 *
 * @returns Result of operation.
 *
 * @retval TEOSOCK_SOCKET_SUCCESS if operation completed successfully.
 * @retval TEOSOCK_SOCKET_ERROR if operation failed.
 *
 * @warning As stated here https://msdn.microsoft.com/en-us/library/ms740476(v=vs.85).aspx
 * we should not set TCP_NODELAY unless the impact of doing so is well-understood
 * and desired because setting TCP_NODELAY can have a significant negative impact
 * on network and application performance.
 */
TEOBASE_API int teosockSetTcpNodelay(teonetSocket socket_descriptor);

/**
 * Initialize socket library.
 *
 * Call this function before any other socket function.
 * On Windows this function initiates use of the Winsock 2 library.
 * This function does nothing on Linux.
 *
 * @returns Result of operation.
 *
 * @retval TEOSOCK_SOCKET_SUCCESS if operation completed successfully.
 * @retval TEOSOCK_SOCKET_ERROR if operation failed.
 */
TEOBASE_API int teosockInit(void);

/**
 * Cleanup socket library.
 *
 * Call this function when socket functions are no longer needed.
 * On Windows this function terminates use of the Winsock 2 library.
 * This function does nothing on Linux.
 *
 * @returns Result of operation.
 *
 * @retval TEOSOCK_SOCKET_SUCCESS if operation completed successfully.
 * @retval TEOSOCK_SOCKET_ERROR if operation failed.
 */
TEOBASE_API int teosockCleanup(void);

#ifdef __cplusplus
}
#endif

#endif
