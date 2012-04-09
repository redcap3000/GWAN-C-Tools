// ============================================================================
// This is a Servlet sample for the G-WAN Web Server (http://www.trustleap.com)
// ----------------------------------------------------------------------------
//	Ronaldo Barbachano 4-9-2012				
// tumblr.c:
// Simple example of how to connect to tumblr's API using 
// xbuf_frurl and modified function call_site (to remove the var tumblr_api_read from
// api response
//
// ============================================================================
// USAGE
// ----------------------------------------------------------------------------
// 	pass the script the param of 
//			?tumblr.c&u=username
//  to get the pages 
//			?tumblr.c&u=userName&p=1
// Returns 404 if too many or two few params exist.	But could direct user to a form/etc.
// Change OK status to 1 (or less than 99) to return raw data.
// ============================================================================
// TODO
// ----------------------------------------------------------------------------
// Implement K-V stores for caching. Process JSON/XML. Implement easy to use templates.
				
#include "gwan.h" // G-WAN exported functions


static char host[]="tumblr.com";

// set this to 1 to disable headers to return raw data
static int ok=200;
// number of chars to allow for the total size of the URI
static int uri_size = 1024;

static int call_site(const char *site,const char *directive, const char *d_value, const void *my_ctx)
{
	// using d_value to quickly pass a char instead of making another string
	// Ideally process more than one 'directive' and pass in multi-dimensional array
	// but the user could just as easily append it to whatever is passed into value...

   xbuf_t *reply = (xbuf_t*)my_ctx; // used here to pass the 'reply' buffer

    int call_status=0;char str[uri_size];
    // make a new buffer for the replacement action
    xbuf_t buf;
    xbuf_init(&buf);

    s_snprintf(str, uri_size - 1, "%s%s", directive,d_value);
	// this is where rendering happens... (or not)
	// perhaps display something while this is loading ?
	
    call_status = xbuf_frurl(&buf, site, 80, HTTP_GET,str, 5000, 0);
	// use a buf to strip this from the top of returned values...
	if(directive=="/api/read/json"){
		xbuf_repl(&buf, "var tumblr_api_read = ", "");
	}

	if(buf.len){
		// do more processing from here...
		 xbuf_ncat(reply, buf.ptr, buf.len);
	}

	xbuf_free(&buf);
	// do a little cleanup on xbuf to avoid showing tmblr_api read var?

   return call_status;	
}

int main(int argc, char *argv[])
{
	xbuf_t *reply = get_reply(argv);
	   
	char *term = 0;
	// to build the action string like /api/read/json etc...
	char *action =0;
	
	char str[uri_size];
	
	// only listen to calls with two argument's max, or else return form
	if(argc != 1 && argc != 2){
		// or return a default page
		return 404;
	}
	
	if(argc == 1){
		// default show json by default if only a user param is provided
		// need to simply determine what the param name is and write switch statement 
		// instead of making all of these get_arg calls..
		get_arg("u=", &term, argc, argv);
		// host name needs to be formed like tumbruser.tubmlr.com
		// check for other calls...
		action = "/api/read/json";
	}
	
	if(argc == 2){
		
		// get pages if u=tmblrUsr&p=1
		// u=tumblrusr
		get_arg("u=", &term, argc, argv);
		
		// p=1 (get pages)
		get_arg("p=", &action, argc, argv);
		
		if(term==0 || action ==0){
			return 404;
		}
		action = "/api/pages";
		
	}
	
	s_snprintf(str, uri_size - 1, "%s.%s", term,host);

	// if term is continually equal to zero call_site is not executed.
	// probably could handle the const char*d_value better if not present instead of passing a blank string
	if(term!=0 && call_site(str,action,"",reply) == 200){
		return ok;
	}
	
   	return 404;
}
// ============================================================================
// End of Source Code
// ============================================================================
