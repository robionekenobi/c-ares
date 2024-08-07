/* MIT License
 *
 * Copyright (c) The c-ares project and its contributors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * SPDX-License-Identifier: MIT
 */

// Include ares internal file for DNS protocol details
#include "ares_setup.h"
#include "ares.h"
#include "ares_dns.h"
#include "dns-proto.h"

#include <stdio.h>
#include <stdlib.h>

#include <sstream>
#include <algorithm>

#if defined(_WIN32) && !defined(strcasecmp)
#  define strcasecmp(a,b) stricmp(a,b)
#endif

void arestest_strtolower(char *dest, const char *src, size_t dest_size)
{
  size_t len;

  if (dest == NULL)
    return;

  memset(dest, 0, dest_size);

  if (src == NULL)
    return;

  len = strlen(src);
  if (len >= dest_size)
    return;

  for (size_t i = 0; i<len; i++) {
    dest[i] = (char)tolower(src[i]);
  }
}


namespace ares {

std::string HexDump(std::vector<byte> data) {
  std::stringstream ss;
  for (size_t ii = 0; ii < data.size();  ii++) {
    char buffer[2 + 1];
    snprintf(buffer, sizeof(buffer), "%02x", data[ii]);
    ss << buffer;
  }
  return ss.str();
}

std::string HexDump(const byte *data, int len) {
  return HexDump(std::vector<byte>(data, data + len));
}

std::string HexDump(const char *data, int len) {
  return HexDump(reinterpret_cast<const byte*>(data), len);
}

std::string StatusToString(int status) {
  switch (status) {
  case ARES_SUCCESS: return "ARES_SUCCESS";
  case ARES_ENODATA: return "ARES_ENODATA";
  case ARES_EFORMERR: return "ARES_EFORMERR";
  case ARES_ESERVFAIL: return "ARES_ESERVFAIL";
  case ARES_ENOTFOUND: return "ARES_ENOTFOUND";
  case ARES_ENOTIMP: return "ARES_ENOTIMP";
  case ARES_EREFUSED: return "ARES_EREFUSED";
  case ARES_EBADQUERY: return "ARES_EBADQUERY";
  case ARES_EBADNAME: return "ARES_EBADNAME";
  case ARES_EBADFAMILY: return "ARES_EBADFAMILY";
  case ARES_EBADRESP: return "ARES_EBADRESP";
  case ARES_ECONNREFUSED: return "ARES_ECONNREFUSED";
  case ARES_ETIMEOUT: return "ARES_ETIMEOUT";
  case ARES_EOF: return "ARES_EOF";
  case ARES_EFILE: return "ARES_EFILE";
  case ARES_ENOMEM: return "ARES_ENOMEM";
  case ARES_EDESTRUCTION: return "ARES_EDESTRUCTION";
  case ARES_EBADSTR: return "ARES_EBADSTR";
  case ARES_EBADFLAGS: return "ARES_EBADFLAGS";
  case ARES_ENONAME: return "ARES_ENONAME";
  case ARES_EBADHINTS: return "ARES_EBADHINTS";
  case ARES_ENOTINITIALIZED: return "ARES_ENOTINITIALIZED";
  case ARES_ELOADIPHLPAPI: return "ARES_ELOADIPHLPAPI";
  case ARES_EADDRGETNETWORKPARAMS: return "ARES_EADDRGETNETWORKPARAMS";
  case ARES_ECANCELLED: return "ARES_ECANCELLED";
  default: return "UNKNOWN";
  }
}

std::string RcodeToString(int rcode) {
  switch (rcode) {
  case NOERROR: return "NOERROR";
  case FORMERR: return "FORMERR";
  case SERVFAIL: return "SERVFAIL";
  case NXDOMAIN: return "NXDOMAIN";
  case NOTIMP: return "NOTIMP";
  case REFUSED: return "REFUSED";
  case YXDOMAIN: return "YXDOMAIN";
  case YXRRSET: return "YXRRSET";
  case NXRRSET: return "NXRRSET";
  case NOTAUTH: return "NOTAUTH";
  case NOTZONE: return "NOTZONE";
  case TSIG_BADSIG: return "BADSIG";
  case TSIG_BADKEY: return "BADKEY";
  case TSIG_BADTIME: return "BADTIME";
  default: return "UNKNOWN";
  }
}

std::string RRTypeToString(int rrtype) {
  switch (rrtype) {
  case T_A: return "A";
  case T_NS: return "NS";
  case T_MD: return "MD";
  case T_MF: return "MF";
  case T_CNAME: return "CNAME";
  case T_SOA: return "SOA";
  case T_MB: return "MB";
  case T_MG: return "MG";
  case T_MR: return "MR";
  case T_NULL: return "NULL";
  case T_WKS: return "WKS";
  case T_PTR: return "PTR";
  case T_HINFO: return "HINFO";
  case T_MINFO: return "MINFO";
  case T_MX: return "MX";
  case T_TXT: return "TXT";
  case T_RP: return "RP";
  case T_AFSDB: return "AFSDB";
  case T_X25: return "X25";
  case T_ISDN: return "ISDN";
  case T_RT: return "RT";
  case T_NSAP: return "NSAP";
  case T_NSAP_PTR: return "NSAP_PTR";
  case T_SIG: return "SIG";
  case T_KEY: return "KEY";
  case T_PX: return "PX";
  case T_GPOS: return "GPOS";
  case T_AAAA: return "AAAA";
  case T_LOC: return "LOC";
  case T_NXT: return "NXT";
  case T_EID: return "EID";
  case T_NIMLOC: return "NIMLOC";
  case T_SRV: return "SRV";
  case T_ATMA: return "ATMA";
  case T_NAPTR: return "NAPTR";
  case T_KX: return "KX";
  case T_CERT: return "CERT";
  case T_A6: return "A6";
  case T_DNAME: return "DNAME";
  case T_SINK: return "SINK";
  case T_OPT: return "OPT";
  case T_APL: return "APL";
  case T_DS: return "DS";
  case T_SSHFP: return "SSHFP";
  case T_RRSIG: return "RRSIG";
  case T_NSEC: return "NSEC";
  case T_DNSKEY: return "DNSKEY";
  case T_TKEY: return "TKEY";
  case T_TSIG: return "TSIG";
  case T_IXFR: return "IXFR";
  case T_AXFR: return "AXFR";
  case T_MAILB: return "MAILB";
  case T_MAILA: return "MAILA";
  case T_ANY: return "ANY";
  case T_URI: return "URI";
  case T_MAX: return "MAX";
  default: return "UNKNOWN";
  }
}

std::string ClassToString(int qclass) {
  switch (qclass) {
  case C_IN: return "IN";
  case C_CHAOS: return "CHAOS";
  case C_HS: return "HESIOD";
  case C_NONE: return "NONE";
  case C_ANY: return "ANY";
  default: return "UNKNOWN";
  }
}

std::string AddressToString(const void* vaddr, int len) {
  const byte* addr = reinterpret_cast<const byte*>(vaddr);
  std::stringstream ss;
  if (len == 4) {
    char buffer[4*4 + 3 + 1];
    snprintf(buffer, sizeof(buffer), "%u.%u.%u.%u",
             (unsigned char)addr[0],
             (unsigned char)addr[1],
             (unsigned char)addr[2],
             (unsigned char)addr[3]);
    ss << buffer;
  } else if (len == 16) {
    for (int ii = 0; ii < 16;  ii+=2) {
      if (ii > 0) ss << ':';
      char buffer[4 + 1];
      snprintf(buffer, sizeof(buffer), "%02x%02x", (unsigned char)addr[ii], (unsigned char)addr[ii+1]);
      ss << buffer;
    }
  } else {
    ss << "!" << HexDump(addr, len) << "!";
  }
  return ss.str();
}

std::string PacketToString(const std::vector<byte>& packet) {
  const byte* data = packet.data();
  int len = (int)packet.size();
  std::stringstream ss;
  if (len < NS_HFIXEDSZ) {
    ss << "(too short, len " << len << ")";
    return ss.str();
  }
  ss << ((DNS_HEADER_QR(data) == 0) ? "REQ " : "RSP ");
  switch (DNS_HEADER_OPCODE(data)) {
  case O_QUERY: ss << "QRY "; break;
  case O_IQUERY: ss << "IQRY "; break;
  case O_STATUS: ss << "STATUS "; break;
  case O_NOTIFY: ss << "NOTIFY "; break;
  case O_UPDATE: ss << "UPDATE "; break;
  default: ss << "UNKNOWN(" << DNS_HEADER_OPCODE(data) << ") "; break;
  }
  if (DNS_HEADER_AA(data)) ss << "AA ";
  if (DNS_HEADER_TC(data)) ss << "TC ";
  if (DNS_HEADER_RD(data)) ss << "RD ";
  if (DNS_HEADER_RA(data)) ss << "RA ";
  if (DNS_HEADER_Z(data)) ss << "Z ";
  if (DNS_HEADER_QR(data) == 1) ss << RcodeToString(DNS_HEADER_RCODE(data));

  int nquestions = DNS_HEADER_QDCOUNT(data);
  int nanswers = DNS_HEADER_ANCOUNT(data);
  int nauths = DNS_HEADER_NSCOUNT(data);
  int nadds = DNS_HEADER_ARCOUNT(data);

  const byte* pq = data + NS_HFIXEDSZ;
  len -= NS_HFIXEDSZ;
  for (int ii = 0; ii < nquestions; ii++) {
    ss << " Q:" << QuestionToString(packet, &pq, &len);
  }
  const byte* prr = pq;
  for (int ii = 0; ii < nanswers; ii++) {
    ss << " A:" << RRToString(packet, &prr, &len);
  }
  for (int ii = 0; ii < nauths; ii++) {
    ss << " AUTH:" << RRToString(packet, &prr, &len);
  }
  for (int ii = 0; ii < nadds; ii++) {
    ss << " ADD:" << RRToString(packet, &prr, &len);
  }
  return ss.str();
}

std::string QuestionToString(const std::vector<byte>& packet,
                             const byte** data, int* len) {
  std::stringstream ss;
  ss << "{";
  if (*len < NS_QFIXEDSZ) {
    ss << "(too short, len " << *len << ")";
    return ss.str();
  }

  char *name = nullptr;
  long enclen;
  int rc = ares_expand_name(*data, packet.data(), (int)packet.size(), &name, &enclen);
  if (rc != ARES_SUCCESS) {
    ss << "(error from ares_expand_name)";
    return ss.str();
  }
  if (enclen > *len) {
    ss << "(error, encoded name len " << enclen << "bigger than remaining data " << *len << " bytes)";
    return ss.str();
  }
  *len -= (int)enclen;
  *data += enclen;

  // DNS 0x20 may mix case, output as all lower for checks as the mixed case
  // is really more of an internal thing
  char lowername[256];
  arestest_strtolower(lowername, name, sizeof(lowername));
  ares_free_string(name);

  ss << "'" << lowername << "' ";
  if (*len < NS_QFIXEDSZ) {
    ss << "(too short, len left " << *len << ")";
    return ss.str();
  }
  ss << ClassToString(DNS_QUESTION_CLASS(*data)) << " ";
  ss << RRTypeToString(DNS_QUESTION_TYPE(*data));
  *data += NS_QFIXEDSZ;
  *len -= NS_QFIXEDSZ;
  ss << "}";
  return ss.str();
}

std::string RRToString(const std::vector<byte>& packet,
                       const byte** data, int* len) {
  std::stringstream ss;
  ss << "{";
  if (*len < NS_RRFIXEDSZ) {
    ss << "too short, len " << *len << ")";
    return ss.str();
  }

  char *name = nullptr;
  long enclen;
  int rc = ares_expand_name(*data, packet.data(), (int)packet.size(), &name, &enclen);
  if (rc != ARES_SUCCESS) {
    ss << "(error from ares_expand_name)";
    return ss.str();
  }
  if (enclen > *len) {
    ss << "(error, encoded name len " << enclen << "bigger than remaining data " << *len << " bytes)";
    return ss.str();
  }
  *len -= (int)enclen;
  *data += enclen;
  ss << "'" << name << "' ";
  ares_free_string(name);
  name = nullptr;

  if (*len < NS_RRFIXEDSZ) {
    ss << "(too short, len left " << *len << ")";
    return ss.str();
  }
  int rrtype = DNS_RR_TYPE(*data);
  if (rrtype == T_OPT) {
    ss << "MAXUDP=" << DNS_RR_CLASS(*data) << " ";
    ss << RRTypeToString(rrtype) << " ";
    ss << "RCODE2=" << DNS_RR_TTL(*data);
  } else {
    ss << ClassToString(DNS_RR_CLASS(*data)) << " ";
    ss << RRTypeToString(rrtype) << " ";
    ss << "TTL=" << DNS_RR_TTL(*data);
  }
  int rdatalen = DNS_RR_LEN(*data);

  *data += NS_RRFIXEDSZ;
  *len -= NS_RRFIXEDSZ;
  if (*len < rdatalen) {
    ss << "(RR too long at " << rdatalen << ", len left " << *len << ")";
  } else {
    switch (rrtype) {
    case T_A:
    case T_AAAA:
      ss << " " << AddressToString(*data, rdatalen);
      break;
    case T_TXT: {
      const byte* p = *data;
      while (p < (*data + rdatalen)) {
        int tlen = *p++;
        if ((p + tlen) <= (*data + rdatalen)) {
          std::string txt(p, p + tlen);
          ss << " " << tlen << ":'" << txt << "'";
        } else {
          ss << "(string too long)";
        }
        p += tlen;
      }
      break;
    }
    case T_CNAME:
    case T_NS:
    case T_PTR: {
      rc = ares_expand_name(*data, packet.data(), (int)packet.size(), &name, &enclen);
      if (rc != ARES_SUCCESS) {
        ss << "(error from ares_expand_name)";
        break;
      }
      ss << " '" << name << "'";
      ares_free_string(name);
      break;
    }
    case T_MX:
      if (rdatalen > 2) {
        rc = ares_expand_name(*data + 2, packet.data(), (int)packet.size(), &name, &enclen);
        if (rc != ARES_SUCCESS) {
          ss << "(error from ares_expand_name)";
          break;
        }
        ss << " " << DNS__16BIT(*data) << " '" << name << "'";
        ares_free_string(name);
      } else {
        ss << "(RR too short)";
      }
      break;
    case T_SRV: {
      if (rdatalen > 6) {
        const byte* p = *data;
        unsigned long prio = DNS__16BIT(p);
        unsigned long weight = DNS__16BIT(p + 2);
        unsigned long port = DNS__16BIT(p + 4);
        p += 6;
        rc = ares_expand_name(p, packet.data(), (int)packet.size(), &name, &enclen);
        if (rc != ARES_SUCCESS) {
          ss << "(error from ares_expand_name)";
          break;
        }
        ss << prio << " " << weight << " " << port << " '" << name << "'";
        ares_free_string(name);
      } else {
        ss << "(RR too short)";
      }
      break;
    }
    case T_URI: {
      if (rdatalen > 4) {
        const byte* p = *data;
        unsigned long prio = DNS__16BIT(p);
        unsigned long weight = DNS__16BIT(p + 2);
        p += 4;
        std::string uri(p, p + (rdatalen - 4));
        ss << prio << " " << weight << " '" << uri << "'";
      } else {
        ss << "(RR too short)";
      }
      break;
    }
    case T_SOA: {
      const byte* p = *data;
      rc = ares_expand_name(p, packet.data(), (int)packet.size(), &name, &enclen);
      if (rc != ARES_SUCCESS) {
        ss << "(error from ares_expand_name)";
        break;
      }
      ss << " '" << name << "'";
      ares_free_string(name);
      p += enclen;
      rc = ares_expand_name(p, packet.data(), (int)packet.size(), &name, &enclen);
      if (rc != ARES_SUCCESS) {
        ss << "(error from ares_expand_name)";
        break;
      }
      ss << " '" << name << "'";
      ares_free_string(name);
      p += enclen;
      if ((p + 20) <= (*data + rdatalen)) {
        unsigned long serial = DNS__32BIT(p);
        unsigned long refresh = DNS__32BIT(p + 4);
        unsigned long retry = DNS__32BIT(p + 8);
        unsigned long expire = DNS__32BIT(p + 12);
        unsigned long minimum = DNS__32BIT(p + 16);
        ss << " " << serial << " " << refresh << " " << retry << " " << expire << " " << minimum;
      } else {
        ss << "(RR too short)";
      }
      break;
    }
    case T_NAPTR: {
      if (rdatalen > 7) {
        const byte* p = *data;
        unsigned long order = DNS__16BIT(p);
        unsigned long pref = DNS__16BIT(p + 2);
        p += 4;
        ss << order << " " << pref;

        int nlen = *p++;
        std::string flags(p, p + nlen);
        ss << " " << flags;
        p += nlen;

        nlen = *p++;
        std::string service(p, p + nlen);
        ss << " '" << service << "'";
        p += nlen;

        nlen = *p++;
        std::string regexp(p, p + nlen);
        ss << " '" << regexp << "'";
        p += nlen;

        rc = ares_expand_name(p, packet.data(), (int)packet.size(), &name, &enclen);
        if (rc != ARES_SUCCESS) {
          ss << "(error from ares_expand_name)";
          break;
        }
        ss << " '" << name << "'";
        ares_free_string(name);
      } else {
        ss << "(RR too short)";
      }
      break;
    }
    default:
      ss << " " << HexDump(*data, rdatalen);
      break;
    }
  }
  *data += rdatalen;
  *len -= rdatalen;

  ss << "}";
  return ss.str();
}

void PushInt32(std::vector<byte>* data, int value) {
  data->push_back((byte)(((unsigned int)value & 0xff000000) >> 24));
  data->push_back((byte)(((unsigned int)value & 0x00ff0000) >> 16));
  data->push_back((byte)(((unsigned int)value & 0x0000ff00) >> 8));
  data->push_back((byte)(value & 0x000000ff));
}

void PushInt16(std::vector<byte>* data, int value) {
  data->push_back((byte)((value & 0xff00) >> 8));
  data->push_back((byte)value & 0x00ff);
}

std::vector<byte> EncodeString(const std::string &name) {
  std::vector<byte> data;
  std::stringstream ss(name);
  std::string label;
  // TODO: cope with escapes
  while (std::getline(ss, label, '.')) {
    /* Label length of 0 indicates the end, and we always push an end
     * terminator, so don't do it twice */
    if (label.length() == 0)
      break;
    data.push_back((byte)label.length());
    data.insert(data.end(), label.begin(), label.end());
  }
  data.push_back(0);
  return data;
}

std::vector<byte> DNSQuestion::data(const char *request_name, const ares_dns_record_t *dnsrec) const {
  std::vector<byte> data;
  std::vector<byte> encname;
  if (request_name != nullptr && strcasecmp(request_name, name_.c_str()) == 0) {
    encname = EncodeString(request_name);
  } else {
    encname = EncodeString(name_);
  }
  data.insert(data.end(), encname.begin(), encname.end());
  PushInt16(&data, rrtype_);
  PushInt16(&data, qclass_);
  return data;
}

std::vector<byte> DNSRR::data(const ares_dns_record_t *dnsrec) const {
  std::vector<byte> data = DNSQuestion::data(dnsrec);
  PushInt32(&data, ttl_);
  return data;
}

std::vector<byte> DNSSingleNameRR::data(const ares_dns_record_t *dnsrec) const {
  std::vector<byte> data = DNSRR::data(dnsrec);
  std::vector<byte> encname = EncodeString(other_);
  int len = (int)encname.size();
  PushInt16(&data, len);
  data.insert(data.end(), encname.begin(), encname.end());
  return data;
}

std::vector<byte> DNSTxtRR::data(const ares_dns_record_t *dnsrec) const {
  std::vector<byte> data = DNSRR::data(dnsrec);
  int len = 0;
  for (const std::string& txt : txt_) {
    len += (1 + (int)txt.size());
  }
  PushInt16(&data, len);
  for (const std::string& txt : txt_) {
    data.push_back((byte)txt.size());
    data.insert(data.end(), txt.begin(), txt.end());
  }
  return data;
}

std::vector<byte> DNSMxRR::data(const ares_dns_record_t *dnsrec) const {
  std::vector<byte> data = DNSRR::data(dnsrec);
  std::vector<byte> encname = EncodeString(other_);
  int len = 2 + (int)encname.size();
  PushInt16(&data, len);
  PushInt16(&data, pref_);
  data.insert(data.end(), encname.begin(), encname.end());
  return data;
}

std::vector<byte> DNSSrvRR::data(const ares_dns_record_t *dnsrec) const {
  std::vector<byte> data = DNSRR::data(dnsrec);
  std::vector<byte> encname = EncodeString(target_);
  int len = 6 + (int)encname.size();
  PushInt16(&data, len);
  PushInt16(&data, prio_);
  PushInt16(&data, weight_);
  PushInt16(&data, port_);
  data.insert(data.end(), encname.begin(), encname.end());
  return data;
}

std::vector<byte> DNSUriRR::data(const ares_dns_record_t *dnsrec) const {
  std::vector<byte> data = DNSRR::data(dnsrec);
  int len = 4 + (int)target_.size();
  PushInt16(&data, len);
  PushInt16(&data, prio_);
  PushInt16(&data, weight_);
  data.insert(data.end(), target_.begin(), target_.end());
  return data;
}

std::vector<byte> DNSAddressRR::data(const ares_dns_record_t *dnsrec) const {
  std::vector<byte> data = DNSRR::data(dnsrec);
  int len = (int)addr_.size();
  PushInt16(&data, len);
  data.insert(data.end(), addr_.begin(), addr_.end());
  return data;
}

std::vector<byte> DNSSoaRR::data(const ares_dns_record_t *dnsrec) const {
  std::vector<byte> data = DNSRR::data(dnsrec);
  std::vector<byte> encname1 = EncodeString(nsname_);
  std::vector<byte> encname2 = EncodeString(rname_);
  int len = (int)encname1.size() + (int)encname2.size() + 5*4;
  PushInt16(&data, len);
  data.insert(data.end(), encname1.begin(), encname1.end());
  data.insert(data.end(), encname2.begin(), encname2.end());
  PushInt32(&data, serial_);
  PushInt32(&data, refresh_);
  PushInt32(&data, retry_);
  PushInt32(&data, expire_);
  PushInt32(&data, minimum_);
  return data;
}

const ares_dns_rr_t *fetch_rr_opt(const ares_dns_record_t *rec)
{
  size_t i;
  for (i = 0; i < ares_dns_record_rr_cnt(rec, ARES_SECTION_ADDITIONAL); i++) {
    const ares_dns_rr_t *rr =
      ares_dns_record_rr_get_const(rec, ARES_SECTION_ADDITIONAL, i);

    if (ares_dns_rr_get_type(rr) == ARES_REC_TYPE_OPT) {
      return rr;
    }
  }
  return NULL;
}

std::vector<byte> DNSOptRR::data(const ares_dns_record_t *dnsrec) const {
  std::vector<byte>    data = DNSRR::data(dnsrec);
  int len                   = 0;
  std::vector<byte>    cookie;
  const ares_dns_rr_t *rr  = fetch_rr_opt(dnsrec);
  size_t               passed_cookie_len = 0;
  const unsigned char *passed_cookie = NULL;

  ares_dns_rr_get_opt_byid(rr, ARES_RR_OPT_OPTIONS, ARES_OPT_PARAM_COOKIE,
                           &passed_cookie, &passed_cookie_len);

  /* Error out if we expected a server cookie but didn't get one, or if the
   * passed in server cookie doesn't match our expected value */
  if (expect_server_cookie_ &&
      (passed_cookie_len <= 8 ||
       passed_cookie_len - 8 != server_cookie_.size() ||
       memcmp(passed_cookie + 8, server_cookie_.data(), server_cookie_.size()) != 0
      )
     ) {
    data.clear();
    return data;
  }

  /* See if we should be applying a server cookie */
  if (server_cookie_.size() && passed_cookie_len >= 8) {
    /* If client cookie was provided to test framework, we are overwriting
     * the one received from the client.  This is likely to test failure
     * scenarios */
    if (client_cookie_.size()) {
      cookie.insert(cookie.end(), client_cookie_.begin(), client_cookie_.end());
    } else {
      cookie.insert(cookie.end(), passed_cookie, passed_cookie+8);
    }
    cookie.insert(cookie.end(), server_cookie_.begin(), server_cookie_.end());
  }

  if (cookie.size()) {
    len += 4 + (int)cookie.size();
  }
  for (const DNSOption& opt : opts_) {
    len += (4 + (int)opt.data_.size());
  }

  PushInt16(&data, len);
  for (const DNSOption& opt : opts_) {
    PushInt16(&data, opt.code_);
    PushInt16(&data, (int)opt.data_.size());
    data.insert(data.end(), opt.data_.begin(), opt.data_.end());
  }

  if (cookie.size()) {
    PushInt16(&data, ARES_OPT_PARAM_COOKIE);
    PushInt16(&data, (int)cookie.size());
    data.insert(data.end(), cookie.begin(), cookie.end());
  }

  return data;
}

std::vector<byte> DNSNaptrRR::data(const ares_dns_record_t *dnsrec) const {
  std::vector<byte> data = DNSRR::data(dnsrec);
  std::vector<byte> encname = EncodeString(replacement_);
  int len = (4 + 1 + (int)flags_.size() + 1 + (int)service_.size() + 1 + (int)regexp_.size() + (int)encname.size());
  PushInt16(&data, len);
  PushInt16(&data, order_);
  PushInt16(&data, pref_);
  data.push_back((byte)flags_.size());
  data.insert(data.end(), flags_.begin(), flags_.end());
  data.push_back((byte)service_.size());
  data.insert(data.end(), service_.begin(), service_.end());
  data.push_back((byte)regexp_.size());
  data.insert(data.end(), regexp_.begin(), regexp_.end());
  data.insert(data.end(), encname.begin(), encname.end());
  return data;
}

std::vector<byte> DNSPacket::data(const char *request_name, const ares_dns_record_t *dnsrec) const {
  std::vector<byte> data;
  PushInt16(&data, qid_);
  byte b = 0x00;
  if (response_) b |= 0x80;
  b |= ((opcode_ & 0x0f) << 3);
  if (aa_) b |= 0x04;
  if (tc_) b |= 0x02;
  if (rd_) b |= 0x01;
  data.push_back(b);
  b = 0x00;
  if (ra_) b |= 0x80;
  if (z_) b |= 0x40;
  if (ad_) b |= 0x20;
  if (cd_) b |= 0x10;
  b |= (rcode_ & 0x0f);
  data.push_back(b);

  int count = (int)questions_.size();
  PushInt16(&data, count);
  count = (int)answers_.size();
  PushInt16(&data, count);
  count = (int)auths_.size();
  PushInt16(&data, count);
  count = (int)adds_.size();
  PushInt16(&data, count);

  for (const std::unique_ptr<DNSQuestion>& question : questions_) {
    std::vector<byte> qdata = question->data(request_name, dnsrec);
    if (qdata.size() == 0) {
      data.clear();
      return data;
    }
    data.insert(data.end(), qdata.begin(), qdata.end());
  }
  for (const std::unique_ptr<DNSRR>& rr : answers_) {
    std::vector<byte> rrdata = rr->data(dnsrec);
    if (rrdata.size() == 0) {
      data.clear();
      return data;
    }
    data.insert(data.end(), rrdata.begin(), rrdata.end());
  }
  for (const std::unique_ptr<DNSRR>& rr : auths_) {
    std::vector<byte> rrdata = rr->data(dnsrec);
    if (rrdata.size() == 0) {
      data.clear();
      return data;
    }
    data.insert(data.end(), rrdata.begin(), rrdata.end());
  }
  for (const std::unique_ptr<DNSRR>& rr : adds_) {
    std::vector<byte> rrdata = rr->data(dnsrec);
    if (rrdata.size() == 0) {
      data.clear();
      return data;
    }
    data.insert(data.end(), rrdata.begin(), rrdata.end());
  }
  return data;
}

}  // namespace ares
