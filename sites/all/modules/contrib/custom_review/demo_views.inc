<?php
//$Id: demo_views.inc,v 1.2 2008/10/12 16:15:41 striky2 Exp $

/**
 * Demo default view: look for reviews on a specific node type (reviewed arg) 
 * and made by someone (uid arg) 
 * 
 * @param $views
 */
function custom_review_demo_view_1() {
  $view->name = 'demo_reviews_on_a_node_type_by';
  $view->description = '';
  $view->access = array();
  $view->view_args_php = '';
  $view->page = TRUE;
  $view->page_title = '';
  $view->page_header = '';
  $view->page_header_format = '1';
  $view->page_footer = '';
  $view->page_footer_format = '1';
  $view->page_empty = '';
  $view->page_empty_format = '1';
  $view->page_type = 'table';
  $view->url = 'custom_reviews/on/$arg/by/$arg';
  $view->use_pager = TRUE;
  $view->nodes_per_page = '10';
  $view->menu = TRUE;
  $view->menu_title = '';
  $view->menu_tab = TRUE;
  $view->menu_tab_weight = '0';
  $view->menu_tab_default = TRUE;
  $view->menu_tab_default_parent = NULL;
  $view->menu_tab_default_parent_type = 'tab';
  $view->menu_parent_tab_weight = '0';
  $view->menu_parent_title = '';
  $view->sort = array();
  $view->argument = array(
    array(
      'type' => 'reviewed',
      'argdefault' => '2',
      'title' => '',
      'options' => '',
      'wildcard' => '',
      'wildcard_substitution' => '',
    ),
    array(
      'type' => 'uid',
      'argdefault' => '1',
      'title' => '',
      'options' => '',
      'wildcard' => '',
      'wildcard_substitution' => '',
    ),
  );
  $view->field = array(
    array(
      'tablename' => 'node',
      'field' => 'title',
      'label' => 'Article title',
      'handler' => 'views_handler_field_nodelink',
      'options' => 'link',
    ),
    array(
      'tablename' => 'node_reviewed',
      'field' => 'title',
      'label' => 'Node reviewed title',
    ),
  );
  $view->filter = array();
  $view->exposed_filter = array();
  $view->requires = array(node, node_reviewed);
  
  return $view;
}

/**
 * Demo default view: look for reviews of a certain type (reviewer arg) 
 * and made on a specific node (nid arg) 
 * 
 * @param $views
 */
function custom_review_demo_view_2() {
  $view->name = 'demo_reviews_typed_on_a_node';
  $view->description = '';
  $view->access = array();
  $view->view_args_php = '';
  $view->page = TRUE;
  $view->page_title = '';
  $view->page_header = '';
  $view->page_header_format = '1';
  $view->page_footer = '';
  $view->page_footer_format = '1';
  $view->page_empty = '';
  $view->page_empty_format = '1';
  $view->page_type = 'table';
  $view->url = 'custom_reviews/reviewer/$arg/on/$arg';
  $view->use_pager = TRUE;
  $view->nodes_per_page = '10';
  $view->menu = TRUE;
  $view->menu_title = '';
  $view->menu_tab = TRUE;
  $view->menu_tab_weight = '0';
  $view->menu_tab_default = TRUE;
  $view->menu_tab_default_parent = NULL;
  $view->menu_tab_default_parent_type = 'tab';
  $view->menu_parent_tab_weight = '0';
  $view->menu_parent_title = '';
  $view->sort = array();
  $view->argument = array(
    array(
      'type' => 'reviewer',
      'argdefault' => '2',
      'title' => '',
      'options' => '',
      'wildcard' => '',
      'wildcard_substitution' => '',
    ),
    array(
      'type' => 'nid',
      'argdefault' => '2',
      'title' => '',
      'options' => '',
      'wildcard' => '',
      'wildcard_substitution' => '',
    ),
  );
  $view->field = array(
    array(
      'tablename' => 'node',
      'field' => 'title',
      'label' => 'Article title',
      'handler' => 'views_handler_field_nodelink',
      'options' => 'link',
    ),
    array(
      'tablename' => 'node_reviewed',
      'field' => 'title',
      'label' => 'Node reviewed title',
    ),
  );
  $view->filter = array();
  $view->exposed_filter = array();
  $view->requires = array(node, node_reviewed);
  
  return $view;
}
?>