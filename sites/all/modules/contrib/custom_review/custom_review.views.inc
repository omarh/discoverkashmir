<?php
function custom_review_views_data() {
  $data['custom_reviews']['table']['group'] = t('Custom reviews');
  //$data['node_reviewed']['table']['group'] = t('Custom reviews');

  $data['custom_reviews']['table']['join'] = array (
    'node' => array(
      'left_field' => 'nid',
      'field' => 'rid',
    ),
  );
  
  $data['custom_reviews']['rid'] = array(
    'title' => t('Custom Reviews'),
    'help' => t('The node the uploaded file is attached to'),
    'relationship' => array(
      'label' => t('Custom Reviews'),
      'base' => 'node',
      'base field' => 'nid',
      // This allows us to not show this relationship if the base is already
      // node so users won't create circular relationships.
    ),
  );
  
  /*
  //return link between a nid value in table custom_review and a node reviewed
  $data['node_reviewed']['table']['join'] = array(
  
  
    'name' => 'node',
    'custom_reviews' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );
  */

  $data['custom_reviews']['nid'] = array(
     'title' => t('Custom review: nid reviewed'),
     'help' => t('Display the nid being reviewed'),
  );

  /*
  $data['node_reviewed']['table']['join'] = array(
   'node' => array(
        'left_table' => 'custom_review',
        'left_field' => 'nid',
        'field' => 'tid',
    ),
    'right' => array(
    	'field' => 'nid',
    ),
  );

  $data['node_reviewed']['title'] = array(
     'title' => t('Custom review: node reviewed title'),
     'help' => t('display the node reviewed title'),
     'field' => array(
       'handler'  => 'views_handler_field_node',
       'click sortable' => false,
    ),
  );

  $data['node_reviewed']['type'] = array(
     'title' => t('Custom review: node reviewed type'),
     'help' => t('display the node reviewed type'),
     'argument' => array(
       'handler'  => 'views_handler_argument_custom_review_reviewed',
       'click sortable' => false,
     ),
  );
   
  $data['node_reviewed']['reviewed_nid'] = array(
    'title' => t('Custom review: nid to be reviewed'),
    'help' => t('Takes reviews which are related to a node nid'),
    'argument' => array(
      'handler'  => 'views_handler_argument_custom_review_reviewed_nid'
    ),
  );
  */

  /*
  $data['node']['reviewer'] = array(
    'title' => t('Custom review: reviewer type'),
    'help' => t('Takes reviews which are done using a specific reviewer type'),
    'argument' => array(
      'handler' => 'custom_review_reviewer_views_handler_argument'
    ),
  );
*/
  /*
  $data['node_reviewed']['uid'] = array(
    'title' => t('Custom review: review uid'),
    'help'  => t('Takes reviews which are made by this user'),
    'argument' => array(
      'handler'  => 'views_handler_argument_custom_review_uid'
    ),
  );
  */
  
  /*
  $data['node_reviewed']['reviewed'] = array(
    'title' => t('Custom review: reviewed type'),
    'help' => t('Takes reviews which are made on a specific reviewed type node'),
    'argument' => array(
    	'handler' => 'custom_review_reviewed_views_handler_argument',
    ),
  );
  */
  return $data;
}

class views_handler_argument_custom_review_reviewed_nid extends views_handler_argument {
  function query() {
    $this->ensure_my_table();
    $this->query->add_where(0, "$this->table_alias.nid = %d", $this->argument);
  }
}

class views_handler_argument_custom_review_uid extends views_handler_argument {

  function query() {
    $this->ensure_my_table();
    $this->query->add_where(0, "$this->table_alias.uid = %d", $this->argument);
  }
}

class views_handler_argument_custom_review_reviewed extends views_handler_argument {
  function query() {
    dump($this->query);
    global $_custom_review_reviewers;
    if (empty($_custom_review_reviewers)) {
      $_custom_review_reviewers = custom_review_get_reviewers();
    }
    //if ($this->argument != 'all') {
      $this->ensure_my_table();
      $this->query->add_where(0,"node_reviewed.type = '%s'", $this->argument);
    //}
    //else {
      //$this->query->add_where(0,"node.type IN ('". implode('\',\'', array_keys($_custom_review_reviewers)) ."')");
    //}
    echo "hiha";
    dump($this->query);
  }
}

/*
class views_handler_argument_custom_review_reviewer extends views_handler_argument {
  function query() {
    $this->ensure_my_table();
    $this->query->add_where(0, "node.type = %s", $this->argument);
  }
}
*/