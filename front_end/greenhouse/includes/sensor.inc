<?php

function greenhouse_sensor() {
    // add sensor form include
    module_load_include('inc', 'greenhouse', 'rest_api/greenhouse_con');

    //show cureent sensors.
    $sensors = greenhouse_get_sensor_array();
    $display = "";
    foreach ($sensors as $sensor => $info) {
        global $base_url;
        $delete_link = $base_url .'/greenhouse/sensors/delete/'.$info['ID'];
        $display .= '<div class="sensor_infomation highlighted-block light">'
                . '<div class="row"><div class="col-md-8">'
                . '<h5>'
                . $info['Name']
                . '</h5></div>'
                . '<div class="col-md-2">'
                . '<a class="btn" href="'
                . $base_url.'/greenhouse/sensor/update/'.$info['ID']
                . '">Edit</a>'
                . '</div>'
                . '<div class="col-md-2">'
                . '<a class="btn" href="'
                . $delete_link
                . '">Delete</a></div>'
                . '</div>'
                . '<ul>';
        foreach ($info as $infoname => $infoitem) {
            if (!is_array($infoitem)) {
                $display .= "<li> <b>" . $infoname . "</b>&nbsp:" . $infoitem . "</li>";
            }
        }
        $display .= "</ul>";
        if (isset($info['Data'])) {
            $display .= greenhouse_get_sensor_data_table($info['Data']);
        }
        $display .= "</div>"
                . "<p></p>";
    }

    $display .= "<div class=\"add_sensor_form highlighted-block light\">";
    $add_form = drupal_get_form('greenhouse_addsensor_form');
    $display .= drupal_render($add_form);
    $display .= "</div><p></p>";
    return $display;
}

function greenhouse_get_sensor_data_table($data) {
    $display = '<table class="table table-condensed">';
    $title_array = array("DataName", "Sensor", "Upper", "Lower");
    $display .= '<tr>';
    foreach ($title_array as $title) {
        $display .= '<th>';
        $display .= $title;
        $display .= '</th>';
    }
    $display .= '</tr>';
    foreach ($data as $item_title => $item) {
        $display .= '<tr>';
        foreach ($title_array as $title) {
            if (isset($item[$title])) {
                $display .= "<td>" . $item[$title] . "</td>";
            } else {
                $display .= "<td>NULL</td>";
            }
        }
        $display .= '</tr>';
    }
    $display .= "</table>";
    return $display;
}

function greenhouse_addsensor_form($form, &$form_state) {
    module_load_include('inc', 'greenhouse', 'add_sensor');
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
    $form['#tree'] = true;


    $form['data'] = array(
        '#type' => 'fieldset',
        '#title' => t('Data Set'),
        '#collapsible' => FALSE,
        '#prefix' => '<div id="greenhouse_sensor_data_ajax_wrapper_0">',
        '#suffix' => '</div>',
    );

    if (empty($form_state['num_data'])) {
        $form_state['num_data'] = 1;
    }
    
    for ($i = 0; $i < $form_state['num_data']; $i++) {
        $form['data'][$i] = array(
            '#prefix' => '<div>',
            '#suffix' => '</div>'
        );
        $form['data'][$i]['title'] = array(
            '#type' => 'textfield',
            '#title' => t('Data title'),
            '#default_value' => '',
            '#size' => 60,
            '#maxlength' => 128,
        );
        $form['data'][$i]['upbound'] = array(
            '#type' => 'textfield',
            '#title' => t('Upper Bound'),
            '#default_value' => '',
            '#size' => 60,
            '#maxlength' => 128,
        );
        $form['data'][$i]['lobound'] = array(
            '#type' => 'textfield',
            '#title' => t('Lower Bound'),
            '#default_value' => '',
            '#size' => 60,
            '#maxlength' => 128,
        );
    }

    $form['add data'] = array(
        '#type' => 'submit',
        '#value' => t('Add data'),
        '#submit' => array('greenhouse_sensor_add_one'),
        '#ajax' => array(
            'callback' => 'greenhouse_sensor_data_ajax_callback',
            'wrapper' => 'greenhouse_sensor_data_ajax_wrapper_0',
           // 'method'=>'append',
            'effect' => 'fade'
        )
    );
    $form['add'] = array(
        '#type' => 'submit',
        '#value' => t('Add'),
    );

    return $form;
}

function greenhouse_sensor_data_ajax_callback($form, &$form_state) {
    return $form['data'];
}

function greenhouse_sensor_add_one($form, &$form_state) {
  $form_state['num_data']++;
  $form_state['rebuild'] = TRUE;
}

function greenhouse_addsensor_form_submit($form, &$form_state) {
    $json_array = array(
        'name'=> $form_state['values']['name'],
        'description'=> isset($form_state['values']['description'])?
                $form_state['values']['description']:'',
        'enable'=> $form_state['values']['enable'],
        'status'=> isset($form_state['values']['status'])?
            $form_state['values']['status']:'',
    );
    
    foreach ($form_state['values']['data'] as $value) {
        if (isset($value['title'])&&$value['title']) {
            $json_array['data'][] = array(
                'name' => $value['title'],
                'upper' => isset($value['upbound']) ?
                        $value['upbound'] : '',
                'lower' => isset($value['lobound']) ?
                       $value['lobound'] : ''
            );
        }
    }

    $json = json_encode($json_array);
    module_load_include('inc', 'greenhouse','rest_api/greenhouse_con');
    greenhouse_post_sensor($form_state['values']['id'], $json);
}

function greenhouse_delete_sensor($form,&$form_state,$id) {
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

function greenhouse_delete_sensor_submit($form,&$form_state) {
    module_load_include('inc', 'greenhouse','rest_api/greenhouse_con');
    greenhouse_rest_delete_sensor($form_state['values']['id']);
    global $base_url;
    drupal_goto($base_url .'/greenhouse/sensors/');
}

function greenhouse_update_sensor($form,&$form_state,$id) {
    module_load_include('inc', 'greenhouse','rest_api/greenhouse_con');
    $data = greenhouse_get_sensor_array();
    if(!isset($data[$id])) {
        return array();
    }
    
    $data = $data[$id];
    $form['id_show'] = array(
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
    $form['#tree'] = true;

    $form['data'] = array(
        '#type' => 'fieldset',
        '#title' => t('Data Set'),
        '#collapsible' => FALSE,
    );

    for ($i = 0; $i < sizeof($data['Data']); $i++) {
        $form['data'][$i] = array(
            '#prefix' => '<div>',
            '#suffix' => '</div>'
        );
        $form['data'][$i]['title_show'] = array(
            '#type' => 'item',
            '#title' => t('Data title'),
            '#markup' => $data['Data'][$i]['DataName'],
        );

        
        $form['data'][$i]['title'] = array(
            '#type' => 'value',
            
            
            '#value' => $data['Data'][$i]['DataName'],
        );
        $form['data'][$i]['upbound'] = array(
            '#type' => 'textfield',
            '#title' => t('Upper Bound'),
            '#default_value' => $data['Data'][$i]['Upper'],
            '#size' => 60,
            '#maxlength' => 128,
        );
        $form['data'][$i]['lobound'] = array(
            '#type' => 'textfield',
            '#title' => t('Lower Bound'),
            '#default_value' => $data['Data'][$i]['Lower'],
            '#size' => 60,
            '#maxlength' => 128,
        );
    }
    
    $form['add'] = array(
        '#type' => 'submit',
        '#value' => t('Add'),
    );
    return $form;
}

function greenhouse_update_sensor_submit($form, &$form_state) {
    $json_array = array(
        'name' => $form_state['values']['name'],
        'description' => isset($form_state['values']['description']) ?
                $form_state['values']['description'] : '',
        'enable' => $form_state['values']['enable'],
        'status' => isset($form_state['values']['status']) ?
                $form_state['values']['status'] : '',
    );

    foreach ($form_state['values']['data'] as $value) {
        if (isset($value['title']) && $value['title']) {
            $json_array['data'][] = array(
                'name' => $value['title'],
                'upper' => isset($value['upbound']) ?
                        $value['upbound'] : '',
                'lower' => isset($value['lobound']) ?
                        $value['lobound'] : ''
            );
        }
    }

    $json = json_encode($json_array);
    module_load_include('inc', 'greenhouse', 'rest_api/greenhouse_con');
    greenhouse_post_sensor($form_state['values']['id'], $json);
}
