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
        printf("\n");
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
        printf("Data: \n");
    } 
    else {
        packet_type = "Unknown";
    }

    (void) packet;
}

void store_values(unsigned int packets[], char *memory)
{
    int i = 0;
    //If packet is a Write Request
    while (((packets[i] >> 31) & 1) == 0 && ((packets[i] >> 30) & 1) == 1 && ((packets[i] >> 10) & 0x7FFFF) == 0) {

        //Address
        unsigned int packet_address = (packets[i+2]);
        //Length
        unsigned int packet_length = packets[i] & 0x03FF;
        //Last BE
        unsigned int packet_Last_BE = ((packets[i+1] >> 4) & 0x000F);
        //1st BE
        unsigned int packet_First_BE = ((packets[i+1] & 0x000F));

        if (packet_address > 0x100000) {
            i += (3 + packet_length);
            continue;
        }

        //Data elements
        for (unsigned int j = 0; j < packet_length; j++) {
            if (j == 0) { //first word/data, activate First BE
                //Byte 0
                if (packet_First_BE & 0x1) { 
                    memory[packet_address] = (packets[i+3+j] & 0xFF);
                }
                packet_address++;
                //Byte 1
                if (packet_First_BE & 0x2) {
                    memory[packet_address] = (packets[i+3+j] >> 8) & 0xFF;
                }
                packet_address++; 
                //Byte 2
                if (packet_First_BE & 0x4) {
                    memory[packet_address] = (packets[i+3+j] >> 16) & 0xFF;
                }
                packet_address++;
                //Byte 3
                if (packet_First_BE & 0x8) {
                    memory[packet_address] = (packets[i+3+j] >> 24) & 0xFF;
                }
                packet_address++;
            }
        
            else if (j == packet_length - 1) { //Last word/data, activate Last BE
                //Byte 0
                if (packet_Last_BE & 0x1) { 
                    memory[packet_address] = (packets[i+3+j] & 0xFF);
                }
                packet_address++;
                //Byte 1
                if (packet_Last_BE & 0x2) {
                    memory[packet_address] = (packets[i+3+j] >> 8) & 0xFF;
                }
                packet_address++;
                //Byte 2
                if (packet_Last_BE & 0x4) {
                    memory[packet_address] = (packets[i+3+j] >> 16) & 0xFF;
                }
                packet_address++;
                //Byte 3
                if (packet_Last_BE & 0x8) {
                    memory[packet_address] = (packets[i+3+j] >> 24) & 0xFF;
                }
                packet_address++;
            }

            else {
                //Byte 0
                memory[packet_address++] = (packets[i+3+j] & 0xFF);
                //Byte 1
                memory[packet_address++] = (packets[i+3+j] >> 8) & 0xFF;
                //Byte 2
                memory[packet_address++] = (packets[i+3+j] >> 16) & 0xFF;
                //Byte 3
                memory[packet_address++] = (packets[i+3+j] >> 24) & 0xFF;
            }
        }
    i += packet_length + 3;
    }
    (void)packets;
    (void)memory; 
}

unsigned int* create_completion(unsigned int packets[], const char *memory)
{
    int packet_index = 0;
    unsigned int complete_size = 0;

    //If packet is Read Request
    while (!((packets[packet_index] >> 10) & 0x7FFFF)) 
    {
        int packet_length = packets[packet_index++] & 0x03FF;
        packet_index++;
        int packet_address = (packets[packet_index++]);
        int address = (packet_address % 0x4000) + (packet_length * 4);

        //Determine size of completion packets
        if (address > 0x4000) {
            int split = 6 + packet_length;
            complete_size += split;
        }
        else {
            int no_split = 3 + packet_length;
            complete_size += no_split;
        }
        packet_index++;
         
    }

    packet_index = 0;

    int complete_index = 0;

    //Malloc space for Completion Packets
    unsigned int* completion_packets = (unsigned int*)malloc(complete_size * sizeof(unsigned int));

    //Packet is a Read Request
    while (!((packets[packet_index] >> 10) & 0x7FFFF)) 
    {
        int packet_length = packets[packet_index] & 0x03FF;
        int packet_address = packets[packet_index + 2];
        int requester_ID = (packets[packet_index + 1] >> 16) & 0xFFFF;
        int packet_tag = (packets[packet_index + 1] >> 8) & 0x00FF;
        int address = (packet_address % 0x4000) + (packet_length * 4);
        
        //Create First Completion
        int first_packet_length = 0;
        if (address >= 0x4000) 
        {
            first_packet_length = (0x4000 - (packet_address % 0x4000))/4;
            completion_packets[complete_index] = first_packet_length;
        }
        else {
            completion_packets[complete_index] = packet_length;
        }

        completion_packets[complete_index] = first_packet_length;
        completion_packets[complete_index++] |= ((0x4A) << 24);
        completion_packets[complete_index] = (220 << 16);
        completion_packets[complete_index++] |= first_packet_length * 4;
        completion_packets[complete_index] = (requester_ID << 16);
        completion_packets[complete_index] |= (packet_tag << 8);
        completion_packets[complete_index] |= packet_address & 0x7F;
        complete_index++;
       
        int mem_place = 0;

        //Split into two Completion Packets
        if (address >= 0x4000) {
            mem_place = 0;
            
            for (int i = packet_address % 0x4000; i < 0x4000; i++) {
                unsigned int packet_memory = memory[packet_address++];

                packet_memory &= 0xFF;
                packet_memory <<= (mem_place * 8);
                completion_packets[complete_index] |= packet_memory;
                mem_place++;
                
                if (mem_place == 3) {
                    mem_place = 0;
                    complete_index++;
                    completion_packets[complete_index] &= 0;
                }
                complete_index++;
            
            }     
            complete_index++;


            //Create the Second Packet
            int second_length = packet_length - first_packet_length;
 
            completion_packets[complete_index] = second_length;
            completion_packets[complete_index++] |= 0x4A000000;
            completion_packets[complete_index] = (220 << 16);
            completion_packets[complete_index++] |= second_length * 4;
            completion_packets[complete_index] = (requester_ID << 16);
            completion_packets[complete_index++] |= (packet_tag << 8);
            
            mem_place = 0;

            for (int i = packet_address % 0x4000; i < 0x4000; i++) 
            {
                unsigned int packet_memory = memory[packet_address++];

                packet_memory &= 0xFF;
                packet_memory <<= (mem_place * 8);
                completion_packets[complete_index] |= packet_memory;
                mem_place++;
                
                if (mem_place == 3) {
                    mem_place = 0;
                    complete_index++;
                    completion_packets[complete_index] &= 0;
                }
                complete_index++;
            }
        }   
        else {
            mem_place = 0;

            for (int i = packet_address; i < (packet_address = packet_length * 4); i++) 
            {
                
                unsigned int packet_memory = memory[i];
                
                packet_memory &= 0xFF;
                packet_memory <<= (mem_place * 8);
                completion_packets[complete_index] |= packet_memory;
                mem_place++;
                if (mem_place == 3) 
                {
                    mem_place = 0;
                    complete_index++;
                    completion_packets[complete_index] &= 0;
                }
                
            }    
            complete_index++;   
        }
        packet_index += 3;
    }
    return completion_packets;
}


