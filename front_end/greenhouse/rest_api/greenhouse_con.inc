<?php

function greenhouse_get_sensor_array() {
    $rest_api = variable_get('greenhouse_rest_api');
    $result = drupal_http_request($rest_api."/sensors");
    if ($result->code != 200) {
        return null;
    }
    
    $returns = json_decode($result->data,TRUE);
    return $returns;
}

function greenhouse_get_power_array(){
    $rest_api = variable_get('greenhouse_rest_api');
    $result = drupal_http_request($rest_api."/powers");
    if ($result->code != 200) {
        return null;
    }
    $returns = json_decode($result->data,TRUE);
    return $returns;    
}

function greenhouse_post_sensor($id,$body) {
    $rest_api = variable_get('greenhouse_rest_api');
    $result = drupal_http_request($rest_api."/sensors/$id",
            array ('method'=>'POST',
                'data'=>$body));
   if ($result->code == 200) {
        return true;
   }
   
   return false;
    
}

function greenhouse_post_power($id,$body) {

    $rest_api = variable_get('greenhouse_rest_api');
    $result = drupal_http_request($rest_api."/power/$id",
            array ('method'=>'POST',
                'data'=>$body));
   if ($result->code == 200) {
        return true;
   }
   return false;
    
}

function greenhouse_rest_delete_power($id) {
    $rest_api = variable_get('greenhouse_rest_api');
    $result = drupal_http_request($rest_api . "/powers/$id", array('method' => 'DELETE'));

    if ($result->code == 200) {
        return true;
    }

    return false;
}

function greenhouse_rest_delete_sensor($id) {
    $rest_api = variable_get('greenhouse_rest_api');
    $result = drupal_http_request($rest_api . "/sensors/$id", array('method' => 'DELETE'));

    if ($result->code == 200) {
        return true;
    }

    return false;
}

function greenhouse_day_info($sensor_id,$start,$end) {
    
    $rest_api = variable_get('greenhouse_rest_api');
    $result = drupal_http_request($rest_api."/sensor_data/$sensor_id/$start/$end");
    if($result->code != 200) {
        return array();
    }
    
    $returns = json_decode($result->data,TRUE);
    return $returns;
}