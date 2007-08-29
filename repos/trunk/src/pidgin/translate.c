#include "internal.h"
#include "debug.h"
#include "plugin.h"
#include "signals.h"
#include "version.h"
#include "../core/lingua-franca.h"

/** Plugin id : sashank (to guarantee uniqueness) */
#define TRANSLATE_PLUGIN_ID "lingua-franca"
#define VERSION 0.1


static void
sending_im_msg_cb(PurpleAccount *account, char *recipient, char **buffer, void *data)
{
        purple_debug_misc("lingua-franca", "sending-im-msg (%s, %s, %s)\n",
                                        purple_account_get_username(account), recipient, *buffer);
	lf_translate_outgoing(*buffer,recipient);
}


static void
received_im_msg_cb(PurpleAccount *account, char *sender, char *buffer,
                                   PurpleConversation *conv, PurpleMessageFlags flags, void *data)
{
        purple_debug_misc("lingua-franca", "received-im-msg (%s, %s, %s, %s, %d)\n",
                                        purple_account_get_username(account), sender, buffer,
                                        (conv != NULL) ? purple_conversation_get_name(conv) : "(null)", flags);
	lf_translate_incoming(*buffer,sender);
}


static gboolean
plugin_load(PurplePlugin *plugin)
{

	void *conv_handle     = purple_conversations_get_handle();

	purple_debug(PURPLE_DEBUG_INFO, "translate", "translate plugin loaded.\n");

        /* Get all Buddies*/
	
        /*Get the path to locate XML files */

        /*Initialise the plug-in */
	lf_init();

        /* Translate Message before it is sent */
	purple_signal_connect(conv_handle, "wrote-im-msg",
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

static PurplePluginUiInfo prefs_info = {
        lf_ui,
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
	PURPLE_PLUGIN_STANDARD,                             /**< type           */
	NULL,                                             /**< ui_requirement */
	0,                                                /**< flags          */
	NULL,                                             /**< dependencies   */
	PURPLE_PRIORITY_DEFAULT,                            /**< priority       */

	TRANSLATE_PLUGIN_ID,                                 /**< id             */
	N_("Translate Plugin"),                              /**< name           */
	VERSION,                                          /**< version        */
	                                                  /**  summary        */
	N_("This plugin is to translate chat text into different languages set by the user "),
	                                                  /**  description    */
	N_("This plugin is to translate chat text into different languages set by the user "),
	"sashank <krishna.sashank@gmail.org>",        /**< author         */
	"http://code.google.com/p/lingua-franca",                                     /**< homepage       */

	plugin_load,                                      /**< load           */
	plugin_unload,                                    /**< unload         */
	NULL,                                             /**< destroy        */

	NULL,                                             /**< ui_info        */
	NULL,                                             /**< extra_info     */
	NULL,
	NULL,
	/* Padding */
	NULL,
	NULL,
	NULL,
	NULL
};

static void
init_plugin(PurplePlugin *plugin)
{
}

PURPLE_INIT_PLUGIN(translate, init_plugin, info)
