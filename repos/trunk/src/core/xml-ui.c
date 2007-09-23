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
GList *outgoingList;
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

void processXml(xmlTextReaderPtr reader)
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
        char *view;
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
                                                                     
void xml_ui_init(char *filename)
{
    printf("xml-ui.c: xml_ui_init entered \n");
 /* Read from file 
    If file not found , create a new file */
    xmlTextReaderPtr xml_reader;
    int ret;

    printf("xml-ui.c: file name %s \n",filename);
    lf_prefs_file = strdup(filename);

    xml_reader = xmlNewTextReaderFilename(lf_prefs_file);
    if (xml_reader != NULL) {
        ret = xmlTextReaderRead(xml_reader);
        while (ret == 1) {
            processXml(xml_reader);
            ret = xmlTextReaderRead(xml_reader);
        }
        xmlFreeTextReader(xml_reader);
        if (ret != 0) {
            printf("%s : failed to parse\n", lf_prefs_file);
        }
    } else {
        printf("Unable to open %s\n", lf_prefs_file);
    }
    printf("xml-ui.c:  xml_ui_init exiting \n");
}

char *get_incoming_lang_pref()
{
 printf("xml-ui.c:  get_incoming_lang_pref entered\n");
 
 if(incoming_enabled == TRUE)
 {
   printf("xml-ui.c:  incoming_enabled \n");
   printf("xml-ui.c:  lang is %s \n",incoming_lang);
   printf("xml-ui.c:  get_incoming_lang_pref exiting \n");
 	return incoming_lang;
 }
 else
 {
   printf("xml-ui.c:  incoming not enabled \n");
   printf("xml-ui.c:  get_incoming_lang_pref exiting \n");
 	return "No Translation";
 }
}

char *get_outgoing_lang_pref(char *buddy)
{
 printf("xml-ui.c:  get_outgoing_lang_pref entered\n");
   printf("xml-ui.c:  Buddy  is %s \n",buddy);
   int count = g_list_length(outgoingList);
 printf("xml-ui.c:  buddy list count %d \n",count);
   int i ;
   LangPref *lp;
   gboolean exists = FALSE ;
   char *lang ;
   for ( i=0 ; i < count ; i++)
   {
        lp = (LangPref)g_list_nth_data(outgoingList,i);
        if ( strcmp(lp->buddy,buddy) == 0 )
        {
          exists = TRUE ;
	  lang = strdup(lp->lang);
        }
   }
    if ( exists == FALSE)
	  lang = strdup("No Translation");

   printf("xml-ui.c: Outgoing Lang is %s \n",lang);
   printf("xml-ui.c:  get_outgoing_lang_pref exiting\n");
   return lang;
}
gboolean get_pref_status(char *buddy)
{
   printf("xml-ui.c:  get_outgoing_lang_pref status entering\n");
   printf("xml-ui.c:  Buddy  is %s \n",buddy);
   int count = g_list_length(outgoingList);
   printf("xml-ui.c:  buddy list count %d \n",count);
   int i ;
   LangPref *lp;
   char *lang;
   gboolean exists = FALSE ;
   for ( i=0 ; i < count ; i++)
   {
        lp = (LangPref)g_list_nth_data(outgoingList,i);
        if ( strcmp(lp->buddy,buddy) == 0 )
          exists = lp->enabled;
   }
   if ( exists == TRUE)
    printf("xml-ui.c: pref  status is enabled\n");
   else
    printf("xml-ui.c: pref  status is not enabled\n");
  printf("xml-ui.c:  get_outgoing_lang_pref status exiting\n");
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
   int i ;
   LangPref *lp;
   gboolean exists = FALSE ;
   for ( i=0 ; i < count ; i++)
   {
	lp = (LangPref)g_list_nth_data(outgoingList,i);	
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
   int i ;
   LangPref *lp;
   for ( i=0 ; i < count ; i++)
   {
        lp = (LangPref)g_list_nth_data(outgoingList,i);
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

  g_free(outgoingList);
  g_free(lf_prefs_file);
}

void save_preferences()
{
  /* This will write the Preferences into File */
        printf("xml-ui.c: save_preferences entered \n ");
      int rc;
    xmlTextWriterPtr writer;
    xmlBufferPtr buf;
    xmlChar *tmp;
    FILE *fp;
     int count = g_list_length(outgoingList);
        printf("xml-ui.c: Out going Prefs Count is %d \n",count);
     int i ;
     LangPref *lp;

    /* Create a new XML buffer, to which the XML document will be
     * written */
    buf = xmlBufferCreate();
    if (buf == NULL) {
        printf(" Error creating the xml buffer\n");
        return;
    }

     printf("xml-ui.c: xml buffer created ");
    /* Create a new XmlWriter for memory, with no compression.
     * Remark: there is no compression for this kind of xmlTextWriter */
    writer = xmlNewTextWriterMemory(buf, 0);
    if (writer == NULL) {
        printf(" Error creating the xml writer\n");
        return;
    }
        printf("xml-ui.c: xml writer created ");

    /* Start an element named "lingua-franca". Since this is the first
     * element, this will be the root element of the document. */
       rc = xmlTextWriterStartElement(writer, BAD_CAST "lingua_franca");
       if ( rc < 0 )
       {
	  printf(" cud not write lingua_franca \n");
       }

    /* Start an element named "lang_pref" as child of lingua_franca. */
       rc = xmlTextWriterStartElement(writer, BAD_CAST "lang_pref");
       if ( rc < 0 )
       {
	  printf(" cud not write lang_pref \n");
       }

    /* Start an element named "incoming_pref" as child of lang_pref. */
      rc =  xmlTextWriterStartElement(writer, BAD_CAST "incoming_pref");
       if ( rc < 0 )
       {
	  printf(" cud not write incoming_pref \n");
       }

    /* Add an attribute with name "who" and value "ALL" to incoming_pref. */
      rc =  xmlTextWriterWriteAttribute(writer, BAD_CAST "who", BAD_CAST "ALL");
       if ( rc < 0 )
       {
	  printf(" cud not write Attributes 1 \n");
       }
    
    /* Add an attribute with name "lang" and value  to incoming_pref. */
       rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "lang", BAD_CAST incoming_lang);
       if ( rc < 0 )
       {
	  printf(" cud not write Attributes 2 \n");
       }

    /* Add an attribute with name "enable" and value  to incoming_pref. */
       rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "enable", BAD_CAST incoming_enabled);
       if ( rc < 0 )
       {
	  printf(" cud not write Attributes 3  \n");
       }

    /* Close the element named "incoming_pref" */
       rc = xmlTextWriterEndElement(writer);
       if ( rc < 0 )
       {
	  printf(" cud not write lingua_franca \n");
       }

      printf("xml-ui.c: Before Outgoing ");
    /* Add the outgoing preferences */
     for ( i=0 ; i < count ; i++)
     {
       lp = (LangPref)g_list_nth_data(outgoingList,i);

       /* Start an element named "outgoing_pref" as child of lang_pref. */
       xmlTextWriterStartElement(writer, BAD_CAST "outgoing_pref");

       /* Add an attribute with name "who" and value "ALL" to incoming_pref. */
       xmlTextWriterWriteAttribute(writer, BAD_CAST "who", BAD_CAST lp->buddy );

       /* Add an attribute with name "lang" and value  to incoming_pref. */
       xmlTextWriterWriteAttribute(writer, BAD_CAST "lang", BAD_CAST lp->lang);

       /* Add an attribute with name "enable" and value  to incoming_pref. */
       xmlTextWriterWriteAttribute(writer, BAD_CAST "enable", BAD_CAST lp->enabled );

       /* Close the element named "outgoing_pref" */
       xmlTextWriterEndElement(writer);

     }

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
          char *timeout ;
          sprintf(timeout,"%d",time_out);
          xmlTextWriterWriteElement(writer, BAD_CAST "time_out", BAD_CAST timeout  );

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

      printf("xml-ui.c: save_preferences exiting ");
}

