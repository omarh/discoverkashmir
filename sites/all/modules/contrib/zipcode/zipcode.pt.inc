<?php
// $Id: zipcode.pt.inc,v 1.5 2009/03/07 23:52:51 thierrygd Exp $

// Copyright 2008 Thierry GUEGAN http://www.arvoriad.com 

/**
 * @file
 * CCK Field for Portuguese zipcodes.
 */
 
/**  
 * Verification for Portuguese zipcodes.  
 *
 * @param string $text
 * @return boolean Returns boolean FALSE if the zipcode is not valid. 
 * On success, returns a string containting the zipcode with some formatting.
 */
function valid_pt_zipcode($zipcodestring) {
  $zipcodestring = trim($zipcodestring);

  if (!preg_match("/(^\d{4}$)|(^\d{4}-\d{3}$)$/i",$zipcodestring)) {
      return FALSE;
  }
  //9999 or 9999-999
  else
  {
    return TRUE;
  }
}
	
/**
 * Formatting for zipcode.  
 *
 * @param string $zipcodestring
 * @return string Returns a string containting the zipcode with some formatting.
 */
function format_pt_zipcode($zipcodestring) {

  $zipcodestring = trim($zipcodestring);
  //do some formatting 
  return $zipcodestring;
}
