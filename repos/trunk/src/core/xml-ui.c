/**
 * @file xml-ui.c
 * @ingroup core
 *
 * lingua-franca
 *
 * lingua-franca is the legal property of its developers, whose names are too numerous
 * to list here.  Please refer to the COPYRIGHT file distributed with this
 * source distribution.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "xml-ui.h"

#define MY_ENCODING "ISO-8859-1"

char *incoming_lang;
gboolean incoming_enabled ;
gboolean view_trans_enabled ;
char *trans_server;
int time_out ;
GList outgoingList;
char *lf_prefs_file;

typedef struct _LangPrefStruct {
  char *buddy;
  char *lang;
  gboolean enabled;
}LangPref;

void add_outgoing_pref(char *buddy,char *lang,char *enable)
{
  LangPref *lp;

  g_return_if_fail(buddy !=NULL);
  g_return_if_fail(lang !=NULL);
  g_return_if_fail(enable !=NULL);

  lp = g_new0(LangPref , 1);
  lp->buddy = g_strdup(buddy);
  lp->lang  = g_strdup(lang);
  if ( strcmp(enable , "on") == 0 )
  	lp->enabled = TRUE;
  else 
	lp->enabled = FALSE;

  outgoingList = g_list_append(outgoingList,lp);
}

void *add_outgoing_pref_xml(gpointer data,gpointer userdata)
{
     /* Start an element named "outgoing_pref" as child of lang_pref. */
       xmlTextWriterStartElement((xmlTextWriterPtr *)userdata, BAD_CAST "outgoing_pref");

    /* Add an attribute with name "who" and value "ALL" to incoming_pref. */
       xmlTextWriterWriteAttribute((xmlTextWriterPtr *)userdata, BAD_CAST "who", BAD_CAST (LangPref)data->who );

    /* Add an attribute with name "lang" and value  to incoming_pref. */
       xmlTextWriterWriteAttribute((xmlTextWriterPtr *)userdata, BAD_CAST "lang", BAD_CAST (LangPref)data->lang);

    /* Add an attribute with name "enable" and value  to incoming_pref. */
       xmlTextWriterWriteAttribute((xmlTextWriterPtr *)userdata, BAD_CAST "enable", BAD_CAST (LangPref)data->enabled );

    /* Close the element named "incoming_pref" */
       xmlTextWriterEndElement((xmlTextWriterPtr *)userdata);

}
void processNode(xmlTextReaderPtr xml_reader)
{
    xmlChar *name, *value;

    name = xmlTextReaderName(reader);
    if (name == NULL)
        name = xmlStrdup(BAD_CAST "--");
    
    if(strcmp(name,"incoming_pref") == 0 )
     {
	char *who = xmlTextReaderGetAttribute(reader,(xmlChar *)"who");
        char *lang = xmlTextReaderGetAttribute(reader,(xmlChar *)"lang");
        char *enable = xmlTextReaderGetAttribute(reader,(xmlChar *)"enable");
        if ( strcmp(enable,"on") == 0 ) 
          incoming_enabled = TRUE ;
        else
          incoming_enabled = FALSE ;

        incoming_lang = strdup(lang);
     }
    else if(strcmp(name,"outgoing_pref") == 0 )
     {
        char *who = xmlTextReaderGetAttribute(reader,(xmlChar *)"who");
        char *lang = xmlTextReaderGetAttribute(reader,(xmlChar *)"lang");
        char *enable = xmlTextReaderGetAttribute(reader,(xmlChar *)"enable");

        add_outgoing_pref(who,lang,enable);
     }
     else if(strcmp(name,"trans_server") == 0 )
     {
	if( xmlTextReaderNodeType(reader) == 1 )
                xmlTextReaderRead(reader);

        if(xmlTextReaderNodeType(reader) == 3)
        {
                value = xmlTextReaderValue(reader);
                trans_server = g_strdup(value);
                xmlTextReaderRead(reader);
        }

        if( xmlTextReaderNodeType(reader) == 2 )
                xmlTextReaderRead(reader);

              /*  printf("Server Name is %s \n",trans_server); */
     }
     else if(strcmp(name,"time_out") == 0)
     {
        if( xmlTextReaderNodeType(reader) == 1 )
                xmlTextReaderRead(reader);

        if(xmlTextReaderNodeType(reader) == 3)
        {
                value = xmlTextReaderValue(reader);
                trans_server = g_strdup(value);
                xmlTextReaderRead(reader);
        }

        if( xmlTextReaderNodeType(reader) == 2 )
                xmlTextReaderRead(reader);

        time_out = atoi(value);
              /*  printf("Time out  is %d \n",time_out); */
     }
     else if(strcmp(name,"view_trans") == 0)  
     {
	if( xmlTextReaderNodeType(reader) == 1 )
                xmlTextReaderRead(reader);

        if(xmlTextReaderNodeType(reader) == 3)
        {
                value = xmlTextReaderValue(reader);
                view = g_strdup(value);
                xmlTextReaderRead(reader);
        }

        if( xmlTextReaderNodeType(reader) == 2 )
                xmlTextReaderRead(reader);

        if ( strcmp(view,"on") == 0 )
          view_trans_enabled = TRUE ;
        else
          view_trans_enabled = FALSE ;
     }
    
}
                                                                     
void xml_ui_init(char *dir)
{

 /* Read from file 
    If file not found , create a new file */
    xmlTextReaderPtr xml_reader;
    int ret;

    lf_prefs_file = g_build_filename(dir,"lf_prefs.xml");

    xml_reader = xmlNewTextReaderFilename(lf_prefs_file);
    if (xml_reader != NULL) {
        ret = xmlTextReaderRead(xml_reader);
        while (ret == 1) {
            processNode(xml_reader);
            ret = xmlTextReaderRead(xml_reader);
        }
        xmlFreeTextReader(xml_reader);
        if (ret != 0) {
            printf("%s : failed to parse\n", lf_prefs_file);
        }
    } else {
        printf("Unable to open %s\n", lf_prefs_file);
    }
}

char *get_incoming_lang_pref()
{
 if(incoming_enabled == TRUE)
 	return incoming_lang;
 else
 	return "No Translation";
}

char *get_outgoing_lang_pref(char *buddy)
{
   int count = g_list_length(outgoingList);
   LangPref lp;
   gboolean exists = FALSE ;
   for ( int i=0 ; i < count ; i++)
   {
        lp = g_list_nth_data(outgoingList);
        if ( strcmp(lp->buddy,buddy) == 0 )
        {
          exists = TRUE ;
	  return lp->lang;
        }
   }
    if ( exists == FALSE)
	return "No Translation" ;

}
gboolean get_pref_status(char *buddy)
{
   int count = g_list_length(outgoingList);
   LangPref lp;
   gboolean exists = FALSE ;
   for ( int i=0 ; i < count ; i++)
   {
        lp = g_list_nth_data(outgoingList);
        if ( strcmp(lp->buddy,buddy) == 0 )
          exists = lp->enabled;
   }
  return exists ;
}
void set_incoming_lang_pref(char *lang)
{
  incoming_lang = lang ;
  incoming_enabled = TRUE ;
}

void set_outgoing_lang_pref(char *buddy,char  *lang)
{
   int count = g_list_length(outgoingList); 
   LangPref lp;
   gboolean exists = FALSE ;
   for ( int i=0 ; i < count ; i++)
   {
	lp = g_list_nth_data(outgoingList);	
	if ( strcmp(lp->buddy,buddy) == 0 ) 
	{
	  lp->lang = lang ;
	  exists = TRUE ;
        }
   }
    if ( exists == FALSE)
	add_outgoing_pref(buddy,lang,"on");
}

void set_view_translation(int  state)
{
   
}

void set_buddy_toggle(gchar *buddy)
{
   int count = g_list_length(outgoingList);
   LangPref lp;
   for ( int i=0 ; i < count ; i++)
   {
        lp = g_list_nth_data(outgoingList);
        if ( strcmp(lp->buddy,buddy) == 0 )
        {
          if(lp->enabled == TRUE)	
          	lp->enabled = FALSE ;
	  else
          	lp->enabled = TRUE ;
        }
   }
}

void set_trans_server(char *server)
{
   trans_server = server ;
}

void set_time_out(int timeout)
{
   time_out = timeout ;
}

void xml_ui_unload()
{

}

void save_preferences()
{
  /* This will write the Preferences into File */
      int rc;
    xmlTextWriterPtr writer;
    xmlBufferPtr buf;
    xmlChar *tmp;
    FILE *fp;

    /* Create a new XML buffer, to which the XML document will be
     * written */
    buf = xmlBufferCreate();
    if (buf == NULL) {
        printf(" Error creating the xml buffer\n");
        return;
    }

    /* Create a new XmlWriter for memory, with no compression.
     * Remark: there is no compression for this kind of xmlTextWriter */
    writer = xmlNewTextWriterMemory(buf, 0);
    if (writer == NULL) {
        printf(" Error creating the xml writer\n");
        return;
    }

    /* Start an element named "lingua-franca". Since this is the first
     * element, this will be the root element of the document. */
       xmlTextWriterStartElement(writer, BAD_CAST "lingua_franca");

    /* Start an element named "lang_pref" as child of lingua_franca. */
       xmlTextWriterStartElement(writer, BAD_CAST "lang_pref");

    /* Start an element named "incoming_pref" as child of lang_pref. */
       xmlTextWriterStartElement(writer, BAD_CAST "incoming_pref");

    /* Add an attribute with name "who" and value "ALL" to incoming_pref. */
       xmlTextWriterWriteAttribute(writer, BAD_CAST "who", BAD_CAST "ALL");
    
    /* Add an attribute with name "lang" and value  to incoming_pref. */
       xmlTextWriterWriteAttribute(writer, BAD_CAST "lang", BAD_CAST incoming_lang);

    /* Add an attribute with name "enable" and value  to incoming_pref. */
       xmlTextWriterWriteAttribute(writer, BAD_CAST "enable", BAD_CAST incoming_enabled);

    /* Close the element named "incoming_pref" */
       xmlTextWriterEndElement(writer);

    /* Add the outgoing preferences */
      g_list_foreach(outgoingList,(GFunc)add_outgoing_pref_xml,(gpointer)writer);

    /* Close the element named "lang_pref" */
       xmlTextWriterEndElement(writer);

    /* Add an element named "trans_server" and value */
       xmlTextWriterWriteElement(writer, BAD_CAST "trans_server", BAD_CAST trans_server );

    /* Add an element named "view_trans" and value */
       if(view_trans_enabled == TRUE )
          xmlTextWriterWriteElement(writer, BAD_CAST "view_trans", BAD_CAST "on" );
       else if(view_trans_enabled == FALSE )
          xmlTextWriterWriteElement(writer, BAD_CAST "view_trans", BAD_CAST "off" );

    /* Add an element named "time_out" and value */ 
          xmlTextWriterWriteElement(writer, BAD_CAST "time_out", BAD_CAST itoa(time_out)  );

    /* Close the element named "lingua_franca" */
       xmlTextWriterEndElement(writer);
	
       xmlFreeTextWriter(writer);

    /* Now write to a file */
       fp = g_fopen(lf_prefs_file, "w");

       if (fp == NULL) {
        printf(" Could not open file %s \n",lf_prefs_file);
        return;
       }

      fprintf(fp, "%s", (const char *) buf->content);

      fclose(fp);

    xmlBufferFree(buf);

}

