/*
 * Test.cpp
 *
 *  Created on: 11 maj 2016
 *      Author: jack
 */

#include "Test.h"

#include <sys/time.h>
#include <math.h>

/* Acts like a server. Echoes all the received messages. */
int Test::run_tests(){
	listen_and_accept_conn();
	char buffer[9];
	while(1)
		recv_and_send_echo_msg(buffer,sizeof(buffer));
	return 0;
}

/* Generates and sends messages and listens for echo. Measures time etc... */
int Test::run_tests(std::string remote_ble_addr,int number_of_packages_to_send,int sleep_between_packages_in_ms,
		int min_interval_start,int min_interval_stop,int max_interval_start,
		int max_interval_stop,int latency_start,int latency_stop,
		int timeout_start,int timeout_stop){

	if(validate_parameters(min_interval_start,min_interval_stop,max_interval_start,max_interval_stop,latency_start,latency_stop,timeout_start,timeout_stop)<0)
		return -2;
	if(conn_to_ble_dev(remote_ble_addr)<0)
		return -1;

	char array1[3],array2[3];
	int success,fail;
	double max_rtt,min_rtt,average_rtt,current_rtt,best_min_rtt,best_max_rtt,best_average_rtt,best_min_rtt_max_interval, best_min_rtt_min_interval,best_min_rtt_latency,best_max_rtt_max_interval, best_max_rtt_min_interval,best_max_rtt_latency,best_average_rtt_max_interval, best_average_rtt_min_interval,best_average_rtt_latency;
	best_min_rtt = 10000; best_max_rtt = 10000; best_average_rtt = 1000;

	for(int min_interval=min_interval_start;min_interval<=min_interval_stop;min_interval++){
		for(int max_interval=max_interval_start;max_interval<=max_interval_stop;max_interval++){
			for(int latency=latency_start;latency<=latency_stop;latency++){
				for(int timeout=timeout_start;timeout<=timeout_stop;timeout++){

					print_current_test_parameters(min_interval,max_interval,latency,timeout,number_of_packages_to_send);
					success = 0; fail = 0; max_rtt = 0; min_rtt = 100000; average_rtt=0;//Reset the success and fail-rate and the best case RTT values

					//Update the connection with the parameters
					ble_sender.update_conn_param(min_interval,max_interval,latency,timeout);
					long rtts[20];
					start_clock();
					for(int packages_sent=0;packages_sent<number_of_packages_to_send+1;packages_sent++){
						fill_array_with_random_characters(array1,sizeof(array1));
						start_clock();
						send_and_recv_echo_msg(array1,sizeof(array1),array2);
						stop_clock();
						current_rtt = get_time_in_ms();
						if(packages_sent>0){
							if(compare_arrays(array1,array2,sizeof(array1))==1){
								success++;

								if(number_of_packages_to_send==20)
									rtts[packages_sent] = (long)current_rtt;

								//std::cout << "\nRTT for package nr " << packages_sent << ": " << current_rtt << "  ";
								if(current_rtt>max_rtt)
									max_rtt = current_rtt;
								if(current_rtt<min_rtt)
									min_rtt = current_rtt;
								average_rtt = average_rtt + current_rtt;
							}else{
								fail++;
							}
						}
						if(sleep_between_packages_in_ms>0)
							usleep(sleep_between_packages_in_ms);
					}//packages_sent
					average_rtt = average_rtt/number_of_packages_to_send;
					//print_result(success,fail,max_rtt,min_rtt,average_rtt);

					if(number_of_packages_to_send==20)
						print_kofidensintervall(rtts);

					if(average_rtt<best_average_rtt){
						best_average_rtt = average_rtt; best_average_rtt_latency = latency; best_average_rtt_min_interval = min_interval; best_average_rtt_max_interval = max_interval;
					}
					if(min_rtt<best_min_rtt){
						best_min_rtt = min_rtt; best_min_rtt_latency = latency; best_min_rtt_min_interval = min_interval; best_min_rtt_max_interval = max_interval;
					}
					if(max_rtt<best_max_rtt){
						best_max_rtt = max_rtt; best_max_rtt_latency = latency; best_max_rtt_min_interval = min_interval; best_max_rtt_max_interval = max_interval;
					}

				}//timeout
			}//latency
		}//max_interval
	}//min_interval

	print_end_results(best_average_rtt,best_average_rtt_min_interval,best_average_rtt_max_interval,best_average_rtt_latency,best_min_rtt,best_min_rtt_min_interval,best_min_rtt_max_interval,best_min_rtt_latency,best_max_rtt,best_max_rtt_min_interval,best_max_rtt_max_interval,best_max_rtt_latency);

	return 0;
}

// **************************************************
// Private functions
// **************************************************

int Test::conn_to_ble_dev(std::string remote_ble_addr) {
	if((conn_socket = ble_sender.connect_to_remote_ble_device(remote_ble_addr)) < 0)
		return -1;
	ble_receiver.set_socket_number(conn_socket);
	return 0;
}

int Test::listen_and_accept_conn() {
	if((conn_socket = ble_receiver.l2cap_le_listen_and_accept()) < 0)
		return -1;
	ble_sender.set_socket_number(conn_socket);
	return 0;
}

int Test::send_and_recv_echo_msg(char *msg, int size,char *echo) {
	if(ble_sender.send_over_ble(msg, size)==0)
		if(ble_receiver.read_data(echo, size)==0)
			return 0;
	return -1;
}

int Test::recv_and_send_echo_msg(char* msg, int size) {
	if(ble_receiver.read_data(msg, size)==0)
		if(ble_sender.send_over_ble(msg, size)==0)
			return 0;
	return -1;
}

void Test::start_clock(){
	start = std::chrono::system_clock::now();
}

void Test::stop_clock(){
	end = std::chrono::system_clock::now();
}

double Test::get_time_in_ms(){
	std::chrono::duration<double> elapsed_seconds = end-start;
	return (elapsed_seconds).count()*1000;
}

void Test::fill_array_with_random_characters(char *array,int arraysize){
	for(int i=0;i<arraysize;i++)
		array[i]=rand() % 255 + 0;
}

int Test::compare_arrays(char *array1,char *array2,int arraysize){
	for(int i=0;i<arraysize;i++)
		if(array1[i]!=array2[i])
			return 0;
	return 1;
}

int Test::validate_parameters(int min_interval_start,int min_interval_stop,int max_interval_start,int max_interval_stop,int latency_start,int latency_stop,int timeout_start,int timeout_stop){
	if(min_interval_start<0x0006 || min_interval_start>0x0C80)
		return -2;
	if(max_interval_start<0x0006 || max_interval_start>0x0C80)
		return -2;
	if(latency_start<0x0000 || latency_stop>0x03E8)
		return -2;
	if(timeout_start<0x000A || timeout_stop> 0x0C80)
		return -2;
	return 0;
}

void Test::print_current_test_parameters(int min_interval,int max_interval,int latency,int timeout,int number_of_packages_to_send){
	std::cout << "Running test with parameters:\n";
	std::cout << "min_interval:" << min_interval << " ";
	std::cout << "max_interval: " << max_interval << " ";
	std::cout << "latency: " << latency << " ";
	std::cout << "timeout: " << timeout << " ";
	std::cout << "Number of packages to send: " << number_of_packages_to_send << std::endl;
}

void Test::print_result(int success,int fail,double max_rtt,double min_rtt,double average_rtt){
	std::cout << "\nResult-> ";
	if(fail>0)
		std::cout << "Success: " << success << " : Fail: " << fail << " : ";
	std::cout << " : Max_RTT:" << max_rtt << " : Min_RTT: " << min_rtt << " : Average RTT: " << average_rtt << std::endl;
}


void Test::print_end_results(double best_average_rtt,double best_average_rtt_min_interval,double best_average_rtt_max_interval,double best_average_rtt_latency,double best_min_rtt,double best_min_rtt_min_interval,double best_min_rtt_max_interval,double best_min_rtt_latency,double best_max_rtt,double best_max_rtt_min_interval,double best_max_rtt_max_interval,double best_max_rtt_latency){
	std::cout << "\nTests completed\n Best Results:\n";

	std::cout << "\n\nBest average RTT was: " << best_average_rtt;
	std::cout << "\nWith the following parameters:";
	std::cout << "\nmin_interval: " << best_average_rtt_min_interval;
	std::cout << "\nmax_interval: " << best_average_rtt_max_interval;
	std::cout << "\nlatency: " << best_average_rtt_latency;

	std::cout << "\n\nBest minimum RTT was: " << best_min_rtt;
	std::cout << "\nWith the following parameters:";
	std::cout << "\nmin_interval: " << best_min_rtt_min_interval;
	std::cout << "\nmax_interval: " << best_min_rtt_max_interval;
	std::cout << "\nlatency: " << best_min_rtt_latency;

	std::cout << "\n\nBest maximum RTT was: " << best_max_rtt;
	std::cout << "\nWith the following parameters:";
	std::cout << "\nmin_interval: " << best_max_rtt_min_interval;
	std::cout << "\nmax_interval: " << best_max_rtt_max_interval;
	std::cout << "\nlatency: " << best_max_rtt_latency << std::endl << std::endl;
}


	//Don't know the english name of "kofidensintervall"
	//This function only works if the array is of size 20.
	//The array should include 20 RTT.

void Test::print_kofidensintervall(long *rtts){
	float average = 0;
	for(int i = 0;i<20;i++){
		average=average + rtts[i];
	}
	average = average/20;
	double left = average-((2.0860*get_varians(rtts,average))/19);
	double right = average+((2.0860*get_varians(rtts,average))/19);
	std::cout << "\nKoefidensintervall: left: "<< left << " right: " << right << std::endl;
}

double Test::get_varians(long *array,float average){
	float a = 0;
	for(int i = 0; i<20;i++){
		a = a + (array[i]-average)*(array[i]-average);
	}
	float b = a/19;
	double ret = sqrt(b);
	return ret;
}
