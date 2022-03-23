var devorators = {
    interval,
    s0,
    s1,
    s2,
    s3,
    s4,
    s5,
    s6,
    s7,
    s8,
    s9,
    s10,
    s11,
    s12
};

function decorator() {
    var decorator = document.getElementById('decorator');
    
    var i = 0;
    decorator.s0 = setTimeout(function () {
        decorator.style.top = '-0%';
    }, i);


    decorator.s1 = setTimeout(function () {
        decorator.style.top = '-100%';
    }, i += 700);

    decorator.s2 = setTimeout(function () {
        decorator.style.top = '-10%';
    }, i += 700);

    decorator.s3 = setTimeout(function () {
        decorator.style.top = '-95%';
    }, i += 700);

    decorator.s4 = setTimeout(function () {
        decorator.style.top = '-20%';
    }, i += 700);

    decorator.s5 = setTimeout(function () {
        decorator.style.top = '-80%';
    }, i += 700);

    decorator.s6 = setTimeout(function () {
        decorator.style.top = '-35%';
    }, i += 700);

    decorator.s7 = setTimeout(function () {
        decorator.style.top = '-70%';
    }, i += 700);

    decorator.s8 = setTimeout(function () {
        decorator.style.top = '-40%';
    }, i += 700);

    decorator.s9 = setTimeout(function () {
        decorator.style.top = '-60%';
    }, i += 700);

    decorator.s10 = setTimeout(function () {
        decorator.style.top = '-48%';
    }, i += 700);

    decorator.s11 = setTimeout(function () {
        decorator.style.top = '-51%';
    }, i += 700);

    decorator.s12 = setTimeout(function () {
        decorator.style.top = '-50%';
    }, i += 700);
}

function decorator_down() {
    clearInterval(decorator.interval);
    clearTimeout(decorator.s0);
    clearTimeout(decorator.s1);
    clearTimeout(decorator.s2);
    clearTimeout(decorator.s3);
    clearTimeout(decorator.s4);
    clearTimeout(decorator.s5);
    clearTimeout(decorator.s6);
    clearTimeout(decorator.s7);
    clearTimeout(decorator.s8);
    clearTimeout(decorator.s9);
    clearTimeout(decorator.s10);
    clearTimeout(decorator.s11);
    clearTimeout(decorator.s12);
    decorator();
}

decorator.interval = setInterval(decorator, 10000)