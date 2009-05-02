<?php
// $Id: zipcode.fr.inc,v 1.8 2009/03/07 23:52:51 thierrygd Exp $

// Copyright 2008 Thierry GUEGAN http://www.arvoriad.com 

/**
 * @file
 * CCK Field for French zipcodes.
 */
 
/**  
 * Verification for French zipcodes.  
 *
 * @param string $text
 * @return boolean Returns boolean FALSE if the zipcode is not valid. 
 * On success, returns a string containting the zipcode with some formatting.
 */
function valid_fr_zipcode($zipcodestring) {

  $zipcodestring = trim($zipcodestring);

  if (!preg_match("/^[0-9]{4,5}$/i",$zipcodestring)) { 
  	return FALSE; 
  }
  //right now just do a quick check ==> has to be improved 
  //need to check more precisely by doing a query on a zipcode table, ...
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
function format_fr_zipcode($zipcodestring) {

  $zipcodestring = trim($zipcodestring);
  //do some formatting 
  return $zipcodestring;
}
