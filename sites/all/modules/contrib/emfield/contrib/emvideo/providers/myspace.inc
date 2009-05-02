<?php
// $Id: myspace.inc,v 1.1.2.9 2008/12/19 18:17:14 aaron Exp $

/**
 * @file
 *   This include processes myspace.com media files for use by emfield.module.
 */

define('EMVIDEO_MYSPACE_MAIN_URL', 'http://vids.myspace.com/index.cfm?fuseaction=vids.videos');

function emvideo_myspace_info() {
  $features = array(
    array(t('Autoplay'), t('No'), ''),
    array(t('RSS Attachment'), t('No'), ''),
    array(t('Thumbnails'), t('Yes'), ''),
  );
  return array(
    'provider' => 'myspace',
    'name' => t('MySpace'),
    'url' => EMVIDEO_MYSPACE_MAIN_URL,
    'settings_description' => t('These settings specifically affect videos displayed from <a href="@provider" target="_blank">MySpace</a>.', array('@provider' => EMVIDEO_MYSPACE_MAIN_URL)),
    'supported_features' => $features,
  );
}

function emvideo_myspace_settings() {
  $form = array();
  return $form;
}

function emvideo_myspace_request($code, $cacheable = TRUE) {
  $args = array();
  return module_invoke('emfield', 'request_xml', 'myspace', "http://vids.myspace.com/index.cfm?fuseaction=vids.individual&amp;videoid=$code", $args, $cacheable);
}

/**
 *  this scrapes the thumbnail from the video page and caches it
 */
function _emvideo_myspace_scrape_thumbnail($video, $cached = TRUE) {
  if (!$cached || !$cache = cache_get('myspace:thumbnail:'. $video, 'cache')) {
    $vidid = drupal_substr($video, 0, 10);

    if ($str = file_get_contents("http://vids.myspace.com/index.cfm?fuseaction=vids.individual&amp;videoid=$vidid")) {
      // grab videos channel-id first
      $channel_id = preg_replace('/.*tvchanid=([0-9]+);.*/si', '\1', $str);

      if (is_numeric($channel_id)) {
        // now from channel-page grab videos thumbnail
        $str2 = file_get_contents("http://vids.myspace.com/index.cfm?fuseaction=vids.viewVideos&amp;channelid=$channel_id");

        if ($str2) {
          $picturelink = preg_replace('@.*href="[^"]+'. $vidid .'"[^>]+><img[^>]+src="([^"]+)".*@si', '\1',  $str2);

          if ($picturelink) {
            $thumbnail = $picturelink;
            cache_set('myspace:thumbnail:'. $video, $thumbnail, 'cache', time() + 3600);
          }
        }
      }
    }
  }
  else {
    $thumbnail = $cache->data;
  }

  return $thumbnail;
}

/**
 *  Implements emvideo_PROVIDER_extract().
 */
function emvideo_myspace_extract($url) {
  // http://vids.myspace.com/index.cfm?fuseaction=vids.individual&videoid=43381231
  // <a href="http://myspacetv.com/index.cfm?fuseaction=vids.individual&videoid=14686340">What's That Floating In The Water Part 1</a><br><embed src="http://lads.myspace.com/videos/vplayer.swf" flashvars="m=14686340&v=2&type=video" type="application/x-shockwave-flash" width="430" height="346"></embed><br><a href="http://myspacetv.com/index.cfm?fuseaction=vids.addToProfileConfirm&videoid=14686340&title=What's That Floating In The Water Part 1">Add to My Profile</a> | <a href="http://myspacetv.com/index.cfm?fuseaction=vids.home">More Videos</a>
  return array(
    '@src="myspace\.com/index.cfm\?fuseaction=vids\.individual&amp;videoid=([^&"]+)@i',
    '@myspace\.com/index\.cfm\?fuseaction=vids\.individual&amp;videoid=([^&"]+)@i',
    '@myspace\.com/index\.cfm\?fuseaction=vids\.individual&videoid=([^&"]+)@i',
    '@src="myspacetv\.com/index.cfm\?fuseaction=vids\.individual&amp;videoid=([^&"]+)"@i',
    '@myspacetv\.com/index\.cfm\?fuseaction=vids\.individual&amp;videoid=([^&"]+)@i',
    '@myspacetv\.com/index\.cfm\?fuseaction=vids\.individual&videoid=([^&"]+)@i',
  );
}

/**
 *  Implements emvideo_PROVIDER_link().
 */
function emvideo_myspace_video_link($video_code) {
  return 'http://vids.myspace.com/index.cfm?fuseaction=vids.individual&amp;videoid='. $video_code;
}

function theme_emvideo_myspace_flash($embed, $width, $height) {
  static $count = 0;
  if ($embed) {
    $output .= "    <object type=\"application/x-shockwave-flash\" height=\"$height\" width=\"$width\" data=\"http://lads.myspace.com/videos/vplayer.swf\" id=\"emfield-video-myspace-$count\" >
      <param name=\"movie\" value=\"http://lads.myspace.com/videos/vplayer.swf\" />
      <param name=\"allowScriptAcess\" value=\"sameDomain\"/>
      <param name=\"quality\" value=\"best\"/>
      <param name=\"bgcolor\" value=\"#FFFFFF\"/>
      <param name=\"scale\" value=\"noScale\"/>
      <param name=\"salign\" value=\"TL\"/>
      <param name=\"FlashVars\" value=\"m=$embed&amp;type=video\" />
      <param name=\"wmode\" value=\"transparent\" />
    </object>\n";
  }
  $count++;
  return $output;
}

function emvideo_myspace_thumbnail($field, $item, $formatter, $node, $width, $height) {
  return _emvideo_myspace_scrape_thumbnail($item['value']);
}

function emvideo_myspace_video($embed, $width, $height, $field, $item, &$node) {
  $output = theme('emvideo_myspace_flash', $embed, $width, $height);
  return $output;
}

function emvideo_myspace_preview($embed, $width, $height, $field, $item, &$node) {
  $output = theme('emvideo_myspace_flash', $embed, $width, $height);
  return $output;
}

/**
 * Implementation of hook_emfield_subtheme.
 */
function emvideo_myspace_emfield_subtheme() {
  return array(
    'emvideo_myspace_flash' => array(
      'arguments' => array('embed' => NULL, 'width' => NULL, 'height' => NULL, 'autoplay' => NULL),
      'file' => 'providers/myspace.inc'
    )
  );
}
