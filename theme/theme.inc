<?php

/**
 * @file
 * This file contains all theme and preprocess functions
 */

/**
 *
 * @global type $base_url
 * @param array $variables
 *   an array of variables that will be passed to the theme function
 */
function islandora_remote_resource_preprocess_islandora_remote_resource(&$variables) {
  drupal_add_js('misc/form.js');
  drupal_add_js('misc/collapse.js');
  $islandora_object = $variables['islandora_object'];
  $repository = $islandora_object->repository;
  module_load_include('inc', 'islandora', 'includes/datastream');
  module_load_include('inc', 'islandora', 'includes/utilities');

  try {
    $dc = $islandora_object['DC']->content;
    $dc_object = DublinCore::importFromXMLString($dc);
  } catch (Exception $e) {
    drupal_set_message(t('Error retrieving object %s %t', array('%s' => $islandora_object->id, '%t' => $e->getMessage())), 'error', FALSE);
  }
  $variables['islandora_dublin_core'] = isset($dc_object) ? $dc_object : NULL;
  $variables['dc_array'] = isset($dc_object) ? $dc_object->asArray() : array();
  $variables['islandora_object_label'] = $islandora_object->label;
  $variables['theme_hook_suggestions'][] = 'islandora_remote_resource__' . str_replace(':', '_', $islandora_object->id);
  $variables['parent_collections'] = islandora_get_parents_from_rels_ext($islandora_object);
  global $base_url;
  
  // full size url
  if (isset($islandora_object['LINK'])) {
    $full_size_url = url(islandora_datastream_get_url($islandora_object['LINK'], 'view'), array('absolute' => TRUE));
    $variables['islandora_full_url'] = $full_size_url;
    
    $download_url = islandora_datastream_get_url($islandora_object['LINK'], 'download');
    $download_text = t("View the Resource");
    $variables['islandora_download_link'] = l($download_text, $download_url, array('attributes' => array('class' => array('islandora-remote-resource-link'))));
    $variables['islandora_content'] = $variables['islandora_download_link'];
  }
  // thumbnail
  if (isset($islandora_object['TN'])) {
    $thumbnail_size_url = url(islandora_datastream_get_url($islandora_object['TN'], 'view'));
    $params = array(
      'title' => $islandora_object->label,
      'path' => $thumbnail_size_url
    );
    $variables['islandora_thumbnail_img'] = theme('image', $params);
  }
  
  // preview image + link
  if (isset($islandora_object['PREVIEW'])) {
    $preview_url = url(islandora_datastream_get_url($islandora_object['PREVIEW'], 'view'));
    $params = array(
      'title' => $islandora_object->label,
      'path' => $preview_url
    );
    $variables['islandora_preview_img'] = theme('image', $params);
    $variables['islandora_content'] = l($variables['islandora_preview_img'], $variables['islandora_full_url'], array('html' => TRUE));
  }
}
