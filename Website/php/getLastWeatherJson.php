<?php
    $servername = "localhost";
    $username = "root";
    $password = "";
    $db = "weatherstation";

    $conn = new mysqli($servername, $username, $password, $db);

    if ($conn->connect_error) {
        die("Connection failed: " . $conn->connect_error);
    }

    $sql = "SELECT * FROM `weatherData` WHERE 1 ORDER BY `idWeatherData` DESC LIMIT 1";
    $result = $conn->query($sql);

    if ($result->num_rows > 0)
    {
        $row = $result->fetch_assoc();
        echo json_encode($row);
    }


    $conn->close();
?>
