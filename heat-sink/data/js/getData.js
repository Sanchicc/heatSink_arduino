var flag = false; // 调试用的

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
} else {
    setInterval(function () {
        getJson();
    }, 5000);
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

// 存放数据
var data = {
    temperature: '0',
    Model: '0',
    Temperature_MAX: '0',
    fanStatus: '0',
    Humidity: '0'
};


function getJson() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            data = JSON.parse(this.responseText);
            console.log(data);
            setData(data);
        }
    };
    xhttp.open("GET", "json", true);
    xhttp.send();
}

function setData(data) {
    document.getElementById("Temperature").innerText = data.temperature;
    var i;
    switch (data.Model) {
        case 'open': i = 0; break;
        case 'close': i = 2; break;
        default: i = 1;
    }
    document.querySelectorAll('#radio_list P input')[i] = true;
    document.getElementById('setMaxT').value = data.Temperature_MAX;
    document.getElementById("fanStatus").innerText = data.fanStatus;
    document.getElementById("Humidity").innerText = data.Humidity;
}