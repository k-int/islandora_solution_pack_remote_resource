<?php

/**
 * @file
 *
 * Contains all the functions required for adding derived datastreams from the uploaded file for the remote resource content model.
 */

/**
 * Creates all the derivatives for this content model type
 *
 * @param FedoraObject $object
 *   The object to generate/add the deriviative datastreams to.
 *
 * @return boolean
 *   TRUE if all derivatives were created successfully, FALSE otherwise.
 */
function islandora_remote_resource_create_all_derivatives(FedoraObject $object) {
  if (!isset($object['LINK'])) {
    drupal_set_message(t('Could not create derivatives for %s.  No link has been set.', array('%s' => $object->id)), 'error');
    return FALSE;
  }
  
  $success = TRUE;
  $file_uri = islandora_remote_resource_get_derivative_source_as_unmanaged_file($object);

  islandora_remote_resource_set_maetadata($object, $file_uri);
  
  // Extract the text from the original.
  if (variable_get('islandora_remote_resource_extract_text', FALSE)) {
    $success &= islandora_remote_resource_add_text_derivative($object, $file_uri);
  }
//   file_unmanaged_delete($file_uri);
  return $success;
}

/**
 * Gets the source of all derivatives as an unmanaged file, deletion of the file is left to the caller.
 *
 * @param FedoraObject $object
 *   The object to generate/add the deriviative datastreams to.
 *
 * @return string
 *   The file path to the unmanaged derivative source file.
 */
function islandora_remote_resource_get_derivative_source_as_unmanaged_file(FedoraObject $object) {
  
  // Create a filename to use when saving locally.
  $file_name = str_replace(':', '-', $object->id);
  $file_uri = file_create_filename("{$file_name}.LINK.temp", 'temporary://');
  $file_uri = drupal_realpath($file_uri);
  
  // Get the contents of the resource.
  $file = file_get_contents($object['LINK']->url);
  
  // Save here...
  file_put_contents($file_uri, $file);
  
  // Return the URI.
  return drupal_realpath($file_uri);
}

/**
 * Uses tika to extract some technical data about the file.
 *
 * @param FedoraObject $object
 *   The object to edit.
 */
function islandora_remote_resource_set_maetadata(FedoraObject $object, $file_uri) {
	
	// Add the details for the type and name of the resource.
  module_load_include('inc', 'islandora_remote_resource', 'includes/apache-tika-api');
  $tika = new TikaAPI();
  
  // Get the metadata of the file.
  $meta = json_decode($tika->getMetadataJSON($file_uri));
  
  // Set the title.
  $object['LINK']->label = $meta->title;
  
  // Get the type.
  $type = $meta->{'Content-Type'};
  
  // Remove anything after a semi-colon.
  $type = explode(";", $type)[0];
  
  // Set as the mime type.
  $object['LINK']->mimetype = $type;
}

/**
 * Creates a JPG deriviative for the TN datastream.
 *
 * @param FedoraObject $object
 *   The object to add the deriviative datastreams to.
 * @param string $file_uri
 *   The URI to the remote resource file the derivative will be generated from.
 *
 * @return boolean
 *   TRUE if successful, FALSE otherwise.
 */
// function islandora_remote_resource_add_tn_derivative(FedoraObject $object, $file_uri) {
//   $width = variable_get('islandora_remote_resource_thumbnail_width', 200);
//   $height = variable_get('islandora_remote_resource_thumbnail_height', 200);
//   return islandora_remote_resource_add_jpg_derivative($object, $file_uri, 'TN', $width, $height);
// }

/**
 * Creates a textural deriviative for the EXTRACTED_TEXT datastream.
 *
 * @param FedoraObject $object
 *   The object to add the deriviative datastreams to.
 * @param string $file_uri
 *   The URI to the remote resource file the derivative will be generated from.
 *
 * @return boolean
 *   TRUE if successful, FALSE otherwise.
 */
function islandora_remote_resource_add_text_derivative(FedoraObject $object, $file_uri) {
  $dsid = "EXTRACTED_TEXT";
  $source = drupal_realpath($file_uri);
  $success = FALSE;
  
  // Get the Tika API and extract the text.
  module_load_include('inc', 'islandora_remote_resource', 'includes/apache-tika-api');
  $tika = new TikaAPI();
  $success = islandora_remote_resource_add_text_datastream($object, $dsid, $tika->getText($source));
  
  if ($success == FALSE) {
    drupal_set_message(t('Failed to extract text for @dsid.', array('@dsid' => $dsid)), 'error');
  }
  return $success;
}

/**
 * Creates a JPG deriviative for the PREVIEW datastream.
 *
 * @param FedoraObject $object
 *   The object to add the deriviative datastreams to.
 * @param string $file_uri
 *   The URI to the remote resource file the derivative will be generated from.
 *
 * @return boolean
 *   TRUE if successful, FALSE otherwise.
 */
// function islandora_remote_resource_add_preview_derivative(FedoraObject $object, $file_uri) {
//   $width = variable_get('islandora_remote_resource_preview_width', 500);
//   $height = variable_get('islandora_remote_resource_preview_height', 700);
//   return islandora_remote_resource_add_jpg_derivative($object, $file_uri, 'PREVIEW', $width, $height);
// }

/**
 * Creates a JPG deriviative of the given file.
 *
 * @param FedoraObject $object
 *   The object to add the deriviative datastreams to.
 * @param string $file_uri
 *   The URI to the remote resource file the derivative will be generated from.
 * @param string $dsid
 *   The datastream ID to use for the derived datastream.
 * @param integer $width
 *   The width to make the derived datastream.
 * @param integer $height
 *   The height to make the derived datastream.
 *
 * @return boolean
 *   TRUE if successful, FALSE otherwise.
 */
// function islandora_remote_resource_add_jpg_derivative(FedoraObject $object, $file_uri, $dsid, $width, $height) {
//   $success = FALSE;
//   $derivative_file_uri = islandora_remote_resource_create_jpg_derivative($file_uri, $dsid, $width, $height);
//   if ($derivative_file_uri !== FALSE) {
//     $success = islandora_remote_resource_add_datastream($object, $dsid, $derivative_file_uri);
//     file_unmanaged_delete($derivative_file_uri);
//   }
//   if ($success == FALSE) {
//     drupal_set_message(t('Failed to create @dsid derivative.', array('@dsid' => $dsid)), 'error');
//   }
//   return $success;
// }

/**
 * Creates a jpg derivative from the given remote resource file.
 *
 * @param string $file_uri
 *   The URI to the remote resource file the derivative will be generated from.
 * @param string $dsid
 *   Used to identify the generated file.
 * @param integer $width
 *   The width to make the derived datastream.
 * @param integer $height
 *   The height to make the derived datastream.
 *
 * @return string
 *   A URI to the generated derivative if successful, FALSE otherwise.
 */
// function islandora_remote_resource_create_jpg_derivative($file_uri, $dsid, $width, $height) {
//   $source = drupal_realpath($file_uri) . '[0]';
//   $matches = array();
//   preg_match("/\/([^.]*).*$/", $source, $matches); // Get the base name of the source file.
//   $temp = drupal_tempnam("temporary://", "{$matches[1]}.$dsid.jpg");
//   $dest = drupal_realpath($temp);
//   $args['quality'] = '-quality ' . escapeshellarg(variable_get('imagemagick_quality', 75));
//   $args['previewsize'] = '-resize ' . escapeshellarg("{$width}x{$height}");
//   $args['colors'] = '-colorspace RGB';
  
//   // PDFs with transparency in them will cause odd results when converting,
//   // to JPG as there is no alpha channel in the JPG format. To correct this,
//   // we should flatten the image to remove the alpha channel.
//   $args['background'] = '-flatten';
//   $context = array(
//     'source' => $source,
//     'destination' => $dest,
//   );
//   drupal_alter('imagemagick_arguments', $args, $context);
//   // To make use of ImageMagick 6's parenthetical command grouping we need to make
//   // the $source image the first parameter and $dest the last.
//   // See http://www.imagemagick.org/Usage/basics/#cmdline
//   $command = escapeshellarg($source) . ' ' . implode(' ', $args) . ' ' . escapeshellarg("jpg:$dest");
//   $output = '';
//   $ret = -1;
//   if (_imagemagick_convert_exec($command, $output, $ret) !== TRUE) {
//     $message = 'imagemagick convert failed to create derivative<br/>Error: @ret<br/>Command: @command<br/>Output: @output';
//     $variables = array('@ret' => $ret, '@command' => $command, '@output' => $output);
//     watchdog('islandora_remote_resource', $message, $variables, WATCHDOG_ERROR);
//     return FALSE;
//   }
//   return $dest;
// }

/**
 * Adds the given file as a datastream to the given object using the given datastream id to identify it.
 *
 * @param FedoraObject $object
 *   The object to add the datasteam to.
 * @param string $datastream_id
 *   The datastream id of the added datastream.
 * @param string $file_uri
 *   A URI to the file containing the content for the datastream.
 */
// function islandora_remote_resource_add_datastream($object, $datastream_id, $file_uri) {
//   try {
//     $mime_detector = new MimeDetect();
//     $ds = $object->constructDatastream($datastream_id, "M");
//     $ds->label = $datastream_id;
//     $ds->mimetype = $mime_detector->getMimetype($file_uri);
//     $ds->setContentFromFile(drupal_realpath($file_uri), FALSE);
//     $object->ingestDatastream($ds);
//     return TRUE;
//   } catch (exception $e) {
//     drupal_set_message(t("@message", array("@message" => $e->getMessage())));
//     return FALSE;
//   }
// }

function islandora_remote_resource_add_text_datastream(FedoraObject $object, $datastream_id, $text) {
	try {
		$ds = $object->constructDatastream($datastream_id, "M");
		$ds->label = $datastream_id;
		$ds->mimetype = 'text/plain';
		$ds->setContentFromString($text);
		$object->ingestDatastream($ds);
		return TRUE;
	} catch (exception $e) {
		drupal_set_message(t("@message", array("@message" => $e->getMessage())));
		return FALSE;
	}
}
