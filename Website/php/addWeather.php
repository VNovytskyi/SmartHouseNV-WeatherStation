<?php
    // Example: http://localhost/addWeather.php?t=16&h=60&p=994&a=1
    //http://localhost/weatherStation/addWeather.php?t=16&h=60&p=994

    //Get values from request
    $temperature = $_GET['t'];
    $humidity = $_GET['h'];
    $pressure = $_GET['p'];

    //Connect to server
    $servername = "localhost";
    $username = "root";
    $password = "";
    $db = "weatherstation";

    $conn = new mysqli($servername, $username, $password, $db);

    if ($conn->connect_error) {
        die("Connection failed: " . $conn->connect_error);
    }

    //Insert data into DB
    $sql = "INSERT INTO `weatherdata` (`idWeatherData`, `DateTime`, `Temperature`, `Humidity`, `Pressure`) 
            VALUES (NULL, now(), '$temperature', '$humidity', '$pressure')";

    if ($conn->query($sql) === TRUE) {
        echo "New record created successfully";
    } else {
        echo "Error: " . $sql . "<br>" . $conn->error;
    }

    $conn->close();
?>