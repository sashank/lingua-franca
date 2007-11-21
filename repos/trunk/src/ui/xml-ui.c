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

char *view_trans_enabled ;
char *trans_server;
int time_out ;
GList *lpList = NULL;
char *lf_prefs_file;

typedef struct _LangPrefStruct {
  char *buddy;
  char *lang;
  char *enabled; /* On or Off */
}LangPref;

void add_lang_pref(char *buddy,char *lang,char *enable)
{
   int count = g_list_length(lpList);
   int i ;
   LangPref *lp;
   gboolean exists = FALSE ;

   g_return_if_fail(buddy !=NULL);
   g_return_if_fail(lang !=NULL);
   g_return_if_fail(enable !=NULL);

   for (i = 0 ; i < count ;i++)
   {
        lp = (LangPref *)g_list_nth_data(lpList,i);
        if (NULL != lp)
        {
        	if (( strcmp(lp->buddy,buddy) == 0 ) )
        	{
          		lp->lang = g_strdup(lang) ;
          		exists = TRUE ;
          		break;
        	}
	}
   }
    if ( exists == FALSE)
    {
	 lp = g_new0(LangPref , 1);
  	 lp->buddy = g_strdup(buddy);
  	 lp->lang  = g_strdup(lang);
         lp->enabled = g_strdup(enable);
     	 lpList = g_list_append(lpList,lp);
    }
}
void view_lps()
{
   int count = g_list_length(lpList);
   int i ;
   LangPref *lp;
   for (i = 0 ; i < count ;i++)
   {
        lp = (LangPref *)g_list_nth_data(lpList,i);
	if ( NULL != lp)
        {
        	printf("Buddy is %s \n",lp->buddy);
        	printf("Lang is %s \n",lp->lang);
        	printf("Enabled is %s \n",lp->enabled);
	}
   }
}
void processXml(xmlTextReaderPtr reader)
{
    xmlChar *name, *value;

    name = xmlTextReaderName(reader);
    if (name == NULL)
        name = xmlStrdup(BAD_CAST "--");
    
    if(strcmp(name,"lang_pref") == 0 )
     {
        char *who = xmlTextReaderGetAttribute(reader,(xmlChar *)"who");
        char *lang = xmlTextReaderGetAttribute(reader,(xmlChar *)"lang");
        char *enable = xmlTextReaderGetAttribute(reader,(xmlChar *)"enable");

        add_lang_pref(who,lang,enable);
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
                view_trans_enabled = g_strdup(value);
                xmlTextReaderRead(reader);
        }

        if( xmlTextReaderNodeType(reader) == 2 )
                xmlTextReaderRead(reader);

     }
    
}
                                                                     
void xml_ui_init()
{
    printf("xml-ui.c: xml_ui_init entered \n");

    gboolean load_default = TRUE ;

   /* check for lf_prefs xml */
   gchar *user_home = g_get_home_dir();
   gchar *lf_dir = g_build_filename(user_home,".lf",NULL);
   lf_prefs_file = g_build_filename(lf_dir,"lf_prefs.xml",NULL);
   printf("xml-ui.c: file is %s \n",lf_prefs_file);
   /* If File does not exists make dir and return */
   if(!g_file_test(lf_prefs_file,G_FILE_TEST_EXISTS))
   {
      g_mkdir(lf_dir,0755);
      printf("xml-ui.c:  xml_ui_init exiting \n");
   }

   /* If File exists Read from file */
    xmlTextReaderPtr xml_reader;
    int ret;

    xml_reader = xmlNewTextReaderFilename(lf_prefs_file);
    if (xml_reader != NULL) 
     {
        ret = xmlTextReaderRead(xml_reader);
        while (ret == 1) 
        {
            processXml(xml_reader);
            ret = xmlTextReaderRead(xml_reader);
        }
        load_default = FALSE ;
        xmlFreeTextReader(xml_reader);
        if (ret != 0) 
            printf("%s : failed to parse\n", lf_prefs_file);
    } 
    else {
        printf("Unable to open %s\n", lf_prefs_file);
    }

    if (load_default == TRUE )
    {
    	/* Set default values */
        printf("Loading default \n ");
    	time_out = -1;
    	trans_server = "google";
    	view_trans_enabled = "No";
    	add_lang_pref("MY_LANG","None","on");
    	add_lang_pref("ALL","None","on");
    }
   
    printf("xml-ui.c:  xml_ui_init exiting \n");
}

char *get_lang_pref(char *buddy)
{
 printf("xml-ui.c:  get_lang_pref entered\n");
/* printf("xml-ui.c:  Buddy  is %s \n",buddy);*/
 int count = g_list_length(lpList);
   int i ;
   LangPref *lp;
   gboolean exists = FALSE ;
   char *lang ;
   for ( i=0 ; i < count ; i++)
   {
        lp = (LangPref *)g_list_nth_data(lpList,i);
        if ( strcmp(lp->buddy,buddy) == 0 )
        {
          exists = TRUE ;
	  lang = strdup(lp->lang);
        }
   }
    if ( exists == FALSE)
	  lang = strdup("None");

   printf("xml-ui.c:  Lang is %s \n",lang);
   printf("xml-ui.c:  get_lang_pref exiting\n");
   return lang;
}
char *get_pref_status(char *buddy)
{
   printf("xml-ui.c:  get_lang_pref status entering\n");
   printf("xml-ui.c:  Buddy  is %s \n",buddy);
   int count = g_list_length(lpList);
   printf("xml-ui.c:  buddy list count %d \n",count);
   int i ;
   LangPref *lp;
   char *lang;
   char *exists = "Off";
   for ( i=0 ; i < count ; i++)
   {
        lp = (LangPref *)g_list_nth_data(lpList,i);
        if ( strcmp(lp->buddy,buddy) == 0 )
          exists = lp->enabled;
   }
   if ( strcmp(exists,"On"))
    printf("xml-ui.c: pref  status is enabled\n");
   else
    printf("xml-ui.c: pref  status is not enabled\n");
  printf("xml-ui.c:  get_lang_pref status exiting\n");
  return exists ;
}

void set_lang_pref(char *buddy,char  *lang)
{
   printf("Set lang pref %s,%s\n",buddy,lang);
   add_lang_pref(buddy,lang,"on");
}

void set_view_translation(int  state)
{
   
}

void set_buddy_toggle(gchar *buddy)
{
   int count = g_list_length(lpList);
   int i ;
   LangPref *lp;
   for ( i=0 ; i < count ; i++)
   {
        lp = (LangPref *)g_list_nth_data(lpList,i);
        if ( strcmp(lp->buddy,buddy) == 0 )
        {
          if(lp->enabled == "On")	
          	lp->enabled = "Off" ;
	  else
          	lp->enabled = "On" ;
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

  g_free(lpList);
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
        printf("xml-ui.c: xml writer created ");

    /* Start an element named "lingua-franca". Since this is the first
     * element, this will be the root element of the document. */
       rc = xmlTextWriterStartElement(writer, BAD_CAST "lingua_franca");
       if ( rc < 0 )
       {
	  printf(" cud not write lingua_franca \n");
       }

    /* Start an element named "lang_prefs" as child of lingua_franca. */
       rc = xmlTextWriterStartElement(writer, BAD_CAST "lang_prefs");
       if ( rc < 0 )
       {
	  printf(" cud not write lang_prefs \n");
       }

    /* Add the Lang preferences */
     int count = g_list_length(lpList);
     int i ;
     LangPref *lp;
     for ( i=0 ; i < count ; i++)
     {
       
       lp = (LangPref *)g_list_nth_data(lpList,i);
       if (lp == NULL)
	continue;

       /* Start an element named "lang_pref" as child of lang_prefs. */
       xmlTextWriterStartElement(writer, BAD_CAST "lang_pref");

       /* Add an attribute with name "who" and value "ALL" to incoming_pref. */
       xmlTextWriterWriteAttribute(writer, BAD_CAST "who", BAD_CAST lp->buddy );

       /* Add an attribute with name "lang" and value  to incoming_pref. */
       xmlTextWriterWriteAttribute(writer, BAD_CAST "lang", BAD_CAST lp->lang);

       /* Add an attribute with name "enable" and value  to incoming_pref. */
       xmlTextWriterWriteAttribute(writer, BAD_CAST "enable", BAD_CAST lp->enabled );

       /* Close the element named "lang_pref" */
       xmlTextWriterEndElement(writer);

     }

    /* Close the element named "lang_prefs" */
       xmlTextWriterEndElement(writer);

    /* Add an element named "trans_server" and value */
       xmlTextWriterWriteElement(writer, BAD_CAST "trans_server", BAD_CAST trans_server );

    /* Add an element named "view_trans" and value */
       xmlTextWriterWriteElement(writer, BAD_CAST "view_trans", BAD_CAST view_trans_enabled );

    /* Add an element named "time_out" and value */ 
          char *timeout ;
          sprintf(timeout,"%d",time_out);
          xmlTextWriterWriteElement(writer, BAD_CAST "time_out", BAD_CAST timeout  );

    /* Close the element named "lingua_franca" */
       xmlTextWriterEndElement(writer);
	
       xmlFreeTextWriter(writer);

    /* Now write to a file */
       fp = fopen(lf_prefs_file, "w");

       if (fp == NULL) {
        printf(" Could not open file %s \n",lf_prefs_file);
        return;
       }

      fprintf(fp, "%s", (const char *) buf->content);

      fclose(fp);

     /* Don't mess with this code */
      xmlBufferFree(buf);
      lpList = NULL;
      xml_ui_init();

     /* Clean Up and reload 
      xml_ui_unload();
      xml_ui_init();*/
      printf("xml-ui.c: save_preferences exiting ");
      return ;
}

