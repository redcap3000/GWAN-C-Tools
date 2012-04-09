// ============================================================================
// This is a Servlet sample for the G-WAN Web Server (http://www.trustleap.com)
// ----------------------------------------------------------------------------
// loadfile.c:
//				Loads a file based on a GET query without headers, 
//				returns 404 if id is not found. Designed to be used like an api.
//				Check out json_load.c for an example of calling with  xbuf_frurl()								
//	Ronaldo Barbachano 4-6-2012				
// 				
// noheaders.c: 
				
//				build a complete HTTP reply -without any response header
//              (this useful to send a raw JSON reply for example)
//
//              Returning an INVALID HTTP status code in the 1-99 range 
//              (inclusive) will prevent G-WAN from injecting the missing
//              response HTTP Headers.
// ----------------------------------------------------------------------------
#include "gwan.h" // G-WAN exported functions

// ----------------------------------------------------------------------------
// imported functions:
//   get_reply(): get a pointer on the 'reply' dynamic buffer from the server
//    xbuf_cat(): formatted strcat() (a la printf) in a given dynamic buffer 
// ----------------------------------------------------------------------------

// pratice with this script and see if you cannot include it in others and make multiple call
int main(int argc, char *argv[])
{
	xbuf_t *reply = get_reply(argv);
	   
    xbuf_t f;
	
	char *file = 0, str[1024];
	char *wwwpath = (char*)get_env(argv, WWW_ROOT);
	
	if(argc==1){
	// get the id argument..
		get_arg("id=", &file, argc, argv);
		// build string path with wwwpath and s_snprintf, switch out 'json' with something else..
		if(file!=0){
			s_snprintf(str, 1023, "%s/%s", wwwpath, file);
	
		// init dynamic buffer
			xbuf_init(&f);
	
		// load file with assembled str
	    	xbuf_frfile(&f, str);
	
		// load succeeded?
		    if(f.len){                                       
		    	xbuf_ncat(reply,f.ptr, f.len);
		   	 	xbuf_free(&f);
				return 200;
			}
			xbuf_free(&f);
		}	
	}
	// return error because something went wrong (invalid 'id')
   	return 1;
}
// ============================================================================
// End of Source Code
// ============================================================================

