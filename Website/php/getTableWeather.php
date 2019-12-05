<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>getTableWeather</title>
    <style>
        table {
            font-family: "Lucida Sans Unicode", "Lucida Grande", Sans-Serif;
            font-size: 14px;
            border-collapse: collapse;
            text-align: center;
        }

        th, td:first-child {
            background: #AFCDE7;
            color: white;
            padding: 10px 20px;
        }

        th, td {
            border-style: solid;
            border-width: 0 1px 1px 0;
            border-color: white;
        }
        
        td {
            background: #D8E6F3;
            padding-left: 10px;
            padding-right: 10px;
        }
       
        th:first-child, td:first-child {
            text-align: left;
        }
    </style>
</head>
<body>
    <table>
        <tr>
            <td>id</td>
            <td>DateTime</td>
            <td>Temperature</td>
            <td>Humidity</td>
            <td>Pressure</td>
            <td>boolRain</td>
            <td>probabilityRain</td>
            <td>lightAmount</td>
            <td>windDirection</td>
            <td>windStrength</td>
        </tr>

        <?php
            $servername = "localhost";
            $username = "root";
            $password = "3105vlad3010vlada";
            $db = "weatherstation";

            $conn = new mysqli($servername, $username, $password, $db);

            if ($conn->connect_error) {
                die("Connection failed: " . $conn->connect_error);
            }
            
            //echo "Connected successfully";

            $sql = "SELECT *     FROM `weatherData` WHERE 1";
            $result = $conn->query($sql);

            if ($result->num_rows > 0) {
                while($row = $result->fetch_assoc()) {
                    echo "<tr>";
                    echo    "<td>" . $row["idweatherCharacteristics"] . "</td>";
                    echo    "<td>" . $row["DateTime"] . "</td>";
                    echo    "<td>" . $row["Temperature"] . " &degC</td>";
                    echo    "<td>" . $row["Humidity"] . "   %</td>";
                    echo    "<td>" . $row["Pressure"] . "</td>";
                    echo    "<td>" . $row["boolRain"] . "</td>";
                    echo    "<td>" . $row["probabilityRain"] . "</td>";
                    echo    "<td>" . $row["lightAmount"] . "</td>";
                    echo    "<td>" . $row["windDirection"] . "</td>";
                    echo    "<td>" . $row["windStrength"] . "</td>";
                    echo "</tr>";
                }
            } else {
                echo "0 results";
            }
            $conn->close();
        ?>
    </table>
</body>
</html>
