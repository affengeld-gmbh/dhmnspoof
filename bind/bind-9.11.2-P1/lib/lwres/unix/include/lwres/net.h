/*
 * Copyright (C) 2000-2002, 2004, 2005, 2007, 2016  Internet Systems Consortium, Inc. ("ISC")
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/* $Id: net.h,v 1.9 2007/06/19 23:47:23 tbox Exp $ */

#ifndef LWRES_NET_H
#define LWRES_NET_H 1

/*****
 ***** Module Info
 *****/

/*! \file net.h
 * This module is responsible for defining the following basic networking
 * types:
 *
 *\li		struct in_addr
 *\li		struct in6_addr
 *\li		struct sockaddr
 *\li		struct sockaddr_in
 *\li		struct sockaddr_in6
 *
 * It ensures that the AF_ and PF_ macros are defined.
 *
 * It declares ntoh[sl]() and hton[sl]().
 *
 * It declares lwres_net_aton(), lwres_net_ntop(), and lwres_net_pton().
 *
 * It ensures that #INADDR_LOOPBACK, #INADDR_ANY and #IN6ADDR_ANY_INIT
 * are defined.
 */

/***
 *** Imports.
 ***/

#include <lwres/platform.h>	/* Required for LWRES_PLATFORM_*. */

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>		/* Contractual promise. */
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/un.h>

#include <netinet/in.h>		/* Contractual promise. */
#include <arpa/inet.h>		/* Contractual promise. */
#ifdef LWRES_PLATFORM_NEEDNETINETIN6H
#include <netinet/in6.h>	/* Required on UnixWare. */
#endif
#ifdef LWRES_PLATFORM_NEEDNETINET6IN6H
#include <netinet6/in6.h>	/* Required on BSD/OS for in6_pktinfo. */
#endif
#include <net/if.h>

#include <lwres/lang.h>

#ifndef LWRES_PLATFORM_HAVEIPV6
#include <lwres/ipv6.h>		/* Contractual promise. */
#endif

#ifdef LWRES_PLATFORM_HAVEINADDR6
#define in6_addr in_addr6	/* Required for pre RFC2133 implementations. */
#endif

/*!
 * Required for some pre RFC2133 implementations.
 * IN6ADDR_ANY_INIT and IN6ADDR_LOOPBACK_INIT were added in
 * draft-ietf-ipngwg-bsd-api-04.txt or draft-ietf-ipngwg-bsd-api-05.txt.
 * If 's6_addr' is defined then assume that there is a union and three
 * levels otherwise assume two levels required.
 */
#ifndef IN6ADDR_ANY_INIT
#ifdef s6_addr
#define IN6ADDR_ANY_INIT { { { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 } } }
#else
#define IN6ADDR_ANY_INIT { { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 } }
#endif
#endif

/*!
 * Initialize address loopback.  See IN6ADDR_ANY_INIT
 */
#ifndef IN6ADDR_LOOPBACK_INIT
#ifdef s6_addr
#define IN6ADDR_LOOPBACK_INIT { { { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 } } }
#else
#define IN6ADDR_LOOPBACK_INIT { { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 } }
#endif
#endif

/*% Used by AI_ALL */
#ifndef AF_INET6
#define AF_INET6 99
#endif


/*% Used to return IPV6 address types. */
#ifndef PF_INET6
#define PF_INET6 AF_INET6
#endif

/*% inaddr Loopback */
#ifndef INADDR_LOOPBACK
#define INADDR_LOOPBACK 0x7f000001UL
#endif

LWRES_LANG_BEGINDECLS

const char *
lwres_net_ntop(int af, const void *src, char *dst, size_t size);

int
lwres_net_pton(int af, const char *src, void *dst);

int
lwres_net_aton(const char *cp, struct in_addr *addr);

LWRES_LANG_ENDDECLS

#endif /* LWRES_NET_H */