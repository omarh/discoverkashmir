<?php
// $Id: guba.inc,v 1.1.2.6 2008/12/19 18:17:14 aaron Exp $

/**
 * @file
 *   Provide support for the Guba provider to the emfield.module.
 */

define('EMVIDEO_GUBA_MAIN_URL', 'http://www.guba.com/');
define('EMVIDEO_GUBA_API_INFO', 'http://www.guba.com/restApiDoc/restReference.html');
define('EMVIDEO_GUBA_API_APPLICATION_URL', 'http://www.guba.com/developers.html');

function emvideo_guba_info() {
  $features = array(
    array(t('Autoplay'), t('Yes'), ''),
    array(t('RSS Attachment'), t('No'), ''),
    array(t('Show related videos'), t('No'), t('This is embedded in the video itself when enabled; currently not available with other providers. Set the feature above.')),
    array(t('Thumbnails'), t('Yes'), t('May not currently resize thumbnails. Must have an API key for thumbnails at the moment, although research is underway to determine an alternative to this. Set your API key above.')),
  );
  return array(
    'provider' => 'guba',
    'name' => t('GUBA'),
    'url' => EMVIDEO_GUBA_MAIN_URL,
    'settings_description' => t('These settings specifically affect videos displayed from <a href="@guba" target="_blank">GUBA</a>. You can learn more about its <a href="@api" target="_blank">API</a> here.', array('@guba' => EMVIDEO_GUBA_MAIN_URL, '@api' => EMVIDEO_GUBA_API_INFO)),
    'supported_features' => $features,
  );
}

function emvideo_guba_settings() {
  $form['guba']['api'] = array(
    '#type' => 'fieldset',
    '#title' => t('GUBA API'),
    '#description' => t('If you wish to be able to display GUBA thumbnails automatically, you will first need to apply for an API Developer Key from the <a href="@guba" target="_blank">GUBA Developer Profile page</a>. Note that you do not need this key to display GUBA videos themselves.', array('@guba' => EMVIDEO_GUBA_API_APPLICATION_URL)),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['guba']['api']['emvideo_guba_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('GUBA API Key'),
    '#default_value' => variable_get('emvideo_guba_api_key', ''),
    '#description' => t('Please enter your GUBA Developer Key here.'),
  );
  $form['guba']['api']['emvideo_guba_api_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('GUBA API Secret'),
    '#default_value' => variable_get('emvideo_guba_api_secret', ''),
    '#description' => t('If you have a secret for the GUBA API, enter it here.'),
  );
  return $form;
}


function emvideo_guba_request($method, $args = array(), $cached = TRUE) {
  //$args = array('docid' => $embed);
  return module_invoke('emfield', 'request_xml', 'guba', "http://www.guba.com/rest/video/". $method ."/details", $args, $cacheable);
}

function emvideo_guba_extract($embed = '') {
  return array(
    '@http://www\.guba\.com/watch/([^"\?]+)@i',
    '@http://www\.guba\.com/f/root\.swf\?video_url=http://free\.guba\.com/uploaditem/([^"\?/]*)@i',
  );
}

function emvideo_guba_embedded_link($video_code) {
  return 'http://www.guba.com/watch/'. $video_code;
}

function theme_emvideo_guba_flash($embed, $width, $height, $autoplay) {
  $autoplay = $autoplay ? '&amp;autoPlay=TRUE' : '&amp;autoPlay=FALSE';
  $output .= '<embed src="http://www.guba.com/f/root.swf?video_url=http://free.guba.com/uploaditem/'. $embed .'/flash.flv&amp;isEmbeddedPlayer=FALSE'. $autoplay .'" quality="best" bgcolor="#FFFFFF" menu="TRUE" width="'. $width .'" height="'. $height .'" name="root" id="root" align="middle" scaleMode="noScale" allowScriptAccess="always" allowFullScreen="TRUE" type="application/x-shockwave-flash" pluginspage="http://www.macromedia.com/go/getflashplayer"></embed>';
  return $output;
}

function emvideo_guba_thumbnail($field, $item, $formatter, $node, $width, $height) {
  $thumbid = $item['value'];
  $thm = 'http://img.guba.com/public/video/f/58/'. $thumbid .'-m.jpg';
  return $thm;
}

function emvideo_guba_video($embed, $width, $height, $field, $item, &$node, $autoplay) {
  $output = theme('emvideo_guba_flash', $embed, $width, $height, $autoplay);
  return $output;
}

function emvideo_guba_preview($embed, $width, $height, $field, $item, &$node, $autoplay) {
  $output = theme('emvideo_guba_flash', $embed, $width, $height, $autoplay);
  return $output;
}

function emvideo_guba_emfield_subtheme() {
  return array(
    'emvideo_guba_flash' => array(
      'arguments' => array('embed' => NULL, 'width' => NULL, 'height' => NULL, 'autoplay' => NULL),
      'file' => 'providers/guba.inc'
    )
  );
}
