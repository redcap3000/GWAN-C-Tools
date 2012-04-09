// ============================================================================
// This is a Servlet sample for the G-WAN Web Server (http://www.trustleap.com)
// ----------------------------------------------------------------------------
// json_load.c: example uses loadfile.c to load a json text file into the buffer
// Ronaldo Barbachano 4-6-12
// ----------------------------------------------------------------------------
#include "gwan.h" // G-WAN exported functions
#include <string.h>

int main(int argc, char *argv[])
{
	xbuf_t *reply = get_reply(argv);
	// use ctx incase you want to use another script to process the loaded json
	xbuf_ctx buf2;

	char *text=0, *record = 0, *jsonCall=0, str[1024];
	
	// maybe just parse the uri
	if( argc == 1){

		// we're using a ctx buffer (for some reason) Maybe because of the xbuf_frurl (which is basically curl!)
	   xbuf_init(&buf2);

	   // get directive .. probably error correct and return error status if this fails?
	   get_arg("id=", &record, argc, argv);

	   if(record==0){
		return 404;
		}

	   // build uri to call get_arg(id) using modified noheaders to return files from the filesystem
	   s_snprintf(str, 1023, "?loadfile.c&id=%s", record);

	   // get json file using local script and record, alternativly load from a char or where 
	   jsonCall = xbuf_frurl(&buf2, "localhost", 8080, HTTP_GET,str, 500, 0);

	   // import from text
	   jsn_t *import = jsn_frtext(buf2.ptr, "import"); 

	   text = jsn_totext(&buf2, import, 1); 

	   // 1:formatted form , show that we have  run jsn_totext properly
	   xbuf_xcat(reply, "<pre>%s</pre>", text);

	   // assuming this -should- be disabled if you are continuing to use the ctx?
	   xbuf_free(&buf2);
	   return 1;
	}
	return 404;
   	
}
// ============================================================================
// End of Source Code
// ============================================================================
