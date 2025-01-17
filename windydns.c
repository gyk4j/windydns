/*Placed in the public domain by Sam Trenholme*/
/*Ported by gyk4j to build on Windows/MinGW*/

#if defined _MSC_VER || defined __MINGW32__

#include <stdio.h>
#include <winsock2.h>
#include <windows.h>

#else

#include <arpa/inet.h>
#define SOCKET int

#endif

#include <string.h>
#include <stdint.h>

#if defined _MSC_VER || defined __MINGW32__
typedef int socklen_t;
#endif

int main(int argc, char **argv){
    // Stores number of bytes received (packet size)
    uint32_t i;
    
    // For incoming DNS request data
    char query[512],
    // Template packet for outgoing DNS responses
    // Note first 12 bytes are initialized, with last 5 bytes unwritten.
    // The last 5 bytes will be filled in after parsing the command line
    // reply IP address.
        packet[17]="\xc0\f\0\x01\0\x01\0\0\0\0\0\x04";
    
    if(argc > 1){

        // Initialize Windows Socket
        // Reference: https://jweinst1.medium.com/how-to-use-udp-sockets-on-windows-29e7e60679fe
        #if defined _MSC_VER || defined __MINGW32__
        WSADATA wsaData;
        int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (res != NO_ERROR) {
            printf("WSAStartup failed with error %d\n", res);
            return 1;
        }
        #endif

        // Allocate storage for source address
        struct sockaddr_in from;
        socklen_t fromlen=511;
        // Clear the memory block. Set to bind to 0.0.0.0.
        //bzero(&from, sizeof(struct sockaddr_in));
        memset(&from, 0, sizeof(struct sockaddr_in));
        
        SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);
        
        // Parse the DNS reply IP string into a inet_addr (unsigned long 4 bytes),
        // and pre-fill it in the response packet template.
        *((uint32_t*)(packet+12)) = inet_addr(argv[1]);
        from.sin_family=AF_INET;
        from.sin_port=htons(53);
        
        // Bind to 0.0.0.0 of local address. Implicitly set by bzero. 
        bind(s,(struct sockaddr*)&from, sizeof(struct sockaddr_in));
        
        // Keep processing incoming DNS requests
        for(;;){
            // For each DNS request, store data in query and 
            // return number of bytes received in i.
            i = recvfrom(s, query, 255, 0, (struct sockaddr*)&from, &fromlen);
            
            // Sanity checks for valid request UDP packet
            // Check if number of bytes received > 9, and 3rd byte >= 0
            if( i > 9 && query[2] >= 0 ){
                // Patch the question and reuse for reply. 
                query[2] |= 128;
                query[11] ? query[3] |= 4 : 1;
                query[7]++;
                
                // Skip the area used by DNS query.
                // Append the DNS reply after that.
                memcpy(query+i, packet, 16);
                
                // Send the reply.
                sendto(s, query, i+16, 0, (struct sockaddr*)&from, sizeof(struct sockaddr_in));
            }
        }
    }
    
    return 0;
}
