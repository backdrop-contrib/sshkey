<?php

/**
 * @file
 * Hooks provided by the SSH key module.
 */

/**
 * Control access to an SSH key.
 *
 * Modules may implement this hook if they want to have a say in whether or not
 * a given user has access to perform a given operation on an SSH key.
 *
 * The administrative account (user ID #1) always passes any access check, so
 * this hook is not called in that case. Users with the "administer SSH public
 * keys" permission may always view and edit SSH keys through the
 * administrative interface.
 *
 * Note that not all modules will want to influence access on all SSH keys. If
 * your module does not want to actively grant or block access, return
 * SSHKEY_ACCESS_IGNORE or simply return nothing. Blindly returning FALSE will
 * break other node access modules.
 *
 * @param string $op
 *   The operation to be performed. Possible values:
 *   - 'view'
 *   - 'create'
 *   - 'edit'
 *   - 'delete'
 * @param string $entity_type
 *   The entity type that the key operation is associated with.
 * @param int $entity_id
 *   The entity ID that the key operation is associated with.
 * @param object $key
 *   The SSH key object.
 * @param object $account
 *   A user object representing the user for whom the operation is to be
 *   performed.
 *
 * @return string|null
 *   SSHKEY_ACCESS_ALLOW if the operation is to be allowed; SSHKEY_ACCESS_DENY
 *   if the operation is to be denied; SSHKEY_ACCESSS_IGNORE to not affect this
 *   operation at all.
 */
function hook_sshkey_access($op, $entity_type, $entity_id, $key, $account) {
  return NULL;
}

/**
 * Act on an SSH key being inserted or updated.
 *
 * This hook is invoked from sshkey_save() before the SSH key is saved to the
 * database.
 *
 * @param object $key
 *   The SSH key object being inserted or updated.
 */
function hook_sshkey_presave($key) {

}

/**
 * Respond to creation of a new SSH key.
 *
 * This hook is invoked from sshkey_save() after the SSH key is inserted into
 * the sshkey table in the database.
 *
 * @param object $key
 *   The SSH key object that was created.
 */
function hook_sshkey_insert($key) {

}

/**
 * Respond to updates to an SSH key.
 *
 * This hook is invoked from sshkey_save() after the SSH key is updated in the
 * sshkey table in the database.
 *
 * @param object $key
 *   The SSH key object that was updated.
 */
function hook_sshkey_update($key) {

}

/**
 * Respond to an SSH key deletion.
 *
 * This hook is invoked from sshkey_delete_multiple() after the SSH key has
 * been removed from the sshkey table in the database.
 *
 * @param object $key
 *   The SSH key object that was deleted.
 */
function hook_sshkey_delete($key) {

}

/**
 * Perform SSH key validation before a SSH key is created or updated.
 *
 * This hook is invoked from sshkey_validate(), after a user has has finished
 * editing an SSH key and is submitting it. It is invoked at the end of all the
 * standard validation steps.
 *
 * To indicate a validation error, use form_set_error().
 *
 * Note: Changes made to the $key object within your hook implementation will
 * have no effect. The preferred method to change a key's content is to use
 * hook_sshkey_alter() instead. If it is really necessary to change the SSH key
 * at the validate stage, you can use form_set_value().
 *
 * @param object $key
 *   The SSH key being validated.
 * @param array $form
 *   The form being used to edit the SSH key.
 * @param array $form_state
 *   The form state array.
 * @param array $parsed
 *   The parsed key. This contains the elements 'algorithm', 'key',
 *   'fingerprint', and 'comment'.
 */
function hook_sshkey_validate($key, $form, $form_state, array $parsed) {

}
