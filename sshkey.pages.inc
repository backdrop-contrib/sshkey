<?php

/**
 * @file
 * Menu page callbacks.
 */

/**
 * Page callback, list an entity’s SSH keys.
 */
function sshkey_list_page($entity_type, $entity_id) {
  $destination = backdrop_get_destination();

  // Add the local actions.
  if (sshkey_access('create', $entity_type, $entity_id)) {
    $local_actions['add'] = array(
      'title' => t('Add a public key'),
      'href' => "{$entity_type}/{$entity_id}/ssh-keys/add",
      'query' => $destination,
      'modal' => TRUE,
    );
    $build['actions'] = array(
      '#theme' => 'links',
      '#links' => $local_actions,
      '#attributes' => array('class' => 'item-list action-links'),
    );
    $build['sshkey_help'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="description">' . config_get('sshkey.settings', 'sshkey_help') . '</div>',
    );
  }

  $header = array(
    'name' => array(
      'data' => t('Title'),
      'field' => 'title',
      'sort' => 'asc',
    ),
    'fingerprint' => array(
      'data' => t('Fingerprint'),
      'field' => 'fingerprint',
    ),
    'operations' => array('data' => t('Operations')),
  );

  $query = db_select('sshkey')->extend('PagerDefault')->extend('TableSort');
  $query->addField('sshkey', 'key_id');
  $query->condition('entity_type', $entity_type);
  $query->condition('entity_id', $entity_id);
  $query->orderByHeader($header);
  $query->limit(25);
  $key_ids = $query->execute()->fetchCol();
  $keys = sshkey_load_multiple($key_ids);

  $rows = array();
  foreach ($key_ids as $key_id) {
    $key = $keys[$key_id];
    $key_base_uri = "{$entity_type}/{$entity_id}/ssh-keys/{$key_id}";

    $rows[$key_id]['title'] = check_plain($key->title);
    $rows[$key_id]['fingerprint'] = theme('sshkey_fingerprint', array('key' => $key));

    $operations = array();
    if (sshkey_access('view', $entity_type, $entity_id, $key)) {
      $operations['view'] = array(
        'title' => t('View'),
        'href' => $key_base_uri . '/view',
      );
    }
    if (sshkey_access('edit', $entity_type, $entity_id, $key)) {
      $operations['edit'] = array(
        'title' => t('Edit'),
        'href' => $key_base_uri . '/edit',
        'query' => $destination,
        'modal' => TRUE,
      );
    }
    if (sshkey_access('delete', $entity_type, $entity_id, $key)) {
      $operations['delete'] = array(
        'title' => t('Delete'),
        'href' => $key_base_uri . '/delete',
        'query' => $destination,
        'modal' => TRUE,
      );
    }
    $rows[$key_id]['operations'] = array(
      'data' => array(
        '#theme' => 'links',
        '#links' => $operations,
        '#attributes' => array('class' => array('links', 'inline', 'nowrap')),
      ),
    );
  }

  $empty_text[] = t('No SSH public keys available.');
  if (sshkey_access('create', $entity_type, $entity_id)) {
    $empty_text[] = l(t('Add a public key'), "{$entity_type}/{$entity_id}/ssh-keys/add", array('query' => $destination, 'modal' => TRUE));
  }

  $build['keys_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => implode(' ', $empty_text),
    '#attributes' => array('id' => array('ssh-keys')),
  );
  $build['keys_table_pager'] = array('#theme' => 'pager');

  return $build;
}

/**
 * Page callback for viewing an SSH key.
 */
function sshkey_view_key($entity_type, $entity_id, $key) {
  // We are returning plain text, so tell the browser.
  backdrop_add_http_header('Content-Type', 'text/plain; charset=utf-8');
  print $key->value;
  backdrop_page_footer();
}

/**
 * Form callback, edit an SSH key.
 */
function sshkey_edit_form($form, $form_state, $entity_type, $entity_id, $key = NULL) {
  $entity = entity_load($entity_type, array($entity_id));
  $entity = reset($entity);

  if (!isset($key)) {
    $key = new stdClass();
    $key->is_new = TRUE;
    $key->key_id = NULL;
    $key->fingerprint = NULL;
    $key->title = '';
    $key->value = '';
    $key->entity_type = $entity_type;
    $key->entity_id = $entity_id;
  }

  $form['#sshkey'] = $key;
  $form['#entity'] = $entity;

  $form['key_id'] = array(
    '#type' => 'value',
    '#value' => isset($key->key_id) ? $key->key_id : NULL,
  );
  $form['entity_type'] = array(
    '#type' => 'value',
    '#value' => $entity_type,
  );
  $form['entity_id'] = array(
    '#type' => 'value',
    '#value' => $entity_id,
  );
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t("If this field is left blank, the key's title will be automatically generated."),
    '#default_value' => $key->title,
    '#maxlength' => 128,
  );

  $form['value'] = array(
    '#type' => 'textarea',
    '#title' => t('Key'),
    '#default_value' => $key->value,
    '#rows' => 10,
    '#wysiwyg' => FALSE,
    '#required' => TRUE,
  );

  $form['actions'] = array(
    '#weight' => 100,
  );
  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => isset($_GET['destination']) ? $_GET['destination'] : '<front>',
  );

  return $form;
}

/**
 * Form validate callback, see sshkey_edit_form().
 */
function sshkey_edit_form_validate($form, &$form_state) {
  sshkey_validate($form_state['values'], $form, $form_state);
}

/**
 * Form submit callback, see sshkey_edit_form().
 */
function sshkey_edit_form_submit($form, &$form_state) {
  form_state_values_clean($form_state);
  $key = (object) $form_state['values'];
  sshkey_save($key);
  backdrop_set_message(t('The SSH public key %title has been saved.', array('%title' => $key->title)));
}

/**
 * Form callback. Confirm key deletion.
 */
function sshkey_delete_form($form, $form_state, $key) {
  $form['#sshkey'] = $key;

  $form['key_id'] = array(
    '#type' => 'value',
    '#value' => $key->key_id,
  );

  return confirm_form(
    $form,
    t('Are you sure you want to delete the public key %title?', array('%title' => $key->title)),
    isset($_GET['destination']) ? $_GET['destination'] : '<front>',
    '',
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Form submit callback, see sshkey_delete_form().
 */
function sshkey_delete_form_submit($form, $form_state) {
  sshkey_delete($form_state['values']['key_id']);
  backdrop_set_message(t('The SSH public key %title has been deleted.', array('%title' => $form['#sshkey']->title)));
}
