#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 80
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <IP address>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *ip = argv[1];  // Use the provided IP address from the command-line argument
    int sock;
    struct sockaddr_in server_addr;
    char request[BUFFER_SIZE];
    char response[BUFFER_SIZE];

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return EXIT_FAILURE;
    }

    // Set up the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Convert IP address to binary format
    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        close(sock);
        return EXIT_FAILURE;
    }

     struct timeval timeout;
     // Set timeout for connect
    timeout.tv_sec = 4; // Timeout in seconds
    timeout.tv_usec = 0; // 0 microseconds
    
    // Set socket option for timeout
    if (setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout)) < 0) {
        perror("Set socket send timeout failed");
        close(sock);
        return EXIT_FAILURE;
    }
    // Connect to the server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("X(..%s\n",argv[1]);
        close(sock);
        return EXIT_FAILURE;
    }

    // Create HTTP GET request with additional headers
    snprintf(request, sizeof(request),
             "GET / HTTP/1.1\r\n"
             "Host: www.nirsoft.net\r\n"
             "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8\r\n"
             "Accept-Encoding: gzip, deflate, br, zstd\r\n"
             "Accept-Language: en-US,en;q=0.9\r\n"
             "Cache-Control: max-age=0\r\n"
             "Connection: close\r\n"  // Use 'close' for a one-time request
             "Referer: https://search.brave.com/\r\n"
             "Sec-CH-UA: \"Chromium\";v=\"130\", \"Brave\";v=\"130\", \"Not?A_Brand\";v=\"99\"\r\n"
             "Sec-CH-UA-Mobile: ?0\r\n"
             "Sec-CH-UA-Platform: \"Linux\"\r\n"
             "Sec-Fetch-Dest: document\r\n"
             "Sec-Fetch-Mode: navigate\r\n"
             "Sec-Fetch-Site: cross-site\r\n"
             "Sec-Fetch-User: ?1\r\n"
             "Sec-GPC: 1\r\n"
             "Upgrade-Insecure-Requests: 1\r\n"
             "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/130.0.0.0 Safari/537.36\r\n"
             "\r\n");

    // Send the request
    send(sock, request, strlen(request), 0);

    // Read response headers
    int bytes_received;
    int total_bytes = 0;
    char *header_end = NULL;

    while ((bytes_received = recv(sock, response + total_bytes, sizeof(response) - total_bytes - 1, 0)) > 0) {
        total_bytes += bytes_received;
        response[total_bytes] = '\0';  // Null-terminate the response

        // Check for end of headers
        if ((header_end = strstr(response, "\r\n\r\n")) != NULL) {
            *header_end = '\0';  // Null-terminate at the end of headers
            break;
        }
    }

    // Print the response headers
    printf("%s\n%s\n", argv[1], response);

    // Close the socket
    close(sock);

    return EXIT_SUCCESS;
}

