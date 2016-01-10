
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"

#include "httpserver.h"

#include <xboxrt/string.h>
#include <hal/xbox.h>

#if LWIP_NETCONN

#ifndef HTTPD_DEBUG
#define HTTPD_DEBUG         LWIP_DBG_OFF
#endif

static const char http_html_hdr[]   = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
static const char http_index_html[] = "<html><head><title>Congrats!</title></head><body><h1>Welcome to our lwIP HTTP server!</h1><p>This is a small test page, served by httpserver-netconn.<hr /><a href=\"/quit\">Restart</a></body></html>";
static const char http_quit_html[]  = "<html><head><title>Restarting...</title></head><body>Restarting...</body></html>";

static void http_server_serve_index(struct netconn *conn)
{
  /* Send the HTML header
         * subtract 1 from the size, since we dont send the \0 in the string
         * NETCONN_NOCOPY: our data is const static, so no need to copy it
   */
  netconn_write(conn, http_html_hdr, sizeof(http_html_hdr)-1, NETCONN_NOCOPY);

  /* Send our HTML page */
  netconn_write(conn, http_index_html, sizeof(http_index_html)-1, NETCONN_NOCOPY);
}

static void http_server_serve_quit(struct netconn *conn)
{
  netconn_write(conn, http_html_hdr, sizeof(http_html_hdr)-1, NETCONN_NOCOPY);
  netconn_write(conn, http_quit_html, sizeof(http_quit_html)-1, NETCONN_NOCOPY);
}

/** Serve one HTTP connection accepted in the http thread */
static void
http_server_netconn_serve(struct netconn *conn)
{
  struct netbuf *inbuf;
  char *buf;
  u16_t buflen;
  err_t err;
  ip_addr_t naddr;
  u16_t port = 0;
  
  /* Read the data from the port, blocking if nothing yet there. 
   We assume the request (the part we care about) is in one netbuf */
  err = netconn_recv(conn, &inbuf);
  
  if (err == ERR_OK) {

    netbuf_data(inbuf, (void**)&buf, &buflen);

    /* Get and display remote ip address and request headers */
    netconn_peer(conn, &naddr, &port);
    debugPrint("[Request from %s]\n", ip4addr_ntoa(ip_2_ip4(&naddr)));
    for (int i = 0; i < buflen; i++) debugPrint("%c", buf[i]);

    if ((buflen >= 9) && (memcmp(buf, "GET /quit", 9) == 0))
    {
      http_server_serve_quit(conn);
      netconn_close(conn);
      netbuf_delete(inbuf);
      XReboot();
    }

    /* Is this an HTTP GET command? (only check the first 5 chars, since
    there are other formats for GET, and we're keeping it very simple )*/
    if (buflen>=5 &&
        buf[0]=='G' &&
        buf[1]=='E' &&
        buf[2]=='T' &&
        buf[3]==' ' &&
        buf[4]=='/' ) {
      http_server_serve_index(conn);
    }
  }
  /* Close the connection (server closes in HTTP) */
  netconn_close(conn);
  
  /* Delete the buffer (netconn_recv gives us ownership,
   so we have to make sure to deallocate the buffer) */
  netbuf_delete(inbuf);
}

/** The main function, never returns! */
static void
http_server_netconn_thread(void *arg)
{
  struct netconn *conn, *newconn;
  err_t err;
  LWIP_UNUSED_ARG(arg);
  
  /* Create a new TCP connection handle */
  conn = netconn_new(NETCONN_TCP);
  LWIP_ERROR("http_server: invalid conn", (conn != NULL), return;);
  
  /* Bind to port 80 (HTTP) with default IP address */
  netconn_bind(conn, NULL, 80);
  
  /* Put the connection into LISTEN state */
  netconn_listen(conn);
  
  do {
    err = netconn_accept(conn, &newconn);
    if (err == ERR_OK) {
      http_server_netconn_serve(newconn);
      netconn_delete(newconn);
    }
  } while(err == ERR_OK);
  LWIP_DEBUGF(HTTPD_DEBUG,
    ("http_server_netconn_thread: netconn_accept received error %d, shutting down",
    err));
  netconn_close(conn);
  netconn_delete(conn);
}

/** Initialize the HTTP server (start its thread) */
void
http_server_netconn_init(void)
{
  sys_thread_new("http_server_netconn", http_server_netconn_thread, NULL, DEFAULT_THREAD_STACKSIZE, DEFAULT_THREAD_PRIO);
}

#endif /* LWIP_NETCONN*/
