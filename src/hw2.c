#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "hw2.h"

// char int_to_bits (int num) {
//     static char bits[33];
//     bits[32] = '\0';
    
//     for (int i = 31; i >= 0; i--) {
//         int bit = (num >> i) & 1;
//         bits[31- i] = bit ? '1' : '0';
//     }
//     //printf("%s", bits);
//     return bits;
// }

int checkRead (unsigned int packet) {
    if ((packet >> 10) & 0x7FFFF) {
        return 0; //Not read request (if any of the bits are '1')
    }
    return 1; //Is a read request
}

int checkWrite (unsigned int packet) {
    if (((packet >> 31) & 1) == 0 && ((packet >> 30) & 1) == 1 && ((packet >> 10) & 0x7FFFF) == 0) {
        return 1; //Is write request
    }

    return 0; //Not write request
}

unsigned int getRequesterID (unsigned int packet) {
    return (packet >> 16) & 0xFFFF;
}

void print_packet(unsigned int packet[])
{
    //char packet_type;
    //int packet_address;
    //int *packet_data;    

    // //Convert Packet to Bits
    // char* packet_in_bits = int_to_bits(packet);

    //Length
    int packet_length = packet[0] & 0x03FF;

    //Requester ID 
    int packet_ID = getRequesterID(packet[1]);

    //Tag
    int packet_tag = ((packet[1] >> 8) & 0x00FF);

    //Last BE
    int packet_Last_BE = ((packet[1] >> 4) & 0x000F);

    //1st BE
    int packet_First_BE = ((packet[1] & 0x000F));


    //printf("Packet Type: %d\n", packet_type);
    //printf("Address: %d\n", packet_address);
    printf("Length: %d\n", packet_length);
    printf("Requester ID: %d\n", packet_ID);
    printf("Tag: %d\n", packet_tag);
    printf("Last BE: %d\n", packet_Last_BE);
    printf("1st BE: %d\n", packet_First_BE);
    //printf("Data: %d\n", packet_data);

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
