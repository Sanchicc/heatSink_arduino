var succerseng = {
    interval: 0
};

// 获取数据
getJson();
function runInterval() {
    succerseng.interval = setInterval(function () {
        console.log('getJson()');
        getJson();
    }, 1000);
}

// 显示滑块的值
setInterval(
    function () {
        var rangeCurrentValue = document.getElementById('rangeCurrentValue');
        rangeCurrentValue.innerText = document.getElementById('setMaxT').value;
    }, 50);

function reset() {
    document.getElementById('setMaxT').value = '40';
    document.querySelectorAll('#radio_list P input')[1] = true;
    postCurrentValue();
}

// 提交
function postCurrentValue() {
    clearInterval(succerseng.interval); // 关掉 不然会有奇怪的事情
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
    var params = 'max_t=' + setMaxT + '&status=' + radiosValue;
    xhttp.open("POST", "submit", true);
    // 把字符串类型的参数序列化成Form Data
    xhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    xhttp.send(params);
    runInterval();
}

// 存放Json数据
var data = {
    temperature: '0',
    model: '0',
    max_t: '0',
    status: '0',
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
    switch (data.model) {
        case 'open': i = 0; break;
        case 'close': i = 2; break;
        default: i = 1;
    }

    document.querySelectorAll('#radio_list P input')[i] = true;
    document.getElementById('setMaxT').value = data.max_t;
    document.getElementById("status").innerText = data.status;
    document.getElementById("Humidity").innerText = data.Humidity;
}