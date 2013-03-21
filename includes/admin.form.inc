<?php

/**
 * This file contains all admin functions
 */
module_load_include('inc', 'islandora_remote_resource', 'includes/apache-tika-api');

/**
 * Admin form function.
 */
function islandora_remote_resource_admin($form, &$form_state) {
  
  $form = array();

  // Add the file fields.
//   islandora_remote_resource_admin_add_file_fields($form, $form_state);
  
  // Add the text extraction fields.
  islandora_remote_resource_admin_add_text_extraction($form, $form_state);
  
//   $form['islandora_remote_resource_thumbnail_fieldset'] = array(
//     '#type' => 'fieldset',
//     '#title' => t('Thumbnail'),
//     '#description' => t('Settings for remote resource thumbnails'),
//   );

//   $form['islandora_remote_resource_thumbnail_fieldset']['islandora_remote_resource_thumbnail_width'] = array(
//     '#type' => 'textfield',
//     '#title' => t('Width'),
//     '#description' => t('The width of the thumbnail in pixels.'),
//     '#element_validate' => array('element_validate_number'),
//     '#default_value' => variable_get('islandora_remote_resource_thumbnail_width', 200),
//     '#size' => 5,
//   );

//   $form['islandora_remote_resource_thumbnail_fieldset']['islandora_remote_resource_thumbnail_height'] = array(
//     '#type' => 'textfield',
//     '#title' => t('Height'),
//     '#description' => t('The height of the thumbnail in pixels.'),
//     '#element_validate' => array('element_validate_number'),
//     '#default_value' => variable_get('islandora_remote_resource_thumbnail_height', 200),
//     '#size' => 5,
//   );

//   $form['islandora_remote_resource_preview_fieldset'] = array(
//     '#type' => 'fieldset',
//     '#title' => t('Preview image'),
//     '#description' => t('Settings for remote resource preview image'),
//   );

//   $form['islandora_remote_resource_preview_fieldset']['islandora_remote_resource_preview_width'] = array(
//     '#type' => 'textfield',
//     '#title' => t('Max width'),
//     '#description' => t('The maximum width of the preview in pixels.'),
//     '#element_validate' => array('element_validate_number'),
//     '#default_value' => variable_get('islandora_remote_resource_preview_width', 500),
//     '#size' => 5,
//   );

//   $form['islandora_remote_resource_preview_fieldset']['islandora_remote_resource_preview_height'] = array(
//     '#type' => 'textfield',
//     '#title' => t('Max height'),
//     '#description' => t('The maximum height of the preview in pixels.'),
//     '#element_validate' => array('element_validate_number'),
//     '#default_value' => variable_get('islandora_remote_resource_preview_height', 700),
//     '#size' => 5,
//   );

  return system_settings_form($form);
}

/**
 * Add text extraction form fields to the form.
 */
function islandora_remote_resource_admin_add_text_extraction(&$form, &$form_state) {
	
	// Get the current value.
	$islandora_path_to_tika = islandora_remote_resource_get_current_form_val ($form_state, 'islandora_path_to_tika');
	
	// Default to not found...
	$confirmation_message = '<img src="' . url('misc/watchdog-error.png') . '"/> '
					. t('Please select the location of the tika library.');
	
	if (isset($islandora_path_to_tika) && $islandora_path_to_tika != '') {
		 
		// Check the version.
		$version = islandora_remote_resource_tika_check($islandora_path_to_tika);
		 
		if ($version != null) {
			$confirmation_message = '<img src="' . url('misc/watchdog-ok.png') . '"/>'
							. t('!version executable found at !url', array(
									'!url' => "<strong>$islandora_path_to_tika</strong>",
									"!version" => $version
							));
		} else {
			$confirmation_message = '<img src="' . url('misc/watchdog-error.png') . '"/> '
							. t('Unable to find Apache Tika executable at !url', array('!url' => "<strong>$islandora_path_to_tika</strong>"));
		}
	}
	
	// The fieldset.
	$form['islandora_remote_resource_text_fieldset'] = array(
			'#type' => 'fieldset',
			'#title' => t('TEXT EXTRACTION'),
	);
	
	// Checkbox.
	$form['islandora_remote_resource_text_fieldset']['islandora_remote_resource_extract_text'] = array(
			'#type' => 'checkbox',
			'#title' => t("Attempt to extract the text of the remote resource and store for indexing purposes?"),
			'#default_value' => variable_get('islandora_remote_resource_extract_text', FALSE)
	);
	
	// Ajax message wrapper.
	$form['islandora_remote_resource_text_fieldset']['wrapper'] = array(
			'#prefix' => '<div id="islandora-tika-path">',
			'#suffix' => '</div>',
			'#type' => 'markup',
	);
	
	// The path to apache tika element.
	$form['islandora_remote_resource_text_fieldset']['wrapper']['islandora_path_to_tika'] = array(
			'#type' => 'textfield',
			'#title' => t('Tika jar file'),
			'#size' => 100,
			'#default_value' => variable_get('islandora_path_to_tika', ''),
			'#description' => t('The full path to the Apache Tika library <br /> !confirmation_message',
							array(
									'!confirmation_message' => $confirmation_message)
			),
			'#ajax' => array(
					'callback' => 'islandora_update_tika_div',
					'wrapper' => 'islandora-tika-path',
					'effect' => 'fade',
					'event' => 'blur',
					'progress' => array('type' => 'throbber'),
			),
// 			'#states' => array(
// 					'visible' => array(
// 							':input[name="islandora_remote_resource_extract_text"]' => array('checked' => TRUE),
// 					),
// 			)
	);
}

/**
 * Add the file behaviour fields to the form.
 */
// function islandora_remote_resource_admin_add_file_fields(&$form, &$form_state) {
	
// 	// Fieldset.
// 	$form['islandora_remote_resource_file_fieldset'] = array(
// 			'#type' => 'fieldset',
// 			'#title' => t('File'),
// 	);
	
// 	// Allowed file types.
// 	$form['islandora_remote_resource_file_fieldset']['islandora_remote_resource_allowed_types'] = array(
// 			'#type' => 'textfield',
// 			'#title' => t('Allowed file extensions'),
// 			'#description' => t('Enter a list of allowed file extensions separated by a comma. Leave blank to not limit by extension.'),
// 			'#default_value' => variable_get('islandora_remote_resource_allowed_types', ''),
// 			'#size' => 100,
// 	);
	
// 	// Fieldset.
// 	$form['islandora_remote_resource_jodc_fieldset'] = array(
// 			'#type' => 'fieldset',
// 			'#title' => t('JODConverter'),
// 	);
	
// 	// Wrapper for AJAX callback.
// 	$form['islandora_remote_resource_jodc_fieldset']['wrapper'] = array(
// 			'#prefix' => '<div id="islandora-jodc">',
// 			'#suffix' => '</div>',
// 			'#type' => 'markup',
// 	);
	
// 	// The path to apache tika element.
// 	$form['islandora_remote_resource_jodc_fieldset']['wrapper']['islandora_remote_resource_jodc_lib'] = array(
// 			'#type' => 'textfield',
// 			'#title' => t('JODConverter lib directory.'),
// 			'#size' => 100,
// 			'#description' => t("The full path to the JODConverter lib directory."),
// 			'#default_value' => $islandora_remote_resource_jodc_lib,
// 			'#ajax' => array(
// 					'callback' => 'islandora_update_jodc_div',
// 					'wrapper' => 'islandora-jodc',
// 					'effect' => 'fade',
// 					'event' => 'blur',
// 					'progress' => array('type' => 'throbber'),
// 			),
// 	);
	
// 	$form['islandora_remote_resource_jodc_fieldset']['wrapper']['islandora_remote_resource_jodc_jar'] = array(
// 			'#type' => 'textfield',
// 			'#title' => t('JODConverter executable jar name.'),
// 			'#size' => 100,
// 			'#description' => t("The name of the JODConverter executable jar file within the above lib directory."),
// 			'#default_value' => $islandora_remote_resource_jodc_jar,
// 			'#ajax' => array(
// 					'callback' => 'islandora_update_jodc_div',
// 					'wrapper' => 'islandora-jodc',
// 					'effect' => 'fade',
// 					'event' => 'blur',
// 					'progress' => array('type' => 'throbber'),
// 			)
// 	);
	
// 	$form['islandora_remote_resource_jodc_fieldset']['wrapper']['islandora_remote_resource_jodc_port'] = array(
// 			'#type' => 'textfield',
// 			'#title' => t('JODConverter port.'),
// 			'#size' => 100,
// 			'#description' => t('The port number JODConverter uses to connect to OpenOffice/LibreOffice runnning in headless mode.<br /> !confirmation_message',
// 							array(
// 									'!confirmation_message' => $confirmation_message)
// 			),
// 			'#default_value' => $islandora_remote_resource_jodc_port,
// 			'#ajax' => array(
// 					'callback' => 'islandora_update_jodc_div',
// 					'wrapper' => 'islandora-jodc',
// 					'effect' => 'fade',
// 					'event' => 'blur',
// 					'progress' => array('type' => 'throbber'),
// 			)
// 	);
// }

/**
 * Get current value of a field.
 */
function islandora_remote_resource_get_current_form_val(&$form_state, $name, $default = '') {
	if (isset($form_state['values'][$name])) {
		$value = $form_state['values'][$name];
	}
	else {
		$value = variable_get($name, $default);
	}
	
	return $value;
}


/**
 * Admin form validate function
 */
function islandora_remote_resource_admin_validate($form, &$form_state) {
	
	// Extract text is checked but tika has not been configured.
//   if ($form_state['values']['islandora_remote_resource_extract_text']) {
    if (islandora_remote_resource_tika_check($form_state['values']['islandora_path_to_tika']) == null) {
    	form_set_error('islandora_path_to_tika', t('Please supply a valid path to Apache Tika.'));
    }
//   }
}

/**
 * Check to see if the tika api can be initialised by the path specified.
 */
function islandora_remote_resource_tika_check($path) {
	
	// Just get the version.
	$tika = new TikaAPI($path);
	return $tika->getVersion();
}

/**
 * Callback function to return a status message to the admin forms.
 */
function islandora_update_tika_div($form, $form_state) {
  unset($form_state['submit_handlers']);
  $form_state['rebuild'] = TRUE;

  return $form['islandora_remote_resource_text_fieldset']['wrapper'];
}