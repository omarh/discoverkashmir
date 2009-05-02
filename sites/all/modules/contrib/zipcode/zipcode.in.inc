<?php
// $Id: zipcode.in.inc,v 1.8 2009/05/02 $

/**
 * @file
 * CCK Field for Indian zipcodes.
 */
 
/** 
 * Verification for Indian zipcodes.  
 *
 * @param string $text
 * @return boolean Returns boolean FALSE if the zipcode is not valid. 
 * On success, returns a string containting the zipcode with some formatting.
 */
function valid_in_zipcode($zipcodestring) {

  $zipcodestring = trim($zipcodestring);
  if (!preg_match("/(^\d{6})/",$zipcodestring)) { 
  	return FALSE; 
  }
  //190003
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
function format_us_zipcode($zipcodestring) {

  $zipcodestring = trim($zipcodestring);
  //do some formatting 
  return $zipcodestring;
}
