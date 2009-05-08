<?php
// $Id: moderation.pages.inc,v 1.1.2.2 2009/04/22 16:17:25 sanduhrs Exp $

/**
 * @file
 * Moderation - nodes and comments queue
 *
 * @author
 * Stefan Auditor <stefan.auditor@erdfisch.de>
 */
 
function moderation_callback_switch($obj_type, $obj_id, $op, $attribute) {
  $attributes = array('status', 'promote', 'sticky', 'moderate', 'preview');
  $types      = array('node', 'comment');
  
  if (!is_numeric($obj_id) OR !in_array($obj_type, $types) OR !in_array($attribute, $attributes)) {
    return drupal_not_found();
  }
  
  if ($op == 'get') {
    if ($attribute == 'preview') {
      moderation_get_preview($obj_id, $obj_type);
    }
    else {
      moderation_get_attribute($obj_id, $obj_type, $attribute);
    }
    exit();
  }
  
  if ($op == 'set') {
    switch ($attribute) {
      case 'status':
      case 'promote':
      case 'sticky':
        moderation_switch_attribute($obj_id, $obj_type, $attribute);
        break;
      case 'moderate':
        moderation_switch_moderation($obj_id, $obj_type);
        break;
    }
  }
  
  return drupal_not_found();
}

/**
 * Switch moderation flag
 *
 * @param string $obj_type one of 'node', 'comment'
 * @param integer $obj_id
 */
function moderation_switch_moderation($obj_id, $obj_type) {
  global $user;
    
  $status = db_result(db_query("SELECT status FROM {moderation_moderation} WHERE obj_id=%d AND obj_type='%s'", $obj_id, $obj_type));
  db_query("INSERT INTO {moderation} SET obj_id=%d, obj_type='%s', uid=%d, attribute='%s', status=%d, created=%d", $obj_id, $obj_type, $user->uid, 'moderate', !$status, time());
  db_query("DELETE FROM {moderation_moderation} WHERE obj_id=%d AND obj_type='%s'", $obj_id, $obj_type);
  print drupal_to_js(array(db_query("INSERT INTO {moderation_moderation} SET obj_id=%d, obj_type='%s', status=%d", $obj_id, $obj_type, !$status), !$status, 'moderate'));
  exit();
}

/**
 * Switch an attribute
 *
 * @param integer $obj_id
 * @param string $obj_type one of 'node', 'comment'
 * @param string $attribute one of 'status', 'promote', 'sticky'
 */
function moderation_switch_attribute($obj_id, $obj_type, $attribute) {
  global $user;
  
  if ($obj_type == 'node') {
    $status_new = !db_result(db_query("SELECT %s FROM {node} WHERE nid=%d", $attribute, $obj_id));
    $success = db_query("UPDATE {node} SET %s=%d WHERE nid=%d", $attribute, $status_new, $obj_id);
  }
  else if ($obj_type == 'comment') {
    $status_new = !db_result(db_query("SELECT %s FROM {comments} WHERE cid=%d", $attribute, $obj_id));
    $success = db_query("UPDATE {comments} SET %s=%d WHERE cid=%d", $attribute, $status_new, $obj_id);
  }

  db_query("INSERT INTO {moderation} SET obj_id=%d, obj_type='%s', uid=%d, attribute='%s', status=%d, created=%d", $obj_id, $obj_type, $user->uid, $attribute, $status_new, time());  
  print drupal_to_js(array($success, $status_new, $attribute));
  exit();
}

/**
 * Get the preview markup for a node or a comment
 * 
 * @param integer $obj_id
 * @param string $obj_type
 */
function moderation_get_preview($obj_id, $obj_type) {
  if ($obj_type == 'node') { 
    if ($node = node_load($obj_id)) {
      $data = theme('moderation_node_preview', $node);
    }
  }
  else if ($obj_type == 'comment') {
    if ($comment = _comment_load($obj_id)) {
      $_GET['q'] = 'node/'. $comment->nid;
      $node = node_load($comment->nid);
      $data = theme('comment_view', $comment, $node);
    }
  }
  print drupal_to_js($data);
  exit();
}

/**
 * Get an objects attribute
 *
 * @param integer $obj_id
 * @param string $obj_type
 * @param string $attribute one of 'status', 'promote', 'sticky', 'moderate'
 */
function moderation_get_attribute($obj_id, $obj_type, $attribute) {
  $table = ($obj_type == 'comment') ? 'comments' : 'node';
  $id    = ($obj_type == 'comment') ? 'cid'      : 'nid';
  
  if ($attribute == 'moderate') {
    print drupal_to_js(array(db_result(db_query("SELECT status FROM {moderation_moderation} WHERE obj_id=%d AND obj_type='%s'", $obj_id, $obj_type))));
  }
  else {
    print drupal_to_js(array(db_result(db_query("SELECT %s FROM {%s} WHERE %s=%d", $attribute , $table, $id, $obj_id))));
  }
  
  exit();
}
