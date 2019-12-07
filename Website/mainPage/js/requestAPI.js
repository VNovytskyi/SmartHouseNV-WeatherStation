var RequestFromAWS = new XMLHttpRequest();
var RequestFromOW = new XMLHttpRequest();

RequestFromAWS.open('GET', 'http://192.168.1.102/getJSONLastWeather.php', true);
RequestFromAWS.send();

RequestFromOW.open("GET", "http://api.openweathermap.org/data/2.5/weather?id=703448&APPID=1e77b0d2964f81799833ea447f48491d", true);
RequestFromOW.send();

//Ответ от автономной метеостанции
RequestFromAWS.onreadystatechange = function() {
    if (this.readyState != 4) return;

    if (RequestFromAWS.status != 200) 
    {
        alert(this.status + ': ' + this.statusText);
    } 
    else 
    {
        var AWS = JSON.parse(this.responseText);

        document.getElementById("AWS_LastUpdate").innerHTML = "last update: " + AWS["DateTime"];
        document.getElementById("AWS_LastTemperature").innerHTML += AWS["Temperature"];
        document.getElementById("AWS_LastHumidity").innerHTML += AWS["Humidity"];
        document.getElementById("AWS_LastPressure").innerHTML += AWS["Pressure"];
    }
}

//Ответ от SimpleWeather
RequestFromOW.onreadystatechange = function() {
    if (this.readyState != 4) return;

    if (this.status != 200) 
    {
        alert(this.status + ': ' + this.statusText);
    }
    else
    {
        var obj = JSON.parse(this.responseText);

        document.getElementById("mainTemperature").innerHTML += Math.round(obj.main.temp - 273) + "&deg";
        document.getElementById("mainIcon").src = "http://openweathermap.org/img/wn/" + obj.weather[0].icon + "@2x.png";
        document.getElementById("mainPressure").innerHTML += Math.round(obj.main.pressure / 133 * 100);
        document.getElementById("mainHumidity").innerHTML += Math.round(obj.main.humidity);
    }
};      