<?php
// $Id: moderation.moderation.inc,v 1.1.2.4 2009/04/23 09:35:58 sanduhrs Exp $

/**
 * @file
 * Moderation - nodes and comments queue
 *
 * @author
 * Stefan Auditor <stefan.auditor@erdfisch.de>
 */

/*
 * Menu callback: content administration.
 */
function moderation_node_queue() {
  drupal_add_css(drupal_get_path('module', 'moderation') .'/moderation.css');
  drupal_add_js(drupal_get_path('module', 'moderation') .'/moderation_node.js');

  $query = "SELECT n.*, u.name, u.uid, mm.status as moderate FROM {node} n  
              INNER JOIN {users} u ON n.uid = u.uid
              LEFT JOIN {moderation_moderation} mm ON n.nid = mm.obj_id
                WHERE mm.obj_type = 'node'
                  AND (mm.status IS NULL OR mm.status=0)
                  AND n.type IN ('" . implode('\' ,\'', variable_get('moderation_moderated_types', array())) ."')
                ORDER BY n.created DESC";
  $result = pager_query($query, 50);
  
  $destination = drupal_get_destination();
  
  while ($node = db_fetch_object($result)) {
    $query = "SELECT m.*, u.name FROM {moderation} m  
                LEFT JOIN {users} u ON m.uid = u.uid
                WHERE m.obj_id=%d
                  AND m.obj_type='node'
                ORDER BY m.created DESC
                LIMIT 1";
    $moderation = db_fetch_object(db_query($query, $node->nid));

    $item = '<div id="moderation-node-'. $node->nid .'" class="moderation-node clear-block container-inline">';
    $item .= '<div id="moderation-title-'. $node->nid .'" class="moderation-title title">';
    $item .= format_date($node->created, 'small') .' ';
    $item .= l($node->title, 'node/'. $node->nid) .' ';
    $item .= theme('mark', node_mark($node->nid, $node->changed));
    if ($moderation) {
      $item .= ' <div class="moderation-moderation" title="'. t('Last status change') .'">'. t('(!user - !action - !date)', array('!user' => theme('username', (object) array('uid' => $moderation->uid, 'name' => $moderation->name)), '!action' => moderation_log_message($moderation->attribute, $moderation->status, $moderation->obj_type), '!date' => format_date($moderation->created, 'small'))) .'</div>';
    }
    $item .= '</div>';
    $item .= '</div>';
    
    $item .= '<div id="moderation-info-'. $node->nid .'" class="moderation-info clear-block">';
    $item .= '  <span id="moderation-operations-'. $node->nid .'" class="moderation-attribute moderation-operations">'. l(t('edit'), 'node/'. $node->nid .'/edit', array('target' => '_blank'), $destination) .'</span>';
    $item .= '  <span id="moderation-username-'. $node->nid .'" class="moderation-attribute moderation-username">'. t('By !user', array('!user' => theme('username', $node))) .'</span>';
    $item .= '  <span id="moderation-name-'. $node->nid .'" class="moderation-attribute moderation-name">'. check_plain(node_get_types('name', $node)) .'</span>';
    $item .= '  <span id="moderation-status-'. $node->nid .'" class="moderation-attribute moderation-status">'. ($node->status   ? t('published') : t('not published')) .'</span>';
    $item .= '  <span id="moderation-promote-'. $node->nid .'" class="moderation-attribute moderation-promote">'. ($node->promote  ? t('promoted')  : t('not promoted')) .'</span>';
    $item .= '  <span id="moderation-sticky-'. $node->nid .'" class="moderation-attribute moderation-sticky">'. ($node->sticky   ? t('sticky')    : t('not sticky')) .'</span>';
    $item .= '  <span id="moderation-moderate-'. $node->nid .'" class="moderation-attribute moderation-moderate">'. ($node->moderate ? t('moderated') : t('not moderated')) .'</span>';
    $item .= '</div>';
    
    $rows[] = array('data' => 
      array(
        array(
          'data' => $item,
        ), 
      )
    );
  }
  
  if (!$rows) {
    $output = t('No posts available.');
  }
  
  $output .= theme('table', array(), $rows);
  $output .= theme('pager', NULL, 50);
  
  return $output;
}

/**
 * Menu callback; present an administrative comment listing.
 */
function moderation_comment_queue() {
  drupal_add_css(drupal_get_path('module', 'moderation') .'/moderation.css');
  drupal_add_js(drupal_get_path('module', 'moderation') .'/moderation_comment.js');
  
  $result = pager_query("SELECT c.*, mm.status as moderate FROM {comments} c 
                           LEFT JOIN {moderation_moderation} mm ON c.cid = mm.obj_id
                           WHERE mm.obj_type = 'comment'
                             AND (mm.status IS NULL OR mm.status=0)
                           ORDER BY c.timestamp DESC", 50);
  $destination = drupal_get_destination();
  
  while ($comment = db_fetch_object($result)) {
    $query = "SELECT m.*, u.name FROM {moderation} m  
                LEFT JOIN {users} u ON m.uid = u.uid
                WHERE m.obj_id=%d
                  AND m.obj_type='comment'
                ORDER BY m.created DESC
                LIMIT 1";
    $moderation = db_fetch_object(db_query($query, $comment->cid));
    
    $item = '<div id="moderation-comment-'. $comment->cid .'" class="moderation-comment clear-block container-inline">';
    $item .= '<div id="moderation-title-'. $comment->cid .'" class="moderation-title title">';
    $item .= format_date($comment->timestamp, 'small') .' ';
    $item .= l($comment->subject, 'node/'. $comment->nid, array(), NULL, 'comment-'. $comment->cid) .' ';
    $item .= theme('mark', node_mark($comment->cid, $comment->changed));
    if ($moderation) {
      $item .= ' <div class="moderation-moderation" title="'. t('Last status change') .'">'. t('(!user - !action - !date)', array('!user' => theme('username', (object) array('uid' => $moderation->uid, 'name' => $moderation->name)), '!action' => moderation_log_message($moderation->attribute, $moderation->status, $moderation->obj_type), '!date' => format_date($moderation->created, 'small'))) .'</div>';
    }
    $item .= '</div>';
    $item .= '</div>';
    
    $item .= '<div id="moderation-info-'. $comment->cid .'" class="moderation-info clear-block">';
    $item .= '  <span id="moderation-operations-'. $comment->cid .'" class="moderation-attribute moderation-operations">'. l(t('edit'), 'comment/edit/'. $comment->cid, array('target' => '_blank'), $destination) .'</span>';
    $item .= '  <span id="moderation-username-'. $comment->cid .'" class="moderation-attribute moderation-username">'. t('By !user', array('!user' => theme('username', $comment))) .'</span>';
    $item .= '  <span id="moderation-moderate-'. $comment->cid .'" class="moderation-attribute moderation-moderate">'. ($comment->moderate ? t('moderated') : t('not moderated')) .'</span>';
    $item .= '  <span id="moderation-status-'. $comment->cid .'" class="moderation-attribute moderation-status">'. ($comment->status   ? t('not published') : t('published')) .'</span>';
    $item .= '</div>';
    
    $rows[] = array('data' => 
      array(
        array(
          'data' => $item,
        ), 
      )
    );
  }
  
  if (!$rows) {
    $output = t('No comments available.');
  }
  
  $output .= theme('table', array(), $rows);
  $output .= theme('pager', NULL, 50);
  return $output;
}

function moderation_log($type = 'node', $obj_id = NULL) {

  switch ($type) {
    case 'comment':
      $sql = "SELECT m.*, c.nid, c.cid, c.subject, u.uid, u.name FROM {moderation} m 
                LEFT JOIN {comments} c ON c.cid = m.obj_id 
                LEFT JOIN {users} u ON u.uid = m.uid 
                WHERE m.obj_type = 'comment' " . ($obj_id ? 'AND m.obj_id = ' . $obj_id : '') . " ORDER BY m.created DESC";
      break;
    case 'node':
      $sql = "SELECT m.*, n.title, n.nid, u.uid, u.name FROM {moderation} m 
                LEFT JOIN {node} n ON n.nid = m.obj_id 
                LEFT JOIN {users} u ON u.uid = m.uid
                WHERE m.obj_type = 'node' " . ($obj_id ? 'AND m.obj_id = ' . $obj_id : '') . " ORDER BY m.created DESC";
      break;
  }

  $result = pager_query($sql, 25, 0, NULL);
  while ($moderation = db_fetch_object($result)) {
    $title = $moderation->subject ? $moderation->subject : $moderation->title;
    $fragment = $moderation->cid ? 'comment-' . $moderation->cid : NULL;
    
    $user->uid = $moderation->uid;
    $user->name = $moderation->name;
    
    $moderations[] = array(
      l($title, 'node/' . $moderation->nid, array('fragment' => $fragment)),
      theme('username', $user),
      moderation_log_message($moderation->attribute, $moderation->status, $type),
      format_date($moderation->created),
    );
  }

  $header = array(
    t('Title'), t('User'), t('Action'), t('Date'),
  );
  $output = theme('table', $header, $moderations);
  $output .= theme('pager');
  return $output;
}

function moderation_log_message($attribute, $status, $type) {
  switch ($attribute) {
    case 'status':
      $message = $status ? t('Published') : t('Unpublished');
      break;
    case 'moderate':
      $message = $status ? t('Moderated') : t('Unmoderated');
      break;
    case 'sticky':
      $message = $status ? t('Made sticky') : t('Removed stickyness');
      break;
    case 'promote':
      $message = $status ? t('Promoted') : t('Not promoted');
      break;
  }
  return $message;
}
