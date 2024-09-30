#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "hw2.h"

// int checkRead (unsigned int packet[]) {
//     if ((packet[0] >> 10) & 0x7FFFF) {
//         return 0; //Not read request (if any of the bits are '1')
//     }
//     return 1; //Is a read request
// }

// int checkWrite (unsigned int packet[]) {
//     if (((packet[0] >> 31) & 1) == 0 && ((packet[0] >> 30) & 1) == 1 && ((packet[0] >> 10) & 0x7FFFF) == 0) {
//         return 1; //Is write request
//     }
//     return 0; //Not write request
// }

void print_packet(unsigned int packet[])
{
    char* packet_type; 

    //Packet Type
    if (((packet[0] >> 31) & 1) == 0 && ((packet[0] >> 30) & 1) == 1 && ((packet[0] >> 10) & 0x7FFFF) == 0) {
        packet_type = "Write";
    }
    else if (!((packet[0] >> 10) & 0x7FFFF)) {
        packet_type = "Read";
    }

    //Address
    int packet_address = (packet[2]);

    //Length
    int packet_length = packet[0] & 0x03FF;

    //Requester ID 
    int packet_ID = ((packet[1] >> 16) & 0xFFFF);

    //Tag
    int packet_tag = ((packet[1] >> 8) & 0x00FF);

    //Last BE
    int packet_Last_BE = ((packet[1] >> 4) & 0x000F);

    //1st BE
    int packet_First_BE = ((packet[1] & 0x000F));

    //Data
    int packet_data = packet[3];


    printf("Packet Type: %s\n", packet_type);
    printf("Address: %d\n", packet_address);
    printf("Length: %d\n", packet_length);
    printf("Requester ID: %d\n", packet_ID);
    printf("Tag: %d\n", packet_tag);
    printf("Last BE: %d\n", packet_Last_BE);
    printf("1st BE: %d\n", packet_First_BE);
    printf("Data: %d\n", packet_data);

    (void) packet;
}

void store_values(unsigned int packets[], char *memory)
{
    (void)packets;
    (void)memory;
}

unsigned int* create_completion(unsigned int packets[], const char *memory)
{
    (void)packets;
    (void)memory;
	return NULL;
}
