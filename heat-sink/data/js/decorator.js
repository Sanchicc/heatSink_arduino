var devorators = {
    s1,
    s2,
    s3,
    s4,
    s5
};

function decorator() {
    var decorator = document.getElementById('decorator');

        decorator.style.top = '-0%';

        decorator.s1 = setTimeout(function () {
            decorator.style.top = '-90%';
        }, 800);

        decorator.s2 = setTimeout(function () {
            decorator.style.top = '-35%';
        }, 1500);

        decorator.s3 = setTimeout(function () {
            decorator.style.top = '-74%';
        }, 2250);

        decorator.s4 =  setTimeout(function () {
            decorator.style.top = '-40%';
        }, 2850);

        decorator.s5 = setTimeout(function () {
            decorator.style.top = '-50%';
        }, 3600);
}

function decorator_down(){
    clearTimeout(decorator.s1);
    clearTimeout(decorator.s2);
    clearTimeout(decorator.s3);
    clearTimeout(decorator.s4);
    clearTimeout(decorator.s5);
    decorator();
}