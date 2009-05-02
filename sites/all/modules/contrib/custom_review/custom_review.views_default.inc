<?php
/**
 * Implementation of hook_default_views
 */
function custom_review_views_default_views() {
  $views = array();

  if (file_exists(drupal_get_path('module', 'custom_review') .'/demo_views.inc')) {
    include_once drupal_get_path('module', 'custom_review') .'/demo_views.inc';
  }

  $i = 0;
  while ($i < 5) {
    $i++;
    if (function_exists('custom_review_demo_view_'. $i)) {
      $view = new view;
      call_user_func_array('custom_review_demo_view_'. $i, array($view));
      $views[$view->name] = $view;
    }
  }
  return $views;
}
