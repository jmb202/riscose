/* os/socket.c - implementation of the Socket module. */

/* See http://riscose.sf.net/ for terms of distribution, and to
 * pick up a later version of the software.
 *
 * Created by defmod, riscose version 1.01. */

#include <stdio.h>

#include "monty/monty.h"
#include "types.h"
#include "rom/rom.h"
#include "socket.h"

/* ---- socket_swi_register_extra ----------------------------------- */

void socket_swi_register_extra(void)
{
    return;
}

/* ---- xsocket_creat ----------------------------------------------- */

os_error *xsocket_creat(socket_af af,
    socket_sock sock,
    socket_ipproto proto,
    socket_s *s)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xsocket_bind ------------------------------------------------ */

os_error *xsocket_bind(socket_s s,
    socket_sockaddr *sockaddr,
    int32_t namelen)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xsocket_listen ---------------------------------------------- */

os_error *xsocket_listen(socket_s s,
    int32_t backlog)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xsocket_accept ---------------------------------------------- */

os_error *xsocket_accept(socket_s s,
    socket_sockaddr *sockaddr,
    int32_t *addrlen,
    socket_s *ns)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xsocket_connect --------------------------------------------- */

os_error *xsocket_connect(socket_s s,
    socket_sockaddr *sockaddr,
    int32_t namelen)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xsocket_recv ------------------------------------------------ */

os_error *xsocket_recv(socket_s s,
    byte *buf,
    int32_t len,
    socket_msg_flags flags,
    int32_t *cc)
{
    return ERR_NO_SUCH_SWI();
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
    return ERR_NO_SUCH_SWI();
}

/* ---- xsocket_recvmsg --------------------------------------------- */

os_error *xsocket_recvmsg(socket_s s,
    socket_msghdr *msg,
    socket_msg_flags flags,
    int32_t *cc)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xsocket_send ------------------------------------------------ */

os_error *xsocket_send(socket_s s,
    byte *msg,
    int32_t len,
    socket_msg_flags flags,
    int32_t *cc)
{
    return ERR_NO_SUCH_SWI();
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
    return ERR_NO_SUCH_SWI();
}

/* ---- xsocket_sendmsg --------------------------------------------- */

os_error *xsocket_sendmsg(socket_s s,
    socket_msghdr *msg,
    socket_msg_flags flags,
    int32_t *cc)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xsocket_shutdown -------------------------------------------- */

os_error *xsocket_shutdown(socket_s s,
    socket_shutdown_type type)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xsocket_setsockopt ------------------------------------------ */

os_error *xsocket_setsockopt(socket_s s,
    socket_level level,
    socket_so optname,
    byte *optval,
    int32_t optlen)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xsocket_getsockopt ------------------------------------------ */

os_error *xsocket_getsockopt(socket_s s,
    socket_level level,
    socket_so optname,
    byte *optval,
    int32_t *optlen)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xsocket_getpeername ----------------------------------------- */

os_error *xsocket_getpeername(socket_s s,
    socket_sockaddr *sockaddr,
    int32_t *namelen)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xsocket_getsockname ----------------------------------------- */

os_error *xsocket_getsockname(socket_s s,
    socket_sockaddr *sockaddr,
    int32_t *namelen)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xsocket_close ----------------------------------------------- */

os_error *xsocket_close(socket_s s)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xsocket_select ---------------------------------------------- */

os_error *xsocket_select(int32_t nfds,
    socket_fdset *readfds,
    socket_fdset *writefds,
    socket_fdset *exceptfds,
    socket_timeval *timeout,
    int32_t *nfound)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xsocket_ioctl ----------------------------------------------- */

os_error *xsocket_ioctl(socket_s s,
    socket_ioctl_no flags,
    byte *argp)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xsocket_read ------------------------------------------------ */

os_error *xsocket_read(socket_s s,
    byte *buf,
    int32_t len,
    int32_t *cc)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xsocket_write ----------------------------------------------- */

os_error *xsocket_write(socket_s s,
    byte *msg,
    int32_t len,
    int32_t *cc)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xsocket_readv ----------------------------------------------- */

os_error *xsocket_readv(socket_s s,
    socket_iovec *iov,
    int32_t iov_count,
    int32_t *cc)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xsocket_writev ---------------------------------------------- */

os_error *xsocket_writev(socket_s s,
    socket_iovec *iov,
    int32_t iov_count,
    int32_t *cc)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xsocket_gettsize -------------------------------------------- */

os_error *xsocket_gettsize(int32_t *size)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xsocket_version --------------------------------------------- */

os_error *xsocket_version(int32_t *n)
{
    return ERR_NO_SUCH_SWI();
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

