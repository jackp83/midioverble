/*
 * Test.h
 *
 *  Created on: 11 maj 2016
 *      Author: jack
 */

#ifndef TEST_H_
#define TEST_H_

#include <iostream>
#include <unistd.h>
#include <chrono>
#include <ctime>

#include "ble/BLESender.h"
#include "ble/BLEReceiver.h"

using namespace std;

class Test {
public:
	int run_tests();
	int run_tests(std::string remote_ble_addr,int number_of_packages_to_send,int sleep_between_packages_in_ms,int min_interval_start,int min_interval_stop,int max_interval_start,int max_interval_stop,int latency_start,int latency_stop,int timeout_start,int timeout_stop);
private:
	BLEReceiver ble_receiver;
	BLESender ble_sender;
	int conn_socket;
	std::chrono::time_point<std::chrono::system_clock> start,end;
	int conn_to_ble_dev(std::string remote_ble_addr);
	int listen_and_accept_conn();
	int send_and_recv_echo_msg(char* msg, int size,char* echo);
	int recv_and_send_echo_msg(char* msg, int size);
	int validate_parameters(int min_interval_start,int min_interval_stop,int max_interval_start,int max_interval_stop,int latency_start,int latency_stop,int timeout_start,int timeout_stop);
	int compare_arrays(char *array1,char *array2,int arraysize);
	double get_time_in_ms();
	void fill_array_with_random_characters(char *array,int arraysize);
	void start_clock();
	void stop_clock();
	void print_current_test_parameters(int min_interval,int max_interval,int latency,int timeout,int number_of_packages_to_send);
	void print_result(int success,int fail,double max_rtt,double min_rtt,double average_rtt);
	void print_end_results(double best_average_rtt,double best_average_rtt_min_interval,double best_average_rtt_max_interval,double best_average_rtt_latency,double best_min_rtt,double best_min_rtt_min_interval,double best_min_rtt_max_interval,double best_min_rtt_latency,double best_max_rtt,double best_max_rtt_min_interval,double best_max_rtt_max_interval,double best_max_rtt_latency);
	void print_kofidensintervall(long *rtts);
	double get_varians(long *array,float average);
};

#endif /* TEST_H_ */
