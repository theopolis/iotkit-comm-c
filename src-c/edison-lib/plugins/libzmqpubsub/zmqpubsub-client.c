/*
 * zmqpubsub-client.c
 *
 *  Created on: 2014-04-18
 *      Author: skothurx
 */

#include <stdio.h>
#include <zmq.h>
#include <zmq_utils.h>
#include "zmqpubsub-client.h"
#include "../inc/zhelpers.h"

#ifndef DEBUG
#define DEBUG 0
#endif

struct Holder {
	void *context;
	void *subscriber;
};

struct Holder zmqContainer;

int init(char *host, int port, char *type, void *sslargs) {
	#if DEBUG
		printf("In createClient\n");
	#endif
	zmqContainer.context = zmq_ctx_new();
	#if DEBUG
		printf("context initialised\n");
	#endif
    char addr[128];
    sprintf(addr, "tcp://%s:%d\n", host, port);
    #if DEBUG
        printf("going to connect %s\n",addr);
    #endif
    zmqContainer.subscriber = zmq_socket(zmqContainer.context,ZMQ_SUB);
    int rc = zmq_connect(zmqContainer.subscriber,addr);
    #if DEBUG
        printf("subscriber connected\n");
    #endif
    return rc;
}

int send(char *message,Context context) {
    #if DEBUG
    	printf("\nsending message: %s\n",message);
    #endif
}

int subscribe(char *topic) {
	#if DEBUG
		printf("\nsubscribing for topic: %s\n",topic);
	#endif
	//  Subscribe only for the topic
	int rc = zmq_setsockopt (zmqContainer.subscriber, ZMQ_SUBSCRIBE, topic, strlen(topic));
    return rc;
}

int receive(void (*handler)(char *, Context)) {
    #if DEBUG
        printf("In receive Waiting for the message\n");
    #endif
    //  Read message contents
    char *contents = s_recv (zmqContainer.subscriber);
    #if DEBUG
        printf ("Message Received: %s\n", contents);
    #endif
    Context context;
    context.name = "event";
    context.value = "message";
    #if DEBUG
        printf ("calling handler\n");
    #endif
    handler(contents,context);
    #if DEBUG
        printf ("\nfree the contents\n");
    #endif
    free (contents);

}

int unsubscribe(char *topic) {
	#if DEBUG
		printf("\nunsubscribing the topic: %s\n",topic);
	#endif
    int rc = zmq_setsockopt (zmqContainer.subscriber, ZMQ_UNSUBSCRIBE, topic, strlen(topic));
    return rc;
}

int done() {
	if (zmqContainer.subscriber != NULL) {
		zmq_close(zmqContainer.subscriber);
		zmqContainer.subscriber = NULL;
		#if DEBUG
			printf("subscriber freed\n");
		#endif
	}
	if (zmqContainer.context != NULL) {
		zmq_ctx_destroy(zmqContainer.context);
		zmqContainer.context = NULL;
		#if DEBUG
			printf("context freed\n");
		#endif
	}
	#if DEBUG
		printf("\nclosed\n");
	#endif
}
