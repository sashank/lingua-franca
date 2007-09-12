#include "../src/core/xml-translate.h"

int main(int argc, char **argv) {
    const char *filename = argv[1];
	xml_translate_init(argv[1]);
    GString *post = g_string_new(NULL);
    post =  get_post_string(argv[2],argv[3],argv[4]);
    printf("Lang pair = %s\n",get_lang_pref(argv[3],argv[4]));
    printf("Post String  %s \n" , post->str);
    printf("Get Server Name %s\n",get_server_name());
    printf("Get Response Prefix %s\n",get_response_prefix());
    printf("Get Response Suffix %s\n",get_response_suffix());
    printf("Host url %s\n",get_host_url());
}
