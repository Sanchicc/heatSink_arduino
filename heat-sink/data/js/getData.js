var flag = true; // 调试用的

if (flag === true) {
    getData();
    getModel();
    getTemperature_MAX();
    getfanStatus();
    getHumidity();

    setInterval(function () {
        getData();
        getModel();
        getTemperature_MAX();
        getfanStatus();
        getHumidity();
    }, 10000);
}

setInterval(
    function () {
        var rangeCurrentValue = document.getElementById('rangeCurrentValue');
        rangeCurrentValue.innerText = document.getElementById('setMaxT').value;
    }, 50);

function getData() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("Temperature").innerText = this.responseText;
        }
    };
    xhttp.open("GET", "temperature", true);
    xhttp.send();
}

function getModel() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            // 同步单选框
            var i;
            switch (this.responseText) {
                case 'open': i = 0; break;
                case 'close': i = 2; break;
                default: i = 1;
            }
            document.querySelectorAll('#radio_list P input')[i] = true;
        }
    };
    xhttp.open("GET", "Model", true);
    xhttp.send();
}

function getTemperature_MAX() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            // 设一下值
            document.getElementById('setMaxT').value = this.responseText;
        }
    };
    xhttp.open("GET", "Temperature_MAX", true);
    xhttp.send();
}

function getfanStatus() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("fanStatus").innerText = this.responseText;
        }
    };
    xhttp.open("GET", "fanStatus", true);
    xhttp.send();
}

function getHumidity() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("Humidity").innerText = this.responseText;
        }
    };
    xhttp.open("GET", "Humidity", true);
    xhttp.send();
}

function reset() {
    document.getElementById('setMaxT').value = '40';
    document.querySelectorAll('#radio_list P input')[1] = true;
}

// 提交
function postCurrentValue() {
    var setMaxT = document.getElementById('setMaxT').value;
    var radios = document.getElementsByName('status');
    var radiosValue;
    for (let i = 0; i < 3; i++)
        if (radios[i].checked === true) {
            radiosValue = radios[i].value;
        }

    console.log(radiosValue);
    console.log(setMaxT);

    var xhttp = new XMLHttpRequest();
    xhttp.open("POST", "fan", true);
    xhttp.send("max_t=${setMaxT}&status=${radiosValue}");
}