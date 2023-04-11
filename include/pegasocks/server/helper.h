#ifndef _PGS_HELPER_THREAD_H
#define _PGS_HELPER_THREAD_H

#include "event2/bufferevent.h"
#include <event2/util.h>
#include <stdint.h>

#include "manager.h"
#include "ssl.h"
#include "utils.h"
#include "local.h"

typedef void(pgs_timer_cb_t)(evutil_socket_t fd, short event, void *data);

typedef struct pgs_timer_s {
	struct event *ev;
	struct timeval tv;
	void *ctx;
} pgs_timer_t;

typedef struct pgs_helper_thread_s {
	uint32_t tid;
	struct event_base *base;
	struct evdns_base *dns_base;

	pgs_timer_t *log_timer;
	pgs_timer_t *ping_timer;
	struct event *ev_term;

	pgs_local_server_t fake_local;
} pgs_helper_thread_t;

typedef struct pgs_helper_thread_ctx_s {
	int cfd;
	pgs_config_t *config;
	pgs_logger_t *logger;
	pgs_server_manager_t *sm;
	pgs_ssl_ctx_t *ssl_ctx;

	void **helper_ref; /* use this to exit helper thread later */
} pgs_helper_thread_ctx_t;

pgs_helper_thread_t *pgs_helper_thread_new(int cfd, pgs_config_t *config,
					   pgs_logger_t *logger,
					   pgs_server_manager_t *sm,
					   pgs_ssl_ctx_t *ssl_ctx);
void pgs_helper_thread_free(pgs_helper_thread_t *ptr);

void *pgs_helper_thread_start(void *data);

pgs_timer_t *pgs_timer_init(int interval, pgs_timer_cb_t,
			    pgs_helper_thread_t *ptr);

/* this will ping remote servers and reinit ping timer */
void pgs_helper_ping_remote(pgs_helper_thread_t *);

void pgs_timer_destroy(pgs_timer_t *);

#endif
