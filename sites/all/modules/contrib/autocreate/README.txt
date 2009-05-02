$Id: README.txt,v 1.2.2.2 2009/02/08 16:55:34 sunsetbill Exp $

The autocreate module is a CCK field module that creates a nodereference to a
node cloned from a template.  The most common uses are for including a
reference to a webform or poll, but references to other content types are
possible.  While the module itself uses a modified copy of the clone_node()
function, clone settings (allowable types, status settings, etc.) are taken
from the Clone Node module configuration settings.  You do not have to set
clone permissions for users, but users must have permission to create content
types that use autocreated nodereferences.


How-to:

1.  After installing and enabling, go to the Site configuration > Autocreate 
nodereference page.  Set the Template token you wish to use to identify your
template nodes.  The default is AC_TEMPLATE, but everybody doesn't code in
English. ;^)  The rest of the page options are for suffixes to append to
the node title when a content type is cloned and referenced.

2.  For any node you want be able to clone for a nodereference, create an
unpublished "template" node that has the Template token as part of the node
title--e.g., if your Template token is 'WOUGFWIO', the template node for a
webform might be called, uh, 'Webform 1 WOUGFWIO'.  You can have different
templates for the same content type to use in different situations, if you
wish.

3.  Add a Nodereference (autocreate) field to your content type using the
machine-readable name of the type you wish to reference, and select the
template node to be associated with that field.

When a user creates new content for a type with ANR field(s), the template 
node(s) will be cloned and renamed using the node title + the node suffix 
specified in your Autocreate nodereference configuration.

