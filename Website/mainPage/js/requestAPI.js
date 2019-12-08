var RequestFromAWS = new XMLHttpRequest();
var RequestFromSW = new XMLHttpRequest();
var RequestFromSW5days = new XMLHttpRequest();

var ObjRequestFromAWS, 
    ObjRequestFromSW, 
    ObjRequestFromSW5days;

RequestFromAWS.open('GET', 'http://192.168.1.102/getJSONLastWeather.php', true);
RequestFromSW.open("GET", "http://api.openweathermap.org/data/2.5/weather?id=703448&APPID=1e77b0d2964f81799833ea447f48491d", true);
RequestFromSW5days.open("GET", "http://api.openweathermap.org/data/2.5/forecast?id=703448&appid=1e77b0d2964f81799833ea447f48491d", true);

RequestFromAWS.send();
RequestFromSW.send();
RequestFromSW5days.send();



//Answer from autonomous weather station
RequestFromAWS.onreadystatechange = function() {
    if (this.readyState != 4) return;

    if (RequestFromAWS.status != 200) 
    {
        console.log("[ ERROR ] Answer from autonomous weather station");
        console.log(this.status + ': ' + this.statusText)
    } 
    else 
    {
        console.log("[ OK ] Answer from autonomous weather station");
        ObjRequestFromAWS = JSON.parse(this.responseText);

        document.getElementById("AWS_LastUpdate").innerHTML = "last update: " + ObjRequestFromAWS["DateTime"];
        document.getElementById("AWS_LastTemperature").innerHTML += ObjRequestFromAWS["Temperature"];
        document.getElementById("AWS_LastHumidity").innerHTML += ObjRequestFromAWS["Humidity"];
        document.getElementById("AWS_LastPressure").innerHTML += ObjRequestFromAWS["Pressure"];
    }
}

//Answer from SimpleWeather (current weather)
RequestFromSW.onreadystatechange = function() {
    if (this.readyState != 4) return;

    if (this.status != 200) 
    {
        console.log("[ ERROR ] Answer from SimpleWeather (current weather)");
        console.log(this.status + ': ' + this.statusText)
    }
    else
    {
        console.log("[ OK ] Answer from SimpleWeather (current weather)");
        ObjRequestFromSW = JSON.parse(this.responseText);

        document.getElementById("mainIcon").src = "http://openweathermap.org/img/wn/" + ObjRequestFromSW.weather[0].icon + "@2x.png";
        document.getElementById("mainTemperature").innerHTML += Math.round(ObjRequestFromSW.main.temp - 273) + "&deg";
        document.getElementById("mainPressure").innerHTML += Math.round(ObjRequestFromSW.main.pressure / 133 * 100);
        document.getElementById("mainHumidity").innerHTML += Math.round(ObjRequestFromSW.main.humidity);
        document.getElementById("webIcon").href = "http://openweathermap.org/img/wn/" + ObjRequestFromSW.weather[0].icon + "@2x.png";
        document.getElementById("mainDescription").innerHTML += ObjRequestFromSW.weather[0].main + ", " + ObjRequestFromSW.weather[0].description;
        
        //$("#mainIcon").attr("src","http://openweathermap.org/img/wn/" + ObjRequestFromSW.weather[0].icon + "@2x.png");
        //$("#mainTemperature").html(Math.round(ObjRequestFromSW.main.temp - 273) + "&deg");
        //$("#mainPressure").append(Math.round(ObjRequestFromSW.main.pressure / 133 * 100));
        //$("#mainHumidity").append(Math.round(ObjRequestFromSW.main.humidity));
        //$("#webIcon").attr("src","http://openweathermap.org/img/wn/" + ObjRequestFromSW.weather[0].icon + "@2x.png");
        //$("#mainDescription").html(ObjRequestFromSW.weather[0].main + ", " + ObjRequestFromSW.weather[0].description);
    }
};

//Answer from SimpleWeather (forecast for next 5 days)
RequestFromSW5days.onreadystatechange = function() {
    if (this.readyState != 4) return;

    if (this.status != 200) 
    {
        console.log("[ ERROR ] Answer from SimpleWeather (forecast for next 5 days)");
        console.log(this.status + ': ' + this.statusText)
    }
    else
    {
        console.log("[ OK ] Answer from SimpleWeather (forecast for next 5 days)");
        ObjRequestFromSW5days = JSON.parse(this.responseText);


    }
};