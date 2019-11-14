<?php
    // Example: http://localhost/addWeather.php?t=16&h=60&p=994&a=1

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

    //Insert data into DB
    $sql = "INSERT INTO `weathercharacteristics` (`idweatherCharacteristics`, `idaddress`, `DateTime`, `Temperature`, `Humidity`, `Pressure`) 
            VALUES (NULL, '$addressID', now(), '$temperature', '$humidity', '$pressure')";

    if ($conn->query($sql) === TRUE) {
        echo "New record created successfully";
    } else {
        echo "Error: " . $sql . "<br>" . $conn->error;
    }

    $conn->close();
?>