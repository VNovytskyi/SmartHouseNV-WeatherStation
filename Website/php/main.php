<?php
    //http://localhost/weatherStation/addWeather.php?t=16&h=60&p=994
    //type=addNewRecord
    
    // Y-m-d H:i:s -> requestType | connectionStatus | sql |

    $defaultValue = "null";
    $logFilePath = "D:\MyServer\data\php\mainLog.txt";
    $stringToFile = date("Y-m-d H:i:s [" . $_SERVER['REMOTE_ADDR'] . "] -> ");

    if(isset($_GET['type']))
    {
        $requestType = $_GET['type'];
        $stringToFile .= $requestType . " | ";
    }
    else
    {
        $stringToFile .= "Error! RequestType incorrect!";

        //TODO: fwrite заменить на file_put_contents
        file_put_contents($logFilePath, $stringToFile . "\n", FILE_APPEND);

        die($stringToFile); 
    }
    
    $conn = new mysqli("localhost", "root", "", "weatherstation");
    if ($conn->connect_error) 
    {
        $stringToFile .= "Connection failed: " . $conn->connect_error;

        file_put_contents($logFilePath, $stringToFile . "\n", FILE_APPEND);

        die($stringToFile); 
    }
    else
    {
        $stringToFile .= "Connection successful | ";
    }
    
    
    switch($requestType)
    {
        case "addNewRecord": 
            $humidity = isset($_GET['h']) ? $_GET['h'] : $defaultValue;
            $pressure = isset($_GET['p']) ? $_GET['p'] : $defaultValue;
            $temperature = isset($_GET['t']) ? $_GET['t'] : $defaultValue;
            $batteryVoltage = isset($_GET['v']) ? $_GET['v'] : $defaultValue;
    
            $sql = "INSERT INTO `weatherdata` (`idWeatherData`, `DateTime`, `Temperature`, `Humidity`, `Pressure`, `batteryVoltage`) VALUES (NULL, now(), '$temperature', '$humidity', '$pressure', '$batteryVoltage')";
            $stringToFile .= $sql . " | ";
    
            if ($conn->query($sql) === TRUE)
            {
                $stringToFile .= "Query successful";
            }
            else
            {
                $stringToFile .= "Query failed: " . $conn->connect_error;

                file_put_contents($logFilePath, $stringToFile . "\n", FILE_APPEND);
        
                die($stringToFile); 
            }
        break;

        default: 
            $stringToFile .= "Error! RequestType incorrect!";

            file_put_contents($logFilePath, $stringToFile . "\n", FILE_APPEND);
    
            die($stringToFile); 
        break;
    }

    $conn->close();

    file_put_contents($logFilePath, $stringToFile . "\n", FILE_APPEND);

    echo $stringToFile;
?>