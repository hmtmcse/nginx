#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


//static u_char  ngx_printable_message[] = "Yes I was print.... ha ha ha";
static char *ngx_http_parse(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static ngx_command_t ngx_http_parse_commands[] = {

    { ngx_string("parse"),
        NGX_HTTP_LOC_CONF | NGX_CONF_NOARGS,
        ngx_http_parse,
        0,
        0,
        NULL},
    ngx_null_command
};

static ngx_http_module_t ngx_http_parse_module_ctx = {
    NULL, /* preconfiguration */
    NULL, /* postconfiguration */

    NULL, /* create main configuration */
    NULL, /* init main configuration */

    NULL, /* create server configuration */
    NULL, /* merge server configuration */

    NULL, /* create location configuration */
    NULL /* merge location configuration */
};


ngx_module_t ngx_http_parse_module = {
    NGX_MODULE_V1,
    &ngx_http_parse_module_ctx, /* module context */
    ngx_http_parse_commands, /* module directives */
    NGX_HTTP_MODULE, /* module type */
    NULL, /* init master */
    NULL, /* init module */
    NULL, /* init process */
    NULL, /* init thread */
    NULL, /* exit thread */
    NULL, /* exit process */
    NULL, /* exit master */
    NGX_MODULE_V1_PADDING
};

ngx_int_t
ngx_http_arg2(ngx_http_request_t *r, u_char *name, size_t len, ngx_str_t *value) {
    u_char *p, *last;
    if (r->args.len == 0) {
        return NGX_DECLINED;
    }
    p = r->args.data;
    last = p + r->args.len;
    for (/* void */; p < last; p++) {
        if (p + len > last) {
            return NGX_DECLINED;
        }
        if (ngx_strncasecmp(p, name, len) != 0) {
            continue;
        }
        if (p == r->args.data || *(p - 1) == '&' || (p + len) == last || *(p + len) == '&' || *(p + len) == '=') {
            if ((p + len) < last && *(p + len) == '=') {
                value->data = p + len + 1;
                p = ngx_strlchr(p, last, '&');
                if (p == NULL) {
                    p = r->args.data + r->args.len;
                }
                value->len = p - value->data;
            } else {
                value->len = 0;
            }
            return NGX_OK;
        }
    }
    return NGX_DECLINED;
}

ngx_int_t
search_headers_in(ngx_http_request_t *r, u_char *name, size_t len) {

    ngx_list_part_t *part;
    ngx_table_elt_t *h;
    ngx_uint_t i;
    part = &r->headers_in.headers.part;
    h = part->elts;
    for (i = 0; /* void */; i++) {
        if (i >= part->nelts) {
            if (part->next == NULL) {
                /* The last part, search is done. */
                break;
            }

            part = part->next;
            h = part->elts;
            i = 0;
        }



        /*
        Just compare the lengths and then the names case insensitively.
         */

        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, (char *) h[i].value.data);
        if (len != h[i].key.len || ngx_strcasecmp(name, h[i].key.data) != 0) {
            /* This header doesn't match. */
            continue;
        }



        /*
        Ta-da, we got one!
        Note, we'v stop the search at the first matched header
        while more then one header may fit.
         */
        return NGX_DECLINED;
    }
    return NGX_OK;
}

//static void ngx_http_form_input_post_read(ngx_http_request_t *r) {
//    ngx_http_parse_module_ctx_t *ctx;
//
//    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
//            "http form_input post read request body");
//
//    r->read_event_handler = ngx_http_request_empty_handler;
//
//    ctx = ngx_http_get_module_ctx(r, ngx_http_parse_module);
//    ctx->done = 1;
//
//#if defined(nginx_version) && nginx_version >= 8011
//    r->main->count--;
//#endif
//    if (ctx->waiting_more_body) {
//        ctx->waiting_more_body = 0;
//
//        ngx_http_core_run_phases(r);
//    }
//}


static ngx_int_t ngx_http_parse_handler(ngx_http_request_t *r) {
    ngx_buf_t *b;
    ngx_chain_t out;

    ngx_str_t args = r->args;

    r->headers_out.content_type_len = sizeof ("text/html") - 1;
    r->headers_out.content_type.len = sizeof ("text/html") - 1;
    r->headers_out.content_type.data = (u_char *) "text/html";

    b = ngx_pcalloc(r->pool, sizeof (ngx_buf_t));

    out.buf = b;
    out.next = NULL;

    ngx_str_t arg_val;
    char *params_args = "my_args";

//    if (r->method == NGX_HTTP_POST) {
//        ngx_int_t rc = ngx_http_read_client_request_body(r, ngx_http_form_input_post_read);
//    }


    if (ngx_http_arg2(r, (u_char *) params_args, ngx_strlen((u_char *) params_args), &arg_val) == NGX_OK) {
  ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,(char *)r->args.data);
        b->pos = arg_val.data;
        b->last = arg_val.data + arg_val.len;
        r->headers_out.content_length_n = arg_val.len;
    } else {
        b->pos = args.data;
        b->last = args.data + args.len;
        r->headers_out.content_length_n = args.len;
    }

    b->memory = 1;
    b->last_buf = 1;

    r->headers_out.status = NGX_HTTP_OK;
    ngx_http_send_header(r);

    return ngx_http_output_filter(r, &out);
}

static char *ngx_http_parse(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
    ngx_http_core_loc_conf_t *clcf;
    clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    clcf->handler = ngx_http_parse_handler;
    return NGX_CONF_OK;
}


