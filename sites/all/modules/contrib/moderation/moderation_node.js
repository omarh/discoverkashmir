Drupal.moderationInit = function() {
  var id = $(this).attr("id").substring(16);
  
  $(this).after('<div id="moderation-preview-'+id+'" class="moderation-preview"></div>');
  $('#moderation-preview-'+id).hide();
}

Drupal.moderationPreview = function() {
  $(this).click(function () {
    var id = $(this).parent().attr("id").substring(17);
    var preview = $('#moderation-preview-'+id);

    if (preview.html().length == 0) {
      //load node data
      var html = $.ajax({
        url: "?q=moderation/node/"+id+"/get/preview",
        dataType: "json",
        success: function(html){
          preview.html(html);
        }
      });
    }
    preview.toggle();

    $.ajax({
      url: "?q=moderation/node/"+id+"/get/moderate",
      dataType: "json",
      success: function(status) {
        $('#moderation-preview-'+id+' .moderation-messages').remove();
        if (status == 1) {
          $('#moderation-preview-'+id).prepend('<div class="moderation-messages status">'+Drupal.t('This item has been moderated in the meantime.')+'</div>');
        }
      }
    });

    return false;
  });
}

Drupal.moderationButtonStatus = function() {
  var nid = $(this).attr("id").substring(18);

  //insert buttons
  if ($(this).attr("id").match('moderation-status-')) $(this).html('<a id="moderation-status-link-'+nid+'" href="#">'+$(this).html()+'</a>');
  $('#moderation-status-link-'+nid).click(function () {
    //load node data
    var html = $.ajax({
      url: "?q=moderation/node/"+nid+"/set/status",
      dataType: "json",
      success: function(result){
        if (result[0]) {
          if (result[1]) $('#moderation-status-link-'+nid).html(Drupal.t('published'));
          else $('#moderation-status-link-'+nid).html(Drupal.t('not published'));
        }
      }
    });
    return false;
  });
}

Drupal.moderationButtonPromote = function() {
  var nid = $(this).attr("id").substring(19);

  //insert buttons
  if ($(this).attr("id").match('moderation-promote-')) $(this).html('<a id="moderation-promote-link-'+nid+'" href="#">'+$(this).html()+'</a>');
  $('#moderation-promote-link-'+nid).click(function () {
    //load node data
    var html = $.ajax({
      url: "?q=moderation/node/"+nid+"/set/promote",
      dataType: "json",
      success: function(result){
        if (result[0]) {
          if (result[1]) $('#moderation-promote-link-'+nid).html(Drupal.t('promoted'));
          else $('#moderation-promote-link-'+nid).html(Drupal.t('not promoted'));
        }
      }
    });
    return false;
  });
}

Drupal.moderationButtonSticky = function() {
  var nid = $(this).attr("id").substring(18);
  
  //insert buttons
  if ($(this).attr("id").match('moderation-sticky-')) $(this).html('<a id="moderation-sticky-link-'+nid+'" href="#">'+$(this).html()+'</a>');
  $('#moderation-sticky-link-'+nid).click(function () {
    //load node data
    var html = $.ajax({
      url: "?q=moderation/node/"+nid+"/set/sticky",
      dataType: "json",
      success: function(result){
        if (result[0]) {
          if (result[1]) $('#moderation-sticky-link-'+nid).html(Drupal.t('sticky'));
          else $('#moderation-sticky-link-'+nid).html(Drupal.t('not sticky'));
        }
      }
    });
    return false;
  });
}

Drupal.moderationButtonModerate = function() {
  var nid = $(this).attr("id").substring(20);

  //insert buttons
  if ($(this).attr("id").match('moderation-moderate-')) $(this).html('<a id="moderation-moderate-link-'+nid+'" href="#">'+$(this).html()+'</a>');
  $('#moderation-moderate-link-'+nid).click(function () {
    //load node data
    var html = $.ajax({
      url: "?q=moderation/node/"+nid+"/set/moderate",
      dataType: "json",
      success: function(result){
        if (result[0]) {
          if (result[1]) {
            $('#moderation-moderate-link-'+nid).html(Drupal.t('moderated'));
            $('#moderation-preview-'+nid+':visible').slideUp();
          }
          else {
            $('#moderation-moderate-link-'+nid).html(Drupal.t('not moderated'));
          }
        }
      }
    });
    return false;
  });
}

// Global Killswitch
if (Drupal.jsEnabled) {
  $(document).ready(function() {
    $('.moderation-info').each(Drupal.moderationInit);
    $('.moderation-node .title a').each(Drupal.moderationPreview);

    $('.moderation-status').each(Drupal.moderationButtonStatus);
    $('.moderation-promote').each(Drupal.moderationButtonPromote);
    $('.moderation-sticky').each(Drupal.moderationButtonSticky);
    $('.moderation-moderate').each(Drupal.moderationButtonModerate);
    
    $('.moderation-moderate, .moderation-sticky, .moderation-promote, .moderation-status').show();
  });
}
