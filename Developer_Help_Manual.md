This is lingua-franca plug-in developers Help Document

# Introduction #

> This document contains
    * Libraries required to develop the plug-in
    * Project membership and code checking
    * Files and Directory Structure
    * Development Cycle


# Prerequisites #

> Install the below for the development of plug-in
    * GTK , Glib
> > > http://www.gtk.org/download/
    * libxml2  the latest one from xmlsoft or  freshmeat or icewalkers
> > > http://xmlsoft.org/downloads.html
> > > http://www.icewalkers.com/Linux/Software/510250/libxml2.html
    * libcurl  the latest one
> > > http://curl.haxx.se/download.html
    * pidgin the latest one
> > > http://www.pidgin.im/download/source/
    * purple plugin pack ( optional)
> > > http://plugins.guifications.org/trac/downloads
    * the subversion client
> > > http://subversion.tigris.org/getting_subversion.html


> If you wish to develop plug-in for any other chat client , then it is obvious that instead of pidgin you need to download your chat client

# Project Member Ship and Code Check out #

> Code base is located on http://code.google.com/p/lingua-franca

### Working Copy ###
> If you wish your code changes to appear in main stream code
    * Send a email request to any of the project owners, if they are interested they will add you as a project member.
    * Check out the code using the below command

> _svn checkout https://lingua-franca.googlecode.com/svn/repos/trunk/  lingua-franca –username user_

> You should be prompted for a password , supply the GoogleCode.com password you can find yours logging here
> http://code.google.com/hosting/settings

### Read Only Copy ###
> If you wish to have a read only copy, you will still get it
> > _svn checkout http://lingua-franca.googlecode.com/svn/repose/trunk/ lingua-franca_

# Development Cycle #


> If you are familiar with Subversion development cycle , you can ignore this section
Newbies are strongly recommended to read on .

  * Below is quick tutorial on basic subversion commands , ( plenty of tutorials exists online , i found this more quick and useful)
> > http://www.owlnet.rice.edu/~comp314/svn.html

  * A complete book on subversion exists here
> > http://svnbook.red-bean.com/

# To Do List #


> A detailed requirements and design document are available ( links will be provided shortly.