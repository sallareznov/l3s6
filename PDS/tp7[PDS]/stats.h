#ifndef _STAT_H_
#define _STAT_H_

struct stats_t {
	int nb_clients;
	int rcv_lines;
	int snt_lines;
	int max_clients;
	int max_rcv_lines_by_client;
	int max_snt_lines_to_client;
};

void init_stats();

void update_nb_clients(int connexion);

void update_rcv_lines();

void update_snt_lines();

void update_max_clients();

void update_max_rcv_lines_by_client();

void update_max_snt_lines_to_client();

void print_stats();

#endif