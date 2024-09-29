#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "hw2.h"

void int_to_bits (int num) {
    static char bits[33];
    bits[32] = '\0';
    
    for (int i = 31; i >= 0; i--) {
        int bit = (num >> i) & 1;
        bits[31- i] = bit ? '1' : '0';
    }
    //printf("%s", bits);
    return bits;
}

void print_packet(unsigned int packet[])
{

    char* packet_type;
    int packet_address;
    int packet_ID;
    int packet_tag;
    int packet_Last_BE;
    int packet_First_BE;
    int packet_data;    

    //Determine if it's a Write Request or Read Request
    //Check bits 31 to 10
    char* 


    printf("Packet Type: %d\n", packet_type);
    printf("Address: %d\n", packet_address);
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
