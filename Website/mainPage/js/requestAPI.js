/*
    Current weather from Rapid API
*/
var nullData = null;
var obj;
var currentWeatherFromRapid = new XMLHttpRequest();

currentWeatherFromRapid.onreadystatechange = function() 
{
    if (this.readyState != 4) return;

    if (this.status == 200) 
    {
        obj = JSON.parse(this.responseText);
        $("#mainIcon").attr("src","C:/Users/vladyslavN/Documents/Projects/KievWeather/Website/mainPage/icons/" + obj.data[0].weather.icon + ".png");
        $("#webIcon").attr("src","C:/Users/vladyslavN/Documents/Projects/KievWeather/Website/mainPage/icons/" + obj.data[0].weather.icon + ".png");
        $("#mainTemperature").html(Math.round(obj.data[0].temp) + "&deg");
        $("#mainHumidity").append(Math.round(obj.data[0].rh));
        $("#mainPressure").append(Math.round(obj.data[0].pres / 133 * 100));
        $("#mainDescription").html(obj.data[0].weather.description);
    }
};

currentWeatherFromRapid.open("GET", "https://weatherbit-v1-mashape.p.rapidapi.com/current?lang=en&lon=30.52&lat=50.43");
currentWeatherFromRapid.setRequestHeader("x-rapidapi-host", "weatherbit-v1-mashape.p.rapidapi.com");
currentWeatherFromRapid.setRequestHeader("x-rapidapi-key", "491db123camshc535f3524f34e89p133072jsn5dd65ef95462");
currentWeatherFromRapid.send(nullData);




/************************************************************************************************************************************

*/


var RequestFromSW5days = new XMLHttpRequest();



var date, outDate, outTime;


RequestFromSW5days.open("GET", "http://api.openweathermap.org/data/2.5/forecast?id=703448&appid=1e77b0d2964f81799833ea447f48491d", true);


RequestFromSW5days.send();

//Answer from SimpleWeather (current weather)


//Answer from SimpleWeather (forecast for next 5 days)
RequestFromSW5days.onreadystatechange = function() {
    if (this.readyState != 4) return;

    if (this.status == 200) 
    {
        ObjRequestFromSW5days = JSON.parse(this.responseText);

        var dt = (ObjRequestFromSW5days.list[0].dt) * 1000;
        
        date = new Date(dt);
        
        
        outDate = date.getUTCFullYear() + "." + date.getMonth() + "." + date.getDay();
        outTime = date.getUTCHours() + ":00";
        

        //ObjRequestFromSW5days.list[0].main.temp
        //ObjRequestFromSW5days.list[0].dt
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