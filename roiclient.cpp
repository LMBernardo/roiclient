//--------------------------
// Roomba Open Interface Client
//--------------------------
// Description:
// Sends commands to roiserver to control roomba
// Designed specifically for Roomba 405, may not provide full functionality for more advanced models.
//--------------------------
// Author: Layne Bernardo
// Email: retnuh66@gmail.com
//
// Created July 20th, 2018
// Modified: August 11th, 2018
// Version 0.2
//--------------------------

//-----------------------------
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <string>

#include "lib/client_socket_handler.h"

enum Console_type { SOCKET, SERIAL, DRIVE, MOTORS };

void print_usage(){
        std::cout << "Usage: roiclient host port\n\n";
}

void err_quit(int errsv){
        std::cout << "Errno: " << errsv << std::endl;
        std::cout << "Exiting.." << std::endl;
        std::exit(-1);
}

void wakeRoomba(client_socket_handler* c_socket_p){
        c_socket_p->send_string("sendcmd,wake");
}

// Start ROI, send CONTROL command, and change LED to orange
// Puts Roomba in SAFE mode
void initSafe(client_socket_handler* c_socket_p){
        c_socket_p->send_string("sendcmd,init_safe");
        c_socket_p->send_string("sendcmd,power_led,100 220");
}

void power_led(client_socket_handler* c_socket_p){
        std::string led_data = "";
        std::cout << "Please enter power LED data: " << std::endl;
        std::getline(std::cin, led_data);
        std::string command_string = "sendcmd,power_led," + led_data;
        c_socket_p->send_string(command_string);
}

void add_song(client_socket_handler* c_socket_p){
        std::string song_string = "";
        std::cout << "Please enter song data: " << std::endl;
        std::getline(std::cin, song_string);
        std::string command_string = "";
        command_string = "sendcmd,add_song," + song_string;
        c_socket_p->send_string(command_string);
}

void play_song(client_socket_handler* c_socket_p){
        std::string song_num = "";
        std::cout << "Please enter song number: " << std::endl;
        std::getline(std::cin, song_num);
        std::string command_string = "";
        command_string = "sendcmd,play_song," + song_num;
        c_socket_p->send_string(command_string);
}

// Generic console
void command_console(Console_type type, client_socket_handler* c_socket_p){
        std::string mode;
        std::string command;
        switch(type) {
        case SOCKET:
                mode = "Raw Socket";
                command = "sendcmd,";
                break;
        case SERIAL:
                mode = "Raw Serial";
                command = "sendcmd,raw,";
                break;
        case DRIVE:
                mode = "Drive";
                command = "sendcmd,drive";
        case MOTORS:
                mode = "Motors";
                command = "sendcmd,motors";
        default:
                mode = "Raw Socket";
                command = "sendcmd,";
                break;

        }
        while (1) {
                std::cout << mode + " Mode: enter data or type 'quit' to exit:" << std::endl;

                std::string velocity;
                if (type == DRIVE) {
                        std::cout << "Velocity: " << std::endl;
                        std::getline(std::cin, velocity);
                        if (velocity == "quit" || velocity == "Quit" || velocity == "QUIT") {
                                break;
                        }
                        std::cout << "Radius: " << std::endl;
                }

                std::string input_string;
                std::getline(std::cin, input_string);
                if (input_string == "quit" || input_string == "Quit" || input_string == "QUIT") {
                        break;
                }
                if (type == DRIVE) input_string = velocity + " " + input_string;
                c_socket_p->send_string(command + input_string);
        }
}

void drive_console(client_socket_handler* c_socket_p){
        while (1) {
                std::cout << "Drive Mode: enter data or type 'quit' to exit:" << std::endl;

                std::string velocity, radius;
                std::cout << "Velocity: " << std::endl;
                std::getline(std::cin, velocity);
                if (velocity == "quit" || velocity == "Quit" || velocity == "QUIT") {
                        break;
                }
                std::cout << "Radius: " << std::endl;
                std::getline(std::cin, radius);
                if (radius == "quit" || radius == "Quit" || radius == "QUIT") {
                        break;
                }

                c_socket_p->send_string("sendcmd,drive," + velocity + " " + radius);
        }
}

void server_console(client_socket_handler* c_socket_p, std::string command = "sendcmd,"){
        while (1) {
                std::cout << "Raw Socket Mode: enter data or type 'quit' to exit:" << std::endl;

                std::string input_string;
                std::getline(std::cin, input_string);
                if (input_string == "quit" || input_string == "Quit" || input_string == "QUIT") {
                        break;
                }
                c_socket_p->send_string(command + input_string);
        }
}

void roomba_console(client_socket_handler* c_socket_p){
        while (1) {
                std::cout << "Raw Serial Mode: enter data or type 'quit' to exit:" << std::endl;

                std::string input_string;
                std::getline(std::cin, input_string);
                if (input_string == "quit" || input_string == "Quit" || input_string == "QUIT") {
                        break;
                }
                c_socket_p->send_string("sendcmd,raw," + input_string);
        }
}

int advanced_menu(client_socket_handler* c_socket_p){

        int myChoice = 0;

        std::cout << std::endl;
        std::cout << "********** Advanced Menu **********" << std::endl;
        std::cout << "(1): Send Raw Data to Server" << std::endl;
        std::cout << "(2): Send Raw Data to Roomba" << std::endl;
        std::cout << "(3): Quit Menu" << std::endl;
        std::cin >> myChoice;

        std::cin.ignore(100,'\n');

        switch(myChoice)
        {
        case 1:
                server_console(c_socket_p);
                break;
        case 2:
                roomba_console(c_socket_p);
                break;
        case 3:
                return 1;
        default:
                std::cout << "ERROR! You have selected an invalid choice." << std::endl;
                break;
        }

        return 0;
}


// Main menu
// Taken from: http://www.cplusplus.com/forum/general/44671/
int mainMenu(client_socket_handler* c_socket_p){
        int myChoice = 0;

        std::cout << std::endl;
        std::cout << "********** Main Menu **********" << std::endl;
        std::cout << "(1): Init Roomba (Safe Mode)" << std::endl;
        std::cout << "(2): Power LED" << std::endl;
        std::cout << "(3): Add Song" << std::endl;
        std::cout << "(4): Play Song" << std::endl;
        std::cout << "(5): Drive" << std::endl;
        std::cout << "(6): Motors" << std::endl;
        std::cout << "(7): Advanced" << std::endl;
        std::cout << "(8): Power Off" << std::endl;
        std::cout << "(9): Exit" << std::endl;
        std::cin >> myChoice;

        std::cin.ignore(100,'\n');

        switch(myChoice)
        {
        case 1:
                wakeRoomba(c_socket_p);
                initSafe(c_socket_p);
                break;
        case 2:
                power_led(c_socket_p);
                break;
        case 3:
                add_song(c_socket_p);
                break;
        case 4:
                play_song(c_socket_p);
                break;
        case 5:
                drive_console(c_socket_p);
                break;
        case 6:
                command_console(MOTORS, c_socket_p);
                break;
        case 7:
                // Loop advanced menu
                while ( advanced_menu(c_socket_p) == 0 ) {}
                break;
        case 8:
                c_socket_p->send_string( "sendcmd,power_off" );
                break;
        case 9:
                return 1;
                break;
        default:
                std::cout << "ERROR! You have selected an invalid choice." << std::endl;
                break;
        }
        return 0;
}

int main( int argc, char* argv[] ){

        if (argc < 3) {
                print_usage();
                exit(-1);
        }

        std::string hostname;
        int port_number;
        try {
                port_number = std::stoi(argv[2]);
        } catch (...) {
                print_usage();
                exit(-1);
        }

        hostname = argv[1];

        client_socket_handler c_socket;

        if ( c_socket.c_create() < 0) {
                std::cout << "Failed to create socket" << std::endl;
                std::exit(-1);
        } else {
                std::cout << "Socket created successfully" << std::endl;
        }

        if ( c_socket.c_connect(hostname.c_str(), port_number) == 0 ) {
                std::cout << "Connected to host: " << hostname << std::endl;
        } else {
                std::exit(-1);
        }

        // Loop main menu
        while ( mainMenu(&c_socket) == 0 ) {}

        c_socket.c_close();

}
