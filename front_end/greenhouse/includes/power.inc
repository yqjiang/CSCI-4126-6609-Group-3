<?php


function greenhouse_power() {
    // add sensor form include
    module_load_include('inc', 'greenhouse', 'rest_api/greenhouse_con');
     //show cureent sensors.
    $powers = greenhouse_get_power_array();
    $display = "";
    foreach($powers as $power => $info) {
        global $base_url;
        $delete_link = $base_url .'/greenhouse/power/delete/'.$info['ID'];
        $display .= '<div class="sensor_infomation highlighted-block light">'
                . '<div class="row"><div class="col-md-8">'
                . '<h5>'
                . $info['Name']
                . '</h5></div>'
                . '<div class="col-md-2">'
                . '<a class="btn" href="'
                . $base_url.'/greenhouse/power/update/'.$info['ID']
                . '">Edit</a>'
                . '</div>'
                . '<div class="col-md-2">'
                . '<a class="btn" href="'
                . $delete_link
                . '">Delete</a></div>'
                . '</div>'
                . '<ul>';
        foreach($info as $infoname => $infoitem) {
            if(!is_array($infoitem)){
                 $display .= "<li> <b>".$infoname."</b>&nbsp:".$infoitem."</li>";
            }
        }
        $display .= "</ul>";
        $display .= "</div>"
                . "<p></p>";
    }
    
    $display .= "<div class=\"add_sensor_form highlighted-block light\">";
    $add_form = drupal_get_form('greenhouse_addpower_form');
    $display .= drupal_render($add_form);
    $display .= "</div><p></p>";
    return $display;
}

function greenhouse_addpower_form($form, &$form_state) {
    $form['id'] = array(
        '#type' => 'textfield',
        '#title' => t('ID'),
        '#default_value' => '',
        '#size' => 60,
        '#maxlength' => 128,
        '#required' => TRUE,);
    $form['name'] = array(
        '#type' => 'textfield',
        '#title' => t('Name'),
        '#default_value' => '',
        '#size' => 60,
        '#maxlength' => 128,
        '#required' => TRUE,);
    $form['description'] = array(
        '#title' => t('Description'),
        '#type' => 'textarea',
        '#description' => t('The sensor discription'),
        '#default_value' => '',
    );
    $form['enable'] = array(
        '#type' => 'checkbox',
        '#default_value' => TRUE,
        '#title' => t('Enable this sensor'),
    );
    $form['status'] = array(
        '#type' => 'textfield',
        '#title' => t('Status'),
        '#default_value' => '',
        '#size' => 60,
        '#maxlength' => 128,
    );
    $form['add'] = array(
        '#type' => 'submit',
        '#value' => t('Add'),
    );

    return $form;
}

function greenhouse_addpower_form_submit($form, &$form_state) {
    $json_array = array(
        'name'=> $form_state['values']['name'],
        'description'=> isset($form_state['values']['description'])?
                $form_state['values']['description']:'',
        'enable'=> $form_state['values']['enable'],
        'status'=> isset($form_state['values']['status'])?
            $form_state['values']['status']:'',
    );    
    $json = json_encode($json_array);
    module_load_include('inc', 'greenhouse', 'rest_api/greenhouse_con');
    greenhouse_post_power($form_state['values']['id'], $json);
}

function greenhouse_delete_power($form,&$form_state,$id) {
    $form['notice'] = array(
        '#type' => 'markup',
        '#markup' => t('This Process can not Undo. Please be careful'),
        '#prefix' => '<div>',
        '#suffix' => '</div>',
    );
   $form['id'] = array(
        '#type' => 'hidden',
        '#value' => $id,
    );
   
    $form['add'] = array(
        '#type' => 'submit',
        '#value' => t('Delete'),
    );
    
    return $form;
}

function greenhouse_delete_power_submit($form,&$form_state) {
    module_load_include('inc', 'greenhouse','rest_api/greenhouse_con');
    greenhouse_rest_delete_power($form_state['values']['id']);
    global $base_url;
    drupal_goto($base_url .'/greenhouse/power/');
}

function greenhouse_update_power($form, &$form_state,$id) {
    module_load_include('inc', 'greenhouse','rest_api/greenhouse_con');
    $result = greenhouse_get_power_array();
    $data = null;
    foreach ($result as $item){
        if(isset($item['ID'])&&$item['ID']===$id){
            $data = $item;
        }
    }
    
    if(!$data) {
        return array();
    }
    $form['show'] = array(
        '#type' => 'item',
        '#title' => t('ID'),
        '#markup' => $id,
        );
    $form['id'] = array(
        '#type' => 'value',
        '#value' => $id,
        );
    $form['name'] = array(
        '#type' => 'textfield',
        '#title' => t('Name'),
        '#default_value' => $data['Name'],
        '#size' => 60,
        '#maxlength' => 128,
        '#required' => TRUE,);
    $form['description'] = array(
        '#title' => t('Description'),
        '#type' => 'textarea',
        '#description' => t('The sensor discription'),
        '#default_value' => $data['Description'],
    );
    $form['enable'] = array(
        '#type' => 'checkbox',
        '#default_value' => $data['Enable'],
        '#title' => t('Enable this sensor'),
    );
    $form['status'] = array(
        '#type' => 'textfield',
        '#title' => t('Status'),
        '#default_value' => $data['Status'],
        '#size' => 60,
        '#maxlength' => 128,
    );
    $form['add'] = array(
        '#type' => 'submit',
        '#value' => t('Edit'),
    );

    return $form;
}

function greenhouse_update_power_submit($form, &$form_state) {
    $json_array = array(
        'name'=> $form_state['values']['name'],
        'description'=> isset($form_state['values']['description'])?
                $form_state['values']['description']:'',
        'enable'=> $form_state['values']['enable'],
        'status'=> isset($form_state['values']['status'])?
            $form_state['values']['status']:'',
    );    
    $json = json_encode($json_array);
    module_load_include('inc', 'greenhouse', 'rest_api/greenhouse_con');
    greenhouse_post_power($form_state['values']['id'], $json);
}