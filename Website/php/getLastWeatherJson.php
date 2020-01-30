<?php
    header('Access-Control-Allow-Origin: *');

    $logFilePath = "D:\MyServer\data\php\mainLog.txt";
    $stringToFile = date("Y-m-d H:i:s [" . $_SERVER['REMOTE_ADDR'] . "] -> ");
    $stringToFile .= "Get | ";

    $servername = "localhost";
    $username = "root";
    $password = "";
    $db = "weatherstation";

    $conn = new mysqli($servername, $username, $password, $db);

    if ($conn->connect_error) 
    {
        $stringToFile .= "Connect error";
        file_put_contents($logFilePath, $stringToFile . "\n", FILE_APPEND);

        die("Connection failed: " . $conn->connect_error);
    }

    $stringToFile .= "Connection successful | ";

    $sql = "SELECT * FROM `weatherData` WHERE 1 ORDER BY `idWeatherData` DESC LIMIT 1";
    $result = $conn->query($sql);

    if ($result->num_rows > 0)
    {
        $stringToFile .= "Result num rows > 0";
        $row = $result->fetch_assoc();
        echo json_encode($row);
    }
    else
    {
        $stringToFile .= "Result num rows <= 0";
        file_put_contents($logFilePath, $stringToFile . "\n", FILE_APPEND);
    }

    $stringToFile .= " [ OK ]";
    file_put_contents($logFilePath, $stringToFile . "\n", FILE_APPEND);

    $conn->close();
?>
