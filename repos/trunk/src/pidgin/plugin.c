/**
 * @file lingua-franca
 * @ingroup pidgin
 *
 * lingua-franca
 *
 * lingua-franca is the legal property of its developers, 
 * Please refer to the COPYRIGHT file distributed with this
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

#define PURPLE_PLUGINS
#include <account.h>
#include <debug.h>
#include <plugin.h>
#include <signals.h>
#include <version.h>
#include <blist.h>
#include "../ui/lingua-franca.h"
#include <glib.h>

#include <gtkplugin.h>
#include <gtkprefs.h>
#include <gtkutils.h>
#include <pidgin.h>

#define TRANSLATE_PLUGIN_ID "lingua-franca"
#define VERSION "0.1"
#define PLUGIN_STATIC_NAME  "lingua-franca"


 void
sending_im_msg_cb(PurpleAccount *account, char *recipient, char **buffer, void *data)
{
        purple_debug_misc("lingua-franca", "msg typed  to  %s is  %s)\n",
                                        recipient, *buffer);

	*buffer = lf_translate_outgoing(*buffer,recipient);

        purple_debug_misc("lingua-franca", "sending-im-msg (%s, %s, %s)\n",
                                        purple_account_get_username(account), recipient, *buffer);
}


static void
received_im_msg_cb(PurpleAccount *account, char *sender, char *buffer,
                                   PurpleConversation *conv, PurpleMessageFlags flags, void *data)
{
        purple_debug_misc("lingua-franca", "received-im-msg (%s, %s, %s, %s, %d)\n",
                                        purple_account_get_username(account), sender, buffer,
                                        (conv != NULL) ? purple_conversation_get_name(conv) : "(null)", flags);
	buffer = lf_translate_incoming(buffer,sender);

        purple_debug_misc("lingua-franca", "displaying im msg (%s, %s, %s, %s, %d)\n",
                                        purple_account_get_username(account), sender, buffer,
                                        (conv != NULL) ? purple_conversation_get_name(conv) : "(null)", flags);
}

static GList *
get_user_names()
{
        PurpleBlistNode *gnode,*cnode,*bnode;
        static GList *tmp = NULL;

        g_list_free(tmp);
        tmp = NULL;

                for(gnode = purple_get_blist()->root; gnode; gnode = gnode->next) {
                        if(!PURPLE_BLIST_NODE_IS_GROUP(gnode))
                                continue;
                        for(cnode = gnode->child; cnode; cnode = cnode->next) {
                                if(!PURPLE_BLIST_NODE_IS_CONTACT(cnode))
                                        continue;
                                for(bnode = cnode->child; bnode; bnode = bnode->next) {
                                        PurpleBuddy *buddy;

                                        if(!PURPLE_BLIST_NODE_IS_BUDDY(bnode))
                                                continue;

                                        buddy = (PurpleBuddy *)bnode;

                                        tmp = g_list_insert_sorted(tmp, buddy->name, (GCompareFunc)g_utf8_collate);
                                }
                        }
                }
        return tmp;
}

static gboolean
plugin_load(PurplePlugin *plugin)
{

	void *conv_handle     = purple_conversations_get_handle();
	purple_debug(PURPLE_DEBUG_INFO, "translate", "translate plugin loaded.\n");

        /* Get all Buddies*/
        /*Initialise the plug-in */
	lf_init(get_user_names());

        /* Translate Message before it is sent */
	purple_signal_connect(conv_handle, "sending-im-msg",
                                                plugin, PURPLE_CALLBACK(sending_im_msg_cb), NULL);

	/* Translate Message as it comes */
        purple_signal_connect(conv_handle, "received-im-msg",
                                                plugin, PURPLE_CALLBACK(received_im_msg_cb), NULL);


	return TRUE;
}

static gboolean
plugin_unload(PurplePlugin *plugin)
{
	purple_debug(PURPLE_DEBUG_INFO, "translate", "translate plugin unloaded.\n");
	return TRUE;
}

static GtkWidget *get_config_frame(PurplePlugin *plugin)
{
  purple_debug(PURPLE_DEBUG_INFO, "translate", "translate plugin ui creation .\n");
  printf("Getting get_config_frame");
  GtkWidget *ui = lf_ui();
  printf("Getting get_config_frame");
  return ui; 
}

static PidginPluginUiInfo ui_info = {
        get_config_frame,
        0,   /* page_num (Reserved) */
        NULL, /* frame (Reserved) */
        /* Padding */
        NULL,
        NULL,
        NULL,
        NULL
};

static PurplePluginInfo info =
{
	PURPLE_PLUGIN_MAGIC,
	PURPLE_MAJOR_VERSION,
	PURPLE_MINOR_VERSION,
	PURPLE_PLUGIN_STANDARD,                           /**< type           */
	PIDGIN_UI,				  /**< ui_requirement */
	0,                                                /**< flags          */
	NULL,                                             /**< dependencies   */
	PURPLE_PRIORITY_DEFAULT,                          /**< priority       */
	TRANSLATE_PLUGIN_ID,                              /**< id             */
	N_("lingua-Franca"),        /**< name           */
	VERSION,                                          /**< version        */
	                                                  /**  summary        */
	N_("This plugin is to translate chat text into different languages set by the user "),
	                                                  /**  description    */
	N_("This plugin is to translate chat text into different languages set by the user "),
	"sashank <krishna.sashank@gmail.org>",        	  /**< author         */
	"http://code.google.com/p/lingua-franca",         /**< homepage       */
	plugin_load,                                      /**< load           */
	plugin_unload,                                    /**< unload         */
	NULL,                                             /**< destroy        */

	&ui_info,                                             /**< ui_info        */
	NULL,                                             /**< extra_info     */
	NULL,
	NULL,
	/* Padding */
	NULL,
	NULL,
	NULL,
	NULL
};

static void init_plugin(PurplePlugin *plugin)
{
	purple_debug(PURPLE_DEBUG_INFO, "lingua-franca", "translate plugin inited.\n");
}

PURPLE_INIT_PLUGIN(PLUGIN_STATIC_NAME, init_plugin, info)

