<?php
    //Get values from request
    $temperature = $_GET['t'];
    $humidity = $_GET['h'];
    $pressure = $_GET['p'];
    $addressID = $_GET['a'];

    //Connect to server
    $servername = "localhost";
    $username = "root";
    $password = "3105vlad3010vlada";
    $db = "weatherstation";

    $conn = new mysqli($servername, $username, $password, $db);

    if ($conn->connect_error) {
        die("Connection failed: " . $conn->connect_error);
    }

    //


?>