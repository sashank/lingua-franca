#include "xml-translate.h"

void processNode(xmlTextReaderPtr reader)
{
  xmlChar *name, *value;

    name = xmlTextReaderName(reader);
    if (name == NULL)
        name = xmlStrdup(BAD_CAST "--");

    if(strcmp(name,"server_name") == 0 )
     {
        if( xmlTextReaderNodeType(reader) == 1 )
                xmlTextReaderRead(reader);

        if(xmlTextReaderNodeType(reader) == 3)
        {
                value = xmlTextReaderValue(reader);
                printf("Server Name is %s \n",value);
                xmlTextReaderRead(reader);
        }

        if( xmlTextReaderNodeType(reader) == 2 )
                xmlTextReaderRead(reader);
     }

    if(strcmp(name,"host_url") == 0 )
     {
        if( xmlTextReaderNodeType(reader) == 1 )
                xmlTextReaderRead(reader);

        if(xmlTextReaderNodeType(reader) == 3)
        {
                value = xmlTextReaderValue(reader);
                printf("host Name is %s \n",value);
  		/* specify URL to get */
  		curl_easy_setopt(curl, CURLOPT_URL, value);
                xmlTextReaderRead(reader);
        }

        if( xmlTextReaderNodeType(reader) == 2 )
                xmlTextReaderRead(reader);
     }

    if (strcmp(name,"post_option")==0)
    {
        printf("Attribute Count %d \n", xmlTextReaderAttributeCount(reader));
        printf("Post Name %s \n",xmlTextReaderGetAttribute(reader,(xmlChar *)"name"));
        printf("Post Value %s \n",xmlTextReaderGetAttribute(reader,(xmlChar *)"value"));
	char *post_name = xmlTextReaderGetAttribute(reader,(xmlChar *)"name");
        char *post_value = xmlTextReaderGetAttribute(reader,(xmlChar *)"value");
	add_post_options(post_name,post_value);
    }

    if (strcmp(name,"lang_pair")==0)
    {
	char *from = xmlTextReaderGetAttribute(reader,(xmlChar *)"from");
        char *to = xmlTextReaderGetAttribute(reader,(xmlChar *)"to");
        char *lp = xmlTextReaderGetAttribute(reader,(xmlChar *)"lp");
	add_lang_pair(from,to,lp);
    }

     
}

void xml_init(char *server)
{
    xmlTextReaderPtr xml_reader;
    int ret;

    xml_reader = xmlNewTextReaderFilename(server);
    if (xml_reader != NULL) {
        ret = xmlTextReaderRead(xml_reader);
        while (ret == 1) {
            processNode(xml_reader);
            ret = xmlTextReaderRead(xml_reader);
        }
        xmlFreeTextReader(xml_reader);
        if (ret != 0) {
            printf("%s : failed to parse\n", server);
        }
    } else {
        printf("Unable to open %s\n", server);
    }

}
