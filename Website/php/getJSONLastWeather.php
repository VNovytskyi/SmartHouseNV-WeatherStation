<?php
    header('Access-Control-Allow-Origin: *');
        
    $username = "root";
    $servername = "localhost";
    $password = "";
    $db = "weatherstation";
        
    $conn = new mysqli($servername, $username, $password, $db);
               
    if ($conn->connect_error) {
        die("Connection failed: " . $conn->connect_error);
    }
        
    $sql = "SELECT `idWeatherData`,`dateTime`,`temperature`,`humidity`,`pressure` 
            FROM `weatherdata` 
            WHERE `idWeatherData` = (SELECT MAX(`idWeatherData`) FROM `weatherdata`)";

    $result = $conn->query($sql);

    if ($result->num_rows > 0) {
        $row = $result->fetch_assoc();

    $arr = array(
                    'idWeatherData' => $row["idWeatherData"],
                    'dateTime' => $row['dateTime'],
                    'temperature' => $row["temperature"],
                    'humidity' => $row["humidity"],
                    'pressure' => $row["pressure"],
                    );
            
        echo json_encode($arr);

    } else {
        echo "0 results";
    }
    
    $conn->close();
?>
