var decorators = {
    s0:0,
    s1:0,
    s2:0,
    s3:0,
    s4:0,
    s5:0,
    s6:0,
    s7:0,
    s8:0,
    s9:0,
    s10:0,
    s11:0,
    s12:0,
    i : 0
};

function decorator() {
    console.log('进入动画');
    var decorator = document.getElementById('decorator');

    decorators.i = 0;
    decorators.s0 = setTimeout(function () {
        decorator.style.top = '-0%';
    }, decorators.i);

    decorators.s1 = setTimeout(function () {
        decorator.style.top = '-100%';
    }, decorators.i += 700);

    decorators.s2 = setTimeout(function () {
        decorator.style.top = '-10%';
    }, decorators.i += 700);

    decorators.s3 = setTimeout(function () {
        decorator.style.top = '-95%';
    }, decorators.i += 700);

    decorators.s4 = setTimeout(function () {
        decorator.style.top = '-20%';
    }, decorators.i += 700);

    decorators.s5 = setTimeout(function () {
        decorator.style.top = '-80%';
    }, decorators.i += 700);

    decorators.s6 = setTimeout(function () {
        decorator.style.top = '-35%';
    }, decorators.i += 700);

    decorators.s7 = setTimeout(function () {
        decorator.style.top = '-70%';
    }, decorators.i += 700);

    decorators.s8 = setTimeout(function () {
        decorator.style.top = '-40%';
    }, decorators.i += 700);

    decorators.s9 = setTimeout(function () {
        decorator.style.top = '-60%';
    }, decorators.i += 700);

    decorators.s10 = setTimeout(function () {
        decorator.style.top = '-48%';
    }, decorators.i += 700);

    decorators.s11 = setTimeout(function () {
        decorator.style.top = '-51%';
    }, decorators.i += 700);

    decorators.s12 = setTimeout(function () {
        decorator.style.top = '-50%';
    }, decorators.i += 700);
}

function decorator_down() {
    console.log('启动动画');
        clearTimeout(decorators.s0);
        clearTimeout(decorators.s1);
        clearTimeout(decorators.s2);
        clearTimeout(decorators.s3);
        clearTimeout(decorators.s4);
        clearTimeout(decorators.s5);
        clearTimeout(decorators.s6);
        clearTimeout(decorators.s7);
        clearTimeout(decorators.s8);
        clearTimeout(decorators.s9);
        clearTimeout(decorators.s10);
        clearTimeout(decorators.s11);
        clearTimeout(decorators.s12);
        decorator();
}