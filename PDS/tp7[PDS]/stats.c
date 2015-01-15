#include <stdio.h>
#include "stats.h"
#include "tools.h"

#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)

static struct stats_t str_stats;
static int rl = 0;
static int sl = 0;

void init_stats() {
	str_stats.nb_clients = 0;
	str_stats.rcv_lines = 0;
	str_stats.snt_lines = 0;
	str_stats.max_clients = 0;
	str_stats.max_rcv_lines_by_client = 0;
	str_stats.max_snt_lines_to_client = 0;
}

void update_nb_clients(int connexion) {
	if (connexion == TRUE)
		str_stats.nb_clients++;
	else
		str_stats.nb_clients--;
}

void update_rcv_lines() {
	str_stats.rcv_lines += str_stats.nb_clients;
	rl = str_stats.rcv_lines - str_stats.snt_lines; 
}

void update_snt_lines() {
	str_stats.snt_lines++;
	sl++;
}

void update_max_clients() {
	if (str_stats.nb_clients > str_stats.max_clients)
		str_stats.max_clients = str_stats.nb_clients;
}

void update_max_rcv_lines_by_client() {
	if (rl > str_stats.max_rcv_lines_by_client)
		str_stats.max_rcv_lines_by_client = rl;
}

void update_max_snt_lines_to_client() {
	if (sl > str_stats.max_snt_lines_to_client)
		str_stats.max_snt_lines_to_client = sl;

}

void print_stats() {
	printf("nb_clients = %d\n",str_stats.nb_clients);
	printf("rcv_lines = %d\n",str_stats.rcv_lines);
	printf("snt_lines = %d\n",str_stats.snt_lines);
	printf("max_clients = %d\n",str_stats.max_clients);
	printf("max_rcv_lines_by_client = %d\n",str_stats.max_rcv_lines_by_client);
	printf("max_snt_lines_to_client = %d\n",str_stats.max_snt_lines_to_client);
}