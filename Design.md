This is basic design document for the Plug-in

# Introduction #

> Any Chat Client which has support for plug-ins can be provided this language translation feature. Lingua-Franca plug-in would be basically a shared Object file or a dll file with core functions being
    * User Interface for preference setting
> > > `void lf_ui()`
    * Translate Outgoing messages
> > > `char *lf_translate_outgoing_mesg(char *mesg,char *buddy)`
    * Translate Incoming messages
> > > `char *lf_translate_incoming_mesg(char *mesg,char *buddy)`



> There are four basic modules in the plug-in
    * Chat client Specific Module
    * Plug-in Specific Modules
      * User Interface Module
      * Core Translation Module
      * XML Helper Module

## Chat Client Specific Module ##
> Skeleton of the chat client specific code would be
    * Initialize the plug-in
    * Load the plug-in into chat client
    * User to configure the preferences
    * Wait for the signal "message-typed"
      * Call translate\_outgoing\_message method  when event occurs
      * Send the translated message to the concerned buddy
    * Wait for the signal "message-received"
      * Call translate\_incoming\_message method when event occurs
      * Send the translated message back to chat client
    * Unload the plug-in gracefully when the chat client process is shutdown

> _**Note**_  The above design is highly specific to pidgin , more generic design will be made once we are ready to develop for other chat clients


## Plug-in Specific Modules ##
> Plug-in has three modules
    * User Interface Module
      * Initialize the Preferences window.
    * Core Translation Module
      * Translate Outgoing Messages
      * Translate Incoming Messages
    * XML Helper Module Provides Helper Methods
      * To save user preferences
      * To read the translation service config files

### User Interface ###

  1. Initialization of the User Interface the below are required
    * List of buddies available
> > > This can be got from the layer above , i.e from Chat Client API.
    * List of available Languages
> > > This can be got from the XML Helper
    * List of available Translation Services
> > > This can be got from the XML Helper
    * XML File Location.
> > > The directory path to preferences XML
  1. Preferences Window Layout and Components
    1. General Tab
      * Incoming Language Preference
        * An Incoming Lang Preference Label
        * A Combo Box containing list of available languages
      * Outgoing Language Preference
        * An Outgoing Lang Preference Label
        * A Combo Box containing list of available languages
    1. Buddy Config Tab
      * A table having three columns
        * Buddy Name
        * List of available languages
        * Enable/ Disable button
    1. Miscellaneous Tab
      * Translation Service
        * Translation service label
        * Combo Box containing list of available Servers
      * View Translation
        * View Translation label
        * Enable / Disable Button
      * Time Out
        * Time out label
        * Spin button for counter
    1. Ok , Apply , Cancel Buttons
      * A Panel containing Ok , Apply ,Cancel Buttons common to all the three tabs
  1. Call back Methods and Saving Preferences
    * Call back methods for all the above widgets should be provided
    * Changes in the any of the preferences to be updated in the memory and written to an XML file only when "Apply" or "Ok" are clicked
  1. A DTD of the XML file is provided in the XML Helper Module Section


### Core Translation Module ###


  1. Translate Outgoing Messages
```
              char *lf_translate_outgoing_mesg(char *mesg,char *buddy)
              {
                 char *to_lang ;
                 char *from_lang;
                 char *translated_mesg;
                /* determine outgoing language preference for this buddy*/
                     to = get_lang_pref(buddy) 

                /* determine the language of the "mesg" */
                     from = determine_lang(mesg)

                /* call the core translate method */
                     translated_mesg = translate (mesg,from ,to);             
               }
```
  1. Translate Incoming Messages
```
              char *lf_translate_incoming_mesg(char *mesg,char *buddy)
              {
                 char *to_lang ;
                 char *from_lang;
                 char *translated_mesg;
                /* determine incoming language preference */
                     to = get_incoming_lang_pref() 

                /* determine the language of the "mesg" */
                     from = determine_lang(mesg)

                /* call the core translate method */
                     translated_mesg = translate (mesg,from ,to);             
               }
```
  1. Other Methods
    1. Translate method needs the following inputs
> > > `char *translate(char *mesg,char *to, char *from)`
    1. Determine Language Method
```
                       char *determine_lang(char *mesg)
                       {
                          List avail_translations;
                          char *translated_mesg;
                           while ( list->next != NULL)
                            {
                              avail = list->avail;
                              translated_mesg = translate(mesg,avail->from,avail->to);
                              if (strcmp(mesg,translated_mesg) !=0)
                               {
                                  printf("Translation Available");
                                  return avail->from;
                               }
                             }
                           return "NA";
                         }
```

### XML Helper Module ###

> This module basically does the required XML processing , it has two sub modules
  * XML - User Preferences
> > The User Preferences  will be saved in an XML file , the DTD of this file will be soon given
  * XML - Translate
> > The translation service specific details are provided in an XML File , the DTD will be soon posted .