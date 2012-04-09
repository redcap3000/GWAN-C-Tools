// ============================================================================
// This is a Servlet sample for the G-WAN Web Server (http://www.trustleap.com)
// ----------------------------------------------------------------------------
//	Ronaldo Barbachano 4-6-2012				
// call_site.c:
// 				Loads a website and passes a generated URI/URL. Shows usage of 
//				xbuf_frurl() and get_arg() and custom function call_site(). 
//				Use this to call on API's, mirror sites, etc.							
// 				call_site( char * http host, char * uri/url, 
//						   char *extra appended value, buf_t)

// 				Can pass an extra value as char (useful for get_arg()) to use 
//				a get param somewhere in the new call... Example usage shows 
//				ways to interface with rxnav's API. Use with care,
//				its kinda slow, and returns (sometimes invalid) xml by default.
//				Next step is to figure out how to send it the right headers 
//				(Application/JSON) to get it in json, or parse the XML.
				
#include "gwan.h" // G-WAN exported functions


static char host[]="rxnav.nlm.nih.gov";

// used as a return value; set this to 1 to disable headers to return raw data
static int ok=200;
// number of chars to allow for the total size of the URI
static int uri_size = 2024;

static int call_site(const char *site,const char *directive, const char *d_value, const void *my_ctx)
{
	// using d_value to quickly pass a char instead of making another string
	// Ideally process more than one 'directive' and pass in multi-dimensional array
	// but the user could just as easily append it to whatever is passed into value...

   xbuf_t *reply = (xbuf_t*)my_ctx; // used here to pass the 'reply' buffer

   int call_status=0;char str[uri_size];
   
   // build URI string with one parameter could simplify by just passing in an entire URI and host
   // but would still require a s_snprintf somewhere to assemble, this ensures that people cant inject
   //as easily and gives the developer enough options.
   s_snprintf(str, uri_size - 1, "%s%s", directive,d_value);

	// this is where rendering happens... (or not)
	// perhaps display something while this is loading ?
   call_status = xbuf_frurl(reply, site, 80, HTTP_GET,str, 5000, 0);
   
   return call_status;	
}

int main(int argc, char *argv[])
{
	xbuf_t *reply = get_reply(argv);
	   
	char *term = 0;
	
	// only listen to calls with one argument
	if(argc == 1){
		// need to simply determine what the param name is and write switch statement 
		// instead of making all of these get_arg calls..
		get_arg("aterm=", &term, argc, argv);
		
		// if term is continually equal to zero call_site is not executed.
		if(term!=0 && call_site(host,"REST/approx?term=",term,reply) == 200){
			return ok;
		}
		// check for other calls...
		get_arg("all=",&term,argc, argv);
		
		if(term!=0 && call_site(host,"REST/allconcepts?tty=",term,reply) == 200){
			return ok;
		}
		
		get_arg("brands=",&term,argc, argv);

		if(term!=0 && call_site(host,"REST/brands?ingredientids=",term,reply) == 200){
			return ok;
		}
		
	}	
   	return 404;
}
// ============================================================================
// End of Source Code
// ============================================================================
