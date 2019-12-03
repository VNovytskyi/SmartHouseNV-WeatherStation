<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>getJSONLastWeather</title>
</head>
<body>
    <?php
        $username = "root";
        $servername = "localhost";
        $password = "3105vlad3010vlada";
        $db = "weatherstation";
        
        $conn = new mysqli($servername, $username, $password, $db);
               
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        }
        
        $sql = "SELECT `idweatherCharacteristics`,`idaddress`,`DateTime`,`Temperature`,`Humidity`,`Pressure` 
                FROM `weatherdata` 
                WHERE `idweatherCharacteristics` = (SELECT MAX(`idweatherCharacteristics`) FROM `weatherdata`)";

        $result = $conn->query($sql);

        if ($result->num_rows > 0) {
            $row = $result->fetch_assoc();

            $arr = array(
                        'idweatherCharacteristics' => $row["idweatherCharacteristics"],
                        'idaddress' => $row['idaddress'],
                        'DateTime' => $row['DateTime'],
                        'Temperature' => $row["Temperature"],
                        'Humidity' => $row["Humidity"],
                        'Pressure' => $row["Pressure"],
                        );
            
            echo json_encode($arr);
           
        } else {
            echo "0 results";
        }
        $conn->close();
    ?>
</body>
</html>
