### Note-1 : Module Configuration Struct(s)

Modules can define up to three configuration structs, one for the main, server, and location contexts. Most modules just need a location configuration. The naming convention for these is ngx_http_<module name>_(main|srv|loc)_conf_t. Here's an example, taken from the dav module:


```C
typedef struct {
    ngx_str_t   message;
} ngx_http_<module_name>_loc_conf_t;

```

### Note-2 : Module Directives

The declaration of ngx_command_t. Example:

```C
struct ngx_command_t {
    ngx_str_t             name;
    ngx_uint_t            type;
    char               *(*set)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
    ngx_uint_t            conf;
    ngx_uint_t            offset;
    void                 *post;
};
```

##### name: 
The name is the directive string, no spaces. The data type is an ngx_str_t, which is usually instantiated with just (e.g.) ngx_str("proxy_pass").

##### type:
type is a set of flags that indicate where the directive is legal and how many arguments the directive takes. Applicable flags, which are bitwise-OR'd, are:

1. NGX_HTTP_MAIN_CONF: directive is valid in the main config
2. NGX_HTTP_SRV_CONF: directive is valid in the server (host) config
3. NGX_HTTP_LOC_CONF: directive is valid in a location config
4. NGX_HTTP_UPS_CONF: directive is valid in an upstream config
5. NGX_CONF_NOARGS: directive can take 0 arguments
6. NGX_CONF_TAKE1: directive can take exactly 1 argument
7. NGX_CONF_TAKE2: directive can take exactly 2 arguments ........
8. NGX_CONF_TAKE7: directive can take exactly 7 arguments
9. NGX_CONF_FLAG: directive takes a boolean ("on" or "off")
10. NGX_CONF_1MORE: directive must be passed at least one argument
11. NGX_CONF_2MORE: directive must be passed at least two arguments

##### set:
The set struct element is a pointer to a function for setting up part of the module's configuration; typically this function will translate the arguments passed to this directive and save an appropriate value in its configuration struct. This setup function will take three arguments:

1. a pointer to an ngx_conf_t struct, which contains the arguments passed to the directive
2. a pointer to the current ngx_command_t struct
3. a pointer to the module's custom configuration struct

This setup function will be called when the directive is encountered. Nginx provides a number of functions for setting particular types of values in the custom configuration struct. These functions include:

1. ngx_conf_set_flag_slot: translates "on" or "off" to 1 or 0
2. ngx_conf_set_str_slot: saves a string as an ngx_str_t
3. ngx_conf_set_num_slot: parses a number and saves it to an int
4. ngx_conf_set_size_slot: parses a data size ("8k", "1m", etc.) and saves it to a size_t

##### conf & offset:
How do these built-in functions know where to save the data? That's where the next two elements of ngx_command_t come in, conf and offset. conf tells Nginx whether this value will get saved to the module's main configuration, server configuration, or location configuration

1. NGX_HTTP_MAIN_CONF_OFFSET
2. NGX_HTTP_SRV_CONF_OFFSET
3. NGX_HTTP_LOC_CONF_OFFSET

##### post:
Finally, post is just a pointer to other crap the module might need while it's reading the configuration. It's often NULL.

The commands array is terminated with ngx_null_command as the last element.


### Note-3: Variables

##### Variables Type:
1. NGX_HTTP_VAR_CHANGEABLE      :   variable is subject to change (for example the "set" command in the rewrite module);
2. NGX_HTTP_VAR_NOCACHEABLE     :   the value cannot be cached;


##### Adding a variable

Syntax
```C
ngx_http_variable_t *ngx_http_add_variable(ngx_conf_t *cf, ngx_str_t *name, ngx_uint_t flags);
```

1. cf - a module configuration to which the variable will be added;
2. name - name of the variable to add;
3. flags - flags defining the type of variable (see above)
4. result - a pointer to the added ngx_http_variable_t structure.

Example:
```C
static u_char  ngx_printable_message[] = "Yes I was print.... ha ha ha";
static ngx_int_t
ngx_http_ab_router_prefix_generator(ngx_http_request_t *r, ngx_http_variable_value_t *v, uintptr_t data){
	ngx_http_variable_value_t *vv = v;	
	vv->data = ngx_printable_message;	
	if (vv->data == NULL) {
		vv->valid = 0;
		vv->not_found = 1;
	} else {
		vv->len = ngx_strlen( vv->data );
		vv->valid = 1;
		vv->no_cacheable = 0;
		vv->not_found = 0;
	}
	return NGX_OK;
}


static ngx_int_t ngx_http_ab_router(ngx_conf_t *cf){ 
    
    ngx_http_variable_t *ab_prefix_var;
    ngx_str_t ab_prefix_var_name = ngx_string("ab_prefix");

    ab_prefix_var = ngx_http_add_variable(cf, &ab_prefix_var_name, NGX_HTTP_VAR_NOCACHEABLE );
    if (ab_prefix_var == NULL) {
            return NGX_ERROR;
    }
    ab_prefix_var->get_handler = ngx_http_ab_router_prefix_generator;
    return NGX_OK;
}
```


### Note-4 OpenSSL AES : 

1. EVP_CIPHER_CTX_init()                :    Initializes cipher context ctx.
```C
void EVP_CIPHER_CTX_init(EVP_CIPHER_CTX *a);
```
2. EVP_EncryptInit()                    :    Sets up cipher context ctx for encryption

```C
int EVP_EncryptInit(EVP_CIPHER_CTX *ctx, const EVP_CIPHER *type, unsigned char *key, unsigned char *iv);
```

3. EVP_EncryptFinal()                   :   If padding is enabled (the default) then encrypts the ``final'' data, that is any data that remains in a partial block.

```C
 int EVP_EncryptFinal_ex(EVP_CIPHER_CTX *ctx, unsigned char *out, int *outl);
```

4. EVP_DecryptFinal()                   :   If padding is enabled (the default) then decrypt the ``final'' data, that is any data that remains in a partial block.

```C
int EVP_DecryptFinal(EVP_CIPHER_CTX *ctx, unsigned char *outm, int *outl);
```

5. EVP_CIPHER_CTX_cleanup()             :   Clears all information from a cipher context and free up any allocated memory associate with it.

```C
int EVP_CIPHER_CTX_cleanup(EVP_CIPHER_CTX *a);
```

6. EVP_DecryptInit()                    :    Sets up cipher context ctx for Decryption

```C
int EVP_DecryptInit(EVP_CIPHER_CTX *ctx, const EVP_CIPHER *type, unsigned char *key, unsigned char *iv);
```

7. EVP_DecryptUpdate()                  :   If padding is enabled (the default) then decrypt the ``final'' data, that is any data that remains in a partial block.                  

```C
int EVP_DecryptUpdate(EVP_CIPHER_CTX *ctx, unsigned char *out, int *outl, unsigned char *in, int inl);
```

### Note-5 Added Access Cookie : 
```C
ngx_str_t cookie = (ngx_str_t)ngx_string("auth_token"); 
ngx_int_t location; 
ngx_str_t cookie_value; 
location = ngx_http_parse_multi_header_lines(&r->headers_in.cookies, &cookie, &cookie_value);
```

### Note-6 Access Request Type: 
1. NGX_HTTP_GET
2. NGX_HTTP_HEAD
3. NGX_HTTP_POST 
4. [Method Constant](http://lxr.nginx.org/source/src/http/ngx_http_request.h#L158)

Example:
```C
 /* we response to 'GET' and 'HEAD' requests only */
    if (!(r->method & (NGX_HTTP_GET|NGX_HTTP_HEAD))) {
        return NGX_HTTP_NOT_ALLOWED;
    }
```


### Note-7 Added Debug Log : 

Add Parameter when Compile nginx : ./configure --with-debug

Example:
```bash
./configure --with-debug --add-module=/root/nginx/Parse/
```

Then the debug level should be set with the error_log directive:

```bash
........

error_log  logs/debug.log debug;

......

http {
    include       mime.types;
    default_type  application/octet-stream;
    
    .......................

```


### Note-8 : Print Error Log in error.log 
```C
ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,"message");
```

### Note- : 
```C

```

### Note- : 
```C

```

