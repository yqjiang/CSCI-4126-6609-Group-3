<?php

function greenhouse_infomation() {
    // show general infomation
    module_load_include('inc', 'greenhouse', 'rest_api/greenhouse_con');
    $end = strtotime("now");
    $start = strtotime("-1 day", $end);
    $sensors = greenhouse_get_sensor_array();

    // prepair data
    $current_data = array();
    $day_data = array();
    $sensor_array = array();
    foreach ($sensors as $sensor_id => $data) {
        //get current data
        $current_data[$data['Name']] = array();
        $data_get = greenhouse_day_info($sensor_id, $start, $end);
        $day_data[$data['Name']] = $data_get;
        $sensor_array[$data['Name']] = $sensor_id;
        foreach ($data_get as $data_name => $day) {
            ksort($day);
            if (!empty($day)) {
                $current_data[$data['Name']][$data_name] = end($day);
            } else {
                $current_data[$data['Name']][$data_name] = t("No Data");
            }
        }
    }


    //display Data
    $task_url = variable_get('greenhouse_smart_scheduler');
    $display = "<div class=\"current_info highlighted-block light\">";
    $display .= "<h3>Current Info</h2>";

    foreach ($current_data as $sensor_title => $sensor_data) {
        $display .= "<div class=\"col-md-6\">";
        $display .= "<h3>" . $sensor_title . "</h4>";

        foreach ($sensor_data as $data_title => $data_data) {
            $display .= "<p><b>" . $data_title . ": </b>" . $data_data . "</p>";
        }
        $display .= "</div>";
    }
    $display .= "<div class=\"col-md-6\"><p><a href=\""
            . $task_url
            . "\" class=\"btn\" >See the tasks</a></p></div>";

    $display .= "</div><p>&nbsp</p>";


    //Display chart;
    global $base_url;

    $display .= "<div class=\"day_info\">";
    foreach ($day_data as $sensor_name => $sensor_data) {
        $display .= "<div class=\"current_info_sensor highlighted-block light\">";
        $display .= "<div class=\"row\"><div class=\"col-md-9\"><h4>" . $sensor_name . "</h4></div>";
        $display .= "<div class=\"col-md-3\"><p><a class=\"btn\" href=\""
                . $base_url
                . "/greenhouse/information/sensors/"
                . $sensor_array[$sensor_name]
                . "\" >view more</a></p></div></div>";

        foreach ($sensor_data as $key => $value) {
            $values = array();
            foreach ($value as $time => $dat) {
                $time = date('c', $time);
                $values[$time] = $dat;
            }
            $display .="<div class=\"col-md-12\"><h5>" . $key . "</h5>";
            $display .= greenhouse_get_single_line_graph($key, $values);
            $display .= "</div>";
        }
        $display .= "</div>";
        $display .= "<p>&nbsp</p>";
    }

    $display .= "</div>";
    return $display;
}

function greenhouse_get_single_line_graph($name, $data) {
    if (empty($data)) {
        return NULL;
    }

    $rows = array();
    foreach ($data as $time => $value) {
        $rows[] = array($time, $value);
    }

    $id = str_replace(' ', '_', $name);
    $chart = array(
        'id' => 'visualization_' . $id,
        'type' => 'LineGraph',
        'legend' => array($name),
        'rows' => $rows,
    );
    $display = d3_draw($chart);
    return $display;
}

function greenhouse_infomation_sensor_date_form($form, &$form_state, $id) {
    $form['start'] = array(
        '#title' => t('Start date'),
        '#type' => 'date_popup',
        '#date_format' => 'Y-m-d',
        '#description' => '',
    );
    $form['end'] = array(
        '#title' => t('End date'),
        '#type' => 'date_popup',
        '#date_format' => 'Y-m-d',
        '#description' => '',
    );
    $form['next'] = array(
        '#type' => 'submit',
        '#value' => t('Next'),
    );
    $form['id'] = array(
        '#type' => 'hidden',
        '#value' => $id
    );
    return $form;
}

function greenhouse_infomation_sensor_date_form_submit($form, &$form_state) {
    $start = strtotime($form_state['values']['start'] . " 0:0:0");
    $end = strtotime($form_state['values']['end'] . " 11:59:59");
    $id = $form_state['values']['id'];
    drupal_goto("greenhouse/information/sensors/data/$id/$start/$end");
}

function greenhouse_infomation_sensor_viewmore($id, $start, $end) {
    $display = "";
    // prepair data
    module_load_include('inc', 'greenhouse', 'rest_api/greenhouse_con');
    $datas = greenhouse_day_info($id, $start, $end);

    $display .= '<div class="highlighted-block light">'
            . '<h3>'
            . 'Line graph'
            . '</h3>';
    // display line graphs
    foreach ($datas as $name => $data) {
        $display .= '<h4>'
                . $name
                . '</h4>'
                . '<div class="graph">';
        // covert time
        $data_time = array();
        foreach ($data as $time => $data_item) {
            $time_cov = date('c', $time);
            $data_time[$time_cov] = $data_item;
        }
        $graph = greenhouse_get_single_line_graph($name, $data_time);
        $display .= $graph;
        $display.='</div>';
    }
    $display .= '</div><p></p><div class="datatable highlighted-block light"><table><tr>';
    $display.= '<th>'
            . 'time'
            . '</th>';
    foreach ($datas as $name => $data) {
        $display.= '<th>'
                . $name
                . '</th>';
    }

    $time_array = array();
    foreach ($datas as $name => $data) {
        $times = array_keys($data);
        foreach ($times as $time) {
            if (!in_array($times, $time_array)) {
                $time_array = $times;
            }
        }
    }
    sort($time_array);
    $display .= '</tr>';
    foreach ($time_array as $time_item) {
        $display .= '<tr>';
        $display .= '<td>'
                . date('c', $time_item)
                . '</td>';
        foreach ($datas as $name => $data) {
            if(isset($data[$time_item])) {
                $display .= '<td>'
                        . $data[$time_item]
                        . '</td>';
            } else {
                $display .= '<td>'
                        . 'null'
                        . '</td>';
            }
        }
        $display .= '</tr>';
    }
    $display .= '</table></div>';

    return $display;
}
