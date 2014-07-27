<?php

/**
 * This function is to test if the REST API is working
 */

function greenhouse_test_rest_root($url) {
    $result = drupal_http_request($url);
    if($result->code != 200) {
        return false;
    }
    $json = json_decode($result->data);
    if(strcasecmp($json->API, 'Dalgreenhouse') != 0) {
        return false;
    }
    
    return true;
}