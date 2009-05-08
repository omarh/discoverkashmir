$Id: README.txt,v 1.8 2008/09/05 01:13:10 pwolanin Exp $

Modr8 module for Drupal 6.x.

The moderation of content was removed from core in 5.x.  This module adds back
that capability and also prevents posts that are in moderation from showing
up in lists, on the /node page, etc., unless the user is the node's author or
unless the user has the "administer nodes" or "moderate content" permission.
This is NOT an access control module, however, so (as with Drupal 4.7.x), posts
that are in moderation can still be viewed if a user knows the path (URL)
corresponding to that post.

This module provides an admin interface for managing content in moderation,
and an optional block to show how many posts are in moderation and the titles
of recently added posts.

After installation of this module, nodes have an extra publishing option called
'In moderation queue'. Generally, each content type which requires moderation,
should have this option checked. (path: admin/content/types)

The admin interface allows a user with the "moderate content" permission to
preview content in moderation, as well as approve or delete each moderated post,
and (optionally) to send an e-mail to the author informing him/her of the choice.
*Important note:* as of version 5.x-2.3 unpublished nodes in the WILL be shown
in the moderation queue listing for users with the "adminiser nodes" permission.
For these users, approving a post in the queue will also publish it. However,
posts to be moderated should generally be set to be published so that users
without the "administer nodes" permission can also effectively work as
moderators.

Visit the settings page to customize the e-mail messages and set other defaults
(path: admin/settings/modr8).

As of version 5.x-2.0, modr8 also includes a moderation log to record the
actions of moderators on items in the moderation queue.  This may be especially
helpful if you are using the feature to send a note by e-mail even when taking
"no action" on a post.  Then, other moderators can see the note, and whether
the post's author has responded to the suggested changes.  This log may also be
useful if you want to know which moderator approved or deleted a post, and if
they provided any specific rationale in their e-mail note.  The data for this
log is saved in the table named {modr8_log}. Note that if you have an access
control module enabled, events in the moderation log for nodes that have been
deleted may only be visible to users with the "administer nodes" permission, but
not to users with just the "moderate content" permission.

This module was originally written for Drupal 4.7.x by Jeff Robbins
(jjeff@drupal).  Upgrade to 5.x and 5.x, 6.x maintenance by 
Peter Wolanin (pwolanin@drupal).
Initial upgrade to 6.x by Croitor Alexander (Placinta@drupal)
