<?php


function greenhouse_setting_form() {
    module_load_include('inc', 'greenhouse','rest_api/greenhouse_resttest');
    $url = variable_get('greenhouse_rest_api');
    $test = greenhouse_test_rest_root($url);
   
    $form = array();
    $form['resthost'] = array(
        '#type' => 'textfield',
        '#title' => t('REST API Url'),
        '#description' => t('e.g. https://projects.cs.dal.ca/greenhouse/rest_api'),
        '#default_value' => variable_get('greenhouse_rest_api'),
        '#size' => 60,
        '#maxlength' => 128,
        '#required' => TRUE,
    );
    
    $form['resthost_test'] = array(
        '#type' => 'item',
        '#title' => t('REST API Status'),
        '#prefix' => '<div class="status-wrapper">',
        '#surfix' => '</div>',
        '#markup' => $test?t('working'):  t('notworking'),
    );
    
        
    $form['task'] = array(
        '#type' => 'textfield',
        '#title' => t('Smart Scheduler URL'),
        '#description' => t('e.g. https://projects.cs.dal.ca/greenhouse/task/'),
        '#default_value' => variable_get('greenhouse_smart_scheduler'),
        '#size' => 60,
        '#maxlength' => 128,
        '#required' => TRUE,
    );
    
    $form['change'] = array(
        '#type' => 'submit',
        '#value' => t('Change The URL'),  
    );
    return $form;
}


function greenhouse_setting_form_submit($form,$form_state) {
    variable_set('greenhouse_rest_api',$form_state['values']['resthost']);
    variable_set('greenhouse_smart_scheduler',$form_state['values']['task']);
}

function greenhouse_settings() {
    $form = drupal_get_form('greenhouse_setting_form');
    return $form;
}