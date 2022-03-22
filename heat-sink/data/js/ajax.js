getData();
getModel();
getTemperature_MAX();
getfanStatus();
getHumidity();

setInterval(function () {
    // Call a function repetatively with 1 Second interval
    getData();
    getModel();
    getTemperature_MAX();
    getfanStatus();
    getHumidity();
}, 1000); //1000mSeconds update rate

function getData() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("Temperature").innerHTML = this.responseText;
        }
    };
    xhttp.open("GET", "temperature", true);
    xhttp.send();
}

function getModel() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("Model").innerHTML = this.responseText;
        }
    };
    xhttp.open("GET", "Model", true);
    xhttp.send();
}

function getTemperature_MAX() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("Temperature_MAX").innerHTML = this.responseText;
        }
    };
    xhttp.open("GET", "Temperature_MAX", true);
    xhttp.send();
}

function getfanStatus() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("fanStatus").innerHTML = this.responseText;
        }
    };
    xhttp.open("GET", "fanStatus", true);
    xhttp.send();
}

function getHumidity() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("Humidity").innerHTML = this.responseText;
        }
    };
    xhttp.open("GET", "Humidity", true);
    xhttp.send();
}
