<?php
// $Id: modr8_admin.inc,v 1.25 2009/02/03 23:33:37 pwolanin Exp $

function modr8_settings_form($form_state) {

  $form['modr8_default_option'] = array(
    '#type' => 'radios',
    '#title' => t('Default action'),
    '#options' => array(
      'approve' => t('approve'),
      'delete' => t('delete'),
      'nada' => t('no action'),
    ),
    '#default_value' => variable_get('modr8_default_option', 'nada'),
  );

  $form['modr8_nodes_per_page'] = array(
    '#type' => 'select',
    '#title' => t('Number of moderated posts to display per page'),
    '#options' => drupal_map_assoc(array(5, 10, 15, 20, 25, 50, 75, 100, 150, 200)),
    '#default_value' => variable_get('modr8_nodes_per_page', 10),
  );
  $period = drupal_map_assoc(array(86400, 172800, 259200, 604800, 1209600, 2419200, 4838400, 7257600), 'format_interval');
  $period[0] = t('Never');
  $form['modr8_log_clear'] = array(
    '#type' => 'select',
    '#title' => t('Discard log entries older than'),
    '#default_value' => variable_get('modr8_log_clear', 0),
    '#options' => $period,
    '#description' => t('The time log entries should be kept. Older entries will be automatically discarded. Requires crontab.')
  );
  $form['text'] = array(
    '#type' => 'fieldset',
    '#title' => t('E-mail'),
  );

  $form['text']['modr8_email_from'] = array(
    '#type' => 'textfield',
    '#title' => t('Moderator email adress'),
    '#description' => t('E-mail notices sent by modr8 will have this as the "From" address. Leave empty to use same "From" address  as is used for user registration other administrative notices as set at <a href="@site-info">Site information</a>.', array('@site-info' => url('admin/settings/site-information'))),
    '#default_value' => variable_get('modr8_email_from', ''),
  );

  $form['text']['modr8_send_approve'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send approval messages'),
    '#default_value' => variable_get('modr8_send_approve', FALSE),
  );

  $form['text']['modr8_accepted_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Acceptance e-mail subject'),
    '#default_value' => variable_get('modr8_accepted_subject', "[%site] %title has been approved"),
  );

  $macros = implode(', ', array_keys(modr8_replacements()));

  $accept_default = modr8_accept_default();

  $form['text']['modr8_accepted_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Acceptance e-mail'),
    '#default_value' => variable_get('modr8_accepted_text', $accept_default),
    '#description' => t('Replacement strings are: %macros', array('%macros' => $macros)),
  );

  $form['text']['modr8_send_deny'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send denial messages'),
    '#default_value' => variable_get('modr8_send_deny', FALSE),
  );

  $form['text']['modr8_denial_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Denial e-mail subject'),
    '#default_value' => variable_get('modr8_denial_subject', "[%site] %title has been denied"),
  );

  $denial_default = modr8_denial_default();

  $form['text']['modr8_denial_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Denial e-mail'),
    '#default_value' => variable_get('modr8_denial_text', $denial_default),
    '#description' => t('Replacement strings are: %macros', array('%macros' => $macros)),
  );

  $form['text']['modr8_send_noact'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send a message when taking no action, but only if the moderator enters a "Note to author".'),
    '#default_value' => variable_get('modr8_send_noact', FALSE),
  );

  $form['text']['modr8_noact_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('No action e-mail subject'),
    '#default_value' => variable_get('modr8_noact_subject', "[%site] note to author about %title"),
  );

  $noact_default = modr8_noact_default();

  $form['text']['modr8_noact_text'] = array(
    '#type' => 'textarea',
    '#title' => t('No action e-mail note'),
    '#default_value' => variable_get('modr8_noact_text', $noact_default),
    '#description' => t('Replacement strings are: %macros', array('%macros' => $macros)),
  );
  $form['#validate'] = array('modr8_settings_validate');

  return system_settings_form($form);
}

function modr8_settings_validate($form, &$form_state) {
  if ($form_state['values']['modr8_email_from'] && !valid_email_address($form_state['values']['modr8_email_from'])) {
    form_set_error('modr8_email_from', t('You must either enter a valid e-mail address, or leave the moderator e-mail field empty.'));
  }
}

/**
 * Menu callback; the moderation response page.
 */
function modr8_response_page($node) {
  if ($node->moderate) {
    drupal_set_title(t('Submit response regarding %title', array('%title' => $node->title)));
    return drupal_get_form('modr8_response_form', $node);
  }
  else {
    drupal_set_title(t('The moderator already approved %title', array('%title' => $node->title)));
    return '<p>'. t('This post has already been approved by the moderator. No response is needed.') .'</p>';
  }
}

function modr8_response_form($form_state, $node) {
  $form = array();
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Response title'),
    '#required' => TRUE,
    '#maxlength' => 80,
    '#weight' => -5,
  );
  $form['body'] = array(
    '#type' => 'textarea',
    '#title' => t('Message to the moderator'),
    '#description' => t('Please respond to the moderation messsage you received and provide additional information as appropriate to help the moderator.'),
    '#rows' => 20,
    '#required' => TRUE,
  );
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#weight' => 5,
  );
  // This flag can be used by modr8, or other modules to change the teaser specifically
  // for when it's being shown in the moderation list.
  $node->modr8_form_teaser = TRUE;
  $teaser = node_view($node, TRUE, FALSE, FALSE);
  $form['preview'] = array(
    '#type' => 'value',
    '#value' => $teaser,
  );
  $form['author_uid'] = array(
    '#type' => 'value',
    '#value' => $node->uid,
  );
  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $node->nid,
  );

  return $form;
}

/**
 * Form submit handler - log author response.
 *
 * @see.modr8_response_form().
 */
function modr8_response_form_submit($form, &$form_state) {
  $form_state['values']['title'] = check_plain($form_state['values']['title']);
  $message = filter_xss(nl2br($form_state['values']['body']), array('br'));
  modr8_log_action('response', $form_state['values']['nid'], $form_state['values'], $message);
  $form_state['redirect'] = 'node/'. $form_state['values']['nid'];
  drupal_set_message(t('Your response has been logged.'));
}

/**
 * Menu callback; displays the content moderation form.
 */
function modr8_page() {

  $is_published = '';
  if (!user_access('administer nodes')) {
    // Users who don't have the 'administer nodes' permission can only see published nodes.
    $is_published = 'n.status = 1 AND ';
  }
  $count_sql = db_rewrite_sql('SELECT COUNT(*) FROM {node} n WHERE '. $is_published .' n.moderate = 1');
  $page_sql = db_rewrite_sql('SELECT n.nid FROM {node} n WHERE '. $is_published .' n.moderate = 1 ORDER BY n.changed DESC');
  $result = pager_query($page_sql, 10, 0, $count_sql);
  $result_count = pager_query($page_sql, 10, 0, $count_sql);
  $output = '<p>'. l(t('Show log of all actions on moderated content.'), 'admin/reports/modr8') .'</p>';


  if (db_fetch_object($result_count)) {
    $output .= drupal_get_form('modr8_form', $result);
    $output .= theme('pager');

  }
  else {
    $output .= '<p>'. t('@items in moderation', array('@items' => format_plural(0, '1 post', '@count posts'))) .'</p>';
  }
  return $output;
}

function modr8_form($form_state, $result = NULL) {
  $form = array(
    '#theme' => 'modr8_form',
    '#tree' => TRUE,
  );

  while ($r = db_fetch_object($result)) {
    $op_options = array();
    $node = node_load($r->nid);
    // This flag can be used by modr8, or other modules to change the teaser specifically
    // for when it's being shown in the moderation list.
    $node->modr8_form_teaser = TRUE;
    $teaser = node_view($node, TRUE, FALSE, FALSE);

    $form[$node->nid] = array(
      '#tree' => TRUE,
    );

    $op_options['approve'] = t('Approve');
    if (node_access("delete", $node)) {
      $op_options['delete'] = t('Delete');
    }
    $op_options['nada'] = t('No action');

    $form[$node->nid]['ops'] = array(
      '#type' => 'radios',
      '#options' => $op_options,
      '#default_value' => variable_get('modr8_default_option', 'nada'),
    );
    if (variable_get('modr8_send_approve', FALSE) || variable_get('modr8_send_deny', FALSE)) {
      $form[$node->nid]['note'] = array(
        '#type' => 'textarea',
        '#title' => t('Note to author'),
        '#cols' => 15, // keep it narrow
      );
    }
    $form[$node->nid]['preview'] = array(
      '#type' => 'value',
      '#value' => $teaser,
    );

    $log_link = '';
    $events = db_query("SELECT modid FROM {modr8_log} WHERE nid = %d", $node->nid);
    $count = db_result(db_query("SELECT COUNT(modid) FROM {modr8_log} WHERE nid = %d", $node->nid));
    if ($count) {
      if ($count == 1) {
        $url = 'admin/reports/modr8/event/'. db_result($events);
      }
      else {
        $url = 'node/'. $node->nid .'/modr8/';
      }
      $message = format_plural($count, 'See the 1 moderation log event for this post', 'Overview of the @count moderation log events for this post');
      $log_link .= l($message, $url);
    }
    $form[$node->nid]['log_link'] = array(
      '#value' => $log_link,
    );
    $form[$node->nid]['author_uid'] = array(
      '#type' => 'value',
      '#value' => $node->uid,
    );
    $form[$node->nid]['title'] = array(
      '#type' => 'value',
      '#value' => check_plain($node->title),
    );

    $form[$node->nid]['type'] = array(
      '#type' => 'value',
      '#value' => node_get_types('name', $node),
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Themes the content moderation form.
 */
function theme_modr8_form(&$form) {
  $headers = array(
    t('Operations'),
    t('Content')
  );
  foreach (element_children($form) as $key) {
    // Only do this for nodes; not the submit button.
    if (is_numeric($key)) {
      $row = array();
      $note_field = '';
      if (variable_get('modr8_send_approve', FALSE) || variable_get('modr8_send_deny', FALSE)) {
        $note_field .= drupal_render($form[$key]['note']);
      }
      $row[] = array(
        'data' => drupal_render($form[$key]['ops']) . $note_field,
        'style' => 'vertical-align:top;'
      );
      // Apply extra filtering to insure we don't have nested form elements,
      // unexpected script, etc.
      $preview = filter_xss_admin($form[$key]['preview']['#value']);

      if (!empty($form[$key]['log_link']['#value'])) {
        $preview .= '<div><em>'. drupal_render($form[$key]['log_link']) .'</em></div>';
      }
      $row[] = array(
        'data' => $preview,
        'style' => 'vertical-align:top;',
      );
      $rows[] = $row;
    }
  }
  $output = theme('table', $headers, $rows);
  $output .= drupal_render($form);

  return $output;
}

/**
 * Form submit handler, which approves or deletes the node.
 */
function modr8_form_submit($form, &$form_state) {
  foreach ($form_state['values'] as $nid => $values) {
    $message = '';
    switch ($values['ops']) {
      case 'approve':
        if (variable_get('modr8_send_approve', FALSE)) {
          $message = modr8_usermail('approve', $nid, $values);
        }
        $publish = '';
        if (user_access('administer nodes')) {
          $publish = ', status = 1';
        }
        db_query('UPDATE {node} SET moderate = 0 '. $publish .' WHERE nid = %d', $nid);
        drupal_set_message(t('The %type with title %title has been approved.', array('%title' => $values['title'], '%type' => $values['type'])));
        cache_clear_all();
        modr8_log_action('approve', $nid, $values, $message);
        break;
      case 'delete':
        if (variable_get('modr8_send_deny', FALSE)) {
          $message = modr8_usermail('deny', $nid, $values);
        }
        node_delete($nid);
        // drupal does its own message
        modr8_log_action('delete', $nid, $values, $message);
        break;
      case 'nada':
        if (variable_get('modr8_send_noact', FALSE) && !empty($values['note'])) {
          $message = modr8_usermail('nada', $nid, $values);
          modr8_log_action('nada', $nid, $values, $message);
        }
        break;
    }
  }
}

function modr8_log_action($op, $nid, $values, $message) {
  global $user;
  $actions = array('approve' => 'Approve', 'delete' => 'Delete', 'nada' => 'No action', 'response' => 'Response');

  db_query("INSERT INTO {modr8_log} (nid, uid, author_uid, action, title, message, teaser, timestamp) VALUES (%d, %d, %d, '%s', '%s', '%s', '%s', %d)", $nid, $user->uid, $values['author_uid'], $actions[$op], $values['title'], $message, $values['preview'], time());
  // Notify modules interested in each action we took.
  // modules have to implement the function: hook_modr8_log ( $op ,$nid , $values, $message);
  module_invoke_all('modr8_log', $op, $nid, $values, $message);
}

function modr8_usermail($op, $nid, $values) {
  $node = node_load($nid);

  if (is_object($node)) {

    switch ($op) {
      case 'approve':
        $subject = variable_get('modr8_accepted_subject', '[%site] %title has been approved');
        $message = variable_get('modr8_accepted_text', modr8_accept_default());
        $optype = t('approval');
        break;
      case 'deny':
        $subject = variable_get('modr8_denial_subject', '[%site] %title has been denied');
        $message = variable_get('modr8_denial_text', modr8_denial_default());
        $optype = t('denial');
        break;
      case 'nada':
        $subject = variable_get('modr8_noact_subject', "[%site] note to author about %title");
        $message = variable_get('modr8_noact_text', modr8_noact_default());
        $optype = t('note (no action)');
        break;
    }

    // get the user info for author
    $account = user_load(array('uid' => $node->uid));

    $note = theme('modr8_note', $values['note']);

    $sendmail_from = '';
    $site_mail = variable_get('modr8_email_from', '');
    if (!$site_mail) {
      $sendmail_from = ini_get('sendmail_from');
      $site_mail = variable_get('site_mail', $sendmail_from);
    }
    if (empty($site_mail) || $site_mail == $sendmail_from) {
      drupal_set_message(t('You should create an administrator mail address for your site! <a href="@url">Do it here</a>.', array('@url' => url('admin/settings/site-information'))), 'error');
    }
    // send the email
    $language = user_preferred_language($account);
    $params = array('account' => $account, 'node' => $node, 'subject' => $subject, 'message' => $message, 'note' => $note);

    if ($account->uid == 0) {
      $message = t('Anonymous user; no %type message was sent.', array('%type' => $optype));
    }
    else {
      $sent = drupal_mail('modr8', 'notify', $account->mail, user_preferred_language($account), $params);
      if (!empty($sent['result'])) {
        drupal_set_message(t('%type message was sent to %username', array('%type' => $optype, '%username' => $account->name)));
        $message = filter_xss(nl2br($sent['body']), array('br', 'a')); // Return sanitized e-mail with HTML breaks added.
      }
      else {
        $message = t('There was a problem sending the %type message to %username', array('%type' => $optype, '%username' => $account->name));
        drupal_set_message($message, 'error');
      }
    }
  }
  else {
    $message = t('An error occurred when trying to load this content.');
    drupal_set_message($message); // this probably won't ever get called
  }
  return $message;
}

/**
 * Implementation of hook_mail()
 */
function modr8_mail($key, &$message, $params) {
  $language = $message['language'];
  $node = $params['node'];
  $account = $params['account'];
  $note = $params['note'];
  switch ($key) {
    case 'notify':
      // eval the replacements
      $replacements_raw = modr8_replacements();
      foreach ($replacements_raw as $key => $val) {
        eval('$replacements["$key"] = '. $val .';');
      }
      drupal_set_message(print_r($replacements,1));
      $params['subject'] = strtr($params['subject'], $replacements);
      $params['message'] = strtr($params['message'], $replacements);
      $message['subject'] = $params['subject'];
      $message['body'] = $params['message'];
    break;
  }
}

function theme_modr8_note($note) {
  if ($note) {
    // Do not filter here (use !) since this note is sanitized after e-mailing
    $note = t("Note:
  !note", array('!note' => $note));
  }
  return $note;
}

function modr8_replacements() {
  return array(
    '%title' => '$node->title',
    '%teaser' => '$node->teaser',
    '%body' => '$node->body',
    '%short_date' => 'format_date($node->created, "short")',
    '%medium_date' => 'format_date($node->created, "medium")',
    '%long_date' => 'format_date($node->created, "long")',
    '%type' => 'node_get_types("name", $node)',
    '%node_url' => 'url("node/". $node->nid, array("absolute" => TRUE))',
    '%nid' => '$node->nid',
    '%author_name' => '$account->name',
    '%author_mail' => '$account->mail',
    '%author_url' => 'url("user/". $account->uid, array("absolute" => TRUE))',
    '%site' => 'variable_get("site_name", "Drupal")',
    '%note' => '$note',
    '%response_url' => 'url("node/". $node->nid ."/log/response/". modr8_response_token($node->nid, $account->uid), array("absolute" => TRUE))',
  );
}

function modr8_accept_default() {
  return t(
'Your %type entry entitled "%title" has been approved by our content moderator! Other visitors to %site will now be able to view it.

You can visit %node_url to view it yourself.

%note

Regards,
The %site team');
}

function modr8_denial_default() {
  return t(
'Your %type entry entitled "%title" has been denied by our content moderator. The content has been deleted from our site.

%note

Regards,
The %site team');
}

function modr8_noact_default() {
  return t(
'Your %type entry entitled "%title" has been reviewed by our content moderator, but not yet approved.

%note

To respond to the moderator, you can visit %response_url

You can visit %node_url to view it yourself, but is is not yet visible to other site visitors.

Regards,
The %site team');
}


/**
 * menu callback for moderation log.
 */
function modr8_log_view($op = '', $id = 0) {

  switch ($op) {
    case '':
      return modr8_log_overview();
    case 'node':
      if (is_numeric($id)) {
        $node = node_load($id);
        if (!empty($node->title)) {
          drupal_set_title(check_plain($node->title));
        }
        return modr8_log_overview($id);
      }
      break;
    case 'event':
      if (is_numeric($id)) {
        drupal_set_title(t('Moderation log event'));
        return modr8_log_event($id);

      }
      break;
  }
  drupal_not_found();
}

function modr8_log_overview($nid = 0) {

  $header = array(
    array('data' => t('Action'), ),
    array('data' => t('User'), 'field' => 'u.name'),
    array('data' => t('Date'), 'field' => 'ml.modid', 'sort' => 'desc'),
    array('data' => t('Title (view event)')),
  );
  $tablesort = tablesort_sql($header);

  $count_sql = "SELECT COUNT(*) FROM {modr8_log} ml";
  $pager_sql = "SELECT ml.modid, ml.action, ml.title, ml.timestamp, u.name, u.uid FROM {modr8_log} ml LEFT JOIN {users} u ON u.uid = ml.uid";
  if ($nid) {
    $count_sql .= " WHERE ml.nid = %d";
    $pager_sql .= " WHERE ml.nid = %d";
  }
  $count_sql = db_rewrite_sql($count_sql, 'ml');
  $pager_sql = db_rewrite_sql($pager_sql, 'ml');
  $result = pager_query($pager_sql . $tablesort, 50, 0, $count_sql, $nid);

  $rows = array();

  while ($event = db_fetch_object($result)) {
    $rows[] = array(t($event->action), theme('username', $event), format_date($event->timestamp, 'small'),
    l(truncate_utf8($event->title, 50, TRUE, TRUE), 'admin/reports/modr8/event/'. $event->modid, array(), NULL, NULL, FALSE, TRUE)
    );
  }

  if (!$rows) {
    $rows[] = array(array('data' => t('No log messages available.'), 'colspan' => 4));
  }
  $output = '';
  $output .= theme('table', $header, $rows);
  $output .= theme('pager', NULL, 50, 0);

  return $output;
}

function modr8_log_event($modid) {

  if (is_numeric($modid)) {
    $sql = db_rewrite_sql("SELECT ml.*, u.name FROM {modr8_log} ml LEFT JOIN {users} u ON u.uid = ml.uid WHERE ml.modid = %d", 'ml');
    $event = db_fetch_object(db_query($sql, $modid));
    if ($event) {
      $event->author = db_fetch_object(db_query("SELECT u.name, u.uid from {users} u WHERE u.uid = %d", $event->author_uid));
      return theme('moderation_event', $event);
    }
  }

  drupal_not_found();
}

function theme_moderation_event($event) {

  $rows[] = array(array('data' => l(t('Overview of all moderation log events for this post'), 'node/'. $event->nid .'/modr8/'), 'colspan' => 2));
  $rows[] = array(t('Action:'), t($event->action));
  $rows[] = array(t('Date:'), format_date($event->timestamp, 'small'));
  if ($event->action == 'Response') {
    $rows[] = array(t('Author:'), theme('username', $event));
    $rows[] = array(t('Response title:'), $event->title);
    $rows[] = array('data' => array(t('Response message:'), $event->message), 'style' => 'vertical-align:top;');
    $rows[] = array('data' => array(t('Teaser (as of response):'), $event->teaser), 'style' => 'vertical-align:top;');
  }
  else {
    $rows[] = array(t('Moderator:'), theme('username', $event));
    $rows[] = array('data' => array(t('E-mail message:'), $event->message), 'style' => 'vertical-align:top;');
    $rows[] = array(t('Author:'), theme('username', $event->author));
    $rows[] = array('data' => array(t('Teaser (as reviewed):'), $event->teaser), 'style' => 'vertical-align:top;');
  }

  return theme('table', NULL, $rows);
}
