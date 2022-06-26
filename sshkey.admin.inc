<?php

/**
 * @file
 * Admin pages for sshkey module.
 */

/**
 * Form callback, admin settings.
 */
function sshkey_settings_form() {
  $form['#config'] = 'sshkey.settings';
  $form['sshkey_help'] = array(
    '#type' => 'textarea',
    '#title' => t('Help text for public key listing pages and the add public key forms.'),
    '#default_value' => config_get('sshkey.settings', 'sshkey_help'),
  );

  return system_settings_form($form);
}
