var RequestFromSW = new XMLHttpRequest();
var RequestFromSW5days = new XMLHttpRequest();

//For debug in console
var ObjRequestFromSW = null, 
    ObjRequestFromSW5days = null;

var date;

RequestFromSW.open("GET", "http://api.openweathermap.org/data/2.5/weather?id=703448&APPID=1e77b0d2964f81799833ea447f48491d", true);
RequestFromSW5days.open("GET", "http://api.openweathermap.org/data/2.5/forecast?id=703448&appid=1e77b0d2964f81799833ea447f48491d", true);

RequestFromSW.send();
RequestFromSW5days.send();

//Answer from SimpleWeather (current weather)
RequestFromSW.onreadystatechange = function() {
    if (this.readyState != 4) return;

    if (this.status == 200) 
    {
        ObjRequestFromSW = JSON.parse(this.responseText);

        $("#mainIcon").attr("src","http://openweathermap.org/img/wn/" + ObjRequestFromSW.weather[0].icon + "@2x.png");
        $("#mainTemperature").html(Math.round(ObjRequestFromSW.main.temp - 273) + "&deg");
        $("#mainPressure").append(Math.round(ObjRequestFromSW.main.pressure / 133 * 100));
        $("#mainHumidity").append(Math.round(ObjRequestFromSW.main.humidity));
        $("#webIcon").attr("src","http://openweathermap.org/img/wn/" + ObjRequestFromSW.weather[0].icon + "@2x.png");
        $("#mainDescription").html(ObjRequestFromSW.weather[0].main + ", " + ObjRequestFromSW.weather[0].description);
    }
};

//Answer from SimpleWeather (forecast for next 5 days)
RequestFromSW5days.onreadystatechange = function() {
    if (this.readyState != 4) return;

    if (this.status == 200) 
    {
        ObjRequestFromSW5days = JSON.parse(this.responseText);

        var dt = ObjRequestFromSW5days.list[0].dt * 1000;
        
        date = new Date(dt);
        

        //ObjRequestFromSW5days.list[0].main.temp
        //ObjRequestFromSW5days.list[0].dt

        // echo date("Y-m-d H:i:s", 1575925200);
        

    }
};

setInterval(function() {
    var adr = 'http://192.168.1.102/weatherStation/getLastWeatherJson.php';

    var xhr = new XMLHttpRequest(); 
    xhr.open('GET', adr, false);
    xhr.send();

    if (xhr.status != 200) 
    {
        alert( xhr.status + ': ' + xhr.statusText ); // пример вывода: 404: Not Found
    } 
    else 
    {
        var obj = JSON.parse(xhr.responseText);
        document.getElementById("AWS_LastUpdate").innerHTML = obj.dateTime;
        document.getElementById("AWS_LastTemperature").innerHTML = '<i class="fas fa-thermometer-half"> ' + obj.temperature;
        document.getElementById("AWS_LastHumidity").innerHTML = '<i class="fas fa-tint"></i>' + obj.humidity;
        document.getElementById("AWS_LastPressure").innerHTML = '<i class="fab fa-product-hunt"></i>' + obj.pressure;
        //document.getElementById("batteryVoltage").innerHTML = obj.batteryVoltage;   
    }
}, 1000);