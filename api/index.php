<?php
/*
require slim framework
*/
require 'Slim/Slim.php';

\Slim\Slim::registerAutoloader();

/**
 *  Instantiate a Slim application
 */
$app = new \Slim\Slim();

/**
 *  Slim application routes
 *
 * `Slim::get`, `Slim::post`, `Slim::put`, `Slim::patch`, and `Slim::delete`
 *  use anonymous function.
 */

// create routes
$app->get('/', 'getSensor');
$app->get('/post', 'postSensor');
$app->get('/put', 'updateSensor');
$app->get('/delete', 'deleteSensor');
$app->get('/patch', 'patchSensor');


 // get function   
   function getSensor () {
        //$template = "goat";

        echo "I am a verified billy tarantula goat and I need a database to do rest api calls";
    }

// POST route
    function postSensor () {
        echo 'This is a POST route';
    }
	

// PUT route
    function updateSensor () {
        echo 'This is a PUT route';
    }


// PATCH route
 function patchSensor () {
    echo 'This is a PATCH route';
}

// DELETE route

    function deleteSensor() {
        echo 'This is a DELETE route';
    }


/** this should be called last 
 *  the HTTP response to the HTTP client.
 */
$app->run();
