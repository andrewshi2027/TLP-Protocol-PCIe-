#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "hw2.h"

void print_packet(unsigned int packet[])
{
    char* packet_type; 
     //Address
    int packet_address;
    //Length
    int packet_length;
    //Requester ID
    int packet_ID;
    //Tag
    int packet_tag;
    //Last BE
    int packet_Last_BE;
    //1st BE
    int packet_First_BE;

    //Packet Type
    //Write
    if (((packet[0] >> 31) & 1) == 0 && ((packet[0] >> 30) & 1) == 1 && ((packet[0] >> 10) & 0x7FFFF) == 0) {
        packet_type = "Write";
        //Address
        packet_address = (packet[2]);
        //Length
        packet_length = packet[0] & 0x03FF;
        //Requester ID
        packet_ID = ((packet[1] >> 16) & 0xFFFF);
        //Tag
        packet_tag = ((packet[1] >> 8) & 0x00FF);
        //Last BE
        packet_Last_BE = ((packet[1] >> 4) & 0x000F);
        //1st BE
        packet_First_BE = ((packet[1] & 0x000F));

        printf("Packet Type: %s\n", packet_type);
        printf("Address: %d\n", packet_address);
        printf("Length: %d\n", packet_length);
        printf("Requester ID: %d\n", packet_ID);
        printf("Tag: %d\n", packet_tag);
        printf("Last BE: %d\n", packet_Last_BE);
        printf("1st BE: %d\n", packet_First_BE);
        printf("Data: ");
        for (int i = 3; i < packet_length + 3; i++) {
            printf("%d ", packet[i]);
        }
    }
    //Read
    else if (!((packet[0] >> 10) & 0x7FFFF)) {
        packet_type = "Read";
        //Address
        packet_address = (packet[2]);
        //Length
        packet_length = packet[0] & 0x03FF;
        //Requester ID
        packet_ID = ((packet[1] >> 16) & 0xFFFF);
        //Tag
        packet_tag = ((packet[1] >> 8) & 0x00FF);
        //Last BE
        packet_Last_BE = ((packet[1] >> 4) & 0x000F);
        //1st BE
        packet_First_BE = ((packet[1] & 0x000F));

        printf("Packet Type: %s\n", packet_type);
        printf("Address: %d\n", packet_address);
        printf("Length: %d\n", packet_length);
        printf("Requester ID: %d\n", packet_ID);
        printf("Tag: %d\n", packet_tag);
        printf("Last BE: %d\n", packet_Last_BE);
        printf("1st BE: %d\n", packet_First_BE);
    } 
    else {
        packet_type = "Unknown";
    }

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
