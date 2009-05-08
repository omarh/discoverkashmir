<?php
// $Id: moderation.admin.inc,v 1.1.2.1 2009/04/22 14:42:24 sanduhrs Exp $

/**
 * @file
 * Moderation - un/install procedures
 *
 * @author
 * Stefan Auditor <stefan.auditor@erdfisch.de>
 */
 
function moderation_admin_settings() {
  $form['moderation_moderated_types'] = array(
    '#type' => 'checkboxes',
    '#options' => node_get_types('names'),
    '#default_value' => variable_get('moderation_moderated_types', array()),
    '#title' => t('Set nodetypes shown which are moderated'),
  );
  return system_settings_form($form);
}

/**
 * Only saves contentypes which are checked.
 */
function moderation_admin_settings_validate(&$form, &$form_state) {
  foreach ($form_state['values']['moderation_moderated_types'] as $type => $value) {
    if (!$value) {
      unset($form_state['values']['moderation_moderated_types'][$type]);
    }
  }
}

