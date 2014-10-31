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
10.NGX_CONF_1MORE: directive must be passed at least one argument
11.NGX_CONF_2MORE: directive must be passed at least two arguments

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


### Note- : 
```C

```

### Note- : 
```C

```

### Note- : 
```C

```

### Note- : 
```C

```

### Note- : 
```C

```

### Note- : 
```C

```

### Note- : 
```C

```

### Note- : 
```C

```
