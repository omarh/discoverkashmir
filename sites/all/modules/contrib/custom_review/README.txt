// $Id: README.txt,v 1.8 2008/10/12 16:15:41 striky2 Exp $

The custom_review modules enables node to be reviewers on other nodes

Presentation:
-------------

 Usecase:  let's imagine you want to create some specific tools to allow your users reviewing some nodes (for instance a CCK-audio review, a CCK-text review, a complex-type-CCK review, ...). Let's also imagine you want them to review specific (ie, an admin can set that before) CCK-node types, then you've got it, it's basically what custom review is about.

Some vocabulary :

    * the reviewer: the CCK-node type seen as a tool set by an admin to allow users reviewing specific other CCK-node-types
    * the reviewee: the CCK-node type seen as the type one can review with a specific reviewer
    * the review: an instance of reviewer (like an advanced comment if you want)
    * the (node) reviewed: an instance of reviewee (the node the user has been reviewing)

Features:
---------

It allows only one review by person and by reviewer type on each node. So it's neither
a tool to make comments nore supercomments, but real nodes associated in a specific way to an original one. These reviews are usually seen from the original reviewed node.

Process: the process has been created using nodeapi, so that you can go back to your node being reviewed at each time (and you keep seing a reference to your node being reviewed while making an add/edit review) . Default demo views with specific args are provided to make it easier for the end-user to list the reviews in different situations. (so ya, it's depending on views).

Admin binder: the administrator use a "binder" to put each of his "cck-node-tool-reviewers" on each available "cck-node-reviewable/reviewees"

custom_review was designed by Laurent Pipitone (striky2).

Dependencies
------------
[core module]
 * CCK

[additional core functionnalities]
* views
* panel

Install
-------
1) Copy the custom_review folder to the modules folder in your installation.
2) Enable the modules using Administer -> Modules (/admin/build/modules)

Configuration
-------------
Create the binding between the node-type which can be reviewed and the node-type which will be authorized to actually make the review.

Contributing & support
------------
Want to contribute core evolutions? Want some support?
Post on the issue queue: http://drupal.org/project/issues/custom_review