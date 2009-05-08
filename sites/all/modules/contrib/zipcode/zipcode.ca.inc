<?php
// $Id: zipcode.ca.inc,v 1.8 2009/03/07 23:52:51 thierrygd Exp $

// Copyright 2008 Thierry GUEGAN http://www.arvoriad.com 

/**
 * @file
 * CCK Field for Canadian zipcodes.
 */
 
/**  
 * Verification for Canadian zipcodes.  
 *
 * @param string $text
 * @return boolean Returns boolean FALSE if the zipcode is not valid. 
 * On success, returns a string containting the zipcode with some formatting.
 */
function valid_ca_zipcode($zipcodestring) {

  $zipcodestring = trim($zipcodestring);

  if (!preg_match("/^\D{1}\d{1}\D{1}[ ]?\d{1}\D{1}\d{1}/i",$zipcodestring)) { 
  	return FALSE; 
  }
  //Z5Z 5Z5 orZ5Z5Z5 
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
function format_ca_zipcode($zipcodestring) {

  $zipcodestring = strtoupper(trim($zipcodestring));
  //do some formatting 
  if (strlen($zipcodestring) == 6) {
  	$zipcodestring = substr($zipcodestring, 0, 3) . ' ' . substr($zipcodestring, 3, 3);
  }
  
  return $zipcodestring;
}

