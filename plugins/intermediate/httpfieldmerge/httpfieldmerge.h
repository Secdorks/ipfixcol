/*
 * \file httpfieldmerge.h
 * \author Kirc <kirc&secdorks.net>
 * \brief IPFIXcol 'httpfieldmerge' intermediate plugin.
 *
 * Intermediate plugin for IPFIXcol that merges HTTP-related fields from various vendors
 * into one unified set, such that analysis applications can always rely on the unified
 * set of fields. The following fields are currently supported:
 *
 *     - HTTP hostname
 *     - HTTP URL
 *     - HTTP user agent (UA)
 *
 * Specifically, this plugin performs only a single task:
 *
 *     - Replace the IE definitions of HTTP-related fields with those of the unified
 *          set of fields. As such, only templates are modified (and data records are
 *          not).
 *
 * HTTP-related fields from the following vendors are currently supported:
 *
 *     - Cisco,                 PEN: 9
 *     - Masaryk University,    PEN: 16982
 *     - INVEA-TECH,            PEN: 39499
 *     - ntop,                  PEN: 35632
 *
 * The unified set of fields uses PEN '44913'.
 *
 * Copyright (c) 2015 Secdorks.net
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of the Company nor the names of its contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * ALTERNATIVELY, provided that this notice is retained in full, this
 * product may be distributed under the terms of the GNU General Public
 * License (GPL) version 2 or later, in which case the provisions
 * of the GPL apply INSTEAD OF those given above.
 *
 * This software is provided ``as is, and any express or implied
 * warranties, including, but not limited to, the implied warranties of
 * merchantability and fitness for a particular purpose are disclaimed.
 * In no event shall the company or contributors be liable for any
 * direct, indirect, incidental, special, exemplary, or consequential
 * damages (including, but not limited to, procurement of substitute
 * goods or services; loss of use, data, or profits; or business
 * interruption) however caused and on any theory of liability, whether
 * in contract, strict liability, or tort (including negligence or
 * otherwise) arising in any way out of the use of this software, even
 * if advised of the possibility of such damage.
 *
 */

#ifndef HTTPFIELDMERGE_H_
#define HTTPFIELDMERGE_H_

#include <ipfixcol.h>

#include "uthash.h"

#define NFV9_CONVERSION_PEN     0xFFFFFFFF
#define TEMPL_MAX_LEN           100000

#define CISCO_PEN   9
#define INVEA_PEN   39499
#define MASARYK_PEN 16982
#define NTOP_PEN    35632
#define RS_PEN      44913
#define TARGET_PEN  RS_PEN

/* IPFIX Information Elements used within this plugin (PEN, ID) */
/* Cisco uses multiple instances (4) of field e9id12235 for exporting
   HTTP-related information, always in the following order:
        - Instance 1: URL
        - Instance 2: hostname
        - Instance 3: user agent string
        - Instance 4: unknown?
 */
#define ciscoHttpHost           { CISCO_PEN, 12235 }
#define ciscoHttpUrl            { CISCO_PEN, 12235 }
#define ciscoHttpUserAgent      { CISCO_PEN, 12235 }
#define ciscoHttpUnknown        { CISCO_PEN, 12235 }
#define cisco_field_count       4

#define inveaHttpHost           { INVEA_PEN,  1 }
#define inveaHttpUrl            { INVEA_PEN,  2 }
#define inveaHttpUserAgent      { INVEA_PEN, 20 }
#define invea_field_count       3

#define masarykHttpHost         { MASARYK_PEN, 501 }
#define masarykHttpUrl          { MASARYK_PEN, 502 }
#define masarykHttpUserAgent    { MASARYK_PEN, 504 }
#define masaryk_field_count     3

#define ntopHttpHost            { NTOP_PEN, 187 }
#define ntopHttpUrl             { NTOP_PEN, 180 }
#define ntopHttpUserAgent       { NTOP_PEN, 183 }
#define ntop_field_count        3

#define ntopHttpHostv9          { NFV9_CONVERSION_PEN, 24891 } /* Original ID: 57659 */
#define ntopHttpUrlv9           { NFV9_CONVERSION_PEN, 24884 } /* Original ID: 57652 */
#define ntopHttpUserAgentv9     { NFV9_CONVERSION_PEN, 24887 } /* Original ID: 57655 */
// #define ntop_field_count        3 (already defined above)

#define rsHttpHost              { RS_PEN, 20 }
#define rsHttpUrl               { RS_PEN, 21 }
#define rsHttpUserAgent         { RS_PEN, 22 }
#define rs_field_count          3

#define targetHttpHost          { TARGET_PEN, 20 }
#define targetHttpUrl           { TARGET_PEN, 21 }
#define targetHttpUserAgent     { TARGET_PEN, 22 }

struct ipfix_entity {
    uint32_t pen;
    uint16_t element_id;
};

static struct ipfix_entity cisco_fields[] = {
    ciscoHttpHost, ciscoHttpUrl, ciscoHttpUserAgent, ciscoHttpUnknown
};
static struct ipfix_entity invea_fields[] = {
    inveaHttpHost, inveaHttpUrl, inveaHttpUserAgent
};
static struct ipfix_entity masaryk_fields[] = {
    masarykHttpHost, masarykHttpUrl, masarykHttpUserAgent
};
static struct ipfix_entity ntop_fields[] = {
    ntopHttpHost, ntopHttpUrl, ntopHttpUserAgent
};
static struct ipfix_entity ntopv9_fields[] = {
    ntopHttpHostv9, ntopHttpUrlv9, ntopHttpUserAgentv9
};
static struct ipfix_entity rs_fields[] = {
    rsHttpHost, rsHttpUrl, rsHttpUserAgent
};

#define vendor_fields_count 3

struct field_mapping {
    struct ipfix_entity from;
    struct ipfix_entity to;
};

static struct field_mapping invea_field_mappings[] = {
    { inveaHttpHost,        targetHttpHost },
    { inveaHttpUrl,         targetHttpUrl },
    { inveaHttpUserAgent,   targetHttpUserAgent }
};
static struct field_mapping masaryk_field_mappings[] = {
    { masarykHttpHost,      targetHttpHost },
    { masarykHttpUrl,       targetHttpUrl },
    { masarykHttpUserAgent, targetHttpUserAgent }
};
static struct field_mapping ntop_field_mappings[] = {
    { ntopHttpHost,         targetHttpHost },
    { ntopHttpUrl,          targetHttpUrl },
    { ntopHttpUserAgent,    targetHttpUserAgent }
};
static struct field_mapping ntopv9_field_mappings[] = {
    { ntopHttpHostv9,       targetHttpHost },
    { ntopHttpUrlv9,        targetHttpUrl },
    { ntopHttpUserAgentv9,  targetHttpUserAgent }
};
static struct field_mapping rs_field_mappings[] = {
    { rsHttpHost,           targetHttpHost },
    { rsHttpUrl,            targetHttpUrl },
    { rsHttpUserAgent,      targetHttpUserAgent }
};

struct templ_stats_elem_t {
    int id;                         // Hash key
    uint32_t http_fields_pen;       // Exporter PEN in case template contains HTTP-related fields
    int http_fields_pen_determined; // Indicates whether the PEN belonging HTTP-related has been determined before
    UT_hash_handle hh;              // Hash handle for internal hash functioning
};

/* Stores plugin's internal configuration */
struct httpfieldmerge_config {
    char *params;
    void *ip_config;
    uint32_t ip_id;
    struct ipfix_template_mgr *tm;

    /*
     * Hashmap for storing the IP version used in every template by template ID. We
     * place this structure in proxy_config rather than proxy_processor, since
     * it should be persistent between various IPFIX messages (and proxy processor
     * is reset for every IPFIX message).
     */
    struct templ_stats_elem_t *templ_stats;
};

struct httpfieldmerge_processor {
    uint8_t *msg;
    uint16_t allocated_msg_length, offset;
    uint32_t length, odid;
    int type;
    
    struct httpfieldmerge_config *plugin_conf; // Pointer to proxy_config, such that we don't have to store some pointers twice
    struct ipfix_template_key *key; // Stores the key of a newly added template within the template manager
};

#endif /* HTTPFIELDMERGE_H_ */
