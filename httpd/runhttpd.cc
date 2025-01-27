#include <sys/types.h>
#ifndef _WIN32
#include <sys/select.h>
#include <sys/socket.h>
#else
#include <winsock2.h>
#endif
#include <MHD_config.h>
#include <curl/curl.h>
#include <errno.h>
#include <iostream>
#include <microhttpd.h>
#include <mhd_has_param.h>
#include <mhd_has_in_name.h>
#include <platform.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SERVER_PORT 8888

/**
 * Run the hello browser exmaple from GNU microhttpd tutorial
 */
static enum MHD_Result answer_to_connection( void *cls,
                                             struct MHD_Connection *connection,
                                             const char *url,
                                             const char *method,
                                             const char *version,
                                             const char *upload_data,
                                             size_t *upload_data_size,
                                             void **con_cls )
{
    const char *page = "<html><body>Hello, browser!</body></html>";
    struct MHD_Response *response;
    enum MHD_Result ret;
    (void)cls;              /* Unused. Silent compiler warning. */
    (void)url;              /* Unused. Silent compiler warning. */
    (void)method;           /* Unused. Silent compiler warning. */
    (void)version;          /* Unused. Silent compiler warning. */
    (void)upload_data;      /* Unused. Silent compiler warning. */
    (void)upload_data_size; /* Unused. Silent compiler warning. */
    (void)con_cls;          /* Unused. Silent compiler warning. */

    response = MHD_create_response_from_buffer( strlen( page ), (void *)page, MHD_RESPMEM_PERSISTENT );
    ret = MHD_queue_response( connection, MHD_HTTP_OK, response );
    MHD_destroy_response( response );

    return ret;
}

static enum MHD_Result ahc_echo( void *cls,
                                 struct MHD_Connection *connection,
                                 const char *url,
                                 const char *method,
                                 const char *version,
                                 const char *upload_data,
                                 size_t *upload_data_size,
                                 void **req_cls )
{
    static int ptr;
    struct MHD_Response *response;
    enum MHD_Result ret;
    const char *v;
    (void)cls;
    (void)version;
    (void)upload_data;
    (void)upload_data_size; /* Unused. Silence compiler warning. */

    if ( 0 != strcmp( MHD_HTTP_METHOD_GET, method ) )
        return MHD_NO; /* unexpected method */
    if ( &ptr != *req_cls )
    {
        *req_cls = &ptr;
        return MHD_YES;
    }
    *req_cls = NULL;
    v = MHD_lookup_connection_value( connection, MHD_GET_ARGUMENT_KIND, "a" );
    if ( ( NULL == v ) || ( 0 != strcmp( "&", v ) ) )
    {
        fprintf( stderr, "Found while looking for 'a=&': 'a=%s'\n", NULL == v ? "NULL" : v );
        _exit( 17 );
    }
    v = NULL;
    if ( MHD_YES != MHD_lookup_connection_value_n( connection, MHD_GET_ARGUMENT_KIND, "b", 1, &v, NULL ) )
    {
        fprintf( stderr, "Not found 'b' GET argument.\n" );
        _exit( 18 );
    }
    if ( ( NULL == v ) || ( 0 != strcmp( "c", v ) ) )
    {
        fprintf( stderr, "Found while looking for 'b=c': 'b=%s'\n", NULL == v ? "NULL" : v );
        _exit( 19 );
    }
    response = MHD_create_response_from_buffer_copy( strlen( url ), (const void *)url );
    ret = MHD_queue_response( connection, MHD_HTTP_OK, response );
    MHD_destroy_response( response );
    if ( ret == MHD_NO )
    {
        fprintf( stderr, "Failed to queue response.\n" );
        _exit( 19 );
    }
    return ret;
}

struct CBC
{
    char *buf;
    size_t pos;
    size_t size;
};

#define EXPECTED_URI_PATH "/hello_world?a=%26&b=c"
static size_t copyBuffer( void *ptr, size_t size, size_t nmemb, void *ctx )
{
    struct CBC *cbc = (CBC *)ctx;

    if ( cbc->pos + size * nmemb > cbc->size )
        return 0; /* overflow */
    memcpy( &cbc->buf[cbc->pos], ptr, size * nmemb );
    cbc->pos += size * nmemb;
    return size * nmemb;
}

static void *log_cb( void *cls, const char *uri, struct MHD_Connection *con )
{
    (void)cls;
    (void)con;
    if ( 0 != strcmp( uri, EXPECTED_URI_PATH ) )
    {
        fprintf( stderr, "Wrong URI: `%s'\n", uri );
        _exit( 22 );
    }
    return NULL;
}

static int oneone;
static uint16_t global_port;

static unsigned int testInternalGet( uint32_t poll_flag )
{
    std::cout << "Test Internal Get\n";
    struct MHD_Daemon *d;
    CURL *c;
    char buf[2048];
    struct CBC cbc;
    CURLcode errornum;

    if ( ( 0 == global_port ) && ( MHD_NO == MHD_is_feature_supported( MHD_FEATURE_AUTODETECT_BIND_PORT ) ) )
    {
        global_port = 1220;
        if ( oneone )
            global_port += 20;
    }

    cbc.buf = buf;
    cbc.size = 2048;
    cbc.pos = 0;
    d = MHD_start_daemon( MHD_USE_INTERNAL_POLLING_THREAD | MHD_USE_ERROR_LOG | (enum MHD_FLAG)poll_flag,
                          global_port,
                          NULL,
                          NULL,
                          &ahc_echo,
                          NULL,
                          MHD_OPTION_URI_LOG_CALLBACK,
                          &log_cb,
                          NULL,
                          MHD_OPTION_END );
    if ( d == NULL )
        return 1;
    if ( 0 == global_port )
    {
        const union MHD_DaemonInfo *dinfo;
        dinfo = MHD_get_daemon_info( d, MHD_DAEMON_INFO_BIND_PORT );
        if ( ( NULL == dinfo ) || ( 0 == dinfo->port ) )
        {
            MHD_stop_daemon( d );
            return 32;
        }
        global_port = dinfo->port;
    }
    c = curl_easy_init();
    curl_easy_setopt( c, CURLOPT_URL, "http://127.0.0.1" EXPECTED_URI_PATH );
    curl_easy_setopt( c, CURLOPT_PORT, (long)global_port );
    curl_easy_setopt( c, CURLOPT_WRITEFUNCTION, &copyBuffer );
    curl_easy_setopt( c, CURLOPT_WRITEDATA, &cbc );
    curl_easy_setopt( c, CURLOPT_FAILONERROR, 1L );
    curl_easy_setopt( c, CURLOPT_TIMEOUT, 150L );
    curl_easy_setopt( c, CURLOPT_CONNECTTIMEOUT, 150L );
    if ( oneone )
        curl_easy_setopt( c, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1 );
    else
        curl_easy_setopt( c, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_0 );
    /* NOTE: use of CONNECTTIMEOUT without also
     setting NOSIGNAL results in really weird
     crashes on my system!*/
    curl_easy_setopt( c, CURLOPT_NOSIGNAL, 1L );
    if ( CURLE_OK != ( errornum = curl_easy_perform( c ) ) )
    {
        fprintf( stderr, "curl_easy_perform failed: `%s'\n", curl_easy_strerror( errornum ) );
        curl_easy_cleanup( c );
        MHD_stop_daemon( d );
        return 2;
    }
    curl_easy_cleanup( c );
    MHD_stop_daemon( d );
    if ( cbc.pos != strlen( "/hello_world" ) )
        return 4;
    if ( 0 != strncmp( "/hello_world", cbc.buf, strlen( "/hello_world" ) ) )
        return 8;
    return 0;
}

/**
 * HTTP Get Hello 
 */
int httpAnswer()
{
    struct MHD_Daemon *daemon;

    daemon = MHD_start_daemon( MHD_USE_AUTO | MHD_USE_INTERNAL_POLLING_THREAD,
                               SERVER_PORT,
                               NULL,
                               NULL,
                               &answer_to_connection,
                               NULL,
                               MHD_OPTION_END );
    if ( NULL == daemon )
        return 1;
    // Hit any key and stdin will return to terminate this program
    (void)getchar();

    // Exit
    MHD_stop_daemon( daemon );
    return 0;
}


int main(int argc, char **argv) try {
  unsigned int errorCount = 0;
  unsigned int test_result = 0;
  int verbose = 0;

  if ((NULL == argv) || (0 == argv[0])){
    return 99;
  }else if(has_param (argc, argv, "--server")){
    std::cout << "Starting HTTPD Server on port "<< SERVER_PORT << "\n";
    std::cout << "Hit Enter key to terminate \n";
    return httpAnswer();
  }
  oneone = has_in_name (argv[0], "11");
  verbose = has_param (argc, argv, "-v") || has_param (argc, argv, "--verbose");
  if (0 != curl_global_init (CURL_GLOBAL_WIN32))
    return 2;
  global_port = 0;
  test_result += testInternalGet (0);
  if (test_result)
      fprintf (stderr, "FAILED: testInternalGet (0) - %u.\n", test_result);
  else if (verbose)
      printf ("PASSED: testInternalGet (0).\n");
  errorCount += test_result;
}
catch (std::exception const &e){
    std::cerr << "Unexpected exception " << e.what() << std::endl;
}