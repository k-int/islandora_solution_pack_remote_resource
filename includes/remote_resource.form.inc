<?php

/**
 * @file
 *
 * Handles the uploading of the document file as part of the ingest process.
 */

/**
 * Defines a text box for a URL entry.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 * @return
 *   The drupal form definition.
 */
function islandora_remote_resource_remote_resource_form(array $form, array &$form_state) {
  $form = array(
  	'remote_resource_url' => array(
	  	'#type' => 'textfield',
	  	'#title' => t('Resource URL'),
	  	'#default_value' => NULL,
	  	'#size' => 100,
	  	'#maxlength' => 256,
	  	'#required' => TRUE,
  	)
  );
  return $form;
}

/**
 * Adds the link to the LINK datastream.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 */
function islandora_remote_resource_remote_resource_form_submit(array $form, array &$form_state) {
	
	// Get the current object.
  $object = islandora_ingest_form_get_object($form_state);
  
  // Either create a dataset or return the dataset.
  if (empty($object['LINK'])) {
    $ds = $object->constructDatastream('LINK', 'R');
    $object->ingestDatastream($ds);
  }
  else {
    $ds = $object['LINK'];
  }
  
  // Set the URL.
  $ds->url = $form_state['values']['remote_resource_url'];
}
