/* os/socket.c - implementation of the Socket module. */

/* See http://riscose.sf.net/ for terms of distribution, and to
 * pick up a later version of the software.
 *
 * Created by defmod, riscose version 1.01. */

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#include "monty/monty.h"
#include "types.h"
#include "rom/rom.h"
#include "osmodule.h"
#include "socket.h"

static os_error *inet_error;

static int roerrno_from_host(int hosterr)
{
#define MAP(x) case x: return socket_##x
    switch (hosterr) {
        case 0:
            /* Success */
            return 0;
        MAP(EBADF);
        /* Ensure that EAGAIN is mapped to EWOULDBLOCK as that is the
         * error value actually used under RISC OS. */
        case EAGAIN:
            return socket_EWOULDBLOCK;
        MAP(EFAULT);
        MAP(EINVAL);
        /* Ensure we have a mapping from the host's EWOULDBLOCK, too
         * in the event that it's different from EAGAIN */
#if EWOULDBLOCK != EAGAIN
        MAP(EWOULDBLOCK);
#endif
        MAP(EINPROGRESS);
        MAP(EALREADY);
        MAP(ENOTSOCK);
        MAP(EDESTADDRREQ);
        MAP(EMSGSIZE);
        MAP(EPROTOTYPE);
        MAP(ENOPROTOOPT);
        MAP(EPROTONOSUPPORT);
        MAP(ESOCKTNOSUPPORT);
        MAP(EOPNOTSUPP);
        MAP(EPFNOSUPPORT);
        MAP(EAFNOSUPPORT);
        MAP(EADDRINUSE);
        MAP(EADDRNOTAVAIL);
        MAP(ENETDOWN);
        MAP(ENETUNREACH);
        MAP(ENETRESET);
        MAP(ECONNABORTED);
        MAP(ECONNRESET);
        MAP(ENOBUFS);
        MAP(EISCONN);
        MAP(ENOTCONN);
        MAP(ESHUTDOWN);
        MAP(ETOOMANYREFS);
        MAP(ETIMEDOUT);
        MAP(ECONNREFUSED);
        MAP(EHOSTDOWN);
        MAP(EHOSTUNREACH);
        default:
            /* Some other error we can't map: use EINVAL */
            return socket_EINVAL;
    }
#undef MAP
}

/* Populate inet_error from the host error provided */
static os_error *set_error(int hosterr)
{
/* Internet errors are offset from this error base: */
#define INET_ERRBASE 0x20E00
    inet_error->errnum = INET_ERRBASE + roerrno_from_host(hosterr);
    strncpy(inet_error->errmess, strerror(hosterr),
        sizeof(inet_error->errmess));
#undef INET_ERRBASE
    return inet_error;
}

/* ---- socket_swi_register_extra ----------------------------------- */

void socket_swi_register_extra(void)
{
    /* Nothing to register, but allocate error block */
    os_error *err = xosmodule_alloc(sizeof(*inet_error), (void **) &inet_error);
    assert(err == NULL && inet_error != NULL);
}

/* ---- xsocket_creat ----------------------------------------------- */

os_error *xsocket_creat(socket_af af,
    socket_sock sock,
    socket_ipproto proto,
    socket_s *s)
{
    int domain, type, protocol;
    int sockfd;

    //fprintf(stderr, "socket(%d, %d, %d)\n", af, sock, proto);

    if (af != socket_AF_INET)
        return set_error(EAFNOSUPPORT);

    domain = AF_INET;
    switch (sock) {
        case socket_SOCK_STREAM:
            type = SOCK_STREAM;
            break;
        case socket_SOCK_DGRAM:
            type = SOCK_DGRAM;
            break;
        case socket_SOCK_RAW:
            type = SOCK_RAW;
            break;
        default:
            return set_error(EPROTONOSUPPORT);
    }
    switch (proto) {
        case socket_IPPROTO_IP:
            protocol = IPPROTO_IP;
            break;
        case socket_IPPROTO_ICMP:
            protocol = IPPROTO_ICMP;
            break;
        case socket_IPPROTO_TCP:
            protocol = IPPROTO_TCP;
            break;
        case socket_IPPROTO_UDP:
            protocol = IPPROTO_UDP;
            break;
        case socket_IPPROTO_RAW:
            protocol = IPPROTO_RAW;
            break;
        default:
            return set_error(EPROTONOSUPPORT);
    }

    sockfd = socket(domain, type, protocol);
    if (sockfd == -1)
        return set_error(errno);
    *s = (socket_s) sockfd;

    //fprintf(stderr, "socket(%d, %d, %d) => %d\n", af, sock, proto, sockfd);

    return NULL;
}

/* ---- xsocket_bind ------------------------------------------------ */

os_error *xsocket_bind(socket_s s,
    socket_sockaddr *sockaddr,
    int32_t namelen)
{
    struct sockaddr_in addr;

    //fprintf(stderr, "bind(%d, %p, %d)\n", s, sockaddr, namelen);

    if (sockaddr == NULL || namelen != sizeof(*sockaddr))
        return set_error(EINVAL);

    /* sockaddr could be v4 or v5 format */
    if (sockaddr->sockaddr.af != socket_AF_INET &&
        (sockaddr->sockaddr.af < 0xff ||
        ((socket5_sockaddr *) sockaddr)->sockaddr.af != socket_AF_INET))
        return set_error(EINVAL);

    addr.sin_family = AF_INET;
    addr.sin_port = sockaddr->sockaddr_in.port;
    addr.sin_addr.s_addr = sockaddr->sockaddr_in.addr;

    //fprintf(stderr, "binding %d to %d.%d.%d.%d:%d\n", s,
    //    addr.sin_addr.s_addr & 0xff,
    //    (addr.sin_addr.s_addr >> 8) & 0xff,
    //    (addr.sin_addr.s_addr >> 16) & 0xff,
    //    (addr.sin_addr.s_addr >> 24) & 0xff,
    //    ntohs(addr.sin_port));

    if (bind(s, (struct sockaddr *) &addr, sizeof(addr)) == -1)
        return set_error(errno);

    return NULL;
}

/* ---- xsocket_listen ---------------------------------------------- */

os_error *xsocket_listen(socket_s s,
    int32_t backlog)
{
    //fprintf(stderr, "listen(%d, %d)\n", s, backlog);

    if (listen(s, backlog) == -1)
        return set_error(errno);

    return NULL;
}

/* ---- xsocket_accept ---------------------------------------------- */

os_error *xsocket_accept(socket_s s,
    socket_sockaddr *sockaddr,
    int32_t *addrlen,
    socket_s *ns)
{
    os_error *err;

    err = xsocket_accept_1(s, (socket5_sockaddr *) sockaddr, addrlen, ns);
    if (err == NULL) {
        socket5_af af = ((socket5_sockaddr *) sockaddr)->sockaddr.af;
        sockaddr->sockaddr_in.af = (socket_af) af;
    }

    return NULL;
}

/* ---- xsocket_connect --------------------------------------------- */

os_error *xsocket_connect(socket_s s,
    socket_sockaddr *sockaddr,
    int32_t namelen)
{
    struct sockaddr_in addr;

    //fprintf(stderr, "connect(%d, %p, %d)\n", s, sockaddr, namelen);

    if (sockaddr == NULL || namelen != sizeof(*sockaddr))
        return set_error(EINVAL);

    /* sockaddr could be v4 or v5 format */
    if (sockaddr->sockaddr.af != socket_AF_INET &&
        (sockaddr->sockaddr.af < 0xff ||
        ((socket5_sockaddr *) sockaddr)->sockaddr.af != socket_AF_INET))
        return set_error(EINVAL);

    addr.sin_family = AF_INET;
    addr.sin_port = sockaddr->sockaddr_in.port;
    addr.sin_addr.s_addr = sockaddr->sockaddr_in.addr;

    //fprintf(stderr, "connecting %d to %d.%d.%d.%d:%d\n", s,
    //    addr.sin_addr.s_addr & 0xff,
    //    (addr.sin_addr.s_addr >> 8) & 0xff,
    //    (addr.sin_addr.s_addr >> 16) & 0xff,
    //    (addr.sin_addr.s_addr >> 24) & 0xff,
    //    ntohs(addr.sin_port));

    if (connect(s, (struct sockaddr *) &addr, sizeof(addr)) == -1)
        return set_error(errno);

    return NULL;
}

/* ---- xsocket_recv ------------------------------------------------ */

os_error *xsocket_recv(socket_s s,
    byte *buf,
    int32_t len,
    socket_msg_flags flags,
    int32_t *cc)
{
    int fl = 0;
    ssize_t received;

    //fprintf(stderr, "recv(%d, %p, %d, %x)\n", s, buf, len, flags);

    if (flags & socket_MSG_OOB)
        fl |= MSG_OOB;
    if (flags & socket_MSG_PEEK)
        fl |= MSG_PEEK;
    if (flags & socket_MSG_DONTROUTE)
        fl |= MSG_DONTROUTE;

    received = recv(s, (void *) buf, len, fl);
    if (received == -1)
        return set_error(errno);

    *cc = received;

    return NULL;
}

/* ---- xsocket_recvfrom -------------------------------------------- */

os_error *xsocket_recvfrom(socket_s s,
    byte *buf,
    int32_t len,
    socket_msg_flags flags,
    socket_sockaddr *from,
    int32_t *fromlen,
    int32_t *cc)
{
    os_error *err;

    err = xsocket_recvfrom_1(s, buf, len, flags,
        (socket5_sockaddr *) from, fromlen, cc);
    if (err == NULL) {
        socket5_af af = ((socket5_sockaddr *) from)->sockaddr.af;
        from->sockaddr_in.af = (socket_af) af;
    }

    return err;
}

/* ---- xsocket_recvmsg --------------------------------------------- */

os_error *xsocket_recvmsg(socket_s s,
    socket_msghdr *msg,
    socket_msg_flags flags,
    int32_t *cc)
{
    socket5_msghdr hdr;
    os_error *err;

    err = xsocket_recvmsg_1(s, &hdr, flags, cc);
    if (err == NULL) {
        msg->name = hdr.name;
        msg->namelen = hdr.namelen;
        msg->iov = hdr.iov;
        msg->iov_count = hdr.iov_count;
        msg->accrights = hdr.control;
        msg->accrightslen = hdr.controllen;
    }

    return err;
}

/* ---- xsocket_send ------------------------------------------------ */

os_error *xsocket_send(socket_s s,
    byte *msg,
    int32_t len,
    socket_msg_flags flags,
    int32_t *cc)
{
    int fl = 0;
    ssize_t sent;

    //fprintf(stderr, "send(%d, %p, %d, %x)\n", s, msg, len, flags);

    if (flags & socket_MSG_OOB)
        fl |= MSG_OOB;
    if (flags & socket_MSG_PEEK)
        fl |= MSG_PEEK;
    if (flags & socket_MSG_DONTROUTE)
        fl |= MSG_DONTROUTE;

    sent = send(s, (void *) msg, len, fl);
    if (sent == -1)
        return set_error(errno);

    *cc = sent;

    return NULL;
}

/* ---- xsocket_sendto ---------------------------------------------- */

os_error *xsocket_sendto(socket_s s,
    byte *msg,
    int32_t len,
    socket_msg_flags flags,
    socket_sockaddr *to,
    int32_t tolen,
    int32_t *cc)
{
    struct sockaddr_in addr;
    int fl = 0;
    ssize_t sent;

    //fprintf(stderr, "sendto(%d, %p, %d, %x, %p, %d)\n",
    //    s, msg, len, flags, to, tolen);

    if (to == NULL || tolen != sizeof(*to))
        return set_error(EINVAL);

    /* sockaddr could be v4 or v5 format */
    if (to->sockaddr.af != socket_AF_INET &&
        (to->sockaddr.af < 0xff ||
        ((socket5_sockaddr *) to)->sockaddr.af != socket_AF_INET))
        return set_error(EINVAL);

    addr.sin_family = AF_INET;
    addr.sin_port = to->sockaddr_in.port;
    addr.sin_addr.s_addr = to->sockaddr_in.addr;

    if (flags & socket_MSG_OOB)
        fl |= MSG_OOB;
    if (flags & socket_MSG_PEEK)
        fl |= MSG_PEEK;
    if (flags & socket_MSG_DONTROUTE)
        fl |= MSG_DONTROUTE;

    sent = sendto(s, (void *) msg, len, fl,
        (struct sockaddr *) &addr, sizeof(addr));
    if (sent == -1)
        return set_error(errno);

    return NULL;
}

/* ---- xsocket_sendmsg --------------------------------------------- */

os_error *xsocket_sendmsg(socket_s s,
    socket_msghdr *msg,
    socket_msg_flags flags,
    int32_t *cc)
{
    socket5_msghdr hdr;

    if (msg == NULL)
        return set_error(EINVAL);

    hdr.name = msg->name;
    hdr.namelen = msg->namelen;
    hdr.iov = msg->iov;
    hdr.iov_count = msg->iov_count;
    hdr.control = msg->accrights;
    hdr.controllen = msg->accrightslen;
    hdr.msg_flags = 0;

    return xsocket_sendmsg_1(s, &hdr, flags, cc);
}

/* ---- xsocket_shutdown -------------------------------------------- */

os_error *xsocket_shutdown(socket_s s,
    socket_shutdown_type type)
{
    int how;

    //fprintf(stderr, "shutdown(%d, %d)\n", s, type);

    switch (type) {
        case socket_SHUTDOWN_RECV:
            how = SHUT_RD;
        case socket_SHUTDOWN_SEND:
            how = SHUT_WR;
        case socket_SHUTDOWN_ALL:
            how = SHUT_RDWR;
        default:
            return set_error(EINVAL);
    }
    if (shutdown(s, how) == -1)
        return set_error(errno);
    return NULL;
}

/* ---- xsocket_setsockopt ------------------------------------------ */

os_error *xsocket_setsockopt(socket_s s,
    socket_level level,
    socket_so optname,
    byte *optval,
    int32_t optlen)
{
    int l, o, v;

    //fprintf(stderr, "setsockopt(%d, %d, %d, %p, %d)\n",
    //    s, level, optname, optval, optlen);

    if (optval == NULL || optlen == 0)
        return set_error(EINVAL);

    switch (level) {
        case socket_SOL_SOCKET:
            l = SOL_SOCKET;
            switch (optname) {
                case socket_SO_REUSEADDR:
                    if (optlen != sizeof(int32_t))
                        return set_error(EINVAL);
                    o = SO_REUSEADDR;
                    v = *((int32_t *) optval);
                    if (v != 0)
                        v = 1;
                    break;
                default:
                    return set_error(ENOPROTOOPT);
            }
            break;
        case socket_IPPROTO_TCP:
            l = IPPROTO_TCP;
            switch (optname) {
                case 1:
                    if (optlen != sizeof(int32_t))
                        return set_error(EINVAL);
                    o = TCP_NODELAY;
                    v = *((int32_t *) optval);
                    if (v != 0)
                        v = 1;
                    break;
                case 2:
                    if (optlen != sizeof(int32_t))
                        return set_error(EINVAL);
                    o = TCP_MAXSEG;
                    v = *((int32_t *) optval);
                    if (v != 0)
                        v = 1;
                    break;
                default:
                    return set_error(ENOPROTOOPT);
            }
            break;
        default:
            return set_error(EINVAL);
    }

    if (setsockopt(s, l, o, &v, sizeof(v)) == -1)
        return set_error(errno);

    //fprintf(stderr, "-> setsockopt(%d, %d, %d, %d, %zu)\n",
    //    s, l, o, v, sizeof(v));

    return NULL;
}

/* ---- xsocket_getsockopt ------------------------------------------ */

os_error *xsocket_getsockopt(socket_s s,
    socket_level level,
    socket_so optname,
    byte *optval,
    int32_t *optlen)
{
    int l, o, v;
    socklen_t e = sizeof(v);

    //fprintf(stderr, "getsockopt(%d, %d, %d, %p, %p)\n",
    //    s, level, optname, optval, optlen);

    if (optval == NULL || optlen == NULL)
        return set_error(EINVAL);

    switch (level) {
        case socket_SOL_SOCKET:
            l = SOL_SOCKET;
            switch (optname) {
                case socket_SO_ERROR:
                    if (*optlen < sizeof(int32_t))
                        return set_error(EINVAL);
                    o = SO_ERROR;
                    break;
                default:
                    return set_error(ENOPROTOOPT);
            }
            break;
        case socket_IPPROTO_TCP:
            l = IPPROTO_TCP;
            switch (optname) {
                case 1:
                    if (*optlen < sizeof(int32_t))
                        return set_error(EINVAL);
                    o = TCP_NODELAY;
                    break;
                case 2:
                    if (*optlen < sizeof(int32_t))
                        return set_error(EINVAL);
                    o = TCP_MAXSEG;
                    break;
                default:
                    return set_error(ENOPROTOOPT);
            }
            break;
        default:
            return set_error(EINVAL);
    }

    if (getsockopt(s, l, o, &v, &e) == -1)
        return set_error(errno);

    switch (level) {
        case socket_SOL_SOCKET:
            switch (optname) {
                case socket_SO_ERROR:
                    *((int32_t *) optval) = roerrno_from_host(v);
                    break;
            }
            break;
        case socket_IPPROTO_TCP:
            *((int32_t *) optval) = v;
            break;
    }
    *optlen = sizeof(int32_t);

    //fprintf(stderr, "-> getsockopt(%d, %d, %d, %d, %d)\n",
    //    s, level, optname, *optval, *optlen);

    return NULL;
}

/* ---- xsocket_getpeername ----------------------------------------- */

os_error *xsocket_getpeername(socket_s s,
    socket_sockaddr *sockaddr,
    int32_t *namelen)
{
    os_error *err;

    err = xsocket_getpeername_1(s, (socket5_sockaddr *) sockaddr, namelen);
    if (err == NULL) {
        socket5_af af = ((socket5_sockaddr *) sockaddr)->sockaddr.af;
        sockaddr->sockaddr_in.af = (socket_af) af;
    }

    return err;
}

/* ---- xsocket_getsockname ----------------------------------------- */

os_error *xsocket_getsockname(socket_s s,
    socket_sockaddr *sockaddr,
    int32_t *namelen)
{
    os_error *err;

    err = xsocket_getsockname_1(s, (socket5_sockaddr *) sockaddr, namelen);
    if (err == NULL) {
        socket5_af af = ((socket5_sockaddr *) sockaddr)->sockaddr.af;
        sockaddr->sockaddr_in.af = (socket_af) af;
    }

    return err;
}

/* ---- xsocket_close ----------------------------------------------- */

os_error *xsocket_close(socket_s s)
{
    //fprintf(stderr, "close(%d)\n", s);
    if (close(s) == -1)
        return set_error(errno);
    return NULL;
}

/* ---- xsocket_select ---------------------------------------------- */

os_error *xsocket_select(int32_t nfds,
    socket_fdset *readfds,
    socket_fdset *writefds,
    socket_fdset *exceptfds,
    socket_timeval *timeout,
    int32_t *nfound)
{
    fd_set r, w, x;
    int found;

    //fprintf(stderr, "select(%d, %p, %p, %p, %d.%d, %p)\n",
    //    nfds, readfds, writefds, exceptfds,
    //    timeout->sec, timeout->usec, nfound);

    if (nfds < 0)
        return set_error(EINVAL);

    FD_ZERO(&r);
    FD_ZERO(&w);
    FD_ZERO(&x);

    /* Marshal RISC OS fdsets into host-native ones */
#define RO_FD_ISSET(d,set) ((uint32_t *) (set))[(fd) / 32] & (1 << ((fd) % 32))
    for (int fd = 0; fd < nfds; fd++) {
        if (readfds != NULL && RO_FD_ISSET(fd, readfds))
            FD_SET(fd, &r);
        if (writefds != NULL && RO_FD_ISSET(fd, writefds))
            FD_SET(fd, &w);
        if (exceptfds != NULL && RO_FD_ISSET(fd, exceptfds))
            FD_SET(fd, &x);
    }
#undef RO_FD_ISSET

    if (timeout != NULL) {
        struct timeval tv;
        tv.tv_sec = timeout->sec;
        tv.tv_usec = timeout->usec;

        found = select(nfds, &r, &w, &x, &tv);
    } else {
        found = select(nfds, &r, &w, &x, NULL);
    }

    if (found == -1)
        return set_error(errno);

    /* Marshal results back into RISC OS format */
#define RO_FD_ZERO(set) ((void) memset((set), 0, 8 * sizeof(uint32_t)))
    if (readfds != NULL)
        RO_FD_ZERO(readfds);
    if (writefds != NULL)
        RO_FD_ZERO(writefds);
    if (exceptfds != NULL)
        RO_FD_ZERO(exceptfds);
#undef RO_FD_ZERO

#define RO_FD_SET(d,set) ((uint32_t *) (set))[(fd) / 32] |= (1 << ((fd) % 32))
    for (int fd = 0; fd < nfds; fd++) {
        if (readfds != NULL && FD_ISSET(fd, &r))
            RO_FD_SET(fd, readfds);
        if (writefds != NULL && FD_ISSET(fd, &w))
            RO_FD_SET(fd, writefds);
        if (exceptfds != NULL && FD_ISSET(fd, &x))
            RO_FD_SET(fd, exceptfds);
    }
#undef RO_FD_SET

    *nfound = found;

    return NULL;
}

/* ---- xsocket_ioctl ----------------------------------------------- */

os_error *xsocket_ioctl(socket_s s,
    socket_ioctl_no flags,
    byte *argp)
{
    int val;

    /* XXX: Only support setting FIONBIO for now */
    if (flags != socket_FIONBIO) {
        fprintf(stderr, "ioctl(%d, %x, %d)\n", s, flags, *argp);
        return set_error(EINVAL);
    }

    val = *((int32_t *)argp);

    if (ioctl(s, FIONBIO, &val) == -1)
        return set_error(errno);

    return NULL;
}

/* ---- xsocket_read ------------------------------------------------ */

os_error *xsocket_read(socket_s s,
    byte *buf,
    int32_t len,
    int32_t *cc)
{
    ssize_t received;

    //fprintf(stderr, "read(%d, %p, %d)\n", s, buf, len);

    received = read(s, buf, len);
    if (received == -1)
        return set_error(errno);

    *cc = received;

    return NULL;
}

/* ---- xsocket_write ----------------------------------------------- */

os_error *xsocket_write(socket_s s,
    byte *msg,
    int32_t len,
    int32_t *cc)
{
    ssize_t sent;

    //fprintf(stderr, "write(%d, %p, %d)\n", s, msg, len);

    sent = write(s, msg, len);
    if (sent == -1)
        return set_error(errno);

    *cc = sent;

    return NULL;
}

/* ---- xsocket_readv ----------------------------------------------- */

os_error *xsocket_readv(socket_s s,
    socket_iovec *iov,
    int32_t iov_count,
    int32_t *cc)
{
    fprintf(stderr, "readv(%d, %p, %d)\n", s, iov, iov_count);

    return ERR_NO_SUCH_SWI();
}

/* ---- xsocket_writev ---------------------------------------------- */

os_error *xsocket_writev(socket_s s,
    socket_iovec *iov,
    int32_t iov_count,
    int32_t *cc)
{
    fprintf(stderr, "writev(%d, %p, %d)\n", s, iov, iov_count);

    return ERR_NO_SUCH_SWI();
}

/* ---- xsocket_gettsize -------------------------------------------- */

os_error *xsocket_gettsize(int32_t *size)
{
    //fprintf(stderr, "gettsize(%p)\n", size);

    /* As good an answer as any */
    *size = 256;

    return NULL;
}

/* ---- xsocket_accept_1 -------------------------------------------- */

os_error *xsocket_accept_1(socket_s s,
    socket5_sockaddr *sockaddr,
    int32_t *addrlen,
    socket_s *ns)
{
    struct sockaddr_storage addr;
    socklen_t alen = sizeof(addr);
    int sockfd;

    //fprintf(stderr, "accept_1(%d, %p, %p, %p)\n", s, sockaddr, addrlen, ns);

    if (sockaddr != NULL &&
        (addrlen == NULL || *addrlen < sizeof(*sockaddr)))
        return set_error(EINVAL);

    sockfd = accept(s, (struct sockaddr *) &addr, &alen);
    if (sockfd == -1)
        return set_error(errno);

    assert(alen == sizeof(struct sockaddr_in));
    assert(addr.ss_family == AF_INET);

    if (sockaddr != NULL) {
        struct sockaddr_in *inaddr = (struct sockaddr_in *) &addr;
        sockaddr->sockaddr_in.len = sizeof(socket5_sockaddr_in);
        sockaddr->sockaddr_in.af = socket_AF_INET;
        sockaddr->sockaddr_in.port = inaddr->sin_port;
        sockaddr->sockaddr_in.addr = inaddr->sin_addr.s_addr;
        *addrlen = sizeof(socket5_sockaddr_in);
    }

    *ns = sockfd;

    return NULL;
}

/* ---- xsocket_recvfrom_1 ------------------------------------------ */

os_error *xsocket_recvfrom_1(socket_s s,
    byte *buf,
    int32_t len,
    socket_msg_flags flags,
    socket5_sockaddr *from,
    int32_t *fromlen,
    int32_t *cc)
{
    struct sockaddr_storage addr;
    socklen_t alen = sizeof(addr);
    int fl = 0;
    ssize_t received;

    //fprintf(stderr, "recvfrom_1(%d, %p, %d, %x, %p, %p)\n",
    //    s, buf, len, flags, from, fromlen);

    if (from != NULL && (fromlen == NULL || *fromlen < sizeof(*from)))
        return set_error(EINVAL);

    if (flags & socket_MSG_OOB)
        fl |= MSG_OOB;
    if (flags & socket_MSG_PEEK)
        fl |= MSG_PEEK;
    if (flags & socket_MSG_DONTROUTE)
        fl |= MSG_DONTROUTE;

    received = recvfrom(s, (void *) buf, len, fl,
        (struct sockaddr *) &addr, &alen);
    if (received == -1)
        return set_error(errno);

    assert(alen == sizeof(struct sockaddr_in));
    assert(addr.ss_family == AF_INET);

    if (from != NULL) {
        struct sockaddr_in *inaddr = (struct sockaddr_in *) &addr;
        from->sockaddr_in.len = sizeof(socket5_sockaddr_in);
        from->sockaddr_in.af = socket_AF_INET;
        from->sockaddr_in.port = inaddr->sin_port;
        from->sockaddr_in.addr = inaddr->sin_addr.s_addr;
        *fromlen = sizeof(socket5_sockaddr_in);
    }

    *cc = received;

    return NULL;
}

/* ---- xsocket_recvmsg_1 ------------------------------------------- */

os_error *xsocket_recvmsg_1(socket_s s,
    socket5_msghdr *msg,
    socket_msg_flags flags,
    int32_t *cc)
{
    fprintf(stderr, "recvmsg_1(%d, %p, %x)\n", s, msg, flags);

    return ERR_NO_SUCH_SWI();
}

/* ---- xsocket_sendmsg_1 ------------------------------------------- */

os_error *xsocket_sendmsg_1(socket_s s,
    socket5_msghdr *msg,
    socket_msg_flags flags,
    int32_t *cc)
{
    fprintf(stderr, "sendmsg_1(%d, %p, %x)\n", s, msg, flags);

    return ERR_NO_SUCH_SWI();
}

/* ---- xsocket_getpeername_1 --------------------------------------- */

os_error *xsocket_getpeername_1(socket_s s,
    socket5_sockaddr *sockaddr,
    int32_t *namelen)
{
    struct sockaddr_storage addr;
    struct sockaddr_in *inaddr;
    socklen_t alen = sizeof(addr);

    //fprintf(stderr, "getpeername_1(%d, %p, %p)\n", s, sockaddr, namelen);

    if (sockaddr == NULL || namelen == NULL || *namelen < sizeof(*sockaddr))
        return set_error(EINVAL);

    if (getpeername(s, (struct sockaddr *) &addr, &alen) == -1)
        return set_error(errno);

    assert(alen == sizeof(struct sockaddr_in));
    assert(addr.ss_family == AF_INET);

    inaddr = (struct sockaddr_in *) &addr;
    sockaddr->sockaddr_in.len = sizeof(socket5_sockaddr_in);
    sockaddr->sockaddr_in.af = socket_AF_INET;
    sockaddr->sockaddr_in.port = inaddr->sin_port;
    sockaddr->sockaddr_in.addr = inaddr->sin_addr.s_addr;
    *namelen = sizeof(socket5_sockaddr_in);

    return NULL;
}

/* ---- xsocket_getsockname_1 --------------------------------------- */

os_error *xsocket_getsockname_1(socket_s s,
    socket5_sockaddr *sockaddr,
    int32_t *namelen)
{
    struct sockaddr_storage addr;
    struct sockaddr_in *inaddr;
    socklen_t alen = sizeof(addr);

    //fprintf(stderr, "getsockname_1(%d, %p, %p)\n", s, sockaddr, namelen);

    if (sockaddr == NULL || namelen == NULL || *namelen < sizeof(*sockaddr))
        return set_error(EINVAL);

    if (getsockname(s, (struct sockaddr *) &addr, &alen) == -1)
        return set_error(errno);

    assert(alen == sizeof(struct sockaddr_in));
    assert(addr.ss_family == AF_INET);

    inaddr = (struct sockaddr_in *) &addr;
    sockaddr->sockaddr_in.len = sizeof(socket5_sockaddr_in);
    sockaddr->sockaddr_in.af = socket_AF_INET;
    sockaddr->sockaddr_in.port = inaddr->sin_port;
    sockaddr->sockaddr_in.addr = inaddr->sin_addr.s_addr;
    *namelen = sizeof(socket5_sockaddr_in);

    return NULL;
}

/* ---- xsocket_version --------------------------------------------- */

os_error *xsocket_version(int32_t *n)
{
    /* Conform to Internet 5.02 */
    *n = 502;
    return NULL;
}

/* ---- xsocket_filter_deregister ----------------------------------- */

os_error *xsocket_filter_deregister(socket_af addr_family,
    int32_t prot_hash,
    void *routine,
    int32_t priv_word)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xsocket_filter_register ------------------------------------- */

os_error *xsocket_filter_register(socket_af addr_family,
    int32_t prot_hash,
    void *routine,
    int32_t priv_word)
{
    return ERR_NO_SUCH_SWI();
}

