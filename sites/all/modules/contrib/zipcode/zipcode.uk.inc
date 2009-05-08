<?php
// $Id: zipcode.uk.inc,v 1.8 2009/03/07 23:52:51 thierrygd Exp $

// Copyright 2008 Thierry GUEGAN http://www.arvoriad.com 

/**
 * @file
 * CCK Field for British zipcodes.
 */
 
/** 
 * Verification for British zipcodes.  
 *
 * @param string $text
 * @return boolean Returns boolean FALSE if the zipcode is not valid. 
 * On success, returns a string containting the zipcode with some formatting.
 */
function valid_uk_zipcode($zipcodestring) {

  $zipcodestring = trim($zipcodestring);

  // See http://www.govtalk.gov.uk/gdsc/html/noframes/PostCode-2-1-Release.htm
  $in  = 'ABDEFGHJLNPQRSTUWXYZ';
  $fst = 'ABCDEFGHIJKLMNOPRSTUWYZ';
  $sec = 'ABCDEFGHJKLMNOPQRSTUVWXY';
  $thd = 'ABCDEFGHJKSTUW';
  $fth = 'ABEHMNPRVWXY';
  $num = '0123456789';
  $nom = '0123456789';
  $gap = '\s\.';

  if (preg_match("/^[$fst][$num][$gap]*[$nom][$in][$in]$/i", $zipcodestring) ||
      preg_match("/^[$fst][$num][$num][$gap]*[$nom][$in][$in]$/i", $zipcodestring) ||
      preg_match("/^[$fst][$sec][$num][$gap]*[$nom][$in][$in]$/i", $zipcodestring) ||
      preg_match("/^[$fst][$sec][$num][$num][$gap]*[$nom][$in][$in]$/i", $zipcodestring) ||
      preg_match("/^[$fst][$num][$thd][$gap]*[$nom][$in][$in]$/i", $zipcodestring) ||
      preg_match("/^[$fst][$sec][$num][$fth][$gap]*[$nom][$in][$in]$/i", $zipcodestring)) {
      return TRUE;
    } 
  else 
    {
      return FALSE;
    }
}
	
/**
 * Formatting for zipcode.  
 *
 * @param string $zipcodestring
 * @return string Returns a string containting the zipcode with some formatting.
 */
function format_uk_zipcode($zipcodestring) {

  $zipcodestring = strtoupper(trim($zipcodestring));
  return $zipcodestring;
}


